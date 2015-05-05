using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2Modelica_v2.Modelica
{
    public static class Extentions
    {
        public static CyPhy.Component DerivedFrom(this CyPhy.Component component)
        {
            if ((component.Impl as GME.MGA.IMgaFCO).DerivedFrom != null)
            {
                return CyPhyClasses.Component.Cast((component.Impl as GME.MGA.IMgaFCO).DerivedFrom);
            }
            else
            {
                return null;
            }
        }

        public static CyPhy.TestComponent DerivedFrom(this CyPhy.TestComponent component)
        {
            if ((component.Impl as GME.MGA.IMgaFCO).DerivedFrom != null)
            {
                return CyPhyClasses.TestComponent.Cast((component.Impl as GME.MGA.IMgaFCO).DerivedFrom);
            }
            else
            {
                return null;
            }
        }
    }
}
