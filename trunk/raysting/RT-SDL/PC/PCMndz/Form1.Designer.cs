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
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.lbl_range = new System.Windows.Forms.Label();
            this.lbl_ohm = new System.Windows.Forms.Label();
            this.lbl_current = new System.Windows.Forms.Label();
            this.gb_real = new System.Windows.Forms.GroupBox();
            this.lbl_currscale = new System.Windows.Forms.Label();
            this.lbl_datetime = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.btn_zeroon = new Mndz.RectButton();
            this.rbtn_real15 = new Mndz.RectButton();
            this.rbtn_real14 = new Mndz.RectButton();
            this.rbtn_real13 = new Mndz.RectButton();
            this.rbtn_real12 = new Mndz.RectButton();
            this.rbtn_real11 = new Mndz.RectButton();
            this.rbtn_real10 = new Mndz.RectButton();
            this.rbtn_real9 = new Mndz.RectButton();
            this.rbtn_real8 = new Mndz.RectButton();
            this.rbtn_real7 = new Mndz.RectButton();
            this.rbtn_real6 = new Mndz.RectButton();
            this.rbtn_real5 = new Mndz.RectButton();
            this.rbtn_real4 = new Mndz.RectButton();
            this.rbtn_real3 = new Mndz.RectButton();
            this.rbtn_real2 = new Mndz.RectButton();
            this.rbtn_real1 = new Mndz.RectButton();
            this.led_current = new DmitryBrant.CustomControls.SevenSegmentArray();
            this.btn_rangeReal = new Mndz.RectButton();
            this.btn_range3 = new Mndz.RectButton();
            this.btn_range2 = new Mndz.RectButton();
            this.btn_range1 = new Mndz.RectButton();
            this.btn_range0 = new Mndz.RectButton();
            this.rbtn_2dn = new Mndz.RectButton();
            this.rbtn_2up = new Mndz.RectButton();
            this.rbtn_3dn = new Mndz.RectButton();
            this.rbtn_3up = new Mndz.RectButton();
            this.rbtn_4dn = new Mndz.RectButton();
            this.rbtn_4up = new Mndz.RectButton();
            this.rbtn_5dn = new Mndz.RectButton();
            this.rbtn_5up = new Mndz.RectButton();
            this.rbtn_6dn = new Mndz.RectButton();
            this.rbtn_6up = new Mndz.RectButton();
            this.rbtn_1dn = new Mndz.RectButton();
            this.rbtn_1up = new Mndz.RectButton();
            this.led_ohm = new DmitryBrant.CustomControls.SevenSegmentArray();
            this.btn_turnon = new Mndz.RectButton();
            this.gb_real.SuspendLayout();
            this.SuspendLayout();
            // 
            // lbl_range
            // 
            this.lbl_range.AutoSize = true;
            this.lbl_range.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.lbl_range.Location = new System.Drawing.Point(36, 110);
            this.lbl_range.Name = "lbl_range";
            this.lbl_range.Size = new System.Drawing.Size(87, 39);
            this.lbl_range.TabIndex = 6;
            this.lbl_range.Text = "量程";
            // 
            // lbl_ohm
            // 
            this.lbl_ohm.AutoSize = true;
            this.lbl_ohm.Font = new System.Drawing.Font("Arial Narrow", 48F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_ohm.Location = new System.Drawing.Point(754, 395);
            this.lbl_ohm.Name = "lbl_ohm";
            this.lbl_ohm.Size = new System.Drawing.Size(121, 78);
            this.lbl_ohm.TabIndex = 4;
            this.lbl_ohm.Text = "mΩ";
            // 
            // lbl_current
            // 
            this.lbl_current.AutoSize = true;
            this.lbl_current.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.lbl_current.Location = new System.Drawing.Point(820, 80);
            this.lbl_current.Name = "lbl_current";
            this.lbl_current.Size = new System.Drawing.Size(87, 39);
            this.lbl_current.TabIndex = 7;
            this.lbl_current.Text = "电流";
            // 
            // gb_real
            // 
            this.gb_real.Controls.Add(this.rbtn_real15);
            this.gb_real.Controls.Add(this.rbtn_real14);
            this.gb_real.Controls.Add(this.rbtn_real13);
            this.gb_real.Controls.Add(this.rbtn_real12);
            this.gb_real.Controls.Add(this.rbtn_real11);
            this.gb_real.Controls.Add(this.rbtn_real10);
            this.gb_real.Controls.Add(this.rbtn_real9);
            this.gb_real.Controls.Add(this.rbtn_real8);
            this.gb_real.Controls.Add(this.rbtn_real7);
            this.gb_real.Controls.Add(this.rbtn_real6);
            this.gb_real.Controls.Add(this.rbtn_real5);
            this.gb_real.Controls.Add(this.rbtn_real4);
            this.gb_real.Controls.Add(this.rbtn_real3);
            this.gb_real.Controls.Add(this.rbtn_real2);
            this.gb_real.Controls.Add(this.rbtn_real1);
            this.gb_real.Location = new System.Drawing.Point(163, 495);
            this.gb_real.Name = "gb_real";
            this.gb_real.Size = new System.Drawing.Size(658, 154);
            this.gb_real.TabIndex = 8;
            this.gb_real.TabStop = false;
            // 
            // lbl_currscale
            // 
            this.lbl_currscale.AutoSize = true;
            this.lbl_currscale.Font = new System.Drawing.Font("Arial Narrow", 21.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_currscale.Location = new System.Drawing.Point(971, 189);
            this.lbl_currscale.Name = "lbl_currscale";
            this.lbl_currscale.Size = new System.Drawing.Size(53, 36);
            this.lbl_currscale.TabIndex = 4;
            this.lbl_currscale.Text = "A";
            // 
            // lbl_datetime
            // 
            this.lbl_datetime.AutoSize = true;
            this.lbl_datetime.Location = new System.Drawing.Point(29, 663);
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
            this.label1.Location = new System.Drawing.Point(151, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(689, 67);
            this.label1.TabIndex = 0;
            this.label1.Text = "智能数字微电阻标准装置";
            // 
            // btn_zeroon
            // 
            this.btn_zeroon.BackColor = System.Drawing.Color.Blue;
            this.btn_zeroon.Enabled = true;
            this.btn_zeroon.bOn = true;
            this.btn_zeroon.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25743F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_zeroon.Location = new System.Drawing.Point(897, 228);
            this.btn_zeroon.Margin = new System.Windows.Forms.Padding(6, 6, 6, 6);
            this.btn_zeroon.Name = "btn_zeroon";
            this.btn_zeroon.Size = new System.Drawing.Size(92, 39);
            this.btn_zeroon.TabIndex = 11;
            // 
            // rbtn_real15
            // 
            this.rbtn_real15.BackColor = System.Drawing.Color.Blue;
            this.rbtn_real15.Enabled = true;
            this.rbtn_real15.bOn = true;
            this.rbtn_real15.Location = new System.Drawing.Point(508, 94);
            this.rbtn_real15.Name = "rbtn_real15";
            this.rbtn_real15.Size = new System.Drawing.Size(60, 54);
            this.rbtn_real15.TabIndex = 10;
            // 
            // rbtn_real14
            // 
            this.rbtn_real14.BackColor = System.Drawing.Color.Blue;
            this.rbtn_real14.Enabled = true;
            this.rbtn_real14.bOn = true;
            this.rbtn_real14.Location = new System.Drawing.Point(426, 94);
            this.rbtn_real14.Name = "rbtn_real14";
            this.rbtn_real14.Size = new System.Drawing.Size(60, 54);
            this.rbtn_real14.TabIndex = 7;
            // 
            // rbtn_real13
            // 
            this.rbtn_real13.BackColor = System.Drawing.Color.Blue;
            this.rbtn_real13.Enabled = true;
            this.rbtn_real13.bOn = true;
            this.rbtn_real13.Location = new System.Drawing.Point(344, 94);
            this.rbtn_real13.Name = "rbtn_real13";
            this.rbtn_real13.Size = new System.Drawing.Size(60, 54);
            this.rbtn_real13.TabIndex = 4;
            // 
            // rbtn_real12
            // 
            this.rbtn_real12.BackColor = System.Drawing.Color.Blue;
            this.rbtn_real12.Enabled = true;
            this.rbtn_real12.bOn = true;
            this.rbtn_real12.Location = new System.Drawing.Point(262, 94);
            this.rbtn_real12.Name = "rbtn_real12";
            this.rbtn_real12.Size = new System.Drawing.Size(60, 54);
            this.rbtn_real12.TabIndex = 3;
            // 
            // rbtn_real11
            // 
            this.rbtn_real11.BackColor = System.Drawing.Color.Blue;
            this.rbtn_real11.Enabled = true;
            this.rbtn_real11.bOn = true;
            this.rbtn_real11.Location = new System.Drawing.Point(180, 94);
            this.rbtn_real11.Name = "rbtn_real11";
            this.rbtn_real11.Size = new System.Drawing.Size(60, 54);
            this.rbtn_real11.TabIndex = 6;
            // 
            // rbtn_real10
            // 
            this.rbtn_real10.BackColor = System.Drawing.Color.Blue;
            this.rbtn_real10.Enabled = true;
            this.rbtn_real10.bOn = true;
            this.rbtn_real10.Location = new System.Drawing.Point(98, 94);
            this.rbtn_real10.Name = "rbtn_real10";
            this.rbtn_real10.Size = new System.Drawing.Size(60, 54);
            this.rbtn_real10.TabIndex = 5;
            // 
            // rbtn_real9
            // 
            this.rbtn_real9.BackColor = System.Drawing.Color.Blue;
            this.rbtn_real9.Enabled = true;
            this.rbtn_real9.bOn = true;
            this.rbtn_real9.Location = new System.Drawing.Point(16, 94);
            this.rbtn_real9.Name = "rbtn_real9";
            this.rbtn_real9.Size = new System.Drawing.Size(60, 54);
            this.rbtn_real9.TabIndex = 2;
            // 
            // rbtn_real8
            // 
            this.rbtn_real8.BackColor = System.Drawing.Color.Blue;
            this.rbtn_real8.Enabled = true;
            this.rbtn_real8.bOn = true;
            this.rbtn_real8.Location = new System.Drawing.Point(590, 19);
            this.rbtn_real8.Name = "rbtn_real8";
            this.rbtn_real8.Size = new System.Drawing.Size(60, 54);
            this.rbtn_real8.TabIndex = 2;
            // 
            // rbtn_real7
            // 
            this.rbtn_real7.BackColor = System.Drawing.Color.Blue;
            this.rbtn_real7.Enabled = true;
            this.rbtn_real7.bOn = true;
            this.rbtn_real7.Location = new System.Drawing.Point(508, 19);
            this.rbtn_real7.Name = "rbtn_real7";
            this.rbtn_real7.Size = new System.Drawing.Size(60, 54);
            this.rbtn_real7.TabIndex = 2;
            // 
            // rbtn_real6
            // 
            this.rbtn_real6.BackColor = System.Drawing.Color.Blue;
            this.rbtn_real6.Enabled = true;
            this.rbtn_real6.bOn = true;
            this.rbtn_real6.Location = new System.Drawing.Point(426, 19);
            this.rbtn_real6.Name = "rbtn_real6";
            this.rbtn_real6.Size = new System.Drawing.Size(60, 54);
            this.rbtn_real6.TabIndex = 2;
            // 
            // rbtn_real5
            // 
            this.rbtn_real5.BackColor = System.Drawing.Color.Blue;
            this.rbtn_real5.Enabled = true;
            this.rbtn_real5.bOn = true;
            this.rbtn_real5.Location = new System.Drawing.Point(344, 19);
            this.rbtn_real5.Name = "rbtn_real5";
            this.rbtn_real5.Size = new System.Drawing.Size(60, 54);
            this.rbtn_real5.TabIndex = 2;
            // 
            // rbtn_real4
            // 
            this.rbtn_real4.BackColor = System.Drawing.Color.Blue;
            this.rbtn_real4.Enabled = true;
            this.rbtn_real4.bOn = true;
            this.rbtn_real4.Location = new System.Drawing.Point(262, 19);
            this.rbtn_real4.Name = "rbtn_real4";
            this.rbtn_real4.Size = new System.Drawing.Size(60, 54);
            this.rbtn_real4.TabIndex = 2;
            // 
            // rbtn_real3
            // 
            this.rbtn_real3.BackColor = System.Drawing.Color.Blue;
            this.rbtn_real3.Enabled = true;
            this.rbtn_real3.bOn = true;
            this.rbtn_real3.Location = new System.Drawing.Point(180, 19);
            this.rbtn_real3.Name = "rbtn_real3";
            this.rbtn_real3.Size = new System.Drawing.Size(60, 54);
            this.rbtn_real3.TabIndex = 2;
            // 
            // rbtn_real2
            // 
            this.rbtn_real2.BackColor = System.Drawing.Color.Blue;
            this.rbtn_real2.Enabled = true;
            this.rbtn_real2.bOn = true;
            this.rbtn_real2.Location = new System.Drawing.Point(98, 19);
            this.rbtn_real2.Name = "rbtn_real2";
            this.rbtn_real2.Size = new System.Drawing.Size(60, 54);
            this.rbtn_real2.TabIndex = 2;
            // 
            // rbtn_real1
            // 
            this.rbtn_real1.BackColor = System.Drawing.Color.Blue;
            this.rbtn_real1.Enabled = true;
            this.rbtn_real1.bOn = true;
            this.rbtn_real1.Location = new System.Drawing.Point(16, 19);
            this.rbtn_real1.Name = "rbtn_real1";
            this.rbtn_real1.Size = new System.Drawing.Size(60, 54);
            this.rbtn_real1.TabIndex = 2;
            // 
            // led_current
            // 
            this.led_current.ArrayCount = 5;
            this.led_current.ColorBackground = System.Drawing.Color.White;
            this.led_current.ColorDark = System.Drawing.Color.WhiteSmoke;
            this.led_current.ColorLight = System.Drawing.Color.Lime;
            this.led_current.DecimalShow = true;
            this.led_current.ElementPadding = new Mndz.Padding(5,2,5,2);
            this.led_current.ElementWidth = 5;
            this.led_current.ItalicFactor = 0F;
            this.led_current.Location = new System.Drawing.Point(768, 137);
            this.led_current.Name = "led_current";
            this.led_current.Size = new System.Drawing.Size(198, 88);
            this.led_current.TabIndex = 5;
            this.led_current.TabStop = false;
            this.led_current.Value = null;
            // 
            // btn_rangeReal
            // 
            this.btn_rangeReal.BackColor = System.Drawing.Color.Blue;
            this.btn_rangeReal.Enabled = true;
            this.btn_rangeReal.bOn = true;
            this.btn_rangeReal.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.68317F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_rangeReal.Location = new System.Drawing.Point(17, 541);
            this.btn_rangeReal.Margin = new System.Windows.Forms.Padding(8, 7, 8, 7);
            this.btn_rangeReal.Name = "btn_rangeReal";
            this.btn_rangeReal.Size = new System.Drawing.Size(135, 50);
            this.btn_rangeReal.TabIndex = 3;
            // 
            // btn_range3
            // 
            this.btn_range3.BackColor = System.Drawing.Color.Blue;
            this.btn_range3.Enabled = true;
            this.btn_range3.bOn = true;
            this.btn_range3.Font = new System.Drawing.Font("Microsoft Sans Serif", 20.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_range3.Location = new System.Drawing.Point(17, 335);
            this.btn_range3.Margin = new System.Windows.Forms.Padding(8, 7, 8, 7);
            this.btn_range3.Name = "btn_range3";
            this.btn_range3.Size = new System.Drawing.Size(135, 50);
            this.btn_range3.TabIndex = 3;
            // 
            // btn_range2
            // 
            this.btn_range2.BackColor = System.Drawing.Color.Blue;
            this.btn_range2.Enabled = true;
            this.btn_range2.bOn = true;
            this.btn_range2.Font = new System.Drawing.Font("Microsoft Sans Serif", 20.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_range2.Location = new System.Drawing.Point(17, 278);
            this.btn_range2.Margin = new System.Windows.Forms.Padding(8, 7, 8, 7);
            this.btn_range2.Name = "btn_range2";
            this.btn_range2.Size = new System.Drawing.Size(135, 50);
            this.btn_range2.TabIndex = 3;
            // 
            // btn_range1
            // 
            this.btn_range1.BackColor = System.Drawing.Color.Blue;
            this.btn_range1.Enabled = true;
            this.btn_range1.bOn = true;
            this.btn_range1.Font = new System.Drawing.Font("Microsoft Sans Serif", 20.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_range1.Location = new System.Drawing.Point(17, 222);
            this.btn_range1.Margin = new System.Windows.Forms.Padding(8, 7, 8, 7);
            this.btn_range1.Name = "btn_range1";
            this.btn_range1.Size = new System.Drawing.Size(135, 50);
            this.btn_range1.TabIndex = 3;
            // 
            // btn_range0
            // 
            this.btn_range0.BackColor = System.Drawing.Color.Blue;
            this.btn_range0.Enabled = true;
            this.btn_range0.bOn = true;
            this.btn_range0.Font = new System.Drawing.Font("Microsoft Sans Serif", 20.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_range0.Location = new System.Drawing.Point(17, 164);
            this.btn_range0.Margin = new System.Windows.Forms.Padding(8, 7, 8, 7);
            this.btn_range0.Name = "btn_range0";
            this.btn_range0.Size = new System.Drawing.Size(135, 50);
            this.btn_range0.TabIndex = 3;
            // 
            // rbtn_2dn
            // 
            this.rbtn_2dn.BackColor = System.Drawing.Color.Blue;
            this.rbtn_2dn.Enabled = true;
            this.rbtn_2dn.bOn = true;
            this.rbtn_2dn.Location = new System.Drawing.Point(582, 418);
            this.rbtn_2dn.Name = "rbtn_2dn";
            this.rbtn_2dn.Size = new System.Drawing.Size(61, 44);
            this.rbtn_2dn.TabIndex = 2;
            // 
            // rbtn_2up
            // 
            this.rbtn_2up.BackColor = System.Drawing.Color.Blue;
            this.rbtn_2up.Enabled = true;
            this.rbtn_2up.bOn = true;
            this.rbtn_2up.Location = new System.Drawing.Point(582, 110);
            this.rbtn_2up.Name = "rbtn_2up";
            this.rbtn_2up.Size = new System.Drawing.Size(61, 44);
            this.rbtn_2up.TabIndex = 2;
            // 
            // rbtn_3dn
            // 
            this.rbtn_3dn.BackColor = System.Drawing.Color.Blue;
            this.rbtn_3dn.Enabled = true;
            this.rbtn_3dn.bOn = true;
            this.rbtn_3dn.Location = new System.Drawing.Point(487, 418);
            this.rbtn_3dn.Name = "rbtn_3dn";
            this.rbtn_3dn.Size = new System.Drawing.Size(61, 44);
            this.rbtn_3dn.TabIndex = 2;
            // 
            // rbtn_3up
            // 
            this.rbtn_3up.BackColor = System.Drawing.Color.Blue;
            this.rbtn_3up.Enabled = true;
            this.rbtn_3up.bOn = true;
            this.rbtn_3up.Location = new System.Drawing.Point(487, 110);
            this.rbtn_3up.Name = "rbtn_3up";
            this.rbtn_3up.Size = new System.Drawing.Size(61, 44);
            this.rbtn_3up.TabIndex = 2;
            // 
            // rbtn_4dn
            // 
            this.rbtn_4dn.BackColor = System.Drawing.Color.Blue;
            this.rbtn_4dn.Enabled = true;
            this.rbtn_4dn.bOn = true;
            this.rbtn_4dn.Location = new System.Drawing.Point(392, 418);
            this.rbtn_4dn.Name = "rbtn_4dn";
            this.rbtn_4dn.Size = new System.Drawing.Size(61, 44);
            this.rbtn_4dn.TabIndex = 2;
            // 
            // rbtn_4up
            // 
            this.rbtn_4up.BackColor = System.Drawing.Color.Blue;
            this.rbtn_4up.Enabled = true;
            this.rbtn_4up.bOn = true;
            this.rbtn_4up.Location = new System.Drawing.Point(392, 110);
            this.rbtn_4up.Name = "rbtn_4up";
            this.rbtn_4up.Size = new System.Drawing.Size(61, 44);
            this.rbtn_4up.TabIndex = 2;
            // 
            // rbtn_5dn
            // 
            this.rbtn_5dn.BackColor = System.Drawing.Color.Blue;
            this.rbtn_5dn.Enabled = true;
            this.rbtn_5dn.bOn = true;
            this.rbtn_5dn.Location = new System.Drawing.Point(297, 418);
            this.rbtn_5dn.Name = "rbtn_5dn";
            this.rbtn_5dn.Size = new System.Drawing.Size(61, 44);
            this.rbtn_5dn.TabIndex = 2;
            // 
            // rbtn_5up
            // 
            this.rbtn_5up.BackColor = System.Drawing.Color.Blue;
            this.rbtn_5up.Enabled = true;
            this.rbtn_5up.bOn = true;
            this.rbtn_5up.Location = new System.Drawing.Point(297, 110);
            this.rbtn_5up.Name = "rbtn_5up";
            this.rbtn_5up.Size = new System.Drawing.Size(61, 44);
            this.rbtn_5up.TabIndex = 2;
            // 
            // rbtn_6dn
            // 
            this.rbtn_6dn.BackColor = System.Drawing.Color.Blue;
            this.rbtn_6dn.Enabled = true;
            this.rbtn_6dn.bOn = true;
            this.rbtn_6dn.Location = new System.Drawing.Point(202, 418);
            this.rbtn_6dn.Name = "rbtn_6dn";
            this.rbtn_6dn.Size = new System.Drawing.Size(61, 44);
            this.rbtn_6dn.TabIndex = 2;
            // 
            // rbtn_6up
            // 
            this.rbtn_6up.BackColor = System.Drawing.Color.Blue;
            this.rbtn_6up.Enabled = true;
            this.rbtn_6up.bOn = true;
            this.rbtn_6up.Location = new System.Drawing.Point(202, 110);
            this.rbtn_6up.Name = "rbtn_6up";
            this.rbtn_6up.Size = new System.Drawing.Size(61, 44);
            this.rbtn_6up.TabIndex = 2;
            // 
            // rbtn_1dn
            // 
            this.rbtn_1dn.BackColor = System.Drawing.Color.Blue;
            this.rbtn_1dn.Enabled = true;
            this.rbtn_1dn.bOn = true;
            this.rbtn_1dn.Location = new System.Drawing.Point(677, 418);
            this.rbtn_1dn.Name = "rbtn_1dn";
            this.rbtn_1dn.Size = new System.Drawing.Size(61, 44);
            this.rbtn_1dn.TabIndex = 2;
            // 
            // rbtn_1up
            // 
            this.rbtn_1up.BackColor = System.Drawing.Color.Blue;
            this.rbtn_1up.Enabled = true;
            this.rbtn_1up.bOn = true;
            this.rbtn_1up.Location = new System.Drawing.Point(677, 110);
            this.rbtn_1up.Name = "rbtn_1up";
            this.rbtn_1up.Size = new System.Drawing.Size(61, 44);
            this.rbtn_1up.TabIndex = 2;
            // 
            // led_ohm
            // 
            this.led_ohm.ArrayCount = 6;
            this.led_ohm.ColorBackground = System.Drawing.Color.White;
            this.led_ohm.ColorDark = System.Drawing.Color.WhiteSmoke;
            this.led_ohm.ColorLight = System.Drawing.Color.Red;
            this.led_ohm.DecimalShow = true;
            this.led_ohm.ElementPadding = new Mndz.Padding(5, 2, 5, 2); ;
            this.led_ohm.ElementWidth = 6;
            this.led_ohm.ItalicFactor = -0.05F;
            this.led_ohm.Location = new System.Drawing.Point(192, 164);
            this.led_ohm.Name = "led_ohm";
            this.led_ohm.Padding = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.led_ohm.Size = new System.Drawing.Size(570, 239);
            this.led_ohm.TabIndex = 1;
            this.led_ohm.TabStop = false;
            this.led_ohm.Value = null;
            // 
            // btn_turnon
            // 
            this.btn_turnon.BackColor = System.Drawing.Color.Blue;
            this.btn_turnon.Enabled = true;
            this.btn_turnon.bOn = true;
            this.btn_turnon.Font = new System.Drawing.Font("Microsoft Sans Serif", 21.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_turnon.Location = new System.Drawing.Point(861, 504);
            this.btn_turnon.Margin = new System.Windows.Forms.Padding(8);
            this.btn_turnon.Name = "btn_turnon";
            this.btn_turnon.Size = new System.Drawing.Size(140, 140);
            this.btn_turnon.TabIndex = 9;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(1018, 698);
            this.Controls.Add(this.btn_zeroon);
            this.Controls.Add(this.lbl_datetime);
            this.Controls.Add(this.gb_real);
            this.Controls.Add(this.lbl_current);
            this.Controls.Add(this.lbl_range);
            this.Controls.Add(this.led_current);
            this.Controls.Add(this.btn_rangeReal);
            this.Controls.Add(this.btn_range3);
            this.Controls.Add(this.btn_range2);
            this.Controls.Add(this.btn_range1);
            this.Controls.Add(this.lbl_currscale);
            this.Controls.Add(this.lbl_ohm);
            this.Controls.Add(this.btn_range0);
            this.Controls.Add(this.rbtn_2dn);
            this.Controls.Add(this.rbtn_2up);
            this.Controls.Add(this.rbtn_3dn);
            this.Controls.Add(this.rbtn_3up);
            this.Controls.Add(this.rbtn_4dn);
            this.Controls.Add(this.rbtn_4up);
            this.Controls.Add(this.rbtn_5dn);
            this.Controls.Add(this.rbtn_5up);
            this.Controls.Add(this.rbtn_6dn);
            this.Controls.Add(this.rbtn_6up);
            this.Controls.Add(this.rbtn_1dn);
            this.Controls.Add(this.rbtn_1up);
            this.Controls.Add(this.led_ohm);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btn_turnon);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "Form1";
            this.gb_real.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private DmitryBrant.CustomControls.SevenSegmentArray led_ohm;
        private RectButton rbtn_1up;
        private RectButton rbtn_1dn;
        private RectButton rbtn_6up;
        private RectButton rbtn_6dn;
        private RectButton rbtn_5up;
        private RectButton rbtn_5dn;
        private RectButton rbtn_4up;
        private RectButton rbtn_4dn;
        private RectButton rbtn_3up;
        private RectButton rbtn_3dn;
        private RectButton rbtn_2up;
        private RectButton rbtn_2dn;
        private RectButton btn_range0;
        private DmitryBrant.CustomControls.SevenSegmentArray led_current;
        private System.Windows.Forms.Label lbl_range;
        private System.Windows.Forms.Label lbl_ohm;
        private System.Windows.Forms.Label lbl_current;
        private System.Windows.Forms.GroupBox gb_real;
        private RectButton rbtn_real15;
        private RectButton rbtn_real14;
        private RectButton rbtn_real13;
        private RectButton rbtn_real12;
        private RectButton rbtn_real11;
        private RectButton rbtn_real10;
        private RectButton rbtn_real9;
        private RectButton rbtn_real8;
        private RectButton rbtn_real7;
        private RectButton rbtn_real6;
        private RectButton rbtn_real5;
        private RectButton rbtn_real4;
        private RectButton rbtn_real3;
        private RectButton rbtn_real2;
        private RectButton rbtn_real1;
        private RectButton btn_range1;
        private RectButton btn_range2;
        private RectButton btn_rangeReal;
        private RectButton btn_range3;
        private System.Windows.Forms.Label lbl_currscale;
        private RectButton btn_turnon;
        private System.Windows.Forms.Label lbl_datetime;
        private System.Windows.Forms.Label label1;
        private RectButton btn_zeroon;
    }
}

