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
            this.lbl_stdchan = new System.Windows.Forms.Label();
            this.lbl_current = new System.Windows.Forms.Label();
            this.lbl_serial = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.btn_next = new Zddq2.RectButton();
            this.btn_last = new Zddq2.RectButton();
            this.btn_enable = new Zddq2.RectButton();
            this.lbl_vmode = new System.Windows.Forms.Label();
            this.btn_chan = new Zddq2.RectButton();
            this.btn_vmode = new Zddq2.RectButton();
            this.btn_sqrt = new Zddq2.RectButton();
            this.btn_current = new Zddq2.RectButton();
            this.btn_serial = new Zddq2.RectButton();
            this.btn_stdchan = new Zddq2.RectButton();
            this.btn_SysConfig = new Zddq2.RectButton();
            this.btn_RxConfig = new Zddq2.RectButton();
            this.btn_RsConfig = new Zddq2.RectButton();
            this.btn_quit = new Zddq2.RectButton();
            this.lbl_range = new System.Windows.Forms.Label();
            this.btn_rangeP001 = new Zddq2.RectButton();
            this.btn_rangeP01 = new Zddq2.RectButton();
            this.btn_rangeP1 = new Zddq2.RectButton();
            this.btn_range1 = new Zddq2.RectButton();
            this.btn_range10 = new Zddq2.RectButton();
            this.btn_range100 = new Zddq2.RectButton();
            this.btn_range1k = new Zddq2.RectButton();
            this.btn_range10k = new Zddq2.RectButton();
            this.btn_range100k = new Zddq2.RectButton();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // lbl_stdchan
            // 
            this.lbl_stdchan.AutoSize = true;
            this.lbl_stdchan.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_stdchan.Location = new System.Drawing.Point(372, 195);
            this.lbl_stdchan.Name = "lbl_stdchan";
            this.lbl_stdchan.Size = new System.Drawing.Size(122, 25);
            this.lbl_stdchan.TabIndex = 1;
            this.lbl_stdchan.Text = "lbl_stdchan";
            // 
            // lbl_current
            // 
            this.lbl_current.AutoSize = true;
            this.lbl_current.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_current.Location = new System.Drawing.Point(22, 114);
            this.lbl_current.Name = "lbl_current";
            this.lbl_current.Size = new System.Drawing.Size(113, 25);
            this.lbl_current.TabIndex = 1;
            this.lbl_current.Text = "lbl_current";
            // 
            // lbl_serial
            // 
            this.lbl_serial.AutoSize = true;
            this.lbl_serial.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_serial.Location = new System.Drawing.Point(372, 114);
            this.lbl_serial.Name = "lbl_serial";
            this.lbl_serial.Size = new System.Drawing.Size(98, 25);
            this.lbl_serial.TabIndex = 1;
            this.lbl_serial.Text = "lbl_serial";
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.btn_next);
            this.panel1.Controls.Add(this.btn_last);
            this.panel1.Controls.Add(this.btn_enable);
            this.panel1.Controls.Add(this.lbl_range);
            this.panel1.Controls.Add(this.lbl_vmode);
            this.panel1.Controls.Add(this.lbl_current);
            this.panel1.Controls.Add(this.lbl_serial);
            this.panel1.Controls.Add(this.lbl_stdchan);
            this.panel1.Controls.Add(this.btn_chan);
            this.panel1.Controls.Add(this.btn_range100k);
            this.panel1.Controls.Add(this.btn_range10k);
            this.panel1.Controls.Add(this.btn_range1k);
            this.panel1.Controls.Add(this.btn_range100);
            this.panel1.Controls.Add(this.btn_range10);
            this.panel1.Controls.Add(this.btn_range1);
            this.panel1.Controls.Add(this.btn_rangeP1);
            this.panel1.Controls.Add(this.btn_rangeP01);
            this.panel1.Controls.Add(this.btn_rangeP001);
            this.panel1.Controls.Add(this.btn_vmode);
            this.panel1.Controls.Add(this.btn_sqrt);
            this.panel1.Controls.Add(this.btn_current);
            this.panel1.Controls.Add(this.btn_serial);
            this.panel1.Controls.Add(this.btn_stdchan);
            this.panel1.Location = new System.Drawing.Point(12, 88);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(722, 402);
            this.panel1.TabIndex = 6;
            // 
            // btn_next
            // 
            this.btn_next.BackColor = System.Drawing.Color.Red;
            this.btn_next.bEnabled = true;
            this.btn_next.bOn = true;
            this.btn_next.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_next.Location = new System.Drawing.Point(193, 20);
            this.btn_next.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.btn_next.Name = "btn_next";
            this.btn_next.Size = new System.Drawing.Size(45, 46);
            this.btn_next.TabIndex = 4;
            // 
            // btn_last
            // 
            this.btn_last.BackColor = System.Drawing.Color.Red;
            this.btn_last.bEnabled = true;
            this.btn_last.bOn = true;
            this.btn_last.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_last.Location = new System.Drawing.Point(16, 20);
            this.btn_last.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.btn_last.Name = "btn_last";
            this.btn_last.Size = new System.Drawing.Size(43, 46);
            this.btn_last.TabIndex = 4;
            // 
            // btn_enable
            // 
            this.btn_enable.BackColor = System.Drawing.Color.Red;
            this.btn_enable.bEnabled = true;
            this.btn_enable.bOn = true;
            this.btn_enable.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_enable.Location = new System.Drawing.Point(529, 20);
            this.btn_enable.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btn_enable.Name = "btn_enable";
            this.btn_enable.Size = new System.Drawing.Size(162, 46);
            this.btn_enable.TabIndex = 3;
            // 
            // lbl_vmode
            // 
            this.lbl_vmode.AutoSize = true;
            this.lbl_vmode.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_vmode.Location = new System.Drawing.Point(25, 195);
            this.lbl_vmode.Name = "lbl_vmode";
            this.lbl_vmode.Size = new System.Drawing.Size(110, 25);
            this.lbl_vmode.TabIndex = 2;
            this.lbl_vmode.Text = "lbl_vmode";
            // 
            // btn_chan
            // 
            this.btn_chan.BackColor = System.Drawing.Color.Red;
            this.btn_chan.bEnabled = true;
            this.btn_chan.bOn = true;
            this.btn_chan.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_chan.Location = new System.Drawing.Point(81, 20);
            this.btn_chan.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btn_chan.Name = "btn_chan";
            this.btn_chan.Size = new System.Drawing.Size(91, 46);
            this.btn_chan.TabIndex = 0;
            // 
            // btn_vmode
            // 
            this.btn_vmode.BackColor = System.Drawing.Color.Red;
            this.btn_vmode.bEnabled = true;
            this.btn_vmode.bOn = true;
            this.btn_vmode.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_vmode.Location = new System.Drawing.Point(167, 186);
            this.btn_vmode.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.btn_vmode.Name = "btn_vmode";
            this.btn_vmode.Size = new System.Drawing.Size(162, 46);
            this.btn_vmode.TabIndex = 0;
            // 
            // btn_sqrt
            // 
            this.btn_sqrt.BackColor = System.Drawing.Color.Red;
            this.btn_sqrt.bEnabled = true;
            this.btn_sqrt.bOn = true;
            this.btn_sqrt.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_sqrt.Location = new System.Drawing.Point(277, 103);
            this.btn_sqrt.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.btn_sqrt.Name = "btn_sqrt";
            this.btn_sqrt.Size = new System.Drawing.Size(52, 46);
            this.btn_sqrt.TabIndex = 0;
            // 
            // btn_current
            // 
            this.btn_current.BackColor = System.Drawing.Color.Red;
            this.btn_current.bEnabled = true;
            this.btn_current.bOn = true;
            this.btn_current.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_current.Location = new System.Drawing.Point(167, 103);
            this.btn_current.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.btn_current.Name = "btn_current";
            this.btn_current.Size = new System.Drawing.Size(98, 46);
            this.btn_current.TabIndex = 0;
            // 
            // btn_serial
            // 
            this.btn_serial.BackColor = System.Drawing.Color.Red;
            this.btn_serial.bEnabled = true;
            this.btn_serial.bOn = true;
            this.btn_serial.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_serial.Location = new System.Drawing.Point(529, 103);
            this.btn_serial.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.btn_serial.Name = "btn_serial";
            this.btn_serial.Size = new System.Drawing.Size(162, 46);
            this.btn_serial.TabIndex = 0;
            // 
            // btn_stdchan
            // 
            this.btn_stdchan.BackColor = System.Drawing.Color.Red;
            this.btn_stdchan.bEnabled = true;
            this.btn_stdchan.bOn = true;
            this.btn_stdchan.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_stdchan.Location = new System.Drawing.Point(529, 186);
            this.btn_stdchan.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_stdchan.Name = "btn_stdchan";
            this.btn_stdchan.Size = new System.Drawing.Size(162, 46);
            this.btn_stdchan.TabIndex = 0;
            // 
            // btn_SysConfig
            // 
            this.btn_SysConfig.BackColor = System.Drawing.Color.Blue;
            this.btn_SysConfig.bEnabled = true;
            this.btn_SysConfig.bOn = true;
            this.btn_SysConfig.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_SysConfig.Location = new System.Drawing.Point(12, 12);
            this.btn_SysConfig.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btn_SysConfig.Name = "btn_SysConfig";
            this.btn_SysConfig.Size = new System.Drawing.Size(136, 36);
            this.btn_SysConfig.TabIndex = 5;
            // 
            // btn_RxConfig
            // 
            this.btn_RxConfig.BackColor = System.Drawing.Color.Blue;
            this.btn_RxConfig.bEnabled = true;
            this.btn_RxConfig.bOn = true;
            this.btn_RxConfig.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_RxConfig.Location = new System.Drawing.Point(316, 12);
            this.btn_RxConfig.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btn_RxConfig.Name = "btn_RxConfig";
            this.btn_RxConfig.Size = new System.Drawing.Size(136, 36);
            this.btn_RxConfig.TabIndex = 4;
            // 
            // btn_RsConfig
            // 
            this.btn_RsConfig.BackColor = System.Drawing.Color.Blue;
            this.btn_RsConfig.bEnabled = true;
            this.btn_RsConfig.bOn = true;
            this.btn_RsConfig.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_RsConfig.Location = new System.Drawing.Point(164, 12);
            this.btn_RsConfig.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btn_RsConfig.Name = "btn_RsConfig";
            this.btn_RsConfig.Size = new System.Drawing.Size(136, 36);
            this.btn_RsConfig.TabIndex = 3;
            // 
            // btn_quit
            // 
            this.btn_quit.BackColor = System.Drawing.Color.Red;
            this.btn_quit.bEnabled = true;
            this.btn_quit.bOn = true;
            this.btn_quit.Location = new System.Drawing.Point(674, 1);
            this.btn_quit.Name = "btn_quit";
            this.btn_quit.Size = new System.Drawing.Size(71, 71);
            this.btn_quit.TabIndex = 0;
            // 
            // lbl_range
            // 
            this.lbl_range.AutoSize = true;
            this.lbl_range.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_range.Location = new System.Drawing.Point(25, 272);
            this.lbl_range.Name = "lbl_range";
            this.lbl_range.Size = new System.Drawing.Size(101, 25);
            this.lbl_range.TabIndex = 2;
            this.lbl_range.Text = "lbl_range";
            // 
            // btn_rangeP001
            // 
            this.btn_rangeP001.BackColor = System.Drawing.Color.Red;
            this.btn_rangeP001.bEnabled = true;
            this.btn_rangeP001.bOn = true;
            this.btn_rangeP001.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_rangeP001.Location = new System.Drawing.Point(167, 262);
            this.btn_rangeP001.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.btn_rangeP001.Name = "btn_rangeP001";
            this.btn_rangeP001.Size = new System.Drawing.Size(71, 46);
            this.btn_rangeP001.TabIndex = 0;
            // 
            // btn_rangeP01
            // 
            this.btn_rangeP01.BackColor = System.Drawing.Color.Red;
            this.btn_rangeP01.bEnabled = true;
            this.btn_rangeP01.bOn = true;
            this.btn_rangeP01.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_rangeP01.Location = new System.Drawing.Point(258, 262);
            this.btn_rangeP01.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.btn_rangeP01.Name = "btn_rangeP01";
            this.btn_rangeP01.Size = new System.Drawing.Size(71, 46);
            this.btn_rangeP01.TabIndex = 0;
            // 
            // btn_rangeP1
            // 
            this.btn_rangeP1.BackColor = System.Drawing.Color.Red;
            this.btn_rangeP1.bEnabled = true;
            this.btn_rangeP1.bOn = true;
            this.btn_rangeP1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_rangeP1.Location = new System.Drawing.Point(339, 262);
            this.btn_rangeP1.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.btn_rangeP1.Name = "btn_rangeP1";
            this.btn_rangeP1.Size = new System.Drawing.Size(71, 46);
            this.btn_rangeP1.TabIndex = 0;
            // 
            // btn_range1
            // 
            this.btn_range1.BackColor = System.Drawing.Color.Red;
            this.btn_range1.bEnabled = true;
            this.btn_range1.bOn = true;
            this.btn_range1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_range1.Location = new System.Drawing.Point(423, 262);
            this.btn_range1.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.btn_range1.Name = "btn_range1";
            this.btn_range1.Size = new System.Drawing.Size(71, 46);
            this.btn_range1.TabIndex = 0;
            // 
            // btn_range10
            // 
            this.btn_range10.BackColor = System.Drawing.Color.Red;
            this.btn_range10.bEnabled = true;
            this.btn_range10.bOn = true;
            this.btn_range10.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_range10.Location = new System.Drawing.Point(513, 262);
            this.btn_range10.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.btn_range10.Name = "btn_range10";
            this.btn_range10.Size = new System.Drawing.Size(71, 46);
            this.btn_range10.TabIndex = 0;
            // 
            // btn_range100
            // 
            this.btn_range100.BackColor = System.Drawing.Color.Red;
            this.btn_range100.bEnabled = true;
            this.btn_range100.bOn = true;
            this.btn_range100.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_range100.Location = new System.Drawing.Point(167, 318);
            this.btn_range100.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.btn_range100.Name = "btn_range100";
            this.btn_range100.Size = new System.Drawing.Size(71, 46);
            this.btn_range100.TabIndex = 0;
            // 
            // btn_range1k
            // 
            this.btn_range1k.BackColor = System.Drawing.Color.Red;
            this.btn_range1k.bEnabled = true;
            this.btn_range1k.bOn = true;
            this.btn_range1k.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_range1k.Location = new System.Drawing.Point(258, 318);
            this.btn_range1k.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.btn_range1k.Name = "btn_range1k";
            this.btn_range1k.Size = new System.Drawing.Size(71, 46);
            this.btn_range1k.TabIndex = 0;
            // 
            // btn_range10k
            // 
            this.btn_range10k.BackColor = System.Drawing.Color.Red;
            this.btn_range10k.bEnabled = true;
            this.btn_range10k.bOn = true;
            this.btn_range10k.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_range10k.Location = new System.Drawing.Point(339, 318);
            this.btn_range10k.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.btn_range10k.Name = "btn_range10k";
            this.btn_range10k.Size = new System.Drawing.Size(71, 46);
            this.btn_range10k.TabIndex = 0;
            // 
            // btn_range100k
            // 
            this.btn_range100k.BackColor = System.Drawing.Color.Red;
            this.btn_range100k.bEnabled = true;
            this.btn_range100k.bOn = true;
            this.btn_range100k.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_range100k.Location = new System.Drawing.Point(423, 318);
            this.btn_range100k.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.btn_range100k.Name = "btn_range100k";
            this.btn_range100k.Size = new System.Drawing.Size(71, 46);
            this.btn_range100k.TabIndex = 0;
            // 
            // RxConfigWnd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(745, 502);
            this.ControlBox = false;
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.btn_SysConfig);
            this.Controls.Add(this.btn_RxConfig);
            this.Controls.Add(this.btn_RsConfig);
            this.Controls.Add(this.btn_quit);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "RxConfigWnd";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "RxConfigWnd";
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private RectButton btn_quit;
        private System.Windows.Forms.Label lbl_stdchan;
        private RectButton btn_stdchan;
        private RectButton btn_current;
        private System.Windows.Forms.Label lbl_current;
        private RectButton btn_sqrt;
        private RectButton btn_chan;
        private System.Windows.Forms.Label lbl_serial;
        private RectButton btn_serial;
        private RectButton btn_SysConfig;
        private RectButton btn_RxConfig;
        private RectButton btn_RsConfig;
        private System.Windows.Forms.Panel panel1;
        private RectButton btn_enable;
        private System.Windows.Forms.Label lbl_vmode;
        private RectButton btn_vmode;
        private RectButton btn_next;
        private RectButton btn_last;
        private System.Windows.Forms.Label lbl_range;
        private RectButton btn_range100k;
        private RectButton btn_range10k;
        private RectButton btn_range1k;
        private RectButton btn_range100;
        private RectButton btn_range10;
        private RectButton btn_range1;
        private RectButton btn_rangeP1;
        private RectButton btn_rangeP01;
        private RectButton btn_rangeP001;
    }
}