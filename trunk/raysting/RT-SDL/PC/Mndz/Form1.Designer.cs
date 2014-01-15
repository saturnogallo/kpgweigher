namespace Mndz
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.lbl_range = new System.Windows.Forms.Label();
            this.lbl_ohm = new System.Windows.Forms.Label();
            this.led_rx = new DmitryBrant.CustomControls.SevenSegmentArray();
            this.btn_turnon = new Mndz.RectButton();
            this.lbl_datetime = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.btn_zeroon = new Mndz.RectButton();
            this.label2 = new System.Windows.Forms.Label();
            this.led_rs = new DmitryBrant.CustomControls.SevenSegmentArray();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.led_vx = new DmitryBrant.CustomControls.SevenSegmentArray();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.led_vs = new DmitryBrant.CustomControls.SevenSegmentArray();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.rectMeter1 = new Mndz.MyCtrl.RectMeter();
            this.label13 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // lbl_range
            // 
            this.lbl_range.AutoSize = true;
            this.lbl_range.Font = new System.Drawing.Font("Microsoft Sans Serif", 36.35643F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_range.Location = new System.Drawing.Point(21, 119);
            this.lbl_range.Name = "lbl_range";
            this.lbl_range.Size = new System.Drawing.Size(154, 59);
            this.lbl_range.TabIndex = 6;
            this.lbl_range.Text = "R   = ";
            this.lbl_range.Click += new System.EventHandler(this.lbl_range_Click);
            // 
            // lbl_ohm
            // 
            this.lbl_ohm.AutoSize = true;
            this.lbl_ohm.Font = new System.Drawing.Font("Arial Narrow", 48F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_ohm.Location = new System.Drawing.Point(747, 119);
            this.lbl_ohm.Name = "lbl_ohm";
            this.lbl_ohm.Size = new System.Drawing.Size(76, 78);
            this.lbl_ohm.TabIndex = 4;
            this.lbl_ohm.Text = "G";
            // 
            // led_rx
            // 
            this.led_rx.ArrayCount = 6;
            this.led_rx.ColorBackground = System.Drawing.Color.White;
            this.led_rx.ColorDark = System.Drawing.Color.WhiteSmoke;
            this.led_rx.ColorLight = System.Drawing.Color.Red;
            this.led_rx.DecimalShow = true;
            this.led_rx.ElementPadding = ((Mndz.Padding)(resources.GetObject("led_rx.ElementPadding")));
            this.led_rx.ElementWidth = 12;
            this.led_rx.ItalicFactor = -0.05F;
            this.led_rx.Location = new System.Drawing.Point(179, 91);
            this.led_rx.Name = "led_rx";
            this.led_rx.Size = new System.Drawing.Size(562, 115);
            this.led_rx.TabIndex = 1;
            this.led_rx.TabStop = false;
            this.led_rx.Value = null;
            // 
            // btn_turnon
            // 
            this.btn_turnon.BackColor = System.Drawing.Color.Blue;
            this.btn_turnon.bgScale = 3;
            this.btn_turnon.bOn = true;
            this.btn_turnon.colorShadow = System.Drawing.Color.LightGray;
            this.btn_turnon.colorTop = System.Drawing.Color.Pink;
            this.btn_turnon.Font = new System.Drawing.Font("Microsoft Sans Serif", 21.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_turnon.Label = null;
            this.btn_turnon.Location = new System.Drawing.Point(876, 187);
            this.btn_turnon.Margin = new System.Windows.Forms.Padding(8);
            this.btn_turnon.Name = "btn_turnon";
            this.btn_turnon.Size = new System.Drawing.Size(140, 140);
            this.btn_turnon.Style = Mndz.MyButtonType.roundButton;
            this.btn_turnon.TabIndex = 9;
            this.btn_turnon.Load += new System.EventHandler(this.btn_turnon_Load);
            // 
            // lbl_datetime
            // 
            this.lbl_datetime.AutoSize = true;
            this.lbl_datetime.Location = new System.Drawing.Point(28, 546);
            this.lbl_datetime.Name = "lbl_datetime";
            this.lbl_datetime.Size = new System.Drawing.Size(41, 15);
            this.lbl_datetime.TabIndex = 10;
            this.lbl_datetime.Text = "label2";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 42F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.ForeColor = System.Drawing.Color.MediumBlue;
            this.label1.Location = new System.Drawing.Point(192, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(629, 67);
            this.label1.TabIndex = 0;
            this.label1.Text = "双电源高压高电阻电桥";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // btn_zeroon
            // 
            this.btn_zeroon.BackColor = System.Drawing.Color.Blue;
            this.btn_zeroon.bgScale = 3;
            this.btn_zeroon.bOn = true;
            this.btn_zeroon.colorShadow = System.Drawing.Color.LightGray;
            this.btn_zeroon.colorTop = System.Drawing.Color.Pink;
            this.btn_zeroon.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25743F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_zeroon.Label = null;
            this.btn_zeroon.Location = new System.Drawing.Point(711, 240);
            this.btn_zeroon.Margin = new System.Windows.Forms.Padding(6, 6, 6, 6);
            this.btn_zeroon.Name = "btn_zeroon";
            this.btn_zeroon.Size = new System.Drawing.Size(92, 39);
            this.btn_zeroon.Style = Mndz.MyButtonType.roundButton;
            this.btn_zeroon.TabIndex = 11;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(71, 139);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 39);
            this.label2.TabIndex = 6;
            this.label2.Text = "x";
            this.label2.Click += new System.EventHandler(this.lbl_range_Click);
            // 
            // led_rs
            // 
            this.led_rs.ArrayCount = 4;
            this.led_rs.ColorBackground = System.Drawing.Color.White;
            this.led_rs.ColorDark = System.Drawing.Color.WhiteSmoke;
            this.led_rs.ColorLight = System.Drawing.Color.Red;
            this.led_rs.DecimalShow = true;
            this.led_rs.ElementPadding = ((Mndz.Padding)(resources.GetObject("led_rs.ElementPadding")));
            this.led_rs.ElementWidth = 10;
            this.led_rs.ItalicFactor = -0.05F;
            this.led_rs.Location = new System.Drawing.Point(179, 230);
            this.led_rs.Name = "led_rs";
            this.led_rs.Size = new System.Drawing.Size(279, 106);
            this.led_rs.TabIndex = 1;
            this.led_rs.TabStop = false;
            this.led_rs.Value = null;
            this.led_rs.Load += new System.EventHandler(this.sevenSegmentArray1_Load);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Arial Narrow", 48F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(464, 240);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(181, 78);
            this.label3.TabIndex = 4;
            this.label3.Text = "x10 Ω";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 36.35643F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(21, 254);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(154, 59);
            this.label4.TabIndex = 6;
            this.label4.Text = "R   = ";
            this.label4.Click += new System.EventHandler(this.lbl_range_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.Location = new System.Drawing.Point(558, 216);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(37, 39);
            this.label5.TabIndex = 6;
            this.label5.Text = "6";
            this.label5.Click += new System.EventHandler(this.lbl_range_Click);
            // 
            // led_vx
            // 
            this.led_vx.ArrayCount = 5;
            this.led_vx.ColorBackground = System.Drawing.Color.White;
            this.led_vx.ColorDark = System.Drawing.Color.WhiteSmoke;
            this.led_vx.ColorLight = System.Drawing.Color.Red;
            this.led_vx.DecimalShow = true;
            this.led_vx.ElementPadding = ((Mndz.Padding)(resources.GetObject("led_vx.ElementPadding")));
            this.led_vx.ElementWidth = 10;
            this.led_vx.ItalicFactor = -0.05F;
            this.led_vx.Location = new System.Drawing.Point(179, 350);
            this.led_vx.Name = "led_vx";
            this.led_vx.Size = new System.Drawing.Size(350, 97);
            this.led_vx.TabIndex = 1;
            this.led_vx.TabStop = false;
            this.led_vx.Value = null;
            this.led_vx.Load += new System.EventHandler(this.sevenSegmentArray1_Load);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Arial Narrow", 48F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(554, 358);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(70, 78);
            this.label6.TabIndex = 4;
            this.label6.Text = "V";
            this.label6.Click += new System.EventHandler(this.label3_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 36.35643F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(21, 372);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(151, 59);
            this.label7.TabIndex = 6;
            this.label7.Text = "V   = ";
            this.label7.Click += new System.EventHandler(this.lbl_range_Click);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label8.Location = new System.Drawing.Point(71, 392);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(35, 39);
            this.label8.TabIndex = 6;
            this.label8.Text = "x";
            this.label8.Click += new System.EventHandler(this.lbl_range_Click);
            // 
            // led_vs
            // 
            this.led_vs.ArrayCount = 5;
            this.led_vs.ColorBackground = System.Drawing.Color.White;
            this.led_vs.ColorDark = System.Drawing.Color.WhiteSmoke;
            this.led_vs.ColorLight = System.Drawing.Color.Red;
            this.led_vs.DecimalShow = true;
            this.led_vs.ElementPadding = ((Mndz.Padding)(resources.GetObject("led_vs.ElementPadding")));
            this.led_vs.ElementWidth = 10;
            this.led_vs.ItalicFactor = -0.05F;
            this.led_vs.Location = new System.Drawing.Point(179, 457);
            this.led_vs.Name = "led_vs";
            this.led_vs.Size = new System.Drawing.Size(350, 95);
            this.led_vs.TabIndex = 1;
            this.led_vs.TabStop = false;
            this.led_vs.Value = null;
            this.led_vs.Load += new System.EventHandler(this.sevenSegmentArray1_Load);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("Arial Narrow", 48F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.Location = new System.Drawing.Point(554, 474);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(70, 78);
            this.label9.TabIndex = 4;
            this.label9.Text = "V";
            this.label9.Click += new System.EventHandler(this.label3_Click);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Microsoft Sans Serif", 36.35643F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.Location = new System.Drawing.Point(21, 474);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(151, 59);
            this.label10.TabIndex = 6;
            this.label10.Text = "E   = ";
            this.label10.Click += new System.EventHandler(this.lbl_range_Click);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label11.Location = new System.Drawing.Point(71, 494);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(35, 39);
            this.label11.TabIndex = 6;
            this.label11.Text = "s";
            this.label11.Click += new System.EventHandler(this.lbl_range_Click);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label12.Location = new System.Drawing.Point(71, 279);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(35, 39);
            this.label12.TabIndex = 6;
            this.label12.Text = "s";
            this.label12.Click += new System.EventHandler(this.lbl_range_Click);
            // 
            // rectMeter1
            // 
            this.rectMeter1.Angle = 0;
            this.rectMeter1.BgResId = "";
            this.rectMeter1.Location = new System.Drawing.Point(662, 358);
            this.rectMeter1.Name = "rectMeter1";
            this.rectMeter1.PointColor = System.Drawing.Color.Yellow;
            this.rectMeter1.Size = new System.Drawing.Size(339, 170);
            this.rectMeter1.TabIndex = 12;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Font = new System.Drawing.Font("Arial Narrow", 24.23762F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label13.Location = new System.Drawing.Point(809, 149);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(38, 40);
            this.label13.TabIndex = 13;
            this.label13.Text = "Ω";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(1018, 568);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.rectMeter1);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.btn_zeroon);
            this.Controls.Add(this.lbl_datetime);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.lbl_range);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.lbl_ohm);
            this.Controls.Add(this.led_vs);
            this.Controls.Add(this.led_vx);
            this.Controls.Add(this.led_rs);
            this.Controls.Add(this.led_rx);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btn_turnon);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "Form1";
            this.Text = "ON";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private DmitryBrant.CustomControls.SevenSegmentArray led_rx;
        private System.Windows.Forms.Label lbl_range;
        private System.Windows.Forms.Label lbl_ohm;
        private RectButton btn_turnon;
        private System.Windows.Forms.Label lbl_datetime;
        private System.Windows.Forms.Label label1;
        private RectButton btn_zeroon;
        private System.Windows.Forms.Label label2;
        private DmitryBrant.CustomControls.SevenSegmentArray led_rs;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private DmitryBrant.CustomControls.SevenSegmentArray led_vx;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private DmitryBrant.CustomControls.SevenSegmentArray led_vs;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private Mndz.MyCtrl.RectMeter rectMeter1;
        private System.Windows.Forms.Label label13;
    }
}

