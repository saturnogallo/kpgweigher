using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Text.RegularExpressions;


namespace ioex_cs
{
    /// <summary>
    /// Interaction logic for SingleMode.xaml
    /// </summary>
    public partial class SingleMode : Window
    {
        System.Windows.Forms.Timer uiTimer;
        private object lastsender = null;
        private string lastcall = "";
        private bool bSelectAll = false;
        private void HandleLongCommand()
        {
            if (lastcall == "UpdateProdNo")
            {
                App p = Application.Current as App;
                p.curr_packer.LoadConfig(p.curr_packer.curr_cfg.product_no);
                p.curr_packer.SaveCurrentConfig();
                lastcall = "UpdateUI";
            }
            if (lastcall.IndexOf("cali") == 0)
            {
                calibrate(lastcall);
                lastcall = "";
                MessageBox.Show(StringResource.str("cali_done"));
            }
            if (lastcall == "UpdateUI")
                UpdateUI();
            if (lastcall == "ApplyToAll")
                sub_applyall();
            if (lastcall == "select_prdno")
                new_prd_Click(null, null);
            if (lastcall == "save_config")
            {
                (Application.Current as App).curr_packer.SaveCurrentConfig();
                lastcall = "";
                MessageBox.Show(StringResource.str("store_done"));
            }
            lastcall = "";
            txt_oper.Visibility = Visibility.Hidden;
            bg_oper.Visibility = Visibility.Hidden;
        }
        public SingleMode()
        {
            InitializeComponent();
            curr_btn = null;
            last_btn = null;
            uiTimer = new System.Windows.Forms.Timer();
            uiTimer.Tick += new EventHandler(uiTimer_Tick);
            uiTimer.Interval = 100;
            uiTimer.Start();
        }
        private App currentApp()
        {
            return (System.Windows.Application.Current as App);
        }

        private Button curr_btn;
        private Button last_btn;
        void uiTimer_Tick(object sender, EventArgs e)
        {
            if (!this.IsVisible)
                return;

            UIPacker p = currentApp().curr_packer;
            if (p.curr_node_index == -1)
            {
                bucket_click(1);
            }else{
                if (p.NeedRefresh || p.status == PackerStatus.RUNNING)
                {
                    foreach (byte n in p.weight_nodes)
                    {
                        UpdateNodeUI("wei_node" + n.ToString());
                    }
                }
                if (lastcall != "")
                    HandleLongCommand();
            }
            if (p.status == PackerStatus.RUNNING)
            {
                lbl_speed.Content = p.speed.ToString();
                lbl_lastweight.Content = p.last_pack_weight.ToString("F2");
                lbl_totalpack.Content = p.total_sim_packs.ToString();
                lbl_totalweights.Content = p.total_sim_weights.ToString("F2");
            }
        }
        private void UpdateNodeUI(string param)
        {
            Label lb = this.FindName(param) as Label;
            Button btn = this.FindName(param.Replace("wei_node", "bucket")) as Button;
            byte n = (byte)(RunMode.StringToId(param));
            NodeAgent agent = currentApp().curr_packer.agent;
            
            if(agent.weight(n) > -1000)
                lb.Content = agent.weight(n).ToString("F1");
            if (agent.GetStatus(n) == NodeStatus.ST_LOST)
            {
                btn.Template = this.FindResource("WeightBarError") as ControlTemplate;
            }
            if (n != currentApp().curr_packer.vib_addr)
            {
                if (agent.GetWNodeFlag(n,"bRelease"))
                    btn.Template = this.FindResource("WeightBarRelease") as ControlTemplate;
            }
            if (agent.GetStatus(n) == NodeStatus.ST_IDLE)
            {
                if(btn == curr_btn || bSelectAll)
                    btn.Template = this.FindResource("WeightBarFocus") as ControlTemplate;
                else
                    btn.Template = this.FindResource("WeightBar") as ControlTemplate;
            }
            btn.ApplyTemplate();
        }
        private void weibucket_Click(object sender, RoutedEventArgs e)
        {
            bucket_click(ButtonToId(sender));
            bSelectAll = false;
        }
        private void passbucket_Click(object sender, RoutedEventArgs e)
        {
            bucket_click(ButtonToId(sender));
            bSelectAll = false;
        }
        private void vibbucket_Click(object sender, RoutedEventArgs e)
        {
            bucket_click(ButtonToId(sender));
            bSelectAll = false;
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
        
        private Button IdToButton(int nodeid)
        {
            return this.FindName("bucket" + nodeid) as Button;
        }
        private void bucket_click(int nodeid)
        {
            App p = Application.Current as App;
            int last_node = p.curr_packer.curr_node_index;
            if (nodeid < 1)
            {
                return;
            }
            
            last_btn = IdToButton(last_node);
            curr_btn = IdToButton(nodeid);

            p.curr_packer.curr_node_index = nodeid;

            if (last_btn is Button)
            {
                last_btn.Template = this.FindResource("WeightBar") as ControlTemplate;
                last_btn.ApplyTemplate();
            }
            if (curr_btn is Button)
            {
                curr_btn.Template = this.FindResource("WeightBarFocus") as ControlTemplate;
                curr_btn.ApplyTemplate();
            }
            //update parameter of the current node to UI.
            lastcall = "UpdateUI";
            return;
            
            
        }
        private void UpdateUI()
        {
            App p = Application.Current as App;
            NodeAgent n = p.curr_packer.agent;
            byte vn = p.curr_packer.vib_addr;

            sub_freq_input.Content = n.GetNodeReg((byte)p.curr_packer.curr_node_index, "magnet_freq");
            sub_amp_input.Content = n.GetNodeReg((byte)p.curr_packer.curr_node_index, "magnet_amp");
            sub_time_input.Content = n.GetNodeReg((byte)p.curr_packer.curr_node_index, "magnet_time");

            wei_otime_input.Content = n.GetNodeReg((byte)p.curr_packer.curr_node_index, "open_w");
            wei_dtime_input.Content = n.GetNodeReg((byte)p.curr_packer.curr_node_index, "delay_w");
            col_dtime_input.Content = n.GetNodeReg((byte)p.curr_packer.curr_node_index, "delay_s");
            col_otime_input.Content = n.GetNodeReg((byte)p.curr_packer.curr_node_index, "open_s");
            openwei_input.Content = n.GetNodeReg((byte)p.curr_packer.curr_node_index, "delay_f");

            motor_speed_input.Content = n.GetNodeReg((byte)p.curr_packer.curr_node_index, "motor_speed");

            run_freq.Content = n.GetNodeReg(p.curr_packer.vib_addr,"magnet_freq");
            run_amp.Content = n.GetNodeReg(p.curr_packer.vib_addr,"magnet_amp");
            run_time.Content = n.GetNodeReg(p.curr_packer.vib_addr, "magnet_time");
            btn_prd_no.Content = p.curr_packer.curr_cfg.product_no.ToString();
            btn_prd_desc.Content = p.curr_packer.curr_cfg.product_desc.ToString();
            btn_target.Content = p.curr_packer.curr_cfg.target.ToString() +StringResource.str("gram");
            btn_uvar.Content = p.curr_packer.curr_cfg.upper_var.ToString() + StringResource.str("gram");
            btn_dvar.Content = p.curr_packer.curr_cfg.lower_var.ToString() + StringResource.str("gram");

            lbl_currNode.Content = (p.curr_packer.curr_node_index).ToString();
            Rectangle rect = this.FindName("ellipseWithImageBrush") as Rectangle;
            //load the corresponding pictiure.
            (rect.Fill as ImageBrush).ImageSource = new BitmapImage(new Uri("c:\\ioex\\prodpic\\" + p.curr_packer.curr_cfg.product_desc.ToString() + ".jpg"));
            
        }
        private void node_reg(string regname)
        {
            App p = Application.Current as App;
            if (p.curr_packer.status == PackerStatus.RUNNING)
            {
                //MessageBox.Show("is_running");
                return;
            }
            p.kbdwnd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }
        public void KbdData(string param, string data)
        {
            //update the display based on keyboard input
            App p = Application.Current as App;
            try
            {
                
                UIPacker pack = p.curr_packer;
                NodeAgent n = p.curr_packer.agent;
                
                if (param == "sub_freq_input")
                {
                    n.SetNodeReg((byte)p.curr_packer.curr_node_index, "magnet_freq", UInt32.Parse(data));
                }
                if (param == "sub_amp_input")
                {
                    n.SetNodeReg((byte)p.curr_packer.curr_node_index, "magnet_amp", UInt32.Parse(data));
                }
                if (param == "sub_time_input")
                {
                    n.SetNodeReg((byte)p.curr_packer.curr_node_index, "magnet_time", UInt32.Parse(data));
                }
                if (param == "wei_otime_input")
                {
                    n.SetNodeReg((byte)p.curr_packer.curr_node_index, "open_w", UInt32.Parse(data));
                }
                if (param == "wei_dtime_input")
                {
                    n.SetNodeReg((byte)p.curr_packer.curr_node_index, "delay_w", UInt32.Parse(data));
                }
                if (param == "col_dtime_input")
                {
                    n.SetNodeReg((byte)p.curr_packer.curr_node_index, "delay_s", UInt32.Parse(data));
                }
                if (param == "col_otime_input")
                {
                    n.SetNodeReg((byte)p.curr_packer.curr_node_index, "open_s", UInt32.Parse(data));
                }
                if (param == "openwei_input")
                {
                    n.SetNodeReg((byte)p.curr_packer.curr_node_index, "delay_f", UInt32.Parse(data));
                }
                if (param == "motor_speed_input")
                {
                    n.SetNodeReg((byte)p.curr_packer.curr_node_index, "motor_speed", UInt32.Parse(data));
                }
                if (param == "run_freq")
                {
                    n.SetNodeReg(p.curr_packer.vib_addr, "magnet_freq", UInt32.Parse(data));
                }
                if (param == "run_time")
                {
                    n.SetNodeReg(p.curr_packer.vib_addr, "magnet_time", UInt32.Parse(data));
                }
                
                if (param == "run_amp")
                {
                    n.SetNodeReg(p.curr_packer.vib_addr, "magnet_amp", UInt32.Parse(data));
                }
                if (param == "target")
                {
                    p.curr_packer.curr_cfg.target = Double.Parse(data);
                }
                if (param == "uvar")
                {
                    p.curr_packer.curr_cfg.upper_var = Double.Parse(data);
                }
                if (param == "dvar")
                {
                    p.curr_packer.curr_cfg.lower_var = Double.Parse(data);
                }

                if (param == "cali1" || param == "cali2" || param == "cali3" || param == "cali4" || param == "cali5")
                {
                    ShowStatus("calibrating");
                    int i = RunMode.StringToId(param) - 1;
                    if (p.curr_packer.curr_node_index >= 0)
                    {
                        n.SetNodeReg((byte)p.curr_packer.curr_node_index, "poise_weight_gram" + i.ToString(), UInt32.Parse(data));
                        string cs_mtrl_val = n.GetNodeReg((byte)p.curr_packer.curr_node_index, "cs_mtrl");
                        n.SetNodeReg((byte)p.curr_packer.curr_node_index, "cs_poise" + i.ToString(), UInt32.Parse(cs_mtrl_val));
                    }
                    p.curr_packer.agent.Action((byte)p.curr_packer.curr_node_index, "flash");
                    return;
                }
                ShowStatus("modifying");
                lastcall = "UpdateUI";
            }
            catch (System.Exception e)
            {
                MessageBox.Show("Invalid Parameter");
                
                return;
            }
        }

        private void input_GotFocus(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            if (p.curr_packer.status == PackerStatus.RUNNING)
            {
                //MessageBox.Show("is_running");
                return;
            }
            string regname = (sender as Label).Name;
            p.kbdwnd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }
        private void calibrate(string calreg)
        {
            App p = Application.Current as App;
            if (calreg == "cali0")
            {
                NodeAgent n = p.curr_packer.agent;
                if (p.curr_packer.curr_node_index >= 0)
                {
                    string cs_mtrl_val = n.GetNodeReg((byte)p.curr_packer.curr_node_index, "cs_mtrl");
                    n.SetNodeReg((byte)p.curr_packer.curr_node_index, "cs_zero", UInt32.Parse(cs_mtrl_val));
                    p.curr_packer.agent.Action((byte)p.curr_packer.curr_node_index, "flash");
                }
            }
            if (calreg == "cali1" )
                KbdData(calreg, "50");
            if (calreg == "cali2")
                KbdData(calreg, "100");
            if (calreg == "cali3")
                KbdData(calreg, "200");
            if (calreg == "cali4")
                KbdData(calreg, "300");
            if (calreg == "cali5")
                KbdData(calreg, "500");

        }
        private void btn_cali_Click(object sender, RoutedEventArgs e)
        {
            string calreg = (sender as Button).Name.Remove(0,4); //remove "btn_" string
            

            ShowStatus("modifying");
            lastcall = calreg;                

        }

        private void btn_action_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            byte n = (byte)p.curr_packer.curr_node_index;
            string name = (sender as Button).Name;
            if (name == "btn_mainvib")
            {
                p.curr_packer.agent.Action(p.curr_packer.vib_addr, "fill");
            }
            if (name == "btn_pass" || name == "btn_empty" || name == "btn_fill" || name == "btn_flash" || name == "btn_zero")
            {
                if (!bSelectAll)
                    p.curr_packer.agent.Action(n, name.Replace("btn_", ""));
                else
                {
                    if (name == "btn_flash")
                    {
                        foreach (byte addr in p.curr_packer.weight_nodes)
                        {
                            p.curr_packer.agent.Action(addr, name.Replace("btn_", ""));
                        }
                    }
                    else
                    {
                        p.curr_packer.agent.Action((byte)(0x80+p.curr_packer._pack_id), name.Replace("btn_", ""));
                    }
                }
            }
        }
        private void prd_desc_selected(string item)
        {
            App p = Application.Current as App;
            p.curr_packer.curr_cfg.product_desc = item;
            ShowStatus("loading");
            lastcall = "UpdateUI";
        }
        private void prd_no_selected(string item)
        {
            App p = Application.Current as App;
            p.curr_packer.curr_cfg.product_no = item;

            ShowStatus("loading");
            lastcall = "UpdateProdNo";
        }
        private void btn_prd_desc_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            p.prodwnd.Init(prd_desc_selected);
        }

        private void btn_trial_Click(object sender, RoutedEventArgs e)
        {
            btn_trial.Click -= btn_trial_Click;
            App p = Application.Current as App;
            if (p.curr_packer.status == PackerStatus.RUNNING)
            {
                p.curr_packer.StopRun();
                this.btn_trial.Content = StringResource.str("sall_start");
            }
            else
            {
                App.bSimulate = true;
                p.curr_packer.StartRun();
                this.btn_trial.Content = StringResource.str("sall_stop");
                p.curr_packer.status = PackerStatus.RUNNING;
            }
            btn_trial.Click += btn_trial_Click;
        }
        private void btn_save_Click(object sender, RoutedEventArgs e)
        {
            ShowStatus("store_setting");
            lastcall = "save_config";
            
        }

        private void btn_prd_no_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            p.prodnum.Init(prd_no_selected,true);
        }

        private void btn_pack_dvar_Click(object sender, RoutedEventArgs e)
        {
            (Application.Current as App).kbdwnd.Init(StringResource.str("enter_dvar"), "dvar", false, KbdData);
        }
        private void btn_pack_target_Click(object sender, RoutedEventArgs e)
        {
            (Application.Current as App).kbdwnd.Init(StringResource.str("enter_target"), "target", false, KbdData);
        }
        private void btn_pack_uvar_Click(object sender, RoutedEventArgs e)
        {
            (Application.Current as App).kbdwnd.Init(StringResource.str("enter_uvar"), "uvar", false, KbdData);
        }
        private void ShowStatus(string msg)
        {
            txt_oper.Content = StringResource.str(msg);
            txt_oper.Visibility = Visibility.Visible;
            bg_oper.Visibility = Visibility.Visible;

        }
        private void sub_applyall()
        {
            App p = Application.Current as App;
            UIPacker pack = p.curr_packer;
            NodeAgent agent = pack.agent;
            byte cn = (byte)p.curr_packer.curr_node_index;

            string[] regs = { "magnet_freq", "magnet_amp", "magnet_time", "open_w", "delay_w", "open_s", "delay_s", "delay_f", "motor_speed", "target_weight", "cs_filter", "cs_gain_wordrate" };
            foreach (string reg in regs)
            {

                UInt32 val = UInt32.Parse(p.curr_packer.agent.GetNodeReg(cn, reg));
                foreach (byte n in p.curr_packer.weight_nodes)
                {
                    if (agent.GetStatus(n) == NodeStatus.ST_LOST)
                        continue;
                    if (n == cn)
                        continue;
                    p.curr_packer.agent.SetNodeReg(n, reg, val);
                }
            }
        }
        private void btn_sub_applyall_Click(object sender, RoutedEventArgs e)
        {
            //apply for other nodes.
            ShowStatus("modifying");
            lastcall = "ApplyToAll";
        }
        public void new_prd_no_input(string param, string data)
        {
            if ((Application.Current as App).curr_packer.all_conf.Keys.Contains<string>(data))
            {
                MessageBox.Show(StringResource.str("used_prdno"));
                lastcall = "select_prdno";
            }
            else
            {
                (Application.Current as App).curr_packer.DuplicateCurrentConfig(data);
                ShowStatus("loading");
                lastcall = "UpdateUI";
            }
        }
        private void new_prd_Click(object sender, RoutedEventArgs e)
        {
            (Application.Current as App).kbdwnd.Init(StringResource.str("input_newprdno"), "", false, new_prd_no_input);
        }

        private void btn_ret_config_Click(object sender, RoutedEventArgs e)
        {
            if ((Application.Current as App).curr_packer.status != PackerStatus.RUNNING)
                (Application.Current as App).SwitchTo("configmenu");
        }
        private void btn_ret_run_Click(object sender, RoutedEventArgs e)
        {
            if ((Application.Current as App).curr_packer.status != PackerStatus.RUNNING)
                (Application.Current as App).SwitchTo("runmode");
        }
        private void lbl_currNode_Click(object sender, RoutedEventArgs e)
        {

        }
        private void main_input_click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            if (p.curr_packer.status == PackerStatus.RUNNING)
            {
                //MessageBox.Show("is_running");
                return;
            }
            string regname = (sender as Label).Name;
            p.kbdwnd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }

        private void btn_selectall_Click(object sender, RoutedEventArgs e)
        {
            bSelectAll = !bSelectAll;
            lastcall = "UpdateUI";
            btn_selectall.Style = bSelectAll ? this.FindResource("ButtonStyle2") as Style : this.FindResource("ButtonStyle") as Style;
        }
    }
}
