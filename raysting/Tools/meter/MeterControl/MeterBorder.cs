using System;
using System.Drawing;
using System.ComponentModel;

namespace MeterControl {
	/// <summary>
	/// Summary description for MeterBorder.
	/// </summary>
	[TypeConverter(typeof(MeterBorderTypeConverter))]
	public class MeterBorder {
		
		Color borderColor1;
		

		public System.Drawing.Color BorderColor1 {
			get {
				return this.borderColor1;
			}
			set {
				this.borderColor1 = value;
				PropertyChanged("borderColor1");
			}
		}

		Color borderColor2;
		public System.Drawing.Color BorderColor2 {
			get {
				return this.borderColor2;
			}
			set {
				this.borderColor2 = value;
				PropertyChanged("borderColor2");
			}
		}

		
		public MeterBorder() {
			borderColor1 = Color.IndianRed;
			borderColor2 = Color.SlateGray;

		}

		public override string ToString() {
			return "Meter Border Colors";
		}

		#region Events
		public event PropertyChangedEventHandler PropertyChanged; 
		#endregion

		public delegate void PropertyChangedEventHandler( string propertyname);
	}
}



