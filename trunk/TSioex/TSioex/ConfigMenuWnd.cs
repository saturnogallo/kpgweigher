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
    public partial class ConfigMenuWnd : Form
    {
        public ConfigMenuWnd()
        {

            InitializeComponent();
            this.BackColor = SingleModeWnd.bgWindow;
            btn_alert.SetStyle(Color.LightGreen, MyButtonType.round2RectButton);
            btn_bottom.SetStyle(Color.LightGreen, MyButtonType.round2RectButton);
            btn_pwd.SetStyle(Color.LightGreen, MyButtonType.round2RectButton);
            btn_eng.SetStyle(Color.LightGreen, MyButtonType.round2RectButton);
            btn_history.SetStyle(Color.LightGreen, MyButtonType.round2RectButton);
            btn_product.SetStyle(Color.LightGreen, MyButtonType.round2RectButton);
            UpdateDisplay();

            btn_alert.ValidClick += new EventHandler(lbl_sub_Click);
            btn_bottom.ValidClick += new EventHandler(lbl_sub_Click);
            btn_pwd.ValidClick += new EventHandler(lbl_sub_Click);
            btn_eng.ValidClick += new EventHandler(lbl_sub_Click);
            btn_history.ValidClick += new EventHandler(lbl_sub_Click);
            btn_product.ValidClick += new EventHandler(lbl_sub_Click);
            btn_return.Click += new EventHandler(btn_return_Click);
        }
        public void UpdateDisplay()
        {
            btn_alert.Text = StringResource.str("alert_config");
            btn_bottom.Text = StringResource.str("bottom_config");
            btn_pwd.Text = StringResource.str("password_config");
            btn_eng.Text = StringResource.str("eng_config");
            btn_history.Text = StringResource.str("history");
            btn_product.Text = StringResource.str("product_config");
            lbl_title.Text = StringResource.str("configpage");
        }
        private void lbl_sub_Click(object sender, EventArgs e)
        {
            Control b = sender as Control;
            

            if (b.Name == "btn_pwd")
            {

                Program.SwitchTo("password");

            }
            if (b.Name == "btn_eng")
            {
                Program.kbdwnd.Init(StringResource.str("enter_singlemode_pwd"), "engmode", true, KbdData);
                return;
            }
            if (b.Name == "btn_product")
            {
                Program.SwitchTo("product");

            }
            if (b.Name == "btn_bottom")
            {
                Program.SwitchTo("bottom");

            }
            if (b.Name == "btn_history")
            {
                Program.SwitchTo("history");

            }
            if (b.Name == "btn_alert")
            {
                Program.SwitchTo("alert");

            }
            Hide();
        }
        public void KbdData(string param, string data)
        {
            if (param == "engmode")
            {
                if (Password.compare_pwd("admin", data))
                {
                    Program.SwitchTo("engineer");
                    Hide();
                    return;
                }
            }
        }
        private void btn_return_Click(object sender, EventArgs e)
        {
            Program.SwitchTo("runmode");
            //this.Hide();
        }
    }
}