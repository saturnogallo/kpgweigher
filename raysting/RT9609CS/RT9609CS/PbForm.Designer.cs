namespace RT9609CS
{
    partial class PbForm
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
            this.lbProbe = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btNewProbe = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.btCoef = new System.Windows.Forms.Button();
            this.lblCoef = new System.Windows.Forms.Label();
            this.cbMethod = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.ck3Wire = new System.Windows.Forms.CheckBox();
            this.ckSqr = new System.Windows.Forms.CheckBox();
            this.cbCurr = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.cbStd = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.tbSerial = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.btSave = new System.Windows.Forms.Button();
            this.btQuit = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // lbProbe
            // 
            this.lbProbe.Location = new System.Drawing.Point(3, 27);
            this.lbProbe.Name = "lbProbe";
            this.lbProbe.Size = new System.Drawing.Size(94, 212);
            this.lbProbe.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(3, 7);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(71, 17);
            this.label1.Text = "探头选择";
            // 
            // btNewProbe
            // 
            this.btNewProbe.Location = new System.Drawing.Point(3, 245);
            this.btNewProbe.Name = "btNewProbe";
            this.btNewProbe.Size = new System.Drawing.Size(94, 29);
            this.btNewProbe.TabIndex = 2;
            this.btNewProbe.Text = "增加探头";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.btCoef);
            this.panel1.Controls.Add(this.lblCoef);
            this.panel1.Controls.Add(this.cbMethod);
            this.panel1.Controls.Add(this.label5);
            this.panel1.Controls.Add(this.ck3Wire);
            this.panel1.Controls.Add(this.ckSqr);
            this.panel1.Controls.Add(this.cbCurr);
            this.panel1.Controls.Add(this.label4);
            this.panel1.Controls.Add(this.cbStd);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.tbSerial);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Location = new System.Drawing.Point(132, 12);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(407, 227);
            this.panel1.GotFocus += new System.EventHandler(this.panel1_GotFocus);
            // 
            // btCoef
            // 
            this.btCoef.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.btCoef.Location = new System.Drawing.Point(15, 179);
            this.btCoef.Name = "btCoef";
            this.btCoef.Size = new System.Drawing.Size(65, 20);
            this.btCoef.TabIndex = 4;
            this.btCoef.Text = "探头系数..";
            // 
            // lblCoef
            // 
            this.lblCoef.Location = new System.Drawing.Point(86, 179);
            this.lblCoef.Name = "lblCoef";
            this.lblCoef.Size = new System.Drawing.Size(299, 20);
            this.lblCoef.Text = "探头系数";
            // 
            // cbMethod
            // 
            this.cbMethod.Location = new System.Drawing.Point(85, 137);
            this.cbMethod.Name = "cbMethod";
            this.cbMethod.Size = new System.Drawing.Size(127, 22);
            this.cbMethod.TabIndex = 14;
            // 
            // label5
            // 
            this.label5.Location = new System.Drawing.Point(15, 139);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(63, 20);
            this.label5.Text = "转换方法";
            // 
            // ck3Wire
            // 
            this.ck3Wire.Location = new System.Drawing.Point(268, 59);
            this.ck3Wire.Name = "ck3Wire";
            this.ck3Wire.Size = new System.Drawing.Size(86, 19);
            this.ck3Wire.TabIndex = 11;
            this.ck3Wire.Text = "三线补偿";
            // 
            // ckSqr
            // 
            this.ckSqr.Location = new System.Drawing.Point(268, 98);
            this.ckSqr.Name = "ckSqr";
            this.ckSqr.Size = new System.Drawing.Size(86, 19);
            this.ckSqr.TabIndex = 10;
            this.ckSqr.Text = "倍功率";
            // 
            // cbCurr
            // 
            this.cbCurr.Location = new System.Drawing.Point(85, 97);
            this.cbCurr.Name = "cbCurr";
            this.cbCurr.Size = new System.Drawing.Size(127, 22);
            this.cbCurr.TabIndex = 7;
            // 
            // label4
            // 
            this.label4.Location = new System.Drawing.Point(15, 97);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(63, 20);
            this.label4.Text = "参考电流";
            // 
            // cbStd
            // 
            this.cbStd.Location = new System.Drawing.Point(84, 56);
            this.cbStd.Name = "cbStd";
            this.cbStd.Size = new System.Drawing.Size(127, 22);
            this.cbStd.TabIndex = 4;
            // 
            // label3
            // 
            this.label3.Location = new System.Drawing.Point(15, 58);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(63, 20);
            this.label3.Text = "标准电阻";
            // 
            // tbSerial
            // 
            this.tbSerial.Location = new System.Drawing.Point(84, 19);
            this.tbSerial.Name = "tbSerial";
            this.tbSerial.Size = new System.Drawing.Size(128, 21);
            this.tbSerial.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(15, 20);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(63, 20);
            this.label2.Text = "序列号";
            // 
            // btSave
            // 
            this.btSave.Location = new System.Drawing.Point(203, 245);
            this.btSave.Name = "btSave";
            this.btSave.Size = new System.Drawing.Size(94, 29);
            this.btSave.TabIndex = 4;
            this.btSave.Text = "保存修改";
            // 
            // btQuit
            // 
            this.btQuit.Location = new System.Drawing.Point(371, 245);
            this.btQuit.Name = "btQuit";
            this.btQuit.Size = new System.Drawing.Size(94, 29);
            this.btQuit.TabIndex = 4;
            this.btQuit.Text = "放弃退出";
            // 
            // PbForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(577, 294);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.btQuit);
            this.Controls.Add(this.btNewProbe);
            this.Controls.Add(this.btSave);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.lbProbe);
            this.Name = "PbForm";
            this.Text = "铂电阻探头参数设置";
            this.TopMost = true;
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox lbProbe;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btNewProbe;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TextBox tbSerial;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label lblCoef;
        private System.Windows.Forms.ComboBox cbMethod;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.CheckBox ck3Wire;
        private System.Windows.Forms.CheckBox ckSqr;
        private System.Windows.Forms.ComboBox cbCurr;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox cbStd;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button btCoef;
        private System.Windows.Forms.Button btSave;
        private System.Windows.Forms.Button btQuit;
    }
}