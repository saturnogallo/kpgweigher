using System;
using System.Linq;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Xml.Linq;
using System.Threading;
namespace TSioex
{
    static class Program
    {
        static public List<UIPacker> packers; //list of available packers , config in app_config.xml

        static private SqlConfig app_cfg;     //configuration loaded from app_config.xml, fixed settings
        static public XElement curr_cfg;      //current application configuration including upper_var, lower_var, target_weight, operator

        //window list
        static public RunModeWnd runwnd;
        static public SingleModeWnd singlewnd;
        static public ProdHistory histwnd;
        static public PwdWnd pwdwnd;
        static private AlertWnd alertwnd;
        static public EngWnd engwnd;

        static private ConfigMenuWnd configwnd;
        static private BottomWnd bottomwnd;

        static public ProdWnd prodwnd;
        static public ProdNum prodnum;
        static public kbdWnd kbdwnd;
        static public MsgDlg msgwnd;
        static public NodeMaster nm;
        static public string topwnd = "logon";
        static public string oper
        { //global variable for operator setting.
            get
            {
                try
                {
                    return curr_cfg.Element("operator").Value.ToString();
                }
                catch
                {
                    return "999";
                }
            }
            set
            {
                curr_cfg.SetElementValue("operator", value);
                SaveAppConfig();
            }
        }
        static public int line = 0;
        static public void Initialize()
        {
/*
            System.Diagnostics.Process[] pses = System.Diagnostics.Process..GetProcessesByName("TSioex");
            if (pses.Length > 0) 
            {
                System.Diagnostics.Process.GetCurrentProcess().Kill();
              return;
            }
 */
            Thread.Sleep(3000);
            try
            {
                StringResource.SetLanguage();
                msgwnd = new MsgDlg();
                app_cfg = new SqlConfig("app");
                app_cfg.LoadConfigFromFile(); 

                curr_cfg = app_cfg.Current;

                packers = new List<UIPacker>();
                for (int i = 0; i < Int32.Parse(curr_cfg.Element("machine_number").Value); i++)
                {
                    UIPacker p = new UIPacker(i); line++;
                    p.InitConfig();
                    packers.Add(p);
                }
                line = 999;
                NodeMaster.Dummy(); line++;

                singlewnd = new SingleModeWnd(); line++;
                runwnd = new RunModeWnd(); line++;


                histwnd = new ProdHistory(); line++;

                kbdwnd = new kbdWnd(); line++;
                bottomwnd = new BottomWnd(); line++;
                alertwnd = new AlertWnd(); line++;
                alertwnd.UpdateUI(); //load alert configuration which is in app_config.xml too

                pwdwnd = new PwdWnd(); line++;
                engwnd = new EngWnd(); line++;
                configwnd = new ConfigMenuWnd(); line++;
                prodwnd = new ProdWnd(); line++;
                prodnum = new ProdNum(); line++;
                
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
                System.Diagnostics.Process.GetCurrentProcess().Kill();
            }
        }
        static public void MsgShow(string line)
        {
            msgwnd.Message = line;
            msgwnd.ShowDialog();
        }
        static public void SaveAppConfig()
        {
            app_cfg.AddConfig(app_cfg.cfg_name, curr_cfg);
            app_cfg.SaveConfigToFile();
        }
        static public void SwitchTo(string mode)
        {
            if (mode == "history")
            {
                topwnd = mode;
                histwnd.BringToFront();
                histwnd.Show();
                histwnd.UpdateDisplay();
                histwnd.UpdateList();
                
                return;
            }
            if (mode == "alert")
            {
                topwnd = mode;
                alertwnd.BringToFront();
                alertwnd.Show();
                alertwnd.UpdateUI();
                return;
            }
            if (mode == "bottom")
            {
                topwnd = mode;
                bottomwnd.BringToFront();
                bottomwnd.UpdateDisplay();
                bottomwnd.Show();
                return;
            }
            if (mode == "password")
            {
                topwnd = mode;
                pwdwnd.UpdateDisplay();
                pwdwnd.Show();
                pwdwnd.BringToFront();
                return;
            }
            if (mode == "engineer")
            {
                topwnd = mode;
                engwnd.InitDisplay();
                engwnd.Show();
                engwnd.BringToFront();
                return;
            }
            /*
            singlewnd.Hide();
            histwnd.Hide();
            runwnd.Hide();
            alertwnd.Hide();
            bottomwnd.Hide();
            pwdwnd.Hide();
            engwnd.Hide();
            kbdwnd.Hide();
            configwnd.Hide();
            */
            if (mode == "configmenu")
            {
                topwnd = mode;
                configwnd.BringToFront();
                configwnd.Show();
                configwnd.UpdateDisplay();
                return;
            }
            if (mode == "product")
            {
                mode = "singlemode";
            }
            if (mode == "runmode")
            {
                topwnd = mode;
                runwnd.BringToFront();
                runwnd.Show();
                if (runwnd.btn_allstart.Visible == false)
                    Program.MsgShow(StringResource.str("license"));
                runwnd.UpdateSysConfigUI();

                return;
            }
            if (mode == "singlemode")
            {
                topwnd = mode;
                singlewnd.BringToFront();
                singlewnd.Show();
                singlewnd.UpdateUI();
                return;
            }
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [MTAThread]
        static void Main(string[] argv)
        {
//            if((argv.Length > 1) && (argv[1].IndexOf("/debug") > 0))
            //NodeAgent.IsDebug = true;
            Thread.Sleep(2000);
            Program.Initialize();
            Application.Run(new LogonWindow());
        }

    }
}