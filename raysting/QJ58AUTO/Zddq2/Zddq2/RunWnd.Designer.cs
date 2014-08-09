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
            this.statusBar1 = new Zddq2.StatusBar();
            this.rxDisplay1 = new Zddq2.RxDisplay();
            this.rxDisplay2 = new Zddq2.RxDisplay();
            this.rxDisplay3 = new Zddq2.RxDisplay();
            this.rxDisplay4 = new Zddq2.RxDisplay();
            this.dataGrid2 = new System.Windows.Forms.DataGridView();
            this.graphPane2 = new Zddq2.GraphPane();
            this.btn_dispmode = new Raysting.Controls.RectButton();
            ((System.ComponentModel.ISupportInitialize)(this.dataGrid2)).BeginInit();
            this.SuspendLayout();
            // 
            // statusBar1
            // 
            this.statusBar1.BackColor = System.Drawing.Color.Black;
            this.statusBar1.count = 0;
            this.statusBar1.Location = new System.Drawing.Point(0, 553);
            this.statusBar1.message = null;
            this.statusBar1.Name = "statusBar1";
            this.statusBar1.pause = false;
            this.statusBar1.Size = new System.Drawing.Size(801, 49);
            this.statusBar1.status = Zddq2.RUN_STATE.DISABLED;
            this.statusBar1.TabIndex = 0;
            this.statusBar1.total = 0;
            this.statusBar1.waiting = 0;
            // 
            // rxDisplay1
            // 
            this.rxDisplay1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.rxDisplay1.BackColor = System.Drawing.Color.Green;
            this.rxDisplay1.Font = new System.Drawing.Font("Microsoft Sans Serif", 36F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rxDisplay1.Location = new System.Drawing.Point(2, 12);
            this.rxDisplay1.Margin = new System.Windows.Forms.Padding(8, 7, 8, 7);
            this.rxDisplay1.myRx = null;
            this.rxDisplay1.Name = "rxDisplay1";
            this.rxDisplay1.Size = new System.Drawing.Size(786, 133);
            this.rxDisplay1.TabIndex = 1;
            // 
            // rxDisplay2
            // 
            this.rxDisplay2.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.rxDisplay2.BackColor = System.Drawing.Color.Green;
            this.rxDisplay2.Location = new System.Drawing.Point(2, 9);
            this.rxDisplay2.myRx = null;
            this.rxDisplay2.Name = "rxDisplay2";
            this.rxDisplay2.Size = new System.Drawing.Size(786, 130);
            this.rxDisplay2.TabIndex = 1;
            // 
            // rxDisplay3
            // 
            this.rxDisplay3.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.rxDisplay3.BackColor = System.Drawing.Color.Green;
            this.rxDisplay3.Location = new System.Drawing.Point(2, 9);
            this.rxDisplay3.myRx = null;
            this.rxDisplay3.Name = "rxDisplay3";
            this.rxDisplay3.Size = new System.Drawing.Size(786, 133);
            this.rxDisplay3.TabIndex = 1;
            // 
            // rxDisplay4
            // 
            this.rxDisplay4.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.rxDisplay4.BackColor = System.Drawing.Color.Green;
            this.rxDisplay4.Location = new System.Drawing.Point(2, 12);
            this.rxDisplay4.myRx = null;
            this.rxDisplay4.Name = "rxDisplay4";
            this.rxDisplay4.Size = new System.Drawing.Size(786, 127);
            this.rxDisplay4.TabIndex = 1;
            // 
            // dataGrid2
            // 
            this.dataGrid2.BackgroundColor = System.Drawing.Color.LightGreen;
            this.dataGrid2.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGrid2.GridColor = System.Drawing.Color.LightGreen;
            this.dataGrid2.Location = new System.Drawing.Point(2, 173);
            this.dataGrid2.Name = "dataGrid2";
            this.dataGrid2.Size = new System.Drawing.Size(786, 152);
            this.dataGrid2.TabIndex = 3;
            // 
            // graphPane2
            // 
            this.graphPane2.Location = new System.Drawing.Point(2, 341);
            this.graphPane2.Name = "graphPane2";
            this.graphPane2.Size = new System.Drawing.Size(786, 191);
            this.graphPane2.TabIndex = 4;
            this.graphPane2.XAxisMax = 10;
            this.graphPane2.XAxisTitle = "";
            this.graphPane2.YAxisMax = 0;
            this.graphPane2.YAxisMin = 0;
            // 
            // btn_dispmode
            // 
            this.btn_dispmode.BackColor = System.Drawing.Color.Blue;
            this.btn_dispmode.bgScale = 3;
            this.btn_dispmode.bOn = true;
            this.btn_dispmode.colorShadow = System.Drawing.Color.LightGray;
            this.btn_dispmode.colorTop = System.Drawing.Color.Pink;
            this.btn_dispmode.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_dispmode.Label = null;
            this.btn_dispmode.Location = new System.Drawing.Point(630, 145);
            this.btn_dispmode.Name = "btn_dispmode";
            this.btn_dispmode.Size = new System.Drawing.Size(91, 42);
            this.btn_dispmode.Style = Raysting.Controls.MyButtonType.roundButton;
            this.btn_dispmode.TabIndex = 5;
            // 
            // RunWnd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Black;
            this.ClientSize = new System.Drawing.Size(800, 600);
            this.Controls.Add(this.btn_dispmode);
            this.Controls.Add(this.graphPane2);
            this.Controls.Add(this.dataGrid2);
            this.Controls.Add(this.statusBar1);
            this.Controls.Add(this.rxDisplay1);
            this.Controls.Add(this.rxDisplay2);
            this.Controls.Add(this.rxDisplay3);
            this.Controls.Add(this.rxDisplay4);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "RunWnd";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.RunWnd_Load_1);
            ((System.ComponentModel.ISupportInitialize)(this.dataGrid2)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private StatusBar statusBar1;
        private RxDisplay rxDisplay1;
        private RxDisplay rxDisplay2;
        private RxDisplay rxDisplay3;
        private RxDisplay rxDisplay4;
        private System.Windows.Forms.DataGridView dataGrid2;
        private GraphPane graphPane2;
        private Raysting.Controls.RectButton btn_dispmode;

    }
}

