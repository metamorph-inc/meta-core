using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

namespace CyPhy2ComponentModel
{
    internal class LayoutDataWrapper
    {
        private object obj;
        public LayoutDataWrapper(object avmObj)
        {
            obj = avmObj;
        }

        private PropertyInfo GetProperty(String name)
        {
            return obj.GetType().GetProperty(name, BindingFlags.Instance | BindingFlags.Public);
        }

        private void SetProperty(String name, object value)
        {
            PropertyInfo propInfo = GetProperty(name);
            if (null != propInfo && propInfo.CanWrite)
            {
                propInfo.SetValue(obj, value, null);
            }
        }

        private Boolean PropertyExistsAndIsAndTrue(String name)
        {
            PropertyInfo propInfo = GetProperty(name);
            if (propInfo == null)
            {
                return false;
            }

            var propVal = propInfo.GetValue(obj, null);
            if (propVal == null)
            {
                return false;
            }

            return System.Convert.ToBoolean(propVal);
        }

        public Boolean hasLayoutData
        {
            get
            {
                return (PropertyExistsAndIsAndTrue("XPositionSpecified")
                        && PropertyExistsAndIsAndTrue("YPositionSpecified"));
            }
        }

        public UInt32 xpos
        {
            get
            {
                if (false == PropertyExistsAndIsAndTrue("XPositionSpecified"))
                {
                    return 0;
                }
                else
                {
                    return System.Convert.ToUInt32(GetProperty("XPosition").GetValue(obj, null));
                }
            }
            set
            {
                SetProperty("XPosition", value);
                SetProperty("XPositionSpecified", true);
            }
        }

        public UInt32 ypos
        {
            get
            {
                if (false == PropertyExistsAndIsAndTrue("YPositionSpecified"))
                {
                    return 0;
                }
                else
                {
                    return System.Convert.ToUInt32(GetProperty("YPosition").GetValue(obj, null));
                }
            }
            set
            {
                SetProperty("YPosition", value);
                SetProperty("YPositionSpecified", true);
            }
        }
    }
}
