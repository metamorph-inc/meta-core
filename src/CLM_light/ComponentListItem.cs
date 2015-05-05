// -----------------------------------------------------------------------
// <copyright file="ComponentListItem.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace CLM_light
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using GME.MGA;
    using System.Diagnostics.Contracts;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class ComponentListItem
    {
        internal MgaFCO Fco { get; set; }

        public string Name { get { return Fco.Name; } }

        public string ObjectType { get { return Fco.RootFCO.Meta.Name; } }

        public string PathWithinProject { get { return Fco.ExGetPath("/", false); } }

        public ComponentListItem(MgaFCO fco)
        {
            Contract.Requires(fco != null);

            Fco = fco;
        }

    }

    public static class Helper
    {
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
    }
}
