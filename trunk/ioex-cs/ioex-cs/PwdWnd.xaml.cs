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
    /// Interaction logic for PwdWnd.xaml
    /// </summary>
    public partial class PwdWnd : Window
    {
        public PwdWnd()
        {
            InitializeComponent();
        }

        private void btn_return_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            p.SwitchTo("ConfigMenu");
        }

        private void btn_modify_Click(object sender, RoutedEventArgs e)
        {
            if (Password.get_pwd(this.usr_input.Text) == this.old_pwd_input.Password)
            {
                if (new_pwd_input1 == new_pwd_input2)
                {
                    Password.set_pwd(this.usr_input.Text, new_pwd_input1.Password);
                    App p = Application.Current as App;
                    btn_return_Click(sender,e);
                }
            }
        }

        private void btn_restore_Click(object sender, RoutedEventArgs e)
        {
            Password.set_pwd(this.usr_input.Text, "123456");
            App p = Application.Current as App;
            btn_return_Click(sender, e);
        }
    }
}
