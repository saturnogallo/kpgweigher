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
//        static public SerialPort adport; //AD board (weighing board)

        static public double reading;
        static private StringBuilder inbuffer;
        static private StringBuilder cmdbuffer;
        static private StringBuilder adbuffer;
        static public void Dummy()
        {

        }
        static DeviceMgr()
        {
            #region bit relay definition
            bitregmap = new Dictionary<string, byte>();
            byte[] bitall_relay = new byte[]{
                0x40,0x41,0x42,0x43,0x13,0x1A,0x1E,0x17,
                0x11,0x18,0x31,0x38,0x30,0x39,0x22,0x2B,
                0x05,0x0C,0x04,0x0D,0x07,0x0E,0x06,0x0F,
                0x32,0x3B,0x21,0x28,0x33,0x3A,0x02,0x0B,
                0x10,0x19,0x00,0x09,0x12,0x1B,0x01,0x08,
                0x25,0x2C,0x24,0x2D,0x27,0x2E,0x26,0x2F,
                0x01,0x00,0x02,0x00,0x04,0x00,0x08,0x00,  //"BIT00",     "BIT01"   ,     "BIT02",        "BIT03",
                0x10,0x00,0x20,0x00,0x40,0x00,0x80,0x00,  //"BIT04",     "BIT05"   ,     "BIT06",        "BIT07",                                 
                0x00,0x01,0x00,0x02,0x00,0x04,0x00,0x08,  //"BIT08:KTT",       "BIT09:RANGE_5A"   ,     "BIT10:RANGE_10A",        "BIT11:RANGE_100A",                                   
                0x00,0x10,0x00,0x20,0x00,0x40,0x00,0x80,

                0x01,0x00,0x04,0x00,0x10,0x00,0x40,0x00,
                0x00,0x01,0x00,0x04,0x00,0x10,0x00,0x40,
                };
            //REG is for relay board, which use x,y map
            //BIT is the voltage control, which use 1 means on , 0 means off
            //PUL means pulse, just give a pulse on give position
            string[] bitabbr_relay = {"REG21",     "REG22",        "REG23",        "REG24", 
                                   "REG11",     "REG12",        "REG13",        "REG14",
                                   "REG61",     "REG62",        "REG63",        "REG64",
                                   "REG51",     "REG52",        "REG53",        "REG54",
                                   "REG31",     "REG32",        "REG33",        "REG34",
                                   "REG41",     "REG42",        "REG43",        "REG44",
                                   "BIT00",     "BIT01"   ,     "BIT02",        "BIT03",
                                   "BIT04",     "BIT05"   ,     "BIT06",        "BIT07",                                 
                                   "BIT08:KTT",       "BIT09"   ,     "BIT10",        "BIT11",
                                   "BIT12",     "BIT13"   ,     "BIT14",        "BIT15",
                                   "PUL00",     "PUL01"   ,     "PUL02",        "PUL03",
                                   "PUL04",     "PUL05"   ,     "PUL06",        "PUL07",

                                  };
            int i;
            for (i = 0; i < bitabbr_relay.Length; i++)
            {
                string abbr = bitabbr_relay[i];

                string[] abbrs;
                if (abbr.IndexOf(':') > 0)
                {
                    abbrs = abbr.Split(new char[] { ':' });

                    if (abbrs[0].StartsWith("BIT"))
                    {
                        bitregmap[abbrs[1] + "_ON_P0"] = bitall_relay[2 * i];
                        bitregmap[abbrs[1] + "_OFF_P0"] = Convert.ToByte(bitall_relay[2 * i] ^ 0xff);
                        bitregmap[abbrs[1] + "_ON_P1"] = bitall_relay[2 * i + 1];
                        bitregmap[abbrs[1] + "_OFF_P1"] = Convert.ToByte(bitall_relay[2 * i + 1] ^ 0xff);
                    }
                    if (abbrs[0].StartsWith("REG"))
                    {
                        bitregmap[abbrs[1] + "_ON"] = bitall_relay[2 * i];
                        bitregmap[abbrs[1] + "_OFF"] = bitall_relay[2 * i + 1];
                    }
                    if (abbrs[0].StartsWith("PUL"))
                    {
                        bitregmap[abbrs[1] + "_ON_P0"] = bitall_relay[2 * i];
                        bitregmap[abbrs[1] + "_OFF_P0"] = Convert.ToByte(bitall_relay[2 * i] << 1);
                        bitregmap[abbrs[1] + "_ON_P1"] = bitall_relay[2 * i + 1];
                        bitregmap[abbrs[1] + "_OFF_P1"] = Convert.ToByte(bitall_relay[2 * i + 1] << 1);
                    }
                }

            }

            #endregion

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
            /*
             * equivilant in QJ55 and QJ58 W relay
             * 100A:  64, 
             * 200A:  128
             * 300A:  2048
             * 400A:  256
             * 500A:  512
             * 600A:  1024
             * KTT:   32
             */
            byte[]  all_relay = new byte[]{
                0x40,0x41,0x42,0x43,0x13,0x1A,0x1E,0x17,
                0x11,0x18,0x31,0x38,0x30,0x39,0x22,0x2B,
                0x05,0x0C,0x04,0x0D,0x07,0x0E,0x06,0x0F,
                0x32,0x3B,0x21,0x28,0x33,0x3A,0x02,0x0B,
                0x10,0x19,0x00,0x09,0x12,0x1B,0x01,0x08,
                0x25,0x2C,0x24,0x2D,0x27,0x2E,0x26,0x2F};
            string[] abbr_relay = {"REG21:200A",     "REG22:100A",    "REG23:KTT",     "REG24", 
                                   "REG11",     "REG12",    "REG13",     "REG14",
                                   "REG61",     "REG62",    "REG63",     "REG64",
                                   "REG51",     "REG52",    "REG53",     "REG54",
                                   "REG31:300A","REG32:600A",    "REG33:500A",     "REG34:400A",
                                   "REG41",     "REG42",    "REG43",     "REG44"
                                  };
            
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
            #region seperate AD port , not in use now
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
            #endregion
        }
        #region seperate AD port , not in use now
        //protocal of ad weighing board
        //02 26 30 20 31 30 30 34 33 35 30 30 30 30 30 30 0D
        static private int adstate = -1;
        static private int adst_a = 0;

        static public double adreading = -9999;
        static private bool ad_dynamic = false;
        static private bool ad_overload = false;
        static private bool ad_negative = false;
        static private List<double> ad_rdgbuf;
        /*
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
        }*/
        #endregion
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
            RelayState("ALL_OFF");
            Action("daoutput", new byte[] { 0x55, 0x64, 0xfc, 0x00, 0x00, 0x00, 0x04 }); //set control
            Action("navto1v", 0);
            if(!GlobalConfig.ISDEBUG)
                Thread.Sleep(3000);
        }

        static private Dictionary<string, byte> regmap;
        static private Dictionary<string, byte> bitregmap;
        static private string oldcoil = ""; //old coil switch satus
        static private string oldreal = "";  //old real resistance
        static private string oldoutput = ""; //old output status
        //static private string[] coiltbl = new string[] { "COIL_10T", "COIL_100T", "COIL_1T", "COIL_REAL" };
        static private string[] outputtbl = new string[] { "100A", "200A", "300A", "400A", "500A", "600A" };
        static private byte[] UsHead = new byte[] { Convert.ToByte('U'), Convert.ToByte('s'), Convert.ToByte('U'), Convert.ToByte('U') };
        static private byte[] UtHead = new byte[] { Convert.ToByte('U'), Convert.ToByte('t') };
        static private byte[] UdHead = new byte[] { Convert.ToByte('U'), Convert.ToByte('d') };
        static private byte[] UsTail = new byte[] { Convert.ToByte('V') };

        static private string oldktt = ""; //old ktt status
        static public void RelayKTT(string ktt)
        {
            if (Form1.s_scale == "6000") //6000A use direct relay control
            {
                DelayWrite(UtHead, 0, 2);

                Byte P0 = 0x00;
                Byte P1 = 0xf3; //!00001100

                P0 = (byte)(P0 | bitregmap["KTT_"+ktt+"_P0"]);
                P1 = (byte)(P1 | bitregmap["KTT_"+ktt+"_P1"]);

                DelayWrite(new byte[] { P0, P1 }, 0, 2);
                DelayWrite(UsTail, 0, 1);
                Thread.Sleep(100);

                return;
            }

            if ((ktt == oldktt) || (ktt == ""))
                return;
            DelayWrite(UsHead, 0, 4);

            DelayWrite(new byte[] { regmap["KTT_"+ktt] }, 0, 1);
            oldktt = ktt;

            DelayWrite(UsTail, 0, 1);
        }

        static public void RelayState(string output)
        {
            if (Form1.s_scale == "6000") //6000A use direct relay control
                return;

            if((output == oldoutput) || (output == ""))
                return;
            DelayWrite(UsHead, 0, 4);

            DelayWrite(new byte[] { regmap["100A_OFF"], regmap["200A_OFF"], regmap["300A_OFF"], regmap["400A_OFF"] , regmap["500A_OFF"], regmap["600A_OFF"] }, 0, 2);
            if (outputtbl.Contains(output))
            {
                DelayWrite(new byte[] { regmap[output + "_ON"] }, 0, 1);
                oldoutput = output;
            }
            DelayWrite(UsTail, 0, 1);
        }
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
                if (_setting != value)
                {
                    _setting = value;
                    //Util.ConstIni.WriteString("LASTSETTING", "setting", _setting.ToString());
                }
                _track_setting = _setting;
                trackfilter.Clear();
            }
        }
        private void SaveDA()
        {
            //sleep 500 ms, then reset configuration
            DeviceMgr.Action("daoutput", new byte[] { 0x55, 0x64, 0xfc, 0x00, 0x00, 0x00, 0x04 }); //set control
            Thread.Sleep(100);
        }
        private bool _bKTT;
        internal bool bKTT
        {
            get
            {
                return _bKTT;
            }
            set
            {
                _bKTT = value;
                if (_bKTT)
                    DeviceMgr.RelayKTT("ON");
                else
                    DeviceMgr.RelayKTT("OFF");
                //TODO set relay;
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
                    DeviceMgr.RelayState("ALL_OFF");
                    ToDAValue(0);
                    _bOn = false;
                    return;
                }
                SaveDA();
                /*
                10V - 600A 
                10V - 1000A (10，100，1000A)
                端子不同(1A ,10A, 100A, 0-10V
	            300A, (并继电器)
	            600A，(并继电器)
	            )

                0.6V->600A
                1V->1000A
                1V->6000A
                 */
                if (Form1.scale_vals.Contains(Form1.s_scale)) //update relay state
                {
                    DeviceMgr.RelayState(Form1.s_scale + "A");
                }
                else
                {
                    DeviceMgr.RelayState("ALL_OFF");
                }
                _track_setting = _setting;
                trackfilter.Clear();
                ToVoltage(_setting);                

                _bOn = true;
            }
        }
        private Double _adscale;
        internal Double adscale  //AD scale for calibration
        {
            get
            {
                return _adscale;
            }
            set
            {
                if (Math.Abs(Convert.ToDouble(value -1 )) > 0.01) //too much variance
                    return;

                if (_adscale != value)
                {
                    _adscale = value;
                    Util.ConstIni.WriteString("LASTSETTING", "adscale", _adscale.ToString());
                }
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
                if (Math.Abs(Convert.ToDouble(value)) > 0.1) //too much offset //change from 0.01 to 0.1 because for 6000A the offset could be bigger, 2015-07-27 song,jie
                    return;
                
                if (_daoffset != value)
                {
                    _daoffset = value;
                    Util.ConstIni.WriteString("LASTSETTING", "daoffset", _daoffset.ToString());
                }
            }
        }
        private int[] validrng = new int[] { 1, 10, 100, 200, 300, 400, 500, 600, 1000,6000};
        private int _range;
        internal int range{
            get
            {
                if (!validrng.Contains(_range))
                {
                    if (Form1.s_scale == "1000" || Form1.s_scale == "6000")
                        _range = 10;
                    else
                        _range = 1;
                }
                return _range;
            }
            set
            {
                if (validrng.Contains(value))
                {
                    if (_range != value)
                    {
                        _range = value;
                        //Util.ConstIni.WriteString("LASTSETTING", "range", _range.ToString());
                    }
                }
            }
        }

        private Queue<double> datafilter;
        private Queue<double> trackfilter;
        public bool bTracking = true;
        
        internal Processor()
        {
            datafilter = new Queue<double>();
            trackfilter = new Queue<double>();

            _setting = 0;  //Decimal.Parse(Util.ConstIni.StringValue("LASTSETTING", "setting"));
            _range = 10; // Int32.Parse(Util.ConstIni.StringValue("LASTSETTING", "range"));

            if(Form1.s_scale == "6000")
                _range = 6000; //for 6000A only
            try
            {
                _daoffset = Decimal.Parse(Util.ConstIni.StringValue("LASTSETTING", "daoffset"));
            }
            catch
            {
                _daoffset = 0;
            }
            _adscale = 1;
            try
            {
                _adscale = Double.Parse(Util.ConstIni.StringValue("LASTSETTING", "adscale"));
            }
            catch
            {
                _adscale = 1;
            }
           
            bTracking = true; // (Util.ConstIni.StringValue("LASTSETTING", "tracking") != "");

            if (Math.Abs(Convert.ToDouble(_daoffset)) > 0.1) //100mV //changed from 10mV to 100mV because 6000A has bigger offset 2015-07-27 song,jie
                _daoffset = 0;

            bOn = false;
            bKTT = true;
        }
        private Decimal _track_setting = 0;
        public void RefreshOutput()
        {
            UpdateCurrentOnly();
            if (bOn)
            {
                if (bTracking)
                {
                    double _dsetting = Convert.ToDouble(_setting);
                    if ((Math.Abs(_dsetting - Current) < _dsetting * 3e-3) &&  //close enough to setting //change variance from 1e-3 to 3e-3 2015-07-27 song,jie
                        (Math.Abs(_dsetting - Current) > _dsetting * 2e-6) &&  //bigger than 2 ppm
                        (_dsetting >= range * 0.01))   //setting is big enough
                    {
                        trackfilter.Enqueue(Current);
                        if (trackfilter.Count > 5)
                            trackfilter.Dequeue();
                        if (trackfilter.Count == 5)
                        {
                            trackfilter.Dequeue(); //throw 1st value;
                            double avg_current = ((trackfilter.Sum() - trackfilter.Max() - trackfilter.Min()) / (trackfilter.Count - 2));
                            _track_setting = _track_setting + Convert.ToDecimal((_dsetting - avg_current) / 1.7);
                            ToVoltage(_track_setting);
                            trackfilter.Clear();
                        }
                    }
                    else
                    {
                        ToVoltage(_track_setting);
                    }
                }
                else
                {
                    ToVoltage(_setting);
                }
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
                #region disable range change
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
                #endregion
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
        public bool CalibrateADScale(double std)
        {
            double va;
            double sum = 0;
            //average of ten readings
            for (int i = 0; i < 10; i++)
            {
                if (!CollectVoltage(out va))
                    return false;
                sum = sum + va;
            }
            sum = sum / 10;
            adscale = std / sum;
            return true;
        }
        public void Reset()
        {
            DeviceMgr.Reset();
        }
        private bool UpdateCurrentOnly()
        {
            double va;
            if (!CollectVoltage(out va))
                return false;
            va = va * adscale;
            if((Form1.s_scale == "300") )
               Current = va * 1000.0; // 0-0.3V=>300A
            if ((Form1.s_scale == "600") || (Form1.s_scale == "1000") || (Form1.s_scale == "6000"))
            {
                if (range == 6000)
                    Current = va * 6000;//1V->6000A
                else if (range == 600 || range == 1000)
                    Current = va * 1000.0; // 0-0.6V=>600A 0-1V=>1000A
                else if (range == 300)
                    Current = va * range / 10.0; //0-10V=>range
                else if (range == 10 || range == 100)
                    Current = va * range;  //0-1V=>range
                else
                    Current = va * range;  //0-1V=>range
            }
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
                    DeviceMgr.Action("navzero", 0);
                    System.Threading.Thread.Sleep(1000);
                    return;
                }
                System.Threading.Thread.Sleep(1000);
            }
        }
        private bool ToVoltage(Decimal current)
        {
            double volt=0;
            if (Form1.s_scale == "300")
                volt = Convert.ToDouble(current)/50.0; //0-8V => 0-400A
            if (Form1.s_scale == "600")
            {
                if(range == 1) //1A
                    volt = Convert.ToDouble(current) * 10.0; //0-10V => 1A
                if(range == 10) //10A
                    volt = Convert.ToDouble(current) * 1.0; //0-10V => 10A
                if (range == 100) //100A
                    volt = Convert.ToDouble(current) / 10.0; //0-10V => 100A
                if (range == 300) //300A
                    volt = Convert.ToDouble(current) / 30.0; //0-10V => 300A
                if (range == 600) //600A
                    volt = Convert.ToDouble(current) / 60.0; //0-10V => 600A
            }
            if (Form1.s_scale == "1000")
            {
                if (range == 1) //1A
                    volt = Convert.ToDouble(current) * 10.0; //0-10V => 1A
                if (range == 10) //10A
                    volt = Convert.ToDouble(current) * 1.0; //0-10V => 10A
                if (range == 100) //100A
                    volt = Convert.ToDouble(current) / 10.0; //0-10V => 100A
                if (range == 300) //300A
                    volt = Convert.ToDouble(current) / 30.0; //0-10V => 300A
                if (range == 600) //600A
                    volt = Convert.ToDouble(current) / 60.0; //0-10V => 600A
                if (range == 1000) //1000A
                    volt = Convert.ToDouble(current) / 100.0; //0-10V => 1000A

            }
            if (Form1.s_scale == "6000")
            {
                if (range == 6000) //6000A
                    volt = Convert.ToDouble(current) / 600.0; //0-10V => 6000A

            }
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

            double volt = voltage - Convert.ToDouble(daoffset);
            if (volt < 0)
                volt = 0;
            if (Math.Abs(volt) > 10) //turn off output or invalid adreading
                return false;

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
    }
}
