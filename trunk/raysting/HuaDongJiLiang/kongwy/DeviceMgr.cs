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
                    MessageBox.Show("GPIB 接口打开失败");
                }
                return bConnected;
            }
            catch (SystemException ex)
            {
                MessageBox.Show("GPIB 接口打开失败 " + PRIMARY_ADDR_OF_PPS.ToString() + " " + ex.Source + "  " + ex.Message, "GPIBMeasConfig", MessageBoxButtons.OK, MessageBoxIcon.Error);
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
                li.ibclr(Dev);
                li.ibwrt(Dev, line);
                Thread.Sleep(50);
                    if ((li.ibsta & c.ERR) != 0)                          // throw an error 
                    {
                        MessageBox.Show("命令发送失败");
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
            catch
            {
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

    class Scanner
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

        public virtual bool ScanTo(int position){
            throw new Exception("Class Scanner cannot be called directly.");
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
    class WD1529: SerialPortOven
    {   //4channel at most
        private TEMPRDG[] rdgs = new TEMPRDG[4];
        private TEMPRDG invalid_rdg = new TEMPRDG();
        public TEMPRDG GetTemperature(int channel)
        {
            if(channel <=0 || channel >4)
                return invalid_rdg;
            TEMPRDG ret = rdgs[channel-1];
            rdgs[channel-1] = invalid_rdg;
            return ret;
        }
        public bool HasReading(int channel)
        {
            if (channel <= 0 || channel > 4)
                return false;
            return rdgs[channel-1].reading > -999;
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
            {
                if (TalkTryThree("*IDN?").Trim().StartsWith("HART,1529"))
                {
                    
                    bInUse = true;
                }
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
                    inbuffer.Remove(0, inbuffer.Length);
                    //4  ........ C  05:18:01 09-05-13
                    Match m = Regex.Match(response, "(\\d)\\s+([-0-9.]+) (C|O)  ([-0-9: ]+)");
                    if (!m.Success)
                    {
                        
                        return;
                    }
                    Decimal r;
                    if (Decimal.TryParse(m.Groups[2].Value, out r))
                    {
                        rdgs[Int32.Parse(m.Groups[1].Value) - 1].reading = r;
                        rdgs[Int32.Parse(m.Groups[1].Value) - 1].time = DateTime.Now;
                    }
                }
            }
            else
            {
                if (inbuffer.Length > 100)
                    inbuffer.Remove(0, inbuffer.Length);
                inbuffer.Append(inchar);
            }
        }

    }
    class DQF700
    {
              internal string id;
        internal bool bInUse;
        private GPIBDevice dev;
        public DQF700(string section)
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
                     response = dev.Read();
                     if (j-- < 0)
                         break;
                 }
                 if (response != "")
                     return response;
             }
             throw new Exception("与电桥通讯失败");
         }
         public bool bStable = false;
         public Decimal Resistance
         {
             get
             {
                 if (Util.GPIBDBG)
                 {
                     return Convert.ToDecimal(100);
                 }
                 if (dev.IsConnected())
                 {

                     while (true)
                     {
                         string value = HitStyle("([0-9.eE\\+\\-]+\\w)", TalkTryThree("D\r"));
                         Decimal result;
                         if (!value.EndsWith("B"))
                         {
                             bStable = false;
                             return Convert.ToDecimal(0.0);
                         }
                         value = value.TrimEnd(new char[] { 'B' });
                         if (Decimal.TryParse(value, out result))
                         {
                             return result;
                         }
                         throw new Exception("读取F700数值失败");
                     }
                 }
                 throw new Exception("设备未连接");
             }
         }
    }
    //10channel scanner
    class ScannerSB148: Scanner
    {
        public ScannerSB148(string section)
        {
            this.id = section;
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
        //position is zero based
        public override bool ScanTo(int position)
        {
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
            if (dev.IsConnected())
            {
                dev.Send(String.Format("I{0}L{1}M{2}\r\n", i, l, m), true);
                Thread.Sleep(100);
                return true;
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
    class SwitchSER
    {
        protected SerialPort dev;
        public string id;
        public bool bInUse;
        public SwitchSER(string section)
        {
            this.id = section;
            string port = Util.ConstIni.StringValue(section, "PORT");

            bInUse = false;
            try
            {
                dev = new SerialPort(port, 9600, Parity.None, 8, StopBits.One);

                dev.Open();
                bInUse = dev.IsOpen;
            }
            catch
            {
                
            }

            
        }
        private byte[] ary = new byte[] { 0x55, 0xAA, 0xf0 };
        private byte[] inbuf = new byte[3];
        public bool IsInLoop()
        {
            if (bInUse)
            {
                dev.DiscardInBuffer();
                dev.Write(ary,0,3);
                Thread.Sleep(100);
                if(dev.BytesToRead < 3)
                    return false;
                dev.Read(inbuf, 0 ,3);
                return (inbuf[0] == ary[0] && inbuf[1] == ary[1] && inbuf[2] == ary[2]);
            }
            return false;
        }
    }
    class SwitchS200
    {
        protected SerialPort dev;
        public string id;
        public bool bInUse;
        public SwitchS200(string section)
        {
            this.id = section;
            string port = Util.ConstIni.StringValue(section, "PORT");
            //length = int.Parse(Util.ConstIni.StringValue(section, "LENGTH"));
            //addr = double.Parse(Util.ConstIni.StringValue(section, "ADDR"));
            bInUse = false;
            dev = new SerialPort(port, 9600, Parity.Even, 8, StopBits.One);
            dev.Open();
            bInUse = dev.IsOpen;
            if ("" == ReadInput())
                dev.Close();
            bInUse = dev.IsOpen;
                
        }
        int length = 1;
        double addr = 2;
        
        public string ReadInput()
        {
            byte[] data = new byte[] { 0x68, 0x1B, 0x1B, 0x68, 0x02, 0x00, 0x7C, 0x32, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x0E, 0x00, 0x00, 0x04, 0x01, 0x12, 0x0A, 0x10, 0x02, 0x00, 0x01, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x84, 0x16 };// ConvertDataToByte.ReadI(2, length, addr);
            if (bInUse)
            {
                dev.Write(data, 0, data.Length);
                Thread.Sleep(100);
            }
            length = dev.BytesToRead;
            data = new byte[length];
            if (length > 0)
            {
                dev.Read(data, 0, length);
                dev.DiscardInBuffer();
                //如果接收到的数据长度为1，值为E5H (229)，则为确认码，返回读写确认命令
                if (data[0] == (byte)229 && data.Length == 1)
                {
                    //收到数据
                    byte[] dataSend = new byte[] { 0x10, 0x02, 0x00, 0x5C, 0x5E, 0x16 };
                    //dataSend[0] = 16;   
                    //dataSend[1] = 0x02;//站号
                    //dataSend[2] = 0x00;
                    //dataSend[3] = 0x5C;
                    //dataSend[4] = (byte)(dataSend[3] + dataSend[1]);
                    //dataSend[5] = 0x16;
                    dev.DiscardOutBuffer();
                    dev.DiscardInBuffer();
                    Thread.Sleep(100);
                    dev.Write(dataSend, 0, dataSend.Length);
                    Thread.Sleep(100);

                    length = dev.BytesToRead;
                    data = new byte[length];
                    if (length > 0)
                    {
                        dev.Read(data, 0, length);
                        dev.DiscardInBuffer();
                        //如果接收到的数据长度为1，值为E5H (229)，则为确认码，返回读写确认命令
                        if (data[0] == 0x68 && data.Length == 28 && data[27] == 0x16)
                        {
                            StringBuilder ret = new StringBuilder("0000");
                            if ((data[25] & 0x01) != 0)
                                ret[0] = '1';
                            if ((data[25] & 0x02) != 0)
                                ret[1] = '1';
                            if ((data[25] & 0x04) != 0)
                                ret[2] = '1';
                            if ((data[25] & 0x08) != 0)
                                ret[3] = '1';


                            return ret.ToString();
                        }
                    }
                }    
            }
            return "";
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
            try
            {
                dev.Open();
                bInUse = true;
                if (!IsAlive())
                    dev.Close();
            }
            catch
            {
            }
             bInUse = dev.IsOpen;
            
        }
        private bool IsAlive()
        {
            if (!bInUse)
                return false;
            dev.DiscardInBuffer();
            byte[] rdcmd = new byte[1] { 0xa0 };
            dev.Write(rdcmd, 0, 1);
            Thread.Sleep(300);
            return (dev.BytesToRead > 2);
                

        }

        public void SetTemperature(string temp)
        {
            
            if (!bInUse)
                throw new Exception("温度槽未连接");
                
            if(!IsAlive())
                throw new Exception("设定温度槽温度失败");
            
            //protocal 2
            byte[] cmd = new byte[5];
            cmd[0] = 0xb0;
            double dtemp = Math.Abs(Double.Parse(temp));
            int itemp = Convert.ToInt32(Math.Floor(dtemp * 1000.0));
            if (temp[1] == '-')
                cmd[1] = 0x01;
            else
                cmd[1] = 0x00;
            cmd[2] = Convert.ToByte(itemp % 100);
            itemp = (itemp - (itemp % 100)) / 100;
            cmd[3] = Convert.ToByte(itemp % 100);
            itemp = (itemp - (itemp % 100)) / 100;
            cmd[4] = Convert.ToByte(itemp % 100);
            Thread.Sleep(1000);

            dev.Write(cmd, 0, cmd.Length);
            
            Thread.Sleep(1000);
            /* protocal 1
            byte[] cmd = new byte[5];
            cmd[0] = 0xb0;
            double dtemp = Math.Abs(Double.Parse(temp));
            int itemp = Convert.ToInt32(Math.Floor(dtemp * 1000.0));
            String stemp = "000000" + itemp.ToString();
            stemp = stemp.Remove(0,stemp.Length - 6);
            if (temp[1] == '-')
                cmd[1] = 0x01;
            else
                cmd[1] = 0x00;
            cmd[2] = Convert.ToByte(Convert.ToByte(stemp[5] - '0') + Convert.ToByte(stemp[4] - '0') * 16);
            cmd[3] = Convert.ToByte(Convert.ToByte(stemp[3] - '0') + Convert.ToByte(stemp[2] - '0') * 16);
            cmd[4] = Convert.ToByte(Convert.ToByte(stemp[1] - '0') + Convert.ToByte(stemp[0] - '0') * 16);

            dev.Write(cmd, 0, cmd.Length);
             */

/*
            //                                    addr  func  len   sign                     xor   end
            byte[] cmd = new byte[] { 0x05, 0xc1, 0x01, 0x02, 0x04, 0x00, 0x02, 0x03, 0x04, 0xff, 0xAA };
            double dtemp =  Double.Parse(temp);
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
 */
        }
 
    }
  
    enum ONOFFSTATE
    {
        ON,
        OFF
    }
    class SerialPortOven
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

    class DQ6622A
    {
        internal string id;
        internal bool bInUse;
        private GPIBDevice dev;
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
                     response = dev.Read();
                     if (j-- < 0)
                         break;
                 }
                 if (response != "")
                     return response;
             }
             throw new Exception("与电桥通讯失败");
         }
         public Decimal Resistance
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
         public bool bPowerX2
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
         public bool bInMeasure
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
