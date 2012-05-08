using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Windows;
using System.IO;
using System.Security.Cryptography;
using System.Xml;
using System.Xml.Linq;
using System.Threading;
namespace TSioex
{
    public partial class LogonWindow : Form
    {
        private Thread bootup;
        private System.Windows.Forms.Timer uiTimer;
        private int boot_ok = 0;

        public LogonWindow()
        {
            InitializeComponent();
            this.Load += new EventHandler(Window_Loaded);
            bootup = new Thread(PackerBoot);
            bootup.IsBackground = true;
            uiTimer = new System.Windows.Forms.Timer();
            uiTimer.Tick += new EventHandler(uiTimer_Tick);
            uiTimer.Interval = 200;
            uiTimer.Enabled = false;
        }

        void uiTimer_Tick(object sender, EventArgs e)
        {
            if (!bootup.Join(10)) //still under bootup cycle
                return;
            uiTimer.Enabled = false;
            if (boot_ok == 0)
            {
                MessageBox.Show(StringResource.str("bootfail"));
                bootmsg.Text = "";
                bootup = new Thread(PackerBoot);

                bootup.IsBackground = false;

                bootup.Start();
                uiTimer.Enabled = true;
            }
            else
            {
                StringResource.SetLanguage();
                
                Program.SwitchTo("runmode");
                this.Visible = false;
                this.bgPicture.Dispose();
            }
        }
        public void PackerBoot()
        {
            try
            {
                UpdateMessage("App Version: " + System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString() + "\r\n");
                //search for nodes
                //UpdateMessage(StringResource.str("search_newnode"));
//                NodeMaster.SearchNode(byte.Parse(Program.curr_cfg.Element("def_addr").Value));

                boot_ok = 0;
                int tryfound;
                //check the availability of each board
                foreach (UIPacker pk in Program.packers)
                {
                    foreach (byte n in pk.weight_nodes)
                    {
                        if ((n % 4) == 3)
                            UpdateMessage("\r\n");
                        tryfound = 3;
                        while (!NodeMaster.SearchNode(n) && (tryfound > 0))
                        {
                            Thread.Sleep(500);
                            tryfound--;
                        }
                        if (tryfound <= 0)
                        {
                            UpdateMessage(StringResource.str("search_node") + n + StringResource.str("fail") + "\t\t");
                            Program.engwnd.btn_updatefw_Click(null, null);
                        }
                        else
                        {
                            boot_ok++;
                            UpdateMessage(n + ": ver " + NodeMaster.GetNodeReg(n, "fw_rev_uw").ToString() + "\t\t");
                        }
                    }
                    byte nvib = pk.vib_addr;
                    tryfound = 3;
                    while ((!NodeMaster.SearchNode(nvib)) && (tryfound > 0))
                    {
                        Thread.Sleep(500);
                        tryfound--;
                    }
                    if (tryfound <= 0)
                    {
                        UpdateMessage(StringResource.str("search_node") + nvib + StringResource.str("fail") + "\t\t");
                    }
                    else
                    {
                        boot_ok++;
                        UpdateMessage(nvib + ": ver " + NodeMaster.GetNodeReg(nvib, "fw_rev_uw").ToString() + "\t\t");
                    }
                    Thread.Sleep(2000);
                    if (boot_ok > 0)
                    {
                        //load config for node
                        UpdateMessage("\r\n" + StringResource.str("init_nodereg"));
                        //todo
                        pk.LoadPackConfig(pk.pkg_confs.cfg_name, true);
                        NodeMaster.Action(new byte[] { pk.vib_addr }, "intf");
                        NodeMaster.Action(pk.wn_addrs, "stop");
                        NodeMaster.Action(new byte[] { pk.vib_addr }, "stop");
                    }
                }
            }
            catch (Exception ex)
            {
                UpdateMessage(ex.Message);
                boot_ok = 0;
                Thread.Sleep(3000);
            }

        }
        public void UpdateMessage(string msg)
        {
            this.Invoke((Action)delegate
            {
                bootmsg.Text = bootmsg.Text + msg;
            });

        }
        private void Window_Loaded(object sender, EventArgs e)
        {
            boot_ok = 0;
            bootup.Start();
            uiTimer.Enabled = true;
        }
    }
    internal class StringResource
    {
        private static Dictionary<string, string> str_tbl;
        static public string language;
        static public void SetLanguage(string lang)
        {
            language = lang;
            string lang_file = "TSioex.Resources.lang." + lang + ".xml";
            StreamReader sr = new StreamReader(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream(lang_file));
            XDocument xml_doc = XDocument.Load(sr);
            
            IEnumerable<XElement> regs = xml_doc.Descendants("string");
            str_tbl.Clear();
            foreach (XElement reg in regs)
            {
                str_tbl[reg.Attribute("skey").Value] = reg.Value;
            }
        }
        static StringResource()
        {
            str_tbl = new Dictionary<string, string>();
            if (!File.Exists(ProdNum.baseDir + "\\history.log"))
                File.Create(ProdNum.baseDir + "\\history.log").Close();
            FileStream fsLog = new FileStream(ProdNum.baseDir + "\\history.log", FileMode.Truncate, FileAccess.Write, FileShare.Read);
            fsLog.Close();
        }
        static public void SetLanguage()
        {
            if (EngWnd.GetRegistData("locale") == "0")
                SetLanguage("en-US");
            else
                SetLanguage(EngWnd.GetRegistData("locale"));
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
            catch (System.Exception)
            {

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

    internal class Password
    {
        private static SqlConfig pwds;
        static private string MD5Value(String str, Boolean isStr)
        {
            MD5 md5 = new MD5CryptoServiceProvider();
            byte[] md5ch;
            if (isStr)
            {
                byte[] ch = System.Text.Encoding.Default.GetBytes(str);
                md5ch = md5.ComputeHash(ch);
            }
            else
            {
                if (!File.Exists(str))
                    return string.Empty;
                FileStream fs = new FileStream(str, FileMode.Open, FileAccess.Read);
                md5ch = md5.ComputeHash(fs);
                fs.Close();
            }
            md5.Clear();
            string strMd5 = "";
            for (int i = 0; i < md5ch.Length - 1; i++)
            {
                strMd5 += md5ch[i].ToString("x").PadLeft(2, '0');
            }
            return strMd5;

        }


        public static void CheckLicense(string lic)
        {
/*
            string cpuInfo = "";

            ManagementClass cimobject = new ManagementClass("Win32_Processor");
            ManagementObjectCollection moc = cimobject.GetInstances();
            foreach (ManagementObject mo in moc)
            {
                cpuInfo = mo.Properties["ProcessorId"].Value.ToString();
                cpuInfo = cpuInfo.Substring(cpuInfo.Length - 6);
                break;
            }

            string HDid = "";
            ManagementClass cimobject1 = new ManagementClass("Win32_DiskDrive");
            ManagementObjectCollection moc1 = cimobject1.GetInstances();
            foreach (ManagementObject mo in moc1)
            {
                HDid = (string)mo.Properties["Model"].Value;
                HDid = HDid.Substring(HDid.Length - 6);
                break;
            }


            if (lic != MD5Value(cpuInfo + "255" + HDid, true))
            {
                //StringResource.dolog(MD5Value(cpuInfo + "255" + HDid, true));
                MessageBox.Show("Please provide following string to vendor for a valid license : " + cpuInfo + HDid);
                System.Diagnostics.Process.GetCurrentProcess().Kill();
            }
 */
        }
        static Password()
        {
            //load password.xml and fill in the username list
            //pwds = new XmlConfig("password.xml");
            pwds = new SqlConfig("password");
            pwds.LoadConfigFromFile();
            //string a = MD5Value("0106C2255SJ 8GB", true);
            //"0106C2255SJ 8GB"
            //""
        }
        public static IEnumerable<string> users
        {
            get
            {
                return pwds.Keys;
            }
        }
        public static bool compare_pwd(string user, string data)
        {
            return (get_pwd(user) == MD5Value(data + user + "1r%4#", true));

        }
        private static string get_pwd(string user)
        {
            XElement x = pwds.LoadConfig(user);
            if (null == x)
                return pwds.GetHashCode().ToString(); //just a difficult password to guess
            else
                return x.Element("sn").Value;
        }
        public static void set_pwd(string user, string value)
        {
            XElement x = new XElement("Item");
            x.SetElementValue("sn", MD5Value(value + user + "1r%4#", true));
            pwds.AddConfig(user, x);
            pwds.SaveConfigToFile();

        }
    }
}