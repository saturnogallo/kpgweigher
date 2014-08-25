using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Mndz
{
    public partial class MsgDlg : Form
    {
        public MsgDlg()
        {
            InitializeComponent();
            this.Location = new System.Drawing.Point(212, 183);
            this.BackColor = Color.Bisque;
            this.textBox1.BackColor = Color.Bisque;
            roundRect1.Style = MyButtonType.rectButton;
            roundRect1.colorShadow = Color.Transparent;
            roundRect1.colorTop = Color.Bisque;
            roundRect1.Refresh();
            this.textBox1.BorderStyle = BorderStyle.None;
            this.Load += new EventHandler(MsgDlg_Load);
            button1.Text = StringResource.str("ok");
        }

        void MsgDlg_Load(object sender, EventArgs e)
        {
            textBox1.Select(0, 0);
            this.Focus();
            roundRect1.Focus();
        }
        public void Init(string text)
        {
            Message = text;
            this.ShowDialog();
        }
        public string Message
        {
            set
            {
                textBox1.Text = value;
            }
        }
        private void button1_Click(object sender, EventArgs e)
        {
            Form1.DoBeep();
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}
