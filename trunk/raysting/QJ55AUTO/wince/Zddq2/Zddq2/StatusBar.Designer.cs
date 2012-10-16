namespace Zddq2
{
    partial class StatusBar
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.lbl_datetime = new System.Windows.Forms.Label();
            this.bt_run = new System.Windows.Forms.Button();
            this.bt_menu = new Zddq2.RectButton();
            this.SuspendLayout();
            // 
            // lbl_datetime
            // 
            this.lbl_datetime.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular);
            this.lbl_datetime.ForeColor = System.Drawing.Color.MintCream;
            this.lbl_datetime.Location = new System.Drawing.Point(687, 13);
            this.lbl_datetime.Name = "lbl_datetime";
            this.lbl_datetime.Size = new System.Drawing.Size(98, 25);
            this.lbl_datetime.Text = "10:32PM";
            // 
            // bt_run
            // 
            this.bt_run.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Regular);
            this.bt_run.Location = new System.Drawing.Point(137, 4);
            this.bt_run.Name = "bt_run";
            this.bt_run.Size = new System.Drawing.Size(530, 38);
            this.bt_run.TabIndex = 2;
            this.bt_run.Text = "Start/Stop";
            // 
            // bt_menu
            // 
            this.bt_menu.BackColor = System.Drawing.Color.Blue;
            this.bt_menu.bEnabled = false;
            this.bt_menu.bOn = true;
            this.bt_menu.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold);
            this.bt_menu.Location = new System.Drawing.Point(13, 4);
            this.bt_menu.Name = "bt_menu";
            this.bt_menu.Size = new System.Drawing.Size(105, 42);
            this.bt_menu.TabIndex = 0;
            // 
            // StatusBar
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.LightBlue;
            this.Controls.Add(this.bt_run);
            this.Controls.Add(this.lbl_datetime);
            this.Controls.Add(this.bt_menu);
            this.Name = "StatusBar";
            this.Size = new System.Drawing.Size(800, 49);
            this.ResumeLayout(false);

        }

        #endregion

        private RectButton bt_menu;
        private System.Windows.Forms.Label lbl_datetime;
        private System.Windows.Forms.Button bt_run;
    }
}
