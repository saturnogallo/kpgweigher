using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml.Linq;

namespace TSioex
{
    public partial class AlertWnd : Form
    {
        public static bool b_turnon_alert = false;
        public static bool b_show_alert = false;
        public static bool b_stop_onalert = false;
        public static bool b_manual_reset = false;
        public AlertWnd()
        {
            InitializeComponent();
            this.BackColor = SingleModeWnd.bgWindow;
            btn_manual_reset.SetStyle(Color.LightGreen, MyButtonType.round2RectButton);
            btn_show_alert.SetStyle(Color.LightGreen, MyButtonType.round2RectButton);
            btn_stop_onalert.SetStyle(Color.LightGreen, MyButtonType.round2RectButton);
            btn_turnon_alert.SetStyle(Color.LightGreen, MyButtonType.round2RectButton);
            btn_ret_config.Click += new EventHandler(btn_ret_config_Click);
            btn_ret_run.Click += new EventHandler(btn_ret_run_Click);
            btn_show_alert.ValidClick += new EventHandler(btn_alert_Click);
            btn_stop_onalert.ValidClick += new EventHandler(btn_alert_Click);
            btn_turnon_alert.ValidClick += new EventHandler(btn_alert_Click);
            btn_manual_reset.ValidClick += new EventHandler(btn_alert_Click);
            
        }

        void btn_ret_run_Click(object sender, EventArgs e)
        {
            Program.SwitchTo("runmode");
            if (bDirty)
            {
                btn_ret_config.Visible = false;
                btn_ret_run.Visible = false;
                Application.DoEvents();
                this.Invoke(new Action(Program.SaveAppConfig));
                bDirty = false;
                btn_ret_run.Visible = true;
                btn_ret_config.Visible = true;
            }
            //this.Hide();
        }

        void btn_ret_config_Click(object sender, EventArgs e)
        {
            Program.SwitchTo("configmenu");
            if (bDirty)
            {
                btn_ret_config.Visible = false;
                btn_ret_run.Visible = false;
                Application.DoEvents();
                this.Invoke(new Action(Program.SaveAppConfig));
                bDirty = false;
                btn_ret_config.Visible = true;
                btn_ret_run.Visible = true;
            }
            //this.Hide();
        }
        public void UpdateUI()
        {
            XElement cfg = Program.curr_cfg;
            b_turnon_alert = (cfg.Element("turnon_alert").Value == "ON");
            b_show_alert = (cfg.Element("show_alert").Value == "ON");
            b_stop_onalert = (cfg.Element("stop_onalert").Value == "ON");
            b_manual_reset = (cfg.Element("manual_reset").Value == "ON");

            lbl_title.Text = StringResource.str("alertpage");
            btn_manual_reset.Text = StringResource.str("manual_reset");
            btn_show_alert.Text = StringResource.str("show_alert");
            btn_stop_onalert.Text = StringResource.str("stop_onalert");
            btn_turnon_alert.Text = StringResource.str("turnon_alert");
            
            btn_turnon_alert.bEnabled = b_turnon_alert;
            btn_show_alert.bEnabled = b_show_alert;
            btn_stop_onalert.bEnabled = b_stop_onalert;
            btn_manual_reset.bEnabled = b_manual_reset;
            if(b_manual_reset)
                btn_manual_reset.SetStyle(Color.LightGreen, MyButtonType.round2RectButton);
            else
                btn_manual_reset.SetStyle(Color.Gray, MyButtonType.round2RectButton);
            if (b_turnon_alert)
                btn_turnon_alert.SetStyle(Color.LightGreen, MyButtonType.round2RectButton);
            else
                btn_turnon_alert.SetStyle(Color.Gray, MyButtonType.round2RectButton);
            if (b_show_alert)
                btn_show_alert.SetStyle(Color.LightGreen, MyButtonType.round2RectButton);
            else
                btn_show_alert.SetStyle(Color.Gray, MyButtonType.round2RectButton);

            if (b_stop_onalert)
                btn_stop_onalert.SetStyle(Color.LightGreen, MyButtonType.round2RectButton);
            else
                btn_stop_onalert.SetStyle(Color.Gray, MyButtonType.round2RectButton);
            this.Invalidate();
        }
        private bool bDirty = false;
        private void btn_alert_Click(object sender, EventArgs e)
        {
            RectButton btn = sender as RectButton;
                XElement cfg = Program.curr_cfg;
                if (btn.Name == "btn_turnon_alert")
                {
                    if (b_turnon_alert)
                        cfg.SetElementValue("turnon_alert", "OFF");
                    else
                        cfg.SetElementValue("turnon_alert", "ON");
                    UpdateUI();
                    bDirty = true;
                }
                if (btn.Name == "btn_stop_onalert")
                {
                    if (b_stop_onalert)
                        cfg.SetElementValue("stop_onalert", "OFF");
                    else
                        cfg.SetElementValue("stop_onalert", "ON");
                    UpdateUI();
                    bDirty = true;
                }
                if (btn.Name == "btn_show_alert")
                {
                    if (b_show_alert)
                        cfg.SetElementValue("show_alert", "OFF");
                    else
                        cfg.SetElementValue("show_alert", "ON");
                    UpdateUI();
                    bDirty = true;
                    
                }
                if (btn.Name == "btn_manual_reset")
                {
                    if (b_manual_reset)
                        cfg.SetElementValue("manual_reset", "OFF");
                    else
                        cfg.SetElementValue("manual_reset", "ON");
                    UpdateUI();
                    bDirty = true;
                    
                }
            
        }
    }
}