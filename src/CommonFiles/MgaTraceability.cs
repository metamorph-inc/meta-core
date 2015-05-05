// -----------------------------------------------------------------------
// <copyright file="MgaTraceability.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace META
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class MgaTraceability : CyPhyCOMInterfaces.IMgaTraceability
    {
        private Dictionary<string, string> map = new Dictionary<string, string>();

        #region IMgaTraceability Members

        public void AddItem(string copy, string original)
        {
            this.map[copy] = original;
        }

        public void CopyTo(CyPhyCOMInterfaces.IMgaTraceability copy)
        {
            foreach (var item in this.map)
            {
                string mapped;
                if (copy.TryGetMappedObject(item.Value, out mapped))
                {
                    copy.AddItem(item.Key, mapped);
                }
                else
                {
                    copy.AddItem(item.Key, item.Value);
                }
            }
        }

        public string GetMappedObject(string copy)
        {
            return map[copy];
        }

        public bool TryGetMappedObject(string copy, out string original)
        {
            bool result = false;
            original = string.Empty;

            result = this.map.TryGetValue(copy, out original);

            return result;
        }

        #endregion
    }
}
