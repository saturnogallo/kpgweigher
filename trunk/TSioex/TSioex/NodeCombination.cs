using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO.Ports;
using System.Xml.Linq;
using System.Windows.Forms;
using System.Diagnostics;
namespace TSioex
{
    public class CombineEventArgs : EventArgs
    {
        public CombineEventArgs(byte packer_id, byte[] release_addrs,double[] release_wts, double weight)
        {
            this.packer_id = packer_id;
            this.release_addrs = release_addrs;
            this.release_wts = release_wts;
            this.weight = weight;
        }
        public byte packer_id;
        public byte[] release_addrs;
        public double[] release_wts;
        public double weight;
    }	//end of class CombineEventArgs



    internal class NodeCombination   //the class is used to do combinations and release action
    {
        private byte[] release_addrs = null;
        private double[] release_wts = null;
        private double release_weight;
        private int pack_cnt = 0; //counter for packers before one trigger.
        private UInt32 release_cnt = 0; //trigger count;
        private UInt32 release_timeout = 0;
        public static Queue<CombineEventArgs> q_hits = new Queue<CombineEventArgs>();
        private double llim
        { //low limit of packer setting
            get
            {
                if (packer.curr_cfg.upper_var < packer.curr_cfg.lower_var)
                    return packer.curr_cfg.upper_var / 5;
                else
                    return packer.curr_cfg.lower_var / 5;
                
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

        public delegate void HitCombineEventHandler(object sender, CombineEventArgs ce);
        

        internal NodeCombination(UIPacker p)
        {
            packer = p;

            bSimNodeValid = new Dictionary<byte,bool>();
            foreach (byte i in packer.weight_nodes)
                bSimNodeValid[i] = true;
            bSimCombine = false;

            rand = new Random();
            pack_cnt = 0;
            release_cnt = 0;
            release_timeout = 0;
        }
        static public int phase = 0;        
        public void Step()
        {
            //phase 0_start, 1_collect weight, 2_comb_release, 3 packer,
            if (phase == 0)
            {
                NodeMaster.ClearWeights(packer.wn_addrs);
                NodeMaster.Action(packer.wn_addrs, "query");
                phase = 10;
                return;
            }
            //collect weight
            if (phase == 10)
            {
                int cnt = packer.wn_addrs.Length;
                foreach (byte addr in packer.wn_addrs)
                {
                    double wt = NodeMaster.GetWeight(addr);
                    if(NodeMaster.GetStatus(addr) != NodeStatus.ST_IDLE)
                    {
                        cnt--;
                        continue;
                    }
                    if (wt > -1000 && wt <= WeighNode.MAX_VALID_WEIGHT)
                    cnt--;                 
                }
                if (cnt <= NodeAgent.LASTCOMB_NUM)
                {
                    phase = 20;
                    return;
                }
                //prepare for next round query
                if (cnt > 0)
                {
                byte[] addrs = new byte[cnt];
                    //
                int idx = 0;
                foreach (byte addr in packer.wn_addrs)
                {
                    if (NodeMaster.GetStatus(addr) != NodeStatus.ST_IDLE)
                    {
                            continue;
                        }
                        double wt = NodeMaster.GetWeight(addr);
                        if (wt > -1000 && wt <= WeighNode.MAX_VALID_WEIGHT)
                            continue;
                        
                        addrs[idx++] = addr;
                }
                }
                NodeMaster.Action(packer.wn_addrs, "query");
                return;
            }
            if (phase == 20)
            {
                if (packer.status != PackerStatus.RUNNING)
                {
                    phase = 0;
                    return;
                }
                //do simulated combination
                bSimCombine = true;
                foreach (byte b in packer.weight_nodes)
                    bSimNodeValid[b] = true;
                while (CheckCombination())
                    ;
                bSimCombine = false;
                foreach (byte b in packer.weight_nodes)
                {
                    if (!bSimNodeValid[b]) //has a combination
                    {
                        phase = 30;
                    }
                }
                if (phase == 30)
                    ProcessGoonNodes();
                else  //no combination at all
                {
                    while (ProcessGoonNodes())
                        ;
                    CheckNodeStatus();
                    phase = 0;
                    return;
                }
            }
            if (phase == 30)
            {
                if (!CheckCombination())
                {
                    phase = 50;
                    return;
                }
                ProcessGoonNodes();
                ReleaseAction(release_addrs, release_weight);
                q_hits.Enqueue(new CombineEventArgs((byte)packer._pack_id, release_addrs, release_wts, release_weight));
                

                Intf intf_reg = new Intf(Convert.ToUInt16(NodeMaster.GetNodeReg(packer.vib_addr, "target_weight")));
                //if (pack_cnt % (intf_reg.feed_times + 1) != 0)
                //{
                //    phase = 30;
                //    return;
                //}
                {
                    pack_cnt = 0;
                    NodeMaster.Action(new byte[] { packer.vib_addr }, "trigger");
                    ProcessGoonNodes();
                    phase = 40;
                }
            }
            if (phase == 40)
            {
                NodeMaster.RefreshNodeReg(packer.vib_addr, new string[] { "pack_rel_cnt" });
                UInt32 lw_ub = NodeMaster.GetNodeReg(packer.vib_addr, "pack_rel_cnt");
                if (lw_ub != release_cnt)
                {
                    release_cnt = lw_ub;
                    release_timeout = 0;
                    phase = 30;
                }
                else
                {
                    if (release_timeout++ > 4)
                    {
                        release_timeout = 0;
                        NodeMaster.Action(new byte[] { packer.vib_addr }, "trigger");
                    }
                    ProcessGoonNodes();
                    return;
                }
            }
            if (phase == 50)
            {
                NodeMaster.Action(new byte[] { packer.vib_addr }, "fill");
                while (ProcessGoonNodes())
                    ;
                CheckNodeStatus();
                phase = 0;
                return;
            }
           
            //do combination 
            //release action
        }
        public void Start()
        {
            q_hits.Clear();
            release_timeout = 0;
        }
        public void Stop(int ms)
        {
            while((phase != 0) && (ms--) > 0)
            {
                Step();
                if (phase == 50)
                    phase = 0;
                Thread.Sleep(1);
            }
            phase = 0;
        }
        public void CheckNodeStatus() //update node status after combination is completed
        {
            double wt;
            foreach (byte addr in packer.weight_nodes)
            {
                if (NodeMaster.GetStatus(addr) == NodeStatus.ST_LOST || NodeMaster.GetStatus(addr) == NodeStatus.ST_DISABLED)
                    continue;
                //no match case
                NodeMaster.IncMatchCount(addr);
                //over_weight check
                wt = NodeMaster.GetWeight(addr);
                double nw = wt;
                if (nw > (packer.curr_cfg.target * 0.66) && (nw <= WeighNode.MAX_VALID_WEIGHT)) //has overweight
                {
                    if (AlertWnd.b_turnon_alert)
                    {
                        NodeMaster.SetOverWeight(addr, true);
                        if (!AlertWnd.b_manual_reset) //auto reset
                        {
                            NodeMaster.Action(new byte[]{addr}, "empty");
                        }
                    }
                    continue;
                }
                else
                {
                    if ((nw > -1000) && (nw <= WeighNode.MAX_VALID_WEIGHT))
                    {
                        NodeMaster.SetOverWeight(addr, false);
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
                if (NodeMaster.GetStatus(addr) == NodeStatus.ST_LOST || NodeMaster.GetStatus(addr) == NodeStatus.ST_DISABLED)
                    continue;
                double wt = NodeMaster.GetWeight(addr);
                if ((wt < (packer.curr_cfg.target / 2)) && (wt >= 0.0) && (wt <= WeighNode.MAX_VALID_WEIGHT))
                {
                    NodeMaster.Action(new byte[]{addr}, "flag_goon"); //no match hit
                }
                else
                {
                    if ((wt > packer.curr_cfg.target / 2) && (wt <= WeighNode.MAX_VALID_WEIGHT))
                        wt = 100;
                }
                bSimNodeValid[addr] = false;
                return true;
            }
            return false;
        }
        public static void logTimeStick(TimeSpan ts1,string step)
        {
            TimeSpan ts2 = new TimeSpan(DateTime.Now.Ticks);
            TimeSpan ts = ts2.Subtract(ts1).Duration();
            Debug.WriteLine(step+ts.Milliseconds + "ms");
            
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
                release_wts = new double[addrs.Length];
                for (int i = 0; i < release_wts.Length; i++)
                    release_wts[i] = NodeMaster.GetWeight(addrs[i]);
            }
        }
        private bool bSimCombine; //simulate the combination to see whether there is node need to goon ealier.
        private Dictionary<byte,bool> bSimNodeValid; //array used to store the state of each node in simulation mode;
        
        private bool CacuInvalidNode(int i)
        {
            if (NodeMaster.GetStatus(packer.weight_nodes[i]) == NodeStatus.ST_LOST)
                return true;
            if (NodeMaster.GetStatus(packer.weight_nodes[i]) == NodeStatus.ST_DISABLED)
                return true;

            if ((bSimCombine) && (!bSimNodeValid[packer.weight_nodes[i]]))
                return true;
            double wt = NodeMaster.GetWeight(packer.weight_nodes[i]);
            if (wt <= llim)
                return true;
            if (wt < 0)
                return true;
            if (wt > WeighNode.MAX_VALID_WEIGHT)
                return true;
            return false;
        }
        //one bucket combination
        private bool Caculation1()
        {
            double sum = 0;
            
            for (int i = 0; i < packer.weight_nodes.Count; i++)
            {
                if (CacuInvalidNode(i))
                    continue;
                sum = NodeMaster.GetWeight(packer.weight_nodes[i]);
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

                    sum = NodeMaster.GetWeight(packer.weight_nodes[i]) + NodeMaster.GetWeight(packer.weight_nodes[j]);
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

                        sum = NodeMaster.GetWeight(packer.weight_nodes[i]) + NodeMaster.GetWeight(packer.weight_nodes[j]) + NodeMaster.GetWeight(packer.weight_nodes[k]);
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

                            sum = NodeMaster.GetWeight(packer.weight_nodes[i]) + NodeMaster.GetWeight(packer.weight_nodes[j]) + NodeMaster.GetWeight(packer.weight_nodes[k]) + NodeMaster.GetWeight(packer.weight_nodes[l]);
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

                                sum = NodeMaster.GetWeight(packer.weight_nodes[i]) + NodeMaster.GetWeight(packer.weight_nodes[j]) + NodeMaster.GetWeight(packer.weight_nodes[k]) + NodeMaster.GetWeight(packer.weight_nodes[l]) + NodeMaster.GetWeight(packer.weight_nodes[m]);
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
                log = log + String.Format("({0}){1}\t", n, NodeMaster.GetWeight(n));

                while(true)
                {
                try
                {
                    NodeMaster.Action(new byte[] { n }, "flag_release");
                    break;
                }
                catch
                {
                    Program.MsgShow(StringResource.str("tryagain"));
                }
                }
            }
            NodeMaster.HitMatch(addrs);
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
                if (Caculation1() || Caculation2() || Caculation3() || Caculation4())// || Caculation5())
                {
                    return true;

                }
                if (NodeAgent.IsDebug)
                {
                    if (rand.NextDouble() > 0.6)
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
 
}
