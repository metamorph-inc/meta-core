using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using GME;
using GME.CSharp;
using GME.MGA;
using GME.MGA.Core;
using GME.Util;
using Cyber = ISIS.GME.Dsml.CyberComposition.Interfaces;
using CyberClasses = ISIS.GME.Dsml.CyberComposition.Classes;
using Simulink = ISIS.GME.Dsml.CyberComposition.Simulink.Interfaces;
using SimulinkClasses = ISIS.GME.Dsml.CyberComposition.Simulink.Classes;
using SignalFlow = ISIS.GME.Dsml.CyberComposition.SignalFlow.Interfaces;
using SignalFlowClasses = ISIS.GME.Dsml.CyberComposition.SignalFlow.Classes;
using avm;

namespace Cyber2AVM
{
    public class AVMComponentBuilder
    {
        private avm.Component _avmComponent = new avm.Component();

        private int _idCounter = 0;

        public GMEConsole GMEConsole { get; set; }

        public avm.Component getAVMComponent()
        {
            return _avmComponent;
        }

        private void createAVMCyberParameter(List<avm.modelica.Parameter> avmCyberModelParameterList, Cyber.ParameterRef cyberModelParameterRef)
        {
            string idString = "ID" + _idCounter;
            _idCounter++;

            // Initialize a avmCyber Parameter
            avm.modelica.Parameter avmCyberParameter = new avm.modelica.Parameter();
            
            ////
            // Set avm cyber parameter Value
            avmCyberParameter.Value = new avm.Value();
            
            // Assign ValueExpressionType for avm Value
            avm.DerivedValue avmDerivedValue = new avm.DerivedValue();
            avmDerivedValue.ValueSource = idString;
            avmCyberParameter.Value.ValueExpression = avmDerivedValue;    

            // Assign ID attr content
            avmCyberParameter.Value.ID = META2AVM_Util.UtilFuncs.ensureMetaIDAttribute(cyberModelParameterRef);
            ////
            
            // Assign Locator relative to the parent cyberModel
            avmCyberParameter.Locator = cyberModelParameterRef.Name;

            /// Now do the outer representation
            avm.PrimitiveProperty avmProperty = new avm.PrimitiveProperty();
            avmProperty.Name = avmCyberParameter.Locator;
            avmProperty.ID = "property." + idString;
            avmProperty.Notes = avmCyberParameter.Notes;

            avmProperty.Value = new avm.Value();
            avmProperty.Value.ID = idString;

            string val_str = "";
            string min_str = "";
            string max_str = "";

            Cyber.ParameterBase pbase = cyberModelParameterRef.Referred as Cyber.ParameterBase;
            if (pbase == null)
            {
                val_str = cyberModelParameterRef.Attributes.Value;
            }
            else if (pbase.Kind == "VFL_Parameter")
            {
                Simulink.SF_Parameter sfparam = pbase as Simulink.SF_Parameter;
                val_str = sfparam.Attributes.Value;

            }
            else if (pbase.Kind == "Data")
            {
                Simulink.Data data = pbase as Simulink.Data;
                val_str = data.Attributes.InitialValue;
                min_str = data.Attributes.Min;
                max_str = data.Attributes.Max;
            }
            else if (pbase.Kind == "SFData")
            {
                Simulink.SFData sfdata = pbase as Simulink.SFData;
                val_str = sfdata.Attributes.InitialValue;
                min_str = sfdata.Attributes.Min;
                max_str = sfdata.Attributes.Max;
            }

            avmProperty.Value.DimensionType = DimensionTypeEnum.Scalar;
            avmProperty.Value.DataType = DataTypeEnum.Real;
            avm.ParametricValue avmParametricValue = new avm.ParametricValue();

            // Try to get data out of each of the strings (cast to a scalar double)
            if (val_str != null && val_str.Length > 0)
            {
                Double value = 0.0;

                if (Double.TryParse(val_str, out value) == true)
                {
                    avm.FixedValue assignedVal = new avm.FixedValue();
                    assignedVal.Value = val_str;
                    avmParametricValue.AssignedValue = assignedVal;
                }
            }

            if (min_str != null && min_str.Length > 0)
            {
                Double min_val = 0.0;
                if (Double.TryParse(min_str, out min_val) == true)
                {
                    avm.FixedValue minVal = new avm.FixedValue();
                    minVal.Value = min_str;
                    avmParametricValue.Minimum = minVal;

                }
            }

            if (max_str != null && max_str.Length > 0)
            {
                Double max_val = 0.0;
                if (Double.TryParse(max_str, out max_val) == true)
                {
                    avm.FixedValue maxVal = new avm.FixedValue();
                    maxVal.Value = max_str;
                    avmParametricValue.Maximum = maxVal;
                }
            }

            avmProperty.Value.ValueExpression = avmParametricValue;

            // Add avm param to the given List
            avmCyberModelParameterList.Add(avmCyberParameter);
            _avmComponent.Property.Add(avmProperty);
        }

        private void createAVMCyberConnector(List<avm.modelica.Connector> avmCyberModelConnectorList,  ISIS.GME.Common.Interfaces.Model cyberModelSignalInterface)
        {

            //Intialize new avm.modelica.Connector
            avm.modelica.Connector avmCyberConnector = new avm.modelica.Connector();

            // Assign the Class to avmCyberConnector from cyber SignalInterface
            avmCyberConnector.Class = META2AVM_Util.UtilFuncs.getMetaAttributeContent(cyberModelSignalInterface, "Class");

            // Assign name to the avmconnector from cyber SignalInterface
            //avmCyberConnector.Name = META2AVM_Util.UtilFuncs.getMetaObjectName(cyberModelSignalInterface);
            avmCyberConnector.Name = cyberModelSignalInterface.Name;

            // Assign Locator to the avmconnector from cyber SignalInterface relative path from its CyberModel
            avmCyberConnector.Locator = cyberModelSignalInterface.Name; // the Port is a direct child of the CyberModel hence locator is the name

            // Assign id to the avmconnector from cyber SignalInterface UDM ID
            avmCyberConnector.ID = META2AVM_Util.UtilFuncs.ensureMetaIDAttribute(cyberModelSignalInterface);
            


            // Now create the outer connector
            avm.Connector avmConnector = new avm.Connector();
            avmConnector.Name = avmCyberConnector.Name;
            avmConnector.ID = avmCyberConnector.ID;

            avm.modelica.Connector innerConnector = new avm.modelica.Connector();
            innerConnector.ID = META2AVM_Util.UtilFuncs.ensureMetaIDAttribute();
            innerConnector.Class = avmCyberConnector.Class;
            innerConnector.Locator = avmCyberConnector.Locator;
            innerConnector.Name = avmCyberConnector.Name;
            innerConnector.PortMap.Add(avmCyberConnector.ID);
     
            avmConnector.Role.Add(innerConnector);
            _avmComponent.Connector.Add(avmConnector);

            // Add the created avm connector to the given list
            avmCyberModelConnectorList.Add(avmCyberConnector);
        }

        public void createAVMCyberModel(Cyber.ModelicaComponent cyberModel, string filename)
        {
            // Initialize new AVM cyber model
            avm.cyber.CyberModel avmCyberModel = new avm.cyber.CyberModel();

            // Set Class
            avmCyberModel.Class = cyberModel.Attributes.Class;

            // Set Name
            _avmComponent.Name = cyberModel.Name;
            avmCyberModel.Name = cyberModel.Name;

            _avmComponent.ID = META2AVM_Util.UtilFuncs.ensureMetaIDAttribute();

            _avmComponent.SchemaVersion = "2.5";
            avmCyberModel.Notes = "";
            avmCyberModel.UsesResource = "cyber.path";

            // Create locator string
            string[] path_parts = filename.Split('\\');
            string[] filename_parts = path_parts[path_parts.Length - 1].Split('.');
            avmCyberModel.Locator = cyberModel.Path.Replace('/', '.');

            avm.Resource res = new avm.Resource();
            res.Path = "Cyber\\" + path_parts[path_parts.Length - 1];
            res.Name = "CyberModel";
            res.ID = "cyber.path";
            
            _avmComponent.ResourceDependency.Add(res);

            avm.Resource res2 = new avm.Resource();
            res2.Path = "Cyber\\" + filename_parts[0] + ".xml";
            res2.Name = "CyberXML";
            res2.ID = "cyberxml.path";

            _avmComponent.ResourceDependency.Add(res2);

            if (cyberModel != null && cyberModel.Impl is IMgaFCO
                && (cyberModel.Impl as IMgaFCO).Meta.Name == "SimulinkWrapper")
            {
                string simulink_file =  (cyberModel.Impl as IMgaFCO).StrAttrByName["ModelFilename"];
                string[] sl_path_parts = simulink_file.Split('\\');
                string sl_filename_part = sl_path_parts[sl_path_parts.Length - 1];

                System.IO.File.Copy(simulink_file, System.IO.Path.Combine("Cyber", sl_filename_part), true);

                avm.Resource res3 = new avm.Resource();
                res3.Path = "Cyber\\" + sl_filename_part;
                res3.Name = "SimulinkModel";
                res3.ID = "simulink.path";

                _avmComponent.ResourceDependency.Add(res3);
            }


            // Set Type
            GMEConsole.Out.WriteLine(cyberModel.GetType().Name);
            if (cyberModel.Impl.MetaBase.Name == "SimulinkWrapper")
            {
               avmCyberModel.Type = avm.cyber.ModelType.Simulink;
            }
            else //(cyberModel.Impl.MetaBase.Name == "SignalFlowWrapper")
            {
               avmCyberModel.Type = avm.cyber.ModelType.SignalFlow;
            }
            //else
            //{
                // Other ModelicaComponent types not handled for now
            //    return;
            //}

            // for every InputSignalInterface in the ModelicaComponent create avm.modelica.Connector.
            // Note: Connector in AVM Cyber Model in a copy of AVM ModelicaModel
            foreach (Cyber.InputSignalInterface cyberInputSignalInterface in cyberModel.Children.InputSignalInterfaceCollection)
            {
                createAVMCyberConnector(avmCyberModel.Connector, cyberInputSignalInterface);
            }

            // for every OutputSignalInterface in the ModelicaComponent create avm.modelica.connector
            foreach (Cyber.OutputSignalInterface cyberOutputSignalInterface in cyberModel.Children.OutputSignalInterfaceCollection)
            {
                createAVMCyberConnector(avmCyberModel.Connector, cyberOutputSignalInterface);
            }

            // for every BusInterface in the ModelicaComponent create avm.modelica.connector
            // FIX: may require fix. Not sure!
            foreach (Cyber.BusInterface cyberBusInterface in cyberModel.Children.BusInterfaceCollection)
            {
                createAVMCyberConnector(avmCyberModel.Connector, cyberBusInterface);
            }

            // for every ParameterRef in the ModelicaComponent create avm.modelica.parameter
            foreach (Cyber.ParameterRef cyberParameterRef in cyberModel.Children.ParameterRefCollection)
            {
                createAVMCyberParameter(avmCyberModel.Parameter, cyberParameterRef);
            }
            
            // Add it to _avmComponent's DomainModel List
            _avmComponent.DomainModel.Add(avmCyberModel);
            // 
        }
    }
}
