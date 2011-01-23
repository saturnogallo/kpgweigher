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
            Intf i = p.curr_packer.getInterface();
            intf_ckb_mem.IsChecked = i.b_Hasmem;
            intf_handshake.IsChecked = i.b_Handshake;
            intf_ckb_delay.Content = i.delay_length.ToString();
            intf_lb_feed_times.Content = i.feed_times.ToString();
            intf_input_trigger.SelectedIndex = i.fmt_input;
            intf_output_trigger.SelectedIndex = i.fmt_output;
            intf_pulse_width.Content = p.curr_packer.agent.GetNodeReg(p.curr_packer.vib_addr, "cs_filter").ToString();
        }
        private void ApplySetting()
        {
            App p = Application.Current as App;
            Intf i = new Intf(0);
            UInt16 delay = UInt16.Parse(intf_ckb_delay.Content.ToString());

            i.b_Hasdelay = (delay > 0);
            i.b_Hasmem = intf_ckb_mem.IsChecked.Value;
            i.b_Handshake = intf_handshake.IsChecked.Value;
            i.delay_length = delay;
            i.feed_times = UInt16.Parse(intf_lb_feed_times.Content.ToString());
            i.fmt_input = Convert.ToUInt16(intf_input_trigger.SelectedIndex);
            i.fmt_output = Convert.ToUInt16(intf_output_trigger.SelectedIndex);
            p.curr_packer.setInterface(i);
            p.curr_packer.agent.SetNodeReg(p.curr_packer.vib_addr, "cs_filter", Convert.ToUInt16(intf_pulse_width.Content));
        }
        private void btn_return_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;

            ApplySetting();
            
            p.curr_packer.agent.SetNodeReg(p.curr_packer.vib_addr, "cs_filter", Convert.ToUInt16(intf_pulse_width.Content));
            p.curr_packer.SaveCurrentConfig();
            p.SwitchTo("configmenu");
            
        }
        private void btn_ret_run_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            Intf i = new Intf(0);
            UInt16 delay = UInt16.Parse(intf_ckb_delay.Content.ToString());

            ApplySetting();
            
            
            p.curr_packer.SaveCurrentConfig();
            p.SwitchTo("runmode");

        }
        
        private void node_reg(string regname)
        {
            (Application.Current as App).kbdwnd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }

        private void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            
            if (sender is Label)
            {
                Label t = sender as Label;
                if (t.Name == "intf_lb_feed_times" || t.Name == "intf_ckb_delay" || t.Name == "intf_pulse_width")
                {
                    node_reg(t.Name);
                }
            }
        }
        public void KbdData(string param, string data)
        {
            if (param == "intf_ckb_delay")
            {
                intf_ckb_delay.Content = data;
            }
            if (param == "intf_lb_feed_times")
            {
                intf_lb_feed_times.Content = data;
            }
            if (param == "intf_pulse_width")
            {
                intf_pulse_width.Content = data;
            }
            ApplySetting();
        }

        private void btn_cancel_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            this.Hide();
            if((sender as Button).Name == "btn_ret")
                p.SwitchTo("configmenu");
            else
                p.SwitchTo("runmode");
            
        }

        private void btn_run_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            p.curr_packer.agent.Action(p.curr_packer.vib_addr, "trigger");
        }    
    }
}
