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

        void uiTimer_Tick(object sender, EventArgs e)
        {
            if (!this.IsVisible)
                return;
            
            Packer p = (System.Windows.Application.Current as App).packers[0];

            //currentApp().bMainPause = true;
            foreach (WeighNode n in p.weight_node)
            {
                UpdateUI("wei_node" + n.node_id);
            }
            //currentApp().bMainPause = false;
        }
        void  RunMode_Loaded(object sender, RoutedEventArgs e)
        {
        }
        private void group_action(string action)
        {
            App p = currentApp();
            if (p.packers[0].status == PackerStatus.PAUSED || p.packers[0].status == PackerStatus.RUNNING)
            {
                return;
            }
            p.packers[0].ActionAll(action);
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
            if (p.packers[0].status == PackerStatus.RUNNING)
            {
                p.packers[0].status = PackerStatus.IDLE;
                this.btn_allstart.Content = StringResource.str("all_start");
            }else{
                p.bSimulate = false;
                p.packers[0].status = PackerStatus.RUNNING;
                this.btn_allstart.Content = StringResource.str("all_stop");
            }
        }
        private void btn_history_click(object sender, RoutedEventArgs e)
        {
            App p = currentApp();
            if (p.packers[0].status == PackerStatus.RUNNING)
            {
                return;
            }
            p.SwitchTo("histmode");
        }
        //going to config menu
        private void btn_singlemode_click(object sender, RoutedEventArgs e)
        {
            App p = currentApp();
            if (p.packers[0].status == PackerStatus.RUNNING)
            {
                return;
            }
            p.SwitchTo("singlemode");
        }
        private void grp_reg(string regname)
        {
            App p = currentApp();
            if (p.packers[0].status == PackerStatus.RUNNING)
            {
                MessageBox.Show("is_running");
                return;
            }
            p.kbdwnd.Init(StringResource.str("enter_"+regname), regname, false, KbdData);

        }
        public int StringToId(string name)
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
            Packer pack = p.packers[0];
            int id;
            //todo diplay the variable based on current setting
            if (int.TryParse(param,out id))
            {
                //todo update the display of each node
            }
            if (param == "run_uvar")
            {
//                this.grp_uvar_input.Text = pack.sys_cfg.upper_var.ToString();
            }
            if (param == "run_dvar")
            {
//                this.grp_dvar_input.Text = pack.sys_cfg.lower_var.ToString();
            }
            if (param == "run_target")
            {
//                this.grp_target_input.Text = pack.target.ToString();
            }
            if (param == "lbl_prd_no")
            {
//                this.grp_amp_input.Text = pack.vib_node[0]["magnet_amp"].Value.ToString();
                //todo load the corresponding pictiure and description.
            }
            if (param == "lbl_desc")
            {
//                this.grp_freq_input.Text = pack.vib_node[0]["magnet_freq"].Value.ToString();
            }
            if (param.IndexOf("wei_node") == 0)
            {
                Label lb = NameToControl(param) as Label;
                string ct = pack.weight_node[StringToId(param) - 1].weight.ToString();
                lb.Content = ct;
            }
        }
        private object NameToControl(string name)
        {
            if (name == "wei_node1")
                return wei_node1;
            if (name == "wei_node2")
                return wei_node2;
            if (name == "wei_node3")
                return wei_node3;
            if (name == "wei_node4")
                return wei_node4;
            if (name == "wei_node5")
                return wei_node5;
            if (name == "wei_node6")
                return wei_node6;
            if (name == "wei_node7")
                return wei_node7;
            if (name == "wei_node8")
                return wei_node8;
            if (name == "wei_node9")
                return wei_node9;
            if (name == "wei_node10")
                return wei_node10;
            return null;

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
        public void KbdData(string param, string data)
        {
            //todo update the input into the setting
            /*
            try
            {
                App p = Application.Current as App;
                Packer pack = p.packers[0];
                if (param == "run_uvar")
                {
                    pack.sys_cfg.upper_var = double.Parse(data);
                    
                }
                if (param == "run_dvar")
                {
                    pack.sys_cfg.lower_var = double.Parse(data);
                    
                }
                if (param == "run_target")
                {
                    pack.target = double.Parse(data);
                    
                }
                if (param == "run_amp")
                {
                    pack.vib_node[0]["magnet_amp"] = UInt32.Parse(data);
                    
                }
                if (param == "run_freq")
                {
                    pack.vib_node[0]["magnet_amp"] = UInt32.Parse(data);
                    
                }
                UpdateUI(param);
            }
            catch (System.Exception e)
            {
                MessageBox.Show("Invalid Parameter");
                return;
            }
          * */
        }
    }
}
