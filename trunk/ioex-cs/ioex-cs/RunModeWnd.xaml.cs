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
using System.Collections.ObjectModel;
using System.Text.RegularExpressions;
using System.Threading;
using System.Xml.Linq;
namespace ioex_cs
{
    /// <summary>
    /// Interaction logic for RunMode.xaml
    /// </summary>

    public partial class RunMode : Window
    {
        private string lastcall = "";
        private byte curr_node = 0xff;
        System.Windows.Forms.Timer uiTimer;
        private App currentApp()
        {
            return Application.Current as App;
        }
        public RunMode()
        {
            InitializeComponent();

            this.Loaded +=new RoutedEventHandler(RunMode_Loaded);
            uiTimer = new System.Windows.Forms.Timer();
            uiTimer.Tick += new EventHandler(uiTimer_Tick);
            uiTimer.Interval = 200;
            uiTimer.Start();
        }

        void uiTimer_Tick(object sender, EventArgs e)
        {
            
            lbl_datetime.Content = DateTime.Now.ToLongDateString() + "\n" + DateTime.Now.ToLongTimeString();

            if (!this.IsVisible)
                return;

            UIPacker p = (System.Windows.Application.Current as App).curr_packer;
            if (lastcall != "")
            {
                if (lastcall == "UpdatePrdNo")
                {
                    p.LoadConfig(p.curr_cfg.product_no);
                    UpdateUI("sys_config");
                    Thread.Sleep(3000);
                }
                lastcall = "";

                txt_oper.Visibility = Visibility.Hidden;
                bg_oper.Visibility = Visibility.Hidden;
                return;
            }
        
            if (!p.NeedRefresh) //no data update
                return;

            lbl_status.Content = "";
            foreach (byte n in p.weight_nodes)
            {
                UpdateUI("wei_node" + n);
            }
            if (lbl_status.Content.ToString() == "")
            {
                lbl_status.Content = StringResource.str("normal");
                lbl_status.Foreground = Brushes.Green;
            }
            else
            {
                lbl_status.Foreground = Brushes.Red;
            }
            if (p.status == PackerStatus.RUNNING)
            {
                lbl_speed.Content = p.speed.ToString() ;
                lbl_lastweight.Content = p.last_pack_weight.ToString("F2") ;
                lbl_totalpack.Content = p.total_packs.ToString()  ;
            }
        }
        void  RunMode_Loaded(object sender, RoutedEventArgs e)
        {
        }
        private void group_action(string action)
        {
            App p = currentApp();
            if (p.curr_packer.status == PackerStatus.PAUSED || p.curr_packer.status == PackerStatus.RUNNING)
            {
                return;
            }
            p.curr_packer.agent.Action((Byte)(0x80+p.curr_packer._pack_id),action);
            //todo update result message
        }
        private void btn_empty_click(object sender, RoutedEventArgs e)
        {
            if ((Application.Current as App).curr_packer.status != PackerStatus.RUNNING)
                group_action("empty");
        }

        private void btn_zero_click(object sender, RoutedEventArgs e)
        {
            if ((Application.Current as App).curr_packer.status != PackerStatus.RUNNING)
                group_action("zero");
        }
        private void btn_start_click(object sender, RoutedEventArgs e)
        {
            btn_allstart.Click -= btn_start_click;   
            App p = currentApp();
            if (p.curr_packer.status == PackerStatus.RUNNING)
            {
                p.curr_packer.StopRun();
                this.btn_allstart.Content = StringResource.str("all_start");
                btn_allstart.Template = this.FindResource("StartBtn") as ControlTemplate;
            }else{
                App.bSimulate = false;
                
                p.curr_packer.StartRun();
                p.curr_packer.status = PackerStatus.RUNNING;
                
                this.btn_allstart.Content = StringResource.str("all_stop");
                btn_allstart.Template = this.FindResource("StartBtn2") as ControlTemplate;
            }
            btn_allstart.ApplyTemplate();
            btn_allstart.Click += btn_start_click;
        }
        private void btn_history_click(object sender, RoutedEventArgs e)
        {
            App p = currentApp();
            if (p.curr_packer.status == PackerStatus.RUNNING)
            {
                return;
            }

            p.SwitchTo("history");
        }

        //going to config menu
        private void btn_singlemode_click(object sender, RoutedEventArgs e)
        {
            App p = currentApp();
            if (p.curr_packer.status == PackerStatus.RUNNING)
            {
                return;
            }
            (Application.Current as App).kbdwnd.Init(StringResource.str("enter_singlemode_pwd"), "singlemode", true, KbdData);
            
        }
        private void grp_reg(string regname)
        {
            App p = currentApp();
            if (p.curr_packer.status == PackerStatus.RUNNING)
            {
                return;
            }
            p.kbdwnd.Init(StringResource.str("enter_"+regname), regname, false, KbdData);

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
        public void UpdateUI(string param)
        {
            App p = currentApp();
            UIPacker pack = p.curr_packer;
            
            //display the variable based on current setting
            if(param == "sys_config")
            {
                this.input_uvar.Content = pack.curr_cfg.upper_var.ToString() + StringResource.str("gram");

                this.input_dvar.Content = pack.curr_cfg.lower_var.ToString() + StringResource.str("gram");

                this.lbl_weight.Content = pack.curr_cfg.target.ToString() + StringResource.str("gram");

                this.prd_no.Content = pack.curr_cfg.product_no.ToString();

                this.operator_no.Content = PackerConfig.oper;

                this.prd_desc.Content = pack.curr_cfg.product_desc.ToString();
                Rectangle rect = this.FindName("ellipseWithImageBrush") as Rectangle;
                //load the corresponding picture.
                (rect.Fill as ImageBrush).ImageSource = new BitmapImage(new Uri("c:\\ioex\\prodpic\\" + pack.curr_cfg.product_desc.ToString() + ".jpg"));

            }
            if (param.IndexOf("wei_node") == 0)
            {
                Label lb = NameToControl(param) as Label;
                Button btn = NameToControl(param.Replace("wei_node", "bucket")) as Button;
                Button pbtn = NameToControl(param.Replace("wei_node", "pass")) as Button;
                byte n = (byte)(StringToId(param) );
                
                string ct = pack.agent.weight(n).ToString("F1");
                string err = pack.agent.GetErrors(n);
                if (pack.agent.weight(n) > -1000)
                {
                    lb.Content = ct;
                }

                if (err == "")
                {
                    pbtn.Template = this.FindResource("PassBar") as ControlTemplate;
                    pbtn.ToolTip = "";
                }
                else
                {
                    pbtn.Template = this.FindResource("PassBarError") as ControlTemplate;
                    pbtn.ToolTip = StringResource.str(err.Substring(0, err.IndexOf(';')));
                    lbl_status.Content = StringResource.str(err.Substring(0, err.IndexOf(';'))) + "\n";
                    if (AlertWnd.b_turnon_alert && AlertWnd.b_stop_onalert && pack.status == PackerStatus.RUNNING)
                        btn_start_click(null, null);
                }
                pbtn.ApplyTemplate();

                if (pack.agent.GetStatus(n) == NodeStatus.ST_LOST)
                {
                    btn.Template = this.FindResource("WeightBarError") as ControlTemplate;
                }
                else
                {
                    if (pack.status == PackerStatus.IDLE)
                    {
                        btn.Template = this.FindResource("WeightBar") as ControlTemplate;
                    }
                    else
                    {
                        if (n != pack.vib_addr)
                        {
                            byte[] lbs = pack.last_one_pack.bucket;
                            if (lbs != null)
                            {
                                bool hit = false;
                                foreach (byte b in lbs)
                                {
                                    if (n == b)
                                    {
                                        btn.Template = this.FindResource("WeightBarRelease") as ControlTemplate;
                                        hit = true;
                                        break;
                                    }
                                }
                                if (!hit)
                                {
                                    btn.Template = this.FindResource("WeightBar") as ControlTemplate;
                                }
                            }
                        }
                    }
                }
                btn.ApplyTemplate();
            }
        }
        private object NameToControl(string name)
        {
            return this.FindName(name);
        }
        private void grp_target_click(object sender, RoutedEventArgs e)
        {
            grp_reg("run_target");
        }
        private void grp_uvar_click(object sender, RoutedEventArgs e)
        {
            grp_reg("run_uvar");
        }
        private void grp_dvar_click(object sender, RoutedEventArgs e)
        {
            grp_reg("run_dvar");
        }
        private void weibucket_Click(object sender, RoutedEventArgs e)
        {
        }
        private void vibbucket_Click(object sender, RoutedEventArgs e)
        {
        }

        public void KbdData(string param, string data)
        {
            try
            {
                App p = Application.Current as App;
                UIPacker pack = p.curr_packer;
                if (param == "run_uvar")
                {
                    pack.curr_cfg.upper_var = double.Parse(data);
                    
                }
                if (param == "run_dvar")
                {
                    pack.curr_cfg.lower_var = double.Parse(data);
                    
                }
                if (param == "run_target")
                {
                    pack.curr_cfg.target = double.Parse(data);
                }
                if (param == "run_operator")
                {
                    PackerConfig.oper = data;
                    XElement cfg = p.curr_cfg;
                    cfg.SetElementValue("operator", data);
                    p.SaveAppConfig();
                }
                if (param.IndexOf("run") == 0)
                    pack.SaveCurrentConfig();
                if (param == "singlemode")
                {
                    if (data == Password.get_pwd("user"))
                    {
                        p.SwitchTo("configmenu");
                        return;
                    }
                    else
                        MessageBox.Show(StringResource.str("invalid_pwd"));
                }
                UpdateUI("sys_config");
            }
            catch (System.Exception e)
            {
                MessageBox.Show("Invalid Parameter");
                return;
            }
        }

        private void operator_no_Click(object sender, RoutedEventArgs e)
        {
            if ((Application.Current as App).curr_packer.status != PackerStatus.RUNNING)
                (Application.Current as App).kbdwnd.Init(StringResource.str("enter_operator_no"), "run_operator", false, KbdData);
        }

        private void prd_no_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            if ((Application.Current as App).curr_packer.status != PackerStatus.RUNNING)
                p.prodnum.Init(prd_no_selected,false);
        }
        private void prd_no_selected(string item)
        {
            App p = Application.Current as App;
            p.curr_packer.curr_cfg.product_no = item;
            p.curr_packer.SaveCurrentConfig();
            lastcall = "UpdatePrdNo";
            txt_oper.Content = StringResource.str("downloading");
            txt_oper.Visibility = Visibility.Visible;
            bg_oper.Visibility = Visibility.Visible;
            
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

        private void prd_no_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            App p = Application.Current as App;
            p.prodnum.Init(prd_no_selected,false);
        }
        private void UpdateAlertWindow(bool visible)
        {
            rect_alert1.Visibility = visible ? Visibility.Visible : Visibility.Hidden;
            rect_alert2.Visibility = visible ? Visibility.Visible : Visibility.Hidden;
            rect_alert3.Visibility = visible ? Visibility.Visible : Visibility.Hidden;
            lbl_alert.Visibility = visible ? Visibility.Visible : Visibility.Hidden;
            lbl_alert1.Visibility = visible ? Visibility.Visible : Visibility.Hidden;
            lbl_alert2.Visibility = visible ? Visibility.Visible : Visibility.Hidden;
            lbl_alert3.Visibility = visible ? Visibility.Visible : Visibility.Hidden;
            lbl_alert4.Visibility = visible ? Visibility.Visible : Visibility.Hidden;
            if (visible)
                lbl_alert.Content = "("+curr_node.ToString()+")"+StringResource.str("alert_select");
        }
        private void lbl_alert_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            Label l = sender as Label;
            if ((l is Label) && (curr_node != 0xff))
            {
                App p = Application.Current as App;
                byte id = curr_node;
                if (l.Name == "lbl_alert1") //alert solved
                {
                    p.agent.ClearErrors(id);
                }
                if (l.Name == "lbl_alert2") //alert force
                {
                    p.agent.Action(id,"empty");
                }
                if (l.Name == "lbl_alert3") //alert disable
                {
                    p.agent.SetStatus(id, NodeStatus.ST_LOST);
                }
                if (l.Name == "lbl_alert4") //alert quit
                {
                    
                }
                curr_node = 0xff;
                UpdateAlertWindow(false);
            }
        }

        private void passbar_MouseLeftButtonUp(object sender, RoutedEventArgs e)
        {
            
            Button l = sender as Button;
            if (l is Button)
            {
                App p = Application.Current as App;
                byte id = (byte)ButtonToId(sender);
                if (p.agent.GetErrors(id) != "")
                {
                    curr_node = id;
                    if (AlertWnd.b_turnon_alert && AlertWnd.b_show_alert)
                    {
                        UpdateAlertWindow(true);
                    }
                }
            }               
        }
    }
}
