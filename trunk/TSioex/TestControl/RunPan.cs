using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace TestControl
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
        private int radius;
        private int innradius;
        private int outradius;
        private int pannum;
        public PanClickHandler handler;
        public RunPan()
        {
            InitializeComponent();
 

            this.Click += new EventHandler(RunPan_Click);
            Init(10);
        }
        public void Init(int nodenum)
        {
            pannum = nodenum;

            radius = this.ClientSize.Width;
            if (radius > this.ClientSize.Height)
                radius = this.ClientSize.Height;
            radius = radius / 2;
            innradius = radius / 3;
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
            Point p = System.Windows.Forms.Control.MousePosition;
            p = base.PointToClient(p);
            for (int i = 0; i < pannum; i++)
            {
                if (CheckMouseHover(p.X, p.Y,i))
                {
                    handler(pannum - i);
                }
            }
        }
        public int idfocus = 0;
        private void DrawPie(Graphics graphics, Brush c, double r, int index)
        {
            Point[] pts = new Point[14];
            pts[13] = center;
            for (int i = 0; i < 13; i++)
            {
                double angle = index*1.0 /pannum + i*1.0/(pannum*12);
                pts[i].Y = Convert.ToInt32(r * Math.Sin((2 * Math.PI) * angle));
                pts[i].X = Convert.ToInt32(r * Math.Cos((2 * Math.PI) * angle));
                pts[i].Offset(center.X, center.Y);
            }
            graphics.FillPolygon(c, pts);
            if ((idfocus == index) || bSelectAll)
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
        private SolidBrush myBlack = new SolidBrush(Color.Black);
        private SolidBrush myBlue = new SolidBrush(Color.Blue);
        private SolidBrush myRed = new SolidBrush(Color.DarkRed);
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
                DrawPie(graphics, myRed, outradius, i);
            else
                DrawPie(graphics, myGreen, outradius, i);
            
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            //base.OnPaint(e);
            Graphics graphics = e.Graphics;
            graphics.DrawEllipse(myBlackPen, 0, 0, 2 * radius, 2 * radius);
            for (int i = 0; i < pannum; i++)
            {
                if (i == idfocus)
                    continue;
                PaintOne(graphics,i);
                DrawText(e, i);
            }
            PaintOne(graphics,idfocus);
            DrawText(e, idfocus);

            Rectangle inner = new Rectangle(center.X - innradius, center.Y - innradius, 1 + innradius * 2, 1 + innradius * 2 );
            graphics.DrawEllipse(myBlackPen, inner);
            inner.Inflate(-1, -1);
            if (pack_stat == PanStatus.DISBALED || pack_stat == PanStatus.LOST)
            {
                graphics.FillEllipse(myBlack, inner) ;
            }
            if (pack_stat == PanStatus.RELEASE)
            {
                graphics.FillEllipse(myWhite, inner);
            }
            if (pack_stat == PanStatus.IDLE || pack_stat == PanStatus.ERROR)
            {
                graphics.FillEllipse(myPurple, inner);
            }
        }
        protected override void OnPaintBackground(PaintEventArgs e)
        {
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
        private void DrawText(PaintEventArgs e, int index)
        {
            int textw = 10;
            int texth = 5;
            if (node_texts[index] != null)
            {
                double angle = index*1.0  / pannum ;
                double angle1 = (index+1) * 1.0 / pannum ;
                double r = radius * 4.0 / 5;
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
                    using (SolidBrush b = new SolidBrush(Color.Red))
                    {
                        e.Graphics.DrawString(
                            node_texts[index],
                            this.Font,
                            b,
                            rect,
                            f);
                    }
                }
            }
        }
    }
}
