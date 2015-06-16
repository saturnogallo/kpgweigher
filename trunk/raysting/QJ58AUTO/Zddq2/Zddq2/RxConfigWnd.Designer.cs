namespace Zddq2
{
    partial class RxConfigWnd
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
            this.lbl_current = new System.Windows.Forms.Label();
            this.lbl_serial = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.lbl_sqrt = new System.Windows.Forms.Label();
            this.lbl_serial_select = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.lbl_range = new System.Windows.Forms.Label();
            this.lbl_rtp = new System.Windows.Forms.Label();
            this.lbl_coefB = new System.Windows.Forms.Label();
            this.lbl_coefA = new System.Windows.Forms.Label();
            this.lbl_coefC = new System.Windows.Forms.Label();
            this.lbl_prbtype = new System.Windows.Forms.Label();
            this.lbl_meastimes = new System.Windows.Forms.Label();
            this.lbl_measdelay = new System.Windows.Forms.Label();
            this.lbl_rx_pos = new System.Windows.Forms.Label();
            this.btn_enable = new Raysting.Controls.RectButton();
            this.btn_coefa = new Raysting.Controls.RectButton();
            this.btn_coefb = new Raysting.Controls.RectButton();
            this.btn_coefc = new Raysting.Controls.RectButton();
            this.btn_range = new Raysting.Controls.RectButton();
            this.btn_rtp = new Raysting.Controls.RectButton();
            this.btn_meastimes = new Raysting.Controls.RectButton();
            this.btn_measdelay = new Raysting.Controls.RectButton();
            this.btn_sqrt = new Raysting.Controls.RectButton();
            this.btn_current = new Raysting.Controls.RectButton();
            this.btn_serial_select = new Raysting.Controls.RectButton();
            this.btn_serial = new Raysting.Controls.RectButton();
            this.btn_prbtype = new Raysting.Controls.RectButton();
            this.btn_quit = new Raysting.Controls.RectButton();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // lbl_current
            // 
            this.lbl_current.AutoSize = true;
            this.lbl_current.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_current.Location = new System.Drawing.Point(13, 80);
            this.lbl_current.Name = "lbl_current";
            this.lbl_current.Size = new System.Drawing.Size(126, 29);
            this.lbl_current.TabIndex = 1;
            this.lbl_current.Text = "lbl_current";
            // 
            // lbl_serial
            // 
            this.lbl_serial.AutoSize = true;
            this.lbl_serial.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_serial.Location = new System.Drawing.Point(13, 7);
            this.lbl_serial.Name = "lbl_serial";
            this.lbl_serial.Size = new System.Drawing.Size(111, 29);
            this.lbl_serial.TabIndex = 1;
            this.lbl_serial.Text = "lbl_serial";
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.lbl_sqrt);
            this.panel1.Controls.Add(this.lbl_serial_select);
            this.panel1.Controls.Add(this.panel2);
            this.panel1.Controls.Add(this.lbl_prbtype);
            this.panel1.Controls.Add(this.lbl_current);
            this.panel1.Controls.Add(this.lbl_serial);
            this.panel1.Controls.Add(this.btn_meastimes);
            this.panel1.Controls.Add(this.btn_measdelay);
            this.panel1.Controls.Add(this.btn_sqrt);
            this.panel1.Controls.Add(this.btn_current);
            this.panel1.Controls.Add(this.btn_serial_select);
            this.panel1.Controls.Add(this.btn_serial);
            this.panel1.Controls.Add(this.lbl_meastimes);
            this.panel1.Controls.Add(this.btn_prbtype);
            this.panel1.Controls.Add(this.lbl_measdelay);
            this.panel1.Location = new System.Drawing.Point(12, 88);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(722, 402);
            this.panel1.TabIndex = 6;
            // 
            // lbl_sqrt
            // 
            this.lbl_sqrt.AutoSize = true;
            this.lbl_sqrt.Location = new System.Drawing.Point(646, 21);
            this.lbl_sqrt.Name = "lbl_sqrt";
            this.lbl_sqrt.Size = new System.Drawing.Size(41, 15);
            this.lbl_sqrt.TabIndex = 4;
            this.lbl_sqrt.Text = "label1";
            this.lbl_sqrt.Visible = false;
            // 
            // lbl_serial_select
            // 
            this.lbl_serial_select.AutoSize = true;
            this.lbl_serial_select.Location = new System.Drawing.Point(588, 20);
            this.lbl_serial_select.Name = "lbl_serial_select";
            this.lbl_serial_select.Size = new System.Drawing.Size(41, 15);
            this.lbl_serial_select.TabIndex = 4;
            this.lbl_serial_select.Text = "label1";
            this.lbl_serial_select.Visible = false;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.btn_coefa);
            this.panel2.Controls.Add(this.lbl_range);
            this.panel2.Controls.Add(this.lbl_rtp);
            this.panel2.Controls.Add(this.lbl_coefB);
            this.panel2.Controls.Add(this.btn_coefb);
            this.panel2.Controls.Add(this.lbl_coefA);
            this.panel2.Controls.Add(this.btn_coefc);
            this.panel2.Controls.Add(this.btn_range);
            this.panel2.Controls.Add(this.btn_rtp);
            this.panel2.Controls.Add(this.lbl_coefC);
            this.panel2.Location = new System.Drawing.Point(-1, 263);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(721, 143);
            this.panel2.TabIndex = 3;
            // 
            // lbl_range
            // 
            this.lbl_range.AutoSize = true;
            this.lbl_range.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_range.Location = new System.Drawing.Point(328, 17);
            this.lbl_range.Name = "lbl_range";
            this.lbl_range.Size = new System.Drawing.Size(114, 29);
            this.lbl_range.TabIndex = 2;
            this.lbl_range.Text = "lbl_range";
            // 
            // lbl_rtp
            // 
            this.lbl_rtp.AutoSize = true;
            this.lbl_rtp.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_rtp.Location = new System.Drawing.Point(4, 17);
            this.lbl_rtp.Name = "lbl_rtp";
            this.lbl_rtp.Size = new System.Drawing.Size(80, 29);
            this.lbl_rtp.TabIndex = 2;
            this.lbl_rtp.Text = "lbl_rtp";
            // 
            // lbl_coefB
            // 
            this.lbl_coefB.AutoSize = true;
            this.lbl_coefB.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_coefB.Location = new System.Drawing.Point(246, 85);
            this.lbl_coefB.Name = "lbl_coefB";
            this.lbl_coefB.Size = new System.Drawing.Size(80, 29);
            this.lbl_coefB.TabIndex = 2;
            this.lbl_coefB.Text = "CoefB";
            // 
            // lbl_coefA
            // 
            this.lbl_coefA.AutoSize = true;
            this.lbl_coefA.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_coefA.Location = new System.Drawing.Point(5, 85);
            this.lbl_coefA.Name = "lbl_coefA";
            this.lbl_coefA.Size = new System.Drawing.Size(79, 29);
            this.lbl_coefA.TabIndex = 2;
            this.lbl_coefA.Text = "CoefA";
            // 
            // lbl_coefC
            // 
            this.lbl_coefC.AutoSize = true;
            this.lbl_coefC.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_coefC.Location = new System.Drawing.Point(489, 85);
            this.lbl_coefC.Name = "lbl_coefC";
            this.lbl_coefC.Size = new System.Drawing.Size(81, 29);
            this.lbl_coefC.TabIndex = 2;
            this.lbl_coefC.Text = "CoefC";
            // 
            // lbl_prbtype
            // 
            this.lbl_prbtype.AutoSize = true;
            this.lbl_prbtype.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_prbtype.Location = new System.Drawing.Point(362, 80);
            this.lbl_prbtype.Name = "lbl_prbtype";
            this.lbl_prbtype.Size = new System.Drawing.Size(133, 29);
            this.lbl_prbtype.TabIndex = 2;
            this.lbl_prbtype.Text = "lbl_prbtype";
            // 
            // lbl_meastimes
            // 
            this.lbl_meastimes.AutoSize = true;
            this.lbl_meastimes.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_meastimes.Location = new System.Drawing.Point(362, 142);
            this.lbl_meastimes.Name = "lbl_meastimes";
            this.lbl_meastimes.Size = new System.Drawing.Size(169, 29);
            this.lbl_meastimes.TabIndex = 2;
            this.lbl_meastimes.Text = "lbl_meastimes";
            // 
            // lbl_measdelay
            // 
            this.lbl_measdelay.AutoSize = true;
            this.lbl_measdelay.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_measdelay.Location = new System.Drawing.Point(13, 142);
            this.lbl_measdelay.Name = "lbl_measdelay";
            this.lbl_measdelay.Size = new System.Drawing.Size(169, 29);
            this.lbl_measdelay.TabIndex = 2;
            this.lbl_measdelay.Text = "lbl_measdelay";
            // 
            // lbl_rx_pos
            // 
            this.lbl_rx_pos.AutoSize = true;
            this.lbl_rx_pos.Font = new System.Drawing.Font("Microsoft Sans Serif", 17.82178F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_rx_pos.Location = new System.Drawing.Point(185, 20);
            this.lbl_rx_pos.Name = "lbl_rx_pos";
            this.lbl_rx_pos.Size = new System.Drawing.Size(138, 29);
            this.lbl_rx_pos.TabIndex = 1;
            this.lbl_rx_pos.Text = "lbl_rx_pos";
            // 
            // btn_enable
            // 
            this.btn_enable.BackColor = System.Drawing.Color.Red;
            this.btn_enable.bgScale = 3;
            this.btn_enable.bOn = true;
            this.btn_enable.colorShadow = System.Drawing.Color.LightGray;
            this.btn_enable.colorTop = System.Drawing.Color.Pink;
            this.btn_enable.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_enable.Label = null;
            this.btn_enable.Location = new System.Drawing.Point(12, 13);
            this.btn_enable.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btn_enable.Name = "btn_enable";
            this.btn_enable.Size = new System.Drawing.Size(128, 46);
            this.btn_enable.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_enable.TabIndex = 3;
            // 
            // btn_coefa
            // 
            this.btn_coefa.BackColor = System.Drawing.Color.Red;
            this.btn_coefa.bgScale = 3;
            this.btn_coefa.bOn = true;
            this.btn_coefa.colorShadow = System.Drawing.Color.LightGray;
            this.btn_coefa.colorTop = System.Drawing.Color.Pink;
            this.btn_coefa.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_coefa.Label = null;
            this.btn_coefa.Location = new System.Drawing.Point(90, 68);
            this.btn_coefa.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_coefa.Name = "btn_coefa";
            this.btn_coefa.Size = new System.Drawing.Size(149, 46);
            this.btn_coefa.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_coefa.TabIndex = 0;
            // 
            // btn_coefb
            // 
            this.btn_coefb.BackColor = System.Drawing.Color.Red;
            this.btn_coefb.bgScale = 3;
            this.btn_coefb.bOn = true;
            this.btn_coefb.colorShadow = System.Drawing.Color.LightGray;
            this.btn_coefb.colorTop = System.Drawing.Color.Pink;
            this.btn_coefb.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_coefb.Label = null;
            this.btn_coefb.Location = new System.Drawing.Point(333, 68);
            this.btn_coefb.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_coefb.Name = "btn_coefb";
            this.btn_coefb.Size = new System.Drawing.Size(149, 46);
            this.btn_coefb.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_coefb.TabIndex = 0;
            // 
            // btn_coefc
            // 
            this.btn_coefc.BackColor = System.Drawing.Color.Red;
            this.btn_coefc.bgScale = 3;
            this.btn_coefc.bOn = true;
            this.btn_coefc.colorShadow = System.Drawing.Color.LightGray;
            this.btn_coefc.colorTop = System.Drawing.Color.Pink;
            this.btn_coefc.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_coefc.Label = null;
            this.btn_coefc.Location = new System.Drawing.Point(568, 68);
            this.btn_coefc.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_coefc.Name = "btn_coefc";
            this.btn_coefc.Size = new System.Drawing.Size(149, 46);
            this.btn_coefc.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_coefc.TabIndex = 0;
            // 
            // btn_range
            // 
            this.btn_range.BackColor = System.Drawing.Color.Red;
            this.btn_range.bgScale = 3;
            this.btn_range.bOn = true;
            this.btn_range.colorShadow = System.Drawing.Color.LightGray;
            this.btn_range.colorTop = System.Drawing.Color.Pink;
            this.btn_range.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_range.Label = null;
            this.btn_range.Location = new System.Drawing.Point(511, 5);
            this.btn_range.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_range.Name = "btn_range";
            this.btn_range.Size = new System.Drawing.Size(206, 46);
            this.btn_range.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_range.TabIndex = 0;
            // 
            // btn_rtp
            // 
            this.btn_rtp.BackColor = System.Drawing.Color.Red;
            this.btn_rtp.bgScale = 3;
            this.btn_rtp.bOn = true;
            this.btn_rtp.colorShadow = System.Drawing.Color.LightGray;
            this.btn_rtp.colorTop = System.Drawing.Color.Pink;
            this.btn_rtp.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_rtp.Label = null;
            this.btn_rtp.Location = new System.Drawing.Point(91, 5);
            this.btn_rtp.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_rtp.Name = "btn_rtp";
            this.btn_rtp.Size = new System.Drawing.Size(203, 46);
            this.btn_rtp.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_rtp.TabIndex = 0;
            // 
            // btn_meastimes
            // 
            this.btn_meastimes.BackColor = System.Drawing.Color.Red;
            this.btn_meastimes.bgScale = 3;
            this.btn_meastimes.bOn = true;
            this.btn_meastimes.colorShadow = System.Drawing.Color.LightGray;
            this.btn_meastimes.colorTop = System.Drawing.Color.Pink;
            this.btn_meastimes.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_meastimes.Label = null;
            this.btn_meastimes.Location = new System.Drawing.Point(538, 142);
            this.btn_meastimes.Margin = new System.Windows.Forms.Padding(5);
            this.btn_meastimes.Name = "btn_meastimes";
            this.btn_meastimes.Size = new System.Drawing.Size(162, 46);
            this.btn_meastimes.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_meastimes.TabIndex = 0;
            // 
            // btn_measdelay
            // 
            this.btn_measdelay.BackColor = System.Drawing.Color.Red;
            this.btn_measdelay.bgScale = 3;
            this.btn_measdelay.bOn = true;
            this.btn_measdelay.colorShadow = System.Drawing.Color.LightGray;
            this.btn_measdelay.colorTop = System.Drawing.Color.Pink;
            this.btn_measdelay.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_measdelay.Label = null;
            this.btn_measdelay.Location = new System.Drawing.Point(167, 142);
            this.btn_measdelay.Margin = new System.Windows.Forms.Padding(5);
            this.btn_measdelay.Name = "btn_measdelay";
            this.btn_measdelay.Size = new System.Drawing.Size(162, 46);
            this.btn_measdelay.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_measdelay.TabIndex = 0;
            // 
            // btn_sqrt
            // 
            this.btn_sqrt.BackColor = System.Drawing.Color.Red;
            this.btn_sqrt.bgScale = 3;
            this.btn_sqrt.bOn = true;
            this.btn_sqrt.colorShadow = System.Drawing.Color.LightGray;
            this.btn_sqrt.colorTop = System.Drawing.Color.Pink;
            this.btn_sqrt.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_sqrt.Label = null;
            this.btn_sqrt.Location = new System.Drawing.Point(277, 80);
            this.btn_sqrt.Margin = new System.Windows.Forms.Padding(5);
            this.btn_sqrt.Name = "btn_sqrt";
            this.btn_sqrt.Size = new System.Drawing.Size(52, 46);
            this.btn_sqrt.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_sqrt.TabIndex = 0;
            // 
            // btn_current
            // 
            this.btn_current.BackColor = System.Drawing.Color.Red;
            this.btn_current.bgScale = 3;
            this.btn_current.bOn = true;
            this.btn_current.colorShadow = System.Drawing.Color.LightGray;
            this.btn_current.colorTop = System.Drawing.Color.Pink;
            this.btn_current.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_current.Label = null;
            this.btn_current.Location = new System.Drawing.Point(167, 80);
            this.btn_current.Margin = new System.Windows.Forms.Padding(5);
            this.btn_current.Name = "btn_current";
            this.btn_current.Size = new System.Drawing.Size(98, 46);
            this.btn_current.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_current.TabIndex = 0;
            // 
            // btn_serial_select
            // 
            this.btn_serial_select.BackColor = System.Drawing.Color.Red;
            this.btn_serial_select.bgScale = 3;
            this.btn_serial_select.bOn = true;
            this.btn_serial_select.colorShadow = System.Drawing.Color.LightGray;
            this.btn_serial_select.colorTop = System.Drawing.Color.Pink;
            this.btn_serial_select.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_serial_select.Label = null;
            this.btn_serial_select.Location = new System.Drawing.Point(433, 7);
            this.btn_serial_select.Margin = new System.Windows.Forms.Padding(5);
            this.btn_serial_select.Name = "btn_serial_select";
            this.btn_serial_select.Size = new System.Drawing.Size(57, 46);
            this.btn_serial_select.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_serial_select.TabIndex = 0;
            // 
            // btn_serial
            // 
            this.btn_serial.BackColor = System.Drawing.Color.Red;
            this.btn_serial.bgScale = 3;
            this.btn_serial.bOn = true;
            this.btn_serial.colorShadow = System.Drawing.Color.LightGray;
            this.btn_serial.colorTop = System.Drawing.Color.Pink;
            this.btn_serial.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_serial.Label = null;
            this.btn_serial.Location = new System.Drawing.Point(167, 7);
            this.btn_serial.Margin = new System.Windows.Forms.Padding(5);
            this.btn_serial.Name = "btn_serial";
            this.btn_serial.Size = new System.Drawing.Size(243, 46);
            this.btn_serial.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_serial.TabIndex = 0;
            // 
            // btn_prbtype
            // 
            this.btn_prbtype.BackColor = System.Drawing.Color.Red;
            this.btn_prbtype.bgScale = 3;
            this.btn_prbtype.bOn = true;
            this.btn_prbtype.colorShadow = System.Drawing.Color.LightGray;
            this.btn_prbtype.colorTop = System.Drawing.Color.Pink;
            this.btn_prbtype.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_prbtype.Label = null;
            this.btn_prbtype.Location = new System.Drawing.Point(538, 80);
            this.btn_prbtype.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_prbtype.Name = "btn_prbtype";
            this.btn_prbtype.Size = new System.Drawing.Size(149, 46);
            this.btn_prbtype.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_prbtype.TabIndex = 0;
            // 
            // btn_quit
            // 
            this.btn_quit.BackColor = System.Drawing.Color.Red;
            this.btn_quit.bgScale = 3;
            this.btn_quit.bOn = true;
            this.btn_quit.colorShadow = System.Drawing.Color.LightGray;
            this.btn_quit.colorTop = System.Drawing.Color.Pink;
            this.btn_quit.Label = null;
            this.btn_quit.Location = new System.Drawing.Point(674, 1);
            this.btn_quit.Name = "btn_quit";
            this.btn_quit.Size = new System.Drawing.Size(71, 71);
            this.btn_quit.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_quit.TabIndex = 0;
            // 
            // RxConfigWnd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(745, 502);
            this.ControlBox = false;
            this.Controls.Add(this.btn_enable);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.btn_quit);
            this.Controls.Add(this.lbl_rx_pos);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "RxConfigWnd";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "RxConfigWnd";
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Raysting.Controls.RectButton btn_quit;
        private Raysting.Controls.RectButton btn_prbtype;
        private Raysting.Controls.RectButton btn_current;
        private System.Windows.Forms.Label lbl_current;
        private Raysting.Controls.RectButton btn_sqrt;
        private System.Windows.Forms.Label lbl_serial;
        private Raysting.Controls.RectButton btn_serial;
        private System.Windows.Forms.Panel panel1;
        private Raysting.Controls.RectButton btn_enable;
        private System.Windows.Forms.Label lbl_measdelay;
        private Raysting.Controls.RectButton btn_measdelay;
        private System.Windows.Forms.Label lbl_prbtype;
        private System.Windows.Forms.Label lbl_rx_pos;
        private Raysting.Controls.RectButton btn_meastimes;
        private System.Windows.Forms.Label lbl_meastimes;
        private System.Windows.Forms.Label lbl_coefA;
        private System.Windows.Forms.Label lbl_rtp;
        private Raysting.Controls.RectButton btn_coefa;
        private Raysting.Controls.RectButton btn_rtp;
        private System.Windows.Forms.Label lbl_coefB;
        private Raysting.Controls.RectButton btn_coefb;
        private System.Windows.Forms.Panel panel2;
        private Raysting.Controls.RectButton btn_coefc;
        private System.Windows.Forms.Label lbl_coefC;
        private System.Windows.Forms.Label lbl_range;
        private Raysting.Controls.RectButton btn_range;
        private Raysting.Controls.RectButton btn_serial_select;
        private System.Windows.Forms.Label lbl_serial_select;
        private System.Windows.Forms.Label lbl_sqrt;
    }
}