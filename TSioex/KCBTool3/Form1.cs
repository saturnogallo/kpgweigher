using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using Microsoft.Win32;
using System.IO;

namespace KCBTool3
{
    public struct Pernode
    {
        public string fwversion;
        public string type;
    }

    public delegate void VoidHandler();
    public partial class Form1 : Form
    {
        delegate void CompleteFlashHandler(byte id, object sender);
        Dictionary<byte, SubNode> nodemap;
        Dictionary<byte, Pernode> nodestatus;
        List<byte> nodelist;
        SPort sp = null;
        bool bOpen = false;
        public static string fwpath = "";
        long fwlen = 0;
        private string language;
        private string GetText(string id)
        {
            if (language == "en-US")
            {
                if (id == "Open") return id;
                if (id == "Close") return id;
                if (id == "select_addrfrom") return "Please select a node to operate";
                if (id == "select_addrto") return "Please select the target address";
                if (id == "success") return "Operation completed suceessfully";
                if (id == "fail") return "Operation failed";
            }
            if (language == "zh-CN")
            {
                if (id == "Open") return "打开";
                if (id == "Close") return "关闭";
                if (id == "select_addrfrom") return "请选择一个节点";
                if (id == "select_addrto") return "请选择目标地址";
                if (id == "success") return "操作成功完成";
                if (id == "fail") return "操作失败";
            }
            return id;
        }
        public Form1()
        {
            InitializeComponent();
            sp = null;
            nodemap = new Dictionary<byte, SubNode>();
            nodestatus = new Dictionary<byte, Pernode>();
            nodelist = new List<byte>();
            lb_addrto.SelectedIndex = 0;
            cb_com.SelectedIndex = 0;
//            lb_typeto.SelectedIndex = 0;
            if (GetRegistData("locale") == "zh-CN")
                language = "zh-CN";
            else
                language = "en-US";
            SubNode.Dummy();
            panel4.Visible = false;
//            panel2.Visible = false;
            panel3.Visible = false;
        }

        private void UpdateAllNodeStatus()
        {
            if (bOpen == false)
            {
                panel4.Visible = false;
                panel3.Visible = false;
                lb_addrfrom.Items.Clear();
                btn_open.Text = GetText("Open");
                return;
            }
            else
            {
                panel4.Visible = true;
                panel3.Visible = true;
            }
            btn_open.Text = GetText("Close");

            nodemap.Clear();
            nodestatus.Clear();
            nodelist.Clear();
            lb_addrfrom.Items.Clear();

            lb_addrfrom.Items.Add("All boards");
            nodelist.Add(0);
            byte[] tofind = new byte[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 36 };
            foreach(byte i in tofind)
            {
                SubNode n = new SubNode(sp, i);
                n.status = NodeStatus.ST_IDLE;
                n["addr"] = null;
                if (!WaitForIdleSlow(n))
                    continue;
                n["board_id"] = null;//get board typ
                if (!WaitForIdleSlow(n))
                    continue;
                string org = n["board_id"].Value.ToString();
                n["fw_rev_uw"] = null;//get revistion
                if (!WaitForIdleSlow(n))
                    continue;
                nodemap[i] = n;
                nodestatus[i] = new Pernode();
                
                string type = "Unknown";
                if(n["board_id"].Value >= 16 && n["board_id"].Value < 32)
                    type = "Weight Node";
                if(n["board_id"].Value < 16)
                    type = "Top Node    ";
                lb_addrfrom.Items.Add("Node " + i.ToString("D2") + "  ; Type: " + type + "  ; Rev: " + n["fw_rev_uw"].Value.ToString());
                nodelist.Add(i);
            }
        }

        private bool WaitForIdleSlow(SubNode n)
        {
            int i = 2;
            //int j = 0;
            while (n.status == NodeStatus.ST_BUSY)
            {
                Thread.Sleep(5);
                //i = i + (i + 1) / 2; //8,12,18,28,42,64,96,
                if (i++ > 30)
                {
                    return false;
                }
            }
            return true;
        }
        
        private void btn_changeaddr_Click(object sender, EventArgs e)
        {
            if(lb_addrfrom.SelectedIndex <= 0)
            {
                Program.MsgShow(GetText("select_addrfrom"));
                return;
            }
            if(lb_addrto.SelectedIndex < 0)
            {
                Program.MsgShow(GetText("select_addrto"));
                return;
            }
            btn_changeaddr.Visible = false;
            SubNode n = nodemap[nodelist[lb_addrfrom.SelectedIndex]];
            n["test_mode_reg2"] = Convert.ToByte(0xC0+lb_addrto.SelectedIndex + 1);
            Thread.Sleep(500);
            this.Invoke(new CompleteFlashHandler(CompleteFlash), new object[] { Convert.ToByte(lb_addrto.SelectedIndex + 1), btn_changeaddr });
        }
        /*
        private void btn_changetype_Click(object sender, EventArgs e)
        {
            
            if (lb_addrfrom.SelectedIndex <= 0)
            {
                Program.MsgShow(GetText("select_addrfrom"));
                return;
            }
            if (lb_typeto.SelectedIndex < 0)
            {
                Program.MsgShow(GetText("select_addrto"));
                return;
            }
            this.Invoke(new VoidHandler(this.hidetypebtn));
            
            SubNode n = nodemap[nodelist[lb_addrfrom.SelectedIndex]];
            byte org = Convert.ToByte(n["board_id"] % 16);
            n["board_id"] = Convert.ToUInt32(org + (lb_typeto.SelectedIndex * 16));
            Thread.Sleep(300);
            this.Invoke(new CompleteFlashHandler(CompleteFlash), new object[] { nodelist[lb_addrfrom.SelectedIndex], btn_changetype });
            btn_changetype.Visible = true;

        }*/
        public void CompleteFlash(byte id, object sender)
        {
            /*
            if (sender == btn_changetype)
            {
                nodemap[id]["NumOfDataToBePgmed"] = 45;
                Thread.Sleep(1000);
                UpdateAllNodeStatus();
                Program.MsgShow(GetText("success"));
                btn_changetype.Visible = true;
            }
             */
            if (sender == btn_changeaddr)
            {
                Thread.Sleep(100);
                UpdateAllNodeStatus();
                if (nodemap.ContainsKey(id))
                {
                    nodemap[id]["NumOfDataToBePgmed"] = 45;
                    Thread.Sleep(1000);
                    Program.MsgShow(GetText("success"));
                }
                else
                {
                    Program.MsgShow(GetText("fail"));
                }
                btn_changeaddr.Visible = true;
            }
        }
        private void btn_choosefw_Click(object sender, EventArgs e)
        {
            openFileDialog1.Filter = "BIN File(*.bin)|*.bin";
            openFileDialog1.InitialDirectory = "\\Storage Card";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                if (File.Exists(openFileDialog1.FileName))
                {
                    fwpath = openFileDialog1.FileName;

                    FileInfo fi= new FileInfo(fwpath);
                    lbl_fw.Text = fi.Name;
                    fwlen = fi.Length;
                    if (fwpath != "")
                    {
                        panel2.Visible = true;
                    }

                }
            }
        }
        private void btn_open_Click(object sender, EventArgs e)
        {
            if(bOpen)
            {
                sp.Close();
                bOpen = false;
                UpdateAllNodeStatus();
                return;
            }
            sp = new SPort(cb_com.SelectedItem.ToString(), 115200, System.IO.Ports.Parity.Even, 8, System.IO.Ports.StopBits.One);
            bOpen = sp.Open();
            if (!bOpen)
                return;
            UpdateAllNodeStatus();
        }
        public delegate void UpdateProgress(uint percent);
        private int curr_sel = -1;
        public void updatebar(uint percent)
        {
            lbl_fwstatus.Text = curr_sel + ": " + percent.ToString() + "Blocks";
            this.progressBar_fw.Value = Convert.ToInt16(percent);
        }
        public void fwprogress(uint percent)
        {
            this.Invoke(new UpdateProgress(updatebar), new object[]{percent});
            this.Invoke(new VoidHandler(Invalidate), new object[]{});
        }
        /*
        public void hidetypebtn()
        {
            btn_changetype.Visible = false;
        }*/
        public void hidebtn()
        {
            btn_updatefw.Visible = false;
        }
        public void hiderefreshbtn()
        {
            btn_refresh.Visible = false;
        }
        private void btn_updatefw_Click(object sender, EventArgs e)
        {
            if(lb_addrfrom.SelectedIndex < 0)
            {
                Program.MsgShow(GetText("select_addrfrom"));
                return;
            }

            this.Invoke(new VoidHandler(this.hidebtn));
            string ret = "";
            foreach (byte i in nodemap.Keys)
            {
                if (lb_addrfrom.SelectedIndex == 0 || i == nodelist[lb_addrfrom.SelectedIndex])
                {
                    progressBar_fw.Visible = true;
                    progressBar_fw.Minimum = 0;
                    progressBar_fw.Maximum = (int)(fwlen / 128);
                    curr_sel = i;
                    bootloader bl = new bootloader(nodemap[i]);
                    ret = bl.download(fwprogress,fwpath);
                    if (ret != "")
                        break;
                }
            }
            btn_updatefw.Visible = true;
            progressBar_fw.Visible = true;
            Thread.Sleep(1000);
            UpdateAllNodeStatus();
            if (ret == "")
                Program.MsgShow(GetText("success"));
            else
                Program.MsgShow(GetText("fail"));
            
        }
        private void btn_refresh_Click(object sender, EventArgs e)
        {
            this.Invoke(new VoidHandler(this.hiderefreshbtn));
            UpdateAllNodeStatus();
            btn_refresh.Visible = true;
        }
        private void copyonefile(string src, string dst)
        {
            if(File.Exists(dst))
            {
                FileInfo fa = new FileInfo(dst);
                if ((fa.Attributes & FileAttributes.ReadOnly) == FileAttributes.ReadOnly)
                {
                    fa.Attributes = fa.Attributes & (~FileAttributes.ReadOnly);
                }
            }
            File.Copy(src, dst, true);

        }
        private const string basedir = "\\NANDFlash\\TSioex";

        //private const string basedir = "\\Storage Card\\TSioex";
        private void btn_updatepic_Click(object sender, EventArgs e)
        {
            bool done = false;
            openFileDialog1.Filter = "File(*.exe)|*.exe";
            openFileDialog1.InitialDirectory = "\\Storage Card";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                FileInfo fi2 = new FileInfo(openFileDialog1.FileName);
                string SelectedPath = fi2.Directory.FullName;
                try
                {
                    if (Directory.Exists(SelectedPath + "\\prodpic\\zh-CN"))
                    {
                        foreach (string file in Directory.GetFiles(SelectedPath + "\\prodpic\\zh-CN", "*.jpg"))
                        {
                            FileInfo fi = new FileInfo(file);
                            copyonefile(fi.FullName, basedir + "\\prodpic\\zh-CN\\" + fi.Name);
                        }
                    }
                    if (Directory.Exists(SelectedPath + "\\prodpic\\en-US"))
                    {
                        foreach (string file in Directory.GetFiles(SelectedPath + "\\prodpic\\en-US", "*.jpg"))
                        {
                            FileInfo fi = new FileInfo(file);
                            copyonefile(fi.FullName, basedir + "\\prodpic\\en-US\\" + fi.Name);
                        }
                    }
                    done = true;
                }
                catch (Exception ex)
                {
                    done = false;
                }
                if (done)
                    Program.MsgShow(GetText("success"));
                else
                    Program.MsgShow(GetText("fail"));

            }
        }
        private void btn_updateprg_Click(object sender, EventArgs e)
        {
            bool done = false;
            openFileDialog1.InitialDirectory = "\\Storage Card";
            openFileDialog1.Filter = "File(*.exe)|*.exe";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                FileInfo fi2 = new FileInfo(openFileDialog1.FileName);
                string SelectedPath = fi2.Directory.FullName;
                try
                {
                    if (File.Exists(SelectedPath + "\\TSioex.exe"))
                    {
                        copyonefile(SelectedPath + "\\TSioex.exe", basedir+"\\Tsioex.exe");
                    }
                    foreach (string file in Directory.GetFiles(SelectedPath, "*.dll"))
                    {
                        FileInfo fi = new FileInfo(file);
                        copyonefile(file, basedir + "\\" + fi.Name);
                    }
                    done = true;
                }
                catch(Exception ex)
                {
                    done = false;
                }

            }
            if (done)
                Program.MsgShow(GetText("success"));
            else
                Program.MsgShow(GetText("fail"));

        }
        public static string GetRegistData(string name)
        {
            return "";
/*
            string registData;
            RegistryKey hkml = Registry.LocalMachine;
            RegistryKey aimdir = hkml.OpenSubKey("SYSTEM", true);
            try
            {
                if (!aimdir.GetValueNames().Contains(name))
                    return "0";
                registData = aimdir.GetValue(name).ToString();
                return registData;
            }
            catch
            {
                return "";
            }
 */
        }


    }
}