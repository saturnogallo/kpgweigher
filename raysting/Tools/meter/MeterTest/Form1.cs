using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace MeterTest
{
	/// <summary>
	/// Example on how to use the MeterControl. 
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TrackBar trackBar1;
		private MeterControl.Meter meter1;
		
		
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.meter1 = new MeterControl.Meter();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.SuspendLayout();
            // 
            // trackBar1
            // 
            this.trackBar1.Location = new System.Drawing.Point(24, 280);
            this.trackBar1.Maximum = 180;
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(432, 48);
            this.trackBar1.SmallChange = 10;
            this.trackBar1.TabIndex = 1;
            this.trackBar1.Scroll += new System.EventHandler(this.trackBar1_Scroll);
            // 
            // meter1
            // 
            this.meter1.Angle = 360;
            this.meter1.BackColor = System.Drawing.Color.LightSkyBlue;
            this.meter1.BorderColors.BorderColor1 = System.Drawing.Color.DarkGreen;
            this.meter1.BorderColors.BorderColor2 = System.Drawing.Color.Lavender;
            this.meter1.FaceColors.FaceColor1 = System.Drawing.Color.Ivory;
            this.meter1.FaceColors.FaceColor2 = System.Drawing.Color.PaleGreen;
            this.meter1.FaceColors.ZoneAngle = 45;
            this.meter1.ForeColor = System.Drawing.SystemColors.Highlight;
            this.meter1.Location = new System.Drawing.Point(24, 48);
            this.meter1.MeterlineColor = System.Drawing.Color.Fuchsia;
            this.meter1.Name = "meter1";
            this.meter1.NumberColor = System.Drawing.Color.Black;
            this.meter1.Size = new System.Drawing.Size(432, 224);
            this.meter1.TabIndex = 2;
            this.meter1.Text = "meter1";
            // 
            // Form1
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.BackColor = System.Drawing.Color.AliceBlue;
            this.ClientSize = new System.Drawing.Size(504, 342);
            this.Controls.Add(this.meter1);
            this.Controls.Add(this.trackBar1);
            this.Name = "Form1";
            this.Text = "Meter Control Test";
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}

		private void trackBar1_Scroll(object sender, System.EventArgs e) {
			meter1.Angle = trackBar1.Value;
		}


	
	}
}
