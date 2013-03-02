using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.IO;
using System.Reflection;
using System.Diagnostics;
using System.Text.RegularExpressions;
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
            /*
                    Assembly assemb = Assembly.LoadFrom(StringResource.udiskdir + @"\tsioex.exe");
                    Version myversion = new Version(Assembly.GetExecutingAssembly().GetName().Version.ToString());
                    Version newversion = new Version(assemb.GetName().Version.ToString());
                    //MessageBox.Show("found "+newversion.ToString() +" curr " + myversion.ToString());
                    if (newversion.ToString().CompareTo(myversion.ToString()) > 0)
                    {
                    }                    
            */
            mainwnd = new RunWnd();
            Application.Run(mainwnd);
        }
        public static void OpenLog(int method)
        {
            string file = StringResource.basedir + "\\run.txt";


            if (File.Exists(file))
            {
                Process app;
                app = new Process();
                app.StartInfo.WorkingDirectory = StringResource.basedir;
                app.StartInfo.FileName = "\\Windows\\Notepad.exe";
                app.StartInfo.Arguments = file;
                app.Start();
                return;
            }
            
        }
        public static void Upgrade()
        {
            if (Directory.Exists(StringResource.udiskdir))
            {
                if (File.Exists(StringResource.udiskdir + @"\QJ55a.exe"))
                {

                    Process app = new Process();
                    app.StartInfo.WorkingDirectory = StringResource.basedir;
                    app.StartInfo.FileName = StringResource.basedir + @"\CEUpgrade.exe";
                    app.StartInfo.Arguments = "\"/from:" + StringResource.udiskdir + "\\QJ55a.exe\" \"/to:" + StringResource.basedir + "\\tsioex.exe\"";
                    app.Start();
                    Process.GetCurrentProcess().Kill();
                    return;
                }
            }

        }


        public static kbdWnd kbd;
        public static MsgDlg msg;
        public static ChoiceWnd choice;
        static SysConfigWnd syswnd;
        public static RxConfigWnd rxwnd;
        public static RsConfigWnd rswnd;
        public static RunWnd mainwnd;

        public static List<RxInfo> lst_rxinfo;
        public static List<RsInfo> lst_rsinfo;

        static Program()
        {
            msg = new MsgDlg();
            kbd = new kbdWnd();
            syswnd = new SysConfigWnd();
            rxwnd = new RxConfigWnd();
            rswnd = new RsConfigWnd();
            choice = new ChoiceWnd();
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
            Program.msg.Init(line);
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