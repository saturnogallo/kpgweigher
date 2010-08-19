using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

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
            cfgNode.Add(new XElement("product_no", target));
            cfgNode.Add(new XElement("product_desc", target));
        }

    }

    public class onepack
    {
        public double weight { get; set; }
        public char bucket1 { get; set; }
        public char bucket2 { get; set; }
        public char bucket3 { get; set; }
        public char bucket4 { get; set; }
        public char bucket5 { get; set; }
        public onepack(double w, char b1, char b2, char b3, char b4, char b5)
        {
            weight = w; bucket1 = b1; bucket2 = b2; bucket3 = b3; bucket4 = b4; bucket5 = b5;
        }
        public onepack()
        {

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
        static string pack_define_file = "pack_define.xml";

        public PackerStatus status { get; set; } //status of running

        public Packer()
        {
            vib_node = null;
            pack_node = null;
            weight_node = new List<WeighNode>();
            all_conf = new XmlConfig(pack_define_file);
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
        //load all the configuration and update the UI,
        //packer and sub node will share the same configuration name
        public void LoadConfig(string cfgname)
        {
            foreach (WeighNode n in this.weight_node)
            {
                n.LoadCurrentConfig(cfgname);
            }
            vib_node.LoadCurrentConfig(cfgname);
            pack_node.LoadCurrentConfig(cfgname);
            
            //load the configuration of packer settings
            XElement cfgNode = all_conf.LoadConfig(cfgname);

            sys_cfg.FromElement(cfgNode);
            vib_node.FromElement(cfgNode);
            pack_node.FromElement(cfgNode);
        }
        //convert the current configuration into an XElement node
        private void SaveCurrentConfig()
        {
            XElement cfgNode = new XElement("Item");
            sys_cfg.ToElement(cfgNode);
            vib_node.ToElement(cfgNode);
            pack_node.ToElement(cfgNode);
            all_conf.AddConfig(all_conf.cfg_name, cfgNode);
            all_conf.SaveConfigToFile();
        }

        public void DuplicateCurrentConfig(string newcfg)
        {
            //add new configuration
            foreach (WeighNode n in this.weight_node)
            {
                n.DuplicateCurrentConfig(newcfg);
            }
            vib_node.DuplicateCurrentConfig(newcfg);
            pack_node.DuplicateCurrentConfig(newcfg);

            XElement cfgNode = new XElement("NodeProperty");
            sys_cfg.ToElement(cfgNode);
            vib_node.ToElement(cfgNode);
            pack_node.ToElement(cfgNode);

            all_conf.AddConfig(newcfg, cfgNode);
            all_conf.SaveConfigToFile();
            this.LoadConfig(newcfg);    
        }
        private void DoRelease(SubNode[] addrs, double weight)
        {
            foreach (SubNode n in addrs)
            {
                n.Action("release",false);
            }
            //check the bSimulate
            //todo add record to datalog.xml
        }
        //one bucket combination
        private void Caculation1()
        {
            double sum = 0;
            double uvar = sys_cfg.target + sys_cfg.upper_var;
            double dvar = sys_cfg.target - sys_cfg.lower_var;

            for (int i = 0; i < weight_node.Count; i++ )
            {
                if(weight_node[i].status != NodeStatus.ST_RUNNING)
                    continue;
                sum = weight_node[i].weight;
                if(sum > uvar && sum < dvar)
                {
                    DoRelease(new SubNode[] { weight_node[i] }, sum);
                }
            }
        }
        private void Caculation2()
        {
            double sum = 0;
            double uvar = sys_cfg.target + sys_cfg.upper_var;
            double dvar = sys_cfg.target - sys_cfg.lower_var;

            for (int i = 0; i < weight_node.Count-1; i++)
            {
                if (weight_node[i].status != NodeStatus.ST_RUNNING)
                    continue;
                for (int j = i + 1; j < weight_node.Count; j++)
                {
                    if (weight_node[j].status != NodeStatus.ST_RUNNING)
                        continue;
                    sum = weight_node[i].weight + weight_node[j].weight;
                    if (sum > uvar && sum < dvar)
                    {
                        DoRelease(new SubNode[] { weight_node[i], weight_node[j]}, sum);
                    }
                }
            }
        }
        private void Caculation3()
        {
            double sum = 0;
            double uvar = sys_cfg.target + sys_cfg.upper_var;
            double dvar = sys_cfg.target - sys_cfg.lower_var;

            for (int i = 0; i < weight_node.Count - 2; i++)
            {
                if (weight_node[i].status != NodeStatus.ST_RUNNING)
                    continue;
                for (int j = i + 1; j < weight_node.Count-1; j++)
                {
                    if (weight_node[j].status != NodeStatus.ST_RUNNING)
                        continue;
                    for (int k = j + 1; k < weight_node.Count ; k++)
                    {
                        if (weight_node[k].status != NodeStatus.ST_RUNNING)
                            continue;

                        sum = weight_node[i].weight + weight_node[j].weight + weight_node[k].weight;
                        if (sum > uvar && sum < dvar)
                        {
                            DoRelease(new SubNode[] { weight_node[i], weight_node[j], weight_node[k] }, sum);
                        }
                    }
                }
            }
        }
        private void Caculation4()
        {
            double sum = 0;
            double uvar = sys_cfg.target + sys_cfg.upper_var;
            double dvar = sys_cfg.target - sys_cfg.lower_var;

            for (int i = 0; i < weight_node.Count - 3; i++)
            {
                if (weight_node[i].status != NodeStatus.ST_RUNNING)
                    continue;
                for (int j = i + 1; j < weight_node.Count - 2; j++)
                {
                    if (weight_node[j].status != NodeStatus.ST_RUNNING)
                        continue;
                    for (int k = j + 1; k < weight_node.Count - 1; k++)
                    {
                        if (weight_node[k].status != NodeStatus.ST_RUNNING)
                            continue;

                        for (int l = k + 1; l < weight_node.Count; l++)
                        {
                            if (weight_node[l].status != NodeStatus.ST_RUNNING)
                                continue;
                            sum = weight_node[i].weight + weight_node[j].weight + weight_node[k].weight + weight_node[l].weight;
                            if (sum > uvar && sum < dvar)
                            {
                                DoRelease(new SubNode[] { weight_node[i], weight_node[j], weight_node[k], weight_node[l] }, sum);
                            }
                        }
                    }
                }
            }
        }
        private void Caculation5()
        {
            double sum = 0;
            double uvar = sys_cfg.target + sys_cfg.upper_var;
            double dvar = sys_cfg.target - sys_cfg.lower_var;

            for (int i = 0; i < weight_node.Count - 4; i++)
            {
                if (weight_node[i].status != NodeStatus.ST_RUNNING)
                    continue;
                for (int j = i + 1; j < weight_node.Count - 3; j++)
                {
                    if (weight_node[j].status != NodeStatus.ST_RUNNING)
                        continue;
                    for (int k = j + 1; k < weight_node.Count - 2; k++)
                    {
                        if (weight_node[k].status != NodeStatus.ST_RUNNING)
                            continue;

                        for (int l = k + 1; l < weight_node.Count-1; l++)
                        {
                            if (weight_node[l].status != NodeStatus.ST_RUNNING)
                                continue;
                            for (int m = l + 1; m < weight_node.Count; m++)
                            {
                                if (weight_node[m].status != NodeStatus.ST_RUNNING)
                                    continue;

                                sum = weight_node[i].weight + weight_node[j].weight + weight_node[k].weight + weight_node[l].weight + weight_node[m].weight;
                                if (sum > uvar && sum < dvar)
                                {
                                    DoRelease(new SubNode[] { weight_node[i], weight_node[j], weight_node[k], weight_node[l], weight_node[m] }, sum);
                                }
                            }
                        }
                    }
                }
            }
        }

        public void CheckCombination()
        {
            //todo
        }
        
        public void StartRun()
        {
            vib_node.Action("start",false);
            status = PackerStatus.RUNNING;
        }
        public void StopRun()
        {
            status = PackerStatus.IDLE;
            vib_node.Action("stop",false);
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
