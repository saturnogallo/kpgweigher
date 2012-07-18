using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace TSioex
{
    public partial class BottomWnd : Form
    {
        private Intf vib_intf;
        private UIPacker curr_packer
        {
            get
            {
                return Program.packers[0];
            }
        }
        public BottomWnd()
        {
            InitializeComponent();
            this.BackColor = SingleModeWnd.bgWindow;
            vib_intf = new Intf(0);
            btn_ret.Focus();
            UpdateDisplay();
            btn_ret.Click += new EventHandler(btn_return_Click);
            btn_ret_run.Click += new EventHandler(btn_ret_run_Click);
            intf_ckb_delay.GotFocus += new EventHandler(TextBox_GotFocus);
            intf_pulse_width.GotFocus += new EventHandler(TextBox_GotFocus);
            intf_lb_feed_times.GotFocus += new EventHandler(TextBox_GotFocus);
            intf_input_trigger.SelectedIndexChanged += new EventHandler(intf_input_trigger_GotFocus);
            intf_output_trigger.SelectedIndexChanged += new EventHandler(intf_input_trigger_GotFocus);
            intf_handshake.Click +=new EventHandler(intf_handshake_Click);
            intf_ckb_mem.Click += new EventHandler(intf_handshake_Click);
            btn_run.Click += new EventHandler(btn_run_Click);
            this.panel1.BackColor = SingleModeWnd.bgWindow;
        }

        void intf_input_trigger_GotFocus(object sender, EventArgs e)
        {
            ApplySetting();
        }


        public void UpdateDisplay()
        {
            intf_lb_pulse_width.Text = StringResource.str("intf_lb_pulse_width");
            intf_lb_output_fmt_label.Text = StringResource.str("intf_lb_output_fmt");
            intf_lb_input_fmt_label.Text = StringResource.str("intf_lb_input_fmt");
            intf_lb_feed_times_label.Text = StringResource.str("intf_lb_feed_times");
            label11.Text = StringResource.str("packsetting");
            btn_run.Text = StringResource.str("runonce");
            intf_ckb_mem.Text = StringResource.str("intf_ckb_mem");
            intf_handshake.Text = StringResource.str("intf_handshake");
            intf_ckb_delay_label.Text = StringResource.str("intf_ckb_delay");
            intf_output_trigger.Items.Clear();
            intf_output_trigger.Items.Add(StringResource.str("intftrig1"));
            intf_output_trigger.Items.Add(StringResource.str("intftrig2"));
            intf_output_trigger.Items.Add(StringResource.str("intftrig3"));
            intf_output_trigger.Items.Add(StringResource.str("intftrig4"));
            intf_input_trigger.Items.Clear();
            intf_input_trigger.Items.Add(StringResource.str("intftrig1"));
            intf_input_trigger.Items.Add(StringResource.str("intftrig2"));
            intf_input_trigger.Items.Add(StringResource.str("intftrig3"));
            intf_input_trigger.Items.Add(StringResource.str("intftrig4"));
            UpdateData();
        }
        private void UpdateData()
        {
            vib_intf.buf = Convert.ToUInt16(NodeMaster.GetNodeReg(curr_packer.bot_addr, "target_weight"));
            intf_ckb_mem.Checked = vib_intf.b_Hasmem;
            intf_handshake.Checked = vib_intf.b_Handshake;
            intf_ckb_delay.Text = vib_intf.delay_length.ToString();
            intf_lb_feed_times.Text = (vib_intf.feed_times + 1).ToString();
            intf_input_trigger.SelectedIndex = vib_intf.fmt_input;
            intf_output_trigger.SelectedIndex = vib_intf.fmt_output;
            intf_pulse_width.Text = NodeMaster.GetNodeReg(curr_packer.bot_addr, "cs_filter").ToString();
        }
        private void ApplySetting()
        {
            UInt16 delay = UInt16.Parse(intf_ckb_delay.Text.ToString());
            if (intf_input_trigger.SelectedIndex < 0 || intf_output_trigger.SelectedIndex < 0)
                return;
            vib_intf.b_Hasdelay = (delay > 0);
            vib_intf.b_Hasmem = intf_ckb_mem.Checked;
            vib_intf.b_Handshake = intf_handshake.Checked;
            vib_intf.delay_length = delay;
            vib_intf.feed_times = Convert.ToUInt16(UInt16.Parse(intf_lb_feed_times.Text.ToString()) - 1);
            vib_intf.fmt_input = Convert.ToUInt16(intf_input_trigger.SelectedIndex);
            vib_intf.fmt_output = Convert.ToUInt16(intf_output_trigger.SelectedIndex);
            NodeMaster.SetNodeReg(this.curr_packer.bot_addr, new string[] { "target_weight", "cs_filter" }, new UInt32[] { vib_intf.buf, Convert.ToUInt16(intf_pulse_width.Text) });
            NodeMaster.Action(new byte[]{this.curr_packer.bot_addr}, "intf");

            UpdateData();
        }
        private void btn_return_Click(object sender, EventArgs e)
        {

            ApplySetting();
            curr_packer.SaveCurrentConfig(2);
            Program.SwitchTo("configmenu");

        }
        private void btn_ret_run_Click(object sender, EventArgs e)
        {
            ApplySetting();

            curr_packer.SaveCurrentConfig(2);
            Program.SwitchTo("runmode");

        }

        private void node_reg(string regname)
        {
            btn_ret.Focus();
            Program.kbdwnd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }

        private void TextBox_GotFocus(object sender, EventArgs e)
        {
            if (sender is Control)
            {
                Control t = sender as Control;
                if (t.Name == "intf_lb_feed_times" || t.Name == "intf_ckb_delay" || t.Name == "intf_pulse_width")
                {
                    node_reg(t.Name);
                }
            }
        }
        public void KbdData(string param, string data)
        {
            
            if (param == "intf_ckb_delay")
            {
                intf_ckb_delay.Text = data;
            }
            if (param == "intf_lb_feed_times")
            {
                intf_lb_feed_times.Text = data;
            }
            if (param == "intf_pulse_width")
            {
                intf_pulse_width.Text = data;
            }
            ApplySetting();
        }


        private void btn_run_Click(object sender, EventArgs e)
        {
            NodeMaster.Action(new byte[] { curr_packer.bot_addr }, "trigger");
        }


        private void intf_handshake_Click(object sender, EventArgs e)
        {
            ApplySetting();
        }    
    }
}