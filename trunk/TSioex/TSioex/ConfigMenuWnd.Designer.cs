namespace TSioex
{
    partial class ConfigMenuWnd
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ConfigMenuWnd));
            this.btn_return = new System.Windows.Forms.PictureBox();
            this.lbl_title = new System.Windows.Forms.Label();
            this.btn_eng = new TSioex.RectButton();
            this.btn_pwd = new TSioex.RectButton();
            this.btn_alert = new TSioex.RectButton();
            this.btn_bottom = new TSioex.RectButton();
            this.btn_history = new TSioex.RectButton();
            this.btn_product = new TSioex.RectButton();
            this.SuspendLayout();
            // 
            // btn_return
            // 
            this.btn_return.Image = ((System.Drawing.Image)(resources.GetObject("btn_return.Image")));
            this.btn_return.Location = new System.Drawing.Point(389, 639);
            this.btn_return.Name = "btn_return";
            this.btn_return.Size = new System.Drawing.Size(175, 67);
            this.btn_return.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            // 
            // lbl_title
            // 
            this.lbl_title.Font = new System.Drawing.Font("Arial", 36F, System.Drawing.FontStyle.Regular);
            this.lbl_title.ForeColor = System.Drawing.Color.Green;
            this.lbl_title.Location = new System.Drawing.Point(365, 36);
            this.lbl_title.Name = "lbl_title";
            this.lbl_title.Size = new System.Drawing.Size(287, 64);
            this.lbl_title.Text = "label11";
            this.lbl_title.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // btn_eng
            // 
            this.btn_eng.Font = new System.Drawing.Font("Arial", 36F, System.Drawing.FontStyle.Regular);
            this.btn_eng.Location = new System.Drawing.Point(566, 471);
            this.btn_eng.Name = "btn_eng";
            this.btn_eng.Size = new System.Drawing.Size(342, 101);
            this.btn_eng.TabIndex = 13;
            // 
            // btn_pwd
            // 
            this.btn_pwd.Font = new System.Drawing.Font("Arial", 36F, System.Drawing.FontStyle.Regular);
            this.btn_pwd.Location = new System.Drawing.Point(74, 471);
            this.btn_pwd.Name = "btn_pwd";
            this.btn_pwd.Size = new System.Drawing.Size(342, 101);
            this.btn_pwd.TabIndex = 12;
            // 
            // btn_alert
            // 
            this.btn_alert.Font = new System.Drawing.Font("Arial", 36F, System.Drawing.FontStyle.Regular);
            this.btn_alert.Location = new System.Drawing.Point(566, 310);
            this.btn_alert.Name = "btn_alert";
            this.btn_alert.Size = new System.Drawing.Size(342, 101);
            this.btn_alert.TabIndex = 11;
            // 
            // btn_bottom
            // 
            this.btn_bottom.Font = new System.Drawing.Font("Arial", 36F, System.Drawing.FontStyle.Regular);
            this.btn_bottom.Location = new System.Drawing.Point(74, 310);
            this.btn_bottom.Name = "btn_bottom";
            this.btn_bottom.Size = new System.Drawing.Size(342, 101);
            this.btn_bottom.TabIndex = 10;
            // 
            // btn_history
            // 
            this.btn_history.Font = new System.Drawing.Font("Arial", 36F, System.Drawing.FontStyle.Regular);
            this.btn_history.Location = new System.Drawing.Point(566, 143);
            this.btn_history.Name = "btn_history";
            this.btn_history.Size = new System.Drawing.Size(342, 101);
            this.btn_history.TabIndex = 9;
            // 
            // btn_product
            // 
            this.btn_product.Font = new System.Drawing.Font("Arial", 36F, System.Drawing.FontStyle.Regular);
            this.btn_product.Location = new System.Drawing.Point(74, 143);
            this.btn_product.Name = "btn_product";
            this.btn_product.Size = new System.Drawing.Size(342, 101);
            this.btn_product.TabIndex = 6;
            // 
            // ConfigMenuWnd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(1024, 768);
            this.ControlBox = false;
            this.Controls.Add(this.btn_eng);
            this.Controls.Add(this.btn_pwd);
            this.Controls.Add(this.btn_alert);
            this.Controls.Add(this.btn_bottom);
            this.Controls.Add(this.btn_history);
            this.Controls.Add(this.btn_product);
            this.Controls.Add(this.btn_return);
            this.Controls.Add(this.lbl_title);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "ConfigMenuWnd";
            this.Text = "ConfigMenuWnd";
            this.ResumeLayout(false);

        }

        #endregion

        private RectButton btn_product;
        private System.Windows.Forms.PictureBox btn_return;
        private System.Windows.Forms.Label lbl_title;
        private RectButton btn_history;
        private RectButton btn_alert;
        private RectButton btn_bottom;
        private RectButton btn_eng;
        private RectButton btn_pwd;
    }
}