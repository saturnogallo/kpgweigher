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

        public const int win_width = 800;
        public const int win_height = 600;
        public const int bar_height = 70;
        private TaskMachine task;

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
        private Regex reg_meastimes = new Regex("M([0-9]+)");
        private Regex reg_sampletimes = new Regex("D([0-9]+)"); //sample times for single measurements
        private Regex reg_measure = new Regex("D([0-9]+)Measurements:");
        //return formats DxxMeasurements:R1.1234...,S1.23434, #2,&1.123456789ABCD+000
        private Regex reg_rsa = new Regex("A([0-9.eE\\+\\-]+)");
        private Regex reg_rsb = new Regex("B([0-9.eE\\+\\-]+)");
        private string oper_mode = "";
        public void pc_cmd(string cmd)
        {
            if (cmd == "S")
            {
                task.Stop();//STOP; need reply //todo
                return;
            }
            if (cmd == "H")
            {
//                DeviceMgr.Reset();//reset 
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
                else 
                    syscfg.sFilterType = "filtertype1";
                return;
            }
            m = reg_ix.Match(cmd);
            if (m.Success)
            {/*
                //todo
                if (currRx.iIx == 0) //1mA
                if (currRx.iIx == 1) //10mA
                if (currRx.iIx == 2) //0.1A
                if (currRx.iIx == 3) //0.3A
                if (currRx.iIx == 4) //1A
                if (currRx.iIx == 5) //5A
                    */
                double myIx = double.Parse(m.Groups[1].Value)/1000;
                if(myIx >= 3)
                    Program.lst_rxinfo[0].iIx = 5;
                else if (myIx >= 0.65)
                    Program.lst_rxinfo[0].iIx = 4;
                else if (myIx >= 0.2)
                    Program.lst_rxinfo[0].iIx = 3;
                else if (myIx >= 0.05)
                    Program.lst_rxinfo[0].iIx = 2;
                else if (myIx >= 0.005)
                    Program.lst_rxinfo[0].iIx = 1;
                else if (myIx >= 0.0005)
                    Program.lst_rxinfo[0].iIx = 0;
                else
                    Program.lst_rxinfo[0].iIx = -1;
                return;
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
            m = reg_sampletimes.Match(cmd);
            if (m.Success)
            {
                syscfg.iSampleTimes = Convert.ToInt32(m.Groups[1].Value);
                return;
            }
            m = reg_measure.Match(cmd);
            if (m.Success)
            {
                syscfg.iMeasTimes = Convert.ToInt32(m.Groups[1].Value);
                task.Start();
                return;
            }
            m = reg_rsa.Match(cmd);
            if (m.Success) //A mode
            {
                Program.lst_rsinfo[0].dValue = Convert.ToDouble(m.Groups[1].Value);
                Program.lst_rxinfo[0].cStdChan = 'A';
                DeviceMgr.ReportHeader(syscfg.iMeasTimes);
                for (int i = 0; i < syscfg.iMeasTimes; i++)
                {
                    DeviceMgr.ReportData(i+1,10.01234566);
                    Thread.Sleep(1000);
                }
                return;
            }
            m = reg_rsb.Match(cmd);
            if (m.Success) //B mode
            {
                Program.lst_rsinfo[0].dValue = Convert.ToDouble(m.Groups[1].Value);
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
                        DeviceMgr.Log("Measurement Stopped...");
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
            uiTimer.Interval = 250; //250ms for UI update
            uiTimer.Enabled = true; ;

            rxDisplay1.Click += new EventHandler(rx_display_Click);
            rxDisplay2.Click += new EventHandler(rx_display_Click);
            rxDisplay3.Click += new EventHandler(rx_display_Click);
            rxDisplay4.Click += new EventHandler(rx_display_Click);

            SetGridStyle();

            //dataGrid2.DataSource = Program.data.dt;
            dataGrid2.Click += new EventHandler(dataGrid2_Click);

            graphPane2.XAxisTicCount = 10;
            graphPane2.YAxisTicCount = 4;
            graphPane2.YAxisMax = 5;
            graphPane2.YAxisMin = -5;
            graphPane2.YAxisTitle = "PPM";
            graphPane2.XAxisTicCount = 10;
            /*
            for (int j = 0; j < 25;j++ )
                AddValue(j+1, 100152.34-j);
            ThrowData();
             */
            graphPane2.Invalidate();
            
            
            statusBar1.Top = win_height - bar_height;
            statusBar1.Left = 0;
            statusBar1.Size = new Size(win_width,bar_height);
            statusBar1.Width = win_width;
            statusBar1.Height = bar_height;
            statusBar1.MenuClick += new EventHandler(statusBar1_MenuClick);
            statusBar1.StartStopClick += new EventHandler(statusBar1_StartStopClick);
            statusBar1.PauseResumeClick += new EventHandler(statusBar1_PauseResumeClick);
            ArrangeWnd();
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
//            FullScreenClass.SetFullScreen(true, ref rect);
        }
        void UpdateStatusBar(object sender, string msg)
        {
            if (msg == "newvalue")
            {
                /*
                RX_VAR rv = task.currRx.var;
                rxDisplay1.AddValue(task.iMeasCnt, rv.rRx, 0,0);
                DeviceMgr.ReportData(task.iMeasCnt,rv.rRx);
                rxDisplay1.LogData(task.iMeasCnt, rv.rRx);
                statusBar1.total = syscfg.iMeasTimes;
                statusBar1.count = task.iMeasCnt;
                 */
            }
            if (msg == "complete")
            {
                /*
                if (RunWnd.syscfg.bThrow)
                    rxDisplay1.ThrowData();
                rxDisplay1.LogComplete();
                 */
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
                
            }
            else
            {
                RsInfo rs = Program.lst_rsinfo[Program.mainwnd.selectedRs];
                RxInfo rx = Program.lst_rxinfo[Program.mainwnd.selectedRx];
                statusBar1.total = syscfg.iMeasTimes;
                statusBar1.count = 1;
                statusBar1.status = RUN_STATE.INITIALIZING;
                DeviceMgr.Log(String.Format(@"==== New Measurement Paramter========
                                            Measure times = {0}\r\nRs ID = {1}\r\nRs Value = {2}\r\nRs Temp. Alpha = {3}\r\nRs Temp. Beta = {4}\r\n
                                            Temperature = {5}\r\nRx ID = {6}\r\nSample times = {7}\r\nSwitch delay = {8}\r\n
                                            Filter = {9}\r\nFilter Length = {10}\r\nMeasure delay = {12}\r\n
                                            AutoFilter = {13}\r\nRx curr. = {14}\r\n
                                            New Measurement Started...", syscfg.iMeasTimes.ToString(), rs.sSerial, rs.dValue.ToString("F8"),rs.dAlpha.ToString("F3"),rs.dBeta.ToString("F3"),
                                                                       syscfg.dTemp.ToString("F3"),rx.sSerial,syscfg.iSampleTimes, syscfg.iKTT, 
                                                                       syscfg.sFilterType, syscfg.iFilter, syscfg.iMeasDelay, syscfg.bThrow, rx.iIx                                       
                                            ));
                task.Start();
                Program.data.ClearAll();
                foreach (RxDisplay rxdisp in Enumerable.Range(1, 4).Select(x => findrx(x)))
                {
                    rxdisp.ReDraw();
                }
            }
        }
        private RxDisplay findrx(int i)
        {
            return Controls.Find(String.Format("rxDisplay{0}", i), false)[0] as RxDisplay;
        }
        void statusBar1_MenuClick(object sender, EventArgs e)
        {

            Program.choice.Init("Test", new string[] { "choice1", "choice2", "choice1", "choice2", "choice1", "choice8", "choice1", "choice8" }, 1, null, new KbdDataHandler((a, b) => { }));
            //Program.SwitchWindow("sysconfig");
        }
        public void PageDown()  //turn to next page
        {
            if (total_validRx <= ((page_currRx + 1) * syscfg.iDisplay))
                page_currRx = page_currRx + 1;
        }
        public void ReDraw(bool newmode)
        {
            rxDisplay1.ReDraw();
            rxDisplay2.ReDraw();
            rxDisplay3.ReDraw();
            rxDisplay4.ReDraw();

            if (newmode)
            {
                /*
                lbl_aux1.Top = this.ClientRectangle.Top + 50;
                lbl_aux1.Left = this.ClientRectangle.Left + 10;
                btn_aux1.Top = lbl_aux1.Top;
                btn_aux1.Left = lbl_aux1.Left + 80;

                lbl_aux2.Top = this.ClientRectangle.Top + 50;
                lbl_aux2.Left = this.ClientRectangle.Left + 110;
                btn_aux2.Top = lbl_aux2.Top;
                btn_aux2.Left = lbl_aux2.Left + 80;

                lbl_reading.Top = this.ClientRectangle.Top + 10;
                lbl_reading.Left = this.lbl_reading.Left + 20;
                btn_bigdisp.Top = lbl_reading.Top;
                btn_bigdisp.Left = lbl_reading.Left + 200;
                
                if (config.iDataMode == RXDISP_MODE.GRAPH)
                {
                    dataGrid2.Visible = false;
                    graphPane2.Top = dataGrid2.Top;
                    graphPane2.Height = 340;
                    graphPane2.Visible = true;
                    graphPane2.Refresh();
                }
                if (config.iDataMode == RXDISP_MODE.GRID)
                {
                    graphPane2.Visible = false;
                    dataGrid2.Height = 340;
                    dataGrid2.Visible = true;
                }
                if (config.iDataMode == RXDISP_MODE.MIXED)
                {
                    dataGrid2.Height = 170;
                    dataGrid2.Visible = true;
                    graphPane2.Top = dataGrid2.Top + dataGrid2.Height + 5;
                    graphPane2.Height = 170;
                    graphPane2.Visible = true;
                    graphPane2.Refresh();
                }
                 */
                //arrange dialogs
            }

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
        #region grid related functions

        void dataGrid2_Click(object sender, EventArgs e)
        {
            if (dataGrid2.CurrentCell.ColumnIndex == 4)
            {
                if (dataGrid2.CurrentCell.RowIndex >= 0)
                {
                    /*
                    PointPair pp = graphPane2.PointList[dataGrid2.CurrentCell.RowIndex];

                    if (pp.excluded)
                        dt.Rows[dataGrid2.CurrentCell.RowIndex]["excluded"] = "";
                    else
                        dt.Rows[dataGrid2.CurrentCell.RowIndex]["excluded"] = "【√】";
                    pp.excluded = !pp.excluded;

                    graphPane2.PointList[dataGrid2.CurrentCell.RowIndex] = pp;
                     */
                    ReDraw(true);
                }
            }
        }
        private void SetGridStyle()
        {
            DataGridTableStyle tblStyle = new DataGridTableStyle();

            tblStyle.MappingName = "values";
            string[] cols = new string[] { "ch", "id","excluded","data", "ratio", "temp", "time" };
            int[] colwidth = new int[]{50,50,50,150,150,150, 150};
            for(int i=0;i<cols.Length;i++)
            {
                DataGridColumnStyle dgcs = new DataGridTextBoxColumn();
                dgcs.MappingName = cols[i];
                dgcs.HeaderText = StringResource.str("col_"+cols[i]);
                dgcs.Width = colwidth[i];
                dgcs.NullText = string.Empty;
                tblStyle.GridColumnStyles.Add(dgcs);
            }
            //dataGridView1.TableStyles.Add(tblStyple)
        }
        #endregion

        #region graphpane related function
        //throw top 30% data and bottom 30% data;
        public void ThrowData()
        {/*
            List<PointPair> tempList = new List<PointPair>(this.graphPane2.PointList);
            tempList.Sort();
            int skip = (int)Math.Floor(tempList.Count * 0.3);
            if (skip > 0)
            {
                while (skip-- > 0)
                {
                    dt.Rows.RemoveAt(0);
                    dt.Rows.RemoveAt(dt.Rows.Count - 1);
                    graphPane2.PointList.RemoveAt(0);
                    graphPane2.PointList.RemoveAt(this.graphPane2.PointList.Count - 1);
                }
                skip = (int)Math.Floor(tempList.Count * 0.3);
                for (int n = 0; n < dt.Rows.Count; n++)
                {
                    dt.Rows[n]["id"] = Convert.ToInt32(dt.Rows[n]["id"].ToString()) - skip;
                    graphPane2.PointList[n].index = graphPane2.PointList[n].index - skip;
                }
                RefreshData(dt.Rows.Count);
            }
            */
        }
        public string dispch = "";
        private void RefreshData(int index)
        {
            if (graphPane2.Visible)
            {
                graphPane2.XAxisMax = Program.data.validrows(dispch) + 1;
                if (index > 1)
                {
                    graphPane2.YAxisMax = Math.Floor(5 * Program.data.my_sqrtsum(dispch) * 1e6 + 0.5);
                    if (graphPane2.YAxisMax < 3)
                        graphPane2.YAxisMax = 3;
                }
                else
                {
                    graphPane2.YAxisMax = 1;
                }
                graphPane2.YAxisMin = -graphPane2.YAxisMax;
                graphPane2.Refresh();
            }
            ReDraw(false);
        }
        #endregion

    }
}
