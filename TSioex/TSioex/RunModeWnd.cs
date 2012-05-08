using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Runtime.InteropServices;
using System.IO;
using System.Text.RegularExpressions;
namespace TSioex
{
    public partial class RunModeWnd : Form
    {
        //[DllImport("user32.dll", EntryPoint = "ShowCursor", CharSet = CharSet.Auto)]
        //public static extern int ShowCursor(int bShow);
        public static int ShowCursor(int bShow)
        {
            return 0;
        }
        private System.Windows.Forms.Timer tm_cursor; //timer for cursor hiding
        private bool bShowCursor = true;

        private object ts_pwd = null;   //timespan for reset password check

        private UIPacker curr_packer    //get current packer instance
        {
            get
            {
                return Program.packers[0];
            }
        }

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
        private byte curr_node = 0xff;
        System.Windows.Forms.Timer uiTimer;

        public RunModeWnd()
        {
            InitializeComponent();
            this.BackColor = SingleModeWnd.bgWindow;
            UpdateDisplay();
            lastcalls = new Queue<string>();
            uiTimer = new System.Windows.Forms.Timer();
            uiTimer.Tick += new EventHandler(uiTimer_Tick);
            uiTimer.Interval = 250; //250ms for UI update
            uiTimer.Enabled = true; ;

            tm_cursor = new System.Windows.Forms.Timer();
            tm_cursor.Tick += new EventHandler(tm_cursor_Tick);
            tm_cursor.Interval = 5000;
            this.MouseUp += new MouseEventHandler(RunMode_MouseMove);
            this.MouseDown += new MouseEventHandler(RunModeWnd_MouseDown);

            btn_allempty.SetStyle(Color.Orange, MyButtonType.roundRectButton);
            btn_allzero.SetStyle(Color.Orange, MyButtonType.roundRectButton);
            runPan1.Init(NodeAgent.NodeNum,false);
            runPan1.handler += new PanClickHandler(this.passbar_MouseLeftButtonUp);
            btn_allstart.SetStyle(Color.Green, MyButtonType.round2RectButton);
            btn_config.SetStyle(Color.Purple, MyButtonType.round2Button);
            operator_no.SetStyle(Color.Purple, MyButtonType.round2RectButton);
            prd_no.SetStyle(Color.Purple, MyButtonType.round2RectButton);

            prd_picture.Click +=new EventHandler(this.prd_no_Click);
            btn_allempty.ValidClick += new EventHandler(btn_empty_click);
            btn_allzero.ValidClick += new EventHandler(btn_zero_click);
            btn_allstart.ValidClick += new EventHandler(btn_start_click);
            btn_config.ValidClick += new EventHandler(btn_singlemode_click);
            prd_no.ValidClick += new EventHandler(this.prd_no_Click);
            operator_no.ValidClick += new EventHandler(this.operator_no_Click);

            lbl_alert1.Click += new EventHandler(this.lbl_alert_MouseLeftButtonUp);
            lbl_alert2.Click += new EventHandler(this.lbl_alert_MouseLeftButtonUp);
            lbl_alert3.Click += new EventHandler(this.lbl_alert_MouseLeftButtonUp);
            lbl_alert4.Click += new EventHandler(this.lbl_alert_MouseLeftButtonUp);
            UpdateSysConfigUI();
            UpdateAlertWindow(false);
            txt_oper.Visible = false;

            lbl_speed.Text = "";
            lbl_lastweight.Text = "";
            lbl_totalpack.Text = "";
        }

        void RunModeWnd_MouseDown(object sender, MouseEventArgs e)
        {
            Region r = new Region(this.title_speed.ClientRectangle);
            Point p = new Point(e.X, e.Y);
            if (r.IsVisible(p))
                this.title_speed_MouseDown(sender, e);
        }
        #region mouse hide after 5 seconds in running mode
        void RunMode_MouseMove(object sender, MouseEventArgs e)
        {
            Region r = new Region(this.title_speed.ClientRectangle);
            Point p = new Point(e.X, e.Y);
            if (r.IsVisible(p))
                this.title_speed_MouseLeftButtonUp(sender, e);
            if (!bShowCursor)
            {
                ShowCursor(1);
                bShowCursor = true;
                tm_cursor.Enabled =(curr_packer.status == PackerStatus.RUNNING);
            }
        }

        void tm_cursor_Tick(object sender, EventArgs e)
        {
            if (curr_packer.status == PackerStatus.RUNNING) //in running status, hide the cursor after 5 seconds
            {
                ShowCursor(0);
                bShowCursor = false;
            }
            tm_cursor.Enabled = false;
        }
        #endregion
        private void title_speed_MouseDown(object sender, MouseEventArgs e)
        {
            if (ts_pwd == null)
                ts_pwd = new TimeSpan(DateTime.Now.Ticks);
        }

        //hide start button for nonvalid license
        public void Disable(bool state)
        {
            this.btn_allstart.Visible = state;
        }
        #region refresh UI
        public void RefreshVibUI()
        {
            /*
            UIPacker p = curr_packer;
            if (p.agent.vibstate == VibStatus.VIB_READY || p.agent.vibstate == VibStatus.VIB_INIDLE)
            {
                main_bucket.Template = this.FindResource("MainBucket") as ControlTemplate;
                main_bucket.ApplyTemplate();
            }
            if (p.agent.vibstate == VibStatus.VIB_WORKING)
            {
                main_bucket.Template = this.FindResource("MainBucketAct") as ControlTemplate;
                main_bucket.ApplyTemplate();

                lbl_status.Content = StringResource.str("waitpack");
            }*/
        }
        public void RefreshNodeUI()
        {
            UIPacker p = curr_packer;
            lbl_status.Text = "";
            foreach (byte n in p.weight_nodes)
            {
                UpdateNodeUI(n);
            }
            if (lbl_status.Text.ToString() != "")
            {
                lbl_status.ForeColor = Color.Red;
                return;
            }
            lbl_status.ForeColor = Color.Green;
            RefreshVibUI();
            if (lbl_status.Text.ToString() == "")
            {
                lbl_status.Text = StringResource.str("normal");
            }
        }
        private Bitmap prdBmp;
        public void UpdateSysConfigUI()
        {
            UIPacker pack = curr_packer;
            this.input_uvar.Text = pack.curr_cfg.upper_var.ToString() + StringResource.str("gram");

            this.input_dvar.Text = pack.curr_cfg.lower_var.ToString() + StringResource.str("gram");

            this.lbl_weight.Text = pack.curr_cfg.target.ToString() + StringResource.str("gram");

            this.prd_no.Text = pack.curr_cfg.product_no.ToString();

            this.operator_no.Text = Program.oper;

            this.prd_desc.Text = pack.curr_cfg.product_desc.ToString();

            
            //load the corresponding picture.
            if (prdBmp != null)
            {
                prdBmp.Dispose();
            }


            string path_to_jpg;
            path_to_jpg = ProdNum.baseDir + "\\prodpic\\" + StringResource.language + "\\" + pack.curr_cfg.product_desc.ToString() + ".jpg";
            if (!File.Exists(path_to_jpg))
                path_to_jpg = ProdNum.baseDir + "\\prodpic\\default.jpg";
            prdBmp = new Bitmap(path_to_jpg);

            prd_picture.Image = (Image)prdBmp;
        }
        public void UpdateNodeUI(byte n)
        {
            UIPacker pack = curr_packer;

            //display the variable based on current setting
            string err = NodeMaster.GetErrors(n);
            double wt = -1000;
            string ct = "";

            if (err == "")
            {
                wt = NodeMaster.GetWeight(n);
                ct = wt.ToString("F1");
                runPan1.SetStatus("passbar",n,PanStatus.IDLE);
            }
            else
            {
                if (AlertWnd.b_show_alert && AlertWnd.b_turnon_alert)
                {
                    runPan1.SetStatus("passbar", n, PanStatus.ERROR);
                    lbl_status.Text = StringResource.str(err.Substring(0, err.IndexOf(';'))) + "\n";
                    runPan1.SetText(n, StringResource.str(err.Substring(0, err.IndexOf(';'))));
                }
            }
            if (NodeMaster.GetStatus(n) == NodeStatus.ST_LOST || NodeMaster.GetStatus(n) == NodeStatus.ST_DISABLED)
            {
                runPan1.SetStatus("weightbar",n, PanStatus.DISBALED);
            }
            if (NodeMaster.GetStatus(n) == NodeStatus.ST_IDLE)
            {
                runPan1.SetStatus("weightbar", n, PanStatus.IDLE);
            }
            if (wt > -1000 && wt <= WeighNode.MAX_VALID_WEIGHT)
                runPan1.SetText(n,ct);
        }
        public void RefreshRunNodeUI() //node ui update at run time
        {
            lbl_status.Text = "";
            foreach (UIPacker pk in Program.packers)
            {
                foreach (byte naddr in pk.weight_nodes)
                {
                    string param = "wei_node" + naddr.ToString();
                    byte n = (byte)(RunModeWnd.StringToId(param));
                    NodeCombination nc = pk.nc;

                    double wt = NodeMaster.GetWeight(n);
                    if (wt > -1000 && wt <= WeighNode.MAX_VALID_WEIGHT)
                        runPan1.SetText(n,wt.ToString("F1"));

                    if (NodeMaster.GetStatus(n) == NodeStatus.ST_LOST || NodeMaster.GetStatus(n) == NodeStatus.ST_DISABLED)
                    {
                        runPan1.SetStatus("weightbar", n, PanStatus.ERROR);
                    }
                    string err = NodeMaster.GetErrors(n);
                    if (err != "" && AlertWnd.b_turnon_alert && AlertWnd.b_show_alert)
                        lbl_status.Text = n.ToString() + ":" + StringResource.str(err.Substring(0, err.IndexOf(';'))) + "\n";
                }
                if (pk.status == PackerStatus.RUNNING)
                {
                    lbl_speed.Text = pk.speed.ToString();
                    lbl_lastweight.Text = pk.last_pack_weight.ToString("F1");
                    lbl_totalpack.Text = pk.total_packs.ToString();

                    RefreshVibUI();
                }
            }
            if (lbl_status.Text.ToString() == "")
            {
                lbl_status.Text = StringResource.str("normal");
                lbl_status.ForeColor = Color.Green;
            }
            else
            {
                lbl_status.ForeColor = Color.Red;
                if (AlertWnd.b_turnon_alert && AlertWnd.b_stop_onalert && (curr_packer.status == PackerStatus.RUNNING))
                    btn_start_click(null, null);
            }

        }
        //update UI when a packer is hitted
        public void CombineNodeUI(CombineEventArgs ce)
        {
            foreach (byte naddr in Program.packers[ce.packer_id].weight_nodes)
            {
                string param = "wei_node" + naddr.ToString();

                //update weight first
                double wt = -100000;
                for (int i = 0; i < ce.release_addrs.Length; i++)
                {
                    if (ce.release_addrs[i] == naddr)
                    {
                        wt = ce.release_wts[i];
                        break;
                    }
                }

                if (wt > -1000 && wt <= WeighNode.MAX_VALID_WEIGHT)
                    runPan1.SetText(naddr,wt.ToString("F1"));

                //update status display
                if (NodeMaster.GetStatus(naddr) == NodeStatus.ST_LOST || NodeMaster.GetStatus(naddr) == NodeStatus.ST_DISABLED)
                {
                    runPan1.SetStatus("weightbar",naddr,PanStatus.DISBALED);
                }
                else if (naddr != curr_packer.vib_addr)
                {
                    if (ce.release_addrs.Contains(naddr))
                        runPan1.SetStatus("weightbar",naddr,PanStatus.RELEASE);
                    else
                        runPan1.SetStatus("weightbar", naddr,PanStatus.IDLE);
                }
            }
            //Update speed information
            UIPacker p = Program.packers[ce.packer_id];
            if (p.status == PackerStatus.RUNNING)
            {
                lbl_speed.Text = p.speed.ToString();
                lbl_lastweight.Text = p.last_pack_weight.ToString("F1");
                lbl_totalpack.Text = p.total_packs.ToString();
                RefreshVibUI();
            }
            
        }
        #endregion
        private static bool tmlock = false; //lock for timer handler
        void uiTimer_Tick(object sender, EventArgs e)
        {
            if (Program.topwnd != "runmode")
                return;
            lbl_datetime.Text = DateTime.Now.ToLongDateString() + "  " + DateTime.Now.ToLongTimeString();

            if (tmlock)
                return;
            tmlock = true;

            UIPacker p = curr_packer;

            if (lastcall != "")
            {
                if (lastcall == "StartStop")
                {
                    ToggleStartStop();
                    lbl_status.Text = StringResource.str("normal");
                    lastcall = "";
                }
                if (lastcall == "UpdatePrdNo")
                {
                    p.LoadPackConfig(p.curr_cfg.product_no, false);
                    UpdateSysConfigUI();
                    Thread.Sleep(2000);
                    lastcall = "";
                }
                txt_oper.Visible = false;
                tmlock = false;
                return;
            }
            else
            {
                curr_packer.nc.Step();
            }
            if (NodeCombination.q_hits.Count > 0)
            {
                CombineNodeUI(NodeCombination.q_hits.Dequeue());
                tmlock = false;
                return;
            }
            if (p.status != PackerStatus.RUNNING)
                RefreshNodeUI();
            else
                RefreshRunNodeUI();
            tmlock = false;
        }

        //send group command
        private void group_action(string action)
        {
            if (curr_packer.status == PackerStatus.PAUSED || curr_packer.status == PackerStatus.RUNNING)
            {
                return;
            }
            NodeMaster.Action(curr_packer.wn_addrs, action);
        }
        private void btn_empty_click(object sender, EventArgs e)
        {
            if (curr_packer.status != PackerStatus.RUNNING)
            {
                group_action("empty");
                foreach (byte naddr in Program.packers[0].weight_nodes)
                {
                    NodeMaster.ClearErrors(naddr);
                }
            }
        }

        private void btn_zero_click(object sender, EventArgs e)
        {
            if (curr_packer.status != PackerStatus.RUNNING)
                group_action("zero");
        }
        private void ToggleStartStop()
        {
            if (curr_packer.status == PackerStatus.RUNNING)
            {
                curr_packer.StopRun();
                this.btn_allstart.Text = StringResource.str("all_start");
                btn_allstart.SetStyle(Color.Green, MyButtonType.round2RectButton);
                //show cursor;
                tm_cursor.Enabled = false;
                ShowCursor(1);
                bShowCursor = true;
            }
            else
            {
                curr_packer.bSimulate = false;
                curr_packer.StartRun();
                this.btn_allstart.Text = StringResource.str("all_stop");
                btn_allstart.SetStyle(Color.BlueViolet,MyButtonType.round2RectButton);
            }
        }
        private void btn_start_click(object sender, EventArgs e)
        {
            lastcall = "StartStop";
            if (curr_packer.status == PackerStatus.RUNNING)
            {
                lbl_status.Text = StringResource.str("stopping");
            }
            else
            {
                lbl_status.Text = StringResource.str("starting");
                tm_cursor.Enabled = true;
            }
        }
        private void btn_history_click(object sender, EventArgs e)
        {
            if (curr_packer.status == PackerStatus.RUNNING)
                return;
            while (lastcall != "")
                this.uiTimer_Tick(null, null);
            Program.SwitchTo("history");
        }

        //going to config menu
        private void btn_singlemode_click(object sender, EventArgs e)
        {

            if (curr_packer.status == PackerStatus.RUNNING)
                return;

            Program.kbdwnd.Init(StringResource.str("enter_singlemode_pwd"), "singlemode", true, KbdData);

        }
        private void grp_reg(string regname)
        {
            if (curr_packer.status == PackerStatus.RUNNING)
            {
                return;
            }
            Program.kbdwnd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }
        static public int StringToId(string name)
        {
            StringBuilder sb = new StringBuilder();
            Regex re = new Regex("(\\d+)");
            Match m2 = re.Match(name);
            if (m2.Success)
                return int.Parse(m2.Groups[0].Value);
            else
                return -1;
        }

        private void grp_target_click(object sender, EventArgs e)
        {
            if (curr_packer.status != PackerStatus.RUNNING)
                grp_reg("run_target");
        }
        private void grp_uvar_click(object sender, EventArgs e)
        {
            if (curr_packer.status != PackerStatus.RUNNING)
                grp_reg("run_uvar");
        }
        private void grp_dvar_click(object sender, EventArgs e)
        {
            if (curr_packer.status != PackerStatus.RUNNING)
                grp_reg("run_dvar");
        }

        public void KbdData(string param, string data)
        {
            try
            {
                UIPacker pack = curr_packer;
                if (param == "run_uvar")
                {
                    pack.curr_cfg.upper_var = double.Parse(data);
                    pack.SaveCurrentConfig(4);
                }
                if (param == "run_dvar")
                {
                    pack.curr_cfg.lower_var = double.Parse(data);
                    pack.SaveCurrentConfig(4);
                }
                if (param == "run_target")
                {
                    pack.curr_cfg.target = Double.Parse(data);
                    pack.UpdateEachNodeTarget();
                    pack.SaveCurrentConfig(1 + 4);
                }
                if (param == "run_operator")
                {
                    Program.oper = data;
                    pack.SaveCurrentConfig(4);
                }
                if (param == "singlemode")
                {
                    if (Password.compare_pwd("user", data))
                    {
                        while (lastcall != "")
                            this.uiTimer_Tick(null, null);
                        Program.SwitchTo("configmenu");
                        return;
                    }
                    else
                        MessageBox.Show(StringResource.str("invalid_pwd"));
                }
                UpdateSysConfigUI();
            }
            catch (System.Exception e)
            {
                MessageBox.Show("Invalid Parameter");
                return;
            }
        }

        private void operator_no_Click(object sender, EventArgs e)
        {
            if (curr_packer.status != PackerStatus.RUNNING)
                Program.kbdwnd.Init(StringResource.str("enter_operator_no"), "run_operator", false, KbdData);
        }

        private void prd_no_Click(object sender, EventArgs e)
        {
            if (curr_packer.status == PackerStatus.RUNNING)
                return;

            if (curr_packer.status != PackerStatus.RUNNING)
                Program.prodnum.Init(prd_no_selected, false);
        }
        private void prd_no_selected(string item)
        {
            curr_packer.curr_cfg.product_no = item;
            curr_packer.SaveCurrentConfig(4);
            lastcall = "UpdatePrdNo";
            txt_oper.Text = StringResource.str("downloading");
            txt_oper.Visible = true;
            btn_allstart.Visible = true;
        }
        private int ButtonToId(object sender)
        {
            StringBuilder sb = new StringBuilder();
            Regex re = new Regex("(\\d+)");
            Match m2 = re.Match((sender as Control).Name);
            if (m2.Success)
                return int.Parse(m2.Groups[0].Value);
            else
                return -1;

        }


        private void lbl_alert_MouseLeftButtonUp(object sender, EventArgs e)
        {
            
            if ((sender is Button) && (curr_node != 0xff))
            {
                Button l = sender as Button;
                byte id = curr_node;
                if (l.Name == "lbl_alert1") //alert solved
                {
                    NodeMaster.ClearErrors(id);
                }
                if (l.Name == "lbl_alert2") //alert force
                {
                    NodeMaster.Action(new byte[] { id }, "empty");
                    NodeMaster.ClearErrors(id);
                }
                if (l.Name == "lbl_alert3") //alert disable
                {
                    if ((NodeMaster.GetStatus(id) == NodeStatus.ST_LOST) || (NodeMaster.GetStatus(id) == NodeStatus.ST_DISABLED))
                        NodeMaster.SetStatus(id, NodeStatus.ST_IDLE);
                    else
                        NodeMaster.SetStatus(id, NodeStatus.ST_DISABLED);
                }
                if (l.Name == "lbl_alert4") //alert quit
                {

                }
                curr_node = 0xff;
                UpdateAlertWindow(false);
            }
        }
        public void UpdateDisplay()
        {
            btn_allempty.Text =   StringResource.str("all_empty");
            btn_allzero.Text = StringResource.str("all_zero");
            btn_config.Text = StringResource.str("config");
            btn_allstart.Text = StringResource.str("all_start");

            title_operator.Text = StringResource.str("operator");
            title_lastweight.Text = StringResource.str("lastweight");
            title_speed.Text = StringResource.str("speed");
            title_totalpack.Text = StringResource.str("totalpack");
            
            lbl_alert1.Text = StringResource.str("alert_solved");
            lbl_alert2.Text = StringResource.str("alert_force");
            lbl_alert3.Text = StringResource.str("alert_disable");
            lbl_alert4.Text = StringResource.str("alert_quit");
            lbl_alert.Text = StringResource.str("alert_select");
            grp_dvar_label.Text = StringResource.str("grp_dvar");
            grp_uvar_label.Text = StringResource.str("grp_uvar");
            grp_lbl_prd_no.Text = StringResource.str("product_no");
            grp_target_label.Text = StringResource.str("grp_target");
        }
        private void UpdateAlertWindow(bool visible)
        {
            if((curr_node <= 0 || curr_node > NodeAgent.NodeNum) && visible)
                return;
            panel1.Visible = visible;
            lbl_alert.Visible = visible;
            lbl_alert1.Visible = visible;
            lbl_alert2.Visible = visible;
            lbl_alert3.Visible = visible;
            lbl_alert4.Visible = visible;
            if (visible)
            {
                 lbl_alert.Text = StringResource.str("alert_select") + "("+curr_node+")";
            }
        }
        private void looseFocus()
        {
        }
        private void passbar_MouseLeftButtonUp(int sid)
        {
            if (curr_packer.status == PackerStatus.RUNNING)
            {
                return;
            }
            if (!AlertWnd.b_manual_reset || !AlertWnd.b_turnon_alert)
                return;
            
                    byte id  = (byte)sid;
            curr_node = id;
                    if (AlertWnd.b_turnon_alert && AlertWnd.b_show_alert)
                    {
                        if ((NodeMaster.GetStatus(id) == NodeStatus.ST_LOST) || (NodeMaster.GetStatus(id) == NodeStatus.ST_DISABLED))
                            lbl_alert3.Text = StringResource.str("alert_enable");
                        else
                            lbl_alert3.Text = StringResource.str("alert_disable");
                        UpdateAlertWindow(true);
            }
        }

        private void main_bucket_Click(object sender, EventArgs e)
        {
            //curr_packer.agent.TriggerPacker(curr_packer.vib_addr);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //full time run , internal function only
            NodeMaster.Action(curr_packer.wn_addrs, "start");
            while (true)
            {
                Thread.Sleep(500);
                NodeMaster.Action(curr_packer.wn_addrs, "fill");
                Thread.Sleep(500);
                NodeMaster.Action(curr_packer.wn_addrs, "pass");
                Thread.Sleep(1000);
                NodeMaster.Action(curr_packer.wn_addrs, "empty");
                Thread.Sleep(500);
                NodeMaster.Action(new byte[] { curr_packer.bot_addr }, "fill");
            }
        }

        private int title_cnt = 0;
        private void title_speed_MouseLeftButtonUp(object sender, MouseEventArgs e)
        {
            if (ts_pwd == null)
                return;
            title_cnt++;
            if (title_cnt < 5)
                return;

            TimeSpan ts2 = new TimeSpan(DateTime.Now.Ticks);
            TimeSpan ts = ts2.Subtract((TimeSpan)ts_pwd).Duration();
            if (ts.Seconds < 6)
            {
                Reset();
                MessageBox.Show(StringResource.str("pwd_restore_done"));
            }
            title_cnt = 0;
            ts_pwd = null;
        }
        public static void Reset()
        {
            Password.set_pwd("admin", "020527");
            Password.set_pwd("user", "111111");
        }

    }
}