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
            
            if(b.Name == "btn_pwd")
            {

                p.SwitchTo("password");
            
            }
            if (b.Name == "btn_eng")
            {
                p.SwitchTo("engineer");
                //(Application.Current as App).kbdwnd.Init(StringResource.str("enter_singlemode_pwd"), "engmode", true, KbdData);
                //return;
            }
            if (b.Name == "btn_product")
            {
                p.SwitchTo("product");
            
            }
            if (b.Name == "btn_bottom")
            {
                p.SwitchTo("bottom");
            
            }
            if (b.Name == "btn_history")
            {
                p.SwitchTo("history");
            
            }
            if (b.Name == "btn_alert")
            {
                p.SwitchTo("alert");
            
            }
            Hide();
        }
        public void KbdData(string param, string data)
        {
            if (param == "engmode")
            {
                if (Password.compare_pwd("admin", data))
                {
                    App p = Application.Current as App;
                    p.SwitchTo("engineer");
                    Hide();
                    return;
                }
            }
        }
        private void btn_return_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            p.SwitchTo("runmode");
            this.Hide();
        }
    }
}
