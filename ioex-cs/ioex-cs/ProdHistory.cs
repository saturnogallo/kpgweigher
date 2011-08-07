using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Finisar.SQLite;

namespace ioex_cs
{
    public struct onepack 
    {
        public double weight;//total weight
        public byte[] bucket; //address of combination
        public DateTime time;//time of current pack
    }
    public partial class ProdHistory : Form
    {
        static private SQLiteConnection sql_con;
        static private SQLiteCommand sql_cmd;
        
        private SQLiteDataAdapter DB;
        private DataSet DS = new DataSet();
        private DataTable DT = new DataTable();

        static ProdHistory()
        {
            
        }

        static private void SetConnection()
        {
            sql_con = new SQLiteConnection("Data Source=History.db;Version=3;New=False;Compress=True;");
        }
        public ProdHistory()
        {
            InitializeComponent();
            this.BackColor = Color.FromArgb(0xFF, 0xEE, 0xF2, 0xFB);
            
            lb_oper.SelectedIndexChanged += new EventHandler(UpdateDataGrid);
            lb_prod.SelectedIndexChanged += new EventHandler(UpdateDataGrid);
            lb_prodno.SelectedIndexChanged += new EventHandler(UpdateDataGrid);
            mc_starttime.DateChanged += new DateRangeEventHandler(UpdateDataGrid);
            mc_endtime.DateChanged += new DateRangeEventHandler(UpdateDataGrid);
            UpdateDisplay();
            UpdateList();
        }
        public void UpdateDisplay()
        {
            btnClr.Text = StringResource.str("clearpack");
            lbl_starttime.Text = StringResource.str("start_date");
            lbl_endtime.Text = StringResource.str("end_date");
            lbl_oper.Text = StringResource.str("operator");
            lbl_prodno.Text = StringResource.str("product_no");
            lbl_prod.Text = StringResource.str("product_desc");
        }
        public void UpdateList()
        {
            DistinctValue("operator", lb_oper);
            DistinctValue("product_desc", lb_prod);
            DistinctValue("product_no", lb_prodno);

        }
        static private void ExecuteQuery(string txtQuery)
        {
            lock(sql_con)
            {
                SetConnection();
                sql_con.Open();

                sql_cmd = sql_con.CreateCommand();
                sql_cmd.CommandText = txtQuery;

                sql_cmd.ExecuteNonQuery();
                sql_con.Close();

            }
            
        }
        private void DistinctValue(string field,ListBox lb)
        {
            lb.Items.Clear();
            lb.Items.Add("*");
            SetConnection();
            sql_con.Open();
            sql_cmd = sql_con.CreateCommand();
            string CommandText = "select distinct " + field + " from mains";
            DB = new SQLiteDataAdapter(CommandText, sql_con);
            DS.Reset();
            DB.Fill(DS);
            DT = DS.Tables[0];

            foreach (DataRow dr in DT.Rows)
            {
                lb.Items.Add(dr[0].ToString());
            }
            sql_con.Close();

        }
        public void UpdateDataGrid(object sender, EventArgs e)
        {
            SetConnection();
            sql_con.Open();

            sql_cmd = sql_con.CreateCommand();
            string cols = "select start_date, end_date, operator, product_no, product_desc, target, upper_var, lower_var, weight, pack_num from  mains ";
            DateTime s_dt = mc_starttime.SelectionStart;
            DateTime e_dt = mc_endtime.SelectionEnd;
            string CommandText = cols + String.Format("where start_date>='{0}-{1}-{2} 00:00:00' and end_date<='{3}-{4}-{5} 23:59:59'",
                                                        s_dt.Year,s_dt.Month.ToString("D2"),s_dt.Day.ToString("D2"),e_dt.Year,e_dt.Month.ToString("D2"),e_dt.Day.ToString("D2"));
            if (lb_oper.SelectedIndex >=0 && lb_oper.SelectedItem.ToString() != "*")
            {
                CommandText += String.Format(" and operator='{0}'", lb_oper.SelectedItem.ToString());
            }
            if (lb_prod.SelectedIndex >= 0 && lb_prod.SelectedItem.ToString() != "*")
            {
                CommandText += String.Format(" and product_desc='{0}'", lb_prod.SelectedItem.ToString());
            }
            if (lb_prodno.SelectedIndex >= 0 && lb_prodno.SelectedItem.ToString() != "*")
            {
                CommandText += String.Format(" and product_no='{0}'", lb_prodno.SelectedItem.ToString());
            }

            DB = new SQLiteDataAdapter(CommandText, sql_con);
            DS.Reset();
            DB.Fill(DS);
            DT = DS.Tables[0];
            double total_sum = 0;
            UInt32 total_pack = 0;
            foreach (DataRow dr in DT.Rows)
            {
                total_pack += UInt32.Parse(dr["pack_num"].ToString());
                total_sum += Double.Parse(dr["weight"].ToString());
            }
            this.dataGridView1.DataSource = DT;
            foreach (DataGridViewColumn dcol in dataGridView1.Columns)
            {
                dcol.HeaderText = StringResource.str(dcol.Name);
                dcol.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
                
            }
            this.lbl_summary.Text = String.Format("{0}:{1}{6} , {2}:{3} , {4}:{5}{6}",StringResource.str("totalweight"),total_sum.ToString("F1"),
                StringResource.str("totalpacknum"),total_pack.ToString(),StringResource.str("avgweight"),(total_pack==0 ? "0" :(total_sum/total_pack).ToString("F1")),StringResource.str("gram"));
            sql_con.Close();
        }

        static internal void UpdateRecord(UIPacker p)
        {
            string txtUpdate = "update mains set end_date=\"" + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss") + "\", weight=\"" + p.total_weights.ToString("F2") + "\", pack_num=\"" + p.total_packs.ToString() + "\" where start_date=\"" + p.rStart.ToString("yyyy-MM-dd HH:mm:ss") + "\"";
            ExecuteQuery(txtUpdate);
        }
        static internal void InitNewRun(UIPacker p)
        {
            string txtUpdate = "insert into mains (start_date,end_date,operator,product_no,product_desc,target,upper_var,lower_var,weight,pack_num) values ('";

            txtUpdate = txtUpdate + p.rStart.ToString("yyyy-MM-dd HH:mm:ss") + "','" + p.rStart.ToString("yyyy-MM-dd HH:mm:ss") + "','" + (System.Windows.Application.Current as App).oper + "','" + p.curr_cfg.product_no + "(" + p.curr_cfg.product_desc + ")" + "','" + p.curr_cfg.product_desc + "'," + p.curr_cfg.target.ToString() + "," + p.curr_cfg.upper_var.ToString() + "," + p.curr_cfg.lower_var.ToString() +",0,0)";
            ExecuteQuery(txtUpdate);
        }

        private void btnRet_Click(object sender, EventArgs e)
        {
            Hide();
            App p = System.Windows.Application.Current as App;
            p.SwitchTo("configmenu");
        }

        private void btnClr_Click(object sender, EventArgs e)
        {
            
            curr_packer.total_packs = 0;
            MessageBox.Show(StringResource.str("done"));
        }
        private UIPacker curr_packer
        {
            get
            {
                App p = System.Windows.Application.Current as App;
                return p.packers[0];
            }
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            Hide();
            App p = System.Windows.Application.Current as App;
            p.SwitchTo("configmenu");
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            Hide();
            App p = System.Windows.Application.Current as App;
            p.SwitchTo("runmode");
        }
    }
}
