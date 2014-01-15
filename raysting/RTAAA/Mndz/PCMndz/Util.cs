using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Forms;
namespace Mndz
{
    class Util
    {
        public static IniHandler ConstIni; 
        static Util()
        {
            ConstIni = new IniHandler(Path.Combine(GlobalConfig.basedir,"define.ini"), "");
        }
        public static bool TryDoubleParse(string val, out double result)
        {
            try
            {
                result = Double.Parse(val);
                return true;
            }
            catch
            {
                result = 0;
                return false;
            }
        }
        public static bool TryDecimalParse(string val, out Decimal result)
        {
            try
            {
                result = Decimal.Parse(val);
                return true;
            }
            catch
            {
                result = new Decimal(0);
                return false;
            }
        }
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
                
                   return Int32.Parse(StringValue(section,key));
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
                StreamReader sr = File.OpenText(_fn);
                if (sr is StreamReader)
                {
                    while (!sr.EndOfStream)
                    {
                        string line = sr.ReadLine();
                        if (line.StartsWith(key + "="))
                        {
                            sr.Close();
                            return line.Remove(0, (key + "=").Length);
                        }
                    }
                }
                //GetPrivateProfileString(section + _head, key, "", tmpbuf, 255, _fn);
                //return tmpbuf.ToString();
                return "";
            }
            catch //for unavailable key please return 9999
            {
                return "";
            }
        }
        public void WriteString(string section, string key, object value)
        {
            //WritePrivateProfileString(section + _head, key, value.ToString(), _fn);
            try
            {
                string newfile = _fn + ".bak";
                if (File.Exists(newfile))
                    File.Delete(newfile);
                StreamWriter sw = new StreamWriter(_fn + ".bak");
                if (sw is StreamWriter)
                {
                    StreamReader sr = File.OpenText(_fn);
                    if (sr is StreamReader)
                    {
                        while (!sr.EndOfStream)
                        {
                            string line = sr.ReadLine();
                            if (line.StartsWith(key + "="))
                            {
                                sw.WriteLine(key + "=" + value);
                            }
                            else
                            {
                                sw.WriteLine(line);
                            }
                        }
                        sr.Close();
                    }
                    sw.Close();
                }
                File.Delete(_fn);
                File.Move(newfile, _fn);
            }
            catch
            {
            }
        }
    }
    internal class Logger
    {
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
                    SysLog(ex.Message + ";" + ex.StackTrace);
                }
            }
        }
        static public string GetLogFileName()
        {
            DateTime dt = DateTime.Now;
            string folder = Path.Combine(GlobalConfig.basedir,dt.ToString("yyyy_MM"));
            string file = folder + "\\" + DateTime.Now.ToString("yyyy_MM_dd") + ".txt";
            if (!Directory.Exists(folder))
                Directory.CreateDirectory(folder);
            TouchFile(file);
            return file;
        }
        static public void SysLog(string line)
        {
            string fn = GlobalConfig.system_log;
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
                MessageBox.Show("写入系统日志失败:"+ex.Message);    
            }
        }
        static public void Log(string line)
        {
            string fn = GetLogFileName();
            try
            {
                FileStream fsLog = new FileStream(fn, FileMode.Append, FileAccess.Write, FileShare.Read);
                using (StreamWriter sw = new StreamWriter(fsLog))
                {
                    sw.WriteLine(DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff") + "\t" + line);
                }
                fsLog.Close();
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("写入日志文件失败:" + ex.Message);    
            }
        }

    }
    internal static class GlobalConfig
    {

        public static bool ISDEBUG = false;
        public static string sSwiPort = "COM5";
        public static string sCmdPort = "COM4";
        public static string sADPort = "COM99";
        public static string sNavmeter = "PZ158";
        public static string udiskdir = @"c:\\Code\\GoogleCode\\g_lxhbucket\\raysting\\QJ55AUTO\\Zddq2\\Zddq2\\bin\\Debug";
        public static string udiskdir2 = @"\Ó²ÅÌ\";
        public static string basedir  //exe directory
        {
            get
            {
                return System.Threading.Thread.GetDomain().BaseDirectory;
            }
        }

        public static string history_log;
        public static string system_log;
        
        static GlobalConfig()
        {
            history_log = Path.Combine(basedir,"history.log");
            system_log = Path.Combine(basedir , "run.txt");
        }
    }
    internal class StringResource
    {
        private static Dictionary<string, string> str_tbl;
        static public string language;

        static public void SetLanguage(string lang)
        {
            language = lang;
        }
        static StringResource()
        {
            str_tbl = new Dictionary<string, string>();
            if (!File.Exists(GlobalConfig.history_log))
                File.Create(GlobalConfig.history_log).Close();
            FileStream fsLog = new FileStream(GlobalConfig.history_log, FileMode.Truncate, FileAccess.Write, FileShare.Read);
            fsLog.Close();
            SetLanguage("zh-CN");

            str_tbl["backspace"] = "退格";
            str_tbl["clr"] = "清除";
            str_tbl["quit"] = "退出";
            str_tbl["ok"] = "确定";
            
            str_tbl["NAV_INIT_PZ158"] = "Un%01;00\r";
//            str_tbl["NAV_10MV_PZ158"] = "Un%01;12;02\r";
            str_tbl["NAV_120MV_PZ158"] = "Un%01;12;00\r";
            str_tbl["NAV_1V_PZ158"] = "Un%01;12;01\r";
//          str_tbl["NAV_30V_PZ158"] = "Un%01;12;02\r";
            str_tbl["NAV_AFLTOFF_PZ158"] = "Un%01;26\r";
            str_tbl["NAV_AFLTON_PZ158"] = "Un%01;27\r";
            str_tbl["NAV_ZEROON_PZ158"] = "Un%01;06\r";
            str_tbl["NAV_READ_PZ158"] = "Un%01;01\r";

            str_tbl["NAV_INIT_PZ2182"] = "Un%01;00\r";
            str_tbl["NAV_10MV_PZ2182"] = "Un%01;12;00\r";
            str_tbl["NAV_120MV_PZ2182"] = "Un%01;12;01\r";
            str_tbl["NAV_1V_PZ2182"] = "Un%01;12;02\r";
            str_tbl["NAV_30V_PZ2182"] = "Un%01;12;02\r";
            str_tbl["NAV_AFLTOFF_PZ2182"] = "Un%01;26\r";
            str_tbl["NAV_AFLTON_PZ2182"] = "Un%01;27\r";
            str_tbl["NAV_ZEROON_PZ2182"] = "Un%01;06\r";
            str_tbl["NAV_READ_PZ2182"] = "Un%01;01\r";

            str_tbl["NAV_INIT_2182"] = "Un*RST\n*CLS\n:init:cont on;:ABORT\n:sens:func 'volt:dc'\n:sense:chan 1\n:sens:volt:rang:auto on\n:sens:volt:chan1:lpas off\n:SENS:VOLT:DC:NPLC 1\nVOLT:DIG 8\n:syst:azer on\n";
            str_tbl["NAV_10MV_2182"] = "Un:sens:volt:chan1:rang 0.01\n";
            str_tbl["NAV_120MV_2182"] = "Un:sens:volt:chan1:rang 0.1\n";
            str_tbl["NAV_1V_2182"] = "Un:sens:volt:chan1:rang 1\n";
            str_tbl["NAV_30V_2182"] = "Un:sens:volt:chan1:rang 10\n";
            str_tbl["NAV_AFLTOFF_2182"] = "Un:sens:volt:chan1:dfil:stat off\n";
            str_tbl["NAV_AFLTON_2182"] = "Un:sens:volt:chan1:dfil:wind 5\n:sens:volt:chan1:dfil:coun 10\n:sens:volt:chan1:dfil:tcon mov\n:sens:volt:chan1:dfil:stat on\n";
            str_tbl["NAV_ZEROON_2182"] = "Un:sens:volt:ref:acq\n:sens:volt:ref:stat on\n";
            str_tbl["NAV_READ_2182"] = "Un:fetc?\n";
        }
        static public void dolog(string log)
        {
            try
            {
                FileStream fsLog = new FileStream("history.log", FileMode.Append, FileAccess.Write, FileShare.Read);
                StreamWriter sw = new StreamWriter(fsLog);
                sw.WriteLine(DateTime.Now.ToString("G") + "\t" + log);
                sw.Close();
                fsLog.Close();
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("写入历史记录失败:" + ex.Message);    
            }

        }

        public static string str(string key)
        {
            if (str_tbl.ContainsKey(key))
            {
                return str_tbl[key];
            }
            else
            {
                return "Invalid String Key";
            }

        }
        private StringResource() { }

    }

}
