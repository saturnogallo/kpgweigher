using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Zddq2
{
    class Util
    {
        //scale 0s after 1st one, example if you want 10.000000, scale should be 7
        //return string until  1/10^scale with space
        public static string FormatDataWithSpace(double data, int scale) 
        {
            StringBuilder sb = new StringBuilder(FormatData(data, scale));
            int pos = sb.ToString().IndexOf('.')+4;
            while (pos < sb.Length)
            {
                sb.Insert(pos, " ");
                pos = pos + 4;
            }
            return sb.ToString();
            
        }
           //return data until  1/10^scale
            public static string FormatData(double data, int scale) 
            {
                double r = Math.Abs(data);
                if ((r < Math.Exp(-scale)) && (r > 1))
                {
                    return r.ToString("F11").Substring(0,scale+1);
                }
                if (r < 1e-11)
                {
                    return r.ToString("F11");
                }
                bool sign = (data < 0);
                double rng = 5e+12;
                int pos = 13;
                while (r < rng)
                {
                    rng = rng / 10;
                    pos = pos - 1;
                }
                pos = pos - scale;
                if (pos < 0)
                {
                    pos = -pos;
                    return data.ToString("F" + pos.ToString());
                }
                else
                {
                    return data.ToString("F1");
                }
            }
            public static void Test()
            {
                //string abc = FormatDat(10, 9);
            }

    }
}
