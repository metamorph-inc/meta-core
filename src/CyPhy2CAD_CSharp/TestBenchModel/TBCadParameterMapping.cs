using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2CAD_CSharp.TestBenchModel
{
    public class TBCadParameterMapping
    {
        public string TestBenchParameterName { get; set; }
        public string ComponentInstanceGUID { get; set; }
        public string ComponentCADParameterName { get; set; }

        public TBCadParameterMapping(CyPhy.CADParameter cadParam,
                                     string tbParamName)
        {
            ComponentCADParameterName = (cadParam.Attributes.ParameterName == "") ? cadParam.Name : cadParam.Attributes.ParameterName;

            if (cadParam.ParentContainer.ParentContainer.Kind == "Component")
                ComponentInstanceGUID = CyPhyClasses.Component.Cast(cadParam.ParentContainer.ParentContainer.Impl).Attributes.InstanceGUID;
            TestBenchParameterName = tbParamName;
        }
    }

    public class FEALoadCadParameterMapping
    {
        public string AttributeName { get; set; }
        public string TestBenchParameterName { get; set; }
        public string _id { get; set; }
    }
}
