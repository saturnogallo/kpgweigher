using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using System.IO;
using ZedGraph;
using System.Threading;
namespace QJAppMini
{
    public partial class Form1 : Form
    {

        private QJ55A qj_instr;
        private Scanner scrs_instr;
        private Scanner scrx_instr1;
        private Scanner scrx_instr2;

        private enum RunState
        {
            RUNNING,
            IDLE,
            STOPPING
        }
        
        public Form1()
        {
            InitializeComponent();
            //this.Text = this.Text + "(版本:" + File.GetLastWriteTime(Util.basedir + "\\QJAppMini.exe").ToString("yyyyMMdd") + ")";
            this.Text = this.Text + "(版本:" + "20150614" + ")";
            Util.SysLog("");
            tabControl1.SelectedIndex = 1;
            tabControl1.Selecting +=new TabControlCancelEventHandler(btn_start_Click);
            tabControl1.GotFocus += new EventHandler((o, e) => { dgv_program.Focus(); });

            InitRsTab();

            InitSchemeTab();

            InitDataTab();

            
            backgroundWorker1_Init();

            qj_instr = new QJ55A("QJ55A");
            if (qj_instr.Open())
            {
                qj_instr.Stop();
                Thread.Sleep(2000);
                qj_instr.Reset();
            }
            else
            {
                MessageBox.Show("无法打开GPIB端口,请检查连线");
            }
            qj_instr.DataRecieved += HasNewReading;

            scrs_instr = new Scanner("SCANNER_RS");
            scrx_instr1 = new Scanner("SCANNER_RX1");
            scrx_instr2 = new Scanner("SCANNER_RX2");

            
        }


        #region backgroundwork event handler
        private int meascount; //recieved data couting
        private int MeasTime; //totol measure time for this run
        private int meas_schid;
        private int rs_schid;
        private int rs_chid; //rs channel for this run
        private int rx_chid; //rx channel for this run

        void backgroundWorker1_Init()
        {
            backgroundWorker1.WorkerSupportsCancellation = true;
            backgroundWorker1.WorkerReportsProgress = true;
            backgroundWorker1.DoWork += new DoWorkEventHandler(backgroundWorker1_DoWork);
            backgroundWorker1.ProgressChanged += new ProgressChangedEventHandler(backgroundWorker1_ProgressChanged);
            backgroundWorker1.RunWorkerCompleted += new RunWorkerCompletedEventHandler(backgroundWorker1_RunWorkerCompleted);
        }
        private void UpdateRunButton(RunState brunning)
        {
            TabPage tp = tabControl1.TabPages["tabPage3"];
            if (brunning == RunState.IDLE)
            {
                tp.Text = "开始运行";
                tp.BackColor = Color.LawnGreen;
                tp.ImageKey = "run3.png";
                return;
            }
            if (brunning == RunState.STOPPING)
            {
                tp.Text = "正在停止运行...";
                tp.BackColor = Color.YellowGreen;
            }
            if (brunning == RunState.RUNNING)
            {
                tp.Text = "正在运行";
                tp.BackColor = Color.Yellow;
                tp.ImageKey = "stop3.png";
            }
            tp.Refresh();
        }
        void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                MessageBox.Show(e.Error.Message);
            }
            else if (e.Cancelled)
            {
                // Next, handle the case where the user canceled 
                // the operation.
                // Note that due to a race condition in 
                // the DoWork event handler, the Cancelled
                // flag may not have been set, even though
                // CancelAsync was called.
                // todo
                qj_instr.Stop();
                Thread.Sleep(5000);
                qj_instr.Reset();
                scrs_instr.Reset();
                scrx_instr1.Reset();
                scrx_instr2.Reset();

            }
            else
            {
                // Finally, handle the case where the operation 
                // succeeded.
                // todo
            }
            UpdateRunButton(RunState.IDLE);
            dgv_program.ReadOnly = false;
            cb_scheme.Enabled = true;

        }
        //update current status of the progress
        void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            //throw new NotImplementedException();
        }

        void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;

            if (!qj_instr.Open())
            {
                MessageBox.Show("无法打开GPIB端口,请检查连线");
                return;
            }
            if (!scrs_instr.Open() || !scrx_instr1.Open() || !scrx_instr2.Open())
            {
                MessageBox.Show("无法使用扫描器,请检查连线");
                return;
            }

            qj_instr.Stop();
            Thread.Sleep(2000);
            qj_instr.Reset();
            scrs_instr.Reset();
            scrx_instr1.Reset();
            scrx_instr2.Reset();
            Thread.Sleep(1000);
            DataTable taskdt;
            if (e.Argument is DataTable)
                taskdt = e.Argument as DataTable;
            else
                return;
            for(meas_schid =0;meas_schid < taskdt.Rows.Count;meas_schid++)
            {
                DataRow taskdr = taskdt.Rows[meas_schid];
                try{
                    DataRow[] rsdrs = rs_ds.Tables[taskdr["RsGroup"].ToString()].Select(String.Format("RsID='{0}'", taskdr["RsID"].ToString()));
                    if (rsdrs.Length < 0)
                        continue;
                    runrs_dt.Rows.Add(rsdrs[0].ItemArray);
                    runrs_dt.AcceptChanges();
                    rs_schid = runrs_dt.Rows.Count - 1;

                    MeasTime = (int)taskdr["MeasTime"];
                    if (MeasTime > 0)
                    {
                        meascount = 0;
                        rs_chid = (int)taskdr["RsChannel"];
                        scrs_instr.SwitchTo(rs_chid, false);
                        rx_chid = (int)taskdr["RxChannel"];
                        scrx_instr1.SwitchTo(rx_chid, false);
                        scrx_instr2.SwitchTo(rx_chid, false);
                        Thread.Sleep(200);
                        
                        qj_instr.SendParameter(taskdr,rsdrs[0]);                
                        Thread.Sleep(200);
                        qj_instr.StartRun(MeasTime);
                        while (meascount < MeasTime)
                        {
                            if (worker.CancellationPending)
                            {
                                e.Cancel = true;
                                return;
                            }
                            Thread.Sleep(1000);
                            qj_instr.Refresh();
                        }
                    }
                }catch(Exception ex)
                {
                    Util.SysLog("backgroundWorker1_DoWork" + ex.Message + ex.StackTrace);
                    e.Cancel = true;
                    return;
                }
            }
        }
        #endregion

        #region prg page related variable
        private DataSet ds;  //contain several tables , each table is a scheme
        private DataTable dt;//one scheme table
        private List<string> schemes;

        readonly string[] colname = new string[] { "RsGroup", "RsID", "RsChannel", "RxValue", "RxSerial", "RxCurrent", "RxChannel", "Temp", "MeasTime", "Delay", "SampleTime", "SwitchTime", "FilterLen" };
        readonly string[] coltype = new string[] { "String", "String", "Int32","Decimal", "String","Boolean", "Int32", "Decimal", "Int32", "Int32", "Int32", "Int32", "Int32" };
        readonly string[] colalert = new string[] { "Rs 组名", "Rs 组内序号", "Rs 通道", "Rx 名义值", "Rx 编号", "电流 X 2", "Rx 通道", "温度(℃)", "测量次数", "测量间隔 (秒)", "采样次数", "换向延时 (秒)", "滤波器" };
        readonly int[] colwidth = new int[] { 100, 100,60,150,20, 100, 60, 60,60, 60, 60, 60, 60 };

        readonly string SCH_DEFAULT = "默认程序";
        #endregion

        #region prg init function
        private void InitSchemeTab()
        {
            schemes = new List<string>();
            ds = new DataSet();
            try
            {
                ds.ReadXml(Util.basedir + "\\config.xml", XmlReadMode.Auto);
            }
            catch
            {
                MessageBox.Show("读取配置文件失败");
                return;
            }
            foreach (DataTable currdt in ds.Tables)
            {
                schemes.Add(currdt.TableName);
            }
            if (schemes.Count == 0)
            {
                AddScheme(SCH_DEFAULT);
            }

            LoadScheme(schemes[0]);
            cb_scheme.Items.AddRange(schemes.ToArray());
            cb_scheme.SelectedIndex = 0;
            cb_scheme.SelectedIndexChanged += new EventHandler((o, e) =>
            {
                LoadScheme(cb_scheme.Text);
            });
            btn_schdel.Click +=new EventHandler((o,e)=>{
                if(!bRunning)
                    DeleteScheme(cb_scheme.SelectedItem.ToString());
            });
            bt_schnew.Click += new EventHandler((o, e) =>
            {
                if (!bRunning)
                    AddScheme(cb_scheme.Text);
            });
            bt_rowadd.Click += new EventHandler((o, e) =>
            {
                if (!bRunning)
                    AddRow();
            });
            bt_rowmodify.Click += new EventHandler((o, e) =>
            {
                if (!bRunning)
                {
                    if (dgv_program.SelectedRows.Count > 0)
                        SaveRow(dgv_program.SelectedRows[0].Index);
                }
            });
            bt_del.Click += new EventHandler((o, e) =>
            {
                if (!bRunning)
                    DeleteRow();
            });

            dgv_program.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            dgv_program.AllowUserToOrderColumns = false;
            dgv_program.SelectionChanged += new EventHandler((o, e) =>
            {
                LoadRow();
            });
            dgv_program.CellEndEdit += new DataGridViewCellEventHandler((o, e) =>
            {
                LoadRow();
            });

            //add data grid style
            for (int i = 0; i < colname.Length; i++)
            {
                dgv_program.Columns[i].HeaderText = colalert[i];
                //dgv_program.Columns[i].Width = colwidth[i];
            }
            cb_rsgroup.Items.AddRange(rs_schemes.ToArray());
            cb_rsgroup.SelectedValueChanged +=new EventHandler((o,e)=>
            {
                LoadRsId();
            });
        }
        #endregion

        #region prg scheme function
        private void LoadRsId()
        {
            if (cb_rsgroup.Text == "")
                return;
            cb_rsid.Items.Clear();
            foreach (DataRow dr in rs_ds.Tables[cb_rsgroup.Text].Rows)
                cb_rsid.Items.Add(dr["RsID"].ToString());
        }
        private void DeleteScheme(string schnow)
        {
            if (schnow == SCH_DEFAULT)
                return;
            if (schemes.Count == 1)
                return;
            if (MessageBox.Show(string.Format(@"是否确定要删除程序'{0}'", schnow), "确认删除?", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                schemes.Remove(schnow);
                cb_scheme.Items.Remove(schnow);
                ds.Tables.Remove(schnow);
                SaveAll();
                LoadScheme(schemes[0]);
            }
        }
        private void LoadScheme(string sch_name)
        {
            if (!schemes.Contains(sch_name))
                return;

            if (sch_name == "")
                sch_name = schemes[0];
            dt = ds.Tables[sch_name];
            dgv_program.DataSource = dt;
            LoadRow();
        }
        private void AddScheme(string sch_new)
        {
            if (!schemes.Contains(sch_new))
            {
                DataTable newdt;
                if (sch_new != SCH_DEFAULT)
                {
                    newdt = dt.Copy();
                    newdt.TableName = sch_new;
                    ds.Tables.Add(newdt);
                }
                else
                {
                    newdt = new DataTable(sch_new);

                    for(int i = 0;i < colname.Length;i++)
                        newdt.Columns.Add(colname[i],Type.GetType("System."+coltype[i]));
                    ds.Tables.Add(newdt);
                }
                SaveAll();
                schemes.Add(sch_new);
                cb_scheme.Items.Add(sch_new);
                LoadScheme(sch_new);
            }
            else
            {
                if (cb_scheme.Text == sch_new)
                {
                    SaveAll();
                }
                else
                {
                    LoadScheme(sch_new);
                }
            }
        }
        private void SaveAll()
        {
            ds.AcceptChanges();
            ds.WriteXml(Util.basedir + "\\config.xml", XmlWriteMode.WriteSchema);
        }
        #endregion

        #region prg row function
        private void LoadRow()
        {
            int curr_row = 0;
            if (dt.Rows.Count <= 0)
                return;
            if (dgv_program.SelectedRows.Count > 0)
                curr_row = dgv_program.SelectedRows[0].Index;
            else
                curr_row = 0;

            if (dt.Rows.Count > curr_row)
            {
                DataRow dr = dt.Rows[curr_row];
                if (dr.RowState != DataRowState.Detached && dr.RowState != DataRowState.Deleted)
                {
                    //update field
                    foreach (string col in colname)
                    {
                        Control[] ctrls = this.Controls.Find("tb_" + col, true);
                        if(ctrls.Length > 0)
                            (ctrls[0] as TextBox).Text = dr[col].ToString();
                    }
                    cb_rsgroup.Text = dr["RsGroup"].ToString();
                    cb_rsid.Text = dr["RsID"].ToString();
                    ck_RxCurrent.Checked = (Boolean)dr["RxCurrent"];
                }
            }
        }
        private void AddRow()
        {
            if (!ValidateRow())
                return;
            {
                DataRow row = dt.NewRow();
                //save field
                for (int i = 0; i < coltype.Length; i++)
                {
                    Control[] ctrls = this.Controls.Find("tb_" + colname[i], true);
                    if (ctrls.Length <= 0)
                        continue;
                    string abc = (ctrls[0] as TextBox).Text;
                    try
                    {
                        if (coltype[i].ToString() == "Decimal")
                            row[colname[i]] = Convert.ToDecimal(abc);
                        if (coltype[i].ToString() == "Int32")
                            row[colname[i]] = Convert.ToInt32(abc);
                        if (coltype[i].ToString() == "String")
                            row[colname[i]] = abc;
                    }
                    catch
                    {
                        MessageBox.Show(colalert[i] + "项输入无效");
                        return;
                    }
                }
                row["RsGroup"] = cb_rsgroup.Text;
                row["RsID"] = cb_rsid.Text;
                row["RxCurrent"] = ck_RxCurrent.Checked;
                dt.Rows.Add(row);
                dt.AcceptChanges();
            }
            SaveAll();
        }
        private bool ValidateRow()
        {
            for (int i = 0; i < coltype.Length; i++)
            {
                Control[] ctrls = this.Controls.Find("tb_" + colname[i], true);
                if (ctrls.Length <= 0)
                    continue;
                string abc = (ctrls[0] as TextBox).Text;
                try
                {
                    if (coltype[i].ToString() == "Decimal")
                         Convert.ToDecimal(abc);
                    if (coltype[i].ToString() == "Int32")
                    {
                        if (Convert.ToInt32(abc) <= 0)
                        {
                            MessageBox.Show(colalert[i] + "项输入必须大于0");
                            return false;
                        }
                    }
                }
                catch
                {
                    MessageBox.Show(colalert[i] + "项输入无效");
                    return false;
                }
            }
            if ((Convert.ToDecimal(tb_RxValue.Text) > 1024) && ck_RxCurrent.Checked)
            {
                MessageBox.Show("10k以上电阻不能使用倍电流");
                return false;
            }
            if (Convert.ToDecimal(tb_RxValue.Text) <= 0)
            {
                MessageBox.Show("无效的被测电阻值");
                return false;
            }

            return true;
        }
        private void SaveRow(int curr_row)
        {
            if (!ValidateRow())
                return;
            if (curr_row >= 0 && curr_row < dt.Rows.Count && dgv_program.SelectedRows.Count > 0)
            {
                DataRow row = dt.Rows[curr_row];
                //save field
                for (int i = 0; i < coltype.Length; i++)
                {
                    Control[] ctrls = this.Controls.Find("tb_" + colname[i], true);
                    if(ctrls.Length <= 0)
                        continue;
                    string abc = (ctrls[0] as TextBox).Text;
                    try
                    {
                        if (coltype[i].ToString() ==  "Decimal")
                            row[colname[i]] = Convert.ToDecimal(abc);
                        if (coltype[i].ToString() == "Int32")
                            row[colname[i]] = Convert.ToInt32(abc);
                        if (coltype[i].ToString() == "String")
                            row[colname[i]] = abc;
                    }
                    catch
                    {
                        MessageBox.Show(colalert[i] + "项输入无效");
                        return;
                    }
                }
                row["RsGroup"] = cb_rsgroup.Text;
                row["RsID"] = cb_rsid.Text;
                row["RxCurrent"] = ck_RxCurrent.Checked;
                dt.AcceptChanges();
            }
            SaveAll();
        }
        private void DeleteRow()
        {
            
            if (dgv_program.SelectedRows.Count <= 0)
                return;
            int curr_row = -1;
            curr_row = dgv_program.SelectedRows[0].Index;
            
            if (curr_row >= 0 && curr_row < dt.Rows.Count)
            {
                dt.Rows.RemoveAt(curr_row);
                dt.AcceptChanges();
                SaveAll();
            }
            LoadRow();
        }
        #endregion

        #region Rs page related variable
        private DataSet rs_ds; //contains several tables, each table is a scheme
        private DataTable rs_dt; //one scheme table
        private List<string> rs_schemes;

        readonly string[] rs_colname = new string[] { "RsID","RsValue", "RsSerial", "Alpha", "Beta"};
        readonly string[] rs_coltype = new string[] { "String", "Decimal", "String", "Decimal", "Decimal" };
        readonly string[] rs_colalert = new string[] { "组内序号", "电阻值", "编号", "α", "β" };
        readonly int[] rs_colwidth = new int[] { 100, 150, 150, 50, 50 };

        readonly string RSSCH_DEFAULT = "Raysting";

        #endregion

        #region Rs page init function
        private void InitRsTab()
        {
            rs_schemes = new List<string>();
            rs_ds = new DataSet();
            try
            {
                rs_ds.ReadXml(Util.basedir + "\\rsall.xml", XmlReadMode.Auto);
            }
            catch
            {
                MessageBox.Show("读取标准电阻信息失败");
                return;

            }
            foreach (DataTable currdt in rs_ds.Tables)
            {
                rs_schemes.Add(currdt.TableName);
            }
            if (rs_schemes.Count == 0)
            {
                RsAddScheme(RSSCH_DEFAULT);
            }
            RsLoadScheme(rs_schemes[0]);
            cb_rsscheme.Items.AddRange(rs_schemes.ToArray());
            cb_rsscheme.SelectedIndex = 0;
            cb_rsscheme.SelectedIndexChanged += new EventHandler((o, e) =>
                {
                    RsLoadScheme(cb_rsscheme.Text);
                });
            btn_rsgrpdel.Click += new EventHandler((o, e) =>
                {
                    if (!bRunning)
                        RsDeleteScheme(cb_rsscheme.SelectedItem.ToString());
                });
            btn_rsgrpsave.Click += new EventHandler((o, e) =>
                {
                    if (!bRunning)
                        RsAddScheme(cb_rsscheme.Text);
                });
            btn_rsadd.Click +=new EventHandler((o,e)=>
            {
                if (!bRunning)
                    RsAddRow();
            });
            btn_rsupdate.Click += new EventHandler((o, e) =>
                {
                    if (!bRunning && dgv_rsdata.SelectedRows.Count > 0)
                        RsSaveRow(dgv_rsdata.SelectedRows[0].Index);
                });
            bt_rsdel.Click += new EventHandler((o, e) =>
                {
                    if (!bRunning)
                        RsDeleteRow();
                });
            dgv_rsdata.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            dgv_rsdata.AllowUserToOrderColumns = false;
            dgv_rsdata.SelectionChanged += new EventHandler((o, e) =>
                {
                    RsLoadRow();
                });
            dgv_rsdata.CellEndEdit +=new DataGridViewCellEventHandler((o,e)=>
                {
                    RsLoadRow();
                });

            //add grid style
            for (int i = 0; i < rs_colname.Length; i++)
            {
                dgv_rsdata.Columns[i].HeaderText = rs_colalert[i];
//                dgv_rsdata.Columns[i].Width = rs_colwidth[i];
            }
        }
        #endregion

        #region Rs page scheme and row handler
        private void RsLoadScheme(string sch_name)
        {
            if (!rs_schemes.Contains(sch_name))
                return;

            if (sch_name == "")
                sch_name = rs_schemes[0];
            rs_dt = rs_ds.Tables[sch_name];
            dgv_rsdata.DataSource = rs_dt;
            RsLoadRow();
        }
        private void RsAddScheme(string sch_new)
        {
            if (!rs_schemes.Contains(sch_new))
            {
                DataTable newdt;
                if (sch_new != RSSCH_DEFAULT)
                {
                    newdt = rs_dt.Copy();
                    newdt.TableName = sch_new;
                    rs_ds.Tables.Add(newdt);
                }
                else
                {
                    newdt = new DataTable(sch_new);

                    for (int i = 0; i < rs_colname.Length; i++)
                        newdt.Columns.Add(rs_colname[i], Type.GetType("System." + rs_coltype[i]));
                    rs_ds.Tables.Add(newdt);
                }
                RsSaveAll();
                rs_schemes.Add(sch_new);
                cb_rsgroup.Items.Add(sch_new);
                cb_rsscheme.Items.Add(sch_new);
                RsLoadScheme(sch_new);
                
            }
            else
            {
                if (cb_rsscheme.Text == sch_new)
                {
                    RsSaveAll();
                }
                else
                {
                    RsLoadScheme(sch_new);
                }
            }
            LoadRsId();
        }
        private void RsDeleteScheme(string schnow)
        {
            if (rs_schemes.Count == 1)
                return;
            if (MessageBox.Show(string.Format(@"是否确定要删除组名为'{0}'的标准电阻", schnow), "确认删除?", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                rs_schemes.Remove(schnow);
                cb_rsgroup.Items.Remove(schnow);
                cb_rsscheme.Items.Remove(schnow);

                rs_ds.Tables.Remove(schnow);
                RsSaveAll();
                RsLoadScheme(rs_schemes[0]);
                LoadRsId();
            }
        }
        private void RsDeleteRow()
        {
            if (dgv_rsdata.SelectedRows.Count <= 0)
                return;
            int curr_row = -1;
            curr_row = dgv_rsdata.SelectedRows[0].Index;

            if (curr_row >= 0 && curr_row < rs_dt.Rows.Count)
            {
                rs_dt.Rows.RemoveAt(curr_row);
                rs_dt.AcceptChanges();
                RsSaveAll();
            }
            RsLoadRow();
            LoadRsId();
        }
        private void RsAddRow()
        {
            if (!RsValidateRow())
                return;
            DataRow[] drs = rs_dt.Select(String.Format("RsID='{0}'", tb_RsID.Text));
            if(drs.Length >= 1)
            {
                MessageBox.Show("组内序号已存在，不能重复");
                return;
            }
            {
                DataRow row = rs_dt.NewRow();;
                //save field
                for (int i = 0; i < rs_coltype.Length; i++)
                {
                    string abc = (this.Controls.Find("tb_" + rs_colname[i], true)[0] as TextBox).Text;
                    try
                    {
                        if (rs_coltype[i].ToString() == "Decimal")
                            row[rs_colname[i]] = Convert.ToDecimal(abc);
                        if (rs_coltype[i].ToString() == "Int32")
                            row[rs_colname[i]] = Convert.ToInt32(abc);
                        if (rs_coltype[i].ToString() == "String")
                            row[rs_colname[i]] = abc;
                    }
                    catch
                    {
                        MessageBox.Show(rs_colalert[i] + "项输入无效");
                        return;
                    }
                }
                rs_dt.Rows.Add(row);
                rs_dt.AcceptChanges();
            }
            RsSaveAll();
            LoadRsId();
        }
        private void RsLoadRow()
        {
            int curr_row = 0;
            if (rs_dt.Rows.Count <= 0)
                return;
            if (dgv_rsdata.SelectedRows.Count > 0)
                curr_row = dgv_rsdata.SelectedRows[0].Index;
            else
                curr_row = 0;

            if (rs_dt.Rows.Count > curr_row)
            {
                DataRow dr = rs_dt.Rows[curr_row];
                if (dr.RowState != DataRowState.Detached && dr.RowState != DataRowState.Deleted)
                {
                    //update field
                    foreach (string col in rs_colname)
                    {
                        Control[] ctrls = this.Controls.Find("tb_" + col, true);
                        if (ctrls.Length > 0)
                            (ctrls[0] as TextBox).Text = dr[col].ToString();
                    }
                }
            }
        }
        private void RsSaveRow(int curr_row)
        {
            if (!RsValidateRow())
                return;
            if (curr_row >= 0 && curr_row < rs_dt.Rows.Count && dgv_rsdata.SelectedRows.Count > 0)
            {
                DataRow row = rs_dt.Rows[curr_row];
                //save field
                for (int i = 0; i < rs_coltype.Length; i++)
                {
                    string abc = (this.Controls.Find("tb_" + rs_colname[i], true)[0] as TextBox).Text;
                    try
                    {
                        if (rs_coltype[i].ToString() == "Decimal")
                            row[rs_colname[i]] = Convert.ToDecimal(abc);
                        if (rs_coltype[i].ToString() == "Int32")
                            row[rs_colname[i]] = Convert.ToInt32(abc);
                        if (rs_coltype[i].ToString() == "String")
                            row[rs_colname[i]] = abc;
                    }
                    catch
                    {
                        MessageBox.Show(rs_colalert[i] + "项输入无效");
                        return;
                    }
                }
                rs_dt.AcceptChanges();
            }
            RsSaveAll();
        }
        private void RsSaveAll()
        {
            rs_ds.AcceptChanges();
            rs_ds.WriteXml(Util.basedir + "\\rsall.xml", XmlWriteMode.WriteSchema);
        }
        private bool RsValidateRow()
        {
            for (int i = 0; i < rs_coltype.Length; i++)
            {
                Control[] ctrls = this.Controls.Find("tb_" + rs_colname[i], true);
                if (ctrls.Length <= 0)
                    continue;
                string abc = (ctrls[0] as TextBox).Text;
                try
                {
                    if (rs_coltype[i].ToString() == "Decimal")
                        Convert.ToDecimal(abc);
                    if (rs_coltype[i].ToString() == "Int32")
                        Convert.ToInt32(abc);
                }
                catch
                {
                    MessageBox.Show(rs_colalert[i] + "项输入无效");
                    return false;
                }
            }
            if (Convert.ToDecimal(tb_RsValue.Text) <= 0)
            {
                MessageBox.Show("无效的标准电阻值");
                return false;
            }

            return true;
        }
        #endregion

        #region data page related variable
        readonly string[] datacolname = new string[] { "RxValue", "RsValue", "Ratio", "Time", "scheme","RsID", "rxch", "rsch" };
        readonly string[] datacoltype = new string[] { "Decimal", "Decimal", "Decimal", "DateTime", "Int32", "Int32", "Int32", "Int32" };
        readonly string[] datacolalert = new string[] { "Rx电阻值", "Rs电阻值", "Rx/Rs比例值", "测试时间", "程序位置", "标准电阻位置", "Rx通道", "Rs通道" };
        readonly int[] datacolwidth = new int[] { 150, 150, 150, 120, 60,60,60,60 };
        readonly string DAT_DEFAULT = "当前结果";
        private DataSet dat_ds;     //contain test record, two table one is scheme content , another is test record
        private DataTable datsch_dt;//scheme table in current record
        private DataTable datrec_dt;//record table in current record
        private DataTable datrs_dt; //rs table in current record
        private DataTable runsch_dt;//record of current running scheme
        private DataTable runrec_dt;//record of current running record
        private DataTable runrs_dt;//record of current rs scheme
        private List<string> datarecords; //list of record names
        #endregion

        #region data page init function
        private void InitDataTab()
        {
            dat_ds = new DataSet();
            datarecords = new List<string>();

            
            btn_datasave.Click +=new EventHandler(bt_datasave_Click);
            runsch_dt = null;
            runrec_dt = Data_CreateNewTable();
            runrs_dt = Rs_CreateNewTable();
            dgv_result.DataSource = runrec_dt;
            dgv_result.SelectionChanged += new EventHandler(dataGridView3_SelectionChanged);
            for (int i = 0; i < datacolname.Length; i++)
            {
                dgv_result.Columns[i].HeaderText = datacolalert[i];
//                dgv_result.Columns[i].Width = datacolwidth[i];
            }
            dgv_result.Columns[datacolname.Length - 1].Visible = false; //last rsid column will not be shown
            tv_data.BeforeExpand += new TreeViewCancelEventHandler(tv_data_BeforeExpand);
            tv_data.AfterSelect += new TreeViewEventHandler(tv_data_AfterSelect);

            ExpandNode(null, true);
            tv_data.Refresh();
        }
        #region data management
        private void btn_datafolder_Click(object sender, EventArgs e)
        {
            if (tb_newdata.Text == DAT_DEFAULT)
                return;
            TreeNode tn = tv_data.SelectedNode;
            if (!(tn is TreeNode))
                return;
            string newfolder;
            
            newfolder = Util.datadir + GetPathFromNode(tn);
            if (File.Exists(newfolder+".xml")) //file
            {
                FileInfo fi = new FileInfo(Util.datadir + GetPathFromNode(tn));
                newfolder = fi.Directory.FullName;
            }
            if (IsDataDefault(tn))
                newfolder = Util.datadir;
            newfolder = newfolder + "\\" + tb_newdata.Text;
            try
            {
                Directory.CreateDirectory(newfolder);
            }
            catch
            {
                MessageBox.Show("创建文件夹" + tb_newdata.Text + "失败");
            }
            ExpandNode(null,true);
        }
        private string OneMeasureRecord(DataRow dr, DataRow schdr, DataRow rsdr, IEnumerable<double> ylist, int cnt_total)
        {
            return String.Format(@"
==============测量结果=================
测试时间:\t{18}
平均值:\t{10}
Rx/Rs比例值:\t{19}
不确定度(ppm):\t{11}
统计个数:\t{20}
最大值\t{12}
最小值\t{13}
电流 X2\t{4}
采样次数\t{5}
测量次数\t{6}
测量间隔(秒)\t{7}
滤波器\t{8}
换向延时(秒)\t{9}
Rs编号\t{0}
Rs通道\tCH{1}
温度(℃)\t{14}
Rs电阻值(Ω)\t{17}
alpha\t{15}
beta\t{16}
Rx编号\t{2}
Rx通道\tCH{3}
", rsdr["RsSerial"].ToString(), schdr["RsChannel"].ToString(), schdr["RxSerial"].ToString(), schdr["RxChannel"].ToString(),
                    Util.ToYesNo((bool)schdr["RxCurrent"]), schdr["SampleTime"].ToString(), schdr["MeasTime"].ToString(), schdr["Delay"].ToString(), schdr["FilterLen"].ToString(), schdr["SwitchTime"].ToString(),
                    Util.FormatData(Util.my_avg(ylist), 11), Util.FormatData(1e6 * Util.my_sqrtsum(ylist), 3), Util.FormatData(Util.my_max(ylist), 11), Util.FormatData(Util.my_min(ylist), 11),
                    schdr["Temp"].ToString(), rsdr["Alpha"].ToString(), rsdr["Beta"].ToString(), rsdr["RsValue"].ToString(), dr["time"].ToString(),
                    Util.FormatData(Util.my_avg(ylist) / Convert.ToDouble(dr["RsValue"].ToString()), 11), ylist.Count().ToString()).Replace("\\t", "\t");

        }
        private bool ExportTextRecord(string datafile, string exportfile,bool bFirst)
        {
            try
            {
                DataSet ex_ds = new DataSet();
                DataTable exrec_dt;
                DataTable exrs_dt;
                DataTable exsch_dt;

                ex_ds.Clear();
                try
                {
                    ex_ds.ReadXml(datafile, XmlReadMode.Auto);
                    exsch_dt = ex_ds.Tables["scheme"];

                    if (!ex_ds.Tables.Contains("record"))
                        return false;
                    exrec_dt = ex_ds.Tables["record"];


                    if (!ex_ds.Tables.Contains("rs"))
                        return false;
                    exrs_dt = ex_ds.Tables["rs"];

                }
                catch
                {
                    return false;
                }

                if (!exportfile.ToLower().EndsWith(".txt"))
                    exportfile = exportfile + ".txt";

                FileStream fsLog;
                if(bFirst)
                    fsLog = new FileStream(exportfile, FileMode.Create, FileAccess.Write, FileShare.Read);
                else
                    fsLog = new FileStream(exportfile, FileMode.Append, FileAccess.Write, FileShare.Read);

                StreamWriter sw = new StreamWriter(fsLog);

                List<int> schids = new List<int>();
                foreach (DataRow drrec in exrec_dt.Rows)
                {

                    DataRow dr, rsdr, schdr;
                    IEnumerable<double> ylist;

                    //avoid duplication export for one scheme
                    int cnt_total = (int)drrec["scheme"] - 1;
                    if (schids.Contains(cnt_total))
                        continue;
                    schids.Add(cnt_total);

                    if (exrec_dt == null || exrec_dt.Rows.Count <= cnt_total)
                        break;
                    dr = exrec_dt.Rows[cnt_total];

                    int schrow = (int)dr["scheme"];
                    if (exsch_dt == null || exsch_dt.Rows.Count < schrow || schrow <= 0) //attention, schrow is 1 based for easy read
                        break;
                    schdr = exsch_dt.Rows[schrow - 1];

                    int rsrow = (int)dr["RsID"];
                    if (exrs_dt == null || exrs_dt.Rows.Count <= rsrow)
                        break;
                    rsdr = exrs_dt.Rows[rsrow];

                    ylist = exrec_dt.Select("scheme='" + schrow.ToString() + "'").Select<DataRow, double>((o) => { return Convert.ToDouble(o["RxValue"].ToString()); });

                    sw.Write(OneMeasureRecord(dr, schdr, rsdr, ylist, cnt_total));
                }
                sw.Close();
                fsLog.Close();
                return true;
            }
            catch (Exception ex)
            {
                Util.SysLog("ExportRunRecord:" + ex.Message + ex.StackTrace);
                return false;
            }
        }
        private bool ExportRunRecord(string exportfile)
        {
            DataTable rec_dt = runrec_dt;
            if (rec_dt == null || rec_dt.Rows.Count <= 0)
                return false;
            try
            {
                if (!exportfile.ToLower().EndsWith(".txt"))
                    exportfile = exportfile + ".txt";

                FileStream fsLog = new FileStream(exportfile, FileMode.Create, FileAccess.Write, FileShare.Read);
                StreamWriter sw = new StreamWriter(fsLog);

                List<int> schids = new List<int>();
                foreach (DataRow drrec in rec_dt.Rows)
                {

                    DataRow dr, rsdr, schdr;
                    IEnumerable<double> ylist;

                    //avoid duplication export for one scheme
                    int cnt_total = (int)drrec["scheme"]-1;
                    if (schids.Contains(cnt_total))
                        continue;
                    schids.Add(cnt_total);
                    if (!Data_SearchItem(cnt_total, out dr, out schdr, out rsdr, out ylist))
                        break;

                    sw.Write(OneMeasureRecord(dr,schdr,rsdr,ylist,cnt_total));
                }
                sw.Close();
                fsLog.Close();
                return true;
            }
            catch (Exception ex)
            {
                Util.SysLog("ExportRunRecord:" + ex.Message + ex.StackTrace);
                return false;
            }
        }

        private void btn_export_Click(object sender, EventArgs e)
        {
            bool bDefault = false;
            bool bDir = false;
            bool ret = false;

            bDefault = IsDataDefault(tv_data.SelectedNode);
            bDir = IsDirNode(tv_data.SelectedNode);
            saveFileDialog1.Filter = "Text File(*.txt)|*.txt|All Files(*.*)|*.*";
            saveFileDialog1.FileName = "output.txt";
            if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            try
            {

                if (bDefault)
                {
                    ret = ExportRunRecord(saveFileDialog1.FileName);
                }
                else
                {
                    if (bDir)
                    {
                        bool bFirst = true;
                        foreach (TreeNode tn in tv_data.SelectedNode.Nodes)
                        {
                            ret = ExportTextRecord(Util.datadir + GetPathFromNode(tn) + ".xml", saveFileDialog1.FileName, bFirst);
                            if (!ret)
                                break;
                            if (bFirst)
                                bFirst = false;
                        }
                    }
                    else
                    {
                        ret = ExportTextRecord(Util.datadir + GetPathFromNode(tv_data.SelectedNode) + ".xml", saveFileDialog1.FileName, true);
                    }
                }
                if (ret)
                    MessageBox.Show("数据导出成功");
                else
                    MessageBox.Show("数据导出失败");
                return;

            }
            catch(Exception ex)
            {
                Util.SysLog("export_Click:" + ex.Message + ex.StackTrace);
            }


            

        }

        private void btn_datadel_Click(object sender, EventArgs e)
        {
            TreeNode tn = tv_data.SelectedNode;
            if (IsDataDefault(tn))
                return;
            if (tn.Nodes.Count > 0)
                return;
            if (Directory.Exists(Util.datadir + GetPathFromNode(tn)))
            {
                try
                {
                    Directory.Delete(Util.datadir + GetPathFromNode(tn));
                    tn.Remove();
                }
                catch(Exception ex)
                {
                    Util.SysLog("datadel_Click:" + ex.Message + ex.StackTrace);
                }
                return;
            }
            if (DialogResult.Yes == MessageBox.Show(String.Format("确定要删除数据'{0}'", tn.Text), "问题", MessageBoxButtons.YesNo))
            {
                try
                {
                    File.Delete(Util.datadir + GetPathFromNode(tn) + ".xml");
                    tn.Remove();
                }
                catch(Exception ex)
                {
                    MessageBox.Show("数据删除失败:" + ex.Message);
                    Util.SysLog("datadel_Click:" + ex.Message + ex.StackTrace);
                }
            }
        }

        private string GetPathFromNode(TreeNode node)
        {
            if (node.Parent == null)
            {
                return node.Text;
            }
            return Path.Combine(GetPathFromNode(node.Parent), node.Text);
        }

        private void ExpandNode(TreeNode node,bool bRoot)
        {
            try
            {
                if (bRoot)
                {
                    tv_data.Nodes.Clear();
                    TreeNode runNode = new TreeNode();
                    runNode.Text = DAT_DEFAULT;
                    tv_data.Nodes.Add(runNode);
                }
                string selectPath = ".";
                if(node is TreeNode)
                    selectPath = GetPathFromNode(node);

                if (Directory.Exists(Util.datadir + selectPath))//判断是否为目录
                {
                    DirectoryInfo dirInfo = new DirectoryInfo(Util.datadir + selectPath);

                    
                    FileInfo[] fileInfoInDir = dirInfo.GetFiles();//子文件信息

                    for (int i = 0; i < fileInfoInDir.Length; i++)
                    {
                        if (FileAttributes.Hidden != (fileInfoInDir[i].Attributes & FileAttributes.Hidden) &&
                            fileInfoInDir[i].Name.EndsWith(".xml")) //过滤隐藏的文件
                        {
                            TreeNode subFileNode = new TreeNode();
                            subFileNode.Text = fileInfoInDir[i].Name.Substring(0, fileInfoInDir[i].Name.Length - 4);
                            subFileNode.ImageIndex = 0;
                            subFileNode.SelectedImageIndex = 0;
                            if (bRoot)
                            {
                                tv_data.Nodes.Add(subFileNode);
                            }
                            else
                            {
                                node.Nodes.Add(subFileNode);
                            }
                        }
                    }

                    DirectoryInfo[] subDirInfo = dirInfo.GetDirectories();//子目录信息
                    
                    for (int i = 0; i < subDirInfo.Length; i++)
                    {
                        if (FileAttributes.Hidden != (subDirInfo[i].Attributes & FileAttributes.Hidden)) //过滤隐藏的文件夹
                        {
                            if (!subDirInfo[i].Name.Equals(".") && !subDirInfo[i].Name.Equals(".."))
                            {
                                TreeNode subDirNode = new TreeNode();
                                subDirNode.Text = subDirInfo[i].Name;
                                subDirNode.ImageIndex = 1;
                                subDirNode.SelectedImageIndex = 1;
                                //if (subDirNode.Nodes.Count > 0)
                                {
                                    if (bRoot)
                                        tv_data.Nodes.Add(subDirNode);
                                    else
                                        node.Nodes.Add(subDirNode);
                                }
                                ExpandNode(subDirNode, false);
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Util.SysLog("ExpandNode:" + ex.Message + ex.StackTrace);
            }
        }
        
        void tv_data_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (e.Node.Nodes.Count <= 0) //leaf node
            {
                Data_LoadScheme(e.Node);
                tb_newdata.Text = e.Node.Text;
                dataGridView3_SelectionChanged(null, null);
            }
            if (e.Node.Nodes.Count > 0 || Directory.Exists(Util.datadir + GetPathFromNode(e.Node))) //folder
                e.Node.ImageIndex = 1;
            else
                e.Node.ImageIndex = 0;
        }

        bool IsDirNode(TreeNode n)
        {
            if (n is TreeNode)
            {
                if (n.Nodes.Count > 0 || Directory.Exists(Util.datadir + GetPathFromNode(n)))
                    return true;
            }
            return false;
        }
        void tv_data_BeforeExpand(object sender, TreeViewCancelEventArgs e)
        {
        }
        #endregion
        #endregion

        #region data page event
        void dataGridView3_SelectionChanged(object sender, EventArgs e)
        {
            if (dgv_result.SelectedRows.Count > 0)
                Data_LoadGraph(dgv_result.SelectedRows[0].Index);
            else if (dgv_result.SelectedCells.Count > 0)
                Data_LoadGraph(dgv_result.SelectedCells[0].RowIndex);
            else
                Data_LoadGraph(0);
        }
        private void bt_datasave_Click(object sender, EventArgs e)
        {
            try
            {
                TreeNode tn = tv_data.SelectedNode;

                if (tb_newdata.Text.Trim() == DAT_DEFAULT || (!(tn is TreeNode)))
                    return;

                if (runrec_dt == null || runsch_dt == null || runrs_dt == null)
                    return;
                string newpath;
                if (IsDataDefault(tn))
                    newpath = Util.datadir;
                else
                    newpath = Util.datadir + GetPathFromNode(tn);
                if (!File.Exists(newpath + ".xml"))  //it is a directory
                    newpath = newpath + "\\" + tb_newdata.Text.Trim();

                if (runrec_dt.Rows.Count > 0 && runsch_dt.Rows.Count > 0)
                {
                    newpath = newpath + ".xml";
                    if (File.Exists(newpath))
                        File.Delete(newpath);
                    DataSet new_ds = new DataSet();
                    new_ds.Tables.Add(runsch_dt.Copy());
                    new_ds.Tables.Add(runrec_dt.Copy());
                    new_ds.Tables.Add(runrs_dt.Copy());
                    new_ds.AcceptChanges();
                    new_ds.WriteXml(newpath, XmlWriteMode.WriteSchema);
                    ExpandNode(null, true);
                }
                else
                {
                    MessageBox.Show(@"无法保存数据,当前无测试数据。");
                }
            }
            catch
            {
                MessageBox.Show(@"数据保存失败,请确保数据名不包含.*[]<>\\/等符号");
            }
        }
        #endregion

        #region data page function

        bool IsDataDefault(TreeNode tn)
        {
            if (tn is TreeNode)
            {
                return tn.Text == DAT_DEFAULT && tn.Level == 0;
            }
            return false;
        }
        void Data_LoadScheme(TreeNode tn)
        {
            if (tn.Nodes.Count > 0 || Directory.Exists(Util.datadir + GetPathFromNode(tn))) //folder
                return;
            if (!IsDataDefault(tn)) //not running node
            {
                dat_ds.Clear();
                try
                {
                    dat_ds.ReadXml(Util.datadir + GetPathFromNode(tn) + ".xml", XmlReadMode.Auto);
                }
                catch
                {
                    MessageBox.Show("读取数据文件失败");
                    return;
                }

                datsch_dt = dat_ds.Tables["scheme"];
                if (dat_ds.Tables.Contains("record"))
                {
                    datrec_dt = dat_ds.Tables["record"];
                }
                else
                {
                    datrec_dt = Data_CreateNewTable();
                    dat_ds.Tables.Add(datrec_dt);
                    Data_SaveAll();
                }
                if (dat_ds.Tables.Contains("rs"))
                {
                    datrs_dt = dat_ds.Tables["rs"];
                }
                else
                {
                    datrs_dt = Rs_CreateNewTable();
                    dat_ds.Tables.Add(datrs_dt);
                    Data_SaveAll();
                }
                dgv_result.DataSource = datrec_dt;
            }
            else
            {
                dgv_result.DataSource = runrec_dt;
            }
        }
        
        void Data_SaveAll()
        {
            if (!IsDataDefault(tv_data.SelectedNode))
            {
                dat_ds.AcceptChanges();
                dat_ds.WriteXml(GetPathFromNode(tv_data.SelectedNode), XmlWriteMode.WriteSchema);
            }
        }
        DataTable Data_CreateNewTable()
        {
            DataTable newdt = new DataTable("record");
            for (int i = 0; i < datacolname.Length; i++)
                newdt.Columns.Add(datacolname[i], Type.GetType("System." + datacoltype[i]));
            return newdt;
        }
        DataTable Rs_CreateNewTable()
        {
            DataTable newdt = new DataTable("rs");
            for (int i = 0; i < rs_colname.Length; i++)
                newdt.Columns.Add(rs_colname[i], Type.GetType("System." + rs_coltype[i]));
            return newdt;
        }
        bool Data_SearchItem(int rowid, out DataRow dr, out DataRow schdr, out DataRow rsdr, out IEnumerable<double> ylist)
        {
            DataTable mdat_dt, msch_dt, mrs_dt;
            dr = schdr = rsdr = null;
            ylist = Enumerable.Repeat<double>(0.0, 1);
            if (!IsDataDefault(tv_data.SelectedNode))
            {
                mdat_dt = datrec_dt;
                msch_dt = datsch_dt;
                mrs_dt = datrs_dt;
            }
            else
            {
                mdat_dt = runrec_dt;
                msch_dt = runsch_dt;
                mrs_dt = runrs_dt;
            }
            if (mdat_dt == null || mdat_dt.Rows.Count <= rowid)
                return false;
            dr = mdat_dt.Rows[rowid];

            int schrow = (int)dr["scheme"] ;
            if (msch_dt == null || msch_dt.Rows.Count < schrow || schrow <= 0) //attention, schrow is 1 based for easy read
                return false;
            schdr = msch_dt.Rows[schrow-1];

            int rxch = (int)dr["rxch"];
            int rsch = (int)dr["rsch"];

            int rsrow = (int)dr["RsID"];
            if (mrs_dt == null || mrs_dt.Rows.Count <= rsrow)
                return false;
            rsdr = mrs_dt.Rows[rsrow];

            string qry = "scheme='" + schrow.ToString() + "' and rxch='" + rxch.ToString() +  "' and rsch='" + rsch.ToString() + "'";
            ylist = mdat_dt.Select(qry).Select<DataRow, double>((o) => { return Convert.ToDouble(o["RxValue"].ToString()); });
            return true;
        }
        void Data_LoadGraph(int rowid)
        {
            DataRow dr, schdr,rsdr;
            IEnumerable<double> ylist;

            //drawing curves
            GraphPane myPane = zedGraphControl1.GraphPane;
            myPane.CurveList.Clear();
            tb_DataSummary.Text = "";
            myPane.Title.Text = "测试结果";

            if (!Data_SearchItem(rowid, out dr, out schdr,out rsdr,out ylist))
            {
                zedGraphControl1.Refresh();
                return;
            }
            double[] yvalues = Util.to_ppm_array(ylist);
            tb_DataSummary.Text = String.Format(@"
==============统计信息=================
平均值 ={10}   Rx/Rs比例值 ={18}
不确定度(ppm)  ={11}    统计个数={19}
最大值  ={12}   最小值 ={13}

==============测量参数=================
电流X2={4}    采样次数 ={5}
测量次数={6}    测量间隔(秒)={7}
滤波器={8}    换向延时(秒)={9}

==============Rs信息=================
Rs编号={0}    Rs通道    =CH{1}
温度(℃) ={14}   Rs电阻值(Ω)  ={17}       
alpha ={15}   beta ={16}

==============Rx信息=================
Rx编号={2}    Rx通道    =CH{3}


", rsdr["RsSerial"].ToString(), schdr["RsChannel"].ToString(), schdr["RxSerial"].ToString(), schdr["RxChannel"].ToString(),
Util.ToYesNo((bool)schdr["RxCurrent"]), schdr["SampleTime"].ToString(), schdr["MeasTime"].ToString(), schdr["Delay"].ToString(), schdr["FilterLen"].ToString(),schdr["SwitchTime"].ToString(),
Util.FormatData(Util.my_avg(ylist), 11), Util.FormatData(1e6*Util.my_sqrtsum(ylist), 3), Util.FormatData(Util.my_max(ylist), 11), Util.FormatData(Util.my_min(ylist), 11),
schdr["Temp"].ToString(), rsdr["Alpha"].ToString(), rsdr["Beta"].ToString(), rsdr["RsValue"].ToString(), Util.FormatData(Util.my_avg(ylist) / Convert.ToDouble(dr["RsValue"].ToString()), 11), ylist.Count().ToString());

            LineItem li = myPane.AddCurve("Rx", Enumerable.Range(1, yvalues.Length).Select<int,double>((o) => { return Convert.ToDouble(o); }).ToArray(), yvalues, Color.Black);
            li.Symbol.Fill = new Fill(System.Drawing.Color.Blue);
            myPane.XAxis.Title.Text = "";
            myPane.YAxis.Title.Text = "PPM";
            myPane.Title.Text = String.Format("测试结果 {0}", Util.FormatData(Util.my_avg(ylist), 11));
            myPane.AxisChange();
            zedGraphControl1.Refresh();
        }

        #endregion

        #region running state update
        private bool bRunning
        {
            get
            {
                return backgroundWorker1.IsBusy;
            }
        }
        DataTable runtable;
        private void btn_start_Click(object sender, TabControlCancelEventArgs e)
        {
            if (e.Action == TabControlAction.Selecting && e.TabPageIndex == 0) //first tab is selected
            {
                e.Cancel = true;
                if (bRunning)
                {
                    //stop backworker
                    if (backgroundWorker1.IsBusy)
                    {
                        backgroundWorker1.CancelAsync();
                    }
                    UpdateRunButton(RunState.STOPPING);

                }
                else
                {
                    runsch_dt = dt.Copy();
                    runsch_dt.TableName = "scheme";
                    runrec_dt.Rows.Clear();
                    runrec_dt = Data_CreateNewTable();
                    runrs_dt.Rows.Clear();
                    runrs_dt = Rs_CreateNewTable();

                    runtable = dt.Copy();

                    backgroundWorker1.RunWorkerAsync(runtable);
                    UpdateRunButton(RunState.RUNNING);
                    dgv_program.ReadOnly = true;
                    cb_scheme.Enabled = false;
                }
            }
        }

        private void HasNewReading(object sender, string sval_index)
        {
            QJ55A qjfrom = sender as QJ55A;
            int val_index = Int32.Parse(sval_index);
            if (!bRunning)
                return;


            if (val_index > MeasTime)   //            if (val_index != (runrec_dt.Rows.Count + 1))
            {
                Util.SysLog("Unexpected reading:" + val_index.ToString());
//              MessageBox.Show("无效的读数");
                return;
            }
            
            DataRow dr = runrec_dt.NewRow();
            dr["RxValue"] = qjfrom.myValue;
            if (Convert.ToDouble(qjfrom.myRs) < 1e-10)
                qjfrom.myRs = 9999999;

            dr["RsValue"] = qjfrom.myRs;
            dr["Ratio"] = Convert.ToDecimal(Double.Parse(Util.FormatData(Convert.ToDouble(qjfrom.myValue) / Convert.ToDouble(qjfrom.myRs), 11)));
            dr["Time"] = DateTime.Now;
            dr["scheme"] = meas_schid+1;
            dr["rsid"] = rs_schid;
            dr["rsch"] = rs_chid;
            dr["rxch"] = rx_chid;
            runrec_dt.Rows.Add(dr);
            runrec_dt.AcceptChanges();
            meascount = val_index;
            this.BeginInvoke(new Action(RefreshRunResultDisplay));
        }
        private void RefreshRunResultDisplay()
        {
            try
            {
                if (IsDataDefault(tv_data.SelectedNode))
                {
                    dgv_result.DataSource = runrec_dt;
                    dgv_result.Refresh();
                    Data_LoadGraph(runrec_dt.Rows.Count - 1);
                }
            }
            catch (Exception ex)
            {
                Util.SysLog("RefreshRunResultDisplay:" + ex.Message + ex.StackTrace);
            }

        }
#endregion



        

    }
}
