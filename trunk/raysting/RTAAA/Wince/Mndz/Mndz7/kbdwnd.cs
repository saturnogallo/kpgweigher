using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Mndz7
{
    public delegate void KbdDataHandler(string param, string data);
    public partial class kbdWnd : Form
    {
        private StringBuilder data;

        public string note { get; set; }
        public string param { get; set; } //parameter for window in callback
        public bool password { get; set; }
        public KbdDataHandler kbdhandler;
        private Color btColor = Color.LightBlue;
        
        public kbdWnd()
        {
            InitializeComponent();
            
            RectButton[] btns = new RectButton[] {btn_backspace,btn_clr,btn_quit,btn_ok ,
                btn_num0,btn_num1,btn_num2,btn_num3,btn_num4,
                btn_num5,btn_num6,btn_num7,btn_num8,btn_num9,
                btn_numN,btn_numP
            };
            foreach (RectButton btn in btns)
            {
                btn.SetStyle(btColor,MyButtonType.roundRectButton);
                btn.BackColor = this.BackColor;
            }
            RectButton[] scalebtns = new RectButton[] { btn_kscale, btn_mscale, btn_gscale };
            foreach (RectButton btn in scalebtns)
            {
                btn.SetStyle(Color.Orange, MyButtonType.roundButton);
                btn.BackColor = this.BackColor;
            }

            btn_kscale.Text = "µ";
            btn_mscale.Text = "m";
            btn_gscale.Text = "G";

            btn_kscale.ValidClick += new EventHandler(btn_kscale_ValidClick);
            btn_mscale.ValidClick += new EventHandler(btn_kscale_ValidClick);
            btn_gscale.ValidClick += new EventHandler(btn_kscale_ValidClick);

            roundRect1.SetStyle(Color.Beige, MyButtonType.roundRect);

            btn_backspace.ValidClick +=new EventHandler(btn_backspace_Click);
            btn_clr.ValidClick += new EventHandler(btn_clr_Click);
            btn_quit.ValidClick += new EventHandler(btn_quit_Click);
            btn_ok.ValidClick += new EventHandler(btn_ok_Click);
            btn_num0.ValidClick += new EventHandler(btn_num0_Click);
            btn_num1.ValidClick += new EventHandler(btn_num1_Click);
            btn_num2.ValidClick += new EventHandler(btn_num2_Click);
            btn_num3.ValidClick += new EventHandler(btn_num3_Click);
            btn_num4.ValidClick += new EventHandler(btn_num4_Click);
            btn_num5.ValidClick += new EventHandler(btn_num5_Click);
            btn_num6.ValidClick += new EventHandler(btn_num6_Click);
            btn_num7.ValidClick += new EventHandler(btn_num7_Click);
            btn_num8.ValidClick += new EventHandler(btn_num8_Click);
            btn_num9.ValidClick += new EventHandler(btn_num9_Click);
            btn_numN.ValidClick += new EventHandler(btn_numN_Click);
            btn_numP.ValidClick += new EventHandler(btn_numP_Click);
            btn_backspace.Text = StringResource.str("backspace");
            btn_clr.Text = StringResource.str("clr");
            btn_quit.Text = StringResource.str("quit");
            btn_ok.Text = StringResource.str("ok");
            btn_num0.Text = "0";
            btn_num1.Text = "1";
            btn_num2.Text = "2";
            btn_num3.Text = "3";
            btn_num4.Text = "4";
            btn_num5.Text = "5";
            btn_num6.Text = "6";
            btn_num7.Text = "7";
            btn_num8.Text = "8";
            btn_num9.Text = "9";
            btn_numN.Text = "-";
            btn_numP.Text = ".";
            data = new StringBuilder("", 10);
            note = "Note";
            TopMost = true;
            password = false;
            Hide();
        }

        void btn_kscale_ValidClick(object sender, EventArgs e)
        {
            if (sender != btn_kscale)
                btn_kscale.bOn = false;
            else
                btn_kscale.bOn = !btn_kscale.bOn;

            if (sender != btn_mscale)
                btn_mscale.bOn = false;
            else
                btn_mscale.bOn = !btn_mscale.bOn;

            if (sender != btn_gscale)
                btn_gscale.bOn = false;
            else
                btn_gscale.bOn = !btn_gscale.bOn;
        }

        public void Init(string init_note, string init_param, bool init_pwd, KbdDataHandler handler)
        {
            data.Remove(0, data.Length);
            note = init_note;
            param = init_param;
            password = init_pwd;
            kbdhandler = handler;

            btn_kscale.bOn = false;
            btn_mscale.bOn = false;
            btn_gscale.bOn = false;
            btn_kscale.Visible = note.IndexOf("(ohm)") > 0;
            btn_mscale.Visible = note.IndexOf("(ohm)") > 0;
            //btn_gscale.Visible = note.IndexOf("(ohm)") > 0;
            //deep++;
            UpdateData();
            this.Show();
        }
        private void UpdateData()
        {
            if (!password)
            {
                datalabel.Text = data.ToString();
            }
            else
            {
                datalabel.Text = new String('*', data.Length);
            }

            notelabel.Text = note;
        }
        private void addchar(char c)
        {
            if (data.Length >= 15)
                return;
            if (c >= '0' && c <= '9')
            {
                data.Append(c);

            }
            if (c == '-')
            {
                if (data.Length == 0)
                {
                    data.Append(c);
                }

            }
            if (c == '.')
            {
                for (int i = 0; i < data.Length; i++)
                {
                    if (data[i] == '.')
                    {
                        return;
                    }
                }
                if (data.Length >= 1 && (data[data.Length - 1] != '-'))
                {
                    data.Append(c);
                }

            }
            UpdateData();
        }
        private void btn_num1_Click(object sender, EventArgs e)
        {
            addchar('1');
        }

        private void btn_num2_Click(object sender, EventArgs e)
        {
            addchar('2');
        }

        private void btn_num3_Click(object sender, EventArgs e)
        {
            addchar('3');
        }

        private void btn_num4_Click(object sender, EventArgs e)
        {
            addchar('4');
        }

        private void btn_num5_Click(object sender, EventArgs e)
        {
            addchar('5');
        }

        private void btn_num6_Click(object sender,EventArgs e)
        {
            addchar('6');
        }

        private void btn_num7_Click(object sender, EventArgs e)
        {
            addchar('7');
        }

        private void btn_num8_Click(object sender, EventArgs e)
        {
            addchar('8');
        }

        private void btn_num9_Click(object sender, EventArgs e)
        {
            addchar('9');
        }

        private void btn_num0_Click(object sender, EventArgs e)
        {
            addchar('0');
        }

        private void btn_numP_Click(object sender, EventArgs e)
        {
            addchar('.');
        }

        private void btn_numN_Click(object sender, EventArgs e)
        {
            addchar('-');
        }
        private static int deep = 0;
        public Decimal scale = 1;
        private void btn_ok_Click(object sender, EventArgs e)
        {
            string llimit = StringResource.str(param + "_llimit");
            string ulimit = StringResource.str(param + "_ulimit");
            scale = 1;
            if (btn_kscale.bOn)
                scale = 0.000001M;
            else if (btn_mscale.bOn)
                scale = 0.001M;
            else if (btn_gscale.bOn)
                scale = 1;
            if (data.ToString() == "")
            {
//              Program.MsgShow(StringResource.str("emptydata"));
                return;
            }
            try
            {
                if (llimit == "Invalid String Key") //no limits setting
                {
                    kbdhandler(param, data.ToString());
                }
                else
                {

                    if (llimit.IndexOf(".") < 0) //int case
                    {
                        Int32 ll = Int32.Parse(llimit);
                        Int32 hl = Int32.Parse(ulimit);

                        if (Int32.Parse(data.ToString()) < ll || Int32.Parse(data.ToString()) > hl)
                            throw new Exception(StringResource.str("out_of_range"));
                    }
                    else
                    {
                        double ll = double.Parse(llimit);
                        double hl = double.Parse(ulimit);
                        if (double.Parse(data.ToString()) < ll || double.Parse(data.ToString()) > hl)
                            throw new Exception(StringResource.str("out_of_range"));
                    }
                    kbdhandler(param, data.ToString());
                }
            }
            catch (Exception err)
            {
                Program.MsgShow(err.Message);
                return;
            }
            deep--;
            if (deep <= 0)
                Hide();
            else
                Invalidate();
        }

        private void btn_clr_Click(object sender, EventArgs e)
        {
            data.Remove(0, data.Length);
            UpdateData();
        }

        private void btn_backspace_Click(object sender, EventArgs e)
        {
            if (data.Length > 0)
            {
                data.Remove(data.Length - 1, 1);
                UpdateData();
            }
        }

        private void btn_quit_Click(object sender, EventArgs e)
        {
            Hide();
        }
    }
}
