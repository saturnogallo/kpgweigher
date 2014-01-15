using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
using System.IO;
using System.IO.Ports;
using System.Text.RegularExpressions;
using System.Runtime.InteropServices;
using System.Threading;
using System.Diagnostics;
using System.Windows.Forms;
namespace Mndz
{
    static class  DeviceMgr
    {
        static public AutoResetEvent WaitEvent; //flag for wait response, when flag is on,node agent can process the command else node agent cannot do that
        static public AutoResetEvent OverEvent; //flag for process done event.
        static public SerialPort devport;  //relay board and DA will shared port

        static public double reading;
        static public void Dummy()
        {

        }
        static DeviceMgr()
        {
            
            Logger.SysLog("");
            Logger.SysLog(DateTime.Now.ToLongTimeString());
            //Thread.Sleep(3000);


            WaitEvent = new AutoResetEvent(false);
            OverEvent = new AutoResetEvent(false);

            if(GlobalConfig.ISDEBUG)
                return;

            #region init device port
            devport = new SerialPort();
            
            devport.BaudRate = 9600;

            devport.Parity = Parity.None;
            devport.DataBits = 8;
            devport.StopBits = StopBits.One;
            devport.NewLine = "\r";
            devport.ReadTimeout = 1300;

            foreach (string port in SerialPort.GetPortNames())
            {
                try
                {
                    devport.PortName = port;
                    devport.Open();
                    if (devport.IsOpen)
                    {
                        double dt;
                        if (CollectCurrent(out dt))
                            return;
                        else
                            devport.Close();
                    }
                }
                catch (Exception ex)
                {
                    Logger.SysLog(ex.Message + ";" + ex.StackTrace);
                }
            }
            MessageBox.Show("未找到300A 精密直流大功率恒流源装置，请检查串口线和电源");
            Process.GetCurrentProcess().Kill();
            #endregion
        }

        
        

        static public void SetCurrent(Decimal currval, bool bOn)
        {
            //PC side command
            //curr: 1.234 on
            //curr: 1.345 off
            //setting? return setting: 1.234 on|off
            //curr? return curr: 1.234
            string onoff = bOn ? "on" : "off";
            devport.DiscardInBuffer();
                string line = String.Format("curr: {0} {1}", currval.ToString(), onoff);
                devport.WriteLine(line);
        }
        static public void SetZero()
        {
            devport.WriteLine("ZERO");
            System.Threading.Thread.Sleep(1000);
        }
        static public bool CollectCurrent(out double reading)
        {
            devport.DiscardInBuffer();
            devport.WriteLine("curr?");
            reading = 0;
            try
            {
                string abc = devport.ReadLine();
                if (abc.Length <= 0)
                    return false;

                Match m = Regex.Match(abc, "curr: ([-0-9.]+)");
                if (m.Success)
                {
                    double dat;
                    bool ret = Double.TryParse(m.Groups[1].Value.ToString(), out dat);
                    if (ret)
                    {
                        reading = dat;
                        return true;
                    }
                }
            }
            catch (Exception ex)
            {
                Logger.SysLog(ex.Message + ";" + ex.StackTrace);
            }
            return false;
        }
        static public void Reset()
        {
            devport.WriteLine("H");
            Thread.Sleep(3000);
        }


        static private Dictionary<string, byte> regmap;


    }
    internal class Processor
    {
 
        private Decimal _setting;
        internal Decimal setting
        {
            get
            {
                return _setting;
            }
            set
            {
                if (value < 0)
                {
                    return;
                }

                if (!Form1.IsValidCurrent(value) )
                    return;
                _setting = value;
                DeviceMgr.SetCurrent(_setting, bOn);
                Util.ConstIni.WriteString("LASTSETTING", "setting", _setting.ToString());
            }
        }
        private bool _bOn;  //current on or off state
        internal bool bOn
        {
            get
            {
                return _bOn;
            }
            set
            {
                if (value == false) 
                {
                    _bOn = false;
                    DeviceMgr.SetCurrent(0, false);
                    return;
                }
                DeviceMgr.SetCurrent(_setting, true);                
                _bOn = true;
            }
        }
    
        internal Processor()
        {
            _setting = Decimal.Parse(Util.ConstIni.StringValue("LASTSETTING", "setting"));
            
            bOn = false;
        }
        private decimal lastsetting = -1;
        public void RefreshOutput()
        {
            UpdateCurrentOnly();
            if (bOn)
            {
                if (lastsetting != _setting)
                {
                    DeviceMgr.SetCurrent(_setting, true);
                    lastsetting = _setting;
                }
            }
            else
            {
                lastsetting = -1;
            }
        }
        public void ZeroON()
        {
            DeviceMgr.SetZero();
        }
                    
        internal double Current = -9999;
        private bool UpdateCurrentOnly()
        {
            double va;
            if (!DeviceMgr.CollectCurrent(out va))
                return false;
            Current = va;
            return true;
        }
    }
}
