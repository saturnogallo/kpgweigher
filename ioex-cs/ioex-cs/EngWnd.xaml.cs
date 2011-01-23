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
    /// Interaction logic for EngConfigWnd.xaml
    /// </summary>
    public partial class EngConfigWnd : Window
    {
        public EngConfigWnd()
        {
            InitializeComponent();
            
        }
        int currnode;
        public void UpdateDisplay()
        {
            NodeAgent ag = (Application.Current as App).curr_packer.agent;
            currnode = address.SelectedIndex+1;
            cs_filter_input.Content = ag.GetNodeReg((byte)currnode, "cs_filter");
            gain_input.Content = ag.GetNodeReg((byte)currnode, "cs_gain_wordrate");
            test_mode_reg1_input.Content = ag.GetNodeReg((byte)currnode, "test_mode_reg1");
            test_mode_reg2_input.Content = ag.GetNodeReg((byte)currnode, "test_mode_reg2");
        }
        private void button1_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            p.SwitchTo("configmenu");
        }

        private void btn_modify_Click(object sender, RoutedEventArgs e)
        {
            NodeAgent ag = (Application.Current as App).curr_packer.agent;
            currnode = address.SelectedIndex + 1;
            ag.SetNodeReg((byte)currnode, "cs_filter", UInt32.Parse(cs_filter_input.Content.ToString()));
            ag.SetNodeReg((byte)currnode, "test_mode_reg1", UInt32.Parse(test_mode_reg1_input.Content.ToString()));
            ag.SetNodeReg((byte)currnode, "test_mode_reg2", UInt32.Parse(test_mode_reg2_input.Content.ToString()));
        }
        private void node_reg(string regname)
        {
            (Application.Current as App).kbdwnd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }
        private void gain_input_GotFocus(object sender, RoutedEventArgs e)
        {
            node_reg("cs_gain_wordrate");
        }

        private void cs_filter_input_GotFocus(object sender, RoutedEventArgs e)
        {
            node_reg("cs_filter");
        }
        private void testmodereg2_input_GotFocus(object sender, RoutedEventArgs e)
        {
            node_reg("test_mode_reg2");
        }

        private void testmodereg1_input_GotFocus(object sender, RoutedEventArgs e)
        {
            node_reg("test_mode_reg1");
        }
        public void KbdData(string param, string data)
        {
            if (param == "cs_filter")
            {
                this.cs_filter_input.Content = data;
            }
            if (param == "cs_gain_wordrate")
            {
                gain_input.Content = data;
            }
            if (param == "test_mode_reg1")
            {
                test_mode_reg1_input.Content = data;
            }
            if (param == "test_mode_reg2")
            {
                test_mode_reg2_input.Content = data;
            }

        }

        private void address_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            UpdateDisplay();
        }

        private void btn_quit_eng_click(object sender, RoutedEventArgs e)
        {
            this.Hide();
        }
    }
}
