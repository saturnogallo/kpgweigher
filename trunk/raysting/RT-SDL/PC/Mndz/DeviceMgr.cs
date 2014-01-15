using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.IO.Ports;
using System.Text.RegularExpressions;
using System.Runtime.InteropServices;
using System.Threading;
using System.Diagnostics;

namespace Mndz
{
    static class  DeviceMgr
    {
        static public AutoResetEvent WaitEvent; //flag for wait response, when flag is on,node agent can process the command else node agent cannot do that
        static public AutoResetEvent OverEvent; //flag for process done event.
        static public SerialPort port;  //relay board and DA will shared port
        static public SerialPort cmdport; //all command from PC will use the port
        //static public SerialPort adport; //AD board (weighing board)
        static public SerialPort ad2port; //AD board (weighing board)

        static public double reading;
        static public double ad2reading;
        static private StringBuilder inbuffer;
        static private StringBuilder cmdbuffer;
        static private StringBuilder adbuffer;
        static private StringBuilder ad2buffer;
        static public void Dummy()
        {

        }
        static DeviceMgr()
        {
            #region relay definition
            /*
         * ON OFF 
        40 41  REG21
        42 43  REG22
        13 1A  REG23
        1E 17  REG24
    
        11 18  REG11
        31 38  REG12
        30 39  REG13
        22 2B  REG14

        05 0C  REG61
        04 0D  REG62
        07 0E  REG63
        06 0F  REG64

        32 3B  REG51
        21 28  REG52
        33 3A  REG53
        02 0B  REG54

        10 19  REG31
        00 09  REG32
        12 1B  REG33
        01 08  REG34

        25 2C  REG41
        24 2D  REG42
        27 2E  REG43
        26 2F  REG44
         */
            regmap = new Dictionary<string, byte>();
            byte[]  all_relay = new byte[]{
                0x40,0x41,0x42,0x43,0x13,0x1A,0x1E,0x17,
                0x11,0x18,0x31,0x38,0x30,0x39,0x22,0x2B,
                0x05,0x0C,0x04,0x0D,0x07,0x0E,0x06,0x0F,
                0x32,0x3B,0x21,0x28,0x33,0x3A,0x02,0x0B,
                0x10,0x19,0x00,0x09,0x12,0x1B,0x01,0x08,
                0x25,0x2C,0x24,0x2D,0x27,0x2E,0x26,0x2F};
            string[] abbr_relay = {"REG21",     "REG22:TOREAL", "REG23:OUT",    "REG24", 
                                   "REG11",     "REG12:10T",    "REG13:100T",   "REG14",
                                   "REG61:R13", "REG62:R14",    "REG63:R15",    "REG64",
                                   "REG51:R9",  "REG52:R10",    "REG53:R11",    "REG54:R12",
                                   "REG31:R1",  "REG32:R2",     "REG33:R3",     "REG34:R4",
                                   "REG41:R5",  "REG42:R6",     "REG43:R7",     "REG44:R8"
                                  };
            int i;
            for (i = 0; i < abbr_relay.Length; i++)
            {
                string abbr = abbr_relay[i];
                
                string[] abbrs;
                if (abbr.IndexOf(':') > 0)
                {
                    abbrs = abbr.Split(new char[] { ':' });
                    regmap[abbrs[0] + "_ON"] = all_relay[2 * i];
                    regmap[abbrs[0] + "_OFF"] = all_relay[2 * i + 1];
                    regmap[abbrs[1] + "_ON"] = all_relay[2 * i];
                    regmap[abbrs[1] + "_OFF"] = all_relay[2 * i + 1];
                }
                else
                {
                    regmap[abbr + "_ON"] = all_relay[2 * i];
                    regmap[abbr + "_OFF"] = all_relay[2 * i + 1];
                }
            }
            #endregion
            Logger.SysLog("");
            Logger.SysLog(DateTime.Now.ToLongTimeString());
            //Thread.Sleep(3000);


            WaitEvent = new AutoResetEvent(false);
            OverEvent = new AutoResetEvent(false);
            inbuffer = new StringBuilder();
            cmdbuffer = new StringBuilder();
            adbuffer = new StringBuilder();
            ad2buffer = new StringBuilder();
            //ad_rdgbuf = new List<double>();

            if(GlobalConfig.ISDEBUG)
                return;
            #region init switch board port
            port = new SerialPort();
            port.BaudRate = 9600;
            
            port.PortName = GlobalConfig.sSwiPort;
            port.Parity = Parity.None;
            port.DataBits = 8;
            port.StopBits = StopBits.One;
            port.NewLine = "\r";
            
            for (i = 0; i < 10; i++)
            {
                port.Open();
                if (port.IsOpen)
                    break;
                Thread.Sleep(3000);
            }
            if (i >= 10)
                throw new Exception("Failed to open port A");
            #endregion


            #region init cmdport
            cmdport = new SerialPort();
            cmdport.BaudRate = 9600;
            
            cmdport.PortName = GlobalConfig.sCmdPort;
            cmdport.Parity = Parity.None;
            cmdport.DataBits = 8;
            cmdport.StopBits = StopBits.One;
            cmdport.NewLine = "\r";
            cmdport.Open();
            
            if (!cmdport.IsOpen)
            {
                port.Close();
                throw new Exception("Failed to open port B");
            }
            cmdport.DiscardInBuffer();
            cmdport.DataReceived += new SerialDataReceivedEventHandler(cmdport_DataReceived);
            #endregion


            #region init AD2 board port
            ad2port = new SerialPort();
            ad2port.BaudRate = 2400;

            ad2port.PortName = GlobalConfig.sAD2Port;
            ad2port.Parity = Parity.None;
            ad2port.DataBits = 8;
            ad2port.StopBits = StopBits.One;
            ad2port.NewLine = "\r";
            ad2port.Open();

            if (!ad2port.IsOpen)
            {
                cmdport.Close();
                port.Close();
                throw new Exception("Failed to open AD2 port");
            }
            ad2port.DiscardInBuffer();
            
            #endregion
        }

        static void ScanPort2()
        {
            while (ad2port.BytesToRead > 0)
            {
                char data = Convert.ToChar(ad2port.ReadChar());
                if (((data >= '0') && (data <= '9')) ||
                (data == '.') || (data == '-') || (data == '+') || (data == 'e') || (data == 'E'))
                {
                    ad2buffer.Append(Convert.ToChar(data));
                }
                else
                {
                    if (ad2buffer.Length >= 4)
                    {
                        try
                        {
                            ad2reading = Double.Parse(ad2buffer.ToString());
                        }
                        catch
                        {
                            ad2reading = -9999;
                        }
                    }
                    if (ad2buffer.Length > 0)
                        ad2buffer.Remove(0, ad2buffer.Length);
                }
            }
        }
        
        /*protocal of ad weighing board
        //02 26 30 20 31 30 30 34 33 35 30 30 30 30 30 30 0D
        static private int adstate = -1;
        static private int adst_a = 0;

        static public double adreading = -9999;
        static private bool ad_dynamic = false;
        static private bool ad_overload = false;
        static private bool ad_negative = false;
        static private List<double> ad_rdgbuf;

        static void adport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (!adport.IsOpen)
                return;
            while (adport.BytesToRead > 0)
            {
                int idata = adport.ReadChar();
                byte bdata = Convert.ToByte(idata);
                char cdata = Convert.ToChar(idata);

                if (bdata == 0x02) //start flag
                {
                    adstate = 1;
                    continue;
                }
                if (adstate == 1) //state A
                {
                    if ((bdata & 0xF8) != 0x20)
                    {
                        adstate = -1;
                        continue;
                    }
                    adst_a = bdata - 0x20;
                    adstate = 2;
                    continue;
                }
                if (adstate == 2) //state B
                {
                    if((bdata & 0xF0) != 0x30) 
                    {
                        adstate = -1;
                        continue;
                    }
                    ad_dynamic = (bdata & 0x80) != 0x00;
                    ad_overload = (bdata & 0x40) != 0x00;
                    ad_negative = (bdata & 0x20) != 0x00;
                    adstate = 3;
                    continue;
                }
                if(adstate == 3) //state C is 0x20
                {
                    if (bdata == 0x20)
                    {
                        adstate = 4;
                        adbuffer.Remove(0, adbuffer.Length);
                    }
                    else
                        adstate = -1;
                    continue;
                }
                if (adstate == 4)
                {
                    if (bdata != 0x0d)
                    {
                        if (adbuffer.Length < 7)
                        {
                            adbuffer.Append(cdata);
                            if (adbuffer.Length == (8 - adst_a))
                                adbuffer.Append('.');
                        }
                    }
                    else
                    {
                        double val;
                        if ((adbuffer.Length < 6) ||
                            !Util.TryDoubleParse(adbuffer.Remove(6, adbuffer.Length-6).ToString(), out val))
                        {
                            adstate = -1;
                            continue;
                        }

                        if (ad_overload)
                        {
                            ad_rdgbuf.Add(0);
                        }
                        else
                        {
                            if (ad_negative)
                                val = val * -1;
                            ad_rdgbuf.Add(val);
                            if (ad_rdgbuf.Count >= 3)
                            {
                                double delta = ad_rdgbuf.Max() - ad_rdgbuf.Min();
                                if ((delta < 1e-4) || (delta < Math.Abs(ad_rdgbuf.Max())))
                                {
                                    DeviceMgr.adreading = ad_rdgbuf.Average();
                                }
                                ad_rdgbuf.Clear();
                            }
                        }
                        continue;
                    }

                }
            }
        }
        */
        static void cmdport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (!cmdport.IsOpen)
                return;
            while (cmdport.BytesToRead > 0)
            {
                char data = Convert.ToChar(cmdport.ReadChar());
                if ((data == '\r' || data == '\n') && (cmdbuffer.Length > 0))
                {
                    string cmd = cmdbuffer.ToString();
                    Program.mainwnd.Invoke(new Action<string>(Program.mainwnd.pc_cmd), new object[] { cmd });
                    cmdbuffer.Remove(0, cmdbuffer.Length);

                }
                else
                {
                    if (data != '\r' && data != '\n')
                        cmdbuffer.Append(data);
                }
            }
        }
        static private int uvback_state = -1;
        static public bool success;
        static private char P0;
        static private char P1;
        static public void ScanPort()
        {
            while (port.BytesToRead > 0)
            {
                char data = Convert.ToChar(port.ReadChar());
                if (data == 'U')
                {
                    uvback_state = 0;
                    continue;
                }
                if (((data >= '0') && (data <= '9')) ||
                (data == '.') || (data == '-') || (data == '+') || (data == 'e') || (data == 'E'))
                {
                    inbuffer.Append(Convert.ToChar(data));
                }
                else
                {
                    if ((data == 'V') && (uvback_state == 2))
                    {
                        success = (P0 == P1) && (P0 == '\0');
                        uvback_state = -1;
                        continue;
                    }
                    if (uvback_state == 1)
                    {
                        P1 = data;
                        uvback_state = 2;
                        continue;
                    }
                    if (uvback_state == 0)
                    {
                        P0 = data;
                        uvback_state = 1;
                        continue;
                    }
                    if (inbuffer.Length >= 4)
                    {
                        try
                        {
                            reading = Double.Parse(inbuffer.ToString());
                        }
                        catch
                        {
                            reading = -9999;
                        }
                    }
                    if (inbuffer.Length > 0)
                        inbuffer.Remove(0, inbuffer.Length);
                }
            }
        }

        
        static private void DelayWrite(byte[] buf, int start, int len)
        {
            for (int i = 0; i < len; i++)
            {
                if(!GlobalConfig.ISDEBUG)
                    port.Write(buf, i, 1);
                Thread.Sleep(30);
            }
        }
        static public void Report(string line)
        {
            cmdport.WriteLine(line);
        }

        static public string nav_range2 = "";
        static public void Action2(string action, object param)
        {
            if (GlobalConfig.ISDEBUG)
            {
                success = true;
                return;
            }
            if (action == "navto1v" || action == "navto120mv")
            {
                if (nav_range2 == action)
                    return;
                nav_range2 = action;
                if (action == "navto1v")
                {
                    ad2port.Write(StringResource.str("NAV_1V_" + GlobalConfig.sNavmeter2)); //1v
                }
                if (action == "navto120mv")
                {
                    ad2port.Write(StringResource.str("NAV_120MV_" + GlobalConfig.sNavmeter2)); //120mv
                }
                if (action == "navto10mv")
                {
                    //     ad2port.Write(StringResource.str("NAV_10MV_" + GlobalConfig.sNavmeter2)); //10mv
                }
                if (action == "navto30v")
                {
                    //     ad2port.Write(StringResource.str("NAV_30V_" + GlobalConfig.sNavmeter2)); //10mv
                }
                Thread.Sleep(2000);
                return;
            }
            if (action == "navzero")
            {
                Thread.Sleep(1000);
                ad2port.Write(StringResource.str("NAV_ZEROON_" + GlobalConfig.sNavmeter2)); //120mv
                Thread.Sleep(1000);
                return;
            }
            if (action == "navread")
            {
                ad2reading = -9999;
                ad2port.DiscardInBuffer();
                ad2port.Write(StringResource.str("NAV_READ_" + GlobalConfig.sNavmeter2)); //1v
                Thread.Sleep(10);
                int timeout = 20;
                while ((timeout-- > 0) && (reading < -9000))
                {
                    Thread.Sleep(50);
                    ScanPort2();
                }
                return;
            }
        }
        static public string nav_range = "";


        static public void Action(string action, object param)
        {
            if (GlobalConfig.ISDEBUG)
            {
                success = true;
                return;
            }
            if (action == "navto1v" || action == "navto120mv")
            {
                if (nav_range == action)
                    return;
                nav_range = action;
                if (action == "navto1v")
                {
                    port.Write(StringResource.str("NAV_1V_" + GlobalConfig.sNavmeter)); //1v
                }
                if (action == "navto120mv")
                {
                    port.Write(StringResource.str("NAV_120MV_" + GlobalConfig.sNavmeter)); //120mv
                }
                if (action == "navto10mv")
                {
               //     port.Write(StringResource.str("NAV_10MV_" + GlobalConfig.sNavmeter)); //10mv
                }
                if (action == "navto30v")
                {
               //     port.Write(StringResource.str("NAV_30V_" + GlobalConfig.sNavmeter)); //10mv
                }
                Thread.Sleep(2000);
                return;
            }
            if (action == "navzero")
            {
                Thread.Sleep(1000);
                port.Write(StringResource.str("NAV_ZEROON_" + GlobalConfig.sNavmeter)); //120mv
                Thread.Sleep(1000);
                return;
            }
            if (action == "navread")
            {
                reading = -9999;
                port.DiscardInBuffer();
                port.Write(StringResource.str("NAV_READ_" + GlobalConfig.sNavmeter)); //1v
                Thread.Sleep(10);
                int timeout = 20;
                while ((timeout-- > 0) && (reading < -9000))
                {
                    Thread.Sleep(50);
                    ScanPort();
                }
                return;
            }
            if (action == "daoutput")
            {
                success = false;
                port.DiscardInBuffer();
                byte[] bts = (param as byte[]);
                port.Write(bts, 0, bts.Length); 
                int timeout = 400;
                while ((timeout-- > 0) && (!success))
                {
                    ScanPort();
                    Thread.Sleep(5);
                }
            }
        }
        static public void Reset()
        {
            RelayState("", "", "OFF");
            Action("daoutput", new byte[] { 0x55, 0x64, 0xfc, 0x00, 0x00, 0x00, 0x04 }); //set control
            Action("navto120mv",0);
            Action2("navto120mv", 0);
            Thread.Sleep(3000);
        }

        static private Dictionary<string, byte> regmap;
        static private string oldvolt = ""; //old Es voltage switch satus
        static private string olddivider = "";  //old resistance divider
        static private string oldoutput = ""; //old output status
        static private string[] volttbl = new string[] { "VOLT_1V", "VOLT_5V", "VOLT_10V", "VOLT_20V" };
        static private string[] divtbl = new string[] { "DIV_100M", "DIV_10M", "DIV_1M", "DIV_100K", "DIV_10K", "DIV_1K", "DIV_100", "DIV_10" };
        static private string[] outputtbl = new string[] { "ON", "OFF" };
        static private byte[] UsHead = new byte[] { Convert.ToByte('U'), Convert.ToByte('s'), Convert.ToByte('U'), Convert.ToByte('U') };
        static private byte[] UdHead = new byte[] { Convert.ToByte('U'), Convert.ToByte('d') };
        static private byte[] UsTail = new byte[] { Convert.ToByte('V') };

        //output: ON/OFF
        static public void RelayState(string volt, string divider, string output)
        {
            if (((volt == oldvolt) || (volt == "")) &&
                ((divider == olddivider) || (divider == "")) &&
                ((output == oldoutput) || (output == "")))
                return;
            DelayWrite(UsHead, 0, 4);

            if (volttbl.Contains(volt))
            {
                if (volt == "VOLT_1V")
                    DelayWrite(new byte[] { regmap["10T_OFF"], regmap["100T_OFF"], regmap["TOREAL_OFF"] }, 0, 3);
                if (volt == "VOLT_5V")
                    DelayWrite(new byte[] { regmap["10T_ON"], regmap["100T_OFF"],  regmap["TOREAL_OFF"] }, 0, 3);
                if (volt == "VOLT_10V")
                    DelayWrite(new byte[] { regmap["10T_OFF"], regmap["100T_ON"],  regmap["TOREAL_OFF"] }, 0, 3);
                if (volt == "VOLT_20V")
                    DelayWrite(new byte[] { regmap["10T_OFF"], regmap["100T_OFF"], regmap["TOREAL_ON"] }, 0, 3);
                oldvolt = volt;
            }
            if (divtbl.Contains(divider))
            {
                DelayWrite(new byte[] { regmap["OUT_" + output] }, 0, 1);
                olddivider = divider;
            }
            if (outputtbl.Contains(output))
            {
                DelayWrite(new byte[] { regmap["OUT_"+output] }, 0, 1);
                oldoutput = output;
            }
            
            DelayWrite(UsTail, 0, 1);
        }
    }
    internal class Processor
    {
        private int IND_ES_1V = 0;
        private int IND_ES_5V = 1;
        private int IND_ES_10V = 2;
        private int IND_ES_20V = 3;
        private const int IND_ES_TOTAL = 4;
        private int IND_RS_3 = 0;
        private int IND_RS_4 = 1;
        private int IND_RS_5 = 2;
        private int IND_RS_6 = 3;
        private int IND_RS_7 = 4;
        private int IND_RS_8 = 5;
        private int IND_RS_9 = 6;
        private int IND_RS_10 = 7;
        private int IND_RS_11 = 8;
        private int IND_RS_12 = 9;
        private int IND_RS_BASE = 3;
        private const int IND_RS_TOTAL = 10;


        private Decimal[] _RxTables = new Decimal[] { 1e+2M, 1e+3M, 1e+4M, 1e+5M, 1e+6M, 1e+7M, 1e+8M, 1e+9M, 1e+10M, 1e+11M, 1e+12M, 1e+13M, 1e+14M };
        public static string[] _RxTitles = new string[] {"100 ", "1k  ", "10k ", "100k", "1M  ", "10M ", "100M", "1G  ", "10G ", "100G", "1T  ","10T ","100T" };
        
        private Decimal[] _VsTables = new Decimal[] { 1, 10, 10, 100, 100, 200, 500, 500, 500, 500, 500, 1000, 1000 }; //multiply for voltage

        private int[] _EsTables; //index to values
        private Decimal[] _EsValues = new Decimal[IND_ES_TOTAL];
        public static string[] _EsTitles = new string[] { "1V", "5V", "10V", "20V" };

        private int[] _RsTables; //index to values
        private Decimal[] _RsValues = new Decimal[IND_RS_TOTAL];
        public static string[] _RsTitles = new string[] { "1k  ", "10k ", "100k", "1M  ","10M ","100M","1G  ","10G ","100G","1T  " };

        public Decimal EsValue
        {
            get
            {
                if (EsIndex >= 0 && EsIndex < IND_ES_TOTAL)
                    return _EsValues[EsIndex];
                else
                    return -1;
            }
            set
            {
                if (EsIndex >= 0 && EsIndex < IND_ES_TOTAL)
                {
                    _EsValues[EsIndex] = value;
                    Util.ConstIni.WriteString("LASTSETTING", "esvalue" + EsIndex.ToString(), _EsValues[EsIndex].ToString());
                }
            }
        }
        private int _EsIndex = -1;        //the range setting, Es = 1,2,5,10,20,50 Volt
        internal int EsIndex{
            get
            {
                return _EsIndex;
            }
            private set
            {
                if (value > IND_ES_TOTAL)
                    return;
                if (_EsIndex == value)
                    return;
                bOn = false;
                if (value == -1)
                {
                    _EsIndex = -1;
                }
                else
                {
                    _EsIndex = value;
                }
                Util.ConstIni.WriteString("LASTSETTING", "esindex", _EsIndex.ToString());
            }
        }

        private Decimal _Vx;
        internal Decimal Vx //same da value for output
        {
            get
            {
                return _Vx;
            }
            set
            {
                Vx = value;
            }
        }

        private Decimal _Rx;
        internal Decimal Rx
        {
            get
            {
                double Vg;
                if (!CollectVoltage(out Vg))
                    return -1;
                return Convert.ToDecimal((Convert.ToDouble(Vx) + Vg) * Convert.ToDouble(Rs) / (Convert.ToDouble(EsValue) - Vg));
            }
            set
            {
                if (value <= 0)
                    return;
                
                double dRx = Convert.ToDouble(value);
                for (int i = 0; i < 13; i++)
                {
                    if (Math.Abs(dRx - Math.Pow(10, i + 2)) < 0.5 * Math.Pow(10, i + 2)) //start from 1e+2, 50% variance
                    {
                        _Rx = value;
                        RsIndex = _RsTables[i];
                        EsIndex = _EsTables[i];
                        //Initial Vx
                        Vx = 5;
                    }
                }
            }
        }
        internal Decimal Rs
        {
            get
            {
                if (RsIndex >= 0 && RsIndex < IND_RS_TOTAL)
                    return _RsValues[RsIndex];
                else
                    return -1;
            }
            set
            {
                if (RsIndex >= 0 && RsIndex < IND_RS_TOTAL)
                {
                    _RsValues[RsIndex] = value;
                    if ((Convert.ToDouble(value) - Math.Pow(10, RsIndex + IND_RS_BASE)) > 0.1*Math.Pow(10, RsIndex + IND_RS_BASE)) //too much variance 10%
                        return;
                    Util.ConstIni.WriteString("LASTSETTING", "rsvalue" + RsIndex.ToString() , _RsValues[RsIndex].ToString());
                }
            }
        }
        private int _RsIndex = -1;
        internal int RsIndex
        {
            get
            {
                return _RsIndex;
            }
            private set
            {
                if (value > IND_RS_TOTAL)
                    return;
                if (_RsIndex == value)
                    return;
                bOn = false;
                if (value == -1)
                {
                    _RsIndex = -1;
                }
                else
                {
                    _RsIndex = value;
                }
                Util.ConstIni.WriteString("LASTSETTING", "rsindex", _RsIndex.ToString());
            }
        }

        
        private void SaveDA() //Rescue DA from something external EMS signal
        {
            //sleep 500 ms, then reset configuration
            Thread.Sleep(600);
            DeviceMgr.Action("daoutput", new byte[] { 0x55, 0x64, 0xfc, 0x00, 0x00, 0x00, 0x04 }); //set control
            Thread.Sleep(100);
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
                    DeviceMgr.RelayState("", "", "OFF");
                    _bOn = false;
                    return;
                }
                SaveDA();
                _bOn = true;
            }
        }
        private Decimal _daoffset;
        internal Decimal daoffset  //DA offset
        {
            get
            {
                return _daoffset;
            }
            set
            {
                if (Math.Abs(Convert.ToDouble(value)) > 0.01) //too much offset
                    return;
                _daoffset = value;

                Util.ConstIni.WriteString("LASTSETTING", "daoffset", _daoffset.ToString());

            }
        }

        private Queue<double> datafilter;
        private Queue<double> datafilter2;
        internal Processor()
        {
            _EsTables = new int[] { IND_ES_1V, IND_ES_10V, IND_ES_10V, IND_ES_10V, IND_ES_10V, IND_ES_20V, IND_ES_5V, IND_ES_5V, IND_ES_5V, IND_ES_5V, IND_ES_5V, IND_ES_5V, IND_ES_5V };
            _RsTables = new int[] { IND_RS_3, IND_RS_3, IND_RS_4, IND_RS_4, IND_RS_5, IND_RS_6, IND_RS_6, IND_RS_7, IND_RS_8, IND_RS_9, IND_RS_10, IND_RS_11, IND_RS_12 };

            datafilter = new Queue<double>();
            datafilter2 = new Queue<double>();
            
            _RsIndex = Util.ConstIni.IntValue("LASTSETTING", "rsindex");
            _EsIndex = Util.ConstIni.IntValue("LASTSETTING", "esindex");
            for(int i = 0; i < IND_ES_TOTAL;i++)
                _EsValues[i] = Decimal.Parse(Util.ConstIni.StringValue("LASTSETTING", "esvalue"+i.ToString()));

            for (int i = 0; i < IND_RS_TOTAL; i++)
                _RsValues[i] = Decimal.Parse(Util.ConstIni.StringValue("LASTSETTING", "rsvalue" + i.ToString()));
            
            _daoffset = Decimal.Parse(Util.ConstIni.StringValue("LASTSETTING", "daoffset"));
            if (Math.Abs(Convert.ToDouble(_daoffset)) > 0.0001) //100uV
                _daoffset = 0;
            
            bOn = false;
        }
        public void RefreshOutput()
        {
            //TODO
            if (!bOn)
            {
                //UpdateIndicatorOnly();
            }
            else
            {
                //UpdateVx();
            }
        }
        private bool CollectVoltage(out double reading)
        {
            int badcount = 0; //count of bad communication
            reading = 0;
            
            while (badcount < 3)
            {
                DeviceMgr.Action("navread", 0);
                if (DeviceMgr.reading < -999)
                {
                    if (DeviceMgr.nav_range != "navto120mv")
                    {
                        badcount++;
                        if (badcount < 3)
                        {
                            Thread.Sleep(500);
                            continue;
                        }
                        badcount = 0;
                        DeviceMgr.Action("navto120mv", "");
                        Thread.Sleep(3000);
                        continue;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                }
                //disable range change
                if (false && (DeviceMgr.nav_range == "navto1v") && (Math.Abs(DeviceMgr.reading) < 0.1) && bOn)
                {
                    badcount++;
                    if (badcount < 3)
                    {
                        Thread.Sleep(500);
                        continue;
                    }

                    badcount = 0;
                    DeviceMgr.Action("navto120mv", "");
                    Thread.Sleep(3000);
                    continue;
                }
                if (DeviceMgr.nav_range == "navto120mv")
                    datafilter.Enqueue(DeviceMgr.reading / 1000);
                else
                    datafilter.Enqueue(DeviceMgr.reading);
                if (datafilter.Count < 5)
                    continue;
                double sqr;
                sqr = GetSqrt3(10,datafilter);
                datafilter.Dequeue();
                if ((DeviceMgr.nav_range == "navto120mv") && (sqr > 0.05)) //1mV
                {
                    badcount++;
                    continue;
                }
                if ((DeviceMgr.nav_range == "navto1v") && (sqr > 0.05)) //10mv
                {
                    badcount++;
                    continue;
                }
                badcount = 0;
                reading = datafilter.Skip(2).Take(2).Average();
                return true;
            }
            return false;
        }
        private bool CollectVoltage2(out double reading)
        {
            int badcount = 0; //count of bad communication
            reading = 0;

            while (badcount < 3)
            {
                DeviceMgr.Action2("navread", 0);
                if (DeviceMgr.ad2reading < -999)
                {
                    if (DeviceMgr.nav_range2 != "navto120mv")
                    {
                        badcount++;
                        if (badcount < 3)
                        {
                            Thread.Sleep(500);
                            continue;
                        }
                        badcount = 0;
                        DeviceMgr.Action2("navto120mv", "");
                        Thread.Sleep(3000);
                        continue;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                }
                //disable range change
                if (false && (DeviceMgr.nav_range2 == "navto1v") && (Math.Abs(DeviceMgr.ad2reading) < 0.1) && bOn)
                {
                    badcount++;
                    if (badcount < 3)
                    {
                        Thread.Sleep(500);
                        continue;
                    }

                    badcount = 0;
                    DeviceMgr.Action2("navto120mv", "");
                    Thread.Sleep(3000);
                    continue;
                }
                if (DeviceMgr.nav_range2 == "navto120mv")
                    datafilter2.Enqueue(DeviceMgr.ad2reading / 1000);
                else
                    datafilter2.Enqueue(DeviceMgr.ad2reading);
                if (datafilter2.Count < 5)
                    continue;
                double sqr;
                sqr = GetSqrt3(10, datafilter2);
                datafilter2.Dequeue();
                if ((DeviceMgr.nav_range2 == "navto120mv") && (sqr > 0.05)) //1mV
                {
                    badcount++;
                    continue;
                }
                if ((DeviceMgr.nav_range2 == "navto1v") && (sqr > 0.05)) //10mv
                {
                    badcount++;
                    continue;
                }
                badcount = 0;
                reading = datafilter2.Skip(2).Take(2).Average();
                return true;
            }
            return false;
        }            
        private static double GetSqrt3(double delta, Queue<double> datas)
        {
            double avg = datas.Average();
            double sqr = 0;
            foreach (double v in datas)
            {
                sqr = sqr + v * (v - avg);
            }
            sqr = Math.Sqrt(sqr / (datas.Count - 1));
            if (Math.Abs(sqr / avg) < 5e-9) //0.05ppm
                sqr = avg * 5e-9;
            else if (Math.Abs(sqr) < 1e-9) //10nV
                sqr = 1e-9;
            return sqr * delta;
        }

        public void ZeroON()
        {
            int i = 0;
            while (i++ < 5)
            {
                double va;
                if (CollectVoltage(out va))
                {
                    if (Math.Abs(va) < 0.00001) // < 10uV
                        DeviceMgr.Action("navzero", 0);
                    break;
                }
                if (CollectVoltage2(out va))
                {
                    if (Math.Abs(va) < 0.00001) // < 10uV
                        DeviceMgr.Action2("navzero", 0);
                    break;
                }
                System.Threading.Thread.Sleep(1000);
            }
        }

        private bool UpdateVx(Decimal resistance)
        {
            //TODO update Voltage output
            if (true) //real res case
            {
                ToDAValue(0);
                //DeviceMgr.Action("daoutput", new Byte[] { 0x55, 0x64, 0xff, 0x00, 0x00, 0x00, 0x01 }); //just set to 0
                return DeviceMgr.success;
            }
            double va;
            if(!CollectVoltage(out va))
                return false;
            double volt = Convert.ToDouble(resistance);
        
            return ToDAValue(volt);
        }
        private byte[] lasttosend = new byte[] { 0x00,0x00,0xff, 0xff, 0xff, 0x00 };
        //0 to 10 V setting
        private bool ToDAValue(double voltage)
        {
            // Vout = (Vrefp-Vrefn)*D/(2^20-1)+Vrefn =>  D= (Vout-Vrefn)*(2^20-1)/(Vrefp-Vrefn)
            // when BUF is enabled , Vrefp = 10V;  Vrefn = 0V; D = (Vout)*(2^20-1)/(10)
            // D = Vout*(2^20-1)/10;
            byte[] tosend = new Byte[] { 0x55, 0x64, 0xff, 0x00, 0x00, 0x00, 0x01 };
            byte[] tosend2 = new Byte[] { 0x55, 0x64, 0x55, 0xff, 0x00, 0x00, 0x00, 0x01 };//special case for 0x55 leading value

            bool changed = false;

            double volt = voltage - Convert.ToDouble(daoffset);
            if (volt > 10 || volt < 0) //turn off output or invalid ad reading
                return false;

            Int32 d = Convert.ToInt32(Math.Round((volt) * (1048576 - 1) / 10.0));
            tosend[5] = Convert.ToByte(d % 256); d = d / 256;
            tosend2[6] = tosend[5];
            if (tosend[5] != lasttosend[5])
            {
                lasttosend[5] = tosend[5];
                changed = true;
            }
            tosend[4] = Convert.ToByte(d % 256);
            tosend2[5] = tosend[4];
            d = d / 256;
            if (tosend[4] != lasttosend[4])
            {
                lasttosend[4] = tosend[4];
                changed = true;
            }
            tosend[3] = Convert.ToByte(d % 256);
            tosend2[4] = tosend[3];
            if (tosend[3] != lasttosend[3])
            {
                lasttosend[3] = tosend[3];
                changed = true;
            }
            tosend[2] = Convert.ToByte((256 * 3 - 1 - Convert.ToInt32(tosend[3] + tosend[4] + tosend[5])) % 256);
            tosend2[3] = tosend[2];
            if (changed)
            {
                if (tosend[2] == 0x55)
                    DeviceMgr.Action("daoutput", tosend2);
                else
                    DeviceMgr.Action("daoutput", tosend);
                return DeviceMgr.success;
            }
            else
            {
                return true;
            }
        }
        
    }
}
