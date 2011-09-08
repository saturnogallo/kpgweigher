using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Windows;
using System.IO.Ports;
using System.Text;
using System.Xml.Linq;
using System.Threading;

using System.IO;
namespace ioex_cs
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        internal List<UIPacker> packers; //list of available packers , config in app_config.xml
        internal NodeAgent agent; //agent to manage all the nodes
 
        private XmlConfig app_cfg;     //configuration loaded from app_config.xml, fixed settings
        public XElement curr_cfg;   //current configuration

        //window list
        public RunMode runwnd;
        public SingleMode singlewnd;
        public ProdHistory histwnd;
        public PwdWnd pwdwnd;
        private Help helpwnd;
        private AlertWnd alertwnd;
        private EngConfigWnd engwnd;
        
        private ConfigMenuWnd configwnd;
        private BottomWnd bottomwnd;

        public ProdWnd prodwnd;
        public ProdNum prodnum;
        public kbd kbdwnd;

        public string oper{ //global variable for operator setting.
            get{
                try
                {
                    return curr_cfg.Element("operator").Value.ToString();
                }
                catch {
                    return "999";
                }
            }
            set{
                curr_cfg.SetElementValue("operator", value);
                SaveAppConfig();
            }
        }
        
        public App()
        {
            System.Diagnostics.Process[] pses = System.Diagnostics.Process.GetProcessesByName("ioex-cs");
            if (pses.Length > 0) 
            {
//                System.Diagnostics.Process.GetCurrentProcess().Kill();
//              return;
            }
            
            if (Environment.CommandLine.IndexOf("/debug") > 0)
                NodeAgent.IsDebug = true;
            StringResource.SetLanguage();
            app_cfg = new XmlConfig(ProdNum.baseDir +"\\app_config.xml");
            app_cfg.LoadConfigFromFile();

            curr_cfg = app_cfg.Current;

            agent = new NodeAgent();
            packers = new List<UIPacker>();
            for (int i = 0; i < Int32.Parse(curr_cfg.Element("machine_number").Value); i++)
            {
                UIPacker p = new UIPacker(i, agent);
                p.agent = agent;
                p.InitConfig();
                packers.Add(p);
            }

            singlewnd = new SingleMode(Int32.Parse(curr_cfg.Element("node_number").Value));
            runwnd = new RunMode(Int32.Parse(curr_cfg.Element("node_number").Value));
            
            
            histwnd = new ProdHistory();
            helpwnd = new Help();
            kbdwnd = new kbd();
            bottomwnd = new BottomWnd();
            alertwnd = new AlertWnd();
            alertwnd.UpdateUI(); //load alert configuration which is in app_config.xml too

            pwdwnd = new PwdWnd();
            engwnd = new EngConfigWnd();
            configwnd = new ConfigMenuWnd();
            prodwnd = new ProdWnd();
            prodnum = new ProdNum();
            

        }
        public void SaveAppConfig()
        {
            app_cfg.AddConfig(app_cfg.cfg_name, curr_cfg);
            app_cfg.SaveConfigToFile();
        }
        public void SwitchTo(string mode)
        {
            if (mode == "history")
            {
                histwnd.UpdateDisplay();
                histwnd.UpdateList();
                histwnd.Show();
                histwnd.BringToFront();
                return;
            }
            if (mode == "alert")
            {
                alertwnd.UpdateUI();
                alertwnd.Show();
                alertwnd.BringIntoView();
                return;
            }
            if (mode == "bottom")
            {
                bottomwnd.Show();
                bottomwnd.UpdateDisplay();
                bottomwnd.BringIntoView();
                return;
            }
            if (mode == "password")
            {
                pwdwnd.Show();
                pwdwnd.BringIntoView();
                return;
            }
            if (mode == "engineer")
            {
                engwnd.InitDisplay();
                engwnd.Show();
                engwnd.BringIntoView();
                return;
            }

            singlewnd.Hide();
            histwnd.Hide();
            helpwnd.Hide();
            runwnd.Hide();
            alertwnd.Hide();
            bottomwnd.Hide();
            pwdwnd.Hide();
            engwnd.Hide();
            kbdwnd.Hide();
            configwnd.Hide();

            if (mode == "configmenu")
            {
                configwnd.Show();
                configwnd.BringIntoView();
                return;
            }
            if (mode == "product")
            {
                mode = "singlemode";
            }
            if (mode == "runmode")
            {
                runwnd.Show();
                if (runwnd.btn_allstart.Visibility == Visibility.Hidden)
                    MessageBox.Show(StringResource.str("license"));
                runwnd.UpdateUI("sys_config");
                runwnd.BringIntoView();
                return;
            }
            if (mode == "singlemode")
            {
                singlewnd.Show();
				singlewnd.UpdateUI();
                singlewnd.BringIntoView();
                return;
            }
            if (mode == "helpmode")
            {
                helpwnd.Show();
                helpwnd.BringIntoView();
                return;
            }
        }
    }
}
