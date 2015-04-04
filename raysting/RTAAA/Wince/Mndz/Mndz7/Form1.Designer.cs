namespace Mndz7
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
            this.lbl_currscale = new System.Windows.Forms.Label();
            this.lbl_ohm = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.lbl_version = new System.Windows.Forms.Label();
            this.lbl_datetime = new System.Windows.Forms.Label();
            this.btn_capture = new System.Windows.Forms.Button();
            this.lbl_status = new System.Windows.Forms.Label();
            this.rbtn_2dn = new Mndz7.RectButton();
            this.rbtn_1dn = new Mndz7.RectButton();
            this.rbtn_3dn = new Mndz7.RectButton();
            this.rbtn_4dn = new Mndz7.RectButton();
            this.rbtn_2up = new Mndz7.RectButton();
            this.rbtn_5dn = new Mndz7.RectButton();
            this.rbtn_1up = new Mndz7.RectButton();
            this.rbtn_6dn = new Mndz7.RectButton();
            this.rbtn_3up = new Mndz7.RectButton();
            this.rbtn_4up = new Mndz7.RectButton();
            this.rbtn_5up = new Mndz7.RectButton();
            this.rbtn_6up = new Mndz7.RectButton();
            this.btn_zeroon = new Mndz7.RectButton();
            this.btn_range6 = new Mndz7.RectButton();
            this.btn_range5 = new Mndz7.RectButton();
            this.btn_range4 = new Mndz7.RectButton();
            this.btn_range3 = new Mndz7.RectButton();
            this.btn_range2 = new Mndz7.RectButton();
            this.btn_range1 = new Mndz7.RectButton();
            this.led_current = new DmitryBrant.CustomControls.SevenSegmentArray();
            this.led_ohm = new DmitryBrant.CustomControls.SevenSegmentArray();
            this.btn_range0 = new Mndz7.RectButton();
            this.btn_turnon = new Mndz7.RectButton();
            this.lbl_davalue = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // lbl_currscale
            // 
            this.lbl_currscale.Font = new System.Drawing.Font("Tahoma", 22F, System.Drawing.FontStyle.Regular);
            this.lbl_currscale.Location = new System.Drawing.Point(573, 394);
            this.lbl_currscale.Name = "lbl_currscale";
            this.lbl_currscale.Size = new System.Drawing.Size(64, 35);
            this.lbl_currscale.Text = "mA";
            // 
            // lbl_ohm
            // 
            this.lbl_ohm.Font = new System.Drawing.Font("Tahoma", 36F, System.Drawing.FontStyle.Regular);
            this.lbl_ohm.Location = new System.Drawing.Point(702, 205);
            this.lbl_ohm.Name = "lbl_ohm";
            this.lbl_ohm.Size = new System.Drawing.Size(98, 53);
            this.lbl_ohm.Text = "mΩ";
            // 
            // label3
            // 
            this.label3.Font = new System.Drawing.Font("KaiTi", 22F, System.Drawing.FontStyle.Regular);
            this.label3.ForeColor = System.Drawing.Color.MidnightBlue;
            this.label3.Location = new System.Drawing.Point(454, 151);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(193, 36);
            this.label3.Text = "直流大功率模拟电阻";
            this.label3.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.label3.Visible = false;
            // 
            // label4
            // 
            this.label4.Font = new System.Drawing.Font("KaiTi", 22F, System.Drawing.FontStyle.Regular);
            this.label4.Location = new System.Drawing.Point(181, 352);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(107, 32);
            this.label4.Text = "电流";
            this.label4.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // lbl_version
            // 
            this.lbl_version.Font = new System.Drawing.Font("Arial", 8F, System.Drawing.FontStyle.Regular);
            this.lbl_version.Location = new System.Drawing.Point(702, 10);
            this.lbl_version.Name = "lbl_version";
            this.lbl_version.Size = new System.Drawing.Size(94, 17);
            this.lbl_version.Text = "版本:2015-03-08";
            this.lbl_version.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // lbl_datetime
            // 
            this.lbl_datetime.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Regular);
            this.lbl_datetime.Location = new System.Drawing.Point(501, 151);
            this.lbl_datetime.Name = "lbl_datetime";
            this.lbl_datetime.Size = new System.Drawing.Size(146, 18);
            this.lbl_datetime.Text = "2014-11-15 13:45:45";
            this.lbl_datetime.Visible = false;
            // 
            // btn_capture
            // 
            this.btn_capture.Location = new System.Drawing.Point(6, 10);
            this.btn_capture.Name = "btn_capture";
            this.btn_capture.Size = new System.Drawing.Size(111, 26);
            this.btn_capture.TabIndex = 28;
            this.btn_capture.Text = "截屏";
            this.btn_capture.Visible = false;
            this.btn_capture.Click += new System.EventHandler(this.btn_capture_Click);
            // 
            // lbl_status
            // 
            this.lbl_status.Location = new System.Drawing.Point(716, 96);
            this.lbl_status.Name = "lbl_status";
            this.lbl_status.Size = new System.Drawing.Size(76, 33);
            this.lbl_status.Text = "输出饱和";
            // 
            // rbtn_2dn
            // 
            this.rbtn_2dn.Location = new System.Drawing.Point(520, 278);
            this.rbtn_2dn.Name = "rbtn_2dn";
            this.rbtn_2dn.Size = new System.Drawing.Size(69, 38);
            this.rbtn_2dn.TabIndex = 19;
            // 
            // rbtn_1dn
            // 
            this.rbtn_1dn.Location = new System.Drawing.Point(602, 278);
            this.rbtn_1dn.Name = "rbtn_1dn";
            this.rbtn_1dn.Size = new System.Drawing.Size(69, 38);
            this.rbtn_1dn.TabIndex = 21;
            // 
            // rbtn_3dn
            // 
            this.rbtn_3dn.Location = new System.Drawing.Point(438, 278);
            this.rbtn_3dn.Name = "rbtn_3dn";
            this.rbtn_3dn.Size = new System.Drawing.Size(69, 38);
            this.rbtn_3dn.TabIndex = 22;
            // 
            // rbtn_4dn
            // 
            this.rbtn_4dn.Location = new System.Drawing.Point(356, 278);
            this.rbtn_4dn.Name = "rbtn_4dn";
            this.rbtn_4dn.Size = new System.Drawing.Size(69, 38);
            this.rbtn_4dn.TabIndex = 20;
            // 
            // rbtn_2up
            // 
            this.rbtn_2up.Location = new System.Drawing.Point(520, 25);
            this.rbtn_2up.Name = "rbtn_2up";
            this.rbtn_2up.Size = new System.Drawing.Size(69, 38);
            this.rbtn_2up.TabIndex = 16;
            // 
            // rbtn_5dn
            // 
            this.rbtn_5dn.Location = new System.Drawing.Point(274, 278);
            this.rbtn_5dn.Name = "rbtn_5dn";
            this.rbtn_5dn.Size = new System.Drawing.Size(69, 38);
            this.rbtn_5dn.TabIndex = 18;
            // 
            // rbtn_1up
            // 
            this.rbtn_1up.Location = new System.Drawing.Point(602, 25);
            this.rbtn_1up.Name = "rbtn_1up";
            this.rbtn_1up.Size = new System.Drawing.Size(69, 38);
            this.rbtn_1up.TabIndex = 16;
            // 
            // rbtn_6dn
            // 
            this.rbtn_6dn.Location = new System.Drawing.Point(192, 278);
            this.rbtn_6dn.Name = "rbtn_6dn";
            this.rbtn_6dn.Size = new System.Drawing.Size(69, 38);
            this.rbtn_6dn.TabIndex = 17;
            // 
            // rbtn_3up
            // 
            this.rbtn_3up.Location = new System.Drawing.Point(438, 25);
            this.rbtn_3up.Name = "rbtn_3up";
            this.rbtn_3up.Size = new System.Drawing.Size(69, 38);
            this.rbtn_3up.TabIndex = 16;
            // 
            // rbtn_4up
            // 
            this.rbtn_4up.Location = new System.Drawing.Point(356, 25);
            this.rbtn_4up.Name = "rbtn_4up";
            this.rbtn_4up.Size = new System.Drawing.Size(69, 38);
            this.rbtn_4up.TabIndex = 16;
            // 
            // rbtn_5up
            // 
            this.rbtn_5up.Location = new System.Drawing.Point(274, 25);
            this.rbtn_5up.Name = "rbtn_5up";
            this.rbtn_5up.Size = new System.Drawing.Size(69, 38);
            this.rbtn_5up.TabIndex = 15;
            // 
            // rbtn_6up
            // 
            this.rbtn_6up.Location = new System.Drawing.Point(192, 25);
            this.rbtn_6up.Name = "rbtn_6up";
            this.rbtn_6up.Size = new System.Drawing.Size(69, 38);
            this.rbtn_6up.TabIndex = 14;
            // 
            // btn_zeroon
            // 
            this.btn_zeroon.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.btn_zeroon.Location = new System.Drawing.Point(190, 408);
            this.btn_zeroon.Name = "btn_zeroon";
            this.btn_zeroon.Size = new System.Drawing.Size(90, 47);
            this.btn_zeroon.TabIndex = 10;
            // 
            // btn_range6
            // 
            this.btn_range6.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Bold);
            this.btn_range6.Location = new System.Drawing.Point(5, 407);
            this.btn_range6.Name = "btn_range6";
            this.btn_range6.Size = new System.Drawing.Size(142, 48);
            this.btn_range6.TabIndex = 10;
            // 
            // btn_range5
            // 
            this.btn_range5.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Bold);
            this.btn_range5.Location = new System.Drawing.Point(5, 343);
            this.btn_range5.Name = "btn_range5";
            this.btn_range5.Size = new System.Drawing.Size(142, 48);
            this.btn_range5.TabIndex = 10;
            // 
            // btn_range4
            // 
            this.btn_range4.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Bold);
            this.btn_range4.Location = new System.Drawing.Point(5, 279);
            this.btn_range4.Name = "btn_range4";
            this.btn_range4.Size = new System.Drawing.Size(142, 48);
            this.btn_range4.TabIndex = 10;
            // 
            // btn_range3
            // 
            this.btn_range3.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Bold);
            this.btn_range3.Location = new System.Drawing.Point(5, 215);
            this.btn_range3.Name = "btn_range3";
            this.btn_range3.Size = new System.Drawing.Size(142, 48);
            this.btn_range3.TabIndex = 10;
            // 
            // btn_range2
            // 
            this.btn_range2.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Bold);
            this.btn_range2.Location = new System.Drawing.Point(5, 151);
            this.btn_range2.Name = "btn_range2";
            this.btn_range2.Size = new System.Drawing.Size(142, 48);
            this.btn_range2.TabIndex = 9;
            // 
            // btn_range1
            // 
            this.btn_range1.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Bold);
            this.btn_range1.Location = new System.Drawing.Point(5, 87);
            this.btn_range1.Name = "btn_range1";
            this.btn_range1.Size = new System.Drawing.Size(142, 48);
            this.btn_range1.TabIndex = 8;
            // 
            // led_current
            // 
            this.led_current.ArrayCount = 5;
            this.led_current.Location = new System.Drawing.Point(306, 352);
            this.led_current.Name = "led_current";
            this.led_current.Size = new System.Drawing.Size(261, 81);
            this.led_current.TabIndex = 1;
            // 
            // led_ohm
            // 
            this.led_ohm.ArrayCount = 6;
            this.led_ohm.Location = new System.Drawing.Point(192, 79);
            this.led_ohm.Name = "led_ohm";
            this.led_ohm.Size = new System.Drawing.Size(504, 184);
            this.led_ohm.TabIndex = 1;
            // 
            // btn_range0
            // 
            this.btn_range0.BackColor = System.Drawing.Color.LightGoldenrodYellow;
            this.btn_range0.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Bold);
            this.btn_range0.Location = new System.Drawing.Point(5, 23);
            this.btn_range0.Name = "btn_range0";
            this.btn_range0.Size = new System.Drawing.Size(142, 48);
            this.btn_range0.TabIndex = 0;
            // 
            // btn_turnon
            // 
            this.btn_turnon.Font = new System.Drawing.Font("Arial", 14F, System.Drawing.FontStyle.Bold);
            this.btn_turnon.Location = new System.Drawing.Point(643, 326);
            this.btn_turnon.Name = "btn_turnon";
            this.btn_turnon.Size = new System.Drawing.Size(135, 134);
            this.btn_turnon.TabIndex = 25;
            // 
            // lbl_davalue
            // 
            this.lbl_davalue.Location = new System.Drawing.Point(318, 436);
            this.lbl_davalue.Name = "lbl_davalue";
            this.lbl_davalue.Size = new System.Drawing.Size(249, 31);
            this.lbl_davalue.Text = "label1";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(800, 480);
            this.ControlBox = false;
            this.Controls.Add(this.lbl_davalue);
            this.Controls.Add(this.lbl_status);
            this.Controls.Add(this.btn_capture);
            this.Controls.Add(this.rbtn_2dn);
            this.Controls.Add(this.rbtn_1dn);
            this.Controls.Add(this.lbl_version);
            this.Controls.Add(this.rbtn_3dn);
            this.Controls.Add(this.rbtn_4dn);
            this.Controls.Add(this.rbtn_2up);
            this.Controls.Add(this.rbtn_5dn);
            this.Controls.Add(this.rbtn_1up);
            this.Controls.Add(this.rbtn_6dn);
            this.Controls.Add(this.rbtn_3up);
            this.Controls.Add(this.rbtn_4up);
            this.Controls.Add(this.rbtn_5up);
            this.Controls.Add(this.rbtn_6up);
            this.Controls.Add(this.btn_zeroon);
            this.Controls.Add(this.btn_range6);
            this.Controls.Add(this.btn_range5);
            this.Controls.Add(this.btn_range4);
            this.Controls.Add(this.btn_range3);
            this.Controls.Add(this.btn_range2);
            this.Controls.Add(this.btn_range1);
            this.Controls.Add(this.lbl_ohm);
            this.Controls.Add(this.lbl_currscale);
            this.Controls.Add(this.led_current);
            this.Controls.Add(this.led_ohm);
            this.Controls.Add(this.btn_range0);
            this.Controls.Add(this.btn_turnon);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.lbl_datetime);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "Form1";
            this.Text = "Form1";
            this.TopMost = true;
            this.ResumeLayout(false);

        }

        #endregion

        private RectButton btn_range0;
        private DmitryBrant.CustomControls.SevenSegmentArray led_ohm;
        private DmitryBrant.CustomControls.SevenSegmentArray led_current;
        private System.Windows.Forms.Label lbl_currscale;
        private System.Windows.Forms.Label lbl_ohm;
        private System.Windows.Forms.Label label3;
        private RectButton btn_range1;
        private RectButton btn_range2;
        private RectButton btn_range3;
        private RectButton rbtn_6up;
        private RectButton rbtn_5up;
        private RectButton rbtn_4up;
        private RectButton rbtn_3up;
        private RectButton rbtn_1up;
        private RectButton rbtn_2up;
        private RectButton rbtn_2dn;
        private RectButton rbtn_1dn;
        private RectButton rbtn_3dn;
        private RectButton rbtn_4dn;
        private RectButton rbtn_5dn;
        private RectButton rbtn_6dn;
        private System.Windows.Forms.Label label4;
        private RectButton btn_turnon;
        private System.Windows.Forms.Label lbl_version;
        private System.Windows.Forms.Label lbl_datetime;
        private RectButton btn_zeroon;
        private System.Windows.Forms.Button btn_capture;
        private RectButton btn_range4;
        private RectButton btn_range5;
        private RectButton btn_range6;
        private System.Windows.Forms.Label lbl_status;
        private System.Windows.Forms.Label lbl_davalue;
    }
}

