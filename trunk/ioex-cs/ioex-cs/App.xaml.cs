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

namespace ioex_cs
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public static bool bSimulate = false;
        internal List<UIPacker> packers; //list of available packers , config in app_config.xml
        internal NodeAgent agent;
        internal int machnum;
        internal UIPacker curr_packer { 
            get { 
                return packers[0]; 
            } 
        }
        private XmlConfig app_cfg;     //configuration loaded from app_config.xml
        public XElement curr_cfg;   //current configuration

        //window list
        private RunMode runwnd;
        private SingleMode singlewnd;
        private ProdHistory histwnd;
        private Help helpwnd;
        private AlertWnd alertwnd;
        private EngConfigWnd engwnd;
        private PwdWnd pwdwnd;
        private ConfigMenuWnd configwnd;
        private BottomWnd bottomwnd;
        public ProdWnd prodwnd;
        public ProdNum prodnum;
        public kbd kbdwnd;
        

        private void UpdateUI()
        {
            

        }
        public App()
        {
            try
            {
                bSimulate = false;
                app_cfg = new XmlConfig("app_config.xml");
                app_cfg.LoadConfigFromFile();

                
                curr_cfg = app_cfg.Current;
                try
                {
                    PackerConfig.oper = curr_cfg.Element("operator").Value.ToString();
                }
                catch
                {
                    PackerConfig.oper = "999";
                }


                machnum = Int32.Parse(curr_cfg.Element("machine_number").Value);
                packers = new List<UIPacker>();


                

                histwnd = new ProdHistory();
                
                helpwnd = new Help();
                
                kbdwnd = new kbd();
                
                bottomwnd = new BottomWnd();
                

                alertwnd = new AlertWnd();
                
                alertwnd.UpdateUI(); //load alert configuration
                
                pwdwnd = new PwdWnd();
                
                engwnd = new EngConfigWnd();
                
                configwnd = new ConfigMenuWnd();
                
                prodwnd = new ProdWnd();

                


                prodnum = new ProdNum();

                    agent = new NodeAgent();
                    singlewnd = new SingleMode();

                    runwnd = new RunMode();

                    //configwnd.Show();
                    //runwnd.Show();
                    //create packers

                    for (int i = 0; i < machnum; i++)
                    {
                        agent.packer = new UIPacker(i, agent);
                        packers.Add(agent.packer);
                    }

                    agent.Start();

                    for (int i = 0; i < machnum; i++)
                    {
                        packers[i].InitConfig();
                    }


                    SwitchTo("runmode");

                

            }
            catch (Exception e)
            {
                    MessageBox.Show(e.Message );
                    foreach (Window w in Windows)
                        w.Close();
                    this.Shutdown();

            }
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
                runwnd.UpdateUI("sys_config");
                runwnd.BringIntoView();
                return;
            }
            if (mode == "singlemode")
            {
                singlewnd.Show();
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
