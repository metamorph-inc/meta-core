using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ClmLib
{
    public class VFComponentExchange
    {
        public class Superceder
        {
            public int version { get; set; }
            public string avm_id { get; set; }
            public string name { get; set; }
        }

        public class Component
        {
            public string status { get; set; }
            public string project_name { get; set; }
            public string acm_url { get; set; }
            public string name { get; set; }
            public string url { get; set; }
            public int content_size { get; set; }
            public List<Superceder> superceders { get; set; }
            public string id { get; set; }
            public int version { get; set; }
            public string avm_id { get; set; }
            public string itar_restriction { get; set; }
            public string zip_url { get; set; }
            public bool withdrawn { get; set; }
            public string category_name { get; set; }
        }

        public class RootObject
        {
            public int hits { get; set; }
            public List<Component> components { get; set; }
        }
    }
}
