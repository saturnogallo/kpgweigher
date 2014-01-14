using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Jmbo
{
    public partial class Input : Form
    {
        public Input()
        {
            InitializeComponent();
            this.Load += new EventHandler(Input_Load);
            this.button1.GotFocus += new EventHandler(button1_GotFocus);
//            textBox1.KeyUp += new KeyEventHandler(textBox1_KeyUp);
        }

        void button1_GotFocus(object sender, EventArgs e)
        {
            textBox1.Focus();
        }

        void Input_Load(object sender, EventArgs e)
        {
            textBox1.Focus();
        }

        void textBox1_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                this.DialogResult = DialogResult.OK;
                textBox1.Focus();
                this.Close();
            }
        }
        public string Message
        {
            get
            {
                return lbl_msg.Text;
            }
            set
            {
                lbl_msg.Text = value;
                textBox1.Text = "";
                textBox1.Focus();
            }
        }
        public string result
        {
            get
            {
                return textBox1.Text;
            }
        }

        private void btn_cancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            textBox1.Focus();
            this.Close();
        }
    }
}
