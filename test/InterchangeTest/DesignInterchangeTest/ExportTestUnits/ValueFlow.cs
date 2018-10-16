using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using GME.CSharp;
using GME.MGA;
using System.IO;
using System.Threading.Tasks;
using System.Collections.Concurrent;
using CyPhyGUIs;

namespace DesignExporterUnitTests
{
    public class ValueFlowFixture : IDisposable
    {
        public static String PathTest = Path.Combine(META.VersionInfo.MetaPath,
                                                     "test",
                                                     "InterchangeTest",
                                                     "DesignInterchangeTest",
                                                     "ExportTestModels",
                                                     "ValueFlow");

        public String pathXME = Path.Combine(PathTest, "ValueFlow.xme");

        public ValueFlowFixture()
        {
            String mgaConnectionString;
            GME.MGA.MgaUtils.ImportXMEForTest(pathXME, out mgaConnectionString);

            proj = new MgaProject();
            bool ro_mode;
            proj.Open(mgaConnectionString, out ro_mode);
            proj.EnableAutoAddOns(true);
        }

        public void Dispose()
        {
            proj.Save();
            proj.Close();
        }

        public MgaProject proj { get; private set; }
    }
    
    public class ValueFlow : IUseFixture<ValueFlowFixture>
    {
        #region Fixture
        ValueFlowFixture fixture;
        public void SetFixture(ValueFlowFixture data)
        {
            fixture = data;
        }
        #endregion

        private MgaProject proj { get { return fixture.proj; } }

        [Fact]
        public void CA_SimpleFormula()
        {
            String pathCA = "ComponentAssemblies/SimpleFormula";

            avm.Design design = Convert(pathCA);
            
            // Get objects
            var rc = design.RootContainer;
            Assert.NotNull(rc);
            var sf1 = rc.Formula.OfType<avm.SimpleFormula>().FirstOrDefault(f => f.Name == "SimpleFormula1");
            Assert.NotNull(sf1);
            var sf2 = rc.Formula.OfType<avm.SimpleFormula>().FirstOrDefault(f => f.Name == "SimpleFormula2");
            Assert.NotNull(sf2);
            var prop1 = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name == "Prop1");
            Assert.NotNull(prop1);
            var prop2 = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name == "Prop2");
            Assert.NotNull(prop2);
            var prop3 = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name == "Prop3");
            Assert.NotNull(prop3);
            var param = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name == "Param");
            Assert.NotNull(param);

            // Check Value Flows
            Assert.Equal(2, sf1.Operand.Count);
            Assert.True(sf1.Operand.Contains(prop1.Value.ID));
            Assert.True(sf1.Operand.Contains(prop2.Value.ID));

            Assert.Equal(2, sf2.Operand.Count);
            Assert.True(sf2.Operand.Contains(prop2.Value.ID));
            Assert.True(sf2.Operand.Contains(sf1.ID));

            var dv = prop3.Value.ValueExpression as avm.DerivedValue;
            Assert.NotNull(dv);
            Assert.Equal(sf2.ID, dv.ValueSource);

            Assert.NotNull(param.Value.ValueExpression);
            Assert.IsType<avm.ParametricValue>(param.Value.ValueExpression);
            var pv_param = param.Value.ValueExpression as avm.ParametricValue;
            Assert.NotNull(pv_param.AssignedValue);
            Assert.IsType<avm.DerivedValue>(pv_param.AssignedValue);
            Assert.Equal(sf2.ID, (pv_param.AssignedValue as avm.DerivedValue).ValueSource);
        }

        [Fact]
        public void CA_CustomFormula()
        {
            String pathCA = "ComponentAssemblies/CustomFormula";

            avm.Design design = Convert(pathCA);

            // Get objects
            var rc = design.RootContainer;
            Assert.NotNull(rc);
            var cf1 = rc.Formula.OfType<avm.ComplexFormula>().FirstOrDefault(f => f.Name == "CustomFormula1");
            Assert.NotNull(cf1);
            var cf2 = rc.Formula.OfType<avm.ComplexFormula>().FirstOrDefault(f => f.Name == "CustomFormula2");
            Assert.NotNull(cf2);
            var prop1 = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name == "Prop1");
            Assert.NotNull(prop1);
            var prop2 = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name == "Prop2");
            Assert.NotNull(prop2);
            var prop3 = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name == "Prop3");
            Assert.NotNull(prop3);
            
            // Check Value Flows
            Assert.Equal(2, cf1.Operand.Count);

            var op1_cf1 = cf1.Operand.FirstOrDefault(op => op.ValueSource == prop1.Value.ID);
            Assert.NotNull(op1_cf1);
            Assert.Equal(prop1.Name, op1_cf1.Symbol);

            var op2_cf1 = cf1.Operand.FirstOrDefault(op => op.ValueSource == prop2.Value.ID);
            Assert.NotNull(op2_cf1);
            Assert.Equal("A", op2_cf1.Symbol);

            var op1_cf2 = cf2.Operand.FirstOrDefault(op => op.ValueSource == prop1.Value.ID);
            Assert.NotNull(op1_cf2);
            Assert.Equal(prop1.Name, op1_cf2.Symbol);

            var op2_cf2 = cf2.Operand.FirstOrDefault(op => op.ValueSource == cf1.ID);
            Assert.NotNull(op2_cf2);
            Assert.Equal("C", op2_cf2.Symbol);

            var dv = prop3.Value.ValueExpression as avm.DerivedValue;
            Assert.NotNull(dv);
            Assert.Equal(cf2.ID, dv.ValueSource);
        }

        [Fact]
        public void CA_FlowToSubAsm()
        {
            String pathCA = "ComponentAssemblies/FlowToSubAsm";

            avm.Design design = Convert(pathCA);

            #region Get Objects
            var rc = design.RootContainer;
            Assert.NotNull(rc);
            var InSF = rc.Formula.OfType<avm.SimpleFormula>().FirstOrDefault(f => f.Name.Equals("InSF"));
            Assert.NotNull(InSF);
            var InCF = rc.Formula.OfType<avm.ComplexFormula>().FirstOrDefault(f => f.Name.Equals("InCF"));
            Assert.NotNull(InCF);
            var InProp = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("InProp"));
            Assert.NotNull(InProp);
            var InParam = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("InParam"));
            Assert.NotNull(InParam);
            
            var OutSF = rc.Formula.OfType<avm.SimpleFormula>().FirstOrDefault(f => f.Name.Equals("OutSF"));
            Assert.NotNull(OutSF);
            var OutCF = rc.Formula.OfType<avm.ComplexFormula>().FirstOrDefault(f => f.Name.Equals("OutCF"));
            Assert.NotNull(OutCF);
            var OutProp = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("OutProp"));
            Assert.NotNull(OutProp);
            var OutParam = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("OutParam"));
            Assert.NotNull(OutParam);

            var subAsm = rc.Container1.OfType<avm.Compound>().FirstOrDefault(c => c.Name.Equals("SubAsm"));
            Assert.NotNull(subAsm);
            var in_prop_sf = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_prop_sf"));
            Assert.NotNull(in_prop_sf);
            var in_param_sf = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_param_sf"));
            Assert.NotNull(in_param_sf);
            var in_prop_cf = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_prop_cf"));
            Assert.NotNull(in_prop_cf);
            var in_param_cf = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_param_cf"));
            Assert.NotNull(in_param_cf);
            var in_prop_prop = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_prop_prop"));
            Assert.NotNull(in_prop_prop);
            var in_param_prop = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_param_prop"));
            Assert.NotNull(in_param_prop);
            var in_prop_param = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_prop_param"));
            Assert.NotNull(in_prop_param);
            var in_param_param = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_param_param"));
            Assert.NotNull(in_param_param);
            var out_prop = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("out_prop"));
            Assert.NotNull(out_prop);
            var out_param = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("out_param"));
            Assert.NotNull(out_param);
            #endregion

            Assert.Equal(4, OutCF.Operand.Count);
            Assert.Equal(1, OutCF.Operand.Count(op => op.Symbol.Equals("param_alias") && op.ValueSource.Equals(out_param.Value.ID)));
            Assert.Equal(1, OutCF.Operand.Count(op => op.Symbol.Equals("prop_alias") && op.ValueSource.Equals(out_prop.Value.ID)));
            Assert.Equal(1, OutCF.Operand.Count(op => op.Symbol.Equals("out_param") && op.ValueSource.Equals(out_param.Value.ID)));
            Assert.Equal(1, OutCF.Operand.Count(op => op.Symbol.Equals("out_prop") && op.ValueSource.Equals(out_prop.Value.ID)));

            Assert.Equal(2, OutSF.Operand.Count);
            Assert.Equal(1, OutSF.Operand.Count(op => op.Equals(out_prop.Value.ID)));
            Assert.Equal(1, OutSF.Operand.Count(op => op.Equals(out_param.Value.ID)));

            var dv_outProp = OutProp.Value.ValueExpression as avm.DerivedValue;
            Assert.NotNull(dv_outProp);
            Assert.Equal(out_prop.Value.ID, dv_outProp.ValueSource);

            var pv_outParam = OutParam.Value.ValueExpression as avm.ParametricValue;
            Assert.NotNull(pv_outParam);
            var dv_outParam = pv_outParam.AssignedValue as avm.DerivedValue;
            Assert.NotNull(dv_outParam);
            Assert.Equal(out_param.Value.ID, dv_outParam.ValueSource);
        }

        [Fact]
        public void CA_FlowToComponentInstance()
        {
            String pathCA = "ComponentAssemblies/FlowToComponentInstance";

            avm.Design design = Convert(pathCA);

            #region Get Objects
            var rc = design.RootContainer;
            Assert.NotNull(rc);
            var InSF = rc.Formula.OfType<avm.SimpleFormula>().FirstOrDefault(f => f.Name.Equals("InSF"));
            Assert.NotNull(InSF);
            var InCF = rc.Formula.OfType<avm.ComplexFormula>().FirstOrDefault(f => f.Name.Equals("InCF"));
            Assert.NotNull(InCF);
            var InProp = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("InProp"));
            Assert.NotNull(InProp);
            var InParam = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("InParam"));
            Assert.NotNull(InParam);

            var OutSF = rc.Formula.OfType<avm.SimpleFormula>().FirstOrDefault(f => f.Name.Equals("OutSF"));
            Assert.NotNull(OutSF);
            var OutCF = rc.Formula.OfType<avm.ComplexFormula>().FirstOrDefault(f => f.Name.Equals("OutCF"));
            Assert.NotNull(OutCF);
            var OutProp = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("OutProp"));
            Assert.NotNull(OutProp);
            var OutParam = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("OutParam"));
            Assert.NotNull(OutParam);

            Assert.Equal(1, rc.ComponentInstance.Count);
            var compInst = rc.ComponentInstance.First();

            Assert.NotNull(compInst);
            var in_prop_sf = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("in_prop_sf"));
            Assert.NotNull(in_prop_sf);
            var in_param_sf = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("in_param_sf"));
            Assert.NotNull(in_param_sf);
            var in_prop_cf = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("in_prop_cf"));
            Assert.NotNull(in_prop_cf);
            var in_param_cf = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("in_param_cf"));
            Assert.NotNull(in_param_cf);
            var in_prop_prop = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("in_prop_prop"));
            Assert.NotNull(in_prop_prop);
            var in_param_prop = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("in_param_prop"));
            Assert.NotNull(in_param_prop);
            var in_prop_param = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("in_prop_param"));
            Assert.NotNull(in_prop_param);
            var in_param_param = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("in_param_param"));
            Assert.NotNull(in_param_param);
            var out_prop = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("out_prop"));
            Assert.NotNull(out_prop);
            var out_param = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("out_param"));
            Assert.NotNull(out_param);
            #endregion

            Assert.Equal(4, OutCF.Operand.Count);
            Assert.Equal(1, OutCF.Operand.Count(op => op.Symbol.Equals("param_alias") && op.ValueSource.Equals(out_param.Value.ID)));
            Assert.Equal(1, OutCF.Operand.Count(op => op.Symbol.Equals("prop_alias") && op.ValueSource.Equals(out_prop.Value.ID)));
            Assert.Equal(1, OutCF.Operand.Count(op => op.Symbol.Equals("out_param") && op.ValueSource.Equals(out_param.Value.ID)));
            Assert.Equal(1, OutCF.Operand.Count(op => op.Symbol.Equals("out_prop") && op.ValueSource.Equals(out_prop.Value.ID)));

            Assert.Equal(2, OutSF.Operand.Count);
            Assert.Equal(1, OutSF.Operand.Count(op => op.Equals(out_prop.Value.ID)));
            Assert.Equal(1, OutSF.Operand.Count(op => op.Equals(out_param.Value.ID)));

            var dv_outProp = OutProp.Value.ValueExpression as avm.DerivedValue;
            Assert.NotNull(dv_outProp);
            Assert.Equal(out_prop.Value.ID, dv_outProp.ValueSource);

            var pv_outParam = OutParam.Value.ValueExpression as avm.ParametricValue;
            Assert.NotNull(pv_outParam);
            var dv_outParam = pv_outParam.AssignedValue as avm.DerivedValue;
            Assert.NotNull(dv_outParam);
            Assert.Equal(out_param.Value.ID, dv_outParam.ValueSource);

            VerifyComponentPropertySource(InSF.ID, in_prop_sf);
            VerifyComponentPropertySource(InSF.ID, in_param_sf);
            VerifyComponentPropertySource(InCF.ID, in_prop_cf);
            VerifyComponentPropertySource(InCF.ID, in_param_cf);
            VerifyComponentPropertySource(InProp.Value.ID, in_param_prop);
            VerifyComponentPropertySource(InProp.Value.ID, in_prop_prop);
            VerifyComponentPropertySource(InParam.Value.ID, in_param_param);
            VerifyComponentPropertySource(InParam.Value.ID, in_prop_param);
        }

        private static void VerifyComponentPropertySource(String IDValueSource, avm.ComponentPrimitivePropertyInstance ComponentProperty)
        {
            Assert.NotNull(ComponentProperty.Value);
            Assert.NotNull(ComponentProperty.Value.ValueExpression);
            Assert.IsType<avm.DerivedValue>(ComponentProperty.Value.ValueExpression);
            Assert.Equal(IDValueSource, (ComponentProperty.Value.ValueExpression as avm.DerivedValue).ValueSource);
        }

        [Fact]
        public void DS_SimpleFormula()
        {
            String pathDS = "DesignSpaces/DC_SimpleFormula";

            avm.Design design = Convert(pathDS);

            // Get objects
            var rc = design.RootContainer;
            Assert.NotNull(rc);
            var sf1 = rc.Formula.OfType<avm.SimpleFormula>().FirstOrDefault(f => f.Name == "SimpleFormula1");
            Assert.NotNull(sf1);
            var sf2 = rc.Formula.OfType<avm.SimpleFormula>().FirstOrDefault(f => f.Name == "SimpleFormula2");
            Assert.NotNull(sf2);
            var prop1 = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name == "Prop1");
            Assert.NotNull(prop1);
            var prop2 = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name == "Prop2");
            Assert.NotNull(prop2);
            var prop3 = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name == "Prop3");
            Assert.NotNull(prop3);

            // Check Value Flows
            Assert.Equal(2, sf1.Operand.Count);
            Assert.True(sf1.Operand.Contains(prop1.Value.ID));
            Assert.True(sf1.Operand.Contains(prop2.Value.ID));

            Assert.Equal(2, sf2.Operand.Count);
            Assert.True(sf2.Operand.Contains(prop2.Value.ID));
            Assert.True(sf2.Operand.Contains(sf1.ID));

            var dv = prop3.Value.ValueExpression as avm.DerivedValue;
            Assert.NotNull(dv);
            Assert.Equal(sf2.ID, dv.ValueSource);
        }

        [Fact]
        public void DS_CustomFormula()
        {
            String pathDS = "DesignSpaces/DC_CustomFormula";

            avm.Design design = Convert(pathDS);

            // Get objects
            var rc = design.RootContainer;
            Assert.NotNull(rc);
            var cf1 = rc.Formula.OfType<avm.ComplexFormula>().FirstOrDefault(f => f.Name == "CustomFormula1");
            Assert.NotNull(cf1);
            var cf2 = rc.Formula.OfType<avm.ComplexFormula>().FirstOrDefault(f => f.Name == "CustomFormula2");
            Assert.NotNull(cf2);
            var prop1 = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name == "Prop1");
            Assert.NotNull(prop1);
            var prop2 = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name == "Prop2");
            Assert.NotNull(prop2);
            var prop3 = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name == "Prop3");
            Assert.NotNull(prop3);

            // Check Value Flows
            Assert.Equal(2, cf1.Operand.Count);

            var op1_cf1 = cf1.Operand.FirstOrDefault(op => op.ValueSource == prop1.Value.ID);
            Assert.NotNull(op1_cf1);
            Assert.Equal(prop1.Name, op1_cf1.Symbol);

            var op2_cf1 = cf1.Operand.FirstOrDefault(op => op.ValueSource == prop2.Value.ID);
            Assert.NotNull(op2_cf1);
            Assert.Equal("A", op2_cf1.Symbol);

            var op1_cf2 = cf2.Operand.FirstOrDefault(op => op.ValueSource == prop1.Value.ID);
            Assert.NotNull(op1_cf2);
            Assert.Equal(prop1.Name, op1_cf2.Symbol);

            var op2_cf2 = cf2.Operand.FirstOrDefault(op => op.ValueSource == cf1.ID);
            Assert.NotNull(op2_cf2);
            Assert.Equal("C", op2_cf2.Symbol);

            var dv = prop3.Value.ValueExpression as avm.DerivedValue;
            Assert.NotNull(dv);
            Assert.Equal(cf2.ID, dv.ValueSource);
        }

        [Fact]
        public void DS_FlowToSubAsm()
        {
            String pathDS = "DesignSpaces/DC_FlowToDesignContainer";

            avm.Design design = Convert(pathDS);

            #region Get Objects
            var rc = design.RootContainer;
            Assert.NotNull(rc);
            var InSF = rc.Formula.OfType<avm.SimpleFormula>().FirstOrDefault(f => f.Name.Equals("InSF"));
            Assert.NotNull(InSF);
            var InCF = rc.Formula.OfType<avm.ComplexFormula>().FirstOrDefault(f => f.Name.Equals("InCF"));
            Assert.NotNull(InCF);
            var InProp = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("InProp"));
            Assert.NotNull(InProp);
            var InParam = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("InParam"));
            Assert.NotNull(InParam);

            var OutSF = rc.Formula.OfType<avm.SimpleFormula>().FirstOrDefault(f => f.Name.Equals("OutSF"));
            Assert.NotNull(OutSF);
            var OutCF = rc.Formula.OfType<avm.ComplexFormula>().FirstOrDefault(f => f.Name.Equals("OutCF"));
            Assert.NotNull(OutCF);
            var OutProp = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("OutProp"));
            Assert.NotNull(OutProp);
            var OutParam = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("OutParam"));
            Assert.NotNull(OutParam);

            var subAsm = rc.Container1.OfType<avm.Compound>().FirstOrDefault(c => c.Name.Equals("SubDC"));
            Assert.NotNull(subAsm);
            var in_prop_sf = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_prop_sf"));
            Assert.NotNull(in_prop_sf);
            var in_param_sf = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_param_sf"));
            Assert.NotNull(in_param_sf);
            var in_prop_cf = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_prop_cf"));
            Assert.NotNull(in_prop_cf);
            var in_param_cf = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_param_cf"));
            Assert.NotNull(in_param_cf);
            var in_prop_prop = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_prop_prop"));
            Assert.NotNull(in_prop_prop);
            var in_param_prop = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_param_prop"));
            Assert.NotNull(in_param_prop);
            var in_prop_param = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_prop_param"));
            Assert.NotNull(in_prop_param);
            var in_param_param = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_param_param"));
            Assert.NotNull(in_param_param);
            var out_prop = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("out_prop"));
            Assert.NotNull(out_prop);
            var out_param = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("out_param"));
            Assert.NotNull(out_param);
            #endregion

            Assert.Equal(4, OutCF.Operand.Count);
            Assert.Equal(1, OutCF.Operand.Count(op => op.Symbol.Equals("param_alias") && op.ValueSource.Equals(out_param.Value.ID)));
            Assert.Equal(1, OutCF.Operand.Count(op => op.Symbol.Equals("prop_alias") && op.ValueSource.Equals(out_prop.Value.ID)));
            Assert.Equal(1, OutCF.Operand.Count(op => op.Symbol.Equals("out_param") && op.ValueSource.Equals(out_param.Value.ID)));
            Assert.Equal(1, OutCF.Operand.Count(op => op.Symbol.Equals("out_prop") && op.ValueSource.Equals(out_prop.Value.ID)));

            Assert.Equal(2, OutSF.Operand.Count);
            Assert.Equal(1, OutSF.Operand.Count(op => op.Equals(out_prop.Value.ID)));
            Assert.Equal(1, OutSF.Operand.Count(op => op.Equals(out_param.Value.ID)));

            var dv_outProp = OutProp.Value.ValueExpression as avm.DerivedValue;
            Assert.NotNull(dv_outProp);
            Assert.Equal(out_prop.Value.ID, dv_outProp.ValueSource);

            var pv_outParam = OutParam.Value.ValueExpression as avm.ParametricValue;
            Assert.NotNull(pv_outParam);
            var dv_outParam = pv_outParam.AssignedValue as avm.DerivedValue;
            Assert.NotNull(dv_outParam);
            Assert.Equal(out_param.Value.ID, dv_outParam.ValueSource);
        }

        [Fact]
        public void DS_FlowToComponentInstance()
        {
            String pathDS = "DesignSpaces/DC_FlowToComponentInstance";

            avm.Design design = Convert(pathDS);

            #region Get Objects
            var rc = design.RootContainer;
            Assert.NotNull(rc);
            var InSF = rc.Formula.OfType<avm.SimpleFormula>().FirstOrDefault(f => f.Name.Equals("InSF"));
            Assert.NotNull(InSF);
            var InCF = rc.Formula.OfType<avm.ComplexFormula>().FirstOrDefault(f => f.Name.Equals("InCF"));
            Assert.NotNull(InCF);
            var InProp = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("InProp"));
            Assert.NotNull(InProp);
            var InParam = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("InParam"));
            Assert.NotNull(InParam);

            var OutSF = rc.Formula.OfType<avm.SimpleFormula>().FirstOrDefault(f => f.Name.Equals("OutSF"));
            Assert.NotNull(OutSF);
            var OutCF = rc.Formula.OfType<avm.ComplexFormula>().FirstOrDefault(f => f.Name.Equals("OutCF"));
            Assert.NotNull(OutCF);
            var OutProp = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("OutProp"));
            Assert.NotNull(OutProp);
            var OutParam = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("OutParam"));
            Assert.NotNull(OutParam);

            Assert.Equal(1, rc.ComponentInstance.Count);
            var compInst = rc.ComponentInstance.First();

            Assert.NotNull(compInst);
            var in_prop_sf = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("in_prop_sf"));
            Assert.NotNull(in_prop_sf);
            var in_param_sf = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("in_param_sf"));
            Assert.NotNull(in_param_sf);
            var in_prop_cf = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("in_prop_cf"));
            Assert.NotNull(in_prop_cf);
            var in_param_cf = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("in_param_cf"));
            Assert.NotNull(in_param_cf);
            var in_prop_prop = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("in_prop_prop"));
            Assert.NotNull(in_prop_prop);
            var in_param_prop = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("in_param_prop"));
            Assert.NotNull(in_param_prop);
            var in_prop_param = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("in_prop_param"));
            Assert.NotNull(in_prop_param);
            var in_param_param = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("in_param_param"));
            Assert.NotNull(in_param_param);
            var out_prop = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("out_prop"));
            Assert.NotNull(out_prop);
            var out_param = compInst.PrimitivePropertyInstance.FirstOrDefault(p => p.IDinComponentModel.Equals("out_param"));
            Assert.NotNull(out_param);
            #endregion

            Assert.Equal(4, OutCF.Operand.Count);
            Assert.Equal(1, OutCF.Operand.Count(op => op.Symbol.Equals("param_alias") && op.ValueSource.Equals(out_param.Value.ID)));
            Assert.Equal(1, OutCF.Operand.Count(op => op.Symbol.Equals("prop_alias") && op.ValueSource.Equals(out_prop.Value.ID)));
            Assert.Equal(1, OutCF.Operand.Count(op => op.Symbol.Equals("out_param") && op.ValueSource.Equals(out_param.Value.ID)));
            Assert.Equal(1, OutCF.Operand.Count(op => op.Symbol.Equals("out_prop") && op.ValueSource.Equals(out_prop.Value.ID)));

            Assert.Equal(2, OutSF.Operand.Count);
            Assert.Equal(1, OutSF.Operand.Count(op => op.Equals(out_prop.Value.ID)));
            Assert.Equal(1, OutSF.Operand.Count(op => op.Equals(out_param.Value.ID)));

            var dv_outProp = OutProp.Value.ValueExpression as avm.DerivedValue;
            Assert.NotNull(dv_outProp);
            Assert.Equal(out_prop.Value.ID, dv_outProp.ValueSource);

            var pv_outParam = OutParam.Value.ValueExpression as avm.ParametricValue;
            Assert.NotNull(pv_outParam);
            var dv_outParam = pv_outParam.AssignedValue as avm.DerivedValue;
            Assert.NotNull(dv_outParam);
            Assert.Equal(out_param.Value.ID, dv_outParam.ValueSource);

            VerifyComponentPropertySource(InSF.ID, in_prop_sf);
            VerifyComponentPropertySource(InSF.ID, in_param_sf);
            VerifyComponentPropertySource(InCF.ID, in_prop_cf);
            VerifyComponentPropertySource(InCF.ID, in_param_cf);
            VerifyComponentPropertySource(InProp.Value.ID, in_param_prop);
            VerifyComponentPropertySource(InProp.Value.ID, in_prop_prop);
            VerifyComponentPropertySource(InParam.Value.ID, in_param_param);
            VerifyComponentPropertySource(InParam.Value.ID, in_prop_param);

            Assert.Equal("kg", OutParam.Value.Unit);
            Assert.Equal("kg", OutProp.Value.Unit);
            Assert.Equal("kg", InParam.Value.Unit);
            Assert.Equal("kg", InProp.Value.Unit);
        }

        [Fact]
        public void CA_SimpleFormula_OpTypes()
        {
            String pathCA = "ComponentAssemblies/SimpleFormula_OpTypes";
            avm.Design design = Convert(pathCA);
            var rc = design.RootContainer;
            Assert.NotNull(rc);

            Assert.Equal(6, rc.Formula.OfType<avm.SimpleFormula>().Count());

            foreach (var sf in rc.Formula.OfType<avm.SimpleFormula>())
            {
                Assert.True(sf.OperationSpecified);
                // The name of the SimpleFormula object will match the operation type.
                Assert.Equal(sf.Name, sf.Operation.ToString());
            }
        }

        [Fact]
        public void CA_FlowBetweenComponentInstances()
        {
            String pathCA = "ComponentAssemblies/FlowBetweenComponentInstances";
            avm.Design design = Convert(pathCA);
            var rc = design.RootContainer;
            Assert.NotNull(rc);

            var CompA = rc.ComponentInstance.FirstOrDefault(ci => ci.Name.Equals("CompA"));
            Assert.NotNull(CompA);
            var CompA_Prop1 = CompA.PrimitivePropertyInstance.FirstOrDefault(ppi => ppi.IDinComponentModel.Equals("Prop1"));
            Assert.NotNull(CompA_Prop1);
            var CompA_Param1 = CompA.PrimitivePropertyInstance.FirstOrDefault(ppi => ppi.IDinComponentModel.Equals("Param1"));
            Assert.NotNull(CompA_Param1);

            var CompB = rc.ComponentInstance.FirstOrDefault(ci => ci.Name.Equals("CompB"));
            Assert.NotNull(CompB);
            var CompB_Prop1 = CompB.PrimitivePropertyInstance.FirstOrDefault(ppi => ppi.IDinComponentModel.Equals("Prop1"));
            Assert.NotNull(CompB_Prop1);
            var CompB_Prop2 = CompB.PrimitivePropertyInstance.FirstOrDefault(ppi => ppi.IDinComponentModel.Equals("Prop2"));
            Assert.NotNull(CompB_Prop2);
            var CompB_Param1 = CompB.PrimitivePropertyInstance.FirstOrDefault(ppi => ppi.IDinComponentModel.Equals("Param1"));
            Assert.NotNull(CompB_Param1);
            var CompB_Param2 = CompB.PrimitivePropertyInstance.FirstOrDefault(ppi => ppi.IDinComponentModel.Equals("Param2"));
            Assert.NotNull(CompB_Param2);

            VerifyComponentPropertySource(CompA_Prop1.Value.ID, CompB_Prop1);
            VerifyComponentPropertySource(CompA_Prop1.Value.ID, CompB_Param2);
            VerifyComponentPropertySource(CompA_Param1.Value.ID, CompB_Param1);
            VerifyComponentPropertySource(CompA_Param1.Value.ID, CompB_Prop2);
        }

        [Fact]
        public void CA_Parameters()
        {
            String pathCA = "ComponentAssemblies/Parameters";
            avm.Design design = Convert(pathCA);
            var rc = design.RootContainer;
            Assert.NotNull(rc);

            // HasDefault
            var pv_HasDefault = GetValueExpression(rc, "HasDefault");
            Assert.IsType<avm.FixedValue>(pv_HasDefault.Default);
            Assert.Equal("1", (pv_HasDefault.Default as avm.FixedValue).Value);

            // HasRangeIntegers
            CheckParameterRange(rc, "-2", "2", "HasRangeIntegers");

            // HasRangeReals
            CheckParameterRange(rc, "-2.3", "2.93", "HasRangeReals");

            // HasValue
            var pv_HasValue = GetValueExpression(rc, "HasValue");
            Assert.IsType<avm.FixedValue>(pv_HasValue.AssignedValue);
            var fv_HasValue = pv_HasValue.AssignedValue as avm.FixedValue;
            Assert.Equal("1", fv_HasValue.Value);
            
            var val_HasValue = GetValue(rc, "HasValue");

            // DerivedValue_HasRangeReals
            CheckParameterRange(rc, "-2.3", "2.93", "DerivedValue_HasRangeReals");
            MustHaveNullDefault(rc, "DerivedValue_HasRangeReals");

            // DerivedValue_HasRangeIntegers
            CheckParameterRange(rc, "-2", "2", "DerivedValue_HasRangeIntegers");
            MustHaveNullDefault(rc, "DerivedValue_HasRangeIntegers");

            // DerivedValue_HasDefault
            var pv_DerivedValue_HasDefault = GetValueExpression(rc, "DerivedValue_HasDefault");
            Assert.NotNull(pv_DerivedValue_HasDefault.Default);
            Assert.IsType<avm.FixedValue>(pv_DerivedValue_HasDefault.Default);
            Assert.Equal("4", (pv_DerivedValue_HasDefault.Default as avm.FixedValue).Value);

            // DerivedValue_HasValue
            var pv_DerivedValue_HasValue = GetValueExpression(rc, "DerivedValue_HasValue");
            Assert.NotNull(pv_DerivedValue_HasValue.AssignedValue);
            Assert.IsType<avm.DerivedValue>(pv_DerivedValue_HasValue.AssignedValue);
            Assert.Equal(val_HasValue.ID, (pv_DerivedValue_HasValue.AssignedValue as avm.DerivedValue).ValueSource);
            MustHaveNullDefault(rc, "DerivedValue_HasValue");
            
            // InvalidRange_Whitespace
            var pv_InvalidRange_Whitespace = GetValueExpression(rc, "InvalidRange_Whitespace");
            Assert.Null(pv_InvalidRange_Whitespace.Minimum);
            Assert.Null(pv_InvalidRange_Whitespace.Maximum);

            // InvalidRange_OneNumber
            var pv_InvalidRange_OneNumber = GetValueExpression(rc, "InvalidRange_OneNumber");
            Assert.Null(pv_InvalidRange_OneNumber.Minimum);
            Assert.Null(pv_InvalidRange_OneNumber.Maximum);

            // InvalidRange_String
            var pv_InvalidRange_String = GetValueExpression(rc, "InvalidRange_String");
            Assert.Null(pv_InvalidRange_String.Minimum);
            Assert.Null(pv_InvalidRange_String.Maximum);
        }

        [Fact]
        public void CA_SubAsmFlowToComponentInstance()
        {
            String pathCA = "ComponentAssemblies/SubAsmFlowToComponentInstance";
            avm.Design design = Convert(pathCA);
            var rc = design.RootContainer;
            Assert.NotNull(rc);

            #region Get Objects
            var CompA = rc.ComponentInstance.FirstOrDefault(ci => ci.Name.Equals("CompA"));
            Assert.NotNull(CompA);
            var CompA_Prop1 = CompA.PrimitivePropertyInstance.FirstOrDefault(ppi => ppi.IDinComponentModel.Equals("Prop1"));
            Assert.NotNull(CompA_Prop1);
            var CompA_Param1 = CompA.PrimitivePropertyInstance.FirstOrDefault(ppi => ppi.IDinComponentModel.Equals("Param1"));
            Assert.NotNull(CompA_Param1);
            var CompA_Prop2 = CompA.PrimitivePropertyInstance.FirstOrDefault(ppi => ppi.IDinComponentModel.Equals("Prop2"));
            Assert.NotNull(CompA_Prop2);
            var CompA_Param2 = CompA.PrimitivePropertyInstance.FirstOrDefault(ppi => ppi.IDinComponentModel.Equals("Param2"));
            Assert.NotNull(CompA_Param2);

            var subAsm = rc.Container1.OfType<avm.Compound>().FirstOrDefault(c => c.Name.Equals("SubAsm"));
            Assert.NotNull(subAsm);
            var in_prop_prop = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_prop_prop"));
            Assert.NotNull(in_prop_prop);
            var in_param_prop = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_param_prop"));
            Assert.NotNull(in_param_prop);
            var in_prop_param = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_prop_param"));
            Assert.NotNull(in_prop_param);
            var in_param_param = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("in_param_param"));
            Assert.NotNull(in_param_param);
            var out_prop = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("out_prop"));
            Assert.NotNull(out_prop);
            var out_param = subAsm.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("out_param"));
            Assert.NotNull(out_param);
            #endregion

            VerifyComponentPropertySource(out_param.Value.ID, CompA_Param2);
            VerifyComponentPropertySource(out_prop.Value.ID, CompA_Prop2);

            Assert.NotNull(in_param_prop.Value.ValueExpression);
            Assert.IsType<avm.ParametricValue>(in_param_prop.Value.ValueExpression);
            var pv_in_param_prop = in_param_prop.Value.ValueExpression as avm.ParametricValue;
            Assert.NotNull(pv_in_param_prop.AssignedValue);
            Assert.IsType<avm.DerivedValue>(pv_in_param_prop.AssignedValue);
            Assert.Equal(CompA_Prop1.Value.ID, (pv_in_param_prop.AssignedValue as avm.DerivedValue).ValueSource);
        }

        private static void MustHaveNullDefault(avm.Container rc, String name)
        {
            var ve_DerivedValue_HasRangeReals = GetValue(rc, name).ValueExpression as avm.ParametricValue;
            Assert.Null(ve_DerivedValue_HasRangeReals.Default);
        }

        private static void CheckParameterRange(avm.Container rc, String strMinimum, String strMaximum, String name)
        {
            var parametricValue = GetValueExpression(rc, name);
            Assert.NotNull(parametricValue.Minimum);
            Assert.IsType<avm.FixedValue>(parametricValue.Minimum);
            Assert.Equal(strMinimum, (parametricValue.Minimum as avm.FixedValue).Value);
            Assert.NotNull(parametricValue.Maximum);
            Assert.IsType<avm.FixedValue>(parametricValue.Maximum);
            Assert.Equal(strMaximum, (parametricValue.Maximum as avm.FixedValue).Value);
        }

        private static avm.Value GetValue(avm.Container rc, String propName)
        {
            var parameter = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals(propName));
            Assert.NotNull(parameter);
            Assert.NotNull(parameter.Value);
            return parameter.Value;
        }

        private static avm.ParametricValue GetValueExpression(avm.Container rc, String propName)
        {
            var primitiveProperty = rc.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals(propName));
            Assert.NotNull(primitiveProperty);
            Assert.IsType<avm.ParametricValue>(primitiveProperty.Value.ValueExpression);
            var parametricValue = primitiveProperty.Value.ValueExpression as avm.ParametricValue;
            return parametricValue;
        }

        private avm.Design Convert(String pathDE)
        {
            MgaObject objDE = null;
            proj.PerformInTransaction(delegate
            {
                objDE = proj.get_ObjectByPath(pathDE);
            });
            Assert.NotNull(objDE);

            var interp = new CyPhyDesignExporter.CyPhyDesignExporterInterpreter();
            interp.Initialize(proj);
            InterpreterMainParameters param = new InterpreterMainParameters()
            {
                OutputDirectory = ValueFlowFixture.PathTest,
                CurrentFCO = objDE as MgaFCO,
                Project = proj                
            };
            var result = interp.Main(param);
            Assert.True(result.Success);

            // Load the new .adm file
            var pathAdm = Path.Combine(ValueFlowFixture.PathTest,
                                       pathDE.Split('/').Last() + ".adm");
            var xml = File.ReadAllText(pathAdm);
            var design = OpenMETA.Interchange.AvmXmlSerializer.Deserialize<avm.Design>(xml);
            Assert.NotNull(design);
            
            return design;
        }
    }
}
