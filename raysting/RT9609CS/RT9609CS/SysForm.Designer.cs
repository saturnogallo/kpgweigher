namespace RT9609CS
{
    partial class SysForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.MainMenu mainMenu1;

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
            this.mainMenu1 = new System.Windows.Forms.MainMenu();
            this.btQuit = new System.Windows.Forms.Button();
            this.btSave = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.cbSpeed = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tbThresh = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.tbResponse = new System.Windows.Forms.TextBox();
            this.tbIntegral = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.tbInterval = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.cbAverage = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.tbTemp = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.ckScanner = new System.Windows.Forms.CheckBox();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btQuit
            // 
            this.btQuit.Location = new System.Drawing.Point(319, 236);
            this.btQuit.Name = "btQuit";
            this.btQuit.Size = new System.Drawing.Size(94, 29);
            this.btQuit.TabIndex = 6;
            this.btQuit.Text = "放弃退出";
            // 
            // btSave
            // 
            this.btSave.Location = new System.Drawing.Point(151, 236);
            this.btSave.Name = "btSave";
            this.btSave.Size = new System.Drawing.Size(94, 29);
            this.btSave.TabIndex = 5;
            this.btSave.Text = "保存修改";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.ckScanner);
            this.panel1.Controls.Add(this.tbTemp);
            this.panel1.Controls.Add(this.label8);
            this.panel1.Controls.Add(this.cbAverage);
            this.panel1.Controls.Add(this.label7);
            this.panel1.Controls.Add(this.tbInterval);
            this.panel1.Controls.Add(this.label6);
            this.panel1.Controls.Add(this.tbIntegral);
            this.panel1.Controls.Add(this.label4);
            this.panel1.Controls.Add(this.tbResponse);
            this.panel1.Controls.Add(this.tbThresh);
            this.panel1.Controls.Add(this.cbSpeed);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.label5);
            this.panel1.Location = new System.Drawing.Point(21, 28);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(518, 199);
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(21, 4);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(175, 21);
            this.label1.Text = "系统参数设置";
            // 
            // cbSpeed
            // 
            this.cbSpeed.Items.Add("快速");
            this.cbSpeed.Items.Add("慢速");
            this.cbSpeed.Location = new System.Drawing.Point(359, 58);
            this.cbSpeed.Name = "cbSpeed";
            this.cbSpeed.Size = new System.Drawing.Size(127, 22);
            this.cbSpeed.TabIndex = 16;
            // 
            // label5
            // 
            this.label5.Location = new System.Drawing.Point(289, 58);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(63, 20);
            this.label5.Text = "测量速度";
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(25, 26);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(63, 20);
            this.label2.Text = "阈值";
            // 
            // tbThresh
            // 
            this.tbThresh.Location = new System.Drawing.Point(130, 23);
            this.tbThresh.Name = "tbThresh";
            this.tbThresh.Size = new System.Drawing.Size(100, 21);
            this.tbThresh.TabIndex = 19;
            this.tbThresh.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // label3
            // 
            this.label3.Location = new System.Drawing.Point(25, 89);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(63, 20);
            this.label3.Text = "响应时间";
            // 
            // tbResponse
            // 
            this.tbResponse.Location = new System.Drawing.Point(130, 85);
            this.tbResponse.Name = "tbResponse";
            this.tbResponse.Size = new System.Drawing.Size(100, 21);
            this.tbResponse.TabIndex = 19;
            // 
            // tbIntegral
            // 
            this.tbIntegral.Location = new System.Drawing.Point(130, 116);
            this.tbIntegral.Name = "tbIntegral";
            this.tbIntegral.Size = new System.Drawing.Size(100, 21);
            this.tbIntegral.TabIndex = 22;
            // 
            // label4
            // 
            this.label4.Location = new System.Drawing.Point(25, 119);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(63, 20);
            this.label4.Text = "积分时间";
            // 
            // tbInterval
            // 
            this.tbInterval.Location = new System.Drawing.Point(130, 54);
            this.tbInterval.Name = "tbInterval";
            this.tbInterval.Size = new System.Drawing.Size(100, 21);
            this.tbInterval.TabIndex = 25;
            // 
            // label6
            // 
            this.label6.Location = new System.Drawing.Point(25, 58);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(63, 20);
            this.label6.Text = "采样间隔";
            // 
            // cbAverage
            // 
            this.cbAverage.Items.Add("移动平均");
            this.cbAverage.Items.Add("指数平均");
            this.cbAverage.Location = new System.Drawing.Point(359, 22);
            this.cbAverage.Name = "cbAverage";
            this.cbAverage.Size = new System.Drawing.Size(127, 22);
            this.cbAverage.TabIndex = 28;
            // 
            // label7
            // 
            this.label7.Location = new System.Drawing.Point(289, 24);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(63, 20);
            this.label7.Text = "平均算法";
            // 
            // tbTemp
            // 
            this.tbTemp.Location = new System.Drawing.Point(130, 147);
            this.tbTemp.Name = "tbTemp";
            this.tbTemp.Size = new System.Drawing.Size(100, 21);
            this.tbTemp.TabIndex = 31;
            // 
            // label8
            // 
            this.label8.Location = new System.Drawing.Point(25, 149);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(63, 20);
            this.label8.Text = "当前温度";
            // 
            // ckScanner
            // 
            this.ckScanner.Location = new System.Drawing.Point(289, 145);
            this.ckScanner.Name = "ckScanner";
            this.ckScanner.Size = new System.Drawing.Size(188, 23);
            this.ckScanner.TabIndex = 33;
            this.ckScanner.Text = "使用扫描器(12位)";
            // 
            // SysForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(567, 268);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.btQuit);
            this.Controls.Add(this.btSave);
            this.Menu = this.mainMenu1;
            this.Name = "SysForm";
            this.Text = "系统设置";
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btQuit;
        private System.Windows.Forms.Button btSave;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cbSpeed;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox tbTemp;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.ComboBox cbAverage;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox tbInterval;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox tbIntegral;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox tbResponse;
        private System.Windows.Forms.TextBox tbThresh;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.CheckBox ckScanner;
    }
}