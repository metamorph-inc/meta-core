using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ISIS.Web;

namespace ISIS.VehicleForge
{
    public class VFExchangeFilter : IComponentLibraryFilter
    {
        #region IComponentLibraryFilter Elements

        public string Category { get; set; }
        public int StartPosition { get; set; }
        public int NumberOfResults { get; set; }

        #endregion

        #region VF-Specific

        public Dictionary<string, ComponentLibraryFilterParameter> ParameterList { get; set; }

        #endregion
    }
}
