using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using avm;
using Complexity;

namespace METADesignInterchange2ComplexityLib
{
    public static class METADesign2Complexity
    {
        private static List<avm.ComponentInstance> RecursivelyGetAllComponentInstances(Compound c_root)
        {
            List<avm.ComponentInstance> lci_rtn = new List<avm.ComponentInstance>();

            if (c_root.ComponentInstance != null)
            {
                lci_rtn.InsertRange(0, c_root.ComponentInstance);
            }

            if (c_root.Container1 != null)
            {
                foreach (Compound c in c_root.Container1.Where(c => c is Compound))
                {
                    lci_rtn.InsertRange(0, RecursivelyGetAllComponentInstances(c));
                }
            }

            return lci_rtn;
        }

        private static List<avm.ComponentInstance> RecursivelyGetAllComponentInstances(avm.Design dm_root)
        {
            List<avm.ComponentInstance> lci_rtn = new List<avm.ComponentInstance>();
            foreach (Compound c in dm_root.RootContainer.Container1.Where(c => c is Compound))
            {
                lci_rtn.InsertRange(0, RecursivelyGetAllComponentInstances(c));
            }
            return lci_rtn;
        }

        public static Complexity.Design Design2Complexity(avm.Design dmInput)
        {
            var dMain = new Complexity.Design();
            dMain.ComponentInstances = new List<Complexity.ComponentInstance>();
            dMain.Connections = new List<Complexity.Connection>();
            dMain.AVMID = dmInput.DesignID;
            dMain.Name = dmInput.Name;

            var componentInstances = RecursivelyGetAllComponentInstances(dmInput);
            var componentMapping = new Dictionary<avm.ComponentInstance, Complexity.ComponentInstance>();
            foreach (var ci in componentInstances)
            {
                var cci_new = new Complexity.ComponentInstance
                              {
                                  AVMID = ci.ComponentID,
                                  Name = ci.Name,
                                  Complexity = 1,
                                  DistributionType = DistributionTypeEnum.None
                              };

                ((List<Complexity.ComponentInstance>)dMain.ComponentInstances).Add(cci_new);

                if (!componentMapping.ContainsKey(ci))
                    componentMapping[ci] = cci_new;
            }

            foreach (var ci in componentInstances)
            {
                if (!componentMapping.ContainsKey(ci)) continue;

                foreach (var pi in ci.PortInstance)
                {
                    foreach (var portMap in pi.PortMap)
                    {
                        var parentComponent = componentInstances.Where(x => x.PortInstance.Any(y => y.ID == portMap)).FirstOrDefault();
                        
                        if (parentComponent == null) continue;
                        if (!componentMapping.ContainsKey(parentComponent)) continue;

                        Complexity.Connection cc_new = new Complexity.Connection();
                        ((List<Complexity.Connection>)dMain.Connections).Add(cc_new);
                        cc_new.src = componentMapping[ci];
                        cc_new.dst = componentMapping[parentComponent];
                        cc_new.Complexity = 0.1;
                        cc_new.DistributionType = Complexity.DistributionTypeEnum.None;
                    }
                }
            }
            
            return dMain;
        }
    }
}
