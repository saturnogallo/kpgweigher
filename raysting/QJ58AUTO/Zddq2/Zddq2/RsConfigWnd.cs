using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Raysting.Common;
namespace Zddq2
{
    public partial class RsConfigWnd : Form
    {
        private DataTable rs_dt;

        private int selectedRs = 0;
        public RsConfigWnd()
        {
            InitializeComponent();
            //btn_RsConfig.SetStyle(Color.LightBlue, MyButtonType.round2RectButton);
            btn_RsConfig.Text = StringResource.str("rsconfig");
            //btn_RsConfig.ValidClick += new EventHandler(btn_RsConfig_ValidClick);

            //btn_RxConfig.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_RxConfig.Text = StringResource.str("rxconfig");
            btn_RxConfig.ValidClick += new EventHandler(btn_RxConfig_ValidClick);
            /*
            btn_quit.SetStyle(Color.Beige, MyButtonType.round2Button);
            btn_chan.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_next.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_last.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_serial.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_rvalue.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_ralpha.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_rbeta.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            btn_temp.SetStyle(Color.Beige, MyButtonType.roundRectButton);
            */
            btn_quit.Text = StringResource.str("quit");
            lbl_serial.Text = StringResource.str("serial");
            lbl_rvalue.Text = StringResource.str("rvalue");
            lbl_alpha.Text = StringResource.str("ralpha");
            lbl_beta.Text = StringResource.str("rbeta");
            lbl_temp.Text = StringResource.str("temp");
            btn_last.Text = "<";
            btn_next.Text = ">";

            btn_quit.ValidClick += new EventHandler(btn_quit_ValidClick);
            btn_chan.ValidClick += new EventHandler(btn_dummy);
            btn_serial.ValidClick += new EventHandler(input_GotFocus);
            btn_rvalue.ValidClick += new EventHandler(input_GotFocus);
            btn_ralpha.ValidClick += new EventHandler(input_GotFocus);
            btn_rbeta.ValidClick += new EventHandler(input_GotFocus);
            btn_temp.ValidClick += new EventHandler(input_GotFocus);
            btn_last.ValidClick += new EventHandler(btn_last_ValidClick);
            btn_next.ValidClick += new EventHandler(btn_next_ValidClick);

//            btn_SysConfig.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_SysConfig.Text = StringResource.str("sysconfig");
            btn_SysConfig.ValidClick += new EventHandler(btn_SysConfig_ValidClick);

            //no channel select for Rs channel
            selectedRs = 0;
            btn_last.Visible = false;
            btn_next.Visible = false;
            /*
            rs_dt = new DataTable("RsInfo");
            rs_dt.Columns.Add(StringResource.str("serial"));
            rs_dt.Columns.Add(StringResource.str("rvalue"));
            rs_dt.Columns.Add(StringResource.str("ralpha"));
            rs_dt.Columns.Add(StringResource.str("rbeta"));

            dataGridView1.DataSource = rs_dt;
            dataGridView1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridView1.CellDoubleClick += new DataGridViewCellEventHandler(dataGridView1_CellDoubleClick);
             * 
             *         void dataGridView1_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            MessageBox.Show(e.RowIndex.ToString() + "," + e.ColumnIndex.ToString());
        }


             *
             *             rs_dt.Rows.Clear();
            DataRow dr;
            foreach (RsInfo rs in Program.lst_rsinfo)
            {
                dr = rs_dt.NewRow();
                dr[0] = rs.sSerial;
                dr[1] = rs.dValue.ToString();
                dr[2] = rs.dAlpha.ToString();
                dr[3] = rs.dBeta.ToString();
                rs_dt.Rows.Add(dr);
            }
             */
        }
        void btn_dummy(object sender, EventArgs e)
        {
        }
        void btn_next_ValidClick(object sender, EventArgs e)
        {
            selectedRs += 1;
            if (selectedRs >= Program.lst_rsinfo.Count)
                selectedRs = 0;

            InitDisplay(selectedRs);
        }

        void btn_last_ValidClick(object sender, EventArgs e)
        {
            if (selectedRs <= 0)
                selectedRs = Program.lst_rsinfo.Count;
            selectedRs -= 1;
            InitDisplay(selectedRs);
        }


        void input_GotFocus(object sender, EventArgs e)
        {
            string regname = (sender as Control).Name.Replace("btn_", ""); //remove btn_
            Program.kbd.Init(StringResource.str("enter_" + regname), regname, false, KbdData);
        }

        public void KbdData(string param, string data)
        {
            try
            {
                if (param == "chan")
                {
                    selectedRs = Convert.ToInt32(data) - 1;
                }
                if (param == "serial")
                {
                    Program.lst_rsinfo[selectedRs].sSerial = data;
                }
                if (param == "rvalue")
                {
                    Program.lst_rsinfo[selectedRs].dValue = Convert.ToDouble(data) ;
                }
                if (param == "ralpha")
                {
                    Program.lst_rsinfo[selectedRs].dAlpha = Convert.ToDouble(data);
                }
                if (param == "rbeta")
                {
                    Program.lst_rsinfo[selectedRs].dBeta = Convert.ToDouble(data);
                }
                if (param == "temp")
                {
                    RunWnd.syscfg.dTemp = Convert.ToDouble(data);
                }

                InitDisplay(selectedRs);
            }
            catch
            {
            }
        }


        void btn_SysConfig_ValidClick(object sender, EventArgs e)
        {
            Program.SwitchWindow("sysconfig");   
        }

        void btn_quit_ValidClick(object sender, EventArgs e)
        {
            Program.SwitchWindow("mainwnd");
            Program.mainwnd.Invoke(new Action<bool>(Program.mainwnd.ReDraw), new object[] { false });
        }

        void btn_RxConfig_ValidClick(object sender, EventArgs e)
        {
            Program.SwitchWindow("rxconfig");
        }

        void btn_RsConfig_ValidClick(object sender, EventArgs e)
        {
            //same page no action
        }
        public void InitDisplay(int iRs)
        {
            selectedRs = iRs;
            RsInfo rs = Program.lst_rsinfo[iRs];
            btn_chan.Text = "CH " + (iRs + 1).ToString();
            btn_rvalue.Text = Util.FormatData(rs.dValue,7); 
            btn_ralpha.Text = rs.dAlpha.ToString("F2");
            btn_rbeta.Text = rs.dBeta.ToString("F2");
            btn_temp.Text = RunWnd.syscfg.dTemp.ToString("F3");
            btn_serial.Text = rs.sSerial;
            
        }
    }
}
