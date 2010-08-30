﻿using System;
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
        public SingleMode()
        {
            InitializeComponent();
            uiTimer = new System.Windows.Forms.Timer();
            uiTimer.Tick += new EventHandler(uiTimer_Tick);
            uiTimer.Interval = 10;
            uiTimer.Start();
        }
        private App currentApp()
        {
            return (System.Windows.Application.Current as App);
        }
        void uiTimer_Tick(object sender, EventArgs e)
        {
            if (!this.IsVisible)
                return;
            
            App p = currentApp();
            if (p.curr_packer.curr_node == -1)
            {
                bucket_click(1);
            }

            p.bMainPause = true;
            foreach (WeighNode n in p.curr_packer.weight_node)
            {
                UpdateNodeUI("wei_node" + n.node_id);
            }
            p.bMainPause = false;
        }
        private void UpdateNodeUI(string param)
        {
            Label lb = this.FindName(param) as Label;
            Button btn = this.FindName(param.Replace("wei_node", "bucket")) as Button;
            WeighNode n = currentApp().curr_packer.weight_node[RunMode.StringToId(param) - 1];

            string ct = n.weight.ToString();
            lb.Content = ct;
            if (n.status == NodeStatus.ST_LOST)
            {
                (btn.FindName("BarPath") as Path).Fill = Brushes.Chocolate;
            }
            if (n.status == NodeStatus.ST_RELEASE)
            {
                (btn.FindName("BarPath") as Path).Fill = Brushes.OrangeRed;
            }
        }
        private void weibucket_Click(object sender, RoutedEventArgs e)
        {
            bucket_click(ButtonToId(sender));
        }
        private void passbucket_Click(object sender, RoutedEventArgs e)
        {
            bucket_click(ButtonToId(sender));
        }
        private void vibbucket_Click(object sender, RoutedEventArgs e)
        {
            bucket_click(ButtonToId(sender));
        }
        private int ButtonToId(object sender)
        {
            StringBuilder sb = new StringBuilder();
            Regex re = new Regex("(\\d+)");
            Match m2 = re.Match((sender as Button).Name);
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
            int last_node = p.curr_packer.curr_node;
            if (nodeid < 1)
            {
                return;
            }
            
            Button lastbtn = IdToButton(last_node+1);
            Button currbtn = IdToButton(nodeid);
            if (lastbtn is Button)
            {
                lastbtn.BorderThickness = new Thickness(2);
                lastbtn.BorderBrush = Brushes.DarkGray;

            }

            currbtn.BorderThickness = new Thickness(6);
            currbtn.BorderBrush = Brushes.Black;
            p.curr_packer.curr_node = nodeid - 1;
            //update parameter of the current node to UI.
            p.bMainPause = true;
            WeighNode n = p.curr_packer.weight_node[p.curr_packer.curr_node];
            VibrateNode vn = p.curr_packer.vib_node;
            n["magnet_freq"] = null;
            n["magnet_amp"] = null;
            n["magnet_time"] = null;
            n["open_w"] = null;
            n["delay_w"] = null;
            n["delay_s"] = null;
            n["open_s"] = null;
            n["delay_f"] = null;
            n["motor_speed"] = null;
            vn["magnet_freq"] = null;
            vn["magnet_amp"] = null;
            UpdateUI();
            p.bMainPause = false;
        }
        private void UpdateUI()
        {
            App p = Application.Current as App;
            WeighNode n = p.curr_packer.weight_node[p.curr_packer.curr_node];
            VibrateNode vn = p.curr_packer.vib_node;
            sub_freq_input.Text = n["magnet_freq"].ToString();
            sub_amp_input.Text = n["magnet_amp"].ToString();
            sub_time_input.Text = n["magnet_time"].ToString();

            wei_otime_input.Text = n["open_w"].ToString();
            wei_dtime_input.Text = n["delay_w"].ToString();
            col_dtime_input.Text = n["delay_s"].ToString();
            col_otime_input.Text = n["open_s"].ToString();
            openwei_input.Text = n["delay_f"].ToString();

            motor_speed_input.Text = n["motor_speed"].ToString();

            run_freq.Text = vn["magnet_freq"].ToString();
            run_amp.Text = vn["magnet_amp"].ToString();
            btn_prd_no.Content = p.curr_packer.curr_cfg.product_no.ToString();
            btn_prd_desc.Content = p.curr_packer.curr_cfg.product_desc.ToString();
            btn_target.Content = p.curr_packer.curr_cfg.target.ToString();
            btn_uvar.Content = p.curr_packer.curr_cfg.upper_var.ToString();
            btn_dvar.Content = p.curr_packer.curr_cfg.lower_var.ToString();

            Rectangle rect = this.FindName("ellipseWithImageBrush") as Rectangle;
            //load the corresponding pictiure.
            (rect.Fill as ImageBrush).ImageSource = new BitmapImage(new Uri("f:\\" + p.curr_packer.curr_cfg.product_desc.ToString() + ".jpg"));

        }
        private void node_reg(string regname)
        {
            App p = Application.Current as App;
            if (p.curr_packer.status == PackerStatus.RUNNING)
            {
                MessageBox.Show("is_running");
                return;
            }
            p.kbdwnd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }
        public void KbdData(string param, string data)
        {
            //update the display based on keyboard input
            try
            {
                App p = Application.Current as App;
                Packer pack = p.curr_packer;
                WeighNode n = p.curr_packer.weight_node[p.curr_packer.curr_node];
                VibrateNode vn = p.curr_packer.vib_node;

                if (param == "sub_freq_input")
                {
                    n["magnet_freq"] = UInt32.Parse(data);
                }
                if (param == "sub_amp_input")
                {
                    n["magnet_amp"] = UInt32.Parse(data);
                }
                if (param == "sub_time_input")
                {
                    n["magnet_time"] = UInt32.Parse(data);
                }
                if (param == "wei_otime_input")
                {
                    n["open_w"] = UInt32.Parse(data);
                }
                if (param == "wei_dtime_input")
                {
                    n["delay_w"] = UInt32.Parse(data);
                }
                if (param == "col_dtime_input")
                {
                    n["delay_s"] = UInt32.Parse(data);
                }
                if (param == "col_otime_input")
                {
                    n["open_s"] = UInt32.Parse(data);
                }
                if (param == "openwei_input")
                {
                    n["delay_f"] = UInt32.Parse(data);
                }
                if (param == "motor_speed_input")
                {
                    n["motor_speed"] = UInt32.Parse(data);
                }
                if (param == "run_freq")
                {
                    vn["magnet_freq"] = UInt32.Parse(data);
                }
                if (param == "run_amp")
                {
                    vn["magnet_amp"] = UInt32.Parse(data);
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
                if (param == "prod_no")
                {
                    p.curr_packer.LoadConfig(data);
                }
                if (param == "cali0" || param == "cali1" || param == "cali2" || param == "cali3" || param == "cali4")
                {
                    int i = RunMode.StringToId(param);
                    n["Poise_Weight_gram"+i.ToString()] = UInt32.Parse(data);
                    n["cs_poise"+i.ToString()] = n["cs_mtrl"].Value;
                }
  
                UpdateUI();
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
                MessageBox.Show("is_running");
                return;
            }
            string regname = (sender as TextBox).Name;
            p.kbdwnd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }

        private void btn_cali_Click(object sender, RoutedEventArgs e)
        {
            string calreg = (sender as TextBox).Name;
            App p = Application.Current as App;
            WeighNode n = p.curr_packer.weight_node[p.curr_packer.curr_node];
            if (calreg == "cali0")
            {
                (Application.Current as App).kbdwnd.Init(StringResource.str("enter_"+calreg), calreg, false, KbdData);
            }
        }

        private void btn_action_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            SubNode n = p.nodes[p.curr_packer.curr_node];
            string name = (sender as Button).Name;
            if (name == "btn_pass" || name == "btn_empty" || name == "btn_fill" || name == "btn_flash")
            {
                n.Action(name.Replace("btn_",""), true);
            }
        }
        private void prd_desc_selected(string item)
        {
            App p = Application.Current as App;
            p.curr_packer.curr_cfg.product_desc = item;
            this.UpdateUI();
        }
        private void btn_prd_desc_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            p.prodwnd.Init(prd_desc_selected);
        }

        private void btn_trial_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            if (p.curr_packer.status == PackerStatus.RUNNING)
            {
                p.curr_packer.status = PackerStatus.IDLE;
                this.btn_trial.Content = StringResource.str("all_start");
            }
            else
            {
                p.bSimulate = true;
                p.curr_packer.status = PackerStatus.RUNNING;
                this.btn_trial.Content = StringResource.str("all_stop");
            }
        }

        private void btn_save_Click(object sender, RoutedEventArgs e)
        {
            (Application.Current as App).curr_packer.SaveCurrentConfig();
        }

        private void btn_prd_no_Click(object sender, RoutedEventArgs e)
        {
            (Application.Current as App).kbdwnd.Init(StringResource.str("enter_prd_no"), "prd_no", false, KbdData);
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
            (Application.Current as App).kbdwnd.Init(StringResource.str("enter_dvar"), "uvar", false, KbdData);
        }
        void ShowStatus(string msg)
        {
            lbl_summary.Content = StringResource.str(msg);
        }
        private void btn_sub_applyall_Click(object sender, RoutedEventArgs e)
        {
            //apply for other nodes.
            App p = Application.Current as App;
            Packer pack = p.curr_packer;
            WeighNode cn = p.curr_packer.weight_node[p.curr_packer.curr_node];
            ShowStatus("updating");
            string[] regs ={"magnet_freq","magnet_amp","magnet_time","open_w","delay_w","open_s","delay_s","delay_f","motor_speed"};
            foreach (string reg in regs)
            {
                
                UInt32 val = cn[reg].Value;
                foreach (WeighNode n in p.curr_packer.weight_node)
                {
                    if (n.status == NodeStatus.ST_LOST)
                        continue;
                    if (n == cn)
                        continue;
                    n[reg] = val;
                }
            }
            ShowStatus("done");
        }

        private void new_prd_Click(object sender, RoutedEventArgs e)
        {
            (Application.Current as App).curr_packer.DuplicateCurrentConfig("");
            UpdateUI();
            
        }

        private void btn_ret_config_Click(object sender, RoutedEventArgs e)
        {
            (Application.Current as App).SwitchTo("ConfigMenu");
        }

        private void btn_ret_run_Click(object sender, RoutedEventArgs e)
        {
            (Application.Current as App).SwitchTo("runmode");
        }

    }
}