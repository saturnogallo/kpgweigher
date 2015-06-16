using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Raysting.Common;
using Raysting.Controls;
namespace Zddq2
{
    public partial class RsConfigWnd : Form
    {
        public RsInfo Rs;
        public RsConfigWnd()
        {
            InitializeComponent();


            btn_quit.Text = StringResource.str("quit");
            btn_quit.BackColor = this.BackColor;
            btn_quit.Style = MyButtonType.roundButton;

            btn_quit.ValidClick += new EventHandler((s, e) =>
            {
                if (!Rs.bEnabled)
                    Rs.bSelected = false;
                Program.SwitchWindow("mainconfig");
                //Program.mainwnd.Invoke(new Action<bool>(Program.mainwnd.ReDraw), new object[] { false });
            });
            RectButton[] kbdbtns = new RectButton[] { btn_serial, btn_rvalue, btn_ralpha, btn_rbeta, btn_maxcurr, btn_coefa, btn_coefb, btn_coefc, btn_rtp };
            //keyboard input buttons
            foreach (RectButton rb in kbdbtns)
            {
                rb.Style = MyButtonType.rectButton;
                rb.BackColor = this.BackColor;

                Label lbl = this.Controls.Find(rb.Name.Replace("btn_", "lbl_"), true)[0] as Label;
                lbl.Text = StringResource.str(lbl.Name.Replace("lbl_", ""));

                rb.ValidClick += new EventHandler((s, e) =>
                {
                    string regname = (s as Control).Name.Remove(0, 4);
                    kbdWnd.Init(StringResource.str("input") + StringResource.str(regname), regname, false, KbdData);
                });
            }

            //choice buttons 
            RectButton[] choicebtns = new RectButton[] { btn_prbtype, btn_range, btn_serial_select };
            foreach (RectButton rb in choicebtns)
            {
                rb.Style = MyButtonType.rectButton;
                rb.BackColor = this.BackColor;


                Label lbl = this.Controls.Find(rb.Name.Replace("btn_", "lbl_"), true)[0] as Label;
                lbl.Text = StringResource.str(lbl.Name.Replace("lbl_", ""));

                rb.ValidClick += new EventHandler((s, e) =>
                {
                    string regname = (s as Control).Name.Remove(0, 4);
                    ChoiceWnd.Init(StringResource.str("choose")  + StringResource.str(regname),
                        regname,
                        StringResource.str("lst_" + regname).Split(new char[] { '|' }, StringSplitOptions.RemoveEmptyEntries),
                        0, null, KbdData);
                });
            }

            //toggle buttons 
            RectButton[] togglebtns = new RectButton[] { btn_enable };
            foreach (RectButton rb in togglebtns)
            {
                rb.Style = MyButtonType.rectButton;
                rb.BackColor = this.BackColor;


                //Label lbl = this.Controls.Find(rb.Name.Replace("btn_", "lbl_"), true)[0] as Label;
                //lbl.Text = StringResource.str(lbl.Name.Replace("lbl_", ""));

                rb.ValidClick += new EventHandler((s, e) =>
                {
                    string regname = (s as Control).Name.Remove(0, 4);
                    KbdData(regname, "");
                });
            }
        }
        void btn_dummy(object sender, EventArgs e)
        {
        }

        public void KbdData(string param, string data)
        {
            try
            {
                if (param == "serial")
                {
                    Rs.sSerial = data;
                }
                if (param == "rvalue")
                {
                    Rs.dValue = Convert.ToDouble(data) ;
                }
                if (param == "ralpha")
                {
                    Rs.dAlpha = Convert.ToDouble(data);
                }
                if (param == "rbeta")
                {
                    Rs.dBeta = Convert.ToDouble(data);
                }
                if (param == "maxcurr")
                {
                    Rs.dMaxCurr = Convert.ToDouble(data);
                }
                if (param == "enable")
                {
                    Rs.bEnabled = !Rs.bEnabled;
                }
                if (param == "rtp")
                {
                    Rs.dRtp = Convert.ToDouble(data);
                }
                if (param == "ceofa")
                {
                    Rs.dCoefA = Convert.ToDouble(data);
                }
                if (param == "ceofb")
                {
                    Rs.dCoefB = Convert.ToDouble(data);
                }
                if (param == "ceofc")
                {
                    Rs.dCoefC = Convert.ToDouble(data);
                }
                if (param == "prbtype")
                {
                    Rs.sPTType = Util.FindStringValue(Int32.Parse(data), StringResource.str("lst_" + param));
                }
                if (param == "range")
                {
                    Rs.tRange = (TempRange)Convert.ToInt32(data);
                }
                InitDisplay();
            }
            catch
            {
            }
        }
        public void InitDisplay()
        {
            if (!(Rs is RsInfo))
                return;
            lbl_rs_pos.Text = StringResource.str("standard") + "@" + StringResource.str("channel")+Rs.iChan.ToString();
            btn_rvalue.Label = Util.FormatData(Rs.dValue, 7);
            btn_ralpha.Label = Rs.dAlpha.ToString("F2");
            btn_rbeta.Label = Rs.dBeta.ToString("F2");
            btn_maxcurr.Label = Rs.dMaxCurr.ToString("F2");
            btn_serial.Label = Rs.sSerial;

            btn_prbtype.Label = Rs.sPTType;

            btn_rtp.Visible = btn_coefa.Visible = btn_coefb.Visible = btn_coefc.Visible = btn_range.Visible = Rs.bTempProbe;
            lbl_rtp.Visible = lbl_coefA.Visible = lbl_coefB.Visible = lbl_coefC.Visible = lbl_range.Visible = Rs.bTempProbe;

            btn_rtp.Label = Rs.dRtp.ToString();
            btn_coefa.Label = Rs.dCoefA.ToString();
            btn_coefb.Label = Rs.dCoefB.ToString();
            btn_coefc.Label = Rs.dCoefC.ToString();
            btn_range.Label = Util.FindStringValue((int)Rs.tRange, StringResource.str("lst_range"));

            btn_enable.bOn = Rs.bEnabled;
            if (Rs.bEnabled)
                btn_enable.Label = Util.FindStringValue(1, StringResource.str("lst_enable"));
            else
                btn_enable.Label = Util.FindStringValue(0, StringResource.str("lst_enable"));

            this.Invalidate();
        }
    }
}
