using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Windows;
using System.Threading;
namespace ioex_cs
{
    enum PackerStatus : byte
    {
        RUNNING = 0,
        PAUSED,
        INERROR,
        IDLE
    }
    
    internal class PackerConfig
    {
        public string product_no;
        public string product_desc;
        public double target;
        public double upper_var;
        public double lower_var;
        public double target_comb; //target node number for combination.
        private string wnodes;//weight node
        private string vnode; //main vibrate node
        private string bnode; //bottom node
        
        public PackerConfig()
        {
            target = 0;
            upper_var = 0;
            lower_var = 0;
            
        }
        public void FromElement(XElement cfgNode)
        {
            target = Double.Parse(cfgNode.Element("target").Value);
            upper_var = Double.Parse(cfgNode.Element("upper_var").Value);
            lower_var = Double.Parse(cfgNode.Element("lower_var").Value);
            product_no = cfgNode.Element("product_no").Value;
            product_desc = cfgNode.Element("product_desc").Value;
            wnodes = cfgNode.Element("WNodes").Value;
            vnode = cfgNode.Element("VNode").Value;
            bnode = cfgNode.Element("BNode").Value;
            target_comb = Double.Parse(cfgNode.Element("target_comb").Value);
        }
        public void ToElement(ref XElement cfgNode)
        {
            cfgNode.Add(new XElement("target", target.ToString()));
            cfgNode.Add(new XElement("upper_var", upper_var.ToString()));
            cfgNode.Add(new XElement("lower_var", lower_var.ToString()));
            cfgNode.Add(new XElement("product_no", product_no.ToString()));
            cfgNode.Add(new XElement("product_desc", product_desc.ToString()));
            cfgNode.Add(new XElement("VNode", vnode));
            cfgNode.Add(new XElement("BNode", bnode));
            cfgNode.Add(new XElement("WNodes", wnodes));
            cfgNode.Add(new XElement("target_comb", target_comb));
        }

    }

    internal class UIPacker
    {
        public bool bSimulate = false;
        public NodeAgent agent;
        public NodeCombination nc;
        public int _pack_id;
        #region UI related variable
        
        #endregion



        public byte vib_addr;               //vibrate node address
        public byte bot_addr;               //bottom pack node address
        public List<byte> weight_nodes;     //weight node addr list
        private bool _NeedRefresh;          //indicator of 
        

#region configdata
        public Dictionary<byte, SqlConfig> nodes_config; //config of nodes   

        public SqlConfig pkg_confs;//store all the configurations of the packer
        private PackerConfig _curr_cfg; //store packer related config
        public PackerConfig curr_cfg { get { return _curr_cfg; } }

#endregion
        
#region summarydata
        private Queue<onepack> packhist;
        public double total_weights;
        public UInt32 total_packs;
        public UInt32 speed;

        //for simulation run;
        public double total_sim_weights;
        public UInt32 total_sim_packs;
        public UInt32 sim_speed; //
        
        public DateTime rStart;
        public double last_pack_weight {
            get{
                return last_one_pack.weight;
            }
        }
        public onepack last_one_pack;
#endregion

        
        public PackerStatus status { get; set; } //status of running
        
        public bool NeedRefresh {
            get{
                return _NeedRefresh;
            }
            set{
                _NeedRefresh = value;
            }
        }
        public UIPacker(int pack_id,NodeAgent nagent)
        {
            agent = nagent;
            vib_addr = 0xff;
            bot_addr = 0xff;

            _pack_id = pack_id;
            status = PackerStatus.IDLE;
            
            
            weight_nodes = new List<byte>();
            nodes_config = new Dictionary<byte, SqlConfig>();

            
            pkg_confs = new SqlConfig("pack" + pack_id.ToString());
            
            pkg_confs.LoadConfigFromFile();

            _curr_cfg = new PackerConfig();
            packhist = new Queue<onepack>();
            
        }
        public void BottomAction(string command)
        {
            agent.Action(bot_addr, command);
        }
        public void VibAction(string command)
        {
            agent.Action(vib_addr, command);
        }
        public void GroupAction(string command)
        {
            if (command == "clearweight")
            {
                agent.ClearWeights();
                return;
            }
            
            agent.Action((byte)(0x80 + _pack_id), command);
        }
        public void WeightAction(byte addr, string command)
        {
            if(weight_nodes.Contains(addr))
                agent.Action(addr,command);
            else{
                MessageBox.Show("Invalid node for this packer.");
            }
        }
        private void ResetHistory()
        {
            packhist.Clear();
            speed = 0;
            last_one_pack.bucket = null;
            last_one_pack.weight = 0.0;
        }

        public void AddNewPack(onepack o)
        {
            if (!bSimulate)
            {
                total_packs++;
                total_weights += o.weight;
            }
            else
            {
                total_sim_weights += o.weight;
                total_sim_packs++;
            }
            last_one_pack = o;

            packhist.Enqueue(o);
            if (packhist.Count > 500)
            {
                packhist.Dequeue();
            }
            //update the speed
            long count = 0;
            DateTime lastmin = DateTime.Now;
            lastmin = lastmin.Subtract(new TimeSpan(0, 1, 0));
            foreach (onepack op in packhist)
            {
                if (op.time < lastmin)
                {
                    count = count + 1;
                }
                else
                {
                    count = packhist.Count - count;
                    break;
                }
            }
            speed = (UInt32)count;
            if (bSimulate)
                return;
            if (total_packs % 100 == 1) //update record for every 100 packs
            {
                ProdHistory.UpdateRecord(this);
                EngConfigWnd.DecreasePacker(100);
            }
        }

        public void InitConfig()
        {
            weight_nodes.Clear();
            XElement cfgNode = pkg_confs.Current;
            _curr_cfg.FromElement(cfgNode);
            foreach (string wnode in cfgNode.Element("WNodes").Value.ToString().Split(new char[] { ',' }))
            {
                weight_nodes.Add(byte.Parse(wnode));
            }
            vib_addr = byte.Parse(cfgNode.Element("VNode").Value.ToString());
            bot_addr = byte.Parse(cfgNode.Element("BNode").Value.ToString());
            nc = new NodeCombination(this);
        }
        public Intf getInterface()
        {
            string val = agent.GetNodeReg(bot_addr, "target_weight");
            if (val == "")
                return new Intf(0);
            Intf intf_byte = new Intf(UInt16.Parse(val));
            return intf_byte;
        }
        public void setInterface(Intf intf_byte)
        {
            agent.Action(bot_addr,"interface"+intf_byte.buf.ToString()); //set interface of bottom packer ex, interface123
        }

        //load all the configuration and update the UI,
        //packer and sub node will share the same configuration name
        public void LoadPackConfig(string cfgname,bool init)
        {
            if (status == PackerStatus.RUNNING)
                return;
            pkg_confs.LoadConfig(cfgname);
            _curr_cfg.FromElement(pkg_confs.Current);
            foreach (byte n in weight_nodes)
            {
                if(init)
                {
                    nodes_config[n] = new SqlConfig("node" + n);//new XmlConfig(ProdNum.baseDir + "\\node_" + n + ".xml");
                    nodes_config[n].LoadConfigFromFile();
                }
                nodes_config[n].LoadConfig(cfgname);
            }
            if (!nodes_config.ContainsKey(vib_addr))
            {
                if (init)
                {
                    nodes_config[vib_addr] = new SqlConfig("node" + vib_addr);//new XmlConfig(ProdNum.baseDir + "\\node_" + vib_addr + ".xml");
                    nodes_config[vib_addr].LoadConfigFromFile();
                }
                nodes_config[vib_addr].LoadConfig(cfgname);
            }
            
            if (!nodes_config.ContainsKey(bot_addr))
            {
                if (init)
                {
                    nodes_config[bot_addr] = new SqlConfig("node" + bot_addr);//new XmlConfig(ProdNum.baseDir + "\\node_" + bot_addr + ".xml");
                    nodes_config[bot_addr].LoadConfigFromFile();
                }
                nodes_config[bot_addr].LoadConfig(cfgname);
            }
            

            //download the configuration of node settings
            foreach (string reg in new string[] { "magnet_freq", "magnet_amp", "magnet_time", "target_weight", "cs_filter" })
            {
                try
                {
                    agent.SetNodeReg(vib_addr, reg, UInt32.Parse(nodes_config[vib_addr].Current.Element(reg).Value));
                }catch{

                }
            }
            if (bot_addr != vib_addr)
            {
                foreach (string reg in new string[] { "magnet_freq", "magnet_amp", "magnet_time", "target_weight", "cs_filter" })
                {
                    try
                    {
                        agent.SetNodeReg(bot_addr, reg, UInt32.Parse(nodes_config[bot_addr].Current.Element(reg).Value));
                    }
                    catch
                    {
                    }
                }
            }
            foreach (byte n in weight_nodes)
            {
                foreach (string reg in new string[] { "magnet_freq", "magnet_amp", "magnet_time", "motor_speed", "cs_filter", "cs_gain_wordrate", "delay_f", "delay_w", "delay_s", "open_s", "open_w", "target_weight"})
                {
                    try
                    {
                        agent.SetNodeReg(n, reg, UInt32.Parse(nodes_config[n].Current.Element(reg).Value));
                    }catch{
                    }
                }
            }
        }
        //convert the current configuration into an XElement node
        public void SaveCurrentConfig(byte group)
        {
            XElement cfgNode = new XElement("Item");
            XElement sNode = new XElement("Item");
            //group = pack|vib|sub
            if (group > 3) //save pack config
            {
                _curr_cfg.ToElement(ref cfgNode);
                pkg_confs.AddConfig(pkg_confs.cfg_name, cfgNode);
            }

            if ((group % 4) > 1) //save vib config
            {
                sNode.RemoveAll();
                try
                {
                    agent.GetNodeElement(vib_addr, ref sNode);
                    nodes_config[vib_addr].AddConfig(pkg_confs.cfg_name, sNode);
                    if (bot_addr != vib_addr)
                    {
                        agent.GetNodeElement(bot_addr, ref sNode);
                        nodes_config[bot_addr].AddConfig(pkg_confs.cfg_name, sNode);
                    }
                }
                catch
                {
                }
            }
            if ((group % 2) == 1) //save sub config
            {
                
                foreach (byte n in weight_nodes)
                {
                    sNode.RemoveAll();
                    try
                    {
                        agent.GetNodeElement(n, ref sNode);
                        nodes_config[n].AddConfig(pkg_confs.cfg_name, sNode);
                    }
                    catch
                    {
                    }
                }
            }
        }
        //duplicate all nodes and pack configuration based on config name
        public void DuplicateCurrentConfig(string newcfg)
        {
            if (newcfg == "")
            {
                newcfg = (pkg_confs.Keys.Count() + 1).ToString("D3");
            }
            //copy the current config to  new configuration
            foreach (byte n in this.weight_nodes)
            {
                nodes_config[n].AddConfig(newcfg,nodes_config[n].Current);
                nodes_config[n].LoadConfig(newcfg);
            }
            nodes_config[vib_addr].AddConfig(newcfg,nodes_config[vib_addr].Current);
            nodes_config[vib_addr].LoadConfig(newcfg);
            if (bot_addr != vib_addr)
            {
                nodes_config[bot_addr].AddConfig(newcfg, nodes_config[bot_addr].Current);
                nodes_config[bot_addr].LoadConfig(newcfg);
            }
            XElement cfgNode = new XElement("Item");
            _curr_cfg.ToElement(ref cfgNode);
            cfgNode.Element("product_no").Value = newcfg;
            _curr_cfg.product_no = newcfg;
            
            pkg_confs.AddConfig(newcfg, cfgNode);
            pkg_confs.LoadConfig(newcfg);
        }

        public void StartRun()
        {
            rStart = DateTime.Now;
            ProdHistory.InitNewRun(this);
            nc.ResetStatus();
            agent.Action((byte)(0x80+_pack_id), "start");//0x80 is group action
            agent.Action(vib_addr, "start");
            status = PackerStatus.RUNNING;
        }
        public void StopRun()
        {
                       
            nc.Stop(300);
            agent.Stop(100);
            
            agent.Action((byte)(0x80 + _pack_id), "stop");
            agent.Action(vib_addr, "stop");
            Thread.Sleep(300);
            status = PackerStatus.IDLE;
            agent.Resume();
            nc.Resume();
            
            if (!bSimulate)
            {
                ProdHistory.UpdateRecord(this);
                ResetHistory();
            }
            else
            {
                total_sim_packs = 0;
                total_sim_weights = 0;
            }

        }
        //update weight of each node based on current setting
        public void UpdateEachNodeTarget()
        {
            if (curr_cfg.target_comb <= 1)
            {
                foreach (byte naddr in weight_nodes)
                {
                    if ((agent.GetStatus(naddr) == NodeStatus.ST_IDLE))
                    {
                        agent.SetNodeReg(naddr, "target_weight", 0);
                    }
                }
                return;
            }
            double avg = curr_cfg.target / curr_cfg.target_comb;
            double step = avg * NodeAgent.VAR_RANGE / weight_nodes.Count; // +/-15% variation of avg , example: for 30, step will be 0.9 gram

            foreach (byte naddr in weight_nodes)
            {
                if ((agent.GetStatus(naddr) == NodeStatus.ST_IDLE))
                {
                    double per = avg + (Int32.Parse(naddr.ToString()) - (weight_nodes.Count) / 2) * step;
                    agent.SetNodeReg(naddr, "target_weight", Convert.ToUInt32(per));
                    
                }
            }
        }

        
 
    }
}
