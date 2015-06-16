using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Runtime.InteropServices;
using System.Threading;
using System.IO;
using Raysting.Common;
using Raysting.Controls;
namespace Zddq2
{
    public partial class SysConfigWnd : Form
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct SystemTime
        {
            public ushort wYear;
            public ushort wMonth;
            public ushort wDayOfWeek;
            public ushort wDay;
            public ushort wHour;
            public ushort wMinute;
            public ushort wSecond;
            public ushort wMiliseconds;
        }
        [DllImport("coredll")]
        public static extern bool SetLocalTime(ref SystemTime sysTime); //设置本地时

        public SysConfigWnd()
        {
            InitializeComponent();
            lbl_sysconfig.Text = StringResource.str("sysconfig");
            btn_quit.Text = StringResource.str("quit");
            btn_quit.BackColor = this.BackColor;
            btn_quit.Style = MyButtonType.roundButton;
            btn_quit.ValidClick += new EventHandler((s, e) =>
            {
                Program.SwitchWindow("mainwnd");
                Program.mainwnd.Invoke(new Action<bool>(Program.mainwnd.ReDraw), new object[] { false });
            });
            RectButton[] kbdbtns = new RectButton[] { btn_ktt, btn_measdelay, btn_meastimes, btn_shifttimes, btn_newdate, btn_sampletimes, btn_fltlength };
            //keyboard input buttons
            foreach (RectButton rb in kbdbtns)
            {
                rb.Style = MyButtonType.rectButton;
                rb.colorTop = Color.Beige;
                rb.BackColor = this.BackColor;
                
                Label lbl = this.Controls.Find(rb.Name.Replace("btn_", "lbl_"),true)[0] as Label;
                lbl.Text = StringResource.str(lbl.Name.Replace("lbl_",""));

                rb.ValidClick += new EventHandler((s, e) =>
                {
                    string regname = (s as Control).Name.Remove(0, 4);
                    kbdWnd.Init(StringResource.str("input") + StringResource.str(regname), regname, false, KbdData);
                });
            }

            //choice buttons 
            RectButton[] choicebtns = new RectButton[] { btn_flttype, btn_navmeter, btn_rxscanner, btn_rsscanner };
            foreach (RectButton rb in choicebtns)
            {
                rb.Style = MyButtonType.rectButton;
                rb.colorTop = Color.Beige;
                rb.BackColor = this.BackColor;

                Label lbl = this.Controls.Find(rb.Name.Replace("btn_", "lbl_"),true)[0] as Label;
                lbl.Text = StringResource.str(lbl.Name.Replace("lbl_", ""));

                rb.ValidClick += new EventHandler((s,e) =>{
                    string regname = (s as Control).Name.Remove(0, 4);
                    ChoiceWnd.Init(StringResource.str("choose") + StringResource.str(regname),
                        regname,
                        StringResource.str("lst_" + regname).Split(new char[] { '|' }, StringSplitOptions.RemoveEmptyEntries),
                        0, null, KbdData);
                });
            }

            //toggle buttons 
            RectButton[] togglebtns = new RectButton[] { btn_throw };
            foreach (RectButton rb in togglebtns)
            {
                rb.Style = MyButtonType.rectButton;
                rb.colorTop = Color.Beige;
                rb.BackColor = this.BackColor;

                Label lbl = this.Controls.Find(rb.Name.Replace("btn_", "lbl_"), true)[0] as Label;
                lbl.Text = StringResource.str(lbl.Name.Replace("lbl_", ""));

                rb.ValidClick += new EventHandler((s, e) =>
                {
                    string regname = (s as Control).Name.Remove(0, 4);
                    KbdData(regname, "");
                });
            }

            InitDisplay();
        }
       
        private string newdate = "";


        public void KbdData(string param, string data)
        {
            try
            {
                if (param == "newtime")
                {
                    if (!Regex.IsMatch(newdate, "^\\d\\d\\d\\d\\d\\d\\d\\d$"))
                    {
                        MsgDlg.Show("日期格式不正确, 请按格式20120131输入");
                        return;
                    }

                    if (!Regex.IsMatch(data, "^\\d\\d\\d\\d\\d\\d$"))
                    {
                        MsgDlg.Show("时间格式不正确, 请按格式235959输入");
                        return;
                    }
                    SystemTime time = new SystemTime();
                    time.wYear = Convert.ToUInt16(newdate.Substring(0, 4));
                    time.wMonth = Convert.ToUInt16(newdate.Substring(4, 2));
                    time.wDay = Convert.ToUInt16(newdate.Substring(6, 2));
                    time.wHour = Convert.ToUInt16(data.Substring(0, 2));
                    time.wMinute = Convert.ToUInt16(data.Substring(2, 2));
                    time.wSecond = Convert.ToUInt16(data.Substring(4, 2));
                    time.wMiliseconds = 0;
                    SetLocalTime(ref time);
                }
                if (param == "newdate")
                {
                    newdate = data;
                    this.Invoke(new Action(() =>
                    {
                        kbdWnd.Init(StringResource.str("enter_newdate"), "newdata", false, KbdData);
                    }));
                    return;
                }
                if (param == "ktt")
                {
                    Program.sysinfo.iKTT = Convert.ToInt32(data);
                }
                if (param == "measdelay")
                {
                    Program.sysinfo.iMeasDelay = Convert.ToInt32(data);
                }
                if (param == "sampletimes")
                {
                    Program.sysinfo.iSampleTimes = Convert.ToInt32(data);
                }
                if (param == "meastimes")
                {
                    Program.sysinfo.iMeasTimes = Convert.ToInt32(data);
                }
                if (param == "shifttimes")
                {
                    Program.sysinfo.iShiftTimes = Convert.ToInt32(data);
                }
                if (param == "fltlength")
                {
                    Program.sysinfo.iFilter = Convert.ToInt32(data);
                }
                if (param == "flttype")
                {
                    Program.sysinfo.sFilterType = Util.FindStringValue(Int32.Parse(data), StringResource.str("lst_"+param));
                }
                if (param == "navmeter")
                {
                    Program.sysinfo.sNavmeter = Util.FindStringValue(Int32.Parse(data), StringResource.str("lst_" + param));
                }
                if (param == "rsscanner")
                {
                    Program.sysinfo.sRsscanner = Util.FindStringValue(Int32.Parse(data), StringResource.str("lst_" + param));
                }
                if (param == "rxscanner")
                {
                    Program.sysinfo.sRxscanner = Util.FindStringValue(Int32.Parse(data), StringResource.str("lst_" + param));
                }
                
                if (param == "throw")
                {

                    Program.sysinfo.bThrow = !Program.sysinfo.bThrow;
                }
                InitDisplay();
            }
            catch
            {
                MsgDlg.Show("数据格式不正确。");
            }
        }

        public void InitDisplay()
        {
            btn_ktt.Label = Program.sysinfo.iKTT.ToString();
            btn_measdelay.Label = Program.sysinfo.iMeasDelay.ToString();
            btn_meastimes.Label = Program.sysinfo.iMeasTimes.ToString();
            btn_shifttimes.Label = Program.sysinfo.iShiftTimes.ToString();
            btn_sampletimes.Label = Program.sysinfo.iSampleTimes.ToString();
            btn_fltlength.Label = Program.sysinfo.iFilter.ToString();

            btn_newdate.Label = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");

            btn_flttype.Label = Program.sysinfo.sFilterType;
            btn_navmeter.Label = Program.sysinfo.sNavmeter;
            btn_rxscanner.Label = Program.sysinfo.sRxscanner;
            btn_rsscanner.Label = Program.sysinfo.sRsscanner;
            
            if (Program.sysinfo.iKTT <= 0)
                btn_ktt.Label = StringResource.str("off");

            if (Program.sysinfo.bThrow)
                btn_throw.Label = StringResource.str("on");
            else
                btn_throw.Label = StringResource.str("off");
            this.Invalidate();
        }
    }
}