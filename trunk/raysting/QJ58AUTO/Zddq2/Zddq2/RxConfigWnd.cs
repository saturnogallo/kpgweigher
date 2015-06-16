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
    public partial class RxConfigWnd : Form
    {
        public RxInfo Rx
        {
            get;
            set;
        }
        public RxConfigWnd()
        {
            InitializeComponent();


            btn_quit.Text = StringResource.str("quit");
            btn_quit.BackColor = this.BackColor;
            btn_quit.Style = MyButtonType.roundButton;
            btn_quit.ValidClick += new EventHandler((s, e) =>
            {
                if (!Rx.bEnabled)
                    Rx.bSelected = false;
                Program.SwitchWindow("mainconfig");
                //Program.mainwnd.Invoke(new Action<bool>(Program.mainwnd.ReDraw), new object[] { false });
            });

           
            RectButton[] kbdbtns = new RectButton[] { btn_measdelay, btn_meastimes, btn_rtp, btn_coefa, btn_coefb, btn_coefc, btn_serial };
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
            RectButton[] choicebtns = new RectButton[] { btn_prbtype, btn_current, btn_range, btn_serial_select };
            foreach (RectButton rb in choicebtns)
            {
                rb.Style = MyButtonType.rectButton;
                rb.BackColor = this.BackColor;

                Label lbl = this.Controls.Find(rb.Name.Replace("btn_", "lbl_"), true)[0] as Label;
                lbl.Text = StringResource.str(lbl.Name.Replace("lbl_", ""));

                rb.ValidClick += new EventHandler((s, e) =>
                {
                    string regname = (s as Control).Name.Remove(0, 4);
                    ChoiceWnd.Init(StringResource.str("choose") + StringResource.str(regname),
                        regname,
                        StringResource.str("lst_" + regname).Split(new char[] { '|' }, StringSplitOptions.RemoveEmptyEntries),
                        0, null, KbdData);
                });
            }

            //toggle buttons 
            RectButton[] togglebtns = new RectButton[] { btn_sqrt, btn_enable };
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

            InitDisplay();
        }

        public void KbdData(string param, string data)
        {
            try
            {
                if (param == "current")
                {
                    Rx.iIx = Convert.ToInt32(data);
                }
                if (param == "serial")
                {
                    Rx.sSerial = data;
                }
                if (param == "serial_select")
                {
                    //todo
                }
                if (param == "enable")
                {
                    Rx.bEnabled = !Rx.bEnabled;
                }
                if (param == "sqrt")
                {
                    Rx.bSqrt = !Rx.bSqrt;
                }
                if (param == "prbtype")
                {
                    Rx.sPTType = Util.FindStringValue(Int32.Parse(data), StringResource.str("lst_" + param));
                }
                if (param == "range")
                {
                    Rx.tRange = (TempRange)Convert.ToInt32(data);
                }
                if (param == "measdelay")
                {
                    Rx.iMeasDelay = Convert.ToInt32(data);
                }
                if (param == "meastimes")
                {
                    Rx.iMeasTimes = Convert.ToInt32(data);
                }
                if (param == "rtp")
                {
                    Rx.dRtp = Convert.ToDouble(data);
                }
                if (param == "ceofa")
                {
                    Rx.dCoefA = Convert.ToDouble(data);
                }
                if (param == "ceofb")
                {
                    Rx.dCoefB = Convert.ToDouble(data);
                }
                if (param == "ceofc")
                {
                    Rx.dCoefC = Convert.ToDouble(data);
                }
                InitDisplay();
            }
            catch
            {
            }
        }
        public void InitDisplay()
        {
            if (!(Rx is RxInfo))
                return;
            lbl_rx_pos.Text = StringResource.str("standard") + "@" + StringResource.str("channel") + Rx.iChan.ToString();
            btn_current.Label = Util.FindStringValue(Rx.iIx, StringResource.str("lst_current"));
            btn_serial.Label = Rx.sSerial;
            btn_prbtype.Label = Rx.sPTType;
            btn_measdelay.Label = Rx.iMeasDelay.ToString();
            btn_meastimes.Label = Rx.iMeasTimes.ToString();

            btn_rtp.Visible = btn_coefa.Visible = btn_coefb.Visible = btn_coefc.Visible = btn_range.Visible = Rx.bTempProbe;
            lbl_rtp.Visible = lbl_coefA.Visible = lbl_coefB.Visible = lbl_coefC.Visible = lbl_range.Visible = Rx.bTempProbe;

            btn_rtp.Label = Rx.dRtp.ToString();
            btn_coefa.Label = Rx.dCoefA.ToString();
            btn_coefb.Label = Rx.dCoefB.ToString();
            btn_coefc.Label = Rx.dCoefC.ToString();
            btn_range.Label = Util.FindStringValue((int)Rx.tRange, StringResource.str("lst_range"));

            if (Rx.bSqrt)
                btn_sqrt.Label = "x2";
            else
                btn_sqrt.Label = "x1";

            btn_enable.bOn = Rx.bEnabled;
            if(Rx.bEnabled)
                btn_enable.Label = Util.FindStringValue(1, StringResource.str("lst_enable"));
            else
                btn_enable.Label = Util.FindStringValue(0, StringResource.str("lst_enable"));

            this.Invalidate();
        }
    }
}
