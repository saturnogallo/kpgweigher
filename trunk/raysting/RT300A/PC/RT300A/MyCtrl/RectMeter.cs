using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Mndz.MyCtrl
{
    public partial class RectMeter : UserControl
    {
        private Image _bgimg;
        public RectMeter()
        {
            InitializeComponent();
            PointColor = Color.Yellow;
            
            this.Paint += new PaintEventHandler(RectMeter_Paint);
            this.Resize += new EventHandler(RectMeter_Load);
        }

        private Graphics graphic = null;
        private System.Drawing.Bitmap backBuffer = null;

        void RectMeter_Load(object sender, EventArgs e)
        {
            if (backBuffer == null)
            {
                backBuffer = new Bitmap(this.Width, this.Height);
                graphic = Graphics.FromImage(backBuffer);
            }
        }
        private string _bgid = "";
        public string BgResId //resource of the image in background
        {
            get
            {
                return _bgid;
            }
            set
            {
                try
                {
                    _bgimg = (Mndz.Properties.Resources.ResourceManager.GetObject(value) as Image);
                    _bgid = value;
                }
                catch
                {
                    _bgimg = null;
                    _bgid = "";
                }
            }
        }
        private int _angle = 0;
        public int Angle //angle is from 0 to 180 degree;
        {
            get
            {
                return _angle;
            }
            set
            {
                int v = value;
                if (value > 180)
                    v = 180;
                if (value < 0)
                    v = 0;
                if (Math.Abs(_angle - v) < 0.5) //no refresh for 0.5 degree
                    return;
                _angle = v;
                Invalidate();
            }
        }
        private Color _ptcolor;
        public Color PointColor
        {
            get
            {
                return _ptcolor;
            }
            set
            {
                _ptcolor = value;
            }
        }
        private Point[] tria = new Point[3];
        void RectMeter_Paint(object sender, PaintEventArgs e)
        {
            
            if (backBuffer == null)
            {
                return;
            }
            
            SolidBrush myPen;
            
            myPen = new SolidBrush(PointColor);
            graphic.FillRectangle(new SolidBrush(this.BackColor), ClientRectangle);
            if (_bgid != "")
            {
                graphic.DrawImage(_bgimg, 0,0);
            }

            //draw the pointer
            int ofy = 2;
            int Height = ClientRectangle.Height - ofy;
            int Width = ClientRectangle.Width;
            
            double rad = Math.Min(Height, Width/2)*7/8;
            double x = Width/2 - Math.Cos(_angle*Math.PI/180)*rad ;
            double y = Height - Math.Sin(_angle*Math.PI/180)*rad - ofy;

            tria[0].X = Convert.ToInt32(Math.Floor(x));
            tria[0].Y = Convert.ToInt32(Math.Floor(y));

            rad = Height / 25.0; //Convert.ToInt32(Math.Floor((rad * Math.Sin(2 * Math.PI / 180))));
            x =  Width / 2 - Math.Cos((90+_angle) * Math.PI / 180) * rad ;
            y = Height - Math.Sin((90+ _angle) * Math.PI / 180) * rad -ofy;
            tria[1].X  = Convert.ToInt32(Math.Floor(x));
            tria[1].Y = Convert.ToInt32(Math.Floor(y));


            x = Width / 2 + Math.Cos((90+_angle) * Math.PI / 180) * rad ;
            y = Height + Math.Sin((90+ _angle) * Math.PI / 180) * rad - ofy;

            tria[2].X  = Convert.ToInt32(Math.Floor(x));
            tria[2].Y = Convert.ToInt32(Math.Floor(y));


            graphic.FillPolygon(myPen, tria);
            
            int irad = 8;
            graphic.FillEllipse(myPen, Width / 2 - irad, (Height - irad) - ofy, 2 * irad, 2 * irad);
            graphic.DrawLine(new Pen(Color.Red), Width/2, Height -ofy, tria[0].X, tria[0].Y);
            
            myPen.Dispose();
            
            e.Graphics.DrawImage(backBuffer, 0, 0);
        }
    }
}
