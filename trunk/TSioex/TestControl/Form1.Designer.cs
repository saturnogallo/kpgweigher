namespace TestControl
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.MainMenu mainMenu1;

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
            this.mainMenu1 = new System.Windows.Forms.MainMenu();
            this.runPan1 = new TestControl.RunPan();
            this.rectButton1 = new TestControl.RectButton();
            this.SuspendLayout();
            // 
            // runPan1
            // 
            this.runPan1.Location = new System.Drawing.Point(3, 32);
            this.runPan1.Name = "runPan1";
            this.runPan1.Size = new System.Drawing.Size(237, 219);
            this.runPan1.TabIndex = 1;
            // 
            // rectButton1
            // 
            this.rectButton1.bEnabled = false;
            this.rectButton1.Location = new System.Drawing.Point(14, 3);
            this.rectButton1.Name = "rectButton1";
            this.rectButton1.Size = new System.Drawing.Size(181, 23);
            this.rectButton1.TabIndex = 0;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(240, 268);
            this.Controls.Add(this.runPan1);
            this.Controls.Add(this.rectButton1);
            this.Menu = this.mainMenu1;
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private RectButton rectButton1;
        private RunPan runPan1;
    }
}

