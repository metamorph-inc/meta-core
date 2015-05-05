using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AVM.DDP
{
    public class MetaResults
    {
        public List<Result> Results { get; set; }

        public class Result
        {
            public string Design { get; set; }
            public string DesignID { get; set; }
            public string TestBench { get; set; }
            public string Time { get; set; }
            public string Summary { get; set; }
        }

        public MetaResults()
        {
            Results = new List<Result>();
        }
    }


}
