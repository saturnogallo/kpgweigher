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
        
        static private Queue<onepack> packhist = new Queue<onepack>();
        static private double total_weights = 0;
        static private UInt32 total_packs = 0;
        static private UInt32 speed = 0;
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
            btnRet.Text = StringResource.str("return");
            lb_oper.SelectedIndexChanged += new EventHandler(UpdateDataGrid);
            lb_prod.SelectedIndexChanged += new EventHandler(UpdateDataGrid);
            lb_prodno.SelectedIndexChanged += new EventHandler(UpdateDataGrid);
            mc_starttime.DateChanged += new DateRangeEventHandler(UpdateDataGrid);
            mc_endtime.DateChanged += new DateRangeEventHandler(UpdateDataGrid);
            lbl_starttime.Text = StringResource.str("start_date");
            lbl_endtime.Text = StringResource.str("end_date");
            lbl_oper.Text = StringResource.str("operator");
            lbl_prodno.Text = StringResource.str("product_no");
            lbl_prod.Text = StringResource.str("product_desc");
            
            DistinctValue("operator",lb_oper);
            DistinctValue("product_desc", lb_prod);
            DistinctValue("product_no", lb_prodno);
        }
        static private void ExecuteQuery(string txtQuery)
        {
            SetConnection();
            sql_con.Open();

            sql_cmd = sql_con.CreateCommand();
            sql_cmd.CommandText = txtQuery;

            sql_cmd.ExecuteNonQuery();
            sql_con.Close();
            
        }
        private void DistinctValue(string field,ListBox lb)
        {
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
            string cols = "select start_date, end_date, operator, product_no, product_desc, weight, pack_num from  mains ";
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
                StringResource.str("totalpacknum"),total_pack.ToString(),StringResource.str("avgweight"),(total_sum/total_pack).ToString("F1"),StringResource.str("gram"));
            sql_con.Close();
        }
        /// <summary>
        /// reset the status of history record
        /// </summary>
        static private void ResetHistory()
        {
            total_packs = 0;
            total_weights = 0;
            speed = 0;
            packhist.Clear();
        }
        private static DateTime rStart;
        static public void InitNewRun(string oper,string prod_no,string prod_desc)
        {
            string txtUpdate = "insert into mains (start_date,end_date,operator,product_no,product_desc,weight,pack_num) values ('";
            rStart = DateTime.Now;
            txtUpdate = txtUpdate + rStart.ToString("yyyy-MM-dd HH:mm:ss") + "','" + rStart.ToString("yyyy-MM-dd HH:mm:ss") + "','" + oper + "','" + prod_no + "','" + prod_desc + "',0,0)";
            ExecuteQuery(txtUpdate);
        }
        static public void EndNewRun()
        {
            UpdateRecord();
            ResetHistory();
        }
        static public void UpdateRecord()
        {
            string txtUpdate = "update mains set end_date=\"" + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss") + "\", weight=\"" + total_weights.ToString() + "\", pack_num=\"" + total_packs.ToString() + "\" where start_date=\"" + rStart.ToString("yyyy-MM-dd HH:mm:ss") + "\"";
            ExecuteQuery(txtUpdate);
        }
        static public void AddNewPack(onepack o,bool IsSimulate)
        {
            total_packs++;
            total_weights += o.weight;
            
            packhist.Enqueue(o);
            if (packhist.Count > 500)
            {
                packhist.Dequeue();
            }
            //update the speed
            long count = 0;
            DateTime lastmin = DateTime.Now;
            lastmin.Subtract(new TimeSpan(0,1,1));
            foreach (onepack op in packhist)
            {
                if (op.time < lastmin)
                {
                    count = count + 1;
                }else{
                    count = packhist.Count - count;
                    break;
                }
            }
            speed = (UInt32)count;
            if (IsSimulate)
            {
                return;
            }
            if (total_packs % 100 == 1)
                UpdateRecord();
        }

        private void btnRet_Click(object sender, EventArgs e)
        {
            Hide();
        }
    }
}
