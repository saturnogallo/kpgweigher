namespace TSioex
{
    partial class AlertWnd
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AlertWnd));
            this.btn_ret_run = new System.Windows.Forms.PictureBox();
            this.btn_ret_config = new System.Windows.Forms.PictureBox();
            this.lbl_title = new System.Windows.Forms.Label();
            this.btn_manual_reset = new TSioex.RectButton();
            this.btn_show_alert = new TSioex.RectButton();
            this.btn_stop_onalert = new TSioex.RectButton();
            this.btn_turnon_alert = new TSioex.RectButton();
            this.SuspendLayout();
            // 
            // btn_ret_run
            // 
            this.btn_ret_run.Image = ((System.Drawing.Image)(resources.GetObject("btn_ret_run.Image")));
            this.btn_ret_run.Location = new System.Drawing.Point(559, 604);
            this.btn_ret_run.Name = "btn_ret_run";
            this.btn_ret_run.Size = new System.Drawing.Size(175, 67);
            this.btn_ret_run.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            // 
            // btn_ret_config
            // 
            this.btn_ret_config.Image = ((System.Drawing.Image)(resources.GetObject("btn_ret_config.Image")));
            this.btn_ret_config.Location = new System.Drawing.Point(273, 604);
            this.btn_ret_config.Name = "btn_ret_config";
            this.btn_ret_config.Size = new System.Drawing.Size(157, 67);
            this.btn_ret_config.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            // 
            // lbl_title
            // 
            this.lbl_title.Font = new System.Drawing.Font("Arial", 36F, System.Drawing.FontStyle.Regular);
            this.lbl_title.ForeColor = System.Drawing.Color.Green;
            this.lbl_title.Location = new System.Drawing.Point(379, 28);
            this.lbl_title.Name = "lbl_title";
            this.lbl_title.Size = new System.Drawing.Size(287, 64);
            this.lbl_title.Text = "label11";
            this.lbl_title.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // btn_manual_reset
            // 
            this.btn_manual_reset.Font = new System.Drawing.Font("Arial", 36F, System.Drawing.FontStyle.Regular);
            this.btn_manual_reset.Location = new System.Drawing.Point(559, 367);
            this.btn_manual_reset.Name = "btn_manual_reset";
            this.btn_manual_reset.Size = new System.Drawing.Size(342, 122);
            this.btn_manual_reset.TabIndex = 6;
            // 
            // btn_show_alert
            // 
            this.btn_show_alert.Font = new System.Drawing.Font("Arial", 36F, System.Drawing.FontStyle.Regular);
            this.btn_show_alert.Location = new System.Drawing.Point(88, 367);
            this.btn_show_alert.Name = "btn_show_alert";
            this.btn_show_alert.Size = new System.Drawing.Size(342, 122);
            this.btn_show_alert.TabIndex = 5;
            // 
            // btn_stop_onalert
            // 
            this.btn_stop_onalert.Font = new System.Drawing.Font("Arial", 36F, System.Drawing.FontStyle.Regular);
            this.btn_stop_onalert.Location = new System.Drawing.Point(559, 135);
            this.btn_stop_onalert.Name = "btn_stop_onalert";
            this.btn_stop_onalert.Size = new System.Drawing.Size(342, 122);
            this.btn_stop_onalert.TabIndex = 4;
            // 
            // btn_turnon_alert
            // 
            this.btn_turnon_alert.Font = new System.Drawing.Font("Arial", 36F, System.Drawing.FontStyle.Regular);
            this.btn_turnon_alert.Location = new System.Drawing.Point(88, 135);
            this.btn_turnon_alert.Name = "btn_turnon_alert";
            this.btn_turnon_alert.Size = new System.Drawing.Size(342, 122);
            this.btn_turnon_alert.TabIndex = 3;
            // 
            // AlertWnd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(1024, 768);
            this.ControlBox = false;
            this.Controls.Add(this.btn_manual_reset);
            this.Controls.Add(this.btn_show_alert);
            this.Controls.Add(this.btn_stop_onalert);
            this.Controls.Add(this.btn_turnon_alert);
            this.Controls.Add(this.btn_ret_run);
            this.Controls.Add(this.btn_ret_config);
            this.Controls.Add(this.lbl_title);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AlertWnd";
            this.Text = "AlertWnd";
            this.TopMost = false;
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox btn_ret_run;
        private System.Windows.Forms.PictureBox btn_ret_config;
        private System.Windows.Forms.Label lbl_title;
        private RectButton btn_turnon_alert;
        private RectButton btn_stop_onalert;
        private RectButton btn_manual_reset;
        private RectButton btn_show_alert;
    }
}