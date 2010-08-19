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
        internal List<Packer> packers; //list of available packers , config in app_config.xml
        internal List<SubNode> nodes;  //list of all nodes
        internal List<SPort> allports; //list of all serial ports

        internal bool bQueryBusy;   //status of query loop
        private bool _bPause;       //internal variable for pause
        internal bool bSimulate;    //indicate whether it is simulate run;


        internal bool bMainPause
        {
            get{
                return _bPause;
            }
            set{
                _bPause = value;
                if (_bPause)
                {
                    while (bQueryBusy)
                    {
                        Thread.Sleep(1);
                    }

                }
            }
        }   //indicate the pause status of main loop

        private SubNode missingnode;   //node to find the newly installed board

        private XmlConfig app_cfg;     //configuration loaded from app_config.xml
        private XElement default_cfg;   //current configuration

        private int machnum; //number of packers
        
        //window list
        private RunMode runwnd;
        private SingleMode singlewnd;
        private History histwnd;
        private Help helpwnd;
        private AlertWnd alertwnd;
        private EngConfigWnd engwnd;
        private PwdWnd pwdwnd;
        private ConfigMenuWnd configwnd;
        private BottomWnd bottomwnd;
        private ProdWnd prodwnd;
        public kbd kbdwnd;

        private Thread query_loop;
        public void WeightQuery(object param)
        {
            App app = Application.Current as App;
            while (true)
            {
                bQueryBusy = false;
                Thread.Sleep(1);
                if(bMainPause)
                {
                    bQueryBusy = false;
                    continue;
                }
                bQueryBusy = true;
                foreach (Packer p in app.packers)
                {
                    if (p.status != PackerStatus.RUNNING)
                    {
//                        Thread.Sleep(1);
                    }
                    if (p.status == PackerStatus.INERROR)
                    {
                        return;
                    }
                    
                    foreach (WeighNode n in p.weight_node)
                    {
                        if (n.status == NodeStatus.ST_LOST)
                        {
                            continue;
                        }
                        while (n.status != NodeStatus.ST_IDLE)
                        {
                            Thread.Sleep(1);
                        }
                        n.Action("query", true);
                    }
                    bool alldone = false;
                    while(!alldone)
                    {
                        alldone = true;
                        foreach (WeighNode n in p.weight_node)
                        {
                            if (n.status == NodeStatus.ST_LOST || n["Mtrl_Weight_gram"].HasValue)
                            {
                                continue;
                            }
                            while (n.status != NodeStatus.ST_IDLE)
                            {
                                Thread.Sleep(1);
                            }
                            n.Action("query", true);
                            alldone = false;
                        }
                    }
                    if (p.status == PackerStatus.RUNNING)
                    {
                        p.CheckCombination();
                    }
                    foreach (WeighNode n in p.weight_node)
                    {
                        n.weight = 0;
                    }
                }
                bQueryBusy = false;
            }
        }
        private void UpdateUI()
        {
            

        }
        public App()
        {
            int baudrate;// baudrate
 
           
            histwnd = new History();
            helpwnd = new Help();
            kbdwnd = new kbd();
            bottomwnd = new BottomWnd();
            alertwnd = new AlertWnd();
            pwdwnd = new PwdWnd();
            engwnd = new EngConfigWnd();
            configwnd = new ConfigMenuWnd();
            prodwnd = new ProdWnd();
            //prodwnd.Show();
            
            singlewnd = new SingleMode();
            
            runwnd = new RunMode();

            
            allports = new List<SPort>();
            packers = new List<Packer>();
            nodes = new List<SubNode>();
            app_cfg = new XmlConfig("app_config.xml");
            app_cfg.LoadConfigFromFile();
            default_cfg = app_cfg.LoadConfig("default");

            
            
            query_loop = new Thread(new ParameterizedThreadStart(WeightQuery));
            
            query_loop.IsBackground = false;

            bQueryBusy = false;
            bMainPause = false;

            machnum = Int32.Parse(default_cfg.Element("machine_number").Value);
            baudrate = Int32.Parse(default_cfg.Element("baudrate").Value);
            
            //create all ports
            string tot = default_cfg.Element("totalports").Value.ToString();
            foreach (string port in tot.Split(new char[]{','}))
            {
                SPort sp = new SPort(port, baudrate, Parity.None, 8, StopBits.One);
                if (sp.Open())
                {
                    allports.Add(sp);
                }
                else
                {
                    MessageBox.Show("failed to open port " + port);
                    throw new Exception("failed to open port " + port);
                }
                
            }

            //create all nodes
            List<byte> index2addr = new List<byte>();
            tot = default_cfg.Element("totalnodes").Value.ToString();
            foreach (string node in tot.Split(new char[]{','}))
            {
                string type = default_cfg.Element("node_type" + node).Value;
                string com = default_cfg.Element("node_com" + node).Value;
                SubNode n;
                if (type == "vibrate")
                {
                    n = new VibrateNode(allports[byte.Parse(com)], byte.Parse(node));
                    nodes.Add(n);
                    index2addr.Add(byte.Parse(node));
                }
                if (type == "weight")
                {
                    n = new WeighNode(allports[byte.Parse(com)], byte.Parse(node));
                    nodes.Add(n);
                    index2addr.Add(byte.Parse(node));
                }
                if (type == "pack")
                {
                    n = new BottomPackNode(allports[byte.Parse(com)],byte.Parse(node));
                    nodes.Add(n);
                    index2addr.Add(byte.Parse(node));
                }
            }
            
            //create packers
            for (int i=0;i<machnum;i++)
            {
                packers.Add(new Packer());
                string title = "packer" + i.ToString();
                tot = default_cfg.Element(title).Value.ToString();
                foreach (string node in tot.Split(new char[]{','}))
                {
                    for(int j=0;j < index2addr.Count;j++)
                    {
                        if (index2addr[j] == int.Parse(node))
                        {
                            packers[i].AddNode(nodes[j]);
                        }
                    }
                }

            }
            
            //open sport 
            /*
            foreach (SPort t in allports)
            {
                if (!t.Open())
                {
                    MessageBox.Show("Serial Port open failed");
                    throw new Exception("Serial Port open failed");
                }
            }*/
            //return;
            //check the availability of each board
            foreach (SubNode n in nodes)
            {
                n["addr"] = null;
                if (n.errmsg != "")
                {
                    MessageBox.Show("Node " +n.node_id.ToString()+ " is lost");
                    //todo check the availbility of pending nodes
                    throw new Exception("Node " + n.node_id.ToString() + " is lost");
                }

                n["board_ID"] = null;
                
            }
            //runwnd.Show();
            //todo: load the default setting and download them
            /*
            for (int i = 0; i < machnum; i++)
            {
                packers[i].LoadConfig(default_cfg.Element("packer" + i.ToString() + "_def").Value);
            }*/

            query_loop.Start();
            runwnd.Show();
        }
        
        public void SwitchTo(string mode)
        {
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

            if (mode == "alert")
            {
                alertwnd.Show();
                alertwnd.BringIntoView();
            }
            if (mode == "bottom")
            {
                bottomwnd.Show();
                bottomwnd.BringIntoView();
            }
            if (mode == "password")
            {
                pwdwnd.Show();
                pwdwnd.BringIntoView();
            }
            if (mode == "engineer")
            {
                engwnd.Show();
                engwnd.BringIntoView();
            }
            if (mode == "ConfigMenu")
            {
                configwnd.Show();
                configwnd.BringIntoView();
            }
            if (mode == "product")
            {
                mode = "singlemode";
            }
            if (mode == "history")
            {
                mode = "histmode";
            }
            if (mode == "runmode")
            {
                runwnd.Show();
                runwnd.BringIntoView();

                return;
            }
            if (mode == "singlemode")
            {
                singlewnd.Show();
                singlewnd.BringIntoView();
                return;
            }
            if (mode == "histmode")
            {
                histwnd.Show();
                histwnd.BringIntoView();
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
