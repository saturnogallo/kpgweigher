using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Xml.Linq;

namespace ioex_cs
{
    /// <summary>
    /// Interaction logic for AlertWnd.xaml
    /// </summary>
    public partial class AlertWnd : Window
    {
        public static bool b_turnon_alert = false;
        public static bool b_show_alert = false;
        public static bool b_stop_onalert = false;
        public static bool b_manual_reset = false;
        public AlertWnd()
        {
            InitializeComponent();
        }

        private void btn_ret_config_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            p.SwitchTo("configmenu");
            this.Hide();
        }

        private void btn_ret_run_Click(object sender, RoutedEventArgs e)
        {
            App p = Application.Current as App;
            p.SwitchTo("runmode");
            this.Hide();
        }
        public void UpdateUI()
        {
            App p = Application.Current as App;
            XElement cfg = p.curr_cfg;
            b_turnon_alert = (cfg.Element("turnon_alert").Value == "ON");
            b_show_alert = (cfg.Element("show_alert").Value == "ON");
            b_stop_onalert = (cfg.Element("stop_onalert").Value == "ON");
            b_manual_reset = (cfg.Element("manual_reset").Value == "ON");

            if(b_turnon_alert)
                btn_turnon_alert.Style = this.FindResource("ButtonStyleOn") as Style;
            else
                btn_turnon_alert.Style = this.FindResource("ButtonStyleOff") as Style;

            if (b_show_alert)
                btn_show_alert.Style = this.FindResource("ButtonStyleOn") as Style;
            else
                btn_show_alert.Style = this.FindResource("ButtonStyleOff") as Style;

            if (b_stop_onalert)
                btn_stop_onalert.Style = this.FindResource("ButtonStyleOn") as Style;
            else
                btn_stop_onalert.Style = this.FindResource("ButtonStyleOff") as Style;

            if (b_manual_reset)
                btn_manual_reset.Style = this.FindResource("ButtonStyleOn") as Style;
            else
                btn_manual_reset.Style = this.FindResource("ButtonStyleOff") as Style;
            
        }
        private void btn_alert_Click(object sender, RoutedEventArgs e)
        {
            if (sender is Button)
            {
                Button btn = sender as Button;
                App p = Application.Current as App;
                XElement cfg = p.curr_cfg;
                if (btn.Name == "btn_turnon_alert")
                {
                    if(b_turnon_alert)
                        cfg.SetElementValue("turnon_alert", "OFF");
                    else
                        cfg.SetElementValue("turnon_alert", "ON");
                    UpdateUI();
                    p.SaveAppConfig();
                }
                if (btn.Name == "btn_stop_onalert")
                {
                    if (b_stop_onalert)
                        cfg.SetElementValue("stop_onalert", "OFF");
                    else
                        cfg.SetElementValue("stop_onalert", "ON");
                    UpdateUI();
                    p.SaveAppConfig();
                }
                if (btn.Name == "btn_show_alert")
                {
                    if (b_show_alert)
                        cfg.SetElementValue("show_alert", "OFF");
                    else
                        cfg.SetElementValue("show_alert", "ON");
                    UpdateUI();
                    p.SaveAppConfig();
                }
                if (btn.Name == "btn_manual_reset")
                {
                    if (b_manual_reset)
                        cfg.SetElementValue("manual_reset", "OFF");
                    else
                        cfg.SetElementValue("manual_reset", "ON");
                    UpdateUI();
                    p.SaveAppConfig();
                }
            }
        }
    }
}
