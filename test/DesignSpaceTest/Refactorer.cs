using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using GME.MGA;
using GME.CSharp;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using System.IO;

namespace DesignSpaceTest
{
    public class RefactorerFixture : IDisposable
    {

        #region Paths
        public static string testPath = Path.Combine(META.VersionInfo.MetaPath,
                                                     "models",
                                                     "DesignSpace");
        public static string xmePath = Path.Combine(testPath,
                                                    "Refactorer.xme");
        public string mgaPath;
        #endregion

        public RefactorerFixture()
        {
            String mgaConnectionString;
            GME.MGA.MgaUtils.ImportXMEForTest(xmePath, out mgaConnectionString);
            mgaPath = mgaConnectionString.Substring("MGA=".Length);

            Assert.True(File.Exists(Path.GetFullPath(mgaPath)),
                        String.Format("{0} not found. Model import may have failed.", mgaPath));

            proj = new MgaProject();
            bool ro_mode;
            proj.Open("MGA=" + Path.GetFullPath(mgaPath), out ro_mode);
            proj.EnableAutoAddOns(true);
        }

        public void Dispose()
        {
            // Dispose stuff
            proj.Save();
            proj.Close();
        }

        public MgaProject proj { get; private set; }
    }

    public class RefactorerModelImport
    {
        [Fact]
        [Trait("ProjectImport/Open", "Refactorer")]
        public void ProjectXmeImport()
        {
            Assert.DoesNotThrow(() => {
                var fixture = new RefactorerFixture();
                fixture.Dispose();
            });
        }
    }

    public class Refactorer : IUseFixture<RefactorerFixture>
    {
        private MgaProject proj
        {
            get
            {
                return fixture.proj;
            }
        }

        #region Fixture
        RefactorerFixture fixture;
        public void SetFixture(RefactorerFixture data)
        {
            fixture = data;
        }
        #endregion

        [Fact]
        public void ImportXME()
        {

        }

        [Fact]
        public void OutgoingValueFlowWithVariableName()
        {
            CyPhy.DesignContainer rootDC = null;
            CyPhy.ComponentRef comp1 = null;
            List<MgaFCO> selected = new List<MgaFCO>();

            List<CyPhy.DesignContainer> childDCs = null;
            CyPhy.DesignContainer childDC = null;
            List<CyPhy.ComponentRef> childComponents = null;
            CyPhy.ComponentRef childComp1 = null;
            List<CyPhy.ValueFlow> childValueFlows = null;
            CyPhy.ValueFlow childValueFlow = null;

            proj.PerformInTransaction(delegate
            {
                // Assuming test model has 1 and only 1 DesignContainer called "ValueFlowVariableName"
                rootDC = proj.GetDesignContainersByName("ValueFlowVariableName").First();
                comp1 = rootDC.Children
                                  .ComponentRefCollection
                                  .Where(x => x.Name == "C1")
                                  .First();
                selected.Add(comp1.Impl as MgaFCO);
            });

            Utils.RunDSRefactorer(rootDC.Impl as MgaFCO, selected, "DesignContainer");

            proj.PerformInTransaction(delegate
            {
                // Assuming test model has 1 and only 1 DesignContainer called "ValueFlowVariableName"
                rootDC = proj.GetDesignContainersByName("ValueFlowVariableName").First();

                // Make sure only 1 child DesignContainer of name 'NewDC_C1' is found
                childDCs = rootDC.Children
                                    .DesignContainerCollection
                                    .Where(x => x.Name == "NewDC__C1").ToList();
                String countStr = "Count = " + childDCs.Count();
                Assert.True(childDCs.Count() == 1, "Expected to find exactly 1 DesignContainer named 'NewDC__C1', " + countStr);

                // Make sure only 1 child ComponentRef of name 'C1' is found
                childDC = childDCs.First();
                childComponents = childDC.Children
                                  .ComponentRefCollection
                                  .Where(x => x.Name == "C1").ToList();
                Assert.True(childComponents.Count() == 1, "Expected to find exactly 1 ComponentRefs named 'C1' in DesignContainer 'NewDC__C1'");

                // Make sure only this ComponentRef and original ComponentRef point to the same component
                childComp1 = childComponents.First();
                Assert.True("C1" == childComp1.Referred.Component.Name, "Child ComponentRef and original ComponentRef points to different components!");

                // Make sure that the ValueFlow has the alias 'C1P1'
                childValueFlows = rootDC.Children
                                            .ValueFlowCollection
                                            .Where(x => x.SrcEnd.Name == "CustomFormula_C1" && x.DstEnd.Kind == "CustomFormula").ToList();
                Assert.True(childValueFlows.Count() == 1, "No ValueFlow found from 'CustomFormula_C1' property to the CustomFormula'");
                childValueFlow = childValueFlows.First();
                string aliasName = childValueFlow.Attributes.FormulaVariableName;
                Assert.True("C1P1" == aliasName, "Alias found on ValueFlow from 'CustomFormula_C1' property to CustomFormula is not 'C1P1'");
            });
        }

        [Fact]
        public void CAWithConnector()
        {
            MgaFCO currentobj = null;
            proj.PerformInTransaction(delegate
            {
                currentobj = (MgaFCO)proj.ObjectByPath["/@ComponentAssemblies/@Component Assembly"];
            });

            Utils.RunDSRefactorer(currentobj, new MgaFCO[]{}.ToList(), "DesignContainer");

            proj.PerformInTransaction(delegate
            {
                var path = "/@NewDS__Component Assembly/@NewDC__Component Assembly";
                var rootDC = (MgaModel)proj.ObjectByPath[path];
                Assert.True(rootDC != null, String.Format("Could not find FCO with path '{0}'", path));
                var fcos = rootDC.GetDescendantFCOs(proj.CreateFilter()).Cast<MgaFCO>().ToList();
                Assert.Equal(6, fcos.Count);
                foreach (MgaFCO fco in fcos)
                {
                    if (fco is MgaConnection)
                    {
                        MgaSimpleConnection conn = (MgaSimpleConnection)fco;
                        Assert.NotNull(conn.Src);
                        Assert.NotNull(conn.Dst);
                    }
                }
            });
        }
    }
}
