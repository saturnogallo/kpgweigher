namespace TSioex
{
    partial class SchemeItem2
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
            this.picBox = new System.Windows.Forms.PictureBox();
            this.txtBox = new System.Windows.Forms.TextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.delBtn = new TSioex.RectButton();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // picBox
            // 
            this.picBox.Location = new System.Drawing.Point(-1, -1);
            this.picBox.Name = "picBox";
            this.picBox.Size = new System.Drawing.Size(227, 99);
            this.picBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            // 
            // txtBox
            // 
            this.txtBox.BackColor = System.Drawing.Color.DimGray;
            this.txtBox.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.txtBox.Location = new System.Drawing.Point(3, 105);
            this.txtBox.Multiline = true;
            this.txtBox.Name = "txtBox";
            this.txtBox.ReadOnly = true;
            this.txtBox.Size = new System.Drawing.Size(227, 57);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.picBox);
            this.panel1.Location = new System.Drawing.Point(3, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(227, 99);
            // 
            // delBtn
            // 
            this.delBtn.bEnabled = false;
            this.delBtn.Location = new System.Drawing.Point(174, 105);
            this.delBtn.Name = "delBtn";
            this.delBtn.Size = new System.Drawing.Size(56, 57);
            // 
            // SchemeItem2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.Controls.Add(this.delBtn);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.txtBox);
            this.Name = "SchemeItem2";
            this.Size = new System.Drawing.Size(233, 165);
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);
        }

        #endregion

        private System.Windows.Forms.PictureBox picBox;
        private System.Windows.Forms.TextBox txtBox;
        private RectButton delBtn;
        private System.Windows.Forms.Panel panel1;
    }
}
