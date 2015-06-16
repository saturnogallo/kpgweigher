namespace Zddq2
{
    partial class RunWnd
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
            this.dataGrid2 = new System.Windows.Forms.DataGridView();
            this.lbl_aux2 = new System.Windows.Forms.Label();
            this.lbl_aux1 = new System.Windows.Forms.Label();
            this.lbl_bigdisp = new System.Windows.Forms.Label();
            this.bt_run = new System.Windows.Forms.Button();
            this.graphPane2 = new Zddq2.GraphPane();
            this.btn_dispchan = new Raysting.Controls.RectButton();
            this.rectButton3 = new Raysting.Controls.RectButton();
            this.btn_dispmode = new Raysting.Controls.RectButton();
            this.btn_menu = new Raysting.Controls.RectButton();
            this.btn_bigdisp = new Raysting.Controls.RectButton();
            this.btn_chan = new Raysting.Controls.RectButton();
            this.btn_aux1 = new Raysting.Controls.RectButton();
            this.btn_aux2 = new Raysting.Controls.RectButton();
            ((System.ComponentModel.ISupportInitialize)(this.dataGrid2)).BeginInit();
            this.SuspendLayout();
            // 
            // dataGrid2
            // 
            this.dataGrid2.BackgroundColor = System.Drawing.Color.LightGreen;
            this.dataGrid2.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGrid2.GridColor = System.Drawing.Color.LightGreen;
            this.dataGrid2.Location = new System.Drawing.Point(8, 168);
            this.dataGrid2.Name = "dataGrid2";
            this.dataGrid2.Size = new System.Drawing.Size(780, 370);
            this.dataGrid2.TabIndex = 3;
            // 
            // lbl_aux2
            // 
            this.lbl_aux2.AutoSize = true;
            this.lbl_aux2.BackColor = System.Drawing.Color.Green;
            this.lbl_aux2.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_aux2.Location = new System.Drawing.Point(517, 109);
            this.lbl_aux2.Name = "lbl_aux2";
            this.lbl_aux2.Size = new System.Drawing.Size(235, 39);
            this.lbl_aux2.TabIndex = 7;
            this.lbl_aux2.Text = "0.0000000000";
            // 
            // lbl_aux1
            // 
            this.lbl_aux1.AutoSize = true;
            this.lbl_aux1.BackColor = System.Drawing.Color.Green;
            this.lbl_aux1.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_aux1.Location = new System.Drawing.Point(120, 109);
            this.lbl_aux1.Name = "lbl_aux1";
            this.lbl_aux1.Size = new System.Drawing.Size(235, 39);
            this.lbl_aux1.TabIndex = 8;
            this.lbl_aux1.Text = "0.0000000000";
            // 
            // lbl_bigdisp
            // 
            this.lbl_bigdisp.AutoSize = true;
            this.lbl_bigdisp.BackColor = System.Drawing.Color.Green;
            this.lbl_bigdisp.Font = new System.Drawing.Font("Microsoft Sans Serif", 48F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_bigdisp.Location = new System.Drawing.Point(171, 12);
            this.lbl_bigdisp.Name = "lbl_bigdisp";
            this.lbl_bigdisp.Size = new System.Drawing.Size(470, 78);
            this.lbl_bigdisp.TabIndex = 9;
            this.lbl_bigdisp.Text = "0.0000000000";
            // 
            // bt_run
            // 
            this.bt_run.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bt_run.Location = new System.Drawing.Point(90, 553);
            this.bt_run.Name = "bt_run";
            this.bt_run.Size = new System.Drawing.Size(437, 40);
            this.bt_run.TabIndex = 15;
            this.bt_run.Text = "Start/Stop";
            this.bt_run.UseVisualStyleBackColor = true;
            // 
            // graphPane2
            // 
            this.graphPane2.Location = new System.Drawing.Point(8, 168);
            this.graphPane2.Name = "graphPane2";
            this.graphPane2.Size = new System.Drawing.Size(780, 370);
            this.graphPane2.TabIndex = 16;
            this.graphPane2.XAxisMax = 10;
            this.graphPane2.XAxisTitle = "";
            this.graphPane2.YAxisMax = 0;
            this.graphPane2.YAxisMin = 0;
            // 
            // btn_dispchan
            // 
            this.btn_dispchan.BackColor = System.Drawing.Color.Blue;
            this.btn_dispchan.bgScale = 3;
            this.btn_dispchan.bOn = true;
            this.btn_dispchan.colorShadow = System.Drawing.Color.LightGray;
            this.btn_dispchan.colorTop = System.Drawing.Color.Pink;
            this.btn_dispchan.Enabled = false;
            this.btn_dispchan.Label = null;
            this.btn_dispchan.Location = new System.Drawing.Point(533, 557);
            this.btn_dispchan.Name = "btn_dispchan";
            this.btn_dispchan.Size = new System.Drawing.Size(83, 35);
            this.btn_dispchan.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_dispchan.TabIndex = 13;
            // 
            // rectButton3
            // 
            this.rectButton3.BackColor = System.Drawing.Color.Blue;
            this.rectButton3.bgScale = 3;
            this.rectButton3.bOn = true;
            this.rectButton3.colorShadow = System.Drawing.Color.LightGray;
            this.rectButton3.colorTop = System.Drawing.Color.Pink;
            this.rectButton3.Enabled = false;
            this.rectButton3.Label = null;
            this.rectButton3.Location = new System.Drawing.Point(711, 557);
            this.rectButton3.Name = "rectButton3";
            this.rectButton3.Size = new System.Drawing.Size(85, 35);
            this.rectButton3.Style = Raysting.Controls.MyButtonType.roundButton;
            this.rectButton3.TabIndex = 13;
            // 
            // btn_dispmode
            // 
            this.btn_dispmode.BackColor = System.Drawing.Color.Blue;
            this.btn_dispmode.bgScale = 3;
            this.btn_dispmode.bOn = true;
            this.btn_dispmode.colorShadow = System.Drawing.Color.LightGray;
            this.btn_dispmode.colorTop = System.Drawing.Color.Pink;
            this.btn_dispmode.Enabled = false;
            this.btn_dispmode.Label = null;
            this.btn_dispmode.Location = new System.Drawing.Point(622, 557);
            this.btn_dispmode.Name = "btn_dispmode";
            this.btn_dispmode.Size = new System.Drawing.Size(83, 35);
            this.btn_dispmode.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_dispmode.TabIndex = 13;
            // 
            // btn_menu
            // 
            this.btn_menu.BackColor = System.Drawing.Color.Blue;
            this.btn_menu.bgScale = 3;
            this.btn_menu.bOn = true;
            this.btn_menu.colorShadow = System.Drawing.Color.LightGray;
            this.btn_menu.colorTop = System.Drawing.Color.Pink;
            this.btn_menu.Label = null;
            this.btn_menu.Location = new System.Drawing.Point(8, 557);
            this.btn_menu.Name = "btn_menu";
            this.btn_menu.Size = new System.Drawing.Size(76, 36);
            this.btn_menu.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_menu.TabIndex = 13;
            // 
            // btn_bigdisp
            // 
            this.btn_bigdisp.BackColor = System.Drawing.Color.Blue;
            this.btn_bigdisp.bgScale = 3;
            this.btn_bigdisp.bOn = true;
            this.btn_bigdisp.colorShadow = System.Drawing.Color.LightGray;
            this.btn_bigdisp.colorTop = System.Drawing.Color.Pink;
            this.btn_bigdisp.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_bigdisp.Label = null;
            this.btn_bigdisp.Location = new System.Drawing.Point(8, 12);
            this.btn_bigdisp.Name = "btn_bigdisp";
            this.btn_bigdisp.Size = new System.Drawing.Size(114, 77);
            this.btn_bigdisp.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_bigdisp.TabIndex = 12;
            // 
            // btn_chan
            // 
            this.btn_chan.BackColor = System.Drawing.Color.Blue;
            this.btn_chan.bgScale = 3;
            this.btn_chan.bOn = true;
            this.btn_chan.colorShadow = System.Drawing.Color.LightGray;
            this.btn_chan.colorTop = System.Drawing.Color.Pink;
            this.btn_chan.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_chan.Label = null;
            this.btn_chan.Location = new System.Drawing.Point(674, 13);
            this.btn_chan.Name = "btn_chan";
            this.btn_chan.Size = new System.Drawing.Size(114, 77);
            this.btn_chan.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_chan.TabIndex = 12;
            // 
            // btn_aux1
            // 
            this.btn_aux1.BackColor = System.Drawing.Color.Blue;
            this.btn_aux1.bgScale = 3;
            this.btn_aux1.bOn = true;
            this.btn_aux1.colorShadow = System.Drawing.Color.LightGray;
            this.btn_aux1.colorTop = System.Drawing.Color.Pink;
            this.btn_aux1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_aux1.Label = null;
            this.btn_aux1.Location = new System.Drawing.Point(8, 103);
            this.btn_aux1.Name = "btn_aux1";
            this.btn_aux1.Size = new System.Drawing.Size(79, 46);
            this.btn_aux1.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_aux1.TabIndex = 10;
            // 
            // btn_aux2
            // 
            this.btn_aux2.BackColor = System.Drawing.Color.Blue;
            this.btn_aux2.bgScale = 3;
            this.btn_aux2.bOn = true;
            this.btn_aux2.colorShadow = System.Drawing.Color.LightGray;
            this.btn_aux2.colorTop = System.Drawing.Color.Pink;
            this.btn_aux2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_aux2.Label = null;
            this.btn_aux2.Location = new System.Drawing.Point(417, 106);
            this.btn_aux2.Name = "btn_aux2";
            this.btn_aux2.Size = new System.Drawing.Size(84, 45);
            this.btn_aux2.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_aux2.TabIndex = 11;
            // 
            // RunWnd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Black;
            this.ClientSize = new System.Drawing.Size(800, 600);
            this.Controls.Add(this.graphPane2);
            this.Controls.Add(this.bt_run);
            this.Controls.Add(this.btn_dispchan);
            this.Controls.Add(this.rectButton3);
            this.Controls.Add(this.btn_dispmode);
            this.Controls.Add(this.btn_menu);
            this.Controls.Add(this.btn_bigdisp);
            this.Controls.Add(this.btn_chan);
            this.Controls.Add(this.btn_aux1);
            this.Controls.Add(this.btn_aux2);
            this.Controls.Add(this.lbl_aux2);
            this.Controls.Add(this.lbl_aux1);
            this.Controls.Add(this.lbl_bigdisp);
            this.Controls.Add(this.dataGrid2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "RunWnd";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.RunWnd_Load_1);
            ((System.ComponentModel.ISupportInitialize)(this.dataGrid2)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGrid2;
        private Raysting.Controls.RectButton btn_chan;
        private Raysting.Controls.RectButton btn_aux1;
        private Raysting.Controls.RectButton btn_aux2;
        private System.Windows.Forms.Label lbl_aux2;
        private System.Windows.Forms.Label lbl_aux1;
        private System.Windows.Forms.Label lbl_bigdisp;
        private System.Windows.Forms.Button bt_run;
        private Raysting.Controls.RectButton btn_menu;
        private Raysting.Controls.RectButton btn_dispmode;
        private Raysting.Controls.RectButton btn_dispchan;
        private Raysting.Controls.RectButton rectButton3;
        private Raysting.Controls.RectButton btn_bigdisp;
        private GraphPane graphPane2;

    }
}

