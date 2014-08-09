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
            this.led_current = new DmitryBrant.CustomControls.SevenSegmentArray();
            this.led_setting = new DmitryBrant.CustomControls.SevenSegmentArray();
            this.lbl_datetime = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.btn_zeroon = new Mndz.RectButton();
            this.lbButton1 = new Mndz.RectButton();
            this.lbButton2 = new Mndz.RectButton();
            this.lbButton3 = new Mndz.RectButton();
            this.lbButton4 = new Mndz.RectButton();
            this.lbButton5 = new Mndz.RectButton();
            this.lbButton6 = new Mndz.RectButton();
            this.lbButton7 = new Mndz.RectButton();
            this.lbButton8 = new Mndz.RectButton();
            this.lbButton9 = new Mndz.RectButton();
            this.lbButton0 = new Mndz.RectButton();
            this.lbButtonPT = new Mndz.RectButton();
            this.lbButtonCancel = new Mndz.RectButton();
            this.lbButtonOK = new Mndz.RectButton();
            this.lbButtonPercent = new Mndz.RectButton();
            this.label2 = new System.Windows.Forms.Label();
            this.btn_turnon = new Mndz.RectButton();
            this.rectMeter1 = new Mndz.MyCtrl.RectMeter();
            this.rbtn_up2 = new Mndz.RectButton();
            this.rbtn_up3 = new Mndz.RectButton();
            this.rbtn_up4 = new Mndz.RectButton();
            this.rbtn_up5 = new Mndz.RectButton();
            this.rbtn_up6 = new Mndz.RectButton();
            this.rbtn_up7 = new Mndz.RectButton();
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
            this.label4 = new System.Windows.Forms.Label();
            this.rngbtn_1000 = new Mndz.RectButton();
            this.SuspendLayout();
            // 
            // lbl_range
            // 
            this.lbl_range.AutoSize = true;
            this.lbl_range.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.lbl_range.Location = new System.Drawing.Point(420, 249);
            this.lbl_range.Name = "lbl_range";
            this.lbl_range.Size = new System.Drawing.Size(205, 39);
            this.lbl_range.TabIndex = 6;
            this.lbl_range.Text = "输出电流(A)";
            // 
            // led_current
            // 
            this.led_current.ArrayCount = 7;
            this.led_current.ColorBackground = System.Drawing.Color.White;
            this.led_current.ColorDark = System.Drawing.Color.WhiteSmoke;
            this.led_current.ColorLight = System.Drawing.Color.Lime;
            this.led_current.DecimalShow = true;
            this.led_current.ElementPadding = ((Mndz.Padding)(resources.GetObject("led_current.ElementPadding")));
            this.led_current.ElementWidth = 10;
            this.led_current.ItalicFactor = 0F;
            this.led_current.Location = new System.Drawing.Point(178, 68);
            this.led_current.Name = "led_current";
            this.led_current.Size = new System.Drawing.Size(686, 164);
            this.led_current.TabIndex = 5;
            this.led_current.TabStop = false;
            this.led_current.Value = null;
            // 
            // led_setting
            // 
            this.led_setting.ArrayCount = 7;
            this.led_setting.ColorBackground = System.Drawing.Color.White;
            this.led_setting.ColorDark = System.Drawing.Color.LightSkyBlue;
            this.led_setting.ColorLight = System.Drawing.Color.Red;
            this.led_setting.DecimalShow = true;
            this.led_setting.ElementPadding = ((Mndz.Padding)(resources.GetObject("led_setting.ElementPadding")));
            this.led_setting.ElementWidth = 5;
            this.led_setting.ItalicFactor = -0.05F;
            this.led_setting.Location = new System.Drawing.Point(10, 392);
            this.led_setting.Margin = new System.Windows.Forms.Padding(1);
            this.led_setting.Name = "led_setting";
            this.led_setting.Size = new System.Drawing.Size(538, 117);
            this.led_setting.TabIndex = 1;
            this.led_setting.TabStop = false;
            this.led_setting.Value = null;
            // 
            // lbl_datetime
            // 
            this.lbl_datetime.AutoSize = true;
            this.lbl_datetime.Location = new System.Drawing.Point(820, 9);
            this.lbl_datetime.Name = "lbl_datetime";
            this.lbl_datetime.Size = new System.Drawing.Size(41, 15);
            this.lbl_datetime.TabIndex = 10;
            this.lbl_datetime.Text = "label2";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 27.80198F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.MediumBlue;
            this.label1.Location = new System.Drawing.Point(249, -3);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(550, 44);
            this.label1.TabIndex = 0;
            this.label1.Text = "1000A 精密直流大功率恒流源";
            // 
            // btn_zeroon
            // 
            this.btn_zeroon.BackColor = System.Drawing.Color.Transparent;
            this.btn_zeroon.bgScale = 3;
            this.btn_zeroon.bOn = true;
            this.btn_zeroon.colorShadow = System.Drawing.Color.LightGray;
            this.btn_zeroon.colorTop = System.Drawing.Color.Pink;
            this.btn_zeroon.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.980198F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_zeroon.IsButtonUp = true;
            this.btn_zeroon.Label = "电流表清零";
            this.btn_zeroon.Location = new System.Drawing.Point(42, 249);
            this.btn_zeroon.Margin = new System.Windows.Forms.Padding(6);
            this.btn_zeroon.Name = "btn_zeroon";
            this.btn_zeroon.Size = new System.Drawing.Size(105, 33);
            this.btn_zeroon.Style = Mndz.MyButtonType.rectButton;
            this.btn_zeroon.TabIndex = 11;
            // 
            // lbButton1
            // 
            this.lbButton1.BackColor = System.Drawing.Color.Transparent;
            this.lbButton1.bgScale = 3;
            this.lbButton1.bOn = true;
            this.lbButton1.colorShadow = System.Drawing.Color.LightGray;
            this.lbButton1.colorTop = System.Drawing.Color.Pink;
            this.lbButton1.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbButton1.IsButtonUp = true;
            this.lbButton1.Label = null;
            this.lbButton1.Location = new System.Drawing.Point(705, 439);
            this.lbButton1.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.lbButton1.Name = "lbButton1";
            this.lbButton1.Size = new System.Drawing.Size(63, 57);
            this.lbButton1.Style = Mndz.MyButtonType.roundButton;
            this.lbButton1.TabIndex = 14;
            // 
            // lbButton2
            // 
            this.lbButton2.BackColor = System.Drawing.Color.Transparent;
            this.lbButton2.bgScale = 3;
            this.lbButton2.bOn = true;
            this.lbButton2.colorShadow = System.Drawing.Color.LightGray;
            this.lbButton2.colorTop = System.Drawing.Color.Pink;
            this.lbButton2.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbButton2.IsButtonUp = true;
            this.lbButton2.Label = null;
            this.lbButton2.Location = new System.Drawing.Point(774, 439);
            this.lbButton2.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.lbButton2.Name = "lbButton2";
            this.lbButton2.Size = new System.Drawing.Size(63, 57);
            this.lbButton2.Style = Mndz.MyButtonType.roundButton;
            this.lbButton2.TabIndex = 14;
            // 
            // lbButton3
            // 
            this.lbButton3.BackColor = System.Drawing.Color.Transparent;
            this.lbButton3.bgScale = 3;
            this.lbButton3.bOn = true;
            this.lbButton3.colorShadow = System.Drawing.Color.LightGray;
            this.lbButton3.colorTop = System.Drawing.Color.Pink;
            this.lbButton3.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbButton3.IsButtonUp = true;
            this.lbButton3.Label = null;
            this.lbButton3.Location = new System.Drawing.Point(843, 439);
            this.lbButton3.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.lbButton3.Name = "lbButton3";
            this.lbButton3.Size = new System.Drawing.Size(63, 57);
            this.lbButton3.Style = Mndz.MyButtonType.roundButton;
            this.lbButton3.TabIndex = 14;
            // 
            // lbButton4
            // 
            this.lbButton4.BackColor = System.Drawing.Color.Transparent;
            this.lbButton4.bgScale = 3;
            this.lbButton4.bOn = true;
            this.lbButton4.colorShadow = System.Drawing.Color.LightGray;
            this.lbButton4.colorTop = System.Drawing.Color.Pink;
            this.lbButton4.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbButton4.IsButtonUp = true;
            this.lbButton4.Label = null;
            this.lbButton4.Location = new System.Drawing.Point(705, 379);
            this.lbButton4.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.lbButton4.Name = "lbButton4";
            this.lbButton4.Size = new System.Drawing.Size(63, 57);
            this.lbButton4.Style = Mndz.MyButtonType.roundButton;
            this.lbButton4.TabIndex = 14;
            // 
            // lbButton5
            // 
            this.lbButton5.BackColor = System.Drawing.Color.Transparent;
            this.lbButton5.bgScale = 3;
            this.lbButton5.bOn = true;
            this.lbButton5.colorShadow = System.Drawing.Color.LightGray;
            this.lbButton5.colorTop = System.Drawing.Color.Pink;
            this.lbButton5.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbButton5.IsButtonUp = true;
            this.lbButton5.Label = null;
            this.lbButton5.Location = new System.Drawing.Point(774, 379);
            this.lbButton5.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.lbButton5.Name = "lbButton5";
            this.lbButton5.Size = new System.Drawing.Size(63, 57);
            this.lbButton5.Style = Mndz.MyButtonType.roundButton;
            this.lbButton5.TabIndex = 14;
            // 
            // lbButton6
            // 
            this.lbButton6.BackColor = System.Drawing.Color.Transparent;
            this.lbButton6.bgScale = 3;
            this.lbButton6.bOn = true;
            this.lbButton6.colorShadow = System.Drawing.Color.LightGray;
            this.lbButton6.colorTop = System.Drawing.Color.Pink;
            this.lbButton6.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbButton6.IsButtonUp = true;
            this.lbButton6.Label = null;
            this.lbButton6.Location = new System.Drawing.Point(843, 379);
            this.lbButton6.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.lbButton6.Name = "lbButton6";
            this.lbButton6.Size = new System.Drawing.Size(63, 57);
            this.lbButton6.Style = Mndz.MyButtonType.roundButton;
            this.lbButton6.TabIndex = 14;
            // 
            // lbButton7
            // 
            this.lbButton7.BackColor = System.Drawing.Color.Transparent;
            this.lbButton7.bgScale = 3;
            this.lbButton7.bOn = true;
            this.lbButton7.colorShadow = System.Drawing.Color.Black;
            this.lbButton7.colorTop = System.Drawing.Color.Pink;
            this.lbButton7.Font = new System.Drawing.Font("Arial", 12.11881F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbButton7.IsButtonUp = true;
            this.lbButton7.Label = "7";
            this.lbButton7.Location = new System.Drawing.Point(705, 319);
            this.lbButton7.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.lbButton7.Name = "lbButton7";
            this.lbButton7.Size = new System.Drawing.Size(63, 57);
            this.lbButton7.Style = Mndz.MyButtonType.raiseButton;
            this.lbButton7.TabIndex = 14;
            // 
            // lbButton8
            // 
            this.lbButton8.BackColor = System.Drawing.Color.Transparent;
            this.lbButton8.bgScale = 3;
            this.lbButton8.bOn = true;
            this.lbButton8.colorShadow = System.Drawing.Color.LightGray;
            this.lbButton8.colorTop = System.Drawing.Color.Pink;
            this.lbButton8.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbButton8.IsButtonUp = true;
            this.lbButton8.Label = null;
            this.lbButton8.Location = new System.Drawing.Point(774, 319);
            this.lbButton8.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.lbButton8.Name = "lbButton8";
            this.lbButton8.Size = new System.Drawing.Size(63, 57);
            this.lbButton8.Style = Mndz.MyButtonType.roundButton;
            this.lbButton8.TabIndex = 14;
            // 
            // lbButton9
            // 
            this.lbButton9.BackColor = System.Drawing.Color.Transparent;
            this.lbButton9.bgScale = 3;
            this.lbButton9.bOn = true;
            this.lbButton9.colorShadow = System.Drawing.Color.LightGray;
            this.lbButton9.colorTop = System.Drawing.Color.Pink;
            this.lbButton9.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbButton9.IsButtonUp = true;
            this.lbButton9.Label = null;
            this.lbButton9.Location = new System.Drawing.Point(843, 319);
            this.lbButton9.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.lbButton9.Name = "lbButton9";
            this.lbButton9.Size = new System.Drawing.Size(63, 57);
            this.lbButton9.Style = Mndz.MyButtonType.roundButton;
            this.lbButton9.TabIndex = 14;
            // 
            // lbButton0
            // 
            this.lbButton0.BackColor = System.Drawing.Color.Transparent;
            this.lbButton0.bgScale = 3;
            this.lbButton0.bOn = true;
            this.lbButton0.colorShadow = System.Drawing.Color.LightGray;
            this.lbButton0.colorTop = System.Drawing.Color.Pink;
            this.lbButton0.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbButton0.IsButtonUp = true;
            this.lbButton0.Label = null;
            this.lbButton0.Location = new System.Drawing.Point(705, 499);
            this.lbButton0.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.lbButton0.Name = "lbButton0";
            this.lbButton0.Size = new System.Drawing.Size(63, 57);
            this.lbButton0.Style = Mndz.MyButtonType.roundButton;
            this.lbButton0.TabIndex = 14;
            // 
            // lbButtonPT
            // 
            this.lbButtonPT.BackColor = System.Drawing.Color.Transparent;
            this.lbButtonPT.bgScale = 3;
            this.lbButtonPT.bOn = true;
            this.lbButtonPT.colorShadow = System.Drawing.Color.LightGray;
            this.lbButtonPT.colorTop = System.Drawing.Color.Pink;
            this.lbButtonPT.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbButtonPT.IsButtonUp = true;
            this.lbButtonPT.Label = null;
            this.lbButtonPT.Location = new System.Drawing.Point(774, 499);
            this.lbButtonPT.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.lbButtonPT.Name = "lbButtonPT";
            this.lbButtonPT.Size = new System.Drawing.Size(63, 57);
            this.lbButtonPT.Style = Mndz.MyButtonType.roundButton;
            this.lbButtonPT.TabIndex = 14;
            // 
            // lbButtonCancel
            // 
            this.lbButtonCancel.BackColor = System.Drawing.Color.Transparent;
            this.lbButtonCancel.bgScale = 3;
            this.lbButtonCancel.bOn = true;
            this.lbButtonCancel.colorShadow = System.Drawing.Color.LightGray;
            this.lbButtonCancel.colorTop = System.Drawing.Color.Pink;
            this.lbButtonCancel.Font = new System.Drawing.Font("Arial", 22.09901F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbButtonCancel.IsButtonUp = true;
            this.lbButtonCancel.Label = "取消";
            this.lbButtonCancel.Location = new System.Drawing.Point(912, 319);
            this.lbButtonCancel.Margin = new System.Windows.Forms.Padding(15, 14, 15, 14);
            this.lbButtonCancel.Name = "lbButtonCancel";
            this.lbButtonCancel.Size = new System.Drawing.Size(98, 117);
            this.lbButtonCancel.Style = Mndz.MyButtonType.roundButton;
            this.lbButtonCancel.TabIndex = 14;
            // 
            // lbButtonOK
            // 
            this.lbButtonOK.BackColor = System.Drawing.Color.Transparent;
            this.lbButtonOK.bgScale = 3;
            this.lbButtonOK.bOn = true;
            this.lbButtonOK.colorShadow = System.Drawing.Color.LightGray;
            this.lbButtonOK.colorTop = System.Drawing.Color.Pink;
            this.lbButtonOK.Font = new System.Drawing.Font("Arial", 22.09901F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbButtonOK.IsButtonUp = true;
            this.lbButtonOK.Label = "确定";
            this.lbButtonOK.Location = new System.Drawing.Point(912, 439);
            this.lbButtonOK.Margin = new System.Windows.Forms.Padding(15, 14, 15, 14);
            this.lbButtonOK.Name = "lbButtonOK";
            this.lbButtonOK.Size = new System.Drawing.Size(98, 117);
            this.lbButtonOK.Style = Mndz.MyButtonType.roundButton;
            this.lbButtonOK.TabIndex = 14;
            // 
            // lbButtonPercent
            // 
            this.lbButtonPercent.BackColor = System.Drawing.Color.Transparent;
            this.lbButtonPercent.bgScale = 3;
            this.lbButtonPercent.bOn = true;
            this.lbButtonPercent.colorShadow = System.Drawing.Color.LightGray;
            this.lbButtonPercent.colorTop = System.Drawing.Color.Pink;
            this.lbButtonPercent.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbButtonPercent.IsButtonUp = true;
            this.lbButtonPercent.Label = null;
            this.lbButtonPercent.Location = new System.Drawing.Point(843, 499);
            this.lbButtonPercent.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.lbButtonPercent.Name = "lbButtonPercent";
            this.lbButtonPercent.Size = new System.Drawing.Size(63, 57);
            this.lbButtonPercent.Style = Mndz.MyButtonType.roundButton;
            this.lbButtonPercent.TabIndex = 14;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 22.09901F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(202, 310);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(163, 37);
            this.label2.TabIndex = 6;
            this.label2.Text = "设定值(A)";
            // 
            // btn_turnon
            // 
            this.btn_turnon.BackColor = System.Drawing.Color.Transparent;
            this.btn_turnon.bgScale = 3;
            this.btn_turnon.bOn = true;
            this.btn_turnon.colorShadow = System.Drawing.Color.LightGray;
            this.btn_turnon.colorTop = System.Drawing.Color.Pink;
            this.btn_turnon.Font = new System.Drawing.Font("Microsoft Sans Serif", 36.35643F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_turnon.IsButtonUp = true;
            this.btn_turnon.Label = "ON";
            this.btn_turnon.Location = new System.Drawing.Point(868, 78);
            this.btn_turnon.Margin = new System.Windows.Forms.Padding(15, 14, 15, 14);
            this.btn_turnon.Name = "btn_turnon";
            this.btn_turnon.Size = new System.Drawing.Size(146, 146);
            this.btn_turnon.Style = Mndz.MyButtonType.roundButton;
            this.btn_turnon.TabIndex = 15;
            // 
            // rectMeter1
            // 
            this.rectMeter1.Angle = 45;
            this.rectMeter1.BackgroundImage = global::Mndz.Properties.Resources.BGMETER;
            this.rectMeter1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.rectMeter1.BgResId = "";
            this.rectMeter1.Location = new System.Drawing.Point(1, 119);
            this.rectMeter1.Name = "rectMeter1";
            this.rectMeter1.PointColor = System.Drawing.Color.Yellow;
            this.rectMeter1.Size = new System.Drawing.Size(171, 89);
            this.rectMeter1.TabIndex = 16;
            // 
            // rbtn_up2
            // 
            this.rbtn_up2.BackColor = System.Drawing.Color.Transparent;
            this.rbtn_up2.bgScale = 3;
            this.rbtn_up2.bOn = true;
            this.rbtn_up2.colorShadow = System.Drawing.Color.LightGray;
            this.rbtn_up2.colorTop = System.Drawing.Color.Pink;
            this.rbtn_up2.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbtn_up2.IsButtonUp = true;
            this.rbtn_up2.Label = null;
            this.rbtn_up2.Location = new System.Drawing.Point(392, 347);
            this.rbtn_up2.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.rbtn_up2.Name = "rbtn_up2";
            this.rbtn_up2.Size = new System.Drawing.Size(60, 39);
            this.rbtn_up2.Style = Mndz.MyButtonType.triangleupButton;
            this.rbtn_up2.TabIndex = 18;
            // 
            // rbtn_up3
            // 
            this.rbtn_up3.BackColor = System.Drawing.Color.Transparent;
            this.rbtn_up3.bgScale = 3;
            this.rbtn_up3.bOn = true;
            this.rbtn_up3.colorShadow = System.Drawing.Color.LightGray;
            this.rbtn_up3.colorTop = System.Drawing.Color.Pink;
            this.rbtn_up3.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbtn_up3.IsButtonUp = true;
            this.rbtn_up3.Label = null;
            this.rbtn_up3.Location = new System.Drawing.Point(320, 347);
            this.rbtn_up3.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.rbtn_up3.Name = "rbtn_up3";
            this.rbtn_up3.Size = new System.Drawing.Size(60, 39);
            this.rbtn_up3.Style = Mndz.MyButtonType.triangleupButton;
            this.rbtn_up3.TabIndex = 19;
            // 
            // rbtn_up4
            // 
            this.rbtn_up4.BackColor = System.Drawing.Color.Transparent;
            this.rbtn_up4.bgScale = 3;
            this.rbtn_up4.bOn = true;
            this.rbtn_up4.colorShadow = System.Drawing.Color.LightGray;
            this.rbtn_up4.colorTop = System.Drawing.Color.Pink;
            this.rbtn_up4.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbtn_up4.IsButtonUp = true;
            this.rbtn_up4.Label = null;
            this.rbtn_up4.Location = new System.Drawing.Point(239, 347);
            this.rbtn_up4.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.rbtn_up4.Name = "rbtn_up4";
            this.rbtn_up4.Size = new System.Drawing.Size(60, 39);
            this.rbtn_up4.Style = Mndz.MyButtonType.triangleupButton;
            this.rbtn_up4.TabIndex = 20;
            // 
            // rbtn_up5
            // 
            this.rbtn_up5.BackColor = System.Drawing.Color.Transparent;
            this.rbtn_up5.bgScale = 3;
            this.rbtn_up5.bOn = true;
            this.rbtn_up5.colorShadow = System.Drawing.Color.LightGray;
            this.rbtn_up5.colorTop = System.Drawing.Color.Pink;
            this.rbtn_up5.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbtn_up5.IsButtonUp = true;
            this.rbtn_up5.Label = null;
            this.rbtn_up5.Location = new System.Drawing.Point(160, 347);
            this.rbtn_up5.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.rbtn_up5.Name = "rbtn_up5";
            this.rbtn_up5.Size = new System.Drawing.Size(60, 39);
            this.rbtn_up5.Style = Mndz.MyButtonType.triangleupButton;
            this.rbtn_up5.TabIndex = 21;
            // 
            // rbtn_up6
            // 
            this.rbtn_up6.BackColor = System.Drawing.Color.Transparent;
            this.rbtn_up6.bgScale = 3;
            this.rbtn_up6.bOn = true;
            this.rbtn_up6.colorShadow = System.Drawing.Color.LightGray;
            this.rbtn_up6.colorTop = System.Drawing.Color.Pink;
            this.rbtn_up6.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbtn_up6.IsButtonUp = true;
            this.rbtn_up6.Label = null;
            this.rbtn_up6.Location = new System.Drawing.Point(87, 347);
            this.rbtn_up6.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.rbtn_up6.Name = "rbtn_up6";
            this.rbtn_up6.Size = new System.Drawing.Size(60, 39);
            this.rbtn_up6.Style = Mndz.MyButtonType.triangleupButton;
            this.rbtn_up6.TabIndex = 22;
            // 
            // rbtn_up7
            // 
            this.rbtn_up7.BackColor = System.Drawing.Color.Transparent;
            this.rbtn_up7.bgScale = 3;
            this.rbtn_up7.bOn = true;
            this.rbtn_up7.colorShadow = System.Drawing.Color.LightGray;
            this.rbtn_up7.colorTop = System.Drawing.Color.Pink;
            this.rbtn_up7.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbtn_up7.IsButtonUp = true;
            this.rbtn_up7.Label = null;
            this.rbtn_up7.Location = new System.Drawing.Point(10, 347);
            this.rbtn_up7.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.rbtn_up7.Name = "rbtn_up7";
            this.rbtn_up7.Size = new System.Drawing.Size(60, 39);
            this.rbtn_up7.Style = Mndz.MyButtonType.triangleupButton;
            this.rbtn_up7.TabIndex = 23;
            // 
            // rbtn_up1
            // 
            this.rbtn_up1.BackColor = System.Drawing.Color.Transparent;
            this.rbtn_up1.bgScale = 3;
            this.rbtn_up1.bOn = true;
            this.rbtn_up1.colorShadow = System.Drawing.Color.LightGray;
            this.rbtn_up1.colorTop = System.Drawing.Color.Pink;
            this.rbtn_up1.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbtn_up1.IsButtonUp = true;
            this.rbtn_up1.Label = null;
            this.rbtn_up1.Location = new System.Drawing.Point(471, 347);
            this.rbtn_up1.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.rbtn_up1.Name = "rbtn_up1";
            this.rbtn_up1.Size = new System.Drawing.Size(60, 39);
            this.rbtn_up1.Style = Mndz.MyButtonType.triangleupButton;
            this.rbtn_up1.TabIndex = 24;
            // 
            // rbtn_dn1
            // 
            this.rbtn_dn1.BackColor = System.Drawing.Color.Transparent;
            this.rbtn_dn1.bgScale = 3;
            this.rbtn_dn1.bOn = true;
            this.rbtn_dn1.colorShadow = System.Drawing.Color.LightGray;
            this.rbtn_dn1.colorTop = System.Drawing.Color.Pink;
            this.rbtn_dn1.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbtn_dn1.IsButtonUp = true;
            this.rbtn_dn1.Label = null;
            this.rbtn_dn1.Location = new System.Drawing.Point(471, 514);
            this.rbtn_dn1.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.rbtn_dn1.Name = "rbtn_dn1";
            this.rbtn_dn1.Size = new System.Drawing.Size(60, 39);
            this.rbtn_dn1.Style = Mndz.MyButtonType.trianglednButton;
            this.rbtn_dn1.TabIndex = 26;
            // 
            // rbtn_dn2
            // 
            this.rbtn_dn2.BackColor = System.Drawing.Color.Transparent;
            this.rbtn_dn2.bgScale = 3;
            this.rbtn_dn2.bOn = true;
            this.rbtn_dn2.colorShadow = System.Drawing.Color.LightGray;
            this.rbtn_dn2.colorTop = System.Drawing.Color.Pink;
            this.rbtn_dn2.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbtn_dn2.IsButtonUp = true;
            this.rbtn_dn2.Label = null;
            this.rbtn_dn2.Location = new System.Drawing.Point(392, 514);
            this.rbtn_dn2.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.rbtn_dn2.Name = "rbtn_dn2";
            this.rbtn_dn2.Size = new System.Drawing.Size(60, 39);
            this.rbtn_dn2.Style = Mndz.MyButtonType.trianglednButton;
            this.rbtn_dn2.TabIndex = 27;
            // 
            // rbtn_dn3
            // 
            this.rbtn_dn3.BackColor = System.Drawing.Color.Transparent;
            this.rbtn_dn3.bgScale = 3;
            this.rbtn_dn3.bOn = true;
            this.rbtn_dn3.colorShadow = System.Drawing.Color.LightGray;
            this.rbtn_dn3.colorTop = System.Drawing.Color.Pink;
            this.rbtn_dn3.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbtn_dn3.IsButtonUp = true;
            this.rbtn_dn3.Label = null;
            this.rbtn_dn3.Location = new System.Drawing.Point(320, 514);
            this.rbtn_dn3.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.rbtn_dn3.Name = "rbtn_dn3";
            this.rbtn_dn3.Size = new System.Drawing.Size(60, 39);
            this.rbtn_dn3.Style = Mndz.MyButtonType.trianglednButton;
            this.rbtn_dn3.TabIndex = 28;
            // 
            // rbtn_dn4
            // 
            this.rbtn_dn4.BackColor = System.Drawing.Color.Transparent;
            this.rbtn_dn4.bgScale = 3;
            this.rbtn_dn4.bOn = true;
            this.rbtn_dn4.colorShadow = System.Drawing.Color.LightGray;
            this.rbtn_dn4.colorTop = System.Drawing.Color.Pink;
            this.rbtn_dn4.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbtn_dn4.IsButtonUp = true;
            this.rbtn_dn4.Label = null;
            this.rbtn_dn4.Location = new System.Drawing.Point(239, 514);
            this.rbtn_dn4.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.rbtn_dn4.Name = "rbtn_dn4";
            this.rbtn_dn4.Size = new System.Drawing.Size(60, 39);
            this.rbtn_dn4.Style = Mndz.MyButtonType.trianglednButton;
            this.rbtn_dn4.TabIndex = 29;
            // 
            // rbtn_dn5
            // 
            this.rbtn_dn5.BackColor = System.Drawing.Color.Transparent;
            this.rbtn_dn5.bgScale = 3;
            this.rbtn_dn5.bOn = true;
            this.rbtn_dn5.colorShadow = System.Drawing.Color.LightGray;
            this.rbtn_dn5.colorTop = System.Drawing.Color.Pink;
            this.rbtn_dn5.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbtn_dn5.IsButtonUp = true;
            this.rbtn_dn5.Label = null;
            this.rbtn_dn5.Location = new System.Drawing.Point(160, 514);
            this.rbtn_dn5.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.rbtn_dn5.Name = "rbtn_dn5";
            this.rbtn_dn5.Size = new System.Drawing.Size(60, 39);
            this.rbtn_dn5.Style = Mndz.MyButtonType.trianglednButton;
            this.rbtn_dn5.TabIndex = 30;
            // 
            // rbtn_dn6
            // 
            this.rbtn_dn6.BackColor = System.Drawing.Color.Transparent;
            this.rbtn_dn6.bgScale = 3;
            this.rbtn_dn6.bOn = true;
            this.rbtn_dn6.colorShadow = System.Drawing.Color.LightGray;
            this.rbtn_dn6.colorTop = System.Drawing.Color.Pink;
            this.rbtn_dn6.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbtn_dn6.IsButtonUp = true;
            this.rbtn_dn6.Label = null;
            this.rbtn_dn6.Location = new System.Drawing.Point(87, 514);
            this.rbtn_dn6.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.rbtn_dn6.Name = "rbtn_dn6";
            this.rbtn_dn6.Size = new System.Drawing.Size(60, 39);
            this.rbtn_dn6.Style = Mndz.MyButtonType.trianglednButton;
            this.rbtn_dn6.TabIndex = 31;
            // 
            // rbtn_dn7
            // 
            this.rbtn_dn7.BackColor = System.Drawing.Color.Transparent;
            this.rbtn_dn7.bgScale = 3;
            this.rbtn_dn7.bOn = true;
            this.rbtn_dn7.colorShadow = System.Drawing.Color.LightGray;
            this.rbtn_dn7.colorTop = System.Drawing.Color.Pink;
            this.rbtn_dn7.Font = new System.Drawing.Font("Arial", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbtn_dn7.IsButtonUp = true;
            this.rbtn_dn7.Label = null;
            this.rbtn_dn7.Location = new System.Drawing.Point(10, 514);
            this.rbtn_dn7.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.rbtn_dn7.Name = "rbtn_dn7";
            this.rbtn_dn7.Size = new System.Drawing.Size(60, 39);
            this.rbtn_dn7.Style = Mndz.MyButtonType.trianglednButton;
            this.rbtn_dn7.TabIndex = 32;
            // 
            // rngbtn_1
            // 
            this.rngbtn_1.BackColor = System.Drawing.Color.Transparent;
            this.rngbtn_1.bgScale = 3;
            this.rngbtn_1.bOn = true;
            this.rngbtn_1.colorShadow = System.Drawing.Color.LightGray;
            this.rngbtn_1.colorTop = System.Drawing.Color.Pink;
            this.rngbtn_1.Font = new System.Drawing.Font("Arial", 12.11881F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rngbtn_1.IsButtonUp = true;
            this.rngbtn_1.Label = "  1 A";
            this.rngbtn_1.Location = new System.Drawing.Point(577, 361);
            this.rngbtn_1.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.rngbtn_1.Name = "rngbtn_1";
            this.rngbtn_1.Size = new System.Drawing.Size(100, 34);
            this.rngbtn_1.Style = Mndz.MyButtonType.rectButton;
            this.rngbtn_1.TabIndex = 33;
            // 
            // rngbtn_10
            // 
            this.rngbtn_10.BackColor = System.Drawing.Color.Transparent;
            this.rngbtn_10.bgScale = 3;
            this.rngbtn_10.bOn = true;
            this.rngbtn_10.colorShadow = System.Drawing.Color.LightGray;
            this.rngbtn_10.colorTop = System.Drawing.Color.Pink;
            this.rngbtn_10.Font = new System.Drawing.Font("Arial", 12.11881F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rngbtn_10.IsButtonUp = true;
            this.rngbtn_10.Label = " 10 A";
            this.rngbtn_10.Location = new System.Drawing.Point(577, 400);
            this.rngbtn_10.Margin = new System.Windows.Forms.Padding(28, 27, 28, 27);
            this.rngbtn_10.Name = "rngbtn_10";
            this.rngbtn_10.Size = new System.Drawing.Size(100, 34);
            this.rngbtn_10.Style = Mndz.MyButtonType.rectButton;
            this.rngbtn_10.TabIndex = 33;
            // 
            // rngbtn_100
            // 
            this.rngbtn_100.BackColor = System.Drawing.Color.Transparent;
            this.rngbtn_100.bgScale = 3;
            this.rngbtn_100.bOn = true;
            this.rngbtn_100.colorShadow = System.Drawing.Color.LightGray;
            this.rngbtn_100.colorTop = System.Drawing.Color.Pink;
            this.rngbtn_100.Font = new System.Drawing.Font("Arial", 12.11881F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rngbtn_100.IsButtonUp = true;
            this.rngbtn_100.Label = "100 A";
            this.rngbtn_100.Location = new System.Drawing.Point(577, 439);
            this.rngbtn_100.Margin = new System.Windows.Forms.Padding(28, 27, 28, 27);
            this.rngbtn_100.Name = "rngbtn_100";
            this.rngbtn_100.Size = new System.Drawing.Size(100, 34);
            this.rngbtn_100.Style = Mndz.MyButtonType.rectButton;
            this.rngbtn_100.TabIndex = 34;
            // 
            // rngbtn_300
            // 
            this.rngbtn_300.BackColor = System.Drawing.Color.Transparent;
            this.rngbtn_300.bgScale = 3;
            this.rngbtn_300.bOn = true;
            this.rngbtn_300.colorShadow = System.Drawing.Color.LightGray;
            this.rngbtn_300.colorTop = System.Drawing.Color.Pink;
            this.rngbtn_300.Font = new System.Drawing.Font("Arial", 12.11881F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rngbtn_300.IsButtonUp = true;
            this.rngbtn_300.Label = "300 A";
            this.rngbtn_300.Location = new System.Drawing.Point(577, 478);
            this.rngbtn_300.Margin = new System.Windows.Forms.Padding(28, 27, 28, 27);
            this.rngbtn_300.Name = "rngbtn_300";
            this.rngbtn_300.Size = new System.Drawing.Size(100, 34);
            this.rngbtn_300.Style = Mndz.MyButtonType.rectButton;
            this.rngbtn_300.TabIndex = 35;
            // 
            // rngbtn_600
            // 
            this.rngbtn_600.BackColor = System.Drawing.Color.Transparent;
            this.rngbtn_600.bgScale = 3;
            this.rngbtn_600.bOn = true;
            this.rngbtn_600.colorShadow = System.Drawing.Color.LightGray;
            this.rngbtn_600.colorTop = System.Drawing.Color.Pink;
            this.rngbtn_600.Font = new System.Drawing.Font("Arial", 12.11881F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rngbtn_600.IsButtonUp = true;
            this.rngbtn_600.Label = "600 A";
            this.rngbtn_600.Location = new System.Drawing.Point(577, 517);
            this.rngbtn_600.Margin = new System.Windows.Forms.Padding(28, 27, 28, 27);
            this.rngbtn_600.Name = "rngbtn_600";
            this.rngbtn_600.Size = new System.Drawing.Size(100, 34);
            this.rngbtn_600.Style = Mndz.MyButtonType.rectButton;
            this.rngbtn_600.TabIndex = 36;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 19.9604F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(589, 315);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(75, 33);
            this.label4.TabIndex = 37;
            this.label4.Text = "量程";
            // 
            // rngbtn_1000
            // 
            this.rngbtn_1000.BackColor = System.Drawing.Color.Transparent;
            this.rngbtn_1000.bgScale = 3;
            this.rngbtn_1000.bOn = true;
            this.rngbtn_1000.colorShadow = System.Drawing.Color.LightGray;
            this.rngbtn_1000.colorTop = System.Drawing.Color.Pink;
            this.rngbtn_1000.Font = new System.Drawing.Font("Arial", 12.11881F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rngbtn_1000.IsButtonUp = true;
            this.rngbtn_1000.Label = "1000 A";
            this.rngbtn_1000.Location = new System.Drawing.Point(577, 555);
            this.rngbtn_1000.Margin = new System.Windows.Forms.Padding(28, 27, 28, 27);
            this.rngbtn_1000.Name = "rngbtn_1000";
            this.rngbtn_1000.Size = new System.Drawing.Size(100, 34);
            this.rngbtn_1000.Style = Mndz.MyButtonType.rectButton;
            this.rngbtn_1000.TabIndex = 38;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.LightSkyBlue;
            this.ClientSize = new System.Drawing.Size(1018, 568);
            this.Controls.Add(this.rngbtn_1000);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.rngbtn_600);
            this.Controls.Add(this.rngbtn_300);
            this.Controls.Add(this.rngbtn_100);
            this.Controls.Add(this.rngbtn_10);
            this.Controls.Add(this.rngbtn_1);
            this.Controls.Add(this.rbtn_dn7);
            this.Controls.Add(this.rbtn_dn6);
            this.Controls.Add(this.rbtn_dn5);
            this.Controls.Add(this.rbtn_dn4);
            this.Controls.Add(this.rbtn_dn3);
            this.Controls.Add(this.rbtn_dn2);
            this.Controls.Add(this.rbtn_dn1);
            this.Controls.Add(this.rbtn_up1);
            this.Controls.Add(this.rbtn_up7);
            this.Controls.Add(this.rbtn_up6);
            this.Controls.Add(this.rbtn_up5);
            this.Controls.Add(this.rbtn_up4);
            this.Controls.Add(this.rbtn_up3);
            this.Controls.Add(this.rbtn_up2);
            this.Controls.Add(this.rectMeter1);
            this.Controls.Add(this.btn_turnon);
            this.Controls.Add(this.lbButton9);
            this.Controls.Add(this.lbButton8);
            this.Controls.Add(this.lbButton7);
            this.Controls.Add(this.lbButton6);
            this.Controls.Add(this.lbButton5);
            this.Controls.Add(this.lbButton4);
            this.Controls.Add(this.lbButtonOK);
            this.Controls.Add(this.lbButtonCancel);
            this.Controls.Add(this.lbButtonPercent);
            this.Controls.Add(this.lbButtonPT);
            this.Controls.Add(this.lbButton3);
            this.Controls.Add(this.lbButton2);
            this.Controls.Add(this.lbButton0);
            this.Controls.Add(this.lbButton1);
            this.Controls.Add(this.btn_zeroon);
            this.Controls.Add(this.lbl_datetime);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.lbl_range);
            this.Controls.Add(this.led_current);
            this.Controls.Add(this.led_setting);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "Form1";
            this.Text = "上海兰斯汀仪表研究所-电流源软件";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private DmitryBrant.CustomControls.SevenSegmentArray led_setting;
        private DmitryBrant.CustomControls.SevenSegmentArray led_current;
        private System.Windows.Forms.Label lbl_range;
        private System.Windows.Forms.Label lbl_datetime;
        private System.Windows.Forms.Label label1;
        private RectButton btn_zeroon;
        private Mndz.RectButton lbButton1;
        private Mndz.RectButton lbButton2;
        private Mndz.RectButton lbButton3;
        private Mndz.RectButton lbButton4;
        private Mndz.RectButton lbButton5;
        private Mndz.RectButton lbButton6;
        private Mndz.RectButton lbButton7;
        private Mndz.RectButton lbButton8;
        private Mndz.RectButton lbButton9;
        private Mndz.RectButton lbButton0;
        private Mndz.RectButton lbButtonPT;
        private Mndz.RectButton lbButtonCancel;
        private Mndz.RectButton lbButtonOK;
        private Mndz.RectButton lbButtonPercent;
        private System.Windows.Forms.Label label2;
        private Mndz.RectButton btn_turnon;
        private Mndz.MyCtrl.RectMeter rectMeter1;
        private RectButton rbtn_up2;
        private RectButton rbtn_up3;
        private RectButton rbtn_up4;
        private RectButton rbtn_up5;
        private RectButton rbtn_up6;
        private RectButton rbtn_up7;
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
        private System.Windows.Forms.Label label4;
        private RectButton rngbtn_1000;
    }
}

