using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;
using System.IO;
namespace CEUpgrade
{
    public partial class Form1 : Form
    {
        private string[] myargs;
        public Form1(string[] args)
        {
            myargs = args;
            InitializeComponent();
            this.Load += new EventHandler(Form1_Load);
        }

        void Form1_Load(object sender, EventArgs e)
        {
            

            string[] froms = null;
            string[] tos = null;
            for (int i = 0; i < myargs.Length; i++)
            {
                string _arg = myargs[i].Trim(new char[] { '"' });
                if (_arg.StartsWith("/from:"))
                {
                    froms = _arg.Remove(0, "/from:".Length).Split(new char[] { ';' });
                    continue;
                }
                if (_arg.StartsWith("/to:"))
                {
                    tos = _arg.Remove(0, "/to:".Length).Split(new char[] { ';' });
                    continue;
                }
            }
            bool ret = true;
            Thread.Sleep(2000);
            if (froms is string[])
            {
                for (int i = 0; i < froms.Length; i++)
                {
                    try
                    {
                        if(File.Exists(froms[i]))
                            File.Copy(froms[i], tos[i], true);
                    }
                    catch
                    {
                        ret = false;
                    }
                }
                if (ret)
                    label1.Text = "升级成功完成,请重启";
                else
                    label1.Text = "升级失败";
            }
            else
            {
                label1.Text = "升级未进行";
            }
            Application.DoEvents();
            Thread.Sleep(3000);
        }

    }
}