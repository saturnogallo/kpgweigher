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
        static public SerialPort ad2port; //AD board
        static Dictionary<string, UInt16> reg_map;
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
            int i;

            Logger.SysLog("");
            Logger.SysLog(DateTime.Now.ToLongTimeString());
            //Thread.Sleep(3000);

            reg_map = new Dictionary<string, ushort>();
            string[] regs = new string[] { "VOLT_1V", "VOLT_2V", "VOLT_5V", "VOLT_10V", "VOLT_20V" , "VOLT_50V" , "VOLT_OFF","MUL_OFF",
                                           "MUL_1", "MUL_10", "MUL_100", "MUL_500", "MUL_1000"};

            UInt16 r = 0x0001;
            #region set regmap
            {
                reg_map["VOLT_1V"]  = 0x0004;
                reg_map["VOLT_2V"]  = 0x0008;
                reg_map["VOLT_5V"]  = 0x0010;
                reg_map["VOLT_10V"] = 0x0020;
                reg_map["VOLT_20V"] = 0x0040;
                reg_map["VOLT_50V"] = 0x0080;
                reg_map["VOLT_OFF"] = 0x0004; //=VOLT_1V
                //0x4000 no use
                reg_map["MUL_1"]    = 0x2000;
                reg_map["MUL_10"]   = 0x1000;
                reg_map["MUL_100"]  = 0x4800;
                reg_map["MUL_500"]  = 0x8800;
                reg_map["MUL_1000"] = 0x0900;
                reg_map["MUL_OFF"]  = 0x2000; //=MUL_1
            }
            #endregion
            WaitEvent = new AutoResetEvent(false);
            OverEvent = new AutoResetEvent(false);
            inbuffer = new StringBuilder();
            cmdbuffer = new StringBuilder();
            adbuffer = new StringBuilder();
            ad2buffer = new StringBuilder();
            //ad_rdgbuf = new List<double>();

            if (GlobalConfig.ISDEBUG)
                return;
            #region init switch board port
            port = new SerialPort();
            port.BaudRate = 9600;

            port.PortName = GlobalConfig.sSwiPort;
            port.Parity = Parity.None;
            port.DataBits = 8;
            port.StopBits = StopBits.One;
            port.NewLine = "\r";

            string[] ports = SerialPort.GetPortNames();
            if(! SerialPort.GetPortNames().Contains(port.PortName))
                throw new Exception(port.PortName + " not exists");
            for (i = 0; i < 10; i++)
            {
                try
                {
                    
                    port.Open();
                    if (port.IsOpen)
                        break;
                    Thread.Sleep(3000);
                }
                catch(Exception ex)
                {
                    Logger.Log(ex.Message);
                }
            }
            if (i >= 10)
                throw new Exception("Failed to open port " + port.PortName);
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
                throw new Exception("Failed to open port " + cmdport.PortName);
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
        
        /*nouse now protocal of ad weighing board
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
            if (action == "navto1v" || action == "navto120mv" || action == "navto30v")
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
                    ad2port.Write(StringResource.str("NAV_10MV_" + GlobalConfig.sNavmeter2)); //10mv
                }
                if (action == "navto30v")
                {
                    ad2port.Write(StringResource.str("NAV_30V_" + GlobalConfig.sNavmeter2)); //10mv
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
                while ((timeout-- > 0) && (ad2reading < -9000))
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
            if (action == "navto1v" || action == "navto120mv" || action == "navto30v")
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
                    port.Write(StringResource.str("NAV_10MV_" + GlobalConfig.sNavmeter)); //10mv
                }
                if (action == "navto30v")
                {
                    port.Write(StringResource.str("NAV_30V_" + GlobalConfig.sNavmeter)); //10mv
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
            RelayState("VOLT_OFF", "MUL_OFF");
            Action("daoutput", new byte[] { 0x55, 0x64, 0xfc, 0x00, 0x00, 0x00, 0x04 }); //set control
            Action("navto30v",0); //vg
            Action2("navto1v", 0); //vx
            
            Thread.Sleep(3000);
        }

        static private string oldvolt = ""; //old Es voltage switch satus
        static private string oldmultiply = "";  //old resistance divider
        static private string[] volttbl = new string[] { "VOLT_1V", "VOLT_2V", "VOLT_5V", "VOLT_10V", "VOLT_20V","VOLT_50V", "VOLT_OFF" };
        static private string[] multbl = new string[] { "MUL_1", "MUL_10", "MUL_100", "MUL_500", "MUL_1000", "MUL_OFF"};
        static private string[] outputtbl = new string[] { "ON", "OFF" };
        static private byte[] UsHead = new byte[] { Convert.ToByte('U'), Convert.ToByte('s'), Convert.ToByte('U'), Convert.ToByte('U') };
        static private byte[] UtHead = new byte[] { Convert.ToByte('U'), Convert.ToByte('t')};
        static private byte[] UdHead = new byte[] { Convert.ToByte('U'), Convert.ToByte('d') };
        static private byte[] UsTail = new byte[] { Convert.ToByte('V') };
        static private byte[] values = new byte[] { 0x00, 0x00, 0xff };

        //output: ON/OFF
        static public void RelayState(string volt, string multiply)
        {
            if (volt == "")
                volt = oldvolt;
            if (multiply == "")
                multiply = oldmultiply;

            if ((volt == oldvolt)  &&
                (multiply == oldmultiply))
                return;
            UInt16 regout = 0x0000;
            DelayWrite(UtHead, 0, 2);

            if (volttbl.Contains(volt))
            {
                regout = (ushort)(regout | reg_map[volt]);
                oldvolt = volt;
            }
            if (multbl.Contains(multiply))
            {
                regout = (ushort)(regout | reg_map[multiply]);
                oldmultiply = multiply;
            }
            values[0]= Convert.ToByte( regout & 0x00ff);
            values[1] = Convert.ToByte(regout >> 8);
            values[2] = Convert.ToByte((0x2ff - values[0] - values[1]) & 0xff);
            DelayWrite(values, 0, 3);

            return;
            //send again to avoid missing
            //DelayWrite(UtHead, 0, 2);
            //DelayWrite(values, 0, 3);
            //DelayWrite(UsTail, 0, 1);
        }
    }
    internal class Processor
    {
        //Es range types
        private int IND_ES_1V = 0;
        private int IND_ES_2V = 1;
        private int IND_ES_5V = 2;
        private int IND_ES_10V = 3;
        private int IND_ES_20V = 4;
        private int IND_ES_50V = 5;
        //store real Es values for each range.
        private Decimal[] _EsReals = new Decimal[6]; //total 6 types
        public static string[] _EsTitles = new string[] { "1V", "2V", "5V", "10V", "20V", "50V"};

        //Rs types from 1e^3 to 1e^12
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
        internal static int IND_RS_BASE = 3;
        //store real RsValues for each range
        private Decimal[] _RsReals = new Decimal[10]; //total 10 types
        public static string[] _RsTitles = new string[] { "1k  ", "10k ", "100k", "1M  ", "10M ", "100M", "1G  ", "10G ", "100G", "1T  " };

        //Rx range can be 1e+2 to 1e+14
        public static Decimal[] _RxTables = new Decimal[] { 1e+2M, 1e+3M, 1e+4M, 1e+5M, 1e+6M, 1e+7M, 1e+8M, 1e+9M, 1e+10M, 1e+11M, 1e+12M, 1e+13M, 1e+14M };
        public static string[] _RxTitles = new string[] {"100 ", "1k  ", "10k ", "100k", "1M  ", "10M ", "100M", "1G  ", "10G ", "100G", "1T  ","10T ","100T" };
        //public static string[] _RxMuls = new string[] { "1", "10", "10", "100", "100", "500", "500", "500", "500", "500", "500", "1000", "1000" };

        
        public Decimal EsValue  //Es value can only be fixed values
        {
            get
            {
                if (EsIndex >= 0 && EsIndex < _EsReals.Length)
                    return _EsReals[EsIndex] ;
                else
                    return -1;
            }
            set //modification only allowed for back door
            {
                if (EsIndex >= 0 && EsIndex < _EsReals.Length)
                {
                    _EsReals[EsIndex] = value;
                    Util.ConstIni.WriteString("LASTSETTING", "esreal" + EsIndex.ToString(), _EsReals[EsIndex].ToString());
                }
            }
        }
        private int _EsIndex = -1;        //Index to EsValues the range setting
        internal int EsIndex{
            get
            {
                return _EsIndex;
            }
            set
            {
                if (value > _EsReals.Length)
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
        public int Percent
        {
            get
            {
                int v = Convert.ToInt32(Math.Round(_VxOutput*18));
                if (v < 0)
                    return 0;
                if (v > 180)
                    return 180;
                return v;

            }
        }
        internal double VxMeasure = 0;
        public static string[] _MulTitles = new string[] { "1V - 10V", "10 - 100V", "100 - 1kV", "200 - 2kV", "500 - 5kV", "1k - 10kV" };
        public static int[] MulValues = new int[] { 10, 100, 1000, 2000, 5000, 10000 };
        private double _VxOutput = 0; //store the real DA value
        internal double VxOutput //stored for DA value output * mulitply
        {
            get
            {
                return _VxOutput * VxMultiplier;
            }
            set
            {
                if (value < 0)
                    return;
                if (value / VxMultiplier >= 10) //over 10 volt
                    return;
                _VxOutput = value / VxMultiplier;
            }
        }
        internal Decimal RxNominal = 0;
        private int _RxIndex;
        internal int RxIndex
        {
            get
            {
                return _RxIndex;
            }
            set
            {
                int i = value;
                if (i >= 0 && i < _RxTables.Length)
                {
                    RsIndex = _RsTables[i];
                    EsIndex = _EsTables[i];
                    RxNominal = _RxTables[i];
                    VxMultiplier = _MulTables[RxIndex];
                    //Initial VxOutput and _Rx
                    VxOutput = 1; //1V at most
                    _RxIndex = i;
                    
                }

            }
        }
        private Decimal lastRx = 0;
        internal Decimal Rx 
        {
            get
            {
                return lastRx;
            }
            set
            {//target Rx setting
                if (value <= 0)
                    return;
                
                double dRx = Convert.ToDouble(value);
                for (int i = 0; i < _RxTables.Length; i++)
                {
                    if (Math.Abs(dRx - Convert.ToDouble(_RxTables[i])) < 0.5 * Convert.ToDouble(_RxTables[i])) //start from 1e+2, 50% variance
                    {
                        RxIndex = i;
                        return;
                    }
                }
            }
        }

        internal Decimal RsValue //Rs value can only be fixed values
        {
            get
            {
                if (RsIndex >= 0 && RsIndex < _RsReals.Length)
                    return _RsReals[RsIndex];
                else
                    return -1;
            }

            set
            {
                if (RsIndex >= 0 && RsIndex < _RsReals.Length)
                {
                    _RsReals[RsIndex] = value;
                    Util.ConstIni.WriteString("LASTSETTING", "rsreal" + RsIndex.ToString() , _RsReals[RsIndex].ToString());
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
            set
            {
                if (value >= _RsReals.Length || value < -1)
                    return;
                if (_RsIndex == value)
                    return;

                bOn = false;
                _RsIndex = value;

                Util.ConstIni.WriteString("LASTSETTING", "rsindex", _RsIndex.ToString());
            }
        }
        private int _vxmultiplier = 1;
        public int VxMultiplier{
            get
            {
                return _vxmultiplier;
            }
            set
            {
                Util.ConstIni.WriteString("LASTSETTING", "vxmultiplier", _vxmultiplier.ToString());
                _vxmultiplier = value;
            }
        }

        private void RescueDA() //Rescue DA from something external EMS signal
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
                
                if ((value == false) ||(EsIndex < 0 || RxIndex < 0))
                {
                    ToDAValue(0);
                    DeviceMgr.RelayState("VOLT_OFF", "MUL_OFF");
                    _bOn = false;
                    return;
                }
                RescueDA();
                Thread.Sleep(200);
                DeviceMgr.RelayState("VOLT_" + _EsTitles[EsIndex], "MUL_" + this.VxMultiplier.ToString());
                ToDAValue(_VxOutput);
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

        //Es & Rs & VsTables table store the index to EsValue, RsValue and VsMultiply for each Rx selection
        private int[] _EsTables; //index to values
        private int[] _RsTables; //index to values
        private int[] _MulTables;//multiply for Vx Voltage

        internal Processor()
        {
            _EsTables = new int[]     { IND_ES_1V, IND_ES_10V, IND_ES_10V, IND_ES_10V, IND_ES_10V, IND_ES_20V, IND_ES_5V, IND_ES_5V, IND_ES_5V, IND_ES_5V, IND_ES_5V, IND_ES_10V, IND_ES_10V };
            _RsTables = new int[]     { IND_RS_3,  IND_RS_3,   IND_RS_4,   IND_RS_4,   IND_RS_5,   IND_RS_6,  IND_RS_6,  IND_RS_7,  IND_RS_8,  IND_RS_9,  IND_RS_10, IND_RS_11,  IND_RS_12 };
            _MulTables = new int[]     { 1,         10,         10,         100,        100,        500,       500,       500,       500,       500,       500,       1000,       1000 }; 
            datafilter = new Queue<double>();
            datafilter2 = new Queue<double>();
            
            _RsIndex = Util.ConstIni.IntValue("LASTSETTING", "rsindex");
            _EsIndex = Util.ConstIni.IntValue("LASTSETTING", "esindex");
            VxMultiplier = Util.ConstIni.IntValue("LASTSETTING", "vxmultiplier");
            for(int i = 0; i < _EsReals.Length;i++)
                _EsReals[i] = Decimal.Parse(Util.ConstIni.StringValue("LASTSETTING", "esreal"+i.ToString()));

            for (int i = 0; i < _RsReals.Length; i++)
                _RsReals[i] = Decimal.Parse(Util.ConstIni.StringValue("LASTSETTING", "rsreal" + i.ToString()));
            
            _daoffset = Decimal.Parse(Util.ConstIni.StringValue("LASTSETTING", "daoffset"));
            if (Math.Abs(Convert.ToDouble(_daoffset)) > 0.0001) //100uV
                _daoffset = 0;
            
            bOn = false;
        }
        public int RefreshOutput()
        {
            if (!bOn)
            {
                //UpdateIndicatorOnly;
                //No indicator to update here
                return 0;
            }
            else
            {
                return this.UpdateVxOutput();
                //UpdateVxOutput();
            }
        }
        //Delta voltage measurement
        private bool CollectVoltage(out double reading)
        {
            int badcount = 0; //count of bad communication
            reading = 0;
            
            while (badcount < 3)
            {
                DeviceMgr.Action("navread", 0);
                if (DeviceMgr.reading < -999)
                {
                    if (DeviceMgr.nav_range != "navto30v")
                    {
                        badcount++;
                        if (badcount < 3)
                        {
                            Thread.Sleep(500);
                            continue;
                        }
                        badcount = 0;
                        DeviceMgr.Action("navto30v", "");
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
                /*disable range change
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
                if (false && (DeviceMgr.nav_range == "navto30v") && (Math.Abs(DeviceMgr.reading) < 1) && bOn)
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
                 **/
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
                if ((DeviceMgr.nav_range == "navto30v") && (sqr > 0.05)) //30v
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
        //Vx output measurement
        private bool CollectVoltage2(out double reading)
        {
            int badcount = 0; //count of bad communication
            reading = 0;

            while (badcount < 3)
            {
                DeviceMgr.Action2("navread", 0);
                if (DeviceMgr.ad2reading < -999)
                {
                    /*
                    if (DeviceMgr.nav_range2 != "navto1v")
                    {
                        badcount++;
                        if (badcount < 3)
                        {
                            Thread.Sleep(500);
                            continue;
                        }
                        badcount = 0;
                        DeviceMgr.Action2("navto1v", "");
                        Thread.Sleep(3000);
                        continue;
                    }
                    else
                    {
                        return false;
                    }*/
                }
                else
                {
                }
                /*disable range change
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
                if (false && (DeviceMgr.nav_range2 == "navto30v") && (Math.Abs(DeviceMgr.ad2reading) < 1) && bOn)
                {
                    badcount++;
                    if (badcount < 3)
                    {
                        Thread.Sleep(500);
                        continue;
                    }

                    badcount = 0;
                    DeviceMgr.Action2("navto1v", "");
                    Thread.Sleep(3000);
                    continue;
                }
                */
                if (DeviceMgr.nav_range2 == "navto120mv")
                    datafilter2.Enqueue(DeviceMgr.ad2reading / 1000);
                if (DeviceMgr.nav_range2 == "navto1v")
                    datafilter2.Enqueue(DeviceMgr.ad2reading);
                if (DeviceMgr.nav_range2 == "navto30v")
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
                if ((DeviceMgr.nav_range2 == "navto30v") && (sqr > 0.05)) //30V
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
                System.Threading.Thread.Sleep(1000);
            }
        }
        public void ZeroON2()
        {
            int i = 0;
            while (i++ < 5)
            {
                double va;
                if (CollectVoltage2(out va))
                {
                    if (Math.Abs(va) < 0.00001) // < 10uV
                        DeviceMgr.Action2("navzero", 0);
                    break;
                }
                System.Threading.Thread.Sleep(1000);
            }
        }
        private Queue<double> q_vgs = new Queue<double>(6);
        public int ADdelay = 3;
        public bool bStable = false;
        //return value is the interval for next reading
        private int UpdateVxOutput()
        {
            bStable = false;
            //update Voltage output
            if (GlobalConfig.ISDEBUG) //debug use
            {
                ToDAValue(0);
                return 0;// DeviceMgr.success;
            }
            //measurement of Rx 
            double Vg;
            if (!CollectVoltage(out Vg))
            {
                CollectVoltage2(out Vg);
                return 0;
            }
            Vg = -Vg;
            //q_vgs.
            double Vx;
            if (!CollectVoltage2(out Vx))
                return 0;
            
            VxMeasure = Vx * VxMultiplier*10; 
            double thisRx;
            thisRx = ((VxMeasure + Vg) * Convert.ToDouble(RsValue) / (Convert.ToDouble(EsValue) - Vg));
            //Vg < Vmeasure/1000 and variation < 5/10000
            if (Math.Abs(Convert.ToDouble(lastRx) - thisRx) < Math.Abs(Convert.ToDouble(lastRx) * 0.0001) && (Math.Abs(Vg) < Math.Abs(VxMeasure) * 0.00005))
            {
                lastRx = Convert.ToDecimal(thisRx);
                bStable = true;
                return 0; //no delay for next reading
            }
            
            lastRx = Convert.ToDecimal(thisRx);
            double newVx = thisRx * Convert.ToDouble(EsValue) /  Convert.ToDouble(RsValue);

            double volt = (newVx - VxMeasure) / (1.1);
            if (q_vgs.Count <= 3 || Math.Abs(volt) > ( VxMultiplier * 1000.00 / 262144)) //bigger than 1000 counts
            {
                VxOutput = VxOutput + volt;
                q_vgs.Clear();
                if(!ToDAValue(_VxOutput))
                    return 0;
                return ADdelay;//5 seconds for stable
            }
            q_vgs.Enqueue(volt);
            if (q_vgs.Count > 5)
            {
                q_vgs.Dequeue();
            }
            if (q_vgs.Count == 5)
            {
                volt = ((q_vgs.Sum() - q_vgs.Max() - q_vgs.Min()) / (q_vgs.Count - 2));
                VxOutput = VxOutput + volt;
                if(!ToDAValue(_VxOutput))
                    return 0;
                if (Math.Abs(Vg) > Math.Abs(VxMeasure) / 1000)
                    q_vgs.Clear();
                return ADdelay;//3 seconds for stable
            }
            return 0;
        }
        private byte[] lasttosend = new byte[] { 0x00,0x00,0xff, 0xff, 0xff, 0x00 };
        //0 to 10 V setting
        public bool ToDAValue(double voltage)
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
