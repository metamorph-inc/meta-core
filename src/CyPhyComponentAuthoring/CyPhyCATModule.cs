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
        private CyPhy.DesignElement CurrentDesignElement;
        public CyPhy.DesignElement GetCurrentDesignElement()
        {
            return CurrentDesignElement;
        }
        public void SetCurrentDesignElement(CyPhy.DesignElement comp)
        {
            CurrentDesignElement = comp;
        }
        //        CyPhy.Component CurrentDesignElement { get; set; }

        public MgaProject CurrentProj
        {
            get
            {
                return CurrentDesignElement.Impl.Project;
            }
        }

        public MgaFCO CurrentObj { get; set; }
    }
}
