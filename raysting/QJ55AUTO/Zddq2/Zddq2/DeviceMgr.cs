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

    public class FullScreenClass
    {
        public const int SPI_SETWORKAREA = 47;
        public const int SPI_GETWORKAREA = 48;
        public const int SW_HIDE = 0x00;
        public const int SW_SHOW = 0x0001;
        public const int SPIF_UPDATEINIFILE = 0x01;
        [DllImport("coredll.dll", EntryPoint = "FindWindow")]
        private static extern IntPtr FindWindow(string lpWindowName, string lpClassName);
        [DllImport("coredll.dll", EntryPoint = "ShowWindow")]
        private static extern bool ShowWindow(IntPtr hwnd, int nCmdShow);
        [DllImport("coredll.dll", EntryPoint = "SystemParametersInfo")]
        private static extern int SystemParametersInfo(int uAction, int uParam, ref Rectangle lpvParam, int fuWinIni);

        /// <summary>
        /// 设置全屏或取消全屏
        /// </summary>
        /// <param name="fullscreen">true:全屏 false:恢复</param>
        /// <param name="rectOld">设置的时候，此参数返回原始尺寸，恢复时用此参数设置恢复</param>
        /// <returns>设置结果</returns>
        public static bool SetFullScreen(bool fullscreen, ref Rectangle rectOld)
        {
            IntPtr Hwnd = FindWindow("HHTaskBar", null);
            if (Hwnd == IntPtr.Zero) return false;
            if (fullscreen)
            {
                ShowWindow(Hwnd, SW_HIDE);
                /*
                Rectangle rectFull = Screen.PrimaryScreen.Bounds;
                SystemParametersInfo(SPI_GETWORKAREA, 0, ref rectOld, SPIF_UPDATEINIFILE);//get
                SystemParametersInfo(SPI_SETWORKAREA, 0, ref rectFull, SPIF_UPDATEINIFILE);//set
                 */
            }
            else
            {
                ShowWindow(Hwnd, SW_SHOW);
                /*
                SystemParametersInfo(SPI_SETWORKAREA, 0, ref rectOld, SPIF_UPDATEINIFILE);
                 */
            }
            return true;
        }

    }

    public enum ACTION_STATE
    {
        IDLE,
        INIT_FOR_START,
        INIT_FOR_SEARCH,
        INIT_FOR_RS_KTTP,
        INIT_FOR_RS_KTTN,
        COMPLETE_ISRC,
        INIT_FOR_TRACKP,
        INIT_FOR_TRACKN,
        UPDATE_K_TRACK,
        SHOW_NEW_VALUE,
        COMPLETE,
        INIT_FOR_SAFE
    }
    public enum ACTION_REQUEST
    {
        INIT_NAV_RANGE, //set initial navmeter range\
        INITKI_FOR_SEARCH, //set up current and K status for search
        SET_FOR_RS_KTTP,
        SET_FOR_RS_KTTN,
        SET_FOR_RX_KTTP,
        SET_FOR_RX_KTTN,
        SET_FOR_TRACKP,
        SET_FOR_TRACKN,
        SET_NEXT_K,
        SET_SAFE_STATE,
    }
    public class TaskMachine
    {
        public RxInfo currRx;
        public bool bPaused = false;
        public ActionMgr act_mgr;
        private DataMgr data_mgr;
        public TaskMachine()
        {
            act_mgr = new ActionMgr();
            data_mgr = new DataMgr();
        }
        public int iMeasCnt
        {
            get
            {
                return data_mgr.iMeasCnt;
            }
        }
        public int waiting
        {
            get
            {
                return act_mgr.waiting;
            }
        }
        private int curr_ch = 0;
        public ACTION_STATE stm = ACTION_STATE.IDLE;

        public StrEventHandler StatusChanged;

        public bool bRunning {
            get {
                return stm != ACTION_STATE.IDLE;
            }
            set
            {
                if (stm == ACTION_STATE.IDLE && value)
                    stm = ACTION_STATE.INIT_FOR_START;
                if (!value)
                    stm = ACTION_STATE.IDLE;
            }
        }
        public void Start()
        {
            if (!bRunning)
            {
                curr_ch = 0;
                currRx = Program.lst_rxinfo[curr_ch];
                currRx.var.rRs = Program.lst_rsinfo[0].dTValue;

                act_mgr.Reset();
                act_mgr.rx = currRx;
                act_mgr.rs = Program.lst_rsinfo[0];
                data_mgr.Reset();
                data_mgr.rx = currRx;
                data_mgr.rs = Program.lst_rsinfo[0];
                DeviceMgr.ReportHeader(RunWnd.syscfg.iMeasTimes);
                bRunning = true;
            }
        }
        public void PauseResume()
        {
            if (bRunning)
            {
                bPaused = !bPaused;
            }
        }
        public bool is1V = true;
        public void Step()
        {
            if (DeviceMgr.IsInAction()) //communication action is going on
                return;

            if (currRx.i_State == RUN_STATE.STOPPING)   //user choose to stop
            {
                DeviceMgr.Reset();
                act_mgr.Reset();
                bPaused = false;
                bRunning = false;
                currRx.i_State = RUN_STATE.IDLE;
            }
            if (bPaused || !bRunning || act_mgr.IsBusy)
                return;


            if (stm == ACTION_STATE.INIT_FOR_START)//search the next valid channel
            {
                currRx.i_State = RUN_STATE.SEARCHING;
                StatusChanged(this, "search");
                stm = act_mgr.Do(ACTION_REQUEST.INIT_NAV_RANGE,stm);
                return;
            }
            if (stm == ACTION_STATE.INIT_FOR_SEARCH) //prepare the capture with 1:1 mode
            {
                currRx.i_State = RUN_STATE.SEARCHING;
                StatusChanged(this, "search");
                stm = act_mgr.Do(ACTION_REQUEST.INITKI_FOR_SEARCH,stm);
                return;
            }
            if (stm == ACTION_STATE.INIT_FOR_RS_KTTP) //capture once //set to KTTP mode and delay 1s
            {
                currRx.i_State = RUN_STATE.SEARCHING;
                StatusChanged(this, "search");
                stm = act_mgr.Do(ACTION_REQUEST.SET_FOR_RS_KTTP, stm);
                return;
            }
            if (stm == ACTION_STATE.INIT_FOR_RS_KTTN) 
            {
                currRx.i_State = RUN_STATE.SEARCHING;
                StatusChanged(this, "search");

                double val = act_mgr.dStableReading;
                if (val < -999)
                {
                    currRx.i_State = RUN_STATE.STOPPING;
                    StatusChanged(this, "stopping");
                    return;
                } 
                currRx.var.vRs = val;
                stm = act_mgr.Do(ACTION_REQUEST.SET_FOR_RS_KTTN, stm);
                return;
            }
            if (stm == ACTION_STATE.COMPLETE_ISRC) //capture once //set to KTTP mode 
            {
                currRx.i_State = RUN_STATE.SEARCHING;
                StatusChanged(this, "search");
                double val = act_mgr.dStableReading;
                if (val < -999)
                {
                    currRx.i_State = RUN_STATE.STOPPING;
                    StatusChanged(this, "stopping");
                    return;
                }
                currRx.var.vRs = Math.Abs(currRx.var.vRs - val) / 2.0;
                currRx.var.iSrc = currRx.var.vRs / currRx.var.rRs;
                currRx.var.iSrc = currRx.var.iSrc * RX_VAR.INIT_LOOP / (double)currRx.var.iK;
                stm = act_mgr.Do(ACTION_REQUEST.SET_FOR_TRACKP, stm);
                return;
            }
            #region  no use 
            /*
            if (stm == ACTION_STATE.INIT_FOR_RX_KTTP) //capture once //set to KTTP mode 
            {
                currRx.i_State = RUN_STATE.SEARCHING;
                StatusChanged(this, "search");
                double val = act_mgr.dStableReading;
                if (val < -999)
                {
                    currRx.i_State = RUN_STATE.STOPPING;
                    StatusChanged(this, "stopping");
                    return;
                }

                currRx.var.vRs = Math.Abs(currRx.var.vRs - val) / 2.0;
                currRx.var.iSrc = currRx.var.vRs / currRx.var.rRs;
                stm = act_mgr.Do(ACTION_REQUEST.SET_FOR_RX_KTTP, stm);
                return;
            }
            if (stm == ACTION_STATE.INIT_FOR_RX_KTTN)
            {
                currRx.i_State = RUN_STATE.SEARCHING;
                StatusChanged(this, "search");
                double val = act_mgr.dStableReading;
                if (val < -999)
                {
                    currRx.i_State = RUN_STATE.STOPPING;
                    StatusChanged(this, "stopping");
                    return;
                }
                currRx.var.vRx = val;
                stm = act_mgr.Do(ACTION_REQUEST.SET_FOR_RX_KTTN, stm);
                return;
            }
            if (stm == ACTION_STATE.INIT_K_ADJUST) //adjust K
            {
                double val = act_mgr.dStableReading;
                if (val < -999)
                {
                    currRx.i_State = RUN_STATE.STOPPING;
                    StatusChanged(this, "stopping");
                    return;
                }

                currRx.var.vRx = Math.Abs(currRx.var.vRx - val)/ 2;
                currRx.var.rRx = currRx.var.vRx / currRx.var.iSrc;

                currRx.var.iK = currRx.var.calc_capture_nextk();
                DeviceMgr.Action("turnk", Convert.ToUInt32(currRx.var.iK));
                stm = ACTION_STATE.INIT_FOR_TRACKP;
            }
*/
            #endregion
            if (stm == ACTION_STATE.INIT_FOR_TRACKP) //capture once //set to KTTP mode 
            {
                currRx.i_State = RUN_STATE.TRACKING;
                StatusChanged(this, "tracking");
                
                stm = act_mgr.Do(ACTION_REQUEST.SET_FOR_TRACKP, stm);
                return;
            }
            if (stm == ACTION_STATE.INIT_FOR_TRACKN)
            {
                currRx.i_State = RUN_STATE.TRACKING;
                StatusChanged(this, "tracking");
                double val = act_mgr.dStableReading;
                if (val < -999)
                {
                    currRx.i_State = RUN_STATE.STOPPING;
                    StatusChanged(this, "stopping");
                    return;
                }
                currRx.var.vCrossP = val;
                stm = act_mgr.Do(ACTION_REQUEST.SET_FOR_TRACKN, stm);
                return;
            }
            if (stm == ACTION_STATE.UPDATE_K_TRACK)
            {
                currRx.i_State = RUN_STATE.TRACKING;
                StatusChanged(this, "tracking");

                double val = act_mgr.dStableReading;
                if (val < -999)
                {
                    currRx.i_State = RUN_STATE.STOPPING;
                    StatusChanged(this, "stopping");
                    return;
                }
                currRx.var.vCrossN = val;
                               
                stm = act_mgr.Do(ACTION_REQUEST.SET_NEXT_K,stm);
                if (stm == ACTION_STATE.SHOW_NEW_VALUE)
                    currRx.var.log_start(currRx.iRRange);
                return;
            }
            if (stm == ACTION_STATE.SHOW_NEW_VALUE)
            {
                if (data_mgr.AddNewValue(ref currRx.var.rRx))
                {
                     StatusChanged(this, "newvalue");
                }
                if (data_mgr.iMeasCnt == RunWnd.syscfg.iMeasTimes)
                    stm = ACTION_STATE.COMPLETE;
                else
                    stm = ACTION_STATE.INIT_FOR_TRACKP;
                return;
            }
          
            if (stm == ACTION_STATE.COMPLETE) //update the display and try the next channel
            {
                currRx.i_State = RUN_STATE.IDLE;
                bRunning = false;
                StatusChanged(this, "complete");
                act_mgr.Do(ACTION_REQUEST.INIT_NAV_RANGE,stm);
                DeviceMgr.Reset();
                while (DeviceMgr.IsInAction())
                    ;
                stm = ACTION_STATE.IDLE;
            }
        }
        
        public void Stop()
        {
            if (bRunning)
            {
                currRx.i_State = RUN_STATE.STOPPING;
            }
        }
    }

    internal class ActMessage
    {
        public string action;
        public byte addr;
        public UInt32 value;
    }
    /*
     1,2,4,8,16,32,64,128,256,512,1024,2048,4096  --- 12
B -- 4
ohm 1, -1,-2,-3 --- 4
A 0.1,0.3,10mA ,1A,5A-- 4 , ALL OFF IS 1MA
curr2, ktt, rs/rx, dvm  --  4
3V,10,30V -- 3
00/09(ON),J6 -- 1024
01/08(ON), J4 --- 256
02/0B(ON), J0 -- 1 ohm
03/0A -N/A
04(ON)/0D, J10 -- BACKUP 3 -- 0.3A
05(ON)/0C, J11 -- BACKUP 4  --1A
06/0F(ON), J8 -- BACKUP 1 -- overlap - KTT,
07/0E(ON), J9 -- BACKUP 2 -- 5A
10/19(ON), 	J7 --- 2048
11/18(ON) ---- 8
12/1B(ON), J5 ---512
13/1A(ON) ---- 32
14/1D(ON) ---- x2 (big) -- 100mA
15/1C - N/A
16/1F(ON) ---- N/A
1E/17(ON)  -- 16
20/29(ON) --- N/A
21/28(ON),  J2 --- 10^-2 ohm
22/2B(ON) ----- 1
23/2A(ON) ----  CURR x 2 (small) --0.1mA 
24/2D(ON),  J14 --- GAOZU
25/2C(ON), J15 --- 4096
26/2F(ON),	J12 -- RS/RX
27/2E(ON),  J13 -- DVM ON/OFF
30/39(ON) --- 2
31/38(ON)  -- 4
32/3B(ON),  J3 --10^-3 ohm
33/3A(ON), J1, --- 10^-1 ohm
3D(ON)/34,   J19            --- 1 A / 100mA
3C/35(ON), J17            -- 0.3A / 10mA
3F/36(ON),   J18	      --- 1mA (9606)
3E/37(ON), J16  -- 0.1mA - (9606)
40/41(ON)  -- 128
42/43(ON)  -- 64
*/
    internal class DeviceMgr
    {
        static public AutoResetEvent WaitEvent; //flag for wait response, when flag is on,node agent can process the command else node agent cannot do that
        static public AutoResetEvent OverEvent; //flag for process done event.
        static public object agent_access = new object(); //lock for agent access
        static public ActMessage actmsg;
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
        static DeviceMgr()
        {
            bool bDebugGPIB = false;
            SysLog("");
            SysLog(DateTime.Now.ToLongTimeString());
            Thread.Sleep(3000);
            WaitEvent = new AutoResetEvent(false);
            OverEvent = new AutoResetEvent(false);
            inbuffer = new StringBuilder();
            cmdbuffer = new StringBuilder();
            port = new SerialPort();
            
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
            
             
            agent_access = false;
            actmsg = new ActMessage();
            msg_loop = new Thread(new ThreadStart(MessageHandler));
            msg_loop.IsBackground = false;
            if(bDebugGPIB)
                return;
            
            msg_loop.Start();
        }
        static public string GetSysLogFileName()
        {
            return StringResource.basedir + "\\run.txt";
        }
        static public void TouchFile(string file)
        {
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
        }
        static public string GetLogFileName()
        {
            DateTime dt = DateTime.Now;
            string folder = StringResource.basedir + "\\" + dt.ToString("yyyy_MM");
            string file = folder + "\\"+DateTime.Now.ToString("yyyy_MM_dd")+".txt";
            if (!Directory.Exists(folder))
                Directory.CreateDirectory(folder);
            TouchFile(file);
            return file;
        }
        static public void SysLog(string line)
        {
            string fn = GetSysLogFileName();
            try
            {
                if (line == "")
                {
                    if(File.Exists(fn))
                        File.Delete(fn);
                    TouchFile(fn);
                    return;
                }
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
            reply = String.Format("D{0} Measurements:", total);
            cmdport.Write(reply);
            cmdport.Write(cmdport.NewLine);
            //cmdport.WriteLine(reply);
            reply = String.Format("R{0}", Program.lst_rxinfo[0].dRxInput.ToString("E13"));
            cmdport.WriteLine(reply);
            cmdport.Write(cmdport.NewLine);
            reply = String.Format("S{0}", Program.lst_rsinfo[0].dTValue.ToString("E13"));
            cmdport.WriteLine(reply);
            cmdport.Write(cmdport.NewLine);

        }
        static public void ReportData(int index, double value)
        {
            string reply = String.Format("#{0}", index.ToString());
            cmdport.WriteLine(reply);
            cmdport.Write(cmdport.NewLine);
            reply = String.Format("&{0}", value.ToString("E11"));
            cmdport.WriteLine(reply);
            cmdport.Write(cmdport.NewLine);
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
                    if(data != '\r' && data != '\n')
                    cmdbuffer.Append(data);
                }
            }
        }
        static public void ScanPort()
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
                            reading = -9999;
                        }
                    }
                    if(inbuffer.Length > 0)
                        inbuffer.Remove(0, inbuffer.Length);
                }
            }
        }
        static public bool IsInAction()
        {
            if(actmsg.action == "" || actmsg.action == null)
                return false;
            if (!OverEvent.WaitOne(30,false))
                return true;
            OverEvent.Reset();
            if ((actmsg.action != "retry") && (actmsg.action != "fail"))
            {
                actmsg.action = "";
                return false;
            }
            if (actmsg.action == "retry")
            {
                throw new Exception(StringResource.str("tryagain"));
            }
            if (actmsg.action == "fail")
            {
                throw new Exception("Critical Action failed.");
            }
            return false;
        }
        static public void Action(string cmd, Int32 param)
        {
            if (param < 0)
                Action(cmd, 0);
            else
                Action(cmd, Convert.ToUInt32(param));
        }
        static public void Action(string cmd, UInt32 param)
        {
            if (cmd == "")
                throw new Exception("Unexpected command");
            actmsg.action = cmd;
            actmsg.addr = 0;
            actmsg.value = param;
            WaitEvent.Set();
//            OverEvent.WaitOne();
        }
        //k1 = RS/RX, DVM
        //ktt = ON/OFF
        //curr = CURR_P001|CURR_P01/P1/P3/1/5 ; 1mA/10mA,0.1A,0.3A,1A,5A
        static private string oldswi = ""; //old measurement switch satus
        static private string oldktt = "";
        static private string oldcurr = "";
        static private string oldsqr = "";
        static private UInt32 oldk = 0;
        static private string oldconstv = "";
        static private string oldstd = "";
        static public string[] constvtbl = new string[] { "", "VMODE_3V", "VMODE_10V", "VMODE_30V","VMODE_10VHIGH","VMODE_30VHIGH" };
        static public string[] stdtbl = new string[] { "",  "STD_P001", "STD_P01", "STD_P1","STD_1" };
        static public string[] meastbl = new string[] { "", "MEAS_RS","MEAS_RX","MEAS_DELTA" };
        static public string[] currtbl = new string[] { "", "CURR_OFF", "CURR_P0001","CURR_P001", "CURR_P01", "CURR_P1", "CURR_P3", "CURR_1", "CURR_5"};
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
        //compose string of curr, sqr std, constv
        public static string ComposeAction(RxInfo myRx)
        {
            string ret = "";
            switch (myRx.iIx)
            {
                case -1: ret = ret + "CURR_P0001"; break; //0.1mA
                case 0: ret = ret + "CURR_P001"; break; //1mA
                case 1: ret = ret + "CURR_P01"; break; //10mA
                case 2: ret = ret + "CURR_P1"; break; //0.1A
                case 3: ret = ret + "CURR_P3"; break; //0.3A
                case 4: ret = ret + "CURR_1"; break; //1A
                case 5: ret = ret + "CURR_5"; break; //5A

            }
            ret = ret + "|";
            if (myRx.bSqrt)
                ret = ret + "X2|";
            else
                ret = ret + "X1|";

            if (myRx.iIx == 3) //0.3A
                ret = ret + "STD_P1|VMODE_3V";
            else if (myRx.iIx == 4) //1A
                ret = ret + "STD_P01|VMODE_3V";
            else if (myRx.iIx == 5) //5A
                ret = ret + "STD_P001|VMODE_3V";
            else
                ret = ret + "STD_1|VMODE_3V";

            return ret;
        }
        static public void KState(UInt32 k)
        {
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
            RelayState("MEAS_RS", "KTTP", "CURR_OFF", "X1", "STD_1", "VMODE_3V");
            Action("navto1v", 0);
        }
        static public void RelayState(string swi, string ktt, string curr, string sqr, string std, string constv)
        {
            if (((swi == oldswi) || (swi == "")) && 
                ((ktt == oldktt) || (ktt == "")) &&
                ((curr == oldcurr) || (curr == "")) &&
                ((sqr == oldsqr) || (sqr == "")) &&
                ((std == oldstd) || (std == "")) &&
                ((constv == oldconstv) || (constv == "")))
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
                    //std = "STD_OFF";
                }
                oldswi = swi;
            }
            if (ktt.StartsWith("KTT"))
            {
                if(ktt == "KTTN")
                    DelayWrite(new byte[] { 0x06 }, 0, 1);
                if (ktt == "KTTP")
                    DelayWrite(new byte[] { 0x0F }, 0, 1);
                oldktt = ktt;
            }
            if (sqr.StartsWith("X"))
            {
                if ((curr == "CURR_P3") || (curr == "CURR_1") || (curr == "CURR_5"))
                {
                    DelayWrite(new byte[] { 0x24 }, 0, 1);
                    if (sqr == "X2")
                        DelayWrite(new byte[] { 0x2A }, 0, 1); //todo?
                    if (sqr == "X1")
                        DelayWrite(new byte[] { 0x23 }, 0, 1); //todo?
                }
                else
                {
                    DelayWrite(new byte[] { 0x2D }, 0, 1);
                    if (sqr == "X2")
                        DelayWrite(new byte[] { 0x1D }, 0, 1);
                    if (sqr == "X1")
                        DelayWrite(new byte[] { 0x14 }, 0, 1);
                }
                oldsqr = sqr;
            }
            if (curr.StartsWith("CURR_"))
            {
                byte[] cb = new byte[] { 0x3E, 0x3F, 0x3C, 0x34, 0x0D, 0x0C, 0x0E }; //1mA
                if (curr == "CURR_P0001") cb[0] = 0x37; //0.1mA
                if (curr == "CURR_P001") cb[1] = 0x36; //1mA
                if (curr == "CURR_P01") cb[2] = 0x35; //10mA
                if (curr == "CURR_P1") cb[3] = 0x3d; //0.1A
                if (curr == "CURR_P3") cb[4] = 0x04; //0.3A
                if (curr == "CURR_1") cb[5] = 0x05; //1A
                if (curr == "CURR_5") cb[6] = 0x07; //5A
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
                if (std == "STD_OFF")
                    DelayWrite(new byte[] { 0x02, 0x33, 0x21, 0x32 }, 0, 4);
                if (std == "STD_1")
                    DelayWrite(new byte[] { 0x0B, 0x33, 0x21, 0x32 }, 0, 4);
                if (std == "STD_P1")
                    DelayWrite(new byte[] { 0x3A, 0x02, 0x21, 0x32 }, 0, 4);
                if (std == "STD_P01")
                    DelayWrite(new byte[] { 0x28, 0x02, 0x33, 0x32 }, 0, 4);
                if (std == "STD_P001")
                    DelayWrite(new byte[] { 0x3B, 0x02, 0x33, 0x21,  }, 0, 4);
            }
            DelayWrite(UsTail, 0, 1);
        }
        static public bool Loaded = false;
        static public void MessageHandler()
        {
            Reset();
            Loaded = true;
            while (true)
            {
                if (!WaitEvent.WaitOne(10, false))
                {
                    ScanPort();
                    continue;
                }
                WaitEvent.Reset();
                try
                {
                    #region navmeter action
                    ActMessage msg = DeviceMgr.actmsg;
                    if (msg.action == "navto30v" || msg.action == "navto1v" || msg.action == "navto120mv" || msg.action == "navto10mv")
                    {
                        if (msg.action == "navto1v")
                        {
//                            port.Write(NAV_INIT); //init
                            port.Write(StringResource.str("NAV_1V_"+RunWnd.syscfg.sNavmeter)); //1v
//                            DelayWrite(NAV_AFLTON); //slowmode and aflton
                        }
                        if (msg.action == "navto120mv")
                        {
                            port.Write(StringResource.str("NAV_120MV_" + RunWnd.syscfg.sNavmeter)); //120mv
                        }
                        if (msg.action == "navto10mv")
                        {
                            port.Write(StringResource.str("NAV_10MV_" + RunWnd.syscfg.sNavmeter)); //10mv
                        }
                        if (msg.action == "navto30v")
                        {
                            port.Write(StringResource.str("NAV_30V_" + RunWnd.syscfg.sNavmeter)); //10mv
                        }

                        Thread.Sleep(3000);
                        actmsg.action = "done";
                        OverEvent.Set();
                        continue;
                    }
                    if (msg.action == "navread")
                    {
                        reading = -9999;
                        port.DiscardInBuffer();
                        port.Write(StringResource.str("NAV_READ_" + RunWnd.syscfg.sNavmeter)); //1v
                        Thread.Sleep(5);
                        int timeout = 400;
                        while ((timeout-- > 0) && (reading < -9000))
                        {
                            ScanPort();
                            Thread.Sleep(5);
                        }
                        actmsg.action = "done";
                        OverEvent.Set();
                        continue;
                    }
                    if (msg.action == "turnk")
                    {
                        KState(msg.value);
                        actmsg.action = "done";
                        OverEvent.Set();
                        continue;
                    }
                    if (msg.action.StartsWith("MEAS")) 
                    {
                        string[] cmd = msg.action.Split(new char[]{'|'});
                        if (cmd.Length == 6)
                        {
                            RelayState(cmd[0],cmd[1], cmd[2], cmd[3], cmd[4], cmd[5]);
                        }
                        actmsg.action = "done";
                        OverEvent.Set();
                        continue;
                    }
                    #endregion
                    if(actmsg.action != "")
                    throw new Exception("Invalid command " + actmsg.action);
                }
                catch (Exception ex)
                {
                    SysLog(ex.Message);
                    Debug.WriteLine(ex.Message);
                    actmsg.action = "fail";
                    OverEvent.Set();
                }
            }
        }
    }
}
