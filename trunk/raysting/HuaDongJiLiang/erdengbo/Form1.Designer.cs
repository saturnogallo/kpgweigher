namespace Jmbo
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
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
            this.components = new System.ComponentModel.Container();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.NewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.SaveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.OpenToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.ReportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.生成检定报告ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.结果通知书ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.校准证书ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.检定证书ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.生成分度表ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.AboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.zedallprobes = new ZedGraph.ZedGraphControl();
            this.panel1 = new System.Windows.Forms.Panel();
            this.lbl_status = new System.Windows.Forms.Label();
            this.btn_start = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.cb_probetype = new System.Windows.Forms.ComboBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.webBrowser1 = new System.Windows.Forms.WebBrowser();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.menuStrip1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.NewToolStripMenuItem,
            this.SaveToolStripMenuItem,
            this.OpenToolStripMenuItem,
            this.toolStripMenuItem1,
            this.ReportToolStripMenuItem,
            this.生成检定报告ToolStripMenuItem,
            this.生成分度表ToolStripMenuItem,
            this.toolStripMenuItem2,
            this.AboutToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1016, 27);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // NewToolStripMenuItem
            // 
            this.NewToolStripMenuItem.Name = "NewToolStripMenuItem";
            this.NewToolStripMenuItem.Size = new System.Drawing.Size(73, 23);
            this.NewToolStripMenuItem.Text = "新建测量";
            this.NewToolStripMenuItem.Click += new System.EventHandler(this.NewToolStripMenuItem_Click);
            // 
            // SaveToolStripMenuItem
            // 
            this.SaveToolStripMenuItem.Name = "SaveToolStripMenuItem";
            this.SaveToolStripMenuItem.Size = new System.Drawing.Size(73, 23);
            this.SaveToolStripMenuItem.Text = "保存结果";
            this.SaveToolStripMenuItem.Click += new System.EventHandler(this.SaveToolStripMenuItem_Click);
            // 
            // OpenToolStripMenuItem
            // 
            this.OpenToolStripMenuItem.Name = "OpenToolStripMenuItem";
            this.OpenToolStripMenuItem.Size = new System.Drawing.Size(99, 23);
            this.OpenToolStripMenuItem.Text = "打开历史数据";
            this.OpenToolStripMenuItem.Click += new System.EventHandler(this.OpenToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(25, 23);
            this.toolStripMenuItem1.Text = "|";
            // 
            // ReportToolStripMenuItem
            // 
            this.ReportToolStripMenuItem.Name = "ReportToolStripMenuItem";
            this.ReportToolStripMenuItem.Size = new System.Drawing.Size(112, 23);
            this.ReportToolStripMenuItem.Text = "生成原始记录单";
            this.ReportToolStripMenuItem.Click += new System.EventHandler(this.ReportToolStripMenuItem_Click);
            // 
            // 生成检定报告ToolStripMenuItem
            // 
            this.生成检定报告ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.结果通知书ToolStripMenuItem,
            this.校准证书ToolStripMenuItem,
            this.检定证书ToolStripMenuItem});
            this.生成检定报告ToolStripMenuItem.Name = "生成检定报告ToolStripMenuItem";
            this.生成检定报告ToolStripMenuItem.Size = new System.Drawing.Size(73, 23);
            this.生成检定报告ToolStripMenuItem.Text = "生成证书";
            // 
            // 结果通知书ToolStripMenuItem
            // 
            this.结果通知书ToolStripMenuItem.Name = "结果通知书ToolStripMenuItem";
            this.结果通知书ToolStripMenuItem.Size = new System.Drawing.Size(152, 24);
            this.结果通知书ToolStripMenuItem.Text = "结果通知书";
            this.结果通知书ToolStripMenuItem.Click += new System.EventHandler(this.结果通知书ToolStripMenuItem_Click);
            // 
            // 校准证书ToolStripMenuItem
            // 
            this.校准证书ToolStripMenuItem.Name = "校准证书ToolStripMenuItem";
            this.校准证书ToolStripMenuItem.Size = new System.Drawing.Size(152, 24);
            this.校准证书ToolStripMenuItem.Text = "校准证书";
            this.校准证书ToolStripMenuItem.Click += new System.EventHandler(this.校准证书ToolStripMenuItem_Click);
            // 
            // 检定证书ToolStripMenuItem
            // 
            this.检定证书ToolStripMenuItem.Name = "检定证书ToolStripMenuItem";
            this.检定证书ToolStripMenuItem.Size = new System.Drawing.Size(152, 24);
            this.检定证书ToolStripMenuItem.Text = "检定证书";
            this.检定证书ToolStripMenuItem.Click += new System.EventHandler(this.检定证书ToolStripMenuItem_Click);
            // 
            // 生成分度表ToolStripMenuItem
            // 
            this.生成分度表ToolStripMenuItem.Name = "生成分度表ToolStripMenuItem";
            this.生成分度表ToolStripMenuItem.Size = new System.Drawing.Size(86, 23);
            this.生成分度表ToolStripMenuItem.Text = "生成分度表";
            this.生成分度表ToolStripMenuItem.Click += new System.EventHandler(this.生成分度表ToolStripMenuItem_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(25, 23);
            this.toolStripMenuItem2.Text = "|";
            // 
            // AboutToolStripMenuItem
            // 
            this.AboutToolStripMenuItem.Name = "AboutToolStripMenuItem";
            this.AboutToolStripMenuItem.Size = new System.Drawing.Size(47, 23);
            this.AboutToolStripMenuItem.Text = "关于";
            this.AboutToolStripMenuItem.Click += new System.EventHandler(this.AboutToolStripMenuItem_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 27);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1016, 707);
            this.tabControl1.TabIndex = 1;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.zedallprobes);
            this.tabPage1.Controls.Add(this.panel1);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(1008, 681);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "各探头阻值";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // zedallprobes
            // 
            this.zedallprobes.Dock = System.Windows.Forms.DockStyle.Fill;
            this.zedallprobes.Location = new System.Drawing.Point(3, 43);
            this.zedallprobes.Name = "zedallprobes";
            this.zedallprobes.ScrollGrace = 0;
            this.zedallprobes.ScrollMaxX = 0;
            this.zedallprobes.ScrollMaxY = 0;
            this.zedallprobes.ScrollMaxY2 = 0;
            this.zedallprobes.ScrollMinX = 0;
            this.zedallprobes.ScrollMinY = 0;
            this.zedallprobes.ScrollMinY2 = 0;
            this.zedallprobes.Size = new System.Drawing.Size(1002, 635);
            this.zedallprobes.TabIndex = 1;
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.LightGray;
            this.panel1.Controls.Add(this.lbl_status);
            this.panel1.Controls.Add(this.btn_start);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.cb_probetype);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(3, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1002, 40);
            this.panel1.TabIndex = 0;
            // 
            // lbl_status
            // 
            this.lbl_status.AutoSize = true;
            this.lbl_status.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_status.ForeColor = System.Drawing.Color.Blue;
            this.lbl_status.Location = new System.Drawing.Point(534, 10);
            this.lbl_status.Name = "lbl_status";
            this.lbl_status.Size = new System.Drawing.Size(258, 17);
            this.lbl_status.TabIndex = 3;
            this.lbl_status.Text = "请在原始记录页填入信息后,开始测试.";
            // 
            // btn_start
            // 
            this.btn_start.FlatAppearance.MouseOverBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(192)))), ((int)(((byte)(0)))));
            this.btn_start.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_start.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btn_start.Location = new System.Drawing.Point(311, 5);
            this.btn_start.Name = "btn_start";
            this.btn_start.Size = new System.Drawing.Size(118, 27);
            this.btn_start.TabIndex = 2;
            this.btn_start.Text = "开始测量";
            this.btn_start.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.btn_start.UseVisualStyleBackColor = true;
            this.btn_start.Click += new System.EventHandler(this.button_Start_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(5, 11);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(103, 17);
            this.label2.TabIndex = 1;
            this.label2.Text = "被检探头类型:";
            // 
            // cb_probetype
            // 
            this.cb_probetype.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cb_probetype.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cb_probetype.FormattingEnabled = true;
            this.cb_probetype.Items.AddRange(new object[] {
            "Pt100_四线",
            "Pt100_三线",
            "Pt25_四线",
            "Pt25_三线"});
            this.cb_probetype.Location = new System.Drawing.Point(129, 7);
            this.cb_probetype.MaxDropDownItems = 10;
            this.cb_probetype.Name = "cb_probetype";
            this.cb_probetype.Size = new System.Drawing.Size(148, 25);
            this.cb_probetype.TabIndex = 0;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.webBrowser1);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(1008, 681);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "原始记录";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // webBrowser1
            // 
            this.webBrowser1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.webBrowser1.Location = new System.Drawing.Point(3, 3);
            this.webBrowser1.MinimumSize = new System.Drawing.Size(20, 20);
            this.webBrowser1.Name = "webBrowser1";
            this.webBrowser1.Size = new System.Drawing.Size(1002, 675);
            this.webBrowser1.TabIndex = 0;
            this.webBrowser1.Url = new System.Uri("file://c:\\blank.htm", System.UriKind.Absolute);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1016, 734);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "二等铂电阻温度计标准装置自动测量系统 - 上海市计量测试技术研究院(SIMT)";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem NewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem SaveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem OpenToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ReportToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem AboutToolStripMenuItem;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.WebBrowser webBrowser1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button btn_start;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.Label lbl_status;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox cb_probetype;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem 生成检定报告ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem2;
        private ZedGraph.ZedGraphControl zedallprobes;
        private System.Windows.Forms.ToolStripMenuItem 生成分度表ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 结果通知书ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 校准证书ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 检定证书ToolStripMenuItem;
    }
}

