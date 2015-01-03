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
            this.rbtn_up7 = new Mndz.RectButton();
            this.rbtn_up6 = new Mndz.RectButton();
            this.rbtn_up5 = new Mndz.RectButton();
            this.rbtn_up4 = new Mndz.RectButton();
            this.rbtn_up3 = new Mndz.RectButton();
            this.rbtn_up2 = new Mndz.RectButton();
            this.rbtn_up1 = new Mndz.RectButton();
            this.rbtn_dn1 = new Mndz.RectButton();
            this.rbtn_dn2 = new Mndz.RectButton();
            this.rbtn_dn3 = new Mndz.RectButton();
            this.rbtn_dn4 = new Mndz.RectButton();
            this.rbtn_dn5 = new Mndz.RectButton();
            this.rbtn_dn6 = new Mndz.RectButton();
            this.rbtn_dn7 = new Mndz.RectButton();
            this.rngbtn_1 = new Mndz.RectButton();
            this.rngbtn_10 = new Mndz.RectButton();
            this.rngbtn_100 = new Mndz.RectButton();
            this.rngbtn_300 = new Mndz.RectButton();
            this.rngbtn_600 = new Mndz.RectButton();
            this.rngbtn_1000 = new Mndz.RectButton();
            this.SuspendLayout();
            // 
            // lbl_currscale
            // 
            this.lbl_currscale.Font = new System.Drawing.Font("KaiTi", 26F, System.Drawing.FontStyle.Bold);
            this.lbl_currscale.Location = new System.Drawing.Point(581, 317);
            this.lbl_currscale.Name = "lbl_currscale";
            this.lbl_currscale.Size = new System.Drawing.Size(103, 44);
            this.lbl_currscale.Text = "量程";
            // 
            // label3
            // 
            this.label3.Font = new System.Drawing.Font("KaiTi", 36F, System.Drawing.FontStyle.Regular);
            this.label3.ForeColor = System.Drawing.Color.MidnightBlue;
            this.label3.Location = new System.Drawing.Point(136, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(736, 55);
            this.label3.Text = "程控300A直流稳流源";
            this.label3.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // label4
            // 
            this.label4.Font = new System.Drawing.Font("KaiTi", 28F, System.Drawing.FontStyle.Bold);
            this.label4.Location = new System.Drawing.Point(198, 317);
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
            this.lbl_version.Text = "版本:2014-08-09";
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
            this.label1.Location = new System.Drawing.Point(401, 257);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(259, 44);
            this.label1.Text = "输出电流(A)";
            this.label1.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // rectMeter1
            // 
            this.rectMeter1.BackColor = System.Drawing.Color.LightSkyBlue;
            this.rectMeter1.Location = new System.Drawing.Point(1, 119);
            this.rectMeter1.Name = "rectMeter1";
            this.rectMeter1.Size = new System.Drawing.Size(171, 89);
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
            this.lbButtonOK.Label = "设定";
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
            this.btn_zeroon.Location = new System.Drawing.Point(42, 249);
            this.btn_zeroon.Name = "btn_zeroon";
            this.btn_zeroon.Size = new System.Drawing.Size(105, 33);
            this.btn_zeroon.TabIndex = 10;
            // 
            // led_current
            // 
            this.led_current.ArrayCount = 7;
            this.led_current.ElementWidth = 12;
            this.led_current.Location = new System.Drawing.Point(178, 68);
            this.led_current.Name = "led_current";
            this.led_current.Size = new System.Drawing.Size(686, 177);
            this.led_current.TabIndex = 1;
            // 
            // led_setting
            // 
            this.led_setting.ArrayCount = 7;
            this.led_setting.DecimalShow = true;
            this.led_setting.ElementWidth = 10;
            this.led_setting.Location = new System.Drawing.Point(18, 411);
            this.led_setting.Name = "led_setting";
            this.led_setting.Size = new System.Drawing.Size(538, 117);
            this.led_setting.TabIndex = 1;
            // 
            // btn_turnon
            // 
            this.btn_turnon.Font = new System.Drawing.Font("Arial", 14F, System.Drawing.FontStyle.Bold);
            this.btn_turnon.Label = "OFF";
            this.btn_turnon.Location = new System.Drawing.Point(868, 78);
            this.btn_turnon.Name = "btn_turnon";
            this.btn_turnon.Size = new System.Drawing.Size(146, 146);
            this.btn_turnon.TabIndex = 25;
            // 
            // rbtn_up7
            // 
            this.rbtn_up7.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.rbtn_up7.Label = "电流表清零";
            this.rbtn_up7.Location = new System.Drawing.Point(18, 366);
            this.rbtn_up7.Name = "rbtn_up7";
            this.rbtn_up7.Size = new System.Drawing.Size(60, 39);
            this.rbtn_up7.TabIndex = 52;
            // 
            // rbtn_up6
            // 
            this.rbtn_up6.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.rbtn_up6.Label = "电流表清零";
            this.rbtn_up6.Location = new System.Drawing.Point(95, 366);
            this.rbtn_up6.Name = "rbtn_up6";
            this.rbtn_up6.Size = new System.Drawing.Size(60, 39);
            this.rbtn_up6.TabIndex = 53;
            // 
            // rbtn_up5
            // 
            this.rbtn_up5.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.rbtn_up5.Label = "电流表清零";
            this.rbtn_up5.Location = new System.Drawing.Point(168, 366);
            this.rbtn_up5.Name = "rbtn_up5";
            this.rbtn_up5.Size = new System.Drawing.Size(60, 39);
            this.rbtn_up5.TabIndex = 54;
            // 
            // rbtn_up4
            // 
            this.rbtn_up4.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.rbtn_up4.Label = "电流表清零";
            this.rbtn_up4.Location = new System.Drawing.Point(247, 366);
            this.rbtn_up4.Name = "rbtn_up4";
            this.rbtn_up4.Size = new System.Drawing.Size(60, 39);
            this.rbtn_up4.TabIndex = 55;
            // 
            // rbtn_up3
            // 
            this.rbtn_up3.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.rbtn_up3.Label = "电流表清零";
            this.rbtn_up3.Location = new System.Drawing.Point(325, 366);
            this.rbtn_up3.Name = "rbtn_up3";
            this.rbtn_up3.Size = new System.Drawing.Size(60, 39);
            this.rbtn_up3.TabIndex = 56;
            // 
            // rbtn_up2
            // 
            this.rbtn_up2.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.rbtn_up2.Label = "电流表清零";
            this.rbtn_up2.Location = new System.Drawing.Point(401, 366);
            this.rbtn_up2.Name = "rbtn_up2";
            this.rbtn_up2.Size = new System.Drawing.Size(60, 39);
            this.rbtn_up2.TabIndex = 57;
            // 
            // rbtn_up1
            // 
            this.rbtn_up1.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.rbtn_up1.Label = "电流表清零";
            this.rbtn_up1.Location = new System.Drawing.Point(479, 366);
            this.rbtn_up1.Name = "rbtn_up1";
            this.rbtn_up1.Size = new System.Drawing.Size(60, 39);
            this.rbtn_up1.TabIndex = 58;
            // 
            // rbtn_dn1
            // 
            this.rbtn_dn1.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.rbtn_dn1.Label = "电流表清零";
            this.rbtn_dn1.Location = new System.Drawing.Point(479, 534);
            this.rbtn_dn1.Name = "rbtn_dn1";
            this.rbtn_dn1.Size = new System.Drawing.Size(60, 39);
            this.rbtn_dn1.TabIndex = 65;
            // 
            // rbtn_dn2
            // 
            this.rbtn_dn2.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.rbtn_dn2.Label = "电流表清零";
            this.rbtn_dn2.Location = new System.Drawing.Point(401, 534);
            this.rbtn_dn2.Name = "rbtn_dn2";
            this.rbtn_dn2.Size = new System.Drawing.Size(60, 39);
            this.rbtn_dn2.TabIndex = 64;
            // 
            // rbtn_dn3
            // 
            this.rbtn_dn3.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.rbtn_dn3.Label = "电流表清零";
            this.rbtn_dn3.Location = new System.Drawing.Point(326, 534);
            this.rbtn_dn3.Name = "rbtn_dn3";
            this.rbtn_dn3.Size = new System.Drawing.Size(60, 39);
            this.rbtn_dn3.TabIndex = 63;
            // 
            // rbtn_dn4
            // 
            this.rbtn_dn4.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.rbtn_dn4.Label = "电流表清零";
            this.rbtn_dn4.Location = new System.Drawing.Point(247, 534);
            this.rbtn_dn4.Name = "rbtn_dn4";
            this.rbtn_dn4.Size = new System.Drawing.Size(60, 39);
            this.rbtn_dn4.TabIndex = 62;
            // 
            // rbtn_dn5
            // 
            this.rbtn_dn5.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.rbtn_dn5.Label = "电流表清零";
            this.rbtn_dn5.Location = new System.Drawing.Point(168, 534);
            this.rbtn_dn5.Name = "rbtn_dn5";
            this.rbtn_dn5.Size = new System.Drawing.Size(60, 39);
            this.rbtn_dn5.TabIndex = 61;
            // 
            // rbtn_dn6
            // 
            this.rbtn_dn6.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.rbtn_dn6.Label = "电流表清零";
            this.rbtn_dn6.Location = new System.Drawing.Point(95, 534);
            this.rbtn_dn6.Name = "rbtn_dn6";
            this.rbtn_dn6.Size = new System.Drawing.Size(60, 39);
            this.rbtn_dn6.TabIndex = 60;
            // 
            // rbtn_dn7
            // 
            this.rbtn_dn7.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.rbtn_dn7.Label = "电流表清零";
            this.rbtn_dn7.Location = new System.Drawing.Point(18, 534);
            this.rbtn_dn7.Name = "rbtn_dn7";
            this.rbtn_dn7.Size = new System.Drawing.Size(60, 39);
            this.rbtn_dn7.TabIndex = 59;
            // 
            // rngbtn_1
            // 
            this.rngbtn_1.BackColor = System.Drawing.Color.Transparent;
            this.rngbtn_1.bgScale = 3;
            this.rngbtn_1.bOn = true;
            this.rngbtn_1.colorShadow = System.Drawing.Color.LightGray;
            this.rngbtn_1.colorTop = System.Drawing.Color.Pink;
            this.rngbtn_1.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.rngbtn_1.Label = "  1 A";
            this.rngbtn_1.Location = new System.Drawing.Point(581, 366);
            this.rngbtn_1.Name = "rngbtn_1";
            this.rngbtn_1.Size = new System.Drawing.Size(100, 34);
            this.rngbtn_1.TabIndex = 66;
            // 
            // rngbtn_10
            // 
            this.rngbtn_10.BackColor = System.Drawing.Color.Transparent;
            this.rngbtn_10.bgScale = 3;
            this.rngbtn_10.bOn = true;
            this.rngbtn_10.colorShadow = System.Drawing.Color.LightGray;
            this.rngbtn_10.colorTop = System.Drawing.Color.Pink;
            this.rngbtn_10.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.rngbtn_10.Label = " 10 A";
            this.rngbtn_10.Location = new System.Drawing.Point(581, 406);
            this.rngbtn_10.Name = "rngbtn_10";
            this.rngbtn_10.Size = new System.Drawing.Size(100, 34);
            this.rngbtn_10.TabIndex = 67;
            // 
            // rngbtn_100
            // 
            this.rngbtn_100.BackColor = System.Drawing.Color.Transparent;
            this.rngbtn_100.bgScale = 3;
            this.rngbtn_100.bOn = true;
            this.rngbtn_100.colorShadow = System.Drawing.Color.LightGray;
            this.rngbtn_100.colorTop = System.Drawing.Color.Pink;
            this.rngbtn_100.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.rngbtn_100.Label = "100 A";
            this.rngbtn_100.Location = new System.Drawing.Point(581, 446);
            this.rngbtn_100.Name = "rngbtn_100";
            this.rngbtn_100.Size = new System.Drawing.Size(100, 34);
            this.rngbtn_100.TabIndex = 68;
            // 
            // rngbtn_300
            // 
            this.rngbtn_300.BackColor = System.Drawing.Color.Transparent;
            this.rngbtn_300.bgScale = 3;
            this.rngbtn_300.bOn = true;
            this.rngbtn_300.colorShadow = System.Drawing.Color.LightGray;
            this.rngbtn_300.colorTop = System.Drawing.Color.Pink;
            this.rngbtn_300.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.rngbtn_300.Label = "300 A";
            this.rngbtn_300.Location = new System.Drawing.Point(581, 486);
            this.rngbtn_300.Name = "rngbtn_300";
            this.rngbtn_300.Size = new System.Drawing.Size(100, 34);
            this.rngbtn_300.TabIndex = 69;
            // 
            // rngbtn_600
            // 
            this.rngbtn_600.BackColor = System.Drawing.Color.Transparent;
            this.rngbtn_600.bgScale = 3;
            this.rngbtn_600.bOn = true;
            this.rngbtn_600.colorShadow = System.Drawing.Color.LightGray;
            this.rngbtn_600.colorTop = System.Drawing.Color.Pink;
            this.rngbtn_600.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.rngbtn_600.Label = "600 A";
            this.rngbtn_600.Location = new System.Drawing.Point(581, 526);
            this.rngbtn_600.Name = "rngbtn_600";
            this.rngbtn_600.Size = new System.Drawing.Size(100, 34);
            this.rngbtn_600.TabIndex = 70;
            // 
            // rngbtn_1000
            // 
            this.rngbtn_1000.BackColor = System.Drawing.Color.Transparent;
            this.rngbtn_1000.bgScale = 3;
            this.rngbtn_1000.bOn = true;
            this.rngbtn_1000.colorShadow = System.Drawing.Color.LightGray;
            this.rngbtn_1000.colorTop = System.Drawing.Color.Pink;
            this.rngbtn_1000.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.rngbtn_1000.Label = "1000 A";
            this.rngbtn_1000.Location = new System.Drawing.Point(581, 566);
            this.rngbtn_1000.Name = "rngbtn_1000";
            this.rngbtn_1000.Size = new System.Drawing.Size(100, 34);
            this.rngbtn_1000.TabIndex = 77;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(1024, 600);
            this.ControlBox = false;
            this.Controls.Add(this.rngbtn_1000);
            this.Controls.Add(this.rngbtn_600);
            this.Controls.Add(this.rngbtn_300);
            this.Controls.Add(this.rngbtn_100);
            this.Controls.Add(this.rngbtn_10);
            this.Controls.Add(this.rngbtn_1);
            this.Controls.Add(this.rbtn_dn1);
            this.Controls.Add(this.rbtn_dn2);
            this.Controls.Add(this.rbtn_dn3);
            this.Controls.Add(this.rbtn_dn4);
            this.Controls.Add(this.rbtn_dn5);
            this.Controls.Add(this.rbtn_dn6);
            this.Controls.Add(this.rbtn_dn7);
            this.Controls.Add(this.rbtn_up1);
            this.Controls.Add(this.rbtn_up2);
            this.Controls.Add(this.rbtn_up3);
            this.Controls.Add(this.rbtn_up4);
            this.Controls.Add(this.rbtn_up5);
            this.Controls.Add(this.rbtn_up6);
            this.Controls.Add(this.rbtn_up7);
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
        private RectButton rbtn_up7;
        private RectButton rbtn_up6;
        private RectButton rbtn_up5;
        private RectButton rbtn_up4;
        private RectButton rbtn_up3;
        private RectButton rbtn_up2;
        private RectButton rbtn_up1;
        private RectButton rbtn_dn1;
        private RectButton rbtn_dn2;
        private RectButton rbtn_dn3;
        private RectButton rbtn_dn4;
        private RectButton rbtn_dn5;
        private RectButton rbtn_dn6;
        private RectButton rbtn_dn7;
        private RectButton rngbtn_1;
        private RectButton rngbtn_10;
        private RectButton rngbtn_100;
        private RectButton rngbtn_300;
        private RectButton rngbtn_600;
        private RectButton rngbtn_1000;
    }
}

