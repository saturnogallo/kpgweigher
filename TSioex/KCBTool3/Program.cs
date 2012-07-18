using System;
using System.Linq;
using System.Collections.Generic;
using System.Windows.Forms;

namespace KCBTool3
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [MTAThread]
        static void Main()
        {
            Application.Run(new Form1());

        }
        public static void MsgShow(string line)
        {
            MessageBox.Show(line);
        }
    }
}