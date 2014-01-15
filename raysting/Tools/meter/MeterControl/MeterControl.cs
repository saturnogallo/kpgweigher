using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Windows.Forms.Design;

namespace MeterControl
{
		
	/// <summary>
	/// Eileen Rendon
	/// March 11, 2005
	/// This is my first windows control. The meter control is an example
	/// of an analog meter. It simply shows the current value as the value is changed.
	/// Designer Support will be added soon. 
	/// 
	/// 
	/// Revisions:
	/// 1) Made changes suggested by Mav.Northwind to fix flicker. Initialized angle to zero in constructor
	///    to draw line at resting place. Added Invalidate to the Angle Property instead of using an event
	///    to redraw the meter. 
	///    
	/// 2) Added regions to make the Meter control code easier to read, less cluttered
	/// 3) Added designer support to customized the look and feel of the control
	/// 
	/// </summary>
	    public class Meter : System.Windows.Forms.Control {
	
	
		#region Fonts, Brushes, Pens

		// font and brush used to draw the text for the scale on the 
		// face of the meter
		Font boldFont;
		SolidBrush numeralBrush;
		
		// brushes and pens needed to draw the face of the meter
		// later when the designer portion is added, the client will
		// be able to change the colors to the brushes
		LinearGradientBrush outlineBrush;
		LinearGradientBrush zoneBrush;
		Pen outlinePen;
		Pen meterlinePen;
        SolidBrush meterlineBrush;
		#endregion

		// angle is what is used to draw the meter line that displays the on the meter
		int angle;

		// offset is an offset that is added to adjust for the location of the text
		// on the meter face. 
		int offset;

		// x and y are the starting points
		int x , y;

		#region Face Colors
		
		// number color 

		Color numberColor;
        Color meterlineColor;	

		private MeterBorder borderColors;
		private MeterFace  faceColors;

		
		#endregion
	


		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Meter() {
		 	// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();
			
			// This is additional buffering to stop flickering when
			// the control is redrawn

			this.SetStyle(System.Windows.Forms.ControlStyles.ResizeRedraw, true);
			this.SetStyle(System.Windows.Forms.ControlStyles.DoubleBuffer, true);
			this.SetStyle(System.Windows.Forms.ControlStyles.AllPaintingInWmPaint, true);

			// initialization after the InitComponent call

			// colors
			numberColor = Color.Chartreuse;
			borderColors = new MeterBorder();
			faceColors = new MeterFace();
			meterlineColor = Color.Chartreuse;
			//Brushes and Pens
		
		
			boldFont = new Font("ArialBold", 12F, Font.Style | FontStyle.Bold);
			numeralBrush = new SolidBrush(numberColor);
			outlineBrush = new LinearGradientBrush(ClientRectangle, borderColors.BorderColor1, borderColors.BorderColor2, 45, true);
			zoneBrush = new LinearGradientBrush(ClientRectangle, faceColors.FaceColor1, faceColors.FaceColor2, faceColors.ZoneAngle, true);
			outlinePen = new Pen(outlineBrush, 32);
			meterlinePen = new Pen(meterlineColor, 3);
            meterlineBrush = new SolidBrush(Color.Black);
			x=0;
			y=0;
			offset = 0; 
			this.angle = 0;
		
			
			// make sure control is redrawn when it's resized
			SetStyle(System.Windows.Forms.ControlStyles.ResizeRedraw, true);
			this.borderColors.PropertyChanged +=new MeterBorder.PropertyChangedEventHandler(this.MeterBorderColors_PropertyChanged);
			this.faceColors.PropertyChanged +=new MeterFace.PropertyChangedEventHandler(this.MeterFaceColors_PropertyChanged);


		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing ) {
			if( disposing ) {
				if( components != null )
					components.Dispose();
			}
			base.Dispose( disposing );
			numeralBrush.Dispose();
			outlineBrush.Dispose();
			outlinePen.Dispose();
			meterlinePen.Dispose();

		}

		#region Component Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
			// 
			// Meter
			// 
			this.Name = "Meter";
			this.ClientSize = new System.Drawing.Size(448, 232);
			this.Location = new System.Drawing.Point(32, 16);
			
		
		}
		#endregion
	
		
		#region "Meter Control Drawing Methods "

        protected override void OnPaint(System.Windows.Forms.PaintEventArgs Pe)
        {

            Graphics g = Pe.Graphics;
            DrawPie(g);
            FillPie(g);

            this.offset = -30; //-30 is the offset for when the line is pointing to
            // zero or 180. It would be ideal to change the offset
            // gradually to -10 as it reaches 270. I'll leave this for 
            // the next version
            //DrawLine(g, angle, offset);
            string type = "0-100";
            int max = scale_stop;
            int step = scale_step;
            int stepcnt = (scale_stop - scale_start)/step;
            /*
            if (type == "0-360")
            {
                max = 360;
                step = 60;
            }
            if (type == "0-120")
            {
                max = 120;
                step = 20;
            }
            if (type == "0-100")
            {
                max = 100;
                step = 20;
            }
            */
            for (angle = scale_start; angle <= max; angle += step)
            {

                DrawScale(g, angle * 180 / max, angle.ToString(), 180);
            }
            for (angle = scale_start; angle <= max; angle += step)
            {
                DrawMarks(g, angle * 180 / max, step * 180 / (5 * max), stepcnt, 180);
            }
        }

        public int scale_start = 0;
        public int scale_step = 10;
        public int scale_stop = 100;
		private void DrawLine(Graphics g, int angle, int offset) {	
			Matrix m = new Matrix();
			PointF center = 
				new PointF(this.ClientRectangle.Width/2 ,this.ClientRectangle.Bottom );
			m.RotateAt( angle+180, center );
			m.Translate( center.X, center.Y );
			g.Transform = m;
			g.DrawLine( meterlinePen, 0, 0, this.ClientRectangle.Width/2 + offset , 0 );
		
		}


		private void DrawPie(Graphics g) {
			
			g.SmoothingMode = SmoothingMode.AntiAlias; 
			g.DrawPie(new Pen(outlineBrush, 30), x + this.ClientRectangle.Width/16 ,y + this.ClientRectangle.Height/16 , (this.ClientRectangle.Width -this.ClientRectangle.Width/8),((this.ClientRectangle.Height * 2)-this.ClientRectangle.Height/4), 0, -180);

			
		}


		private void FillPie(Graphics g) {
			g.FillPie(zoneBrush, x + this.ClientRectangle.Width/16 ,y + this.ClientRectangle.Height/16 , (this.ClientRectangle.Width -this.ClientRectangle.Width/8),((this.ClientRectangle.Height * 2)-this.ClientRectangle.Height/4), 0, -180);
		}
        private void DrawMarks(Graphics g, int start, int step , int stepcnt, int offset)
        {

            PointF center =
                new PointF(this.ClientRectangle.Width / 2, this.ClientRectangle.Bottom-15);
            float rad = (this.ClientRectangle.Width / 2 - 37);
            int pos = start;
            int yoffset = -1;
            int cnt = 0;
            while (cnt < stepcnt)
            {
                Matrix m = new Matrix();
                if (pos <= 180)
                {
                    m.RotateAt(pos + 180, center);
                    m.Translate(center.X, center.Y);
                    g.Transform = m;

                    if (pos == start)
                        g.FillRectangle(meterlineBrush, rad -  5*Math.Abs(pos-90)/90, -3-yoffset, 12, 3-yoffset); //big mark
                    else
                        g.FillRectangle(meterlineBrush, rad + 4 - 5*Math.Abs(pos-90)/90, -1 - yoffset, 8, 1 - yoffset); //small mark
                }
                pos = pos + step;
                cnt++;
            }
            

        }
		private void DrawScale(Graphics g,int angle, string mark, int offset) {
//			Matrix 	m = new Matrix();
			PointF center = 
				new PointF(this.ClientRectangle.Width/2 ,this.ClientRectangle.Bottom -15 );
            float rad = (this.ClientRectangle.Width/2 - 60);
            center.X = center.X - (float)(Math.Cos(angle * Math.PI / 180.0)*rad );
            center.Y = center.Y - (float)(Math.Sin(angle * Math.PI / 180.0)*rad );

            mark = ("   " + mark);
            mark = mark.Substring(mark.Length - 3, 3);
			StringFormat format = new StringFormat();
			format.Alignment= StringAlignment.Center;
			SizeF        
				textSize = 
				new SizeF(g.MeasureString(mark, boldFont));
			RectangleF   
				textBox = 
				new RectangleF(new PointF(center.X  - textSize.Width/2 - 5*angle/180, center.Y  -textSize.Height/2), 
				textSize);

//			m.RotateAt( angle, center );
//			g.Transform = m;
			g.SmoothingMode = SmoothingMode.AntiAlias;
			g.DrawString( mark, boldFont, numeralBrush,	textBox, format );
//            g.DrawString(mark, boldFont, numeralBrush, new PointF(center.X , center.Y ));
		}

			#endregion

		
		#region "Meter Properties"

		[Browsable(false)]
        // This angle is used to draw the meter needle that points to the current value

		public int Angle {
			get {
				return this.angle;
			}
			set {
				this.angle = value;
				Invalidate();


			}
		}


		[
		Description("This changes the color of the numbers drawn on the meter"),
		CategoryAttribute("Appearance"),
				
		]
		public Color NumberColor {
			get {
				return numberColor;
			}
			set {

				numberColor = value;
				numeralBrush.Color = numberColor;
				Invalidate();
			}
		}

		[
		Description("This changes the color of the needle on the meter"),
		CategoryAttribute("Appearance"),
				
		]
		public Color MeterlineColor {
			get {
				return meterlineColor;
			}
			set {

				meterlineColor = value;
				meterlinePen.Color = meterlineColor;
				Invalidate();
			}
		}
        
		[
		Description("BorderColor1 is the StartColor and BorderColor2 is the End Color for the Outline of the Meter"),
		DesignerSerializationVisibility(DesignerSerializationVisibility.Content),
		CategoryAttribute("Appearance")
				
		]
		public MeterBorder BorderColors {
			get {
				return this.borderColors;
			}
			set {
				this.borderColors = value;
			
			}
		}
		
		private void MeterBorderColors_PropertyChanged(string colorChanged) {
			UpdateBorderColors();
			Invalidate();	
		}

		private void UpdateBorderColors() {
			outlineBrush.Dispose();
			outlineBrush = new LinearGradientBrush(ClientRectangle, borderColors.BorderColor1, borderColors.BorderColor2, 45, true);
		
		}

		[
		Description("FaceColor1 is the StartColor and FaceColor2 is the End Color for the Face of the Meter. FaceColorAngle is the Angle at which the colors blend."),
		DesignerSerializationVisibility(DesignerSerializationVisibility.Content),
		CategoryAttribute("Appearance")
			
		]

		public MeterFace FaceColors {
			get {
				return this.faceColors;
			}
			set {
				this.faceColors = value;
			
			}
		}
		
		private void MeterFaceColors_PropertyChanged(string colorChanged) {
				UpdateFaceColors();
				Invalidate();	
			}

		private void UpdateFaceColors() {
				zoneBrush.Dispose();
				zoneBrush = new LinearGradientBrush(ClientRectangle, faceColors.FaceColor1, faceColors.FaceColor2, faceColors.ZoneAngle, true);
		
			}


		#endregion

		#region "Designer Attributes that are not browsable"

		[Browsable(false)]
		public override RightToLeft RightToLeft {
			get {
				return base.RightToLeft;
			}
			set {
				base.RightToLeft = value;
			}
		}
		
		[Browsable(false)]
		public override Image BackgroundImage {
			get {
				return base.BackgroundImage;
			}
			set {
				base.BackgroundImage = value;
			}
		}
		
		[Browsable(false)]
		public override Font Font {
			get {
				return base.Font;
			}
			set {
				base.Font = value;
			}
		}

		[Browsable(false)]
			public override Color ForeColor {
			get {
				return base.ForeColor;
			}
			set {
				base.ForeColor = value;
			}
		}

		[Browsable(false)]
			public override Cursor Cursor {
			get {
				return base.Cursor;
			}
			set {
				base.Cursor = value;
			}
		}

		[Browsable(false)]
			public override string Text {
				get {
					return base.Text;
				}
				set {
					base.Text = value;
				}
			}


		

		#endregion

		
		
	}
}
