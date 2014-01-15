using System;
using System.Collections.Generic;
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
        static public SerialPort devport;  //port to control Mndz

        static public double reading;
        static private StringBuilder inbuffer;
        static private StringBuilder cmdbuffer;

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
            inbuffer = new StringBuilder();
            cmdbuffer = new StringBuilder();

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
                catch(Exception ex)
                {
                    Logger.SysLog(ex.Message + ";" + ex.StackTrace);
                }
            }
            MessageBox.Show("未找到智能数字微电阻标准装置，请检查串口线和电源");
            Process.GetCurrentProcess().Kill();
            #endregion

        }

        static void  devport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (!devport.IsOpen)
                return;
            while (devport.BytesToRead > 0)
            {
                char data = Convert.ToChar(devport.ReadChar());
                if ((data == '\r' || data == '\n') && (cmdbuffer.Length > 0))
                {
                    string cmd = cmdbuffer.ToString();
                    Program.mainwnd.Invoke(new Action<string>(Program.mainwnd.dev_cmd), new object[] { cmd });
                    cmdbuffer.Remove(0, cmdbuffer.Length);
                }
                else
                {
                    if (data != '\r' && data != '\n')
                        cmdbuffer.Append(data);
                }
            }
        }
        
        static public void SetResi(int iRange, int iReal, Decimal resival, bool bOn)
        {
            //H: reset
            //resi: 1.234 on
            //resi: 1.345 off
            //resi? return resi: 1.234 on|off
            //curr? return curr: 1.234
            string onoff = bOn?"on":"off";
            devport.DiscardInBuffer();
            if (iRange < 0) //real resistance
            {
                if (iReal >= 0)
                {
                    string line = String.Format("resi: {0} {1} {2}", Processor.real_title[iReal], onoff, iRange.ToString());
                    devport.WriteLine(line);
                }
            }
            else
            {
                string line = String.Format("resi: {0} {1} {2}", resival.ToString(), onoff, iRange.ToString());
                devport.WriteLine(line);
            }
        }
        static public void SetZero()
        {
            devport.WriteLine("ZERO");
            System.Threading.Thread.Sleep(1000);
        }
        static public bool CollectCurrent(out double reading)
        {
            devport.DiscardInBuffer();
            devport.WriteLine("current?");
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
                    bool ret =  Double.TryParse(m.Groups[1].Value.ToString(), out dat);
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
     }
    internal class Processor
    {
        private int _iRange = -1;        //the range setting, -1 for real resistance, 0 for 20mohm, 1 for 200mohm, 2 for 2ohm, 3 for 20ohm
        internal int iRange{
            get
            {
                return _iRange;
            }
            set
            {
                if (_iRange == value)
                    return;
                _bOn = false;
                _iReal = -1;
                if (value == -1) //real resistance case
                {
                    _iRange = -1;
                    resistance = -1;
                    
                }
                else //input resistance case
                {
                    Decimal newresi = Convert.ToDecimal(0); 
                    double b = Math.Abs(Convert.ToDouble(resistance));
                    if (value == 0) //20m ohm case
                    {
                        if(b <= 0.02)
                            newresi = resistance;
                    }
                    else if (value == 1) //200m ohm case
                    {
                        if(b <= 0.2)
                            newresi = resistance;
                    }
                    else if (value == 2)//2 ohm case
                    {
                        if(b <= 2)
                            newresi = resistance;
                    }
                    else if (value == 3)//20 ohm case
                    {
                        if(b <= 20)
                            newresi = resistance;
                    }
                    else
                        return;
                    _iRange = value;
                    resistance = newresi;
                }
                Util.ConstIni.WriteString("LASTSETTING", "range", _iRange.ToString());
            }
        }
        private Decimal _resistance;
        internal Decimal resistance
        {
            get
            {
                if (iRange < 0) //real resistance case
                {
                    if (iReal < 0) //uncertain case
                        return Convert.ToDecimal(-9999);
                    else
                        return Convert.ToDecimal(real_value[iReal]);
                }
                return _resistance;
            }
            set
            {
                _resistance = value;
                DeviceMgr.SetResi(_iRange, _iReal, _resistance, _bOn);
                Util.ConstIni.WriteString("LASTSETTING", "resistance", _resistance.ToString());
            }
        }
        private int _iReal = -1;         //real resistance index
        internal int iReal
        {
            get
            {
                return _iReal;
            }
            set
            {
                _iReal = value;
                Util.ConstIni.WriteString("LASTSETTING", "real", _iReal.ToString());
                if (iRange == -1) //real resistance case
                {
                    if ((_iReal < 0) || (_iReal >= real_title.Length)) ////uncertain real case
                    {
                        //do nothing
                    }
                    else
                    {
                        DeviceMgr.SetResi(_iRange, _iReal, -1, _bOn);
                    }
                }
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
                    DeviceMgr.SetResi(_iRange, _iReal, _resistance, _bOn);
                    return;
                }
                if (iRange < 0) //real resistance case
                {
                    if ((iReal < 0) || (iReal >= real_title.Length))
                    {
                        _bOn = false; //uncertain real case
                        DeviceMgr.SetResi(_iRange, _iReal, _resistance, _bOn);
                        return;
                    }
                }
                _bOn = true;
                DeviceMgr.SetResi(_iRange, _iReal, _resistance, _bOn);
            }
        }
    
        internal static double[] real_dispvalue = new double[] { 20, 100, 190, 200, 1000, 1900, 2000, 10000, 19000, 
            20000, 100000, 190000, 200000, 1, 1.9};

        internal static string[] real_disptitle = new string[]     { "", "",  "",  "",  "","","","",""
            ,"","","","","M","M"};
        internal static double[] real_value = new double[] { 20, 100, 190, 200, 1000, 1900, 2000, 10000, 19000, 
            20000, 100000, 190000, 200000, 1000000, 1900000 };
        internal static string[] real_title = new string[] { "20", "100", "190", "200", "1k", "1.9k", "2k", "10k", "19k" ,
            "20k", "100k", "190k","200k", "1M", "1.9M"};

        private Queue<double> datafilter;
        internal Processor()
        {
            _iReal = Util.ConstIni.IntValue("LASTSETTING", "real");
            _resistance = Decimal.Parse(Util.ConstIni.StringValue("LASTSETTING", "resistance"));
            iRange = Util.ConstIni.IntValue("LASTSETTING", "range");
            bOn = false;
        }
        public void RefreshOutput()
        {
            if (iRange < 0 || iRange > 3) //real res case
                return;

            DeviceMgr.CollectCurrent(out Current);
                
        }
        
                    
        internal double Current = -9999;
     
        public void ZeroON()
        {
            DeviceMgr.SetZero();
        }
 

        internal static int CheckRange(Decimal resi, int nowRange)
        {
            double resistance = Convert.ToDouble(resi);
            if (resistance > 0)
            {
                if (resistance <= 0.02) //20mohm
                {
                    if (nowRange >= 0) 
                        return nowRange;
                    return 0;
                }
                if (resistance <= 0.2)  //200mohm
                {
                    if (nowRange >= 1)
                        return nowRange;
                    return 1;
                }
                if (resistance <= 2)    //2ohm
                {
                    if (nowRange >= 2)
                        return nowRange;
                    return 2;
                }
                if (resistance <= 20)    //20 ohm
                {
                    if (nowRange >= 3)
                        return nowRange;
                    return 3;
                }
            }
            return -1;
        }
    }
}
