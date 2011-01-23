using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Xml.Linq;
using System.IO;
namespace ioex_cs
{
    public class StringResource
    {
        private static Dictionary<string, string> str_tbl;
        static StringResource()
        {
            str_tbl = new Dictionary<string, string>();
            
            XDocument xml_doc;
            xml_doc = XDocument.Load("Resources\\Lang\\zh-CN.xaml");
            IEnumerable<XElement> elem = xml_doc.Elements().First().Elements();
            foreach (XElement e in elem)
            {
                
                str_tbl[e.FirstAttribute.Value] = e.Value.ToString();
            }
            FileStream fsLog = new FileStream(System.Threading.Thread.GetDomain().BaseDirectory + "\\history.log", FileMode.Truncate, FileAccess.Write, FileShare.Read);
            fsLog.Close();
        }
        static public void dolog(string log)
        {
            try
            {
                FileStream fsLog = new FileStream(System.Threading.Thread.GetDomain().BaseDirectory + "\\history.log", FileMode.Append, FileAccess.Write, FileShare.Read);
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
            }else
            {
                return "Invalid String Key";
            }
            
        }
        private StringResource(){}

    }

    public class Password
    {
        private static XmlConfig pwds;
        
        static Password()
        {
            //load password.xml and fill in the username list
            pwds = new XmlConfig("password.xml");
            pwds.LoadConfigFromFile();
        }
        public static IEnumerable<string> users {
            get{
                return pwds.Keys;
            }
        }
        public static string get_pwd(string user){
                XElement x = pwds.LoadConfig(user);
                if (null == x)
                    return pwds.GetHashCode().ToString(); //just a difficult password to guess
                else
                    return x.Value;
            }
        public static void set_pwd(string user,string value){
                XElement x = new XElement("Item");
                x.SetAttributeValue("Name", user);
                x.SetValue(value);
                pwds.AddConfig(user,x);
                pwds.SaveConfigToFile();
            
        }
    }
    /// <summary>
    /// Interaction logic for Logon.xaml
    /// </summary>
    public partial class LogonWindow : Window
    {
        
        public LogonWindow()
        {
            
            InitializeComponent();
            string abc = StringResource.str("invalid_pwd");
            
            foreach(string name in Password.users)
            {
                this.usr_input.Items.Add(name);
            }
        }

        void pwd_input_GotFocus(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            p.kbdwnd.Init(StringResource.str("enter_pwd"), "logon_pwd", true, KbdData);
            
        }

        private void btn_submit_Click(object sender, RoutedEventArgs e)
        {
            
            if ((Password.get_pwd(usr_input.SelectedValue.ToString()) != pwd_input.Password))
            {
                MessageBox.Show(StringResource.str("invalid_pwd"));
                return;
            }
            //open configmenu window.
            App p = Application.Current as App;
            PackerConfig.oper = usr_input.SelectedValue.ToString();
            p.SwitchTo("configmenu");
        }

        private void btn_help_Click(object sender, RoutedEventArgs e)
        {
            //display the help manual
            App p = Application.Current as App;
            p.SwitchTo("helpmode");
        }
        public void KbdData(string param, string data)
        {
            if (param == "logon_pwd")
            {
                this.pwd_input.Password = data;
            }
        }
    }
}
