using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO.Ports;
using System.Xml.Linq;
using System.Windows.Forms;
namespace ioex_cs
{
    class NodeAgent
    {
        public static bool IsDebug = false;
        internal List<WeighNode> weight_nodes; //list of all weight nodes
        internal List<SPort> allports; //list of all serial ports
        internal Dictionary<byte, SubNode> nodemap;//map the address to node
        internal VibrateNode vibnode;
        
        private SubNode missingnode;   //node used to find the newly installed board

        private byte addrIn;    //address used to store the incoming command address
        private string CmdIn;   //string used to store the incoming command
        public string ResultOut; //string used to store the feedback
        private object InLock;
        private object OutLock;
        private Thread msg_loop;
        private bool QueryLoopDone; //indicate the completion of loop query

        public UIPacker packer; //reference to pack
        Random rand;
        public double weight(byte addr)
        {
            if (nodemap.ContainsKey(addr))
            {
                return (nodemap[addr] as WeighNode).weight;
            }
            return -9999.99;

        }
        public bool GetWNodeFlag(int addr, string flag)
        {
            if (flag == "bRelease")
            {
                if (addr < 0 || addr > 254 || (!nodemap.ContainsKey((byte)addr)))
                    return false;
                return (nodemap[(byte)addr] as WeighNode).bRelease;
            }
            throw new NotImplementedException();
        }
        public string GetErrors(byte addr)
        {
            if (addr < 0 || addr > 254 || (!nodemap.ContainsKey(addr)))
                return "";
            return nodemap[addr].errlist;
        }
        public void ClearErrors(byte addr)
        {
            if (addr < 0 || addr > 254 || (!nodemap.ContainsKey(addr)))
                return;
            nodemap[addr].errlist = "";
        }
        public void SetStatus(byte addr, NodeStatus ns)
        {
            if (addr < 0 || addr > 254 || (!nodemap.ContainsKey(addr)))
                return;
            Action(addr, "disable");
            
        }

        public NodeStatus GetStatus(byte addr)
        {
            if (addr < 0 || addr > 254 || (!nodemap.ContainsKey(addr)))
                return NodeStatus.ST_IDLE;
            return nodemap[addr].status;
        }
        public void SetNodeReg(byte addr, string regname , UInt32 val)
        {
            if (!nodemap.ContainsKey(addr))
                return;
            
            Action(addr, "setreg:" + regname + ":"+val.ToString());
        }
        public void GetNodeElement(byte addr, ref XElement x)
        {
            if (!nodemap.ContainsKey(addr))
                return;
            string[] regs = { "magnet_freq", "magnet_amp", "magnet_time", "open_w", "delay_w", "open_s", "delay_s", "delay_f", 
                                "motor_speed", "cs_filter", "cs_gain_wordrate","target_weight"};
            foreach (string reg in regs)
            {
                if (nodemap[addr][reg].HasValue)
                    x.Add(new XElement(reg, nodemap[addr][reg].Value.ToString()));
                else
                    x.Add(new XElement(reg, "0"));
            }
        }
        public string GetNodeReg(byte addr,string regname)
        {
             if (!nodemap.ContainsKey(addr))
                 return "";

             if (!nodemap[addr][regname].HasValue)
             {
                 Action(addr, "getreg:" + regname);
                 if (ResultOut != "")
                 {
                     MessageBox.Show("Failed to download config, please try again");
                     throw new Exception("Failed to download config, please try again");
                 }
             }
             return nodemap[addr][regname].ToString();
        }
        public void Action(byte addr, string command)
        {
            if ((addr < 0x80) && (!nodemap.ContainsKey(addr)))
                return;

            lock(InLock)
            {
                ResultOut = null;
                addrIn = addr;
                CmdIn = command;
            }
            while (ResultOut == null)
            {
                //Thread.SpinWait(1);
                Thread.Sleep(2);
            }
            
        }
        private void LoadConfiguration()
        {
            //load current configuration
            
            XmlConfig app_cfg = new XmlConfig("app_config.xml");
            app_cfg.LoadConfigFromFile();
            
            XElement def_cfg = app_cfg.Current;

            Int32 baudrate = Int32.Parse(def_cfg.Element("baudrate").Value);
            string parity = def_cfg.Element("parity").Value.ToLower();
            string[] ser_ports = def_cfg.Element("totalports").Value.ToString().Split(new char[] { ',' });
            string[] node_addrs = def_cfg.Element("totalnodes").Value.ToString().Split(new char[] { ',' });
            
            //create all serial ports
            foreach (string port in ser_ports)
            {
                if (parity == "even")
                    allports.Add(new SPort(port, baudrate, Parity.Even, 8, StopBits.One));
                else if (parity == "odd")
                    allports.Add(new SPort(port, baudrate, Parity.Odd, 8, StopBits.One));
                else
                    allports.Add(new SPort(port, baudrate, Parity.None, 8, StopBits.One));
            }
            foreach (SPort sp in allports)
            {
                if (!sp.Open())
                {
                    MessageBox.Show("Failed to open the port");
                    throw new Exception("Failed to open the port");
                }
            }
            //create all nodes
            foreach (string node in node_addrs)
            {
                string type = def_cfg.Element("node_type" + node).Value;
                string com = def_cfg.Element("node_com" + node).Value;
                
                if (type == "vibrate")
                {
                    vibnode = new VibrateNode(allports[byte.Parse(com)], byte.Parse(node));
                    nodemap[byte.Parse(node)] = vibnode;
                }
                if (type == "weight")
                {
                    nodemap[byte.Parse(node)] = new WeighNode(allports[byte.Parse(com)], byte.Parse(node));
                    weight_nodes.Add(nodemap[byte.Parse(node)] as WeighNode);
                }
            }

            //check the availability of each board
            
            foreach (SubNode n in nodemap.Values)
            {
/*                if (n.node_id == 1)
                {
                    bootloader bl = new bootloader(n);
                    bl.download("C:\\MAIN.bin");
                }
 */
                n["addr"] = null;
                if (!WaitForIdle(n))
                {
                    missingnode = new WeighNode(n.serialport, byte.Parse(def_cfg.Element("def_addr").Value)); //36 is the default address of unassigned address board
                    //check the availability of pending nodes
                    missingnode["addr"] = null;
                    if (WaitForIdle(missingnode)) //node is found
                    {
                        missingnode["addr"] = n.node_id;
                        if (WaitForIdle(missingnode))
                        {
                            n["NumOfDataToBePgmed"] = UInt32.Parse(def_cfg.Element("romsize").Value); //write address to flash
                            Thread.Sleep(1000);
                            continue;
                        }
                    }
                    MessageBox.Show("Node " + n.node_id.ToString() + " is lost");
                    throw new Exception("Node " + n.node_id.ToString() + " is lost");
                }
                else
                {
                    n["flag_enable"] = 0;
                }
                n["board_id"] = null;//get board type
                WaitForIdle(n);
            }
        }
        internal NodeAgent()
        {
            allports = new List<SPort>();
            weight_nodes = new List<WeighNode>();
            nodemap = new Dictionary<byte, SubNode>();
            rand = new Random();
            CmdIn = null;
            ResultOut = "done";
            packer = null;
            QueryLoopDone = true;
            
            InLock = new object();
            OutLock = new object();


            msg_loop = new Thread(new ThreadStart(MessageLoop));
            msg_loop.IsBackground = true;


            LoadConfiguration();
            
            
        }
        public void Start()
        {
            msg_loop.Start();
            
          
        }
        public void Stop()
        {
        }
        
        private bool WaitUntilGetValue(SubNode n, string regname, UInt32 val)
        {
            int i = 10;
            while (WaitForIdle(n))
            {
                if (n[regname].HasValue && n[regname].Value == val)
                {
                    return true;
                }
                Thread.Sleep(i);
                i = i * 2;
                if (i > 1000)
                {
                    return false;
                }
                n[regname] = null; //ask for value again
            }
            return false;
        }
        /*
         * wait until the read command is responded
         */
        private bool WaitForIdle(SubNode n)
        {
            int i = 4;
            int j = 0;
            while (n.status == NodeStatus.ST_BUSY)
            {
                //Thread.SpinWait(i);
                Thread.Sleep(i);
                i = i + (i+1)/2;
                if (i>500)
                {
                    if (j > 3) //resend for 3 times
                    {
                        n.status = NodeStatus.ST_LOST;
                        return false;
                    }
                    i = 4;
                    n.ResendCommand();
                    j++;
                }
            }
            return true;
        }

        private void DoRelease(byte[] addrs, double weight)
        {

            string log = weight.ToString("F2") + "\t";
            foreach (byte n in addrs)
            {
                WeighNode wn = (nodemap[n] as WeighNode);
                log = log + String.Format("({0}){1}\t", wn.node_id, wn.weight.ToString("F2"));
                
                wn.cnt_match = 0;
                if(wn.errlist != "")
                    wn.errlist.Replace("err_om;", "");
                wn.bRelease = true;
                wn["flag_release"] = 1; //release the packer
            }
            StringResource.dolog(log);
            foreach (byte n in addrs)
            {
                WeighNode wn = (nodemap[n] as WeighNode);
                Thread.Sleep(5);
                WaitUntilGetValue(wn, "flag_release", 1);
                wn.ClearWeight();
                wn["flag_goon"] = 1;
            }
            
            //Thread.Sleep(100);
            //trigger the bottom packer
            int k = 0;
            VibrateNode vn = (nodemap[packer.vib_addr] as VibrateNode);
            vn["hw_status"] = null;
            while (!WaitUntilGetValue(vn, "hw_status", 0))
            {
                if (k++ < 30)
                {
                   Thread.Sleep(100);//todo : add alert
                }
            }
            if (k < 30)
            {
                vn["flag_enable"] = 10;
            }else{
                packer.status = PackerStatus.INERROR;
                return;
            }
//            if (!WaitUntilGetValue(vn, "flag_enable", 10))
//                return;

            //todo update the display;

            onepack o = new onepack();
            o.bucket = new byte[5];
            o.bucket[1] = o.bucket[2] = o.bucket[3] = o.bucket[4] = o.bucket[0] = (byte)0;
            int i = 0;
            foreach (byte n in addrs)
            {
                o.bucket[i++] = n;
            }
            o.time = DateTime.Now;
            o.weight = weight;
            packer.AddNewPack(o);
            
        }
        //one bucket combination
        private bool Caculation1()
        {
            double sum = 0;
            double uvar = packer.curr_cfg.target + packer.curr_cfg.upper_var;
            double dvar = packer.curr_cfg.target - packer.curr_cfg.lower_var;

            for (int i = 0; i < weight_nodes.Count; i++)
            {
                if (weight_nodes[i].status == NodeStatus.ST_LOST)
                    continue;
                sum = weight_nodes[i].weight;
                if (sum > dvar && sum < uvar)
                {
                    DoRelease(new byte[] { weight_nodes[i].node_id }, sum);
                    return true;
                }
            }
            return false;
        }
        private bool Caculation2()
        {
            double sum = 0;
            double uvar = packer.curr_cfg.target + packer.curr_cfg.upper_var;
            double dvar = packer.curr_cfg.target - packer.curr_cfg.lower_var;

            for (int i = 0; i < weight_nodes.Count - 1; i++)
            {
                if (weight_nodes[i].status == NodeStatus.ST_LOST)
                    continue;
                for (int j = i + 1; j < weight_nodes.Count; j++)
                {
                    if (weight_nodes[j].status == NodeStatus.ST_LOST)
                        continue;
                    sum = weight_nodes[i].weight + weight_nodes[j].weight;
                    if (sum > dvar && sum < uvar)
                    {
                        DoRelease(new byte[] { weight_nodes[i].node_id, weight_nodes[j].node_id }, sum);
                        return true;
                    }
                }
            }
            return false;
        }
        private bool Caculation3()
        {
            double sum = 0;
            double uvar = packer.curr_cfg.target + packer.curr_cfg.upper_var;
            double dvar = packer.curr_cfg.target - packer.curr_cfg.lower_var;

            for (int i = 0; i < weight_nodes.Count - 2; i++)
            {
                if (weight_nodes[i].status == NodeStatus.ST_LOST)
                    continue;
                for (int j = i + 1; j < weight_nodes.Count - 1; j++)
                {
                    if (weight_nodes[j].status == NodeStatus.ST_LOST)
                        continue;
                    for (int k = j + 1; k < weight_nodes.Count; k++)
                    {
                        if (weight_nodes[k].status == NodeStatus.ST_LOST)
                            continue;

                        sum = weight_nodes[i].weight + weight_nodes[j].weight + weight_nodes[k].weight;
                        if (sum > dvar && sum < uvar)
                        {
                            DoRelease(new byte[] { weight_nodes[i].node_id, weight_nodes[j].node_id, weight_nodes[k].node_id }, sum);
                            return true;
                        }
                    }
                }
            }
            return false;
        }
        private bool Caculation4()
        {
            double sum = 0;
            double uvar = packer.curr_cfg.target + packer.curr_cfg.upper_var;
            double dvar = packer.curr_cfg.target - packer.curr_cfg.lower_var;

            for (int i = 0; i < weight_nodes.Count - 3; i++)
            {
                if (weight_nodes[i].status == NodeStatus.ST_LOST)
                    continue;
                for (int j = i + 1; j < weight_nodes.Count - 2; j++)
                {
                    if (weight_nodes[j].status == NodeStatus.ST_LOST)
                        continue;
                    for (int k = j + 1; k < weight_nodes.Count - 1; k++)
                    {
                        if (weight_nodes[k].status == NodeStatus.ST_LOST)
                            continue;

                        for (int l = k + 1; l < weight_nodes.Count; l++)
                        {
                            if (weight_nodes[l].status == NodeStatus.ST_LOST)
                                continue;
                            sum = weight_nodes[i].weight + weight_nodes[j].weight + weight_nodes[k].weight + weight_nodes[l].weight;
                            if (sum > dvar && sum < uvar)
                            {
                                DoRelease(new byte[] { weight_nodes[i].node_id, weight_nodes[j].node_id, weight_nodes[k].node_id, weight_nodes[l].node_id }, sum);
                                return true;
                            }
                        }
                    }
                }
            }
            return false;
        }
        private bool Caculation5()
        {
            double sum = 0;
            double uvar = packer.curr_cfg.target + packer.curr_cfg.upper_var;
            double dvar = packer.curr_cfg.target - packer.curr_cfg.lower_var;

            for (int i = 0; i < weight_nodes.Count - 4; i++)
            {
                if (weight_nodes[i].status == NodeStatus.ST_LOST)
                    continue;
                for (int j = i + 1; j < weight_nodes.Count - 3; j++)
                {
                    if (weight_nodes[j].status == NodeStatus.ST_LOST)
                        continue;
                    for (int k = j + 1; k < weight_nodes.Count - 2; k++)
                    {
                        if (weight_nodes[k].status == NodeStatus.ST_LOST)
                            continue;

                        for (int l = k + 1; l < weight_nodes.Count - 1; l++)
                        {
                            if (weight_nodes[l].status == NodeStatus.ST_LOST)
                                continue;
                            for (int m = l + 1; m < weight_nodes.Count; m++)
                            {
                                if (weight_nodes[m].status == NodeStatus.ST_LOST)
                                    continue;

                                sum = weight_nodes[i].weight + weight_nodes[j].weight + weight_nodes[k].weight + weight_nodes[l].weight + weight_nodes[m].weight;
                                if (sum > dvar && sum < uvar)
                                {
                                    DoRelease(new byte[] { weight_nodes[i].node_id, weight_nodes[j].node_id, weight_nodes[k].node_id, weight_nodes[l].node_id, weight_nodes[m].node_id }, sum);
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
            return false;
        }

        public void CheckCombination()
        {
            if (NodeAgent.IsDebug)
            {
                if (rand.NextDouble() > 0.1)
                {
                    DoRelease(new byte[] { weight_nodes[1].node_id, weight_nodes[2].node_id, weight_nodes[3].node_id, weight_nodes[4].node_id, weight_nodes[5].node_id }, 115);
                }
                //                return;
            }
            else
            {
                while (Caculation1())
                {
                }
                while (Caculation5())
                {
                }
                while (Caculation4())
                {
                }
                while (Caculation3())
                {
                }
                while (Caculation2())
                {
                }
            }       
            foreach (WeighNode n in weight_nodes)
            {
                if (n.status == NodeStatus.ST_LOST)
                    continue;
                //no match case
                n.cnt_match++;
                if (n.cnt_match > 10)
                {
                    if (AlertWnd.b_turnon_alert)
                    {
                        if (!AlertWnd.b_manual_reset) //auto reset
                        {
                            if (n.errlist.IndexOf("err_om;") < 0)
                                n.errlist = n.errlist + "err_om;";
                        }
                    }
                }
                //over_weight case
                if ((n.weight < packer.curr_cfg.target/2) && (n.weight > -1000.0))
                {
                    n["flag_goon"] = 1;
                    n["flag_goon"] = null;
                    WaitUntilGetValue(n, "flag_goon", 1);
                }else{
                    if (n.weight > (packer.curr_cfg.target - packer.curr_cfg.lower_var))
                    {
                        if (AlertWnd.b_turnon_alert)
                        {
                            if (n.errlist.IndexOf("err_ow;") < 0)
                                n.errlist = n.errlist + "err_ow;";

                            if (!AlertWnd.b_manual_reset) //auto reset
                            {
                                n["flag_enable"] = 4;
                                n["flag_enable"] = null;
                                WaitUntilGetValue(n, "flag_enable", 4);
                            }
                        }
                    }
                    else
                    {
                        if(n.errlist != "")
                            n.errlist.Replace("err_ow;", "");
                    }
                }
                n.ClearWeight();
            }
            vibnode["flag_enable"] = 5; //more fill
            vibnode["flag_enable"] = null;
        }
        
        private void SetResultOut(string ret)
        {
            lock(OutLock)
            {
                ResultOut = ret;
            }
        }

        public void MessageLoop()
        {
            string action;
            byte addr;
            bool HasNewCmd;
            SubNode node;
            UInt32 enable = 0;
            string ret = "";
            const byte INVALID_ENABLE = 2;
            try
            {
                while (true)
                {
                    HasNewCmd = false;
                    if (CmdIn != null && CmdIn != "") //new command available
                        HasNewCmd = true;

                    if (HasNewCmd) //new command available
                    {
                        lock (InLock)
                        {
                            addr = addrIn;
                            action = CmdIn;
                            CmdIn = "";
                        }
                        #region single_node_command

                        if (addr < 0x80)    //command to single node
                        {
                            node = nodemap[addr];

                            if (node.status == NodeStatus.ST_LOST)
                            {
                                SetResultOut("lost_node");
                                continue;
                            }
                            if (action.IndexOf("getreg") == 0)
                            {
                                string[] param = action.Split(':');
                                node[param[1]] = null;
                                SetResultOut(WaitForIdle(node) ? "" : "fail");
                                continue;
                            }

                            if (action.IndexOf("setreg") == 0)
                            {
                                string[] param = action.Split(':');
                                node[param[1]] = UInt32.Parse(param[2]);
                                SetResultOut(WaitUntilGetValue(node, param[1], UInt32.Parse(param[2])) ? "" : "fail");
                                continue;
                            }
                            if (action == "stop" || action == "start")
                            {
                                if (action == "stop")
                                    enable = 0;
                                if (action == "start")
                                    enable = 0x80;
                                node["flag_enable"] = enable;
                                node["flag_enable"] = null;
                                SetResultOut(WaitUntilGetValue(node, "flag_enable", enable) ? "" : "fail");
                            }
                            if (action == "flash")
                            {
                                //wait until all data is programed.
                                node["NumOfDataToBePgmed"] = 45;
                                Thread.Sleep(1000);
                                node["NumOfDataToBePgmed"] = null;
                                SetResultOut(WaitUntilGetValue(node, "NumOfDataToBePgmed", 0) ? "" : "fail");
                                continue;
                            }
                            if (action == "disable")
                            {
                                node.status = NodeStatus.ST_LOST;
                                node.errlist = "";
                                SetResultOut("");
                                continue;
                            }
                            if ((node is WeighNode))
                            {
                                if (action == "goon")
                                {
                                    enable = 1;
                                    node["flag_goon"] = enable;
                                    node["flag_goon"] = null;
                                    SetResultOut(WaitUntilGetValue(node, "flag_goon", enable) ? "" : "fail");
                                }
                                if (action == "zero" || action == "empty" || action == "stop" || action == "release" ||
                                    action == "pass" || action == "fill")
                                {

                                    if (action == "zero")
                                        enable = 7;
                                    if (action == "empty" || action == "stop" || action == "release")
                                        enable = 4;
                                    if (action == "pass")
                                        enable = 3;
                                    if (action == "fill")
                                        enable = 5;
                                    node["flag_enable"] = enable;
                                    node["flag_enable"] = null;
                                    ret = WaitUntilGetValue(node, "flag_enable", enable) ? "" : "fail";
                                }
                                if (action.IndexOf("calib") == 0) //command format calib1300
                                {
                                    byte slot = byte.Parse(action.Substring("calib".Length, 1));
                                    UInt32 weight = UInt32.Parse(action.Remove(0, "calib1".Length));
                                    ret = "fail";
                                    node["cs_mtrl"] = null;
                                    if (WaitForIdle(node))
                                    {
                                        if (slot == 0) //zero point calibration
                                        {
                                            node["cs_zero"] = node["cs_mtrl"];
                                            ret = WaitUntilGetValue(node, "cs_zero", node["cs_mtrl"].Value) ? "" : "fail";
                                        }
                                        else
                                        {
                                            node["cs_poise" + slot.ToString()] = node["cs_mtrl"];
                                            ret = WaitUntilGetValue(node, "cs_poise" + slot.ToString(), node["cs_mtrl"].Value) ? "" : "fail";
                                            node["poise_weight_gram" + slot.ToString()] = weight;
                                            ret = WaitUntilGetValue(node, "poise_weight_gram" + slot.ToString(), weight) ? ret : "fail";
                                        }
                                    }
                                }
                                SetResultOut(ret);
                                continue;
                            }
                            if (node is VibrateNode)
                            {

                                if (action == "trigger")
                                {
                                    node["flag_enable"] = 10; //trigger the interface
                                    node["flag_enable"] = null;
                                    ret = WaitUntilGetValue(node, "flag_enable", 10) ? "" : "fail";
                                }
                                if (action.IndexOf("interface") == 0) //command format interface1234
                                {
                                    UInt32 val = UInt32.Parse(action.Remove(0, "interface".Length));
                                    node["target_weight"] = val;
                                    ret = WaitUntilGetValue(node, "target_weight", val) ? "" : "fail";
                                    node["flag_enable"] = 9; //initialize the interface
                                    node["flag_enable"] = null;
                                    ret = WaitUntilGetValue(node, "flag_enable", 9) ? ret : "fail";
                                }

                                if (action == "fill")
                                {
                                    node["flag_enable"] = 5;
                                    node["flag_enable"] = null;
                                    ret = WaitUntilGetValue(node, "flag_enable", 5) ? "" : "fail";
                                }
                                SetResultOut(ret);

                                continue;
                            }
                            continue;
                        }
                        #endregion
                        #region group_command1

                        if (addr == 0x81 || addr == 0x82 || addr == 0x83 || addr == 0x80)
                        { //group action address = 0xfe, mode is out first ,then query one by one.
                            //group action address = 0xff, mode is one by one action
                            ret = "";

                            if (action == "zero" || action == "empty" || action == "stop" || action == "release" ||
                                action == "pass" || action == "fill" || action == "start")
                            {
                                enable = INVALID_ENABLE; //just invalid value
                                if (action == "start")
                                    enable = 1;
                                if (action == "stop")
                                    enable = 0;
                                if (action == "zero")
                                    enable = 7;
                                if (action == "empty" || action == "release")
                                    enable = 4;
                                if (action == "pass")
                                    enable = 3;
                                if (action == "fill")
                                    enable = 5;

                                vibnode.writebyte_group_reg(addr,new string[]{"flag_enable"},new byte[]{(byte)enable});
                                foreach (WeighNode wnode in weight_nodes)
                                {
                                    if (action == "clearweight")
                                        wnode.ClearWeight(); //clear the weight value
                                    if (action == "start")
                                        wnode.cnt_match = 0;
                                }
                                if (enable != INVALID_ENABLE)
                                {
                                    //todo send command to group //wnode["flag_enable"] = enable;
                                    ret = "done";
                                }
                            }
                            SetResultOut(ret);
                            continue;
                        }
                        #endregion
                        #region group_command2

                        if (addr == 0xff)
                        {   
                            //group action address = 0xff, mode is one by one action
                            ret = "";
                            foreach (WeighNode wnode in weight_nodes)
                            {
                                if (wnode.status == NodeStatus.ST_LOST)
                                    continue;
                                if (action.IndexOf("getreg") == 0)
                                {
                                    string[] param = action.Split(':');
                                    wnode[param[1]] = null;
                                    ret = WaitForIdle(wnode) ? ret : "fail";
                                    continue;
                                }
                                if (action.IndexOf("setreg") == 0)
                                {
                                    string[] param = action.Split(':');
                                    wnode[param[1]] = UInt32.Parse(param[2]);
                                    ret = (WaitUntilGetValue(wnode, param[1], UInt32.Parse(param[2])) ? ret : "fail");
                                    continue;
                                }
                                if (action == "query")
                                {
                                    if (wnode.weight < 0 || wnode.weight > 65530) //invalid weight
                                    {
                                        wnode.Query();
                                        ret = WaitForIdle(wnode) ? ret : "fail";
                                    }
                                    continue;
                                }
                                if (action == "zero" || action == "empty" || action == "stop" || action == "release" ||
                                    action == "pass" || action == "fill" || action == "start")
                                {
                                    enable = INVALID_ENABLE; //just invalid value
                                    if (action == "start")
                                    {
                                        wnode.cnt_match = 0;
                                        enable = 1;
                                    }
                                    if (action == "stop")
                                        enable = 0;
                                    if (enable != INVALID_ENABLE)
                                    {
                                        wnode["flag_enable"] = enable;
                                        wnode["flag_enable"] = null;
                                        ret = WaitUntilGetValue(wnode, "flag_enable", enable) ? ret : "fail";
                                    }

                                    enable = INVALID_ENABLE; //just invalid value
                                    if (action == "clearweight")
                                        wnode.ClearWeight(); //clear the weight value

                                    if (action == "zero")
                                        enable = 7;
                                    if (action == "empty" || action == "release")
                                        enable = 4;
                                    if (action == "pass")
                                        enable = 3;
                                    if (action == "fill")
                                        enable = 5;
                                    if (enable != INVALID_ENABLE)
                                    {
                                        wnode["flag_enable"] = enable;
                                        wnode["flag_enable"] = null;
                                        ret = WaitUntilGetValue(wnode, "flag_enable", enable) ? ret : "fail";
                                    }
                                }
                            }
                            SetResultOut(ret);
                            continue;
                        }
                        #endregion
                        SetResultOut(ret);
                        continue;
                    }
                    if ((packer == null) || (packer.status == PackerStatus.INERROR))
                    {
                        SetResultOut("lost_packer");
                        continue;
                    }
                    ret = "";
                    foreach (WeighNode wn in weight_nodes)
                    {
                        if (wn.status == NodeStatus.ST_LOST)
                            continue;

                        if (QueryLoopDone == true || (wn.weight < -1000 || wn.weight > 65530)) //invalid weight
                        {
                            wn.Query();
                            ret = WaitForIdle(wn) ? ret : "fail";
                        }
                        if (CmdIn != "")
                            break;
                    }

                    QueryLoopDone = true;
                    foreach (WeighNode wn in weight_nodes)
                    {
                        if (wn.weight < -1000 || wn.weight > 65530)
                        {
                            QueryLoopDone = false;
                        }

                    }
                    if (!QueryLoopDone)
                        continue;

                    //check whether bottom packer is ready
                    if (ret == "" && (packer.status == PackerStatus.RUNNING))
                    {
                        CheckCombination();
                    }
                    QueryLoopDone = true;
                    packer.NeedRefresh = true;
                    //SetResultOut(ret);
                    continue;
                }
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message + e.StackTrace + e.Source);
            }
        }
    }
}
