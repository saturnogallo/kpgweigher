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
namespace ioex_cs
{
    /// <summary>
    /// Interaction logic for RunMode.xaml
    /// </summary>

    public partial class RunMode : Window
    {
        
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
            uiTimer.Interval = 10;
            uiTimer.Start();
        }

        public bool bNeedInvalidate = false;
        void uiTimer_Tick(object sender, EventArgs e)
        {
            if (!this.IsVisible)
                return;
            if (! bNeedInvalidate)
            {
                return;
            }
            Packer p = (System.Windows.Application.Current as App).curr_packer;
            
            currentApp().bMainPause = true;
            foreach (WeighNode n in p.weight_node)
            {
                UpdateUI("wei_node" + n.node_id);
            }
            currentApp().bMainPause = false;
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
            p.curr_packer.ActionAll(action);
        }
        private void btn_empty_click(object sender, RoutedEventArgs e)
        {
            group_action("empty");
        }

        private void btn_zero_click(object sender, RoutedEventArgs e)
        {
            group_action("zero");
        }
        private void btn_start_click(object sender, RoutedEventArgs e)
        {
            App p = currentApp();
            if (p.curr_packer.status == PackerStatus.RUNNING)
            {
                p.curr_packer.StopRun();
                this.btn_allstart.Content = StringResource.str("all_start");

            }else{
                p.bSimulate = false;
                p.curr_packer.StartRun();
                p.curr_packer.status = PackerStatus.RUNNING;
                this.btn_allstart.Content = StringResource.str("all_stop");
            }
        }
        private void btn_history_click(object sender, RoutedEventArgs e)
        {
            App p = currentApp();
            if (p.curr_packer.status == PackerStatus.RUNNING)
            {
                return;
            }
            p.SwitchTo("histmode");
        }
        //going to config menu
        private void btn_singlemode_click(object sender, RoutedEventArgs e)
        {
            App p = currentApp();
            if (p.curr_packer.status == PackerStatus.RUNNING)
            {
                return;
            }
            p.SwitchTo("singlemode");
        }
        private void grp_reg(string regname)
        {
            App p = currentApp();
            if (p.curr_packer.status == PackerStatus.RUNNING)
            {
                MessageBox.Show("is_running");
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
            Packer pack = p.curr_packer;
            
            //diplay the variable based on current setting

            if (param == "run_uvar")
            {
                this.input_uvar.Content = pack.curr_cfg.upper_var.ToString();
            }
            if (param == "run_dvar")
            {
                this.input_dvar.Content = pack.curr_cfg.lower_var.ToString();
            }
            if (param == "run_target")
            {
                this.lbl_weight.Content = pack.curr_cfg.target.ToString();
            }
            if (param == "lbl_prd_no")
            {
                this.lbl_prd_no.Content = pack.curr_cfg.product_no.ToString();

            }
            if (param == "lbl_desc")
            {
                this.lbl_desc.Content = pack.curr_cfg.product_desc.ToString();
                Rectangle rect = this.FindName("ellipseWithImageBrush") as Rectangle;
                //load the corresponding pictiure.
                (rect.Fill as ImageBrush).ImageSource = new BitmapImage(new Uri("f:\\" + pack.curr_cfg.product_desc.ToString() + ".jpg"));

            }
            if (param.IndexOf("wei_node") == 0)
            {
                Label lb = NameToControl(param) as Label;
                Button btn = NameToControl(param.Replace("wei_node", "bucket")) as Button;
                WeighNode n = pack.weight_node[StringToId(param) - 1];
                //todo add fixed point position
                string ct = n.weight.ToString().Substring(0,n.weight.ToString().IndexOf('.')+2);
                lb.Content = ct;
                if (n.status == NodeStatus.ST_LOST)
                {
                    btn.Template = this.FindResource("WeightBarError") as ControlTemplate;
                }
                if (n is WeighNode)
                {
                    if ((n as WeighNode).bRelease)
                        btn.Template = this.FindResource("WeightBarRelease") as ControlTemplate;
                }
                if (n.status == NodeStatus.ST_IDLE)
                {
                    btn.Template = this.FindResource("WeightBar") as ControlTemplate;
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
                Packer pack = p.curr_packer;
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
                UpdateUI(param);
            }
            catch (System.Exception e)
            {
                MessageBox.Show("Invalid Parameter");
                return;
            }
        }
    }
}
