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

        public Form1()
        {
            InitializeComponent();

            string mypath = Path.Combine(GlobalConfig.udiskdir2, "screen");
            if (Directory.Exists(mypath))
                btn_capture.Visible = true;
            Cursor.Hide();
            processor = new Processor();
            
            digi_upbtns = new RectButton[] { rbtn_1up, rbtn_2up, rbtn_3up, rbtn_4up, rbtn_5up, rbtn_6up  };
            digi_dnbtns = new RectButton[] { rbtn_1dn, rbtn_2dn, rbtn_3dn, rbtn_4dn, rbtn_5dn, rbtn_6dn  };
            real_btns = new RectButton[] { rbtn_real1,rbtn_real2,rbtn_real3,rbtn_real4,rbtn_real5,rbtn_real6,rbtn_real7, rbtn_real8, rbtn_real9, rbtn_real10,
                                            rbtn_real11,rbtn_real12,rbtn_real13,rbtn_real14,rbtn_real15};
            dlg_choice = new ChoiceWnd();
            dlg_kbd = new kbdWnd();
            range_btns = new RectButton[] { btn_rangeReal, btn_range0, btn_range1, btn_range2, btn_range3 };
            string[] range_string = new String[] { "实物电阻", "20mΩ", "200mΩ", "2Ω", "20Ω" };
            led_ohm.ColorLight = Color.Red;
            led_ohm.ColorBackground = this.BackColor;
            led_ohm.ElementWidth = 12;
            led_ohm.RecreateSegments(led_ohm.ArrayCount);
            led_current.ColorLight = Color.DarkGreen;
            led_current.ColorBackground = this.BackColor;
            led_current.ElementWidth = 8;
            led_current.RecreateSegments(led_current.ArrayCount);
            led_current.Value = "0.0000";

            btn_zeroon.bgColor = this.BackColor;
            btn_zeroon.SetStyle(Color.Bisque, MyButtonType.roundRectButton);
            btn_zeroon.Text = "清零";
            btn_zeroon.ValidClick += new EventHandler((o, e) =>
            {
                led_current.Value = "     ";
                processor.ZeroON();
            });

            btn_turnon.bgColor = this.BackColor;
            for (int i = 0; i < digi_upbtns.Length; i++)
            {
                RectButton rb = digi_upbtns[i];
                rb.bgColor = this.BackColor;
                rb.SetStyle(Color.DarkOrange, MyButtonType.triangleupButton);

                rb.ValidClick += new EventHandler((o, e) =>
                {
                    if (processor.iRange >= 0)
                    {
                        TickDigit(digi_upbtns.Length - NameInArray(o, digi_upbtns), true);
                        RefreshDisplay(false);
                    }
                });
            }
            for (int i = 0; i < digi_dnbtns.Length; i++)
            {
                RectButton rb = digi_dnbtns[i];
                rb.bgColor = this.BackColor;
                rb.SetStyle(Color.DarkOrange, MyButtonType.trianglednButton);
                rb.ValidClick += new EventHandler((o,e)=>
                {
                    if (processor.iRange >= 0)
                    {
                        TickDigit(digi_upbtns.Length - NameInArray(o, digi_dnbtns) , false);
                        RefreshDisplay(false);
                    }
                });
            }
            for(int i = 0;i < real_btns.Length; i++)
            {
                RectButton rb = real_btns[i];
                rb.bgColor = this.BackColor;
                rb.SetStyle(Color.DarkOrange, MyButtonType.roundButton);
                rb.Text = Processor.real_title[i] + "Ω";
                rb.ValidClick += new EventHandler((o, e) =>
                {
                    processor.iReal = NameInArray(o, real_btns);
                    RefreshDisplay(false);
                });
            }
            btn_turnon.SetStyle(Color.Green, MyButtonType.round2Button);
            btn_turnon.Text = "OFF";
            btn_turnon.Click += new EventHandler((o, e) =>
            {
                if (!processor.bOn)
                    dt_lastoutput = DateTime.Now.AddSeconds(2);
                processor.bOn = !processor.bOn;
                RefreshDisplay(false);
            });

            for (int i = 0; i < range_btns.Length; i++)
            {
                RectButton rb = range_btns[i];
                rb.bgColor = this.BackColor;
                rb.Text = range_string[i];
                rb.SetStyle(Color.Green, MyButtonType.roundRectButton);

                rb.ValidClick += new EventHandler((o, e) => 
                {
                    int newrange = NameInArray(o, range_btns)-1;
                    if( newrange == processor.iRange)
                        return;

                    processor.iRange = newrange;
                    RefreshDisplay(true); 
                });
            }
            tm = new Timer();
            tm.Interval = 500;
            tm.Tick += new EventHandler((o, e) =>
            {
                lbl_datetime.Text = DateTime.Now.ToString("yyyy-MM-dd hh:mm:ss");
                if (DateTime.Now.Subtract(dt_lastoutput).TotalSeconds < 1)
                {
                    dt_lastoutput = DateTime.Now;
                    return;
                }
                processor.RefreshOutput();
                if (processor.Current > -999)
                {
                    UpdateCurrent(processor.Current);
                    processor.Current = -9999;
                }
            });
            tm.Enabled = true;
            led_ohm.Click += new EventHandler(led_ohm_Click);
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
        public static void SaveScreen()
        {
      
            string mypath = Path.Combine(GlobalConfig.udiskdir2, "screen");
            if (!Directory.Exists(mypath))
                return;

            Random rnd = new Random();
            CaptureScreen.SaveScreenToFile(Path.Combine(mypath,rnd.Next().ToString()+".bmp"));
        }
        private void KbdData(string id, string param)
        {
                    Decimal a;
                    if (id == "daoffset")
                    {
                        if (!Util.TryDecimalParse(param, out a))
                            return;

                        processor.daoffset = a + processor.daoffset;
                        
                    }
                    if (id == "value")
                    {
                        if (param == "65890192")
                        {
                            Process.GetCurrentProcess().Kill();
                            return;
                        }
                        if (param == "12345678") //calibration screen
                        {
                            Form1.TouchCalibrate();
                            /*
                            Process app = new Process();
                            app.StartInfo.WorkingDirectory = @"\Windows";
                            app.StartInfo.FileName = @"\Windows\TouchKit.exe";
                            app.StartInfo.Arguments = "";
                            app.Start();
                             */
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
                                dlg_kbd.Init("请输入DA零位值", "daoffset", false, KbdData);
                            }));
                            return;
                        }
                        if (!Util.TryDecimalParse(param, out a))
                            return;
                        int newrange = Processor.CheckRange(a, processor.iRange);
                        if (newrange < 0)
                        {
                            this.Invoke(new Action(() => {
                                Program.MsgShow("输入值超出范围");
                            }));
                            return;
                        }
                        processor.iRange = newrange;
                        processor.resistance = a;
                        RefreshDisplay(true);
                    }
        }
        public void led_ohm_Click(object sender, object e)
        {
            if (processor.iRange >= 0)
            {
                dlg_kbd.Init("请输入模拟电阻值", "value", false, KbdData);
            }
        }
        private RectButton[] digi_dnbtns;
        private RectButton[] digi_upbtns;
        private RectButton[] real_btns;

        private RectButton[] range_btns;
        
        private RectButton Button(string type, int index)
        {
            if (type == "digi_up")
                return digi_upbtns[index];
            if (type == "digi_dn")
                return digi_dnbtns[index];
            if (type == "real")
                return real_btns[index];
            return null;
        }
        //up or down the digit based on position highest ditig is 1 lowest digit is 6
        internal void TickDigit(int position, bool bUp)
        {
            Decimal a;
            if (processor.iRange < 0) //real resi case
                return;

            a = processor.resistance;
            int unit = 0;
            unit = processor.iRange - position - 1;
            
            if(bUp)
                a = a + Convert.ToDecimal(Math.Pow(10,  unit));
            else
                a = a - Convert.ToDecimal(Math.Pow(10, unit));

            double b = Convert.ToDouble(a);
            if ((processor.iRange == 0) && (b > 0.02))
                return;
            if (((processor.iRange == 1) && (b > 0.2)))
                return;
            if (((processor.iRange == 2) && (b > 2)) )
                return;
            if (((processor.iRange == 3) && (b > 20)))
                return;
            if (b < 0)
                return;
            processor.resistance = a;
        }
        public void UpdateCurrent(double reading)
        {
            if(processor.iRange < 0 || processor.iRange > 3)
            {
                lbl_currscale.Text = "A";
                led_current.Value = "     ";
                return;
            }

            if(Math.Abs(reading) < 0.95)
            {
//                reading = reading * 1000;
//                lbl_currscale.Text  = "mA";
                lbl_currscale.Text = "A";
            }else{
                lbl_currscale.Text = "A";
            }
            string newcurr = reading.ToString("F6").Substring(0, 6);
            if (newcurr != led_current.Value)
                led_current.Value = newcurr;
        }
        private void RefreshDisplay(bool bRangeChange)
        {
            if (processor.bOn)
            {
                btn_turnon.Text = "ON";
                btn_turnon.bOn = true;
            }
            else
            {
                btn_turnon.Text = "OFF";
                btn_turnon.bOn = false;
            }
            if (bRangeChange)
            {
                for (int i = 0; i < range_btns.Length; i++)
                {
                    range_btns[i].bOn = (processor.iRange == (i - 1));
                }
            }
            bool realmode = (processor.iRange < 0); //real resistance case
            
            foreach (RectButton rb in digi_upbtns)
            {
                rb.Visible = !realmode;
            }
            foreach (RectButton rb in digi_dnbtns)
            {
                rb.Visible = !realmode;
            }
            gb_real.Visible = realmode;
            foreach (RectButton rb in real_btns)
            {
                rb.bOn = false;
            }
            if (processor.iReal >= 0)
                real_btns[processor.iReal].bOn = true;

            if (realmode)
            {
                if (processor.iReal >= 0)
                {
                    led_ohm.Value = Processor.real_dispvalue[processor.iReal].ToString(); //real case
                    lbl_ohm.Text = Processor.real_disptitle[processor.iReal]+"Ω";
                }
                else
                {
                    led_ohm.Value = "       ";
                    lbl_ohm.Text = "Ω";
                }
            }
            else
            {
                double a = Convert.ToDouble(processor.resistance);
                if (processor.iRange < 2)
                {
                    lbl_ohm.Text = "mΩ";
                    a = a * 1000.0;
                    led_ohm.Value = a.ToString("F" + (4 - processor.iRange).ToString());
                }
                else
                {
                    lbl_ohm.Text = "Ω";
                    led_ohm.Value = a.ToString("F" + (7 - processor.iRange).ToString());
                }
            }
        }
        //PC side command
	//H reset
	//ZERO zero current reading
        //resi: 1.234 on
        //resi: 1.345 off
        //resi? return resi: 1.234 on|off -1
        //curr? return curr: 1.234
        private Regex resi_set_mode = new Regex(@"resi:\s+([0-9.Mk]+)\s+(on|off)\s+([-0-9]+)");
        internal void pc_cmd(string cmd)
        {
            Logger.SysLog(cmd);
            if (cmd == "resi?")
            {
                if (processor.bOn)
                    DeviceMgr.Report("resi: " + processor.resistance.ToString() + " on");
                else
                    DeviceMgr.Report("resi: " + processor.resistance.ToString() + " off"); ;
                return;
            }
            if (cmd == "current?")
            {
                DeviceMgr.Report("curr: " + led_current.Value );
                return;
            }
            if (cmd == "ZERO")
            {
                Program.mainwnd.processor.ZeroON();
            }
            if (cmd == "H")
            {
                DeviceMgr.Reset();
            }
            Match m;

            m = resi_set_mode.Match(cmd);
            if (m.Success)
            {
                
                string rvalue = m.Groups[1].ToString();
                int newrange = Int32.Parse(m.Groups[3].ToString());
                if (newrange < -1 || newrange > 3)
                    return;
                    
                if (newrange == -1)
                {
                    int i = 0;
                    for (i = 0; i < Processor.real_title.Length; i++)
                    {
                        if (rvalue == Processor.real_title[i])
                            break;
                    }
                    if (i >= Processor.real_title.Length)
                        return;
                    processor.iRange = -1;
                    processor.iReal = i;
                }
                else
                {
                    Decimal a;
                    if (!Util.TryDecimalParse(m.Groups[1].ToString(), out a))
                        return;
                    processor.iRange = newrange;
                    processor.resistance = a;
                }    
                processor.bOn = (m.Groups[2].ToString() == "on");
            }
            RefreshDisplay(true);            
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
    #endregion
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
}
