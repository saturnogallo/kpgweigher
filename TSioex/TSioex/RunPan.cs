using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
namespace TSioex
{
 
   public enum PanStatus
    {
        IDLE,
        RELEASE,
        LOST,
        DISBALED,
        ERROR
    }
    public delegate void PanClickHandler(int indexer);
    public partial class RunPan : UserControl
    {
        private Point center;
        private Font idfont;
        private int vibradius;
        private int innradius;
        private int outradius;
        private int radius;
        private int pannum = 0;
        public PanClickHandler handler;
        private bool bSelectable;
        public RunPan()
        {
            InitializeComponent();
            
            this.Click += new EventHandler(RunPan_Click);
            pannum = 0;

        }
        private System.Drawing.Bitmap backBuffer;
        private Graphics graphic;
        public void Init(int nodenum, bool bSelect)
        {
            backBuffer = new Bitmap(this.Width, this.Height);
            graphic = Graphics.FromImage(backBuffer);
            pannum = nodenum;
            bSelectable = bSelect;
            radius = this.ClientSize.Width;
            if (radius > this.ClientSize.Height)
                radius = this.ClientSize.Height;

            radius = radius / 2;
            innradius = radius / 3;
            vibradius = radius / 5;
            outradius = radius *3 / 5;
            center = new Point(radius, radius);

            node_texts = new string[nodenum];
            node_stats = new PanStatus[nodenum];
            for (int i = 0; i < nodenum; i++)
            {
                node_texts[i] = "";
                node_stats[i] = PanStatus.IDLE;
            }
        }

        void RunPan_Click(object sender, EventArgs e)
        {
            if (handler is PanClickHandler)
            {
                Point p = this.PointToClient(System.Windows.Forms.Control.MousePosition);
                for (int i = 0; i < pannum; i++)
                {
                    if (CheckMouseHover(p.X, p.Y, i))
                    {
                        if (bSelectable)
                            idfocus = pannum - i - 1;
                        handler(pannum - i);
                }
            }
        }
        }
        public int idfocus = 0;
        private void DrawPie(Graphics graphics, Brush c, double r, int index)
        {
            Point[] pts = new Point[26];
            pts[25] = center;
            for (int i = 0; i < 25; i++)
            {
                double angle = index*1.0 /pannum + i*1.0/(pannum*24);
                pts[i].Y = Convert.ToInt32(r * Math.Sin((2 * Math.PI) * angle));
                pts[i].X = Convert.ToInt32(r * Math.Cos((2 * Math.PI) * angle));
                pts[i].Offset(center.X, center.Y);
            }
            graphics.FillPolygon(c, pts);
            
            if (bSelectable && ((idfocus == index) || bSelectAll))
                graphics.DrawPolygon(myWhitePen, pts);
            else
                graphics.DrawPolygon(myBlackPen, pts);
        }
        public bool bSelectAll = false;
        private string[] node_texts;
        private PanStatus[] node_stats;
        private PanStatus pack_stat;
        public void SetStatus(string item, byte node, PanStatus status)
        {
            if (item == "weightbar" && node <= pannum)
            {
                node_stats[node-1] = status;
            }
            if (item == "passbar" && node <= pannum)
            {
                node_stats[node-1] = status;
            }
            if (item == "packer" && node <= pannum)
            {
                pack_stat = status;
            }
            this.Invalidate();
        }
        public void SetText(byte node, string text)
        {
            if (node <= pannum)
                node_texts[node-1] = text;
            this.Invalidate();
        }
        private SolidBrush myYellow = new SolidBrush(Color.Yellow);
        private SolidBrush myBlack = new SolidBrush(Color.Black);
        private SolidBrush myBlue = new SolidBrush(Color.Blue);
        private SolidBrush myRed = new SolidBrush(Color.Red);
        private SolidBrush myDarkRed = new SolidBrush(Color.DarkRed);
        private SolidBrush myGreen = new SolidBrush(Color.Green);
        private SolidBrush myWhite = new SolidBrush(Color.White);
        private SolidBrush myPurple = new SolidBrush(Color.Purple);
        private Pen myWhitePen = new Pen(Color.White, 2);
        private Pen myBlackPen = new Pen(Color.Black, 2);
        private void PaintOne(Graphics graphics, int i)
        {
            
            if (node_stats[i] == PanStatus.DISBALED || node_stats[i] == PanStatus.LOST)
            {
                DrawPie(graphics, myBlack, radius, i);
            }
            if (node_stats[i] == PanStatus.RELEASE)
            {
                DrawPie(graphics, myWhite, radius, i);
            }
            if (node_stats[i] == PanStatus.IDLE || node_stats[i] == PanStatus.ERROR)
            {
                DrawPie(graphics, myBlue, radius, i);
            }
            if (node_stats[i] == PanStatus.ERROR)
                DrawPie(graphics, myDarkRed, outradius, i);
            else
                DrawPie(graphics, myGreen, outradius, i);

            DrawPie(graphics, myYellow, innradius, i);
            
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            
            if (pannum == 0)
                return;

            graphic.FillRectangle(new SolidBrush(SingleModeWnd.bgWindow),this.ClientRectangle);
            //base.OnPaint(e);
            graphic.DrawEllipse(myBlackPen, 0, 0, 2 * radius, 2 * radius);
            for (int i = 0; i < pannum; i++)
            {
                if (i == idfocus)
                    continue;
                PaintOne(graphic,i);
                DrawText(graphic, i);
                DrawIDText(graphic, i);
            }
            PaintOne(graphic,idfocus);
            DrawText(graphic, idfocus);
            DrawIDText(graphic, idfocus);
            
            
            Rectangle inner = new Rectangle(center.X - vibradius, center.Y - vibradius, 1 + vibradius * 2, 1 + vibradius * 2 );
            graphic.DrawEllipse(myBlackPen, inner);
            inner.Inflate(-1, -1);
            if (pack_stat == PanStatus.DISBALED || pack_stat == PanStatus.LOST)
            {
                graphic.FillEllipse(myBlack, inner) ;
            }
            if (pack_stat == PanStatus.RELEASE)
            {
                graphic.FillEllipse(myWhite, inner);
            }
            if (pack_stat == PanStatus.IDLE || pack_stat == PanStatus.ERROR)
            {
                graphic.FillEllipse(myPurple, inner);
            }
            e.Graphics.DrawImage(backBuffer, 0, 0);
        }
        protected override void OnPaintBackground(PaintEventArgs e)
        {
            /*
            Graphics graphics = e.Graphics;
            graphics.FillRectangle(new SolidBrush(SingleModeWnd.bgWindow), ClientRectangle);
             */
            //base.OnPaintBackground(e);
        }
        /// <summary> 
        /// 检测释放发生鼠标悬停状态发生改变,若发生改变则重写绘制控件 
        /// </summary> 
        /// <param name="x">测试点X坐标</param> 
        /// <param name="y">测试点Y坐标</param> 
        /// <returns>测试点是否在椭圆区域中</returns> 
        private bool CheckMouseHover(int x, int y, int index)
        {
            //index = pannum + 1 - index;
            Point p = new Point(x, y);
            //p = base.PointToClient(p);
            
            double dist = (p.X - radius) * (p.X - radius) + (p.Y - radius) * (p.Y - radius);
            
            bool posy = (p.Y - radius) > 0;
            double ag = Math.Acos((p.X - radius)/Math.Sqrt(dist))*360/(2*Math.PI);
            if(posy)
                ag = 360 - ag;
            if (dist < (outradius * outradius) && dist > (innradius * innradius ))
            {
                if (ag > (index * 360.0 / pannum) && ag < ((index + 1) * 360.0 / pannum))
                    return true;
            }
            return false;
        }
        private void DrawIDText(Graphics graphic, int index)
        {
            int textw = 20;
            int texth = 5;
                double angle = index * 1.0 / pannum;
                double angle1 = (index + 1) * 1.0 / pannum;
                double r = innradius * 4.1 / 5;
                int Y = Convert.ToInt32(r * Math.Sin(angle * (2 * Math.PI))) + center.Y;
                int X = Convert.ToInt32(r * Math.Cos(angle * (2 * Math.PI))) + center.X;
                int Y1 = Convert.ToInt32(r * Math.Sin(angle1 * (2 * Math.PI))) + center.Y;
                int X1 = Convert.ToInt32(r * Math.Cos(angle1 * (2 * Math.PI))) + center.X;
                RectangleF rect = new RectangleF((X + X1) / 2 - textw, (Y + Y1) / 2 - texth, 2 * textw, 2 * texth);

                // 绘制文本 
                index++;
                using (StringFormat f = new StringFormat())
                {
                    // 水平居中对齐 
                    f.Alignment = System.Drawing.StringAlignment.Center;
                    // 垂直居中对齐 
                    f.LineAlignment = System.Drawing.StringAlignment.Center;
                    // 设置为单行文本 
                    f.FormatFlags = System.Drawing.StringFormatFlags.NoWrap;
                    // 绘制文本 
                    {
                        graphic.DrawString(
                            index.ToString(),
                            this.Font,
                            myBlack,
                            rect,
                            f);
                    }
                }
       
        }
        private void DrawText(Graphics graphic, int index)
        {
            int textw = 30;
            int texth = 5;
            if (node_texts[index] != null)
            {
                double angle = index*1.0  / pannum ;
                double angle1 = (index+1) * 1.0 / pannum ;
                double r = radius * 4.2 / 5;
                int Y = Convert.ToInt32(r * Math.Sin(angle * (2 * Math.PI))) + center.Y;
                int X = Convert.ToInt32(r * Math.Cos(angle * (2 * Math.PI))) + center.X;
                int Y1 = Convert.ToInt32(r * Math.Sin(angle1 * (2 * Math.PI))) + center.Y;
                int X1 = Convert.ToInt32(r * Math.Cos(angle1 * (2 * Math.PI))) + center.X;
                RectangleF rect = new RectangleF((X + X1)/2 - textw , (Y + Y1)/2 - texth, 2*textw, 2*texth);
                
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
                    {
                        graphic.DrawString(
                            node_texts[index],
                            this.Font,
                            myRed,
                            rect,
                            f);
                    }
                }
            }
        }
    }
}
