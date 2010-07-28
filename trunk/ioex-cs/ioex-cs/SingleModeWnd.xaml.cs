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

        private void btn_pack_param_Click(object sender, RoutedEventArgs e)
        {
            
        }
        private void weibucket_Click(object sender, RoutedEventArgs e)
        {
            
        }
        private void passbucket_Click(object sender, RoutedEventArgs e)
        {
            
        }

        private void vibbucket_Click(object sender, RoutedEventArgs e)
        {
        }

  


        private Button IdToButton(int nodeid)
        {
            /*
            if (nodeid == 0)
                return bucket1;
            if (nodeid == 1)
                return bucket2;
            if (nodeid == 2)
                return bucket3;
            if (nodeid == 3)
                return bucket4;
            if (nodeid == 4)
                return bucket5;
            if (nodeid == 5)
                return bucket6;
            if (nodeid == 6)
                return bucket7;
            if (nodeid == 7)
                return bucket8;
            if (nodeid == 8)
                return bucket9;
            if (nodeid == 9)
                return bucket10;
             */
            return null;
        }
        private void bucket_click(int nodeid)
        {
            App p = Application.Current as App;
            int last_node = p.packers[0].curr_node;
            p.packers[0].curr_node = nodeid-1;
            if (p.packers[0].curr_node == (nodeid-1))
            {
                Button lastbtn = IdToButton(last_node);
                Button currbtn = IdToButton(nodeid-1);
                lastbtn.BorderThickness = new Thickness(2);
                lastbtn.BorderBrush = Brushes.DarkGray;

                currbtn.BorderThickness = new Thickness(6);
                currbtn.BorderBrush = Brushes.Black;
            }
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
            //todo
        }

        private void input_GotFocus(object sender, RoutedEventArgs e)
        {
            node_reg("cs_filter");//gain, open_wei, wei_dtime,wei_otime,
            //sub_freq,sub_time,col_dtime,sub_amp,col_otime,motor_speed
        }

        private void btn_cali_Click(object sender, RoutedEventArgs e)
        {
            node_reg("cali0"); //cali1,2,3,4,
        }


        private void intf_lb_input_fmt_GotFocus(object sender, RoutedEventArgs e)
        {

        }

        private void intf_ckb_mem_Click(object sender, RoutedEventArgs e)
        {

        }

        private void intf_handshake_Click(object sender, RoutedEventArgs e)
        {

        }

        private void intf_lb_output_fmt_GotFocus(object sender, RoutedEventArgs e)
        {

        }

        private void intf_ckb_delay_GotFocus(object sender, RoutedEventArgs e)
        {

        }

        private void intf_lb_feed_times_GotFocus(object sender, RoutedEventArgs e)
        {

        }

        private void btn_action_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            SubNode n = p.nodes[p.packers[0].curr_node];
            string name = (sender as Button).Name;
            if (name == "btn_pass" || name == "btn_empty" || name == "btn_fill" || name == "btn_flash")
            {
                n.Action(name.Replace("btn_",""), true);
            }
            
        }

        private void btn_prd_desc_Click(object sender, RoutedEventArgs e)
        {

        }

        private void btn_trial_Click(object sender, RoutedEventArgs e)
        {

        }

        private void btn_save_Click(object sender, RoutedEventArgs e)
        {

        }

        private void btn_prd_no_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
