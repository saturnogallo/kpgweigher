using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Threading;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
namespace Zddq2
{

    public partial class RunWnd : Form
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct SystemTime
        {
            public ushort wYear;
            public ushort wMonth;
            public ushort wDayOfWeek;
            public ushort wDay;
            public ushort wHour;
            public ushort wMinute;
            public ushort wSecond;
            public ushort wMiliseconds;
        }
        [DllImport("coredll")]
        public static extern bool SetLocalTime(ref SystemTime sysTime); //设置本地时

        public const int win_width = 1024;
        public const int win_height = 768;
        public const int bar_height = 70;
        public TaskMachine task;

        private int total_validRx = 0;  //total number of valid Rx Number
        private int page_currRx = 0;    //current page index
        public static SysConfig syscfg;

        public int selectedRx = 0;
        public int selectedRs = 0;
        private Queue<string> lastcalls;//buffer of calls
        //interface for lastcall buffer io
        private string lastcall
        {
            get //peek the lastcall
            {
                if (lastcalls.Count == 0)
                    return "";
                else
                    return lastcalls.Peek();
            }
            set
            {
                if (value == "")    //throw away the top call
                {
                    if (lastcalls.Count > 0)
                        lastcalls.Dequeue();
                }
                else
                {
                    if (!lastcalls.Contains(value)) //no duplicate calls in the list
                        lastcalls.Enqueue(value);
                }
            }
            
        }
        private System.Windows.Forms.Timer uiTimer;
        static RunWnd()
        {
            syscfg = new SysConfig();
//			ShowCursor(0);

        }
        //H is reset, S is stop
        private Regex reg_mode = new Regex("G([0-9]+)");
        private Regex reg_statistic = new Regex("J([0-9]+)");
        private Regex reg_filter = new Regex("F([0-9]+)");
        private Regex reg_ix = new Regex("I([0-9.]+)"); //mA
        private Regex reg_delay = new Regex("T([0-9]+)");
        private Regex reg_switch = new Regex("W([0-9]+)");
        private Regex reg_alpha = new Regex("ALPHA([0-9]+)");
        private Regex reg_beta = new Regex("BETA([0-9]+)");
        private Regex reg_rxid = new Regex("RX_(.*)");
        private Regex reg_rsid = new Regex("RS_(.*)");
        private Regex reg_temp = new Regex("TEMP([0-9]+)");
        private Regex reg_meastimes = new Regex("M([0-9]+)");
        private Regex reg_sampletimes = new Regex("D([0-9]+)"); //sample times for single measurements
        private Regex reg_measure = new Regex("D([0-9]+)Measurements:");
        //return formats DxxMeasurements:R1.1234...,S1.23434, #2,&1.123456789ABCD+000
        private Regex reg_rsa = new Regex("A([0-9.eE\\+\\-]+)");
        private Regex reg_rsb = new Regex("B([0-9.eE\\+\\-]+)");
        private string oper_mode = "";
        public void pc_cmd(string cmd)
        {
            DeviceMgr.SysLog(cmd);
            if (cmd == "ECHO")
            {
                DeviceMgr.Resend();
                return;
            }
            if (cmd == "S")
            {
                if (task.bRunning)
                {
                    statusBar1_StartStopClick(null, null); //click stop
                }
                return;
            }

            if (cmd == "H")
            {
                if((!task.bRunning) && (!DeviceMgr.IsInAction()))
                {
                    DeviceMgr.Reset();//reset 
                }
                return;
            }
            Match m;
            m = reg_mode.Match(cmd);
            if (m.Success)
            {
                oper_mode = m.Groups[1].Value;
                return;
            }
            m = reg_statistic.Match(cmd);
            if (m.Success)
            {
                syscfg.iFilter = Convert.ToInt32(m.Groups[1].Value);
                return;
            }
            m = reg_filter.Match(cmd);
            if (m.Success)
            {
                if(m.Groups[1].Value == "2")
                    syscfg.sFilterType = "filtertype3";
                else if (m.Groups[1].Value == "1")
                    syscfg.sFilterType = "filtertype2";
                else if (m.Groups[1].Value == "3")
                    syscfg.sFilterType = "filtertype4";
                else 
                    syscfg.sFilterType = "filtertype1";
                return;
            }
            m = reg_ix.Match(cmd);
            if (m.Success)
            {
                #region current convert
                double myIx = double.Parse(m.Groups[1].Value)/1000;
                if (myIx >= 5.5)
                {
                    Program.lst_rxinfo[0].iIx = 5; //5A x 2
                    Program.lst_rxinfo[0].bSqrt = true;
                    return;
                }
                if (myIx >= 2.2)
                {
                    Program.lst_rxinfo[0].iIx = 5; //5A
                    Program.lst_rxinfo[0].bSqrt = false;
                    return;
                }
                if (myIx >= 1.1)
                {
                    Program.lst_rxinfo[0].iIx = 4; //1A x 2
                    Program.lst_rxinfo[0].bSqrt = true;
                    return;
                }
                if (myIx >= 0.66)
                {
                    Program.lst_rxinfo[0].iIx = 4; //1A x 2
                    Program.lst_rxinfo[0].bSqrt = false;
                    return;
                }
                if (myIx >= 0.33)
                {
                    Program.lst_rxinfo[0].iIx = 3; //0.3A x 2
                    Program.lst_rxinfo[0].bSqrt = true;
                    return;
                }
                if (myIx >= 0.22)
                {
                    Program.lst_rxinfo[0].iIx = 3; //0.3A
                    Program.lst_rxinfo[0].bSqrt = false;
                    return;
                }
                if (myIx >= 0.11)
                {
                    Program.lst_rxinfo[0].iIx = 2; //0.1A x 2
                    Program.lst_rxinfo[0].bSqrt = true;
                    return;
                }
                if (myIx >= 0.022)
                {
                    Program.lst_rxinfo[0].iIx = 2; //0.1A
                    Program.lst_rxinfo[0].bSqrt = false;
                    return;
                }
                if (myIx >= 0.011)
                {
                    Program.lst_rxinfo[0].iIx = 1; //10mA x 2
                    Program.lst_rxinfo[0].bSqrt = true;
                    return;
                }
                if (myIx >= 0.0022)
                {
                    Program.lst_rxinfo[0].iIx = 1; //10mA
                    Program.lst_rxinfo[0].bSqrt = false;
                    return;
                }

                if (myIx >= 0.0011)
                {
                    Program.lst_rxinfo[0].iIx = 0; //1mA x 2
                    Program.lst_rxinfo[0].bSqrt = true;
                    return;
                }
                if (myIx >= 0.00022)
                {
                    Program.lst_rxinfo[0].iIx = 0; //1mA
                    Program.lst_rxinfo[0].bSqrt = false;
                    return;
                }
                if (myIx >= 0.00011)
                {
                    Program.lst_rxinfo[0].iIx = -1; //0.1mA x 2
                    Program.lst_rxinfo[0].bSqrt = true;
                    return;
                }
                {
                    Program.lst_rxinfo[0].iIx = -1; //0.1mA
                    Program.lst_rxinfo[0].bSqrt = false;
                    return;
                }
#endregion
                
            }
            m = reg_delay.Match(cmd);
            if (m.Success)
            {
                syscfg.iMeasDelay = Convert.ToInt32(m.Groups[1].Value);
                return;
            }
            m = reg_meastimes.Match(cmd);
            if (m.Success)
            {
                syscfg.iMeasTimes = Convert.ToInt32(m.Groups[1].Value);
                return;
            }
            m = reg_switch.Match(cmd);
            if (m.Success)
            {
                syscfg.iKTT = Convert.ToInt32(m.Groups[1].Value);
                return;
            }
            m = reg_measure.Match(cmd);
            if (m.Success)
            {
                syscfg.iMeasTimes = Convert.ToInt32(m.Groups[1].Value);
                RsInfo rs = Program.lst_rsinfo[Program.mainwnd.selectedRs];
                RxInfo rx = Program.lst_rxinfo[Program.mainwnd.selectedRx];
                if (!task.bRunning)
                {
                    statusBar1_StartStopClick(null, null); //click start
                }
                return;
            }
            m = reg_sampletimes.Match(cmd);
            if (m.Success)
            {
                syscfg.iSampleTimes = Convert.ToInt32(m.Groups[1].Value);
                return;
            }
             m = reg_alpha.Match(cmd);
             if (m.Success)
             {
                 Program.lst_rsinfo[0].iRRange = ActionMgr.RNG_INVALID;
                 Program.lst_rsinfo[0].dAlpha = Convert.ToDouble(m.Groups[1].Value);
                 return;
             }
             m = reg_beta.Match(cmd);
             if (m.Success)
             {
                 Program.lst_rsinfo[0].iRRange = ActionMgr.RNG_INVALID;
                 Program.lst_rsinfo[0].dBeta = Convert.ToDouble(m.Groups[1].Value);

                 return;
             }
             m = reg_temp.Match(cmd);
             if (m.Success)
             {
                 syscfg.dTemp = Convert.ToDouble(m.Groups[1].Value);
                 return;
             }
            m = reg_rsa.Match(cmd);
            if (m.Success)
            {
                Program.lst_rsinfo[0].iRRange = ActionMgr.RNG_INVALID;
                Program.lst_rsinfo[0].dValue = Convert.ToDouble(m.Groups[1].Value);
                Program.lst_rxinfo[0].cStdChan = 'A';
                return;
            }
            m = reg_rsid.Match(cmd);
            if (m.Success)
            {
                Program.lst_rsinfo[0].iRRange = ActionMgr.RNG_INVALID;
                Program.lst_rsinfo[0].sSerial = m.Groups[1].Value;
                return;
            }
            m = reg_rxid.Match(cmd);
            if (m.Success)
            {
                Program.lst_rxinfo[0].sSerial = m.Groups[1].Value;
                return;
            }

            m = reg_rsb.Match(cmd);
            if (m.Success)
            {
                Program.lst_rxinfo[0].dRxInput = Convert.ToDouble(m.Groups[1].Value);
                ActionMgr.SetIxRange(0, Program.lst_rxinfo[0].dRxInput,false);
                Program.lst_rxinfo[0].cStdChan = 'B';
                return;
            }
        }

        public void KbdData(string param, string data)
        {
        }
        private static bool tmlock = false; //lock for timer handler
        void uiTimer_Tick(object sender, EventArgs e)
        {
            if (tmlock)
                return;
            tmlock = true;
            if (DeviceMgr.Loaded && statusBar1.status == RUN_STATE.DISABLED)
                statusBar1.status = RUN_STATE.IDLE;
            if (lastcall != "")
            {
                if (lastcall == "newtime")
                {
                    lastcall = "";
                    Program.kbd.Init(StringResource.str("enter_newtime"), "newtime", false, KbdData);
                }
                tmlock = false;
                return;
            }
            else
            {
                if (task.bRunning)
                {
                    task.Step();
                    statusBar1.pause = task.bPaused;
                    statusBar1.waiting = task.waiting;
                    if (!task.bRunning)
                    {
                        statusBar1.status = RUN_STATE.IDLE;
                        DeviceMgr.Log("测量结束...");
                    }
                }
            }
            tmlock = false;
        }
        public RunWnd()
        {
            InitializeComponent();
            task = new TaskMachine();
            task.StatusChanged += new StrEventHandler(UpdateStatusBar);
            lastcalls = new Queue<string>();
            uiTimer = new System.Windows.Forms.Timer();
            uiTimer.Tick += new EventHandler(uiTimer_Tick);
            uiTimer.Interval = 300; //250ms for UI update
            uiTimer.Enabled = true; ;

            rxDisplay1.Click += new EventHandler(rx_display_Click);
            rxDisplay2.Click += new EventHandler(rx_display_Click);
            rxDisplay3.Click += new EventHandler(rx_display_Click);
            rxDisplay4.Click += new EventHandler(rx_display_Click);

            
            
            statusBar1.Top = win_height - bar_height;
            statusBar1.Left = 0;
            statusBar1.Size = new Size(win_width,bar_height);
            statusBar1.Width = win_width;
            statusBar1.Height = bar_height;
            statusBar1.MenuClick += new EventHandler(statusBar1_MenuClick);
            statusBar1.StartStopClick += new EventHandler(statusBar1_StartStopClick);
            statusBar1.PauseResumeClick += new EventHandler(statusBar1_PauseResumeClick);
            ArrangeWnd();
            rxDisplay1.ReDraw(true);
            this.Load += new EventHandler(RunWnd_Load);
        }

        void statusBar1_PauseResumeClick(object sender, EventArgs e)
        {
            task.PauseResume();
        }

        void RunWnd_Load(object sender, EventArgs e)
        {
            statusBar1.status = RUN_STATE.DISABLED;
            Rectangle rect = new Rectangle();
            FullScreenClass.SetFullScreen(true, ref rect);
        }
        void UpdateStatusBar(object sender, string msg)
        {
            if (msg == "newvalue")
            {
                RX_VAR rv = task.currRx.var;
                rxDisplay1.AddValue(task.iMeasCnt,rv.rRx);
                DeviceMgr.ReportData(task.iMeasCnt,rv.rRx);
                rxDisplay1.LogData(task.iMeasCnt, rv.rRx);
                statusBar1.total = syscfg.iMeasTimes;
                statusBar1.count = task.iMeasCnt;
            }
            if (msg == "complete")
            {
                if (RunWnd.syscfg.bThrow)
                    rxDisplay1.ThrowData();
                rxDisplay1.LogComplete();
            }
            statusBar1.status = task.currRx.i_State;
            statusBar1.message = task.currRx.var.errMsg;
        }
        void rx_display_Click(object sender, EventArgs e)
        {
            
            //selectedRx = 0; // (page_currRx * syscfg.iDisplay) + Convert.ToInt32((sender as RxDisplay).Name.Remove(0, 10)) - 1;
            //Program.SwitchWindow("rxconfig");
        }

        void statusBar1_StartStopClick(object sender, EventArgs e)
        {
            if (task.bRunning)
            {
                statusBar1.pause = false;
                statusBar1.status = RUN_STATE.STOPPING;                
                task.Stop();
                try
                {
                    rxDisplay1.LogComplete();
                }
                catch
                {
                }
            }
            else
            {
                RsInfo rs = Program.lst_rsinfo[Program.mainwnd.selectedRs];
                RxInfo rx = Program.lst_rxinfo[Program.mainwnd.selectedRx];
                statusBar1.total = syscfg.iMeasTimes;
                statusBar1.count = 1;
                statusBar1.status = RUN_STATE.INITIALIZING;
                
                task.Start();
                rxDisplay1.ClearAll();
                rxDisplay2.ClearAll();
                rxDisplay3.ClearAll();
                rxDisplay4.ClearAll();
            }
        }

        void statusBar1_MenuClick(object sender, EventArgs e)
        {
            Program.SwitchWindow("sysconfig");
        }
        public void PageDown()  //turn to next page
        {
            if (total_validRx <= ((page_currRx + 1) * syscfg.iDisplay))
                page_currRx = page_currRx + 1;
        }
        public void ReDraw(bool newmode)
        {
            rxDisplay1.ReDraw(newmode);
            rxDisplay2.ReDraw(newmode);
            rxDisplay3.ReDraw(newmode);
            rxDisplay4.ReDraw(newmode);
        }
        public void PageUp()    //turn to last page
        {
            if (page_currRx > 0)
                page_currRx = page_currRx - 1;
            else
                page_currRx = total_validRx / syscfg.iDisplay;
        }
        //Link RxInfo to rxdisplay based on current index
        public void RefreshDisplays()
        {
            Program.lst_rxinfo.Sort();
            total_validRx = Program.lst_rxinfo.Count(rx => rx.bEnabled);
            if(total_validRx > 24)
                total_validRx = 24;
            if (syscfg.iDisplay == 1)
                rxDisplay1.myRx = Program.lst_rxinfo[page_currRx];
            if (syscfg.iDisplay == 2)
            {
                rxDisplay1.myRx = Program.lst_rxinfo[2 * page_currRx];
                rxDisplay2.myRx = Program.lst_rxinfo[2 * page_currRx + 1];
            }
            if (syscfg.iDisplay == 4)
            {
                rxDisplay1.myRx = Program.lst_rxinfo[4 * page_currRx];
                rxDisplay2.myRx = Program.lst_rxinfo[4 * page_currRx + 1];
                rxDisplay3.myRx = Program.lst_rxinfo[4 * page_currRx + 2];
                rxDisplay4.myRx = Program.lst_rxinfo[4 * page_currRx + 3];
            }
            
        }
        //Arrange window by different display mode
        private void ArrangeWnd()
        {
            int dispmode = syscfg.iDisplay;
            int xdiv = 1;
            int ydiv = 1;
            if (dispmode == 2)
            {
                xdiv = 1;
                ydiv = 2;
            }
            if (dispmode == 4)
            {
                xdiv = 2;
                ydiv = 2;
            }

            rxDisplay1.Top = 0;
            rxDisplay1.Left = 0;
            rxDisplay1.Width = win_width / xdiv;
            rxDisplay1.Height = (win_height - bar_height) / ydiv;
            rxDisplay1.AdjustPanel();
            rxDisplay1.Visible = true;

            if (dispmode < 2)
            {
                rxDisplay2.Visible = false;
                rxDisplay3.Visible = false;
                rxDisplay4.Visible = false;
                return;
            }
            if (dispmode == 2)
            {
                rxDisplay2.Top = (win_height - bar_height) / ydiv;
                rxDisplay2.Left = 0;
            }else{
                rxDisplay2.Top = 0;
                rxDisplay2.Left = win_width / xdiv;
            }
            rxDisplay2.Width = win_width / xdiv;
            rxDisplay2.Height = (win_height - bar_height) / ydiv;
            rxDisplay2.AdjustPanel();
            rxDisplay2.Visible = true;

            if (dispmode <= 2)
            {
                rxDisplay3.Visible = false;
                rxDisplay4.Visible = false;
                return;
            }
            rxDisplay3.Top = (win_height - bar_height) / ydiv;
            rxDisplay3.Left = 0;
            rxDisplay3.Width = win_width / xdiv;
            rxDisplay3.Height = (win_height - bar_height) / ydiv;
            rxDisplay3.AdjustPanel();
            rxDisplay3.Visible = true;

            rxDisplay4.Top = (win_height - bar_height) / ydiv;
            rxDisplay4.Left = win_width / xdiv;
            rxDisplay4.Width = win_width / xdiv;
            rxDisplay4.Height = (win_height - bar_height) / ydiv;
            rxDisplay4.AdjustPanel();
            rxDisplay4.Visible = true;
        }
        
    }
}
