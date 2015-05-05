using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using GME.MGA;


namespace CyPhyComponentAuthoring
{
    [CyPhyComponentAuthoringInterpreter.IsCATModule(ContainsCATmethod = true)]
    public abstract class CATModule
    {
        private CyPhy.Component CurrentComp;
        public CyPhy.Component GetCurrentComp()
        {
            return CurrentComp;
        }
        public void SetCurrentComp(CyPhy.Component comp)
        {
            CurrentComp = comp;
        }
        //        CyPhy.Component CurrentComp { get; set; }

        public MgaProject CurrentProj { get; set; }
        public MgaFCO CurrentObj { get; set; }
    }
}
