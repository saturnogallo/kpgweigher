using System;
using System.Drawing;
using System.ComponentModel;

namespace MeterControl
{
	/// <summary>
	/// Summary description for MeterFace.
	/// </summary>
	[TypeConverter(typeof(MeterFaceTypeConverter))]
	public class MeterFace
	{

		System.Drawing.Color faceColor1;
		

		public System.Drawing.Color FaceColor1 {
			get {
				return this.faceColor1;
			}
			set {
				this.faceColor1 = value;
				PropertyChanged("faceColor1");
			}
		}

		System.Drawing.Color faceColor2;

		public System.Drawing.Color FaceColor2 {
			get {
				return this.faceColor2;
			}
			set {
				this.faceColor2 = value;
				PropertyChanged("faceColor2");
			}
		}

		int zoneAngle;

		public int ZoneAngle {
			get {
				return zoneAngle;
			}
			set {
				this.zoneAngle = value;
				PropertyChanged("zoneAngle");
			}
		}


		public MeterFace()
		{
			
		    this.faceColor1 = System.Drawing.Color.Blue;
			this.faceColor2 = System.Drawing.Color.Green;
			this.zoneAngle = 45;
		}

		public override string ToString() {
			return "Meter Face Colors";
		}

		#region Events
		public event PropertyChangedEventHandler PropertyChanged; 
		#endregion

		public delegate void PropertyChangedEventHandler( string propertyname);



	}
}
