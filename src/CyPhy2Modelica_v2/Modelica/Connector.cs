using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2Modelica_v2.Modelica
{
    public class Connector : ModelBase<CyPhy.ModelicaConnector>
    {
        public Connector(CyPhy.ModelicaConnector impl)
            : base(impl)
        {
            Parameters = new SortedSet<UnitParameter>();
        }

        public SortedSet<UnitParameter> Parameters { get; set; }
        public string ModelicaURI { get; set; }
        public string Annotation
        {
            get
            {
                return string.Format("annotation(Placement(transformation(origin={{{0},-{1}}}, extent={{{{-20,-20}},{{20,20}}}})))",
                    CanvasX / CodeGenerator.ScaleFactor, CanvasY / CodeGenerator.ScaleFactor);
            }
        }
    }
}
