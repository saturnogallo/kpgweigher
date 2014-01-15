using System;
using System.ComponentModel;

namespace MeterControl
{
	/// <summary>
	/// Summary description for MeterBorderTypeConverter. This will allow
	/// a + sign to be displayed for the two colors that are used in the
	/// border of the meter control
	/// </summary>
	public class MeterBorderTypeConverter: TypeConverter
	{
		public MeterBorderTypeConverter()
		{
			//
			// TODO: Add constructor logic here
			//
		}

		public override bool GetPropertiesSupported(ITypeDescriptorContext context) {
			return true;
		}

		public override PropertyDescriptorCollection GetProperties(ITypeDescriptorContext context, object value, Attribute[] attributes) {
			return TypeDescriptor.GetProperties(typeof(MeterBorder));
		}


	}
}
