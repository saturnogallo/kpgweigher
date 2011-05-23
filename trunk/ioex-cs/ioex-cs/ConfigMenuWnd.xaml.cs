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
    /// Interaction logic for ConfigMenuWnd.xaml
    /// </summary>
    public partial class ConfigMenuWnd : Window
    {
        public ConfigMenuWnd()
        {
            InitializeComponent();
        }

        private void lbl_sub_Click(object sender, RoutedEventArgs e)
        {
            Button b = sender as Button;
            App p = Application.Current as App;
            Hide();
            if(b.Name == "btn_pwd")
            {

                p.SwitchTo("password");
                return;
            }
            if (b.Name == "btn_eng")
            {
                p.SwitchTo("engineer");
                return;
            }
            if (b.Name == "btn_product")
            {
                p.SwitchTo("product");
                return;
            }
            if (b.Name == "btn_bottom")
            {
                p.SwitchTo("bottom");
                return;
            }
            if (b.Name == "btn_history")
            {
                p.SwitchTo("history");
                return;
            }
            if (b.Name == "btn_alert")
            {
                p.SwitchTo("alert");
                return;
            }

        }

        private void btn_return_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            this.Hide();
            p.SwitchTo("runmode");
        }
    }
}
