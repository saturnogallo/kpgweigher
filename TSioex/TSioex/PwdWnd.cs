using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace TSioex
{
    public partial class PwdWnd : Form
    {
        private string old_pwd;
        private string new_pwd1;
        private string new_pwd2;

        private string old_pwd_input_pwd
        {
            get
            {
                return old_pwd;
            }
            set
            {
                old_pwd = value;
                old_pwd_input.Text = "**********************************".Substring(0, old_pwd.Length);
            }
        }
        private string new_pwd_input1_pwd
        {
            get
            {
                return new_pwd1;
            }
            set
            {
                new_pwd1 = value;
                this.new_pwd_input1.Text = "**********************************".Substring(0, new_pwd1.Length);
            }
        }
        private string new_pwd_input2_pwd
        {
                        get
            {
                return new_pwd2;
            }
            set
            {
                new_pwd2 = value;
                new_pwd_input2.Text = "**********************************".Substring(0, new_pwd2.Length);
            }
        }

        public PwdWnd()
        {
            InitializeComponent();
            this.BackColor = SingleModeWnd.bgWindow;
            btn_modify.SetStyle(Color.Orange,  MyButtonType.roundRectButton);
            btn_restore.SetStyle(Color.Orange, MyButtonType.roundRectButton);
            UpdateDisplay();
            old_pwd_input.GotFocus += new EventHandler(pwd_GotFocus);
            new_pwd_input1.GotFocus += new EventHandler(pwd_GotFocus);
            new_pwd_input2.GotFocus += new EventHandler(pwd_GotFocus);
            btn_modify.ValidClick += new EventHandler(btn_modify_Click);
            btn_restore.ValidClick += new EventHandler(btn_restore_Click);
            btn_ret_run.Click += new EventHandler(btn_return_Click);
            btn_ret_config.Click += new EventHandler(btn_return_Click);
        }


        public void UpdateDisplay()
        {
            lbl_title.Text = StringResource.str("pwdpage");
            usr_label.Text = StringResource.str("account");
            usr_input.Items.Clear();
            usr_input.Items.Add(StringResource.str("admin"));
            usr_input.Items.Add(StringResource.str("user"));

            label1.Text = StringResource.str("org_password");
            label2.Text = StringResource.str("new_password");
            label3.Text = StringResource.str("new_password2");
            btn_modify.Text = StringResource.str("modify_password");
            btn_restore.Text = StringResource.str("find_password");

        }
        private void btn_return_Click(object sender, EventArgs e)
        {
            old_pwd_input_pwd = "";
            new_pwd_input1_pwd = "";
            new_pwd_input2_pwd = "";

            if ((sender as Control).Name == "btn_ret_run")
            {
                Program.SwitchTo("runmode");
                //Hide();
            }
            if ((sender as Control).Name == "btn_ret_config")
            {
                Program.SwitchTo("configmenu");
                //Hide();
            }

        }

        private void btn_modify_Click(object sender, EventArgs e)
        {

                string user = "";
                if( 0== usr_input.SelectedIndex)
                    user = "admin";

                if (1 == usr_input.SelectedIndex)
                    user = "user";
                if (Password.compare_pwd(user, this.old_pwd_input_pwd))
                {
                    if (new_pwd_input1_pwd == new_pwd_input2_pwd)
                    {
                        Password.set_pwd(user, new_pwd_input1_pwd);
                        MessageBox.Show(StringResource.str("pwd_done"));
                        
                        btn_return_Click(sender, e);
                    }
                    else
                    {
                        MessageBox.Show(StringResource.str("notsame_pwd"));
                    }
                }
                else
                {
                    MessageBox.Show(StringResource.str("invalid_pwd"));
                }
            
        }
        public void KbdData(string param, string data)
        {
            if (param == "old_pwd_input")
                old_pwd_input_pwd = data;
            if (param == "new_pwd_input1")
                new_pwd_input1_pwd = data;
            if (param == "new_pwd_input2")
                new_pwd_input2_pwd = data;
            UpdateDisplay();
            btn_modify.Focus();

        }
        private void btn_restore_Click(object sender, EventArgs e)
        {
            RunModeWnd.Reset();
            MessageBox.Show(StringResource.str("pwd_restore_done"));
            btn_return_Click(sender, e);
        }
        private void pwd_GotFocus(object sender, EventArgs e)
        {
            btn_modify.Focus();
            Program.kbdwnd.Init(StringResource.str("enter_singlemode_pwd"), (sender as Control).Name, true, KbdData);
        }


    }
}