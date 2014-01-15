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

    public partial class GraphPane : UserControl
    {
        private Graphics gHdc;
        private XAxis xAxis;
        private YAxis yAxis;
        private double _xMax;
        [EditorBrowsable(EditorBrowsableState.Always)]
        [DefaultValue(100)]
        public double XAxisMax
        {
            get { return _xMax; }
            set {
                _xMax = value;
                if (_xMax < 10)
                    _xMax = 10;
            }
        }
        private double _yMax;
        [EditorBrowsable(EditorBrowsableState.Always)]
        [DefaultValue(100)]
        public double YAxisMax
        {
            get { return _yMax; }
            set { _yMax = value; }
        }
        private double _yMin;
        [EditorBrowsable(EditorBrowsableState.Always)]
        [DefaultValue(100)]
        public double YAxisMin
        {
            get { return _yMin; }
            set { _yMin = value; }
        }

        private string _xTitle = "";
        [EditorBrowsable(EditorBrowsableState.Always)]
        [DefaultValue("X")]
        public string XAxisTitle
        {
            get { return _xTitle; }
            set { _xTitle = value; }
        }
        private string _yTitle = "Y";
        [EditorBrowsable(EditorBrowsableState.Always)]
        [DefaultValue("Y")]
        public string YAxisTitle
        {
            get { return _yTitle; }
            set { _yTitle = value; }
        }
        private int _xTicCount = 100;
        [EditorBrowsable(EditorBrowsableState.Always)]
        [DefaultValue(100)]
        public int XAxisTicCount
        {
            get { return _xTicCount; }
            set {
                _xTicCount = value;
                if (_xTicCount < 10)
                    _xTicCount = 10;
            }
        }
        private int _yTicCount = 100;
        [EditorBrowsable(EditorBrowsableState.Always)]
        [DefaultValue(100)]
        public int YAxisTicCount
        {
            get { return _yTicCount; }
            set { _yTicCount = value; }
        }
        private Pen _linePen;
        [EditorBrowsable(EditorBrowsableState.Never)]
        public Pen LinePen
        {
            get { return _linePen; }
            set { _linePen = value; }
        }
        private Pen _pointPen;
        [EditorBrowsable(EditorBrowsableState.Never)]
        public Pen PointPen
        {
            get { return _pointPen; }
            set { _pointPen = value; }
        }

        public string[] channels=null;

        private const int ticLength = 2;
        Point origin = new Point(25, 20);
        public GraphPane()
        {
            InitializeComponent();
            this._linePen = new Pen(Color.Gray, 1);
            this._pointPen = new Pen(Color.Orange, 1);
        }
        private void GraphPane_Paint(object sender, PaintEventArgs e)
        {
            Bitmap b = new Bitmap(this.Width, this.Height);
            gHdc = Graphics.FromImage((System.Drawing.Image)b);
            gHdc.FillRectangle(new SolidBrush(Color.White), this.ClientRectangle);
            int yAxisLength = ClientRectangle.Height - 2 * origin.Y;
            yAxis = new YAxis(gHdc, this.ClientRectangle, origin, yAxisLength, _yMax, _yMin, _yTitle);
            yAxis.Draw(_yTicCount, ticLength);
            int xAxisLength = ClientRectangle.Width - 2 * origin.X;
            xAxis = new XAxis(gHdc, this.ClientRectangle,origin , xAxisLength, _xMax, _xTitle);
            xAxis.Draw(_xTicCount, ticLength);
            DrawLines(gHdc);
            e.Graphics.DrawImage((System.Drawing.Image)b, 0, 0);
            b.Dispose();
        }
        
        private double average;
        public static readonly Color[] myColor = { Color.Black, Color.Orange, Color.Blue, Color.Red };
        private void DrawLines(Graphics gHdc)
        {
            Point previous = new Point();
            Point current = new Point();
            if (channels == null || channels.Length <= 0)
                return;
            for (int i = 0; i < channels.Length;i++ )
            {
                PointPen.Color = myColor[i];
                average = Program.data.my_avg(channels[i]);
                int index = -1;
                foreach (PointPair v in Program.data.my_list(channels[i]))
                {
                    index++;
                    if (index == 0)
                    {
                        previous = AxisTransfrom(v.value, index);
                        if (previous.Y < 2)
                            previous.Y = 4;
                        if (previous.Y > (ClientRectangle.Height - origin.Y))
                            previous.Y = (ClientRectangle.Height - origin.Y);
                        continue;
                    }
                    gHdc.DrawEllipse(PointPen, previous.X - 2, previous.Y - 2, 4, 4);

                    current = AxisTransfrom(v.value, index);
                    if (current.Y < 2)
                        current.Y = 4;
                    if (current.Y > (ClientRectangle.Height - origin.Y))
                        current.Y = (ClientRectangle.Height - origin.Y - 2);

                    gHdc.DrawLine(LinePen, previous.X, previous.Y, current.X, current.Y);
                    gHdc.DrawEllipse(PointPen, current.X - 2, current.Y - 2, 4, 4);
                    previous = current;
                }
            }
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            GraphPane_Paint(this, e);
        }
        protected override void OnPaintBackground(PaintEventArgs e)
        {
        }
        private int yOffset
        {
            get
            {
                return ClientRectangle.Height / 2 - origin.Y;
            }
        }
        private Point AxisTransfrom(double trueValue, int index)
        {
            double tempX = ((this.ClientRectangle.Width - 2 * this.origin.X) / this.XAxisMax) * index + origin.X;
            double val = (trueValue-average)*1e6/average;
            double tempY = this.ClientRectangle.Height - (((this.ClientRectangle.Height - 2 * this.origin.Y) / (this.YAxisMax - this.YAxisMin))*val) - origin.Y;
            return new Point((int)tempX, (int)tempY - yOffset);
        }
    }
    public abstract class Axis
    {
        protected internal Pen blackPen;
        protected internal Rectangle rectRegion;
        protected internal Graphics gHdc;
        protected internal Font axisFont;
        protected internal Font ticFont;
        private int _axisLength;
        public int AxisLength
        {
            get { return _axisLength; }
            set { _axisLength = value; }
        }

        private Point _origin;
        public Point Origin
        {
            get { return _origin; }
            set { _origin = value; }
        }
        private double _max;
        public double Max
        {
            get { return _max; }
            set { _max = value; }
        }
        private double _min;
        public double Min
        {
            get { return _min; }
            set { _min = value; }
        }

        private string _title;
        public string Title
        {
            get { return _title; }
            set { _title = value; }
        }
        public Axis(Graphics g, Rectangle rect, Point orgin, int length)
        {
            this.gHdc = g;
            this.rectRegion = rect;
            this._origin = orgin;
            this._axisLength = length;
            this.blackPen = new Pen(Color.Black);
            this.axisFont = new Font(FontFamily.GenericSerif, 10, FontStyle.Bold);
            this.ticFont = new Font(FontFamily.GenericSerif, 8, FontStyle.Italic);
        }
        public Axis(Graphics g, Rectangle rect, Point orgin, int length, double max, double min, string title)
            : this(g, rect, orgin, length)
        {
            this._max = max;
            this._min = min;
            this._title = title;
        }
        public void Draw(int ticCount, int ticLength)
        {
            float step = (float)_axisLength / (float)ticCount;
            
            int length = ticLength;
            DrawBaseLine();
            int scale = ((ticCount-1) / 10) + 1;
            
            for (int i = 0; i <= ticCount; i++)
            {
                length = ticLength;
                if (i % 5 == 0)
                {
                    length = ticLength * 2;
                }

                if ((i % scale) == 0)
                {
                    double tic;
                    if (this is YAxis)
                    {
                        tic = ((_max - _min) / ticCount) * i + _min;
                        tic = Math.Round(tic, 3);
                        length = ticLength * 3;
                        DrawTicTitle((int)(step * i), tic);
                        DrawTic((int)(step * i), length);
                    }else{
                        length = ticLength * 3;
                        int j = (int)((i*(_max - _min)) / ticCount);
                        int tempX = (int)(((this.rectRegion.Width - 2 * this.Origin.X) / this._max) * j + Origin.X);
                        DrawTicTitle(tempX, j);
                        tempX = (int)(((this.rectRegion.Width - 2 * this.Origin.X) / this._max) * j + Origin.X);
                        DrawTic(tempX, length);
                    }
                }
                
            }
            DrawTitle();
        }
        public abstract void DrawTitle();
        public abstract void DrawTic(int pos, int length);
        public abstract void DrawTicTitle(int pos, double tic);
        public abstract void DrawBaseLine();
    }

    class XAxis : Axis
    {
        public XAxis(Graphics g, Rectangle rect, Point orgin, int length, double max, string title)
            : base(g, rect, orgin, length, max, 0, title)
        {
        }
        private int yOffset
        {
            get
            {
                return (rectRegion.Height/2) - Origin.Y;
            }
        }
        public override void DrawTic(int i, int length)
        {
            int x1 = i;
            int y1 = rectRegion.Height - Origin.Y - length - yOffset;
            int x2 = x1;
            int y2 = (rectRegion.Height - Origin.Y) - yOffset;
            gHdc.DrawLine(blackPen, x1, y1, x2, y2);
        }
        public override void DrawTitle()
        {
            gHdc.DrawString(Title, axisFont, new SolidBrush(Color.Black), rectRegion.Width - Origin.X - (int)(Title.Length * 2.5), rectRegion.Height - (int)(Origin.Y * 0.8) - yOffset);
        }
        public override void DrawTicTitle(int tempX, double tic)
        {
            if (Math.Round(tic) > 0)
            {
                string text = Math.Round(tic).ToString();
                
                gHdc.DrawString(text, ticFont, new SolidBrush(Color.Red), tempX- text.Length * 3, rectRegion.Height - Origin.Y - yOffset);
            }
        }
        public override void DrawBaseLine()
        {
            int x1 = Origin.X;
            int y1 = rectRegion.Height - Origin.Y - yOffset;
            int x2 = Origin.X + AxisLength;
            int y2 = y1;
            gHdc.DrawLine(blackPen, x1, y1, x2, y2);
        }
    }

    class YAxis : Axis
    {
        public YAxis(Graphics g, Rectangle rect, Point orgin, int length, double max,double min, string title)
            : base(g, rect, orgin, length, max, min, title)
        {
        }
        public override void DrawTic(int pos, int length)
        {
            int x1 = Origin.X;
            int y1 = rectRegion.Height - (Origin.Y + pos);
            int x2 = x1 - length;
            int y2 = y1;
            gHdc.DrawLine(blackPen, x1, y1, x2, y2);
        }
        public override void DrawTitle()
        {
            gHdc.DrawString(Title, axisFont, new SolidBrush(Color.Black), Origin.X / 6, Origin.Y / 4);
        }
        public override void DrawTicTitle(int pos, double tic)
        {
            gHdc.DrawString(tic.ToString(), ticFont, new SolidBrush(Color.Red), Origin.X / 16, rectRegion.Height - Origin.Y - pos);
        }
        public override void DrawBaseLine()
        {
            int x1 = Origin.X;
            int y1 = rectRegion.Height -Origin.Y;
            int x2 = x1;
            int y2 = y1 - AxisLength;
            gHdc.DrawLine(blackPen, x1, y1, x2, y2);
        }
    }

}

