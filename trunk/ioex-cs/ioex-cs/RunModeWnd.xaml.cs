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

namespace ioex_cs
{
    /// <summary>
    /// Interaction logic for RunMode.xaml
    /// </summary>

    public partial class RunMode : Window
    {
        
        public RunMode()
        {
            InitializeComponent();

            this.Loaded +=new RoutedEventHandler(RunMode_Loaded);

            //howto use             listView1.DataContext = mylist;
        }
        void  RunMode_Loaded(object sender, RoutedEventArgs e)
        {
        }
        
        private void btn_empty_click(object sender, RoutedEventArgs e)
        {
            //mylist.Add(new onepack(11, '1', '2', '3', '4', '5'));
        }
        private void btn_group_click(object sender, RoutedEventArgs e)
        {
            //todo add new group
        }
        private void btn_zero_click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            if (p.packers[0].status == PackerStatus.PAUSED || p.packers[0].status == PackerStatus.RUNNING)
            {
                return;
            }
            p.packers[0].ActionAll("zero");
        }
        private void btn_start_click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            if (p.packers[0].status == PackerStatus.RUNNING)
            {
                p.packers[0].status = PackerStatus.IDLE;
                this.btn_allstart.Content = StringResource.str("all_start");
            }else{
                p.packers[0].status = PackerStatus.RUNNING;
                this.btn_allstart.Content = StringResource.str("all_stop");
            }
            
        }
        private void btn_history_click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            if (p.packers[0].status == PackerStatus.RUNNING)
            {
                return;
            }
            p.SwitchTo("histmode");
        }
        private void btn_singlemode_click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            if (p.packers[0].status == PackerStatus.RUNNING)
            {
                return;
            }
            p.SwitchTo("singlemode");
        }
        private void grp_reg(string regname)
        {
            App p = Application.Current as App;
            if (p.packers[0].status == PackerStatus.RUNNING)
            {
                MessageBox.Show("is_running");
                return;
            }
            
            p.kbdwnd.Init(StringResource.str("enter_"+regname), regname, false, KbdData);

        }
        private void UpdateUI(string param)
        {
            App p = Application.Current as App;
            Packer pack = p.packers[0];
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
            if (param == "run_amp")
            {
//                this.grp_amp_input.Text = pack.vib_node[0]["magnet_amp"].Value.ToString();
            }
            if (param == "run_freq")
            {
//                this.grp_freq_input.Text = pack.vib_node[0]["magnet_freq"].Value.ToString();
            }
        }
        private void grp_amp_click(object sender, RoutedEventArgs e)
        {
            grp_reg("run_amp");
        }
        private void grp_freq_click(object sender, RoutedEventArgs e)
        {
            grp_reg("run_freq");
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
        {/*
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
