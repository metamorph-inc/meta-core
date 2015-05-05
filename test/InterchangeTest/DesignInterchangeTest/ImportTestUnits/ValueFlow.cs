using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Xunit;
using System.IO;
using GME.MGA;
using Xunit.Sdk;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;

namespace DesignImporterTests
{
    public class ValueFlowFixture : DesignImporterTestFixtureBase
    {

        public override string pathXME
        {
            get { return "ValueFlow\\ValueFlow.xme"; }
        }
    }

    public class ValueFlow : IUseFixture<ValueFlowFixture>
    {

        [Fact]
        public void CustomFormula()
        {
            var admFilename = "CustomFormula.adm";
            RunDesignImporter(admFilename,
                (design, ret) =>
                {
                    Assert.Equal(10, ret.AllChildren.Count());

                    var prop2 = ret.Children.PropertyCollection.Where(f => f.Name == "Prop2").First();
                    Assert.Equal("2", prop2.Attributes.Value);
                    var prop1 = ret.Children.PropertyCollection.Where(f => f.Name == "Prop1").First();
                    Assert.Equal("5", prop1.Attributes.Value);

                    Assert.Equal("A", prop2.DstConnections.ValueFlowCollection.FirstOrDefault().Attributes.FormulaVariableName);

                    var cf1 = ret.Children.CustomFormulaCollection.Where(f => f.Name == "CustomFormula1").First();
                    Assert.Equal("C", cf1.DstConnections.ValueFlowCollection.First().Attributes.FormulaVariableName);
                }
            );
        }


        [Fact]
        public void FlowBetweenComponentInstances()
        {
            var admFilename = "FlowBetweenComponentInstances.adm";
            RunDesignImporter(admFilename,
                (design, ret) =>
                {
                    Assert.Equal(4, ret.Children.ValueFlowCollection.Count());

                    foreach (var vf in ret.Children.ValueFlowCollection)
                    {
                        IMgaSimpleConnection conn = (IMgaSimpleConnection)vf.Impl;
                        Assert.NotNull(conn.Src);
                        Assert.Equal(1, conn.SrcReferences.Count);
                        Assert.NotNull(conn.Dst);
                        Assert.Equal(1, conn.DstReferences.Count);
                    }
                }
            );
        }

        [Fact]
        public void ConnectorComponentAssembly()
        {
            var admFilename = "ConnectorComponentAssembly.adm";
            RunDesignImporter(admFilename,
                (design, ret) =>
                {
                    Assert.Equal(3, ret.AllChildren.Count());

                    var SubComponentAssembly = ret.Children.DesignContainerCollection.Where(x => x.Name == "SubComponentAssembly").First();
                    Assert.Equal(3, SubComponentAssembly.AllChildren.Count());
                }
            );
        }

        [Fact]
        public void FlowToComponentInstance()
        {
            var admFilename = "FlowToComponentInstance.adm";
            RunDesignImporter(admFilename,
                (design, ret) =>
                {
                    Assert.Equal(25, ret.AllChildren.Count());

                    var OutCF = ret.Children.CustomFormulaCollection.Where(x => x.Name == "OutCF").First();
                    var OutCF_varnames = OutCF.SrcConnections.ValueFlowCollection.Select(x => x.Attributes.FormulaVariableName).OrderBy(x => x).ToList();
                    Assert.True(Enumerable.SequenceEqual(new string[] { "", "", "param_alias", "prop_alias" }, OutCF_varnames));
                }
            );
        }

        [Fact]
        public void FlowToSubAsm()
        {
            var admFilename = "FlowToSubAsm.adm";
            RunDesignImporter(admFilename,
                (design, ret) =>
                {
                    Assert.Equal(25, ret.AllChildren.Count());

                    var OutCF = ret.Children.CustomFormulaCollection.Where(x => x.Name == "OutCF").First();
                    var OutCF_varnames = OutCF.SrcConnections.ValueFlowCollection.Select(x => x.Attributes.FormulaVariableName).OrderBy(x => x).ToList();
                    Assert.True(Enumerable.SequenceEqual(new string[] { "", "", "param_alias", "prop_alias" }, OutCF_varnames));

                    //var SubAsm = ret.Children.ComponentAssemblyCollection.Where(x => x.Name == "SubAsm").First();
                    var SubAsm = ret.Children.DesignContainerCollection.Where(x => x.Name == "SubAsm").First();
                    Assert.Equal(10, SubAsm.AllChildren.Count());
                });
        }

        [Fact]
        public void Parameters()
        {
            var admFilename = "Parameters.adm";
            RunDesignImporter(admFilename,
                (design, ret) =>
                {
                    Assert.Equal(15, ret.AllChildren.Count());

                    var HasRangeReals = ret.Children.ParameterCollection.Where(x => x.Name == "HasRangeReals").First();
                    // Assert.Equal("-2.3..2.93", HasRangeReals.Attributes.Range);


                    var DerivedValue_HasRangeReals = ret.Children.ParameterCollection.Where(x => x.Name == "DerivedValue_HasRangeReals").First();
                    //Assert.Equal("-2.3..2.93", DerivedValue_HasRangeReals.Attributes.Range);
                });
        }

        [Fact]
        public void SimpleFormula()
        {
            var admFilename = "SimpleFormula.adm";
            RunDesignImporter(admFilename,
                (design, ret) =>
                {
                    Assert.True(0 < design.RootContainer.Property.Count);
                    //Xunit.Assert.Equal(1, ret.Children.PropertyCollection.Count());
                    var param = ret.Children.ParameterCollection.Where(f => f.Name == "Param").First();
                    Assert.Equal(1, ((MgaFCO)param.Impl).PartOfConns.Count);

                    var SimpleFormula2 = ret.Children.SimpleFormulaCollection.Where(x => x.Name == "SimpleFormula2").First();
                    Assert.Equal(2, SimpleFormula2.DstConnections.ValueFlowCollection.Count());
                    Assert.Equal(2, SimpleFormula2.SrcConnections.ValueFlowCollection.Count());
                }
            );
        }


        [Fact]
        public void SimpleFormula_OpTypes()
        {
            var admFilename = "SimpleFormula_OpTypes.adm";
            RunDesignImporter(admFilename,
                (design, ret) =>
                {
                    foreach (var formula in ret.Children.SimpleFormulaCollection)
                    {
                        Assert.NotEqual("", formula.Name);
                        Assert.Equal(formula.Name, formula.Attributes.Method.ToString());
                    }
                }
            );
        }
        [Fact]
        public void SubAsmFlowToComponentInstance()
        {
            var admFilename = "SubAsmFlowToComponentInstance.adm";
            RunDesignImporter(admFilename,
                (design, ret) =>
                {
                    Assert.Equal(8, ret.AllChildren.Count());

                    //var SubAsm = ret.Children.ComponentAssemblyCollection.Where(x => x.Name == "SubAsm").First();
                    var SubAsm = ret.Children.DesignContainerCollection.Where(x => x.Name == "SubAsm").First();
                    Assert.Equal(6, SubAsm.AllChildren.Count());
                }
            );
        }

        private void RunDesignImporter(string admFilename, Action<avm.Design, CyPhy.DesignContainer> test)
        {
            avm.Design design;
            using (StreamReader streamReader = new StreamReader(Path.Combine(fixture.AdmPath, admFilename)))
                design = CyPhyDesignImporter.CyPhyDesignImporterInterpreter.DeserializeAvmDesignXml(streamReader);

            proj.BeginTransactionInNewTerr();
            try
            {
                var importer = new CyPhyDesignImporter.AVMDesignImporter(null, proj);
                var ret = importer.ImportDesign(design, CyPhyDesignImporter.AVMDesignImporter.DesignImportMode.CREATE_DS);
                test(design, (CyPhy.DesignContainer)ret);
            }
            finally
            {
                proj.CommitTransaction();
                if (Debugger.IsAttached)
                {
                    proj.Save(proj.ProjectConnStr + admFilename + ".mga", true);
                }
            }
        }

        private IMgaProject proj { get { return fixture.proj; } }
        private ValueFlowFixture fixture;
        public void SetFixture(ValueFlowFixture data)
        {
            fixture = data;
        }
    }

}
