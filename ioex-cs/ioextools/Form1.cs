using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Finisar.SQLite;

namespace ioextools
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //clean history
            SQLiteConnection sql_con;
            SQLiteCommand sql_cmd;
            
            {
                sql_con = new SQLiteConnection("Data Source=History.db;Version=3;New=False;Compress=True;");
                sql_con.Open();

                sql_cmd = sql_con.CreateCommand();
                sql_cmd.CommandText = "delete from mains";

                sql_cmd.ExecuteNonQuery();
                sql_con.Close();

                MessageBox.Show("操作成功！");
            }
        }
    }
}
