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
    class PackerConfig
    {
        public string product_no;
        public string product_desc;
        public double target;
        public double upper_var;
        public double lower_var;
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
        }
        public void ToElement(XElement cfgNode)
        {
            cfgNode.Add(new XElement("target", target.ToString()));
            cfgNode.Add(new XElement("upper_var", upper_var.ToString()));
            cfgNode.Add(new XElement("lower_var", lower_var.ToString()));
            cfgNode.Add(new XElement("product_no", product_no.ToString()));
            cfgNode.Add(new XElement("product_desc", product_desc.ToString()));
        }

    }

    
    class Packer
    {
        public int curr_node { get; set; } //for single mode , curr_node means the index of last selected weight node.

        public VibrateNode vib_node;      //vibrate node list
        public List<WeighNode> weight_node;     //weight node list
        public BottomPackNode pack_node;  //bottom pack node list
        private XmlConfig all_conf;//store all the configurations of the packer
        private PackerConfig sys_cfg;
        public PackerConfig curr_cfg { get {return sys_cfg;} }
        public string pack_define_file = "pack_define.xml";

        public PackerStatus status { get; set; } //status of running

        public Packer(int id)
        {
            vib_node = null;
            pack_node = null;
            weight_node = new List<WeighNode>();
            all_conf = new XmlConfig("pack_define"+id.ToString()+".xml");
            all_conf.LoadConfigFromFile();
            sys_cfg = new PackerConfig();
            curr_node = -1;
            status = PackerStatus.IDLE;
        }
        public void AddNode(SubNode node)
        {
            if (node is WeighNode)
            {
                weight_node.Add(node as WeighNode);
            }
            if (node is VibrateNode)
            {
                vib_node = node as VibrateNode;
            }
            if (node is BottomPackNode)
            {
                pack_node = node as BottomPackNode;
            }
        }
        public void InitConfig()
        {
            LoadConfig(all_conf.cfg_name);
        }
        //load all the configuration and update the UI,
        //packer and sub node will share the same configuration name
        public void LoadConfig(string cfgname)
        {
            foreach (WeighNode n in this.weight_node)
            {
                n.LoadCurrentConfig(cfgname);
            }
            XElement cfgNode = all_conf.LoadConfig(cfgname);
            vib_node.LoadCurrentConfig(cfgname);
            
            
            //load the configuration of packer settings
     

            sys_cfg.FromElement(cfgNode);
            vib_node.FromElement(cfgNode);
            if (pack_node is BottomPackNode)
            {
                pack_node.FromElement(cfgNode);
                pack_node.LoadCurrentConfig(cfgname);
            }
        }
        //convert the current configuration into an XElement node
        public void SaveCurrentConfig()
        {
            XElement cfgNode = new XElement("Item");
            sys_cfg.ToElement(cfgNode);
            vib_node.ToElement(cfgNode);
            if (pack_node is BottomPackNode)
            {
                pack_node.ToElement(cfgNode);
            }
            all_conf.AddConfig(all_conf.cfg_name, cfgNode);
            all_conf.SaveConfigToFile();
        }

        public void DuplicateCurrentConfig(string newcfg)
        {
            App p = Application.Current as App;
            p.bMainPause = true;
            if (newcfg == "")
            {
                newcfg = (all_conf.Keys.Count() + 1).ToString("D3");
            }
            //add new configuration

            foreach (WeighNode n in this.weight_node)
            {
                n.DuplicateCurrentConfig(newcfg);
            }
            vib_node.DuplicateCurrentConfig(newcfg);
            if (pack_node != null)
            {
                pack_node.DuplicateCurrentConfig(newcfg);
            }
            

            XElement cfgNode = new XElement("Item");
            sys_cfg.ToElement(cfgNode);
            cfgNode.Element("product_no").Value = newcfg;
            vib_node.ToElement(cfgNode);
            if (pack_node != null)
                pack_node.ToElement(cfgNode);

            all_conf.AddConfig(newcfg, cfgNode);
            this.LoadConfig(newcfg);
            all_conf.SaveConfigToFile();
            p.bMainPause = false;
        }
        private void DoRelease(SubNode[] addrs, double weight)
        {
            foreach (SubNode n in addrs)
            {
                n.Action("looprelease",false);
                (n as WeighNode).bRelease = true;
            }
            //todo update the display;

            onepack o = new onepack();
            o.bucket = new byte[5];
            o.bucket[1] = o.bucket[2] = o.bucket[3] = o.bucket[4] = o.bucket[0] = (byte)0;
            int i = 0;
            foreach (SubNode n in addrs)
            {
                o.bucket[i++] = (byte)n["addr"].Value;
                
            }
            o.time = DateTime.Now;
            o.weight = weight;
            ProdHistory.AddNewPack(o,(Application.Current as App).bSimulate);
            
        }
        //one bucket combination
        private bool Caculation1()
        {
            double sum = 0;
            double uvar = sys_cfg.target + sys_cfg.upper_var;
            double dvar = sys_cfg.target - sys_cfg.lower_var;

            for (int i = 0; i < weight_node.Count; i++ )
            {
                if(weight_node[i].status == NodeStatus.ST_LOST)
                    continue;
                sum = weight_node[i].weight;
                if(sum > uvar && sum < dvar)
                {
                    DoRelease(new SubNode[] { weight_node[i] }, sum);
                    return true;
                }
            }
            return false;
        }
        private bool Caculation2()
        {
            double sum = 0;
            double uvar = sys_cfg.target + sys_cfg.upper_var;
            double dvar = sys_cfg.target - sys_cfg.lower_var;

            for (int i = 0; i < weight_node.Count-1; i++)
            {
                if (weight_node[i].status == NodeStatus.ST_LOST)
                    continue;
                for (int j = i + 1; j < weight_node.Count; j++)
                {
                    if (weight_node[i].status == NodeStatus.ST_LOST)
                        continue;
                    sum = weight_node[i].weight + weight_node[j].weight;
                    if (sum > uvar && sum < dvar)
                    {
                        DoRelease(new SubNode[] { weight_node[i], weight_node[j]}, sum);
                        return true;
                    }
                }
            }
            return false;
        }
        private bool Caculation3()
        {
            double sum = 0;
            double uvar = sys_cfg.target + sys_cfg.upper_var;
            double dvar = sys_cfg.target - sys_cfg.lower_var;

            for (int i = 0; i < weight_node.Count - 2; i++)
            {
                if (weight_node[i].status == NodeStatus.ST_LOST)
                    continue;
                for (int j = i + 1; j < weight_node.Count-1; j++)
                {
                    if (weight_node[i].status == NodeStatus.ST_LOST)
                        continue;
                    for (int k = j + 1; k < weight_node.Count ; k++)
                    {
                        if (weight_node[i].status == NodeStatus.ST_LOST)
                            continue;

                        sum = weight_node[i].weight + weight_node[j].weight + weight_node[k].weight;
                        if (sum > uvar && sum < dvar)
                        {
                            DoRelease(new SubNode[] { weight_node[i], weight_node[j], weight_node[k] }, sum);
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
            double uvar = sys_cfg.target + sys_cfg.upper_var;
            double dvar = sys_cfg.target - sys_cfg.lower_var;

            for (int i = 0; i < weight_node.Count - 3; i++)
            {
                if (weight_node[i].status == NodeStatus.ST_LOST)
                    continue;
                for (int j = i + 1; j < weight_node.Count - 2; j++)
                {
                    if (weight_node[i].status == NodeStatus.ST_LOST)
                        continue;
                    for (int k = j + 1; k < weight_node.Count - 1; k++)
                    {
                        if (weight_node[i].status == NodeStatus.ST_LOST)
                            continue;

                        for (int l = k + 1; l < weight_node.Count; l++)
                        {
                            if (weight_node[i].status == NodeStatus.ST_LOST)
                                continue;
                            sum = weight_node[i].weight + weight_node[j].weight + weight_node[k].weight + weight_node[l].weight;
                            if (sum > uvar && sum < dvar)
                            {
                                DoRelease(new SubNode[] { weight_node[i], weight_node[j], weight_node[k], weight_node[l] }, sum);
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
            double uvar = sys_cfg.target + sys_cfg.upper_var;
            double dvar = sys_cfg.target - sys_cfg.lower_var;

            for (int i = 0; i < weight_node.Count - 4; i++)
            {
                if (weight_node[i].status == NodeStatus.ST_LOST)
                    continue;
                for (int j = i + 1; j < weight_node.Count - 3; j++)
                {
                    if (weight_node[i].status == NodeStatus.ST_LOST)
                        continue;
                    for (int k = j + 1; k < weight_node.Count - 2; k++)
                    {
                        if (weight_node[i].status == NodeStatus.ST_LOST)
                            continue;

                        for (int l = k + 1; l < weight_node.Count-1; l++)
                        {
                            if (weight_node[i].status == NodeStatus.ST_LOST)
                                continue;
                            for (int m = l + 1; m < weight_node.Count; m++)
                            {
                                if (weight_node[i].status == NodeStatus.ST_LOST)
                                    continue;

                                sum = weight_node[i].weight + weight_node[j].weight + weight_node[k].weight + weight_node[l].weight + weight_node[m].weight;
                                if (sum > uvar && sum < dvar)
                                {
                                    DoRelease(new SubNode[] { weight_node[i], weight_node[j], weight_node[k], weight_node[l], weight_node[m] }, sum);
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
            while (Caculation1())
            {
            }
//            DoRelease(new SubNode[] { weight_node[0], weight_node[1], weight_node[2], weight_node[3], weight_node[4] }, 123);
            for (int i = 0; i < weight_node.Count; i++ )
            {
                if (weight_node[i].status == NodeStatus.ST_LOST)
                    continue;
                //todo check force release and over_weight case
                weight_node[i]["flag_goon"] = 1;
/*                if (weight_node[i]["flag_goon"] != 1)
                {
                    weight_node[i]["flag_goon"] = 1;
                }
 */
            }
        }
        
        public void StartRun()
        {
            ProdHistory.InitNewRun("sj", sys_cfg.product_no, sys_cfg.product_desc);
            vib_node.Action("start",false);
            foreach (WeighNode n in this.weight_node)
            {
                if (n.status != NodeStatus.ST_LOST)
                {
                    n.Action("start", true);
                }
            }
            status = PackerStatus.RUNNING;
        }
        public void StopRun()
        {
            status = PackerStatus.IDLE;
            ProdHistory.EndNewRun();
            vib_node.Action("stop",false);
            foreach (WeighNode n in this.weight_node)
            {
                if (n.status != NodeStatus.ST_LOST)
                {
                    n.Action("stop", true);
                }
            }
        }

       
        public void ActionAll(string action) //select, deselect, empty, set zero
        {
            if (action == "empty")
            {
                foreach (WeighNode n in this.weight_node)
                {
                    if (n.status != NodeStatus.ST_LOST)
                    {
                        n.Action("empty", true);
                    }
                }
            }
            if (action == "zero")
            {
                foreach (WeighNode n in this.weight_node)
                {
                    if (n.status != NodeStatus.ST_LOST)
                    {
                        n.Action("zero", true);
                    }
                }
            }
            if (action == "select")
            {
                foreach (WeighNode n in this.weight_node)
                {
                    if (n.status != NodeStatus.ST_LOST)
                    {
                        n.uidata.selected = true;
                    }
                }
            }
            if (action == "select")
            {
                foreach (WeighNode n in this.weight_node)
                {
                    if (n.status != NodeStatus.ST_LOST)
                    {
                        n.uidata.selected = true;
                    }
                }
            }
            if (action == "deselect")
            {
                foreach (WeighNode n in this.weight_node)
                {
                    if (n.status != NodeStatus.ST_LOST)
                    {
                        n.uidata.selected = false;
                    }
                }
            }
        }
    }
}
