using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Threading;
using System.Text.RegularExpressions;
namespace Jmbo
{
    static class Program
    {
        private static Mutex _appMutex = new Mutex(false, "Jmbo");

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            if (!_appMutex.WaitOne(0, false))
            {
                Application.Exit();
                return;
            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
