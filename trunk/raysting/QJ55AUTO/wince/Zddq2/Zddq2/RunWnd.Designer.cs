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
            this.statusBar1.Location = new System.Drawing.Point(0, 714);
            this.statusBar1.Name = "statusBar1";
            this.statusBar1.Size = new System.Drawing.Size(1024, 54);
            this.statusBar1.TabIndex = 0;
            // 
            // rxDisplay1
            // 
            this.rxDisplay1.Location = new System.Drawing.Point(0, 0);
            this.rxDisplay1.Name = "rxDisplay1";
            this.rxDisplay1.Size = new System.Drawing.Size(1024, 715);
            this.rxDisplay1.TabIndex = 1;
            // 
            // rxDisplay2
            // 
            this.rxDisplay2.Location = new System.Drawing.Point(421, 72);
            this.rxDisplay2.Name = "rxDisplay2";
            this.rxDisplay2.Size = new System.Drawing.Size(150, 150);
            this.rxDisplay2.TabIndex = 1;
            // 
            // rxDisplay3
            // 
            this.rxDisplay3.Location = new System.Drawing.Point(99, 264);
            this.rxDisplay3.Name = "rxDisplay3";
            this.rxDisplay3.Size = new System.Drawing.Size(150, 150);
            this.rxDisplay3.TabIndex = 1;
            // 
            // rxDisplay4
            // 
            this.rxDisplay4.Location = new System.Drawing.Point(421, 264);
            this.rxDisplay4.Name = "rxDisplay4";
            this.rxDisplay4.Size = new System.Drawing.Size(150, 150);
            this.rxDisplay4.TabIndex = 1;
            // 
            // RunWnd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.Black;
            this.ClientSize = new System.Drawing.Size(1024, 768);
            this.ControlBox = false;
            this.Controls.Add(this.rxDisplay4);
            this.Controls.Add(this.rxDisplay3);
            this.Controls.Add(this.rxDisplay2);
            this.Controls.Add(this.rxDisplay1);
            this.Controls.Add(this.statusBar1);
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

