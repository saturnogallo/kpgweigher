using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using Raysting.Controls;
using System.Drawing;
namespace Zddq2
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            
            Application.EnableVisualStyles();
            //Application.SetCompatibleTextRenderingDefault(false);
            mainwnd = new RunWnd();
            Application.Run(mainwnd);
        }

        //Configuration windows Dialog
        public static SysConfigWnd syswnd;
        public static RxConfigWnd rxwnd;
        public static RsConfigWnd rswnd;
        public static MainConfigWnd mcfgwnd;

        public static RunWnd mainwnd;

        /// <summary>
        /// management of all data logging and display
        /// </summary>
        public static DataMgr testdata;     
        public static List<RxInfo> lst_rxinfo;
        public static List<RsInfo> lst_rsinfo;
        public static SysConfig sysinfo; 

        static Program()
        {
            sysinfo = new SysConfig();
            testdata = new DataMgr();
            lst_rxinfo = new List<RxInfo>();
            lst_rsinfo = new List<RsInfo>();

            for (int i = 0; i < sysinfo.iRxCnts; i++)
                lst_rxinfo.Add(new RxInfo(i + 1));
                
            for (int i = 0; i < sysinfo.iRsCnts; i++)
                lst_rsinfo.Add(new RsInfo(i + 1));

            syswnd = new SysConfigWnd();
            rxwnd = new RxConfigWnd();
            rswnd = new RsConfigWnd();
            mcfgwnd = new MainConfigWnd();

            
            DeviceMgr.Dummy();
        }

        public static Color BgColor = Color.White;
        public static string curr_wnd;
        public static void SwitchWindow(string wnd)
        {
            curr_wnd = wnd;
            syswnd.Hide();
            rxwnd.Hide();
            rswnd.Hide();
            mcfgwnd.Hide();
            if (wnd == "sysconfig")
            {
                syswnd.InitDisplay();
                syswnd.BringToFront();
                syswnd.Show();
                return;
            }
            if (wnd == "rxconfig")
            {
                rxwnd.InitDisplay();
                rxwnd.BringToFront();
                rxwnd.Show();
                return;
            }
            if (wnd == "rsconfig")
            {
                rswnd.InitDisplay();
                rswnd.BringToFront();
                rswnd.Show();
                return;
            }
            if (wnd == "mainconfig")
            {
                mcfgwnd.InitDisplay();
                mcfgwnd.BringToFront();
                mcfgwnd.Show();
                return;
            }
        }
    }
}
