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
            this.SuspendLayout();
            // 
            // statusBar1
            // 
            this.statusBar1.BackColor = System.Drawing.Color.Black;
            this.statusBar1.count = 0;
            this.statusBar1.Location = new System.Drawing.Point(0, 553);
            this.statusBar1.message = null;
            this.statusBar1.Name = "statusBar1";
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
            this.rxDisplay1.Location = new System.Drawing.Point(2, 9);
            this.rxDisplay1.Margin = new System.Windows.Forms.Padding(8, 7, 8, 7);
            this.rxDisplay1.myRx = null;
            this.rxDisplay1.Name = "rxDisplay1";
            this.rxDisplay1.Size = new System.Drawing.Size(700, 500);
            this.rxDisplay1.TabIndex = 1;
            // 
            // rxDisplay2
            // 
            this.rxDisplay2.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.rxDisplay2.BackColor = System.Drawing.Color.Green;
            this.rxDisplay2.Location = new System.Drawing.Point(455, 12);
            this.rxDisplay2.myRx = null;
            this.rxDisplay2.Name = "rxDisplay2";
            this.rxDisplay2.Size = new System.Drawing.Size(700, 500);
            this.rxDisplay2.TabIndex = 1;
            // 
            // rxDisplay3
            // 
            this.rxDisplay3.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.rxDisplay3.BackColor = System.Drawing.Color.Green;
            this.rxDisplay3.Location = new System.Drawing.Point(2, 152);
            this.rxDisplay3.myRx = null;
            this.rxDisplay3.Name = "rxDisplay3";
            this.rxDisplay3.Size = new System.Drawing.Size(700, 500);
            this.rxDisplay3.TabIndex = 1;
            // 
            // rxDisplay4
            // 
            this.rxDisplay4.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.rxDisplay4.BackColor = System.Drawing.Color.Green;
            this.rxDisplay4.Location = new System.Drawing.Point(455, 143);
            this.rxDisplay4.myRx = null;
            this.rxDisplay4.Name = "rxDisplay4";
            this.rxDisplay4.Size = new System.Drawing.Size(700, 500);
            this.rxDisplay4.TabIndex = 1;
            // 
            // RunWnd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Black;
            this.ClientSize = new System.Drawing.Size(800, 600);
            this.Controls.Add(this.statusBar1);
            this.Controls.Add(this.rxDisplay1);
            this.Controls.Add(this.rxDisplay2);
            this.Controls.Add(this.rxDisplay3);
            this.Controls.Add(this.rxDisplay4);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "RunWnd";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private StatusBar statusBar1;
        private RxDisplay rxDisplay1;
        private RxDisplay rxDisplay2;
        private RxDisplay rxDisplay3;
        private RxDisplay rxDisplay4;

    }
}

