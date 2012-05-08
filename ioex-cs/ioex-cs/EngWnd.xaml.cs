using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Threading;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Xml.Linq;
using System.IO;
using Microsoft.Win32;
using System.Diagnostics;

namespace ioex_cs
{
    /// <summary>
    /// Interaction logic for EngConfigWnd.xaml
    /// </summary>
    public partial class EngConfigWnd : Window
    {
        private int curr_sel = -1;
        private bool all_sel = false;
        public static bool b_lockon = false;

        public EngConfigWnd()
        {
            InitializeComponent();
            b_lockon = !Password.compare_pwd("lock", "0");
            
            if (b_lockon)
            {
                packer_counter = int.Parse(GetRegistData("lock_on"));
                if (!Password.compare_pwd("lock_on", GetRegistData("lock_on")) || (packer_counter < 1))
                {
                    (Application.Current as App).runwnd.Disable(Visibility.Hidden);
                    (Application.Current as App).singlewnd.Disable(Visibility.Hidden);
                }
            }
            //check license key
            Password.CheckLicense(GetRegistData("lic"));
            
        }
        public void InitDisplay()
        {
            App p = Application.Current as App;
            
            XElement cfg = p.curr_cfg;
            
            b_lockon = !Password.compare_pwd("lock","0");
            
            p.agent.Stop(500);
            foreach (UIPacker pk in p.packers)
            {
                pk.nc.Stop(500);
            }
            UpdateDisplay(true);
            p.histwnd.UpdateDisplay();
        }
        public void UpdateDisplay(bool refresh)
        {
            SubNode n;
            App p = Application.Current as App;
            Button btn;
            if (b_lockon)
            {
                btn_locksys.Style = this.FindResource("ButtonStyleOff") as Style;
                btn_locksys.Content = StringResource.str("lockon");
            }
            else
            {
                btn_locksys.Style = this.FindResource("ButtonStyleOn") as Style;
                btn_locksys.Content = StringResource.str("locksystem");
            }
            for (Byte i = 1; i < 18; i++)
            {
                n = null;
                if (i < 17)
                {
                    btn = IdToButton(i.ToString());
                    n = p.agent[i];
                }
                else
                {
                    btn = IdToButton("36");
                    n = p.agent.missingnode;
                }

                if (n is SubNode)
                {
                    if (refresh) //refresh all the node status
                    {
                        n.status = NodeStatus.ST_IDLE;
                        n["board_id"] = null;
                        Thread.Sleep(100);
                    }

                    if (n["board_id"] != null)
                    {
                        if (curr_sel == -1)
                            curr_sel = n.node_id;

                        if (curr_sel == n.node_id || all_sel)
                        {
                            if ((n.errlist != "") || (n.status == NodeStatus.ST_LOST))
                                btn.Template = this.FindResource("roundButtonselerr2") as ControlTemplate;
                            else
                                btn.Template = this.FindResource("roundButtonsel2") as ControlTemplate;
                        }
                        else
                        {
                            if ((n.errlist != "") || (n.status == NodeStatus.ST_LOST))
                                btn.Template = this.FindResource("roundButtonerr2") as ControlTemplate;
                            else
                                btn.Template = this.FindResource("roundButton2") as ControlTemplate;
                        }
                        btn.ApplyTemplate();
                        btn.Visibility = Visibility.Visible;
                        continue;
                    }
                }
                btn.Visibility = Visibility.Hidden;
            }

        }
        private Button IdToButton(string id)
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
        
        static EngConfigWnd()
        {
        }
        public static string GetRegistData(string name)
        {
            string registData;
            RegistryKey hkml = Registry.LocalMachine;
            RegistryKey software = hkml.OpenSubKey("SOFTWARE", true);
            RegistryKey aimdir = software.OpenSubKey("Microsoft", true);
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
            RegistryKey hklm = Registry.LocalMachine;
            RegistryKey software = hklm.OpenSubKey("SOFTWARE", true);
            RegistryKey aimdir = software.CreateSubKey("Microsoft");
            aimdir.SetValue(name, tovalue);
        }
        
        private static void DeleteRegist(string name)
        {
            string[] aimnames;
            RegistryKey hkml = Registry.LocalMachine;
            RegistryKey software = hkml.OpenSubKey("SOFTWARE", true);
            RegistryKey aimdir = software.OpenSubKey("Microsoft", true);
            aimnames = aimdir.GetSubKeyNames();
            foreach (string aimKey in aimnames)
            {
                if (aimKey == name)
                    aimdir.DeleteSubKeyTree(name);
            }
        } 
        public void KbdData(string param, string data)
        {
            App p = Application.Current as App;
            XElement cfg = p.curr_cfg;

            if (param == "entersys")
            {
                if (Password.compare_pwd("admin",data))
                {
                    Process app = new Process();
                    app.StartInfo.FileName = "Explorer.exe";
                    app.StartInfo.Arguments = "";
                    app.Start();
                    Thread.Sleep(2000);
                    Type shellType = Type.GetTypeFromProgID("Shell.Application");
                    object shellObject = System.Activator.CreateInstance(shellType);
                    shellType.InvokeMember("ToggleDesktop", System.Reflection.BindingFlags.InvokeMethod, null, shellObject, null);

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
                    (Application.Current as App).runwnd.Disable(Visibility.Visible);
                    (Application.Current as App).singlewnd.Disable(Visibility.Visible);
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
                p.SaveAppConfig();
                b_lockon = true;
                UpdateDisplay(false);
            }
            if (param.IndexOf("reg_") == 0)
            {
                Button btn;
                SubNode n = null;
                if (curr_sel == -1)
                    return;
                Byte i = Convert.ToByte(curr_sel);

                if (i < 17)
                {
                    btn = IdToButton(i.ToString());
                    n = p.agent[i];

                }
                else
                {
                    btn = IdToButton("36");
                    n = p.agent.missingnode;
                }
                if (n["addr"] != null)
                {
                    param = param.Remove(0, 4); 
                    n[param] = UInt32.Parse(data);
                    Thread.Sleep(1000);
                    
                    
                    btn_refreshreg_Click(null, null);
                }
            }
            if (param.IndexOf("chgnd_") == 0)
            {
                Byte oldaddr = Byte.Parse(param.Remove(0, 6));
                Byte newaddr = Byte.Parse(data);

                Button btn = IdToButton(newaddr.ToString());
                if (btn != null && newaddr < 36) //valid new address
                {
                    if (btn.Visibility != Visibility.Visible)
                    {
                        SubNode oldn;
                        SubNode newn;
                        if (oldaddr == 36)
                            oldn = p.agent.missingnode;
                        else
                            oldn= p.agent[oldaddr];
                        newn = p.agent[newaddr];
                        newaddr += 0xC0;
                        newn.status = NodeStatus.ST_IDLE;
                        oldn["test_mode_reg2"] = newaddr;
                        
                        Thread.Sleep(500);
                        newn.status = NodeStatus.ST_IDLE;
                        newn["addr"] = null;
                        oldn.status = NodeStatus.ST_LOST;
                        Thread.Sleep(100);
                        if (newn["addr"].HasValue)
                        {
                             MessageBox.Show(StringResource.str("change_addr_success"));
                             return;

                        }
                    }
                }
               
                MessageBox.Show(StringResource.str("change_addr_fail"));
            }
            if (param != "cali0" && (param.IndexOf("cali")==0)) //cali1 to cali10
            {

                string msg = StringResource.str("put_poise") + "(" + data + StringResource.str("gram") + ")";

                if (MessageBox.Show(msg, "", MessageBoxButton.OKCancel) == MessageBoxResult.Cancel)
                    return;
                NodeAgent n = p.agent;
                int i = RunMode.StringToId(param) - 1;
                if (curr_sel >= 0)
                {
                    n.ClearNodeReg((byte)curr_sel, "cs_mtrl");
                    string cs_mtrl_val = n.GetNodeReg((byte)curr_sel, "cs_mtrl");
                    UInt32 val;
                    if (UInt32.TryParse(cs_mtrl_val,out val))
                    {
                        if (val <= WeighNode.MAX_VALID_WEIGHT)
                        {
                            if(i < 5)
                                n.SetNodeReg((byte)curr_sel, "poise_weight_gram" + i.ToString(), UInt32.Parse(cs_mtrl_val));
                            else
                                n.SetNodeReg((byte)curr_sel, "cs_poise" + (i-5).ToString(), UInt32.Parse(cs_mtrl_val));
                            n.Action((byte)curr_sel, "flash");
                            MessageBox.Show(StringResource.str("calidone"));
                            label3_MouseLeftButtonUp(null, null);
                            return;
                        }
                    }
                    MessageBox.Show(StringResource.str("tryagain"));
                    
                }
                return;
            }
        }

        private void calibrate(string calreg)
        {
            App p = Application.Current as App;
            Byte i = Convert.ToByte(curr_sel);
            if (curr_sel == -1)
                return;
            if (calreg == "cali0")
            {
                NodeAgent n = p.agent;
                if (curr_sel >= 0)
                {
                    if (MessageBox.Show(StringResource.str("put_empty"), "", MessageBoxButton.OKCancel) == MessageBoxResult.Cancel)
                        return;

                    n.ClearNodeReg(i, "cs_mtrl");
                    string cs_mtrl_val = n.GetNodeReg(i, "cs_mtrl");
                    UInt32 val = UInt32.Parse(cs_mtrl_val);
                    if (val <= WeighNode.MAX_VALID_WEIGHT)
                    {
                        n.SetNodeReg(i, "cs_zero", UInt32.Parse(cs_mtrl_val));
                        n.Action(i, "flash");
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
                NodeAgent n = p.agent;
                if (curr_sel >= 0)
                {
                    n.Action(i, "empty");
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
            //p.WeightAction(i, "query"); //update the readings
        }
        private void btn_cali_Click(object sender, RoutedEventArgs e)
        {
            string calreg = (sender as Button).Name.Remove(0, 4); //remove "btn_" string

                calibrate(calreg);


        }

        private void btn_entersys_Click(object sender, RoutedEventArgs e)
        {
            (Application.Current as App).kbdwnd.Init(StringResource.str("enter_system"), "entersys", true, KbdData);
        }

        private void btn_locksys_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            XElement cfg = p.curr_cfg;
            if (b_lockon)
            {
                (Application.Current as App).kbdwnd.Init(StringResource.str("enter_unlocknum"), "quitlock", false, KbdData);

            }
            else
            {
                (Application.Current as App).kbdwnd.Init(StringResource.str("enter_locknum"), "enterlock", false, KbdData);
            }
        }

        private void btn_language_Click(object sender, RoutedEventArgs e)
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

        private void nd_1_Click(object sender, RoutedEventArgs e)
        {
            if (sender is Button)
            {
                Button btn = (sender as Button);
                if (btn.Name.IndexOf("nd_") == 0)
                {
                    all_sel = false;
                    curr_sel = int.Parse(btn.Name.Remove(0, 3));
                    btn_refreshreg_Click(null, null);
                }
            }
            UpdateDisplay(false);
        }

        private void btn_changeaddr_Click(object sender, RoutedEventArgs e)
        {
            (Application.Current as App).kbdwnd.Init(StringResource.str("enter_newaddr"), "chgnd_"+curr_sel.ToString(), false, KbdData);
        }

        private void btn_updatefw_Click(object sender, RoutedEventArgs e)
        {
            SubNode n;
            if (curr_sel == -1)
                return;
            int lastcurr = curr_sel;
            App p = Application.Current as App;
            Button btn;
            for (Byte i = 1; i < 18; i++)
            {
                n = null;
                if (i < 17)
                {
                    btn = IdToButton(i.ToString());
                    n = p.agent[i];

                }
                else
                {
                    btn = IdToButton("36");
                    n = p.agent.missingnode;
                }

                if (n is SubNode)
                {
                     n.status = NodeStatus.ST_IDLE;
                     n["board_id"] = null;
                     Thread.Sleep(100);

                    if (n["board_id"] != null)
                    {
                        if ((curr_sel == n.node_id) || (all_sel == true))
                        {
                            curr_sel = n.node_id;
                            btn.Template = this.FindResource("roundButtonsel2") as ControlTemplate;
                            btn.ApplyTemplate();
                            btn.Visibility = Visibility.Visible;

                            fwprgs_bar.Visibility = Visibility.Visible;
                            fwprgs_bar.Minimum = 0;

                            fwprgs_bar.Maximum = (int)(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("ioex_cs.Resources.MAIN.BIN").Length/128);
                            
                            bootloader bl = new bootloader(n);
                            string ret = bl.download(fwprogress);
                            btn_updatefw.Content = StringResource.str("updatefw");
                            fwprgs_bar.Visibility = Visibility.Hidden;
                            if (ret != "")
                                MessageBox.Show(StringResource.str("updatefwfail") +n.node_id.ToString() + "\r\n" + ret);
                            curr_sel = lastcurr;
                            continue;
                        }
                    }
                }
             }
             btn_refreshreg_Click(null, null);
             UpdateDisplay(true);
        }
        public void fwprogress(uint percent)
        {
            Dispatcher.Invoke(System.Windows.Threading.DispatcherPriority.Background, (Action)delegate
            {
                    btn_updatefw.Content = curr_sel.ToString() + ":" + percent.ToString() + "Blocks";
                    fwprgs_bar.Value = percent;
            });
        }
        private void btn_selectall_Click(object sender, RoutedEventArgs e)
        {
            all_sel = !all_sel;

            UpdateDisplay(false);
        }

        private void btn_refreshaddr_Click(object sender, RoutedEventArgs e)
        {
            UpdateDisplay(true);
        }

        private void btn_refreshreg_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
                Button btn;
                SubNode n = null;
                
                if (curr_sel == -1)
                    return;
                Byte i = Convert.ToByte(curr_sel);
                if (i < 17)
                {
                    btn = IdToButton(i.ToString());
                    n = p.agent[i];

                }
                else
                {
                    btn = IdToButton("36");
                    n = p.agent.missingnode;
                }
                if (n["addr"] != null)
                {
                    listBox1.Items.Clear();
                    foreach (string rtkey in SubNode.reg_type_tbl.Keys)
                    {
//                      if (!n[rtkey].HasValue)
                        {
                            n.status = NodeStatus.ST_IDLE;
                            n[rtkey] = null;
                            Thread.Sleep(30);
                        }
                        
                        RegType rt = SubNode.reg_type_tbl[rtkey];
                        if(n[rtkey].HasValue)
                            listBox1.Items.Add(new PerReg { name=rtkey, address = rt.pos.ToString(), value = n[rtkey].Value.ToString(), vhex = n[rtkey].Value.ToString("X")});
                        else
                            listBox1.Items.Add(new PerReg { name = rtkey, address = rt.pos.ToString(), value = "Null" , vhex = "Null"});
                    }
                    return;
                }
        }

        private void TextBlock_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (listBox1.SelectedItem is PerReg)
            {
                (Application.Current as App).kbdwnd.Init(StringResource.str("enter_newvalue"), "reg_"+(listBox1.SelectedItem as PerReg).name, false, KbdData);
            }
        }

        private void btn_ret_run_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            p.agent.Resume();
            foreach (UIPacker pk in p.packers)
            {
                pk.nc.Resume();
            }
            this.Hide();
            p.SwitchTo("runmode");
        }

        private void btn_modify_Click(object sender, RoutedEventArgs e)
        {
            if (listBox1.SelectedItem is PerReg)
            {
                (Application.Current as App).kbdwnd.Init(StringResource.str("enter_newvalue"), "reg_" + (listBox1.SelectedItem as PerReg).name, false, KbdData);
            }
        }

        private void btn_ret_config_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            p.agent.Resume();
            foreach (UIPacker pk in p.packers)
                pk.nc.Resume();
            this.Hide();
            p.SwitchTo("configmenu");
        }

        private void label3_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            App p = Application.Current as App;
            if (curr_sel == -1)
                return;
            Byte i = Convert.ToByte(curr_sel);

            p.agent.Action(i,"query");
            label3.Content = p.agent.weight(i).ToString("F2");
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
