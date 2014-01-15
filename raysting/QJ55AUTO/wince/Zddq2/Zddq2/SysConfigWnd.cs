using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;
using System.IO;
using System.Diagnostics;
using System.Reflection;
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
            lbl_display.Text = StringResource.str("displaymode");
            lbl_flttype.Text = StringResource.str("filtertype");
            lbl_meastimes.Text = StringResource.str("meastimes");
            lbl_throw.Text = StringResource.str("autothrow");
            lbl_date.Text = StringResource.str("date");
            lbl_ktt.Text = StringResource.str("ktt");
            lbl_navmeter.Text = StringResource.str("navmeter");

            btn_quit.SetStyle(Color.Beige, MyButtonType.round2Button);
            btn_quit.Text = StringResource.str("quit");
            btn_quit.ValidClick += new EventHandler(btn_quit_ValidClick);

            btn_flttype.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_flttype.ValidClick += new EventHandler(btn_flttype_ValidClick);

            btn_display.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_display.ValidClick += new EventHandler(btn_display_ValidClick);
            lbl_display.Visible = false;
            btn_display.Visible = false;

            btn_ktt.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_ktt.ValidClick += new EventHandler(btn_ktt_ValidClick);
            
            
            btn_throw.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_throw.ValidClick += new EventHandler(btn_throw_ValidClick);


            btn_navmeter.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_navmeter.ValidClick += new EventHandler(btn_navmeter_ValidClick);

            btn_measdelay.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_measdelay.ValidClick += new EventHandler(input_GotFocus);

            btn_meastimes.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_meastimes.ValidClick += new EventHandler(input_GotFocus);

            btn_date.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_date.ValidClick += new EventHandler(btn_date_ValidClick);

            btn_sampletimes.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_sampletimes.ValidClick += new EventHandler(input_GotFocus);

            btn_fltlength.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_fltlength.ValidClick += new EventHandler(input_GotFocus);

            btn_RsConfig.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_RsConfig.Text = StringResource.str("rsconfig");
            btn_RsConfig.ValidClick += new EventHandler(btn_RsConfig_ValidClick);

            btn_RxConfig.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_RxConfig.Text = StringResource.str("rxconfig");
            btn_RxConfig.ValidClick += new EventHandler(btn_RxConfig_ValidClick);

            btn_SysConfig.SetStyle(Color.LightBlue, MyButtonType.round2RectButton);
            btn_SysConfig.Text = StringResource.str("sysconfig");
            btn_SysConfig.ValidClick += new EventHandler(btn_SysConfig_ValidClick);

            btn_export.SetStyle(Color.LightSeaGreen, MyButtonType.round2RectButton);
            btn_export.Text = StringResource.str("export");
            btn_export.ValidClick += new EventHandler(btn_export_ValidClick);

            btn_throw.Visible = false;
            lbl_throw.Visible = false;
            btn_navmeter.Visible = false;
            lbl_navmeter.Visible = false;
            btn_flttype.Visible = false;
            lbl_flttype.Visible = false;

            FileInfo fi = new FileInfo(StringResource.basedir + "\\Tsioex.exe");
            lbl_version.Text = "版本: " + fi.LastWriteTime.ToString("yyyyMMdd");
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
            InitDisplay();
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
                Program.msg.Init(StringResource.str("noharddisk"));
                return;
            }

            Program.kbd.Init(StringResource.str("enter_exportdate"), "exportdate", false, KbdData);
        }
        public void enterscale(string type)
        {
            Program.kbd.Init(StringResource.str("enter_" + type + "B") + "," + RunWnd.syscfg.GetScaleAdjust(ActionMgr.ConvertToRange(type)).ToString("F3"), "scale" + type, false, KbdData);
        }

        public void entercurrent(string type)
        {
            Program.kbd.Init(StringResource.str("enter_"+type+"A"), type, false, KbdData);
        }
        public void EnterNewTime()
        {
            Program.kbd.Init(StringResource.str("enter_newtime"), "newtime", false, KbdData);
        }
        private string newdate = "";

        void btn_date_ValidClick(object sender, EventArgs e)
        {
            Program.kbd.Init(StringResource.str("enter_newdate"), "newdate", false, KbdData);
        }

        void btn_ktt_ValidClick(object sender, EventArgs e)
        {
            Program.kbd.Init(StringResource.str("enter_ktt"), "ktt", false, KbdData);
            
        }

        void btn_SysConfig_ValidClick(object sender, EventArgs e)
        {
            //same page no action at all
        }
        private string export_start = "";
        public void KbdData(string param, string data)
        {
            try
            {
                if (param.StartsWith("scale"))
                {
                    param = param.Remove(0, "scale".Length);
                    if(ActionMgr.ConvertToRange(param)!= ActionMgr.RNG_INVALID)
                        RunWnd.syscfg.SetScaleAdjust(ActionMgr.ConvertToRange(param), Convert.ToDouble(data));
                    return;
                }
                if (param == "5" || param == "10")
                    RunWnd.syscfg.SetCurrent(5, (param.IndexOfAny("5".ToCharArray()) < 0), Convert.ToDouble(data));
                if (param == "1" || param == "2" )
                    RunWnd.syscfg.SetCurrent(4, (param.IndexOfAny("1".ToCharArray()) < 0), Convert.ToDouble(data));
                if (param == "0.3" || param == "0.6")
                    RunWnd.syscfg.SetCurrent(3, (param.IndexOfAny("3".ToCharArray()) < 0), Convert.ToDouble(data));
                  if(  param == "0.1" || param == "0.2" )
                      RunWnd.syscfg.SetCurrent(2, (param.IndexOfAny("1".ToCharArray()) < 0), Convert.ToDouble(data));
                  if(  param == "0.01" || param == "0.02" )
                      RunWnd.syscfg.SetCurrent(1, (param.IndexOfAny("1".ToCharArray()) < 0), Convert.ToDouble(data));
                  if(  param == "0.001" || param == "0.002" )
                      RunWnd.syscfg.SetCurrent(0, (param.IndexOfAny("1".ToCharArray()) < 0), Convert.ToDouble(data));
                  if(  param == "0.0001" || param == "0.0002")
                      RunWnd.syscfg.SetCurrent(-1, (param.IndexOfAny("1".ToCharArray()) < 0), Convert.ToDouble(data));
                if (param == "exportdate")
                {
                    #region remove date log
                    if (Regex.IsMatch(data, "^820\\d\\d\\d\\d\\d\\d$"))
                    {
                        data = data.Remove(0, 1);
                        try
                        {
                            foreach (string dname in Directory.GetDirectories(StringResource.basedir, "20*"))
                            {
                                string[] todel = Directory.GetFiles(dname, "20*.txt");
                                foreach (string fname in todel)
                                {
                                    FileInfo fi = new FileInfo(fname);
                                    Match m = Regex.Match(fi.Name, @"(\d\d\d\d)_(\d\d)_(\d\d).*");
                                    if (m.Success)
                                    {
                                        if ((Convert.ToUInt16(data.Substring(0, 4)) <= Convert.ToUInt16(m.Groups[1].Value)) &&
                                            (Convert.ToUInt16(data.Substring(4, 2)) <= Convert.ToUInt16(m.Groups[2].Value)) &&
                                            (Convert.ToUInt16(data.Substring(6, 2)) <= Convert.ToUInt16(m.Groups[3].Value)))
                                        {
                                            //do nothing
                                        }
                                        else
                                        {
                                            File.Delete(fname);
                                        }
                                    }
                                }
                            }
                        }
                        catch
                        {
                        }
                        return;
                    }
                    #endregion

                    if (Regex.IsMatch(data, "^20\\d\\d\\d\\d\\d\\d$"))
                    {
                        export_start = data;
                        Program.kbd.Init(StringResource.str("enter_endexport"), "endexport", false, KbdData);
                    }
                    return;
                }
                if(param == "endexport")
                {
                    if (!Regex.IsMatch(data, "^20\\d\\d\\d\\d\\d\\d$"))
                        return;

                    try
                    {
                        foreach (string dname in Directory.GetDirectories(StringResource.basedir, "20*"))
                        {
                            foreach (string fname in Directory.GetFiles(dname, "20*.txt"))
                            {
                                FileInfo fi = new FileInfo(fname);
                                Match m = Regex.Match(fi.Name, @"(\d\d\d\d)_(\d\d)_(\d\d).*");
                                if (m.Success)
                                {
                                    if ((Convert.ToUInt16(data.Substring(0, 4)) >= Convert.ToUInt16(m.Groups[1].Value)) &&
                                        (Convert.ToUInt16(data.Substring(4, 2)) >= Convert.ToUInt16(m.Groups[2].Value)) &&
                                        (Convert.ToUInt16(data.Substring(6, 2)) >= Convert.ToUInt16(m.Groups[3].Value)) &&
                                        (Convert.ToUInt16(export_start.Substring(0, 4)) <= Convert.ToUInt16(m.Groups[1].Value)) &&
                                        (Convert.ToUInt16(export_start.Substring(4, 2)) <= Convert.ToUInt16(m.Groups[2].Value)) &&
                                        (Convert.ToUInt16(export_start.Substring(6, 2)) <= Convert.ToUInt16(m.Groups[3].Value)))
                                    {
                                        File.Copy(fname, StringResource.udiskdir + "\\" + fi.Name, true);
                                    }
                                }
                            }
                        }
                        Program.msg.Init(StringResource.str("export_done"));
                    }
                    catch
                    {
                        Program.msg.Init(StringResource.str("export_fail"));
                        return;
                    }
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
                    if(data.StartsWith("8")) //input of scale offset
                    {
                        string sparam = data.Remove(0, 1);
                        if (ActionMgr.ConvertToRange(sparam) != ActionMgr.RNG_INVALID)
                        {
                            this.Invoke(new Action<string>(enterscale), new string[] { sparam });
                        }
                        return;
                    }
                    if (data=="00000") //current calibration
                    {
                        Program.Upgrade();
                        return;
                    }
                    if(data == "99999") //current calibration
                    {
                        string sparam = data.Remove(0, 1);
                        RxInfo myrx = Program.lst_rxinfo[Program.mainwnd.selectedRx];
                        myrx.var.rRs = Program.lst_rsinfo[0].dTValue;
                        if (myrx.iRRange == ActionMgr.RNG_P001)
                            myrx.var.iK = 1; //1000:1
                        else if (myrx.iRRange == ActionMgr.RNG_P01)
                            myrx.var.iK = 10; //100:1
                        else if (myrx.iRRange == ActionMgr.RNG_P1)
                            myrx.var.iK = 100; //10:1
                        else
                            myrx.var.iK = (int)Math.Floor(1000.0 / (myrx.var.rRs*RunWnd.syscfg.GetStdCurrent(myrx.iIx,myrx.bSqrt)));
                        
                        if(myrx.var.iK > 1020)
                        {
                            Program.msg.Init("标准电阻阻值过小,无法校准。标准电阻上电压应不小于1V");
                            return;
                        }
                        Program.msg.Init(String.Format(@"电流校准完成,电流值为{0}", Util.FormatData(Program.mainwnd.task.act_mgr.CalibrateCurr(myrx), 8)));
                        return;
                    }
                    if (data == "1" || data == "2" ||
                        data == "5" || data == "10" ||
                        data == "0.3" || data == "0.6" ||
                        data == "0.1" || data == "0.2" ||
                        data == "0.01" || data == "0.02" ||
                        data == "0.001" || data == "0.002" ||
                        data == "0.0001" || data == "0.0002")
                    {
                        this.Invoke(new Action<string>(entercurrent), new string[] { data });
                        return;
                    }
                    if (data == "65890192")
                    {
                        Process.GetCurrentProcess().Kill(); //back door to exit the current program
                        return;
                    }
                    if (data == "658901920")
                    {
                        Program.OpenLog(0);
                        return;
                    }

                    if (data == "12345678")
                    {
                        Process app = new Process();
                        app.StartInfo.WorkingDirectory = @"\Windows";
                        app.StartInfo.FileName = @"\Windows\TouchKit.exe";
                        app.StartInfo.Arguments = "";
                        app.Start();
                        btn_quit_ValidClick(null, null);
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

        

        void btn_display_ValidClick(object sender, EventArgs e)
        {
            if (RunWnd.syscfg.iDisplay >= 4 || RunWnd.syscfg.iDisplay <= 0)
            {
                RunWnd.syscfg.iDisplay = 1;
            }
            else
            {
                RunWnd.syscfg.iDisplay = RunWnd.syscfg.iDisplay * 2;
            }
            InitDisplay();
        }

        void btn_flttype_ValidClick(object sender, EventArgs e)
        {
            if (RunWnd.syscfg.sFilterType == "filtertype1")
                RunWnd.syscfg.sFilterType = "filtertype2";
            else if (RunWnd.syscfg.sFilterType == "filtertype2")
                    RunWnd.syscfg.sFilterType = "filtertype3";
            else if (RunWnd.syscfg.sFilterType == "filtertype3")
                    RunWnd.syscfg.sFilterType = "filtertype4";
            else
                    RunWnd.syscfg.sFilterType = "filtertype1";
            InitDisplay();
        }

        void btn_quit_ValidClick(object sender, EventArgs e)
        {
            if (Program.rswnd.CheckError())
            {
                this.Invoke(new Action<object,EventArgs>(btn_RsConfig_ValidClick), new object[]{null,null});
                return;
            }
            Program.SwitchWindow("mainwnd");
            Program.mainwnd.Invoke(new Action<bool>(Program.mainwnd.ReDraw), new object[] { false });                
        }

        void btn_RsConfig_ValidClick(object sender, EventArgs e)
        {
            Program.SwitchWindow("rsconfig");
        }

        void btn_RxConfig_ValidClick(object sender, EventArgs e)
        {
            Program.SwitchWindow("rxconfig");
        }


        public void InitDisplay()
        {
            btn_display.Text = RunWnd.syscfg.iDisplay.ToString();
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

        private void btn_display_Load(object sender, EventArgs e)
        {

        }

        private void rectButton1_Load(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void lbl_fltlength_ParentChanged(object sender, EventArgs e)
        {

        }
    }
}