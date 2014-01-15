using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;
namespace Mndz
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
            Application.SetCompatibleTextRenderingDefault(false);
            mainwnd = new Form1();
            msg = new MsgDlg();
            Application.Run(mainwnd);
        }
        internal static Form1 mainwnd;
        internal static MsgDlg msg;
        internal static void Debug(string line)
        {
            MessageBox.Show(line); //by sojo
        }
        internal static void MsgShow(string line)
        {
            Program.msg.Init(line);
        }
        public static void Upgrade()
        {
            string diskdir = "";
            if (Directory.Exists(GlobalConfig.udiskdir) && File.Exists(GlobalConfig.udiskdir + @"\MNDZ.exe"))
                diskdir = GlobalConfig.udiskdir;
            if (Directory.Exists(GlobalConfig.udiskdir2) && File.Exists(GlobalConfig.udiskdir2 + @"\MNDZ.exe"))
                diskdir = GlobalConfig.udiskdir2;
            if (diskdir != "")
            {

                Process app = new Process();
                app.StartInfo.WorkingDirectory = GlobalConfig.basedir;
                app.StartInfo.FileName = GlobalConfig.basedir + @"\CEUpgrade.exe";
                app.StartInfo.Arguments = "\"/from:" + diskdir + "\\MNDZ.exe\" \"/to:" + GlobalConfig.basedir + "\\tsioex.exe\"";
                app.Start();
                Process.GetCurrentProcess().Kill();
                return;
            }
            

        }

    }
}
