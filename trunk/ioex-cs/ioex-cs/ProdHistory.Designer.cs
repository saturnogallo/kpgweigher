namespace ioex_cs
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
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
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
            this.btnRet = new System.Windows.Forms.Button();
            this.btnClr = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            this.dataGridView1.AllowUserToOrderColumns = true;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(9, 190);
            this.dataGridView1.Margin = new System.Windows.Forms.Padding(2);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.RowTemplate.Height = 24;
            this.dataGridView1.Size = new System.Drawing.Size(1004, 486);
            this.dataGridView1.TabIndex = 2;
            // 
            // mc_starttime
            // 
            this.mc_starttime.Location = new System.Drawing.Point(14, 29);
            this.mc_starttime.Margin = new System.Windows.Forms.Padding(7);
            this.mc_starttime.Name = "mc_starttime";
            this.mc_starttime.TabIndex = 3;
            // 
            // mc_endtime
            // 
            this.mc_endtime.Location = new System.Drawing.Point(297, 29);
            this.mc_endtime.Margin = new System.Windows.Forms.Padding(7);
            this.mc_endtime.Name = "mc_endtime";
            this.mc_endtime.TabIndex = 4;
            // 
            // lb_oper
            // 
            this.lb_oper.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lb_oper.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lb_oper.FormattingEnabled = true;
            this.lb_oper.ItemHeight = 20;
            this.lb_oper.Location = new System.Drawing.Point(573, 25);
            this.lb_oper.Margin = new System.Windows.Forms.Padding(2);
            this.lb_oper.Name = "lb_oper";
            this.lb_oper.Size = new System.Drawing.Size(125, 162);
            this.lb_oper.TabIndex = 5;
            // 
            // lb_prodno
            // 
            this.lb_prodno.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lb_prodno.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lb_prodno.FormattingEnabled = true;
            this.lb_prodno.ItemHeight = 20;
            this.lb_prodno.Location = new System.Drawing.Point(715, 25);
            this.lb_prodno.Margin = new System.Windows.Forms.Padding(2);
            this.lb_prodno.Name = "lb_prodno";
            this.lb_prodno.Size = new System.Drawing.Size(126, 162);
            this.lb_prodno.TabIndex = 5;
            // 
            // lbl_starttime
            // 
            this.lbl_starttime.AutoSize = true;
            this.lbl_starttime.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_starttime.Location = new System.Drawing.Point(14, 3);
            this.lbl_starttime.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lbl_starttime.Name = "lbl_starttime";
            this.lbl_starttime.Size = new System.Drawing.Size(51, 20);
            this.lbl_starttime.TabIndex = 6;
            this.lbl_starttime.Text = "label1";
            // 
            // lbl_endtime
            // 
            this.lbl_endtime.AutoSize = true;
            this.lbl_endtime.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_endtime.Location = new System.Drawing.Point(294, 2);
            this.lbl_endtime.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lbl_endtime.Name = "lbl_endtime";
            this.lbl_endtime.Size = new System.Drawing.Size(51, 20);
            this.lbl_endtime.TabIndex = 6;
            this.lbl_endtime.Text = "label1";
            // 
            // lbl_oper
            // 
            this.lbl_oper.AutoSize = true;
            this.lbl_oper.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_oper.Location = new System.Drawing.Point(570, 2);
            this.lbl_oper.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lbl_oper.Name = "lbl_oper";
            this.lbl_oper.Size = new System.Drawing.Size(51, 20);
            this.lbl_oper.TabIndex = 7;
            this.lbl_oper.Text = "label1";
            // 
            // lbl_prodno
            // 
            this.lbl_prodno.AutoSize = true;
            this.lbl_prodno.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_prodno.Location = new System.Drawing.Point(713, 2);
            this.lbl_prodno.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lbl_prodno.Name = "lbl_prodno";
            this.lbl_prodno.Size = new System.Drawing.Size(83, 20);
            this.lbl_prodno.TabIndex = 8;
            this.lbl_prodno.Text = "lbl_prodno";
            // 
            // lbl_prod
            // 
            this.lbl_prod.AutoSize = true;
            this.lbl_prod.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_prod.Location = new System.Drawing.Point(854, 1);
            this.lbl_prod.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lbl_prod.Name = "lbl_prod";
            this.lbl_prod.Size = new System.Drawing.Size(65, 20);
            this.lbl_prod.TabIndex = 10;
            this.lbl_prod.Text = "lbl_prod";
            // 
            // lb_prod
            // 
            this.lb_prod.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lb_prod.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lb_prod.FormattingEnabled = true;
            this.lb_prod.ItemHeight = 20;
            this.lb_prod.Location = new System.Drawing.Point(856, 24);
            this.lb_prod.Margin = new System.Windows.Forms.Padding(2);
            this.lb_prod.Name = "lb_prod";
            this.lb_prod.Size = new System.Drawing.Size(134, 162);
            this.lb_prod.TabIndex = 9;
            // 
            // lbl_summary
            // 
            this.lbl_summary.AutoSize = true;
            this.lbl_summary.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_summary.ForeColor = System.Drawing.Color.DarkGoldenrod;
            this.lbl_summary.Location = new System.Drawing.Point(166, 705);
            this.lbl_summary.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lbl_summary.Name = "lbl_summary";
            this.lbl_summary.Size = new System.Drawing.Size(0, 24);
            this.lbl_summary.TabIndex = 11;
            // 
            // btnRet
            // 
            this.btnRet.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnRet.Location = new System.Drawing.Point(885, 705);
            this.btnRet.Margin = new System.Windows.Forms.Padding(2);
            this.btnRet.Name = "btnRet";
            this.btnRet.Size = new System.Drawing.Size(128, 52);
            this.btnRet.TabIndex = 12;
            this.btnRet.Text = "button1";
            this.btnRet.UseVisualStyleBackColor = true;
            this.btnRet.Click += new System.EventHandler(this.btnRet_Click);
            // 
            // btnClr
            // 
            this.btnClr.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnClr.Location = new System.Drawing.Point(9, 705);
            this.btnClr.Margin = new System.Windows.Forms.Padding(2);
            this.btnClr.Name = "btnClr";
            this.btnClr.Size = new System.Drawing.Size(202, 52);
            this.btnClr.TabIndex = 13;
            this.btnClr.Text = "button1";
            this.btnClr.UseVisualStyleBackColor = true;
            this.btnClr.Click += new System.EventHandler(this.btnClr_Click);
            // 
            // ProdHistory
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.LightGray;
            this.ClientSize = new System.Drawing.Size(1024, 768);
            this.Controls.Add(this.btnClr);
            this.Controls.Add(this.btnRet);
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
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "ProdHistory";
            this.Text = "ProdHistory";
            this.TopMost = true;
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGridView1;
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
        private System.Windows.Forms.Button btnRet;
        private System.Windows.Forms.Button btnClr;
    }
}