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
        public SingleMode()
        {
            InitializeComponent();
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
            if (nodeid == 1)
                return bucket1;
            if (nodeid == 2)
                return bucket2;
            if (nodeid == 3)
                return bucket3;
            if (nodeid == 4)
                return bucket4;
            if (nodeid == 5)
                return bucket5;
            if (nodeid == 6)
                return bucket6;
            if (nodeid == 7)
                return bucket7;
            if (nodeid == 8)
                return bucket8;
            if (nodeid == 9)
                return bucket9;
            if (nodeid == 10)
                return bucket10;
            
            return null;
        }
        private void bucket_click(int nodeid)
        {
            App p = Application.Current as App;
            int last_node = p.packers[0].curr_node;
            if (nodeid < 1)
            {
                return;
            }
            
            Button lastbtn = IdToButton(last_node+1);
            Button currbtn = IdToButton(nodeid);
            lastbtn.BorderThickness = new Thickness(2);
            lastbtn.BorderBrush = Brushes.DarkGray;

            currbtn.BorderThickness = new Thickness(6);
            currbtn.BorderBrush = Brushes.Black;
            p.packers[0].curr_node = nodeid - 1;
            //todo update parameter of the current node to UI.
        }
        private void node_reg(string regname)
        {
            App p = Application.Current as App;
            if (p.packers[0].status == PackerStatus.RUNNING)
            {
                MessageBox.Show("is_running");
                return;
            }
            p.kbdwnd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }
        public void KbdData(string param, string data)
        {
            //todo update the display
        }

        private void input_GotFocus(object sender, RoutedEventArgs e)
        {
            //todo show the input box based on the sendor type
            node_reg("cs_filter");//gain, open_wei, wei_dtime,wei_otime,
            //sub_freq,sub_time,col_dtime,sub_amp,col_otime,motor_speed
        }

        private void btn_cali_Click(object sender, RoutedEventArgs e)
        {
            //todo do the cali based on the sendor name
            node_reg("cali0"); //cali1,2,3,4,
        }

        private void btn_action_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            SubNode n = p.nodes[p.packers[0].curr_node];
            string name = (sender as Button).Name;
            if (name == "btn_pass" || name == "btn_empty" || name == "btn_fill" || name == "btn_flash")
            {
                p.bMainPause = true;
                n.Action(name.Replace("btn_",""), true);
                p.bMainPause = false;
            }
        }

        private void btn_prd_desc_Click(object sender, RoutedEventArgs e)
        {
            (Application.Current as App).SwitchTo("ProductMode"); 
        }

        private void btn_trial_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            if (p.packers[0].status == PackerStatus.RUNNING)
            {
                p.packers[0].status = PackerStatus.IDLE;
                //this.btn_allstart.Content = StringResource.str("all_start");
            }
            else
            {
                p.bSimulate = true;
                p.packers[0].status = PackerStatus.RUNNING;
                //this.btn_allstart.Content = StringResource.str("all_stop");
            }
        }

        private void btn_save_Click(object sender, RoutedEventArgs e)
        {
            //save the setting to file
        }

        private void btn_prd_no_Click(object sender, RoutedEventArgs e)
        {
            //todo link to data input
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

        private void btn_sub_applyall_Click(object sender, RoutedEventArgs e)
        {
            //todo apply for other nodes.
        }

    }
}
