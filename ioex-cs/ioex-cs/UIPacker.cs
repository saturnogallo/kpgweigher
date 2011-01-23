using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Windows;
namespace ioex_cs
{
    enum PackerStatus : byte
    {
        RUNNING = 0,
        PAUSED,
        INERROR,
        IDLE
    }
    class UIParam
    {
        public static int last_nodeid;
        public bool selected { get; set; }
        static UIParam()
        {
            last_nodeid = -1;
        }
        public UIParam()
        {
            selected = false;
        }

    }
    class PackerConfig
    {
        public string product_no;
        public string product_desc;
        public double target;
        public string operator_no;
        public double upper_var;
        public double lower_var;
        private string wnodes;
        private string vnode;
        
        static public string oper = "001";
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
            operator_no = cfgNode.Element("operator_no").Value;
            product_desc = cfgNode.Element("product_desc").Value;
            wnodes = cfgNode.Element("WNodes").Value;
            vnode = cfgNode.Element("VNode").Value;
        }
        public void ToElement(ref XElement cfgNode)
        {
            cfgNode.Add(new XElement("target", target.ToString()));
            cfgNode.Add(new XElement("upper_var", upper_var.ToString()));
            cfgNode.Add(new XElement("lower_var", lower_var.ToString()));
            cfgNode.Add(new XElement("product_no", product_no.ToString()));
            cfgNode.Add(new XElement("operator_no", operator_no.ToString()));
            cfgNode.Add(new XElement("product_desc", product_desc.ToString()));
            cfgNode.Add(new XElement("VNode", vnode));
            cfgNode.Add(new XElement("WNodes", wnodes));
        }

    }

    class UIPacker
    {
        public NodeAgent agent;
        public int _pack_id;
#region uidata
        public int curr_node_index { get; set; } //for single mode , curr_node means the index of last selected weight node.
        public Dictionary<byte, UIParam> uidata; //UI status of each nodes
        
#endregion
        public byte vib_addr;               //vibrate node address
        public List<byte> weight_nodes;     //weight node addr list
        private bool _NeedRefresh;
        private object refreshlock;

#region nodedata
#endregion
#region configdata
        public Dictionary<byte, XmlConfig> nodes_config; //config of weight nodes   

        public XmlConfig all_conf;//store all the configurations of the packer
        private PackerConfig sys_cfg; //store packer related config
        public PackerConfig curr_cfg { get { return sys_cfg; } }

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
        public double last_pack_weight;
        public onepack last_one_pack;
#endregion

        
        public PackerStatus status { get; set; } //status of running
        
        public bool NeedRefresh {
            get{
                bool ret = _NeedRefresh;
                _NeedRefresh = false;
                return ret;
            }
            set{
                _NeedRefresh = value;
            }
        }
        public UIPacker(int pack_id,NodeAgent nagent)
        {
            agent = nagent;
            vib_addr = 0xff;
            curr_node_index = -1;
            _pack_id = pack_id;
            status = PackerStatus.IDLE;
            
            refreshlock = new object();
            weight_nodes = new List<byte>();
            nodes_config = new Dictionary<byte, XmlConfig>();
            uidata = new Dictionary<byte, UIParam>();
            all_conf = new XmlConfig("pack_define"+pack_id.ToString()+".xml");
            all_conf.LoadConfigFromFile();

            
            sys_cfg = new PackerConfig();
            packhist = new Queue<onepack>();
        }
        private void ResetHistory()
        {
            packhist.Clear();
            //total_weights = 0;
            //total_packs = 0;

            speed = 0;
            last_pack_weight = 0;

        }

        public void AddNewPack(onepack o)
        {
            if (!App.bSimulate)
            {
                total_packs++;
                total_weights += o.weight;
            }
            else
            {
                total_sim_weights += o.weight;
                total_sim_packs++;
            }
            last_pack_weight = o.weight;
            last_one_pack = o;

            
            packhist.Enqueue(o);
            if (packhist.Count > 500)
            {
                packhist.Dequeue();
            }
            //update the speed
            long count = 0;
            DateTime lastmin = DateTime.Now;
            lastmin = lastmin.Subtract(new TimeSpan(0, 1, 1));
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
            if (App.bSimulate)
                return;
            if (total_packs % 100 == 1)
                ProdHistory.UpdateRecord(this);
        }

        public void InitConfig()
        {
            LoadConfig(all_conf.cfg_name);
        }
        public Intf getInterface()
        {
            Intf intf_byte = new Intf(UInt16.Parse(agent.GetNodeReg(vib_addr, "target_weight")));
            return intf_byte;
        }
        public void setInterface(Intf intf_byte)
        {
            agent.Action(vib_addr,"interface"+intf_byte.buf.ToString());
        }

        //load all the configuration and update the UI,
        //packer and sub node will share the same configuration name
        public void LoadConfig(string cfgname)
        {
            if ((Application.Current as App).curr_packer.status == PackerStatus.RUNNING)
                return;
            weight_nodes.Clear();
            all_conf.LoadConfig(cfgname);
            XElement cfgNode = all_conf.Current;
            sys_cfg.FromElement(cfgNode);
            foreach (string wnode in cfgNode.Element("WNodes").Value.ToString().Split(new char[] { ',' }))
            {
                weight_nodes.Add(byte.Parse(wnode));
            }
            foreach (byte n in weight_nodes)
            {
                if (!nodes_config.ContainsKey(n))
                {
                    nodes_config[n] = new XmlConfig("node_" + n + ".xml");
                    nodes_config[n].LoadConfigFromFile();
                }
                nodes_config[n].LoadConfig(cfgname);
                uidata[n] = new UIParam();
            }
            vib_addr = byte.Parse(cfgNode.Element("VNode").Value.ToString());
            if (!nodes_config.ContainsKey(vib_addr))
            {
                nodes_config[vib_addr] = new XmlConfig("node_" + vib_addr + ".xml");
                nodes_config[vib_addr].LoadConfigFromFile();
            }
            nodes_config[vib_addr].LoadConfig(cfgname);
            //todo download the configuration of packer settings
            foreach (string reg in new string[] { "magnet_freq", "magnet_amp", "magnet_time", "target_weight", "cs_filter" })
            {
                try
                {
                    agent.SetNodeReg(vib_addr, reg, UInt32.Parse(nodes_config[vib_addr].Current.Element(reg).Value));
                }
                catch
                {
                }
            }
            foreach (byte n in weight_nodes)
            {
                foreach (string reg in new string[] { "magnet_freq", "magnet_amp", "magnet_time", "motor_speed", "cs_filter", "cs_gain_wordrate", "delay_f", "delay_w", "delay_s", "open_s", "open_w", "target_weight"})
                    
                {
                    try
                    {
                        agent.SetNodeReg(n, reg, UInt32.Parse(nodes_config[n].Current.Element(reg).Value));
                    }catch
                    {
                    }
                }
            }
        }
        //convert the current configuration into an XElement node
        public void SaveCurrentConfig()
        {
            XElement cfgNode = new XElement("Item");
            sys_cfg.ToElement(ref cfgNode);

            all_conf.AddConfig(all_conf.cfg_name, cfgNode);
            all_conf.SaveConfigToFile();
            XElement sNode = new XElement("Item");
            foreach (byte n in weight_nodes)
            {
                sNode.RemoveAll();
                agent.GetNodeElement(n,ref sNode);
                nodes_config[n].AddConfig(all_conf.cfg_name, sNode);
                nodes_config[n].SaveConfigToFile();
            }
            sNode.RemoveAll();
            agent.GetNodeElement(vib_addr, ref sNode);
            nodes_config[vib_addr].AddConfig(all_conf.cfg_name, sNode);
            nodes_config[vib_addr].SaveConfigToFile();
        }

        public void DuplicateCurrentConfig(string newcfg)
        {
            if (newcfg == "")
            {
                newcfg = (all_conf.Keys.Count() + 1).ToString("D3");
            }
            //add new configuration
            foreach (byte n in this.weight_nodes)
            {
                nodes_config[n].AddConfig(newcfg,nodes_config[n].Current);
                nodes_config[n].LoadConfig(newcfg);
                nodes_config[n].SaveConfigToFile();
            }
            nodes_config[vib_addr].AddConfig(newcfg,nodes_config[vib_addr].Current);
            nodes_config[vib_addr].LoadConfig(newcfg);
            nodes_config[vib_addr].SaveConfigToFile();


            XElement cfgNode = new XElement("Item");
            sys_cfg.ToElement(ref cfgNode);
            cfgNode.Element("product_no").Value = newcfg;
            
            all_conf.AddConfig(newcfg, cfgNode);
            this.LoadConfig(newcfg);
            all_conf.SaveConfigToFile();
        }

        public void StartRun()
        {
            rStart = DateTime.Now;
            ProdHistory.InitNewRun(this);
            agent.Action((byte)(0x80+_pack_id), "start");
            status = PackerStatus.RUNNING;
        }
        public void StopRun()
        {
            status = PackerStatus.IDLE;
            if (!App.bSimulate)
            {
                ProdHistory.UpdateRecord(this);
                ResetHistory();
            }
            else
            {
                total_sim_packs = 0;
                total_sim_weights = 0;
            }
            agent.Action((byte)(0x80 + _pack_id), "stop");

        }

        
        public void ActionAll(string action) //select, deselect, empty, set zero
        {
            if (action == "select")
            {
                foreach (byte n in this.weight_nodes)
                {
                    if (agent.GetStatus(n) != NodeStatus.ST_LOST)
                    {
                        uidata[n].selected = true;
                    }
                }
            }
            if (action == "deselect")
            {
                foreach (byte n in this.weight_nodes)
                {
                    if (agent.GetStatus(n) != NodeStatus.ST_LOST)
                    {
                        uidata[n].selected = false;
                    }
                }
            }
        }
    }
}
