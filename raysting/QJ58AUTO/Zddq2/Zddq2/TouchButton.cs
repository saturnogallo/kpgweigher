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
    public partial class TouchButton : UserControl
    {
        public event EventHandler ValidClick;
        public event EventHandler ScrollLeft;
        public event EventHandler ScrollRight;
        public event EventHandler ScrollUp;
        public event EventHandler ScrollDown;
        public const int XMOVE = 30;
        public const int YMOVE = 30;
        protected Graphics graphic;
        protected System.Drawing.Bitmap backBuffer;

        private bool _bEnabled;
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
        private Point m_ptDown;
        public TouchButton()
        {
            InitializeComponent();
            this.MouseDown +=new MouseEventHandler(TouchButton_MouseDown);
            this.MouseUp += new MouseEventHandler(TouchButton_MouseUp);
            m_ptDown = new Point();
            
        }

        void TouchButton_MouseUp(object sender, MouseEventArgs e)
        {
            if (m_ptDown.X == 0 && m_ptDown.Y == 0)
                return;
            if ((Math.Abs(m_ptDown.X - e.X) > XMOVE) && (Math.Abs(m_ptDown.Y - e.Y)*3 < Math.Abs(m_ptDown.X - e.X)))
            {
                if (m_ptDown.X < e.X)
                    ScrollRight(this, e);
                else
                    ScrollLeft(this, e);
            }
            if ((Math.Abs(m_ptDown.X - e.X) * 3 < Math.Abs(m_ptDown.Y - e.Y)) && (Math.Abs(m_ptDown.Y - e.Y) > YMOVE))
            {
                if (m_ptDown.Y < e.Y)
                    ScrollDown(this, e);
                else
                    ScrollUp(this, e);
            }
            if ((Math.Abs(m_ptDown.X - e.X) < XMOVE / 3) && (Math.Abs(m_ptDown.Y - e.Y) < YMOVE / 3))
                ValidClick(this, e);
            m_ptDown.X = 0;
            m_ptDown.Y = 0;
        }

        void TouchButton_MouseDown(object sender, MouseEventArgs e)
        {
            m_ptDown.X = e.X;
            m_ptDown.Y = e.Y;
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            Graphics graphics = e.Graphics;
            graphics.FillRectangle(new SolidBrush(Color.Bisque), ClientRectangle);
        }
        protected override void OnPaintBackground(PaintEventArgs e)
        {
            Graphics graphics = e.Graphics;
            graphics.FillRectangle(new SolidBrush(Color.Bisque), ClientRectangle);
            base.OnPaintBackground(e);
        }
    }
}
