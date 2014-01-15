using System;
using System.ComponentModel;

namespace MeterControl
{
	/// <summary>
	/// Summary description for MeterFaceTypeConverter.
	/// </summary>
	public class MeterFaceTypeConverter: TypeConverter
	{
		public MeterFaceTypeConverter()
		{
			//
			// TODO: Add constructor logic here
			//
		}

		public override bool GetPropertiesSupported(ITypeDescriptorContext context) {
			return true;
		}

		public override PropertyDescriptorCollection GetProperties(ITypeDescriptorContext context, object value, Attribute[] attributes) {
			return TypeDescriptor.GetProperties(typeof(MeterFace));
		}

	}
}
