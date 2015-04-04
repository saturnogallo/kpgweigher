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

namespace Mndz7
{
    static class  DeviceMgr
    {
        static public AutoResetEvent WaitEvent; //flag for wait response, when flag is on,node agent can process the command else node agent cannot do that
        static public AutoResetEvent OverEvent; //flag for process done event.
        static public SerialPort port;  //relay board and DA will shared port
        static public SerialPort cmdport; //all command from PC will use the port
        static public SerialPort adport; //AD board (weighing board)

        static public double reading;
        static private StringBuilder inbuffer;
        static private StringBuilder cmdbuffer;
        static private StringBuilder adbuffer;
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
            ad_rdgbuf = new List<double>();

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

/*
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
 */
/*
            #region init AD board port
            adport = new SerialPort();
            adport.BaudRate = 2400;

            adport.PortName = GlobalConfig.sADPort;
            adport.Parity = Parity.None;
            adport.DataBits = 8;
            adport.StopBits = StopBits.One;
            adport.NewLine = "\r";
            adport.Open();

            if (!adport.IsOpen)
            {
                cmdport.Close();
                port.Close();
                throw new Exception("Failed to open port C");
            }
            adport.DiscardInBuffer();
            adport.DataReceived += new SerialDataReceivedEventHandler(adport_DataReceived);
            #endregion
 */
        }

        //protocal of ad weighing board
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
        static public string nav_range = "";
        static byte[] togglecmd = new byte[] { 0x55,  0x73,  0x55, 0x55, 0x02, 0x56  };
        static public int xstate = 1; //can be 1 or 10;
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
            if (action == "togglex")
            {
                //55 73 55 55 02 56 
                success = false;
                port.DiscardInBuffer();
                port.Write(togglecmd, 0, togglecmd.Length);
                int timeout = 400;
                while ((timeout-- > 0) && (!success))
                {
                    ScanPort();
                    Thread.Sleep(5);
                }
                if (success)
                {
                    if (xstate == 1)
                        xstate = 10;
                    else
                        xstate = 1;
                }
            }
        }
        static public void Reset()
        {
            RelayState("", "", "OFF");
            Action("daoutput", new byte[] { 0x55, 0x64, 0xfc, 0x00, 0x00, 0x00, 0x04 }); //set control
            Action("navto120mv",0);
            Thread.Sleep(3000);
        }

        static private Dictionary<string, byte> regmap;
        static private string oldcoil = ""; //old coil switch satus
        static private string oldreal = "";  //old real resistance
        static private string oldoutput = ""; //old output status
        static private string[] coiltbl = new string[] { "COIL_10T", "COIL_100T", "COIL_1T", "COIL_REAL" };
        static private string[] outputtbl = new string[] { "ON", "OFF" };
        static private byte[] UsHead = new byte[] { Convert.ToByte('U'), Convert.ToByte('s'), Convert.ToByte('U'), Convert.ToByte('U') };
        static private byte[] UdHead = new byte[] { Convert.ToByte('U'), Convert.ToByte('d') };
        static private byte[] UsTail = new byte[] { Convert.ToByte('V') };

        static public void RelayState(string coil, string real, string output)
        {
            if (((coil == oldcoil) || (coil == "")) &&
                ((real == oldreal) || (real == "")) &&
                ((output == oldoutput) || (output == "")))
                return;
            DelayWrite(UsHead, 0, 4);

            if (coiltbl.Contains(coil))
            {
                if (coil == "COIL_1T")
                    DelayWrite(new byte[] { regmap["10T_OFF"], regmap["100T_OFF"], regmap["TOREAL_OFF"] }, 0, 3);
                if (coil == "COIL_10T")
                    DelayWrite(new byte[] { regmap["10T_ON"], regmap["100T_OFF"],  regmap["TOREAL_OFF"] }, 0, 3);
                if (coil == "COIL_100T")
                    DelayWrite(new byte[] { regmap["10T_OFF"], regmap["100T_ON"],  regmap["TOREAL_OFF"] }, 0, 3);
                if (coil == "COIL_REAL")
                {
                    oldreal = real;
                    DelayWrite(new byte[] { regmap["10T_OFF"], regmap["100T_OFF"], regmap["TOREAL_ON"] }, 0, 3);
                }
                oldcoil = coil;
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
        internal int RangeMin = 0;
        internal int RangeMax = 6;
        internal double RangeLimit
        {
            get{
                
                switch (_iRange)
                {
                    case 0: //200u
                        return 0.0002;
                    case 1: //2m
                        return 0.002;
                    case 2: //20m
                        return 0.02;
                    case 3: //200m
                        return 0.2;
                    case 4: //2
                        return 2;
                    case 5: //4
                        return 4;
                    case 6: //20
                        return 20;
                    default:
                        return -1;
                }
            }
        }
        internal int RangeScale
        {
            get
            {
                switch (_iRange)
                {
                    case 0: //200u
                        return -4;
                    case 1: //2m
                        return -3;
                    case 2: //20m
                        return -2;
                    case 3: //200m
                        return -1;
                    case 4: //2
                        return 0;
                    case 5: //4
                        return 0;
                    case 6: //20
                        return 1;
                    default:
                        return -9;
                }
            }
        }


        internal double OPCurrentOffset = 5.0; 
        internal double OPCurrentScale
        {
            get
            {
                switch (_iRange)
                {
                    case 0: //200u  5V->600A
                        return 600/5;
                    case 1: //2m    5V->200A
                        return 200/5;
                    case 2: //20m   5V->100A
                        return 100/5;
                    case 3: //200m  5V->10A
                        return 10/5;
                    case 4: //2     5V->10A
                        return 10/5;
                    case 5: //4     5V->5A
                        return 5/5;
                    case 6: //20    5V->1A
                        return 1/5;
                    default:
                        return -1;
                }
                return -1;
            }
        }
        internal string OPCurrentCoil
        {
            get
            {
                switch (_iRange)
                {
                    case 0: //200u  5V->600A
                    case 1: //2m    5V->200A
                    case 2: //20m   5V->100A
                        return "COIL_1T";
                    case 3: //200m  5V->10A
                    case 4: //2     5V->10A
                    case 5: //4     5V->5A
                        return "COIL_10T";
                    case 6: //20    5V->1A
                        return "COIL_100T";
                    default:
                        return "";
                }
            }
        }
        internal double FBCurrentScale
        {
            get
            {
                switch (iRange)
                {
                    case 0: //200u  1V->600A
                        return 600;
                    case 1: //2m    1V->200A
                        return 200;
                    case 2: //20m   1V->100A
                        return 100;
                    case 3: //200m  1V->10A
                        return 10;
                    case 4: //2     1V->10A
                        return 10;
                    case 5: //4     1V->5A
                        return 5;
                    case 6: //20    1V->1A
                        return 1;
                    default:
                        return -1;
                }
            }
        }

        private int _iRange = -1;        //the range setting, -1 for null, 0 for 200uohm, 1 for 2mohm, 2 for 20mohm, 3 for 200mohm, 4 for 2ohm, 5 for 4ohm, 6 for 20ohm
        internal int iRange{
            get
            {
                return _iRange;
            }
            set
            {
                if (_iRange == value)
                    return;
                bOn = false;
                iReal = -1;
                if (value == -1)
                {
                    _iRange = -1;
                    resistance = -1;
                }
                else
                {
                    Decimal newresi = Convert.ToDecimal(0); 
                    double b = Math.Abs(Convert.ToDouble(resistance));
            
                    if(value < RangeMin || value > RangeMax)
                        return;
                    _iRange = value;
                    DeviceMgr.RelayState(OPCurrentCoil, "", "");
                    if(b <= RangeLimit)
                         newresi = resistance;
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
                    return Convert.ToDecimal(-9999);
                }
                return _resistance;
            }
            set
            {
                if(iRange < RangeMin || iRange > RangeMax)
                    return;
                if (value < 0)
                {
                    DeviceMgr.RelayState("", "", "OFF");
                    return;
                }
                _resistance = value;
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
                if(iRange < RangeMin || iRange > RangeMax)
                {
                     DeviceMgr.RelayState("COIL_REAL", "ALL_OFF", "");
                }
            }
        }
        private void SaveDA()
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
                if(iRange < RangeMin || iRange > RangeMax)
                {
                      //DeviceMgr.RelayState("", "", "OFF");
                      _bOn = false; //uncertain real case
                      return;
                }
                else
                {
                    if ((RangeLimit < 1) && (DeviceMgr.xstate == 1))
                            DeviceMgr.Action("togglex", "");
                    if ((RangeLimit > 1) && (DeviceMgr.xstate == 10))
                        DeviceMgr.Action("togglex", "");
                    //DeviceMgr.RelayState("", "", "ON");
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

        private Decimal _standard;
        internal Decimal standard  //standard resistance
        {
            get
            {
                return _standard;
            }
            set
            {
                if (Convert.ToDouble(value) < 0.0001) //too small resistance
                    return; 
                _standard = value;

                Util.ConstIni.WriteString("LASTSETTING", "standard", _standard.ToString());

            }
        }

        private Queue<double> datafilter;
        internal Processor()
        {
            datafilter = new Queue<double>();

            _resistance = 0;
            _iRange = 0;
            _standard = 1;
            _daoffset = 0;
            try
            {
                _iReal = Util.ConstIni.IntValue("LASTSETTING", "real");
                _resistance = Decimal.Parse(Util.ConstIni.StringValue("LASTSETTING", "resistance"));
                _standard = Decimal.Parse(Util.ConstIni.StringValue("LASTSETTING", "standard"));
                _daoffset = Decimal.Parse(Util.ConstIni.StringValue("LASTSETTING", "daoffset"));
            }
            catch
            {
            }
            if (Math.Abs(Convert.ToDouble(_daoffset)) > 0.0001) //100uV
                _standard = 0;
            iRange = Util.ConstIni.IntValue("LASTSETTING", "range");
            bOn = false;
        }
        public void RefreshOutput()
        {
            if (iRange < RangeMin || iRange > RangeMax) //invalid res case
                return;
            if (!bOn)
            {
                UpdateCurrentOnly();
            }
            else
            {
                ToResistance(resistance);
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
                    if (DeviceMgr.nav_range != "navto1v")
                    {
                        badcount++;
                        if (badcount < 3)
                        {
                            Thread.Sleep(500);
                            continue;
                        }
                        badcount = 0;
                        DeviceMgr.Action("navto1v", "");
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
                sqr = GetSqrt3(10);
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
                    
        private double GetSqrt3(double delta)
        {
            double avg = datafilter.Average();
            double sqr = 0;
            foreach (double v in datafilter)
            {
                sqr = sqr + v * (v - avg);
            }
            sqr = Math.Sqrt(sqr / (datafilter.Count - 1));
            if (Math.Abs(sqr / avg) < 5e-9) //0.05ppm
                sqr = avg * 5e-9;
            else if (Math.Abs(sqr) < 1e-9) //10nV
                sqr = 1e-9;
            return sqr * delta;
        }
        internal double Current = -9999;
        private bool UpdateCurrentOnly()
        {
            double va;
            if (!CollectVoltage(out va))
                return false;
            if (FBCurrentScale < 0)
                return false;

            
            Current = va * FBCurrentScale;
            
            return true;
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
                System.Threading.Thread.Sleep(1000);
            }
        }
        private bool ToResistance(Decimal resistance)
        {
            if (iRange < RangeMin || iRange > RangeMax) //invalide res case
            {
                ToDAValue(0);
                //DeviceMgr.Action("daoutput", new Byte[] { 0x55, 0x64, 0xff, 0x00, 0x00, 0x00, 0x01 }); //just set to 0
                return DeviceMgr.success;
            }
            double va;
            if(!CollectVoltage(out va))
                return false;
            double volt = Convert.ToDouble(resistance);
            if(iRange < RangeMin || iRange > RangeMax)
                return false;
            
            volt = volt * va * FBCurrentScale;
            Current = va * FBCurrentScale;

            return ToDAValue(volt);
        }
        private byte[] lasttosend = new byte[] { 0x00,0x00,0xff, 0xff, 0xff, 0x00 };
        private bool ToDAValue(double voltage)
        {
            // Vout = (Vrefp-Vrefn)*D/(2^20-1)+Vrefn =>  D= (Vout-Vrefn)*(2^20-1)/(Vrefp-Vrefn)
            // when BUF is enabled , Vrefp = 10V;  Vrefn = -10V; D = (Vout+10)*(2^20-1)/(20)
            // D = Vout*(2^20-1)/10;
            byte[] tosend = new Byte[] { 0x55, 0x64, 0xff, 0x00, 0x00, 0x00, 0x01 };
            byte[] tosend2 = new Byte[] { 0x55, 0x64, 0x55, 0xff, 0x00, 0x00, 0x00, 0x01 };//special case for 0x55 leading value
	    
            bool changed = false;

            double volt = voltage - Convert.ToDouble(daoffset) + OPCurrentOffset;
            if (Math.Abs(volt) > 10) //turn off output or invalid adreading
                return false;

	    if(volt <0)
		volt = 0;
            
            
                Int32 d = Convert.ToInt32(Math.Round((volt+10) * (1048576 - 1) / 20.0));
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
                    if(tosend[2] == 0x55)
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
        internal static int CheckRange(Decimal resi, int nowRange)
        {
            double resistance = Convert.ToDouble(resi);
            if (resistance >= 0)
            {
                if (resistance <= 0.002) //200µohm
                {
                    if (nowRange >= 0)
                        return nowRange;
                    return 0;
                }
                if (resistance <= 0.02) //20mohm
                {
                    if (nowRange >= 1) 
                        return nowRange;
                    return 1;
                }
                if (resistance <= 0.2)  //200mohm
                {
                    if (nowRange >= 2)
                        return nowRange;
                    return 2;
                }
                if (resistance <= 2)    //2ohm
                {
                    if (nowRange >= 3)
                        return nowRange;
                    return 3;
                }
                if (resistance <= 4)    //4 ohm
                {
                    if (nowRange >= 4)
                        return nowRange;
                    return 4;
                }
                if (resistance <= 20)    //20 ohm
                {
                    if (nowRange >= 5)
                        return nowRange;
                    return 5;
                }
            }
            return -1;
        }
    }
}
