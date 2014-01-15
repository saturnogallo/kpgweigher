using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Runtime.InteropServices;

namespace Zddq2
{
    public partial class StatusBar : UserControl
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct SystemTime
        {
            public ushort wYear;
            public ushort wMonth;
            public ushort wDayOfWeek;
            public ushort wDay;
            public ushort wHour;
            public ushort wMinute;
            public ushort wSecond;
            public ushort wMiliseconds;
        }
        [DllImport("coredll")]
        public static extern bool SetLocalTime(ref SystemTime sysTime); //设置本地时

        public event EventHandler MenuClick;
        public event EventHandler PauseResumeClick;
        public event EventHandler StartStopClick;
        private System.Windows.Forms.Timer uiTimer;
        private RUN_STATE _status;
        private const int progress_width = 650;

        private bool _pause;
        public bool pause
        {
            get
            {
                return _pause;
            }
            set
            {
                _pause = value;
                if (_pause)
                {
                    bt_menu.Text = StringResource.str("resume");
                    bt_run.Text = StringResource.str("abort");
                }
                else
                    bt_menu.Text = StringResource.str("pause");
            }
        }
        public RUN_STATE status
        {
            get
            {
                return _status;
            }
            set
            {
                _status = value;
                UpdateText();
            }
        }
        private int _waiting;
        public int waiting
        {
            get
            {
                return _waiting;
            }
            set
            {
                _waiting = value;
                UpdateText();
            }

        }
        public string message
        {
            get;
            set;
        }
        public int total
        {
            get;
            set;
        }
        private int _count = 0;
        public int count
        {
            get{
                return _count;
            }
            set
            {
                _count = value;
            }
        }
        public void KbdData(string param, string data)
        {
            try
            {
                this.Invalidate();
            }
            catch (System.Exception e)
            {
                return;
            }
        }

        private void UpdateText()
        {
            if (_status == RUN_STATE.IDLE)
            {
                bt_run.Text = StringResource.str("start");
                bt_menu.Text = StringResource.str("menu");
                bt_run.BackColor = Color.White;
                bt_run.Width = progress_width;
            }
            if (_status == RUN_STATE.ERROR)
            {
                bt_run.Text = StringResource.str("error") + message;
                bt_menu.Text = StringResource.str("menu");
                bt_run.BackColor = Color.MediumVioletRed;
                bt_run.Width = progress_width;
            }
            if (_status == RUN_STATE.DISABLED)
            {
                bt_run.Text = StringResource.str("initialize");
                bt_menu.Text = StringResource.str("menu");
                bt_run.BackColor = Color.LightGray;
                bt_run.Width = progress_width;
            }
            if (_status == RUN_STATE.INITIALIZING)
            {
                bt_run.Text = String.Format("{0}      {1}s",StringResource.str("initialize"),waiting);
                bt_run.BackColor = Color.LightYellow;
            }
            if (_status == RUN_STATE.STOPPING)
            {
                bt_run.Text = StringResource.str("stopping");
                bt_menu.Text = StringResource.str("menu");
                bt_run.BackColor = Color.LightYellow;
                bt_run.Width = progress_width;
            }

            if (_status == RUN_STATE.SEARCHING)
            {
                bt_run.Text = String.Format("{0}      {1}s",StringResource.str("searching"),waiting);
                bt_run.BackColor = Color.LightYellow;
            }
            if (_status == RUN_STATE.READING)
            {
                bt_run.Text = String.Format("{0}      {1}s", StringResource.str("reading"), waiting);
                bt_run.BackColor = Color.LightYellow;
            }

            if (_status == RUN_STATE.TRACKING)
            {
                bt_run.Text = String.Format("{0}  ({1}/{2})     {3}s", StringResource.str("tracking"), count, total, waiting);
                bt_run.BackColor = Color.LightYellow;
            }
            if ((_status == RUN_STATE.TRACKING) || (_status == RUN_STATE.SEARCHING) || (_status == RUN_STATE.INITIALIZING))
            {
//                if (total > 0)
//                    this.bt_run.Width = _count * (progress_width - 140) / total + 140;
//                else
//                    this.bt_run.Width = progress_width;
            }
        }
        public StatusBar()
        {
            InitializeComponent();
            this.BackColor = Color.Black;
            uiTimer = new System.Windows.Forms.Timer();
            uiTimer.Tick += new EventHandler(uiTimer_Tick);
            uiTimer.Interval = 1000; //250ms for UI update
            uiTimer.Enabled = true;
            bt_menu.bgColor = Color.Black;
            bt_menu.SetStyle(Color.LightBlue, MyButtonType.round2RectButton);
            bt_menu.Text = StringResource.str("menu");
            bt_menu.ValidClick += new EventHandler(bt_menu_ValidClick);
            bt_run.Click += new EventHandler(bt_run_Click);
            this.Click += new EventHandler(StatusBar_DoubleClick);
        }

        void StatusBar_DoubleClick(object sender, EventArgs e)
        {
            if (this.lbl_datetime.Bounds.Contains(MousePosition.X, MousePosition.Y))
            {
                
            }
        }
        void bt_run_Click(object sender, EventArgs e)
        {
            StartStopClick(sender, e);
        }

        void bt_menu_ValidClick(object sender, EventArgs e)
        {
            if(bt_menu.Text == StringResource.str("menu"))
                MenuClick(sender, e);
            else
                PauseResumeClick(sender, e);
        }
        void uiTimer_Tick(object sender, EventArgs e)
        {
            lbl_datetime.Text = DateTime.Now.ToString("HH:mm");
        }
    }
}
