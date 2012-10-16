using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Zddq2
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [MTAThread]
        static void Main()
        {
            //Application.SetCompatibleTextRenderingDefault(false);
            mainwnd = new RunWnd();
            Application.Run(mainwnd);
        }

        public static kbdWnd kbd;
        static SysConfigWnd syswnd;
        static RxConfigWnd rxwnd;
        static RsConfigWnd rswnd;
        public static RunWnd mainwnd;

        public static List<RxInfo> lst_rxinfo;
        public static List<RsInfo> lst_rsinfo;


        static Program()
        {
            kbd = new kbdWnd();
            syswnd = new SysConfigWnd();
            rxwnd = new RxConfigWnd();
            rswnd = new RsConfigWnd();
            lst_rxinfo = new List<RxInfo>();
            lst_rsinfo = new List<RsInfo>();
            for (int i = 0; i < 24; i++)
            {
                lst_rxinfo.Add(new RxInfo(i + 1));
                lst_rsinfo.Add(new RsInfo(i + 1));
            }
            DeviceMgr.Dummy();
        }
        public static void DbgShow(string line)
        {
            MessageBox.Show(line);
        }

        public static void MsgShow(string line)
        {
            MessageBox.Show(line);
        }
        public static string curr_wnd;
        public static void SwitchWindow(string wnd)
        {
            curr_wnd = wnd;
            syswnd.Hide();
            rxwnd.Hide();
            rswnd.Hide();
            if (wnd == "sysconfig")
            {
                syswnd.InitDisplay();
                syswnd.BringToFront();
                syswnd.Show();
                return;
            }
            if (wnd == "rxconfig")
            {
                rxwnd.InitDisplay(mainwnd.selectedRx);
                rxwnd.BringToFront();
                rxwnd.Show();
                return;
            }
            if (wnd == "rsconfig")
            {
                rswnd.InitDisplay(mainwnd.selectedRs);
                rswnd.BringToFront();
                rswnd.Show();
                return;
            }
        }
    }
}