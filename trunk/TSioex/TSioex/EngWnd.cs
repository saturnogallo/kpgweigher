using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Xml.Linq;
using Microsoft.Win32;
using System.Diagnostics;
using System.Threading;
namespace TSioex
{
    public partial class EngWnd : Form
    {
       private int curr_sel = -1;
       private bool all_sel = false;
       public static bool b_lockon = false;
       private DataTable reg_dt;
        public EngWnd()
        {
            InitializeComponent();
            reg_dt = new DataTable("regs");
            reg_dt.Columns.Add("Reg");
            reg_dt.Columns.Add("Position");
            reg_dt.Columns.Add("Value");
            reg_dt.Columns.Add("HexValue");
            
            dataGrid1.TableStyles[0].GridColumnStyles[0].Width = 200;
            dataGrid1.TableStyles[0].GridColumnStyles[1].Width = 100;
            dataGrid1.TableStyles[0].GridColumnStyles[2].Width = 100;
            dataGrid1.TableStyles[0].GridColumnStyles[3].Width = 100;
            
            
            dataGrid1.DataSource = reg_dt;

            this.BackColor = SingleModeWnd.bgWindow;
            btn_entersys.SetStyle(Color.LightGreen, MyButtonType.roundGradientButton);
            btn_locksys.SetStyle(Color.LightGreen, MyButtonType.roundGradientButton);
            btn_language.SetStyle(Color.LightGreen, MyButtonType.roundGradientButton);
            btn_updatefw.SetStyle(Color.Orange, MyButtonType.roundRectButton);
            btn_changeaddr.SetStyle(Color.Orange, MyButtonType.roundRectButton);
            b_lockon = !Password.compare_pwd("lock", "0");

            nd_1.SetStyle(Color.LightGreen, MyButtonType.roundButton);
            nd_2.SetStyle(Color.LightGreen, MyButtonType.roundButton);
            nd_3.SetStyle(Color.LightGreen, MyButtonType.roundButton);
            nd_4.SetStyle(Color.LightGreen, MyButtonType.roundButton);
            nd_5.SetStyle(Color.LightGreen, MyButtonType.roundButton);
            nd_6.SetStyle(Color.LightGreen, MyButtonType.roundButton);
            nd_7.SetStyle(Color.LightGreen, MyButtonType.roundButton);
            nd_8.SetStyle(Color.LightGreen, MyButtonType.roundButton);
            nd_9.SetStyle(Color.LightGreen, MyButtonType.roundButton);
            nd_10.SetStyle(Color.LightGreen, MyButtonType.roundButton);
            nd_11.SetStyle(Color.LightGreen, MyButtonType.roundButton);
            nd_12.SetStyle(Color.LightGreen, MyButtonType.roundButton);
            nd_13.SetStyle(Color.LightGreen, MyButtonType.roundButton);
            nd_14.SetStyle(Color.LightGreen, MyButtonType.roundButton);
            nd_15.SetStyle(Color.LightGreen, MyButtonType.roundButton);
            nd_16.SetStyle(Color.LightGreen, MyButtonType.roundButton);
            nd_36.SetStyle(Color.LightGreen, MyButtonType.roundButton);

            nd_1.Text = "1";
            nd_2.Text = "2";
            nd_3.Text = "3";
            nd_4.Text = "4";
            nd_5.Text = "5";
            nd_6.Text = "6";
            nd_7.Text = "7";
            nd_8.Text = "8";
            nd_9.Text = "9";
            nd_10.Text = "10";
            nd_11.Text = "11";
            nd_12.Text = "12";
            nd_13.Text = "13";
            nd_14.Text = "14";
            nd_15.Text = "15";
            nd_16.Text = "16";
            nd_36.Text = "36";
            
            nd_1.ValidClick +=new EventHandler(this.nd_1_Click);
            nd_2.ValidClick += new EventHandler(this.nd_1_Click);
            nd_3.ValidClick += new EventHandler(this.nd_1_Click);
            nd_4.ValidClick += new EventHandler(this.nd_1_Click);
            nd_5.ValidClick += new EventHandler(this.nd_1_Click);
            nd_6.ValidClick += new EventHandler(this.nd_1_Click);
            nd_7.ValidClick += new EventHandler(this.nd_1_Click);
            nd_8.ValidClick += new EventHandler(this.nd_1_Click);
            nd_9.ValidClick += new EventHandler(this.nd_1_Click);
            nd_10.ValidClick += new EventHandler(this.nd_1_Click);
            nd_11.ValidClick += new EventHandler(this.nd_1_Click);
            nd_12.ValidClick += new EventHandler(this.nd_1_Click);
            nd_13.ValidClick += new EventHandler(this.nd_1_Click);
            nd_14.ValidClick += new EventHandler(this.nd_1_Click);
            nd_15.ValidClick += new EventHandler(this.nd_1_Click);
            nd_16.ValidClick += new EventHandler(this.nd_1_Click);
            nd_36.ValidClick += new EventHandler(this.nd_1_Click);

            dataGrid1.CurrentCellChanged +=new EventHandler(this.btn_modify_Click);
            if (b_lockon)
            {
                packer_counter = int.Parse(GetRegistData("lock_on"));
                if (!Password.compare_pwd("lock_on", GetRegistData("lock_on")) || (packer_counter < 1))
                {
                    Program.runwnd.Disable(false);
                    Program.singlewnd.Disable(false);
                }
            }
            //check license key
            Password.CheckLicense(GetRegistData("lic"));
            UpdateDisplay(false);
            btn_refreshreg.Click +=new EventHandler(btn_refreshreg_Click);
            btn_refreshaddr.Click +=new EventHandler(btn_refreshaddr_Click);
            btn_locksys.ValidClick += new EventHandler(btn_locksys_Click);
            btn_language.ValidClick += new EventHandler(btn_language_Click);
            btn_entersys.ValidClick += new EventHandler(btn_entersys_Click);
            btn_updatefw.ValidClick += new EventHandler(btn_updatefw_Click);
            btn_changeaddr.ValidClick +=new EventHandler(btn_changeaddr_Click);
            btn_selectall.Click += new EventHandler(btn_selectall_Click);
            btn_cali0.Click +=new EventHandler(this.btn_cali_Click);
            btn_cali1.Click += new EventHandler(this.btn_cali_Click);
            btn_cali2.Click += new EventHandler(this.btn_cali_Click);
            btn_cali3.Click += new EventHandler(this.btn_cali_Click);
            btn_cali4.Click += new EventHandler(this.btn_cali_Click);
            btn_cali5.Click += new EventHandler(this.btn_cali_Click);
            btn_cali6.Click += new EventHandler(this.btn_cali_Click);
            btn_cali7.Click += new EventHandler(this.btn_cali_Click);
            btn_cali8.Click += new EventHandler(this.btn_cali_Click);
            btn_cali9.Click += new EventHandler(this.btn_cali_Click);
            btn_ret_config.Click +=new EventHandler(btn_ret_config_Click);
            btn_ret_run.Click +=new EventHandler(btn_ret_run_Click);
            this.panel1.BackColor = SingleModeWnd.bgWindow;
            this.panel2.BackColor = SingleModeWnd.bgWindow;
             lbl_weight.Click +=new EventHandler(this.label3_MouseLeftButtonUp);
            btn_empty.Click +=new EventHandler(btn_cali_Click);
        }

        public void InitDisplay()
        {
            XElement cfg = Program.curr_cfg;
            
            b_lockon = !Password.compare_pwd("lock","0");
            
            UpdateDisplay(true);
            Program.histwnd.UpdateDisplay();
        }
        public void UpdateDisplay(bool refresh)
        {
            RectButton btn;
            if (b_lockon)
            {
                btn_entersys.SetStyle(Color.Gray, MyButtonType.roundGradientButton);
                btn_locksys.Text = StringResource.str("lockon");
            }
            else
            {
                btn_locksys.SetStyle(Color.LightGreen, MyButtonType.roundGradientButton);
                btn_locksys.Text = StringResource.str("locksystem");
            }
            for (Byte i = 1; i < 18; i++)
            {
                if (i < 17)
                {
                    btn = IdToButton(i.ToString());
                }
                else
                {
                    btn = IdToButton("36");
                }

                if ((NodeMaster.GetErrors(i) != "") || (NodeMaster.GetStatus(i) == NodeStatus.ST_LOST))
                    btn.SetStyle(Color.Gray,MyButtonType.roundButton);
                else
                    btn.SetStyle(Color.DarkGreen, MyButtonType.roundButton);
                btn.Visible = true;
                continue;
            }

            btn_cali0.Text = StringResource.str("cmd_cali0");
            btn_cali1.Text = StringResource.str("cmd_cali1");
            btn_cali2.Text = StringResource.str("cmd_cali2");
            btn_cali3.Text = StringResource.str("cmd_cali3");
            btn_cali4.Text = StringResource.str("cmd_cali4");
            btn_cali5.Text = StringResource.str("cmd_cali5");
            btn_cali6.Text = StringResource.str("cmd_cali6");
            btn_cali7.Text = StringResource.str("cmd_cali7");
            btn_cali8.Text = StringResource.str("cmd_cali8");
            btn_cali9.Text = StringResource.str("cmd_cali9");
            btn_cali10.Text = StringResource.str("cmd_cali10");
            btn_empty.Text = StringResource.str("cmd_empty");
            btn_refreshaddr.Text = StringResource.str("refresh");
            btn_selectall.Text = StringResource.str("cmd_selectall");
            lbl_title.Text = StringResource.str("engineerpage");
            lbl_seladdr.Text = StringResource.str("seladdr");
            lbl_reg.Text = StringResource.str("setregs");
            btn_changeaddr.Text = StringResource.str("changeaddr");
            btn_updatefw.Text = StringResource.str("updatefw");
            btn_refreshaddr.Text = StringResource.str("refresh");
            btn_locksys.Text = StringResource.str("locksystem");
            btn_entersys.Text = StringResource.str("entersystem");
            btn_language.Text = StringResource.str("language");
            btn_refreshreg.Text = StringResource.str("refresh");
        }
        private RectButton IdToButton(string id)
        {
            if (id == "1")  return nd_1;
            if (id == "2")  return nd_2;
            if (id == "3")  return nd_3;
            if (id == "4")  return nd_4;
            if (id == "5")  return nd_5;
            if (id == "6")  return nd_6;
            if (id == "7")  return nd_7;
            if (id == "8")  return nd_8;
            if (id == "9")  return nd_9;
            if (id == "10") return nd_10;
            if (id == "11") return nd_11;
            if (id == "12") return nd_12;
            if (id == "13") return nd_13;
            if (id == "14") return nd_14;
            if (id == "15") return nd_15;
            if (id == "16") return nd_16;
            if (id == "36") return nd_36;
            return null;
        }
        public static void DecreasePacker(int i)
        {
            if (b_lockon)
            {
                packer_counter = packer_counter - i;
                DeleteRegist("lock_on");
                WTRegedit("lock_on", packer_counter.ToString());
            }
        }
        private static int packer_counter;
        
        static EngWnd()
        {
        }
        public static string GetRegistData(string name)
        {
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
        }
        
        private static void WTRegedit(string name, string tovalue)
        {
            RegistryKey hkml = Registry.LocalMachine;
            RegistryKey aimdir = hkml.OpenSubKey("SYSTEM", true);
            //RegistryKey aimdir = software.CreateSubKey("Microsoft");
            aimdir.SetValue(name, tovalue);
        }
        
        private static void DeleteRegist(string name)
        {
            string[] aimnames;
            RegistryKey hkml = Registry.LocalMachine;
            RegistryKey aimdir = hkml.OpenSubKey("SYSTEM", true);
            aimnames = aimdir.GetSubKeyNames();
            foreach (string aimKey in aimnames)
            {
                if (aimKey == name)
                    aimdir.DeleteSubKeyTree(name);
            }
        } 
        public void KbdData(string param, string data)
        {
            
            XElement cfg = Program.curr_cfg;

            if (param == "entersys")
            {
                
                if (Password.compare_pwd("admin",data))
                {
                    Process app = new Process();
                    app.StartInfo.FileName = "Explorer.exe";
                    app.StartInfo.Arguments = "";
                    app.Start();
                    Thread.Sleep(2000);
                    Program.kbdwnd.Hide();
//                    Type shellType = Type.GetTypeFromProgID("Shell.Application");
//                    object shellObject = System.Activator.CreateInstance(shellType);
//                    shellType.InvokeMember("ToggleDesktop", System.Reflection.BindingFlags.InvokeMethod, null, shellObject, null);
                }
                
                return;
            }
            if (param == "quitlock")
            {
                if (Password.compare_pwd("lock", data))
                {
                    b_lockon = false;
                    Password.set_pwd("lock", "0");
                    b_lockon = false;
                    Program.runwnd.Disable(true);
                    Program.singlewnd.Disable(true);
                    UpdateDisplay(true);
                }
            }
            if (param == "enterlock")
            {
                //App should expire after desired packer number reached , data is the actual packer count
                Password.set_pwd("lock_on", data);
                DeleteRegist("lock_on");
                WTRegedit("lock_on", data);
                Password.set_pwd("lock", data);
                Program.SaveAppConfig();
                b_lockon = true;
                UpdateDisplay(false);
            }
            if (param.IndexOf("reg_") == 0)
            {
                
                
                SubNode n = null;
                if (curr_sel == -1)
                    return;
                Byte i = Convert.ToByte(curr_sel);
                NodeMaster.SetNodeReg(i, new string[] { param.Remove(0, 4) }, new UInt32[] {UInt32.Parse(data) });

                btn_refreshreg_Click(null, null);
            }
            if (param.IndexOf("chgnd_") == 0)
            {
                Byte oldaddr = Byte.Parse(param.Remove(0, 6));
                Byte newaddr = Byte.Parse(data);
                NodeMaster.ChangeAddress(oldaddr, newaddr);
            }
            if (param != "cali0" && (param.IndexOf("cali")==0)) //cali1 to cali10
            {

                string msg = StringResource.str("put_poise") + "(" + data + StringResource.str("gram") + ")";

                if (MessageBox.Show(msg, "", MessageBoxButtons.OKCancel, MessageBoxIcon.Asterisk,MessageBoxDefaultButton.Button1) == DialogResult.Cancel)
                    return;
                
                int i = RunModeWnd.StringToId(param) - 1;
                if (curr_sel >= 0)
                {
                    NodeMaster.RefreshNodeReg((byte)curr_sel, new string[]{"cs_mtrl"});
                    UInt32 cs_mtrl_val = NodeMaster.GetNodeReg((byte)curr_sel, "cs_mtrl");
                    if (cs_mtrl_val <= WeighNode.MAX_VALID_WEIGHT)
                    {
                            if(i < 5)
                                NodeMaster.SetNodeReg((byte)curr_sel, new string[]{"poise_weight_gram" + i.ToString()}, new UInt32[]{cs_mtrl_val});
                            else
                                NodeMaster.SetNodeReg((byte)curr_sel, new string[]{"cs_poise" + (i-5).ToString()}, new UInt32[]{cs_mtrl_val});
                            NodeMaster.Action(new byte[]{(byte)curr_sel}, "flash");
                            MessageBox.Show(StringResource.str("calidone"));
                            label3_MouseLeftButtonUp(null, null);
                            return;
                    }
                    MessageBox.Show(StringResource.str("tryagain"));
                    
                }
                return;
            }
        }

        private void calibrate(string calreg)
        {
            Byte i = Convert.ToByte(curr_sel);
            if (curr_sel == -1)
                return;
            if (calreg == "cali0")
            {
                if (curr_sel >= 0)
                {
                    if (MessageBox.Show(StringResource.str("put_empty"), "", MessageBoxButtons.OKCancel, MessageBoxIcon.Asterisk, MessageBoxDefaultButton.Button1) == DialogResult.Cancel)
                        return;

                    NodeMaster.RefreshNodeReg(i, new string[]{"cs_mtrl"});
                    UInt32 val = NodeMaster.GetNodeReg(i, "cs_mtrl");
                    if (val <= WeighNode.MAX_VALID_WEIGHT)
                    {
                        NodeMaster.SetNodeReg(i,new string[]{"cs_zero"}, new UInt32[]{val});
                        NodeMaster.Action(new byte[]{i}, "flash");
                        MessageBox.Show(StringResource.str("calidone"));
                        label3_MouseLeftButtonUp(null, null);
                    }
                    else
                    {
                        MessageBox.Show(StringResource.str("tryagain"));
                    }

                }
            }
            if (calreg == "empty")
            {
                
                if (curr_sel >= 0)
                {
                    NodeMaster.Action(new byte[]{i}, "empty");
                }
                return;
            }
            if (calreg == "cali1")
                KbdData(calreg, "20");
            if (calreg == "cali2")
                KbdData(calreg, "50");
            if (calreg == "cali3")
                KbdData(calreg, "100");
            if (calreg == "cali4")
                KbdData(calreg, "200");
            if (calreg == "cali5")
                KbdData(calreg, "300");
            if (calreg == "cali6")
                KbdData(calreg, "400");
            if (calreg == "cali7")
                KbdData(calreg, "500");
            if (calreg == "cali8")
                KbdData(calreg, "700");
            if (calreg == "cali9")
                KbdData(calreg, "900");
            if (calreg == "cali10")
                KbdData(calreg, "1000");
            
        }
        private void btn_cali_Click(object sender, EventArgs e)
        {
            string calreg = (sender as Button).Name.Remove(0, 4); //remove "btn_" string

                calibrate(calreg);


        }

        private void btn_entersys_Click(object sender, EventArgs e)
        {
            Program.kbdwnd.Init(StringResource.str("enter_system"), "entersys", true, KbdData);
        }

        private void btn_locksys_Click(object sender, EventArgs e)
        {
            XElement cfg = Program.curr_cfg;
            if (b_lockon)
            {
                Program.kbdwnd.Init(StringResource.str("enter_unlocknum"), "quitlock", false, KbdData);
            }
            else
            {
                Program.kbdwnd.Init(StringResource.str("enter_locknum"), "enterlock", false, KbdData);
            }
        }

        private void btn_language_Click(object sender, EventArgs e)
        {
            if (StringResource.language == "zh-CN")
            {
                StringResource.SetLanguage("en-US");
                WTRegedit("locale", "en-US");
                
            }
            else
            {
                StringResource.SetLanguage("zh-CN");
                WTRegedit("locale", "zh-CN");
            }
            UpdateDisplay(false);
        }

        private void nd_1_Click(object sender, EventArgs e)
        {
            if (sender is RectButton)
            {
                RectButton btn = (sender as RectButton);
                if (btn.Name.IndexOf("nd_") == 0)
                {
                    all_sel = false;
                    curr_sel = int.Parse(btn.Name.Remove(0, 3));
                    btn_refreshreg_Click(null, null);
                }
            }
            UpdateDisplay(false);
        }

        private void btn_changeaddr_Click(object sender, EventArgs e)
        {
            btn_updatefw_Click(sender,e);
        }

        public void btn_updatefw_Click(object sender, EventArgs e)
        {
            Process app = new Process();
            app.StartInfo.FileName = "\\TSioex\\KCBTool3.exe";
            app.StartInfo.Arguments = "";
            app.Start();
            Thread.Sleep(2000);
//            Type shellType = Type.GetTypeFromProgID("Shell.Application");
//            object shellObject = System.Activator.CreateInstance(shellType);
//            shellType.InvokeMember("ToggleDesktop", System.Reflection.BindingFlags.InvokeMethod, null, shellObject, null);
            System.Diagnostics.Process.GetCurrentProcess().Kill();

        }
        public void fwprogress(uint percent)
        {
            /*
            Dispatcher.Invoke(System.Windows.Threading.DispatcherPriority.Background, (Action)delegate
            {
                    btn_updatefw.Content = curr_sel.ToString() + ":" + percent.ToString() + "Blocks";
                    fwprgs_bar.Value = percent;
            });
             */
        }
        private void btn_selectall_Click(object sender, EventArgs e)
        {
            all_sel = !all_sel;

            UpdateDisplay(false);
        }

        private void btn_refreshaddr_Click(object sender, EventArgs e)
        {
            UpdateDisplay(true);
        }

        private void btn_refreshreg_Click(object sender, EventArgs e)
        {
                RectButton btn;
            
                SubNode n = null;
                
                if (curr_sel == -1)
                    return;
                Byte i = Convert.ToByte(curr_sel);
                if (i < 17)
                {
                    btn = IdToButton(i.ToString());
                }
                else
                {
                    btn = IdToButton("36");
                }
                reg_dt.Rows.Clear();
                    foreach (string rtkey in SubNode.reg_type_tbl.Keys)
                    {
                        NodeMaster.RefreshNodeReg(i, new string[]{rtkey });
                        DataRow dr = reg_dt.NewRow();
                        dr[0] = rtkey; 
                        dr[1] = SubNode.reg_type_tbl[rtkey].pos.ToString();
                        dr[2] = NodeMaster.GetNodeReg(i, rtkey).ToString();
                        dr[3] = NodeMaster.GetNodeReg(i, rtkey).ToString("X");
                        reg_dt.Rows.Add(dr);                     
                    }
                    return;
        }


        private void btn_ret_run_Click(object sender, EventArgs e)
        {
            Program.SwitchTo("runmode");
            //this.Hide();
        }

        private void btn_modify_Click(object sender, EventArgs e)
        {
            if((dataGrid1.CurrentRowIndex >= 0) && (dataGrid1.CurrentRowIndex < reg_dt.Rows.Count))
                  Program.kbdwnd.Init(StringResource.str("enter_newvalue"), "reg_" + reg_dt.Rows[dataGrid1.CurrentRowIndex][0], false, KbdData);

        }

        private void btn_ret_config_Click(object sender, EventArgs e)
        {
            
            Program.SwitchTo("configmenu");
            //this.Hide();
        }

        private void label3_MouseLeftButtonUp(object sender, EventArgs e)
        {
            if (curr_sel == -1)
                return;
            Byte i = Convert.ToByte(curr_sel);

            lbl_weight.Text = NodeMaster.GetWeight(i).ToString("F2");
        }


    }
    internal class PerReg
    {
        public string name { get; set; }
        public string address { get; set; }
        public string value {get;set;}
        public string vhex { get; set; }
    }        


 
}