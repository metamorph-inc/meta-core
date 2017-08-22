using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using GME.MGA;
using CyPhy2CAD_CSharp.DataRep;

namespace CyPhy2CAD_CSharp.TestBenchModel
{
    public class FEAConstraintBase
    {
        private CADGeometry Geometry;

        public virtual CAD.AnalysisConstraintType ToCADXMLOutput()
        {
            CAD.AnalysisConstraintType constraintout = new CAD.AnalysisConstraintType();
            constraintout._id = UtilityHelpers.MakeUdmID();
            if (Geometry != null)
                constraintout.Geometry = Geometry.ToCADXMLOutput();

            return constraintout;
        }

        public void AddGeometry(CADGeometry geometry)
        {
            Geometry = geometry;
        }
    }

    public class FEALoadBase
    {
        private CADGeometry Geometry;

        public virtual CAD.LoadType ToCADXMLOutput()
        {
            CAD.LoadType loadout = new CAD.LoadType();
            loadout._id = UtilityHelpers.MakeUdmID();
            if (Geometry != null)
                loadout.Geometry = Geometry.ToCADXMLOutput();

            return loadout;
        }

        public void AddGeometry(CADGeometry geometry)
        {
            Geometry = geometry;
        }

    }

    public class FEAPinConstraint : FEAConstraintBase
    {
        public string AxialRotation { get; set; }
        public string AxialDisplacement { get; set; }

        public override CAD.AnalysisConstraintType ToCADXMLOutput()
        {
            CAD.AnalysisConstraintType constraintout = base.ToCADXMLOutput();
            constraintout.Pin = new CAD.PinType();
            constraintout.Pin._id = UtilityHelpers.MakeUdmID();
            constraintout.Pin.AxialDisplacement = new CAD.AxialDisplacementType();
            constraintout.Pin.AxialDisplacement._id = UtilityHelpers.MakeUdmID();
            constraintout.Pin.AxialDisplacement.Property = AxialDisplacement;
            constraintout.Pin.AxialRotation = new CAD.AxialRotationType();
            constraintout.Pin.AxialRotation._id = UtilityHelpers.MakeUdmID();
            constraintout.Pin.AxialRotation.Property = AxialRotation;
            return constraintout;
        }
    }

    public class FEABallConstraint : FEAConstraintBase
    {
        public override CAD.AnalysisConstraintType ToCADXMLOutput()
        {
            CAD.AnalysisConstraintType constraintout = base.ToCADXMLOutput();
            constraintout.Ball = new CAD.BallType();
            return constraintout;
        }
    }

    public class FEADisplacementConstraint : FEAConstraintBase
    {
        public string TranslationUnits { get; set; }
        public string Translation_X { get; set; }
        public string Translation_Y { get; set; }
        public string Translation_Z { get; set; }
        public string RotationUnits { get; set; }
        public string Rotation_X { get; set; }
        public string Rotation_Y { get; set; }
        public string Rotation_Z { get; set; }

        public override CAD.AnalysisConstraintType ToCADXMLOutput()
        {
            CAD.AnalysisConstraintType constraintout = base.ToCADXMLOutput();
            constraintout.Displacement = new CAD.DisplacementType();
            constraintout.Displacement._id = UtilityHelpers.MakeUdmID();
            constraintout.Displacement.Rotation = new CAD.RotationType();
            constraintout.Displacement.Rotation._id = UtilityHelpers.MakeUdmID();
            constraintout.Displacement.Rotation.Units = RotationUnits;
            constraintout.Displacement.Rotation.x = Rotation_X;
            constraintout.Displacement.Rotation.y = Rotation_Y;
            constraintout.Displacement.Rotation.z = Rotation_Z;
            constraintout.Displacement.Translation = new CAD.TranslationType();
            constraintout.Displacement.Translation._id = UtilityHelpers.MakeUdmID();
            constraintout.Displacement.Translation.Units = TranslationUnits;
            constraintout.Displacement.Translation.x = Translation_X;
            constraintout.Displacement.Translation.y = Translation_Y;
            constraintout.Displacement.Translation.z = Translation_Z;
            return constraintout;
        }
    }

    public class FEAccelerationLoad : FEALoadBase
    {
        public string Units { get; set; }
        public double X { get; set; }
        public double Y { get; set; }
        public double Z { get; set; }
        public string Acceleration_id { get; } = UtilityHelpers.MakeUdmID();

        public override CAD.LoadType ToCADXMLOutput()
        {
            CAD.LoadType loadout = base.ToCADXMLOutput();
            loadout.Acceleration = new CAD.AccelerationType();
            loadout.Acceleration._id = Acceleration_id;
            loadout.Acceleration.Units = Units;
            loadout.Acceleration.x = X;
            loadout.Acceleration.y = Y;
            loadout.Acceleration.z = Z;
            return loadout;
        }
    }

    public class FEAPressureLoad : FEALoadBase
    {
        public double Value { get; set; }
        public string Units { get; set; }
        public string Pressure_id { get; } = UtilityHelpers.MakeUdmID();

        public override CAD.LoadType ToCADXMLOutput()
        {
            CAD.LoadType loadout = base.ToCADXMLOutput();
            loadout.Pressure = new CAD.PressureType();
            loadout.Pressure._id = Pressure_id;
            loadout.Pressure.Units = Units;
            loadout.Pressure.Value = Value;
            return loadout;
        }
    }

    public class FEAForceLoad : FEALoadBase
    {
        public string ForceUnit { get; set; }
        public double Force_X { get; set; }
        public double Force_Y { get; set; }
        public double Force_Z { get; set; }
        public string MomentUnit { get; set; }
        public double Moment_X { get; set; }
        public double Moment_Y { get; set; }
        public double Moment_Z { get; set; }
        public string Force_id { get; } = UtilityHelpers.MakeUdmID();
        public string Moment_id { get; } = UtilityHelpers.MakeUdmID();

        public override CAD.LoadType ToCADXMLOutput()
        {
            CAD.LoadType loadout = base.ToCADXMLOutput();
            loadout.ForceMoment = new CAD.ForceMomentType();
            loadout.ForceMoment._id = UtilityHelpers.MakeUdmID();
            loadout.ForceMoment.Force = new CAD.ForceType();
            loadout.ForceMoment.Force._id = Force_id;
            loadout.ForceMoment.Force.Units = ForceUnit;
            loadout.ForceMoment.Force.x = Force_X;
            loadout.ForceMoment.Force.y = Force_Y;
            loadout.ForceMoment.Force.z = Force_Z;
            loadout.ForceMoment.Moment = new CAD.MomentType();
            loadout.ForceMoment.Moment._id = Moment_id;
            loadout.ForceMoment.Moment.Units = MomentUnit;
            loadout.ForceMoment.Moment.x = Moment_X;
            loadout.ForceMoment.Moment.y = Moment_Y;
            loadout.ForceMoment.Moment.z = Moment_Z;
            return loadout;
        }
    }


    public class FEAThermalElement
    {
        public CADGeometry Geometry;
        public bool IsGeometryBased = true;
        public string ComponentID;
        public string Type;
        public string Unit;
        public double LoadValue;

        public FEAThermalElement(CyPhy.Parameter param)
        {
            Type = ((MgaFCO)param.Impl).MetaRole.Name;
            ConvertFromParameter(param);
        }

        public static FEAThermalElement[] Extract(CyPhy.ThermalFEAElements thermalElement, string compid, CADGeometry geom)
        {
            List<FEAThermalElement> result = new List<FEAThermalElement>();

            if (thermalElement is CyPhy.ThermalEnvironment)
            {
                if ((thermalElement as CyPhy.ThermalEnvironment).Children.ParameterCollection.Any())
                {
                    var param = (thermalElement as CyPhy.ThermalEnvironment).Children.ParameterCollection.First();
                    string role = ((MgaFCO)param.Impl).MetaRole.Name;
                    var elem = new FEAThermalElement(param) { ComponentID = compid, Geometry = geom };
                    result.Add(elem);
                }
            }
            else if (thermalElement is CyPhy.ThermalLoad)
            {
                foreach (var param in (thermalElement as CyPhy.ThermalLoad).Children.ParameterCollection)
                {
                    var elem = new FEAThermalElement(param) { ComponentID = compid, Geometry = geom };
                    if (elem.Type == "Convection")
                    {
                        elem.Unit = "J/(s-mm^2-K)";
                    }
                    else if (elem.Type == "HeatFlux")
                    {
                        elem.Unit = "J/(s-mm^2)";
                    }
                    else if (elem.Type == "HeatGeneration")
                    {
                        elem.Unit = "J/s";
                    }
                    else if (elem.Type == "InitialTemperature")
                    {
                        elem.Unit = "K";
                    }
                    else if (elem.Type == "SpecifiedTemperature")
                    {
                        elem.Unit = "K";
                    }
                    result.Add(elem);
                }
            }
            return result.ToArray();
        }

        private void ConvertFromParameter(CyPhy.Parameter parameter)
        {
            try
            {
                LoadValue = String.IsNullOrEmpty(parameter.Attributes.Value) ? 0 : double.Parse(parameter.Attributes.Value);                
            }
            catch (InvalidOperationException)
            {
                Logger.Instance.AddLogMessage("Parameter" + parameter.Name + " was not found in Load.", Severity.Error);
            }
            catch (FormatException)
            {
                Logger.Instance.AddLogMessage("Parameter" + parameter.Name + " is not a valid floating point value.", Severity.Error);
            }

            if (parameter.Referred != null)
            {
                if (parameter.Referred.unit != null)
                {
                    if (!String.IsNullOrEmpty(parameter.Referred.unit.Name))
                        Unit = parameter.Referred.unit.Name;
                }
            }

        }
    }

}
