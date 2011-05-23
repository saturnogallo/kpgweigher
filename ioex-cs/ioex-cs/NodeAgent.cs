using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO.Ports;
using System.Xml.Linq;
using System.Windows.Forms;
using System.Diagnostics;
namespace ioex_cs
{
    class NodeAgent
    {
        public static bool IsDebug = false;

        const byte INVALID_ENABLE = 2;
        internal List<WeighNode> weight_nodes; //list of all weight nodes
        internal List<SPort> allports; //list of all serial ports
        internal Dictionary<byte, SubNode> nodemap;//map the address to node
        internal List<VibrateNode> vib_nodes;
        public bool bBootDone = false;
        public bool bActionMode = false;
        public SubNode missingnode;   //node used to find the newly installed board
        private byte[] release_addrs = null;
        private double release_weight;
        private UInt32 release_cnt;
        private UInt32 release_timeout = 0;
        private byte addrIn;    //address used to store the incoming command address
        private string CmdIn;   //string used to store the incoming command
        public string ResultOut; //string used to store the feedback
        private object InLock;
        private object OutLock;
        private Thread msg_loop;
        private bool QueryLoopDone; //indicate the completion of loop query
        public const int VIB_INIDLE = 0;
        public const int VIB_READY = 1;
        public const int VIB_WORKING = 2;
        public const int VIB_ERROR = 3;
        public int VibStatus;  
        private double llim;
        public UIPacker packer; //reference to pack
        Random rand;
        int checkcnt = 0;
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
            if(ns == NodeStatus.ST_LOST)
                Action(addr, "disable");
            if (ns == NodeStatus.ST_IDLE)
                Action(addr, "enable");
            
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
        public bool search(SubNode n)
        {
            n["addr"] = null;
            if (!WaitForIdleQuick(n))
            {
                n.status = NodeStatus.ST_LOST;
                return false;
            }
             n["board_id"] = null;//get board type
            WaitForIdleQuick(n);
            return true;
        }
        public void InvalidNodeReg(byte addr, string regname)
        {
            if (!nodemap.ContainsKey(addr))
                return;
            if (nodemap[addr].status == NodeStatus.ST_LOST)
                return;
            bActionMode = true;
            nodemap[addr].status = NodeStatus.ST_IDLE;
            nodemap[addr][regname] = null;
            WaitForIdle(nodemap[addr]);
            bActionMode = false;
        }
        public string GetNodeReg(byte addr,string regname)
        {
             if (!nodemap.ContainsKey(addr))
                 return "";
             if (nodemap[addr].status == NodeStatus.ST_LOST)
                 return "";
             if (!nodemap[addr][regname].HasValue)
             {
                 Action(addr, "getreg:" + regname);
                 if (ResultOut != "")
                 {
                     //MessageBox.Show("Failed to download config, please try again");
                     throw new Exception("Failed to read config, please try again");
                 }
             }
             return nodemap[addr][regname].ToString();
        }
        public void Action(byte addr, string command)
        {
            if ((addr < 0x80) && (!nodemap.ContainsKey(addr)))
                return;
            bActionMode = true;
            int tout = 0;
            while (true)
            {
                tout = 0;
                lock (InLock)
                {
                    ResultOut = null;
                    addrIn = addr;
                    CmdIn = command;
                }

                while ((ResultOut == null) && (tout++ < 1000))
                {
                    //Thread.SpinWait(1);
                    Thread.Sleep(2);
                }
                if (tout < 1000)
                {
                    bActionMode = false;
                    return;
                }
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
                    nodemap[byte.Parse(node)] = new VibrateNode(allports[byte.Parse(com)], byte.Parse(node));
                    vib_nodes.Add(nodemap[byte.Parse(node)] as VibrateNode);
                }
                if (type == "weight")
                {
                    nodemap[byte.Parse(node)] = new WeighNode(allports[byte.Parse(com)], byte.Parse(node));
                    weight_nodes.Add(nodemap[byte.Parse(node)] as WeighNode);
                }
            }
            missingnode = new WeighNode(allports[0], byte.Parse(def_cfg.Element("def_addr").Value)); //36 is the default address of unassigned address board
        }
        public void progress(uint i)
        {
        }
        internal NodeAgent()
        {
            allports = new List<SPort>();
            weight_nodes = new List<WeighNode>();
            vib_nodes = new List<VibrateNode>();
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
        public void Resume()
        {
            msg_loop.Resume();
        }
        public void Start()
        {
            msg_loop.Start();
            
          
        }
        public void Stop()
        {
            msg_loop.Suspend();
        }
        
        private bool WaitUntilGetValue(SubNode n, string regname, UInt32 val)
        {
            int i = 5;
            int j = 0;

            n.status = NodeStatus.ST_IDLE;
            n[regname] = null;
            Thread.Sleep(15);

            
            
            while (WaitForIdleQuick(n))
            {
                if (n[regname].HasValue && n[regname].Value == val)
                {
                    return true;
                }
                Thread.Sleep(i);
                i = i * 2;
                j = j + 1;
                if (i > 4000)
                {
                    return false;
                }
                if(j % 6 == 5)
                    n[regname] = null; //ask for value again
            }
            return false;
        }
        /*
         * wait until the read command is responded
         */
        private bool WaitForIdle(SubNode n)
        {
            int i = 2;
            int j = 0;
            while (n.status == NodeStatus.ST_BUSY)
            {
                Thread.Sleep(i);
                i = i + (i+1)/2;
                if (i>400)
                {
                    if (j > 10) //resend for 3 times
                    {
                        n.status = NodeStatus.ST_LOST;
                        return false;
                    }
                    i = 4;
                    if(j % 4 == 3)
                        n.ResendCommand();
                    j++;
                }
            }
            return true;
        }
        private bool WaitForIdleQuick(SubNode n)
        {
            int i = 2;
            int j = 0;
            while (n.status == NodeStatus.ST_BUSY)
            {
                Thread.Sleep(i);
                i = i + (i + 1) / 2;
                if (i > 10)
                {
                    if (j > 3) //resend for 3 times
                    {
                        return false;
                    }
                    i = 4;
                    if (j % 3 == 2)
                        n.ResendCommand();
                    j++;
                }
            }
            return true;
        }
        private void DoRelease(byte[] addrs, double weight)
        {
            release_addrs = addrs;
            release_weight = weight;
        }
        private bool WaitPackerReady()
        {
            VibrateNode vn = (nodemap[packer.vib_addr] as VibrateNode);
            vn["hw_status"] = null;

            WaitForIdle(vn);
            if ((!vn["hw_status"].HasValue) || (vn["hw_status"].Value != 0))
            {
                Thread.Sleep(20);//todo : add alert
                return false;
            }
            return true;
        }
        private void TriggerPacker()
        {
            VibrateNode vn = (nodemap[packer.vib_addr] as VibrateNode);
            if ((packer.total_packs % (vn.intf_byte.feed_times + 1) != 0))
                return;
            

            
            vn["flag_enable"] = 10;
            WaitUntilFlagSent(vn);
        }
        private bool WaitUntilFlagSent(SubNode n)
        {
            if (IsDebug)
                return true;
            string reg = "cs_sys_offset_cal_data_lw";
            int k = 1000;
            while(k-- > 0)
            {
                Thread.Sleep(10);
                n[reg] = null;
                WaitForIdleQuick(n);
                if (n[reg].HasValue)
                {
                    uint lw_lb = n[reg].Value % 256;
                    if (( lw_lb) != n.flag_cnt)
                    {
                        n.flag_cnt = lw_lb;
                        return true;
                    }
                }
                Thread.Sleep(10);
                n.ResendCommand();
                
            }
            n.status = NodeStatus.ST_LOST;
            return false;
        }
        private bool WaitPackerDone()
        {
            VibrateNode vn = (nodemap[packer.vib_addr] as VibrateNode);
            vn["cs_sys_offset_cal_data_lw"] = null;
            WaitForIdleQuick(vn);
            if (vn["cs_sys_offset_cal_data_lw"].HasValue)
            {
                if((packer.total_packs % (vn.intf_byte.feed_times+1) != 0))
                {
                    return true;
                }
                uint lw_ub = vn["cs_sys_offset_cal_data_lw"].Value >>8;
                if(lw_ub != release_cnt)
                {
                    release_cnt = lw_ub;
                    return true;
                }
            }
            if (release_timeout++ < 1000)
            {
                Thread.Sleep(5);
                if ((release_timeout % 100) == 49)
                {
                    vn.status = NodeStatus.ST_IDLE;
                    vn["cs_sys_offset_cal_data_lw"] = null;
                }
            }
            else
            {
                release_timeout = 0;
                VibStatus = VIB_READY; //retry by sending the command
            }
            return false;
        }
        private void ReleaseAction(byte[] addrs, double weight)
        {
            string log = weight.ToString("F2") + "\t";
/*
            string log2 = "";
            foreach (byte n in addrs)
            {
                WeighNode wn = (nodemap[n] as WeighNode);
                log2 = log2 + String.Format("({0}){1}\t", wn.node_id, wn.weight.ToString("F2"));
            }


            foreach (byte n in addrs)
            {
                WeighNode wn = (nodemap[n] as WeighNode);
                log = log + String.Format("({0}#){1}\t", wn.node_id, wn.weight.ToString("F2"));
            }
  
            StringResource.dolog(log);
 */
            log = weight.ToString("F2") + "\t";
            
            foreach(byte n in addrs)
            {
                WeighNode wn = (nodemap[n] as WeighNode);
                log = log + String.Format("({0}){1}\t", wn.node_id, wn.weight.ToString("F2"));
                
                wn.cnt_match = 0;
                if(wn.errlist != "")
                    wn.errlist.Replace("err_om;", "");

                Debug.Write('(');
                WaitUntilGetValue(wn, "flag_release", 0);
                Debug.Write(')');

                wn.bRelease = true;
                wn["flag_release"] = 1; //release the packer
                WaitUntilFlagSent(wn);
            }
            StringResource.dolog(log);
            Thread.Sleep(150);
            foreach (byte n in addrs)
            {
                WeighNode wn = (nodemap[n] as WeighNode);
                wn.ClearWeight();
            }
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
                if (weight_nodes[i].weight <= llim)
                    continue;

                for (int j = i + 1; j < weight_nodes.Count; j++)
                {
                    if (weight_nodes[j].status == NodeStatus.ST_LOST)
                        continue;
                    if (weight_nodes[j].weight <= llim)
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
                if (weight_nodes[i].weight <= llim)
                    continue;

                for (int j = i + 1; j < weight_nodes.Count - 1; j++)
                {
                    if (weight_nodes[j].status == NodeStatus.ST_LOST)
                        continue;
                    if (weight_nodes[j].weight <= llim)
                        continue;

                    for (int k = j + 1; k < weight_nodes.Count; k++)
                    {
                        if (weight_nodes[k].status == NodeStatus.ST_LOST)
                            continue;
                        if (weight_nodes[k].weight <= llim)
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
                if (weight_nodes[i].weight <= llim)
                    continue;

                for (int j = i + 1; j < weight_nodes.Count - 2; j++)
                {
                    if (weight_nodes[j].status == NodeStatus.ST_LOST)
                        continue;
                    if (weight_nodes[j].weight <= llim)
                        continue;

                    for (int k = j + 1; k < weight_nodes.Count - 1; k++)
                    {
                        if (weight_nodes[k].status == NodeStatus.ST_LOST)
                            continue;
                        if (weight_nodes[k].weight <= llim)
                            continue;

                        for (int l = k + 1; l < weight_nodes.Count; l++)
                        {
                            if (weight_nodes[l].status == NodeStatus.ST_LOST)
                                continue;
                            if (weight_nodes[l].weight <= llim)
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
                if (weight_nodes[i].weight <= llim)
                    continue;

                for (int j = i + 1; j < weight_nodes.Count - 3; j++)
                {
                    if (weight_nodes[j].status == NodeStatus.ST_LOST)
                        continue;
                    if (weight_nodes[j].weight <= llim)
                        continue;

                    for (int k = j + 1; k < weight_nodes.Count - 2; k++)
                    {
                        if (weight_nodes[k].status == NodeStatus.ST_LOST)
                            continue;
                        if (weight_nodes[k].weight <= llim)
                            continue;

                        for (int l = k + 1; l < weight_nodes.Count - 1; l++)
                        {
                            if (weight_nodes[l].status == NodeStatus.ST_LOST)
                                continue;
                            if (weight_nodes[l].weight <= llim)
                                continue;

                            for (int m = l + 1; m < weight_nodes.Count; m++)
                            {
                                if (weight_nodes[m].status == NodeStatus.ST_LOST)
                                    continue;
                                if (weight_nodes[m].weight <= llim)
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
        public void ResetStatus()
        {
            VibrateNode vn = (nodemap[packer.vib_addr] as VibrateNode);
            if (vn["cs_sys_offset_cal_data_uw"].HasValue)
                release_cnt = vn["cs_sys_offset_cal_data_uw"].Value;
            else
            {
                release_cnt = 0;
                //vn["cs_sys_offset_cal_data_uw"] = 0;
            }
            release_addrs = null;
            release_timeout = 0;
            VibStatus = VIB_INIDLE;
            SetResultOut("");
        }
        public void CheckNodeStatus()
        {
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
                            if (n.errlist.IndexOf("err_om;") < 0)   //no matchinf for serveral times
                                n.errlist = n.errlist + "err_om;";
                        }
                    }
                }
                //over_weight case
                if ((n.weight < packer.curr_cfg.target / 2) && (n.weight > -1000.0))
                {
                    n["flag_goon"] = 1;
                    WaitUntilFlagSent(n);
                }
                else
                {
                    double nw = n.weight;
                    if (nw > (packer.curr_cfg.target - packer.curr_cfg.lower_var) && nw < 65521)
                    {
                        if (AlertWnd.b_turnon_alert)
                        {
                            if (n.errlist.IndexOf("err_ow;") < 0)
                                n.errlist = n.errlist + "err_ow;";

                            if (!AlertWnd.b_manual_reset) //auto reset
                            {
                                n["flag_enable"] = 4;
                                WaitUntilFlagSent(n);
                            }
                        }
                    }
                    else
                    {
                        if (nw < 65521)
                        {
                            if (n.errlist != "")
                                n.errlist.Replace("err_ow;", "");//reset overweight error
                        }
                    }
                }
            }

        }
        public bool CheckCombination()  //return whether there is hit or not.
        {
            if (NodeAgent.IsDebug)
            {
                if (rand.NextDouble() > 0.1)
                {
                    DoRelease(new byte[] { weight_nodes[1].node_id, weight_nodes[2].node_id, weight_nodes[3].node_id, weight_nodes[4].node_id, weight_nodes[5].node_id }, 115);
                    return true;
                }
                return false;
            }
            else
            {
                llim = packer.curr_cfg.lower_var;
                if (packer.curr_cfg.upper_var < llim)
                    llim = packer.curr_cfg.upper_var;
                llim = llim / 2;

                if (Caculation3() || Caculation4() || Caculation5() || Caculation1() || Caculation2())
                {
                    return true;
                    
                }
                
                
                packer.last_one_pack.bucket = null;
                return false;
            }       
        }
        
        private void SetResultOut(string ret)
        {
            lock(OutLock)
            {
                ResultOut = ret;
            }
        }

        private void single_command(byte addr, string action)
        {
            string ret = "";
            uint enable;
            SubNode node = nodemap[addr];
            if (action == "enable")
            {
                node.status = NodeStatus.ST_IDLE;
                node.errlist = "";
                SetResultOut("");
                return;
            }
            if (node.status == NodeStatus.ST_LOST)
            {
                SetResultOut("lost_node");
                return;
            }
            if (action.IndexOf("getreg") == 0)
            {
                string[] param = action.Split(':');
                node[param[1]] = null;
                if (!WaitForIdle(node))
                    node.status = NodeStatus.ST_LOST;
                SetResultOut("");
                return;
            }

            if (action.IndexOf("setreg") == 0)
            {
                string[] param = action.Split(':');
                node[param[1]] = UInt32.Parse(param[2]);
                if(!WaitUntilGetValue(node, param[1], UInt32.Parse(param[2])))
                    node.status = NodeStatus.ST_LOST;
                SetResultOut("");
                return;
            }
            if (action == "stop" || action == "start")
            {
                if (action == "stop")
                    node["flag_enable"] = 0;
                if (action == "start")
                    node["flag_enable"] = 0x80;

                WaitUntilFlagSent(node);
                SetResultOut("");    
                return;
            }
            if (action == "flash")
            {
                //wait until all data is programed.
                node["NumOfDataToBePgmed"] = 45;
                Thread.Sleep(1000);

                if (!WaitUntilGetValue(node, "NumOfDataToBePgmed", 0))
                    MessageBox.Show("burnning failed.");
                SetResultOut("");
                return;
            }
            if (action == "disable")
            {
                node.status = NodeStatus.ST_LOST;
                node.errlist = "";
                SetResultOut("");
                return;
            }

            if ((node is WeighNode))
            {
                if (action == "goon")
                {
                    enable = 1;
                    node["flag_goon"] = enable;
                    WaitUntilFlagSent(node);
                }
                if (action == "zero" || action == "empty" || action == "stop" || action == "release" ||
                    action == "pass" || action == "fill")
                {
                    enable = INVALID_ENABLE;
                    if (action == "zero")
                        enable = 7;
                    if (action == "empty" || action == "stop" || action == "release")
                        enable = 4;
                    if (action == "pass")
                        enable = 3;
                    if (action == "fill")
                        enable = 5;
                    node["flag_enable"] = enable;
                    WaitUntilFlagSent(node);
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
                            if (!WaitUntilGetValue(node, "cs_poise" + slot.ToString(), node["cs_mtrl"].Value))
                                node.status = NodeStatus.ST_LOST;
                            node["poise_weight_gram" + slot.ToString()] = weight;
                            if(!WaitUntilGetValue(node, "poise_weight_gram" + slot.ToString(), weight))
                                node.status = NodeStatus.ST_LOST;
                        }
                    }
                }
                SetResultOut(ret);
                return;
            }
            if (node is VibrateNode)
            {

                if (action == "trigger")
                {
                    node["flag_enable"] = 10; //trigger the interface
                    WaitUntilFlagSent(node);
                }
                if (action.IndexOf("interface") == 0) //command format interface1234
                {
                    UInt32 val = UInt32.Parse(action.Remove(0, "interface".Length));
                    node["target_weight"] = val;
                    Thread.Sleep(25);
                    //ret = WaitUntilGetValue(node, "target_weight", val) ? "" : "fail";
                    node["flag_enable"] = 8; //initialize the interface
                    WaitUntilFlagSent(node);
                }

                if (action == "fill")
                {
                    node["flag_enable"] = 5;
                    WaitUntilFlagSent(node);
                }
                SetResultOut(ret);

                return;
            }
        }
        
        private void group_command1(byte addr, string action) //command send to group address
        {
            string ret = "";
            uint enable;
            if (action == "zero" || action == "empty" || action == "stop" || action == "release" ||
                action == "pass" || action == "fill" || action == "start")
            {
                enable = INVALID_ENABLE; //just invalid value
                if (action == "start")
                {
                    enable = 1;
                }
                if (action == "stop")
                {
                    enable = 0;
                }
                if (action == "zero")
                    enable = 7;
                if (action == "empty" || action == "release")
                    enable = 4;
                if (action == "pass")
                    enable = 3;
                if (action == "fill")
                    enable = 5;

                vib_nodes[0].writebyte_group_reg(addr, new string[] { "flag_enable" }, new byte[] { (byte)enable });
                foreach (WeighNode wnode in weight_nodes)
                {
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
            return ;

        }
        private void update_vibstatus()
        {
            if (packer.status == PackerStatus.RUNNING)
            {
                if (VibStatus == VIB_WORKING)
                {
                    if (WaitPackerDone()) //check pack count
                    {
                        VibStatus = VIB_INIDLE;
                        Debug.WriteLine("inidle");
                    }
                }
            }
            if (VibStatus == VIB_INIDLE)
            {
                if (WaitPackerReady())//hw_status == 0
                {
                    Debug.WriteLine("inready");
                    VibStatus = VIB_READY;
                }
            }

        }
        private bool check_weights_ready()
        {
            bool ret = false;
            foreach (WeighNode wn in weight_nodes)
            {
                if (wn.status == NodeStatus.ST_LOST)
                    continue;

                if (wn.weight < -1000 ) //invalid weight || wn.weight >= 65521
                {
                    wn.Query();
                    WaitForIdleQuick(wn);
                }
                if (CmdIn != "")
                    return false;
            }
            ret = true;
            foreach (WeighNode wn in weight_nodes)
            {
                if (wn.weight < -1000) //|| wn.weight >= 65521
                {
                    Debug.WriteLine("re" + wn.node_id);
                    Thread.Sleep(2);
                    ret = false;
                    break;
                }
            }
            return ret;

        }
        private void clearweights()
        {
            foreach (WeighNode wnode in weight_nodes)
                wnode.ClearWeight(); //clear the weight value
        }
        public void MessageLoop()
        {
            string action;
            byte addr;
            bool HasNewCmd;
            SubNode node;
            UInt32 enable = 0;
            string ret = "";
            
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
                        

                        if (addr < 0x80)    //command to single node
                        {
                            single_command(addr, action);
                            continue;
                        }
                        if (addr == 0x81 || addr == 0x82 || addr == 0x83 || addr == 0x80)
                        { //group action address = 0xfe, mode is out first ,then query one by one.
                           
                            group_command1(addr, action);
                            continue;
                        }

                        SetResultOut("");
                        continue;
                    }
                    if ((packer == null) || (packer.status == PackerStatus.INERROR))
                    {
                        SetResultOut("lost_packer");
                        continue;
                    }
                    if (!bBootDone || bActionMode)
                        continue;

                    if (packer.status != PackerStatus.RUNNING)
                    {
                        if (packer.NeedRefresh == false)
                        {
                            if (check_weights_ready())
                            {
                                Debug.Write(".");
                                packer.NeedRefresh = true;
                            }
                            else
                            {
                                
                                if(checkcnt++ > 6)
                                {
                                    packer.NeedRefresh = true;
                                    checkcnt = 0;
                                }
                            }
                        }
                        update_vibstatus();
                        continue;
                    }else{
                        if (VibStatus != VIB_READY)
                        {
                            update_vibstatus();
                            continue;
                        }
                        clearweights();
                        //running mode
                        while ((!check_weights_ready()) && (CmdIn == ""))
                        {
                            
                            Thread.Sleep(25);
                        }
                        if (CmdIn != "")
                            continue;
                        
                        while(CheckCombination())
                        {
                             ReleaseAction(release_addrs, release_weight); //send release command, send goon command and clear weight
                             packer.NeedRefresh = true;
                             TriggerPacker();
                             Thread.Sleep(25); //wait until packer is working
                             VibStatus = VIB_WORKING;
                             Debug.WriteLine("packing");
                             while(VibStatus == VIB_WORKING && (CmdIn == ""))
                             {
                                 Thread.Sleep(25);
                                 update_vibstatus();
                             }
                             if (CmdIn != "")
                                 break;
                        }
                        if (CmdIn != "")
                            continue;

                        packer.NeedRefresh = true;
                        CheckNodeStatus();//send goon for remaining nodes and check for overweight errors
                        vib_nodes[0]["flag_enable"] = 5; //more fill
                        WaitUntilFlagSent(vib_nodes[0]);
                        clearweights();
                        Thread.Sleep(150);
                        clearweights();
                        
                        
                    }
                }
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message + e.StackTrace + e.Source);
            }
        }
    }
}
