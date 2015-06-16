using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Diagnostics;
using System.IO;
using System.IO.Ports;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Data;
using Raysting.Common;
namespace Zddq2
{
    public delegate void StrEventHandler(object sender, string e);
    public enum RUN_STATE
    {
        DISABLED,
        IDLE,
        INITIALIZING,
        SEARCHING,
        TRACKING,
        READING,
        STOPPING,
        ERROR
    }
    public enum RXDATA_MODE
    {
        RATIO_RX,
        RX,
        TEMPERATURE,
        RS,
        AVG_RX,
        MAX_RX,
        MIN_RX,
        VAR_RX


    }
 


    
    /*
     1,2,4,8,16,32,64,128,256,512,1024,2048,4096  --- 12
B -- 4
ohm 1, -1,-2,-3 --- 4
A 0.1,0.3,10mA ,1A,5A-- 4 , ALL OFF IS 1MA
curr2, ktt, rs/rx, dvm  --  4
3V,10,30V -- 3
     * 
00/09(ON), --- 1024 (J6) -d
01/08(ON), --- 256 (J4) -d
02/0B(ON), J0 -- Rx1
03/0A    , -N/A
04(ON)/0D, J10 NONE
05(ON)/0C, J11 RS1/RS2 -D
06/0F(ON), J8 NONE
07/0E(ON), J9 NONE
10/19(ON), --- 2048 (J7) -d
11/18(ON), --- 8 -d
12/1B(ON), ---512 (J5) -d 
13/1A(ON), --- 32 -d 
14/1D(ON) ---- SQRT -D
15/1C     - NONE
16/1F(ON) - 2uA -D
1E/17(ON)  --- 16 -D
20/29(ON) - - 10uA -D
21/28(ON),  J2 --- Rx3 -D
22/2B(ON), --- 1 -d
23/2A(ON), ----  5uA -D
24/2D(ON),  J14 --- KTT -D
25/2C(ON), J15 --- 4096 -d
26/2F(ON),	J12 -- RS/RX -D
27/2E(ON),  J13 -- DVM ON/OFF -d
30/39(ON),  --- 2  d
31/38(ON),  --- 4  d
32/3B(ON), J3  -- RX4 -D
33/3A(ON), J1, -- RX2 -D
3D(ON)/34, J19  --- 0.1mA(9606) -D
3C/35(ON), J17  --- 1mA (9606) -D
3F/36(ON), J18	--- 10mA (9606) -D
3E/37(ON), J16  --- 3mA - (9606) -D
40/41(ON),  --- 128 -d
42/43(ON),  --- 64 -d
     * 

*/
    internal class DeviceMgr
    {
        static public AutoResetEvent WaitEvent; //flag for wait response, when flag is on,node agent can process the command else node agent cannot do that
        static public AutoResetEvent OverEvent; //flag for process done event.
        static public object agent_access = new object(); //lock for agent access
        
        //message 
        static private string msg_action = "";
        static private UInt32 msg_value;
        static private DateTime msg_deaddate;


        static private Thread msg_loop;
        static public SerialPort port;  //all navmeter and relay will share the same port
        static public SerialPort cmdport; //all command from PC will use the port
        static public void Dummy()
        {
            
        }
        static public double reading;
        static public bool done;
        static private StringBuilder inbuffer;
        static private StringBuilder cmdbuffer;
        static public bool DEBUG = true;
        static DeviceMgr()
        {
            
            bool bDebugGPIB = true;
            Thread.Sleep(3000);
            WaitEvent = new AutoResetEvent(false);
            OverEvent = new AutoResetEvent(false);
            inbuffer = new StringBuilder();
            cmdbuffer = new StringBuilder();
            port = new SerialPort();

            if (DEBUG)
                return;

            
            #region init port
            port.BaudRate = 9600;
            if (!bDebugGPIB)
                port.PortName = "COM10";
            else
                port.PortName = "COM11";
            port.Parity = Parity.None;
            port.DataBits = 8;
            port.StopBits = StopBits.One;
            port.NewLine = "\r";
            port.DataReceived += new SerialDataReceivedEventHandler(swiport_DataReceived);
            int i;
            for (i = 0; i < 10; i++)
            {
                if (bDebugGPIB)
	                break;
                port.Open();
                if (port.IsOpen)
                    break;
                Thread.Sleep(3000);
            }
            if(i >= 10)
                throw new Exception("Failed to open port A");
            #endregion 
          /*   
            
            #region init cmdport
            cmdport = new SerialPort();
            cmdport.BaudRate = 9600;
            if(!bDebugGPIB)
                cmdport.PortName = "COM11";
            else
                cmdport.PortName = "COM10";
            cmdport.Parity = Parity.None;
            cmdport.DataBits = 8;
            cmdport.StopBits = StopBits.One;
            cmdport.NewLine = "\r";
            cmdport.Open();
            #endregion
            if (!cmdport.IsOpen)
            {
                port.Close();
                throw new Exception("Failed to open port B");
            }
            cmdport.DiscardInBuffer();
            cmdport.DataReceived += new SerialDataReceivedEventHandler(cmdport_DataReceived);
            */ 
            agent_access = false;
            
            msg_loop = new Thread(new ThreadStart(MessageHandler));
            msg_loop.IsBackground = false;
            if(bDebugGPIB)
                return;
            msg_loop.Start();
        }

        

        static public string GetLogFileName()
        {
            string file = @"C:\\"+DateTime.Now.ToString("yyyy_MM_dd")+".txt";
            if (!File.Exists(file))
            {
                try
                {
                    FileStream fsLog = new FileStream(file, FileMode.Create, FileAccess.Write, FileShare.Read);
                    StreamWriter sw = new StreamWriter(fsLog);
                    sw.Close();
                    fsLog.Close();
                }
                catch (System.Exception ex)
                {
                }
            }
            return file;
        }
        static public void Log(string line)
        {
            string fn = GetLogFileName();
            try
            {
                FileStream fsLog = new FileStream(fn, FileMode.Append, FileAccess.Write, FileShare.Read);
                using (StreamWriter sw = new StreamWriter(fsLog))
                {
                    sw.WriteLine(DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff") + "\t" + line);
                }
                fsLog.Close();
            }
            catch (System.Exception ex)
            {
            }
        }
        static public void ReportHeader(int total)
        {
            string reply;
            cmdport.Write(cmdport.NewLine);
            cmdport.Write(cmdport.NewLine);
            cmdport.Write(cmdport.NewLine);
            reply = String.Format("D{0} Measurements:", total);
            cmdport.Write(reply);
            cmdport.Write(cmdport.NewLine);
            //cmdport.WriteLine(reply);
            reply = String.Format("R{0}", Program.lst_rxinfo[0].dRxInput.ToString("E13"));
            cmdport.WriteLine(reply);
            reply = String.Format("S{0}", Program.lst_rsinfo[0].dTValue.ToString("E13"));
            cmdport.WriteLine(reply);

        }
        static public void ReportData(int index, double value)
        {
            if (DEBUG)
                return;

            string reply;
            reply = String.Format("#{0}",index.ToString());
            cmdport.WriteLine(reply);
            reply = String.Format("&{0}",value.ToString("E13"));
            cmdport.WriteLine(reply);
            
        }
        /// <summary>
        /// PC command interface
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">args</param>
        static void cmdport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
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
                    if(data != '\r' && data != '\n')
                    cmdbuffer.Append(data);
                }
            }
        }
        /// <summary>
        /// checking switch board port
        /// </summary>
        static void swiport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            while (port.BytesToRead > 0)
            {
                char data = Convert.ToChar(port.ReadChar());
                if (((data >= '0') && (data <= '9')) ||
                (data == '.') || (data == '-') || (data == '+') || (data == 'e') || (data == 'E'))
                {
                    inbuffer.Append(Convert.ToChar(data));
                }
                else
                {
                    if (inbuffer.Length >= 4)
                    {
                        try
                        {
                            reading = Double.Parse(inbuffer.ToString());
                        }
                        catch
                        {
                            reading = INVALID_READING;
                        }
                    }
                    if(inbuffer.Length > 0)
                        inbuffer.Remove(0, inbuffer.Length);
                }
            }
        }
        public const double INVALID_READING = -9999;
        public const double MIN_READING = -999;
        static public bool IsInAction
        {
            get{
                if (DEBUG)
                    return false;
                if (msg_action == "" || msg_action == null)
                    return false;
                else
                    return true;
            }
        }
        static public bool IsTimeOut = false;

        static public void Action(string cmd)
        {
            Action(cmd, 0, 300);
        }
        static public void Action(string cmd, UInt32 param)
        {
            Action(cmd, param, 300);
        }
        static Random rand = new Random();
        static public void Action(string cmd, UInt32 param, UInt32 timeout)
        {
            if (DEBUG)
            {
                reading = rand.NextDouble();
                return;
            }
            while (IsInAction)
                Thread.Sleep(10);
            IsTimeOut = false;
            msg_action = cmd;
            msg_value = param;
            msg_deaddate = DateTime.Now.AddMilliseconds(timeout);
        }
        #region relay operation
        //k1 = RS/RX, DVM
        //ktt = ON/OFF
        //curr = CURR_P001|CURR_P01/P1/P3/1/5 ; 1mA/10mA,0.1A,0.3A,1A,5A
        static private string oldswi = ""; //old measurement switch satus
        static private string oldktt = "";
        static private string oldcurr = "";
        static private string oldsqr = "";
        static private string oldrxch = "";
        static private UInt32 oldk = 0;
        static private string oldconstv = "";
        static private string oldstd = "";
        static public string[] rstbl = new string[] { "",  "STD_CH1", "STD_CH2"};
        static public string[] rxtbl = new string[] { "", "RX_CH1", "RX_CH2", "RX_CH3", "RX_CH4" };
        static public string[] meastbl = new string[] { "", "MEAS_RS","MEAS_RX","MEAS_DELTA" };
        static public string[] currtbl = new string[] { "", "CURR_OFF", "CURR_2UA","CURR_5UA", "CURR_10UA", "CURR_P1MA", "CURR_1MA", "CURR_3MA", "CURR_10MA"};
        static private byte[] UsHead = new byte[] { Convert.ToByte('U'), Convert.ToByte('s'), Convert.ToByte('U'), Convert.ToByte('U') };
        static private byte[] UsTail = new byte[] { Convert.ToByte('V')};
        static private void DelayWrite(byte[] buf, int start, int len)
        {
            
            for (int i = 0; i < len; i++)
            {
                port.Write(buf,i,1);
                Thread.Sleep(30 * len);
            }
        }
        

        /// <summary>
        /// switch to k state
        /// </summary>
        /// <param name="k">coil number</param>
        static public void KState(UInt32 k)
        {
            if (DEBUG)
                return;
            if (oldk == k)
                return;
            DelayWrite(UsHead, 0, 4);
            if( k % 2 == 0) //1
                DelayWrite(new byte[] { 0x2B }, 0, 1);
            else
                DelayWrite(new byte[] { 0x22 }, 0, 1);
            k = k / 2;
            if (k % 2 == 0) //2
                DelayWrite(new byte[] { 0x39 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x30 }, 0, 1);
            k = k / 2;
            if (k % 2 == 0) //4
                DelayWrite(new byte[] { 0x38 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x31 }, 0, 1);
            k = k / 2;      //8
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x18 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x11 }, 0, 1);
            k = k / 2;      //16
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x17 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x1E }, 0, 1);
            k = k / 2;      //32
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x1A }, 0, 1);
            else
                DelayWrite(new byte[] { 0x13 }, 0, 1);
            k = k / 2;      //64
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x43 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x42 }, 0, 1);
            k = k / 2;      //128
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x41 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x40 }, 0, 1);
            k = k / 2;      //256
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x08 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x01 }, 0, 1);
            k = k / 2;      //512
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x1B }, 0, 1);
            else
                DelayWrite(new byte[] { 0x12 }, 0, 1);
            k = k / 2;      //1024
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x09 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x00 }, 0, 1);
            k = k / 2;      //2048
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x19 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x10 }, 0, 1);
            k = k / 2;      //4096
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x2C }, 0, 1);
            else
                DelayWrite(new byte[] { 0x25 }, 0, 1);
            
            DelayWrite(UsTail, 0, 1);
        }
        static public void Reset()
        {
            KState(Convert.ToUInt32(65536)); //clear all
            RelayState("MEAS_RS", "KTTP", "CURR_OFF", "X1", "STD_CH1", "RX_OFF");
            msg_action = "";
        }
        static public void RelayState(string swi, string ktt, string curr, string sqr, string std, string rxch)
        {
            if (DEBUG)
                return;
            if (((swi == oldswi) || (swi == "")) && 
                ((ktt == oldktt) || (ktt == "")) &&
                ((curr == oldcurr) || (curr == "")) &&
                ((sqr == oldsqr) || (sqr == "")) &&
                ((std == oldstd) || (std == "")))
                return;
                
            DelayWrite(UsHead, 0, 4);
            if (swi.StartsWith("MEAS_"))
            {
                if (swi == "MEAS_RS")
                {
                    DelayWrite(new byte[] { 0x26, 0x27 }, 0, 2);
                    std = "STD_OFF";
                }
                if (swi == "MEAS_RX")
                {
                    DelayWrite(new byte[] { 0x2F, 0x27 }, 0, 2);
                }
                if (swi == "MEAS_DELTA")
                {
                    DelayWrite(new byte[] { 0x2F, 0x2E }, 0, 2);
                }
                oldswi = swi;
            }
            if (ktt.StartsWith("KTT"))
            {
                if(ktt == "KTTN")
                    DelayWrite(new byte[] { 0x24 }, 0, 1); //0x06
                if (ktt == "KTTP")
                    DelayWrite(new byte[] { 0x2D }, 0, 1); //0x0F
                oldktt = ktt;
            }
            if (sqr.StartsWith("X"))
            {
                if (sqr == "X2")
                    DelayWrite(new byte[] { 0x1D }, 0, 1);
                if (sqr == "X1")
                    DelayWrite(new byte[] { 0x14 }, 0, 1);
                oldsqr = sqr;
            }
            if (curr.StartsWith("CURR_"))
            {
                //"CURR_OFF", "CURR_2UA","CURR_5UA", "CURR_10UA", "CURR_P1MA", "CURR_1MA", "CURR_3MA", "CURR_10MA"

                byte[] cb = new byte[] { 0x16, 0x23, 0x20, 0x34, 0x3C, 0x3E, 0x3F }; //1mA
                if (curr == "CURR_2UA") cb[0] = 0x1F; //2UA
                if (curr == "CURR_5UA") cb[1] = 0x2A; //5UA
                if (curr == "CURR_10UA") cb[2] = 0x29; //10UA
                if (curr == "CURR_P1MA") cb[3] = 0x3D; //0.1MA
                if (curr == "CURR_1MA") cb[4] = 0x35; //1MA
                if (curr == "CURR_3MA") cb[5] = 0x37; //3MA
                if (curr == "CURR_10MA") cb[6] = 0x36; //10MA
                DelayWrite(cb, 0, 7);
                //if (oldcurr == "CURR_P001") cb[0] = 0xff;//turn off all is 1mA
                //                    if (oldcurr == "CURR_P01") cb[0] = 0x3E;
                //                  if (oldcurr == "CURR_P1") cb[0] = 0x3F;
                //                if (oldcurr == "CURR_P3") cb[0] = 0x3C;
                //              if (oldcurr == "CURR_1") cb[0] = 0x3D;
                //            if (oldcurr == "CURR_5") cb[0] = 0x0C;

                //          DelayWrite(cb, 0, 1);
                //}
                //                oldcurr = curr;
            }
            if (std.StartsWith("STD_"))
            {
                if (std == "STD_CH1")
                    DelayWrite(new byte[] { 0x05 }, 0, 1);
                if (std == "STD_CH2")
                    DelayWrite(new byte[] { 0x0C }, 0, 1);
            }
            if (rxch.StartsWith("RX_"))
            {
                byte[] rxoff = new byte[] { 0x02, 0x33, 0x21, 0x32};
                if (rxch == "RX_CH1") rxoff[0] = 0x0B;
                if (rxch == "RX_CH2") rxoff[1] = 0x3A;
                if (rxch == "RX_CH3") rxoff[2] = 0x28;
                if (rxch == "RX_CH4") rxoff[3] = 0x3B;
                DelayWrite(rxoff, 0, 4);
            }
            DelayWrite(UsTail, 0, 1);
        }
        #endregion
        static public bool Loaded = false;
        static public void MessageHandler()
        {
            Reset();
            Loaded = true;
            while (true)
            {
                if (msg_action == "")
                {
                    Thread.Sleep(10);
                    continue;
                }
                if (DateTime.Now > msg_deaddate)
                {
                    IsTimeOut = true;
                    msg_action = "";
                    continue;
                }
                if (msg_action == "navread" && reading > MIN_READING)
                {
                    msg_action = "";
                    continue;
                }
                try
                {
                    #region navmeter action
                    
                    if (msg_action == "navto1v" || msg_action == "navto120mv" || msg_action == "navto10mv")
                    {
                        if (msg_action == "navto1v")
                        {
//                            port.Write(NAV_INIT); //init
                            port.Write(StringResource.str("NAV_1V_"+Program.sysinfo.sNavmeter)); //1v
//                            DelayWrite(NAV_AFLTON); //slowmode and aflton
                        }
                        if (msg_action == "navto120mv")
                        {
                            port.Write(StringResource.str("NAV_120MV_" + Program.sysinfo.sNavmeter)); //120mv
                        }
                        if (msg_action == "navto10mv")
                        {
                            port.Write(StringResource.str("NAV_10MV_" + Program.sysinfo.sNavmeter)); //10mv
                        }
                        continue;
                    }
                    if (msg_action == "navread")
                    {
                        reading = INVALID_READING;
                        port.DiscardInBuffer();
                        port.Write(StringResource.str("NAV_READ_" + Program.sysinfo.sNavmeter)); //1v
                        continue;
                    }
                    if (msg_action == "turnk")
                    {
                        KState(msg_value);
                        continue;
                    }
                    if (msg_action.StartsWith("MEAS")) 
                    {
                        string[] cmd = msg_action.Split(new char[]{'|'});
                        if (cmd.Length >= 6)
                        {
                            RelayState(cmd[0],cmd[1], cmd[2], cmd[3], cmd[4],cmd[5]);
                        }
                        continue;
                    }
                    #endregion
                    throw new Exception("Invalide command " + msg_action);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                    IsTimeOut = true;
                    msg_action = "";
                    
                }
            }
        }
    }

    internal class DataMgr
    {
        public List<PointPair> PointList;
        public DataMgr()
        {
            PointList = new List<PointPair>();
        }
        public void ClearAll()
        {
            PointList.Clear();
        }
        public void AddValue(string chan, int index, double val, double ratio, double temp)
        {
            PointList.Add(new PointPair(chan, index, val, ratio, temp, false, DateTime.Now));
        }
        
        public IEnumerable<PointPair> my_list(string chan)
        {
            return PointList.Where(x => (x.chan == chan || chan == "") && (!x.excluded));
        }
        #region data summary
        public int validrows(string chan)
        {
            return PointList.Count(x => (x.chan == chan || chan == "") && (!x.excluded));
        }
        public double my_last(string chan){
            try
            {
                return PointList.Where(x => (x.chan == chan || chan == "") && (!x.excluded)).Last().value;
            }
            catch
            {
                return 0;
            }
        }
        public double my_min(string chan)
        {
            try
            {
                return PointList.Where(x => (x.chan == chan || chan == "") && (!x.excluded)).Min().value;
            }
            catch
            {
                return 0;
            }
        }
        public double my_max(string chan)
        {
            try
            {
                return PointList.Where(x => (x.chan == chan || chan == "") && (!x.excluded)).Max().value;
            }
            catch
            {
                return 0;
            }
        }
        public double my_sqrtsum(string chan)
        {
            try
            {
                if (validrows(chan) < 2)
                    return 0;
                double avg = my_avg(chan);
                double sqr = 0;
                foreach (double v in PointList.Where(x => (x.chan == chan || chan == "") && (!x.excluded)).Select(x => x.value))
                {
                    sqr = sqr + v * (v - avg);
                }
                return Math.Sqrt(sqr/ (validrows(chan)-1))/avg;
            }
            catch
            {
                return 0;
            }
        }
        public double my_sum(string chan)
        {
            try
            {
                return PointList.Where(x => (x.chan == chan || chan == "") && (!x.excluded)).Sum(x => x.value);
            }
            catch
            {
                return 0;
            }
        }
        public double my_avg(string chan)
        {
            try
            {
                return PointList.Where(x => (x.chan == chan || chan == "") && (!x.excluded)).Sum(x => x.value) / validrows(chan);
            }
            catch
            {
                return 0;
            }
        }
        #endregion



    }
    public class PointPair : IComparable
    {
        public int CompareTo(object r)
        {
            if (r is PointPair)
            {
                PointPair pp = r as PointPair;
                return this.value.CompareTo(pp.value);
            }
            else
            {
                return this.ToString().CompareTo(r.ToString());
            }
        }
        public int CompareTo(PointPair r)
        {
            return this.value.CompareTo(r.value);
        }

        public PointPair(string ch,int id, double val, double rt, double t, bool ex, DateTime st)
        {
            chan = ch;
            index = id;
            rvalue = val;
            ratio = rt;
            temp = t;
            stamp = st;
            excluded = ex;
        }
        public DateTime stamp; //"HH:mm:ss.fff"
        public int index;
        public string chan;
        public double value{
            get
            {
                if (datamode == RXDATA_MODE.RATIO_RX)
                    return ratio;
                else if (datamode == RXDATA_MODE.RX)
                    return rvalue;
                else
                    return temp;
            }
        }
        public double rvalue;
        public double ratio;
        public double temp;
        public bool excluded;

        public static RXDATA_MODE datamode;
        
    }
}
