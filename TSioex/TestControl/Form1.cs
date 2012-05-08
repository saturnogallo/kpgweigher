using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace TestControl
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            runPan1.Init(10);
            runPan1.handler = myclick;
            for (byte i = 1; i < 11; i++)
            {
                runPan1.SetText(i, i.ToString());
                runPan1.SetStatus("weightbar", i, PanStatus.IDLE);

            }
            runPan1.SetStatus("packer", 0, PanStatus.IDLE);
            rectButton1.Text = "roundRect";
            rectButton1.SetStyle(Color.Goldenrod, MyButtonType.roundRectButton);
            rectButton1.ValidClick += new EventHandler(rectButton1_ValidClick);
        }

        void rectButton1_ValidClick(object sender, EventArgs e)
        {
            MessageBox.Show("I am hitted");
        }
        public void myclick(int pos)
        {
            MessageBox.Show(pos.ToString());
        }
    }
}