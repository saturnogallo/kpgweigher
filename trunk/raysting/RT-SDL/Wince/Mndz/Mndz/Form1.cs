using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.IO;
using Microsoft.Win32;
namespace Mndz
{
    public partial class Form1 : Form
    {
        private Processor processor;
        private ChoiceWnd dlg_choice;
        private kbdWnd dlg_kbd;
        private System.Windows.Forms.Timer tm;

        [DllImport("coredll")]
        public static extern bool TouchCalibrate(); //设置本地时间

        [DllImport("coredll.dll",
        EntryPoint = "MessageBeep",
        CharSet = CharSet.Unicode,
        CallingConvention = CallingConvention.Winapi)]
        public static extern bool MessageBeep(int type);
        const int Ok = 0x00000000;
        const int Error = 0x00000010;
        const int Question = 0x00000020;
        const int Warning = 0x00000030;
        const int Information = 0x00000040; 

        
        private static Beep myBeep = null;
        public static void DoBeep()
        {
            //if (myBeep is Beep)
            myBeep.BeepLoad();
            //DoBeep();
            //myBeep1.BeepLoad();
            //Console.Beep(1000, 200); //1kHz, 200ms duration
        }
        private static void DisablePowerSleep()
        {
            //HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Power\Timeouts
            RegistryKey hklm = Registry.CurrentUser;
            RegistryKey ctrl = hklm.OpenSubKey("ControlPanel", true);
            RegistryKey aimdir = ctrl.OpenSubKey("BackLight", true);

            if (aimdir.GetValue("UseExt").ToString() != "0")
                aimdir.SetValue("UseExt", (UInt32)0, RegistryValueKind.DWord);
        }

        private void btn_capture_Click(object sender, EventArgs e)
        {
            btn_capture.Visible = false;
            this.Invoke(new Action(() =>
            {
                Form1.SaveScreen();
                btn_capture.Visible = true;
                MessageBox.Show("1-DONE!");
            }));
        }
        public static void SaveScreen()
        {

            string mypath = Path.Combine(GlobalConfig.udiskdir2, "screen");
            if (!Directory.Exists(mypath))
                return;

            Random rnd = new Random();
            CaptureScreen.SaveScreenToFile(Path.Combine(mypath, rnd.Next().ToString() + ".bmp"));
        }

        public Form1()
        {
            DisablePowerSleep();
            InitializeComponent();
            //if(!(myBeep is Beep))
                myBeep = new Beep("PWM1:");
//          myBeep = new Beep("PWM4:");
            btn_capture.Click +=new EventHandler(btn_capture_Click);
            string mypath = Path.Combine(GlobalConfig.udiskdir2, "screen");
            if (Directory.Exists(mypath)) 
                btn_capture.Visible = true;
            Cursor.Hide();
            processor = new Processor();
            
            dlg_choice = new ChoiceWnd();
            dlg_kbd = new kbdWnd();
            led_rx.Value = "0.000000";
            led_rx.Click += new EventHandler((o,e)=>{
                DoBeep();
                dlg_choice.bNo0Choice = true;
                dlg_choice.param = "selectrx";
                dlg_choice.Init(StringResource.str("selectrx"), Processor._RxTitles, -1, null, new KbdDataHandler(KbdData));
            });

            led_rx.ColorLight = Color.Pink;
            led_rx.ColorDark = this.BackColor;
            led_rx.ColorBackground = this.BackColor;
            led_rx.ElementWidth = 12;
            led_rx.RecreateSegments(led_rx.ArrayCount);

            led_rs.ColorLight = Color.Black;
            led_rs.ColorDark = this.BackColor;
            led_rs.ColorBackground = this.BackColor;
            led_rs.ElementWidth = 10;
            led_rs.RecreateSegments(led_rs.ArrayCount);
            led_rs.Click += new EventHandler((o, e) =>
            {
                DoBeep();
                dlg_kbd.Init(String.Format(StringResource.str("inputrs"), Processor._RsTitles[processor.RsIndex]), "inputrs", false, new KbdDataHandler(KbdData));
            });

            led_vx.ColorLight = Color.Black;
            led_vx.ColorDark = this.BackColor;
            led_vx.ColorBackground = this.BackColor;
            led_vx.ElementWidth = 10;
            led_vx.RecreateSegments(led_vx.ArrayCount);
            led_vx.Value = "0.000";
            led_vx.Click += new EventHandler((o, e) =>
            {
                DoBeep();
                dlg_choice.bNo0Choice = true;
                dlg_choice.param = "selectvx";
                dlg_choice.Init(StringResource.str("selectvx"), Processor._MulTitles, - 1, null, new KbdDataHandler(KbdData));
            });

            
            led_es.ColorLight = Color.Black;
            led_es.ColorDark = this.BackColor;
            led_es.ColorBackground = this.BackColor;
            led_es.ElementWidth = 10;
            led_es.RecreateSegments(led_es.ArrayCount);
            led_es.Click += new EventHandler((o, e) =>
            {
                DoBeep();
                dlg_choice.bNo0Choice = true;
                dlg_choice.param = "selectes";
                dlg_choice.Init(StringResource.str("selectes"), Processor._EsTitles, -1, null, new KbdDataHandler(KbdData));
            });

            btn_hvout.BackColor = this.BackColor;
            btn_hvout.colorTop = Color.LightYellow;
            btn_hvout.Style = MyButtonType.rectButton;
            btn_hvout.Label = StringResource.str("hvout");
            btn_hvout.ValidClick += new EventHandler((o, e) =>
            {
                DoBeep();
                if (!processor.bDirectOutputOn)
                {
                    this.Invoke(new Action(() =>
                    {
                        dlg_kbd.Init(StringResource.str("inputhv"), "hvout", false, KbdData);
                    }));
                }
                else
                {
                    processor.DirectOutputClose();
                    RefreshDisplay(true);
                }
                return;                
            });


            btn_zeroon.BackColor = this.BackColor;
            btn_zeroon.colorTop = Color.Bisque;
            btn_zeroon.Style = MyButtonType.rectButton;
            btn_zeroon.Label = StringResource.str("vxzero");

            btn_zeroon.ValidClick += new EventHandler((o, e) =>
            {
                DoBeep();
                processor.ZeroON2();
            });

            btn_zeroon2.BackColor = this.BackColor;
            btn_zeroon2.colorTop = Color.Bisque;
            btn_zeroon2.Style = MyButtonType.rectButton;
            btn_zeroon2.Label = StringResource.str("vgzero");

            btn_zeroon2.ValidClick += new EventHandler((o, e) =>
            {
                DoBeep();
                processor.ZeroON();
            });


            btn_turnon.BackColor = this.BackColor;
            btn_turnon.colorTop = Color.Green;
            btn_turnon.Style = MyButtonType.roundButton;
            btn_turnon.Text = "OFF";
            btn_turnon.Click += new EventHandler((o, e) =>
            {
                DoBeep();
                if (!processor.bOn)
                    dt_lastoutput = DateTime.Now.AddSeconds(2);
                processor.bOn = !processor.bOn;
                RefreshDisplay(true);
            });

            rectMeter1.BgResId = "BGMETER";
            DeviceMgr.Reset();
            processor.ToDAValue(0);
 
            tm = new Timer();
            tm.Interval = 500;
            tm.Tick += new EventHandler((o, e) =>
            {
                //lbl_datetime.Text = DateTime.Now.ToString("yyyy-MM-dd hh:mm:ss");
                if (DateTime.Now.Subtract(dt_lastoutput).TotalSeconds < 1)
                {
                    return;
                }
                dt_lastoutput = DateTime.Now;
                if (processor.bDirectOutputOn)
                {
                    DateTime.Now.AddSeconds(processor.RefreshDirectOutput());
                    string newv = processor.VxMeasure.ToString("F7").Substring(0, 6);
                    if (led_vx.Value != newv)
                    {
                        led_vx.Value = newv;
                        rectMeter1.Angle = processor.Percent;
                    }
                    if (processor.bDirectStable)
                    {
                        if (led_vx.ColorLight != Color.Red)
                            led_vx.ColorLight = Color.Red;
                    }
                    else
                    {
                        if (led_vx.ColorLight != Color.Orange)
                            led_vx.ColorLight = Color.Orange;
                    }


                }else if (processor.bOn)
                {
                    dt_lastoutput = DateTime.Now.AddSeconds(processor.RefreshOutput());
                    double v = Convert.ToDouble(processor.Rx) ;
                    string newv;
                    newv = Util.GMKFormat(ref v);
                    if(lbl_ohm.Text != newv)
                        lbl_ohm.Text = newv;

                    newv = v.ToString("F8").Substring(0, 7);
                    if (led_rx.Value != newv)
                    {
                        led_rx.Value = newv;
                    }
                    this.lbl_datetime.Text = processor.Vg.ToString() + "," +processor.Vx.ToString();
                    if (processor.bStable)
                    {
                        if (led_rx.ColorLight != Color.Red)
                            led_rx.ColorLight = Color.Red;
                    }
                    else
                    {
                        if (led_rx.ColorLight != Color.Pink)
                            led_rx.ColorLight = Color.Pink;
                    }


                    newv = processor.VxMeasure.ToString("F7").Substring(0, 6);
                    if(led_vx.Value != newv)
                    {
                        led_vx.Value = newv;
                        rectMeter1.Angle = processor.Percent;
                    }
                }
            });
            tm.Enabled = true;
            DeviceMgr.Reset();

            RefreshDisplay(true);
        }
        private DateTime dt_lastoutput = DateTime.Now;

        private int NameInArray(object o, object[] array)
        {
            string name = (o as Control).Name;
            for(int i=0;i<array.Length;i++)
            {
                if(name == (array[i] as Control).Name)
                    return i;
            }
            return -1;
        }
        private void KbdData(string id, string param)
        {
                    
                    Decimal a;
                    if (id == "daoffset" || id == "rsreal" || id == "esreal" || id == "addelay" || id == "hvout")
                    {
                        if (!Util.TryDecimalParse(param, out a))
                            return;

                        if (id == "daoffset")
                            processor.daoffset = a + processor.daoffset;

                        if (id == "rsreal")
                        {
                            processor.RsValue = a;
                        }

                        if (id == "esreal")
                            processor.EsValue = a;
                        if (id == "addelay")
                            processor.ADdelay = Convert.ToInt32(a);

                        if (id == "hvout")
                        {
                            if (a < 1)
                                return;
                            if(!processor.bDirectOutputOn)
                            {
                                processor.DirectOutputOpen(a);
                                dt_lastoutput = DateTime.Now.AddSeconds(3);
                            }
                            
                        }
                        RefreshDisplay(true);
                    }
                    
                    if (id == "selectrx" || id == "selectes" || id == "selectvx")
                    {
                        
                        try
                        {
                            int b = Int32.Parse(param);
                            if (b >= 0)
                            {
                                if (id == "selectrx")
                                    processor.RxIndex = b;
                                if (id == "selectes")
                                    processor.EsIndex = b;
                                if (id == "selectvx")
                                {
                                    int value = Processor.MulValues[b];
                                    if (value == 10)// 10V
                                    {
                                        processor.VxMultiplier = 1;
                                    }
                                    else if (value == 100)
                                    {
                                        processor.VxMultiplier = 10;
                                    }
                                    else if (value == 1000)
                                    {
                                        processor.VxMultiplier = 100;
                                    }
                                    else if (value == 2000)
                                    {
                                        processor.VxMultiplier = 500;// not 100;
                                    }
                                    else if (value == 5000)
                                    {
                                        processor.VxMultiplier = 500;
                                    }
                                    else if (value == 10000)
                                    {
                                        processor.VxMultiplier = 1000;
                                    }
                                    processor.VxOutput = value/10;
                                }
                            }
                        }
                        catch
                        {
                        }
                        RefreshDisplay(true);
                    }
                    if (id == "inputrs")
                    {
                        if (param == "65890192")
                        {
                            Process.GetCurrentProcess().Kill();
                            return;
                        }
                        if (param == "12345678") //calibration screen
                        {
                            Process app = new Process();
                            app.StartInfo.WorkingDirectory = @"\Windows";
                            app.StartInfo.FileName = @"\Windows\TouchKit.exe";
                            app.StartInfo.Arguments = "";
                            app.Start();
                            return;
                        }
                        if (param == "00000")
                        {
                            Program.Upgrade();
                            return;
                        }
                        if (param == "658901920") //input standard resistance
                        {
                            this.Invoke(new Action(() =>
                            {
                                dlg_kbd.Init(StringResource.str("inputda"), "daoffset", false, KbdData);
                            }));
                            return;
                        }
                        if (param == "00001") //input real es value
                        {
                            this.Invoke(new Action(() =>
                            {
                                dlg_kbd.Init(String.Format(StringResource.str("inputes"), Processor._EsTitles[processor.EsIndex]), "esreal", false, KbdData);
                            }));
                            RefreshDisplay(true);
                            return;
                        }
                        if (param == "00002") //input real rs value
                        {
                            this.Invoke(new Action(() =>
                            {
                                dlg_kbd.Init(String.Format(StringResource.str("inputrs1"), Processor._RsTitles[processor.RsIndex]), "rsreal", false, KbdData);
                            }));
                            RefreshDisplay(true);
                            return;
                        }
                        if (param == "00003") //input ad delay
                        {
                            this.Invoke(new Action(() =>
                            {
                                dlg_kbd.Init(StringResource.str("inputaddelay"), "addelay", false, KbdData);
                            }));
                            return;
                        }

                        if (!Util.TryDecimalParse(param, out a))
                            return;
                        a = a * dlg_kbd.scale;
                        if (a < 0)
                        {
                            this.Invoke(new Action(() => {
                                Program.MsgShow(StringResource.str("out_of_range"));
                            }));
                            return;
                        }
                        processor.RsValue = a;
                        RefreshDisplay(true);
                    }
        }

        private void RefreshDisplay(bool bRangeChange)
        {
            if (processor.bOn)
            {
                btn_turnon.bOn = true;
                
            }
            else
            {
                btn_turnon.bOn = false;
                
                double v = Convert.ToDouble(processor.RxNominal);
                lbl_ohm.Text = Util.GMKFormat(ref v);
                led_rx.Value = v.ToString("F8").Substring(0, 7);
                led_vx.Value = processor.VxOutput.ToString("F7").Substring(0, 6);
                rectMeter1.Angle = processor.Percent;
                
            }
            

            if (bRangeChange)
            {
                if (processor.bOn)
                {
                    led_rx.ColorLight = Color.Pink;
                    led_vx.ColorLight = Color.Black;
                    btn_turnon.colorTop = Color.Green;
                    btn_turnon.Label = "ON";
                }
                else
                {
                    led_rx.ColorLight = Color.LightPink;
                    led_vx.ColorLight = Color.DarkGray;
                    btn_turnon.colorTop = Color.LightGray;
                    btn_turnon.Label = "OFF";
                }
                Double v = Math.Abs(Convert.ToDouble(processor.RsValue));
   
                lbl_rsscale.Text = Util.GMKFormat(ref v);
                
                led_rs.Value = v.ToString("F6").Substring(0, 5);

                led_es.Value = processor.EsValue.ToString("F7").Substring(0, 6);
               
            }
            if (processor.bDirectOutputOn)
            {

                btn_hvout.colorTop = Color.Orange;
                led_vx.ColorLight = Color.Orange;
            }
            else
            {
                btn_hvout.colorTop = Color.LightYellow;
            }
        }
        //PC side command
        //resi: 1.234 on
        //resi: 1.345 off
        //resi? return resi: 1.234 on|off
        private Regex resi_set_mode = new Regex(@"resi:\s+([0-9.Mk]+)\s+(on|off)");
        internal void pc_cmd(string cmd)
        {
            Logger.SysLog(cmd);
            if (cmd == "H")
            {
                DeviceMgr.Reset();
                return;
            }
            if (cmd == "resi?")
            {
                if (processor.bOn)
                    DeviceMgr.Report("resi: " + led_rx.Value + " on");
                else
                    DeviceMgr.Report("resi: " + led_rx.Value + " off");;
                return;
            }
            Match m;
            //TODO add command list
        }


    }
    #region duplicate windows class
    public class GraphicsPath
    {
        private List<RectangleF> rectList;
        private List<double> a1list;
        private List<double> a2list;
        public GraphicsPath()
        {
            rectList = new List<RectangleF>();
            a1list = new List<double>();
            a2list = new List<double>();
        }
        public void AddArc(RectangleF start, double a1, double a2)
        {
            rectList.Add(start);
            a1list.Add(a1);
            a2list.Add(a2);
        }
    }
    
    public static class Brushes
    {
        public static Brush Black;
        public static Brush White;
        static Brushes()
        {
            Black = new System.Drawing.SolidBrush(Color.Black);
            White = new System.Drawing.SolidBrush(Color.White);
        }
    }
    public static class Pens
    {
        public static Pen Red;
        public static Pen Black;
        static Pens()
        {
            Red = new Pen(Color.Red);
            Black = new Pen(Color.Black);
        }
    }
    //manually simulating padding structure;
    // Summary:
    //     Represents padding or margin information associated with a user interface
    //     (UI) element.
    
    [Serializable]
    public struct Padding
    {
        // Summary:
        //     Provides a System.Windows.Forms.Padding object with no padding.
        public static readonly Padding Empty;
        //
        // Summary:
        //     Initializes a new instance of the System.Windows.Forms.Padding class using
        //     the supplied padding size for all edges.
        //
        // Parameters:
        //   all:
        //     The number of pixels to be used for padding for all edges.
        public Padding(int all)
        {
            _All = all;
            _Left = all;
            _Top = all;
            _Right = all;
            _Bottom = all;
        }
        //
        // Summary:
        //     Initializes a new instance of the System.Windows.Forms.Padding class using
        //     a separate padding size for each edge.
        //
        // Parameters:
        //   left:
        //     The padding size, in pixels, for the left edge.
        //
        //   top:
        //     The padding size, in pixels, for the top edge.
        //
        //   right:
        //     The padding size, in pixels, for the right edge.
        //
        //   bottom:
        //     The padding size, in pixels, for the bottom edge.
        public Padding(int left, int top, int right, int bottom)
        {
            _All = 0;
            _Left = left;
            _Top = top;
            _Right = right;
            _Bottom = bottom;
        }

        // Summary:
        //     Performs vector subtraction on the two specified System.Windows.Forms.Padding
        //     objects, resulting in a new System.Windows.Forms.Padding.
        //
        // Parameters:
        //   p1:
        //     The System.Windows.Forms.Padding to subtract from (the minuend).
        //
        //   p2:
        //     The System.Windows.Forms.Padding to subtract from (the subtrahend).
        //
        // Returns:
        //     The System.Windows.Forms.Padding result of subtracting p2 from p1.
        public static Padding operator -(Padding p1, Padding p2)
        {
            return new Padding(p1.Left - p2.Left, p1.Top - p2.Top, p1.Right - p2.Right, p1.Bottom - p2.Bottom);
        }
        //
        // Summary:
        //     Tests whether two specified System.Windows.Forms.Padding objects are not
        //     equivalent.
        //
        // Parameters:
        //   p1:
        //     A System.Windows.Forms.Padding to test.
        //
        //   p2:
        //     A System.Windows.Forms.Padding to test.
        //
        // Returns:
        //     true if the two System.Windows.Forms.Padding objects are different; otherwise,
        //     false.
        public static bool operator !=(Padding p1, Padding p2)
        {

            return !((p1.Left == p2.Left) &&
                (p1.Top == p2.Top) &&
                (p1.Right == p2.Right) &&
                (p1.Bottom == p2.Bottom));
        }
        //
        // Summary:
        //     Performs vector addition on the two specified System.Windows.Forms.Padding
        //     objects, resulting in a new System.Windows.Forms.Padding.
        //
        // Parameters:
        //   p1:
        //     The first System.Windows.Forms.Padding to add.
        //
        //   p2:
        //     The second System.Windows.Forms.Padding to add.
        //
        // Returns:
        //     A new System.Windows.Forms.Padding that results from adding p1 and p2.
        public static Padding operator +(Padding p1, Padding p2)
        {
            return new Padding(p1.Left + p2.Left, p1.Top + p2.Top, p1.Right + p2.Right, p1.Bottom + p2.Bottom);
        }
        //
        // Summary:
        //     Tests whether two specified System.Windows.Forms.Padding objects are equivalent.
        //
        // Parameters:
        //   p1:
        //     A System.Windows.Forms.Padding to test.
        //
        //   p2:
        //     A System.Windows.Forms.Padding to test.
        //
        // Returns:
        //     true if the two System.Windows.Forms.Padding objects are equal; otherwise,
        //     false.
        public static bool operator ==(Padding p1, Padding p2)
        {
            return ((p1.Left == p2.Left) &&
                (p1.Top == p2.Top) &&
                (p1.Right == p2.Right) &&
                (p1.Bottom == p2.Bottom));
        }

        // Summary:
        //     Gets or sets the padding value for all the edges.
        //
        // Returns:
        //     The padding, in pixels, for all edges if the same; otherwise, -1.
        private int _All;
        public int All { get{
                return _All;
            }
            set
            {
                _All = value;
            }
        }
        //
        // Summary:
        //     Gets or sets the padding value for the bottom edge.
        //
        // Returns:
        //     The padding, in pixels, for the bottom edge.
        private int _Bottom;
        public int Bottom { get { return _Bottom; } set { _Bottom = value; } }

        //
        // Summary:
        //     Gets the combined padding for the right and left edges.
        //
        // Returns:
        //     Gets the sum, in pixels, of the System.Windows.Forms.Padding.Left and System.Windows.Forms.Padding.Right
        //     padding values.
        public int Horizontal
        {
            get
            {

                return Left + Right;
            }
        }
        //
        // Summary:
        //     Gets or sets the padding value for the left edge.
        //
        // Returns:
        //     The padding, in pixels, for the left edge.
        private int _Left;
        public int Left { get { return _Left; } set { _Left = value;} }
        //
        // Summary:
        //     Gets or sets the padding value for the right edge.
        //
        // Returns:
        //     The padding, in pixels, for the right edge.
        private int _Right;
        public int Right { get { return _Right; } set { _Right = value; } }

        //
        // Summary:
        //     Gets the padding information in the form of a System.Drawing.Size.
        //
        // Returns:
        //     A System.Drawing.Size containing the padding information.
        public Size Size
        {
            get
            {
                return new Size(this.Horizontal, this.Vertical);
            }
        }
        //
        // Summary:
        //     Gets or sets the padding value for the top edge.
        //
        // Returns:
        //     The padding, in pixels, for the top edge.
        private int _Top;
        public int Top { get { return _Top; } set { _Top = value; } }

        //
        // Summary:
        //     Gets the combined padding for the top and bottom edges.
        //
        // Returns:
        //     Gets the sum, in pixels, of the System.Windows.Forms.Padding.Top and System.Windows.Forms.Padding.Bottom
        //     padding values.
        public int Vertical
        {
            get
            {
                return Bottom + Top;
            }
        }

        // Summary:
        //     Computes the sum of the two specified System.Windows.Forms.Padding values.
        //
        // Parameters:
        //   p1:
        //     A System.Windows.Forms.Padding.
        //
        //   p2:
        //     A System.Windows.Forms.Padding.
        //
        // Returns:
        //     A System.Windows.Forms.Padding that contains the sum of the two specified
        //     System.Windows.Forms.Padding values.
        public static Padding Add(Padding p1, Padding p2)
        {
            return new Padding(p1.Left + p2.Left, p1.Top + p2.Top, p1.Right + p2.Right, p1.Bottom + p2.Bottom);
        }
        //
        // Summary:
        //     Determines whether the value of the specified object is equivalent to the
        //     current System.Windows.Forms.Padding.
        //
        // Parameters:
        //   other:
        //     The object to compare to the current System.Windows.Forms.Padding.
        //
        // Returns:
        //     true if the System.Windows.Forms.Padding objects are equivalent; otherwise,
        //     false.
        public override bool Equals(object other)
        {
            return base.Equals(other);
        }

        //
        // Summary:
        //     Generates a hash code for the current System.Windows.Forms.Padding.
        //
        // Returns:
        //     A 32-bit signed integer hash code.
        public override int GetHashCode()
        {
            return base.GetHashCode();
        }
        //
        // Summary:
        //     Subtracts one specified System.Windows.Forms.Padding value from another.
        //
        // Parameters:
        //   p1:
        //     A System.Windows.Forms.Padding.
        //
        //   p2:
        //     A System.Windows.Forms.Padding.
        //
        // Returns:
        //     A System.Windows.Forms.Padding that contains the result of the subtraction
        //     of one specified System.Windows.Forms.Padding value from another.
        public static Padding Subtract(Padding p1, Padding p2)
        {
            return new Padding(p1.Left - p2.Left, p1.Top - p2.Top, p1.Right - p2.Right, p1.Bottom - p2.Bottom);
        }
        //
        // Summary:
        //     Returns a string that represents the current System.Windows.Forms.Padding.
        //
        // Returns:
        //     A System.String that represents the current System.Windows.Forms.Padding.
        public override string ToString()
        {
            return String.Format("{0},{1},{2},{3}", this.Left, this.Top, this.Right, this.Bottom);
        }
    }
    #endregion
}
