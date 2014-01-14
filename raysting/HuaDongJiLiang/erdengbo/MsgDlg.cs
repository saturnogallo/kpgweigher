using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Jmbo
{
    public partial class MsgDlg : Form
    {
        public MsgDlg()
        {
            InitializeComponent();
        }
        public string message
        {
            set
            {
                textBox1.Text = value;
                btn_skip.Visible = (value.IndexOf("跳过") > 0);
            }
        }
        public string choice = "";
        private void button1_Click(object sender, EventArgs e)
        {
            choice = "ok";
            this.Close();
        }

        private void btn_skip_Click(object sender, EventArgs e)
        {
            choice = "skip";
            this.Close();
        }
    }
}
