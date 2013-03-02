using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;
namespace QJAppMini
{
    static class Util
    {
        public static IniHandler ConstIni; 
        static Util()
        {
            ConstIni = new IniHandler(Util.basedir + "\\Const.ini", "");
        }
        static public void TouchFile(string file)
        {
            if (!File.Exists(file))
            {
                try
                {
                    FileStream fsLog = new FileStream(file, FileMode.Create, FileAccess.Write, FileShare.Read);
                    StreamWriter sw = new StreamWriter(fsLog);
                    sw.Close();
                    fsLog.Close();
                }
                catch (System.Exception ex)
                {
                }
            }
        }

        static public void SysLog(string line)
        {
            string fn = GetSysLogFileName();
            try
            {
                if (line == "")
                {
                    if (File.Exists(fn))
                        File.Delete(fn);
                    TouchFile(fn);
                    return;
                }
                FileStream fsLog = new FileStream(fn, FileMode.Append, FileAccess.Write, FileShare.Read);
                using (StreamWriter sw = new StreamWriter(fsLog))
                {
                    sw.WriteLine(DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff") + "\t" + line);
                }
                fsLog.Close();
            }
            catch (System.Exception ex)
            {
            }
        }
        static public string GetSysLogFileName()
        {
            return Util.basedir + "\\run.txt";
        }

        public static string ToYesNo(bool yesno)
        {
            if (yesno)
                return "Yes";
            else
                return "No";
        }
        //scale 0s after 1st one, example if you want 10.000000, scale should be 7
        //return string until  1/10^scale with space
        public static string FormatDataWithSpace(double data, int scale)
        {
            StringBuilder sb = new StringBuilder(FormatData(data, scale));
            int pos = sb.ToString().IndexOf('.') + 4;
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
                return r.ToString("F11").Substring(0, scale + 1);
            }
            if (r < 1e-11)
            {
                return "0.0000000000".Substring(0, scale + 1);
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
        public static string datadir //data folder
        {
            get
            {
                return basedir + "data\\";
            }
        }
        public static string basedir  //exe directory
        {
            get
            {
                return System.Threading.Thread.GetDomain().BaseDirectory;
            }
        }
        public static double my_min(IEnumerable<double> values)
        {
            if (values.Count() < 1)
                return 0;
            return values.Min();
        }
        public static double my_max(IEnumerable<double> values)
        {
            if (values.Count() < 1)
                return 0;
            return values.Max();
        }
        public static double my_sum(IEnumerable<double> values)
        {
            if (values.Count() < 1)
                return 0;
            return values.Sum();
        }
        public static double my_avg(IEnumerable<double> values)
        {
            if (values.Count() < 1)
                return 0;

            return values.Average();
        }
        public static double my_sqrtsum(IEnumerable<double> values)
        {
            if (values.Count() < 2)
                return 0;
            double avg = my_avg(values);
            if (Math.Abs(avg) < 1e-15)
                return 1e-15;
            double sqr = values.Select<double, double>((o) => { return o * (o - avg); }).Sum();
            sqr = Math.Sqrt(sqr / (values.Count() - 1)) / avg;
            return sqr;
        }
        public static double[] to_ppm_array(IEnumerable<double> values)
        {
            if (values.Count() < 2)
                return new double[]{};

            double avg = my_avg(values);
            if (Math.Abs(avg) < 1e-15)
                avg = 1e-15;
            return values.Select<double, double>((o) => { return (o - avg) * 1e6 / avg; }).ToArray();
        }
    }

    public class IniHandler
    {
        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filepath);
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retval, int size, string filepath);
        public string head
        {
            get
            {
                return _head;
            }
            set
            {
                if (value != "")
                    _head = "_" + value;
                else
                    _head = "";
            }
        }
        private string _head; //US or CN
        private string _fn;   //File
        private StringBuilder tmpbuf;
        public IniHandler(string filename, string hd)
        {
            head = hd;
            _fn = filename;
            tmpbuf = new StringBuilder("", 255);

        }
        public void ClearAll()
        {
            if (File.Exists(_fn))
                File.Delete(_fn);
            FileStream fs = File.Create(_fn);
            if (fs is FileStream)
            {
                fs.Close();
                fs.Dispose();
            }
            fs = null;
        }
        public int IntValue(string section, string key)
        {
            try
            {
                GetPrivateProfileString(section + _head, key, "", tmpbuf, 255, _fn);
                return Int32.Parse(tmpbuf.ToString());
            }
            catch //for unavailable key please return 9999
            {
                return 999;
            }
        }
        public string StringValue(string section, string key)
        {
            try
            {
                GetPrivateProfileString(section + _head, key, "", tmpbuf, 255, _fn);
                return tmpbuf.ToString();
            }
            catch //for unavailable key please return 9999
            {
                return "";
            }
        }
        public void WriteString(string section, string key, object value)
        {
            WritePrivateProfileString(section + _head, key, value.ToString(), _fn);
        }
    }

}
