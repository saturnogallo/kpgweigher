using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Mndz
{
    public enum MyButtonType
    {
        roundButton,    //round button with shadow , example: node icon in eng page
        rectButton,     //rect button with round corner and shadow color
        raiseButton,    //rect button with highligh corner and shadow
        triangleupButton, //triangle up button with shadow
        trianglednButton, //triangle down button with shadow
        trianglelfButton, //triangle left button with shadow
        trianglertButton, //triangle right button with shadow

        invalid
    }
    public partial class RectButton : UserControl
    {
        enum btnState
        {
            BUTTON_UP = 0,
            BUTTON_DOWN = 1,
            BUTTON_FOCUSED = 2,
            BUTTON_MOUSE_ENTER = 3,
            BUTTON_DISABLED = 4,
        }
        private btnState _state = btnState.BUTTON_UP;
        private bool mouseEnter = false;

        private MyButtonType btnType = MyButtonType.roundButton;
        public MyButtonType Style
        {
            get
            {
                return btnType;
            }
            set
            {
                btnType = value;
                Invalidate();
            }
        }
        public event EventHandler ValidClick;
        
        
        private Graphics graphic = null;
        private System.Drawing.Bitmap backBuffer = null;

        private bool _bOn = true;
        public bool bOn
        {
            get
            {
                return _bOn;
            }
            set
            {
                _bOn = value;
                this.Invalidate();
            }
        }
        private string _label = null;
        public string Label
        {
            set
            {
                if (_label != value)
                {
                    _label = value;
                    Invalidate();
                }
            }
            get
            {
                return _label;
            }
        }

        private Color colorDisable = Color.LightGray;
        public RectButton()
        {
            InitializeComponent();
            this.MouseDown += new MouseEventHandler(RectButton_MouseDown);
            this.MouseUp += new MouseEventHandler(RectButton_MouseUp);
            //this.MouseEnter += new EventHandler(RectButton_MouseEnter);
            //this.MouseLeave += new EventHandler(RectButton_MouseLeave);
            this.Paint += new PaintEventHandler(RectButton_Paint);
            this.EnabledChanged += new EventHandler(RectButton_EnabledChanged);
            this.GotFocus += new EventHandler(RectButton_GotFocus);
            this.LostFocus += new EventHandler(RectButton_LostFocus);
            this.KeyDown += new KeyEventHandler(RectButton_KeyDown);
            this.KeyUp += new KeyEventHandler(RectButton_KeyUp);
            this.DoubleClick += new EventHandler(RectButton_DoubleClick);
            
            this.Resize += new EventHandler(RectButton_Load);
        }

        void RectButton_DoubleClick(object sender, EventArgs e)
        {
            RectButton_MouseUp(sender, null);
        }

        void RectButton_Load(object sender, EventArgs e)
        {
            if (backBuffer == null)
            {
                backBuffer = new Bitmap(this.Width, this.Height);
                graphic = Graphics.FromImage(backBuffer);
            }
        }

        void RectButton_KeyUp(object sender, KeyEventArgs e)
        {/*
            if (e.KeyData == Keys.Space)
            {
                // still has focus
                _state = btnState.BUTTON_FOCUSED;
                Invalidate();
            }
          */
        }

        void RectButton_KeyDown(object sender, KeyEventArgs e)
        {
            /*
            if (e.KeyData == Keys.Space)
            {
                _state = btnState.BUTTON_DOWN;
                Invalidate();
            }
             */
        }

        void RectButton_LostFocus(object sender, EventArgs e)
        {
            /*
            if (mouseEnter)
            {
                _state = btnState.BUTTON_MOUSE_ENTER;
            }
            else
            {
                _state = btnState.BUTTON_UP;
            }
            Invalidate();
            */
        }

        void RectButton_GotFocus(object sender, EventArgs e)
        {
            /*
            _state = btnState.BUTTON_FOCUSED;
            Invalidate();
             */
        }

        void RectButton_EnabledChanged(object sender, EventArgs e)
        {
            if (Enabled)
            {
                _state = btnState.BUTTON_UP;
            }
            else
            {
                _state = btnState.BUTTON_DISABLED;
            }
            Invalidate();

        }

        void RectButton_MouseDown(object sender, MouseEventArgs e)
        {
            if (!CheckMouseHover())
                return;
            _state = btnState.BUTTON_DOWN;
            Invalidate();
        }
        void RectButton_MouseUp(object sender, MouseEventArgs e)
        {
            if (!CheckMouseHover())
                return;
            _state = btnState.BUTTON_FOCUSED;
            Invalidate();


            if (ValidClick != null)
            {
                if (ValidClick.Target != null)
                {
                    this.Invoke(ValidClick, new object[] { this, e });
                }
            }

        }

        private Rectangle areaTop;
        private Rectangle areaBottom;

    
        private Color _colorTop = Color.Pink;
        public Color colorTop
        {
            get
            {
                return _colorTop;
            }
            set
            {
                _colorTop = value;
                Invalidate();
            }
        }
        private Color _colorShadow = Color.LightGray;
        public Color colorShadow
        {
            get
            {
                return _colorShadow;
            }
            set
            {
                _colorShadow = value;
            }
        }
        private int _bgScale = 3;
        public int bgScale {
            get
            {
                return _bgScale;

            }
            set
            {
                _bgScale = value;
                Invalidate();
            }
        }
        private void DrawRoundTriangle(Graphics g, Brush br, Rectangle area, bool istop)
        {
            int rd = 8;
            rd = (int)Math.Floor(0.5 + (double)area.Width / 30.0);
            if (rd < 8)
                rd = 8;
            

            Point[] trian_pts = new Point[3];
            if (istop)
            {
                g.FillEllipse(br, area.X, area.Y, rd * 2, rd * 2);
                trian_pts[0].X = area.Left;
                trian_pts[0].Y = area.Bottom;
                trian_pts[1].X = area.Right - rd;
                trian_pts[1].Y = area.Top + rd;
                trian_pts[2].X = area.Left ;
                trian_pts[2].Y = area.Top + rd;
                g.FillPolygon(br, trian_pts);

                trian_pts[0].X = area.Left + rd;
                trian_pts[0].Y = area.Bottom -rd;
                trian_pts[1].X = area.Right;
                trian_pts[1].Y = area.Top;
                trian_pts[2].X = area.Left + rd;
                trian_pts[2].Y = area.Top;
                g.FillPolygon(br, trian_pts);
            }
            else
            {
                g.FillEllipse(br, area.Right -rd*2, area.Bottom - rd*2 , rd * 2, rd * 2);
                trian_pts[0].X = area.Left;
                trian_pts[0].Y = area.Bottom;
                trian_pts[1].X = area.Right - rd;
                trian_pts[1].Y = area.Top + rd;
                trian_pts[2].X = area.Right - rd;
                trian_pts[2].Y = area.Bottom;
                g.FillPolygon(br, trian_pts);

                trian_pts[0].X = area.Left + rd;
                trian_pts[0].Y = area.Bottom - rd;
                trian_pts[1].X = area.Right;
                trian_pts[1].Y = area.Top;
                trian_pts[2].X = area.Right;
                trian_pts[2].Y = area.Bottom - rd;
                g.FillPolygon(br, trian_pts);
            }
            

        }

        private void DrawTriangle(Graphics g, Brush br, Rectangle area, bool istop)
        {
            Point[] trian_pts = new Point[3];
            trian_pts[0].X = area.Left;
            trian_pts[0].Y = area.Bottom;
            trian_pts[1].X = area.Right;
            trian_pts[1].Y = area.Top;
            if (istop)
            {
                trian_pts[2].X = area.Left;
                trian_pts[2].Y = area.Top;
            }
            else
            {
                trian_pts[2].X = area.Right;
                trian_pts[2].Y = area.Bottom;
            }
            g.FillPolygon(br, trian_pts);
        }
        private void DrawTriangle(Graphics g, Brush br, Rectangle area, MyButtonType direction)
        {
            Point[] trian_pts = new Point[3];
            trian_pts[0].X = area.Left;
            trian_pts[0].Y = area.Bottom;
            trian_pts[1].X = area.Right;
            trian_pts[1].Y = area.Top;
            trian_pts[2].X = area.Right;
            trian_pts[2].Y = area.Bottom;
            if (direction == MyButtonType.trianglelfButton)
            {
                trian_pts[0].Y = area.Top + area.Height / 2;
            }
            else if (direction == MyButtonType.triangleupButton)
            {
                trian_pts[1].X = area.Left + area.Width / 2;
            }
            else if (direction == MyButtonType.trianglertButton)
            {
                trian_pts[1].X = area.Left;
                trian_pts[2].Y = area.Top + area.Height / 2;
            }
            else if (direction == MyButtonType.trianglednButton)
            {
                trian_pts[0].Y = area.Top;
                trian_pts[2].X = area.Left + area.Width / 2;
            }

            g.FillPolygon(br, trian_pts);
        }
    
        private void DrawRoundRect(Graphics g, Brush br, Rectangle area)
        {
            int rd = 8;
            rd = (int)Math.Floor(0.5 + (double)area.Width / 30.0);
            if (rd < 8)
                rd = 8;
            g.FillEllipse(br, area.X , area.Y , rd * 2, rd * 2);
            g.FillEllipse(br, area.X, area.Y + area.Height - rd * 2, rd * 2, rd * 2);
            g.FillEllipse(br, area.X + area.Width - rd * 2, area.Y, rd * 2, rd * 2);
            g.FillEllipse(br, area.X + area.Width - rd * 2, area.Y + area.Height - rd * 2, rd * 2, rd * 2);
            g.FillRectangle(br, area.X, area.Y + rd, area.Width + 1, area.Height - rd * 2);
            g.FillRectangle(br, area.X + rd, area.Y, area.Width - rd * 2, area.Height + 1);
        }
        void RectButton_Paint(object sender, PaintEventArgs e)
        {
            if (backBuffer == null)
            {
                return;
            }
            graphic.FillRectangle(new SolidBrush(this.BackColor), this.ClientRectangle);
            SolidBrush myTopPen;
            if(bOn)
                myTopPen = new SolidBrush(colorTop);
            else
                myTopPen = new SolidBrush(colorDisable);

            SolidBrush myBotPen = new SolidBrush(colorShadow);

            if (btnType == MyButtonType.trianglednButton || 
                btnType == MyButtonType.trianglelfButton ||
                btnType == MyButtonType.trianglertButton ||
                btnType == MyButtonType.triangleupButton)
            {
                areaTop = new Rectangle(0, 0, this.ClientSize.Width - bgScale, this.ClientSize.Height - bgScale);
                areaBottom = new Rectangle(bgScale, bgScale, this.ClientSize.Width - bgScale, this.ClientSize.Height - bgScale);
                if (_state == btnState.BUTTON_DOWN)
                {
                    DrawTriangle(graphic, new SolidBrush(Color.Black), areaBottom, btnType);
                    DrawTriangle(graphic, myTopPen, areaTop, btnType);
                }else{
                    DrawTriangle(graphic, myBotPen, areaBottom, btnType);
                    DrawTriangle(graphic, myTopPen, areaTop, btnType);
                }
            }
            if (btnType == MyButtonType.roundButton)
            {
                int iBorder = this.ClientSize.Width;
                if (iBorder > this.ClientSize.Height)
                    iBorder = this.ClientSize.Height;
                areaTop = new Rectangle(0, 0, iBorder - bgScale, iBorder - bgScale);
                areaBottom = new Rectangle(bgScale, bgScale, iBorder - bgScale, iBorder - bgScale);

                if (_state == btnState.BUTTON_DOWN)
                {
                    graphic.FillEllipse(myBotPen, areaBottom);
                    graphic.FillEllipse(myBotPen, areaTop);
                }
                else
                {
                    graphic.FillEllipse(myBotPen, areaBottom);
                    graphic.FillEllipse(myTopPen, areaTop);
                }
                DrawText(graphic);
            }

            if (btnType == MyButtonType.rectButton)
            {
                areaTop = new Rectangle(0, 0, this.ClientSize.Width - bgScale, this.ClientSize.Height - bgScale);
                areaBottom = new Rectangle(bgScale, bgScale, this.ClientSize.Width - bgScale, this.ClientSize.Height - bgScale);

                if (_state == btnState.BUTTON_DOWN)
                {
                    DrawRoundRect(graphic, myBotPen, areaBottom);
                    DrawRoundRect(graphic, myBotPen, areaTop);
                }else
                {
                    DrawRoundRect(graphic, myBotPen, areaBottom);
                    DrawRoundRect(graphic, myTopPen, areaTop);
                }
                DrawText(graphic);
            }
            if (btnType == MyButtonType.raiseButton)
            {
                areaTop = new Rectangle(bgScale, bgScale, this.ClientSize.Width - 2*bgScale, this.ClientSize.Height - 2*bgScale);
                areaBottom = new Rectangle(0, 0, this.ClientSize.Width, this.ClientSize.Height);
                if (_state == btnState.BUTTON_DOWN)
                {
                    DrawTriangle(graphic, myBotPen, areaBottom, true);
                    DrawTriangle(graphic, new SolidBrush(Color.White), areaBottom, false);
                }
                else
                {
                    DrawTriangle(graphic, new SolidBrush(Color.White), areaBottom, true);
                    DrawTriangle(graphic, myBotPen, areaBottom, false);
                }
                graphic.FillRectangle(myTopPen,areaTop);
                DrawText(graphic);
            }
            myTopPen.Dispose();
            myBotPen.Dispose();
            e.Graphics.DrawImage(backBuffer, 0, 0);
        }
 
       
        /// <summary> 
        /// 检测释放发生鼠标悬停状态发生改变,若发生改变则重写绘制控件 
        /// </summary> 
        /// <param name="x">测试点X坐标</param> 
        /// <param name="y">测试点Y坐标</param> 
        /// <returns>测试点是否在椭圆区域中</returns> 
        private bool CheckMouseHover()
        {
            Point p = System.Windows.Forms.Control.MousePosition;
            p = base.PointToClient(p);

            if (btnType == MyButtonType.rectButton || btnType == MyButtonType.raiseButton)
            {
                areaTop = new Rectangle(0, 0, this.ClientSize.Width - bgScale, this.ClientSize.Height - bgScale);
                return areaTop.Contains(p);
            }
            if (btnType == MyButtonType.trianglednButton ||
                btnType == MyButtonType.trianglelfButton ||
                btnType == MyButtonType.trianglertButton ||
                btnType == MyButtonType.triangleupButton)
            {
                areaTop = new Rectangle(0, 0, this.ClientSize.Width - bgScale, this.ClientSize.Height - bgScale);
                if (!areaTop.Contains(p))
                    return false;
                if (btnType == MyButtonType.triangleupButton)
                {
                    return ((areaTop.Bottom - p.Y) * areaTop.Width)  < ( 2 * areaTop.Height * Math.Min(Math.Abs(p.X - areaTop.Right), Math.Abs(p.X - areaTop.Left)));
                }
                if (btnType == MyButtonType.trianglednButton)
                {
                    return ((p.Y - areaTop.Top) * areaTop.Width) < (2 * areaTop.Height * Math.Min(Math.Abs(p.X - areaTop.Right), Math.Abs(p.X - areaTop.Left)));
                }
                if (btnType == MyButtonType.trianglertButton)
                {
                    return ((p.X - areaTop.Left) * areaTop.Height) < (2 * areaTop.Width * Math.Min(Math.Abs(p.Y - areaTop.Top), Math.Abs(p.Y - areaTop.Bottom)));
                }
                if (btnType == MyButtonType.trianglelfButton)
                {
                    return ((areaTop.Right - p.X) * areaTop.Height) < (2 * areaTop.Width * Math.Min(Math.Abs(p.Y - areaTop.Top), Math.Abs(p.Y - areaTop.Bottom)));
                }
            }
            if (btnType == MyButtonType.roundButton)
            {
                int iBorder = this.ClientSize.Width;
                if (iBorder > this.ClientSize.Height)
                    iBorder = this.ClientSize.Height;
                areaTop = new Rectangle(0, 0, iBorder - bgScale, iBorder - bgScale);

                int cx = 2 * p.X - areaTop.Width;
                int cy = 2 * p.Y - areaTop.Height;
                return (cx * cx + cy * cy) < areaTop.Width * areaTop.Width;
            }
            return false;
        }
        private void DrawText(Graphics graphic)
        {
            if (_label != null)
            {
                // 绘制文本 
                using (StringFormat f = new StringFormat())
                {
                    // 水平居中对齐 
                    f.Alignment = System.Drawing.StringAlignment.Center;
                    // 垂直居中对齐 
                    f.LineAlignment = System.Drawing.StringAlignment.Center;
                    // 设置为单行文本 
                    f.FormatFlags = System.Drawing.StringFormatFlags.NoWrap;
                    // 绘制文本 
                    using (SolidBrush b = new SolidBrush(this.ForeColor))
                    {
                        graphic.DrawString(
                            _label,
                            this.Font,
                            b,
                            new System.Drawing.RectangleF(
                                areaTop.X,
                                areaTop.Y,
                                areaTop.Width,
                                areaTop.Height),
                            f);
                    }
                }
            }

        }
    }
}
