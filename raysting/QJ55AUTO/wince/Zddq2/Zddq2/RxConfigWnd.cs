using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Zddq2
{
    public partial class RxConfigWnd : Form
    {
        public RxConfigWnd()
        {
            InitializeComponent();
            btn_current.SetStyle(Color.White, MyButtonType.round2RectButton);
            btn_serial.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_stdchan.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_sqrt.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_vmode.SetStyle(Color.White, MyButtonType.round2RectButton);
            btn_last.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_next.SetStyle(Color.Beige, MyButtonType.round2RectButton);

            btn_vmode.Visible = false;
            lbl_vmode.Visible = false;

            lbl_range.Text = StringResource.str("rrange");
            btn_rangeP001.Text = "0.001";
            btn_rangeP01.Text = "0.01";
            btn_rangeP1.Text = "0.1";
            btn_range1.Text = "1";
            btn_range10.Text = "10";
            btn_range100.Text = "100";
            btn_range1k.Text = "1k";
            btn_range10k.Text = "10k";
            btn_range100k.Text = "100k";

            btn_rangeP001.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_rangeP01.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_rangeP1.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_range1.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_range10.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_range100.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_range1k.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_range10k.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_range100k.SetStyle(Color.Beige, MyButtonType.round2RectButton);

            btn_rangeP001.ValidClick += new EventHandler(btn_rangeP001_ValidClick);
            btn_rangeP01.ValidClick += new EventHandler(btn_rangeP01_ValidClick);
            btn_rangeP1.ValidClick += new EventHandler(btn_rangeP1_ValidClick);
            btn_range1.ValidClick += new EventHandler(btn_range1_ValidClick);
            btn_range10.ValidClick += new EventHandler(btn_range10_ValidClick);
            btn_range100.ValidClick += new EventHandler(btn_range100_ValidClick);
            btn_range1k.ValidClick += new EventHandler(btn_range1k_ValidClick);
            btn_range10k.ValidClick += new EventHandler(btn_range10k_ValidClick);
            btn_range100k.ValidClick += new EventHandler(btn_range100k_ValidClick);


//            btn_current.ValidClick += new EventHandler(input_GotFocus);
            btn_serial.ValidClick += new EventHandler(input_GotFocus);
            btn_stdchan.ValidClick += new EventHandler(input_GotFocus);
//            btn_vmode.ValidClick += new EventHandler(btn_vmode_ValidClick);
            btn_sqrt.ValidClick +=new EventHandler(btn_sqrt_ValidClick);

            btn_last.ValidClick += new EventHandler(btn_last_ValidClick);
            btn_next.ValidClick += new EventHandler(btn_next_ValidClick);

            lbl_current.Text = StringResource.str("current");
            lbl_serial.Text = StringResource.str("serial");
            lbl_stdchan.Text = StringResource.str("stdchan");
            lbl_vmode.Text = StringResource.str("vmode");

            btn_last.Text = "<";
            btn_next.Text = ">";

            //no channel select for Rx channel
            selectedRx = 0;
            btn_last.Visible = false;
            btn_next.Visible = false;
            btn_stdchan.Visible = false;
            lbl_stdchan.Visible = false;
            btn_enable.Visible = false;

            btn_chan.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_chan.ValidClick += new EventHandler(btn_dummy);

            btn_enable.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_enable.ValidClick +=new EventHandler(btn_enable_ValidClick);

            btn_quit.SetStyle(Color.Beige, MyButtonType.round2Button);
            btn_quit.Text = StringResource.str("quit");
            btn_quit.ValidClick += new EventHandler(btn_quit_ValidClick);

            btn_RsConfig.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_RsConfig.Text = StringResource.str("rsconfig");
            btn_RsConfig.ValidClick += new EventHandler(btn_RsConfig_ValidClick);

            btn_RxConfig.SetStyle(Color.LightBlue, MyButtonType.round2RectButton);
            btn_RxConfig.Text = StringResource.str("rxconfig");
            btn_RxConfig.ValidClick += new EventHandler(btn_RxConfig_ValidClick);

            btn_SysConfig.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_SysConfig.Text = StringResource.str("sysconfig");
            btn_SysConfig.ValidClick += new EventHandler(btn_SysConfig_ValidClick);
            
        }

        void btn_rangeP001_ValidClick(object sender, EventArgs e)
        {
            //5A, 3V mode
            Program.lst_rxinfo[selectedRx].iVMode = 0;
            Program.lst_rxinfo[selectedRx].iIx = 5; 
            Program.lst_rxinfo[selectedRx].iRRange = 0;
            InitDisplay(selectedRx);
        }
        void btn_rangeP01_ValidClick(object sender, EventArgs e)
        {
            //1A, 3V mode
            Program.lst_rxinfo[selectedRx].iVMode = 0;
            Program.lst_rxinfo[selectedRx].iIx = 4;
            Program.lst_rxinfo[selectedRx].iRRange = 1;

            InitDisplay(selectedRx);
        }
        void btn_rangeP1_ValidClick(object sender, EventArgs e)
        {
            //0.3A, 3V mode
            Program.lst_rxinfo[selectedRx].iVMode = 0;
            Program.lst_rxinfo[selectedRx].iIx = 3;
            Program.lst_rxinfo[selectedRx].iRRange = 2;
            InitDisplay(selectedRx);
        }
        void btn_range1_ValidClick(object sender, EventArgs e)
        {
            //0.1A, 10V mode
            Program.lst_rxinfo[selectedRx].iVMode = 1;
            Program.lst_rxinfo[selectedRx].iIx = 2;
            Program.lst_rxinfo[selectedRx].iRRange = 3;
            InitDisplay(selectedRx);
        }
        void btn_range10_ValidClick(object sender, EventArgs e)
        {
            //0.01A, 10V mode
            Program.lst_rxinfo[selectedRx].iVMode = 1;
            Program.lst_rxinfo[selectedRx].iIx = 1;
            Program.lst_rxinfo[selectedRx].iRRange = 4;
            InitDisplay(selectedRx);
        }
        void btn_range100_ValidClick(object sender, EventArgs e)
        {
            //0.01A, 10V mode
            Program.lst_rxinfo[selectedRx].iVMode = 1;
            Program.lst_rxinfo[selectedRx].iIx = 1;
            Program.lst_rxinfo[selectedRx].iRRange = 5;
            InitDisplay(selectedRx);
        }
        void btn_range1k_ValidClick(object sender, EventArgs e)
        {
            //0.001, 10V mode
            Program.lst_rxinfo[selectedRx].iVMode = 2;
            Program.lst_rxinfo[selectedRx].iIx = 0;
            Program.lst_rxinfo[selectedRx].iRRange = 6;

            InitDisplay(selectedRx);
        }
        void btn_range10k_ValidClick(object sender, EventArgs e)
        {
            //1mA fix, 10V high mode
            Program.lst_rxinfo[selectedRx].iVMode = 2;
            Program.lst_rxinfo[selectedRx].iIx = 0;
            Program.lst_rxinfo[selectedRx].iRRange = 7;
            InitDisplay(selectedRx);
        }
        void btn_range100k_ValidClick(object sender, EventArgs e)
        {
            //0.3mA fix, 30V high mode
            Program.lst_rxinfo[selectedRx].iVMode = 3;
            Program.lst_rxinfo[selectedRx].iIx = -1;
            Program.lst_rxinfo[selectedRx].iRRange = 8;
            InitDisplay(selectedRx);
        }
        void btn_dummy(object sender, EventArgs e)
        {
        }
        void btn_next_ValidClick(object sender, EventArgs e)
        {
            selectedRx += 1;
            if (selectedRx >= Program.lst_rxinfo.Count)
                selectedRx = 0;

            InitDisplay(selectedRx);
        }

        void btn_last_ValidClick(object sender, EventArgs e)
        {
            if (selectedRx <=0 )
                selectedRx =  Program.lst_rxinfo.Count;
            selectedRx -= 1;
            InitDisplay(selectedRx);
        }

        void btn_vmode_ValidClick(object sender, EventArgs e)
        {
        }



        void btn_RsConfig_ValidClick(object sender, EventArgs e)
        {
            Program.SwitchWindow("rsconfig");
        }

        void btn_RxConfig_ValidClick(object sender, EventArgs e)
        {
            //same page here
        }

        void btn_SysConfig_ValidClick(object sender, EventArgs e)
        {
            Program.SwitchWindow("sysconfig");
        }

        void btn_quit_ValidClick(object sender, EventArgs e)
        {
            Program.SwitchWindow("mainwnd");
            Program.mainwnd.Invoke(new Action<bool>(Program.mainwnd.ReDraw), new object[] { false });                
        }

        void btn_enable_ValidClick(object sender, EventArgs e)
        {
            RxInfo rx = Program.lst_rxinfo[selectedRx];
            rx.bEnabled = !rx.bEnabled;
            InitDisplay(selectedRx);
        }
        
        void btn_sqrt_ValidClick(object sender, EventArgs e)
        {
            RxInfo rx = Program.lst_rxinfo[selectedRx];
            rx.bSqrt = !rx.bSqrt;
            InitDisplay(selectedRx);
        }

        void input_GotFocus(object sender, EventArgs e)
        {
            string regname = (sender as RectButton).Name.Replace("btn_",""); //remove btn_
            Program.kbd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }

        public void KbdData(string param, string data)
        {
            try
            {
                if (param == "chan")
                {
                    selectedRx = Convert.ToInt32(data) - 1;
                }
                if (param == "stdchan")
                {
                    Program.lst_rxinfo[selectedRx].iStdChan = Convert.ToInt32(data);
                }
                if (param == "current")
                {
                    double ix = Convert.ToDouble(data);
                    Program.lst_rxinfo[selectedRx].iIx = -1; //0.1mA
                    if (ix > 0.0005)
                        Program.lst_rxinfo[selectedRx].iIx = 0; //1mA
                    if(ix > 0.005)
                        Program.lst_rxinfo[selectedRx].iIx = 1; //10mA
                    if(ix > 0.05)
                        Program.lst_rxinfo[selectedRx].iIx = 2; //0.1A
                    if (ix > 0.2)
                        Program.lst_rxinfo[selectedRx].iIx = 3; //0.3A
                    if (ix > 0.7)
                        Program.lst_rxinfo[selectedRx].iIx = 4; //1A
                    if (ix > 3)
                        Program.lst_rxinfo[selectedRx].iIx = 5; //5A
                }
                if (param == "serial")
                {
                    Program.lst_rxinfo[selectedRx].sSerial = data;
                }
                InitDisplay(selectedRx);
            }
            catch
            {
            }
        }
        private int selectedRx;
        public void InitDisplay(int iRx)
        {
            selectedRx = iRx;
            
            
            RxInfo rx = Program.lst_rxinfo[iRx];
            btn_chan.Text = "CH " + (iRx+1).ToString();
            if (rx.bEnabled)
            {
                if (rx.iIx == -1)
                    btn_current.Text = "0.1mA";
                if(rx.iIx == 0)
                    btn_current.Text = "1mA";
                if (rx.iIx == 1)
                    btn_current.Text = "10mA";
                if (rx.iIx == 2)
                    btn_current.Text = "0.1A";
                if (rx.iIx == 3)
                    btn_current.Text = "0.3A";
                if (rx.iIx == 4)
                    btn_current.Text = "1A";
                if (rx.iIx == 5)
                    btn_current.Text = "5A";

                btn_serial.Text = rx.sSerial;
                btn_stdchan.Text = rx.iStdChan.ToString();
                
                if (rx.bSqrt)
                {
                    btn_sqrt.Text = "x2";
                }
                else
                {
                    btn_sqrt.Text = "x1"; 
                }
                btn_enable.Text = StringResource.str("enabled");
            }
            else
            {
                btn_enable.Text = StringResource.str("disabled");
            }
            btn_enable.bOn = rx.bEnabled;
            //btn_current.bOn = rx.bEnabled && (rx.iVMode == 0);
            btn_serial.bOn = rx.bEnabled;
            btn_stdchan.bOn = rx.bEnabled;
            //btn_sqrt.bOn = rx.bEnabled && (rx.iVMode == 0);
//            btn_vmode.bOn = rx.bEnabled;

            btn_rangeP001.bOn = (rx.iRRange == 0);
            btn_rangeP01.bOn = (rx.iRRange == 1);
            btn_rangeP1.bOn = (rx.iRRange == 2);
            btn_range1.bOn = (rx.iRRange == 3);
            btn_range10.bOn = (rx.iRRange == 4);
            btn_range100.bOn = (rx.iRRange == 5);
            btn_range1k.bOn = (rx.iRRange == 6);
            btn_range10k.bOn = (rx.iRRange == 7);
            btn_range100k.bOn = (rx.iRRange == 8);

        }
    }
}