using GME.MGA;
using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;

namespace CyPhyCOMInterfaces
{
    public static class TraceabilityExtensions
    {
        public static IMgaObject getTracedObjectOrSelf(this IMgaObject obj, CyPhyCOMInterfaces.IMgaTraceability traceability)
        {
            string originalID;
            if (traceability != null && traceability.TryGetMappedObject(obj.ID, out originalID))
            {
                return obj.Project.GetObjectByID(originalID);
            }
            else
            {
                return obj;
            }
        }

        public static string ToMgaHyperLink<T>(this T subject, CyPhyCOMInterfaces.IMgaTraceability elaboratedObjects = null, bool useName = false)
            where T : GME.MGA.IMgaObject
        {
            StringBuilder sb = new StringBuilder();

            if (elaboratedObjects != null)
            {
                string id = string.Empty;
                if (elaboratedObjects.TryGetMappedObject(subject.ID, out id))
                {
                    GME.MGA.MgaFCO fco = subject.Project.GetFCOByID(id);
                    if (fco != null)
                    {
                        // TODO: maybe get it until we reach the original source???
                        sb.AppendFormat("<a href=\"mga:{0}\">{1}</a>", fco.ID, SecurityElement.Escape(fco.Name));
                        return sb.ToString();
                    }
                }
            }

            if (useName)
            {
                sb.AppendFormat("<a href=\"mga:{0}\">{1}</a>", subject.ID, subject.Name);
            }
            else
            {
                sb.AppendFormat("<a href=\"mga:{0}\">{1}</a>", subject.ID, getPath(subject));
            }
            return sb.ToString();
        }

        public static string getPath(IMgaObject o)
        {
            MgaObject parent;
            GME.MGA.Meta.objtype_enum type;
            o.GetParent(out parent, out type);
            Stack<IMgaObject> stack = new Stack<IMgaObject>();
            StringBuilder sb = new StringBuilder();

            stack.Push(o);

            while (parent != null)
            {
                stack.Push(parent);
                parent.GetParent(out parent, out type);
            }

            foreach (var item in stack)
            {
                if (item != stack.Peek())
                {
                    sb.Append("/");
                }
                sb.Append(item.Name);
            }

            return sb.ToString();
        }

    }

}
