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
            old_pwd_input.Password = "";
            new_pwd_input1.Password = "";
            new_pwd_input2.Password = "";


            if ((sender as Button).Name == "btn_ret_run")
            {
                p.SwitchTo("runmode");
                Hide();
            }
        }

        private void btn_modify_Click(object sender, RoutedEventArgs e)
        {
            if (usr_input.SelectedItem is ListBoxItem)
            {
                string user = (this.usr_input.SelectedItem as ListBoxItem).Name;
                if (Password.compare_pwd(user,this.old_pwd_input.Password))
                {
                    if (new_pwd_input1.Password == new_pwd_input2.Password)
                    {
                        Password.set_pwd(user, new_pwd_input1.Password);
                        App p = Application.Current as App;
                        btn_return_Click(sender, e);
                    }else{
                        MessageBox.Show(StringResource.str("notsame_pwd"));
                    }
                }
                else
                {
                    MessageBox.Show(StringResource.str("invalid_pwd"));
                }
            }
        }
        public void KbdData(string param, string data)
        {
            if (param == "old_pwd_input")
                old_pwd_input.Password = data;
            if (param == "new_pwd_input1")
                new_pwd_input1.Password = data;
            if (param == "new_pwd_input2")
                new_pwd_input2.Password = data;

        }
        private void btn_restore_Click(object sender, RoutedEventArgs e)
        {
//              Password.set_pwd("admin", "020527");
                Password.set_pwd("user", "111111");
                btn_return_Click(sender, e);
        }

        private void pwd_GotFocus(object sender, MouseButtonEventArgs e)
        {
            (Application.Current as App).kbdwnd.Init(StringResource.str("enter_singlemode_pwd"), (sender as PasswordBox).Name, true, KbdData);
        }

        private void new_pwd_input2_GotFocus(object sender, RoutedEventArgs e)
        {
            (Application.Current as App).kbdwnd.Init(StringResource.str("enter_singlemode_pwd"), (sender as PasswordBox).Name, true, KbdData);
        }

    }
}
