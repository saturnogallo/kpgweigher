namespace RT9609CS
{
    partial class CoefForm
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.radioButton3 = new System.Windows.Forms.RadioButton();
            this.radioButton4 = new System.Windows.Forms.RadioButton();
            this.radioButton5 = new System.Windows.Forms.RadioButton();
            this.radioButton6 = new System.Windows.Forms.RadioButton();
            this.radioButton7 = new System.Windows.Forms.RadioButton();
            this.panel2 = new System.Windows.Forms.Panel();
            this.label2 = new System.Windows.Forms.Label();
            this.lblRtp = new System.Windows.Forms.Label();
            this.tbRtp = new System.Windows.Forms.TextBox();
            this.tbWal = new System.Windows.Forms.TextBox();
            this.lblWal = new System.Windows.Forms.Label();
            this.tbWzn = new System.Windows.Forms.TextBox();
            this.lblWzn = new System.Windows.Forms.Label();
            this.tbWsn = new System.Windows.Forms.TextBox();
            this.lblWsn = new System.Windows.Forms.Label();
            this.tbWin = new System.Windows.Forms.TextBox();
            this.lblWin = new System.Windows.Forms.Label();
            this.tbWga = new System.Windows.Forms.TextBox();
            this.lblWga = new System.Windows.Forms.Label();
            this.tbWhg = new System.Windows.Forms.TextBox();
            this.lblWhg = new System.Windows.Forms.Label();
            this.tbWar = new System.Windows.Forms.TextBox();
            this.lblWar = new System.Windows.Forms.Label();
            this.btQuit = new System.Windows.Forms.Button();
            this.btSave = new System.Windows.Forms.Button();
            this.panel3 = new System.Windows.Forms.Panel();
            this.label11 = new System.Windows.Forms.Label();
            this.lblCoefA = new System.Windows.Forms.Label();
            this.lblCoefB = new System.Windows.Forms.Label();
            this.lblCoefC = new System.Windows.Forms.Label();
            this.tbCoefA = new System.Windows.Forms.TextBox();
            this.tbCoefB = new System.Windows.Forms.TextBox();
            this.tbCoefC = new System.Windows.Forms.TextBox();
            this.btConvert = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.radioButton2);
            this.panel1.Controls.Add(this.radioButton6);
            this.panel1.Controls.Add(this.radioButton7);
            this.panel1.Controls.Add(this.radioButton5);
            this.panel1.Controls.Add(this.radioButton4);
            this.panel1.Controls.Add(this.radioButton3);
            this.panel1.Controls.Add(this.radioButton1);
            this.panel1.Location = new System.Drawing.Point(24, 18);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(266, 113);
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Location = new System.Drawing.Point(24, 1);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(64, 15);
            this.label1.Text = "温区";
            // 
            // radioButton1
            // 
            this.radioButton1.Location = new System.Drawing.Point(3, 1);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(113, 31);
            this.radioButton1.TabIndex = 0;
            this.radioButton1.Text = "0～660.323℃";
            this.radioButton1.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
            // 
            // radioButton2
            // 
            this.radioButton2.Location = new System.Drawing.Point(133, 2);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(122, 30);
            this.radioButton2.TabIndex = 0;
            this.radioButton2.Text = "0～419.527   ℃";
            this.radioButton2.CheckedChanged += new System.EventHandler(this.radioButton2_CheckedChanged);
            // 
            // radioButton3
            // 
            this.radioButton3.Location = new System.Drawing.Point(3, 31);
            this.radioButton3.Name = "radioButton3";
            this.radioButton3.Size = new System.Drawing.Size(124, 26);
            this.radioButton3.TabIndex = 0;
            this.radioButton3.Text = "0～231.928℃";
            this.radioButton3.CheckedChanged += new System.EventHandler(this.radioButton3_CheckedChanged);
            // 
            // radioButton4
            // 
            this.radioButton4.Location = new System.Drawing.Point(133, 31);
            this.radioButton4.Name = "radioButton4";
            this.radioButton4.Size = new System.Drawing.Size(125, 26);
            this.radioButton4.TabIndex = 0;
            this.radioButton4.Text = "0～156.5985 ℃";
            this.radioButton4.CheckedChanged += new System.EventHandler(this.radioButton4_CheckedChanged);
            // 
            // radioButton5
            // 
            this.radioButton5.Location = new System.Drawing.Point(3, 57);
            this.radioButton5.Name = "radioButton5";
            this.radioButton5.Size = new System.Drawing.Size(124, 24);
            this.radioButton5.TabIndex = 0;
            this.radioButton5.Text = "0～29.7646℃";
            this.radioButton5.CheckedChanged += new System.EventHandler(this.radioButton5_CheckedChanged);
            // 
            // radioButton6
            // 
            this.radioButton6.Location = new System.Drawing.Point(133, 57);
            this.radioButton6.Name = "radioButton6";
            this.radioButton6.Size = new System.Drawing.Size(122, 24);
            this.radioButton6.TabIndex = 0;
            this.radioButton6.Text = "0～-189.8344℃";
            this.radioButton6.CheckedChanged += new System.EventHandler(this.radioButton6_CheckedChanged);
            // 
            // radioButton7
            // 
            this.radioButton7.Location = new System.Drawing.Point(3, 82);
            this.radioButton7.Name = "radioButton7";
            this.radioButton7.Size = new System.Drawing.Size(157, 24);
            this.radioButton7.TabIndex = 0;
            this.radioButton7.Text = "-38.8344～29.7646℃";
            this.radioButton7.CheckedChanged += new System.EventHandler(this.radioButton7_CheckedChanged);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.tbWar);
            this.panel2.Controls.Add(this.lblWar);
            this.panel2.Controls.Add(this.tbWhg);
            this.panel2.Controls.Add(this.lblWhg);
            this.panel2.Controls.Add(this.tbWga);
            this.panel2.Controls.Add(this.lblWga);
            this.panel2.Controls.Add(this.tbWin);
            this.panel2.Controls.Add(this.lblWin);
            this.panel2.Controls.Add(this.tbWsn);
            this.panel2.Controls.Add(this.lblWsn);
            this.panel2.Controls.Add(this.tbWzn);
            this.panel2.Controls.Add(this.lblWzn);
            this.panel2.Controls.Add(this.tbWal);
            this.panel2.Controls.Add(this.lblWal);
            this.panel2.Controls.Add(this.tbRtp);
            this.panel2.Controls.Add(this.lblRtp);
            this.panel2.Location = new System.Drawing.Point(308, 18);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(224, 212);
            // 
            // label2
            // 
            this.label2.BackColor = System.Drawing.Color.Transparent;
            this.label2.Location = new System.Drawing.Point(317, 2);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 17);
            this.label2.Text = "三相点值";
            // 
            // lblRtp
            // 
            this.lblRtp.BackColor = System.Drawing.Color.Transparent;
            this.lblRtp.Location = new System.Drawing.Point(10, 7);
            this.lblRtp.Name = "lblRtp";
            this.lblRtp.Size = new System.Drawing.Size(84, 19);
            this.lblRtp.Text = "水三相点Rtp";
            // 
            // tbRtp
            // 
            this.tbRtp.Location = new System.Drawing.Point(99, 5);
            this.tbRtp.Name = "tbRtp";
            this.tbRtp.Size = new System.Drawing.Size(110, 21);
            this.tbRtp.TabIndex = 1;
            // 
            // tbWal
            // 
            this.tbWal.Location = new System.Drawing.Point(99, 29);
            this.tbWal.Name = "tbWal";
            this.tbWal.Size = new System.Drawing.Size(110, 21);
            this.tbWal.TabIndex = 3;
            // 
            // lblWal
            // 
            this.lblWal.BackColor = System.Drawing.Color.Transparent;
            this.lblWal.Location = new System.Drawing.Point(9, 32);
            this.lblWal.Name = "lblWal";
            this.lblWal.Size = new System.Drawing.Size(84, 19);
            this.lblWal.Text = "铝凝固点Wal";
            // 
            // tbWzn
            // 
            this.tbWzn.Location = new System.Drawing.Point(99, 53);
            this.tbWzn.Name = "tbWzn";
            this.tbWzn.Size = new System.Drawing.Size(110, 21);
            this.tbWzn.TabIndex = 6;
            // 
            // lblWzn
            // 
            this.lblWzn.BackColor = System.Drawing.Color.Transparent;
            this.lblWzn.Location = new System.Drawing.Point(9, 55);
            this.lblWzn.Name = "lblWzn";
            this.lblWzn.Size = new System.Drawing.Size(84, 19);
            this.lblWzn.Text = "锌凝固点Wzn";
            // 
            // tbWsn
            // 
            this.tbWsn.Location = new System.Drawing.Point(99, 77);
            this.tbWsn.Name = "tbWsn";
            this.tbWsn.Size = new System.Drawing.Size(110, 21);
            this.tbWsn.TabIndex = 9;
            // 
            // lblWsn
            // 
            this.lblWsn.BackColor = System.Drawing.Color.Transparent;
            this.lblWsn.Location = new System.Drawing.Point(9, 79);
            this.lblWsn.Name = "lblWsn";
            this.lblWsn.Size = new System.Drawing.Size(84, 19);
            this.lblWsn.Text = "锡凝固点Wsn";
            // 
            // tbWin
            // 
            this.tbWin.Location = new System.Drawing.Point(99, 100);
            this.tbWin.Name = "tbWin";
            this.tbWin.Size = new System.Drawing.Size(110, 21);
            this.tbWin.TabIndex = 12;
            // 
            // lblWin
            // 
            this.lblWin.BackColor = System.Drawing.Color.Transparent;
            this.lblWin.Location = new System.Drawing.Point(9, 104);
            this.lblWin.Name = "lblWin";
            this.lblWin.Size = new System.Drawing.Size(84, 19);
            this.lblWin.Text = "铟凝固点Win";
            // 
            // tbWga
            // 
            this.tbWga.Location = new System.Drawing.Point(99, 124);
            this.tbWga.Name = "tbWga";
            this.tbWga.Size = new System.Drawing.Size(110, 21);
            this.tbWga.TabIndex = 15;
            // 
            // lblWga
            // 
            this.lblWga.BackColor = System.Drawing.Color.Transparent;
            this.lblWga.Location = new System.Drawing.Point(9, 127);
            this.lblWga.Name = "lblWga";
            this.lblWga.Size = new System.Drawing.Size(84, 19);
            this.lblWga.Text = "镓熔点Wga";
            // 
            // tbWhg
            // 
            this.tbWhg.Location = new System.Drawing.Point(99, 148);
            this.tbWhg.Name = "tbWhg";
            this.tbWhg.Size = new System.Drawing.Size(110, 21);
            this.tbWhg.TabIndex = 18;
            // 
            // lblWhg
            // 
            this.lblWhg.BackColor = System.Drawing.Color.Transparent;
            this.lblWhg.Location = new System.Drawing.Point(9, 150);
            this.lblWhg.Name = "lblWhg";
            this.lblWhg.Size = new System.Drawing.Size(84, 19);
            this.lblWhg.Text = "汞三相点Whg";
            // 
            // tbWar
            // 
            this.tbWar.Location = new System.Drawing.Point(99, 173);
            this.tbWar.Name = "tbWar";
            this.tbWar.Size = new System.Drawing.Size(110, 21);
            this.tbWar.TabIndex = 21;
            // 
            // lblWar
            // 
            this.lblWar.BackColor = System.Drawing.Color.Transparent;
            this.lblWar.Location = new System.Drawing.Point(9, 175);
            this.lblWar.Name = "lblWar";
            this.lblWar.Size = new System.Drawing.Size(84, 19);
            this.lblWar.Text = "氩三相点War";
            // 
            // btQuit
            // 
            this.btQuit.Location = new System.Drawing.Point(325, 236);
            this.btQuit.Name = "btQuit";
            this.btQuit.Size = new System.Drawing.Size(94, 29);
            this.btQuit.TabIndex = 24;
            this.btQuit.Text = "放弃退出";
            // 
            // btSave
            // 
            this.btSave.Location = new System.Drawing.Point(157, 236);
            this.btSave.Name = "btSave";
            this.btSave.Size = new System.Drawing.Size(94, 29);
            this.btSave.TabIndex = 23;
            this.btSave.Text = "保存修改";
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.tbCoefC);
            this.panel3.Controls.Add(this.lblCoefC);
            this.panel3.Controls.Add(this.lblCoefB);
            this.panel3.Controls.Add(this.lblCoefA);
            this.panel3.Controls.Add(this.tbCoefB);
            this.panel3.Controls.Add(this.tbCoefA);
            this.panel3.Location = new System.Drawing.Point(24, 156);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(201, 74);
            // 
            // label11
            // 
            this.label11.BackColor = System.Drawing.Color.Transparent;
            this.label11.Location = new System.Drawing.Point(24, 138);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(64, 15);
            this.label11.Text = "系数";
            // 
            // lblCoefA
            // 
            this.lblCoefA.BackColor = System.Drawing.Color.Transparent;
            this.lblCoefA.Location = new System.Drawing.Point(3, 7);
            this.lblCoefA.Name = "lblCoefA";
            this.lblCoefA.Size = new System.Drawing.Size(51, 19);
            this.lblCoefA.Text = "系数a";
            // 
            // lblCoefB
            // 
            this.lblCoefB.BackColor = System.Drawing.Color.Transparent;
            this.lblCoefB.Location = new System.Drawing.Point(3, 27);
            this.lblCoefB.Name = "lblCoefB";
            this.lblCoefB.Size = new System.Drawing.Size(51, 19);
            this.lblCoefB.Text = "系数b";
            // 
            // lblCoefC
            // 
            this.lblCoefC.BackColor = System.Drawing.Color.Transparent;
            this.lblCoefC.Location = new System.Drawing.Point(3, 48);
            this.lblCoefC.Name = "lblCoefC";
            this.lblCoefC.Size = new System.Drawing.Size(51, 19);
            this.lblCoefC.Text = "系数c";
            // 
            // tbCoefA
            // 
            this.tbCoefA.Location = new System.Drawing.Point(73, 3);
            this.tbCoefA.Name = "tbCoefA";
            this.tbCoefA.Size = new System.Drawing.Size(110, 21);
            this.tbCoefA.TabIndex = 1;
            // 
            // tbCoefB
            // 
            this.tbCoefB.Location = new System.Drawing.Point(73, 25);
            this.tbCoefB.Name = "tbCoefB";
            this.tbCoefB.Size = new System.Drawing.Size(110, 21);
            this.tbCoefB.TabIndex = 1;
            // 
            // tbCoefC
            // 
            this.tbCoefC.Location = new System.Drawing.Point(73, 47);
            this.tbCoefC.Name = "tbCoefC";
            this.tbCoefC.Size = new System.Drawing.Size(110, 21);
            this.tbCoefC.TabIndex = 3;
            // 
            // btConvert
            // 
            this.btConvert.BackColor = System.Drawing.SystemColors.Window;
            this.btConvert.Location = new System.Drawing.Point(240, 173);
            this.btConvert.Name = "btConvert";
            this.btConvert.Size = new System.Drawing.Size(62, 29);
            this.btConvert.TabIndex = 23;
            this.btConvert.Text = "转换<=";
            // 
            // CoefForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(545, 268);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.btQuit);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.btConvert);
            this.Controls.Add(this.btSave);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.panel1);
            this.Menu = this.mainMenu1;
            this.Name = "CoefForm";
            this.Text = "CoefForm";
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel3.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.RadioButton radioButton6;
        private System.Windows.Forms.RadioButton radioButton7;
        private System.Windows.Forms.RadioButton radioButton5;
        private System.Windows.Forms.RadioButton radioButton4;
        private System.Windows.Forms.RadioButton radioButton3;
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.TextBox tbRtp;
        private System.Windows.Forms.Label lblRtp;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tbWar;
        private System.Windows.Forms.Label lblWar;
        private System.Windows.Forms.TextBox tbWhg;
        private System.Windows.Forms.Label lblWhg;
        private System.Windows.Forms.TextBox tbWga;
        private System.Windows.Forms.Label lblWga;
        private System.Windows.Forms.TextBox tbWin;
        private System.Windows.Forms.Label lblWin;
        private System.Windows.Forms.TextBox tbWsn;
        private System.Windows.Forms.Label lblWsn;
        private System.Windows.Forms.TextBox tbWzn;
        private System.Windows.Forms.Label lblWzn;
        private System.Windows.Forms.TextBox tbWal;
        private System.Windows.Forms.Label lblWal;
        private System.Windows.Forms.Button btQuit;
        private System.Windows.Forms.Button btSave;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Label lblCoefC;
        private System.Windows.Forms.Label lblCoefB;
        private System.Windows.Forms.Label lblCoefA;
        private System.Windows.Forms.TextBox tbCoefB;
        private System.Windows.Forms.TextBox tbCoefA;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox tbCoefC;
        private System.Windows.Forms.Button btConvert;
    }
}