using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.IO.Ports;
using System.Xml;
using System.Xml.Linq;
using System.Net;
using System.Net.Sockets;
using System.Windows.Forms;
using System.Diagnostics;
namespace TSioex
{
    class ActMessage
    {
        public string action;
        public byte[] addrs;
        public string[] regs;
        public UInt32[] values;
        public NodeStatus[] stats;
    }
    //server handle all the request to nodes.
    internal class NodeMaster
    {
        static public AutoResetEvent WaitEvent; //flag for wait response, when flag is on,node agent can process the command else node agent cannot do that
        static public AutoResetEvent OverEvent; //flag for process done event.
        static public object agent_access = new object(); //lock for agent access
        static public NodeAgent agent;
        static public ActMessage actmsg;
        #region utility
        static public UInt32 flag_cmd(string action) //translate the action string to reg value
        {
            if (action == "stop")
                return 0;
            if (action == "start")
                return 80;
            if (action == "trigger")
                return 10;
            if (action == "intf") //initialize the interface 
                return 8;
            if (action == "zero")
                return 7;
            if (action == "empty")
                return 4;
            if (action == "pass")
                return 3;
            if (action == "fill")
                return 5;

           throw new Exception("Undefined Flag Command");
        }
        #endregion
        static private Thread msg_loop;
        static NodeMaster()
        {
            Program.line = 9999;
            agent = new NodeAgent();
            Program.line = 99999;
            WaitEvent = new AutoResetEvent(false);
            OverEvent = new AutoResetEvent(false);
            agent_access = false;
            actmsg = new ActMessage();
            msg_loop = new Thread(new ThreadStart(MessageHandler));
            msg_loop.IsBackground = false;
            msg_loop.Start();
        }
        static public void Dummy()
        {
        }
        static public void HitMatch(byte[] addrs)
        {
            lock (agent_access)
            {
                foreach (byte addr in addrs)
                {
                    if ((agent[addr] is WeighNode) && (agent[addr].status == NodeStatus.ST_IDLE))
                    {
                        WeighNode wn = (agent[addr] as WeighNode);
                        wn.ClearWeight();
                        wn.cnt_match = -1;
                        if (wn.errlist != "")
                            wn.errlist = wn.errlist.Replace("err_om;", "");
                        
                    }
                }
            }
        }
        /*
        static public void ReleaseAgentLock()
        {
            agent_access = false;
        }
        static public void GetAgentLock()
        {

            while(agent_access)
                ;
            agent_access = true;
        }
         */
        static public void ClearWeights(byte[] addrs)
        {
            lock (agent_access)
            {
                foreach (byte addr in addrs)
                {
                    if ((agent[addr] is WeighNode) && (agent[addr].status == NodeStatus.ST_IDLE))
                    {
                        (agent[addr] as WeighNode).ClearWeight();
                    }
                }
            }
        }
        static public string GetErrors(byte addr)
        {
            string ret = "";
            lock (agent_access)
            {
                if (agent[addr] is SubNode)
                {
                    ret = agent[addr].errlist;
                }
            }
            return ret;
        }
        static public void ClearErrors(byte addr)
        {
            lock (agent_access)
            {
                if (agent[addr] is SubNode)
                {
                    agent[addr].errlist = "";
                }
            }
            return;
        }
        static public double GetWeight(byte addr)
        {
            double ret = WeighNode.INAVLID_WEIGHT;
            lock (agent_access)
            {
                if (agent[addr] is WeighNode)
                {
                    ret = (agent[addr] as WeighNode).weight;
                }
            }
            return ret;
        }
        static public NodeStatus GetStatus(byte addr)
        {
            NodeStatus ret = NodeStatus.ST_DISABLED;
            lock (agent_access)
            {
                if (agent[addr] is SubNode)
                {
                    ret = agent[addr].status;
                }
            }
            return ret;
        }
        static public void SetStatus(byte addr,NodeStatus status)
        {
            lock (agent_access)
            {
                if (agent[addr] is SubNode)
                {
                    agent[addr].status = status;
                    agent[addr].errlist = "";
                }
            }
            return;
        }
        static public bool GetNodeElement(byte addr, ref XElement node)
        {
            lock (agent_access)
            {
                agent.GetNodeElement(addr, ref node);
            }
            return true;
        }
        static public void ChangeAddress(byte oldaddr, byte newaddr)
        {
            throw new Exception("Not implemented");
        }
        //search the node specified by address
        static public bool SearchNode(byte addr)
        {
            bool found = false;
            actmsg.action = "refresh_regs";
            actmsg.addrs = new byte[] { addr };
            actmsg.regs = new string[] { "addr","board_id", "fw_rev_uw", "fw_rev_lw" };
            WaitEvent.Set();
            OverEvent.WaitOne();
            found = agent[addr]["board_id"].HasValue;
            OverEvent.Reset();
            return found;
            
        }
        static public void SetNodeReg(byte addr, string[] regs, UInt32[] values)
        {
            if (NodeAgent.IsDebug)
            {
                int i;
                for (i = 0; i < regs.GetLength(0); i++)
                {
                    agent[addr][regs[i]] = values[i];
                }
                return;
            }
            int retry = 0;
            while (retry++ < 3)
            {
                //write regs
                actmsg.action = "write_regs";
                actmsg.addrs = new byte[] { addr };
                actmsg.regs = regs;
                actmsg.values = values;
                WaitEvent.Set();
                OverEvent.WaitOne();
                OverEvent.Reset();

                RefreshNodeReg(addr, regs); //refresh node register
                int i = 0;
                for (i = 0; i < regs.GetLength(0); i++)
                {
                    if (values[i] != GetNodeReg(addr, regs[i]))
                        break;
                }
                if (i >= regs.GetLength(0))
                    return;
                Thread.Sleep(200);
            }
            lock (agent_access)
            {
                agent[addr].status = NodeStatus.ST_LOST; //set to lost status
            }
        }
        static public UInt32 GetNodeReg(byte addr, string reg)
        {
            UInt32 ret = 0;
            lock (agent_access)
            {
                if (agent[addr] is SubNode)
                {
                    if (agent[addr][reg].HasValue)
                        ret = agent[addr][reg].Value;
                    else
                        ret = 0;
                }
            }
            return ret;
        }
        static public void RefreshNodeReg(byte addr, string[] reg)
        {
            actmsg.action = "refresh_regs";
            actmsg.addrs = new byte[] { addr };
            actmsg.regs = reg;
            WaitEvent.Set();
            OverEvent.WaitOne();
            OverEvent.Reset();
            Thread.Sleep(10 * reg.Length); //for 210 it is 20 * reg.Length
        }
        static public void Action(byte[] addrs, string action)
        {
            actmsg.action = action;
            actmsg.addrs = addrs;
            WaitEvent.Set();
            OverEvent.WaitOne();
            OverEvent.Reset();
            if (actmsg.action == "retry")
            {
                Program.MsgShow(StringResource.str("tryagain"));
            }
            if (actmsg.action == "fail")
                throw new Exception("Critical Action failed.");
        }
        static public void IncMatchCount(byte addr)
        {
            if (agent[addr] is WeighNode)
            {
                WeighNode n = agent[addr] as WeighNode;
                n.cnt_match++;
                if (n.cnt_match > 50)
                {
                    if (AlertWnd.b_turnon_alert)
                    {
                        if (AlertWnd.b_manual_reset) //manual reset
                        {
                            if (n.errlist.IndexOf("err_om;") < 0)   //no matching for serveral times
                                n.errlist = n.errlist + "err_om;";
                        }
                    }
                }
            }
        }
        static public void SetOverWeight(byte addr, bool ow)
        {
            lock (agent_access)
            {
                if (agent[addr] is WeighNode)
                {
                    WeighNode n = agent[addr] as WeighNode;
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
            }

        }
        static private bool CriticalFlag(byte[] addrs, string reg, UInt32 val)
        {
            UInt32 flag_cnt;
            bool bDone = true;
            int retry = 0;
            if (NodeAgent.IsDebug)
            {
                foreach (byte addr in addrs)
                {
                    if (agent[addr].status == NodeStatus.ST_IDLE)
                    {
                        agent[addr].write_vregs(new string[] { reg }, new UInt32[] { val });
                        Thread.Sleep(2);//for 210 it is 5
                    }
                }
                return true;
            }
            foreach (byte addr in addrs)
            {
                if (agent[addr].status == NodeStatus.ST_IDLE)
                {
                    retry = 8;
                    while (!GetRegister(addr, new string[] { "flag_cnt" }) && (retry-- > 0))
                        Thread.Sleep(15);//for 210 it is 30
                    if (retry < 0)
                    {
                        bDone = false;
                        break;
                    }
                    flag_cnt = agent[addr]["flag_cnt"].Value;
                    retry = 4;
                    do
                    {
                        agent[addr].write_vregs(new string[] { reg }, new UInt32[] { val });
                        Thread.Sleep(2); //for 210 it is 5
                        GetRegister(addr, new string[] { "flag_cnt" });
                    } while ((flag_cnt == agent[addr]["flag_cnt"]) && (retry-- > 0));
                    if (retry < 0)
                    {
                        bDone = false;
                        break;
                    }
                }
            }
            return bDone;
        }
        static private bool GetRegister(byte addr, string[] regs)
        {
            lock (NodeMaster.agent_access)
            {
                foreach (string reg in regs)
                {
                    agent[addr][reg] = null;
                }
            }
            agent[addr].read_regs(regs);
            DateTime ts1 = DateTime.Now;
            bool bHit = false;
            while (true)
            {
                agent.ScanPort();
                bHit = true;
                foreach (string reg in regs)
                {
                    if (!agent[addr].IsRegFilled(reg))
                    {
                        bHit = false;
                        break;
                    }
                }
                TimeSpan ts2 = DateTime.Now.Subtract(ts1);
                
                if (bHit || (ts2.TotalMilliseconds > 300))
                    break;
            }
            return bHit;
        }
        static public void MessageHandler()
        {
            
            while(true)
            {
                if (!WaitEvent.WaitOne(10,false))
                {
                    agent.ScanPort();
                    continue;
                }
                WaitEvent.Reset();
                try
                {
                    ActMessage msg = NodeMaster.actmsg;

                    #region write_regs
                    if ((msg.action == "write_regs") && (msg.addrs.GetLength(0) == 1)) //only reg of one node is allowed
                    {
                        byte addr = msg.addrs[0];
                        if (agent[addr].status == NodeStatus.ST_IDLE)
                        {
                            agent[addr].write_vregs(msg.regs, msg.values);
                        }
                        actmsg.action = "done";
                        OverEvent.Set();
                        continue;
                    }
                    #endregion
                    #region actions
                    if ((msg.action == "fill") || (msg.action == "zero") || (msg.action == "empty") || 
                        (msg.action == "pass") || (msg.action == "pass") )
                    {
                        foreach (byte addr in msg.addrs)
                        {
                            if (agent[addr].status == NodeStatus.ST_IDLE)
                                agent[addr].write_vregs(new string[] { "flag_enable" }, new UInt32[] { NodeMaster.flag_cmd(msg.action) });
                        }
                        actmsg.action = "done";
                        OverEvent.Set();
                        continue;
                    }
                    if (msg.action == "flag_goon")
                    {
                        foreach (byte addr in msg.addrs)
                        {
                            if (agent[addr].status == NodeStatus.ST_IDLE)
                                agent[addr].write_vregs(new string[] { "flag_goon" }, new UInt32[] { 1 });
                        }
                        actmsg.action = "done";
                        OverEvent.Set();
                        continue;
                    }
                    #endregion
                    #region refresh_regs
                    if ((msg.action == "refresh_regs") && (msg.addrs.GetLength(0) == 1))
                    {
                        if (agent[msg.addrs[0]] is SubNode)
                        {
                            GetRegister(msg.addrs[0], msg.regs);
                            OverEvent.Set();
                            continue;
                        }
                    }
                    #endregion
                    #region flag_release
                    if (msg.action == "flag_release")
                    {
                        if (!CriticalFlag(msg.addrs, "flag_release", 1))
                            msg.action = "fail";
                        else
                        {
                            foreach (byte n in msg.addrs)
                            {
                                if (agent[n] is WeighNode)
                                    (agent[n] as WeighNode).cnt_match = -1;
                            }
                            msg.action = "done";
                        }
                        OverEvent.Set();
                        continue;
                    }
                    #endregion
                    #region flash
                    if (msg.action == "flash")
                    {
                        msg.action = "done";
                        foreach (byte addr in msg.addrs)
                        {
                            if (agent[addr].status == NodeStatus.ST_IDLE)
                            {
                                agent[addr].write_vregs(new string[] { "NumOfDataToBePgmed" }, new UInt32[] { 45 });
                                int retry = 20;
                                do
                                {
                                    Thread.Sleep(200);
                                    if (GetRegister(addr, new string[] { "NumOfDataToBePgmed" }))
                                    {
                                        if (agent[addr]["NumOfDataToBePgmed"] == 0)
                                            break;
                                    }
                                } while (retry-- > 0);
                                if (retry <= 0)
                                    msg.action = "retry";
                            }
                        }
                        OverEvent.Set();
                        continue;
                    }
                    #endregion
                    #region start stop trigger
                    if ((msg.action == "stop") || (msg.action == "start") || (msg.action == "trigger") || (msg.action == "intf"))
                    {
                        if (!CriticalFlag(msg.addrs, "flag_enable", NodeMaster.flag_cmd(msg.action)))
                        {
                            msg.action = "retry";
                        }
                        else
                        {
                            if ((msg.action == "stop") || (msg.action == "start"))
                            {
                                foreach (byte n in msg.addrs)
                                {
                                    if (agent[n] is WeighNode)
                                        (agent[n] as WeighNode).cnt_match = -1;
                                }
                            }
                            msg.action = "done";
                        }
                        OverEvent.Set();
                        continue;
                    }
                    #endregion
                    #region query
                    if (msg.action == "query")
                    {
                        foreach (byte addr in msg.addrs)
                        {
                            if (agent[addr].status == NodeStatus.ST_IDLE)
                            {
                                GetRegister(addr, new string[] { "mtrl_weight_gram", "mtrl_weight_decimal" });
                            }
                        }
                        actmsg.action = "done";
                        OverEvent.Set();
                        continue;
                    }
                    #endregion
                    throw new Exception("Invalide commnad " + actmsg.action);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                    actmsg.action = "fail";
                    OverEvent.Set();
                }
            }
        }
    }
    internal class NodeAgent
    {
        public static int NodeNum = 0;
        public static bool IsDebug = false; //debug mode or not
        private List<WeighNode> weight_nodes; //list of all weight nodes, external access can only use map address
        internal List<SPort> allports; //list of all serial ports
        private Dictionary<byte, SubNode> nodemap;//map the address to node
        public SubNode this[byte addr]
        {
            get
            {
                if (nodemap.ContainsKey(addr))
                    return nodemap[addr];
                return null;
            }
        } //return the node of specified address
        //public SubNode missingnode;   //node used to find the newly installed board
        public static double VAR_RANGE = 0.3; //40% percent variation range
        public static uint LASTCOMB_NUM = 2; //minimum invalid nodes weight for combination
        public static uint MSDELAY = 500; //silent time delay

        #region initialize
        //load current configuration
        //create all the subnodes based on ports, type and address
        private void LoadConfiguration()
        {
            SqlConfig app_cfg;
            XElement def_cfg;
            Program.line = 3000;
            Int32 baudrate;
            string parity;
            string[] ser_ports;
            string[] node_addrs;
            try
            {
                app_cfg = new SqlConfig("app"); Program.line++;
                app_cfg.LoadConfigFromFile(); Program.line++;
                def_cfg = app_cfg.Current; Program.line++;

                baudrate = Int32.Parse(def_cfg.Element("baudrate").Value); Program.line++;
                parity = def_cfg.Element("parity").Value.ToLower(); Program.line++;
                ser_ports = def_cfg.Element("totalports").Value.ToString().Split(new char[] { ',' }); Program.line++;
                node_addrs = def_cfg.Element("totalnodes").Value.ToString().Split(new char[] { ',' }); Program.line++;

                VAR_RANGE = Double.Parse(def_cfg.Element("var_range").Value); Program.line++;
                LASTCOMB_NUM = UInt32.Parse(def_cfg.Element("lastcomb_num").Value); Program.line++;
                MSDELAY = UInt32.Parse(def_cfg.Element("msdelay").Value); Program.line++;
            }
            catch
            {
                throw new Exception("Invalid parameter in database");
            }

            //create all serial ports
            try
            {
                foreach (string port in ser_ports)
                {
                    if (parity == "even")
                        allports.Add(new SPort(port, baudrate, Parity.Even, 8, StopBits.One));
                    else if (parity == "odd")
                        allports.Add(new SPort(port, baudrate, Parity.Odd, 8, StopBits.One));
                    else
                        allports.Add(new SPort(port, baudrate, Parity.None, 8, StopBits.One));
                    Program.line++;
                }
            }
            catch
            {
                throw new Exception("Failed to open the COM port");
            }
            
            foreach (SPort sp in allports)
            {
                try
                {
                    if (!sp.Open())
                    {
                        throw new Exception("Failed to open the port");
                    }
                }
                catch (Exception e)
                {
                    throw e;
                }
            }
            Program.line = 5000;
            //create all nodes
            foreach (string node in node_addrs)
            {
                string type = def_cfg.Element("node_type" + node).Value;
                string com = def_cfg.Element("node_com" + node).Value;

                if (type == "weight")
                {
                    nodemap[byte.Parse(node)] = new WeighNode(allports[byte.Parse(com)], byte.Parse(node));
                    weight_nodes.Add(nodemap[byte.Parse(node)] as WeighNode);
                }
                if (type == "vibrate")
                {
                    nodemap[byte.Parse(node)] = new VibrateNode(allports[byte.Parse(com)], byte.Parse(node));
                }
            }
            Program.line = 33333;
            //missingnode = new SubNode(allports[0], byte.Parse(def_cfg.Element("def_addr").Value)); //36 is the default address of unassigned address board
        }
        internal NodeAgent()
        {
            allports = new List<SPort>();
            weight_nodes = new List<WeighNode>();
            nodemap = new Dictionary<byte, SubNode>();
            Program.line = 7;
            this.LoadConfiguration();
            Program.line = 77;
            NodeNum = weight_nodes.Count;
        }

        #endregion
        #region node config xml 
        private string[] wn_regs = new string[] { "magnet_freq", "magnet_amp", "magnet_time", "open_w", "delay_w", "open_s", "delay_s", "delay_f", "motor_speed", "cs_filter", "cs_gain_wordrate", "target_weight" };
        private string[] vib_regs = new string[] { "magnet_freq", "magnet_amp", "magnet_time", "target_weight", "cs_filter" };
        private string[] bot_regs = new string[] { "magnet_freq", "magnet_amp", "magnet_time", "target_weight", "cs_filter" };
        public void GetNodeElement(byte addr, ref XElement x)
        {
            if (this[addr] is SubNode)
            {
                foreach (string reg in wn_regs)
                {
                    if(nodemap[addr][reg].HasValue)
                        x.Add(new XElement(reg, nodemap[addr][reg].Value.ToString()));
                    else
                        x.Add(new XElement(reg, "0"));
                }
            }
        }
        #endregion
        
        public void ScanPort()
        {
            foreach (SPort sp in allports)
            {
                if (sp.Checkport())
                {
                    lock (NodeMaster.agent_access)
                    {
                        if (this[sp.ifrm.addr] is SubNode)
                        {
                            this[sp.ifrm.addr].HandleInFrame(sp.ifrm);
                        }
                        sp.ifrm.ResetFlag();
                    }
                }
            }
        }
    }
}
