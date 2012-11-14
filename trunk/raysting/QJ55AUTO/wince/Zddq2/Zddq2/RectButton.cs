using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Zddq2
{
    public enum MyButtonType
    {
        roundButton,    //simple round button , example: node icon in eng page
        round2Button,    //two cycle round button, example: start button in run mode page
        roundRectButton,//rect button with round corner, example: action button in single window page
        round2RectButton, //rect button with round corner and outer border, example: product setting in single mode page
        roundRect,      //rect area with round corner, example: keyboard background
        roundGradientButton, //rect button with round corner and gradient color, example: button in config 
        invalid
    }
    public partial class RectButton : UserControl
    {
        private MyButtonType btnType = MyButtonType.invalid;
        public event EventHandler ValidClick;
        private bool _bOn = true;
        private bool _bEnabled = true;
        private Graphics graphic;
        private System.Drawing.Bitmap backBuffer;

        public bool bEnabled
        {
            get
            {
                return _bEnabled;
            }
            set
            {
                _bEnabled = value;
                this.Invalidate();
            }
        }
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
        private string _text;
        public override string Text
        {
            set
            {
                if (_text != value)
                {
                    base.Text = value;
                    _text = value;
                    this.Invalidate();
                }
            }
            get
            {
                return base.Text;
            }
        }
        public Color bgColor = Color.White;
        public RectButton()
        {
            InitializeComponent();
            this.MouseUp += new MouseEventHandler(RectButton_Click);
            btnType = MyButtonType.invalid;
            this.colorTop = Color.LightGreen;
            this.colorBottom = Color.Gray;
        }

        private Color colorTop = Color.LightGreen;
        private Rectangle areaTop;
        private Color colorBottom = Color.Gray;
        private Rectangle areaBottom;
        private Color colorDisable = Color.LightGray;
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
        protected override void OnPaint(PaintEventArgs e)
        {
            if (btnType == MyButtonType.invalid)
                return;
            //base.OnPaint(e);
            graphic.FillRectangle(new SolidBrush(this.bgColor), this.ClientRectangle);
            SolidBrush myTopPen;
            if(bOn)
                myTopPen = new SolidBrush(colorTop);
            else
                myTopPen = new SolidBrush(colorDisable);
            SolidBrush myBotPen = new SolidBrush(colorBottom);

            if (btnType == MyButtonType.round2Button || btnType == MyButtonType.roundButton)
            {
                graphic.FillEllipse(myBotPen, areaBottom);
                graphic.FillEllipse(myTopPen, areaTop);
                DrawText(graphic);
            }

            if (btnType == MyButtonType.round2RectButton || btnType == MyButtonType.roundGradientButton || btnType == MyButtonType.roundRectButton)
            {
                DrawRoundRect(graphic, myBotPen, areaBottom);
                DrawRoundRect(graphic, myTopPen, areaTop);
                DrawText(graphic);
            }
            if (btnType == MyButtonType.roundRect)
            {
                DrawRoundRect(graphic, myTopPen, areaTop);
            }
            myTopPen.Dispose();
            myBotPen.Dispose();
            e.Graphics.DrawImage(backBuffer, 0, 0);
        }
        protected override void OnPaintBackground(PaintEventArgs e)
        {
            Graphics graphics = e.Graphics;
            if(this.Text != null && this.Text != "")
                graphics.FillRectangle(new SolidBrush(bgColor), ClientRectangle);
            else
                graphics.FillRectangle(new SolidBrush(Color.LightPink), ClientRectangle);
            //base.OnPaintBackground(e);
        }
        static private bool bLock = false;
        void RectButton_Click(object sender, MouseEventArgs e)
        {
            if (bLock)
                return;
            bLock = true;
            Point p = System.Windows.Forms.Control.MousePosition;
            p = base.PointToClient(p);
            if (CheckMouseHover(p.X, p.Y))
            {
                if (ValidClick != null)
                {
                if(ValidClick.Target != null)
                    ValidClick(this, e);
                }
            }
            else
            {
                base.OnMouseUp(e);
            }
            bLock = false;
        }
        public void SetStyle(Color tcolor, MyButtonType type)
        {
            int bgScale = 3;
            int bdScale = 3;
            if (btnType == MyButtonType.invalid)
            {
                backBuffer = new Bitmap(this.Width, this.Height);
                graphic = Graphics.FromImage(backBuffer);

            }
            /*            Color tcolor = Color.Red;
                        if (style.IndexOf("LightGreen_") == 0)
                            tcolor = Color.LightGreen;
                        if (style.IndexOf("DarkGreen_") == 0)
                            tcolor = Color.DarkGreen;
                        if (style.IndexOf("Goldenrod_") == 0)
                            tcolor = Color.Goldenrod;
                        if (style.IndexOf("Gray_") == 0)
                            tcolor = Color.Goldenrod;
            */
            btnType = type;
            if (type == MyButtonType.roundRectButton || type == MyButtonType.roundGradientButton)
            {
                colorTop = tcolor;
                areaTop = new Rectangle(0, 0, this.ClientSize.Width - bgScale, this.ClientSize.Height - bgScale);
                colorBottom = Color.Gray;
                areaBottom = new Rectangle(bgScale, bgScale, this.ClientSize.Width - bgScale, this.ClientSize.Height - bgScale);

            }
            if (type == MyButtonType.round2RectButton || type == MyButtonType.roundRect)
            {
                colorTop = tcolor;
                areaTop = new Rectangle(bdScale, bdScale, this.ClientSize.Width - 2 * bdScale, this.ClientSize.Height - 2 * bdScale);
                colorBottom = bgColor;
                areaBottom = this.ClientRectangle;

            }
            if (type == MyButtonType.roundButton)
            {
                colorTop = tcolor;
                int iBorder = this.ClientSize.Width;
                if (iBorder > this.ClientSize.Height)
                    iBorder = this.ClientSize.Height;
                areaTop = new Rectangle(0, 0, iBorder - bgScale, iBorder - bgScale);
                colorBottom = Color.Gray;
                areaBottom = new Rectangle(bgScale, bgScale, iBorder - bgScale, iBorder - bgScale);

            }
            if (type == MyButtonType.round2Button)
            {
                colorTop = tcolor;
                int iBorder = this.ClientSize.Width;
                if (iBorder > this.ClientSize.Height)
                    iBorder = this.ClientSize.Height;
                areaTop = new Rectangle(bdScale, bdScale, iBorder - 2 * bdScale, iBorder - 2 * bdScale);
                colorBottom = bgColor;
                areaBottom = new Rectangle(0, 0, iBorder, iBorder);
            }
            areaTop.Inflate(-1, -1);
            areaBottom.Inflate(-1, -1);

            this.Invalidate();
        }
        /// <summary> 
        /// 检测释放发生鼠标悬停状态发生改变,若发生改变则重写绘制控件 
        /// </summary> 
        /// <param name="x">测试点X坐标</param> 
        /// <param name="y">测试点Y坐标</param> 
        /// <returns>测试点是否在椭圆区域中</returns> 
        private bool CheckMouseHover(int x, int y)
        {
            Point p = new Point(x, y);
            //p = base.PointToClient(p);

            if (btnType == MyButtonType.round2RectButton || btnType == MyButtonType.roundGradientButton || btnType == MyButtonType.roundRectButton)
                return areaTop.Contains(p);
            if (btnType == MyButtonType.roundRect)
                return false;
            int cx = 2 * p.X - areaTop.Width;
            int cy = 2 * p.Y - areaTop.Height;
            return (cx * cx + cy * cy) < areaTop.Width * areaTop.Width;
        }
        private void DrawText(Graphics graphic)
        {
            if (this.Text != null)
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
                            this.Text,
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
