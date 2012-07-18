namespace KCBTool3
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
            this.cb_com = new System.Windows.Forms.ComboBox();
            this.btn_open = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.btn_refresh = new System.Windows.Forms.Button();
            this.lb_addrfrom = new System.Windows.Forms.ListBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.btn_updateprg = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.lbl_fw = new System.Windows.Forms.Label();
            this.btn_choosefw = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.panel3 = new System.Windows.Forms.Panel();
            this.lb_addrto = new System.Windows.Forms.ListBox();
            this.btn_changeaddr = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.panel4 = new System.Windows.Forms.Panel();
            this.progressBar_fw = new System.Windows.Forms.ProgressBar();
            this.btn_updatefw = new System.Windows.Forms.Button();
            this.lbl_fwstatus = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.btn_updatepic = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.panel4.SuspendLayout();
            this.SuspendLayout();
            // 
            // cb_com
            // 
            this.cb_com.BackColor = System.Drawing.SystemColors.Control;
            this.cb_com.Items.Add("COM1");
            this.cb_com.Items.Add("COM2");
            this.cb_com.Items.Add("COM3");
            this.cb_com.Items.Add("COM4");
            this.cb_com.Items.Add("COM5");
            this.cb_com.Location = new System.Drawing.Point(40, 26);
            this.cb_com.Name = "cb_com";
            this.cb_com.Size = new System.Drawing.Size(121, 23);
            this.cb_com.TabIndex = 0;
            // 
            // btn_open
            // 
            this.btn_open.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Regular);
            this.btn_open.Location = new System.Drawing.Point(196, 26);
            this.btn_open.Name = "btn_open";
            this.btn_open.Size = new System.Drawing.Size(94, 25);
            this.btn_open.TabIndex = 1;
            this.btn_open.Text = "Open";
            this.btn_open.Click += new System.EventHandler(this.btn_open_Click);
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("Tahoma", 16F, System.Drawing.FontStyle.Regular);
            this.label1.Location = new System.Drawing.Point(40, 150);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(143, 34);
            this.label1.Text = "Choose Node";
            // 
            // btn_refresh
            // 
            this.btn_refresh.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Regular);
            this.btn_refresh.Location = new System.Drawing.Point(196, 150);
            this.btn_refresh.Name = "btn_refresh";
            this.btn_refresh.Size = new System.Drawing.Size(94, 25);
            this.btn_refresh.TabIndex = 1;
            this.btn_refresh.Text = "Refresh";
            this.btn_refresh.Click += new System.EventHandler(this.btn_refresh_Click);
            // 
            // lb_addrfrom
            // 
            this.lb_addrfrom.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Regular);
            this.lb_addrfrom.Location = new System.Drawing.Point(40, 231);
            this.lb_addrfrom.Name = "lb_addrfrom";
            this.lb_addrfrom.Size = new System.Drawing.Size(322, 458);
            this.lb_addrfrom.TabIndex = 3;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.btn_updatepic);
            this.panel1.Controls.Add(this.btn_updateprg);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Location = new System.Drawing.Point(389, 26);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(572, 102);
            // 
            // btn_updateprg
            // 
            this.btn_updateprg.Font = new System.Drawing.Font("Tahoma", 18F, System.Drawing.FontStyle.Regular);
            this.btn_updateprg.Location = new System.Drawing.Point(49, 24);
            this.btn_updateprg.Name = "btn_updateprg";
            this.btn_updateprg.Size = new System.Drawing.Size(227, 63);
            this.btn_updateprg.TabIndex = 1;
            this.btn_updateprg.Text = "Update Program";
            this.btn_updateprg.Click += new System.EventHandler(this.btn_updateprg_Click);
            // 
            // label2
            // 
            this.label2.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Regular);
            this.label2.Location = new System.Drawing.Point(3, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(131, 40);
            this.label2.Text = "Task 0";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.lbl_fw);
            this.panel2.Controls.Add(this.btn_choosefw);
            this.panel2.Controls.Add(this.label3);
            this.panel2.Location = new System.Drawing.Point(389, 150);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(572, 102);
            // 
            // lbl_fw
            // 
            this.lbl_fw.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Regular);
            this.lbl_fw.Location = new System.Drawing.Point(314, 46);
            this.lbl_fw.Name = "lbl_fw";
            this.lbl_fw.Size = new System.Drawing.Size(224, 35);
            this.lbl_fw.Text = "...";
            // 
            // btn_choosefw
            // 
            this.btn_choosefw.Font = new System.Drawing.Font("Tahoma", 18F, System.Drawing.FontStyle.Regular);
            this.btn_choosefw.Location = new System.Drawing.Point(49, 25);
            this.btn_choosefw.Name = "btn_choosefw";
            this.btn_choosefw.Size = new System.Drawing.Size(227, 63);
            this.btn_choosefw.TabIndex = 1;
            this.btn_choosefw.Text = "Choose Firmware";
            this.btn_choosefw.Click += new System.EventHandler(this.btn_choosefw_Click);
            // 
            // label3
            // 
            this.label3.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Regular);
            this.label3.Location = new System.Drawing.Point(3, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(131, 40);
            this.label3.Text = "Task 1";
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.lb_addrto);
            this.panel3.Controls.Add(this.btn_changeaddr);
            this.panel3.Controls.Add(this.label4);
            this.panel3.Location = new System.Drawing.Point(389, 270);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(572, 325);
            // 
            // lb_addrto
            // 
            this.lb_addrto.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Regular);
            this.lb_addrto.Items.Add("1");
            this.lb_addrto.Items.Add("2");
            this.lb_addrto.Items.Add("3");
            this.lb_addrto.Items.Add("4");
            this.lb_addrto.Items.Add("5");
            this.lb_addrto.Items.Add("6");
            this.lb_addrto.Items.Add("7");
            this.lb_addrto.Items.Add("8");
            this.lb_addrto.Items.Add("9");
            this.lb_addrto.Items.Add("10");
            this.lb_addrto.Items.Add("11");
            this.lb_addrto.Items.Add("12");
            this.lb_addrto.Items.Add("13");
            this.lb_addrto.Items.Add("14");
            this.lb_addrto.Items.Add("15");
            this.lb_addrto.Location = new System.Drawing.Point(336, 22);
            this.lb_addrto.Name = "lb_addrto";
            this.lb_addrto.Size = new System.Drawing.Size(218, 287);
            this.lb_addrto.TabIndex = 2;
            // 
            // btn_changeaddr
            // 
            this.btn_changeaddr.Font = new System.Drawing.Font("Tahoma", 18F, System.Drawing.FontStyle.Regular);
            this.btn_changeaddr.Location = new System.Drawing.Point(49, 70);
            this.btn_changeaddr.Name = "btn_changeaddr";
            this.btn_changeaddr.Size = new System.Drawing.Size(227, 63);
            this.btn_changeaddr.TabIndex = 1;
            this.btn_changeaddr.Text = "Change Address to";
            this.btn_changeaddr.Click += new System.EventHandler(this.btn_changeaddr_Click);
            // 
            // label4
            // 
            this.label4.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Regular);
            this.label4.Location = new System.Drawing.Point(3, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(131, 40);
            this.label4.Text = "Task 2";
            // 
            // panel4
            // 
            this.panel4.Controls.Add(this.progressBar_fw);
            this.panel4.Controls.Add(this.btn_updatefw);
            this.panel4.Controls.Add(this.lbl_fwstatus);
            this.panel4.Controls.Add(this.label5);
            this.panel4.Location = new System.Drawing.Point(389, 630);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(572, 102);
            // 
            // progressBar_fw
            // 
            this.progressBar_fw.Location = new System.Drawing.Point(300, 36);
            this.progressBar_fw.Name = "progressBar_fw";
            this.progressBar_fw.Size = new System.Drawing.Size(254, 33);
            // 
            // btn_updatefw
            // 
            this.btn_updatefw.Font = new System.Drawing.Font("Tahoma", 18F, System.Drawing.FontStyle.Regular);
            this.btn_updatefw.Location = new System.Drawing.Point(58, 21);
            this.btn_updatefw.Name = "btn_updatefw";
            this.btn_updatefw.Size = new System.Drawing.Size(227, 63);
            this.btn_updatefw.TabIndex = 1;
            this.btn_updatefw.Text = "Update Firmware";
            this.btn_updatefw.Click += new System.EventHandler(this.btn_updatefw_Click);
            // 
            // lbl_fwstatus
            // 
            this.lbl_fwstatus.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Regular);
            this.lbl_fwstatus.Location = new System.Drawing.Point(58, 35);
            this.lbl_fwstatus.Name = "lbl_fwstatus";
            this.lbl_fwstatus.Size = new System.Drawing.Size(207, 49);
            this.lbl_fwstatus.Text = "lbl_fwstatus";
            // 
            // label5
            // 
            this.label5.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Regular);
            this.label5.Location = new System.Drawing.Point(3, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(131, 40);
            this.label5.Text = "Task 3";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // btn_updatepic
            // 
            this.btn_updatepic.Font = new System.Drawing.Font("Tahoma", 18F, System.Drawing.FontStyle.Regular);
            this.btn_updatepic.Location = new System.Drawing.Point(314, 24);
            this.btn_updatepic.Name = "btn_updatepic";
            this.btn_updatepic.Size = new System.Drawing.Size(227, 63);
            this.btn_updatepic.TabIndex = 1;
            this.btn_updatepic.Text = "Update Pictures";
            this.btn_updatepic.Click += new System.EventHandler(this.btn_updatepic_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(1024, 768);
            this.ControlBox = false;
            this.Controls.Add(this.panel4);
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.lb_addrfrom);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btn_refresh);
            this.Controls.Add(this.btn_open);
            this.Controls.Add(this.cb_com);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "Form1";
            this.Text = "Form1";
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel3.ResumeLayout(false);
            this.panel4.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ComboBox cb_com;
        private System.Windows.Forms.Button btn_open;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btn_refresh;
        private System.Windows.Forms.ListBox lb_addrfrom;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button btn_updateprg;
        private System.Windows.Forms.Button btn_choosefw;
        private System.Windows.Forms.Button btn_changeaddr;
        private System.Windows.Forms.Button btn_updatefw;
        private System.Windows.Forms.ListBox lb_addrto;
        private System.Windows.Forms.ProgressBar progressBar_fw;
        private System.Windows.Forms.Label lbl_fwstatus;
        private System.Windows.Forms.Label lbl_fw;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button btn_updatepic;
    }
}

