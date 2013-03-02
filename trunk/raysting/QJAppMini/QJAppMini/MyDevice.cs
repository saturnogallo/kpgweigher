using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NationalInstruments.NI4882;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;
using System.Data;
using System.Threading;
using System.Text.RegularExpressions;
namespace QJAppMini
{
    enum DeviceType
    {
        GPIB_DEVICE,
        SERIAL_DEVICE
    }
    public delegate void StrEventHandler(object sender, string e);
    abstract class  MyDevice
    {
        protected string address;
        protected DeviceType dvctype;
        internal abstract bool Open();
        internal abstract void Close();
        internal abstract bool IsConnected();
        internal abstract void Refresh();
        internal abstract void ClearInBuffer();
        internal abstract void Send(string line,bool FlushAndEnd);
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
                string[] inputs = port.Split(new char[]{':'});
                PRIMARY_ADDR_OF_PPS = Int32.Parse(inputs[2]);

				//create the formatted io object
                bConnected = false;
                dvctype = DeviceType.GPIB_DEVICE;
                Response = new byte[ARRAYSIZE];
                ResetResponse();
                
			}
			catch(SystemException ex)
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
                foreach (char i in line.ToCharArray())
                {
                    li.ibwrt(Dev, i.ToString());
                    Thread.Sleep(5);
                    if ((li.ibsta & c.ERR) != 0)                          // throw an error 
                    {
                        MessageBox.Show("命令发送失败");
                        return;
                    }
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
            if (IsConnected())
            {
                try
                {
                    while (true)
                    {
                        int ret = li.ibrd(Dev, Response);
                        int asize = li.ibcnt;
                        int size = li.ibcntl;
                        
                        
                        if (Response[0] != 0x00)
                        {
                            string t = "";
                            for(int i=0;i<Response.Length;i++)
                            {
                                if (Response[i] == 0x00)
                                    break;
                                try
                                {

                                    char l = Convert.ToChar(Response[i]);
                                    if (i == 0)
                                        Response[0] = 0x00; //clear flag

                                    t = t + l;
                                    if (l.ToString().IndexOfAny(filter) < 0)
                                    {
                                        Util.SysLog("invalid char" + l.ToString());
                                        throw new Exception("invalid char" + l.ToString());
                                    }
                                    DataRecieved(this, l.ToString());
                                }catch{
                                    Util.SysLog("ECHO");
                                    Thread.Sleep(200);
                                    Send("ECHO\r\n",true);
                                    break;
                                }
                            }
                            Util.SysLog("Rcv:"+t);
                            ResetResponse();
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                catch(Exception ex)
                {
                    Util.SysLog("GPIB Refresh:" + ex.Message + ex.StackTrace);
                    ResetResponse();
                }
            }
            
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
                foreach(char l in serial.ReadExisting().ToCharArray())
                    DataRecieved(this, l.ToString());
            }
        }
        internal override void ClearInBuffer()
        {
            if(IsConnected())
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
            if (IsConnected())
            {
                serial.WriteLine(line);
            }
        }
    }

    enum ScannerType
    {
        RT2010_MI,
        GUIDLINE
    }
    
    class Scanner
    {
        MyDevice dev;
        ScannerType stype;
        string id;
        public bool bInUse;
        public Scanner(string section)
        {
            id = section;
            bInUse = (Util.ConstIni.IntValue(id, "InUse") == 1);
            if (Util.ConstIni.StringValue(id, "Type") == "GPIB")
                dev = new GPIBDevice("GPIB0::" + Util.ConstIni.StringValue(id, "Address") + "::INSTR");
            else
                dev = new SerialDevice("COM" + Util.ConstIni.StringValue(id, "Address"), 9600, System.IO.Ports.Parity.None, 8, System.IO.Ports.StopBits.One);

            stype = (ScannerType)Util.ConstIni.IntValue(id, "Model");
        }
        public bool Open()
        {
            if (!bInUse)
                return true; //in simulation mode
            return dev.Open();
        }
        public void SwitchTo(int port, bool Bmode)
        {
            if (!bInUse)
                return;
            if(stype == ScannerType.RT2010_MI)
            {
                if(Bmode == true)
                    dev.Send(String.Format("{0}B\r\n",port.ToString()),true);
                else
                    dev.Send(String.Format("{0}A\r\n",port.ToString()),true);
                return;
            }
            if(stype == ScannerType.GUIDLINE)
            {
                if(Bmode == true)
                    dev.Send(String.Format("B{0}\r\n",port.ToString("D2")),true);
                else
                    dev.Send(String.Format("A{0}\r\n",port.ToString("D2")),true);
                return;
            }
        }
        public void Reset()
        {
            if (!bInUse)
                return;

            if(stype == ScannerType.RT2010_MI)
            {
                dev.Send("*\r\n",true);
            }
            dev.ClearInBuffer();
        }
    }
    class QJ55A
    {
        public MyDevice dev;
        public StrEventHandler DataRecieved;
        private StringBuilder inbuffer;
        private static string id;
        public QJ55A(string section)
        {
            id = section;
            if (Util.ConstIni.StringValue(id, "Type") == "GPIB")
                dev = new GPIBDevice("GPIB0::" + Util.ConstIni.StringValue(id, "Address") + "::INSTR");
            else
                dev = new SerialDevice("COM" + Util.ConstIni.StringValue(id, "Address"), 9600, System.IO.Ports.Parity.None, 8, System.IO.Ports.StopBits.One);
            
            inbuffer = new StringBuilder(100);
            dev.DataRecieved += new StrEventHandler(ParseData);
        }
        private Regex reg_measure = new Regex("D([0-9]+) Measurements:");
        private Regex reg_rinput = new Regex("R([0-9.eE\\+\\-]+)");
        private Regex reg_sinput = new Regex("S([0-9.eE\\+\\-]+)");
        private Regex reg_index = new Regex("#([0-9]+)");
        private Regex reg_value = new Regex("\\&([0-9.eE\\+\\-]+)");

        public Decimal myRs;
        public Decimal myRx;
        public int myIdx;
        public int myTotal;
        public Decimal myValue;

        public bool Open()
        {
            return dev.Open();
        }
        private void ParseData(object sender, string inchar)
        {
            if(inchar.Length <= 0)
                return;
            inbuffer.Append(inchar);
            if (inchar[0] == '\r' || inchar[0] == '\n')
            {
                string line = inbuffer.ToString();
                Match m = reg_measure.Match(line);
                if (m.Success)
                {
                    myTotal = Int32.Parse(m.Groups[1].Value);
                }
                m = reg_rinput.Match(line);
                if (m.Success)
                {
                    myRx = Convert.ToDecimal(Double.Parse(m.Groups[1].Value));
                }
                m = reg_sinput.Match(line);
                if (m.Success)
                {
                    myRs = Convert.ToDecimal(Double.Parse(m.Groups[1].Value));
                }
                m = reg_index.Match(line);
                if (m.Success)
                {
                    myIdx = Int32.Parse(m.Groups[1].Value);
                }
                m = reg_value.Match(line);
                if (m.Success)
                {
                    myValue = Convert.ToDecimal(Double.Parse(m.Groups[1].Value));
                    DataRecieved(this, myIdx.ToString());
                }
                inbuffer.Remove(0, inbuffer.Length);
            }
        }
        public void Refresh()
        {
            dev.Refresh();
        }
        public void Stop()
        {
            dev.Send("S\r\n", true);
            dev.ClearInBuffer();
        }
        public void Reset()
        {
            dev.Send("H\r\n", true);
            dev.ClearInBuffer();
        }
        public string ConvertCurrent(bool sqrt, Decimal rx)
        {
            Double rname = Convert.ToDouble(rx);
            if (rname > 10240) //100k, 0.1mA
            {
                return "0.1";
            }
            else if (rname > 1024) //10k, 1mA
            {
                return "1";
            }
            else if (rname > 102.4) //1k, 1mA
            {
                if(!sqrt)
                    return "1";
                else
                    return "2";
            }
            else if (rname > 10.24) //100, 10mA
            {
                if (!sqrt)
                    return "10";
                else
                    return "20";
            }
            else if (rname > 1.024) //10, 10mA
            {
                if (!sqrt)
                    return "10";
                else
                    return "20";
            }
            else if (rname > 0.1024) //1, 100mA
            {
                if (!sqrt)
                    return "100";
                else
                    return "200";
            }
            else if (rname > 0.05) //0.1, 0.3A
            {
                if (!sqrt)
                    return "300";
                else
                    return "600";
            }
            else if (rname > 0.005) //0.01, 1A
            {
                if (!sqrt)
                    return "1000";
                else
                    return "2000";
            }
            else  //0.001, 5A
            {
                if (!sqrt)
                    return "5000";
                else
                    return "10000";
            }
        }
        public void SendParameter(DataRow mydr,DataRow rsdr)
        {
            //colname 
            //"RsValue",   "RsSerial", "RsChannel","RxValue", "RxSerial", 
            //"RxChannel","RxCurrent","SampleTime","SwitchTime","Alpha", "Beta","Temp","MeasTime", "Delay",
            //"FilterLen" };
            //so far no use: dev.Send("G0\r\n");
            dev.Send(String.Format("J{0}\r\n", mydr["FilterLen"].ToString()), true);
            //so far no use:dev.Send(String.Format("F{0}",mydr[""].ToString()),true);
            dev.Send(String.Format("I{0}\r\n", ConvertCurrent((bool)mydr["RxCurrent"],(Decimal)mydr["RxValue"])), true);
            dev.Send(String.Format("B{0}\r\n", mydr["RxValue"].ToString()), true);
            dev.Send(String.Format("T{0}\r\n", mydr["Delay"].ToString()), true);
            dev.Send(String.Format("D{0}\r\n", mydr["SampleTime"].ToString()), true);
            dev.Send(String.Format("W{0}\r\n", mydr["SwitchTime"].ToString()), true);
            dev.Send(String.Format("ALPHA{0}\r\n", rsdr["Alpha"].ToString()), true);
            dev.Send(String.Format("BETA{0}\r\n", rsdr["Beta"].ToString()), true);
            dev.Send(String.Format("TEMP{0}\r\n", mydr["Temp"].ToString()), true);
            dev.Send(String.Format("RS_{0}\r\n", rsdr["RsSerial"].ToString()), true);
            dev.Send(String.Format("RX_{0}\r\n", mydr["RxSerial"].ToString()), true);
            dev.Send(String.Format("M{0}\r\n", mydr["MeasTime"].ToString()), true);
            dev.Send(String.Format("A{0}\r\n", rsdr["RsValue"].ToString()), true);
            
        }
        public void StartRun(int meas)
        {
            dev.Send(String.Format("D{0}Measurements:\r\n", meas.ToString()), true);
        }
    }
}
