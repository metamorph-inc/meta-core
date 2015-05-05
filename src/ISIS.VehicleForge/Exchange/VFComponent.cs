using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ISIS.Web;


namespace ISIS.VehicleForge
{
    public class VFComponent : IComponent
    {
        #region IComponent implementation
        public string name { get; set; }
        public string id { get; set; }
        public string category_name { get; set; }
        #endregion

        #region VF-specific
        public string status { get; set; }
        public string project_name { get; set; }
        public string acm_url { get; set; }
        public string url { get; set; }
        public int content_size { get; set; }
        public List<VFComponentSuperceder> superceders { get; set; }
        public int version { get; set; }
        public string avm_id { get; set; }
        public string itar_restriction { get; set; }
        public string zip_url { get; set; }
        public bool withdrawn { get; set; }
        #endregion
    }
}
