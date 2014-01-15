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
            this.led_current = new DmitryBrant.CustomControls.SevenSegmentArray();
            this.led_setting = new DmitryBrant.CustomControls.SevenSegmentArray();
            this.lbl_output = new System.Windows.Forms.Label();
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
            this.SuspendLayout();
            // 
            // lbl_range
            // 
            this.lbl_range.AutoSize = true;
            this.lbl_range.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.lbl_range.Location = new System.Drawing.Point(12, 252);
            this.lbl_range.Name = "lbl_range";
            this.lbl_range.Size = new System.Drawing.Size(157, 39);
            this.lbl_range.TabIndex = 6;
            this.lbl_range.Text = "输出电流";
            // 
            // lbl_ohm
            // 
            this.lbl_ohm.AutoSize = true;
            this.lbl_ohm.Font = new System.Drawing.Font("Arial", 22.09901F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_ohm.Location = new System.Drawing.Point(908, 98);
            this.lbl_ohm.Name = "lbl_ohm";
            this.lbl_ohm.Size = new System.Drawing.Size(36, 36);
            this.lbl_ohm.TabIndex = 4;
            this.lbl_ohm.Text = "A";
            // 
            // led_current
            // 
            this.led_current.ArrayCount = 7;
            this.led_current.ColorBackground = System.Drawing.Color.White;
            this.led_current.ColorDark = System.Drawing.Color.WhiteSmoke;
            this.led_current.ColorLight = System.Drawing.Color.Lime;
            this.led_current.DecimalShow = true;
            this.led_current.ElementPadding = ((Mndz.Padding)(resources.GetObject("led_current.ElementPadding")));
            this.led_current.ElementWidth = 5;
            this.led_current.ItalicFactor = 0F;
            this.led_current.Location = new System.Drawing.Point(249, 221);
            this.led_current.Name = "led_current";
            this.led_current.Size = new System.Drawing.Size(515, 88);
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
            this.led_setting.ElementWidth = 10;
            this.led_setting.ItalicFactor = -0.05F;
            this.led_setting.Location = new System.Drawing.Point(146, 54);
            this.led_setting.Margin = new System.Windows.Forms.Padding(1);
            this.led_setting.Name = "led_setting";
            this.led_setting.Size = new System.Drawing.Size(756, 149);
            this.led_setting.TabIndex = 1;
            this.led_setting.TabStop = false;
            this.led_setting.Value = null;
            // 
            // lbl_output
            // 
            this.lbl_output.AutoSize = true;
            this.lbl_output.Font = new System.Drawing.Font("Arial Narrow", 25.66337F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_output.Location = new System.Drawing.Point(770, 248);
            this.lbl_output.Name = "lbl_output";
            this.lbl_output.Size = new System.Drawing.Size(39, 43);
            this.lbl_output.TabIndex = 4;
            this.lbl_output.Text = "A";
            // 
            // lbl_datetime
            // 
            this.lbl_datetime.AutoSize = true;
            this.lbl_datetime.Location = new System.Drawing.Point(824, 9);
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
            this.label1.Location = new System.Drawing.Point(266, -1);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(527, 44);
            this.label1.TabIndex = 0;
            this.label1.Text = "300A 精密直流大功率恒流源";
            // 
            // btn_zeroon
            // 
            this.btn_zeroon.BackColor = System.Drawing.Color.Transparent;
            this.btn_zeroon.bgScale = 3;
            this.btn_zeroon.bOn = true;
            this.btn_zeroon.colorShadow = System.Drawing.Color.LightGray;
            this.btn_zeroon.colorTop = System.Drawing.Color.Pink;
            this.btn_zeroon.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25743F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_zeroon.Label = "电流表清零";
            this.btn_zeroon.Location = new System.Drawing.Point(850, 237);
            this.btn_zeroon.Margin = new System.Windows.Forms.Padding(6, 6, 6, 6);
            this.btn_zeroon.Name = "btn_zeroon";
            this.btn_zeroon.Size = new System.Drawing.Size(156, 54);
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
            this.lbButton1.Label = null;
            this.lbButton1.Location = new System.Drawing.Point(701, 444);
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
            this.lbButton2.Label = null;
            this.lbButton2.Location = new System.Drawing.Point(770, 444);
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
            this.lbButton3.Label = null;
            this.lbButton3.Location = new System.Drawing.Point(839, 444);
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
            this.lbButton4.Label = null;
            this.lbButton4.Location = new System.Drawing.Point(701, 384);
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
            this.lbButton5.Label = null;
            this.lbButton5.Location = new System.Drawing.Point(770, 384);
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
            this.lbButton6.Label = null;
            this.lbButton6.Location = new System.Drawing.Point(839, 384);
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
            this.lbButton7.Font = new System.Drawing.Font("Arial", 17.82178F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbButton7.Label = "7";
            this.lbButton7.Location = new System.Drawing.Point(701, 324);
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
            this.lbButton8.Label = null;
            this.lbButton8.Location = new System.Drawing.Point(770, 324);
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
            this.lbButton9.Label = null;
            this.lbButton9.Location = new System.Drawing.Point(839, 324);
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
            this.lbButton0.Label = null;
            this.lbButton0.Location = new System.Drawing.Point(701, 504);
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
            this.lbButtonPT.Label = null;
            this.lbButtonPT.Location = new System.Drawing.Point(770, 504);
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
            this.lbButtonCancel.Label = "取消";
            this.lbButtonCancel.Location = new System.Drawing.Point(908, 324);
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
            this.lbButtonOK.Label = "确定";
            this.lbButtonOK.Location = new System.Drawing.Point(908, 444);
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
            this.lbButtonPercent.Label = null;
            this.lbButtonPercent.Location = new System.Drawing.Point(839, 504);
            this.lbButtonPercent.Margin = new System.Windows.Forms.Padding(31, 27, 31, 27);
            this.lbButtonPercent.Name = "lbButtonPercent";
            this.lbButtonPercent.Size = new System.Drawing.Size(63, 57);
            this.lbButtonPercent.Style = Mndz.MyButtonType.roundButton;
            this.lbButtonPercent.TabIndex = 14;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(8, 108);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(157, 39);
            this.label2.TabIndex = 6;
            this.label2.Text = "设定电流";
            // 
            // btn_turnon
            // 
            this.btn_turnon.BackColor = System.Drawing.Color.Transparent;
            this.btn_turnon.bgScale = 3;
            this.btn_turnon.bOn = true;
            this.btn_turnon.colorShadow = System.Drawing.Color.LightGray;
            this.btn_turnon.colorTop = System.Drawing.Color.Pink;
            this.btn_turnon.Font = new System.Drawing.Font("Microsoft Sans Serif", 36.35643F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_turnon.Label = "ON";
            this.btn_turnon.Location = new System.Drawing.Point(463, 362);
            this.btn_turnon.Margin = new System.Windows.Forms.Padding(15, 14, 15, 14);
            this.btn_turnon.Name = "btn_turnon";
            this.btn_turnon.Size = new System.Drawing.Size(148, 139);
            this.btn_turnon.Style = Mndz.MyButtonType.roundButton;
            this.btn_turnon.TabIndex = 15;
            // 
            // rectMeter1
            // 
            this.rectMeter1.Angle = 45;
            this.rectMeter1.BackgroundImage = global::Mndz.Properties.Resources.BGMETER;
            this.rectMeter1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.rectMeter1.BgResId = "";
            this.rectMeter1.Location = new System.Drawing.Point(12, 324);
            this.rectMeter1.Name = "rectMeter1";
            this.rectMeter1.PointColor = System.Drawing.Color.Yellow;
            this.rectMeter1.Size = new System.Drawing.Size(447, 232);
            this.rectMeter1.TabIndex = 16;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.LightSkyBlue;
            this.ClientSize = new System.Drawing.Size(1018, 568);
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
            this.Controls.Add(this.lbl_output);
            this.Controls.Add(this.lbl_ohm);
            this.Controls.Add(this.led_setting);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "Form1";
            this.Text = "北京东方计量测试研究所";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private DmitryBrant.CustomControls.SevenSegmentArray led_setting;
        private DmitryBrant.CustomControls.SevenSegmentArray led_current;
        private System.Windows.Forms.Label lbl_range;
        private System.Windows.Forms.Label lbl_ohm;
        private System.Windows.Forms.Label lbl_output;
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
    }
}

