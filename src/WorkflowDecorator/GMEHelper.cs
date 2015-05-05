using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.CSharp;
using GME.MGA;
using GME.MGA.Meta;
using System.Runtime.InteropServices;

namespace GME.CSharp
{
	public static class GMEHelper
	{
		#region Custom Helper Functions

		public static IEnumerable<MgaObject> ExKindChildren<T>(
			this T subject,
			string Role)
			where T : IMgaObject
		{
			foreach (MgaObject child in subject.ChildObjects)
			{
				if (child.MetaBase.Name == Role)
				{
					yield return child;
				}
			}
		}

		public static IEnumerable<MgaFCO> ExSrcFcos<T>(this T mgafco)
			where T : IMgaFCO
		{
            List<MgaConnPoint> connParts = new List<MgaConnPoint>();

            connParts= mgafco.PartOfConns.Cast<MgaConnPoint>().ToList();

            foreach (IMgaConnPoint cp in connParts)
                {
                    if (cp.ConnRole == "dst")
                    {
                        MgaSimpleConnection simple = cp.Owner as MgaSimpleConnection;
                        yield return simple.Src as MgaFCO;
                    }
                }
		}

		public static IEnumerable<MgaFCO> ExDstFcos<T>(this T mgafco)
			where T : IMgaFCO
		{
            List<MgaConnPoint> connParts = new List<MgaConnPoint>();

            try
            {
                connParts = mgafco.PartOfConns.Cast<MgaConnPoint>().ToList();
            }
            catch (COMException)
            {
                // sometimes the mgafco is inaccessible
            }

            foreach (IMgaConnPoint cp in connParts)
			{
				if (cp.ConnRole == "src")
				{
					MgaSimpleConnection simple = cp.Owner as MgaSimpleConnection;
					yield return simple.Dst as MgaFCO;
				}
			}
		}


		public static MgaConnPoint ExSrcPoint<T>(this T mgaConnPoints) where T : IMgaConnPoints
		{
			foreach (MgaConnPoint connPoint in mgaConnPoints)
			{
				if (connPoint.ConnRole == "src")
				{
					return connPoint;
				}
			}
			return null;
		}

		public static MgaConnPoint ExDstPoint<T>(this T mgaConnPoints) where T : IMgaConnPoints
		{
			foreach (MgaConnPoint connPoint in mgaConnPoints)
			{
				if (connPoint.ConnRole == "dst")
				{
					return connPoint;
				}
			}
			return null;
		}

		public static MgaFCO ExSrc<T>(this T mgaConnection) where T : IMgaConnection
		{
			return mgaConnection.ConnPoints.ExSrcPoint().Target;
		}

		public static MgaFCO ExDst<T>(this T mgaConnection) where T : IMgaConnection
		{
			return mgaConnection.ConnPoints.ExDstPoint().Target;
		}

		public static MgaFCO ExTryGetReferred<T>(this T mgaFco) where T : IMgaFCO
		{
			if (mgaFco == null) { return null; }
			if ((mgaFco as MgaReference) == null) { return null; }
			return (mgaFco as MgaReference).Referred;
		}


		public static string ExAttrString<T>(
			this T subject,
			string AttributeName)
			where T : IMgaFCO
		{
			foreach (MgaAttribute attr in subject.Attributes)
			{
				if (attr.Meta.Name == AttributeName)
				{
					return attr.StringValue;
				}
			}
			return string.Empty;
		}

		/// <summary>
		/// Makes a GME hyperlink for the specified object.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="console"></param>
		/// <param name="subject"></param>
		/// <param name="text"></param>
		/// <returns></returns>
		public static string MakeObjectHyperlink<T>(
			this GMEConsole console,
			T subject,
			string text)
			where T : IMgaObject
		{
			StringBuilder sb = new StringBuilder();
			sb.Append("<a href=\"mga:");
			sb.Append(subject.ID);
			sb.Append("\">");
			sb.Append(text);
			sb.Append("</a>");
			return sb.ToString();
		}


		public static MgaObject ExGetParent<T>(this T subject)
	 where T : IMgaObject
		{
			MgaObject parent;
			objtype_enum type;
			subject.GetParent(out parent, out type);
			return parent;
		}

		public static string ExGetPath<T>(
			this T subject,
			string delimiter = "./.",
			bool hasRootFolderName = true)
			where T : IMgaObject
		{
			string absPath = subject.AbsPath;
			StringBuilder path = new StringBuilder();

			if (absPath == "")
			{
				return path.ToString();
			}
			while (absPath.Contains("/@"))
			{
				absPath = absPath.Substring(absPath.IndexOf("/@") + 2);
				path.Append(absPath.Substring(0, absPath.IndexOf("|")));
				path.Append(delimiter);
				absPath = absPath.Substring(absPath.IndexOf("|"));
			};
			StringBuilder result = new StringBuilder();
			result.Append(path.ToString().Substring(0, path.ToString().LastIndexOf(delimiter)));
			return subject.Project.RootFolder.Name + delimiter + result.ToString();
		}

		#endregion
	}
}
