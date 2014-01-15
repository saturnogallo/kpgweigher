using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace QJApp
{
    class Utility
    {
        public static string basedir
        {
            get
            {
                return System.Threading.Thread.GetDomain().BaseDirectory;
            }
        }
    }
}
