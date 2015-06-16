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
using Raysting.Common;
using Raysting.Controls;
namespace Zddq2
{


    public partial class RunWnd : Form
    {
        public const int win_width = 800;
        public const int win_height = 600;
        public const int bar_height = 70;

        private RunState runstate;
        private TaskMachine task;

        private RUN_STATE status;
        private System.Windows.Forms.Timer uiTimer;

        private Thread taskThread;
        //private string display_chan = "CH1";
        static RunWnd()
        {
        }
        #region PC command set
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
            SysConfig syscfg = Program.sysinfo;
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
                Program.mainwnd.Invoke( new Action<object, EventArgs>(this.statusBar1_StartStopClick), new object[] { 0, 0 });
                //task.Start();
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
        #endregion

        private static bool tmlock = false; //lock for timer handler
        void uiTimer_Tick(object sender, EventArgs e)
        {

            if (tmlock)
                return;
            tmlock = true;
            if (DeviceMgr.Loaded && status == RUN_STATE.DISABLED)
                status = RUN_STATE.IDLE;

            if (task.bRunning)
            {
                task.Step();
                if (!task.bRunning)
                {
                    status = RUN_STATE.IDLE;
                    DeviceMgr.Log("Measurement Stopped...");
                }
            }

            tmlock = false;
        }
        public void DoWork(object o)
        {
        }
        private void SetButtonType(RectButton btn, string type)
        {
            btn.BackColor = this.BackColor;
            btn.Style = MyButtonType.rectButton;
            btn.colorTop = Color.LightBlue;
            btn.colorShadow = this.BackColor;
            btn.Label = StringResource.str(type);
        }
        private RXDATA_MODE aux1_mode = RXDATA_MODE.RX;
        private RXDATA_MODE aux2_mode = RXDATA_MODE.RX;
        private RXDATA_MODE bigdisp_mode = RXDATA_MODE.RX;
        
        public RunWnd()
        {
            InitializeComponent();

            runstate = new RunState();
            task = new TaskMachine();
            task.StatusChanged += new StrEventHandler(UpdateStatusBar);
            uiTimer = new System.Windows.Forms.Timer();
            uiTimer.Tick += new EventHandler(uiTimer_Tick);
            uiTimer.Interval = 250; //250ms for UI update
            uiTimer.Enabled = true; ;

            #region set status bar style
            SetButtonType(btn_aux1,    StringResource.str("val_" + aux1_mode.ToString().ToLower()));
            SetButtonType(btn_aux2,    StringResource.str("val_" + aux2_mode.ToString().ToLower()));
            SetButtonType(btn_bigdisp, StringResource.str("val_" + bigdisp_mode.ToString().ToLower()));
            btn_menu.Enabled = true;
            SetButtonType(btn_menu, "menu");
            btn_menu.ValidClick += new EventHandler((o,e) =>{
                if (task.bRunning || task.bPaused)
                {
                    statusBar1_PauseResumeClick(o, e);
                }
                else
                {
                    Program.SwitchWindow("mainconfig");
                }
            });

            btn_dispchan.Enabled = true;
            SetButtonType(btn_dispchan, "CH1");
            graphPane2.channels = new string[] { "1" };
            btn_dispchan.ValidClick +=new EventHandler((o,e)=>{
                if (StringResource.str("lst_rxchan").Trim(new char[] { '|' }).IndexOf('|') >0)
                {
                    ChoiceWnd.Init(StringResource.str("choose") + StringResource.str("channel"),
                        "dispchan",
                        StringResource.str("lst_rxchan").Split(new char[]{'|'},StringSplitOptions.RemoveEmptyEntries),
                        -1,
                        null,
                        KbdData);
                }
            });

            btn_dispmode.Enabled = true;
            SetButtonType(btn_dispmode, StringResource.str("disp_graph"));
            btn_dispmode.ValidClick += new EventHandler((o, e) =>
            {
                if (Program.sysinfo.iDispMode == RXDISP_MODE.GRID)
                    Program.sysinfo.iDispMode = RXDISP_MODE.GRAPH;
                else
                    Program.sysinfo.iDispMode = RXDISP_MODE.GRID;
                btn_dispmode.Invalidate();
                ReDraw(true);
            });
            #endregion
            bt_run.Text = StringResource.str("start");
            bt_run.Click += new EventHandler((o, e) =>
            {
                statusBar1_StartStopClick(o, e);
            });


            SetGridStyle();

            AddValue("1", 1, 1.000000, 0, 0);
            AddValue("1", 2, 1.000002, 0, 0);
            AddValue("1", 3, 1.000001, 0, 0);
            PointPair.datamode = RXDATA_MODE.RX;
            graphPane2.Invalidate();
            
            this.Load += new EventHandler(RunWnd_Load);
        }
        void KbdData(string param, string data)
        {
            if (param == "dispchan")
            {
                int ich;
                if (Int32.TryParse(data, out ich))
                {
                    //display_chan = "CH" + (ich + 1).ToString();
                    btn_dispchan.Label = "CH" + (ich+1).ToString();
                    graphPane2.channels = new string[] { (ich + 1).ToString() };
                    btn_dispchan.Invalidate();
                    ReDraw(false);
                }

            }
        }
        void statusBar1_PauseResumeClick(object sender, EventArgs e)
        {
            task.PauseResume();
            if (!task.bPaused)
            {
                btn_menu.Label = StringResource.str("pause");
            }
            else
            {
                btn_menu.Label = StringResource.str("continue");
            }
        }

        void RunWnd_Load(object sender, EventArgs e)
        {
            status = RUN_STATE.DISABLED;
            Rectangle rect = new Rectangle();
//          FullScreenClass.SetFullScreen(true, ref rect);
        }
        private void DoNextMeasure()
        {
            RsInfo rs = runstate.selectedRs.Dequeue();
            RxInfo rx = runstate.selectedRx.Dequeue();

            status = RUN_STATE.INITIALIZING;

            if (false)
            {
                SysConfig syscfg = Program.sysinfo;

                DeviceMgr.Log(String.Format(@"==== New Measurement Paramter========
                                            Measure times = {0}\r\nRs ID = {1}\r\nRs Value = {2}\r\nRs Temp. Alpha = {3}\r\nRs Temp. Beta = {4}\r\n
                                            Temperature = {5}\r\nRx ID = {6}\r\nSample times = {7}\r\nSwitch delay = {8}\r\n
                                            Filter = {9}\r\nFilter Length = {10}\r\nMeasure delay = {12}\r\n
                                            AutoFilter = {13}\r\nRx curr. = {14}\r\n
                                            New Measurement Started...", syscfg.iMeasTimes.ToString(), rs.sSerial, rs.dValue.ToString("F8"), rs.dAlpha.ToString("F3"), rs.dBeta.ToString("F3"),
                                                                       syscfg.dTemp.ToString("F3"), rx.sSerial, syscfg.iSampleTimes, syscfg.iKTT,
                                                                       syscfg.sFilterType, syscfg.iFilter, syscfg.iMeasDelay, syscfg.bThrow, rx.iIx
                                            ));
            }
            task.Start(rx, rs);
                
        }
        private string UpdateDisplayReading(RXDATA_MODE mode)
        {
            string ch = "CH" + (task.currRx.iChan+1).ToString();

            if (mode == RXDATA_MODE.RX)
            {
                return Util.FormatData(task.currRx.var.rRx, 8) + " Ω";
            }
            if (mode == RXDATA_MODE.RATIO_RX)
            {
                double r = Program.testdata.my_avg(ch) / task.currRs.dTValue;
                return Util.FormatDataWithSpace(r, 8);
            }
            if (mode == RXDATA_MODE.RS)
            {
                return Util.FormatData(task.currRs.dTValue, 8) + " Ω";
            }
            if (mode == RXDATA_MODE.AVG_RX)
            {
                if (Program.testdata.validrows(ch) == 0)
                    return "0.000 000 0 Ω";

                return Util.FormatData(Program.testdata.my_avg(ch), 8) + " Ω";
            }
            if (mode == RXDATA_MODE.MAX_RX)
                return Util.FormatData(Program.testdata.my_max(ch), 8) + " Ω";

            if (mode == RXDATA_MODE.MIN_RX)
                return Util.FormatData(Program.testdata.my_min(ch), 8) + " Ω";

            if (mode == RXDATA_MODE.VAR_RX)
            {
                if (Program.testdata.my_sqrtsum(ch) < 1e-8)
                    return "0.00 ppm";
                else
                    return (Program.testdata.my_sqrtsum(ch) * 1e6).ToString("F2") + " ppm";
            }
            return "0";
        }
        private void UpdateStatusBar(object sender, string msg)
        {
            if (msg == "newvalue")
            {
                
                RX_VAR rv = task.currRx.var;
                
                AddValue(task.currRx.iChan.ToString(), task.iMeasCnt, rv.rRx,0,0);

                lbl_aux1.Text = UpdateDisplayReading(aux1_mode);
                lbl_aux2.Text = UpdateDisplayReading(aux2_mode);
                lbl_bigdisp.Text = UpdateDisplayReading(bigdisp_mode);
                DeviceMgr.ReportData(task.iMeasCnt,rv.rRx);
                LogData(task.iMeasCnt, rv.rRx);
                /*
                statusBar1.total = syscfg.iMeasTimes;
                statusBar1.count = task.iMeasCnt;
                 */
            }
            else if (msg == "complete")
            {
                if (runstate.NumOfRunsLeft > 0)
                {
                    DoNextMeasure();
                }
                else
                {
                    if (Program.sysinfo.bThrow)
                        ThrowData();
                    LogComplete();
                    bt_run.Text = StringResource.str("start");
                    btn_menu.Label = StringResource.str("menu");
                }
                 
            }
            else
            {
                string txt = StringResource.str(msg);
                if (txt == msg)
                    MsgDlg.Show(task.currRx.var.errMsg);
                else
                    bt_run.Text = txt;
            }
        }


        public void LogData(int index, double value)
        {
            DeviceMgr.Log(String.Format("读数 {0} = {1}", index.ToString(), Util.FormatData(value, 8)));
        }
        public void LogComplete()
        {
            /*
            DeviceMgr.Log(String.Format(@"...... 数据汇总 开始 ......
            Rx(平均值):{0}
            最大值:    {1}
            最小值:    {2}
            Rx/Rs:     {3}
            标准差:    {4}
            不确定度:  {4}", GetData(RXDATA_MODE.AVERAGE),
                GetData(RXDATA_MODE.MAX),
                GetData(RXDATA_MODE.MIN),
                GetData(RXDATA_MODE.RATIO),
                GetData(RXDATA_MODE.VARIANCE)));
             */
            DeviceMgr.Log("...... 数据汇总 结束 ...... ");
        }

        public void AddValue(string chan, int index, double val, double rt, double temp)
        {
            //graphPane2.PointList.Add(new PointPair(chan, index, val, rt, temp, false, DateTime.Now));
            Program.testdata.AddValue(chan, index, val, rt, temp);
            graphPane2.average = Program.testdata.my_avg(chan);
            return;
            DataRow dr = dt.NewRow();
            dr["id"] = index;
            dr["ch"] = "Rx" + chan + "/Rs" + task.currRs.iChan.ToString();
            dr["data"] = Util.FormatDataWithSpace(val, 8);
            dr["ratio"] = Util.FormatDataWithSpace(val / task.currRs.dTValue, 8);
            dr["time"] = DateTime.Now.ToString("HH:mm:ss.fff");
            dr["excluded"] = "";
            dt.Rows.Add(dr);

            RefreshData(index);
        }

        void statusBar1_StartStopClick(object sender, EventArgs e)
        {
            if (task.bRunning)
            {
                btn_menu.Label = StringResource.str("menu");
                bt_run.Text = StringResource.str("start");
                status = RUN_STATE.STOPPING;                
                task.Stop();
                return;
            }

            btn_menu.Label = StringResource.str("pause");
                runstate.LoadScheme();
                if (runstate.NumOfRunsLeft <= 0)
                {
                    MsgDlg.Show("No valid selection");
                    return;
                }

                DoNextMeasure();
                Program.testdata.ClearAll();
            
        }
        void statusBar1_MenuClick(object sender, EventArgs e)
        {
            //ChoiceWnd.Init("Test", new string[] { "choice1", "choice2", "choice1", "choice2", "choice1", "choice8", "choice1", "choice8" }, 1, null, new KbdDataHandler((a, b) => { }));
            //Program.SwitchWindow("sysconfig");
        }

        public void ReDraw(bool newmode)
        {

            if (newmode)
            {
                if (Program.sysinfo.iDispMode == RXDISP_MODE.GRAPH)
                {
                    btn_dispmode.Label = StringResource.str("disp_graph");
                    dataGrid2.Visible = false;
                    graphPane2.Visible = true;
                    graphPane2.Refresh();
                }
                else if (Program.sysinfo.iDispMode == RXDISP_MODE.GRID)
                {
                    btn_dispmode.Label = StringResource.str("disp_grid");
                    graphPane2.Visible = false;
                    dataGrid2.Visible = true;
                }
                btn_dispmode.Invalidate();
                //arrange dialogs
            }

        }
        private void UpdateStatusBar()
        {
            if (status == RUN_STATE.IDLE)
            {
                bt_run.Text = StringResource.str("start");
                btn_menu.Text = StringResource.str("menu");
                bt_run.BackColor = Color.White;
            }
            if (status == RUN_STATE.ERROR)
            {
                bt_run.Text = StringResource.str("error");
                btn_menu.Text = StringResource.str("menu");
                bt_run.BackColor = Color.MediumVioletRed;
            }
            if (status == RUN_STATE.DISABLED)
            {
                bt_run.Text = StringResource.str("initialize");
                btn_menu.Text = StringResource.str("menu");
                bt_run.BackColor = Color.LightGray;
            }
            if (status == RUN_STATE.INITIALIZING)
            {
                bt_run.Text = String.Format("{0}      {1}s", StringResource.str("initialize"), task.waiting);
                bt_run.BackColor = Color.LightYellow;
            }
            if (status == RUN_STATE.STOPPING)
            {
                bt_run.Text = StringResource.str("stopping");
                btn_menu.Text = StringResource.str("menu");
                bt_run.BackColor = Color.LightYellow;
            }

            if (status == RUN_STATE.SEARCHING)
            {
                bt_run.Text = String.Format("{0}      {1}s", StringResource.str("searching"), task.waiting);
                bt_run.BackColor = Color.LightYellow;
            }
            if (status == RUN_STATE.READING)
            {
                bt_run.Text = String.Format("{0}      {1}s", StringResource.str("reading"), task.waiting);
                bt_run.BackColor = Color.LightYellow;
            }

            if (status == RUN_STATE.TRACKING)
            {
                bt_run.Text = String.Format("{0}  (继续第{1}个读数)     {2}s", StringResource.str("tracking"), runstate.NumOfRunsLeft , task.waiting);
                bt_run.BackColor = Color.LightYellow;
            }
        }
        #region grid related functions

        void dataGrid2_Click(object sender, EventArgs e)
        {
            if (dataGrid2.CurrentCell is DataGridViewCell)
            {
                if (dataGrid2.CurrentCell.ColumnIndex == 4)
                {
                    if (dataGrid2.CurrentCell.RowIndex >= 0)
                    {
                        
                        PointPair pp = Program.testdata.PointList[dataGrid2.CurrentCell.RowIndex];
                        
                        if (pp.excluded)
                            dt.Rows[dataGrid2.CurrentCell.RowIndex]["excluded"] = "";
                        else
                            dt.Rows[dataGrid2.CurrentCell.RowIndex]["excluded"] = "【√】";
                        pp.excluded = !pp.excluded;
                        
                        Program.testdata.PointList[dataGrid2.CurrentCell.RowIndex] = pp;
                        
                        ReDraw(true);
                    }
                }
            }
        }
        private DataTable dt;
        private void SetGridStyle()
        {
            dt = new DataTable("values");
            dt.Columns.Add("id");
            dt.Columns.Add("ch");
            dt.Columns.Add("data");
            dt.Columns.Add("temp");
            dt.Columns.Add("ratio");
            dt.Columns.Add("time");
            dt.Columns.Add("excluded");

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
            //dataGridView1.TableStyles.Add(tblStyle);

            dataGrid2.DataSource = dt;// Program.testdata.PointList;
            dataGrid2.Click += new EventHandler(dataGrid2_Click);

            graphPane2.XAxisTicCount = 10;
            graphPane2.YAxisTicCount = 4;
            graphPane2.YAxisMax = 5;
            graphPane2.YAxisMin = -5;
            graphPane2.YAxisTitle = "PPM";
            graphPane2.XAxisTicCount = 10;

        }
        #endregion

        #region graphpane related function
        //throw top 30% data and bottom 30% data;
        public void ThrowData()
        {
            List<PointPair> tempList = new List<PointPair>(Program.testdata.PointList);
            tempList.Sort();
            int skip = (int)Math.Floor(tempList.Count * 0.3);
            if (skip > 0)
            {
                while (skip-- > 0)
                {
                    dt.Rows.RemoveAt(0);
                    dt.Rows.RemoveAt(dt.Rows.Count - 1);
                    Program.testdata.PointList.RemoveAt(0);
                    Program.testdata.PointList.RemoveAt(Program.testdata.PointList.Count - 1);
                }
                skip = (int)Math.Floor(tempList.Count * 0.3);
                for (int n = 0; n < dt.Rows.Count; n++)
                {
                    dt.Rows[n]["id"] = Convert.ToInt32(dt.Rows[n]["id"].ToString()) - skip;
                    Program.testdata.PointList[n].index = Program.testdata.PointList[n].index - skip;
                }
                RefreshData(dt.Rows.Count);
            }
        }
        
        private void RefreshData(int index)
        {
            if (graphPane2.Visible)
            {
                graphPane2.XAxisMax = Program.testdata.validrows(btn_dispchan.Label) + 1;
                if (index > 1)
                {
                    graphPane2.YAxisMax = Math.Floor(5 * Program.testdata.my_sqrtsum(btn_dispchan.Label) * 1e6 + 0.5);
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

        private void RunWnd_Load_1(object sender, EventArgs e)
        {

        }

    }
    public class RunState
    {
        public RunState()
        {
            selectedRs = new Queue<RsInfo>();
            selectedRx = new Queue<RxInfo>();
        }

        //load test scheme
        public int LoadScheme()
        {
            selectedRs.Clear();
            selectedRx.Clear();
            
            foreach (RsInfo rs in Program.lst_rsinfo)
            {
                foreach (RxInfo rx in Program.lst_rxinfo)
                {
                    if (rx.bSelected && rs.bSelected )
                    {
                        for (int i = 0; i < Program.sysinfo.iMeasTimes; i++)
                        {
                            selectedRx.Enqueue(rx);
                            selectedRs.Enqueue(rs);
                        }
                    }
                }
            }
            return NumOfRunsLeft;
        }
        public int NumOfRunsLeft
        {
            get
            {
                return selectedRs.Count;
            }
        }
        public Queue<RxInfo> selectedRx;
        public Queue<RsInfo> selectedRs;
    }
}
