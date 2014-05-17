using System;
using System.Collections.Generic;
using System.Text;
using NationalInstruments.NI4882;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;
using System.Data;
using System.Threading;
using System.Text.RegularExpressions;

namespace Jmbo
{
    class DeviceMgr
    {
    }

    enum DeviceType
    {
        GPIB_DEVICE,
        SERIAL_DEVICE
    }

    public delegate void StrEventHandler(object sender, string e);
    public delegate void DataEventHandler(object sender, decimal v);
    abstract class MyDevice
    {
        protected string address;
        protected DeviceType dvctype;
        internal abstract bool Open();
        internal abstract void Close();
        internal abstract bool IsConnected();
        internal abstract void Refresh();
        internal abstract void ClearInBuffer();
        internal abstract void Send(string line, bool FlushAndEnd);
        //internal abstract void Send(byte[] data, bool FlushAndEnd);
        internal StrEventHandler DataRecieved;
    }
    class GPIBDevice : MyDevice
    {
        protected static LangInt li;                          // declare an instance of the .NET language interface functions 
        protected static GpibConstants c;                    // declare an instance of the .NET language interface constants
        protected static int TIMEOUT;                        // tmeout value 
        private static string[] ErrorMnemonic = {"EDVR", "ECIC", "ENOL", "EADR", "EARG",          // Error codes 
"ESAC", "EABO", "ENEB", "EDMA", "", 
"EOIP", "ECAP", "EFSO", "", "EBUS", 
"ESTB", "ESRQ", "", "", "", "ETAB"};

        static GPIBDevice()
        {
            li = new LangInt();          // Contains all GPIB functions 
            c = new GpibConstants();    // Contains all GPIB global constants 
            TIMEOUT = c.T300ms;
        }
        ~GPIBDevice()
        {
            if (li is LangInt)
                li.Dispose();
        }
        protected const int EOTMODE = 1;                      // enable the END message 
        protected const int EOSMODE = 0;                      // disable the EOS mode 
        protected const int ARRAYSIZE = 1024; // size of ReadBuffer 
        protected int BDINDEX = 0;     // board Index
        protected int PRIMARY_ADDR_OF_PPS = 5;          // pimary address of device 
        protected int NO_SECONDARY_ADDR = 0;            // secondary address of device 


        protected int Dev; // a reference to an instrument on the bus 

        //formattedIO interface
        private byte[] Response;
        private bool bConnected = false;
        internal GPIBDevice(string port)
        {
            try
            {
                string[] inputs = port.Split(new char[] { ':' });
                PRIMARY_ADDR_OF_PPS = Int32.Parse(inputs[2]);

                //create the formatted io object
                bConnected = false;
                dvctype = DeviceType.GPIB_DEVICE;
                Response = new byte[ARRAYSIZE];
                ResetResponse();

            }
            catch (SystemException ex)
            {
                MessageBox.Show("GPIB 接口参数错误 " + ex.Source + "  " + ex.Message, "GPIBMeasConfig", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        internal override void ClearInBuffer()
        {
            if (IsConnected())
                li.ibclr(Dev);

        }
        internal override bool Open()
        {
            if (IsConnected())
                return true;
            try
            {
                Dev = li.ibdev(BDINDEX, PRIMARY_ADDR_OF_PPS, NO_SECONDARY_ADDR, TIMEOUT, EOTMODE, EOSMODE);
                bConnected = ((li.ibsta & c.ERR) == 0);
                if (!bConnected)
                {
                    Logger.Log("GPIB 接口打开失败");
                    //MessageBox.Show("GPIB 接口打开失败");
                }
                return bConnected;
            }
            catch (SystemException ex)
            {
                Logger.Log("GPIB 接口打开失败 " + PRIMARY_ADDR_OF_PPS.ToString() + " " + ex.Source + "  " + ex.Message);
//               MessageBox.Show("GPIB 接口打开失败 " + PRIMARY_ADDR_OF_PPS.ToString() + " " + ex.Source + "  " + ex.Message, "GPIBMeasConfig", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
        }
        internal override void Close()
        {
            if (IsConnected())
            {

                bConnected = false;
                li.ibonl(PRIMARY_ADDR_OF_PPS, 0);
            }

        }

        internal override bool IsConnected()
        {
            return bConnected;
        }
        internal override void Send(string line, bool FlushAndEnd)
        {
            if (IsConnected())
            {
                Logger.Log(line);
                if(FlushAndEnd)
                    li.ibclr(Dev);
                li.ibwrt(Dev, line);
                Thread.Sleep(50);
                    if ((li.ibsta & c.ERR) != 0)                          // throw an error 
                    {
                        throw new Exception("命令发送失败");
                        return;
                    }
                
            }
        }
        internal void ResetResponse()
        {
            for (int j = 0; j < Response.Length; j++)
                Response[j] = 0x00;
        }
        readonly char[] filter = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ .+-&:\r\n#".ToCharArray();
        internal override void Refresh()
        {
            throw new NotImplementedException("No refresh for GRIB device");
        }
        internal string Read()
        {
            if (IsConnected())
            {
                try
                {
                    string ret = "";
                    int size = li.ibrd(Dev, out ret, 2000);
                    if (ret.Length > 1)
                        Logger.Log(ret);
                    return ret;
                }
                catch
                {
                    return "";
                }
                    /*
                    int ret = li.ibrd(Dev, Response);
                    int asize = li.ibcnt;
                    int size = li.ibcntl;

                    if (Response[0] != 0x00)
                    {
                        string t = "";
                        for (int i = 0; i < Response.Length; i++)
                        {
                            if (Response[i] == 0x00)
                                break;
                            char l = Convert.ToChar(Response[i]);
                            if (i == 0)
                                Response[0] = 0x00; //clear flag

                            t = t + l;
                            if (l.ToString().IndexOfAny(filter) < 0)
                            {
                                Util.SysLog("invalid char" + l.ToString());
                                throw new Exception("invalid char" + l.ToString());
                            }
                        }
                        Util.SysLog("Rcv:" + t);
                        ResetResponse();
                        return t;
                    }
                    
                }
                catch (Exception ex)
                {
                    Util.SysLog("GPIB Refresh:" + ex.Message + ex.StackTrace);
                    ResetResponse();
                }*/
            }
            return "";
        }
    }

    class SerialDevice : MyDevice
    {
        private SerialPort serial;
        public string NewLine
        {
            get
            {
                return serial.NewLine;
            }
            set
            {
                serial.NewLine = value;
            }
        }
        public SerialDevice(string port, int baud, Parity p, int databits, StopBits stop)
        {
            address = port;
            try
            {
                serial = new SerialPort(address, baud, p, databits, stop);
                serial.DataReceived += new SerialDataReceivedEventHandler(serial_DataReceived);
            }
            catch
            {
                serial = null;
            }
        }

        void serial_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (serial.BytesToRead > 0)
            {
                foreach (char l in serial.ReadExisting().ToCharArray())
                    DataRecieved(this, l.ToString());
            }
        }
        internal override void ClearInBuffer()
        {
            if (IsConnected())
                serial.DiscardInBuffer();
        }
        internal override bool Open()
        {
            try
            {
                serial.Open();
                return serial.IsOpen;
            }
            catch(Exception ex)
            {
                Logger.Log("Serial port open fail:" + ex.Message + ex.StackTrace);
                return false;
            }
        }
        internal override void Close()
        {
            if (IsConnected())
                serial.Close();
        }
        internal override bool IsConnected()
        {
            return serial.IsOpen;
        }
        internal override void Refresh()
        {

        }

        internal override void Send(string line, bool FlushAndEnd)
        {
            if (Util.ISDEBUG)
                return;
            if (IsConnected())
            {
                Logger.Log(line);
                serial.WriteLine(line);
            }
        }
    }

    public struct TEMPRDG
    {
        public TEMPRDG(Decimal rdg, DateTime dt)
        {
            reading = rdg;
            time = dt;
        }
        public Decimal reading;
        public DateTime time;
    }


   

    enum WIREMODE
    {
        MODE_2WIRE,
        MODE_3WIRE,
        MODE_4WIRE

    };
    abstract class Scanner
    {
        protected MyDevice dev;
        public string id;
        public bool bInUse;

        protected DataEventHandler handler = null;

        protected StringBuilder inbuffer;

        protected string response;
        protected void DataHandler(object sender, string inchar)
        {
            if (inchar.Length <= 0)
                return;

            if (inchar == "\r" || inchar == "\n")
            {
                if (inbuffer.Length > 0)
                {
                    response = inbuffer.ToString();
                    Logger.Log(response);
                    inbuffer.Remove(0, inbuffer.Length);
                }
            }
            else
            {
                inbuffer.Append(inchar);
            }
        }
        protected string HitStyle(string style, string line)
        {
            Regex reg = new Regex(style);
            Match m = reg.Match(line);
            if (m.Success)
                return m.Groups[1].Value;
            throw new Exception("返回数据格式错误");

        }
        protected string TalkTryThree(string msg)
        {
            response = "";

            for (int i = 0; i < 3; i++)
            {
                dev.Send(msg, true);
                int j = 7;
                while (response == "")
                {
                    Thread.Sleep(100);
                    if (j-- < 0)
                        break;
                }
                if (response != "")
                    return response;
            }
            throw new Exception("与扫描器通讯失败");
        }

        public virtual bool ScanTo(int position, WIREMODE mode)
        {
            throw new Exception("Class Scanner cannot be called directly.");
        }
    }

    class Scanner2560 : Scanner  //it is only part of FL1560
    {
        public string header = "";
        public string channel = "";
        public Scanner2560(string section)
        {
            header = String.Format("SYSTem:MOD{0}:DEV{0}:", Util.ConstIni.StringValue(section, "MOD_ID"), Util.ConstIni.StringValue(section, "DEV_ID"));
            channel = "1";
        }
        public override bool ScanTo(int position, WIREMODE wire)
        {
            if (position < 0 || position >= 8)
                throw new Exception("FLUKE 2560 做多只支持8个通道");
            channel = (position+1).ToString();
            return true;
        }
        
    }
    class ScannerST4012A : Scanner
    {
        
        private string serial = "";
        private string version = "";
        
        public ScannerST4012A(string section)
        {
            this.id = section;
            inbuffer = new StringBuilder();
            string port = Util.ConstIni.StringValue(section, "PORT");
            serial = Util.ConstIni.StringValue(section, "SERIAL");
            version = Util.ConstIni.StringValue(section, "VERSION");
            dev = new SerialDevice(port,9600,Parity.None,8,StopBits.One);
            (dev as SerialDevice).NewLine = "\r";
            dev.DataRecieved += new StrEventHandler(MyDataHandler);
            bInUse = false;
            if (dev.Open())
            {
                dev.ClearInBuffer();
                dev.Send(String.Format("LFST4012 {0} R Y\r", serial), true);
                Thread.Sleep(200);
                response = "";
                dev.Send(String.Format("LFST4012 {0} I V\r",serial), true);
                Thread.Sleep(200);
                bInUse = (response == version);
                if (!bInUse)
                    dev.Close();
            }
        }
        private void MyDataHandler(object sender, string inchar)
        {
            if (inchar.Length <= 0)
                return;

            if (inchar == "\r" || inchar == "\n")
            {
                if (inbuffer.Length > 0)
                {
                    response = inbuffer.ToString();
                    Logger.Log(response);
                    inbuffer.Remove(0, inbuffer.Length);
                }
            }
            else
            {
                inbuffer.Append(inchar);
            }
        }
        private int lastpos = -1;
        private WIREMODE lastmode = WIREMODE.MODE_2WIRE;
        //position is zero based
        public override bool ScanTo(int position, WIREMODE wire)
        {
            position++;
            if (position > 12)
                return false;
            if (lastmode == wire && lastpos == position)
                return true;
            Thread.Sleep(1000);
            if (wire == WIREMODE.MODE_2WIRE)
                dev.Send(String.Format("LFST4012 {0} {1} 0\r", serial, position), true);
            else if (wire == WIREMODE.MODE_3WIRE)
                dev.Send(String.Format("LFST4012 {0} {1} 1\r", serial, position), true);
            else if (wire == WIREMODE.MODE_4WIRE)
                dev.Send(String.Format("LFST4012 {0} {1} 2\r", serial, position), true);
            else
                return false;
            Thread.Sleep(1000);
            if (lastpos == position)//send command again for the 4012 to enable channel
            {
                if (wire == WIREMODE.MODE_2WIRE)
                    dev.Send(String.Format("LFST4012 {0} {1} 0\r", serial, position), true);
                else if (wire == WIREMODE.MODE_3WIRE)
                    dev.Send(String.Format("LFST4012 {0} {1} 1\r", serial, position), true);
                else if (wire == WIREMODE.MODE_4WIRE)
                    dev.Send(String.Format("LFST4012 {0} {1} 2\r", serial, position), true);

            }
            lastpos = position;
            lastmode = wire;
            Thread.Sleep(2000);
            return true;
        }
    }
    //10channel scanner
    class ScannerSB148: Scanner
    {
        private string Serial = "";
        public ScannerSB148(string section)
        {
            this.id = section;
            if (Util.GPIBDBG)
            {
                bInUse = true;
                return;
            }
            Serial = Util.ConstIni.StringValue(section, "SERIAL");
            dev = new GPIBDevice("GPIB0::" + Util.ConstIni.StringValue(section, "PORT") + "::INSTR");

            bInUse = false;
            if (dev.Open())
            {
                dev.ClearInBuffer();
                bInUse = true;//(TalkTryThree("*IDN?\r\n") == Serial);
                if (!bInUse)
                    dev.Close();
            }
        }
        //format is 
        public override bool ScanTo(int position,WIREMODE wire)
        {
            if (wire != WIREMODE.MODE_4WIRE)
                throw new Exception("扫描器SB148不支持3线测量");
            string spos = position.ToString();
            
            char l = '0';
            if(spos.Length >=3)
            {
                l = spos[0];
                spos = spos.Remove(0, spos.Length-2);
            }
            char i = '0';
            if (spos.Length >= 2)
            {
                i = spos[0];
                spos = spos.Remove(0, 1);
            }
            char m = '0';
            if (spos.Length >= 1)
            {
                m = spos[0];
                spos = spos.Remove(0, 1);
            }
            while (true)
            {
                try
                {
                    if (dev.IsConnected())
                    {
                        dev.Send(String.Format("I{0}L{1}M{2}\r\n", i, l, m), true);
                        Thread.Sleep(300);
                        dev.Send(String.Format("I{0}L{1}M{2}\r\n", i, l, m), true);
                        Thread.Sleep(300);
                        return true;
                    }
                }
                catch
                {
                    if (DialogResult.Cancel == MessageBox.Show("扫描开关切换失败，请检查连接和电源后按'确定'重试,按'取消'放弃", "错误提示", MessageBoxButtons.OKCancel))
                        throw new Exception("扫描开关切换失败");
                }
            }
            return false;
        }
    }
    //Calibration Bath
    class Bath7341:SerialPortOven
    {
       //default baudrate is 2400
        public Bath7341(string section)
        {
            this.id = section;
            string port = Util.ConstIni.StringValue(section, "PORT");
            inbuffer = new StringBuilder(100);
            bInUse = false;
            dev = new SerialDevice(port, 2400, Parity.None, 8, StopBits.One);
            dev.DataRecieved += new StrEventHandler(DataHandler);
            if (dev.Open())
                bInUse = true;
        }
    }
    class BathCTRJ 
    {
        protected SerialPort dev;
        public string id;
        public bool bInUse;
        public BathCTRJ(string section)
        {
            this.id = section;
            string port = Util.ConstIni.StringValue(section, "PORT");
            
            bInUse = false;
            dev = new SerialPort(port, 9600, Parity.None, 8, StopBits.One);
            dev.Open();
            
             bInUse = dev.IsOpen;
            
        }
        public void SetTemperature(string temp)
        {
            //                                    addr  func  len   sign                     xor   end
            byte[] cmd = new byte[] { 0x05, 0xc1, 0x01, 0x02, 0x04, 0x00, 0x02, 0x03, 0x04, 0xff, 0xAA };
            double dtemp =  Double.Parse(temp);
            if (!bInUse)
                return;
            dev.DiscardInBuffer();
            cmd[9] = 0x03 ^ 0x04;
            if(dtemp < 0)
            {
                cmd[5] = 0x01;
                dtemp = -dtemp;
            }
            
            
            int itemp = Convert.ToInt32(Math.Floor(dtemp * 10000.0));
            cmd[6] = Convert.ToByte(itemp % 256);
            
            itemp = Convert.ToInt32(Math.Floor(itemp / 256.0));
            cmd[7] = Convert.ToByte(itemp % 256);
            
            itemp = Convert.ToInt32(Math.Floor(itemp / 256.0));
            cmd[8] = Convert.ToByte(itemp % 256);
            
            cmd[9] = Convert.ToByte(cmd[9] ^ cmd[5] ^ cmd[6] ^ cmd[7] ^ cmd[8]);
            dev.Write(cmd, 0, cmd.Length);
            Thread.Sleep(500);
            if(dev.ReadBufferSize < 10)
                throw new Exception("设定温度槽温度失败");
            for (int i = 0; i < 10; i++)
            {
                if (dev.ReadByte() != cmd[i])
                    throw new Exception("设定温度槽温度失败");
            }
        }
    }
  
    enum ONOFFSTATE
    {
        ON,
        OFF
    }
    abstract class SerialPortOven
    {
        protected MyDevice dev;
        public string id;
        public bool bInUse;

        protected DataEventHandler handler = null;
        protected StringBuilder inbuffer;

        protected string response;
        protected void DataHandler(object sender, string inchar)
        {
            if (inchar.Length <= 0)
                return;

            if (inchar == "\r" || inchar == "\n")
            {
                if (inbuffer.Length > 0)
                {
                    response = inbuffer.ToString();
                    Logger.Log(response);
                    inbuffer.Remove(0, inbuffer.Length);
                }
            }
            else
            {
                inbuffer.Append(inchar);
            }
        }

        protected string HitStyle(string style, string line)
        {
            Regex reg = new Regex(style);
            Match m = reg.Match(line);
            if (m.Success)
                return m.Groups[1].Value;
            throw new Exception("返回数据格式错误");

        }
        protected string TalkTryThree(string msg)
        {
            response = "";

            for (int i = 0; i < 3; i++)
            {
                dev.Send(msg, true);
                int j = 7;
                while (response == "")
                {
                    Thread.Sleep(100);
                    if (j-- < 0)
                        break;
                }
                if (response != "")
                    return response;
            }
            throw new Exception("与定点炉通讯失败");
        }

        public void SetUnitToC()
        {
            if (dev.IsConnected())
            {
                dev.Send("u=c\r\n", true);
                Thread.Sleep(100);
                if ("C" == HitStyle("u[: ]+([A-Z+])", TalkTryThree("u\r\n")))
                    return;
                throw new Exception("设置温度单位失败");
            }
        }
        public void SetScanRate(string rate)
        {
            if (dev.IsConnected())
            {
                for (int i = 0; i < 3; i++)
                {
                    dev.Send("sr=" + rate + "\r\n", true);
                    Thread.Sleep(200);
                    string value = HitStyle("srat[: ]+([0-9.eE\\+\\-]+)[ ]+(C|F)", TalkTryThree("sr\r\n"));
                    if ((value != "") && (Decimal.Parse(rate) == Decimal.Parse(value))) //setting is downloaded successfully
                        return;
                    Thread.Sleep(1000);
                }
                throw new Exception("温度ScanRate失败");
            }
        }
        public void SetScanMode(ONOFFSTATE onoff)
        {
            if (dev.IsConnected())
            {
                dev.Send(String.Format("sc={0}\r\n",onoff.ToString().ToLower()), true);

                Thread.Sleep(100);
                string ret = HitStyle("scan[: ]+([A-Z]+)", TalkTryThree("sc\r\n"));
                if (ret.Trim() != "" && onoff ==  (ONOFFSTATE)Enum.Parse(typeof(ONOFFSTATE),ret ,true))
                    return;
                throw new Exception("设置ScanMode失败");
            }
        }
        public virtual Decimal ReadTemperature()
        {
            return ReadTemperature("t", "t");
        }
        protected Decimal ReadTemperature(string ask, string reply_header)
        {
            if (dev.IsConnected())
            {
                string value = HitStyle(reply_header + "[: ]+([0-9.eE\\+\\-]+)[ ]+(C|F)", TalkTryThree(ask + "\r\n"));
                Decimal result;
                if (Decimal.TryParse(value, out result))
                    return result;
                throw new Exception("读取温度失败:" + ask);
            }
            throw new Exception("设备未连接");
        }
        public virtual void SetTemperature(string temp)
        {
            if (dev.IsConnected())
            {
                for (int i = 0; i < 3; i++)
                {
                    dev.Send("s=" + temp + "\r\n", true);
                    Thread.Sleep(200);
                    string value = HitStyle("set[: ]+([0-9.eE\\+\\-]+)[ ]+(C|F)", TalkTryThree("s\r\n"));
                    if ((value != "") && (Decimal.Parse(temp) == Decimal.Parse(value))) //setting is downloaded successfully
                        return;
                    Thread.Sleep(1000);
                }
                throw new Exception("温度设置失败");
            }
        }
        public Decimal GetDecimalSetting(string key)
        {
            return Decimal.Parse(Util.ConstIni.StringValue(id, key));
        }
        public string GetSetting(string key)
        {
            return Util.ConstIni.StringValue(id, key);
        }

    }

    //temperature collector used for kongwy
    class WD1529 : SerialPortOven
    {   //4channel at most
        private TEMPRDG[] rdgs = new TEMPRDG[4];
        private TEMPRDG invalid_rdg = new TEMPRDG();
        public TEMPRDG GetTemperature(int channel)
        {
            if (channel <= 0 || channel > 4)
                return invalid_rdg;
            TEMPRDG ret = rdgs[channel];
            rdgs[channel] = invalid_rdg;
            return ret;
        }
        public bool HasReading(int channel)
        {
            if (channel <= 0 || channel > 4)
                return false;
            return rdgs[channel].reading > -999;
        }
        //default baudrate is 9600
        public WD1529(string section)
        {
            this.id = section;
            string port = Util.ConstIni.StringValue(section, "PORT");
            inbuffer = new StringBuilder(100);
            bInUse = false;
            invalid_rdg.reading = Convert.ToDecimal(-9999);
            invalid_rdg.time = DateTime.Now;
            rdgs[0] = rdgs[1] = rdgs[2] = rdgs[3] = invalid_rdg;
            dev = new SerialDevice(port, 9600, Parity.None, 8, StopBits.One);
            dev.DataRecieved += new StrEventHandler(MyDataHandler);
            if (dev.Open())
                bInUse = true;
        }
        private void MyDataHandler(object sender, string inchar)
        {
            if (inchar.Length <= 0)
                return;

            if (inchar == "\r" || inchar == "\n")
            {
                if (inbuffer.Length > 0)
                {
                    response = inbuffer.ToString();
                    Logger.Log(response);
                    inbuffer.Remove(0, inbuffer.Length);
                    //4  ........ C  05:18:01 09-05-13
                    Match m = Regex.Match(response, "(\\d)  (\\d+) C  ([-0-9: ]+)");
                    if (!m.Success)
                        return;
                    rdgs[Int32.Parse(m.Groups[1].Value) - 1].reading = Decimal.Parse(m.Groups[2].Value);
                    rdgs[Int32.Parse(m.Groups[1].Value) - 1].time = DateTime.Parse(m.Groups[3].Value);
                }
            }
            else
            {
                inbuffer.Append(inchar);
            }
        }

    }
    enum ADVTYPE
    {
        FREEZE,
        MAINTAIN,
        MELT,
        NONE
    }
    //triple point of water
    class Oven9210 : SerialPortOven
    {
        //default baudrate is 2400
        public Oven9210(string section)
        {
            this.id = section;
            string port = Util.ConstIni.StringValue(section, "PORT");
            inbuffer = new StringBuilder(100);
            bInUse = false;
            dev = new SerialDevice(port, 2400, Parity.None, 8, StopBits.One);
            dev.DataRecieved += new StrEventHandler(DataHandler);
            if (dev.Open())
                bInUse = true;
        }
        /*
         * df: 6
dm: 480
adv: MAINTAIN
set:    0.01 C
ma:   0.010 C
fr:   -4.50 C
         */
        public int iFreezeDuration = 6;
        public int iMaintDuration = 480;

        public void SetAdvance(ADVTYPE adv)
        {
            if (dev.IsConnected())
            {
                dev.Send(String.Format("adv={0}\r\n", adv.ToString().ToLower()), true);
                Thread.Sleep(100);
                if (adv == (ADVTYPE)Enum.Parse(typeof(ADVTYPE), HitStyle("adv[: ]+([A-Z]+)", TalkTryThree("adv\r\n")), true))
                    return;
                throw new Exception("设置水三相点内置程序失败:"+adv);
            }
        }
        public ADVTYPE ReadAdvance()
        {
            if (dev.IsConnected())
            {
                return (ADVTYPE)Enum.Parse(typeof(ADVTYPE), HitStyle("adv[: ]+([A-Z]+)", TalkTryThree("adv\r\n")), true);
            }
            throw new Exception("设备未连接");
        }
        public Decimal ReadMaintainTemperature()
        {
            return ReadTemperature("ma", "ma");
        }
        public void SetMaintainTemperature(string temp)
        {
            if (dev.IsConnected())
            {
                for (int i = 0; i < 3; i++)
                {
                    dev.Send("ma=" + temp + "\r\n", true);
                    Thread.Sleep(200);
                    string value = HitStyle("ma[: ]+([0-9.eE\\+\\-]+)[ ]+(C|F)", TalkTryThree("ma\r\n"));
                    if ((value != "") && (Decimal.Parse(temp) == Decimal.Parse(value))) //setting is downloaded successfully
                        return;
                    Thread.Sleep(1000);
                }
                throw new Exception("水三相点MAINT温度设置失败:"+temp);
            }
        }
        public Decimal ReadFreezeTemperature()
        {
            return ReadTemperature("fr", "fr");
        }
        public void SetFreezeTemperature(string temp)
        {
            if (dev.IsConnected())
            {
                for (int i = 0; i < 3; i++)
                {
                    dev.Send("fr=" + temp + "\r\n", true);
                    Thread.Sleep(200);
                    string value = HitStyle("fr[: ]+([0-9.eE\\+\\-]+)[ ]+(C|F)", TalkTryThree("fr\r\n"));
                    if ((value != "") && (Decimal.Parse(temp) == Decimal.Parse(value))) //setting is downloaded successfully
                        return;
                    Thread.Sleep(1000);
                }
                throw new Exception("水三相点FREEZ温度设置失败:" + temp);
            }
        }


    }
    enum FixedPoint
    {
        INDIUM,
        TIN,
        ZINC,
        ALUMINUM,

    }
    //Gallium Cell
    class Oven9230 : SerialPortOven
    {
        //default baudrate is 2400
        public Oven9230(string section)
        {
            this.id = section;
            string port = Util.ConstIni.StringValue(section, "PORT");
            inbuffer = new StringBuilder(100);
            bInUse = false;
            
            dev = new SerialDevice(port, 2400, Parity.None, 8, StopBits.One);
            dev.DataRecieved += new StrEventHandler(DataHandler);
            if (dev.Open())
                bInUse = true;

        }
    }
    
    //Mini Fixed-Point Cell Furnace
    class Oven9260 : SerialPortOven
    {
        //default baudrate is 2400
        public Oven9260(string section)
        {
            this.id = section;
            string port = Util.ConstIni.StringValue(section, "PORT");
            inbuffer = new StringBuilder(100);
            bInUse = false;
            
            dev = new SerialDevice(port, 2400, Parity.None, 8, StopBits.One);
            dev.DataRecieved += new StrEventHandler(DataHandler);
            if (dev.Open())
                bInUse = true;
        }
        
        public FixedPoint GetFixPointType()
        {
            if (dev.IsConnected())
            {
                for (int i = 0; i < 3; i++)
                {
                    string value = HitStyle("fp[: ]+([A-Z+])", TalkTryThree("fp\r\n"));
                    if (value != "") //setting is downloaded successfully
                    {
                        return (FixedPoint)Enum.Parse(typeof(FixedPoint), value,true);
                    }
                    Thread.Sleep(1000);
                }
                throw new Exception("定点炉类型读取失败");
            }
            throw new Exception("设备未连接");
        }
    }

    abstract class ThermBridge
    {
        internal string id;
        internal bool bInUse;
        internal MyDevice dev;
        internal bool bStable = false;
        internal abstract void ClearBuffer();
        
        internal virtual Decimal Resistance
        {
            get;
            set;
        }
        internal bool bPowerX2
        {
            get;
            set;
        }
        internal bool bInMeasure
        {
            get;
            set;
        }
    }
    //*IDN?
    //SYST:CONF:MNUM? return 1
    //SYST:CONF:MOD1:DEV1:INF?  return "PRTS",INP,8
    //SYSTem:MODule[n]:DEVice[n]:READ?
    //CALC3:CONV:NAME?  return RES
    //CALC2:CONV:NAME I90
    //FETC? (@1) return 108.4016 //最近值
    //MEAS? (@1) return 108.4016 //新值
    //READ？ return 108.4016 //新值
    //STAT:OPER:COND? return 0
    class FL1560 : ThermBridge
    {
        private Scanner2560 sc2560;
        private string Serial = "";
        private Dictionary<string, string> marks;
        public FL1560(string section,Scanner2560 sc)
        {
            id = section;
            sc2560 = sc;
            Serial = Util.ConstIni.StringValue(section, "SERIAL");
            dev = new SerialDevice(Util.ConstIni.StringValue(section, "PORT"),2400,Parity.None, 8, StopBits.One);
            dev.DataRecieved += new StrEventHandler(DataHandler);
            marks = new Dictionary<string, string>();
            bStable = true;
            if (dev.Open())
            {
                dev.ClearInBuffer();
                bInUse = (TalkTryThree("*IDN?").StartsWith(Serial));
                if (!bInUse)
                    dev.Close();
            }
        }
        internal override void ClearBuffer()
        {
            if (dev.IsConnected())
            {
                dev.ClearInBuffer();
            }
        }
        private bool SetRESMode()
        {
            string channel = sc2560.channel;
            if (marks.ContainsKey("RES" + channel) && (marks["RES" + channel] == "YES"))
                return true;
            if("RES" == TalkTryThree(String.Format("CALC{0}:CONV:NAME?\r\n",channel)))
            {
                marks["RES" + channel] = "YES";
                return true;
            }
            dev.Send(String.Format("CALC{0}:CONV:NAME RES\r\n", channel), true);
            Thread.Sleep(300);
            if ("RES" == TalkTryThree(String.Format("CALC{0}:CONV:NAME?\r\n", channel)))
            {
                return marks["RES" + channel] == "YES";
            }
            return false;
        }
        protected string response;
        protected StringBuilder inbuffer = new StringBuilder();
        protected void DataHandler(object sender, string inchar)
        {
            if (inchar.Length <= 0)
                return;

            if (inchar == "\r" || inchar == "\n")
            {
                if (inbuffer.Length > 0)
                {
                    response = inbuffer.ToString();
                    Logger.Log(response);
                    inbuffer.Remove(0, inbuffer.Length);
                }
            }
            else
            {
                inbuffer.Append(inchar);
            }
        }
        protected string TalkTryThree(string msg)
        {
            response = "";

            for (int i = 0; i < 3; i++)
            {
                dev.Send(msg, true);
                int j = 7;
                while (response == "")
                {
                    Thread.Sleep(100);
                    if (j-- < 0)
                        break;
                }
                if (response != "")
                    return response;
            }
            return "";
        }
        internal override Decimal Resistance
        {
            get
            {
                if (dev.IsConnected())
                {
                    if(!SetRESMode())
                        throw new Exception("设置FLUKE2560失败");
                    bStable = false;
                    string value = TalkTryThree(String.Format("MEAS? (@{0})\r\n",sc2560.channel));
                    Decimal result;
                    if (Decimal.TryParse(value, out result))
                    {
                        bStable = true;
                        return result;
                    }
                    throw new Exception("读取FLUKE1560数值失败");
                }
                throw new Exception("设备未连接");
            }
            
        }
        new internal bool bInMeasure
        {
            get
            {
                return ("0" != String.Format("STAT:OPER:COND?\r\n"));
            }
        }
    }
    class DQF700 : ThermBridge
    {
        private string Serial = "";
        public DQF700(string section)
        {
            id = section;
            if (Util.GPIBDBG)
            {
                bInUse = true;
                return;
            }
            Serial = Util.ConstIni.StringValue(section, "SERIAL");
            dev = new GPIBDevice("GPIB0::" + Util.ConstIni.StringValue(section, "PORT") + "::INSTR");

            bInUse = false;
            if (dev.Open())
            {
                dev.ClearInBuffer();
                bInUse = true;//(TalkTryThree("*IDN?\r\n") == Serial);
                if (!bInUse)
                    dev.Close();
            }
        }
        internal override void ClearBuffer()
        {
            if (dev.IsConnected())
            {
                (dev as GPIBDevice).ResetResponse();
                dev.ClearInBuffer();
            }
        }
        protected string HitStyle(string style, string line)
        {
            Regex reg = new Regex(style);
            Match m = reg.Match(line);
            if (m.Success)
                return m.Groups[1].Value;
            throw new Exception("返回数据格式错误");

        }
        protected string TalkTryThree(string msg)
        {
            return TalkTryThree(msg, true);
        }
        protected string TalkTryThree(string msg, bool clr)
        {
            string response = "";

            for (int i = 0; i < 3; i++)
            {
                dev.Send(msg, clr);
                int j = 7;
                while (response == "")
                {
                    Thread.Sleep(100);
                    response = (dev as GPIBDevice).Read();
                    response = response.Trim(new char[] { '\b', '\r', '\n', ' ' });
                    if (j-- < 0)
                        break;
                }
                if (response != "")
                    return response;
            }
            throw new Exception("与电桥通讯失败");
        }
        
        internal override Decimal Resistance
        {
            get
            {
                if (Util.GPIBDBG)
                {
                    return Convert.ToDecimal(100);
                }
                if (dev.IsConnected())
                {
                    bStable = false;
                    string value = HitStyle("([0-9.eE\\+\\-]+\\w)", TalkTryThree("D\n", false));
                    Decimal result;
                    if (!value.EndsWith("B"))
                    {
                        return Convert.ToDecimal(0.0);
                    }
                    value = value.TrimEnd(new char[] { 'B' });
                    if (Decimal.TryParse(value, out result))
                    {
                        bStable = true;
                        return result;
                    }
                    throw new Exception("读取F700数值失败");
                }
                throw new Exception("设备未连接");
            }
        }
    }
    
    class DQ6622A : ThermBridge
    {
         public DQ6622A(string section)
        {
            id = section;
            if (Util.GPIBDBG)
            {
                bInUse = true;
                return;
            }
            dev = new GPIBDevice("GPIB0::" + Util.ConstIni.StringValue(section, "PORT") + "::INSTR");

            bInUse = false;
            if (dev.Open())
            {
                dev.ClearInBuffer();
                
                bInUse = true;
            }
        }
         protected string HitStyle(string style, string line)
         {
             Regex reg = new Regex(style);
             Match m = reg.Match(line);
             if (m.Success)
                 return m.Groups[1].Value;
             throw new Exception("返回数据格式错误");

         }
         internal override void ClearBuffer()
         {
             if (dev.IsConnected())
                 dev.ClearInBuffer();
         }
         protected string TalkTryThree(string msg)
         {
             string response = "";

             for (int i = 0; i < 3; i++)
             {
                 dev.Send(msg, true);
                 int j = 7;
                 while (response == "")
                 {
                     Thread.Sleep(100);
                     response = (dev as GPIBDevice).Read();
                     if (j-- < 0)
                         break;
                 }
                 if (response != "")
                     return response;
             }
             throw new Exception("与电桥通讯失败");
         }
         new public Decimal Resistance
         {
             get
             {
                 if (Util.GPIBDBG)
                 {
                     return Convert.ToDecimal(100);
                 }
                 if (dev.IsConnected())
                 {
                     string value = HitStyle("([0-9.eE\\+\\-]+)", TalkTryThree("FETCH?"));
                     Decimal result;
                     if (Decimal.TryParse(value, out result))
                         return result;
                     throw new Exception("读取6622A数值失败");
                 }
                 throw new Exception("设备未连接");
             }
         }
         private int _iPowerX2 = -1;
         new public bool bPowerX2
         {
             get
             {
                 if (Util.GPIBDBG)
                 {
                     return (_iPowerX2 == 1);
                 }
                 if (dev.IsConnected())
                 {
                     string value = _iPowerX2.ToString();
                     if (_iPowerX2 == -1)
                     {
                          value = HitStyle("., \\d, \\d, \\d, (\\d)", TalkTryThree("CONF:RESI:DISP?"));
                     }
                     if (value == "0")
                     {
                         _iPowerX2 = 0;
                         return false;
                     }
                     if (value == "1")
                     {
                         _iPowerX2 = 1;
                         return true;
                     }
                     throw new Exception("读取6622A倍功率状态失败");
                 }
                 throw new Exception("设备未连接");
             }
             set
             {
                 if (Util.GPIBDBG)
                 {
                     if (value)
                         _iPowerX2 = 1;
                     else
                         _iPowerX2 = 0;
                     return;
                 }
                 if (dev.IsConnected())
                 {
                     for (int i = 0; i < 3; i++)
                     {
                         if (value && _iPowerX2 != 1)
                         {
                             dev.Send("CONF:RESI:DISP O,2,0,0,1", true);
                         }
                         else if (!value && _iPowerX2 != 0)
                         {
                             dev.Send("CONF:RESI:DISP O,2,0,0,0", true);
                         }
                         
                         Thread.Sleep(3000);
                         _iPowerX2 = -1;
                         try
                         {
                             bool ret = bPowerX2;

                             if (_iPowerX2 != -1)
                             {
                                 if (value == ret)
                                     return;
                             }
                         }
                         catch
                         {
                         }
                         Thread.Sleep(1000);
                     }
                 }
                 throw new Exception("设备未连接");
             }
         }
         new public bool bInMeasure
         {
             get
             {
                 if (Util.GPIBDBG)
                 {
                     return true;
                 }
                 if (dev.IsConnected())
                 {
                     string value = HitStyle("(0|1)", TalkTryThree("MEAS?"));
                     if(value == "0")
                         return false;
                     if (value == "1")
                         return true;
                     throw new Exception("读取6622A数值失败");
                 }
                 throw new Exception("设备未连接");
             }
             set
             {
                 if (Util.GPIBDBG)
                 {
                     return;
                 }
                 if (dev.IsConnected())
                 {
                     if (value)
                     {
                         dev.Send("MEAS 1", true);
                         Thread.Sleep(200);
                         for (int i = 0; i < 3; i++)
                         {
                             if (bInMeasure)
                                 return;
                         }
                     }
                 }
                 throw new Exception("设备未连接");
             }
         }
    }
}
