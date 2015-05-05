using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ISIS.Web;

namespace ISIS.VehicleForge
{
    public class VFExchangeSearchResult : IComponentLibrarySearchResult<VFComponent>
    {
        public int hits { get; set; }
        public List<VFComponent> components { get; set; }
    }
}
