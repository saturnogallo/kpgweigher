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
            lbl_fltlength.Text = StringResource.str("fltlength");
            lbl_measdelay.Text = StringResource.str("measdelay");
            lbl_sampletimes.Text = StringResource.str("sampletimes");
            lbl_flttype.Text = StringResource.str("filtertype");
            lbl_meastimes.Text = StringResource.str("meastimes");
            lbl_throw.Text = StringResource.str("autothrow");
            lbl_date.Text = StringResource.str("date");
            lbl_ktt.Text = StringResource.str("ktt");
            lbl_navmeter.Text = StringResource.str("navmeter");
            lbl_shifttimes.Text = StringResource.str("shifttimes");

            btn_quit.SetStyle(Color.Beige, MyButtonType.round2Button);
            btn_quit.Text = StringResource.str("quit");
            btn_quit.ValidClick += new EventHandler((s, e) =>
            {
                Program.SwitchWindow("mainwnd");
                Program.mainwnd.Invoke(new Action<bool>(Program.mainwnd.ReDraw), new object[] { false }); 
            });

            btn_flttype.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_flttype.ValidClick += new EventHandler(btn_flttype_ValidClick);

            btn_ktt.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_ktt.ValidClick += new EventHandler((s, e) => { Program.kbd.Init(StringResource.str("enter_ktt"), "ktt", false, KbdData); });

            btn_throw.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_throw.ValidClick += new EventHandler(btn_throw_ValidClick);


            btn_navmeter.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_navmeter.ValidClick += new EventHandler(btn_navmeter_ValidClick);

            btn_measdelay.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_measdelay.ValidClick += new EventHandler(input_GotFocus);

            btn_meastimes.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_meastimes.ValidClick += new EventHandler(input_GotFocus);

            btn_shifttimes.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_shifttimes.ValidClick += new EventHandler(input_GotFocus);

            btn_date.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_date.ValidClick += new EventHandler((s,e) =>{Program.kbd.Init(StringResource.str("enter_newdate"), "newdate", false, KbdData);});

            btn_sampletimes.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_sampletimes.ValidClick += new EventHandler(input_GotFocus);

            btn_fltlength.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_fltlength.ValidClick += new EventHandler(input_GotFocus);

            btn_RsConfig.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_RsConfig.Text = StringResource.str("rsconfig");
            btn_RsConfig.ValidClick += new EventHandler((s, e) => { Program.SwitchWindow("rsconfig"); });

            btn_RxConfig.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_RxConfig.Text = StringResource.str("rxconfig");
            btn_RxConfig.ValidClick += new EventHandler((s, e) => { Program.SwitchWindow("rxconfig"); });

            btn_SysConfig.SetStyle(Color.LightBlue, MyButtonType.round2RectButton);
            btn_SysConfig.Text = StringResource.str("sysconfig");
            btn_SysConfig.ValidClick += new EventHandler( (s,e) => {});

            btn_export.SetStyle(Color.LightSeaGreen, MyButtonType.round2RectButton);
            btn_export.Text = StringResource.str("export");
            btn_export.ValidClick += new EventHandler(btn_export_ValidClick);
            InitDisplay();
        }

        void btn_navmeter_ValidClick(object sender, EventArgs e)
        {
            if (RunWnd.syscfg.sNavmeter == "PZ158")
                RunWnd.syscfg.sNavmeter = "PZ2182";
            else if (RunWnd.syscfg.sNavmeter == "PZ2182")
                RunWnd.syscfg.sNavmeter = "2182";
            else
                RunWnd.syscfg.sNavmeter = "PZ158";
        }

        void btn_throw_ValidClick(object sender, EventArgs e)
        {
            RunWnd.syscfg.bThrow = !RunWnd.syscfg.bThrow;
            InitDisplay();
        }

        void btn_export_ValidClick(object sender, EventArgs e)
        {
            if (!Directory.Exists("\\Hard Disk"))
            {
                MessageBox.Show(StringResource.str("noharddisk"));
                return;
            }
            try
            {
                string basedir = StringResource.baseDir;
                foreach (string fname in Directory.GetFiles(basedir, "20*.txt"))
                {
                    FileInfo fi = new FileInfo(fname);
                    File.Copy(basedir + fi.Name, "\\Hard Disk\\" + fi.Name);
                }
                MessageBox.Show(StringResource.str("export_done"));
            }
            catch
            {
                MessageBox.Show(StringResource.str("export_fail"));
                return;
            }
        }

        public void entercurrent(string type)
        {
            Program.kbd.Init(StringResource.str("enter_"+type), type, false, KbdData);
        }
        public void EnterNewTime()
        {
            Program.kbd.Init(StringResource.str("enter_newtime"), "newtime", false, KbdData);
        }
        private string newdate = "";


        public void KbdData(string param, string data)
        {
            try
            {
                if (param == "1kcurr")
                {
                    RunWnd.syscfg.d1Kcurr = Convert.ToDouble(data);
                    return;
                }
                if (param == "10kcurr")
                {
                    RunWnd.syscfg.d10Kcurr = Convert.ToDouble(data);
                    return;
                }
                if (param == "100kcurr")
                {
                    RunWnd.syscfg.d100Kcurr = Convert.ToDouble(data);
                    return;
                }

                if (param == "newtime")
                {
                    if (!Regex.IsMatch(newdate, "^\\d\\d\\d\\d\\d\\d\\d\\d$"))
                        return;

                    if (!Regex.IsMatch(data, "^\\d\\d\\d\\d\\d\\d$"))
                        return;
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
                    if (data == "1000")
                    {
                        this.Invoke(new Action<string>(entercurrent), new string[] { "1kcurr" });
                        return;
                    }
                    if (data == "10000")
                    {
                        this.Invoke(new Action<string>(entercurrent), new string[] { "10kcurr" });
                        return;
                    }
                    if (data == "100000")
                    {
                        this.Invoke(new Action<string>(entercurrent), new string[] { "100kcurr" });
                        return;
                    }

                    newdate = data;
                    this.Invoke(new Action(EnterNewTime));
                    return;
                }
                if (param == "ktt")
                {
                    RunWnd.syscfg.iKTT = Convert.ToInt32(data);
                }
                if (param == "measdelay")
                {
                    RunWnd.syscfg.iMeasDelay = Convert.ToInt32(data);
                }
                if (param == "sampletimes")
                {
                    RunWnd.syscfg.iSampleTimes = Convert.ToInt32(data);
                }
                if (param == "meastimes")
                {
                    RunWnd.syscfg.iMeasTimes = Convert.ToInt32(data);
                }
                if (param == "shifttimes")
                {
                    RunWnd.syscfg.iShiftTimes = Convert.ToInt32(data);
                }

                if (param == "fltlength")
                {
                    RunWnd.syscfg.iFilter = Convert.ToInt32(data);
                }
                InitDisplay();
            }
            catch
            {
            }
        }

        void input_GotFocus(object sender, EventArgs e)
        {
            string regname = (sender as RectButton).Name.Remove(0, 4);
            Program.kbd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }

        

 

        void btn_flttype_ValidClick(object sender, EventArgs e)
        {
            if (RunWnd.syscfg.sFilterType == "filtertype1")
            {
                RunWnd.syscfg.sFilterType = "filtertype2";
            }
            else
            {
                if (RunWnd.syscfg.sFilterType == "filtertype2")
                {
                    RunWnd.syscfg.sFilterType = "filtertype3";
                }
                else
                {
                    RunWnd.syscfg.sFilterType = "filtertype1";
                }
                
            }
            InitDisplay();
        }

         public void InitDisplay()
        {
            btn_flttype.Text = StringResource.str(RunWnd.syscfg.sFilterType);
            btn_fltlength.Text = RunWnd.syscfg.iFilter.ToString();
            btn_measdelay.Text = RunWnd.syscfg.iMeasDelay.ToString();
            btn_sampletimes.Text = RunWnd.syscfg.iSampleTimes.ToString();
            btn_meastimes.Text = RunWnd.syscfg.iMeasTimes.ToString();
            btn_date.Text = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
            btn_ktt.Text = RunWnd.syscfg.iKTT.ToString();
            btn_navmeter.Text = RunWnd.syscfg.sNavmeter;
            if (RunWnd.syscfg.iKTT <= 0)
                btn_ktt.Text = StringResource.str("off");
            if (RunWnd.syscfg.bThrow)
                btn_throw.Text = StringResource.str("on");
            else
                btn_throw.Text = StringResource.str("off");
        }
    }
}