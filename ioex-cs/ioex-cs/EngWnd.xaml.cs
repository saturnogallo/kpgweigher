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

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            p.SwitchTo("ConfigMenu");

        }

        private void button1_Click_1(object sender, RoutedEventArgs e)
        {

        }

        private void btn_modify_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
