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
    public class CombineEventArgs : EventArgs
    {
        public CombineEventArgs(byte packer_id, byte[] release_addrs, double weight)
        {
            this.packer_id = packer_id;
            this.release_addrs = release_addrs;
            this.weight = weight;
        }
        public byte packer_id;
        public byte[] release_addrs;
        public double weight;
    }	//end of class CombineEventArgs



    class NodeCombination   //the class is used to do combinations and release action
    {
        private byte[] release_addrs = null;
        private double release_weight;
        private VibStatus vibstate = VibStatus.VIB_INIDLE;
        private double llim
        { //low limit of packer setting
            get
            {
                if (packer.curr_cfg.upper_var < packer.curr_cfg.lower_var)
                    return packer.curr_cfg.upper_var / 2;
                else
                    return packer.curr_cfg.lower_var / 2;
                
            }
        } 
        private UIPacker packer; //reference to pack
        
        private double uvar
        {
            get
            {
                return packer.curr_cfg.target + packer.curr_cfg.upper_var;
            }
        }
        private double dvar
        {
            get
            {
                return packer.curr_cfg.target - packer.curr_cfg.lower_var;
            }
        }

        private Random rand; //generate weight for debug use
        public NodeAgent agent; //reference to agent
        private Thread comb_loop; //thread for combination loop

        public delegate void HitCombineEventHandler(object sender, CombineEventArgs ce);
        public event HitCombineEventHandler HitEvent;

        private bool bRun;
        internal NodeCombination(UIPacker p)
        {
            packer = p;
            agent = p.agent;
            bRun = false;
            bSimNodeValid = new Dictionary<byte,bool>();
            foreach (byte i in packer.weight_nodes)
                bSimNodeValid[i] = true;
            bSimCombine = false;
            rand = new Random();
            comb_loop = new Thread(new ThreadStart(CombinationLoop));
            comb_loop.IsBackground = true;

        }
        public void Start()
        {
            bRun = true;
            comb_loop.Start();
        }
        private bool bRunCmd = false;
        public void Stop(int ms)
        {
            bRun = false;
            bRunCmd = true;
            while (bRunCmd)
                Thread.Sleep(10);
            
//          comb_loop.Suspend();
        }
        public void Resume()
        {
            bRun = true;
//          comb_loop.Resume();
        }
        public void ResetStatus()
        {
            agent.ClearWeights();
            vibstate = agent.ResetVibStatus(packer.vib_addr);
        }
        public void CheckNodeStatus() //update node status after combination is completed
        {
            double wt;
            foreach (byte addr in packer.weight_nodes)
            {
                if (agent.GetStatus(addr) == NodeStatus.ST_LOST || agent.GetStatus(addr) == NodeStatus.ST_DISABLED)
                    continue;
                //no match case
                agent.IncMatchCount(addr);
                //over_weight check
                wt = agent.weight(addr);
                if ((wt < packer.curr_cfg.target / 2) && (wt > -1000.0) && (wt < 65521))
                    continue; //goon nodes
                {
                    double nw = wt;
                    if (nw > (packer.curr_cfg.target - packer.curr_cfg.lower_var) && (nw < 65521))
                    {
                        if (AlertWnd.b_turnon_alert)
                        {
                            agent.SetOverWeight(addr, true);
                            if (!AlertWnd.b_manual_reset) //auto reset
                            {
                                agent.Action(addr, "empty");
                            }
                        }
                    }
                    else
                    {
                        if (nw < 65521)
                        {
                            agent.SetOverWeight(addr,false);
                        }
                    }
                }
            }
        }
        private bool ProcessGoonNodes() //send command to nodes that needs to goon
        {
            foreach (byte addr in bSimNodeValid.Keys)
            {
                if (bSimNodeValid[addr] == false)
                    continue;
                if (agent.GetStatus(addr) == NodeStatus.ST_LOST || agent.GetStatus(addr) == NodeStatus.ST_DISABLED)
                    continue;
                double wt = agent.weight(addr);
                if ((wt < packer.curr_cfg.target / 2) && (wt >= 0.0) && (wt < 65521))
                {
                    agent.Action(addr, "flag_goon"); //no match hit
                }
                else
                {
                    if ((wt > packer.curr_cfg.target / 2 ) && (wt < 65521))
                        wt = 100;
                }
                bSimNodeValid[addr] = false;
                return true;
            }
            return false;
        }
        public static void logTimeStick(TimeSpan ts1,string step)
        {
            return;
            TimeSpan ts2 = new TimeSpan(DateTime.Now.Ticks);
            TimeSpan ts = ts2.Subtract(ts1).Duration();
            Debug.WriteLine(step+ts.Milliseconds + "ms");
            
        }
        private void CombinationLoop()
        {
            while (true)
            {
                if (!bRun)
                {
                    if (bRunCmd)
                        bRunCmd = false;
                    Thread.Sleep(100);
                    continue;
                }
                
                if (packer.status == PackerStatus.RUNNING)
                {
                    if (agent.bWeightReady)
                    {
                        TimeSpan ts1 = new TimeSpan(DateTime.Now.Ticks);
                        bSimCombine = true;
                        foreach(byte b in packer.weight_nodes)
                            bSimNodeValid[b] = true;
                        while (CheckCombination())
                            ;
                        bSimCombine = false;

                        while (CheckCombination())
                        {
                            //logTimeStick(ts1, "comb:");    
                            HitEvent(this, new CombineEventArgs((byte)packer._pack_id, release_addrs, release_weight));
                            
                            while (vibstate != VibStatus.VIB_READY)
                            {
                                vibstate = agent.UpdateVibStatus(packer.vib_addr,vibstate);
                                ProcessGoonNodes();
                            }
                            //logTimeStick(ts1, "vibr:");    
                            ReleaseAction(release_addrs, release_weight); //send release command and clear weight, trigger the packer, goon the nodes
                            //logTimeStick(ts1, "rele:");    
                            vibstate = agent.TriggerPacker(packer.vib_addr);
                            while (vibstate == VibStatus.VIB_WORKING)
                            {
                                vibstate = agent.UpdateVibStatus(packer.vib_addr,vibstate);
                                ProcessGoonNodes();
                            }
                            //logTimeStick(ts1, "vibw:");    
                            ProcessGoonNodes();
                        }
                        while (ProcessGoonNodes())
                            ;
                        CheckNodeStatus();

                        agent.ClearWeights();
                        agent.bWeightReady = false;
                        agent.Action(packer.vib_addr, "fill");
                        logTimeStick(ts1, "fina:");    
                    }
                }
                else
                {
                  
                    if (vibstate != VibStatus.VIB_READY)
                    {
                        vibstate = agent.UpdateVibStatus(packer.vib_addr,vibstate);
                        
                    }
                }
                Thread.Sleep(5);
            }
        }
        public double weight(byte addr)
        {
            return agent.weight(addr);
        }
        private void DoRelease(byte[] addrs, double weight)
        {
            if (bSimCombine)
            {
                foreach (byte addr in addrs)
                    bSimNodeValid[addr] = false;
            }
            else
            {
                release_addrs = addrs;
                release_weight = weight;
            }
            
        }
        private bool bSimCombine; //simulate the combination to see whether there is node need to goon ealier.
        private Dictionary<byte,bool> bSimNodeValid; //array used to store the state of each node in simulation mode;
        //one bucket combination
        private bool CacuInvalidNode(int i)
        {
            if (agent.GetStatus(packer.weight_nodes[i]) == NodeStatus.ST_LOST)
                return true;
            if (agent.GetStatus(packer.weight_nodes[i]) == NodeStatus.ST_DISABLED)
                return true;

            if ((bSimCombine) && (!bSimNodeValid[packer.weight_nodes[i]]))
                return true;
            double wt = agent.weight(packer.weight_nodes[i]);
            if (wt <= llim)
                return true;
            if (wt < 0)
                return true;
            if (wt >= 65521)
                return true;
            return false;

        }
        private bool Caculation1()
        {
            double sum = 0;
            
            for (int i = 0; i < packer.weight_nodes.Count; i++)
            {
                if (CacuInvalidNode(i))
                    continue;
                sum = agent.weight(packer.weight_nodes[i]);
                if (sum > dvar && sum < uvar)
                {
                    DoRelease(new byte[] { packer.weight_nodes[i] }, sum);
                    return true;
                }
            }
            return false;
        }
        private bool Caculation2()
        {
            double sum = 0;

            for (int i = 0; i < packer.weight_nodes.Count - 1; i++)
            {
                if (CacuInvalidNode(i))
                    continue;

                for (int j = i + 1; j < packer.weight_nodes.Count; j++)
                {
                    if (CacuInvalidNode(j))
                        continue;

                    sum = agent.weight(packer.weight_nodes[i]) + agent.weight(packer.weight_nodes[j]);
                    if (sum > dvar && sum < uvar)
                    {
                        DoRelease(new byte[] { packer.weight_nodes[i], packer.weight_nodes[j]}, sum);
                        return true;
                    }
                }
            }
            return false;
        }
        private bool Caculation3()
        {
            double sum = 0;

            for (int i = 0; i < packer.weight_nodes.Count - 2; i++)
            {
                if (CacuInvalidNode(i))
                    continue;

                for (int j = i + 1; j < packer.weight_nodes.Count - 1; j++)
                {
                    if (CacuInvalidNode(j))
                        continue;


                    for (int k = j + 1; k < packer.weight_nodes.Count; k++)
                    {
                        if (CacuInvalidNode(k))
                            continue;


                        sum = agent.weight(packer.weight_nodes[i]) + agent.weight(packer.weight_nodes[j]) + agent.weight(packer.weight_nodes[k]);
                        if (sum > dvar && sum < uvar)
                        {
                            DoRelease(new byte[] { packer.weight_nodes[i], packer.weight_nodes[j], packer.weight_nodes[k] }, sum);
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

            for (int i = 0; i < packer.weight_nodes.Count - 3; i++)
            {
                if (CacuInvalidNode(i))
                    continue;


                for (int j = i + 1; j < packer.weight_nodes.Count - 2; j++)
                {
                    if (CacuInvalidNode(j))
                        continue;

                    for (int k = j + 1; k < packer.weight_nodes.Count - 1; k++)
                    {
                        if (CacuInvalidNode(k))
                            continue;

                        for (int l = k + 1; l < packer.weight_nodes.Count; l++)
                        {
                            if (CacuInvalidNode(l))
                                continue;

                            sum = agent.weight(packer.weight_nodes[i]) + agent.weight(packer.weight_nodes[j]) + agent.weight(packer.weight_nodes[k]) + agent.weight(packer.weight_nodes[l]);
                            if (sum > dvar && sum < uvar)
                            {
                                DoRelease(new byte[] { packer.weight_nodes[i], packer.weight_nodes[j], packer.weight_nodes[k], packer.weight_nodes[l] }, sum);
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

            for (int i = 0; i < packer.weight_nodes.Count - 4; i++)
            {
                if (CacuInvalidNode(i))
                    continue;

                for (int j = i + 1; j < packer.weight_nodes.Count - 3; j++)
                {
                    if (CacuInvalidNode(j))
                        continue;

                    for (int k = j + 1; k < packer.weight_nodes.Count - 2; k++)
                    {
                        if (CacuInvalidNode(k))
                            continue;

                        for (int l = k + 1; l < packer.weight_nodes.Count - 1; l++)
                        {
                            if (CacuInvalidNode(l))
                                continue;

                            for (int m = l + 1; m < packer.weight_nodes.Count; m++)
                            {
                                if (CacuInvalidNode(m))
                                    continue;

                                sum = agent.weight(packer.weight_nodes[i]) + agent.weight(packer.weight_nodes[j]) + agent.weight(packer.weight_nodes[k]) + agent.weight(packer.weight_nodes[l]) + agent.weight(packer.weight_nodes[m]);
                                if (sum > dvar && sum < uvar)
                                {
                                    DoRelease(new byte[] { packer.weight_nodes[i], packer.weight_nodes[j], packer.weight_nodes[k], packer.weight_nodes[l], packer.weight_nodes[m] }, sum);
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
            return false;
        }
        private void ReleaseAction(byte[] addrs, double weight)
        {
            string log = weight.ToString("F2") + "\t";

            log = weight.ToString("F2") + "\t";

            foreach (byte n in addrs)
            {
                log = log + String.Format("({0}){1}\t", n, agent.weight(n));

                agent.HitMatch(n);

                agent.Action(n,"flag_release");
            }
            StringResource.dolog(log);
            
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
        public bool CheckCombination()  //return whether there is hit or not.
        {

            {


                if (Caculation1() || Caculation2() || Caculation3() || Caculation4() || Caculation5())// || Caculation1() ||
                {
                    return true;

                }
                if (NodeAgent.IsDebug)
                {
                    if (rand.NextDouble() > 0.5)
                    {
                        DoRelease(new byte[] { packer.weight_nodes[1], packer.weight_nodes[2], packer.weight_nodes[3], packer.weight_nodes[4], packer.weight_nodes[5] }, 115);
                        return true;
                    }
                    return false;
                }


                packer.last_one_pack.bucket = null;
                return false;
            }
        }
        
 

    }
    enum VibStatus
    {
        VIB_INIDLE = 0,
        VIB_READY = 1,
        VIB_WORKING = 2,
        VIB_ERROR = 3

    }
    class NodeAgent
    {
        public static bool IsDebug = false; //debug mode or not

        private List<WeighNode> weight_nodes; //list of all weight nodes, external access can only use map address
        
        internal List<SPort> allports; //list of all serial ports
        private Dictionary<byte, SubNode> nodemap;//map the address to node
        private Dictionary<byte, DateTime> lastrelease; //map of the time of the last release        
        public SubNode this[byte addr]{
            get
            {
                if (nodemap.ContainsKey(addr))
                    return nodemap[addr];
                return null;
            }
        }
        

        public static bool bBootDone = false;  //whether it is in boot mode

        private uint pack_cnt = 0; //hit counter for multiple pack one trigger case

        public SubNode missingnode;   //node used to find the newly installed board

        //the mechansium for inter process control
        private static Mutex mut = new Mutex();

        private Thread msg_loop;    //loop to handle the message
        
        public VibStatus vibstate;  
        
        //manage error list of the node
        public string GetErrors(byte addr)
        {
            return this[addr].errlist;
        }
        public void ClearErrors(byte addr)
        {
            this[addr].errlist = "";
        }
        public NodeStatus GetStatus(byte addr)
        {
            return nodemap[addr].status;
        }
        public void SetStatus(byte addr, NodeStatus ns)
        {
            if (ns == NodeStatus.ST_DISABLED)
            {
                this[addr].status = NodeStatus.ST_DISABLED;
                this[addr].errlist = "";    
            }
            if (ns == NodeStatus.ST_IDLE)
            {
                this[addr].status = NodeStatus.ST_IDLE;
                this[addr].errlist = "";    
            }

        }
        public int GetMatchCount(byte addr)
        {
            return (this[addr] as WeighNode).cnt_match;
        }
        public void IncMatchCount(byte addr)
        {
            WeighNode n = this[addr] as WeighNode;
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
        }
        
        public void SetNodeReg(byte addr, string regname , UInt32 val)
        {
            mut.WaitOne();
                this[addr][regname] = val;

            if (!WaitForIdle(this[addr]))
            {
                MessageBox.Show(StringResource.str("tryagain"));
            }
            mut.ReleaseMutex();
        }
        public void GetNodeElement(byte addr, ref XElement x)
        {
            WaitForIdle(this[addr]);
            if (this[addr].status != NodeStatus.ST_IDLE)
                throw new Exception("Node cannot be operated now");

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
        public VibStatus TriggerPacker(byte addr)
        {
            VibrateNode vn = (this[addr] as VibrateNode);
            if (vn == null)
                return VibStatus.VIB_ERROR;
            pack_cnt++;
            if ((pack_cnt % (vn.intf_byte.feed_times + 1) == 0))
            {
                vn["flag_enable"] = 10;
                WaitUntilFlagSent(vn);
                return VibStatus.VIB_WORKING;
            }
            else
            {
                return VibStatus.VIB_READY;
            }
        }


        //search whether the node is available
        public bool search(byte addr)
        {
            SubNode n = this[addr] as SubNode;
            n.status = NodeStatus.ST_IDLE;
            bool ret = false;
            if (mut.WaitOne())
            {
                n["addr"] = null;
                if (!WaitForIdleSlow(n))
                {
                    n.status = NodeStatus.ST_LOST;
                }
                else
                {
                    n["board_id"] = null;//get board type
                    ret = WaitForIdleSlow(n);
                    n["fw_rev_uw"] = null;//uw
                    ret = WaitForIdleSlow(n);
                    ret = n["fw_rev_uw"].HasValue;
                }
                mut.ReleaseMutex();
            }
            
            return ret;
        }
        public void ClearNodeReg(byte addr,string regname)
        {
            mut.WaitOne();
            this[addr][regname] = null;
            if (!WaitForIdle(this[addr]))
            {
                MessageBox.Show(StringResource.str("tryagain"));
            }
            mut.ReleaseMutex();
        }
        public string GetNodeReg(byte addr,string regname)
        {
            while (this[addr].status == NodeStatus.ST_IDLE || this[addr].status == NodeStatus.ST_BUSY)
            {
                if (this[addr].status != NodeStatus.ST_IDLE)
                {
                    Thread.Sleep(20);
                }
                if (!this[addr][regname].HasValue)
                    Action(addr, "getreg:" + regname);
                if (this[addr][regname].HasValue)
                    return nodemap[addr][regname].ToString();
                Thread.Sleep(10);
                continue;
            }
            return "";
        }
        
        private byte flag_cmd(string action) //translate the action string to reg value
        {
            if (action == "stop")
                return 0;
            if (action == "start")
                return 80;
            if (action == "zero")
                return 7;
            if (action == "release")
                return 4;
            if (action == "empty")
                return 4;
            if (action == "pass")
                return 3;
            if (action == "fill")
                return 5;
            
            return 0xff;
        }
        public void Action(byte addr, string cmd)
        {
            if (addr < 0x80)
            {
                SubNode node = this[addr];
                if((this[addr].status == NodeStatus.ST_DISABLED))
                    return;
                if(!(node is SubNode))
                    return;
                if (cmd == "flag_goon") //no response is required so that no mutex is required
                {
                    node["flag_goon"] = 1;
                    return;
                }
                if (cmd == "fill")
                {
                    node["flag_enable"] = flag_cmd(cmd);// no response is required so that no mutex is required
                    return;
                }
                if (mut.WaitOne())
                {
                    try
                    {
                        Debug.WriteLine(node.node_id + ":" + cmd);
                        WaitForIdle(node);
                        if (cmd.IndexOf("getreg") == 0) //getreg:regname
                        {
                            node[cmd.Remove(0,"getreg:".Length)] = null;
                            WaitForIdle(node);
                        }
                        if (flag_cmd(cmd) < 128)
                        {
                            node["flag_enable"] = flag_cmd(cmd);
                            WaitUntilFlagSent(node);
                        }
                        if (cmd == "flag_goon")
                        {
                            node["flag_goon"] = 1;
                            //WaitUntilFlagSent(node);
                        }

                        if (cmd == "flag_release")
                        {
                            lastrelease[node.node_id] = DateTime.Now;
                            node["flag_release"] = 1;
                            WaitUntilFlagSent(node);
                        }
                        if ((cmd == "trigger") && (node is VibrateNode))
                        {
                            node["flag_enable"] = 10; //trigger the interface
                            WaitUntilFlagSent(node);
                        }
                        if (cmd.IndexOf("interface") == 0) //command format interface1234
                        {
                            UInt32 val = UInt32.Parse(cmd.Remove(0, "interface".Length));
                            node["target_weight"] = val;
                            Thread.Sleep(25);
                            
                            node["flag_enable"] = 8; //initialize the interface
                            WaitUntilFlagSent(node);
                                
                        }
                        if (cmd.IndexOf("calib") == 0) //command format calib1300
                        {
                            byte slot = byte.Parse(cmd.Substring("calib".Length,1));
                            UInt32 weight = UInt32.Parse(cmd.Remove(0, "calib1".Length));
                            
                            node["cs_mtrl"] = null;
                            if (WaitForIdle(node))
                            {
                                if (node["cs_mtrl"].Value <= WeighNode.MAX_VALID_WEIGHT)
                                {
                                    if (slot == 0) //zero point calibration
                                    {
                                        node["cs_zero"] = node["cs_mtrl"];
                                        WaitUntilGetValue(node, "cs_zero", node["cs_mtrl"].Value);
                                    }
                                    else
                                    {
                                        node["cs_poise" + slot.ToString()] = node["cs_mtrl"];
                                        WaitUntilGetValue(node, "cs_poise" + slot.ToString(), node["cs_mtrl"].Value);
                                        node["poise_weight_gram" + slot.ToString()] = weight;
                                        WaitUntilGetValue(node, "poise_weight_gram" + slot.ToString(), weight);
                                    }
                                }
                                else
                                {
                                    MessageBox.Show(StringResource.str("tryagain"));
                                }

                            }
                            else
                            {
                                MessageBox.Show(StringResource.str("tryagain"));
                            }
                        }

                        if (cmd == "flash")
                        {
                            //wait until all data is programed.
                            node["NumOfDataToBePgmed"] = 45;
                            Thread.Sleep(1000);

                            if (!WaitUntilGetValue(node, "NumOfDataToBePgmed", 0))
                                throw new Exception("burnning failed.");
                        }
                        mut.ReleaseMutex();
                    }
                    catch (Exception exmsg)
                    {
                        mut.ReleaseMutex();
                        Debug.WriteLine(DateTime.Now.ToString("G")+exmsg.Message);
                    }
                }
                else
                {
                    throw new Exception("Timeout to conduct the action");
                }
            }
            else
            {
                if (mut.WaitOne())
                {
                    try
                    {

                        if (cmd == "grelease")
                        {
                            nodemap[1].writebyte_group_reg(addr, new string[] { "flag_enable" }, new byte[] { (byte)4 });

                        }
                        if (cmd == "zero")
                        {
                             nodemap[1].writebyte_group_reg(addr, new string[] { "flag_enable" }, new byte[] { (byte)flag_cmd(cmd) });

                        }
                        if (cmd == "start" || cmd == "stop")
                        {
                            foreach (WeighNode wnode in weight_nodes)
                            {
                                wnode.writebyte_abs_reg(new string[] { "flag_enable" }, new byte[] { (byte)flag_cmd(cmd) });
                                wnode.cnt_match = 0;
                            }
                        }
                        if (cmd == "pass" || cmd == "fill" || cmd == "empty" || cmd == "release" || cmd=="goon")
                        {
                            foreach (WeighNode wnode in weight_nodes)
                            {
                                wnode.writebyte_abs_reg(new string[] { "flag_enable" }, new byte[] { (byte)flag_cmd(cmd) });
                            }
                        }
                        mut.ReleaseMutex();
                    }
                    catch (Exception exmsg2)
                    {
                        mut.ReleaseMutex();
                    }
                }
                else
                {
                    throw new Exception("Timeout to conduct the action");
                }

            }
          
        }
        private void LoadConfiguration()
        {
            //load current configuration

            XmlConfig app_cfg = new XmlConfig(ProdNum.baseDir + "\\app_config.xml");
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
                
                if (type == "weight")
                {
                    nodemap[byte.Parse(node)] = new WeighNode(allports[byte.Parse(com)], byte.Parse(node));
                    lastrelease[byte.Parse(node)] = DateTime.Now;
                    weight_nodes.Add(nodemap[byte.Parse(node)] as WeighNode);
                }
                if (type == "vibrate")
                {
                    nodemap[byte.Parse(node)] = new VibrateNode(allports[byte.Parse(com)], byte.Parse(node));
                }

            }
            
            missingnode = new WeighNode(allports[0], byte.Parse(def_cfg.Element("def_addr").Value)); //36 is the default address of unassigned address board

            
        }
        public delegate void HitRefreshEventHandler(object sender);
        //public event HitRefreshEventHandler RefreshEvent;
        private void RefreshEvent(object o)
        {
            bNeedRefresh = true;
        }
        internal NodeAgent()
        {
            allports = new List<SPort>();
            weight_nodes = new List<WeighNode>();
            nodemap = new Dictionary<byte, SubNode>();
            lastrelease = new Dictionary<byte, DateTime>();
            vibstate = VibStatus.VIB_INIDLE;
            

            msg_loop = new Thread(new ThreadStart(MessageLoop));
            msg_loop.IsBackground = true;


            LoadConfiguration();
            
            
        }
        public void Resume()
        {
            bBootDone = true;
            //msg_loop.Resume();
        }
        public void Start()
        {
            msg_loop.Start();
        }
        private bool bBootCmd = false;
        public void Stop(int ms)
        {
            bBootDone = false;
            bBootCmd = true;
            while(bBootCmd)
                Thread.Sleep(10);
//          msg_loop.Suspend();
        }
        
        private bool WaitUntilGetValue(SubNode n, string regname, UInt32 val) //until means if timeout happens, node will be set to lost
        {
            int i = 5;
            int j = 0;

            if (n.status == NodeStatus.ST_LOST)
                return false;

            if (n.status == NodeStatus.ST_DISABLED)
                return false;

            n[regname] = null;
            Thread.Sleep(12);

            
            while (WaitForIdleQuick(n))
            {
                if (n[regname].HasValue && n[regname].Value == val)
                {
                    return true;
                }
                Thread.Sleep(10);
                i = i * 2;
                j = j + 1;
                if (j > 240)
                {
                    n.status = NodeStatus.ST_LOST;
                    return false;
                }
                if(j % 40 == 39)
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
            //int j = 0;
            while (n.status == NodeStatus.ST_BUSY)
            {
                Thread.Sleep(5);
                //i = i + (i+1)/2;
                if (i++ > 900)
                {
                        return false;
                }else
                {
                    if(i % 60 == 59)
                        n.ResendCommand();
                }
            }
            return true;
        }
        private bool WaitForIdleSlow(SubNode n)
        {
            int i = 2;
            //int j = 0;
            while (n.status == NodeStatus.ST_BUSY)
            {
                Thread.Sleep(10);
                //i = i + (i + 1) / 2; //8,12,18,28,42,64,96,
                if (i++ > 30)
                {
                    return false;
                }
            }
            return true;
        }

        private bool WaitForIdleQuick(SubNode n)
        {
            int i = 2;
            //int j = 0;
            while (n.status == NodeStatus.ST_BUSY)
            {
                Thread.Sleep(10);
                //i = i + (i + 1) / 2; //8,12,18,28,42,64,96,
                if (i++ > 8)
                {
                    return false;
                }
            }
            return true;
        }
        private bool WaitUntilFlagSent(SubNode n)
        {
            if (IsDebug)
                return true;
            string reg = "flag_cnt";
            Thread.Sleep(5);
            n[reg] = null;
            WaitForIdleQuick(n);
            if (n[reg].HasValue)
            {
                 uint lw_lb = n[reg].Value % 256;
                 if ((lw_lb) != n.flag_cnt)
                 {
                    n.flag_cnt = lw_lb;
                    return true;
                 }
            }
            Debug.Write("lost" + n.node_id);
//          n.status = NodeStatus.ST_LOST;
            return false;
        }

        private bool CheckPackerReady(byte addr)
        {
            VibrateNode vn = (this[addr] as VibrateNode);
            if (vn == null)
                return false;
            if (IsDebug)
                return true;

            vn["hw_status"] = null;
            
            WaitForIdle(vn);
            if ((!vn["hw_status"].HasValue) || (vn["hw_status"].Value != 0))
            {
                Thread.Sleep(10);//todo : add alert
                return false;
            }
            return true;
        }
        private bool CheckPackerDone(byte addr)
        {
            if (IsDebug)
                return true;
            VibrateNode vn = (this[addr] as VibrateNode);
            if (vn == null)
                return false;
            vn["pack_rel_cnt"] = null;
            WaitForIdleQuick(vn);
            if (vn["pack_rel_cnt"].HasValue)
            {
                if((pack_cnt % (vn.intf_byte.feed_times+1) != 0))
                {
                    return true;
                }
                uint lw_ub = vn["pack_rel_cnt"].Value;
                if(lw_ub != release_cnt)
                {
                    release_cnt = lw_ub;
                    release_timeout = 0;
                    return true;
                }
            }
            if (release_timeout++ < 400)
            {
                Thread.Sleep(5);
                if ((release_timeout % 100) == 49)
                {
                    vn.status = NodeStatus.ST_IDLE;
                    vn["pack_rel_cnt"] = null;
                }
            }
            else
            {
                release_timeout = 0;
                vn["flag_enable"] = 10; //retry trigger packer
            }
            return false;
        }



        public VibStatus ResetVibStatus(byte addr)
        {
            VibrateNode vn = (this[addr] as VibrateNode);
            if (vn == null)
                return VibStatus.VIB_ERROR;
            if (vn["pack_rel_cnt"].HasValue)
                release_cnt = vn["pack_rel_cnt"].Value;
            else
            {
                release_cnt = 0;
                //vn["pack_rel_cnt"] = 0;
            }
            
            release_timeout = 0;
            return VibStatus.VIB_INIDLE;
            
        }
        public void SetVibIntf(byte vibaddr, Intf i)
        {
            VibrateNode vn = this[vibaddr] as VibrateNode;
            vn.intf_byte = i;
        }
        public VibStatus UpdateVibStatus(byte vibaddr,VibStatus vibstate)
        {
            VibrateNode vn = this[vibaddr] as VibrateNode;
            if (!vn.intf_byte.b_Handshake)
            {
                return VibStatus.VIB_READY;
            }

            VibStatus oldstate = vibstate;
            mut.WaitOne();
            if (vibstate == VibStatus.VIB_WORKING)
            {
                
                if (CheckPackerDone(vibaddr)) //check botton pack count increase
                {
                    vibstate = VibStatus.VIB_INIDLE;
                    Debug.WriteLine("idle");
                }
            }
            if (vibstate == VibStatus.VIB_INIDLE)
            {
                if (CheckPackerReady(vibaddr))//hw_status == 0
                {
                    Debug.WriteLine("ready");
                    vibstate = VibStatus.VIB_READY;
                }
            }
            if (vibstate != oldstate)
                RefreshEvent(this);
            mut.ReleaseMutex();
            return vibstate;
        }
       
        private int check_weights_ready()
        {
            int ret;
            TimeSpan ts1 = new TimeSpan(DateTime.Now.Ticks);
            DateTime tnow = DateTime.Now;
            
            mut.WaitOne();
               
            foreach (WeighNode wn in weight_nodes)
            {
                
                if (wn.status == NodeStatus.ST_LOST || wn.status == NodeStatus.ST_DISABLED)
                    continue;
                TimeSpan ts3 = DateTime.Now - lastrelease[wn.node_id];
                if (ts3.TotalMilliseconds < 150)
                    continue;
                if (wn.weight < -1000 || (wn.weight >= 100000)) //invalid weight || wn.weight >= 65521
                {
                    wn.Query();
                    
                    WaitForIdleQuick(wn);
                    //NodeCombination.logTimeStick(ts1, "ck2:");
                }
            }
            mut.ReleaseMutex();

            foreach (WeighNode wn in weight_nodes)
            {

                if (wn.status == NodeStatus.ST_LOST || wn.status == NodeStatus.ST_DISABLED)
                    continue;
            }
            ret = weight_nodes.Count;
            foreach (WeighNode wn in weight_nodes)
            {
                double wt = wn.weight;
                TimeSpan ts4 = DateTime.Now - lastrelease[wn.node_id];
                if (ts4.TotalMilliseconds < 150)
                {
                    ret--;
                    continue;
                }
                if ((wt < -1000) || (wt >= 100000)) //invalid reading
                {
                    
                        //Debug.Write(wn.node_id);
                    
                    continue; //break;
                }
                ret--;
            }
            if(ret < 4)
                NodeCombination.logTimeStick(ts1, "ck:");
            return ret;
        }

        public void ClearWeights()
        {
            foreach (WeighNode wnode in weight_nodes)
            {
                wnode.ClearWeight(); //clear the weight value
            }
        }
        public void HitMatch(byte addr)
        {
            WeighNode wn = (this[addr] as WeighNode);
            wn.ClearWeight();
            if (wn.cnt_match == -1)
                throw new Exception("cannot have two hit in one loop");
            wn.cnt_match = -1;
            if (wn.errlist != "")
                wn.errlist = wn.errlist.Replace("err_om;", "");
        }
        private UInt32 release_cnt;
        private UInt32 release_timeout = 0;

        public bool bWeightReady;
        public double weight(byte addr)
        {
            if (this[addr] is WeighNode)
            {
                return (this[addr] as WeighNode).weight;
            }
            return WeighNode.INAVLID_WEIGHT;

        }
        public void SetOverWeight(byte addr, bool ow)
        {

            WeighNode n = this[addr] as WeighNode;
            if (ow)
            {
                if (n.errlist.IndexOf("err_ow;") < 0)
                    n.errlist = n.errlist + "err_ow;";
            }
            else
            {
                if (n.errlist != "")
                    n.errlist = n.errlist.Replace("err_ow;", "");//reset overweight error

            }
        }
        public bool bNeedRefresh = false;

        public void MessageLoop()
        {
            uint checkcnt = 0;
            int notready = 100;
            try
            {
                while (true)
                {
                    if (!bBootDone)
                    {
                        if (bBootCmd)
                        {
                            bBootCmd = false;
                        }
                        Thread.Sleep(10);
                        continue;
                    }

                    if (!bWeightReady)
                    {
                        notready = check_weights_ready();
                        if (notready < 4)
                        {
                            bWeightReady = true;
                            checkcnt = 0;
                            RefreshEvent(this);
                        }
                        else
                        {
                            //                          Thread.Sleep(150);
                            Debug.Write("[");
                        }
                    }
                    if (checkcnt++ > 50)
                    {
                        Debug.Write(".");
                        checkcnt = 0;
                        RefreshEvent(this);
                    }
                    Thread.Sleep(20);
                }
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message + e.StackTrace + e.Source);
            }
        }
    }
}
