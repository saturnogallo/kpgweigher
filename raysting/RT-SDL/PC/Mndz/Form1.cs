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
using DmitryBrant.CustomControls;
namespace Mndz
{
    public partial class Form1 : Form
    {
        private Processor processor;
        private ChoiceWnd dlg_choice;
        private kbdWnd dlg_kbd;
        private System.Windows.Forms.Timer tm;
        [DllImport("user32.dll", EntryPoint = "ShowCursor", CharSet = CharSet.Auto)]

        public static extern int ShowCursor(int bShow);

        public Form1()
        {
            InitializeComponent();
            //ShowCursor(0);
            processor = new Processor();
            
            dlg_choice = new ChoiceWnd();
            dlg_kbd = new kbdWnd();
            led_rx.Value = "0.000000";
            led_rx.Click += new EventHandler((o,e)=>{
                dlg_choice.bNo0Choice = true;
                dlg_choice.param = "selectrx";
                dlg_choice.Init(StringResource.str("selectrx"), Processor._RxTitles, -1, null, new KbdDataHandler(KbdData));
            });

            led_rx.ColorLight = Color.Red;
            led_rx.ColorBackground = this.BackColor;
            led_rx.ElementWidth = 12;
            led_rx.RecreateSegments(led_rx.ArrayCount);

            led_rs.ColorLight = Color.Green;
            led_rs.ColorBackground = this.BackColor;
            led_rs.ElementWidth = 10;
            led_rs.RecreateSegments(led_rs.ArrayCount);

            led_vx.ColorLight = Color.Black;
            led_vx.ColorBackground = this.BackColor;
            led_vx.ElementWidth = 10;
            led_vx.RecreateSegments(led_vx.ArrayCount);

            led_vs.ColorLight = Color.Black;
            led_vs.ColorBackground = this.BackColor;
            led_vs.ElementWidth = 10;
            led_vs.RecreateSegments(led_vs.ArrayCount);

            btn_zeroon.BackColor = this.BackColor;
            btn_zeroon.colorTop = Color.Bisque;
            btn_zeroon.Style = MyButtonType.rectButton;
            btn_zeroon.Text = "清零";
            btn_zeroon.ValidClick += new EventHandler((o, e) =>
            {
                processor.ZeroON();
            });

            btn_turnon.BackColor = this.BackColor;
            btn_turnon.colorTop = Color.Green;
            btn_turnon.Style = MyButtonType.roundButton;
            btn_turnon.Text = "OFF";
            btn_turnon.Click += new EventHandler((o, e) =>
            {
                if (!processor.bOn)
                    dt_lastoutput = DateTime.Now.AddSeconds(2);
                processor.bOn = !processor.bOn;
                RefreshDisplay(false);
            });

            rectMeter1.BgResId = "BGMETER";
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
            });
            tm.Enabled = true;
            tm.Start();
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
                                dlg_kbd.Init("请输入DA零位值", "daoffset", false, KbdData);
                            }));
                            return;
                        }
                        if (!Util.TryDecimalParse(param, out a))
                            return;
                        int newrange = -1;//Processor.CheckRange(a, processor.iRange);
                        if (newrange < 0)
                        {
                            this.Invoke(new Action(() => {
                                Program.MsgShow("输入值超出范围");
                            }));
                            return;
                        }
                        RefreshDisplay(true);
                    }
        }
        public void led_ohm_Click(object sender, object e)
        {
                dlg_kbd.Init("请输入模拟电阻值", "value", false, KbdData);
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
        public void UpdateCurrent(double reading)
        {
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
            //TODO update display
        }
        //PC side command
        //resi: 1.234 on
        //resi: 1.345 off
        //resi? return resi: 1.234 on|off
        //curr? return curr: 1.234
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

        }

        private void lbl_range_Click(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void sevenSegmentArray1_Load(object sender, EventArgs e)
        {

        }

        private void rectButton1_Load(object sender, EventArgs e)
        {

        }

        private void btn_turnon_Load(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

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
