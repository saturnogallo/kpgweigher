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
        public void UpdateDisplay()
        {
            App p = (Application.Current as App);
            p.curr_packer.weight_node[address.SelectedIndex]["cs_filter"] = null;
            p.curr_packer.weight_node[address.SelectedIndex]["cs_gain_wordrate"] = null;
            cs_filter_input.Text = p.curr_packer.weight_node[address.SelectedIndex]["cs_filter"].ToString();
            gain_input.Text = p.curr_packer.weight_node[address.SelectedIndex]["cs_gain_wordrate"].ToString();

        }
        private void button1_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            p.SwitchTo("configmenu");
        }

        private void btn_modify_Click(object sender, RoutedEventArgs e)
        {
            App p = (Application.Current as App);
            p.curr_packer.weight_node[address.SelectedIndex]["cs_filter"] = UInt32.Parse(cs_filter_input.Text);
            p.curr_packer.weight_node[address.SelectedIndex]["cs_gain_wordrate"] = UInt32.Parse(gain_input.Text);
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
        public void KbdData(string param, string data)
        {
            if (param == "cs_filter")
            {
                this.cs_filter_input.Text = data;
            }
            if (param == "cs_gain_wordrate")
            {
                gain_input.Text = data;
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
