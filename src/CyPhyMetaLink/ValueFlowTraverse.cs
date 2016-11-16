using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using CyPhyMLClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;

namespace CyPhyMetaLink
{
    class ValueFlowTraverse
    {
        public List<CyPhyML.CADParameter> Found;
        private List<string> Traversed;

        public ValueFlowTraverse()
        {
            Found = new List<CyPhyML.CADParameter>();
            Traversed = new List<string>();
        }

        public void TraverseValueFlow(CyPhyML.ValueFlowTarget vft)
        {
            if (!Traversed.Contains(vft.ID))
            {
                Traversed.Add(vft.ID);
                foreach (var conn in vft.SrcConnections.ValueFlowCollection)
                {
                    TraverseValueFlow(conn.SrcEnds.ValueFlowTarget);
                }

                foreach (var conn in vft.DstConnections.ValueFlowCollection)
                {
                    TraverseValueFlow(conn.DstEnds.ValueFlowTarget);
                }

                foreach (var conn in vft.DstConnections.CADParameterPortMapCollection)
                {
                    Found.Add(conn.DstEnds.CADParameter);
                }
            }
        }
    }   // end class
}   // end namespace
