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
            this.lbl_currscale = new System.Windows.Forms.Label();
            this.lbl_ohm = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.lbl_version = new System.Windows.Forms.Label();
            this.lbl_datetime = new System.Windows.Forms.Label();
            this.btn_capture = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.rectMeter1 = new Mndz.MyCtrl.RectMeter();
            this.lbButtonPT = new Mndz.RectButton();
            this.lbButtonOK = new Mndz.RectButton();
            this.lbButtonCancel = new Mndz.RectButton();
            this.lbButtonPercent = new Mndz.RectButton();
            this.lbButton9 = new Mndz.RectButton();
            this.lbButton8 = new Mndz.RectButton();
            this.lbButton7 = new Mndz.RectButton();
            this.lbButton4 = new Mndz.RectButton();
            this.lbButton5 = new Mndz.RectButton();
            this.lbButton6 = new Mndz.RectButton();
            this.lbButton3 = new Mndz.RectButton();
            this.lbButton2 = new Mndz.RectButton();
            this.lbButton1 = new Mndz.RectButton();
            this.lbButton0 = new Mndz.RectButton();
            this.btn_zeroon = new Mndz.RectButton();
            this.led_current = new DmitryBrant.CustomControls.SevenSegmentArray();
            this.led_setting = new DmitryBrant.CustomControls.SevenSegmentArray();
            this.btn_turnon = new Mndz.RectButton();
            this.SuspendLayout();
            // 
            // lbl_currscale
            // 
            this.lbl_currscale.Font = new System.Drawing.Font("Tahoma", 26F, System.Drawing.FontStyle.Regular);
            this.lbl_currscale.Location = new System.Drawing.Point(937, 115);
            this.lbl_currscale.Name = "lbl_currscale";
            this.lbl_currscale.Size = new System.Drawing.Size(45, 60);
            this.lbl_currscale.Text = "A";
            // 
            // lbl_ohm
            // 
            this.lbl_ohm.Font = new System.Drawing.Font("Tahoma", 26F, System.Drawing.FontStyle.Regular);
            this.lbl_ohm.Location = new System.Drawing.Point(781, 257);
            this.lbl_ohm.Name = "lbl_ohm";
            this.lbl_ohm.Size = new System.Drawing.Size(62, 53);
            this.lbl_ohm.Text = "A";
            // 
            // label3
            // 
            this.label3.Font = new System.Drawing.Font("KaiTi", 36F, System.Drawing.FontStyle.Regular);
            this.label3.ForeColor = System.Drawing.Color.MidnightBlue;
            this.label3.Location = new System.Drawing.Point(136, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(736, 55);
            this.label3.Text = "300A 精密直流大功率恒流源";
            this.label3.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // label4
            // 
            this.label4.Font = new System.Drawing.Font("KaiTi", 28F, System.Drawing.FontStyle.Bold);
            this.label4.Location = new System.Drawing.Point(-3, 115);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(177, 44);
            this.label4.Text = "设定电流";
            this.label4.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // lbl_version
            // 
            this.lbl_version.Font = new System.Drawing.Font("Arial", 8F, System.Drawing.FontStyle.Regular);
            this.lbl_version.Location = new System.Drawing.Point(891, 38);
            this.lbl_version.Name = "lbl_version";
            this.lbl_version.Size = new System.Drawing.Size(114, 17);
            this.lbl_version.Text = "版本:2013-11-22";
            this.lbl_version.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // lbl_datetime
            // 
            this.lbl_datetime.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Regular);
            this.lbl_datetime.Location = new System.Drawing.Point(878, 10);
            this.lbl_datetime.Name = "lbl_datetime";
            this.lbl_datetime.Size = new System.Drawing.Size(146, 18);
            this.lbl_datetime.Text = "2013-05-01 13:45:45";
            // 
            // btn_capture
            // 
            this.btn_capture.Location = new System.Drawing.Point(3, 10);
            this.btn_capture.Name = "btn_capture";
            this.btn_capture.Size = new System.Drawing.Size(111, 26);
            this.btn_capture.TabIndex = 28;
            this.btn_capture.Text = "截屏";
            this.btn_capture.Visible = false;
            this.btn_capture.Click += new System.EventHandler(this.btn_capture_Click);
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("KaiTi", 28F, System.Drawing.FontStyle.Bold);
            this.label1.Location = new System.Drawing.Point(-3, 266);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(177, 44);
            this.label1.Text = "输出电流";
            this.label1.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // rectMeter1
            // 
            this.rectMeter1.BackColor = System.Drawing.Color.LightSkyBlue;
            this.rectMeter1.Location = new System.Drawing.Point(13, 360);
            this.rectMeter1.Name = "rectMeter1";
            this.rectMeter1.Size = new System.Drawing.Size(437, 224);
            this.rectMeter1.TabIndex = 45;
            // 
            // lbButtonPT
            // 
            this.lbButtonPT.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.lbButtonPT.Label = ".";
            this.lbButtonPT.Location = new System.Drawing.Point(781, 534);
            this.lbButtonPT.Name = "lbButtonPT";
            this.lbButtonPT.Size = new System.Drawing.Size(54, 50);
            this.lbButtonPT.TabIndex = 37;
            // 
            // lbButtonOK
            // 
            this.lbButtonOK.Font = new System.Drawing.Font("Arial", 24F, System.Drawing.FontStyle.Bold);
            this.lbButtonOK.Label = "确定";
            this.lbButtonOK.Location = new System.Drawing.Point(909, 478);
            this.lbButtonOK.Name = "lbButtonOK";
            this.lbButtonOK.Size = new System.Drawing.Size(96, 106);
            this.lbButtonOK.TabIndex = 37;
            // 
            // lbButtonCancel
            // 
            this.lbButtonCancel.Font = new System.Drawing.Font("Arial", 24F, System.Drawing.FontStyle.Bold);
            this.lbButtonCancel.Label = "取消";
            this.lbButtonCancel.Location = new System.Drawing.Point(909, 366);
            this.lbButtonCancel.Name = "lbButtonCancel";
            this.lbButtonCancel.Size = new System.Drawing.Size(96, 106);
            this.lbButtonCancel.TabIndex = 37;
            // 
            // lbButtonPercent
            // 
            this.lbButtonPercent.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.lbButtonPercent.Label = "%";
            this.lbButtonPercent.Location = new System.Drawing.Point(841, 534);
            this.lbButtonPercent.Name = "lbButtonPercent";
            this.lbButtonPercent.Size = new System.Drawing.Size(54, 50);
            this.lbButtonPercent.TabIndex = 37;
            // 
            // lbButton9
            // 
            this.lbButton9.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.lbButton9.Label = "9";
            this.lbButton9.Location = new System.Drawing.Point(841, 366);
            this.lbButton9.Name = "lbButton9";
            this.lbButton9.Size = new System.Drawing.Size(54, 50);
            this.lbButton9.TabIndex = 37;
            // 
            // lbButton8
            // 
            this.lbButton8.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.lbButton8.Label = "8";
            this.lbButton8.Location = new System.Drawing.Point(781, 366);
            this.lbButton8.Name = "lbButton8";
            this.lbButton8.Size = new System.Drawing.Size(54, 50);
            this.lbButton8.TabIndex = 37;
            // 
            // lbButton7
            // 
            this.lbButton7.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.lbButton7.Label = "7";
            this.lbButton7.Location = new System.Drawing.Point(721, 366);
            this.lbButton7.Name = "lbButton7";
            this.lbButton7.Size = new System.Drawing.Size(54, 50);
            this.lbButton7.TabIndex = 37;
            // 
            // lbButton4
            // 
            this.lbButton4.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.lbButton4.Label = "4";
            this.lbButton4.Location = new System.Drawing.Point(721, 422);
            this.lbButton4.Name = "lbButton4";
            this.lbButton4.Size = new System.Drawing.Size(54, 50);
            this.lbButton4.TabIndex = 37;
            // 
            // lbButton5
            // 
            this.lbButton5.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.lbButton5.Label = "5";
            this.lbButton5.Location = new System.Drawing.Point(781, 422);
            this.lbButton5.Name = "lbButton5";
            this.lbButton5.Size = new System.Drawing.Size(54, 50);
            this.lbButton5.TabIndex = 37;
            // 
            // lbButton6
            // 
            this.lbButton6.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.lbButton6.Label = "6";
            this.lbButton6.Location = new System.Drawing.Point(841, 422);
            this.lbButton6.Name = "lbButton6";
            this.lbButton6.Size = new System.Drawing.Size(54, 50);
            this.lbButton6.TabIndex = 37;
            // 
            // lbButton3
            // 
            this.lbButton3.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.lbButton3.Label = "3";
            this.lbButton3.Location = new System.Drawing.Point(841, 478);
            this.lbButton3.Name = "lbButton3";
            this.lbButton3.Size = new System.Drawing.Size(54, 50);
            this.lbButton3.TabIndex = 37;
            // 
            // lbButton2
            // 
            this.lbButton2.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.lbButton2.Label = "2";
            this.lbButton2.Location = new System.Drawing.Point(781, 478);
            this.lbButton2.Name = "lbButton2";
            this.lbButton2.Size = new System.Drawing.Size(54, 50);
            this.lbButton2.TabIndex = 37;
            // 
            // lbButton1
            // 
            this.lbButton1.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.lbButton1.Label = "1";
            this.lbButton1.Location = new System.Drawing.Point(721, 478);
            this.lbButton1.Name = "lbButton1";
            this.lbButton1.Size = new System.Drawing.Size(54, 50);
            this.lbButton1.TabIndex = 37;
            // 
            // lbButton0
            // 
            this.lbButton0.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.lbButton0.Label = "0";
            this.lbButton0.Location = new System.Drawing.Point(721, 534);
            this.lbButton0.Name = "lbButton0";
            this.lbButton0.Size = new System.Drawing.Size(54, 50);
            this.lbButton0.TabIndex = 37;
            // 
            // btn_zeroon
            // 
            this.btn_zeroon.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.btn_zeroon.Label = "电流表清零";
            this.btn_zeroon.Location = new System.Drawing.Point(862, 257);
            this.btn_zeroon.Name = "btn_zeroon";
            this.btn_zeroon.Size = new System.Drawing.Size(143, 53);
            this.btn_zeroon.TabIndex = 10;
            // 
            // led_current
            // 
            this.led_current.ArrayCount = 7;
            this.led_current.Location = new System.Drawing.Point(207, 232);
            this.led_current.Name = "led_current";
            this.led_current.Size = new System.Drawing.Size(558, 109);
            this.led_current.TabIndex = 1;
            // 
            // led_setting
            // 
            this.led_setting.ArrayCount = 7;
            this.led_setting.DecimalShow = true;
            this.led_setting.ElementWidth = 10;
            this.led_setting.Location = new System.Drawing.Point(174, 58);
            this.led_setting.Name = "led_setting";
            this.led_setting.Size = new System.Drawing.Size(760, 157);
            this.led_setting.TabIndex = 1;
            // 
            // btn_turnon
            // 
            this.btn_turnon.Font = new System.Drawing.Font("Arial", 14F, System.Drawing.FontStyle.Bold);
            this.btn_turnon.Label = "OFF";
            this.btn_turnon.Location = new System.Drawing.Point(520, 400);
            this.btn_turnon.Name = "btn_turnon";
            this.btn_turnon.Size = new System.Drawing.Size(137, 128);
            this.btn_turnon.TabIndex = 25;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(1024, 600);
            this.ControlBox = false;
            this.Controls.Add(this.label4);
            this.Controls.Add(this.rectMeter1);
            this.Controls.Add(this.lbButtonPT);
            this.Controls.Add(this.lbButtonOK);
            this.Controls.Add(this.lbButtonCancel);
            this.Controls.Add(this.lbButtonPercent);
            this.Controls.Add(this.lbButton9);
            this.Controls.Add(this.lbButton8);
            this.Controls.Add(this.lbButton7);
            this.Controls.Add(this.lbButton4);
            this.Controls.Add(this.lbButton5);
            this.Controls.Add(this.lbButton6);
            this.Controls.Add(this.lbButton3);
            this.Controls.Add(this.lbButton2);
            this.Controls.Add(this.lbButton1);
            this.Controls.Add(this.lbButton0);
            this.Controls.Add(this.btn_capture);
            this.Controls.Add(this.lbl_datetime);
            this.Controls.Add(this.lbl_version);
            this.Controls.Add(this.btn_zeroon);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.lbl_ohm);
            this.Controls.Add(this.lbl_currscale);
            this.Controls.Add(this.led_current);
            this.Controls.Add(this.led_setting);
            this.Controls.Add(this.btn_turnon);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "Form1";
            this.Text = "Form1";
            this.TopMost = true;
            this.ResumeLayout(false);

        }

        #endregion

        private DmitryBrant.CustomControls.SevenSegmentArray led_setting;
        private DmitryBrant.CustomControls.SevenSegmentArray led_current;
        private System.Windows.Forms.Label lbl_currscale;
        private System.Windows.Forms.Label lbl_ohm;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private RectButton btn_turnon;
        private System.Windows.Forms.Label lbl_version;
        private System.Windows.Forms.Label lbl_datetime;
        private RectButton btn_zeroon;
        private System.Windows.Forms.Button btn_capture;
        private System.Windows.Forms.Label label1;
        private RectButton lbButton0;
        private RectButton lbButton1;
        private RectButton lbButton2;
        private RectButton lbButton3;
        private RectButton lbButton6;
        private RectButton lbButton5;
        private RectButton lbButton4;
        private RectButton lbButton7;
        private RectButton lbButton8;
        private RectButton lbButton9;
        private RectButton lbButtonPT;
        private RectButton lbButtonPercent;
        private RectButton lbButtonCancel;
        private RectButton lbButtonOK;
        private Mndz.MyCtrl.RectMeter rectMeter1;
    }
}

