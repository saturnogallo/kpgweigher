using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
//using System.Linq;
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
        private RectButton[] kbd_btns;
        [DllImport("user32.dll", EntryPoint = "ShowCursor", CharSet = CharSet.Auto)]

        public static extern int ShowCursor(int bShow);
        private StringBuilder data;
        public static string s_scale = "300";
        public Form1()
        {

            
            InitializeComponent();
            s_scale = this.label1.Text.Substring(0, this.label1.Text.IndexOf('A'));
            this.BackColor = Color.LightSkyBlue;
            led_current.ColorDark = this.BackColor;
            led_setting.ColorDark = this.BackColor;
            btn_turnon.BackColor = this.BackColor;
            rectMeter1.BackColor = this.BackColor;
            btn_turnon.BackColor = this.BackColor;
            rectMeter1.BgResId = "BGMETER";
            //ShowCursor(0);
            processor = new Processor();
            data = new StringBuilder(10);   
            kbd_btns = new RectButton[]{ lbButton0,lbButton1,lbButton2,lbButton3,lbButton4,lbButton5,lbButton6,lbButton7,lbButton8,lbButton9,
                lbButtonCancel,lbButtonOK,lbButtonPT,lbButtonPercent};
            string[] btn_cap = "0,1,2,3,4,5,6,7,8,9,取消,确定,.,%".Split(new char[] { ',' });
            for (int i = 0; i < kbd_btns.Length; i++)
            {
                RectButton lbt = kbd_btns[i];
                lbt.BackColor = Color.Transparent;
                lbt.colorTop = Color.LightPink;
                lbt.colorShadow = Color.DarkGray;
                lbt.Label = btn_cap[i];
                lbt.Style = MyButtonType.raiseButton;
                lbt.Click += new EventHandler((o, e) =>
                {
                        KeypadTick(NameInArray(o, kbd_btns));
                });
            }
            #region (NO USE) digi up and down
            /*
            digi_upbtns = new RectButton[] { rbtn_1up, rbtn_2up, rbtn_3up, rbtn_4up, rbtn_5up, rbtn_6up  };
            digi_dnbtns = new RectButton[] { rbtn_1dn, rbtn_2dn, rbtn_3dn, rbtn_4dn, rbtn_5dn, rbtn_6dn  };
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
                rb.ValidClick += new EventHandler((o, e) =>
                {
                    if (processor.iRange >= 0)
                    {
                        TickDigit(digi_upbtns.Length - NameInArray(o, digi_dnbtns), false);
                        RefreshDisplay(false);
                    }
                });
            }
             * */
            #endregion

            dlg_choice = new ChoiceWnd();
            dlg_kbd = new kbdWnd();
            led_setting.ColorLight = Color.Red;
            led_setting.ColorBackground = this.BackColor;
            led_setting.ElementWidth = 12;
//            led_setting.RecreateSegments(led_setting.ArrayCount);
            led_current.ColorLight = Color.DarkGreen;
            led_current.ColorBackground = this.BackColor;
            led_current.ElementWidth = 10;
//          led_current.RecreateSegments(led_current.ArrayCount);
            led_current.Value = "0.0000";
            btn_zeroon.Style = MyButtonType.rectButton;
            btn_zeroon.colorShadow = this.BackColor;
            btn_zeroon.colorTop = Color.Bisque;
            btn_zeroon.Label = "电流表清零";
            btn_zeroon.ValidClick += new EventHandler((o, e) =>
            {
                led_current.Value = "     ";
                processor.ZeroON();
            });

            //btn_turnon.bgColor = this.BackColor;
            //btn_turnon.SetStyle(Color.Green, MyButtonType.round2Button);
            btn_turnon.Label = "OFF";
            btn_turnon.Click += new EventHandler((o, e) =>
            {
                if (!processor.bOn)
                    dt_lastoutput = DateTime.Now.AddSeconds(0.5);
                processor.bOn = !processor.bOn;
                RefreshDisplay(false);
            });

            tm = new Timer();
            tm.Interval = 1000;
            tm.Tick += new EventHandler((o, e) =>
            {
                lbl_datetime.Text = DateTime.Now.ToString("yyyy-MM-dd hh:mm:ss");
                /*
                if (DateTime.Now.Subtract(dt_lastoutput).TotalSeconds < 1)
                {
                    dt_lastoutput = DateTime.Now;
                    return;
                }*/
                if (data.Length == 0)
                {
                    processor.RefreshOutput();
                    if (processor.Current > -999)
                    {
                        UpdateCurrent(processor.Current);
                        processor.Current = -9999;
                    }
                }
            });
            tm.Enabled = true;
            tm.Start();
            DeviceMgr.Reset();
            
            RefreshDisplay(true);
        }
        private DateTime dt_lastoutput = DateTime.Now;
        private void CancelInput()
        {
            data.Remove(0, data.Length);
            led_setting.ColorLight = Color.Red;
            RefreshDisplay(false);
        }
        private void KeypadTick(int c)
        {
            //10 cancel, 11 ok, 12 pt, 13 percent
            if (c == 10)
            {
                CancelInput();
                return;
            }
            if (c == 13) //percent
            {
                try
                {

                    string n = (Decimal.Parse(data.ToString()) * 3).ToString();
                    data.Remove(0, data.Length);
                    data.Append(n);
                    c = 11;
                }
                catch
                {
                }
            }

            if (c == 11)
            {
                KbdData("value", data.ToString());
                CancelInput();

                return;
            }

            if (data.ToString().IndexOf(".") >= 0)
            {
                if (data.Length > 8)
                    return;
            }
            else
            {
                if (data.Length > 7)
                    return;
            }
            if (data.Length == 0) //newly input value
            {
                led_setting.ColorLight = Color.DarkMagenta;
            }

            if (c >= 0 && c <= 9)
            {
                data.Append(("0123456789")[c]);
            }

            if (c == 12)
            {
                if (data.ToString().IndexOf(".") >= 0)
                {
                    return;
                }
                data.Append('.');
            }
            led_setting.Value = data.ToString();
        }
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
        public delegate void VoidDelegate();
        private void Alert()
        {
            if(Form1.s_scale == "300")
                Program.MsgShow("输入值超出范围 (0-400 A)");
        }

        private void KbdData(string id, string param)
        {
                    Decimal a;
                    if (id == "value")
                    {
                        if (param == "5555555")
                        {
                            Process.GetCurrentProcess().Kill();
                            return;
                        }
                        if (!Util.TryDecimalParse(param, out a))
                            return;

                        if (!IsValidCurrent(a))  //range check
                        {
                            this.Invoke(new VoidDelegate(this.Alert));
                            return;
                        }
                        processor.setting = a;
                        if (Form1.s_scale == "300")
                            rectMeter1.Angle = Convert.ToInt32(Math.Floor( Convert.ToDouble(a) * 180 / 400.0));
                        RefreshDisplay(true);
                    }
        }
        #region (NO USE) tick button
        /*
        private RectButton[] digi_dnbtns;
        private RectButton[] digi_upbtns;
        
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
        */
        #endregion
        public void UpdateCurrent(double reading)
        {
            string newcurr = reading.ToString("F4");
            if (newcurr != led_current.Value)
            {
                led_current.Value = newcurr;
                if (Form1.s_scale == "300")
                    rectMeter1.Angle = Convert.ToInt32(Math.Floor(reading * 180 / 400.0));
            }
        }
        private void RefreshDisplay(bool bRangeChange)
        {
            if (processor.bOn)
            {
                btn_turnon.Label = "ON";
                btn_turnon.colorTop = Color.Green;
                //btn_turnon.bOn = true;
            }
            else
            {
                btn_turnon.Label = "OFF";
                btn_turnon.colorTop = Color.LightGray;
                //btn_turnon.bOn = false;
            }
            led_setting.Value = processor.setting.ToString("F4");
        }
        //PC side command
        //curr: 1.234 on
        //curr: 1.345 off
        //setting? return setting: 1.234 on|off
        //curr? return curr: 1.234
  
        public static bool IsValidCurrent(Decimal a)
        {
            if (Form1.s_scale == "300")
                return !(a < 0 || a > 400);
            return false;
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
