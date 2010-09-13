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
    /// Interaction logic for BottomWnd.xaml
    /// </summary>
    public partial class BottomWnd : Window
    {
        public BottomWnd()
        {
            InitializeComponent();
            
        }
        public void UpdateDisplay()
        {
            App p = Application.Current as App;
            Intf i = p.curr_packer.pack_node.getInterface();
            intf_ckb_mem.IsChecked = i.b_Hasmem;
            intf_handshake.IsChecked = i.b_Handshake;
            intf_ckb_delay.Text = i.delay_length.ToString();
            intf_lb_feed_times.Text = i.feed_times.ToString();
            intf_input_trigger.SelectedIndex = i.fmt_input;
            intf_output_trigger.SelectedIndex = i.fmt_output;
        }
        private void btn_return_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            Intf i = new Intf(0);
            UInt16 delay = UInt16.Parse(intf_ckb_delay.Text);

            i.b_Hasdelay = (delay > 0);
            i.b_Hasmem = intf_ckb_mem.IsChecked.Value;
            i.b_Handshake = intf_handshake.IsChecked.Value;
            i.delay_length = delay;
            i.feed_times = UInt16.Parse(intf_lb_feed_times.Text);
            i.fmt_input = Convert.ToUInt16(intf_input_trigger.SelectedIndex);
            i.fmt_output = Convert.ToUInt16(intf_output_trigger.SelectedIndex);
            
            p.curr_packer.pack_node.SetInterface(i);
            p.SwitchTo("configmenu");
            
        }
        private void node_reg(string regname)
        {
            (Application.Current as App).kbdwnd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }

        private void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            
            if (sender is TextBox)
            {
                TextBox t = sender as TextBox;
                if (t.Name == "intf_lb_feed_times")
                {
                    node_reg("intf_lb_feed_times");
                }
                if (t.Name == "intf_ckb_delay")
                {
                    node_reg("intf_ckb_delay");
                }
            }
        }
        public void KbdData(string param, string data)
        {
            if (param == "intf_ckb_delay")
            {
                intf_ckb_delay.Text = data;
            }
            if (param == "intf_lb_feed_times")
            {
                intf_lb_feed_times.Text = data;
            }
        }    
    }
}
