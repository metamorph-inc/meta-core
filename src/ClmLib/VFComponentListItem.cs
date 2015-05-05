using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using System.Diagnostics.Contracts;
using ISIS.VehicleForge;

namespace ClmLib
{
    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class VFComponentListItem
    {
        internal VFComponent Component { get; set; }

        public string Name { get { return Component.name; } }

        public string ItarRestriction { get { return Component.itar_restriction; } }

        public string Status { get { return Component.status; } }

        //public string AVMID { get { return Component.avm_id; } }

        public string ZipURL { get { return Component.zip_url; } }

        public VFComponentListItem(VFComponent component)
        {
            Contract.Requires(component != null);

            Component = component;
        }

    }

}
