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

        private void btn_return_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            p.SwitchTo("ConfigMenu");

        }
    }
}
