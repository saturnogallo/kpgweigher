namespace Zddq2
{
    partial class RxDisplay
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
            this.lbl_reading = new System.Windows.Forms.Label();
            this.lbl_aux1 = new System.Windows.Forms.Label();
            this.lbl_aux2 = new System.Windows.Forms.Label();
            this.dataGrid2 = new System.Windows.Forms.DataGrid();
            this.graphPane2 = new Zddq2.GraphPane();
            this.btn_aux1 = new Zddq2.RectButton();
            this.btn_aux2 = new Zddq2.RectButton();
            this.btn_bigdisp = new Zddq2.RectButton();
            this.btn_dispmode = new Zddq2.RectButton();
            this.btn_left = new Zddq2.RectButton();
            this.btn_right = new Zddq2.RectButton();
            this.SuspendLayout();
            // 
            // lbl_reading
            // 
            this.lbl_reading.BackColor = System.Drawing.Color.LightGreen;
            this.lbl_reading.Font = new System.Drawing.Font("Microsoft Sans Serif", 48F, System.Drawing.FontStyle.Regular);
            this.lbl_reading.Location = new System.Drawing.Point(166, 4);
            this.lbl_reading.Name = "lbl_reading";
            this.lbl_reading.Size = new System.Drawing.Size(678, 73);
            this.lbl_reading.Text = "0.0000000000";
            // 
            // lbl_aux1
            // 
            this.lbl_aux1.BackColor = System.Drawing.Color.LightGreen;
            this.lbl_aux1.Font = new System.Drawing.Font("Arial", 26F, System.Drawing.FontStyle.Regular);
            this.lbl_aux1.Location = new System.Drawing.Point(166, 85);
            this.lbl_aux1.Name = "lbl_aux1";
            this.lbl_aux1.Size = new System.Drawing.Size(281, 49);
            this.lbl_aux1.Text = "0.0000000000";
            this.lbl_aux1.ParentChanged += new System.EventHandler(this.lbl_aux1_ParentChanged);
            // 
            // lbl_aux2
            // 
            this.lbl_aux2.BackColor = System.Drawing.Color.LightGreen;
            this.lbl_aux2.Font = new System.Drawing.Font("Arial", 26F, System.Drawing.FontStyle.Regular);
            this.lbl_aux2.Location = new System.Drawing.Point(593, 85);
            this.lbl_aux2.Name = "lbl_aux2";
            this.lbl_aux2.Size = new System.Drawing.Size(251, 49);
            this.lbl_aux2.Text = "0.0000000000";
            // 
            // dataGrid2
            // 
            this.dataGrid2.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(128)))), ((int)(((byte)(128)))));
            this.dataGrid2.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Regular);
            this.dataGrid2.Location = new System.Drawing.Point(35, 147);
            this.dataGrid2.Name = "dataGrid2";
            this.dataGrid2.Size = new System.Drawing.Size(941, 257);
            this.dataGrid2.TabIndex = 8;
            // 
            // graphPane2
            // 
            this.graphPane2.Location = new System.Drawing.Point(35, 410);
            this.graphPane2.Name = "graphPane2";
            this.graphPane2.Size = new System.Drawing.Size(941, 295);
            this.graphPane2.TabIndex = 7;
            // 
            // btn_aux1
            // 
            this.btn_aux1.BackColor = System.Drawing.Color.Blue;
            this.btn_aux1.bEnabled = true;
            this.btn_aux1.bOn = true;
            this.btn_aux1.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.btn_aux1.Location = new System.Drawing.Point(47, 85);
            this.btn_aux1.Name = "btn_aux1";
            this.btn_aux1.Size = new System.Drawing.Size(90, 44);
            this.btn_aux1.TabIndex = 0;
            // 
            // btn_aux2
            // 
            this.btn_aux2.BackColor = System.Drawing.Color.Blue;
            this.btn_aux2.bEnabled = true;
            this.btn_aux2.bOn = true;
            this.btn_aux2.Font = new System.Drawing.Font("Tahoma", 16F, System.Drawing.FontStyle.Bold);
            this.btn_aux2.Location = new System.Drawing.Point(482, 87);
            this.btn_aux2.Name = "btn_aux2";
            this.btn_aux2.Size = new System.Drawing.Size(95, 43);
            this.btn_aux2.TabIndex = 1;
            // 
            // btn_bigdisp
            // 
            this.btn_bigdisp.BackColor = System.Drawing.Color.Blue;
            this.btn_bigdisp.bEnabled = true;
            this.btn_bigdisp.bOn = true;
            this.btn_bigdisp.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.btn_bigdisp.Location = new System.Drawing.Point(35, 13);
            this.btn_bigdisp.Name = "btn_bigdisp";
            this.btn_bigdisp.Size = new System.Drawing.Size(90, 55);
            this.btn_bigdisp.TabIndex = 2;
            // 
            // btn_dispmode
            // 
            this.btn_dispmode.BackColor = System.Drawing.Color.Blue;
            this.btn_dispmode.bEnabled = true;
            this.btn_dispmode.bOn = true;
            this.btn_dispmode.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this.btn_dispmode.Location = new System.Drawing.Point(872, 13);
            this.btn_dispmode.Name = "btn_dispmode";
            this.btn_dispmode.Size = new System.Drawing.Size(104, 55);
            this.btn_dispmode.TabIndex = 3;
            // 
            // btn_left
            // 
            this.btn_left.BackColor = System.Drawing.Color.Blue;
            this.btn_left.bEnabled = true;
            this.btn_left.bOn = true;
            this.btn_left.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.btn_left.Location = new System.Drawing.Point(25, 86);
            this.btn_left.Name = "btn_left";
            this.btn_left.Size = new System.Drawing.Size(22, 44);
            this.btn_left.TabIndex = 12;
            // 
            // btn_right
            // 
            this.btn_right.BackColor = System.Drawing.Color.Blue;
            this.btn_right.bEnabled = true;
            this.btn_right.bOn = true;
            this.btn_right.Font = new System.Drawing.Font("Arial", 16F, System.Drawing.FontStyle.Bold);
            this.btn_right.Location = new System.Drawing.Point(131, 86);
            this.btn_right.Name = "btn_right";
            this.btn_right.Size = new System.Drawing.Size(22, 44);
            this.btn_right.TabIndex = 13;
            // 
            // RxDisplay
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(101F, 101F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.Green;
            this.Controls.Add(this.dataGrid2);
            this.Controls.Add(this.graphPane2);
            this.Controls.Add(this.btn_aux1);
            this.Controls.Add(this.btn_aux2);
            this.Controls.Add(this.btn_bigdisp);
            this.Controls.Add(this.btn_dispmode);
            this.Controls.Add(this.lbl_aux2);
            this.Controls.Add(this.lbl_aux1);
            this.Controls.Add(this.lbl_reading);
            this.Controls.Add(this.btn_left);
            this.Controls.Add(this.btn_right);
            this.Name = "RxDisplay";
            this.Size = new System.Drawing.Size(1014, 718);
            this.Click += new System.EventHandler(this.RxDisplay_Click);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Label lbl_reading;
        private RectButton btn_dispmode;
        private RectButton btn_bigdisp;
        private RectButton btn_aux2;
        private System.Windows.Forms.Label lbl_aux1;
        private System.Windows.Forms.Label lbl_aux2;
        private RectButton btn_aux1;
        private GraphPane graphPane2;
        private System.Windows.Forms.DataGrid dataGrid2;
        private RectButton btn_left;
        private RectButton btn_right;
    }
}
