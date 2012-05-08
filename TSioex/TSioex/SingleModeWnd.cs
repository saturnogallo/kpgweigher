using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace TSioex
{
    public partial class SingleModeWnd : Form
    {
        static public Color bgWindow = Color.FromArgb(238, 242, 251); //EE,F2,FB
        public SingleModeWnd()
        {
            InitializeComponent();
            this.BackColor = SingleModeWnd.bgWindow;
            InitDisplay();
            btn_zero.ValidClick += new EventHandler(btn_action_Click);
            btn_empty.ValidClick += new EventHandler(btn_action_Click);
            btn_fill.ValidClick += new EventHandler(btn_action_Click);
            btn_mainvib.ValidClick += new EventHandler(btn_action_Click);
            btn_pack.ValidClick += new EventHandler(btn_action_Click);
            btn_pass.ValidClick += new EventHandler(btn_action_Click);
            btn_selectall.ValidClick +=new EventHandler(btn_selectall_Click);
            btn_sub_applyall.ValidClick+= new EventHandler(btn_sub_applyall_Click);
            btn_save.ValidClick += new EventHandler(btn_save_Click);
            btn_ret_run.MouseUp += new MouseEventHandler(btn_ret_run_Click);
            btn_ret_config.MouseUp += new MouseEventHandler(btn_ret_config_Click);
            btn_trial.ValidClick+=new EventHandler(btn_trial_Click);
            

            btn_zero.SetStyle(Color.Orange, MyButtonType.roundRectButton);
            btn_empty.SetStyle(Color.Orange, MyButtonType.roundRectButton);
            btn_fill.SetStyle(Color.Orange, MyButtonType.roundRectButton);
            btn_mainvib.SetStyle(Color.Orange, MyButtonType.roundRectButton);
            btn_pack.SetStyle(Color.Orange, MyButtonType.roundRectButton);
            btn_pass.SetStyle(Color.Orange, MyButtonType.roundRectButton);
            btn_sub_applyall.SetStyle(Color.Orange, MyButtonType.roundRectButton);
            btn_save.SetStyle(Color.Purple, MyButtonType.round2Button);
            btn_trial.SetStyle(Color.Purple, MyButtonType.round2Button);
            btn_selectall.SetStyle(Color.Orange, MyButtonType.roundRectButton);
            ellipseWithImageBrush.MouseUp +=new MouseEventHandler(this.ellipseWithImageBrush_MouseLeftButtonUp);

            sub_freq_input.SetStyle(Color.Bisque, MyButtonType.round2RectButton);
            sub_amp_input.SetStyle(Color.Bisque, MyButtonType.round2RectButton);
            sub_time_input.SetStyle(Color.Bisque, MyButtonType.round2RectButton);
            sub_filter_input.SetStyle(Color.Bisque, MyButtonType.round2RectButton);
            wei_otime_input.SetStyle(Color.Bisque, MyButtonType.round2RectButton);
            wei_dtime_input.SetStyle(Color.Bisque, MyButtonType.round2RectButton);
            col_dtime_input.SetStyle(Color.Bisque, MyButtonType.round2RectButton);
            col_otime_input.SetStyle(Color.Bisque, MyButtonType.round2RectButton);
            openwei_input.SetStyle(Color.Bisque, MyButtonType.round2RectButton);
            motor_speed_input.SetStyle(Color.Bisque, MyButtonType.round2RectButton);
            run_freq.SetStyle(Color.Bisque, MyButtonType.round2RectButton);
            run_amp.SetStyle(Color.Bisque, MyButtonType.round2RectButton);
            run_time.SetStyle(Color.Bisque, MyButtonType.round2RectButton);


            sub_freq_input.ValidClick +=new EventHandler(this.input_GotFocus);
            sub_amp_input.ValidClick += new EventHandler(this.input_GotFocus);
            sub_time_input.ValidClick += new EventHandler(this.input_GotFocus);
            sub_filter_input.ValidClick += new EventHandler(this.input_GotFocus);
            wei_otime_input.ValidClick += new EventHandler(this.input_GotFocus);
            wei_dtime_input.ValidClick += new EventHandler(this.input_GotFocus);
            col_dtime_input.ValidClick += new EventHandler(this.input_GotFocus);
            col_otime_input.ValidClick += new EventHandler(this.input_GotFocus);
            openwei_input.ValidClick += new EventHandler(this.input_GotFocus);
            motor_speed_input.ValidClick += new EventHandler(this.input_GotFocus);
            run_freq.ValidClick += new EventHandler(this.input_GotFocus);
            run_amp.ValidClick += new EventHandler(this.input_GotFocus);
            run_time.ValidClick += new EventHandler(this.input_GotFocus);

            btn_prd_no.ValidClick += new EventHandler(this.btn_prd_no_Click);
            btn_prd_desc.ValidClick += new EventHandler(this.btn_prd_desc_Click);
            btn_target.ValidClick += new EventHandler(this.btn_pack_target_Click);
            btn_uvar.ValidClick += new EventHandler(this.btn_pack_uvar_Click);
            btn_dvar.ValidClick += new EventHandler(this.btn_pack_dvar_Click);

            btn_prd_no.SetStyle(Color.Bisque, MyButtonType.round2RectButton);
            btn_prd_desc.SetStyle(Color.Bisque, MyButtonType.round2RectButton);
            btn_target.SetStyle(Color.Bisque, MyButtonType.round2RectButton);
            btn_uvar.SetStyle(Color.Bisque, MyButtonType.round2RectButton);
            btn_dvar.SetStyle(Color.Bisque, MyButtonType.round2RectButton);

            new_prd.Click += new EventHandler(new_prd_Click);

            runPan1.Init(NodeAgent.NodeNum,true);
            runPan1.handler += new PanClickHandler(bucket_click);
            lastcalls = new Queue<string>();
            curr_btn = null;
            last_btn = null;
            curr_node_index = -1;

            UpdateUI();
            lbl_lastweight.Text = "";
            lbl_totalpack.Text = "";
            lbl_totalweights.Text = "";
            lbl_speed.Text = "";
            uiTimer = new System.Windows.Forms.Timer();
            uiTimer.Tick += new EventHandler(uiTimer_Tick);
            uiTimer.Interval = 250;
            uiTimer.Enabled = true;
            this.panel1.BackColor = SingleModeWnd.bgWindow;
            this.panel2.BackColor = SingleModeWnd.bgWindow;
            cb_autoamp.Click +=new EventHandler(cb_autoamp_Click);
        }

        System.Windows.Forms.Timer uiTimer;
        private Bitmap prdBmp;
        private UIPacker curr_packer
        {
            get
            {
                return Program.packers[0];
            }
        }

        private Queue<string> lastcalls;
        
        private string lastcall
        {
            get
            {
                if (lastcalls.Count == 0)
                    return "";
                else
                    return lastcalls.Peek();
            }
            set
            {
                if (value == "")
                {
                    if (lastcalls.Count > 0)
                        lastcalls.Dequeue();
                }
                else
                {
                    if(!lastcalls.Contains(value))
                        lastcalls.Enqueue(value);
                }
            }
        }
        private bool bSelectAll = false;
        private int curr_node_index { get; set; }
        private void HandleLongCommand()
        {
            if (lastcall == "StartStop")
            {
                ToggleStartStop();
                lastcall = "";
            }

            if (lastcall == "UpdateProdNo")
            {
                curr_packer.LoadPackConfig(curr_packer.curr_cfg.product_no,false);
                curr_packer.SaveCurrentConfig(1+2+4);
                UpdateUI();
                lastcall = "";
            }
            if (lastcall == "UpdateUI")
            {
                UpdateUI();
                lastcall = "";
            }
            if (lastcall == "ApplyToAll")
            {
                sub_applyall();
                lastcall = "";
            }
            if (lastcall == "select_prdno")
            {
                new_prd_Click(null, null);
                lastcall = "";
            }
            if (lastcall == "save_config")
            {
                curr_packer.SaveCurrentConfig(1 + 2 + 4);
                lastcall = "";
                //MessageBox.Show(StringResource.str("store_done"));
            }
            if (lastcall == "")
            {
                txt_oper.Visible = false;
            }
        }
        //refresh all strings
        public void InitDisplay()
        {
            new_prd.Text = "+";

            run_amp_label.Text = StringResource.str("grp_amp");
            cb_autoamp.Text = StringResource.str("auto_amp");
            title_lastweight.Text = StringResource.str("lastweight");
            title_speed.Text = StringResource.str("speed");
            title_totalpack.Text = StringResource.str("totalpack");
            sub_amp_label.Text = StringResource.str("sub_amp");
            sub_freq_label.Text = StringResource.str("sub_freq");
            sub_time_label.Text = StringResource.str("sub_time");
            motor_speed_label.Text = StringResource.str("motor_speed");
            col_dtime_label.Text = StringResource.str("col_dtime");
            col_otime_label.Text = StringResource.str("col_otime");
            openwei_label.Text = StringResource.str("openwei");
            wei_dtime_label.Text = StringResource.str("wei_dtime");
            wei_otime_label.Text = StringResource.str("wei_otime");

            lbl_run_freq.Text = StringResource.str("grp_freq");
            lbl_node.Text = StringResource.str("bucketsetting");
            lbl_desc.Text = StringResource.str("product_desc");
            lbl_filter.Text = StringResource.str("sub_filter");
            lbl_group.Text = StringResource.str("product_config");
            lbl_prd_no.Text = StringResource.str("product_no");
            lbl_run_time.Text = StringResource.str("grp_time");
            lbl_summary.Text = StringResource.str("totalweight");
            grp_dvar_label.Text = StringResource.str("grp_dvar");
            grp_uvar_label.Text = StringResource.str("grp_uvar");
            grp_target_label.Text = StringResource.str("target");
            btn_trial.Text = StringResource.str("cmd_run");
            btn_empty.Text = StringResource.str("cmd_empty");
            btn_fill.Text = StringResource.str("cmd_fill");
            btn_mainvib.Text = StringResource.str("cmd_mainvib");
            btn_pack.Text = StringResource.str("cmd_pack");
            btn_pass.Text = StringResource.str("cmd_pass");
            btn_save.Text = StringResource.str("cmd_save");
            btn_selectall.Text = StringResource.str("cmd_selectall");
            btn_sub_applyall.Text = StringResource.str("subcfg_applyall");
            btn_zero.Text = StringResource.str("cmd_zero");
            
            
        }

        private Button curr_btn;
        private Button last_btn;
        private object tlock = new object();
        private static bool tmlock = false;
        void uiTimer_Tick(object sender, EventArgs e)
        {
            if (Program.topwnd != "singlemode")
                return;
            if (tmlock)
                return;
            tmlock = true;
            
            {
                if ((curr_node_index == -1))
                {
                    bucket_click(1);
                }
                else
                {
                    if (lastcall != "")
                        HandleLongCommand();
                    else
                    {
                        RefreshNodeUI();
                        curr_packer.nc.Step();
                    }
                }
                if (NodeCombination.q_hits.Count > 0)
                {
                    HitCombineNodeUI(NodeCombination.q_hits.Dequeue());
                    tmlock = false;
                    return;
                }
            }
            tmlock = false;
        }
        private void RefreshNodeUI()
        {
            foreach (UIPacker pk in Program.packers)
            {
                foreach (byte naddr in pk.weight_nodes)
                {
                    byte n = naddr;
                    double wt = NodeMaster.GetWeight(n);
                    if (wt > -1000 && wt <= WeighNode.MAX_VALID_WEIGHT)
                        runPan1.SetText(n, wt.ToString("F1"));
                    else
                    {
                        if (wt > WeighNode.MAX_VALID_WEIGHT && wt < 65537)
                        {
                            runPan1.SetText(n, "ERR");
                            runPan1.SetStatus("weightnode", n, PanStatus.ERROR);
                        }
                    }
                    if (NodeMaster.GetStatus(n) == NodeStatus.ST_LOST || NodeMaster.GetStatus(n) == NodeStatus.ST_DISABLED)
                    {
                        runPan1.SetStatus("weightbar", n, PanStatus.DISBALED);
                    }
                    if (NodeMaster.GetStatus(n) == NodeStatus.ST_IDLE)
                    {
                        runPan1.SetStatus("weightbar", n , PanStatus.IDLE);
                    }
                }
            }
            UIPacker p = curr_packer;
            if (p.status == PackerStatus.RUNNING)
            {
                lbl_speed.Text = p.speed.ToString();
                lbl_lastweight.Text = p.last_pack_weight.ToString("F1");
                lbl_totalpack.Text = p.total_sim_packs.ToString();
                lbl_totalweights.Text = p.total_sim_weights.ToString("F1");
            }
        }
        public void Disable(bool state)
        {
            btn_trial.Visible = state;
        }
        private void HitCombineNodeUI(CombineEventArgs ce)
        {
            foreach (byte naddr in Program.packers[ce.packer_id].weight_nodes)
            {
                byte n = naddr;

                if (!ce.release_addrs.Contains(n))
                    continue;
                double wt = NodeMaster.GetWeight(n);
                if (wt > -1000 && wt <= WeighNode.MAX_VALID_WEIGHT)
                    runPan1.SetText(n, NodeMaster.GetWeight(n).ToString("F1"));

                if (NodeMaster.GetStatus(n) == NodeStatus.ST_LOST || NodeMaster.GetStatus(n) == NodeStatus.ST_DISABLED)
                {
                    runPan1.SetStatus("weightbar", n, PanStatus.DISBALED);
                }else{
                    if (n != curr_packer.vib_addr)
                    {
                        if (ce.release_addrs.Contains(n))
                        {
                            runPan1.SetStatus("weightbar", n, PanStatus.RELEASE);
                        }
                        else
                        {
                            runPan1.SetStatus("weightbar", n, PanStatus.IDLE);
                        }
                     }
                }
            }
        }
        
        private void bucket_click(int nodeid)
        {
            if (nodeid < 1)
                return;
            curr_node_index = nodeid;
            ShowStatus("loading");
            //update parameter of the current node to UI.
            lastcall = "UpdateUI";
            return;
        }

        public void UpdateUI()
        {
            byte vn = curr_packer.vib_addr;
            if (curr_node_index < 0)
                return;
             
            sub_freq_input.Text = NodeMaster.GetNodeReg((byte)curr_node_index, "magnet_freq").ToString();
            sub_amp_input.Text = NodeMaster.GetNodeReg((byte)curr_node_index, "magnet_amp").ToString();
            sub_time_input.Text = NodeMaster.GetNodeReg((byte)curr_node_index, "magnet_time").ToString();
            sub_filter_input.Text = NodeMaster.GetNodeReg((byte)curr_node_index, "cs_gain_wordrate").ToString();
            wei_otime_input.Text = NodeMaster.GetNodeReg((byte)curr_node_index, "open_w").ToString();
            wei_dtime_input.Text = NodeMaster.GetNodeReg((byte)curr_node_index, "delay_w").ToString();
            col_dtime_input.Text = NodeMaster.GetNodeReg((byte)curr_node_index, "delay_s").ToString();
            col_otime_input.Text = NodeMaster.GetNodeReg((byte)curr_node_index, "open_s").ToString();
            openwei_input.Text = NodeMaster.GetNodeReg((byte)curr_node_index, "delay_f").ToString();


            motor_speed_input.Text = NodeMaster.GetNodeReg((byte)curr_node_index, "motor_speed").ToString();

            run_freq.Text = NodeMaster.GetNodeReg(curr_packer.vib_addr, "magnet_freq").ToString();
            run_amp.Text = NodeMaster.GetNodeReg(curr_packer.vib_addr, "magnet_amp").ToString();
            run_time.Text = NodeMaster.GetNodeReg(curr_packer.vib_addr, "magnet_time").ToString();
            btn_prd_no.Text = curr_packer.curr_cfg.product_no.ToString();
            btn_prd_desc.Text = curr_packer.curr_cfg.product_desc.ToString();
            btn_target.Text = curr_packer.curr_cfg.target.ToString() + StringResource.str("gram");
            btn_uvar.Text = curr_packer.curr_cfg.upper_var.ToString() + StringResource.str("gram");
            btn_dvar.Text = curr_packer.curr_cfg.lower_var.ToString() + StringResource.str("gram");

            cb_autoamp.Checked = (curr_packer.curr_cfg.target_comb > 0);
            if (cb_autoamp.Checked)
            {
                cb_autoamp.Text = StringResource.str("autoamp_on") + curr_packer.curr_cfg.target_comb.ToString();
            }
            else
            {
                cb_autoamp.Text = StringResource.str("autoamp_off");
            }

            lbl_node.Text = StringResource.str("bucketsetting") + "("+(curr_node_index).ToString() + ")";

            //load the corresponding pictiure.
            string path_to_jpg;
            path_to_jpg = ProdNum.baseDir + "\\prodpic\\" + StringResource.language + "\\" + this.curr_packer.curr_cfg.product_desc.ToString() + ".jpg";
            if (!File.Exists(path_to_jpg))
                path_to_jpg = ProdNum.baseDir + "\\prodpic\\default.jpg";
            prdBmp = new Bitmap(path_to_jpg);

            ellipseWithImageBrush.Image = (Image)prdBmp;

        }
        private void node_reg(string regname)
        {
            if (curr_packer.status == PackerStatus.RUNNING)
            {
                return;
            }
            Program.kbdwnd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }
        public void KbdData(string param, string data)
        {
            //update the display based on keyboard input
            bool bNeedUpdateComb = false; //whether target weight of each node is required
            try
            {
                UIPacker pack = curr_packer;
                if (param == "sub_freq_input")
                {
                    NodeMaster.SetNodeReg((byte)curr_node_index, new string[]{"magnet_freq"}, new UInt32[]{UInt32.Parse(data)});
                    apply_regs.Insert(0, "magnet_freq");
                }
                if (param == "sub_amp_input")
                {
                    NodeMaster.SetNodeReg((byte)curr_node_index, new string[]{"magnet_amp"}, new UInt32[]{UInt32.Parse(data)});
                    apply_regs.Insert(0, "magnet_amp");
                }
                if (param == "sub_time_input")
                {
                    NodeMaster.SetNodeReg((byte)curr_node_index, new string[]{"magnet_time"}, new UInt32[]{UInt32.Parse(data)});
                    apply_regs.Insert(0, "magnet_time");
                }
                if (param == "sub_filter_input")
                {
                    NodeMaster.SetNodeReg((byte)curr_node_index, new string[]{"cs_gain_wordrate"}, new UInt32[]{UInt32.Parse(data)});
                    apply_regs.Insert(0, "cs_gain_wordrate");
                }

                if (param == "wei_otime_input")
                {
                    NodeMaster.SetNodeReg((byte)curr_node_index, new string[]{"open_w"},new UInt32[]{ UInt32.Parse(data)});
                    apply_regs.Insert(0, "open_w");
                }
                if (param == "wei_dtime_input")
                {
                    NodeMaster.SetNodeReg((byte)curr_node_index, new string[]{"delay_w"},new UInt32[]{ UInt32.Parse(data)});
                    apply_regs.Insert(0, "delay_w");
                }
                if (param == "col_dtime_input")
                {
                    NodeMaster.SetNodeReg((byte)curr_node_index, new string[]{"delay_s"}, new UInt32[]{UInt32.Parse(data)});
                    apply_regs.Insert(0, "delay_s");
                }
                if (param == "col_otime_input")
                {
                    NodeMaster.SetNodeReg((byte)curr_node_index, new string[]{"open_s"}, new UInt32[]{UInt32.Parse(data)});
                    apply_regs.Insert(0, "open_s");
                }
                if (param == "openwei_input")
                {
                    NodeMaster.SetNodeReg((byte)curr_node_index, new string[]{"delay_f"}, new UInt32[]{UInt32.Parse(data)});
                    apply_regs.Insert(0, "delay_f");
                }
                if (param == "motor_speed_input")
                {
                    NodeMaster.SetNodeReg((byte)curr_node_index, new string[]{"motor_speed"}, new UInt32[]{UInt32.Parse(data)});
                    apply_regs.Insert(0, "motor_speed");
                }
                if (param == "run_freq")
                {
                    NodeMaster.SetNodeReg(curr_packer.vib_addr, new string[]{"magnet_freq"}, new UInt32[]{UInt32.Parse(data)});
                }
                if (param == "run_time")
                {
                    NodeMaster.SetNodeReg(curr_packer.vib_addr, new string[]{"magnet_time"}, new UInt32[]{UInt32.Parse(data)});
                }
                
                if (param == "run_amp")
                {
                    NodeMaster.SetNodeReg(curr_packer.vib_addr, new string[]{"magnet_amp"}, new UInt32[]{UInt32.Parse(data)});
                }
                if (param == "autoamp")
                {
                    curr_packer.curr_cfg.target_comb  = Double.Parse(data);
                    cb_autoamp.Checked = (curr_packer.curr_cfg.target_comb > 1);
                    bNeedUpdateComb = true;
                }
                if (param == "target")
                {
                    curr_packer.curr_cfg.target = Double.Parse(data);
                    bNeedUpdateComb = true;
                }
                if (param == "uvar")
                {
                    curr_packer.curr_cfg.upper_var = Double.Parse(data);
                }
                if (param == "dvar")
                {
                    curr_packer.curr_cfg.lower_var = Double.Parse(data);
                }

           
                ShowStatus("modifying");
                if (bNeedUpdateComb)
                    curr_packer.UpdateEachNodeTarget();
                lastcall = "UpdateUI";
            }
            catch (System.Exception e)
            {
                //MessageBox.Show("Invalid Parameter");
                return;
            }
        }
        private void input_GotFocus(object sender, EventArgs e)
        {
            if (curr_packer.status == PackerStatus.RUNNING)
            {
                //MessageBox.Show("is_running");
                return;
            }
            string regname = (sender as RectButton).Name;
            Program.kbdwnd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }

        private void btn_action_Click(object sender, EventArgs e)
        {
            byte n = (byte)curr_node_index;
            string name = (sender as RectButton).Name;
            if (name == "btn_mainvib")
            {
                NodeMaster.Action(new byte[] { curr_packer.bot_addr }, "fill");
            }
            if (name == "btn_pack")
            {
                NodeMaster.Action(new byte[] { curr_packer.bot_addr }, "trigger");
            }
            if (name == "btn_pass" || name == "btn_empty" || name == "btn_fill" || name == "btn_flash" || name == "btn_zero")
            {
                if (!runPan1.bSelectAll)
                {
                    NodeMaster.Action(new byte[] { n }, name.Replace("btn_", ""));
                }
                else
                {
                    NodeMaster.Action(curr_packer.wn_addrs,name.Replace("btn_", "")) ;
                }
            }
            
        }
        private void prd_pic_selected(string item)
        {
            curr_packer.curr_cfg.product_desc = item;
            ShowStatus("loading");
            lastcall = "UpdateUI";
        }

        private void prd_desc_selected(string item)
        {
            curr_packer.curr_cfg.product_desc = item;
            ShowStatus("loading");
            lastcall = "UpdateUI";
        }
        private void prd_no_selected(string item)
        {
            curr_packer.curr_cfg.product_no = item;

            ShowStatus("loading");
            lastcall = "UpdateProdNo";
        }
        private void btn_prd_desc_Click(object sender, EventArgs e)
        {
            Program.prodwnd.Init(prd_desc_selected);
        }
        private void ToggleStartStop()
        {
            if (curr_packer.status == PackerStatus.RUNNING)
            {
                curr_packer.StopRun();
                this.btn_trial.Text = StringResource.str("sall_start");
            }
            else
            {
                curr_packer.bSimulate = true;
                curr_packer.StartRun();
                this.btn_trial.Text = StringResource.str("sall_stop");
            }
        }
        private void btn_trial_Click(object sender, EventArgs e)
        {
            lastcall = "StartStop";
            if(curr_packer.status == PackerStatus.RUNNING)
                ShowStatus(StringResource.str("stopping"));
            else
                ShowStatus(StringResource.str("starting"));
        }
        private void btn_save_Click(object sender, EventArgs e)
        {
            ShowStatus("store_setting");
            lastcall = "save_config";
        }

        private void btn_prd_no_Click(object sender, EventArgs e)
        {
            Program.prodnum.Init(prd_no_selected,true);
        }

        private void btn_pack_dvar_Click(object sender, EventArgs e)
        {
            Program.kbdwnd.Init(StringResource.str("enter_dvar"), "dvar", false, KbdData);
        }
        private void btn_pack_target_Click(object sender, EventArgs e)
        {
            Program.kbdwnd.Init(StringResource.str("enter_target"), "target", false, KbdData);
        }
        private void btn_pack_uvar_Click(object sender, EventArgs e)
        {
            Program.kbdwnd.Init(StringResource.str("enter_uvar"), "uvar", false, KbdData);
        }
        private void ShowStatus(string msg)
        {
            txt_oper.Text = StringResource.str(msg);
            txt_oper.Visible = true;
        }
        private List<string> apply_regs = new List<string>();
        private void sub_applyall()
        {
            UIPacker pack = curr_packer;
            
            byte cn = (byte)curr_node_index;

            bool star = true;
            foreach (byte n in curr_packer.weight_nodes) 
            {
                foreach (string reg in apply_regs.Distinct())
                {
                    if (NodeMaster.GetStatus(n) == NodeStatus.ST_LOST)
                        continue;
                    if (NodeMaster.GetStatus(n) == NodeStatus.ST_DISABLED)
                        continue;

                    if (n == cn)
                        continue;
                    UInt32 val = NodeMaster.GetNodeReg(cn, reg);
                    NodeMaster.SetNodeReg(n, new string[] { reg }, new UInt32[] { val });

                    if (star)
                    {
                        txt_oper.Text = StringResource.str("modifying") +" "+n.ToString()+"*";
                    }
                    else
                    {
                        txt_oper.Text = StringResource.str("modifying") + " " + n.ToString();
                     }
                    star = !star;
                }
            }
            apply_regs.Clear();   
        }
        private void btn_sub_applyall_Click(object sender, EventArgs e)
        {
            //apply for other nodes.
            if (lastcall != "")
                return;
            ShowStatus("modifying");
            lastcall = "ApplyToAll";
        }
        public void new_prd_no_input(string param, string data)
        {
            if (curr_packer.pkg_confs.Keys.Contains<string>(data))
            {
                MessageBox.Show(StringResource.str("used_prdno"));
                lastcall = "select_prdno";
            }
            else
            {
                curr_packer.DuplicateCurrentConfig(data);
                ShowStatus("loading");
                lastcall = "UpdateUI";
            }
        }
        private void new_prd_Click(object sender, EventArgs e)
        {
            Program.kbdwnd.Init(StringResource.str("input_newprdno"), "", false, new_prd_no_input);
        }

        private void btn_ret_config_Click(object sender, EventArgs e)
        {
            if (curr_packer.status != PackerStatus.RUNNING)
            {
                while (lastcall != "")
                    this.uiTimer_Tick(null, null);
                Program.SwitchTo("configmenu");
            }
        }
        private void btn_ret_run_Click(object sender, EventArgs e)
        {
            if (curr_packer.status != PackerStatus.RUNNING)
            {
                while (lastcall != "")
                    this.uiTimer_Tick(null, null);
                Program.SwitchTo("runmode");
            }
        }
        private void lbl_currNode_Click(object sender, EventArgs e)
        {

        }
        private void main_input_click(object sender, EventArgs e)
        {
            if (curr_packer.status == PackerStatus.RUNNING)
            {
                //MessageBox.Show("is_running");
                return;
            }
            string regname = (sender as Label).Name;
            Program.kbdwnd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }

        private void btn_selectall_Click(object sender, EventArgs e)
        {
            runPan1.bSelectAll = !runPan1.bSelectAll;
        }

        private void cb_autoamp_Click(object sender, EventArgs e)
        {
            if (curr_node_index < 0)
                return;
            if (!cb_autoamp.Checked)
            {
                KbdData("autoamp", "0");
            }
            else
            {
                Program.kbdwnd.Init(StringResource.str("enter_autoamp"), "autoamp", false, KbdData);
            }
        }

        private void ellipseWithImageBrush_MouseLeftButtonUp(object sender, MouseEventArgs e)
        {
            Program.prodwnd.Init(prd_pic_selected);
        }
    }
}