using System.Drawing;
namespace TSioex
{
    partial class ProdHistory
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">1true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.dataGridView1 = new System.Windows.Forms.DataGrid();
            this.mc_starttime = new System.Windows.Forms.MonthCalendar();
            this.mc_endtime = new System.Windows.Forms.MonthCalendar();
            this.lb_oper = new System.Windows.Forms.ListBox();
            this.lb_prodno = new System.Windows.Forms.ListBox();
            this.lbl_starttime = new System.Windows.Forms.Label();
            this.lbl_endtime = new System.Windows.Forms.Label();
            this.lbl_oper = new System.Windows.Forms.Label();
            this.lbl_prodno = new System.Windows.Forms.Label();
            this.lbl_prod = new System.Windows.Forms.Label();
            this.lb_prod = new System.Windows.Forms.ListBox();
            this.lbl_summary = new System.Windows.Forms.Label();
            this.btnClr = new System.Windows.Forms.Button();
            this.btn_ret_cfg = new System.Windows.Forms.PictureBox();
            this.btn_ret_run = new System.Windows.Forms.PictureBox();
            this.dataGridTableStyle1 = new System.Windows.Forms.DataGridTableStyle();
            this.dataGridTextBoxColumn1 = new System.Windows.Forms.DataGridTextBoxColumn();
            this.dataGridTextBoxColumn2 = new System.Windows.Forms.DataGridTextBoxColumn();
            this.dataGridTextBoxColumn3 = new System.Windows.Forms.DataGridTextBoxColumn();
            this.dataGridTextBoxColumn4 = new System.Windows.Forms.DataGridTextBoxColumn();
            this.dataGridTextBoxColumn5 = new System.Windows.Forms.DataGridTextBoxColumn();
            this.dataGridTextBoxColumn6 = new System.Windows.Forms.DataGridTextBoxColumn();
            this.dataGridTextBoxColumn7 = new System.Windows.Forms.DataGridTextBoxColumn();
            this.dataGridTextBoxColumn8 = new System.Windows.Forms.DataGridTextBoxColumn();
            this.dataGridTextBoxColumn9 = new System.Windows.Forms.DataGridTextBoxColumn();
            this.dataGridTextBoxColumn10 = new System.Windows.Forms.DataGridTextBoxColumn();
            this.SuspendLayout();
            // 
            // dataGridView1
            // 
            this.dataGridView1.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(128)))), ((int)(((byte)(128)))));
            this.dataGridView1.Location = new System.Drawing.Point(9, 226);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(1002, 400);
            this.dataGridView1.TabIndex = 2;
            this.dataGridView1.TableStyles.Add(this.dataGridTableStyle1);
            // 
            // mc_starttime
            // 
            this.mc_starttime.Location = new System.Drawing.Point(9, 29);
            this.mc_starttime.Name = "mc_starttime";
            this.mc_starttime.Size = new System.Drawing.Size(332, 185);
            this.mc_starttime.TabIndex = 3;
            // 
            // mc_endtime
            // 
            this.mc_endtime.Location = new System.Drawing.Point(344, 29);
            this.mc_endtime.Name = "mc_endtime";
            this.mc_endtime.Size = new System.Drawing.Size(332, 185);
            this.mc_endtime.TabIndex = 4;
            // 
            // lb_oper
            // 
            this.lb_oper.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular);
            this.lb_oper.Location = new System.Drawing.Point(679, 29);
            this.lb_oper.Name = "lb_oper";
            this.lb_oper.Size = new System.Drawing.Size(99, 164);
            this.lb_oper.TabIndex = 5;
            // 
            // lb_prodno
            // 
            this.lb_prodno.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular);
            this.lb_prodno.Location = new System.Drawing.Point(784, 29);
            this.lb_prodno.Name = "lb_prodno";
            this.lb_prodno.Size = new System.Drawing.Size(110, 164);
            this.lb_prodno.TabIndex = 5;
            // 
            // lbl_starttime
            // 
            this.lbl_starttime.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular);
            this.lbl_starttime.Location = new System.Drawing.Point(14, 3);
            this.lbl_starttime.Name = "lbl_starttime";
            this.lbl_starttime.Size = new System.Drawing.Size(264, 20);
            this.lbl_starttime.Text = "label1";
            // 
            // lbl_endtime
            // 
            this.lbl_endtime.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular);
            this.lbl_endtime.Location = new System.Drawing.Point(344, 3);
            this.lbl_endtime.Name = "lbl_endtime";
            this.lbl_endtime.Size = new System.Drawing.Size(269, 20);
            this.lbl_endtime.Text = "label1";
            // 
            // lbl_oper
            // 
            this.lbl_oper.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular);
            this.lbl_oper.Location = new System.Drawing.Point(679, 0);
            this.lbl_oper.Name = "lbl_oper";
            this.lbl_oper.Size = new System.Drawing.Size(99, 20);
            this.lbl_oper.Text = "label1";
            // 
            // lbl_prodno
            // 
            this.lbl_prodno.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular);
            this.lbl_prodno.Location = new System.Drawing.Point(784, 0);
            this.lbl_prodno.Name = "lbl_prodno";
            this.lbl_prodno.Size = new System.Drawing.Size(110, 20);
            this.lbl_prodno.Text = "lbl_prodno";
            // 
            // lbl_prod
            // 
            this.lbl_prod.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular);
            this.lbl_prod.Location = new System.Drawing.Point(900, 3);
            this.lbl_prod.Name = "lbl_prod";
            this.lbl_prod.Size = new System.Drawing.Size(112, 20);
            this.lbl_prod.Text = "lbl_prod";
            // 
            // lb_prod
            // 
            this.lb_prod.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular);
            this.lb_prod.Location = new System.Drawing.Point(900, 29);
            this.lb_prod.Name = "lb_prod";
            this.lb_prod.Size = new System.Drawing.Size(113, 164);
            this.lb_prod.TabIndex = 9;
            // 
            // lbl_summary
            // 
            this.lbl_summary.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular);
            this.lbl_summary.ForeColor = System.Drawing.Color.DarkGoldenrod;
            this.lbl_summary.Location = new System.Drawing.Point(184, 713);
            this.lbl_summary.Name = "lbl_summary";
            this.lbl_summary.Size = new System.Drawing.Size(468, 24);
            this.lbl_summary.Text = "...";
            // 
            // btnClr
            // 
            this.btnClr.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular);
            this.btnClr.Location = new System.Drawing.Point(9, 682);
            this.btnClr.Name = "btnClr";
            this.btnClr.Size = new System.Drawing.Size(157, 52);
            this.btnClr.TabIndex = 13;
            this.btnClr.Text = "button1";
            this.btnClr.Click += new System.EventHandler(this.btnClr_Click);
            // 
            // btn_ret_cfg
            // 
            this.btn_ret_cfg.Image = global::TSioex.Properties.Resources.ret1;
            this.btn_ret_cfg.Location = new System.Drawing.Point(683, 692);
            this.btn_ret_cfg.Name = "btn_ret_cfg";
            this.btn_ret_cfg.Size = new System.Drawing.Size(148, 56);
            this.btn_ret_cfg.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            // 
            // btn_ret_run
            // 
            this.btn_ret_run.Image = global::TSioex.Properties.Resources.ret2;
            this.btn_ret_run.Location = new System.Drawing.Point(865, 692);
            this.btn_ret_run.Name = "btn_ret_run";
            this.btn_ret_run.Size = new System.Drawing.Size(148, 56);
            this.btn_ret_run.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            // 
            // dataGridTableStyle1
            // 
            this.dataGridTableStyle1.GridColumnStyles.Add(this.dataGridTextBoxColumn1);
            this.dataGridTableStyle1.GridColumnStyles.Add(this.dataGridTextBoxColumn2);
            this.dataGridTableStyle1.GridColumnStyles.Add(this.dataGridTextBoxColumn3);
            this.dataGridTableStyle1.GridColumnStyles.Add(this.dataGridTextBoxColumn4);
            this.dataGridTableStyle1.GridColumnStyles.Add(this.dataGridTextBoxColumn5);
            this.dataGridTableStyle1.GridColumnStyles.Add(this.dataGridTextBoxColumn6);
            this.dataGridTableStyle1.GridColumnStyles.Add(this.dataGridTextBoxColumn7);
            this.dataGridTableStyle1.GridColumnStyles.Add(this.dataGridTextBoxColumn8);
            this.dataGridTableStyle1.GridColumnStyles.Add(this.dataGridTextBoxColumn9);
            this.dataGridTableStyle1.GridColumnStyles.Add(this.dataGridTextBoxColumn10);
            this.dataGridTableStyle1.MappingName = "BOX";
            // 
            // dataGridTextBoxColumn1
            // 
            this.dataGridTextBoxColumn1.Format = "";
            this.dataGridTextBoxColumn1.FormatInfo = null;
            this.dataGridTextBoxColumn1.HeaderText = "start_date";
            this.dataGridTextBoxColumn1.Width = 100;
            // 
            // dataGridTextBoxColumn2
            // 
            this.dataGridTextBoxColumn2.Format = "";
            this.dataGridTextBoxColumn2.FormatInfo = null;
            this.dataGridTextBoxColumn2.HeaderText = "end_date";
            this.dataGridTextBoxColumn2.Width = 100;
            // 
            // dataGridTextBoxColumn3
            // 
            this.dataGridTextBoxColumn3.Format = "";
            this.dataGridTextBoxColumn3.FormatInfo = null;
            this.dataGridTextBoxColumn3.HeaderText = "operator";
            this.dataGridTextBoxColumn3.Width = 100;
            // 
            // dataGridTextBoxColumn4
            // 
            this.dataGridTextBoxColumn4.Format = "";
            this.dataGridTextBoxColumn4.FormatInfo = null;
            this.dataGridTextBoxColumn4.HeaderText = "product_no";
            this.dataGridTextBoxColumn4.Width = 100;
            // 
            // dataGridTextBoxColumn5
            // 
            this.dataGridTextBoxColumn5.Format = "";
            this.dataGridTextBoxColumn5.FormatInfo = null;
            this.dataGridTextBoxColumn5.HeaderText = "description";
            this.dataGridTextBoxColumn5.Width = 160;
            // 
            // dataGridTextBoxColumn6
            // 
            this.dataGridTextBoxColumn6.Format = "";
            this.dataGridTextBoxColumn6.FormatInfo = null;
            this.dataGridTextBoxColumn6.HeaderText = "target";
            this.dataGridTextBoxColumn6.Width = 80;
            // 
            // dataGridTextBoxColumn7
            // 
            this.dataGridTextBoxColumn7.Format = "";
            this.dataGridTextBoxColumn7.FormatInfo = null;
            this.dataGridTextBoxColumn7.HeaderText = "upper_var";
            this.dataGridTextBoxColumn7.Width = 80;
            // 
            // dataGridTextBoxColumn8
            // 
            this.dataGridTextBoxColumn8.Format = "";
            this.dataGridTextBoxColumn8.FormatInfo = null;
            this.dataGridTextBoxColumn8.HeaderText = "lower_var";
            this.dataGridTextBoxColumn8.Width = 80;
            // 
            // dataGridTextBoxColumn9
            // 
            this.dataGridTextBoxColumn9.Format = "";
            this.dataGridTextBoxColumn9.FormatInfo = null;
            this.dataGridTextBoxColumn9.HeaderText = "weight";
            this.dataGridTextBoxColumn9.Width = 80;
            // 
            // dataGridTextBoxColumn10
            // 
            this.dataGridTextBoxColumn10.Format = "";
            this.dataGridTextBoxColumn10.FormatInfo = null;
            this.dataGridTextBoxColumn10.HeaderText = "pack_num";
            this.dataGridTextBoxColumn10.Width = 100;
            // 
            // ProdHistory
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Inherit;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(238)))), ((int)(((byte)(242)))), ((int)(((byte)(251)))));
            this.ClientSize = new System.Drawing.Size(640, 480);
            this.ControlBox = false;
            this.Controls.Add(this.btn_ret_run);
            this.Controls.Add(this.btn_ret_cfg);
            this.Controls.Add(this.btnClr);
            this.Controls.Add(this.lbl_summary);
            this.Controls.Add(this.lbl_prod);
            this.Controls.Add(this.lb_prod);
            this.Controls.Add(this.lbl_prodno);
            this.Controls.Add(this.lbl_oper);
            this.Controls.Add(this.lbl_endtime);
            this.Controls.Add(this.lbl_starttime);
            this.Controls.Add(this.lb_prodno);
            this.Controls.Add(this.lb_oper);
            this.Controls.Add(this.mc_endtime);
            this.Controls.Add(this.mc_starttime);
            this.Controls.Add(this.dataGridView1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "ProdHistory";
            this.Text = "ProdHistory";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataGrid dataGridView1;
        private System.Windows.Forms.MonthCalendar mc_starttime;
        private System.Windows.Forms.MonthCalendar mc_endtime;
        private System.Windows.Forms.ListBox lb_oper;
        private System.Windows.Forms.ListBox lb_prodno;
        private System.Windows.Forms.Label lbl_starttime;
        private System.Windows.Forms.Label lbl_endtime;
        private System.Windows.Forms.Label lbl_oper;
        private System.Windows.Forms.Label lbl_prodno;
        private System.Windows.Forms.Label lbl_prod;
        private System.Windows.Forms.ListBox lb_prod;
        private System.Windows.Forms.Label lbl_summary;
        private System.Windows.Forms.Button btnClr;
        private System.Windows.Forms.PictureBox btn_ret_cfg;
        private System.Windows.Forms.PictureBox btn_ret_run;
        private System.Windows.Forms.DataGridTableStyle dataGridTableStyle1;
        private System.Windows.Forms.DataGridTextBoxColumn dataGridTextBoxColumn1;
        private System.Windows.Forms.DataGridTextBoxColumn dataGridTextBoxColumn2;
        private System.Windows.Forms.DataGridTextBoxColumn dataGridTextBoxColumn3;
        private System.Windows.Forms.DataGridTextBoxColumn dataGridTextBoxColumn4;
        private System.Windows.Forms.DataGridTextBoxColumn dataGridTextBoxColumn5;
        private System.Windows.Forms.DataGridTextBoxColumn dataGridTextBoxColumn6;
        private System.Windows.Forms.DataGridTextBoxColumn dataGridTextBoxColumn7;
        private System.Windows.Forms.DataGridTextBoxColumn dataGridTextBoxColumn8;
        private System.Windows.Forms.DataGridTextBoxColumn dataGridTextBoxColumn9;
        private System.Windows.Forms.DataGridTextBoxColumn dataGridTextBoxColumn10;
    }
}
