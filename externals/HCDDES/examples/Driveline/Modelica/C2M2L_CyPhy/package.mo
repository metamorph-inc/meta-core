within ;
package C2M2L_CyPhy "Special Components and Test Assemblies for CyPhy Development"
  extends Modelica.Icons.Package;

import SI = Modelica.SIunits;
import Modelica.Constants.pi;


  package Extra_Components
    extends Modelica.Icons.Package;
    package BaseComponents
    extends Modelica.Icons.BasesPackage;
      partial model Suspension

        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor2(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{-154,90},{-174,110}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor1(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{-152,-110},{-172,-90}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing brg_RHS1
          annotation (Placement(transformation(extent={{-210,90},{-190,110}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing brg_LHS1
          annotation (Placement(transformation(extent={{-210,-110},{-190,-90}})));
        outer C2M2L_Decl.Testing_Blocks.Land.Terrains.Ramp_60per ground_context(
          show_trace=true,
          show_road=true,
          wheel_rad=0.3,
          samples=0,
          road=[-1.7,0.0; 0,0.0; 5,0.5; 7,0.5])
          annotation (Placement(transformation(extent={{-160,120},{-140,140}})));
        outer Modelica.Mechanics.MultiBody.World world(gravityType=Modelica.Mechanics.MultiBody.Types.GravityTypes.UniformGravity)
          annotation (Placement(transformation(extent={{-200,120},{-180,140}})));
        Modelica.Electrical.Analog.Interfaces.NegativePin ground
          annotation (Placement(transformation(extent={{-210,-50},{-190,-30}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a
          annotation (Placement(transformation(extent={{-216,44},{-184,76}})));
        C2M2L_Decl.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_bus
          annotation (Placement(transformation(extent={{-10,130},{10,150}})));
      equation
        connect(adaptor2.flange,brg_RHS1. flange) annotation (Line(
            points={{-164,100},{-200,100}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(adaptor1.flange,brg_LHS1. flange) annotation (Line(
            points={{-162,-100},{-200,-100}},
            color={0,0,0},
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=true, extent={{-200,-140},
                  {200,140}}), graphics), Icon(coordinateSystem(extent={{-200,-140},{200,
                  140}}, preserveAspectRatio=true), graphics={Text(
                extent={{-200,140},{200,-140}},
                lineColor={0,0,255},
                textString="Hull and suspension"),            Text(
                extent={{-140,120},{140,60}},
                lineColor={0,0,255},
                textString="1D interface")}));
      end Suspension;

      partial model Suspension_3D_interface_Decl

        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing brg_RHS1(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{-210,90},{-190,110}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing brg_LHS1(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{-210,-110},{-190,-90}})));
        outer Modelica.Mechanics.MultiBody.World world(gravityType=Modelica.Mechanics.MultiBody.Types.GravityTypes.UniformGravity)
          annotation (Placement(transformation(extent={{-200,120},{-180,140}})));
        Modelica.Electrical.Analog.Interfaces.NegativePin ground
          annotation (Placement(transformation(extent={{-210,-50},{-190,-30}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a
          annotation (Placement(transformation(extent={{-216,44},{-184,76}})));
        C2M2L_Decl.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_bus
          annotation (Placement(transformation(extent={{-10,130},{10,150}})));
        outer C2M2L_Decl.Testing_Blocks.Land.Terrains.Ground_Definition_2D
                                                 ground_context
          annotation (Placement(transformation(extent={{-160,120},{-140,140}})));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false,extent={{-200,
                  -140},{200,140}}),
                               graphics), Icon(coordinateSystem(extent={{-200,-140},{200,
                  140}}, preserveAspectRatio=true), graphics={Text(
                extent={{-200,140},{200,-140}},
                lineColor={0,0,255},
                textString="Hull and suspension"),            Text(
                extent={{-140,120},{140,60}},
                lineColor={0,0,255},
                textString="3D interface")}));
      end Suspension_3D_interface_Decl;
    end BaseComponents;

    model Dyno_Non_Motoring_1D "Dyno using an adjustable damper"
      extends C2M2L_Decl.Containers.Mechanical_Models.Rigid_Body_1_Shaft(has_mass=
            false);

    parameter Real max_damping = 10000;
      C2M2L_Ext.Mechanics.Rotational.Components.Damper_Adjustable damper_Adjustable
        annotation (Placement(transformation(extent={{62,18},{82,38}})));
      Modelica.Blocks.Continuous.LimPID PID1(
        yMax=max_damping,
        k=k,
        Ti=Ti,
        Td=Td,
        controllerType=Modelica.Blocks.Types.SimpleController.PI,
        initType=Modelica.Blocks.Types.InitPID.SteadyState,
        yMin=0.01)
              annotation (Placement(transformation(extent={{24,-52},{44,-32}})));
  public
      Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor1(
          includeBearingConnector=false)
        annotation (Placement(transformation(extent={{-22,0},{-2,20}})));
      Modelica.Blocks.Interfaces.RealInput Set_Speed_rpm
      "Dyno set speed in rpm"
        annotation (Placement(transformation(extent={{-20,-20},{20,20}},
            rotation=-90,
            origin={-94,206}), iconTransformation(
            extent={{-20,-20},{20,20}},
            rotation=-90,
            origin={0,240})));
      Modelica.Mechanics.Rotational.Sensors.SpeedSensor speedSensor
        annotation (Placement(transformation(extent={{8,44},{28,64}})));
      Modelica.Mechanics.MultiBody.Parts.Mounting1D mounting1D(n={0,0,1})
        annotation (Placement(transformation(extent={{-8,-88},{12,-68}})));
      Modelica.Blocks.Math.UnitConversions.To_rpm convert_speed_to_rpm
        annotation (Placement(transformation(
            origin={72,54},
            extent={{-10,-10},{10,10}},
            rotation=0)));
      parameter Real k=10 "Proportional Gain of controller";
      parameter SI.Time Ti=4 "Time constant of Integrator block";
      parameter SI.Time Td=0.5 "Time constant of derivative block";
      // Modelica.Mechanics.Rotational.Components.Inertia inertia(w(fixed=true, start=
      //         Set_Speed_rpm*pi/30), J=J)
      //   annotation (Placement(transformation(extent={{18,0},{38,20}})));
      Modelica.Mechanics.Rotational.Components.Inertia inertia(J=J, w(start=w_start))
        annotation (Placement(transformation(extent={{18,0},{38,20}})));
      Modelica.Blocks.Math.Feedback feedback
        annotation (Placement(transformation(extent={{86,80},{106,100}})));
      Modelica.Blocks.Sources.Constant const(k=0)
        annotation (Placement(transformation(extent={{-48,-52},{-28,-32}})));
      parameter SI.Inertia J=1 "Moment of inertia";
      parameter SI.AngularVelocity w_start "initial_speed";
    //  initial equation
    //    inertia.w = Set_Speed_rpm*pi/30;

    equation
      connect(damper_Adjustable.flange_a,adaptor1. flange) annotation (Line(
          points={{62,28},{-2,28},{-2,10},{-12,10}},
          color={0,0,0},
          smooth=Smooth.None));
      connect(adaptor1.flangeAndFrame, brg_01) annotation (Line(
          points={{-22,10},{-110,10},{-110,0},{-200,0}},
          color={135,135,135},
          thickness=0.5,
          smooth=Smooth.None));
      connect(speedSensor.flange, adaptor1.flange) annotation (Line(
          points={{8,54},{-12,54},{-12,10}},
          color={0,0,0},
          smooth=Smooth.None));
      connect(mounting1D.frame_a, origin) annotation (Line(
          points={{2,-88},{2,-200},{0,-200}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(damper_Adjustable.flange_b, mounting1D.flange_b) annotation (Line(
          points={{82,28},{110,28},{110,-78},{12,-78}},
          color={0,0,0},
          smooth=Smooth.None));
      connect(speedSensor.w, convert_speed_to_rpm.u) annotation (Line(
          points={{29,54},{60,54}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(inertia.flange_a, adaptor1.flange) annotation (Line(
          points={{18,10},{-12,10}},
          color={0,0,0},
          smooth=Smooth.None));
      connect(feedback.u2, convert_speed_to_rpm.y) annotation (Line(
          points={{96,82},{96,54},{83,54}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(Set_Speed_rpm, feedback.u1) annotation (Line(
          points={{-94,206},{-94,90},{88,90}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(feedback.y, PID1.u_m) annotation (Line(
          points={{105,90},{126,90},{126,-88},{34,-88},{34,-54}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(PID1.y, damper_Adjustable.u) annotation (Line(
          points={{45,-42},{60,-42},{60,23},{73,23}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(const.y, PID1.u_s) annotation (Line(
          points={{-27,-42},{22,-42}},
          color={0,0,127},
          smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(preserveAspectRatio=false,extent={{-200,
                -320},{200,280}}),
                          graphics), Icon(coordinateSystem(preserveAspectRatio=true,
              extent={{-200,-320},{200,280}}), graphics={
                                                    Text(
              extent={{-102,42},{98,-38}},
              lineColor={0,0,255},
              textString="DYNO"),
            Text(
              extent={{-6,206},{104,166}},
              visible=  not use_rpm,
              lineColor={128,128,128},
              textString="rpm")}));
    end Dyno_Non_Motoring_1D;

    model Parametric_ECU
    "A parametric instance of a conventional diesel or petrol ECU"
      parameter Modelica.SIunits.Conversions.NonSIunits.AngularVelocity_rpm ShiftUpMaxRPM = 2000
      "Maximum RPM in the up shift table";
      parameter Modelica.SIunits.Conversions.NonSIunits.AngularVelocity_rpm ShiftUpMinRPM = ShiftUpMaxRPM * 0.8
      "Minimum RPM in the up shift table";
      parameter Modelica.SIunits.Conversions.NonSIunits.AngularVelocity_rpm ShiftDownMaxRPM = 1195
      "Maximum RPM in the down shift table";
      parameter Modelica.SIunits.Conversions.NonSIunits.AngularVelocity_rpm ShiftDownMinRPM = 800
      "Minimum RPM in the down shift table";

      extends
      C2M2L_Decl.C2M2L_Component_Building_Blocks.Prime_Movers.Electrical_Controls.General_Controllers.Conventional_ECU(
        overspeed_disable= (Modelica.Constants.pi/30)*(overspeed_limit - 150),
        shift_up_map=[0,(Modelica.Constants.pi/30)*ShiftUpMinRPM; 1,(Modelica.Constants.pi/30)*ShiftUpMaxRPM],
        shift_up_delta=(Modelica.Constants.pi/30)*(ShiftUpMaxRPM-ShiftUpMinRPM) * 0.46,
        shift_down_map=[
        0,(Modelica.Constants.pi/30)*(ShiftDownMaxRPM - ShiftDownMinRPM) / 0.5;
        0.1,(Modelica.Constants.pi/30)*(ShiftDownMaxRPM - ShiftDownMinRPM) / 0.48;
        0.2,(Modelica.Constants.pi/30)*(ShiftDownMaxRPM - ShiftDownMinRPM) / 0.47;
        0.3,(Modelica.Constants.pi/30)*(ShiftDownMaxRPM - ShiftDownMinRPM) / 0.465;
        0.4,(Modelica.Constants.pi/30)*(ShiftDownMaxRPM - ShiftDownMinRPM) / 0.434;
        0.5,(Modelica.Constants.pi/30)*(ShiftDownMaxRPM - ShiftDownMinRPM) / 0.416;
        0.6,(Modelica.Constants.pi/30)*(ShiftDownMaxRPM - ShiftDownMinRPM) / 0.389;
        0.7,(Modelica.Constants.pi/30)*(ShiftDownMaxRPM - ShiftDownMinRPM) / 0.336;
        0.8,(Modelica.Constants.pi/30)*(ShiftDownMaxRPM - ShiftDownMinRPM) / 0.336;
        0.9,(Modelica.Constants.pi/30)*(ShiftDownMaxRPM - ShiftDownMinRPM) / 0.33;
        1,(Modelica.Constants.pi/30)*(ShiftDownMaxRPM - ShiftDownMinRPM) / 0.33],
        shift_down_delta=(Modelica.Constants.pi/30)*(ShiftDownMaxRPM-ShiftDownMinRPM) * 0.34);

    end Parametric_ECU;

    package Suspension
    extends Modelica.Icons.Package;
      model Full_hull_and_suspension_2w
      extends C2M2L_CyPhy.Extra_Components.BaseComponents.Suspension;
        inner Testing_Blocks.Hull.Full_hull_3DOF_2w_0g_no_speed_sensors
          full_hull_3DOF_6w_3g_no_speed_sensors(susp_offset=0, m_hull=5000)
          annotation (Placement(transformation(extent={{-12,-20},{148,68}})));
        C2M2L_Ext.Testing_Blocks.Hull.Suspension_Full_Container_2_Wheels
          suspension_Full_Container_2_Wheels
          annotation (Placement(transformation(extent={{-24,-116},{152,-52}})));
        C2M2L_Ext.Testing_Blocks.Hull.Suspension_Full_Container_2_Wheels
          suspension_Full_Container_2_Wheels1
          annotation (Placement(transformation(extent={{-24,140},{152,76}})));
      equation

        connect(suspension_Full_Container_2_Wheels.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket) annotation (Line(
            points={{138,-74},{138,-20},{116.333,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler) annotation (Line(
            points={{-10,-74},{-10,-20},{-7,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_6) annotation (Line(
            points={{4,-70},{4,-20.88},{1.33333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_1) annotation (Line(
            points={{124,-70},{124,-20.88},{101.333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler1) annotation (Line(
            points={{-10,98},{-8,98},{-8,57.44},{-7,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_12) annotation (Line(
            points={{4,94},{4,58.496},{1.33333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_7) annotation (Line(
            points={{124,94},{108,94},{108,58.496},{101.333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket1) annotation (Line(
            points={{138,98},{128,98},{128,57.44},{116.333,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor1.flangeAndFrame, full_hull_3DOF_6w_3g_no_speed_sensors.right_power_in)
          annotation (Line(
            points={{-152,-100},{-56,-100},{-56,-4},{101.333,-4},{101.333,1.12}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.left_power_in, adaptor2.flangeAndFrame)
          annotation (Line(
            points={{101.333,36.32},{-58,36.32},{-58,100},{-154,100}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_a, full_hull_3DOF_6w_3g_no_speed_sensors.frame_a) annotation (
            Line(
            points={{-200,60},{-66,60},{-66,18.72},{68,18.72}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(ground, full_hull_3DOF_6w_3g_no_speed_sensors.pin_n) annotation (Line(
            points={{-200,-40},{-144,-40},{-144,15.2},{-12,15.2}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.driver_bus, driver_bus)
          annotation (Line(
            points={{-12,28.928},{-26,28.928},{-26,30},{-40,30},{-40,140},{0,140}},
            color={170,213,255},
            thickness=0.5,
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-200,
                  -140},{200,140}}), graphics), Icon(coordinateSystem(extent={{-200,
                  -140},{200,140}}, preserveAspectRatio=true), graphics={Text(
                extent={{-88,-52},{94,-106}},
                lineColor={0,0,255},
                textString="2 wheels")}));
      end Full_hull_and_suspension_2w;

      model Full_hull_and_suspension_4w
      extends C2M2L_CyPhy.Extra_Components.BaseComponents.Suspension;
        inner Testing_Blocks.Hull.Full_hull_3DOF_4w_0g_no_speed_sensors
          full_hull_3DOF_6w_3g_no_speed_sensors(susp_offset=0, m_hull=5000)
          annotation (Placement(transformation(extent={{-12,-20},{148,68}})));
        Testing_Blocks.Hull.Suspension_Full_Container_4_Wheels
          suspension_Full_Container_2_Wheels
          annotation (Placement(transformation(extent={{-24,-116},{152,-52}})));
        Testing_Blocks.Hull.Suspension_Full_Container_4_Wheels
          suspension_Full_Container_2_Wheels1
          annotation (Placement(transformation(extent={{-24,140},{152,76}})));
      equation

        connect(suspension_Full_Container_2_Wheels.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket) annotation (Line(
            points={{138,-74},{138,-20},{116.333,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler) annotation (Line(
            points={{-10,-74},{-10,-20},{-7,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_6) annotation (Line(
            points={{4,-70},{4,-20.88},{4.33333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_1) annotation (Line(
            points={{124,-70},{124,-20.88},{101.333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler1) annotation (Line(
            points={{-10,98},{-8,98},{-8,57.44},{-7,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_12) annotation (Line(
            points={{4,94},{4,58.496},{3.33333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_7) annotation (Line(
            points={{124,94},{108,94},{108,58.496},{101.333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket1) annotation (Line(
            points={{138,98},{128,98},{128,57.44},{116.333,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor1.flangeAndFrame, full_hull_3DOF_6w_3g_no_speed_sensors.right_power_in)
          annotation (Line(
            points={{-152,-100},{-56,-100},{-56,-4},{101.333,-4},{101.333,1.12}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.left_power_in, adaptor2.flangeAndFrame)
          annotation (Line(
            points={{101.333,36.32},{-58,36.32},{-58,100},{-154,100}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_a, full_hull_3DOF_6w_3g_no_speed_sensors.frame_a) annotation (
            Line(
            points={{-200,60},{-66,60},{-66,18.72},{68,18.72}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(ground, full_hull_3DOF_6w_3g_no_speed_sensors.pin_n) annotation (Line(
            points={{-200,-40},{-144,-40},{-144,15.2},{-12,15.2}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_4,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_10) annotation (Line(
            points={{50.4,93.6},{50.4,75.8},{41.6667,75.8},{41.6667,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_9) annotation (Line(
            points={{76.4,93.2},{76.4,75.6},{65.3333,75.6},{65.3333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_4,
          suspension_Full_Container_2_Wheels.frame_susp_4) annotation (Line(
            points={{40.6667,-20.88},{40.6667,-45.44},{50.4,-45.44},{50.4,-69.6}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_3) annotation (Line(
            points={{76.4,-69.2},{76.4,-44.6},{63.6667,-44.6},{63.6667,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.driver_bus, driver_bus)
          annotation (Line(
            points={{-12,25.76},{-26,25.76},{-26,26},{-40,26},{-40,140},{0,140}},
            color={170,213,255},
            thickness=0.5,
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-200,
                  -140},{200,140}}), graphics), Icon(coordinateSystem(extent={{-200,
                  -140},{200,140}}, preserveAspectRatio=true), graphics={Text(
                extent={{-90,-52},{92,-106}},
                lineColor={0,0,255},
                textString="4 wheels")}));
      end Full_hull_and_suspension_4w;

      model Full_hull_and_suspension_4w_1g
      extends C2M2L_CyPhy.Extra_Components.BaseComponents.Suspension;
        inner Testing_Blocks.Hull.Full_hull_3DOF_4w_1g_no_speed_sensors
          full_hull_3DOF_6w_3g_no_speed_sensors(susp_offset=0, m_hull=5000)
          annotation (Placement(transformation(extent={{-12,-20},{148,68}})));
        Testing_Blocks.Hull.Suspension_Full_Container_4_Wheels_1_guide
          suspension_Full_Container_2_Wheels
          annotation (Placement(transformation(extent={{-24,-116},{152,-52}})));
        Testing_Blocks.Hull.Suspension_Full_Container_4_Wheels_1_guide
          suspension_Full_Container_2_Wheels1
          annotation (Placement(transformation(extent={{-24,140},{152,76}})));
      equation

        connect(suspension_Full_Container_2_Wheels.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket) annotation (Line(
            points={{138,-74},{138,-20},{116.333,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler) annotation (Line(
            points={{-10,-74},{-10,-20},{-7,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_6) annotation (Line(
            points={{4,-70},{4,-20.88},{4.33333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_1) annotation (Line(
            points={{124,-70},{124,-20.88},{101.333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler1) annotation (Line(
            points={{-10,98},{-8,98},{-8,57.44},{-7,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_12) annotation (Line(
            points={{4,94},{4,58.496},{3.33333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_7) annotation (Line(
            points={{124,94},{108,94},{108,58.496},{101.333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket1) annotation (Line(
            points={{138,98},{128,98},{128,57.44},{116.333,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor1.flangeAndFrame, full_hull_3DOF_6w_3g_no_speed_sensors.right_power_in)
          annotation (Line(
            points={{-152,-100},{-56,-100},{-56,-4},{101.333,-4},{101.333,1.12}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.left_power_in, adaptor2.flangeAndFrame)
          annotation (Line(
            points={{101.333,36.32},{-58,36.32},{-58,100},{-154,100}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_a, full_hull_3DOF_6w_3g_no_speed_sensors.frame_a) annotation (
            Line(
            points={{-200,60},{-66,60},{-66,18.72},{68,18.72}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(ground, full_hull_3DOF_6w_3g_no_speed_sensors.pin_n) annotation (Line(
            points={{-200,-40},{-144,-40},{-144,15.2},{-12,15.2}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_4,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_10) annotation (Line(
            points={{50.4,93.6},{50.4,75.8},{41.6667,75.8},{41.6667,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_9) annotation (Line(
            points={{76.4,93.2},{76.4,75.6},{65.3333,75.6},{65.3333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_4,
          suspension_Full_Container_2_Wheels.frame_susp_4) annotation (Line(
            points={{40.6667,-20.88},{40.6667,-45.44},{50.4,-45.44},{50.4,-69.6}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_3) annotation (Line(
            points={{76.4,-69.2},{76.4,-44.6},{63.6667,-44.6},{63.6667,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.driver_bus, driver_bus)
          annotation (Line(
            points={{-12.3333,27.52},{-26,27.52},{-26,26},{-40,26},{-40,140},{0,140}},
            color={170,213,255},
            thickness=0.5,
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(suspension_Full_Container_2_Wheels.guide_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.guide_2) annotation (Line(
            points={{64,-62},{58,-62},{58,-12.96},{51.3333,-12.96}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.guide_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.guide_5) annotation (Line(
            points={{64,86},{58,86},{58,50.4},{51.3333,50.4}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-200,
                  -140},{200,140}}), graphics), Icon(coordinateSystem(extent={{-200,
                  -140},{200,140}}, preserveAspectRatio=true), graphics={Text(
                extent={{-90,-52},{92,-106}},
                lineColor={0,0,255},
                textString="4 wheels"),                                  Text(
                extent={{-90,-92},{92,-146}},
                lineColor={0,0,255},
                textString="1 guide")}));
      end Full_hull_and_suspension_4w_1g;

      model Full_hull_and_suspension_6w
      extends C2M2L_CyPhy.Extra_Components.BaseComponents.Suspension;
        inner Testing_Blocks.Hull.Full_hull_3DOF_6w_0g_no_speed_sensors
          full_hull_3DOF_6w_3g_no_speed_sensors(susp_offset=0, m_hull=5000)
          annotation (Placement(transformation(extent={{-12,-20},{148,68}})));
        Testing_Blocks.Hull.Suspension_Full_Container_6_Wheels
          suspension_Full_Container_2_Wheels
          annotation (Placement(transformation(extent={{-24,-116},{152,-52}})));
        Testing_Blocks.Hull.Suspension_Full_Container_6_Wheels
          suspension_Full_Container_2_Wheels1
          annotation (Placement(transformation(extent={{-24,140},{152,76}})));
      equation

        connect(suspension_Full_Container_2_Wheels.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket) annotation (Line(
            points={{138,-74},{138,-20},{116.333,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler) annotation (Line(
            points={{-10,-74},{-10,-20},{-7,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_6) annotation (Line(
            points={{4,-70},{4,-20.88},{4.33333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_1) annotation (Line(
            points={{124,-70},{124,-20.88},{101.333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler1) annotation (Line(
            points={{-10,98},{-8,98},{-8,57.44},{-7,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_12) annotation (Line(
            points={{4,94},{4,58.496},{3.33333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_7) annotation (Line(
            points={{124,94},{108,94},{108,58.496},{101.333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket1) annotation (Line(
            points={{138,98},{128,98},{128,57.44},{116.333,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_5,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_5) annotation (Line(
            points={{25.2,-69.6},{25.2,-44.8},{24,-44.8},{24,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_4,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_4) annotation (Line(
            points={{47.6,-69.6},{47.6,-45.8},{40.6667,-45.8},{40.6667,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_3) annotation (Line(
            points={{78.8,-69.2},{78.8,-45.6},{63.6667,-45.6},{63.6667,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_2) annotation (Line(
            points={{102.4,-69.2},{102.4,-45.6},{84,-45.6},{84,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_9) annotation (Line(
            points={{78.8,93.2},{78.8,75.6},{65.3333,75.6},{65.3333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_8) annotation (Line(
            points={{102.4,93.2},{102.4,76.6},{81.3333,76.6},{81.3333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_4,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_10) annotation (Line(
            points={{47.6,93.6},{47.6,75.8},{41.6667,75.8},{41.6667,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_5,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_11) annotation (Line(
            points={{25.2,93.6},{25.2,76.8},{21.6667,76.8},{21.6667,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor1.flangeAndFrame, full_hull_3DOF_6w_3g_no_speed_sensors.right_power_in)
          annotation (Line(
            points={{-152,-100},{-56,-100},{-56,-4},{101.333,-4},{101.333,1.12}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.left_power_in, adaptor2.flangeAndFrame)
          annotation (Line(
            points={{101.333,36.32},{-58,36.32},{-58,100},{-154,100}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_a, full_hull_3DOF_6w_3g_no_speed_sensors.frame_a) annotation (
            Line(
            points={{-200,60},{-66,60},{-66,18.72},{68,18.72}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(ground, full_hull_3DOF_6w_3g_no_speed_sensors.pin_n) annotation (Line(
            points={{-200,-40},{-144,-40},{-144,15.2},{-12,15.2}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.driver_bus, driver_bus)
          annotation (Line(
            points={{-12,26.112},{-26,26.112},{-26,140},{0,140}},
            color={170,213,255},
            thickness=0.5,
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-200,
                  -140},{200,140}}), graphics), Icon(coordinateSystem(extent={{-200,
                  -140},{200,140}}, preserveAspectRatio=true), graphics={Text(
                extent={{-98,-46},{84,-100}},
                lineColor={0,0,255},
                textString="6 wheels")}));
      end Full_hull_and_suspension_6w;

      model Full_hull_and_suspension_6w_3g
      extends C2M2L_CyPhy.Extra_Components.BaseComponents.Suspension;
        inner Testing_Blocks.Hull.Full_hull_3DOF_6w_3g_no_speed_sensors
          full_hull_3DOF_6w_3g_no_speed_sensors(susp_offset=0, m_hull=5000)
          annotation (Placement(transformation(extent={{-12,-20},{148,68}})));
        Testing_Blocks.Hull.Suspension_Full_Container_6_Wheels_3_guides
          suspension_Full_Container_2_Wheels
          annotation (Placement(transformation(extent={{-24,-114},{152,-50}})));
        Testing_Blocks.Hull.Suspension_Full_Container_6_Wheels_3_guides
          suspension_Full_Container_2_Wheels1
          annotation (Placement(transformation(extent={{-24,140},{152,76}})));
      equation

        connect(suspension_Full_Container_2_Wheels.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket) annotation (Line(
            points={{138,-72},{138,-20},{116.333,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler) annotation (Line(
            points={{-10,-72},{-10,-20},{-7,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_6) annotation (Line(
            points={{4,-68},{4,-20.88},{4.33333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_1) annotation (Line(
            points={{124,-68},{124,-20.88},{101.333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler1) annotation (Line(
            points={{-10,98},{-8,98},{-8,57.44},{-7,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_12) annotation (Line(
            points={{4,94},{4,58.496},{3.33333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_7) annotation (Line(
            points={{124,94},{108,94},{108,58.496},{101.333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket1) annotation (Line(
            points={{138,98},{128,98},{128,57.44},{116.333,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_5,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_5) annotation (Line(
            points={{25.2,-67.6},{25.2,-44.8},{24,-44.8},{24,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_4,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_4) annotation (Line(
            points={{47.6,-67.6},{47.6,-45.8},{40.6667,-45.8},{40.6667,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_3) annotation (Line(
            points={{78.8,-67.2},{78.8,-45.6},{63.6667,-45.6},{63.6667,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_2) annotation (Line(
            points={{102.4,-67.2},{102.4,-45.6},{84,-45.6},{84,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_9) annotation (Line(
            points={{78.8,93.2},{78.8,75.6},{65.3333,75.6},{65.3333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_8) annotation (Line(
            points={{102.4,93.2},{102.4,76.6},{81.3333,76.6},{81.3333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_4,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_10) annotation (Line(
            points={{47.6,93.6},{47.6,75.8},{41.6667,75.8},{41.6667,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_5,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_11) annotation (Line(
            points={{25.2,93.6},{25.2,76.8},{21.6667,76.8},{21.6667,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor1.flangeAndFrame, full_hull_3DOF_6w_3g_no_speed_sensors.right_power_in)
          annotation (Line(
            points={{-152,-100},{-56,-100},{-56,-4},{101.333,-4},{101.333,1.12}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.left_power_in, adaptor2.flangeAndFrame)
          annotation (Line(
            points={{101.333,36.32},{-58,36.32},{-58,100},{-154,100}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_a, full_hull_3DOF_6w_3g_no_speed_sensors.frame_a) annotation (
            Line(
            points={{-200,60},{-66,60},{-66,18.72},{68,18.72}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(ground, full_hull_3DOF_6w_3g_no_speed_sensors.pin_n) annotation (Line(
            points={{-200,-40},{-144,-40},{-144,15.2},{-12,15.2}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.driver_bus, driver_bus)
          annotation (Line(
            points={{-12,29.28},{-26,29.28},{-26,140},{0,140}},
            color={170,213,255},
            thickness=0.5,
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.guide_3,
          suspension_Full_Container_2_Wheels.guide_3) annotation (Line(
            points={{9.66667,-12.96},{9.66667,-36.48},{14.4,-36.48},{14.4,-60.4}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.guide_2,
          suspension_Full_Container_2_Wheels.guide_2) annotation (Line(
            points={{51.3333,-12.96},{51.3333,-36.48},{66.8,-36.48},{66.8,-60.4}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.guide_1,
          suspension_Full_Container_2_Wheels.guide_1) annotation (Line(
            points={{93,-12.96},{93,-37.48},{112.8,-37.48},{112.8,-60}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.guide_4,
          suspension_Full_Container_2_Wheels1.guide_1) annotation (Line(
            points={{93,50.4},{93,68.2},{112.8,68.2},{112.8,86}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.guide_5,
          suspension_Full_Container_2_Wheels1.guide_2) annotation (Line(
            points={{51.3333,50.4},{51.3333,68.2},{66.8,68.2},{66.8,86.4}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.guide_6,
          suspension_Full_Container_2_Wheels1.guide_3) annotation (Line(
            points={{9.66667,50.4},{9.66667,68.2},{14.4,68.2},{14.4,86.4}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-200,
                  -140},{200,140}}), graphics), Icon(coordinateSystem(extent={{-200,
                  -140},{200,140}}, preserveAspectRatio=true), graphics={Text(
                extent={{-98,-46},{84,-100}},
                lineColor={0,0,255},
                textString="6 wheels"),                                  Text(
                extent={{-98,-84},{84,-138}},
                lineColor={0,0,255},
                textString="3 guides")}));
      end Full_hull_and_suspension_6w_3g;

      package Tests
        extends C2M2L_Decl.Icons.Package_For_Simple_Test_Cases;

        model Suspension_only_2w
          extends C2M2L_CyPhy.Icons.Simple_Test_Case_200x200;
          inner Modelica.Mechanics.MultiBody.World world(gravityType=Modelica.Mechanics.MultiBody.Types.GravityTypes.UniformGravity)
            annotation (Placement(transformation(extent={{-180,120},{-160,140}})));
          inner Testing_Blocks.Land.Terrains.Flat                 ground_context(
            show_trace=true,
            show_road=true,
            wheel_rad=0.3,
            samples=0,
            road=[-10,0; 50,0; 100,5; 150,5; 200,0; 1000,0])
            annotation (Placement(transformation(extent={{-140,120},{-120,140}})));
          Modelica.Blocks.Sources.Trapezoid torque(
            offset=0,
            startTime=1,
            rising=1,
            period=20,
            nperiod=1,
            width=8,
            amplitude=-2500)
            annotation (Placement(transformation(extent={{-180,-40},{-160,-20}})));
          Modelica.Mechanics.Rotational.Sources.Torque torque2
            annotation (Placement(transformation(extent={{-138,-40},{-118,-20}})));
          Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing flange(
              includeBearingConnector=false)
            annotation (Placement(transformation(extent={{-100,-40},{-80,-20}})));
          Modelica.Blocks.Sources.Trapezoid torque1(
            offset=0,
            startTime=1,
            rising=1,
            period=20,
            nperiod=1,
            width=8,
            amplitude=2500)
            annotation (Placement(transformation(extent={{-180,0},{-160,20}})));
          Modelica.Mechanics.Rotational.Sources.Torque torque3
            annotation (Placement(transformation(extent={{-140,0},{-120,20}})));
          Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing flange1(
              includeBearingConnector=false)
            annotation (Placement(transformation(extent={{-100,0},{-80,20}})));
          replaceable Full_hull_and_suspension_2w full_hull_and_suspension_6w
            constrainedby BaseComponents.Suspension
            annotation (Placement(transformation(extent={{-64,-64},{84,40}})));
          C2M2L_Ext.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_bus1
            annotation (Placement(transformation(extent={{0,80},{20,100}})));
        equation

          connect(torque2.flange,flange. flange) annotation (Line(
              points={{-118,-30},{-90,-30}},
              color={0,0,0},
              smooth=Smooth.None));
          connect(torque.y,torque2. tau) annotation (Line(
              points={{-159,-30},{-140,-30}},
              color={0,0,127},
              smooth=Smooth.None));
          connect(torque3.flange, flange1.flange)
                                                 annotation (Line(
              points={{-120,10},{-90,10}},
              color={0,0,0},
              smooth=Smooth.None));
          connect(torque1.y, torque3.tau)
                                         annotation (Line(
              points={{-159,10},{-142,10}},
              color={0,0,127},
              smooth=Smooth.None));
          connect(full_hull_and_suspension_6w.driver_bus, driver_bus1) annotation (
              Line(
              points={{10,40},{10,90}},
              color={170,213,255},
              thickness=0.5,
              smooth=Smooth.None), Text(
              string="%second",
              index=1,
              extent={{6,3},{6,3}}));
          connect(flange1, full_hull_and_suspension_6w.brg_RHS1) annotation (Line(
              points={{-90,10},{-78,10},{-78,25.1429},{-64,25.1429}},
              color={135,135,135},
              thickness=0.5,
              smooth=Smooth.None));
          connect(flange, full_hull_and_suspension_6w.brg_LHS1) annotation (Line(
              points={{-90,-30},{-78,-30},{-78,-49.1429},{-64,-49.1429}},
              color={135,135,135},
              thickness=0.5,
              smooth=Smooth.None));
          annotation (
            Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-180,-140},{
                    180,160}}), graphics),
            experiment(StopTime=100, __Dymola_Algorithm="Esdirk45a"),
            __Dymola_experimentSetupOutput,
            Icon(coordinateSystem(extent={{-180,-140},{180,160}})));
        end Suspension_only_2w;

        model Suspension_only_4w
          extends C2M2L_CyPhy.Icons.Simple_Test_Case_200x200;
          inner Modelica.Mechanics.MultiBody.World world(gravityType=Modelica.Mechanics.MultiBody.Types.GravityTypes.UniformGravity)
            annotation (Placement(transformation(extent={{-180,120},{-160,140}})));
          inner Testing_Blocks.Land.Terrains.Flat                 ground_context(
            show_trace=true,
            show_road=true,
            wheel_rad=0.3,
            samples=0,
            road=[-10,0; 50,0; 100,5; 150,5; 200,0; 1000,0])
            annotation (Placement(transformation(extent={{-140,120},{-120,140}})));
          Modelica.Blocks.Sources.Trapezoid torque(
            offset=0,
            startTime=1,
            rising=1,
            period=20,
            nperiod=1,
            width=8,
            amplitude=-2500)
            annotation (Placement(transformation(extent={{-180,-40},{-160,-20}})));
          Modelica.Mechanics.Rotational.Sources.Torque torque2
            annotation (Placement(transformation(extent={{-138,-40},{-118,-20}})));
          Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing flange(
              includeBearingConnector=false)
            annotation (Placement(transformation(extent={{-100,-40},{-80,-20}})));
          Modelica.Blocks.Sources.Trapezoid torque1(
            offset=0,
            startTime=1,
            rising=1,
            period=20,
            nperiod=1,
            width=8,
            amplitude=2500)
            annotation (Placement(transformation(extent={{-180,0},{-160,20}})));
          Modelica.Mechanics.Rotational.Sources.Torque torque3
            annotation (Placement(transformation(extent={{-140,0},{-120,20}})));
          Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing flange1(
              includeBearingConnector=false)
            annotation (Placement(transformation(extent={{-100,0},{-80,20}})));
          replaceable Full_hull_and_suspension_4w full_hull_and_suspension_6w
            constrainedby BaseComponents.Suspension
            annotation (Placement(transformation(extent={{-64,-64},{84,40}})));
          C2M2L_Ext.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_bus1
            annotation (Placement(transformation(extent={{0,80},{20,100}})));
        equation

          connect(torque2.flange,flange. flange) annotation (Line(
              points={{-118,-30},{-90,-30}},
              color={0,0,0},
              smooth=Smooth.None));
          connect(torque.y,torque2. tau) annotation (Line(
              points={{-159,-30},{-140,-30}},
              color={0,0,127},
              smooth=Smooth.None));
          connect(torque3.flange, flange1.flange)
                                                 annotation (Line(
              points={{-120,10},{-90,10}},
              color={0,0,0},
              smooth=Smooth.None));
          connect(torque1.y, torque3.tau)
                                         annotation (Line(
              points={{-159,10},{-142,10}},
              color={0,0,127},
              smooth=Smooth.None));
          connect(full_hull_and_suspension_6w.driver_bus, driver_bus1) annotation (
              Line(
              points={{10,40},{10,90}},
              color={170,213,255},
              thickness=0.5,
              smooth=Smooth.None), Text(
              string="%second",
              index=1,
              extent={{6,3},{6,3}}));
          connect(flange1, full_hull_and_suspension_6w.brg_RHS1) annotation (Line(
              points={{-90,10},{-78,10},{-78,25.1429},{-64,25.1429}},
              color={135,135,135},
              thickness=0.5,
              smooth=Smooth.None));
          connect(flange, full_hull_and_suspension_6w.brg_LHS1) annotation (Line(
              points={{-90,-30},{-78,-30},{-78,-49.1429},{-64,-49.1429}},
              color={135,135,135},
              thickness=0.5,
              smooth=Smooth.None));
          annotation (
            Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-180,-140},{
                    180,160}}), graphics),
            experiment(StopTime=100, __Dymola_Algorithm="Esdirk45a"),
            __Dymola_experimentSetupOutput,
            Icon(coordinateSystem(extent={{-180,-140},{180,160}})));
        end Suspension_only_4w;

        model Suspension_only_4w_1g
          extends C2M2L_CyPhy.Icons.Simple_Test_Case_200x200;
          inner Modelica.Mechanics.MultiBody.World world(gravityType=Modelica.Mechanics.MultiBody.Types.GravityTypes.UniformGravity)
            annotation (Placement(transformation(extent={{-180,120},{-160,140}})));
          inner Testing_Blocks.Land.Terrains.Flat                 ground_context(
            show_trace=true,
            show_road=true,
            wheel_rad=0.3,
            samples=0,
            road=[-10,0; 50,0; 100,5; 150,5; 200,0; 1000,0])
            annotation (Placement(transformation(extent={{-140,120},{-120,140}})));
          Modelica.Blocks.Sources.Trapezoid torque(
            offset=0,
            startTime=1,
            rising=1,
            period=20,
            nperiod=1,
            width=8,
            amplitude=-2500)
            annotation (Placement(transformation(extent={{-180,-40},{-160,-20}})));
          Modelica.Mechanics.Rotational.Sources.Torque torque2
            annotation (Placement(transformation(extent={{-138,-40},{-118,-20}})));
          Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing flange(
              includeBearingConnector=false)
            annotation (Placement(transformation(extent={{-100,-40},{-80,-20}})));
          Modelica.Blocks.Sources.Trapezoid torque1(
            offset=0,
            startTime=1,
            rising=1,
            period=20,
            nperiod=1,
            width=8,
            amplitude=2500)
            annotation (Placement(transformation(extent={{-180,0},{-160,20}})));
          Modelica.Mechanics.Rotational.Sources.Torque torque3
            annotation (Placement(transformation(extent={{-140,0},{-120,20}})));
          Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing flange1(
              includeBearingConnector=false)
            annotation (Placement(transformation(extent={{-100,0},{-80,20}})));
          replaceable Full_hull_and_suspension_4w_1g full_hull_and_suspension_6w
            constrainedby BaseComponents.Suspension
            annotation (Placement(transformation(extent={{-64,-64},{84,40}})));
          C2M2L_Ext.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_bus1
            annotation (Placement(transformation(extent={{0,80},{20,100}})));
        equation

          connect(torque2.flange,flange. flange) annotation (Line(
              points={{-118,-30},{-90,-30}},
              color={0,0,0},
              smooth=Smooth.None));
          connect(torque.y,torque2. tau) annotation (Line(
              points={{-159,-30},{-140,-30}},
              color={0,0,127},
              smooth=Smooth.None));
          connect(torque3.flange, flange1.flange)
                                                 annotation (Line(
              points={{-120,10},{-90,10}},
              color={0,0,0},
              smooth=Smooth.None));
          connect(torque1.y, torque3.tau)
                                         annotation (Line(
              points={{-159,10},{-142,10}},
              color={0,0,127},
              smooth=Smooth.None));
          connect(full_hull_and_suspension_6w.driver_bus, driver_bus1) annotation (
              Line(
              points={{10,40},{10,90}},
              color={170,213,255},
              thickness=0.5,
              smooth=Smooth.None), Text(
              string="%second",
              index=1,
              extent={{6,3},{6,3}}));
          connect(flange1, full_hull_and_suspension_6w.brg_RHS1) annotation (Line(
              points={{-90,10},{-78,10},{-78,25.1429},{-64,25.1429}},
              color={135,135,135},
              thickness=0.5,
              smooth=Smooth.None));
          connect(flange, full_hull_and_suspension_6w.brg_LHS1) annotation (Line(
              points={{-90,-30},{-78,-30},{-78,-49.1429},{-64,-49.1429}},
              color={135,135,135},
              thickness=0.5,
              smooth=Smooth.None));
          annotation (
            Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-180,-140},{
                    180,160}}), graphics),
            experiment(StopTime=100, __Dymola_Algorithm="Esdirk45a"),
            __Dymola_experimentSetupOutput,
            Icon(coordinateSystem(extent={{-180,-140},{180,160}})));
        end Suspension_only_4w_1g;

        model Suspension_only_6w
          extends C2M2L_CyPhy.Icons.Simple_Test_Case_200x200;
          inner Modelica.Mechanics.MultiBody.World world(gravityType=Modelica.Mechanics.MultiBody.Types.GravityTypes.UniformGravity)
            annotation (Placement(transformation(extent={{-180,120},{-160,140}})));
          inner Testing_Blocks.Land.Terrains.Flat                 ground_context(
            show_trace=true,
            show_road=true,
            wheel_rad=0.3,
            samples=0,
            road=[-10,0; 50,0; 100,5; 150,5; 200,0; 1000,0])
            annotation (Placement(transformation(extent={{-140,120},{-120,140}})));
          Modelica.Blocks.Sources.Trapezoid torque(
            offset=0,
            startTime=1,
            rising=1,
            period=20,
            nperiod=1,
            width=8,
            amplitude=-2500)
            annotation (Placement(transformation(extent={{-180,-40},{-160,-20}})));
          Modelica.Mechanics.Rotational.Sources.Torque torque2
            annotation (Placement(transformation(extent={{-138,-40},{-118,-20}})));
          Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing flange(
              includeBearingConnector=false)
            annotation (Placement(transformation(extent={{-100,-40},{-80,-20}})));
          Modelica.Blocks.Sources.Trapezoid torque1(
            offset=0,
            startTime=1,
            rising=1,
            period=20,
            nperiod=1,
            width=8,
            amplitude=2500)
            annotation (Placement(transformation(extent={{-180,0},{-160,20}})));
          Modelica.Mechanics.Rotational.Sources.Torque torque3
            annotation (Placement(transformation(extent={{-140,0},{-120,20}})));
          Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing flange1(
              includeBearingConnector=false)
            annotation (Placement(transformation(extent={{-100,0},{-80,20}})));
          replaceable Full_hull_and_suspension_4w_1g full_hull_and_suspension_6w
            constrainedby BaseComponents.Suspension
            annotation (Placement(transformation(extent={{-64,-64},{84,40}})));
          C2M2L_Ext.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_bus1
            annotation (Placement(transformation(extent={{0,80},{20,100}})));
        equation

          connect(torque2.flange,flange. flange) annotation (Line(
              points={{-118,-30},{-90,-30}},
              color={0,0,0},
              smooth=Smooth.None));
          connect(torque.y,torque2. tau) annotation (Line(
              points={{-159,-30},{-140,-30}},
              color={0,0,127},
              smooth=Smooth.None));
          connect(torque3.flange, flange1.flange)
                                                 annotation (Line(
              points={{-120,10},{-90,10}},
              color={0,0,0},
              smooth=Smooth.None));
          connect(torque1.y, torque3.tau)
                                         annotation (Line(
              points={{-159,10},{-142,10}},
              color={0,0,127},
              smooth=Smooth.None));
          connect(full_hull_and_suspension_6w.driver_bus, driver_bus1) annotation (
              Line(
              points={{10,40},{10,90}},
              color={170,213,255},
              thickness=0.5,
              smooth=Smooth.None), Text(
              string="%second",
              index=1,
              extent={{6,3},{6,3}}));
          connect(flange1, full_hull_and_suspension_6w.brg_RHS1) annotation (Line(
              points={{-90,10},{-78,10},{-78,25.1429},{-64,25.1429}},
              color={135,135,135},
              thickness=0.5,
              smooth=Smooth.None));
          connect(flange, full_hull_and_suspension_6w.brg_LHS1) annotation (Line(
              points={{-90,-30},{-78,-30},{-78,-49.1429},{-64,-49.1429}},
              color={135,135,135},
              thickness=0.5,
              smooth=Smooth.None));
          annotation (
            Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-180,-140},{
                    180,160}}), graphics),
            experiment(StopTime=100, __Dymola_Algorithm="Esdirk45a"),
            __Dymola_experimentSetupOutput,
            Icon(coordinateSystem(extent={{-180,-140},{180,160}})));
        end Suspension_only_6w;

        model Suspension_only_6w_3g
          extends C2M2L_CyPhy.Icons.Simple_Test_Case_200x200;
          inner Modelica.Mechanics.MultiBody.World world(gravityType=Modelica.Mechanics.MultiBody.Types.GravityTypes.UniformGravity)
            annotation (Placement(transformation(extent={{-180,120},{-160,140}})));
          inner Testing_Blocks.Land.Terrains.Flat                 ground_context(
            show_trace=true,
            show_road=true,
            wheel_rad=0.3,
            samples=0,
            road=[-10,0; 50,0; 100,5; 150,5; 200,0; 1000,0])
            annotation (Placement(transformation(extent={{-140,120},{-120,140}})));
          Modelica.Blocks.Sources.Trapezoid torque(
            offset=0,
            startTime=1,
            rising=1,
            period=20,
            nperiod=1,
            width=8,
            amplitude=-2500)
            annotation (Placement(transformation(extent={{-180,-40},{-160,-20}})));
          Modelica.Mechanics.Rotational.Sources.Torque torque2
            annotation (Placement(transformation(extent={{-138,-40},{-118,-20}})));
          Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing flange(
              includeBearingConnector=false)
            annotation (Placement(transformation(extent={{-100,-40},{-80,-20}})));
          Modelica.Blocks.Sources.Trapezoid torque1(
            offset=0,
            startTime=1,
            rising=1,
            period=20,
            nperiod=1,
            width=8,
            amplitude=2500)
            annotation (Placement(transformation(extent={{-180,0},{-160,20}})));
          Modelica.Mechanics.Rotational.Sources.Torque torque3
            annotation (Placement(transformation(extent={{-140,0},{-120,20}})));
          Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing flange1(
              includeBearingConnector=false)
            annotation (Placement(transformation(extent={{-100,0},{-80,20}})));
          replaceable Full_hull_and_suspension_6w_3g full_hull_and_suspension_6w
            constrainedby BaseComponents.Suspension
            annotation (Placement(transformation(extent={{-64,-64},{84,40}})));
          C2M2L_Ext.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_bus1
            annotation (Placement(transformation(extent={{0,80},{20,100}})));
        equation

          connect(torque2.flange,flange. flange) annotation (Line(
              points={{-118,-30},{-90,-30}},
              color={0,0,0},
              smooth=Smooth.None));
          connect(torque.y,torque2. tau) annotation (Line(
              points={{-159,-30},{-140,-30}},
              color={0,0,127},
              smooth=Smooth.None));
          connect(torque3.flange, flange1.flange)
                                                 annotation (Line(
              points={{-120,10},{-90,10}},
              color={0,0,0},
              smooth=Smooth.None));
          connect(torque1.y, torque3.tau)
                                         annotation (Line(
              points={{-159,10},{-142,10}},
              color={0,0,127},
              smooth=Smooth.None));
          connect(full_hull_and_suspension_6w.driver_bus, driver_bus1) annotation (
              Line(
              points={{10,40},{10,90}},
              color={170,213,255},
              thickness=0.5,
              smooth=Smooth.None), Text(
              string="%second",
              index=1,
              extent={{6,3},{6,3}}));
          connect(flange1, full_hull_and_suspension_6w.brg_RHS1) annotation (Line(
              points={{-90,10},{-78,10},{-78,25.1429},{-64,25.1429}},
              color={135,135,135},
              thickness=0.5,
              smooth=Smooth.None));
          connect(flange, full_hull_and_suspension_6w.brg_LHS1) annotation (Line(
              points={{-90,-30},{-78,-30},{-78,-49.1429},{-64,-49.1429}},
              color={135,135,135},
              thickness=0.5,
              smooth=Smooth.None));
          annotation (
            Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-180,-140},{
                    180,160}}), graphics),
            experiment(StopTime=100, __Dymola_Algorithm="Esdirk45a"),
            __Dymola_experimentSetupOutput,
            Icon(coordinateSystem(extent={{-180,-140},{180,160}})));
        end Suspension_only_6w_3g;
      end Tests;
    end Suspension;

    package Suspension_3D_interface
    extends Modelica.Icons.Package;
      model Full_hull_and_suspension_2w
      extends
        C2M2L_CyPhy.Extra_Components.BaseComponents.Suspension_3D_interface;
        inner Testing_Blocks.Hull.Full_hull_3DOF_2w_0g_no_speed_sensors
          full_hull_3DOF_6w_3g_no_speed_sensors(susp_offset=0, m_hull=5000)
          annotation (Placement(transformation(extent={{-12,-20},{148,68}})));
        C2M2L_Ext.Testing_Blocks.Hull.Suspension_Full_Container_2_Wheels
          suspension_Full_Container_2_Wheels
          annotation (Placement(transformation(extent={{-24,-116},{152,-52}})));
        C2M2L_Ext.Testing_Blocks.Hull.Suspension_Full_Container_2_Wheels
          suspension_Full_Container_2_Wheels1
          annotation (Placement(transformation(extent={{-24,140},{152,76}})));
      equation

        connect(suspension_Full_Container_2_Wheels.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket) annotation (Line(
            points={{138,-74},{138,-20},{116.333,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler) annotation (Line(
            points={{-10,-74},{-10,-20},{-7,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_6) annotation (Line(
            points={{4,-70},{4,-20.88},{1.33333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_1) annotation (Line(
            points={{124,-70},{124,-20.88},{101.333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler1) annotation (Line(
            points={{-10,98},{-8,98},{-8,57.44},{-7,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_12) annotation (Line(
            points={{4,94},{4,58.496},{1.33333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_7) annotation (Line(
            points={{124,94},{108,94},{108,58.496},{101.333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket1) annotation (Line(
            points={{138,98},{128,98},{128,57.44},{116.333,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_a, full_hull_3DOF_6w_3g_no_speed_sensors.frame_a) annotation (
            Line(
            points={{-200,60},{-66,60},{-66,18.72},{68,18.72}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(ground, full_hull_3DOF_6w_3g_no_speed_sensors.pin_n) annotation (Line(
            points={{-200,-40},{-144,-40},{-144,15.2},{-12,15.2}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.driver_bus, driver_bus)
          annotation (Line(
            points={{-12,28.928},{-26,28.928},{-26,30},{-40,30},{-40,140},{0,140}},
            color={170,213,255},
            thickness=0.5,
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(brg_RHS1, full_hull_3DOF_6w_3g_no_speed_sensors.left_power_in)
          annotation (Line(
            points={{-200,100},{-50,100},{-50,36.32},{101.333,36.32}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.right_power_in, brg_LHS1)
          annotation (Line(
            points={{101.333,1.12},{-50.3335,1.12},{-50.3335,-100},{-200,-100}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-200,
                  -140},{200,140}}), graphics), Icon(coordinateSystem(extent={{-200,
                  -140},{200,140}}, preserveAspectRatio=true), graphics={Text(
                extent={{-88,-52},{94,-106}},
                lineColor={0,0,255},
                textString="2 wheels")}));
      end Full_hull_and_suspension_2w;

      model Full_hull_and_suspension_4w
      extends
        C2M2L_CyPhy.Extra_Components.BaseComponents.Suspension_3D_interface;
        inner Testing_Blocks.Hull.Full_hull_3DOF_4w_0g_no_speed_sensors
          full_hull_3DOF_6w_3g_no_speed_sensors(susp_offset=0, m_hull=5000)
          annotation (Placement(transformation(extent={{-12,-20},{148,68}})));
        Testing_Blocks.Hull.Suspension_Full_Container_4_Wheels
          suspension_Full_Container_2_Wheels
          annotation (Placement(transformation(extent={{-24,-116},{152,-52}})));
        Testing_Blocks.Hull.Suspension_Full_Container_4_Wheels
          suspension_Full_Container_2_Wheels1
          annotation (Placement(transformation(extent={{-24,140},{152,76}})));
      equation

        connect(suspension_Full_Container_2_Wheels.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket) annotation (Line(
            points={{138,-74},{138,-20},{116.333,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler) annotation (Line(
            points={{-10,-74},{-10,-20},{-7,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_6) annotation (Line(
            points={{4,-70},{4,-20.88},{4.33333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_1) annotation (Line(
            points={{124,-70},{124,-20.88},{101.333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler1) annotation (Line(
            points={{-10,98},{-8,98},{-8,57.44},{-7,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_12) annotation (Line(
            points={{4,94},{4,58.496},{3.33333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_7) annotation (Line(
            points={{124,94},{108,94},{108,58.496},{101.333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket1) annotation (Line(
            points={{138,98},{128,98},{128,57.44},{116.333,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_a, full_hull_3DOF_6w_3g_no_speed_sensors.frame_a) annotation (
            Line(
            points={{-200,60},{-66,60},{-66,18.72},{68,18.72}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(ground, full_hull_3DOF_6w_3g_no_speed_sensors.pin_n) annotation (Line(
            points={{-200,-40},{-144,-40},{-144,15.2},{-12,15.2}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_4,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_10) annotation (Line(
            points={{50.4,93.6},{50.4,75.8},{41.6667,75.8},{41.6667,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_9) annotation (Line(
            points={{76.4,93.2},{76.4,75.6},{65.3333,75.6},{65.3333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_4,
          suspension_Full_Container_2_Wheels.frame_susp_4) annotation (Line(
            points={{40.6667,-20.88},{40.6667,-45.44},{50.4,-45.44},{50.4,-69.6}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_3) annotation (Line(
            points={{76.4,-69.2},{76.4,-44.6},{63.6667,-44.6},{63.6667,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.driver_bus, driver_bus)
          annotation (Line(
            points={{-12.3333,27.52},{-26,27.52},{-26,26},{-40,26},{-40,140},{0,140}},
            color={170,213,255},
            thickness=0.5,
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(brg_RHS1, full_hull_3DOF_6w_3g_no_speed_sensors.left_power_in)
          annotation (Line(
            points={{-200,100},{-50,100},{-50,36.32},{101.333,36.32}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(brg_LHS1, full_hull_3DOF_6w_3g_no_speed_sensors.right_power_in)
          annotation (Line(
            points={{-200,-100},{-48,-100},{-48,1.12},{101.333,1.12}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-200,
                  -140},{200,140}}), graphics), Icon(coordinateSystem(extent={{-200,
                  -140},{200,140}}, preserveAspectRatio=true), graphics={Text(
                extent={{-90,-52},{92,-106}},
                lineColor={0,0,255},
                textString="4 wheels")}));
      end Full_hull_and_suspension_4w;

      model Full_hull_and_suspension_4w_1g
      extends
        C2M2L_CyPhy.Extra_Components.BaseComponents.Suspension_3D_interface;
        inner Testing_Blocks.Hull.Full_hull_3DOF_4w_1g_no_speed_sensors
          full_hull_3DOF_6w_3g_no_speed_sensors(susp_offset=0, m_hull=5000)
          annotation (Placement(transformation(extent={{-12,-20},{148,68}})));
        Testing_Blocks.Hull.Suspension_Full_Container_4_Wheels_1_guide
          suspension_Full_Container_2_Wheels
          annotation (Placement(transformation(extent={{-24,-116},{152,-52}})));
        Testing_Blocks.Hull.Suspension_Full_Container_4_Wheels_1_guide
          suspension_Full_Container_2_Wheels1
          annotation (Placement(transformation(extent={{-24,140},{152,76}})));
      equation

        connect(suspension_Full_Container_2_Wheels.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket) annotation (Line(
            points={{138,-74},{138,-20},{116.333,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler) annotation (Line(
            points={{-10,-74},{-10,-20},{-7,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_6) annotation (Line(
            points={{4,-70},{4,-20.88},{4.33333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_1) annotation (Line(
            points={{124,-70},{124,-20.88},{101.333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler1) annotation (Line(
            points={{-10,98},{-8,98},{-8,57.44},{-7,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_12) annotation (Line(
            points={{4,94},{4,58.496},{3.33333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_7) annotation (Line(
            points={{124,94},{108,94},{108,58.496},{101.333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket1) annotation (Line(
            points={{138,98},{128,98},{128,57.44},{116.333,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_a, full_hull_3DOF_6w_3g_no_speed_sensors.frame_a) annotation (
            Line(
            points={{-200,60},{-66,60},{-66,18.72},{68,18.72}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(ground, full_hull_3DOF_6w_3g_no_speed_sensors.pin_n) annotation (Line(
            points={{-200,-40},{-144,-40},{-144,15.2},{-12,15.2}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_4,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_10) annotation (Line(
            points={{50.4,93.6},{50.4,75.8},{41.6667,75.8},{41.6667,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_9) annotation (Line(
            points={{76.4,93.2},{76.4,75.6},{65.3333,75.6},{65.3333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_4,
          suspension_Full_Container_2_Wheels.frame_susp_4) annotation (Line(
            points={{40.6667,-20.88},{40.6667,-45.44},{50.4,-45.44},{50.4,-69.6}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_3) annotation (Line(
            points={{76.4,-69.2},{76.4,-44.6},{63.6667,-44.6},{63.6667,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.driver_bus, driver_bus)
          annotation (Line(
            points={{-12.3333,27.52},{-26,27.52},{-26,26},{-40,26},{-40,140},{0,140}},
            color={170,213,255},
            thickness=0.5,
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(suspension_Full_Container_2_Wheels.guide_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.guide_2) annotation (Line(
            points={{64,-62},{58,-62},{58,-12.96},{51.3333,-12.96}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.guide_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.guide_5) annotation (Line(
            points={{64,86},{58,86},{58,50.4},{51.3333,50.4}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(brg_RHS1, full_hull_3DOF_6w_3g_no_speed_sensors.left_power_in)
          annotation (Line(
            points={{-200,100},{-50,100},{-50,36.32},{101.333,36.32}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.right_power_in, brg_LHS1)
          annotation (Line(
            points={{101.333,1.12},{-49.3335,1.12},{-49.3335,-100},{-200,-100}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-200,
                  -140},{200,140}}), graphics), Icon(coordinateSystem(extent={{-200,
                  -140},{200,140}}, preserveAspectRatio=true), graphics={Text(
                extent={{-90,-52},{92,-106}},
                lineColor={0,0,255},
                textString="4 wheels"),                                  Text(
                extent={{-90,-92},{92,-146}},
                lineColor={0,0,255},
                textString="1 guide")}));
      end Full_hull_and_suspension_4w_1g;

      model Full_hull_and_suspension_6w
      extends
        C2M2L_CyPhy.Extra_Components.BaseComponents.Suspension_3D_interface;
        inner Testing_Blocks.Hull.Full_hull_3DOF_6w_0g_no_speed_sensors
          full_hull_3DOF_6w_3g_no_speed_sensors(susp_offset=0, m_hull=5000)
          annotation (Placement(transformation(extent={{-12,-20},{148,68}})));
        Testing_Blocks.Hull.Suspension_Full_Container_6_Wheels
          suspension_Full_Container_2_Wheels
          annotation (Placement(transformation(extent={{-24,-116},{152,-52}})));
        Testing_Blocks.Hull.Suspension_Full_Container_6_Wheels
          suspension_Full_Container_2_Wheels1
          annotation (Placement(transformation(extent={{-24,140},{152,76}})));
      equation

        connect(suspension_Full_Container_2_Wheels.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket) annotation (Line(
            points={{138,-74},{138,-20},{116.333,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler) annotation (Line(
            points={{-10,-74},{-10,-20},{-7,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_6) annotation (Line(
            points={{4,-70},{4,-20.88},{4.33333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_1) annotation (Line(
            points={{124,-70},{124,-20.88},{101.333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler1) annotation (Line(
            points={{-10,98},{-8,98},{-8,57.44},{-7,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_12) annotation (Line(
            points={{4,94},{4,58.496},{3.33333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_7) annotation (Line(
            points={{124,94},{108,94},{108,58.496},{101.333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket1) annotation (Line(
            points={{138,98},{128,98},{128,57.44},{116.333,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_5,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_5) annotation (Line(
            points={{25.2,-69.6},{25.2,-44.8},{24,-44.8},{24,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_4,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_4) annotation (Line(
            points={{47.6,-69.6},{47.6,-45.8},{40.6667,-45.8},{40.6667,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_3) annotation (Line(
            points={{78.8,-69.2},{78.8,-45.6},{63.6667,-45.6},{63.6667,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_2) annotation (Line(
            points={{102.4,-69.2},{102.4,-45.6},{84,-45.6},{84,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_9) annotation (Line(
            points={{78.8,93.2},{78.8,75.6},{65.3333,75.6},{65.3333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_8) annotation (Line(
            points={{102.4,93.2},{102.4,76.6},{81.3333,76.6},{81.3333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_4,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_10) annotation (Line(
            points={{47.6,93.6},{47.6,75.8},{41.6667,75.8},{41.6667,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_5,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_11) annotation (Line(
            points={{25.2,93.6},{25.2,76.8},{21.6667,76.8},{21.6667,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_a, full_hull_3DOF_6w_3g_no_speed_sensors.frame_a) annotation (
            Line(
            points={{-200,60},{-66,60},{-66,18.72},{68,18.72}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(ground, full_hull_3DOF_6w_3g_no_speed_sensors.pin_n) annotation (Line(
            points={{-200,-40},{-144,-40},{-144,15.2},{-12,15.2}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.driver_bus, driver_bus)
          annotation (Line(
            points={{-12,29.28},{-26,29.28},{-26,140},{0,140}},
            color={170,213,255},
            thickness=0.5,
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.right_power_in, brg_LHS1)
          annotation (Line(
            points={{101.333,1.12},{-49.3335,1.12},{-49.3335,-100},{-200,-100}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(brg_RHS1, full_hull_3DOF_6w_3g_no_speed_sensors.left_power_in)
          annotation (Line(
            points={{-200,100},{-50,100},{-50,36.32},{101.333,36.32}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-200,
                  -140},{200,140}}), graphics), Icon(coordinateSystem(extent={{-200,
                  -140},{200,140}}, preserveAspectRatio=true), graphics={Text(
                extent={{-98,-46},{84,-100}},
                lineColor={0,0,255},
                textString="6 wheels")}));
      end Full_hull_and_suspension_6w;

      model Full_hull_and_suspension_6w_3g
      extends
        C2M2L_CyPhy.Extra_Components.BaseComponents.Suspension_3D_interface;
        inner Testing_Blocks.Hull.Full_hull_3DOF_6w_3g_no_speed_sensors
          full_hull_3DOF_6w_3g_no_speed_sensors(susp_offset=0, m_hull=5000)
          annotation (Placement(transformation(extent={{-12,-20},{148,68}})));
        Testing_Blocks.Hull.Suspension_Full_Container_6_Wheels_3_guides
          suspension_Full_Container_2_Wheels
          annotation (Placement(transformation(extent={{-24,-114},{152,-50}})));
        Testing_Blocks.Hull.Suspension_Full_Container_6_Wheels_3_guides
          suspension_Full_Container_2_Wheels1
          annotation (Placement(transformation(extent={{-24,140},{152,76}})));
      equation

        connect(suspension_Full_Container_2_Wheels.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket) annotation (Line(
            points={{138,-72},{138,-20},{116.333,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler) annotation (Line(
            points={{-10,-72},{-10,-20},{-7,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_6) annotation (Line(
            points={{4,-68},{4,-20.88},{4.33333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_1) annotation (Line(
            points={{124,-68},{124,-20.88},{101.333,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler1) annotation (Line(
            points={{-10,98},{-8,98},{-8,57.44},{-7,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_12) annotation (Line(
            points={{4,94},{4,58.496},{3.33333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_7) annotation (Line(
            points={{124,94},{108,94},{108,58.496},{101.333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket1) annotation (Line(
            points={{138,98},{128,98},{128,57.44},{116.333,57.44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_5,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_5) annotation (Line(
            points={{25.2,-67.6},{25.2,-44.8},{24,-44.8},{24,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_4,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_4) annotation (Line(
            points={{47.6,-67.6},{47.6,-45.8},{40.6667,-45.8},{40.6667,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_3) annotation (Line(
            points={{78.8,-67.2},{78.8,-45.6},{63.6667,-45.6},{63.6667,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_2) annotation (Line(
            points={{102.4,-67.2},{102.4,-45.6},{84,-45.6},{84,-20.88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_9) annotation (Line(
            points={{78.8,93.2},{78.8,75.6},{65.3333,75.6},{65.3333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_8) annotation (Line(
            points={{102.4,93.2},{102.4,76.6},{81.3333,76.6},{81.3333,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_4,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_10) annotation (Line(
            points={{47.6,93.6},{47.6,75.8},{41.6667,75.8},{41.6667,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_5,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_11) annotation (Line(
            points={{25.2,93.6},{25.2,76.8},{21.6667,76.8},{21.6667,58.496}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_a, full_hull_3DOF_6w_3g_no_speed_sensors.frame_a) annotation (
            Line(
            points={{-200,60},{-66,60},{-66,18.72},{68,18.72}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(ground, full_hull_3DOF_6w_3g_no_speed_sensors.pin_n) annotation (Line(
            points={{-200,-40},{-144,-40},{-144,15.2},{-12,15.2}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.driver_bus, driver_bus)
          annotation (Line(
            points={{-12,29.28},{-26,29.28},{-26,140},{0,140}},
            color={170,213,255},
            thickness=0.5,
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.guide_3,
          suspension_Full_Container_2_Wheels.guide_3) annotation (Line(
            points={{9.66667,-12.96},{9.66667,-36.48},{14.4,-36.48},{14.4,-60.4}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.guide_2,
          suspension_Full_Container_2_Wheels.guide_2) annotation (Line(
            points={{51.3333,-12.96},{51.3333,-36.48},{66.8,-36.48},{66.8,-60.4}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.guide_1,
          suspension_Full_Container_2_Wheels.guide_1) annotation (Line(
            points={{93,-12.96},{93,-37.48},{112.8,-37.48},{112.8,-60}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.guide_4,
          suspension_Full_Container_2_Wheels1.guide_1) annotation (Line(
            points={{93,50.4},{93,68.2},{112.8,68.2},{112.8,86}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.guide_5,
          suspension_Full_Container_2_Wheels1.guide_2) annotation (Line(
            points={{51.3333,50.4},{51.3333,68.2},{66.8,68.2},{66.8,86.4}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.guide_6,
          suspension_Full_Container_2_Wheels1.guide_3) annotation (Line(
            points={{9.66667,50.4},{9.66667,68.2},{14.4,68.2},{14.4,86.4}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(brg_LHS1, full_hull_3DOF_6w_3g_no_speed_sensors.right_power_in)
          annotation (Line(
            points={{-200,-100},{-48,-100},{-48,1.12},{101.333,1.12}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.left_power_in, brg_RHS1)
          annotation (Line(
            points={{101.333,36.32},{-47.3335,36.32},{-47.3335,100},{-200,100}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-200,
                  -140},{200,140}}), graphics), Icon(coordinateSystem(extent={{-200,
                  -140},{200,140}}, preserveAspectRatio=true), graphics={Text(
                extent={{-98,-46},{84,-100}},
                lineColor={0,0,255},
                textString="6 wheels"),                                  Text(
                extent={{-98,-84},{84,-138}},
                lineColor={0,0,255},
                textString="3 guides")}));
      end Full_hull_and_suspension_6w_3g;
    end Suspension_3D_interface;
  end Extra_Components;

  partial package Icons
  extends Modelica.Icons.Package;

    partial model Component_Assembly_200x200

      annotation (Icon(coordinateSystem(preserveAspectRatio=true,  extent={{-200,
                -200},{200,200}}), graphics={
                                 Polygon(
              points={{-240,200},{-240,240},{-200,240},{-240,200}},
              smooth=Smooth.None,
              fillColor={0,127,0},
              fillPattern=FillPattern.Solid,
              pattern=LinePattern.None,
              lineColor={0,127,0}),      Polygon(
              points={{-30,-30},{-30,10},{10,10},{-30,-30}},
              smooth=Smooth.None,
              fillColor={0,127,0},
              fillPattern=FillPattern.Solid,
              pattern=LinePattern.None,
              origin={210,-230},
              rotation=180,
              lineColor={0,127,0}),
            Rectangle(
              extent={{-240,300},{240,252}},
              lineColor={0,127,0},
              fillColor={0,127,0},
              fillPattern=FillPattern.Solid),
            Text(
              extent={{-240,290},{242,262}},
              lineColor={255,255,255},
              textString="%name",
              fontName="Comic Sans MS",
              textStyle={TextStyle.Bold}),
                                    Rectangle(extent={{-200,200},{200,-200}},
                lineColor={0,0,0},
              fillColor={250,250,255},
              fillPattern=FillPattern.Solid)}), Diagram(coordinateSystem(
              preserveAspectRatio=true,  extent={{-200,-200},{200,200}})));
    end Component_Assembly_200x200;

    partial model Component_Assembly_300x300

      annotation (Icon(coordinateSystem(preserveAspectRatio=true,  extent={{-300,
                -300},{300,300}}), graphics={
                                 Polygon(
              points={{-340,300},{-340,340},{-300,340},{-340,300}},
              smooth=Smooth.None,
              fillColor={0,127,0},
              fillPattern=FillPattern.Solid,
              pattern=LinePattern.None,
              lineColor={0,127,0}),      Polygon(
              points={{-30,-30},{-30,10},{10,10},{-30,-30}},
              smooth=Smooth.None,
              fillColor={0,127,0},
              fillPattern=FillPattern.Solid,
              pattern=LinePattern.None,
              origin={310,-330},
              rotation=180,
              lineColor={0,127,0}),
            Rectangle(
              extent={{-340,400},{340,358}},
              lineColor={0,127,0},
              fillColor={0,127,0},
              fillPattern=FillPattern.Solid),
            Text(
              extent={{-340,394},{340,364}},
              lineColor={255,255,255},
              textString="%name",
              fontName="Comic Sans MS",
              textStyle={TextStyle.Bold}),
                                    Rectangle(extent={{-300,302},{300,-302}},
                lineColor={0,0,0},
              fillColor={250,250,255},
              fillPattern=FillPattern.Solid)}), Diagram(coordinateSystem(
              preserveAspectRatio=true,  extent={{-300,-300},{300,300}})));
    end Component_Assembly_300x300;

    partial model Component_Assembly_400x300

      annotation (Icon(coordinateSystem(preserveAspectRatio=true,  extent={{-400,
                -300},{400,300}}), graphics={
                                 Polygon(
              points={{-440,302},{-440,342},{-400,342},{-440,302}},
              smooth=Smooth.None,
              fillColor={0,127,0},
              fillPattern=FillPattern.Solid,
              pattern=LinePattern.None,
              lineColor={0,127,0}),      Polygon(
              points={{-30,-30},{-30,10},{10,10},{-30,-30}},
              smooth=Smooth.None,
              fillColor={0,127,0},
              fillPattern=FillPattern.Solid,
              pattern=LinePattern.None,
              origin={410,-328},
              rotation=180,
              lineColor={0,127,0}),
            Rectangle(
              extent={{-440,400},{442,356}},
              lineColor={0,127,0},
              fillColor={0,127,0},
              fillPattern=FillPattern.Solid),
            Text(
              extent={{-340,394},{340,364}},
              lineColor={255,255,255},
              textString="%name",
              fontName="Comic Sans MS",
              textStyle={TextStyle.Bold}),
                                    Rectangle(extent={{-400,302},{400,-300}},
                lineColor={0,0,0},
              fillColor={250,250,255},
              fillPattern=FillPattern.Solid)}), Diagram(coordinateSystem(
              preserveAspectRatio=true,  extent={{-400,-300},{400,300}})));
    end Component_Assembly_400x300;

    partial model Component_Assembly_400x400

      annotation (Icon(coordinateSystem(preserveAspectRatio=true,  extent={{-400,
                -400},{400,400}}), graphics={
                                 Polygon(
              points={{-440,400},{-440,440},{-400,440},{-440,400}},
              smooth=Smooth.None,
              fillColor={0,127,0},
              fillPattern=FillPattern.Solid,
              pattern=LinePattern.None,
              lineColor={0,127,0}),      Polygon(
              points={{-30,-30},{-30,10},{10,10},{-30,-30}},
              smooth=Smooth.None,
              fillColor={0,127,0},
              fillPattern=FillPattern.Solid,
              pattern=LinePattern.None,
              origin={410,-428},
              rotation=180,
              lineColor={0,127,0}),
            Rectangle(
              extent={{-440,500},{442,450}},
              lineColor={0,127,0},
              fillColor={0,127,0},
              fillPattern=FillPattern.Solid),
            Text(
              extent={{-440,500},{442,452}},
              lineColor={255,255,255},
              textString="%name",
              fontName="Comic Sans MS",
              textStyle={TextStyle.Bold}),
                                    Rectangle(extent={{-400,402},{402,-400}},
                lineColor={0,0,0},
              fillColor={250,250,255},
              fillPattern=FillPattern.Solid)}), Diagram(coordinateSystem(
              preserveAspectRatio=true,  extent={{-400,-400},{400,400}})));
    end Component_Assembly_400x400;

    partial model Component_Assembly_500x300

      annotation (Icon(coordinateSystem(preserveAspectRatio=true,  extent={{-500,
                -300},{500,300}}), graphics={
                                 Polygon(
              points={{-540,300},{-540,340},{-500,340},{-540,300}},
              smooth=Smooth.None,
              fillColor={0,127,0},
              fillPattern=FillPattern.Solid,
              pattern=LinePattern.None,
              lineColor={0,127,0}),      Polygon(
              points={{-30,-30},{-30,10},{10,10},{-30,-30}},
              smooth=Smooth.None,
              fillColor={0,127,0},
              fillPattern=FillPattern.Solid,
              pattern=LinePattern.None,
              origin={512,-328},
              rotation=180,
              lineColor={0,127,0}),
            Rectangle(
              extent={{-540,400},{540,352}},
              lineColor={0,127,0},
              fillColor={0,127,0},
              fillPattern=FillPattern.Solid),
            Text(
              extent={{-540,400},{540,358}},
              lineColor={255,255,255},
              textString="%name",
              fontName="Comic Sans MS",
              textStyle={TextStyle.Bold}),
                                    Rectangle(extent={{-500,300},{500,-300}},
                lineColor={0,0,0},
              fillColor={250,250,255},
              fillPattern=FillPattern.Solid)}), Diagram(coordinateSystem(
              preserveAspectRatio=true,  extent={{-500,-300},{500,300}})));
    end Component_Assembly_500x300;

    model Simple_Test_Case_200x200
    "Icon for a simple test case (not a full Test_Bench)"

      annotation (Icon(coordinateSystem(preserveAspectRatio=true, extent={{-200,
                -200},{200,200}}),
                       graphics={Bitmap(extent={{-200,200},{200,-200}}, fileName=
                  "modelica://C2M2L_Ext/Resources/Interfaces/tick.png")}), Diagram(
            coordinateSystem(preserveAspectRatio=true, extent={{-200,-200},{200,200}})));
    end Simple_Test_Case_200x200;

    model Simple_Test_Case_300x300
    "Icon for a simple test case (not a full Test_Bench)"

      annotation (Icon(coordinateSystem(preserveAspectRatio=true, extent={{-300,
                -300},{300,300}}),
                       graphics={Bitmap(extent={{-300,300},{300,-302}}, fileName=
                  "modelica://C2M2L_Ext/Resources/Interfaces/tick.png")}), Diagram(
            coordinateSystem(preserveAspectRatio=true, extent={{-300,-300},{300,300}})));
    end Simple_Test_Case_300x300;

    model Simple_Test_Case_400x400
    "Icon for a simple test case (not a full Test_Bench)"

      annotation (Icon(coordinateSystem(preserveAspectRatio=true, extent={{-400,
                -400},{400,400}}),
                       graphics={Bitmap(extent={{-300,300},{300,-302}}, fileName=
                  "modelica://C2M2L_Ext/Resources/Interfaces/tick.png")}), Diagram(
            coordinateSystem(preserveAspectRatio=true, extent={{-400,-400},{400,400}})));
    end Simple_Test_Case_400x400;

    model Simple_Test_Case_500x400
    "Icon for a simple test case (not a full Test_Bench)"

      annotation (Icon(coordinateSystem(preserveAspectRatio=true, extent={{-500,
                -400},{500,400}}),
                       graphics={Bitmap(extent={{-300,300},{300,-302}}, fileName=
                  "modelica://C2M2L_Ext/Resources/Interfaces/tick.png")}), Diagram(
            coordinateSystem(preserveAspectRatio=true, extent={{-500,-400},{500,400}})));
    end Simple_Test_Case_500x400;

    partial model Test_Component_200x200

      annotation (Icon(coordinateSystem(preserveAspectRatio=true,  extent={{-200,
                -200},{200,200}}), graphics={
                                 Polygon(
              points={{-240,200},{-240,240},{-200,240},{-240,200}},
              smooth=Smooth.None,
              fillColor={213,170,255},
              fillPattern=FillPattern.Solid,
              pattern=LinePattern.None,
              lineColor={213,170,255}),  Polygon(
              points={{-30,-30},{-30,10},{10,10},{-30,-30}},
              smooth=Smooth.None,
              fillColor={213,170,255},
              fillPattern=FillPattern.Solid,
              pattern=LinePattern.None,
              origin={210,-230},
              rotation=180,
              lineColor={213,170,255}),
            Rectangle(
              extent={{-240,300},{240,252}},
              lineColor={213,170,255},
              fillColor={213,170,255},
              fillPattern=FillPattern.Solid),
            Text(
              extent={{-240,290},{242,262}},
              lineColor={255,255,255},
              textString="%name",
              fontName="Comic Sans MS",
              textStyle={TextStyle.Bold}),
                                    Rectangle(extent={{-200,200},{200,-200}},
                lineColor={0,0,0},
              fillColor={250,250,255},
              fillPattern=FillPattern.Solid)}), Diagram(coordinateSystem(
              preserveAspectRatio=true,  extent={{-200,-200},{200,200}})));
    end Test_Component_200x200;
  end Icons;

  package Test_Assemblies_Decl
    extends Modelica.Icons.Package;

    model DriveLineDesign

      replaceable
      C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Drive_Line.Cross_Drive_Transmission.Cross_Drive_with_TC_PTOSteer
        cross_Drive_with_TC_PTOSteer
        annotation (Placement(transformation(extent={{40,-32},{80,28}})));
      replaceable
      C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Drive_Line.Final_Drive.Example_Standalone_Final_Drive_Implementation
        example_Standalone_Final_Drive_Implementation annotation (Placement(
            transformation(
            extent={{20,-30},{-20,30}},
            rotation=270,
            origin={74,118})));
      replaceable
      C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Drive_Line.Final_Drive.Example_Standalone_Final_Drive_Implementation
        example_Standalone_Final_Drive_Implementation1 annotation (Placement(
            transformation(
            extent={{-20,-30},{20,30}},
            rotation=270,
            origin={74,-120})));
      replaceable
      C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Drive_Line.Drive_Shafts.Drive_Shaft
        drive_Shaft annotation (Placement(transformation(
            extent={{-8,-8},{8,8}},
            rotation=270,
            origin={74,74})));
      replaceable
      C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Drive_Line.Drive_Shafts.Drive_Shaft
        drive_Shaft1 annotation (Placement(transformation(
            extent={{-8,-8},{8,8}},
            rotation=270,
            origin={74,-70})));
      Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing brg_1(
          includeBearingConnector=world.driveTrainMechanics3D) annotation (
          Placement(transformation(
            extent={{-10,-10},{10,10}},
            rotation=90,
            origin={74,-160})));
      Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing brg_2(
          includeBearingConnector=world.driveTrainMechanics3D) annotation (
          Placement(transformation(
            extent={{-10,-10},{10,10}},
            rotation=90,
            origin={74,160})));
      replaceable
      C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Prime_Movers.Reciprocating.Compression_Ignition.Engine_Basic.Example_Engine_Basic_Tstat_Degas
        example_Engine_Basic_Tstat_Degas
        annotation (Placement(transformation(extent={{-60,-32},{-20,28}})));
      C2M2L_Decl.Testing_Blocks.Fluid_Source_Sinks.Air_Path_For_Testing
        air_Path_For_Testing
        annotation (Placement(transformation(extent={{-70,40},{-50,60}})));
      C2M2L_Decl.Testing_Blocks.Fluid_Source_Sinks.Charge_Air_Cooler_Surrogate
        charge_Air_Cooler_Surrogate
        annotation (Placement(transformation(extent={{-44,40},{-24,60}})));
      C2M2L_Decl.Testing_Blocks.Fluid_Source_Sinks.Cooling_Cart_For_Testing
        cooling_Cart_For_Testing annotation (Placement(transformation(
            extent={{-10,-10},{10,10}},
            rotation=90,
            origin={10,30})));
      C2M2L_Decl.Testing_Blocks.Fluid_Source_Sinks.Fluid_Source_Sink_Passive
        fluid_Source_Sink_Passive(redeclare package Medium =
            C2M2L_Decl.Media.Incompressible.Essotherm650_Const_Prop)
        annotation (Placement(transformation(extent={{26,40},{46,60}})));
      C2M2L_Decl.C2M2L_Component_Building_Blocks.Prime_Movers.Electrical_Controls.General_Controllers.Conventional_ECU
        conventional_ECU annotation (Placement(transformation(
            extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-14,90})));
      C2M2L_Decl.C2M2L_Component_Building_Blocks.Prime_Movers.Electrical_Controls.General_Controllers.Transmission_TCU
        transmission_TCU annotation (Placement(transformation(
            extent={{-10,-10},{10,10}},
            rotation=90,
            origin={16,90})));
      C2M2L_Decl.Interfaces.Context_Interfaces.Driver.Driver_Bus driver
        annotation (Placement(transformation(extent={{-10,150},{10,170}})));
      C2M2L_Decl.C2M2L_Component_Building_Blocks.Drive_Line.Cross_Drive_Steering.Common_Controls.Steering_Control_Bus
        steering
        annotation (Placement(transformation(extent={{10,150},{30,170}})));
      C2M2L_Decl.C2M2L_Component_Building_Blocks.Drive_Line.Brakes_Clutch.Common_Controls.Brake_Control_Bus
        brake annotation (Placement(transformation(extent={{30,150},{50,170}})));
      C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Fuel_System.Fuel_Tank
        fuel_Tank annotation (Placement(transformation(
            extent={{-9.00001,-13},{9.00001,13}},
            rotation=270,
            origin={-111,8.99999})));
      C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Electrical_System.Starting_Systems.Lead_Acid_Battery
        lead_Acid_Battery annotation (Placement(transformation(
            extent={{-10,-15},{10,15}},
            rotation=270,
            origin={-109,-30})));
      Modelica.Electrical.Analog.Basic.Ground ground
        annotation (Placement(transformation(extent={{-90,-66},{-70,-46}})));
      outer Modelica.Mechanics.MultiBody.World world
        annotation (Placement(transformation(extent={{-120,100},{-100,120}})));
      outer C2M2L_Decl.Environments.Lumped_Thermal_Fixed_HTC_Open_Volume
                                                            env
        annotation (Placement(transformation(extent={{-80,100},{-60,120}})));
    equation
      connect(drive_Shaft.frame_a, example_Standalone_Final_Drive_Implementation.brg_01)
        annotation (Line(
          points={{74,82},{74,100}},
          color={135,135,135},
          thickness=0.5,
          smooth=Smooth.None));
      connect(drive_Shaft.frame_b, cross_Drive_with_TC_PTOSteer.brg_02)
        annotation (Line(
          points={{74,66},{74,20}},
          color={135,135,135},
          thickness=0.5,
          smooth=Smooth.None));
      connect(cross_Drive_with_TC_PTOSteer.brg_03, drive_Shaft1.frame_a)
        annotation (Line(
          points={{74,-20},{74,-62}},
          color={135,135,135},
          thickness=0.5,
          smooth=Smooth.None));
      connect(drive_Shaft1.frame_b,
        example_Standalone_Final_Drive_Implementation1.brg_01) annotation (Line(
          points={{74,-78},{74,-98}},
          color={135,135,135},
          thickness=0.5,
          smooth=Smooth.None));
      connect(example_Standalone_Final_Drive_Implementation1.brg_02, brg_1)
        annotation (Line(
          points={{74,-138},{74,-160}},
          color={135,135,135},
          thickness=0.5,
          smooth=Smooth.None));
      connect(example_Standalone_Final_Drive_Implementation.brg_02, brg_2)
        annotation (Line(
          points={{74,140},{74,160}},
          color={135,135,135},
          thickness=0.5,
          smooth=Smooth.None));
      connect(example_Engine_Basic_Tstat_Degas.brg_02,
        cross_Drive_with_TC_PTOSteer.brg_01) annotation (Line(
          points={{-20,0},{40,0}},
          color={135,135,135},
          thickness=0.5,
          smooth=Smooth.None));
      connect(example_Engine_Basic_Tstat_Degas.top_hose_outlet,
        cooling_Cart_For_Testing.to_rad) annotation (Line(
          points={{-16,14},{4,14},{4,20}},
          color={0,127,255},
          smooth=Smooth.None));
      connect(cooling_Cart_For_Testing.from_rad, example_Engine_Basic_Tstat_Degas.bottom_hose_in)
        annotation (Line(
          points={{16,20},{16,-10},{-16,-10}},
          color={0,127,255},
          smooth=Smooth.None));
      connect(air_Path_For_Testing.exhaust_out, example_Engine_Basic_Tstat_Degas.fluid_out)
        annotation (Line(
          points={{-54,40},{-54,36},{-48,36},{-48,24}},
          color={0,127,255},
          smooth=Smooth.None));
      connect(example_Engine_Basic_Tstat_Degas.fluid_in, air_Path_For_Testing.intake_air)
        annotation (Line(
          points={{-56.2,24},{-56,24},{-56,36},{-66,36},{-66,40}},
          color={0,127,255},
          smooth=Smooth.None));
      connect(example_Engine_Basic_Tstat_Degas.to_cac,
        charge_Air_Cooler_Surrogate.from_engine) annotation (Line(
          points={{-52,28},{-52,34},{-40,34},{-40,40}},
          color={0,127,255},
          smooth=Smooth.None));
      connect(example_Engine_Basic_Tstat_Degas.from_cac,
        charge_Air_Cooler_Surrogate.to_engine) annotation (Line(
          points={{-44,28},{-44,32},{-28,32},{-28,40}},
          color={0,127,255},
          smooth=Smooth.None));
      connect(fluid_Source_Sink_Passive.supply_port, cross_Drive_with_TC_PTOSteer.fluid_in)
        annotation (Line(
          points={{30,40},{30,24},{43.8,24}},
          color={0,127,255},
          smooth=Smooth.None));
      connect(cross_Drive_with_TC_PTOSteer.fluid_out, fluid_Source_Sink_Passive.return_port)
        annotation (Line(
          points={{52,24},{52,32},{42,32},{42,40}},
          color={0,127,255},
          smooth=Smooth.None));
      connect(example_Engine_Basic_Tstat_Degas.engineControlBus, conventional_ECU.engine_control_bus)
        annotation (Line(
          points={{-40,20.4},{-40,30},{-20,30},{-20,79.8}},
          color={175,175,175},
          thickness=0.5,
          smooth=Smooth.None));
      connect(conventional_ECU.driver_bus, transmission_TCU.driver_bus)
        annotation (Line(
          points={{-14,80},{-14,86},{16,86},{16,80}},
          color={170,213,255},
          thickness=0.5,
          smooth=Smooth.None));
      connect(conventional_ECU.range_pack_control_bus, transmission_TCU.range_pack_control_bus)
        annotation (Line(
          points={{-8,80},{-8,74},{22,74},{22,80}},
          color={255,204,51},
          thickness=0.5,
          smooth=Smooth.None));
      connect(transmission_TCU.tcc_control_bus, cross_Drive_with_TC_PTOSteer.torque_Converter_Control_Bus)
        annotation (Line(
          points={{10,80},{10,72},{55,72},{55,20.6}},
          color={85,255,85},
          thickness=0.5,
          smooth=Smooth.None));
      connect(cross_Drive_with_TC_PTOSteer.range_Pack_Control_Bus,
        transmission_TCU.range_pack_control_bus) annotation (Line(
          points={{57.4,20.6},{57.4,74},{22,74},{22,80}},
          color={255,204,51},
          thickness=0.5,
          smooth=Smooth.None));
      connect(conventional_ECU.driver_bus, driver) annotation (Line(
          points={{-14,80},{-14,82},{0,82},{0,160}},
          color={170,213,255},
          thickness=0.5,
          smooth=Smooth.None), Text(
          string="%second",
          index=1,
          extent={{6,3},{6,3}}));
      connect(cross_Drive_with_TC_PTOSteer.steering_Control_Bus, steering)
        annotation (Line(
          points={{60,20.6},{60,78},{38,78},{38,120},{20,120},{20,160}},
          color={0,127,0},
          thickness=0.5,
          smooth=Smooth.None), Text(
          string="%second",
          index=1,
          extent={{6,3},{6,3}}));
      connect(cross_Drive_with_TC_PTOSteer.brake_Control_Bus, brake) annotation (
          Line(
          points={{62.4,20.6},{62.4,80},{40,80},{40,160}},
          color={255,85,85},
          thickness=0.5,
          smooth=Smooth.None), Text(
          string="%second",
          index=1,
          extent={{6,3},{6,3}}));
      connect(fuel_Tank.fluid_out, example_Engine_Basic_Tstat_Degas.fuel_in)
        annotation (Line(
          points={{-100.6,13.4667},{-82.3,13.4667},{-82.3,14},{-64,14}},
          color={0,127,255},
          smooth=Smooth.None));
      connect(lead_Acid_Battery.pin_p, example_Engine_Basic_Tstat_Degas.pin_p)
        annotation (Line(
          points={{-103,-19},{-103,-10},{-80,-10},{-80,10},{-60,10}},
          color={0,0,255},
          smooth=Smooth.None));
      connect(example_Engine_Basic_Tstat_Degas.pin_n, lead_Acid_Battery.pin_n)
        annotation (Line(
          points={{-60,4},{-76,4},{-76,-46},{-103,-46},{-103,-39}},
          color={0,0,255},
          smooth=Smooth.None));
      connect(example_Engine_Basic_Tstat_Degas.pin_n, ground.p) annotation (Line(
          points={{-60,4},{-76,4},{-76,-46},{-80,-46}},
          color={0,0,255},
          smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{
                -160,-160},{160,160}}), graphics), Icon(coordinateSystem(
              preserveAspectRatio=false, extent={{-160,-160},{160,160}})));
    end DriveLineDesign;

    package FullDesignWithSuspension
      extends Modelica.Icons.Package;
      model Full_hull_and_hydro_pneumatic_suspension_6w_3g
      extends
        C2M2L_CyPhy.Extra_Components.BaseComponents.Suspension_3D_interface_Decl(
            brg_RHS1(includeBearingConnector=world.driveTrainMechanics3D), brg_LHS1(
              includeBearingConnector=world.driveTrainMechanics3D));
        inner
        Testing_Blocks.Hull.Full_hull_3DOF_6w_3g_no_speed_sensors_idler_frame_Decl
          full_hull_3DOF_6w_3g_no_speed_sensors(susp_offset=0, m_hull=5000)
          annotation (Placement(transformation(extent={{-50,-48},{110,40}})));
        Testing_Blocks.Hull.Suspension_Full_Container_6_Wheels_3g_Hydro_pneumatic_wTensioner_Decl
          suspension_Full_Container_2_Wheels(is_left_side=false)
          annotation (Placement(transformation(extent={{-62,-134},{114,-70}})));
        Testing_Blocks.Hull.Suspension_Full_Container_6_Wheels_3g_Hydro_pneumatic_wTensioner_Decl
          suspension_Full_Container_2_Wheels1(is_left_side=true)
          annotation (Placement(transformation(extent={{-64,122},{112,58}})));
      equation

        connect(suspension_Full_Container_2_Wheels.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket) annotation (Line(
            points={{100,-92},{100,-70},{104,-70},{104,-48}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler) annotation (Line(
            points={{-48.8,-91.6},{-48.8,-41.4},{-44.4,-41.4}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_6) annotation (Line(
            points={{-34,-88},{-34,-49}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_1) annotation (Line(
            points={{86,-88},{86,-49}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler1) annotation (Line(
            points={{-50.8,79.6},{-58,79.6},{-58,29.8},{-42,29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_12) annotation (Line(
            points={{-36,76},{-36,41.2},{-34,41.2}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_7) annotation (Line(
            points={{84,76},{84,62},{86,62},{86,41.2}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket1) annotation (Line(
            points={{98,80},{98,40},{104,40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_5,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_5) annotation (Line(
            points={{-12.8,-87.6},{-12.8,-68},{-14,-68},{-14,-49}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_4,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_4) annotation (Line(
            points={{9.6,-87.6},{9.6,-68},{6,-68},{6,-49}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_3) annotation (Line(
            points={{40.8,-87.2},{40.8,-68},{46,-68},{46,-49}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_2) annotation (Line(
            points={{64.4,-87.2},{64.4,-68},{66,-68},{66,-49}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_9) annotation (Line(
            points={{38.8,75.2},{38.8,58},{46,58},{46,41.2}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_8) annotation (Line(
            points={{62.4,75.2},{62.4,58},{66,58},{66,41.2}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_4,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_10) annotation (Line(
            points={{7.6,75.6},{7.6,58},{6,58},{6,41.2}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_5,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_11) annotation (Line(
            points={{-14.8,75.6},{-14.8,58},{-14,58},{-14,41.2}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(brg_RHS1, full_hull_3DOF_6w_3g_no_speed_sensors.left_power_in)
          annotation (Line(
            points={{-200,100},{-140,100},{-140,16},{86,16}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_a, full_hull_3DOF_6w_3g_no_speed_sensors.frame_a) annotation (
            Line(
            points={{-200,60},{-160,60},{-160,-4},{46,-4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.pin_n, ground) annotation (Line(
            points={{-50,-8},{-122,-8},{-122,-40},{-200,-40}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.right_power_in, brg_LHS1)
          annotation (Line(
            points={{86,-24},{-108,-24},{-108,-100},{-200,-100}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.driver_bus, driver_bus)
          annotation (Line(
            points={{-50,8},{-80,8},{-80,128},{0,128},{0,140}},
            color={170,213,255},
            thickness=0.5,
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(suspension_Full_Container_2_Wheels1.guide_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.guide_6) annotation (Line(
            points={{-25.6,68.4},{-25.6,50.2},{-24,50.2},{-24,32}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.guide_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.guide_5) annotation (Line(
            points={{26.8,68.4},{26.8,50.2},{26,50.2},{26,32}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.guide_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.guide_4) annotation (Line(
            points={{72.8,68},{72.8,49},{76,49},{76,32}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.guide_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.guide_3) annotation (Line(
            points={{-23.6,-80.4},{-23.6,-60.2},{-24,-60.2},{-24,-40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.guide_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.guide_2) annotation (Line(
            points={{28.8,-80.4},{28.8,-60.2},{26,-60.2},{26,-40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.guide_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.guide_1) annotation (Line(
            points={{74.8,-80},{76,-80},{76,-40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-200,
                  -140},{200,140}}), graphics), Icon(coordinateSystem(extent={{-200,
                  -140},{200,140}}, preserveAspectRatio=false),graphics={Text(
                extent={{-98,-46},{84,-100}},
                lineColor={0,0,255},
                textString="6 wheels"),                                  Text(
                extent={{-98,-84},{84,-138}},
                lineColor={0,0,255},
                textString="3 guides"),
              Text(
                extent={{-160,60},{160,20}},
                lineColor={0,127,0},
                textString="Hydro pneumatic")}));
      end Full_hull_and_hydro_pneumatic_suspension_6w_3g;

      model FullSystemDesign_hydro_pneumatic_6w_3g

        Hull_and_Susp                  full_hull_and_suspension_6w_3g(
          compression_scale=0,
          rebound_scale=0,
          spring_scale=0)
          annotation (Placement(transformation(extent={{14,-30},{100,30}})));
        DriveLineDesign driveLineDesign
          annotation (Placement(transformation(extent={{-80,-20},{-40,20}})));
        C2M2L_Decl.C2M2L_Component_Building_Blocks.Drive_Line.Brakes_Clutch.Common_Controls.Brake_Control_Bus
          brake1 annotation (Placement(transformation(extent={{-64,86},{-44,106}})));
        C2M2L_Decl.C2M2L_Component_Building_Blocks.Drive_Line.Cross_Drive_Steering.Common_Controls.Steering_Control_Bus
          steering1
          annotation (Placement(transformation(extent={{-86,88},{-66,108}})));
        C2M2L_Decl.Interfaces.Context_Interfaces.Driver.Driver_Bus driver1
          annotation (Placement(transformation(extent={{-10,90},{10,110}})));
      equation
        connect(driveLineDesign.brg_2, full_hull_and_suspension_6w_3g.brg_RHS1)
          annotation (Line(
            points={{-50.75,20},{-50,20},{-50,21.4286},{14,21.4286}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(driveLineDesign.brg_1, full_hull_and_suspension_6w_3g.brg_LHS1)
          annotation (Line(
            points={{-50.75,-20},{-50,-20},{-50,-21.4286},{14,-21.4286}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(driveLineDesign.brake, brake1) annotation (Line(
            points={{-55,20},{-54,20},{-54,96}},
            color={255,85,85},
            thickness=0.5,
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(driveLineDesign.steering, steering1) annotation (Line(
            points={{-57.5,20},{-58,20},{-58,80},{-76,80},{-76,98}},
            color={0,127,0},
            thickness=0.5,
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(driveLineDesign.driver, driver1) annotation (Line(
            points={{-60,20},{-60,72},{0,72},{0,100}},
            color={170,213,255},
            thickness=0.5,
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(full_hull_and_suspension_6w_3g.driver_bus, driver1) annotation (Line(
            points={{57,30},{56,30},{56,72},{0,72},{0,100}},
            color={170,213,255},
            thickness=0.5,
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
                  -100},{100,100}}), graphics), Icon(graphics));
      end FullSystemDesign_hydro_pneumatic_6w_3g;

      model FullSystemTest_hydro_pneumatic_6w_3g
        extends Modelica.Icons.Example;
        inner C2M2L_Decl.Testing_Blocks.Land.Terrains.Rough_Ground
                                                                ground_context(
          wheel_rad=0.3,
          show_trace=true,
          show_road=true)
          annotation (Placement(transformation(extent={{-100,-60},{-80,-40}})));
        inner C2M2L_Decl.Environments.Lumped_Thermal_Fixed_HTC_Open_Volume
                                                              env(
          fixed_htc=100,
          redeclare package Medium =
              Modelica.Media.Incompressible.Examples.Essotherm650)
          annotation (Placement(transformation(extent={{-20,-100},{0,-80}})));
        inner Modelica.Fluid.System system
          annotation (Placement(transformation(extent={{-60,-100},{-40,-80}})));
        inner Modelica.Mechanics.MultiBody.World world(
          gravityType=Modelica.Mechanics.MultiBody.Types.GravityTypes.UniformGravity,
          driveTrainMechanics3D=false,
          enableAnimation=true)
          annotation (Placement(transformation(extent={{-100,-100},{-80,-80}})));
        replaceable Testing_Blocks.Land_Decl.FTP_Driver driver_Base_Land constrainedby
        C2M2L_Decl.Testing_Blocks.Land.Driver_Base_Land
          annotation (Placement(transformation(extent={{-50,42},{-30,62}})));
        FullSystemDesign_hydro_pneumatic_6w_3g fullSystemDesign_hydro_pneumatic_6w_3g
          annotation (Placement(transformation(extent={{-30,-40},{30,20}})));
      equation
        connect(driver_Base_Land.driver_bus, fullSystemDesign_hydro_pneumatic_6w_3g.driver1)
          annotation (Line(
            points={{-30,52},{0,52},{0,20}},
            color={170,213,255},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fullSystemDesign_hydro_pneumatic_6w_3g.brake1, driver_Base_Land.brake_Control_Bus)
          annotation (Line(
            points={{-16.2,18.8},{-16.2,48},{-30,48}},
            color={255,85,85},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fullSystemDesign_hydro_pneumatic_6w_3g.steering1, driver_Base_Land.steering_Control_Bus)
          annotation (Line(
            points={{-22.8,19.4},{-22.8,44},{-30,44}},
            color={0,127,0},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (
          Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,-100},{
                  100,100}}), graphics),
          experiment(StopTime=100, __Dymola_Algorithm="Esdirk45a"),
          __Dymola_experimentSetupOutput);
      end FullSystemTest_hydro_pneumatic_6w_3g;

      model SuspensionTest_hydro_pneumatic_6w_3g
        extends Modelica.Icons.Example;
        Full_hull_and_hydro_pneumatic_suspension_6w_3g
                                       full_hull_and_suspension_6w_3g
          annotation (Placement(transformation(extent={{22,-26},{100,28}})));
        inner Modelica.Mechanics.MultiBody.World world(gravityType=Modelica.Mechanics.MultiBody.Types.GravityTypes.UniformGravity,
            enableAnimation=true)
          annotation (Placement(transformation(extent={{-100,-100},{-80,-80}})));
        inner C2M2L_Decl.Testing_Blocks.Land.Terrains.Ramp_60per ground_context(
          show_trace=true,
          show_road=true,
          wheel_rad=0.3,
          samples=0,
          road=[-1.7,0.0; 0,0.0; 5,0.5; 7,0.5])
          annotation (Placement(transformation(extent={{-100,-60},{-80,-40}})));
        Modelica.Blocks.Sources.Trapezoid torque(
          offset=0,
          rising=1,
          period=20,
          nperiod=1,
          width=8,
          amplitude=-800,
          startTime=0)
          annotation (Placement(transformation(extent={{-80,-28},{-60,-8}})));
        Modelica.Mechanics.Rotational.Sources.Torque torque2
          annotation (Placement(transformation(extent={{-48,-28},{-28,-8}})));
        Modelica.Blocks.Sources.Trapezoid torque1(
          offset=0,
          rising=1,
          period=20,
          nperiod=1,
          width=8,
          amplitude=800,
          startTime=0)
          annotation (Placement(transformation(extent={{-80,10},{-60,30}})));
        Modelica.Mechanics.Rotational.Sources.Torque torque3
          annotation (Placement(transformation(extent={{-48,10},{-28,30}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing flange(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{-24,-28},{-4,-8}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing flange1(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{-24,10},{-4,30}})));
        inner Modelica.Fluid.System system
          annotation (Placement(transformation(extent={{-60,-100},{-40,-80}})));
      equation
        connect(torque2.flange,flange. flange) annotation (Line(
            points={{-28,-18},{-14,-18}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torque.y,torque2. tau) annotation (Line(
            points={{-59,-18},{-50,-18}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torque3.flange,flange1. flange)
                                               annotation (Line(
            points={{-28,20},{-14,20}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torque1.y,torque3. tau)
                                       annotation (Line(
            points={{-59,20},{-50,20}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(flange1, full_hull_and_suspension_6w_3g.brg_RHS1) annotation (Line(
            points={{-14,20},{20,20},{20,20.2857},{22,20.2857}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(flange, full_hull_and_suspension_6w_3g.brg_LHS1) annotation (Line(
            points={{-14,-18},{16,-18},{16,-18.2857},{22,-18.2857}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(flange, flange) annotation (Line(
            points={{-14,-18},{-14,-18}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
                  -100},{100,100}}), graphics),
          experiment(StopTime=100, __Dymola_Algorithm="Esdirk45a"),
          __Dymola_experimentSetupOutput);
      end SuspensionTest_hydro_pneumatic_6w_3g;

      model Hull_and_Susp
      extends
        C2M2L_CyPhy.Extra_Components.BaseComponents.Suspension_3D_interface_Decl(
            brg_RHS1(includeBearingConnector=world.driveTrainMechanics3D), brg_LHS1(
              includeBearingConnector=world.driveTrainMechanics3D));
        parameter Real compression_scale = 1.0;
        parameter Real rebound_scale = 0.6;
        parameter Real spring_scale = 1.0;

        inner
        Testing_Blocks.Hull.Full_hull_3DOF_6w_3g_no_speed_sensors_idler_frame_Decl
          full_hull_3DOF_6w_3g_no_speed_sensors(susp_offset=0, m_hull=5000)
          annotation (Placement(transformation(extent={{-50,-48},{110,40}})));
        Testing_Blocks.Hull.Susp_Hydro_Pneumatic_6_Wheels
          suspension_Full_Container_2_Wheels(
          is_left_side=false,
          compression_scale=compression_scale,
          rebound_scale=rebound_scale,
          spring_scale=spring_scale)
          annotation (Placement(transformation(extent={{-62,-134},{114,-70}})));
        Testing_Blocks.Hull.Susp_Hydro_Pneumatic_6_Wheels
          suspension_Full_Container_2_Wheels1(
          is_left_side=true,
          compression_scale=compression_scale,
          rebound_scale=rebound_scale,
          spring_scale=spring_scale)
          annotation (Placement(transformation(extent={{-64,122},{112,58}})));
      equation

        connect(suspension_Full_Container_2_Wheels.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket) annotation (Line(
            points={{100,-92},{100,-70},{104,-70},{104,-48}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler) annotation (Line(
            points={{-48.8,-91.6},{-48.8,-41.4},{-44.4,-41.4}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_6) annotation (Line(
            points={{-34,-88},{-34,-49}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_1) annotation (Line(
            points={{86,-88},{86,-49}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.idler,
          full_hull_3DOF_6w_3g_no_speed_sensors.idler1) annotation (Line(
            points={{-50.8,79.6},{-58,79.6},{-58,29.8},{-42,29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_6,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_12) annotation (Line(
            points={{-36,76},{-36,41.2},{-34,41.2}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_7) annotation (Line(
            points={{84,76},{84,62},{86,62},{86,41.2}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.sprocket,
          full_hull_3DOF_6w_3g_no_speed_sensors.sprocket1) annotation (Line(
            points={{98,80},{98,40},{104,40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_5,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_5) annotation (Line(
            points={{-12.8,-87.6},{-12.8,-68},{-14,-68},{-14,-49}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_4,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_4) annotation (Line(
            points={{9.6,-87.6},{9.6,-68},{6,-68},{6,-49}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_3) annotation (Line(
            points={{40.8,-87.2},{40.8,-68},{46,-68},{46,-49}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.frame_susp_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_2) annotation (Line(
            points={{64.4,-87.2},{64.4,-68},{66,-68},{66,-49}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_9) annotation (Line(
            points={{38.8,75.2},{38.8,58},{46,58},{46,41.2}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_8) annotation (Line(
            points={{62.4,75.2},{62.4,58},{66,58},{66,41.2}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_4,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_10) annotation (Line(
            points={{7.6,75.6},{7.6,58},{6,58},{6,41.2}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.frame_susp_5,
          full_hull_3DOF_6w_3g_no_speed_sensors.frame_susp_11) annotation (Line(
            points={{-14.8,75.6},{-14.8,58},{-14,58},{-14,41.2}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(brg_RHS1, full_hull_3DOF_6w_3g_no_speed_sensors.left_power_in)
          annotation (Line(
            points={{-200,100},{-140,100},{-140,16},{86,16}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_a, full_hull_3DOF_6w_3g_no_speed_sensors.frame_a) annotation (
            Line(
            points={{-200,60},{-160,60},{-160,-4},{46,-4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.pin_n, ground) annotation (Line(
            points={{-50,-8},{-122,-8},{-122,-40},{-200,-40}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.right_power_in, brg_LHS1)
          annotation (Line(
            points={{86,-24},{-108,-24},{-108,-100},{-200,-100}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(full_hull_3DOF_6w_3g_no_speed_sensors.driver_bus, driver_bus)
          annotation (Line(
            points={{-50,8},{-80,8},{-80,128},{0,128},{0,140}},
            color={170,213,255},
            thickness=0.5,
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(suspension_Full_Container_2_Wheels1.guide_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.guide_6) annotation (Line(
            points={{-25.6,68.4},{-25.6,50.2},{-24,50.2},{-24,32}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.guide_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.guide_5) annotation (Line(
            points={{26.8,68.4},{26.8,50.2},{26,50.2},{26,32}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels1.guide_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.guide_4) annotation (Line(
            points={{72.8,68},{72.8,49},{76,49},{76,32}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.guide_3,
          full_hull_3DOF_6w_3g_no_speed_sensors.guide_3) annotation (Line(
            points={{-23.6,-80.4},{-23.6,-60.2},{-24,-60.2},{-24,-40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.guide_2,
          full_hull_3DOF_6w_3g_no_speed_sensors.guide_2) annotation (Line(
            points={{28.8,-80.4},{28.8,-60.2},{26,-60.2},{26,-40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(suspension_Full_Container_2_Wheels.guide_1,
          full_hull_3DOF_6w_3g_no_speed_sensors.guide_1) annotation (Line(
            points={{74.8,-80},{76,-80},{76,-40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-200,
                  -140},{200,140}}), graphics), Icon(coordinateSystem(extent={{-200,
                  -140},{200,140}}, preserveAspectRatio=false),graphics={Text(
                extent={{-98,-46},{84,-100}},
                lineColor={0,0,255},
                textString="6 wheels"),                                  Text(
                extent={{-98,-84},{84,-138}},
                lineColor={0,0,255},
                textString="3 guides"),
              Text(
                extent={{-160,60},{160,20}},
                lineColor={0,127,0},
                textString="Hydro pneumatic")}));
      end Hull_and_Susp;
    end FullDesignWithSuspension;

    model SystemDesignTest
      extends Modelica.Icons.Example;
      DriveLineDesign driveLineDesign
        annotation (Placement(transformation(extent={{-16,-18},{16,14}})));
      replaceable Testing_Blocks.Land_Decl.FTP_Driver driver_Base_Land constrainedby
      C2M2L_Decl.Testing_Blocks.Land.Driver_Base_Land
        annotation (Placement(transformation(extent={{-28,40},{-8,60}})));
      C2M2L_Decl.Testing_Blocks.Land.Road_Wheel_Load_Both_Sides
        road_Wheel_Load_Both_Sides
        annotation (Placement(transformation(extent={{40,-14},{60,6}})));
      inner C2M2L_Decl.MSL_Decl.Fluid.System
                                  system
        annotation (Placement(transformation(extent={{-60,-100},{-40,-80}})));
      inner C2M2L_Decl.Environments.Lumped_Thermal_Fixed_HTC_Open_Volume
                                                            env(
        fixed_htc=100,
        redeclare package Medium =
            C2M2L_Decl.Media.Incompressible.Essotherm650_Const_Prop)
        annotation (Placement(transformation(extent={{-20,-100},{0,-80}})));
      inner Modelica.Mechanics.MultiBody.World world(
        gravityType=Modelica.Mechanics.MultiBody.Types.GravityTypes.UniformGravity,
        n={-1,0,0},
        enableAnimation=false,
        driveTrainMechanics3D=false)
        annotation (Placement(transformation(extent={{-100,-100},{-80,-80}})));
    equation
      connect(driveLineDesign.driver, driver_Base_Land.driver_bus) annotation (
          Line(
          points={{0,14},{0,50},{-8,50}},
          color={170,213,255},
          thickness=0.5,
          smooth=Smooth.None));
      connect(driveLineDesign.brake, driver_Base_Land.brake_Control_Bus)
        annotation (Line(
          points={{4,14},{4,46},{-8,46}},
          color={255,85,85},
          thickness=0.5,
          smooth=Smooth.None));
      connect(driveLineDesign.steering, driver_Base_Land.steering_Control_Bus)
        annotation (Line(
          points={{2,14},{2,42},{-8,42}},
          color={0,127,0},
          thickness=0.5,
          smooth=Smooth.None));
      connect(road_Wheel_Load_Both_Sides.rhs_flange, driveLineDesign.brg_2)
        annotation (Line(
          points={{39.8,0},{28,0},{28,20},{7.4,20},{7.4,14}},
          color={135,135,135},
          thickness=0.5,
          smooth=Smooth.None));
      connect(driveLineDesign.brg_1, road_Wheel_Load_Both_Sides.lhs_flange)
        annotation (Line(
          points={{7.4,-18},{8,-18},{8,-32},{28,-32},{28,-8},{40,-8}},
          color={135,135,135},
          thickness=0.5,
          smooth=Smooth.None));
      connect(road_Wheel_Load_Both_Sides.driver_Bus, driver_Base_Land.driver_bus)
        annotation (Line(
          points={{50,6},{50,50},{-8,50}},
          color={170,213,255},
          thickness=0.5,
          smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
                -100},{100,100}}),      graphics={Text(
              extent={{-100,100},{100,80}},
              lineColor={255,0,0},
              textString=
                  "Tested with Dymola 2014 FD 01 64-bit on Windows. MSL 3.2.1")}),
        experiment(StopTime=1000, __Dymola_Algorithm="Esdirk45a"),
        __Dymola_experimentSetupOutput);
    end SystemDesignTest;

    model SystemDesignTest_100s
      extends Modelica.Icons.Example;
      DriveLineDesign driveLineDesign
        annotation (Placement(transformation(extent={{-16,-18},{16,14}})));
      replaceable Testing_Blocks.Land_Decl.FTP_Driver driver_Base_Land constrainedby
      C2M2L_Decl.Testing_Blocks.Land.Driver_Base_Land
        annotation (Placement(transformation(extent={{-28,40},{-8,60}})));
      C2M2L_Decl.Testing_Blocks.Land.Road_Wheel_Load_Both_Sides
        road_Wheel_Load_Both_Sides
        annotation (Placement(transformation(extent={{40,-14},{60,6}})));
      inner C2M2L_Decl.Environments.Lumped_Thermal_Fixed_HTC_Open_Volume
                                                            env(
        fixed_htc=100,
        redeclare package Medium =
            Modelica.Media.Incompressible.Examples.Essotherm650)
        annotation (Placement(transformation(extent={{-20,-100},{0,-80}})));
      inner C2M2L_Decl.MSL_Decl.Fluid.System
                                  system
        annotation (Placement(transformation(extent={{-60,-100},{-40,-80}})));
      inner Modelica.Mechanics.MultiBody.World world(
        gravityType=Modelica.Mechanics.MultiBody.Types.GravityTypes.UniformGravity,
        n={-1,0,0},
        enableAnimation=false,
        driveTrainMechanics3D=false)
        annotation (Placement(transformation(extent={{-100,-100},{-80,-80}})));
    equation
      connect(driveLineDesign.driver, driver_Base_Land.driver_bus) annotation (
          Line(
          points={{0,14},{0,50},{-8,50}},
          color={170,213,255},
          thickness=0.5,
          smooth=Smooth.None));
      connect(driveLineDesign.brake, driver_Base_Land.brake_Control_Bus)
        annotation (Line(
          points={{4,14},{4,46},{-8,46}},
          color={255,85,85},
          thickness=0.5,
          smooth=Smooth.None));
      connect(driveLineDesign.steering, driver_Base_Land.steering_Control_Bus)
        annotation (Line(
          points={{2,14},{2,42},{-8,42}},
          color={0,127,0},
          thickness=0.5,
          smooth=Smooth.None));
      connect(road_Wheel_Load_Both_Sides.rhs_flange, driveLineDesign.brg_2)
        annotation (Line(
          points={{39.8,0},{28,0},{28,20},{7.4,20},{7.4,14}},
          color={135,135,135},
          thickness=0.5,
          smooth=Smooth.None));
      connect(driveLineDesign.brg_1, road_Wheel_Load_Both_Sides.lhs_flange)
        annotation (Line(
          points={{7.4,-18},{8,-18},{8,-32},{28,-32},{28,-8},{40,-8}},
          color={135,135,135},
          thickness=0.5,
          smooth=Smooth.None));
      connect(road_Wheel_Load_Both_Sides.driver_Bus, driver_Base_Land.driver_bus)
        annotation (Line(
          points={{50,6},{50,50},{-8,50}},
          color={170,213,255},
          thickness=0.5,
          smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
                -100},{100,100}}),      graphics={Text(
              extent={{-100,100},{100,80}},
              lineColor={255,0,0},
              textString=
                  "Tested with Dymola 2014 FD 01 64-bit on Windows. MSL 3.2.1")}),
        experiment(StopTime=100, __Dymola_Algorithm="Esdirk45a"),
        __Dymola_experimentSetupOutput);
    end SystemDesignTest_100s;
  end Test_Assemblies_Decl;

  package Testing_Blocks
    extends Modelica.Icons.Package;
    model Amphibious_Load
      extends C2M2L_Ext.Interfaces.Test_Bed_C2M2L;
      Modelica.Mechanics.MultiBody.Joints.Prismatic prismatic
        annotation (Placement(transformation(extent={{6,-10},{26,10}})));
      Modelica.Mechanics.MultiBody.Parts.Body body(m=vehicle_mass)
                                                            annotation (Placement(
            transformation(
            extent={{-10,-10},{10,10}},
            rotation=90,
            origin={0,20})));
      Modelica.Mechanics.MultiBody.Parts.Fixed fixed annotation (Placement(
            transformation(
            extent={{-10,-10},{10,10}},
            rotation=90,
            origin={40,-10})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation (Placement(
            transformation(rotation=0, extent={{-120,-20},{-80,20}})));
      parameter SI.Mass vehicle_mass=20000 "Mass of rigid body";
    equation
      connect(body.frame_a,prismatic. frame_a) annotation (Line(
          points={{0,10},{0,0},{6,0}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(prismatic.frame_b,fixed. frame_b) annotation (Line(
          points={{26,0},{40,0}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(frame_a, prismatic.frame_a) annotation (Line(points={{-100,0},{-100,0},
              {-100,0},{6,0}},      color={95,95,95}));
      annotation (Diagram(coordinateSystem(extent={{-100,-100},{100,100}},
              preserveAspectRatio=true),  graphics),                         Icon(
            coordinateSystem(extent={{-100,-100},{100,100}}, preserveAspectRatio=true),
            graphics={
            Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,0},
              lineThickness=0.5),
            Text(
              extent={{-60,100},{60,0}},
              lineColor={0,0,0},
              lineThickness=0.5,
              fillColor={0,0,255},
              fillPattern=FillPattern.Solid,
              textString="Aqua
Load"),     Text(
              extent={{-100,-40},{100,-60}},
              lineColor={135,135,135},
              lineThickness=0.5,
              fillColor={0,0,255},
              fillPattern=FillPattern.Solid,
              textString="m=%vehicle_mass kg")}));
    end Amphibious_Load;

    package Hull
      extends Modelica.Icons.Package;

      model Full_hull_3DOF_2w_0g_no_speed_sensors
      "A full hull with 3 degrees of freedom.   2 suspension attachments and 0guide wheel attachments each side."
        extends C2M2L_CyPhy.Testing_Blocks.Hull.Hull_Interface;

        parameter SI.Area frontal_area=16 "Frontal area of vehicle"
          annotation (Dialog(group="Aero drag"));
        parameter SI.Density rho_air = 1.16 "Nominal air density" annotation (Dialog(group="Aero drag"));
        parameter Real Cd = 1.25
        "Vehicle drag factor (Cd) in F = 0.5 * Cd * rho * v^2 * A"   annotation (Dialog(group="Aero drag"));
         parameter SI.Mass m_hull(min=0)=20000 "Total Vehicle Mass"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_xx_hull(min=0)=1e4 "Total Vehicle Roll Inertia"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_yy_hull(min=0)=1e4 "Total Vehicle Yaw Inertia"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_zz_hull(min=0)=3.5e4
        "Total Vehicle Pitch Inertia"
          annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_21_hull=0 " (2,1) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_31_hull=0 " (3,1) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_32_hull=0 " (3,2) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));

         parameter SI.Length CG_hull_x = -3.2
        "Horizontal distance to vehicle center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_hull_y = 0.2
        "Vertical distance to vehicle center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_driver_x = 0
        "Horizontal distance to driver center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_driver_y = 0
        "Vertical distance to driver center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));

        parameter SI.Length hub_spacing_1_x = -0.675
        "Horizontal distance to first suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 1 Location"));
         parameter SI.Length hub_spacing_1_y = -0.15
        "Vertical distance to first suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 1 Location"));

         parameter SI.Length hub_spacing_2_x = -1.475
        "Horizontal distance to second suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 2 Location"));
         parameter SI.Length hub_spacing_2_y = -0.15
        "Vertical distance to second suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 2 Location"));

         parameter SI.Length hub_spacing_3_x = -2.275
        "Horizontal distance to third suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 3 Location"));
         parameter SI.Length hub_spacing_3_y = -0.15
        "Vertical distance to third suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 3 Location"));

         parameter SI.Length hub_spacing_4_x = -3.075
        "Horizontal distance to fourth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 4 Location"));
         parameter SI.Length hub_spacing_4_y = -0.15
        "Vertical distance to fourth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 4 Location"));

         parameter SI.Length hub_spacing_5_x = -3.875
        "Horizontal distance to fifth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 5 Location"));
         parameter SI.Length hub_spacing_5_y = -0.15
        "Vertical distance to fifth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 5 Location"));

         parameter SI.Length hub_spacing_6_x = -4.675
        "Horizontal distance to sixth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 6 Location"));
         parameter SI.Length hub_spacing_6_y = -0.15
        "Vertical distance to sixth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 6 Location"));
         parameter SI.Length idler_x = -6
        "Horizontal distance to idler wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Idler Location"));
         parameter SI.Length idler_y = 0.05
        "Vertical distance to idler wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Idler Location"));

         parameter SI.Length guide_1_x = -1.6
        "Horizontal distance to first guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 1 Location"));
         parameter SI.Length guide_1_y = 0.15
        "Vertical distance to first guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 1 Location"));

         parameter SI.Length guide_2_x = -3.2
        "Horizontal distance to second guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 2 Location"));
         parameter SI.Length guide_2_y = 0.15
        "Vertical distance to second guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 2 Location"));

         parameter SI.Length guide_3_x = -4.8
        "Horizontal distance to third guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 3 Location"));
         parameter SI.Length guide_3_y = 0.15
        "Vertical distance to third guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 3 Location"));

         parameter SI.Length cl_to_rhs = 1.5
        "Distance from center line to right hand side attachment points"
          annotation (Dialog(group="Vehicle width"));
         parameter SI.Length susp_offset = 0.15
        "Use same value as 'offset_wheel' in suspension module"
          annotation (Dialog(group="Vehicle width"));

        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_1(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_1_x,hub_spacing_1_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={150,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_1
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={150,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={280,-225})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        sprocket(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={110,-50}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={370,-220})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        idler(includeBearingConnector=
             true)                                              annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={110,-90}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-370,-220})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation idler_mount(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={idler_x,idler_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={70,-90})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque(
            tau_constant=0)
          annotation (Placement(transformation(extent={{160,-100},{140,-80}})));

        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_track(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,cl_to_rhs})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={30,14})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing left_power_in(
            includeBearingConnector=world.driveTrainMechanics3D)
                                          annotation (Placement(transformation(extent=
                 {{-300,-20},{-280,0}}), iconTransformation(
              extent={{-20,-20},{20,20}},
              rotation=0,
              origin={280,100})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing right_power_in(
            includeBearingConnector=world.driveTrainMechanics3D)
                                          annotation (Placement(transformation(extent=
                 {{-300,-160},{-280,-140}}), iconTransformation(
              extent={{-20,-20},{20,20}},
              rotation=0,
              origin={280,-100})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_6(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_6_x,hub_spacing_6_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-150,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_6
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-150,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-320,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_susp(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,cl_to_rhs - susp_offset})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-92,20})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_7(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_1_x,hub_spacing_1_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={462,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation idler_mount1(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={idler_x,idler_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={454,-84})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque1(
            tau_constant=0)
          annotation (Placement(transformation(extent={{544,-94},{524,-74}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_track1(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,-cl_to_rhs})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={412,32})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_12(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_6_x,hub_spacing_6_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={162,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_susp1(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,-(cl_to_rhs - susp_offset)})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={410,60})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_7
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={462,198}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={280,226})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        sprocket1(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={494,-44}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={370,220})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        idler1(
                                                              includeBearingConnector=
             true)                                              annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={494,-84}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-370,220})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_12
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={162,198}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-320,226})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{358,-4},{378,16}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor1(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{-10,10},{10,-10}},
              rotation=180,
              origin={206,-26})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor2(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{154,-36},{174,-16}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor3(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{458,-4},{438,16}})));
        Modelica.Mechanics.Rotational.Components.IdealGear idealGear(ratio=-1)
          annotation (Placement(transformation(extent={{414,-4},{434,16}})));
        replaceable package Bilge_Medium =
            C2M2L_Ext.Media.Incompressible.Water_Incompressible_Constant;
        parameter SI.Area bilge_area=20 "Area of tank";
        parameter Modelica.Media.Interfaces.PartialMedium.MassFlowRate leak_flow=0
        "Fixed mass flow rate going out of the fluid port";
          parameter SI.Height bilge_initial=0.05
        "Initial level of water in the bilge"
          annotation (Dialog(group="Bilge"));
        Modelica.Mechanics.Rotational.Sensors.TorqueSensor torqueSensor
          annotation (Placement(transformation(extent={{174,-18},{194,2}})));
        Modelica.Mechanics.Rotational.Sensors.TorqueSensor torqueSensor1
          annotation (Placement(transformation(extent={{386,16},{406,-4}})));
        Modelica.Blocks.Interfaces.RealOutput tau_sprocket_left annotation (Placement(
              transformation(extent={{402,200},{422,220}}), iconTransformation(extent=
                 {{402,200},{422,220}})));
        Modelica.Blocks.Interfaces.RealOutput tau_sprocket_right annotation (
            Placement(transformation(extent={{400,-220},{420,-200}}),
              iconTransformation(extent={{400,-220},{420,-200}})));
        Modelica.Blocks.Interfaces.RealOutput f_tractive
        "Tractive force generated at sprocket radius"   annotation (Placement(
              transformation(extent={{348,-74},{368,-54}}),
                                                         iconTransformation(extent={{348,-74},
                  {368,-54}})));
        Modelica.Blocks.Math.MultiSum multiSum(nu=2)
          annotation (Placement(transformation(extent={{284,-182},{296,-170}})));
        Modelica.Blocks.Math.Gain one_over_r(k=2/sprocket_diameter)
        "Divide by sprokect radius"
          annotation (Placement(transformation(extent={{320,-190},{340,-170}})));
        parameter SI.Diameter sprocket_diameter = 0.6
        "Diameter of sprocket used to calculate tractive force";
        parameter Boolean use_electrical_ground = true
        "true to add an electrical ground point";
        Modelica.Electrical.Analog.Basic.Ground ground if use_electrical_ground
          annotation (Placement(transformation(extent={{-370,-60},{-350,-40}})));
        Modelica.Electrical.Analog.Interfaces.NegativePin pin_n if use_electrical_ground
          annotation (Placement(transformation(extent={{-410,-30},{-390,-10}})));
        C2M2L_Ext.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_bus
          annotation (Placement(transformation(extent={{-410,48},{-390,68}})));
        Modelica.Mechanics.MultiBody.Sensors.AbsoluteVelocity absoluteVelocity2(
            resolveInFrame=Modelica.Mechanics.MultiBody.Types.ResolveInFrameA.frame_a)
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=180,
              origin={-142,74})));
      equation
        connect(susp_mount_1.frame_b, frame_susp_1) annotation (Line(
            points={{150,-170},{150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_6.frame_b,frame_susp_6)  annotation (Line(
            points={{-150,-170},{-150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler, idler) annotation (Line(
            points={{110,-90},{110,-90}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque.flange, idler.flange) annotation (Line(
            points={{140,-90},{110,-90}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(idler_mount.frame_b, idler.bearingFrame) annotation (Line(
            points={{80,-90},{110,-90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount.frame_a, half_width_track.frame_b)
                                                         annotation (Line(
            points={{60,-90},{0,-90},{0,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track.frame_b, sprocket.bearingFrame)
                                                           annotation (Line(
            points={{20,14},{0,14},{0,-50},{110,-50}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_1.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{150,-150},{150,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_6.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-150,-150},{-148,-150},{-148,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_7.frame_b,frame_susp_7)  annotation (Line(
            points={{462,220},{462,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_12.frame_b, frame_susp_12)
                                                    annotation (Line(
            points={{162,220},{162,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler1, idler1)
                              annotation (Line(
            points={{494,-84},{494,-84}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque1.flange, idler1.flange)
                                                     annotation (Line(
            points={{524,-84},{494,-84}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(idler_mount1.frame_b, idler1.bearingFrame)
                                                         annotation (Line(
            points={{464,-84},{494,-84}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount1.frame_a, half_width_track1.frame_b)
                                                         annotation (Line(
            points={{444,-84},{384,-84},{384,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track1.frame_b, sprocket1.bearingFrame)
                                                           annotation (Line(
            points={{402,32},{384,32},{384,-44},{494,-44}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_7.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{462,240},{462,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_12.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{162,240},{164,240},{164,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor3.flangeAndFrame, sprocket1) annotation (Line(
            points={{458,6},{476,6},{476,-44},{494,-44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor2.flangeAndFrame, sprocket) annotation (Line(
            points={{154,-26},{94,-26},{94,-50},{110,-50}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(left_power_in, adaptor.flangeAndFrame) annotation (Line(
            points={{-290,-10},{-276,-10},{-276,6},{358,6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(right_power_in, adaptor1.flangeAndFrame) annotation (Line(
            points={{-290,-150},{-240,-150},{-240,-212},{236,-212},{236,-26},{216,-26}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor1.frame, adaptor2.frame) annotation (Line(
            points={{206,-36},{164,-36}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idealGear.flange_b, adaptor3.flange) annotation (Line(
            points={{434,6},{448,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(adaptor.frame, adaptor3.frame) annotation (Line(
            points={{368,-4},{368,-14},{448,-14},{448,-4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(torqueSensor.flange_a, adaptor2.flange) annotation (Line(
            points={{174,-8},{170,-8},{170,-26},{164,-26}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor.flange_b, adaptor1.flange) annotation (Line(
            points={{194,-8},{200,-8},{200,-26},{206,-26}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.flange_b, idealGear.flange_a) annotation (Line(
            points={{406,6},{414,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.flange_a, adaptor.flange) annotation (Line(
            points={{386,6},{368,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.tau, tau_sprocket_left) annotation (Line(
            points={{388,17},{390,17},{390,210},{412,210}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor.tau, tau_sprocket_right) annotation (Line(
            points={{176,-19},{178,-19},{178,-210},{410,-210}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor.tau, multiSum.u[1]) annotation (Line(
            points={{176,-19},{178,-19},{178,-164},{284,-164},{284,-173.9}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor1.tau, multiSum.u[2]) annotation (Line(
            points={{388,17},{388,24},{252,24},{252,-178.1},{284,-178.1}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(multiSum.y, one_over_r.u) annotation (Line(
            points={{297.02,-176},{308,-176},{308,-180},{318,-180}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(one_over_r.y, f_tractive) annotation (Line(
            points={{341,-180},{255.5,-180},{255.5,-64},{358,-64}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(ground.p, pin_n) annotation (Line(
            points={{-360,-40},{-360,-20},{-400,-20}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(half_width_susp.frame_a, fixedTranslation1.frame_a) annotation (Line(
            points={{-82,20},{-20,20},{-20,40},{58,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_b, hull_mass.frame_a) annotation (Line(
            points={{78,40},{98,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track.frame_a, frame_a) annotation (Line(
            points={{40,14},{48,14},{48,40},{56,40},{56,40},{-62,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_track1.frame_a) annotation (
            Line(
            points={{58,40},{48,40},{48,54},{434,54},{434,32},{422,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, frame_a) annotation (Line(
            points={{58,40},{-62,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_susp1.frame_a) annotation (Line(
            points={{58,40},{48,40},{48,54},{434,54},{434,60},{420,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(absoluteVelocity2.v[1], driver_bus.vehicle_speed) annotation (Line(
            points={{-153,74.6667},{-276,74.6667},{-276,58},{-400,58}},
            color={0,0,127},
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(frame_a, absoluteVelocity2.frame_a) annotation (Line(
            points={{-62,40},{-70,40},{-70,68},{-132,68},{-132,74}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(extent={{-400,-220},{560,280}},
                preserveAspectRatio=false,
              initialScale=0.2),           graphics), Icon(coordinateSystem(extent={{-400,
                  -220},{560,280}},        preserveAspectRatio=true,
              initialScale=0.2),                                      graphics={
              Rectangle(
                extent={{-400,0},{400,220}},
                lineColor={255,85,170},
                fillColor={215,215,215},
                fillPattern=FillPattern.Solid),
              Rectangle(
                extent={{-400,-220},{400,0}},
                lineColor={255,85,170},
                fillPattern=FillPattern.Solid,
                fillColor={215,215,215}),
              Text(
                extent={{-228,18},{252,78}},
                lineColor={0,0,255},
                textString="%name"),
              Ellipse(
                extent={{-30,30},{30,-30}},
                lineColor={0,0,255},
                fillColor={135,135,135},
                fillPattern=FillPattern.Solid,
                origin={370,-220},
                rotation=360),
              Text(
                extent={{332,-160},{412,-180}},
                lineColor={0,0,0},
                textString="sp"),
              Text(
                extent={{-410,-160},{-330,-180}},
                lineColor={0,0,0},
                textString="id"),
              Text(
                extent={{-44,-28},{220,-70}},
                lineColor={128,128,128},
                textString="{0,0,0}"),
              Line(
                points={{280,100},{370,100},{370,208}},
                color={95,95,95},
                smooth=Smooth.None,
                thickness=0.5),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={0,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={-260,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={240,-150},
                rotation=360),
              Text(
                extent={{404,230},{484,210}},
                lineColor={128,128,128},
                textString="tau_l"),
              Text(
                extent={{404,-212},{484,-232}},
                lineColor={128,128,128},
                textString="tau_r"),
              Text(
                extent={{326,46},{586,26}},
                lineColor={128,128,128},
                textString="f_tractive"),
              Text(
                extent={{310,106},{570,86}},
                lineColor={128,128,128},
                textString="bilge"),
              Line(
                points={{-390,-20},{-350,-20},{-350,-60}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-370,-60},{-330,-60}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-358,-80},{-342,-80}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-364,-70},{-334,-70}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{280,-100},{370,-100},{370,-208}},
                color={95,95,95},
                smooth=Smooth.None,
                thickness=0.5)}));
      end Full_hull_3DOF_2w_0g_no_speed_sensors;

      model Full_hull_3DOF_4w_0g_no_speed_sensors
      "A full hull with 3 degrees of freedom.   6 suspension attachments and 0guide wheel attachments each side."
        extends C2M2L_CyPhy.Testing_Blocks.Hull.Hull_Interface;

        parameter SI.Area frontal_area=16 "Frontal area of vehicle"
          annotation (Dialog(group="Aero drag"));
        parameter SI.Density rho_air = 1.16 "Nominal air density" annotation (Dialog(group="Aero drag"));
        parameter Real Cd = 1.25
        "Vehicle drag factor (Cd) in F = 0.5 * Cd * rho * v^2 * A"   annotation (Dialog(group="Aero drag"));
         parameter SI.Mass m_hull(min=0)=20000 "Total Vehicle Mass"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_xx_hull(min=0)=1e4 "Total Vehicle Roll Inertia"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_yy_hull(min=0)=1e4 "Total Vehicle Yaw Inertia"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_zz_hull(min=0)=3.5e4
        "Total Vehicle Pitch Inertia"
          annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_21_hull=0 " (2,1) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_31_hull=0 " (3,1) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_32_hull=0 " (3,2) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));

         parameter SI.Length CG_hull_x = -3.2
        "Horizontal distance to vehicle center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_hull_y = 0.2
        "Vertical distance to vehicle center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_driver_x = 0
        "Horizontal distance to driver center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_driver_y = 0
        "Vertical distance to driver center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));

        parameter SI.Length hub_spacing_1_x = -0.675
        "Horizontal distance to first suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 1 Location"));
         parameter SI.Length hub_spacing_1_y = -0.15
        "Vertical distance to first suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 1 Location"));

         parameter SI.Length hub_spacing_2_x = -1.475
        "Horizontal distance to second suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 2 Location"));
         parameter SI.Length hub_spacing_2_y = -0.15
        "Vertical distance to second suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 2 Location"));

         parameter SI.Length hub_spacing_3_x = -2.275
        "Horizontal distance to third suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 3 Location"));
         parameter SI.Length hub_spacing_3_y = -0.15
        "Vertical distance to third suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 3 Location"));

         parameter SI.Length hub_spacing_4_x = -3.075
        "Horizontal distance to fourth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 4 Location"));
         parameter SI.Length hub_spacing_4_y = -0.15
        "Vertical distance to fourth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 4 Location"));

         parameter SI.Length hub_spacing_5_x = -3.875
        "Horizontal distance to fifth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 5 Location"));
         parameter SI.Length hub_spacing_5_y = -0.15
        "Vertical distance to fifth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 5 Location"));

         parameter SI.Length hub_spacing_6_x = -4.675
        "Horizontal distance to sixth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 6 Location"));
         parameter SI.Length hub_spacing_6_y = -0.15
        "Vertical distance to sixth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 6 Location"));
         parameter SI.Length idler_x = -6
        "Horizontal distance to idler wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Idler Location"));
         parameter SI.Length idler_y = 0.05
        "Vertical distance to idler wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Idler Location"));

         parameter SI.Length guide_1_x = -1.6
        "Horizontal distance to first guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 1 Location"));
         parameter SI.Length guide_1_y = 0.15
        "Vertical distance to first guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 1 Location"));

         parameter SI.Length guide_2_x = -3.2
        "Horizontal distance to second guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 2 Location"));
         parameter SI.Length guide_2_y = 0.15
        "Vertical distance to second guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 2 Location"));

         parameter SI.Length guide_3_x = -4.8
        "Horizontal distance to third guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 3 Location"));
         parameter SI.Length guide_3_y = 0.15
        "Vertical distance to third guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 3 Location"));

         parameter SI.Length cl_to_rhs = 1.5
        "Distance from center line to right hand side attachment points"
          annotation (Dialog(group="Vehicle width"));
         parameter SI.Length susp_offset = 0.15
        "Use same value as 'offset_wheel' in suspension module"
          annotation (Dialog(group="Vehicle width"));

        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_1(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_1_x,hub_spacing_1_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={150,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_1
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={150,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={280,-225})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        sprocket(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={110,-50}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={370,-220})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        idler(includeBearingConnector=
             true)                                              annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={110,-90}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-370,-220})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation idler_mount(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={idler_x,idler_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={70,-90})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque(
            tau_constant=0)
          annotation (Placement(transformation(extent={{160,-100},{140,-80}})));

        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_track(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,cl_to_rhs})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={30,14})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing left_power_in(
            includeBearingConnector=world.driveTrainMechanics3D)
                                          annotation (Placement(transformation(extent=
                 {{-300,-20},{-280,0}}), iconTransformation(
              extent={{-20,-20},{20,20}},
              rotation=0,
              origin={280,100})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing right_power_in(
            includeBearingConnector=world.driveTrainMechanics3D)
                                          annotation (Placement(transformation(extent=
                 {{-300,-160},{-280,-140}}), iconTransformation(
              extent={{-20,-20},{20,20}},
              rotation=0,
              origin={280,-100})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_6(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_6_x,hub_spacing_6_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-150,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_6
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-150,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-302,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_susp(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,cl_to_rhs - susp_offset})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-92,20})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_7(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_1_x,hub_spacing_1_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={462,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation idler_mount1(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={idler_x,idler_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={454,-84})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque1(
            tau_constant=0)
          annotation (Placement(transformation(extent={{544,-94},{524,-74}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_track1(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,-cl_to_rhs})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={412,32})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_12(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_6_x,hub_spacing_6_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={162,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_susp1(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,-(cl_to_rhs - susp_offset)})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={410,60})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_7
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={462,198}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={280,226})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        sprocket1(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={494,-44}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={370,220})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        idler1(
                                                              includeBearingConnector=
             true)                                              annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={494,-84}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-370,220})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_12
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={162,198}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-308,226})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{358,-4},{378,16}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor1(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{-10,10},{10,-10}},
              rotation=180,
              origin={206,-26})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor2(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{154,-36},{174,-16}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor3(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{458,-4},{438,16}})));
        Modelica.Mechanics.Rotational.Components.IdealGear idealGear(ratio=-1)
          annotation (Placement(transformation(extent={{414,-4},{434,16}})));
        replaceable package Bilge_Medium =
            C2M2L_Ext.Media.Incompressible.Water_Incompressible_Constant;
        parameter SI.Area bilge_area=20 "Area of tank";
        parameter Modelica.Media.Interfaces.PartialMedium.MassFlowRate leak_flow=0
        "Fixed mass flow rate going out of the fluid port";
          parameter SI.Height bilge_initial=0.05
        "Initial level of water in the bilge"
          annotation (Dialog(group="Bilge"));
        Modelica.Mechanics.Rotational.Sensors.TorqueSensor torqueSensor
          annotation (Placement(transformation(extent={{174,-18},{194,2}})));
        Modelica.Mechanics.Rotational.Sensors.TorqueSensor torqueSensor1
          annotation (Placement(transformation(extent={{386,16},{406,-4}})));
        Modelica.Blocks.Interfaces.RealOutput tau_sprocket_left annotation (Placement(
              transformation(extent={{402,200},{422,220}}), iconTransformation(extent=
                 {{402,200},{422,220}})));
        Modelica.Blocks.Interfaces.RealOutput tau_sprocket_right annotation (
            Placement(transformation(extent={{400,-220},{420,-200}}),
              iconTransformation(extent={{400,-220},{420,-200}})));
        Modelica.Blocks.Interfaces.RealOutput f_tractive
        "Tractive force generated at sprocket radius"   annotation (Placement(
              transformation(extent={{348,-74},{368,-54}}),
                                                         iconTransformation(extent={{348,-74},
                  {368,-54}})));
        Modelica.Blocks.Math.MultiSum multiSum(nu=2)
          annotation (Placement(transformation(extent={{284,-182},{296,-170}})));
        Modelica.Blocks.Math.Gain one_over_r(k=2/sprocket_diameter)
        "Divide by sprokect radius"
          annotation (Placement(transformation(extent={{320,-190},{340,-170}})));
        parameter SI.Diameter sprocket_diameter = 0.6
        "Diameter of sprocket used to calculate tractive force";
        parameter Boolean use_electrical_ground = true
        "true to add an electrical ground point";
        Modelica.Electrical.Analog.Basic.Ground ground if use_electrical_ground
          annotation (Placement(transformation(extent={{-370,-60},{-350,-40}})));
        Modelica.Electrical.Analog.Interfaces.NegativePin pin_n if use_electrical_ground
          annotation (Placement(transformation(extent={{-410,-30},{-390,-10}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_4(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_4_x,hub_spacing_4_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-72,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_4
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-72,-192}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-84,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_3(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_3_x,hub_spacing_3_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-26,-154})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_3
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-26,-186}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={54,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_10(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_4_x,hub_spacing_4_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={230,228})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_10
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={230,196}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-78,226})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_99(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_3_x,hub_spacing_3_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={294,230})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_9
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={294,198}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={64,226})));
        Modelica.Mechanics.MultiBody.Sensors.AbsoluteVelocity absoluteVelocity2(
            resolveInFrame=Modelica.Mechanics.MultiBody.Types.ResolveInFrameA.frame_a)
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=180,
              origin={-144,66})));
        C2M2L_Ext.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_bus
          annotation (Placement(transformation(extent={{-412,40},{-392,60}})));
      equation
        connect(susp_mount_1.frame_b, frame_susp_1) annotation (Line(
            points={{150,-170},{150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_6.frame_b,frame_susp_6)  annotation (Line(
            points={{-150,-170},{-150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler, idler) annotation (Line(
            points={{110,-90},{110,-90}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque.flange, idler.flange) annotation (Line(
            points={{140,-90},{110,-90}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(idler_mount.frame_b, idler.bearingFrame) annotation (Line(
            points={{80,-90},{110,-90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount.frame_a, half_width_track.frame_b)
                                                         annotation (Line(
            points={{60,-90},{0,-90},{0,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track.frame_b, sprocket.bearingFrame)
                                                           annotation (Line(
            points={{20,14},{0,14},{0,-50},{110,-50}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_1.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{150,-150},{150,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_6.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-150,-150},{-148,-150},{-148,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_7.frame_b,frame_susp_7)  annotation (Line(
            points={{462,220},{462,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_12.frame_b, frame_susp_12)
                                                    annotation (Line(
            points={{162,220},{162,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler1, idler1)
                              annotation (Line(
            points={{494,-84},{494,-84}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque1.flange, idler1.flange)
                                                     annotation (Line(
            points={{524,-84},{494,-84}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(idler_mount1.frame_b, idler1.bearingFrame)
                                                         annotation (Line(
            points={{464,-84},{494,-84}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount1.frame_a, half_width_track1.frame_b)
                                                         annotation (Line(
            points={{444,-84},{384,-84},{384,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track1.frame_b, sprocket1.bearingFrame)
                                                           annotation (Line(
            points={{402,32},{384,32},{384,-44},{494,-44}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_7.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{462,240},{462,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_12.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{162,240},{164,240},{164,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor3.flangeAndFrame, sprocket1) annotation (Line(
            points={{458,6},{476,6},{476,-44},{494,-44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor2.flangeAndFrame, sprocket) annotation (Line(
            points={{154,-26},{94,-26},{94,-50},{110,-50}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(left_power_in, adaptor.flangeAndFrame) annotation (Line(
            points={{-290,-10},{-276,-10},{-276,6},{358,6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(right_power_in, adaptor1.flangeAndFrame) annotation (Line(
            points={{-290,-150},{-240,-150},{-240,-212},{236,-212},{236,-26},{216,-26}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor1.frame, adaptor2.frame) annotation (Line(
            points={{206,-36},{164,-36}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idealGear.flange_b, adaptor3.flange) annotation (Line(
            points={{434,6},{448,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(adaptor.frame, adaptor3.frame) annotation (Line(
            points={{368,-4},{368,-14},{448,-14},{448,-4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(torqueSensor.flange_a, adaptor2.flange) annotation (Line(
            points={{174,-8},{170,-8},{170,-26},{164,-26}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor.flange_b, adaptor1.flange) annotation (Line(
            points={{194,-8},{200,-8},{200,-26},{206,-26}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.flange_b, idealGear.flange_a) annotation (Line(
            points={{406,6},{414,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.flange_a, adaptor.flange) annotation (Line(
            points={{386,6},{368,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.tau, tau_sprocket_left) annotation (Line(
            points={{388,17},{390,17},{390,210},{412,210}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor.tau, tau_sprocket_right) annotation (Line(
            points={{176,-19},{178,-19},{178,-210},{410,-210}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor.tau, multiSum.u[1]) annotation (Line(
            points={{176,-19},{178,-19},{178,-164},{284,-164},{284,-173.9}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor1.tau, multiSum.u[2]) annotation (Line(
            points={{388,17},{388,24},{252,24},{252,-178.1},{284,-178.1}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(multiSum.y, one_over_r.u) annotation (Line(
            points={{297.02,-176},{308,-176},{308,-180},{318,-180}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(one_over_r.y, f_tractive) annotation (Line(
            points={{341,-180},{255.5,-180},{255.5,-64},{358,-64}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(ground.p, pin_n) annotation (Line(
            points={{-360,-40},{-360,-20},{-400,-20}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(half_width_susp.frame_a, fixedTranslation1.frame_a) annotation (Line(
            points={{-82,20},{-20,20},{-20,40},{58,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_b, hull_mass.frame_a) annotation (Line(
            points={{78,40},{98,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track.frame_a, frame_a) annotation (Line(
            points={{40,14},{48,14},{48,40},{56,40},{56,40},{-62,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_track1.frame_a) annotation (
            Line(
            points={{58,40},{48,40},{48,54},{434,54},{434,32},{422,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, frame_a) annotation (Line(
            points={{58,40},{-62,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_susp1.frame_a) annotation (Line(
            points={{58,40},{48,40},{48,54},{434,54},{434,60},{420,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_4.frame_b,frame_susp_4)  annotation (Line(
            points={{-72,-170},{-72,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_4.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-72,-150},{-72,-138},{-102,-138},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_3.frame_b,frame_susp_3)  annotation (Line(
            points={{-26,-164},{-26,-186}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_3.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-26,-144},{-26,-132},{-102,-132},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_10.frame_b, frame_susp_10)
                                                    annotation (Line(
            points={{230,218},{230,196}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_10.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{230,238},{232,238},{232,258},{270,258},{270,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_99.frame_b, frame_susp_9)
                                                    annotation (Line(
            points={{294,220},{294,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_99.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{294,240},{296,240},{296,260},{334,260},{334,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(absoluteVelocity2.v[1], driver_bus.vehicle_speed) annotation (Line(
            points={{-155,66.6667},{-278,66.6667},{-278,50},{-402,50}},
            color={0,0,127},
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(frame_a, absoluteVelocity2.frame_a) annotation (Line(
            points={{-62,40},{-72,40},{-72,60},{-134,60},{-134,66}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(extent={{-400,-220},{560,280}},
                preserveAspectRatio=false,
              initialScale=0.2),           graphics), Icon(coordinateSystem(extent={{-400,
                  -220},{560,280}},        preserveAspectRatio=false,
              initialScale=0.2),                                      graphics={
              Rectangle(
                extent={{-400,0},{400,220}},
                lineColor={255,85,170},
                fillColor={215,215,215},
                fillPattern=FillPattern.Solid),
              Rectangle(
                extent={{-400,-220},{400,0}},
                lineColor={255,85,170},
                fillPattern=FillPattern.Solid,
                fillColor={215,215,215}),
              Text(
                extent={{-228,18},{252,78}},
                lineColor={0,0,255},
                textString="%name"),
              Ellipse(
                extent={{-30,30},{30,-30}},
                lineColor={0,0,255},
                fillColor={135,135,135},
                fillPattern=FillPattern.Solid,
                origin={370,-220},
                rotation=360),
              Text(
                extent={{332,-160},{412,-180}},
                lineColor={0,0,0},
                textString="sp"),
              Text(
                extent={{-410,-160},{-330,-180}},
                lineColor={0,0,0},
                textString="id"),
              Text(
                extent={{-44,-28},{220,-70}},
                lineColor={128,128,128},
                textString="{0,0,0}"),
              Line(
                points={{280,100},{370,100},{370,208}},
                color={95,95,95},
                smooth=Smooth.None,
                thickness=0.5),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={0,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={-260,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={240,-150},
                rotation=360),
              Text(
                extent={{404,230},{484,210}},
                lineColor={128,128,128},
                textString="tau_l"),
              Text(
                extent={{404,-212},{484,-232}},
                lineColor={128,128,128},
                textString="tau_r"),
              Text(
                extent={{326,46},{586,26}},
                lineColor={128,128,128},
                textString="f_tractive"),
              Text(
                extent={{310,106},{570,86}},
                lineColor={128,128,128},
                textString="bilge"),
              Line(
                points={{-390,-20},{-350,-20},{-350,-60}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-370,-60},{-330,-60}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-358,-80},{-342,-80}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-364,-70},{-334,-70}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{280,-100},{370,-100},{370,-208}},
                color={95,95,95},
                smooth=Smooth.None,
                thickness=0.5)}));
      end Full_hull_3DOF_4w_0g_no_speed_sensors;

      model Full_hull_3DOF_4w_1g_no_speed_sensors
      "A full hull with 3 degrees of freedom.   6 suspension attachments and 0guide wheel attachments each side."
        extends C2M2L_CyPhy.Testing_Blocks.Hull.Hull_Interface;

        parameter SI.Area frontal_area=16 "Frontal area of vehicle"
          annotation (Dialog(group="Aero drag"));
        parameter SI.Density rho_air = 1.16 "Nominal air density" annotation (Dialog(group="Aero drag"));
        parameter Real Cd = 1.25
        "Vehicle drag factor (Cd) in F = 0.5 * Cd * rho * v^2 * A"   annotation (Dialog(group="Aero drag"));
         parameter SI.Mass m_hull(min=0)=20000 "Total Vehicle Mass"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_xx_hull(min=0)=1e4 "Total Vehicle Roll Inertia"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_yy_hull(min=0)=1e4 "Total Vehicle Yaw Inertia"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_zz_hull(min=0)=3.5e4
        "Total Vehicle Pitch Inertia"
          annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_21_hull=0 " (2,1) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_31_hull=0 " (3,1) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_32_hull=0 " (3,2) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));

         parameter SI.Length CG_hull_x = -3.2
        "Horizontal distance to vehicle center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_hull_y = 0.2
        "Vertical distance to vehicle center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_driver_x = 0
        "Horizontal distance to driver center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_driver_y = 0
        "Vertical distance to driver center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));

        parameter SI.Length hub_spacing_1_x = -0.675
        "Horizontal distance to first suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 1 Location"));
         parameter SI.Length hub_spacing_1_y = -0.15
        "Vertical distance to first suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 1 Location"));

         parameter SI.Length hub_spacing_2_x = -1.475
        "Horizontal distance to second suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 2 Location"));
         parameter SI.Length hub_spacing_2_y = -0.15
        "Vertical distance to second suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 2 Location"));

         parameter SI.Length hub_spacing_3_x = -2.275
        "Horizontal distance to third suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 3 Location"));
         parameter SI.Length hub_spacing_3_y = -0.15
        "Vertical distance to third suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 3 Location"));

         parameter SI.Length hub_spacing_4_x = -3.075
        "Horizontal distance to fourth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 4 Location"));
         parameter SI.Length hub_spacing_4_y = -0.15
        "Vertical distance to fourth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 4 Location"));

         parameter SI.Length hub_spacing_5_x = -3.875
        "Horizontal distance to fifth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 5 Location"));
         parameter SI.Length hub_spacing_5_y = -0.15
        "Vertical distance to fifth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 5 Location"));

         parameter SI.Length hub_spacing_6_x = -4.675
        "Horizontal distance to sixth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 6 Location"));
         parameter SI.Length hub_spacing_6_y = -0.15
        "Vertical distance to sixth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 6 Location"));
         parameter SI.Length idler_x = -6
        "Horizontal distance to idler wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Idler Location"));
         parameter SI.Length idler_y = 0.05
        "Vertical distance to idler wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Idler Location"));

         parameter SI.Length guide_1_x = -1.6
        "Horizontal distance to first guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 1 Location"));
         parameter SI.Length guide_1_y = 0.15
        "Vertical distance to first guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 1 Location"));

         parameter SI.Length guide_2_x = -3.2
        "Horizontal distance to second guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 2 Location"));
         parameter SI.Length guide_2_y = 0.15
        "Vertical distance to second guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 2 Location"));

         parameter SI.Length guide_3_x = -4.8
        "Horizontal distance to third guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 3 Location"));
         parameter SI.Length guide_3_y = 0.15
        "Vertical distance to third guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 3 Location"));

         parameter SI.Length cl_to_rhs = 1.5
        "Distance from center line to right hand side attachment points"
          annotation (Dialog(group="Vehicle width"));
         parameter SI.Length susp_offset = 0.15
        "Use same value as 'offset_wheel' in suspension module"
          annotation (Dialog(group="Vehicle width"));

        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_1(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_1_x,hub_spacing_1_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={150,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_1
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={150,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={280,-225})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        sprocket(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={110,-50}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={370,-220})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        idler(includeBearingConnector=
             true)                                              annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={110,-90}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-370,-220})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation idler_mount(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={idler_x,idler_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={70,-90})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque(
            tau_constant=0)
          annotation (Placement(transformation(extent={{160,-100},{140,-80}})));

        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_track(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,cl_to_rhs})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={30,14})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing left_power_in(
            includeBearingConnector=world.driveTrainMechanics3D)
                                          annotation (Placement(transformation(extent=
                 {{-300,-20},{-280,0}}), iconTransformation(
              extent={{-20,-20},{20,20}},
              rotation=0,
              origin={280,100})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing right_power_in(
            includeBearingConnector=world.driveTrainMechanics3D)
                                          annotation (Placement(transformation(extent=
                 {{-300,-160},{-280,-140}}), iconTransformation(
              extent={{-20,-20},{20,20}},
              rotation=0,
              origin={280,-100})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_6(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_6_x,hub_spacing_6_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-150,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_6
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-150,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-302,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_susp(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,cl_to_rhs - susp_offset})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-92,20})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_7(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_1_x,hub_spacing_1_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={462,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation idler_mount1(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={idler_x,idler_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={454,-84})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque1(
            tau_constant=0)
          annotation (Placement(transformation(extent={{544,-94},{524,-74}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_track1(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,-cl_to_rhs})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={412,32})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_12(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_6_x,hub_spacing_6_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={162,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_susp1(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,-(cl_to_rhs - susp_offset)})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={410,60})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_7
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={462,198}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={280,226})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        sprocket1(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={494,-44}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={370,220})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        idler1(
                                                              includeBearingConnector=
             true)                                              annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={494,-84}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-370,220})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_12
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={162,198}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-308,226})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{358,-4},{378,16}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor1(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{-10,10},{10,-10}},
              rotation=180,
              origin={206,-26})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor2(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{154,-36},{174,-16}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor3(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{458,-4},{438,16}})));
        Modelica.Mechanics.Rotational.Components.IdealGear idealGear(ratio=-1)
          annotation (Placement(transformation(extent={{414,-4},{434,16}})));
        replaceable package Bilge_Medium =
            C2M2L_Ext.Media.Incompressible.Water_Incompressible_Constant;
        parameter SI.Area bilge_area=20 "Area of tank";
        parameter Modelica.Media.Interfaces.PartialMedium.MassFlowRate leak_flow=0
        "Fixed mass flow rate going out of the fluid port";
          parameter SI.Height bilge_initial=0.05
        "Initial level of water in the bilge"
          annotation (Dialog(group="Bilge"));
        Modelica.Mechanics.Rotational.Sensors.TorqueSensor torqueSensor
          annotation (Placement(transformation(extent={{174,-18},{194,2}})));
        Modelica.Mechanics.Rotational.Sensors.TorqueSensor torqueSensor1
          annotation (Placement(transformation(extent={{386,16},{406,-4}})));
        Modelica.Blocks.Interfaces.RealOutput tau_sprocket_left annotation (Placement(
              transformation(extent={{402,200},{422,220}}), iconTransformation(extent=
                 {{402,200},{422,220}})));
        Modelica.Blocks.Interfaces.RealOutput tau_sprocket_right annotation (
            Placement(transformation(extent={{400,-220},{420,-200}}),
              iconTransformation(extent={{400,-220},{420,-200}})));
        Modelica.Blocks.Interfaces.RealOutput f_tractive
        "Tractive force generated at sprocket radius"   annotation (Placement(
              transformation(extent={{348,-74},{368,-54}}),
                                                         iconTransformation(extent={{348,-74},
                  {368,-54}})));
        Modelica.Blocks.Math.MultiSum multiSum(nu=2)
          annotation (Placement(transformation(extent={{284,-182},{296,-170}})));
        Modelica.Blocks.Math.Gain one_over_r(k=2/sprocket_diameter)
        "Divide by sprokect radius"
          annotation (Placement(transformation(extent={{320,-190},{340,-170}})));
        parameter SI.Diameter sprocket_diameter = 0.6
        "Diameter of sprocket used to calculate tractive force";
        parameter Boolean use_electrical_ground = true
        "true to add an electrical ground point";
        Modelica.Electrical.Analog.Basic.Ground ground if use_electrical_ground
          annotation (Placement(transformation(extent={{-370,-60},{-350,-40}})));
        Modelica.Electrical.Analog.Interfaces.NegativePin pin_n if use_electrical_ground
          annotation (Placement(transformation(extent={{-410,-30},{-390,-10}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_4(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_4_x,hub_spacing_4_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-72,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_4
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-72,-192}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-84,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_3(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_3_x,hub_spacing_3_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-26,-154})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_3
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-26,-186}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={54,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_10(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_4_x,hub_spacing_4_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={230,228})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_10
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={230,196}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-78,226})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_99(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_3_x,hub_spacing_3_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={294,230})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_9
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={294,198}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={64,226})));
        Modelica.Mechanics.MultiBody.Sensors.AbsoluteVelocity absoluteVelocity2(
            resolveInFrame=Modelica.Mechanics.MultiBody.Types.ResolveInFrameA.frame_a)
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=180,
              origin={-144,66})));
        C2M2L_Ext.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_bus
          annotation (Placement(transformation(extent={{-412,40},{-392,60}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_2(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_2_x,guide_2_y,0})
          annotation (Placement(transformation(extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-20,-50})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_2(
            tau_constant=0)
          annotation (Placement(transformation(extent={{-94,-60},{-74,-40}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_2(
            includeBearingConnector=true)                                 annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-52,-50}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-20,-180})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_5(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_2_x,guide_2_y,0})
          annotation (Placement(transformation(extent={{10,-10},{-10,10}},
              rotation=0,
              origin={348,-44})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_5(
            tau_constant=0)
          annotation (Placement(transformation(extent={{264,-54},{284,-34}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_5(
            includeBearingConnector=true)                                 annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={308,-44}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-20,180})));
      equation
        connect(susp_mount_1.frame_b, frame_susp_1) annotation (Line(
            points={{150,-170},{150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_6.frame_b,frame_susp_6)  annotation (Line(
            points={{-150,-170},{-150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler, idler) annotation (Line(
            points={{110,-90},{110,-90}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque.flange, idler.flange) annotation (Line(
            points={{140,-90},{110,-90}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(idler_mount.frame_b, idler.bearingFrame) annotation (Line(
            points={{80,-90},{110,-90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount.frame_a, half_width_track.frame_b)
                                                         annotation (Line(
            points={{60,-90},{0,-90},{0,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track.frame_b, sprocket.bearingFrame)
                                                           annotation (Line(
            points={{20,14},{0,14},{0,-50},{110,-50}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_1.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{150,-150},{150,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_6.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-150,-150},{-148,-150},{-148,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_7.frame_b,frame_susp_7)  annotation (Line(
            points={{462,220},{462,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_12.frame_b, frame_susp_12)
                                                    annotation (Line(
            points={{162,220},{162,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler1, idler1)
                              annotation (Line(
            points={{494,-84},{494,-84}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque1.flange, idler1.flange)
                                                     annotation (Line(
            points={{524,-84},{494,-84}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(idler_mount1.frame_b, idler1.bearingFrame)
                                                         annotation (Line(
            points={{464,-84},{494,-84}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount1.frame_a, half_width_track1.frame_b)
                                                         annotation (Line(
            points={{444,-84},{384,-84},{384,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track1.frame_b, sprocket1.bearingFrame)
                                                           annotation (Line(
            points={{402,32},{384,32},{384,-44},{494,-44}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_7.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{462,240},{462,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_12.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{162,240},{164,240},{164,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor3.flangeAndFrame, sprocket1) annotation (Line(
            points={{458,6},{476,6},{476,-44},{494,-44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor2.flangeAndFrame, sprocket) annotation (Line(
            points={{154,-26},{94,-26},{94,-50},{110,-50}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(left_power_in, adaptor.flangeAndFrame) annotation (Line(
            points={{-290,-10},{-276,-10},{-276,6},{358,6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(right_power_in, adaptor1.flangeAndFrame) annotation (Line(
            points={{-290,-150},{-240,-150},{-240,-212},{236,-212},{236,-26},{216,-26}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor1.frame, adaptor2.frame) annotation (Line(
            points={{206,-36},{164,-36}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idealGear.flange_b, adaptor3.flange) annotation (Line(
            points={{434,6},{448,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(adaptor.frame, adaptor3.frame) annotation (Line(
            points={{368,-4},{368,-14},{448,-14},{448,-4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(torqueSensor.flange_a, adaptor2.flange) annotation (Line(
            points={{174,-8},{170,-8},{170,-26},{164,-26}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor.flange_b, adaptor1.flange) annotation (Line(
            points={{194,-8},{200,-8},{200,-26},{206,-26}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.flange_b, idealGear.flange_a) annotation (Line(
            points={{406,6},{414,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.flange_a, adaptor.flange) annotation (Line(
            points={{386,6},{368,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.tau, tau_sprocket_left) annotation (Line(
            points={{388,17},{390,17},{390,210},{412,210}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor.tau, tau_sprocket_right) annotation (Line(
            points={{176,-19},{178,-19},{178,-210},{410,-210}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor.tau, multiSum.u[1]) annotation (Line(
            points={{176,-19},{178,-19},{178,-164},{284,-164},{284,-173.9}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor1.tau, multiSum.u[2]) annotation (Line(
            points={{388,17},{388,24},{252,24},{252,-178.1},{284,-178.1}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(multiSum.y, one_over_r.u) annotation (Line(
            points={{297.02,-176},{308,-176},{308,-180},{318,-180}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(one_over_r.y, f_tractive) annotation (Line(
            points={{341,-180},{255.5,-180},{255.5,-64},{358,-64}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(ground.p, pin_n) annotation (Line(
            points={{-360,-40},{-360,-20},{-400,-20}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(half_width_susp.frame_a, fixedTranslation1.frame_a) annotation (Line(
            points={{-82,20},{-20,20},{-20,40},{58,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_b, hull_mass.frame_a) annotation (Line(
            points={{78,40},{98,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track.frame_a, frame_a) annotation (Line(
            points={{40,14},{48,14},{48,40},{56,40},{56,40},{-62,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_track1.frame_a) annotation (
            Line(
            points={{58,40},{48,40},{48,54},{434,54},{434,32},{422,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, frame_a) annotation (Line(
            points={{58,40},{-62,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_susp1.frame_a) annotation (Line(
            points={{58,40},{48,40},{48,54},{434,54},{434,60},{420,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_4.frame_b,frame_susp_4)  annotation (Line(
            points={{-72,-170},{-72,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_4.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-72,-150},{-72,-138},{-102,-138},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_3.frame_b,frame_susp_3)  annotation (Line(
            points={{-26,-164},{-26,-186}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_3.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-26,-144},{-26,-132},{-102,-132},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_10.frame_b, frame_susp_10)
                                                    annotation (Line(
            points={{230,218},{230,196}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_10.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{230,238},{232,238},{232,258},{270,258},{270,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_99.frame_b, frame_susp_9)
                                                    annotation (Line(
            points={{294,220},{294,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_99.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{294,240},{296,240},{296,260},{334,260},{334,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(absoluteVelocity2.v[1], driver_bus.vehicle_speed) annotation (Line(
            points={{-155,66.6667},{-278,66.6667},{-278,50},{-402,50}},
            color={0,0,127},
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(frame_a, absoluteVelocity2.frame_a) annotation (Line(
            points={{-62,40},{-72,40},{-72,60},{-134,60},{-134,66}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_2.frame_b,guide_2. bearingFrame) annotation (Line(
            points={{-30,-50},{-52,-50}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque_guide_2.flange,guide_2. flange) annotation (Line(
            points={{-74,-50},{-52,-50}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(guide_mount_5.frame_b,guide_5. bearingFrame) annotation (Line(
            points={{338,-44},{308,-44}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque_guide_5.flange,guide_5. flange) annotation (Line(
            points={{284,-44},{308,-44}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(guide_mount_2.frame_a, half_width_track.frame_b) annotation (Line(
            points={{-10,-50},{6,-50},{6,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_5.frame_a, half_width_track1.frame_b) annotation (Line(
            points={{358,-44},{400,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(extent={{-400,-220},{560,280}},
                preserveAspectRatio=false,
              initialScale=0.2),           graphics), Icon(coordinateSystem(extent={{-400,
                  -220},{560,280}},        preserveAspectRatio=false,
              initialScale=0.2),                                      graphics={
              Rectangle(
                extent={{-400,0},{400,220}},
                lineColor={255,85,170},
                fillColor={215,215,215},
                fillPattern=FillPattern.Solid),
              Rectangle(
                extent={{-400,-220},{400,0}},
                lineColor={255,85,170},
                fillPattern=FillPattern.Solid,
                fillColor={215,215,215}),
              Text(
                extent={{-228,18},{252,78}},
                lineColor={0,0,255},
                textString="%name"),
              Ellipse(
                extent={{-30,30},{30,-30}},
                lineColor={0,0,255},
                fillColor={135,135,135},
                fillPattern=FillPattern.Solid,
                origin={370,-220},
                rotation=360),
              Text(
                extent={{332,-160},{412,-180}},
                lineColor={0,0,0},
                textString="sp"),
              Text(
                extent={{-410,-160},{-330,-180}},
                lineColor={0,0,0},
                textString="id"),
              Text(
                extent={{-44,-28},{220,-70}},
                lineColor={128,128,128},
                textString="{0,0,0}"),
              Line(
                points={{280,100},{370,100},{370,208}},
                color={95,95,95},
                smooth=Smooth.None,
                thickness=0.5),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={0,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={-260,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={240,-150},
                rotation=360),
              Text(
                extent={{404,230},{484,210}},
                lineColor={128,128,128},
                textString="tau_l"),
              Text(
                extent={{404,-212},{484,-232}},
                lineColor={128,128,128},
                textString="tau_r"),
              Text(
                extent={{326,46},{586,26}},
                lineColor={128,128,128},
                textString="f_tractive"),
              Text(
                extent={{310,106},{570,86}},
                lineColor={128,128,128},
                textString="bilge"),
              Line(
                points={{-390,-20},{-350,-20},{-350,-60}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-370,-60},{-330,-60}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-358,-80},{-342,-80}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-364,-70},{-334,-70}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{280,-100},{370,-100},{370,-208}},
                color={95,95,95},
                smooth=Smooth.None,
                thickness=0.5)}));
      end Full_hull_3DOF_4w_1g_no_speed_sensors;

      model Full_hull_3DOF_6w_0g_no_speed_sensors
      "A full hull with 3 degrees of freedom.   6 suspension attachments and 0guide wheel attachments each side."
        extends C2M2L_CyPhy.Testing_Blocks.Hull.Hull_Interface;

        parameter SI.Area frontal_area=16 "Frontal area of vehicle"
          annotation (Dialog(group="Aero drag"));
        parameter SI.Density rho_air = 1.16 "Nominal air density" annotation (Dialog(group="Aero drag"));
        parameter Real Cd = 1.25
        "Vehicle drag factor (Cd) in F = 0.5 * Cd * rho * v^2 * A"   annotation (Dialog(group="Aero drag"));
         parameter SI.Mass m_hull(min=0)=20000 "Total Vehicle Mass"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_xx_hull(min=0)=1e4 "Total Vehicle Roll Inertia"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_yy_hull(min=0)=1e4 "Total Vehicle Yaw Inertia"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_zz_hull(min=0)=3.5e4
        "Total Vehicle Pitch Inertia"
          annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_21_hull=0 " (2,1) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_31_hull=0 " (3,1) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_32_hull=0 " (3,2) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));

         parameter SI.Length CG_hull_x = -3.2
        "Horizontal distance to vehicle center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_hull_y = 0.2
        "Vertical distance to vehicle center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_driver_x = 0
        "Horizontal distance to driver center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_driver_y = 0
        "Vertical distance to driver center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));

        parameter SI.Length hub_spacing_1_x = -0.675
        "Horizontal distance to first suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 1 Location"));
         parameter SI.Length hub_spacing_1_y = -0.15
        "Vertical distance to first suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 1 Location"));

         parameter SI.Length hub_spacing_2_x = -1.475
        "Horizontal distance to second suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 2 Location"));
         parameter SI.Length hub_spacing_2_y = -0.15
        "Vertical distance to second suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 2 Location"));

         parameter SI.Length hub_spacing_3_x = -2.275
        "Horizontal distance to third suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 3 Location"));
         parameter SI.Length hub_spacing_3_y = -0.15
        "Vertical distance to third suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 3 Location"));

         parameter SI.Length hub_spacing_4_x = -3.075
        "Horizontal distance to fourth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 4 Location"));
         parameter SI.Length hub_spacing_4_y = -0.15
        "Vertical distance to fourth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 4 Location"));

         parameter SI.Length hub_spacing_5_x = -3.875
        "Horizontal distance to fifth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 5 Location"));
         parameter SI.Length hub_spacing_5_y = -0.15
        "Vertical distance to fifth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 5 Location"));

         parameter SI.Length hub_spacing_6_x = -4.675
        "Horizontal distance to sixth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 6 Location"));
         parameter SI.Length hub_spacing_6_y = -0.15
        "Vertical distance to sixth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 6 Location"));
         parameter SI.Length idler_x = -6
        "Horizontal distance to idler wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Idler Location"));
         parameter SI.Length idler_y = 0.05
        "Vertical distance to idler wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Idler Location"));

         parameter SI.Length guide_1_x = -1.6
        "Horizontal distance to first guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 1 Location"));
         parameter SI.Length guide_1_y = 0.15
        "Vertical distance to first guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 1 Location"));

         parameter SI.Length guide_2_x = -3.2
        "Horizontal distance to second guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 2 Location"));
         parameter SI.Length guide_2_y = 0.15
        "Vertical distance to second guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 2 Location"));

         parameter SI.Length guide_3_x = -4.8
        "Horizontal distance to third guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 3 Location"));
         parameter SI.Length guide_3_y = 0.15
        "Vertical distance to third guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 3 Location"));

         parameter SI.Length cl_to_rhs = 1.5
        "Distance from center line to right hand side attachment points"
          annotation (Dialog(group="Vehicle width"));
         parameter SI.Length susp_offset = 0.15
        "Use same value as 'offset_wheel' in suspension module"
          annotation (Dialog(group="Vehicle width"));

        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_1(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_1_x,hub_spacing_1_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={150,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_1
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={150,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={280,-225})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        sprocket(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={110,-50}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={370,-220})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        idler(includeBearingConnector=
             true)                                              annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={110,-90}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-370,-220})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation idler_mount(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={idler_x,idler_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={70,-90})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque(
            tau_constant=0)
          annotation (Placement(transformation(extent={{160,-100},{140,-80}})));

        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_track(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,cl_to_rhs})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={30,14})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing left_power_in(
            includeBearingConnector=world.driveTrainMechanics3D)
                                          annotation (Placement(transformation(extent=
                 {{-300,-20},{-280,0}}), iconTransformation(
              extent={{-20,-20},{20,20}},
              rotation=0,
              origin={280,100})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing right_power_in(
            includeBearingConnector=world.driveTrainMechanics3D)
                                          annotation (Placement(transformation(extent=
                 {{-300,-160},{-280,-140}}), iconTransformation(
              extent={{-20,-20},{20,20}},
              rotation=0,
              origin={280,-100})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_6(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_6_x,hub_spacing_6_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-150,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_6
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-150,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-302,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_susp(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,cl_to_rhs - susp_offset})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-92,20})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_7(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_1_x,hub_spacing_1_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={462,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation idler_mount1(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={idler_x,idler_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={454,-84})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque1(
            tau_constant=0)
          annotation (Placement(transformation(extent={{544,-94},{524,-74}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_track1(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,-cl_to_rhs})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={412,32})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_12(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_6_x,hub_spacing_6_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={162,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_susp1(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,-(cl_to_rhs - susp_offset)})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={410,60})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_7
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={462,198}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={280,226})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        sprocket1(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={494,-44}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={370,220})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        idler1(
                                                              includeBearingConnector=
             true)                                              annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={494,-84}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-370,220})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_12
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={162,198}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-308,226})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{358,-4},{378,16}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor1(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{-10,10},{10,-10}},
              rotation=180,
              origin={206,-26})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor2(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{154,-36},{174,-16}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor3(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{458,-4},{438,16}})));
        Modelica.Mechanics.Rotational.Components.IdealGear idealGear(ratio=-1)
          annotation (Placement(transformation(extent={{414,-4},{434,16}})));
        replaceable package Bilge_Medium =
            C2M2L_Ext.Media.Incompressible.Water_Incompressible_Constant;
        parameter SI.Area bilge_area=20 "Area of tank";
        parameter Modelica.Media.Interfaces.PartialMedium.MassFlowRate leak_flow=0
        "Fixed mass flow rate going out of the fluid port";
          parameter SI.Height bilge_initial=0.05
        "Initial level of water in the bilge"
          annotation (Dialog(group="Bilge"));
        Modelica.Mechanics.Rotational.Sensors.TorqueSensor torqueSensor
          annotation (Placement(transformation(extent={{174,-18},{194,2}})));
        Modelica.Mechanics.Rotational.Sensors.TorqueSensor torqueSensor1
          annotation (Placement(transformation(extent={{386,16},{406,-4}})));
        Modelica.Blocks.Interfaces.RealOutput tau_sprocket_left annotation (Placement(
              transformation(extent={{402,200},{422,220}}), iconTransformation(extent=
                 {{402,200},{422,220}})));
        Modelica.Blocks.Interfaces.RealOutput tau_sprocket_right annotation (
            Placement(transformation(extent={{400,-220},{420,-200}}),
              iconTransformation(extent={{400,-220},{420,-200}})));
        Modelica.Blocks.Interfaces.RealOutput f_tractive
        "Tractive force generated at sprocket radius"   annotation (Placement(
              transformation(extent={{348,-74},{368,-54}}),
                                                         iconTransformation(extent={{348,-74},
                  {368,-54}})));
        Modelica.Blocks.Math.MultiSum multiSum(nu=2)
          annotation (Placement(transformation(extent={{284,-182},{296,-170}})));
        Modelica.Blocks.Math.Gain one_over_r(k=2/sprocket_diameter)
        "Divide by sprokect radius"
          annotation (Placement(transformation(extent={{320,-190},{340,-170}})));
        parameter SI.Diameter sprocket_diameter = 0.6
        "Diameter of sprocket used to calculate tractive force";
        parameter Boolean use_electrical_ground = true
        "true to add an electrical ground point";
        Modelica.Electrical.Analog.Basic.Ground ground if use_electrical_ground
          annotation (Placement(transformation(extent={{-370,-60},{-350,-40}})));
        Modelica.Electrical.Analog.Interfaces.NegativePin pin_n if use_electrical_ground
          annotation (Placement(transformation(extent={{-410,-30},{-390,-10}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_4(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_4_x,hub_spacing_4_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-72,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_4
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-72,-192}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-84,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_3(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_3_x,hub_spacing_3_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-26,-154})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_3
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-26,-186}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={54,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_10(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_4_x,hub_spacing_4_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={230,228})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_10
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={230,196}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-78,226})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_99(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_3_x,hub_spacing_3_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={294,230})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_9
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={294,198}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={64,226})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_5(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_5_x,hub_spacing_5_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-112,-158})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_5
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-112,-190}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-184,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_88(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_2_x,hub_spacing_2_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={360,230})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_8
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={360,198}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={160,226})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_2(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_2_x,hub_spacing_2_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={38,-154})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_2
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={38,-186}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={176,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_11(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_5_x,hub_spacing_5_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={192,228})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_11
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={192,196}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-198,226})));
        C2M2L_Ext.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_bus
          annotation (Placement(transformation(extent={{-410,50},{-390,70}})));
        Modelica.Mechanics.MultiBody.Sensors.AbsoluteVelocity absoluteVelocity2(
            resolveInFrame=Modelica.Mechanics.MultiBody.Types.ResolveInFrameA.frame_a)
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=180,
              origin={-142,76})));
      equation
        connect(susp_mount_1.frame_b, frame_susp_1) annotation (Line(
            points={{150,-170},{150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_6.frame_b,frame_susp_6)  annotation (Line(
            points={{-150,-170},{-150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler, idler) annotation (Line(
            points={{110,-90},{110,-90}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque.flange, idler.flange) annotation (Line(
            points={{140,-90},{110,-90}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(idler_mount.frame_b, idler.bearingFrame) annotation (Line(
            points={{80,-90},{110,-90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount.frame_a, half_width_track.frame_b)
                                                         annotation (Line(
            points={{60,-90},{0,-90},{0,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track.frame_b, sprocket.bearingFrame)
                                                           annotation (Line(
            points={{20,14},{0,14},{0,-50},{110,-50}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_1.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{150,-150},{150,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_6.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-150,-150},{-148,-150},{-148,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_7.frame_b,frame_susp_7)  annotation (Line(
            points={{462,220},{462,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_12.frame_b, frame_susp_12)
                                                    annotation (Line(
            points={{162,220},{162,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler1, idler1)
                              annotation (Line(
            points={{494,-84},{494,-84}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque1.flange, idler1.flange)
                                                     annotation (Line(
            points={{524,-84},{494,-84}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(idler_mount1.frame_b, idler1.bearingFrame)
                                                         annotation (Line(
            points={{464,-84},{494,-84}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount1.frame_a, half_width_track1.frame_b)
                                                         annotation (Line(
            points={{444,-84},{384,-84},{384,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track1.frame_b, sprocket1.bearingFrame)
                                                           annotation (Line(
            points={{402,32},{384,32},{384,-44},{494,-44}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_7.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{462,240},{462,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_12.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{162,240},{164,240},{164,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor3.flangeAndFrame, sprocket1) annotation (Line(
            points={{458,6},{476,6},{476,-44},{494,-44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor2.flangeAndFrame, sprocket) annotation (Line(
            points={{154,-26},{94,-26},{94,-50},{110,-50}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(left_power_in, adaptor.flangeAndFrame) annotation (Line(
            points={{-290,-10},{-276,-10},{-276,6},{358,6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(right_power_in, adaptor1.flangeAndFrame) annotation (Line(
            points={{-290,-150},{-240,-150},{-240,-212},{236,-212},{236,-26},{216,-26}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor1.frame, adaptor2.frame) annotation (Line(
            points={{206,-36},{164,-36}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idealGear.flange_b, adaptor3.flange) annotation (Line(
            points={{434,6},{448,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(adaptor.frame, adaptor3.frame) annotation (Line(
            points={{368,-4},{368,-14},{448,-14},{448,-4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(torqueSensor.flange_a, adaptor2.flange) annotation (Line(
            points={{174,-8},{170,-8},{170,-26},{164,-26}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor.flange_b, adaptor1.flange) annotation (Line(
            points={{194,-8},{200,-8},{200,-26},{206,-26}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.flange_b, idealGear.flange_a) annotation (Line(
            points={{406,6},{414,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.flange_a, adaptor.flange) annotation (Line(
            points={{386,6},{368,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.tau, tau_sprocket_left) annotation (Line(
            points={{388,17},{390,17},{390,210},{412,210}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor.tau, tau_sprocket_right) annotation (Line(
            points={{176,-19},{178,-19},{178,-210},{410,-210}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor.tau, multiSum.u[1]) annotation (Line(
            points={{176,-19},{178,-19},{178,-164},{284,-164},{284,-173.9}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor1.tau, multiSum.u[2]) annotation (Line(
            points={{388,17},{388,24},{252,24},{252,-178.1},{284,-178.1}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(multiSum.y, one_over_r.u) annotation (Line(
            points={{297.02,-176},{308,-176},{308,-180},{318,-180}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(one_over_r.y, f_tractive) annotation (Line(
            points={{341,-180},{255.5,-180},{255.5,-64},{358,-64}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(ground.p, pin_n) annotation (Line(
            points={{-360,-40},{-360,-20},{-400,-20}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(half_width_susp.frame_a, fixedTranslation1.frame_a) annotation (Line(
            points={{-82,20},{-20,20},{-20,40},{58,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_b, hull_mass.frame_a) annotation (Line(
            points={{78,40},{98,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track.frame_a, frame_a) annotation (Line(
            points={{40,14},{48,14},{48,40},{56,40},{56,40},{-62,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_track1.frame_a) annotation (
            Line(
            points={{58,40},{48,40},{48,54},{434,54},{434,32},{422,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, frame_a) annotation (Line(
            points={{58,40},{-62,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_susp1.frame_a) annotation (Line(
            points={{58,40},{48,40},{48,54},{434,54},{434,60},{420,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_4.frame_b,frame_susp_4)  annotation (Line(
            points={{-72,-170},{-72,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_4.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-72,-150},{-72,-138},{-102,-138},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_3.frame_b,frame_susp_3)  annotation (Line(
            points={{-26,-164},{-26,-186}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_3.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-26,-144},{-26,-132},{-102,-132},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_10.frame_b, frame_susp_10)
                                                    annotation (Line(
            points={{230,218},{230,196}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_10.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{230,238},{232,238},{232,258},{270,258},{270,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_99.frame_b, frame_susp_9)
                                                    annotation (Line(
            points={{294,220},{294,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_99.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{294,240},{296,240},{296,260},{334,260},{334,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_5.frame_b,frame_susp_5)  annotation (Line(
            points={{-112,-168},{-112,-190}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_5.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-112,-148},{-108,-148},{-108,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_88.frame_b, frame_susp_8)
                                                    annotation (Line(
            points={{360,220},{360,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_88.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{360,240},{362,240},{362,260},{400,260},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_2.frame_b, frame_susp_2) annotation (Line(
            points={{38,-164},{38,-186}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_2.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{38,-144},{-34,-144},{-34,-130},{-106,-130},{-106,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_11.frame_b, frame_susp_11)
                                                    annotation (Line(
            points={{192,218},{192,196}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_11.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{192,238},{194,238},{194,258},{232,258},{232,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(absoluteVelocity2.v[1],driver_bus. vehicle_speed) annotation (Line(
            points={{-153,76.6667},{-276,76.6667},{-276,60},{-400,60}},
            color={0,0,127},
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(frame_a,absoluteVelocity2. frame_a) annotation (Line(
            points={{-62,40},{-70,40},{-70,70},{-132,70},{-132,76}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(extent={{-400,-220},{560,280}},
                preserveAspectRatio=false,
              initialScale=0.2),           graphics), Icon(coordinateSystem(extent={{-400,
                  -220},{560,280}},        preserveAspectRatio=false,
              initialScale=0.2),                                      graphics={
              Rectangle(
                extent={{-400,0},{400,220}},
                lineColor={255,85,170},
                fillColor={215,215,215},
                fillPattern=FillPattern.Solid),
              Rectangle(
                extent={{-400,-220},{400,0}},
                lineColor={255,85,170},
                fillPattern=FillPattern.Solid,
                fillColor={215,215,215}),
              Text(
                extent={{-228,18},{252,78}},
                lineColor={0,0,255},
                textString="%name"),
              Ellipse(
                extent={{-30,30},{30,-30}},
                lineColor={0,0,255},
                fillColor={135,135,135},
                fillPattern=FillPattern.Solid,
                origin={370,-220},
                rotation=360),
              Text(
                extent={{332,-160},{412,-180}},
                lineColor={0,0,0},
                textString="sp"),
              Text(
                extent={{-410,-160},{-330,-180}},
                lineColor={0,0,0},
                textString="id"),
              Text(
                extent={{-44,-28},{220,-70}},
                lineColor={128,128,128},
                textString="{0,0,0}"),
              Line(
                points={{280,100},{370,100},{370,208}},
                color={95,95,95},
                smooth=Smooth.None,
                thickness=0.5),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={0,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={-260,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={240,-150},
                rotation=360),
              Text(
                extent={{404,230},{484,210}},
                lineColor={128,128,128},
                textString="tau_l"),
              Text(
                extent={{404,-212},{484,-232}},
                lineColor={128,128,128},
                textString="tau_r"),
              Text(
                extent={{326,46},{586,26}},
                lineColor={128,128,128},
                textString="f_tractive"),
              Text(
                extent={{310,106},{570,86}},
                lineColor={128,128,128},
                textString="bilge"),
              Line(
                points={{-390,-20},{-350,-20},{-350,-60}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-370,-60},{-330,-60}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-358,-80},{-342,-80}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-364,-70},{-334,-70}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{280,-100},{370,-100},{370,-208}},
                color={95,95,95},
                smooth=Smooth.None,
                thickness=0.5)}));
      end Full_hull_3DOF_6w_0g_no_speed_sensors;

      model Full_hull_3DOF_6w_0g_no_speed_sensors_idler_frame
      "A full hull with 3 degrees of freedom.   6 suspension attachments and 0guide wheel attachments each side."
        extends C2M2L_CyPhy.Testing_Blocks.Hull.Hull_Interface;

        parameter SI.Area frontal_area=16 "Frontal area of vehicle"
          annotation (Dialog(group="Aero drag"));
        parameter SI.Density rho_air = 1.16 "Nominal air density" annotation (Dialog(group="Aero drag"));
        parameter Real Cd = 1.25
        "Vehicle drag factor (Cd) in F = 0.5 * Cd * rho * v^2 * A"   annotation (Dialog(group="Aero drag"));
         parameter SI.Mass m_hull(min=0)=20000 "Total Vehicle Mass"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_xx_hull(min=0)=1e4 "Total Vehicle Roll Inertia"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_yy_hull(min=0)=1e4 "Total Vehicle Yaw Inertia"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_zz_hull(min=0)=3.5e4
        "Total Vehicle Pitch Inertia"
          annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_21_hull=0 " (2,1) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_31_hull=0 " (3,1) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_32_hull=0 " (3,2) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));

         parameter SI.Length CG_hull_x = -3.2
        "Horizontal distance to vehicle center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_hull_y = 0.2
        "Vertical distance to vehicle center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_driver_x = 0
        "Horizontal distance to driver center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_driver_y = 0
        "Vertical distance to driver center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));

        parameter SI.Length hub_spacing_1_x = -0.675
        "Horizontal distance to first suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 1 Location"));
         parameter SI.Length hub_spacing_1_y = -0.15
        "Vertical distance to first suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 1 Location"));

         parameter SI.Length hub_spacing_2_x = -1.475
        "Horizontal distance to second suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 2 Location"));
         parameter SI.Length hub_spacing_2_y = -0.15
        "Vertical distance to second suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 2 Location"));

         parameter SI.Length hub_spacing_3_x = -2.275
        "Horizontal distance to third suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 3 Location"));
         parameter SI.Length hub_spacing_3_y = -0.15
        "Vertical distance to third suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 3 Location"));

         parameter SI.Length hub_spacing_4_x = -3.075
        "Horizontal distance to fourth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 4 Location"));
         parameter SI.Length hub_spacing_4_y = -0.15
        "Vertical distance to fourth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 4 Location"));

         parameter SI.Length hub_spacing_5_x = -3.875
        "Horizontal distance to fifth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 5 Location"));
         parameter SI.Length hub_spacing_5_y = -0.15
        "Vertical distance to fifth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 5 Location"));

         parameter SI.Length hub_spacing_6_x = -4.675
        "Horizontal distance to sixth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 6 Location"));
         parameter SI.Length hub_spacing_6_y = -0.15
        "Vertical distance to sixth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 6 Location"));
         parameter SI.Length idler_x = -6
        "Horizontal distance to idler wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Idler Location"));
         parameter SI.Length idler_y = 0.05
        "Vertical distance to idler wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Idler Location"));

         parameter SI.Length guide_1_x = -1.6
        "Horizontal distance to first guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 1 Location"));
         parameter SI.Length guide_1_y = 0.15
        "Vertical distance to first guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 1 Location"));

         parameter SI.Length guide_2_x = -3.2
        "Horizontal distance to second guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 2 Location"));
         parameter SI.Length guide_2_y = 0.15
        "Vertical distance to second guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 2 Location"));

         parameter SI.Length guide_3_x = -4.8
        "Horizontal distance to third guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 3 Location"));
         parameter SI.Length guide_3_y = 0.15
        "Vertical distance to third guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 3 Location"));

         parameter SI.Length cl_to_rhs = 1.5
        "Distance from center line to right hand side attachment points"
          annotation (Dialog(group="Vehicle width"));
         parameter SI.Length susp_offset = 0.15
        "Use same value as 'offset_wheel' in suspension module"
          annotation (Dialog(group="Vehicle width"));

        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_1(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_1_x,hub_spacing_1_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={150,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_1
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={150,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={280,-225})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        sprocket(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={110,-50}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={370,-220})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation idler_mount(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={idler_x,idler_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={70,-90})));

        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_track(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,cl_to_rhs})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={30,14})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing left_power_in(
            includeBearingConnector=world.driveTrainMechanics3D)
                                          annotation (Placement(transformation(extent=
                 {{-300,-20},{-280,0}}), iconTransformation(
              extent={{-20,-20},{20,20}},
              rotation=0,
              origin={280,100})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing right_power_in(
            includeBearingConnector=world.driveTrainMechanics3D)
                                          annotation (Placement(transformation(extent=
                 {{-300,-160},{-280,-140}}), iconTransformation(
              extent={{-20,-20},{20,20}},
              rotation=0,
              origin={280,-100})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_6(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_6_x,hub_spacing_6_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-150,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_6
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-150,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-302,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_susp(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,cl_to_rhs - susp_offset})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-92,20})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_7(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_1_x,hub_spacing_1_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={462,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation idler_mount1(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={idler_x,idler_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={454,-84})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_track1(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,-cl_to_rhs})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={412,32})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_12(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_6_x,hub_spacing_6_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={162,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_susp1(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,-(cl_to_rhs - susp_offset)})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={410,60})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_7
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={462,198}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={280,226})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        sprocket1(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={494,-44}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={370,220})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_12
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={162,198}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-308,226})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{358,-4},{378,16}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor1(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{-10,10},{10,-10}},
              rotation=180,
              origin={206,-26})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor2(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{154,-36},{174,-16}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor3(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{458,-4},{438,16}})));
        Modelica.Mechanics.Rotational.Components.IdealGear idealGear(ratio=-1)
          annotation (Placement(transformation(extent={{414,-4},{434,16}})));
        replaceable package Bilge_Medium =
            C2M2L_Ext.Media.Incompressible.Water_Incompressible_Constant;
        parameter SI.Area bilge_area=20 "Area of tank";
        parameter Modelica.Media.Interfaces.PartialMedium.MassFlowRate leak_flow=0
        "Fixed mass flow rate going out of the fluid port";
          parameter SI.Height bilge_initial=0.05
        "Initial level of water in the bilge"
          annotation (Dialog(group="Bilge"));
        Modelica.Mechanics.Rotational.Sensors.TorqueSensor torqueSensor
          annotation (Placement(transformation(extent={{174,-18},{194,2}})));
        Modelica.Mechanics.Rotational.Sensors.TorqueSensor torqueSensor1
          annotation (Placement(transformation(extent={{386,16},{406,-4}})));
        Modelica.Blocks.Interfaces.RealOutput tau_sprocket_left annotation (Placement(
              transformation(extent={{402,200},{422,220}}), iconTransformation(extent=
                 {{402,200},{422,220}})));
        Modelica.Blocks.Interfaces.RealOutput tau_sprocket_right annotation (
            Placement(transformation(extent={{400,-220},{420,-200}}),
              iconTransformation(extent={{400,-220},{420,-200}})));
        Modelica.Blocks.Interfaces.RealOutput f_tractive
        "Tractive force generated at sprocket radius"   annotation (Placement(
              transformation(extent={{348,-74},{368,-54}}),
                                                         iconTransformation(extent={{348,-74},
                  {368,-54}})));
        Modelica.Blocks.Math.MultiSum multiSum(nu=2)
          annotation (Placement(transformation(extent={{284,-182},{296,-170}})));
        Modelica.Blocks.Math.Gain one_over_r(k=2/sprocket_diameter)
        "Divide by sprokect radius"
          annotation (Placement(transformation(extent={{320,-190},{340,-170}})));
        parameter SI.Diameter sprocket_diameter = 0.6
        "Diameter of sprocket used to calculate tractive force";
        parameter Boolean use_electrical_ground = true
        "true to add an electrical ground point";
        Modelica.Electrical.Analog.Basic.Ground ground if use_electrical_ground
          annotation (Placement(transformation(extent={{-370,-60},{-350,-40}})));
        Modelica.Electrical.Analog.Interfaces.NegativePin pin_n if use_electrical_ground
          annotation (Placement(transformation(extent={{-410,-30},{-390,-10}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_4(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_4_x,hub_spacing_4_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-72,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_4
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-72,-192}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-84,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_3(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_3_x,hub_spacing_3_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-26,-154})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_3
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-26,-186}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={54,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_10(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_4_x,hub_spacing_4_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={230,228})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_10
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={230,196}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-78,226})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_99(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_3_x,hub_spacing_3_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={294,230})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_9
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={294,198}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={64,226})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_5(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_5_x,hub_spacing_5_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-112,-158})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_5
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-112,-190}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-184,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_88(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_2_x,hub_spacing_2_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={360,230})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_8
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={360,198}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={160,226})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_2(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_2_x,hub_spacing_2_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={38,-154})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_2
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={38,-186}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={176,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_11(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_5_x,hub_spacing_5_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={192,228})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_11
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={192,196}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-198,226})));
        C2M2L_Ext.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_bus
          annotation (Placement(transformation(extent={{-410,50},{-390,70}})));
        Modelica.Mechanics.MultiBody.Sensors.AbsoluteVelocity absoluteVelocity2(
            resolveInFrame=Modelica.Mechanics.MultiBody.Types.ResolveInFrameA.frame_a)
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=180,
              origin={-142,76})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a idler1
          annotation (Placement(transformation(extent={{470,-100},{502,-68}}),
              iconTransformation(extent={{-382,138},{-350,170}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a idler
          annotation (Placement(transformation(extent={{94,-106},{126,-74}}),
              iconTransformation(extent={{-376,-156},{-344,-124}})));
      equation
        connect(susp_mount_1.frame_b, frame_susp_1) annotation (Line(
            points={{150,-170},{150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_6.frame_b,frame_susp_6)  annotation (Line(
            points={{-150,-170},{-150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount.frame_a, half_width_track.frame_b)
                                                         annotation (Line(
            points={{60,-90},{0,-90},{0,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track.frame_b, sprocket.bearingFrame)
                                                           annotation (Line(
            points={{20,14},{0,14},{0,-50},{110,-50}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_1.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{150,-150},{150,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_6.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-150,-150},{-148,-150},{-148,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_7.frame_b,frame_susp_7)  annotation (Line(
            points={{462,220},{462,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_12.frame_b, frame_susp_12)
                                                    annotation (Line(
            points={{162,220},{162,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount1.frame_a, half_width_track1.frame_b)
                                                         annotation (Line(
            points={{444,-84},{384,-84},{384,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track1.frame_b, sprocket1.bearingFrame)
                                                           annotation (Line(
            points={{402,32},{384,32},{384,-44},{494,-44}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_7.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{462,240},{462,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_12.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{162,240},{164,240},{164,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor3.flangeAndFrame, sprocket1) annotation (Line(
            points={{458,6},{476,6},{476,-44},{494,-44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor2.flangeAndFrame, sprocket) annotation (Line(
            points={{154,-26},{94,-26},{94,-50},{110,-50}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(left_power_in, adaptor.flangeAndFrame) annotation (Line(
            points={{-290,-10},{-276,-10},{-276,6},{358,6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(right_power_in, adaptor1.flangeAndFrame) annotation (Line(
            points={{-290,-150},{-240,-150},{-240,-212},{236,-212},{236,-26},{216,-26}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor1.frame, adaptor2.frame) annotation (Line(
            points={{206,-36},{164,-36}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idealGear.flange_b, adaptor3.flange) annotation (Line(
            points={{434,6},{448,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(adaptor.frame, adaptor3.frame) annotation (Line(
            points={{368,-4},{368,-14},{448,-14},{448,-4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(torqueSensor.flange_a, adaptor2.flange) annotation (Line(
            points={{174,-8},{170,-8},{170,-26},{164,-26}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor.flange_b, adaptor1.flange) annotation (Line(
            points={{194,-8},{200,-8},{200,-26},{206,-26}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.flange_b, idealGear.flange_a) annotation (Line(
            points={{406,6},{414,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.flange_a, adaptor.flange) annotation (Line(
            points={{386,6},{368,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.tau, tau_sprocket_left) annotation (Line(
            points={{388,17},{390,17},{390,210},{412,210}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor.tau, tau_sprocket_right) annotation (Line(
            points={{176,-19},{178,-19},{178,-210},{410,-210}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor.tau, multiSum.u[1]) annotation (Line(
            points={{176,-19},{178,-19},{178,-164},{284,-164},{284,-173.9}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor1.tau, multiSum.u[2]) annotation (Line(
            points={{388,17},{388,24},{252,24},{252,-178.1},{284,-178.1}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(multiSum.y, one_over_r.u) annotation (Line(
            points={{297.02,-176},{308,-176},{308,-180},{318,-180}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(one_over_r.y, f_tractive) annotation (Line(
            points={{341,-180},{255.5,-180},{255.5,-64},{358,-64}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(ground.p, pin_n) annotation (Line(
            points={{-360,-40},{-360,-20},{-400,-20}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(half_width_susp.frame_a, fixedTranslation1.frame_a) annotation (Line(
            points={{-82,20},{-20,20},{-20,40},{58,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_b, hull_mass.frame_a) annotation (Line(
            points={{78,40},{98,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track.frame_a, frame_a) annotation (Line(
            points={{40,14},{48,14},{48,40},{56,40},{56,40},{-62,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_track1.frame_a) annotation (
            Line(
            points={{58,40},{48,40},{48,54},{434,54},{434,32},{422,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, frame_a) annotation (Line(
            points={{58,40},{-62,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_susp1.frame_a) annotation (Line(
            points={{58,40},{48,40},{48,54},{434,54},{434,60},{420,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_4.frame_b,frame_susp_4)  annotation (Line(
            points={{-72,-170},{-72,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_4.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-72,-150},{-72,-138},{-102,-138},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_3.frame_b,frame_susp_3)  annotation (Line(
            points={{-26,-164},{-26,-186}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_3.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-26,-144},{-26,-132},{-102,-132},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_10.frame_b, frame_susp_10)
                                                    annotation (Line(
            points={{230,218},{230,196}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_10.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{230,238},{232,238},{232,258},{270,258},{270,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_99.frame_b, frame_susp_9)
                                                    annotation (Line(
            points={{294,220},{294,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_99.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{294,240},{296,240},{296,260},{334,260},{334,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_5.frame_b,frame_susp_5)  annotation (Line(
            points={{-112,-168},{-112,-190}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_5.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-112,-148},{-108,-148},{-108,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_88.frame_b, frame_susp_8)
                                                    annotation (Line(
            points={{360,220},{360,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_88.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{360,240},{362,240},{362,260},{400,260},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_2.frame_b, frame_susp_2) annotation (Line(
            points={{38,-164},{38,-186}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_2.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{38,-144},{-34,-144},{-34,-130},{-106,-130},{-106,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_11.frame_b, frame_susp_11)
                                                    annotation (Line(
            points={{192,218},{192,196}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_11.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{192,238},{194,238},{194,258},{232,258},{232,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(absoluteVelocity2.v[1],driver_bus. vehicle_speed) annotation (Line(
            points={{-153,76.6667},{-276,76.6667},{-276,60},{-400,60}},
            color={0,0,127},
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(frame_a,absoluteVelocity2. frame_a) annotation (Line(
            points={{-62,40},{-70,40},{-70,70},{-132,70},{-132,76}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount1.frame_b, idler1) annotation (Line(
            points={{464,-84},{486,-84}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler, idler_mount.frame_b) annotation (Line(
            points={{110,-90},{80,-90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(extent={{-400,-220},{560,280}},
                preserveAspectRatio=false,
              initialScale=0.2),           graphics), Icon(coordinateSystem(extent={{-400,
                  -220},{560,280}},        preserveAspectRatio=false,
              initialScale=0.2),                                      graphics={
              Rectangle(
                extent={{-400,0},{400,220}},
                lineColor={255,85,170},
                fillColor={215,215,215},
                fillPattern=FillPattern.Solid),
              Rectangle(
                extent={{-400,-220},{400,0}},
                lineColor={255,85,170},
                fillPattern=FillPattern.Solid,
                fillColor={215,215,215}),
              Text(
                extent={{-228,18},{252,78}},
                lineColor={0,0,255},
                textString="%name"),
              Ellipse(
                extent={{-30,30},{30,-30}},
                lineColor={0,0,255},
                fillColor={135,135,135},
                fillPattern=FillPattern.Solid,
                origin={370,-220},
                rotation=360),
              Text(
                extent={{332,-160},{412,-180}},
                lineColor={0,0,0},
                textString="sp"),
              Text(
                extent={{-410,-160},{-330,-180}},
                lineColor={0,0,0},
                textString="id"),
              Text(
                extent={{-44,-28},{220,-70}},
                lineColor={128,128,128},
                textString="{0,0,0}"),
              Line(
                points={{280,100},{370,100},{370,208}},
                color={95,95,95},
                smooth=Smooth.None,
                thickness=0.5),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={0,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={-260,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={240,-150},
                rotation=360),
              Text(
                extent={{404,230},{484,210}},
                lineColor={128,128,128},
                textString="tau_l"),
              Text(
                extent={{404,-212},{484,-232}},
                lineColor={128,128,128},
                textString="tau_r"),
              Text(
                extent={{326,46},{586,26}},
                lineColor={128,128,128},
                textString="f_tractive"),
              Text(
                extent={{310,106},{570,86}},
                lineColor={128,128,128},
                textString="bilge"),
              Line(
                points={{-390,-20},{-350,-20},{-350,-60}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-370,-60},{-330,-60}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-358,-80},{-342,-80}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-364,-70},{-334,-70}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{280,-100},{370,-100},{370,-208}},
                color={95,95,95},
                smooth=Smooth.None,
                thickness=0.5)}));
      end Full_hull_3DOF_6w_0g_no_speed_sensors_idler_frame;

      model Full_hull_3DOF_6w_3g_no_speed_sensors
      "A full hull with 3 degrees of freedom.   6 suspension attachments and 0guide wheel attachments each side."
        extends C2M2L_CyPhy.Testing_Blocks.Hull.Hull_Interface;

        parameter SI.Area frontal_area=16 "Frontal area of vehicle"
          annotation (Dialog(group="Aero drag"));
        parameter SI.Density rho_air = 1.16 "Nominal air density" annotation (Dialog(group="Aero drag"));
        parameter Real Cd = 1.25
        "Vehicle drag factor (Cd) in F = 0.5 * Cd * rho * v^2 * A"   annotation (Dialog(group="Aero drag"));
         parameter SI.Mass m_hull(min=0)=20000 "Total Vehicle Mass"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_xx_hull(min=0)=1e4 "Total Vehicle Roll Inertia"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_yy_hull(min=0)=1e4 "Total Vehicle Yaw Inertia"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_zz_hull(min=0)=3.5e4
        "Total Vehicle Pitch Inertia"
          annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_21_hull=0 " (2,1) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_31_hull=0 " (3,1) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_32_hull=0 " (3,2) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));

         parameter SI.Length CG_hull_x = -3.2
        "Horizontal distance to vehicle center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_hull_y = 0.2
        "Vertical distance to vehicle center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_driver_x = 0
        "Horizontal distance to driver center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_driver_y = 0
        "Vertical distance to driver center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));

        parameter SI.Length hub_spacing_1_x = -0.675
        "Horizontal distance to first suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 1 Location"));
         parameter SI.Length hub_spacing_1_y = -0.15
        "Vertical distance to first suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 1 Location"));

         parameter SI.Length hub_spacing_2_x = -1.475
        "Horizontal distance to second suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 2 Location"));
         parameter SI.Length hub_spacing_2_y = -0.15
        "Vertical distance to second suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 2 Location"));

         parameter SI.Length hub_spacing_3_x = -2.275
        "Horizontal distance to third suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 3 Location"));
         parameter SI.Length hub_spacing_3_y = -0.15
        "Vertical distance to third suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 3 Location"));

         parameter SI.Length hub_spacing_4_x = -3.075
        "Horizontal distance to fourth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 4 Location"));
         parameter SI.Length hub_spacing_4_y = -0.15
        "Vertical distance to fourth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 4 Location"));

         parameter SI.Length hub_spacing_5_x = -3.875
        "Horizontal distance to fifth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 5 Location"));
         parameter SI.Length hub_spacing_5_y = -0.15
        "Vertical distance to fifth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 5 Location"));

         parameter SI.Length hub_spacing_6_x = -4.675
        "Horizontal distance to sixth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 6 Location"));
         parameter SI.Length hub_spacing_6_y = -0.15
        "Vertical distance to sixth suspension attachment point on hull, measured from rear hitch point"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 6 Location"));
         parameter SI.Length idler_x = -6
        "Horizontal distance to idler wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Idler Location"));
         parameter SI.Length idler_y = 0.05
        "Vertical distance to idler wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Idler Location"));

         parameter SI.Length guide_1_x = -1.6
        "Horizontal distance to first guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 1 Location"));
         parameter SI.Length guide_1_y = 0.15
        "Vertical distance to first guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 1 Location"));

         parameter SI.Length guide_2_x = -3.2
        "Horizontal distance to second guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 2 Location"));
         parameter SI.Length guide_2_y = 0.15
        "Vertical distance to second guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 2 Location"));

         parameter SI.Length guide_3_x = -4.8
        "Horizontal distance to third guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 3 Location"));
         parameter SI.Length guide_3_y = 0.15
        "Vertical distance to third guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 3 Location"));

         parameter SI.Length cl_to_rhs = 1.5
        "Distance from center line to right hand side attachment points"
          annotation (Dialog(group="Vehicle width"));
         parameter SI.Length susp_offset = 0.15
        "Use same value as 'offset_wheel' in suspension module"
          annotation (Dialog(group="Vehicle width"));

        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_1(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_1_x,hub_spacing_1_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={150,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_1
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={150,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={280,-225})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        sprocket(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={110,-50}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={370,-220})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        idler(includeBearingConnector=
             true)                                              annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={110,-90}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-370,-220})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation idler_mount(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={idler_x,idler_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={70,-90})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque(
            tau_constant=0)
          annotation (Placement(transformation(extent={{160,-100},{140,-80}})));

        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_track(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,cl_to_rhs})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={30,14})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing left_power_in(
            includeBearingConnector=world.driveTrainMechanics3D)
                                          annotation (Placement(transformation(extent=
                 {{-300,-20},{-280,0}}), iconTransformation(
              extent={{-20,-20},{20,20}},
              rotation=0,
              origin={280,100})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing right_power_in(
            includeBearingConnector=world.driveTrainMechanics3D)
                                          annotation (Placement(transformation(extent=
                 {{-300,-160},{-280,-140}}), iconTransformation(
              extent={{-20,-20},{20,20}},
              rotation=0,
              origin={280,-100})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_6(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_6_x,hub_spacing_6_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-150,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_6
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-150,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-302,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_susp(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,cl_to_rhs - susp_offset})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-92,20})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_7(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_1_x,hub_spacing_1_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={462,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation idler_mount1(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={idler_x,idler_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={454,-84})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque1(
            tau_constant=0)
          annotation (Placement(transformation(extent={{544,-94},{524,-74}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_track1(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,-cl_to_rhs})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={412,32})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_12(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_6_x,hub_spacing_6_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={162,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_susp1(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,-(cl_to_rhs - susp_offset)})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={410,60})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_7
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={462,198}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={280,226})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        sprocket1(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={494,-44}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={370,220})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        idler1(
                                                              includeBearingConnector=
             true)                                              annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={494,-84}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-370,220})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_12
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={162,198}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-308,226})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{358,-4},{378,16}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor1(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{-10,10},{10,-10}},
              rotation=180,
              origin={206,-26})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor2(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{154,-36},{174,-16}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor3(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{458,-4},{438,16}})));
        Modelica.Mechanics.Rotational.Components.IdealGear idealGear(ratio=-1)
          annotation (Placement(transformation(extent={{414,-4},{434,16}})));
        replaceable package Bilge_Medium =
            C2M2L_Ext.Media.Incompressible.Water_Incompressible_Constant;
        parameter SI.Area bilge_area=20 "Area of tank";
        parameter Modelica.Media.Interfaces.PartialMedium.MassFlowRate leak_flow=0
        "Fixed mass flow rate going out of the fluid port";
          parameter SI.Height bilge_initial=0.05
        "Initial level of water in the bilge"
          annotation (Dialog(group="Bilge"));
        Modelica.Mechanics.Rotational.Sensors.TorqueSensor torqueSensor
          annotation (Placement(transformation(extent={{174,-18},{194,2}})));
        Modelica.Mechanics.Rotational.Sensors.TorqueSensor torqueSensor1
          annotation (Placement(transformation(extent={{386,16},{406,-4}})));
        Modelica.Blocks.Interfaces.RealOutput tau_sprocket_left annotation (Placement(
              transformation(extent={{402,200},{422,220}}), iconTransformation(extent=
                 {{402,200},{422,220}})));
        Modelica.Blocks.Interfaces.RealOutput tau_sprocket_right annotation (
            Placement(transformation(extent={{400,-220},{420,-200}}),
              iconTransformation(extent={{400,-220},{420,-200}})));
        Modelica.Blocks.Interfaces.RealOutput f_tractive
        "Tractive force generated at sprocket radius"   annotation (Placement(
              transformation(extent={{364,-162},{384,-142}}),
                                                         iconTransformation(extent={{364,
                  -162},{384,-142}})));
        Modelica.Blocks.Math.MultiSum multiSum(nu=2)
          annotation (Placement(transformation(extent={{284,-182},{296,-170}})));
        Modelica.Blocks.Math.Gain one_over_r(k=2/sprocket_diameter)
        "Divide by sprokect radius"
          annotation (Placement(transformation(extent={{320,-190},{340,-170}})));
        parameter SI.Diameter sprocket_diameter = 0.6
        "Diameter of sprocket used to calculate tractive force";
        parameter Boolean use_electrical_ground = true
        "true to add an electrical ground point";
        Modelica.Electrical.Analog.Basic.Ground ground if use_electrical_ground
          annotation (Placement(transformation(extent={{-370,-60},{-350,-40}})));
        Modelica.Electrical.Analog.Interfaces.NegativePin pin_n if use_electrical_ground
          annotation (Placement(transformation(extent={{-410,-30},{-390,-10}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_4(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_4_x,hub_spacing_4_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-72,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_4
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-72,-192}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-84,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_3(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_3_x,hub_spacing_3_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-26,-154})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_3
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-26,-186}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={54,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_10(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_4_x,hub_spacing_4_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={230,228})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_10
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={230,196}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-78,226})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_99(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_3_x,hub_spacing_3_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={294,230})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_9
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={294,198}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={64,226})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_5(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_5_x,hub_spacing_5_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-112,-158})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_5
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-112,-190}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-184,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_88(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_2_x,hub_spacing_2_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={360,230})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_8
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={360,198}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={160,226})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_2(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_2_x,hub_spacing_2_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={38,-154})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_2
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={38,-186}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={176,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_11(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_5_x,hub_spacing_5_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={192,228})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_11
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={192,196}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-198,226})));
        C2M2L_Ext.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_bus
          annotation (Placement(transformation(extent={{-410,50},{-390,70}})));
        Modelica.Mechanics.MultiBody.Sensors.AbsoluteVelocity absoluteVelocity2(
            resolveInFrame=Modelica.Mechanics.MultiBody.Types.ResolveInFrameA.frame_a)
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=180,
              origin={-142,76})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_1(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_1_x,guide_1_y,0})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-20,-14})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_1(
            tau_constant=0)
          annotation (Placement(transformation(extent={{-96,-24},{-76,-4}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_2(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_2_x,guide_2_y,0})
          annotation (Placement(transformation(extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-20,-54})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_3(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_3_x,guide_3_y,0})
          annotation (Placement(transformation(extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-20,-94})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_2(
            tau_constant=0)
          annotation (Placement(transformation(extent={{-94,-64},{-74,-44}})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_3(
            tau_constant=0)
          annotation (Placement(transformation(extent={{-94,-104},{-74,-84}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_1(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-52,-14}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={230,-180})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_2(
            includeBearingConnector=true)                                 annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-52,-54}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-20,-180})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        guide_3(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-52,-94}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-270,-180})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_4(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_1_x,guide_1_y,0})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={344,-38})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_4(
            tau_constant=0)
          annotation (Placement(transformation(extent={{258,-48},{278,-28}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_5(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_2_x,guide_2_y,0})
          annotation (Placement(transformation(extent={{10,-10},{-10,10}},
              rotation=0,
              origin={344,-78})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_6(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_3_x,guide_3_y,0})
          annotation (Placement(transformation(extent={{10,-10},{-10,10}},
              rotation=0,
              origin={344,-118})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_5(
            tau_constant=0)
          annotation (Placement(transformation(extent={{260,-88},{280,-68}})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_6(
            tau_constant=0)
          annotation (Placement(transformation(extent={{260,-128},{280,-108}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_4(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={304,-38}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={230,180})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_5(
            includeBearingConnector=true)                                 annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={304,-78}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-20,180})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        guide_6(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={304,-118}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-270,180})));
      equation
        connect(susp_mount_1.frame_b, frame_susp_1) annotation (Line(
            points={{150,-170},{150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_6.frame_b,frame_susp_6)  annotation (Line(
            points={{-150,-170},{-150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler, idler) annotation (Line(
            points={{110,-90},{110,-90}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque.flange, idler.flange) annotation (Line(
            points={{140,-90},{110,-90}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(idler_mount.frame_b, idler.bearingFrame) annotation (Line(
            points={{80,-90},{110,-90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount.frame_a, half_width_track.frame_b)
                                                         annotation (Line(
            points={{60,-90},{0,-90},{0,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track.frame_b, sprocket.bearingFrame)
                                                           annotation (Line(
            points={{20,14},{0,14},{0,-50},{110,-50}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_1.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{150,-150},{150,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_6.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-150,-150},{-148,-150},{-148,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_7.frame_b,frame_susp_7)  annotation (Line(
            points={{462,220},{462,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_12.frame_b, frame_susp_12)
                                                    annotation (Line(
            points={{162,220},{162,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler1, idler1)
                              annotation (Line(
            points={{494,-84},{494,-84}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque1.flange, idler1.flange)
                                                     annotation (Line(
            points={{524,-84},{494,-84}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(idler_mount1.frame_b, idler1.bearingFrame)
                                                         annotation (Line(
            points={{464,-84},{494,-84}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount1.frame_a, half_width_track1.frame_b)
                                                         annotation (Line(
            points={{444,-84},{384,-84},{384,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track1.frame_b, sprocket1.bearingFrame)
                                                           annotation (Line(
            points={{402,32},{384,32},{384,-44},{494,-44}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_7.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{462,240},{462,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_12.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{162,240},{164,240},{164,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor3.flangeAndFrame, sprocket1) annotation (Line(
            points={{458,6},{476,6},{476,-44},{494,-44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor2.flangeAndFrame, sprocket) annotation (Line(
            points={{154,-26},{94,-26},{94,-50},{110,-50}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(left_power_in, adaptor.flangeAndFrame) annotation (Line(
            points={{-290,-10},{-276,-10},{-276,6},{358,6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(right_power_in, adaptor1.flangeAndFrame) annotation (Line(
            points={{-290,-150},{-240,-150},{-240,-212},{236,-212},{236,-26},{216,-26}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor1.frame, adaptor2.frame) annotation (Line(
            points={{206,-36},{164,-36}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idealGear.flange_b, adaptor3.flange) annotation (Line(
            points={{434,6},{448,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(adaptor.frame, adaptor3.frame) annotation (Line(
            points={{368,-4},{368,-14},{448,-14},{448,-4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(torqueSensor.flange_a, adaptor2.flange) annotation (Line(
            points={{174,-8},{170,-8},{170,-26},{164,-26}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor.flange_b, adaptor1.flange) annotation (Line(
            points={{194,-8},{200,-8},{200,-26},{206,-26}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.flange_b, idealGear.flange_a) annotation (Line(
            points={{406,6},{414,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.flange_a, adaptor.flange) annotation (Line(
            points={{386,6},{368,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.tau, tau_sprocket_left) annotation (Line(
            points={{388,17},{390,17},{390,210},{412,210}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor.tau, tau_sprocket_right) annotation (Line(
            points={{176,-19},{178,-19},{178,-210},{410,-210}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor.tau, multiSum.u[1]) annotation (Line(
            points={{176,-19},{178,-19},{178,-164},{284,-164},{284,-173.9}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor1.tau, multiSum.u[2]) annotation (Line(
            points={{388,17},{388,24},{252,24},{252,-178.1},{284,-178.1}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(multiSum.y, one_over_r.u) annotation (Line(
            points={{297.02,-176},{308,-176},{308,-180},{318,-180}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(one_over_r.y, f_tractive) annotation (Line(
            points={{341,-180},{255.5,-180},{255.5,-152},{374,-152}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(ground.p, pin_n) annotation (Line(
            points={{-360,-40},{-360,-20},{-400,-20}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(half_width_susp.frame_a, fixedTranslation1.frame_a) annotation (Line(
            points={{-82,20},{-20,20},{-20,40},{58,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_b, hull_mass.frame_a) annotation (Line(
            points={{78,40},{98,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track.frame_a, frame_a) annotation (Line(
            points={{40,14},{48,14},{48,40},{-62,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_track1.frame_a) annotation (
            Line(
            points={{58,40},{48,40},{48,54},{434,54},{434,32},{422,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, frame_a) annotation (Line(
            points={{58,40},{-62,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_susp1.frame_a) annotation (Line(
            points={{58,40},{48,40},{48,54},{434,54},{434,60},{420,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_4.frame_b,frame_susp_4)  annotation (Line(
            points={{-72,-170},{-72,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_4.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-72,-150},{-72,-138},{-102,-138},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_3.frame_b,frame_susp_3)  annotation (Line(
            points={{-26,-164},{-26,-186}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_3.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-26,-144},{-26,-132},{-102,-132},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_10.frame_b, frame_susp_10)
                                                    annotation (Line(
            points={{230,218},{230,196}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_10.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{230,238},{232,238},{232,258},{270,258},{270,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_99.frame_b, frame_susp_9)
                                                    annotation (Line(
            points={{294,220},{294,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_99.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{294,240},{296,240},{296,260},{334,260},{334,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_5.frame_b,frame_susp_5)  annotation (Line(
            points={{-112,-168},{-112,-190}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_5.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-112,-148},{-108,-148},{-108,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_88.frame_b, frame_susp_8)
                                                    annotation (Line(
            points={{360,220},{360,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_88.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{360,240},{362,240},{362,260},{400,260},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_2.frame_b, frame_susp_2) annotation (Line(
            points={{38,-164},{38,-186}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_2.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{38,-144},{-34,-144},{-34,-130},{-106,-130},{-106,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_11.frame_b, frame_susp_11)
                                                    annotation (Line(
            points={{192,218},{192,196}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_11.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{192,238},{194,238},{194,258},{232,258},{232,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(absoluteVelocity2.v[1],driver_bus. vehicle_speed) annotation (Line(
            points={{-153,76.6667},{-276,76.6667},{-276,60},{-400,60}},
            color={0,0,127},
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(frame_a,absoluteVelocity2. frame_a) annotation (Line(
            points={{-62,40},{-70,40},{-70,70},{-132,70},{-132,76}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque_guide_1.flange,guide_1. flange)
                                                           annotation (Line(
            points={{-76,-14},{-52,-14}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(guide_1.bearingFrame,guide_mount_1. frame_b)
                                                         annotation (Line(
            points={{-52,-14},{-30,-14}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_2.frame_b,guide_2. bearingFrame) annotation (Line(
            points={{-30,-54},{-52,-54}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_3.frame_b,guide_3. bearingFrame) annotation (Line(
            points={{-30,-94},{-52,-94}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque_guide_2.flange,guide_2. flange) annotation (Line(
            points={{-74,-54},{-52,-54}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(constantTorque_guide_3.flange,guide_3. flange) annotation (Line(
            points={{-74,-94},{-52,-94}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(guide_mount_1.frame_a, half_width_track.frame_b) annotation (Line(
            points={{-10,-14},{6,-14},{6,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_2.frame_a, half_width_track.frame_b) annotation (Line(
            points={{-10,-54},{4,-54},{4,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_3.frame_a, half_width_track.frame_b) annotation (Line(
            points={{-10,-94},{6,-94},{6,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque_guide_4.flange,guide_4. flange)
                                                           annotation (Line(
            points={{278,-38},{304,-38}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(guide_4.bearingFrame,guide_mount_4. frame_b)
                                                         annotation (Line(
            points={{304,-38},{334,-38}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_5.frame_b,guide_5. bearingFrame) annotation (Line(
            points={{334,-78},{304,-78}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_6.frame_b,guide_6. bearingFrame) annotation (Line(
            points={{334,-118},{304,-118}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque_guide_5.flange,guide_5. flange) annotation (Line(
            points={{280,-78},{304,-78}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(constantTorque_guide_6.flange,guide_6. flange) annotation (Line(
            points={{280,-118},{304,-118}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(guide_mount_4.frame_a, half_width_track1.frame_b)
                                                           annotation (Line(
            points={{354,-38},{378,-38},{378,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_5.frame_a, half_width_track1.frame_b)
                                                           annotation (Line(
            points={{354,-78},{378,-78},{378,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_6.frame_a, half_width_track1.frame_b)
                                                           annotation (Line(
            points={{354,-118},{378,-118},{378,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(extent={{-400,-220},{560,280}},
                preserveAspectRatio=false,
              initialScale=0.2),           graphics), Icon(coordinateSystem(extent={{-400,
                  -220},{560,280}},        preserveAspectRatio=false,
              initialScale=0.2),                                      graphics={
              Rectangle(
                extent={{-400,0},{400,220}},
                lineColor={255,85,170},
                fillColor={215,215,215},
                fillPattern=FillPattern.Solid),
              Rectangle(
                extent={{-400,-220},{400,0}},
                lineColor={255,85,170},
                fillPattern=FillPattern.Solid,
                fillColor={215,215,215}),
              Text(
                extent={{-228,18},{252,78}},
                lineColor={0,0,255},
                textString="%name"),
              Ellipse(
                extent={{-30,30},{30,-30}},
                lineColor={0,0,255},
                fillColor={135,135,135},
                fillPattern=FillPattern.Solid,
                origin={370,-220},
                rotation=360),
              Text(
                extent={{332,-160},{412,-180}},
                lineColor={0,0,0},
                textString="sp"),
              Text(
                extent={{-410,-160},{-330,-180}},
                lineColor={0,0,0},
                textString="id"),
              Text(
                extent={{-44,-28},{220,-70}},
                lineColor={128,128,128},
                textString="{0,0,0}"),
              Line(
                points={{280,100},{370,100},{370,208}},
                color={95,95,95},
                smooth=Smooth.None,
                thickness=0.5),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={0,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={-260,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={240,-150},
                rotation=360),
              Text(
                extent={{404,230},{484,210}},
                lineColor={128,128,128},
                textString="tau_l"),
              Text(
                extent={{404,-212},{484,-232}},
                lineColor={128,128,128},
                textString="tau_r"),
              Text(
                extent={{326,46},{586,26}},
                lineColor={128,128,128},
                textString="f_tractive"),
              Text(
                extent={{310,106},{570,86}},
                lineColor={128,128,128},
                textString="bilge"),
              Line(
                points={{-390,-20},{-350,-20},{-350,-60}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-370,-60},{-330,-60}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-358,-80},{-342,-80}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-364,-70},{-334,-70}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{280,-100},{370,-100},{370,-208}},
                color={95,95,95},
                smooth=Smooth.None,
                thickness=0.5)}));
      end Full_hull_3DOF_6w_3g_no_speed_sensors;

      model Full_hull_3DOF_6w_3g_no_speed_sensors_idler_frame
      "A full hull with 3 degrees of freedom.   6 suspension attachments and 3 guide wheel attachments each side."
        import C2M2L_Ext;
        extends C2M2L_Ext.Testing_Blocks.Hull.Hull_Interface;

         parameter SI.Length CG_driver_x = 0
        "Horizontal distance to driver center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_driver_y = 0
        "Vertical distance to driver center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));

        parameter SI.Length hub_spacing_1_x = -0.675
        "Horizontal distance to first suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 1 Location"));
         parameter SI.Length hub_spacing_1_y = -0.15
        "Vertical distance to first suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 1 Location"));

         parameter SI.Length hub_spacing_2_x = -1.475
        "Horizontal distance to second suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 2 Location"));
         parameter SI.Length hub_spacing_2_y = -0.15
        "Vertical distance to second suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 2 Location"));

         parameter SI.Length hub_spacing_3_x = -2.275
        "Horizontal distance to third suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 3 Location"));
         parameter SI.Length hub_spacing_3_y = -0.15
        "Vertical distance to third suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 3 Location"));

         parameter SI.Length hub_spacing_4_x = -3.075
        "Horizontal distance to fourth suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 4 Location"));
         parameter SI.Length hub_spacing_4_y = -0.15
        "Vertical distance to fourth suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 4 Location"));

         parameter SI.Length hub_spacing_5_x = -3.875
        "Horizontal distance to fifth suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 5 Location"));
         parameter SI.Length hub_spacing_5_y = -0.15
        "Vertical distance to fifth suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 5 Location"));

         parameter SI.Length hub_spacing_6_x = -4.675
        "Horizontal distance to sixth suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 6 Location"));
         parameter SI.Length hub_spacing_6_y = -0.15
        "Vertical distance to sixth suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 6 Location"));
         parameter SI.Length idler_x = -6
        "Horizontal distance to idler wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Idler Location"));
         parameter SI.Length idler_y = 0.05
        "Vertical distance to idler wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Idler Location"));

         parameter SI.Length guide_1_x = -1.6
        "Horizontal distance to first guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 1 Location"));
         parameter SI.Length guide_1_y = 0.15
        "Vertical distance to first guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 1 Location"));

         parameter SI.Length guide_2_x = -3.2
        "Horizontal distance to second guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 2 Location"));
         parameter SI.Length guide_2_y = 0.15
        "Vertical distance to second guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 2 Location"));

         parameter SI.Length guide_3_x = -4.8
        "Horizontal distance to third guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 3 Location"));
         parameter SI.Length guide_3_y = 0.15
        "Vertical distance to third guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 3 Location"));

         parameter SI.Length cl_to_rhs = 1.5
        "Distance from center line to right hand side attachment points"
          annotation (Dialog(group="Vehicle width"));
         parameter SI.Length susp_offset = 0.10
        "Use same value as 'offset_wheel' in suspension module"
          annotation (Dialog(group="Vehicle width"));

        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_1(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_1_x,hub_spacing_1_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={150,-160})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_2(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_2_x,hub_spacing_2_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={90,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_1
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={150,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={280,-225})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_2
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={90,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={180,-225})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        sprocket(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={110,-50}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={370,-220})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation idler_mount(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={idler_x,idler_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={24,-92})));

        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_track(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,cl_to_rhs})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={30,14})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing left_power_in(
            includeBearingConnector=world.driveTrainMechanics3D)
                                          annotation (Placement(transformation(extent=
                 {{-300,-20},{-280,0}}), iconTransformation(
              extent={{-20,-20},{20,20}},
              rotation=0,
              origin={280,100})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing right_power_in(
            includeBearingConnector=world.driveTrainMechanics3D)
                                          annotation (Placement(transformation(extent={{-298,
                  -160},{-278,-140}}),       iconTransformation(
              extent={{-20,-20},{20,20}},
              rotation=0,
              origin={280,-100})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_3(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_3_x,hub_spacing_3_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={30,-160})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_4(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_4_x,hub_spacing_4_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-30,-160})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_5(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_5_x,hub_spacing_5_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-90,-160})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_6(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_6_x,hub_spacing_6_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-150,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_3
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={30,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={80,-225})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_4
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-30,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-120,-225})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_5
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-90,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-220,-225})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_6
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-150,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-320,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_1(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_1_x,guide_1_y,0})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-18,-30})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_1(
            tau_constant=0)
          annotation (Placement(transformation(extent={{-94,-40},{-74,-20}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_2(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_2_x,guide_2_y,0})
          annotation (Placement(transformation(extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-18,-70})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_3(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_3_x,guide_3_y,0})
          annotation (Placement(transformation(extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-18,-110})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_2(
            tau_constant=0)
          annotation (Placement(transformation(extent={{-92,-80},{-72,-60}})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_3(
            tau_constant=0)
          annotation (Placement(transformation(extent={{-92,-120},{-72,-100}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_1(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-50,-30}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={230,-180})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_2(
            includeBearingConnector=true)                                 annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-50,-70}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-20,-180})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        guide_3(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-50,-110}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-270,-180})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_susp(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,cl_to_rhs - susp_offset})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-92,20})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_7(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_1_x,hub_spacing_1_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={462,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_8(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_2_x,hub_spacing_2_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={402,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation idler_mount1(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={idler_x,idler_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={454,-84})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_track1(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,-cl_to_rhs})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={412,32})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_9(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_3_x,hub_spacing_3_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={342,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_10(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_4_x,hub_spacing_4_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={282,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_11(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_5_x,hub_spacing_5_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={222,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_12(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_6_x,hub_spacing_6_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={162,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_4(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_1_x,guide_1_y,0})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={350,-24})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_4(
            tau_constant=0)
          annotation (Placement(transformation(extent={{264,-34},{284,-14}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_5(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_2_x,guide_2_y,0})
          annotation (Placement(transformation(extent={{10,-10},{-10,10}},
              rotation=0,
              origin={350,-64})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_6(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_3_x,guide_3_y,0})
          annotation (Placement(transformation(extent={{10,-10},{-10,10}},
              rotation=0,
              origin={350,-104})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_5(
            tau_constant=0)
          annotation (Placement(transformation(extent={{266,-74},{286,-54}})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_6(
            tau_constant=0)
          annotation (Placement(transformation(extent={{266,-114},{286,-94}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_susp1(
          specularCoefficient=0.3,
          color={255,0,0},
          animation=world.enableAnimation,
          r={0,0,-(cl_to_rhs - susp_offset)})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={410,60})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_7
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={462,198}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={280,226})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_8
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={402,198}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={180,226})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        sprocket1(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={494,-44}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={370,220})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_9
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={342,198}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={80,226})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_10
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={282,198}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-120,226})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_11
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={222,198}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-220,226})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_12
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={162,198}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-320,226})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_4(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={310,-24}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={230,180})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_5(
            includeBearingConnector=true)                                 annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={310,-64}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-20,180})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        guide_6(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={310,-104}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-270,180})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{358,-4},{378,16}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor1(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{-10,10},{10,-10}},
              rotation=180,
              origin={206,-26})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor2(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{154,-36},{174,-16}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor3(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{458,-4},{438,16}})));
        Modelica.Mechanics.Rotational.Components.IdealGear idealGear(ratio=-1)
          annotation (Placement(transformation(extent={{414,-4},{434,16}})));
        Modelica.Fluid.Vessels.OpenTank
                                tank(
          redeclare package Medium = Bilge_Medium,
          energyDynamics=Modelica.Fluid.Types.Dynamics.DynamicFreeInitial,
          massDynamics=Modelica.Fluid.Types.Dynamics.DynamicFreeInitial,
          portsData={Modelica.Fluid.Vessels.BaseClasses.VesselPortsData(diameter=0.01,
              height=0.7),Modelica.Fluid.Vessels.BaseClasses.VesselPortsData(diameter=0.01)},
          use_portsData=false,
          nPorts=2,
          redeclare model HeatTransfer =
              Modelica.Fluid.Vessels.BaseClasses.HeatTransfer.IdealHeatTransfer
            (                                                                    k=
                  100),
          use_HeatTransfer=false,
          height=100,
          crossArea=bilge_area,
          level_start=bilge_initial)
          annotation (Placement(transformation(extent={{384,122},{344,162}})));
        Modelica.Fluid.Sources.MassFlowSource_T boundary(
          redeclare package Medium = Bilge_Medium,
          nPorts=1,
          m_flow=leak_flow)
          annotation (Placement(transformation(extent={{326,74},{346,94}})));
        Modelica.Fluid.Interfaces.FluidPort_a port_a(redeclare package Medium
          =   Bilge_Medium)
          annotation (Placement(transformation(extent={{396,72},{416,92}})));
        package Bilge_Medium =
            C2M2L_Ext.Media.Incompressible.Water_Incompressible_Constant;
        parameter SI.Area bilge_area=20 "Area of tank";
        parameter Modelica.Media.Interfaces.PartialMedium.MassFlowRate leak_flow=0
        "Fixed mass flow rate going out of the fluid port";
          parameter SI.Height bilge_initial=0.05
        "Initial level of water in the bilge"
          annotation (Dialog(group="Bilge"));
        Modelica.Mechanics.Rotational.Sensors.TorqueSensor torqueSensor
          annotation (Placement(transformation(extent={{174,-18},{194,2}})));
        Modelica.Mechanics.Rotational.Sensors.TorqueSensor torqueSensor1
          annotation (Placement(transformation(extent={{386,16},{406,-4}})));
        Modelica.Blocks.Interfaces.RealOutput tau_sprocket_left annotation (Placement(
              transformation(extent={{402,200},{422,220}}), iconTransformation(extent=
                 {{402,200},{422,220}})));
        Modelica.Blocks.Interfaces.RealOutput tau_sprocket_right annotation (
            Placement(transformation(extent={{400,-220},{420,-200}}),
              iconTransformation(extent={{400,-220},{420,-200}})));
        Modelica.Blocks.Interfaces.RealOutput f_tractive
        "Tractive force generated at sprocket radius"   annotation (Placement(
              transformation(extent={{402,10},{422,30}}),iconTransformation(extent={{402,10},
                  {422,30}})));
        Modelica.Blocks.Math.MultiSum multiSum(nu=2)
          annotation (Placement(transformation(extent={{284,-182},{296,-170}})));
        Modelica.Blocks.Math.Gain one_over_r(k=2/sprocket_diameter)
        "Divide by sprokect radius"
          annotation (Placement(transformation(extent={{320,-190},{340,-170}})));
        parameter SI.Diameter sprocket_diameter = 0.6
        "Diameter of sprocket used to calculate tractive force";
        parameter Boolean use_electrical_ground = true
        "true to add an electrical ground point";
        Modelica.Electrical.Analog.Basic.Ground ground if use_electrical_ground
          annotation (Placement(transformation(extent={{-370,-60},{-350,-40}})));
        Modelica.Electrical.Analog.Interfaces.NegativePin pin_n if use_electrical_ground
          annotation (Placement(transformation(extent={{-410,-30},{-390,-10}})));
        Modelica.Mechanics.MultiBody.Visualizers.FixedFrame fixedFrame
          annotation (Placement(transformation(extent={{516,160},{536,180}})));
        Modelica.Mechanics.MultiBody.Visualizers.FixedFrame fixedFrame1
          annotation (Placement(transformation(extent={{158,-272},{178,-252}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b idler   annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={106,-90}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-372,-187})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b idler1  annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={494,-84}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-360,169})));
        Modelica.Mechanics.MultiBody.Visualizers.FixedShape fixedShape(
          lengthDirection={-1,0,0},
          width=Total_width,
          height=Total_height,
          color={11,72,107},
          animation=world.enableAnimation,
          widthDirection={0,0,1},
          length=Total_length)
          annotation (Placement(transformation(extent={{46,82},{66,102}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation2(animation=
             false, r={Total_length*0.2,Total_height*0.5,0})
                  annotation (Placement(transformation(extent={{-62,50},{-42,70}})));
        Modelica.Mechanics.MultiBody.Sensors.AbsoluteVelocity absoluteVelocity2(
            resolveInFrame=Modelica.Mechanics.MultiBody.Types.ResolveInFrameA.frame_a)
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=180,
              origin={-150,66})));
        C2M2L_Ext.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_bus
          annotation (Placement(transformation(extent={{-410,50},{-390,70}})));
      equation
        connect(susp_mount_2.frame_b, frame_susp_2) annotation (Line(
            points={{90,-170},{90,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_1.frame_b, frame_susp_1) annotation (Line(
            points={{150,-170},{150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_3.frame_b,frame_susp_3)  annotation (Line(
            points={{30,-170},{30,-181},{30,-181},{30,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_4.frame_b,frame_susp_4)  annotation (Line(
            points={{-30,-170},{-30,-181},{-30,-181},{-30,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_5.frame_b,frame_susp_5)  annotation (Line(
            points={{-90,-170},{-90,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_6.frame_b,frame_susp_6)  annotation (Line(
            points={{-150,-170},{-150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque_guide_1.flange, guide_1.flange)
                                                           annotation (Line(
            points={{-74,-30},{-50,-30}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(guide_1.bearingFrame,guide_mount_1. frame_b)
                                                         annotation (Line(
            points={{-50,-30},{-28,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_2.frame_b,guide_2. bearingFrame) annotation (Line(
            points={{-28,-70},{-50,-70}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_3.frame_b,guide_3. bearingFrame) annotation (Line(
            points={{-28,-110},{-50,-110}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque_guide_2.flange,guide_2. flange) annotation (Line(
            points={{-72,-70},{-50,-70}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(constantTorque_guide_3.flange,guide_3. flange) annotation (Line(
            points={{-72,-110},{-50,-110}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(idler_mount.frame_a, half_width_track.frame_b)
                                                         annotation (Line(
            points={{14,-92},{0,-92},{0,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track.frame_b, sprocket.bearingFrame)
                                                           annotation (Line(
            points={{20,14},{0,14},{0,-50},{110,-50}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_1.frame_a, half_width_track.frame_b)
                                                           annotation (Line(
            points={{-8,-30},{0,-30},{0,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_2.frame_a, half_width_track.frame_b)
                                                           annotation (Line(
            points={{-8,-70},{0,-70},{0,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_3.frame_a, half_width_track.frame_b)
                                                           annotation (Line(
            points={{-8,-110},{0,-110},{0,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_1.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{150,-150},{150,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_2.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{90,-150},{90,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_3.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{30,-150},{30,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_4.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-30,-150},{-30,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_5.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-90,-150},{-90,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_6.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-150,-150},{-148,-150},{-148,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_8.frame_b,frame_susp_8)  annotation (Line(
            points={{402,220},{402,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_7.frame_b,frame_susp_7)  annotation (Line(
            points={{462,220},{462,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_9.frame_b,frame_susp_9)  annotation (Line(
            points={{342,220},{342,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_10.frame_b, frame_susp_10)
                                                    annotation (Line(
            points={{282,220},{282,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_11.frame_b, frame_susp_11)
                                                    annotation (Line(
            points={{222,220},{222,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_12.frame_b, frame_susp_12)
                                                    annotation (Line(
            points={{162,220},{162,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque_guide_4.flange,guide_4. flange)
                                                           annotation (Line(
            points={{284,-24},{310,-24}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(guide_4.bearingFrame,guide_mount_4. frame_b)
                                                         annotation (Line(
            points={{310,-24},{340,-24}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_5.frame_b,guide_5. bearingFrame) annotation (Line(
            points={{340,-64},{310,-64}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_6.frame_b,guide_6. bearingFrame) annotation (Line(
            points={{340,-104},{310,-104}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque_guide_5.flange,guide_5. flange) annotation (Line(
            points={{286,-64},{310,-64}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(constantTorque_guide_6.flange,guide_6. flange) annotation (Line(
            points={{286,-104},{310,-104}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(idler_mount1.frame_a, half_width_track1.frame_b)
                                                         annotation (Line(
            points={{444,-84},{384,-84},{384,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track1.frame_b, sprocket1.bearingFrame)
                                                           annotation (Line(
            points={{402,32},{384,32},{384,-44},{494,-44}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_4.frame_a, half_width_track1.frame_b)
                                                           annotation (Line(
            points={{360,-24},{384,-24},{384,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_5.frame_a, half_width_track1.frame_b)
                                                           annotation (Line(
            points={{360,-64},{384,-64},{384,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_6.frame_a, half_width_track1.frame_b)
                                                           annotation (Line(
            points={{360,-104},{384,-104},{384,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_7.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{462,240},{462,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_8.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{402,240},{402,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_9.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{342,240},{342,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_10.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{282,240},{282,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_11.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{222,240},{222,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_12.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{162,240},{164,240},{164,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor3.flangeAndFrame, sprocket1) annotation (Line(
            points={{458,6},{476,6},{476,-44},{494,-44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor2.flangeAndFrame, sprocket) annotation (Line(
            points={{154,-26},{94,-26},{94,-50},{110,-50}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(left_power_in, adaptor.flangeAndFrame) annotation (Line(
            points={{-290,-10},{-276,-10},{-276,6},{358,6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(right_power_in, adaptor1.flangeAndFrame) annotation (Line(
            points={{-288,-150},{-242,-150},{-242,-208},{234,-208},{234,-26},{216,-26}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor1.frame, adaptor2.frame) annotation (Line(
            points={{206,-36},{164,-36}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(boundary.ports[1],tank. ports[1]) annotation (Line(
            points={{346,84},{368,84},{368,122}},
            color={0,127,255},
            smooth=Smooth.None));
        connect(port_a,tank. ports[2]) annotation (Line(
            points={{406,82},{388,82},{388,122},{360,122}},
            color={0,127,255},
            smooth=Smooth.None));
        connect(idealGear.flange_b, adaptor3.flange) annotation (Line(
            points={{434,6},{442,6},{442,6},{448,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(adaptor.frame, adaptor3.frame) annotation (Line(
            points={{368,-4},{368,-14},{448,-14},{448,-4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(torqueSensor.flange_a, adaptor2.flange) annotation (Line(
            points={{174,-8},{170,-8},{170,-26},{164,-26}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor.flange_b, adaptor1.flange) annotation (Line(
            points={{194,-8},{200,-8},{200,-26},{206,-26}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.flange_b, idealGear.flange_a) annotation (Line(
            points={{406,6},{414,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.flange_a, adaptor.flange) annotation (Line(
            points={{386,6},{368,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.tau, tau_sprocket_left) annotation (Line(
            points={{388,17},{390,17},{390,210},{412,210}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor.tau, tau_sprocket_right) annotation (Line(
            points={{176,-19},{178,-19},{178,-210},{410,-210}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor.tau, multiSum.u[1]) annotation (Line(
            points={{176,-19},{178,-19},{178,-164},{284,-164},{284,-173.9}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor1.tau, multiSum.u[2]) annotation (Line(
            points={{388,17},{388,24},{252,24},{252,-178.1},{284,-178.1}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(multiSum.y, one_over_r.u) annotation (Line(
            points={{297.02,-176},{308,-176},{308,-180},{318,-180}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(one_over_r.y, f_tractive) annotation (Line(
            points={{341,-180},{255.5,-180},{255.5,20},{412,20}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(ground.p, pin_n) annotation (Line(
            points={{-360,-40},{-360,-20},{-400,-20}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_track.frame_a) annotation (Line(
            points={{58,40},{48,40},{48,14},{40,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_susp.frame_a) annotation (Line(
            points={{58,40},{-20,40},{-20,20},{-82,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_susp1.frame_a) annotation (Line(
            points={{58,40},{58,46},{440,46},{440,60},{420,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_track1.frame_a) annotation (
            Line(
            points={{58,40},{58,46},{448,46},{448,32},{422,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedFrame.frame_a, frame_susp_7) annotation (Line(
            points={{516,170},{492,170},{492,172},{462,172},{462,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedFrame1.frame_a, frame_susp_1) annotation (Line(
            points={{158,-262},{154,-262},{154,-192},{150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount.frame_b, idler) annotation (Line(
            points={{34,-92},{70,-92},{70,-90},{106,-90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount1.frame_b, idler1) annotation (Line(
            points={{464,-84},{494,-84}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_a, fixedTranslation2.frame_a) annotation (Line(
            points={{-62,40},{-62,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation2.frame_b, fixedShape.frame_a) annotation (Line(
            points={{-42,60},{-16,60},{-16,58},{34,58},{34,92},{46,92}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_a, absoluteVelocity2.frame_a) annotation (Line(
            points={{-62,40},{-72,40},{-72,60},{-140,60},{-140,66}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(absoluteVelocity2.v[1], driver_bus.vehicle_speed) annotation (Line(
            points={{-161,66.6667},{-262,66.6667},{-262,60},{-400,60}},
            color={0,0,127},
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        annotation (Diagram(coordinateSystem(extent={{-400,-220},{400,220}},
                preserveAspectRatio=false,
              initialScale=0.2),           graphics), Icon(coordinateSystem(extent={{-400,
                  -220},{400,220}},        preserveAspectRatio=true,
              initialScale=0.2),                                      graphics={
              Text(
                extent={{-228,18},{252,78}},
                lineColor={0,0,255},
                textString="%name"),
              Ellipse(
                extent={{-30,30},{30,-30}},
                lineColor={0,0,255},
                fillColor={135,135,135},
                fillPattern=FillPattern.Solid,
                origin={370,-220},
                rotation=360),
              Text(
                extent={{-44,-28},{220,-70}},
                lineColor={128,128,128},
                textString="{0,0,0}"),
              Line(
                points={{280,100},{370,100},{370,208}},
                color={95,95,95},
                smooth=Smooth.None,
                thickness=0.5),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={0,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={-260,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={240,-150},
                rotation=360),
              Text(
                extent={{404,230},{484,210}},
                lineColor={128,128,128},
                textString="tau_l"),
              Text(
                extent={{404,-212},{484,-232}},
                lineColor={128,128,128},
                textString="tau_r"),
              Text(
                extent={{326,46},{586,26}},
                lineColor={128,128,128},
                textString="f_tractive"),
              Text(
                extent={{310,106},{570,86}},
                lineColor={128,128,128},
                textString="bilge"),
              Line(
                points={{-390,-20},{-350,-20},{-350,-60}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-370,-60},{-330,-60}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-358,-80},{-342,-80}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-364,-70},{-334,-70}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{280,-98},{370,-98},{370,-206}},
                color={95,95,95},
                smooth=Smooth.None,
                thickness=0.5),
              Text(
                extent={{330,270},{410,250}},
                lineColor={0,0,0},
                textString="sp"),
              Ellipse(
                extent={{-30,30},{30,-30}},
                lineColor={0,0,255},
                fillColor={135,135,135},
                fillPattern=FillPattern.Solid,
                origin={370,220},
                rotation=360)}));
      end Full_hull_3DOF_6w_3g_no_speed_sensors_idler_frame;

      model Full_hull_3DOF_6w_3g_no_speed_sensors_idler_frame_Decl
      "A full hull with 3 degrees of freedom.   6 suspension attachments and 3 guide wheel attachments each side."
        import C2M2L_Decl;
        extends C2M2L_Decl.Testing_Blocks.Hull.Hull_Interface;

         parameter SI.Length CG_driver_x = 0
        "Horizontal distance to driver center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_driver_y = 0
        "Vertical distance to driver center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));

        parameter SI.Length hub_spacing_1_x = -0.675
        "Horizontal distance to first suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 1 Location"));
         parameter SI.Length hub_spacing_1_y = -0.15
        "Vertical distance to first suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 1 Location"));

         parameter SI.Length hub_spacing_2_x = -1.475
        "Horizontal distance to second suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 2 Location"));
         parameter SI.Length hub_spacing_2_y = -0.15
        "Vertical distance to second suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 2 Location"));

         parameter SI.Length hub_spacing_3_x = -2.275
        "Horizontal distance to third suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 3 Location"));
         parameter SI.Length hub_spacing_3_y = -0.15
        "Vertical distance to third suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 3 Location"));

         parameter SI.Length hub_spacing_4_x = -3.075
        "Horizontal distance to fourth suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 4 Location"));
         parameter SI.Length hub_spacing_4_y = -0.15
        "Vertical distance to fourth suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 4 Location"));

         parameter SI.Length hub_spacing_5_x = -3.875
        "Horizontal distance to fifth suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 5 Location"));
         parameter SI.Length hub_spacing_5_y = -0.15
        "Vertical distance to fifth suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 5 Location"));

         parameter SI.Length hub_spacing_6_x = -4.675
        "Horizontal distance to sixth suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 6 Location"));
         parameter SI.Length hub_spacing_6_y = -0.15
        "Vertical distance to sixth suspension attachment point on hull, measured from sprocket center"
          annotation (Dialog(tab="Suspension Mounts", group="Suspension 6 Location"));
         parameter SI.Length idler_x = -6
        "Horizontal distance to idler wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Idler Location"));
         parameter SI.Length idler_y = 0.05
        "Vertical distance to idler wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Idler Location"));

         parameter SI.Length guide_1_x = -1.6
        "Horizontal distance to first guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 1 Location"));
         parameter SI.Length guide_1_y = 0.15
        "Vertical distance to first guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 1 Location"));

         parameter SI.Length guide_2_x = -3.2
        "Horizontal distance to second guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 2 Location"));
         parameter SI.Length guide_2_y = 0.15
        "Vertical distance to second guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 2 Location"));

         parameter SI.Length guide_3_x = -4.8
        "Horizontal distance to third guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 3 Location"));
         parameter SI.Length guide_3_y = 0.15
        "Vertical distance to third guide wheel attachment point on hull, measured from sprocket center"
          annotation (Dialog(group="Guide Wheel 3 Location"));

         parameter SI.Length cl_to_rhs = 1.5
        "Distance from center line to right hand side attachment points"
          annotation (Dialog(group="Vehicle width"));
         parameter SI.Length susp_offset = 0.10
        "Use same value as 'offset_wheel' in suspension module"
          annotation (Dialog(group="Vehicle width"));

        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_1(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_1_x,hub_spacing_1_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={150,-160})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_2(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_2_x,hub_spacing_2_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={90,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_1
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={150,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={280,-225})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_2
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={90,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={180,-225})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        sprocket(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={110,-50}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={370,-220})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation idler_mount(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={idler_x,idler_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={24,-92})));

        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_track(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,cl_to_rhs})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={30,14})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing left_power_in(
            includeBearingConnector=world.driveTrainMechanics3D)
                                          annotation (Placement(transformation(extent=
                 {{-300,-20},{-280,0}}), iconTransformation(
              extent={{-20,-20},{20,20}},
              rotation=0,
              origin={280,100})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing right_power_in(
            includeBearingConnector=world.driveTrainMechanics3D)
                                          annotation (Placement(transformation(extent={{-298,
                  -160},{-278,-140}}),       iconTransformation(
              extent={{-20,-20},{20,20}},
              rotation=0,
              origin={280,-100})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_3(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_3_x,hub_spacing_3_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={30,-160})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_4(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_4_x,hub_spacing_4_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-30,-160})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_5(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_5_x,hub_spacing_5_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-90,-160})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_6(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_6_x,hub_spacing_6_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-150,-160})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_3
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={30,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={80,-225})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_4
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-30,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-120,-225})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_5
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-90,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-220,-225})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_6
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={-150,-192}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-320,-225})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_1(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_1_x,guide_1_y,0})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-18,-30})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_1(
            tau_constant=0)
          annotation (Placement(transformation(extent={{-94,-40},{-74,-20}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_2(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_2_x,guide_2_y,0})
          annotation (Placement(transformation(extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-18,-70})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_3(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_3_x,guide_3_y,0})
          annotation (Placement(transformation(extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-18,-110})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_2(
            tau_constant=0)
          annotation (Placement(transformation(extent={{-92,-80},{-72,-60}})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_3(
            tau_constant=0)
          annotation (Placement(transformation(extent={{-92,-120},{-72,-100}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_1(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-50,-30}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={230,-180})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_2(
            includeBearingConnector=true)                                 annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-50,-70}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-20,-180})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        guide_3(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={-50,-110}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-270,-180})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_susp(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,cl_to_rhs - susp_offset})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={-92,20})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_7(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_1_x,hub_spacing_1_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={462,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_8(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_2_x,hub_spacing_2_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={402,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation idler_mount1(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={idler_x,idler_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={454,-84})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_track1(
          specularCoefficient=0.3,
          animation=true,
          color={255,0,0},
          r={0,0,-cl_to_rhs})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={412,32})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_9(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_3_x,hub_spacing_3_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={342,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_10(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_4_x,hub_spacing_4_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={282,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_11(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_5_x,hub_spacing_5_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={222,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation susp_mount_12(
            width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={hub_spacing_6_x,hub_spacing_6_y,0})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=270,
              origin={162,230})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_4(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_1_x,guide_1_y,0})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={350,-24})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_4(
            tau_constant=0)
          annotation (Placement(transformation(extent={{264,-34},{284,-14}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_5(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_2_x,guide_2_y,0})
          annotation (Placement(transformation(extent={{10,-10},{-10,10}},
              rotation=0,
              origin={350,-64})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation guide_mount_6(
          width=0.05,
          specularCoefficient=0.3,
          color={0,0,0},
          animation=false,
          r={guide_3_x,guide_3_y,0})
          annotation (Placement(transformation(extent={{10,-10},{-10,10}},
              rotation=0,
              origin={350,-104})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_5(
            tau_constant=0)
          annotation (Placement(transformation(extent={{266,-74},{286,-54}})));
        Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque_guide_6(
            tau_constant=0)
          annotation (Placement(transformation(extent={{266,-114},{286,-94}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation half_width_susp1(
          specularCoefficient=0.3,
          color={255,0,0},
          animation=world.enableAnimation,
          r={0,0,-(cl_to_rhs - susp_offset)})
          annotation (Placement(transformation(
              extent={{10,-10},{-10,10}},
              rotation=0,
              origin={410,60})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_7
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={462,198}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={280,226})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_8
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={402,198}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={180,226})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        sprocket1(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={494,-44}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={370,220})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_9
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={342,198}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={80,226})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_10
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={282,198}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-120,226})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_11
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={222,198}),  iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-220,226})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_susp_12
                                                                annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={162,198}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-320,226})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_4(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={310,-24}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={230,180})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_5(
            includeBearingConnector=true)                                 annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={310,-64}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-20,180})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing
                                                        guide_6(
            includeBearingConnector=true)                       annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=0,
              origin={310,-104}), iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-270,180})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{358,-4},{378,16}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor1(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{-10,10},{10,-10}},
              rotation=180,
              origin={206,-26})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor2(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{154,-36},{174,-16}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor adaptor3(
            includeBearingConnector=true)
          annotation (Placement(transformation(extent={{458,-4},{438,16}})));
        Modelica.Mechanics.Rotational.Components.IdealGear idealGear(ratio=-1)
          annotation (Placement(transformation(extent={{414,-4},{434,16}})));
        Modelica.Fluid.Vessels.OpenTank
                                tank(
          redeclare package Medium = Bilge_Medium,
          energyDynamics=Modelica.Fluid.Types.Dynamics.DynamicFreeInitial,
          massDynamics=Modelica.Fluid.Types.Dynamics.DynamicFreeInitial,
          portsData={Modelica.Fluid.Vessels.BaseClasses.VesselPortsData(diameter=0.01,
              height=0.7),Modelica.Fluid.Vessels.BaseClasses.VesselPortsData(diameter=0.01)},
          use_portsData=false,
          nPorts=2,
          redeclare model HeatTransfer =
              Modelica.Fluid.Vessels.BaseClasses.HeatTransfer.IdealHeatTransfer
            (                                                                    k=
                  100),
          use_HeatTransfer=false,
          height=100,
          crossArea=bilge_area,
          level_start=bilge_initial)
          annotation (Placement(transformation(extent={{384,122},{344,162}})));
        Modelica.Fluid.Sources.MassFlowSource_T boundary(
          redeclare package Medium = Bilge_Medium,
          nPorts=1,
          m_flow=leak_flow)
          annotation (Placement(transformation(extent={{326,74},{346,94}})));
        Modelica.Fluid.Interfaces.FluidPort_a port_a(redeclare package Medium
          =   Bilge_Medium)
          annotation (Placement(transformation(extent={{396,72},{416,92}})));
        package Bilge_Medium =
            C2M2L_Decl.Media.Incompressible.Water_Incompressible_Constant;
        parameter SI.Area bilge_area=20 "Area of tank";
        parameter Modelica.Media.Interfaces.PartialMedium.MassFlowRate leak_flow=0
        "Fixed mass flow rate going out of the fluid port";
          parameter SI.Height bilge_initial=0.05
        "Initial level of water in the bilge"
          annotation (Dialog(group="Bilge"));
        Modelica.Mechanics.Rotational.Sensors.TorqueSensor torqueSensor
          annotation (Placement(transformation(extent={{174,-18},{194,2}})));
        Modelica.Mechanics.Rotational.Sensors.TorqueSensor torqueSensor1
          annotation (Placement(transformation(extent={{386,16},{406,-4}})));
        Modelica.Blocks.Interfaces.RealOutput tau_sprocket_left annotation (Placement(
              transformation(extent={{402,200},{422,220}}), iconTransformation(extent=
                 {{402,200},{422,220}})));
        Modelica.Blocks.Interfaces.RealOutput tau_sprocket_right annotation (
            Placement(transformation(extent={{400,-220},{420,-200}}),
              iconTransformation(extent={{400,-220},{420,-200}})));
        Modelica.Blocks.Interfaces.RealOutput f_tractive
        "Tractive force generated at sprocket radius"   annotation (Placement(
              transformation(extent={{402,10},{422,30}}),iconTransformation(extent={{402,10},
                  {422,30}})));
        Modelica.Blocks.Math.MultiSum multiSum(nu=2)
          annotation (Placement(transformation(extent={{284,-182},{296,-170}})));
        Modelica.Blocks.Math.Gain one_over_r(k=2/sprocket_diameter)
        "Divide by sprokect radius"
          annotation (Placement(transformation(extent={{320,-190},{340,-170}})));
        parameter SI.Diameter sprocket_diameter = 0.6
        "Diameter of sprocket used to calculate tractive force";
        parameter Boolean use_electrical_ground = true
        "true to add an electrical ground point";
        Modelica.Electrical.Analog.Basic.Ground ground if use_electrical_ground
          annotation (Placement(transformation(extent={{-370,-60},{-350,-40}})));
        Modelica.Electrical.Analog.Interfaces.NegativePin pin_n if use_electrical_ground
          annotation (Placement(transformation(extent={{-410,-30},{-390,-10}})));
        Modelica.Mechanics.MultiBody.Visualizers.FixedFrame fixedFrame
          annotation (Placement(transformation(extent={{516,160},{536,180}})));
        Modelica.Mechanics.MultiBody.Visualizers.FixedFrame fixedFrame1
          annotation (Placement(transformation(extent={{158,-272},{178,-252}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b idler   annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={106,-90}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-372,-187})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_b idler1  annotation (
            Placement(transformation(
              extent={{-16,-16},{16,16}},
              rotation=270,
              origin={494,-84}),   iconTransformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={-360,169})));
        Modelica.Mechanics.MultiBody.Visualizers.FixedShape fixedShape(
          lengthDirection={-1,0,0},
          width=Total_width,
          height=Total_height,
          color={11,72,107},
          animation=world.enableAnimation,
          widthDirection={0,0,1},
          length=Total_length)
          annotation (Placement(transformation(extent={{46,82},{66,102}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation2(animation=
             false, r={Total_length*0.2,Total_height*0.5,0})
                  annotation (Placement(transformation(extent={{-62,50},{-42,70}})));
        Modelica.Mechanics.MultiBody.Sensors.AbsoluteVelocity absoluteVelocity2(
            resolveInFrame=Modelica.Mechanics.MultiBody.Types.ResolveInFrameA.frame_a)
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=180,
              origin={-150,66})));
        C2M2L_Decl.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_bus
          annotation (Placement(transformation(extent={{-410,50},{-390,70}})));
      equation
        connect(susp_mount_2.frame_b, frame_susp_2) annotation (Line(
            points={{90,-170},{90,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_1.frame_b, frame_susp_1) annotation (Line(
            points={{150,-170},{150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_3.frame_b,frame_susp_3)  annotation (Line(
            points={{30,-170},{30,-181},{30,-181},{30,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_4.frame_b,frame_susp_4)  annotation (Line(
            points={{-30,-170},{-30,-181},{-30,-181},{-30,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_5.frame_b,frame_susp_5)  annotation (Line(
            points={{-90,-170},{-90,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_6.frame_b,frame_susp_6)  annotation (Line(
            points={{-150,-170},{-150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque_guide_1.flange, guide_1.flange)
                                                           annotation (Line(
            points={{-74,-30},{-50,-30}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(guide_1.bearingFrame,guide_mount_1. frame_b)
                                                         annotation (Line(
            points={{-50,-30},{-28,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_2.frame_b,guide_2. bearingFrame) annotation (Line(
            points={{-28,-70},{-50,-70}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_3.frame_b,guide_3. bearingFrame) annotation (Line(
            points={{-28,-110},{-50,-110}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque_guide_2.flange,guide_2. flange) annotation (Line(
            points={{-72,-70},{-50,-70}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(constantTorque_guide_3.flange,guide_3. flange) annotation (Line(
            points={{-72,-110},{-50,-110}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(idler_mount.frame_a, half_width_track.frame_b)
                                                         annotation (Line(
            points={{14,-92},{0,-92},{0,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track.frame_b, sprocket.bearingFrame)
                                                           annotation (Line(
            points={{20,14},{0,14},{0,-50},{110,-50}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_1.frame_a, half_width_track.frame_b)
                                                           annotation (Line(
            points={{-8,-30},{0,-30},{0,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_2.frame_a, half_width_track.frame_b)
                                                           annotation (Line(
            points={{-8,-70},{0,-70},{0,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_3.frame_a, half_width_track.frame_b)
                                                           annotation (Line(
            points={{-8,-110},{0,-110},{0,14},{20,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_1.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{150,-150},{150,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_2.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{90,-150},{90,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_3.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{30,-150},{30,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_4.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-30,-150},{-30,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_5.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-90,-150},{-90,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_6.frame_a, half_width_susp.frame_b) annotation (Line(
            points={{-150,-150},{-148,-150},{-148,-130},{-110,-130},{-110,20},{-102,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));

        connect(susp_mount_8.frame_b,frame_susp_8)  annotation (Line(
            points={{402,220},{402,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_7.frame_b,frame_susp_7)  annotation (Line(
            points={{462,220},{462,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_9.frame_b,frame_susp_9)  annotation (Line(
            points={{342,220},{342,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_10.frame_b, frame_susp_10)
                                                    annotation (Line(
            points={{282,220},{282,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_11.frame_b, frame_susp_11)
                                                    annotation (Line(
            points={{222,220},{222,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_12.frame_b, frame_susp_12)
                                                    annotation (Line(
            points={{162,220},{162,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque_guide_4.flange,guide_4. flange)
                                                           annotation (Line(
            points={{284,-24},{310,-24}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(guide_4.bearingFrame,guide_mount_4. frame_b)
                                                         annotation (Line(
            points={{310,-24},{340,-24}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_5.frame_b,guide_5. bearingFrame) annotation (Line(
            points={{340,-64},{310,-64}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_6.frame_b,guide_6. bearingFrame) annotation (Line(
            points={{340,-104},{310,-104}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(constantTorque_guide_5.flange,guide_5. flange) annotation (Line(
            points={{286,-64},{310,-64}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(constantTorque_guide_6.flange,guide_6. flange) annotation (Line(
            points={{286,-104},{310,-104}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(idler_mount1.frame_a, half_width_track1.frame_b)
                                                         annotation (Line(
            points={{444,-84},{384,-84},{384,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(half_width_track1.frame_b, sprocket1.bearingFrame)
                                                           annotation (Line(
            points={{402,32},{384,32},{384,-44},{494,-44}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_4.frame_a, half_width_track1.frame_b)
                                                           annotation (Line(
            points={{360,-24},{384,-24},{384,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_5.frame_a, half_width_track1.frame_b)
                                                           annotation (Line(
            points={{360,-64},{384,-64},{384,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_mount_6.frame_a, half_width_track1.frame_b)
                                                           annotation (Line(
            points={{360,-104},{384,-104},{384,32},{402,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_7.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{462,240},{462,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_8.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{402,240},{402,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_9.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{342,240},{342,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_10.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{282,240},{282,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_11.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{222,240},{222,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_mount_12.frame_a, half_width_susp1.frame_b)
                                                               annotation (Line(
            points={{162,240},{164,240},{164,260},{202,260},{202,60},{400,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor3.flangeAndFrame, sprocket1) annotation (Line(
            points={{458,6},{476,6},{476,-44},{494,-44}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor2.flangeAndFrame, sprocket) annotation (Line(
            points={{154,-26},{94,-26},{94,-50},{110,-50}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(left_power_in, adaptor.flangeAndFrame) annotation (Line(
            points={{-290,-10},{-276,-10},{-276,6},{358,6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(right_power_in, adaptor1.flangeAndFrame) annotation (Line(
            points={{-288,-150},{-242,-150},{-242,-208},{234,-208},{234,-26},{216,-26}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(adaptor1.frame, adaptor2.frame) annotation (Line(
            points={{206,-36},{164,-36}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(boundary.ports[1],tank. ports[1]) annotation (Line(
            points={{346,84},{368,84},{368,122}},
            color={0,127,255},
            smooth=Smooth.None));
        connect(port_a,tank. ports[2]) annotation (Line(
            points={{406,82},{388,82},{388,122},{360,122}},
            color={0,127,255},
            smooth=Smooth.None));
        connect(idealGear.flange_b, adaptor3.flange) annotation (Line(
            points={{434,6},{442,6},{442,6},{448,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(adaptor.frame, adaptor3.frame) annotation (Line(
            points={{368,-4},{368,-14},{448,-14},{448,-4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(torqueSensor.flange_a, adaptor2.flange) annotation (Line(
            points={{174,-8},{170,-8},{170,-26},{164,-26}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor.flange_b, adaptor1.flange) annotation (Line(
            points={{194,-8},{200,-8},{200,-26},{206,-26}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.flange_b, idealGear.flange_a) annotation (Line(
            points={{406,6},{414,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.flange_a, adaptor.flange) annotation (Line(
            points={{386,6},{368,6}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(torqueSensor1.tau, tau_sprocket_left) annotation (Line(
            points={{388,17},{390,17},{390,210},{412,210}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor.tau, tau_sprocket_right) annotation (Line(
            points={{176,-19},{178,-19},{178,-210},{410,-210}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor.tau, multiSum.u[1]) annotation (Line(
            points={{176,-19},{178,-19},{178,-164},{284,-164},{284,-173.9}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(torqueSensor1.tau, multiSum.u[2]) annotation (Line(
            points={{388,17},{388,24},{252,24},{252,-178.1},{284,-178.1}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(multiSum.y, one_over_r.u) annotation (Line(
            points={{297.02,-176},{308,-176},{308,-180},{318,-180}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(one_over_r.y, f_tractive) annotation (Line(
            points={{341,-180},{255.5,-180},{255.5,20},{412,20}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(ground.p, pin_n) annotation (Line(
            points={{-360,-40},{-360,-20},{-400,-20}},
            color={0,0,255},
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_track.frame_a) annotation (Line(
            points={{58,40},{48,40},{48,14},{40,14}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_susp.frame_a) annotation (Line(
            points={{58,40},{-20,40},{-20,20},{-82,20}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_susp1.frame_a) annotation (Line(
            points={{58,40},{58,46},{440,46},{440,60},{420,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, half_width_track1.frame_a) annotation (
            Line(
            points={{58,40},{58,46},{448,46},{448,32},{422,32}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedFrame.frame_a, frame_susp_7) annotation (Line(
            points={{516,170},{492,170},{492,172},{462,172},{462,198}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedFrame1.frame_a, frame_susp_1) annotation (Line(
            points={{158,-262},{154,-262},{154,-192},{150,-192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount.frame_b, idler) annotation (Line(
            points={{34,-92},{70,-92},{70,-90},{106,-90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_mount1.frame_b, idler1) annotation (Line(
            points={{464,-84},{494,-84}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_a, fixedTranslation2.frame_a) annotation (Line(
            points={{-62,40},{-62,60}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation2.frame_b, fixedShape.frame_a) annotation (Line(
            points={{-42,60},{-16,60},{-16,58},{34,58},{34,92},{46,92}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_a, absoluteVelocity2.frame_a) annotation (Line(
            points={{-62,40},{-72,40},{-72,60},{-140,60},{-140,66}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(absoluteVelocity2.v[1], driver_bus.vehicle_speed) annotation (Line(
            points={{-161,66.6667},{-262,66.6667},{-262,60},{-400,60}},
            color={0,0,127},
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        annotation (Diagram(coordinateSystem(extent={{-400,-220},{400,220}},
                preserveAspectRatio=false,
              initialScale=0.2),           graphics), Icon(coordinateSystem(extent={{-400,
                  -220},{400,220}},        preserveAspectRatio=true,
              initialScale=0.2),                                      graphics={
              Text(
                extent={{-228,18},{252,78}},
                lineColor={0,0,255},
                textString="%name"),
              Ellipse(
                extent={{-30,30},{30,-30}},
                lineColor={0,0,255},
                fillColor={135,135,135},
                fillPattern=FillPattern.Solid,
                origin={370,-220},
                rotation=360),
              Text(
                extent={{-44,-28},{220,-70}},
                lineColor={128,128,128},
                textString="{0,0,0}"),
              Line(
                points={{280,100},{370,100},{370,208}},
                color={95,95,95},
                smooth=Smooth.None,
                thickness=0.5),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={0,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={-260,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={240,-150},
                rotation=360),
              Text(
                extent={{404,230},{484,210}},
                lineColor={128,128,128},
                textString="tau_l"),
              Text(
                extent={{404,-212},{484,-232}},
                lineColor={128,128,128},
                textString="tau_r"),
              Text(
                extent={{326,46},{586,26}},
                lineColor={128,128,128},
                textString="f_tractive"),
              Text(
                extent={{310,106},{570,86}},
                lineColor={128,128,128},
                textString="bilge"),
              Line(
                points={{-390,-20},{-350,-20},{-350,-60}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-370,-60},{-330,-60}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-358,-80},{-342,-80}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{-364,-70},{-334,-70}},
                color={0,0,255},
                smooth=Smooth.None,
                visible=use_electrical_ground),
              Line(
                points={{280,-98},{370,-98},{370,-206}},
                color={95,95,95},
                smooth=Smooth.None,
                thickness=0.5),
              Text(
                extent={{330,270},{410,250}},
                lineColor={0,0,0},
                textString="sp"),
              Ellipse(
                extent={{-30,30},{30,-30}},
                lineColor={0,0,255},
                fillColor={135,135,135},
                fillPattern=FillPattern.Solid,
                origin={370,220},
                rotation=360)}));
      end Full_hull_3DOF_6w_3g_no_speed_sensors_idler_frame_Decl;

      partial model Hull_Interface
      "A generic hull interface to allow the inner/outer relationship to work with a range of hull implementations"
        parameter Real swimming = 0 "0 if on land 1 if vehicle is swimming";
    protected
        parameter Boolean swimming_bool = if swimming < 0.5 then false else true
        "true if vehicle is swimming";
    public
        parameter SI.Area frontal_area=16 "Frontal area of vehicle"
          annotation (Dialog(group="Aero drag"));
        parameter SI.Density rho_air = 1.16 "Nominal air density" annotation (Dialog(group="Aero drag"));
        parameter Real Cd = 1.25
        "Vehicle drag factor (Cd) in F = 0.5 * Cd * rho * v^2 * A"   annotation (Dialog(group="Aero drag"));
         parameter SI.Mass m_hull(min=0)=20000 "Total Vehicle Mass"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_xx_hull(min=0)=1e4 "Total Vehicle Roll Inertia"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_yy_hull(min=0)=1e4 "Total Vehicle Yaw Inertia"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Inertia I_zz_hull(min=0)=3.5e4
        "Total Vehicle Pitch Inertia"
          annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_21_hull=0 " (2,1) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_31_hull=0 " (3,1) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));
        parameter SI.Inertia I_32_hull=0 " (3,2) element of inertia tensor"  annotation (Dialog(group="Mass Properties"));

         parameter SI.Length CG_hull_x = -3.2
        "Horizontal distance to vehicle center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));
         parameter SI.Length CG_hull_y = 0.2
        "Vertical distance to vehicle center of gravity, measured from sprocket center"
          annotation (Dialog(group="Mass Properties"));

        parameter SI.Position init_pos_longitudinal=-5.0
        "Initial longitudinal (fore-aft) position of hull center of gravity";

        parameter SI.Position init_pos_vertical=1.0
        "Initial vertical (up-down) position of hull center of gravity";

        parameter SI.Position init_pos_lateral=0.0
        "Initial lateral (left-right) position of hull center of gravity";

        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a
          annotation (Placement(transformation(extent={{-78,24},{-46,56}}),
              iconTransformation(extent={{64,-16},{96,16}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation(animation=
             false, r={init_pos_longitudinal,init_pos_vertical,init_pos_lateral})
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=0,
              origin={-182,192})));
        Modelica.Mechanics.MultiBody.Joints.Prismatic prismatic1(stateSelect=
              StateSelect.always, n={0,1,0},
          v(fixed=false),
          s(fixed=true)) if not swimming_bool
          annotation (Placement(transformation(extent={{-162,182},{-142,202}})));
        Modelica.Mechanics.MultiBody.Joints.Prismatic prismatic(stateSelect=
              StateSelect.always, s(fixed=true),
          v(fixed=true))
          annotation (Placement(transformation(extent={{-132,182},{-112,202}})));
        Modelica.Mechanics.MultiBody.Parts.Fixed fixed
          annotation (Placement(transformation(extent={{-232,182},{-212,202}})));
        Modelica.Mechanics.MultiBody.Sensors.AbsoluteVelocity absoluteVelocity1(
            resolveInFrame=Modelica.Mechanics.MultiBody.Types.ResolveInFrameA.frame_a)
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=180,
              origin={-142,42})));
        Modelica.Blocks.Sources.Constant const[3](k={0,0,0})
          annotation (Placement(transformation(extent={{-292,162},{-272,182}})));
        Modelica.Mechanics.MultiBody.Forces.WorldForceAndTorque forceAndTorque(
            resolveInFrame=Modelica.Mechanics.MultiBody.Types.ResolveInFrameB.frame_b)
          annotation (Placement(transformation(extent={{-232,128},{-212,148}})));
        C2M2L_Ext.Testing_Blocks.Hull.aero_drag aero_drag1(
          frontal_area=frontal_area,
          rho=rho_air,
          Cd=Cd) if not swimming_bool
          annotation (Placement(transformation(extent={{-292,122},{-272,142}})));
        Modelica.Mechanics.MultiBody.Joints.Revolute revolute(phi(fixed=true),
            stateSelect=StateSelect.always,
          w(fixed=true)) if not swimming_bool
          annotation (Placement(transformation(extent={{-92,182},{-72,202}})));
        Modelica.Mechanics.MultiBody.Parts.Body hull_mass(
          specularCoefficient=0.3,
          angles_start={0,0,0},
          v_0(fixed=false),
          m=m_hull,
          I_11=I_xx_hull,
          I_22=I_yy_hull,
          I_33=I_zz_hull,
          r_CM={0,0,0},
          I_21=I_21_hull,
          I_31=I_31_hull,
          I_32=I_32_hull,
          sphereDiameter=0.25)
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=270,
              origin={98,30})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation1(animation=
             false, r={CG_hull_x,CG_hull_y,0})
                  annotation (Placement(transformation(extent={{58,30},{78,50}})));
        outer Modelica.Mechanics.MultiBody.World world
          annotation (Placement(transformation(extent={{-378,148},{-358,168}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation lock_for_swim_vertical(r={
              0,0,0}, animation=false) if swimming_bool
          annotation (Placement(transformation(extent={{-162,208},{-142,228}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation lock_for_swim_pitch(
            animation=false, r={0,0,0}) if swimming_bool
          annotation (Placement(transformation(extent={{-92,212},{-72,232}})));
        C2M2L_Ext.Testing_Blocks.Hull.aqua_drag aqua_drag1(
          Total_weight=m_hull,
          Total_height=Total_height,
          Total_length=Total_length,
          Total_width=Total_width,
          Sponson_height=Sponson_height,
          Sponson_width=Sponson_width,
          Approach_angle=Approach_angle,
          Departure_angle=Departure_angle,
          Upper_glacis_angle=Upper_glacis_angle,
          Nose_height_factor=Nose_height_factor,
          Upper_glacis_length=Upper_glacis_length,
          Wheel_frontal_area=Wheel_frontal_area,
          Gx0=Gx0,
          Gy0=Gy0,
          sea_state_drag_multiplier=sea_state_drag_multiplier) if swimming_bool
          annotation (Placement(transformation(extent={{-292,86},{-272,106}})));

        parameter SI.Length Total_height=2288/1000 "Overall height of vehicle";
        parameter SI.Length Total_length=8300/1000 "Overall length of vehicle";
        parameter SI.Length Total_width=3660/1000 "Overall width of vehicle";
        parameter SI.Length Sponson_height=673.1/1000 "Height of sponsons";
        parameter SI.Length Sponson_width=628.6/1000 "Width of sponsons";
        parameter SI.Conversions.NonSIunits.Angle_deg Approach_angle=45
        "Approach angle of body underside";
        parameter SI.Conversions.NonSIunits.Angle_deg Departure_angle=70
        "Departure angle of vehicle underside";
        parameter SI.Conversions.NonSIunits.Angle_deg Upper_glacis_angle=5
        "Upper slope angle at front of vehicle";
        parameter Real Nose_height_factor=1475/2288
        "Height fraction where nose slope starts";
        parameter SI.Length Upper_glacis_length=1598.97/1000
        "Length of upper slope section";
        parameter SI.Area Wheel_frontal_area=0.73 "Frontal area of the wheels";
        parameter SI.Length Gx0=0
        "Longitudinal center of gravity at zero heel angle";
        parameter SI.Length Gy0=1000/1000
        "Vertical C G position at zero heel angle";

        parameter Real sea_state_drag_multiplier=1.0
        "Multiplies drag force to account for drag increase due to sea state";
        Modelica.Mechanics.MultiBody.Visualizers.FixedShape fixedShape(
          lengthDirection={-1,0,0},
          width=Total_width,
          height=Total_height,
          color={11,72,107},
          animation=world.enableAnimation,
          widthDirection={0,0,1},
          length=Total_length)
          annotation (Placement(transformation(extent={{6,60},{26,80}})));
        Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation2(animation=
             false, r={Total_length*0.2,Total_height*0.5,0})
                  annotation (Placement(transformation(extent={{-36,48},{-16,68}})));
      equation
        connect(prismatic1.frame_b,prismatic. frame_a) annotation (Line(
            points={{-142,192},{-132,192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(prismatic1.frame_a,fixedTranslation. frame_b) annotation (Line(
            points={{-162,192},{-172,192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixed.frame_b,fixedTranslation. frame_a) annotation (Line(
            points={{-212,192},{-192,192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(const.y,forceAndTorque. torque) annotation (Line(
            points={{-271,172},{-236,172},{-236,144},{-234,144}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(forceAndTorque.frame_b,hull_mass. frame_a) annotation (Line(
            points={{-212,138},{98,138},{98,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(aero_drag1.f,forceAndTorque. force) annotation (Line(
            points={{-271,132},{-234,132}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(aero_drag1.u,absoluteVelocity1. v) annotation (Line(
            points={{-294,132},{-312,132},{-312,42},{-153,42}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(revolute.frame_a,prismatic. frame_b) annotation (Line(
            points={{-92,192},{-112,192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(revolute.frame_b,hull_mass. frame_a) annotation (Line(
            points={{-72,192},{98,192},{98,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(absoluteVelocity1.frame_a, hull_mass.frame_a) annotation (Line(
            points={{-132,42},{-100,42},{-100,80},{98,80},{98,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_a, frame_a) annotation (Line(
            points={{58,40},{-62,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation1.frame_b, hull_mass.frame_a) annotation (Line(
            points={{78,40},{98,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation.frame_b, lock_for_swim_vertical.frame_a) annotation (
           Line(
            points={{-172,192},{-168,192},{-168,218},{-162,218}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(lock_for_swim_vertical.frame_b, prismatic.frame_a) annotation (Line(
            points={{-142,218},{-138,218},{-138,192},{-132,192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(lock_for_swim_pitch.frame_a, prismatic.frame_b) annotation (Line(
            points={{-92,222},{-102,222},{-102,192},{-112,192}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(lock_for_swim_pitch.frame_b, hull_mass.frame_a) annotation (Line(
            points={{-72,222},{98,222},{98,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(absoluteVelocity1.v, aqua_drag1.u) annotation (Line(
            points={{-153,42},{-312,42},{-312,96},{-294,96}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(aqua_drag1.f, forceAndTorque.force) annotation (Line(
            points={{-271,96},{-254,96},{-254,132},{-234,132}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(frame_a, fixedTranslation2.frame_a) annotation (Line(
            points={{-62,40},{-50,40},{-50,58},{-36,58}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(fixedTranslation2.frame_b, fixedShape.frame_a) annotation (Line(
            points={{-16,58},{-8,58},{-8,70},{6,70}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(extent={{-400,-220},{400,220}},
                preserveAspectRatio=false,
              initialScale=0.2),           graphics), Icon(coordinateSystem(extent={{-400,
                  -220},{400,220}},        preserveAspectRatio=true,
              initialScale=0.2),                                      graphics={
              Rectangle(
                extent={{-400,0},{400,220}},
                lineColor={255,85,170},
                fillColor={215,215,215},
                fillPattern=FillPattern.Solid),
              Rectangle(
                extent={{-400,-220},{400,0}},
                lineColor={255,85,170},
                fillPattern=FillPattern.Solid,
                fillColor={215,215,215}),
              Text(
                extent={{-228,18},{252,78}},
                lineColor={0,0,255},
                textString="%name"),
              Text(
                extent={{332,-160},{412,-180}},
                lineColor={0,0,0},
                textString="sp"),
              Text(
                extent={{-410,-160},{-330,-180}},
                lineColor={0,0,0},
                textString="id"),
              Text(
                extent={{-44,-28},{220,-70}},
                lineColor={128,128,128},
                textString="{0,0,0}"),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={0,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={-260,-150},
                rotation=360),
              Text(
                extent={{-40,10},{40,-10}},
                lineColor={0,0,0},
                textString="gw",
                origin={240,-150},
                rotation=360),
              Rectangle(
                extent={{-442,260},{438,-260}},
                lineColor={170,213,255},
                fillColor={85,170,255},
                fillPattern=FillPattern.Forward,
                visible=swimming)}));
      end Hull_Interface;

      model Susp_Hydro_Pneumatic_6_Wheels
      "Implements a full suspension model inside a container"
        extends
        C2M2L_CyPhy.Testing_Blocks.Hull.Suspension_Interface_2_Wheels_idler_frame_Decl;
        parameter Boolean is_left_side = false;
        parameter Real compression_scale = 1.0;
        parameter Real rebound_scale = 0.6;
        parameter Real spring_scale = 1.0;

        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_1(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{300,-40},{260,0}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_leading(c=100000, d=10000, mass_length=1)
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=90,
              origin={330,50})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Drive_Sprocket_Pulley
          sprocket1(D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=270,
              origin={340,100})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Idler_Pulley
                                                                                     idler_Pulley(
          phi=1.5707963267949,
          m_wheel=10,
          D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-360,78},{-320,118}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con3(mass_length=1)
          annotation (Placement(transformation(extent={{-10,-40},{10,-20}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_6(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-260,-40},{-300,0}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con6(mass_length=1)
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-330,50})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_1(
          offset_wheel=0,
          compression_scale=compression_scale,
          rebound_scale=rebound_scale,
          spring_scale=spring_scale,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{260,20},{300,60}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_2(
          offset_wheel=0,
          compression_scale=compression_scale,
          rebound_scale=rebound_scale,
          spring_scale=spring_scale,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{-300,20},{-260,60}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_4
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-109,68},{-23,108}}),
              iconTransformation(extent={{-112,122},{-92,142}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_4(
          offset_wheel=0,
          compression_scale=compression_scale,
          rebound_scale=rebound_scale,
          spring_scale=spring_scale,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{-86,22},{-46,62}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_4(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-46,-40},{-86,0}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_3(
          offset_wheel=0,
          compression_scale=compression_scale,
          rebound_scale=rebound_scale,
          spring_scale=spring_scale,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{38,22},{78,62}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_3(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{78,-40},{38,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_3
                                                                annotation (Placement(
              transformation(rotation=0, extent={{15,72},{101,112}}),
              iconTransformation(extent={{44,124},{64,144}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con1(mass_length=1)
          annotation (Placement(transformation(extent={{232,-40},{252,-20}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con2(mass_length=1)
          annotation (Placement(transformation(extent={{-246,-40},{-226,-20}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_5(
          offset_wheel=0,
          compression_scale=compression_scale,
          rebound_scale=rebound_scale,
          spring_scale=spring_scale,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{140,20},{180,60}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_2(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{180,-40},{140,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_2
                                                                annotation (Placement(
              transformation(rotation=0, extent={{117,70},{203,110}}),
              iconTransformation(extent={{162,124},{182,144}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con4(mass_length=1)
          annotation (Placement(transformation(extent={{102,-40},{122,-20}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_6(
          offset_wheel=0,
          compression_scale=compression_scale,
          rebound_scale=rebound_scale,
          spring_scale=spring_scale,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{-178,22},{-138,62}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_5(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-138,-40},{-178,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_5
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-201,68},{-115,108}}),
              iconTransformation(extent={{-224,122},{-204,142}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con5(mass_length=1)
          annotation (Placement(transformation(extent={{-122,-40},{-102,-20}})));

        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Rigid_Tensioner_Assembly
          tensioner(
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=-90,
              origin={-396,118})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con9(mass_length=1)
          annotation (Placement(transformation(extent={{-214,144},{-234,164}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley1(
          m_wheel=5,
          stateSelect=StateSelect.always,
          D_wheel=0.6)        annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={46,162})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley2(
          m_wheel=5,
          stateSelect=StateSelect.always,
          D_wheel=0.6)        annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={242,162})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley3(
          m_wheel=5,
          stateSelect=StateSelect.always,
          D_wheel=0.6)        annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={-136,164})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con7(mass_length=1)
          annotation (Placement(transformation(extent={{-34,142},{-54,162}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con8(mass_length=1)
          annotation (Placement(transformation(extent={{142,142},{122,162}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con10(mass_length=1)
          annotation (Placement(transformation(extent={{322,142},{302,162}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_2(
            includeBearingConnector=true) annotation (Placement(transformation(extent={{36,222},
                  {56,202}}),           iconTransformation(extent={{-16,178},{4,158}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_1(
            includeBearingConnector=true) annotation (Placement(transformation(extent={{232,214},
                  {252,194}}),          iconTransformation(extent={{214,180},{234,160}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_3(
            includeBearingConnector=true) annotation (Placement(transformation(extent={{-146,
                  214},{-126,194}}),    iconTransformation(extent={{-278,178},{-258,
                  158}})));
      equation
        connect(road_Wheel_Assembly_1.frame_b,belt_leading. flange_a)
                                                                    annotation (Line(
            points={{300,-30.2},{330,-30.2},{330,40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_leading.flange_b,sprocket1. frame_a)    annotation (Line(
            points={{330,60},{330,80},{330.2,80}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_a,idler_Pulley. frame_b)
          annotation (Line(
            points={{-330,60},{-330,78}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_b,road_Wheel_Assembly_6. frame_a)
          annotation (Line(
            points={{-330,40},{-330,-29.8},{-300,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(sprocket1.flange, sprocket) annotation (Line(
            points={{340.2,100},{340.2,120},{340,120},{340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.wheel, road_Wheel_Assembly_1.flange) annotation (Line(
            points={{280,19.6},{280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.hull, frame_susp_1) annotation (Line(
            points={{280,60.4},{280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.wheel, road_Wheel_Assembly_6.flange) annotation (Line(
            points={{-280,19.6},{-280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.hull, frame_susp_6) annotation (Line(
            points={{-280,60.4},{-280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_b, road_Wheel_Assembly_3.frame_a)
          annotation (Line(
            points={{10,-30},{26,-30},{26,-29.8},{38,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con1.flange_b, road_Wheel_Assembly_1.frame_a)
          annotation (Line(
            points={{252,-30},{252,-29.8},{260,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_a, road_Wheel_Assembly_4.frame_b)
          annotation (Line(
            points={{-10,-30},{-12,-30},{-12,-30.2},{-46,-30.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con2.flange_a, road_Wheel_Assembly_6.frame_b)
          annotation (Line(
            points={{-246,-30},{-246,-30.2},{-260,-30.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_4.flange, susp_4.wheel) annotation (Line(
            points={{-66,-19.8},{-66,21.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_4.hull, frame_susp_4) annotation (Line(
            points={{-66,62.4},{-66,88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_susp_3, susp_3.hull) annotation (Line(
            points={{58,92},{58,62.4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_3.wheel, road_Wheel_Assembly_3.flange) annotation (Line(
            points={{58,21.6},{58,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_2.frame_b, belt_Segment_ZERO_FIDELITY_no_con1.flange_a)
          annotation (Line(
            points={{180,-30.2},{220,-30.2},{220,-30},{232,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_susp_2,susp_5. hull) annotation (Line(
            points={{160,90},{160,60.4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_5.wheel,road_Wheel_Assembly_2. flange) annotation (Line(
            points={{160,19.6},{160,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_3.frame_b, belt_Segment_ZERO_FIDELITY_no_con4.flange_a)
          annotation (Line(
            points={{78,-30.2},{91,-30.2},{91,-30},{102,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con4.flange_b, road_Wheel_Assembly_2.frame_a)
          annotation (Line(
            points={{122,-30},{132,-30},{132,-29.8},{140,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_a, belt_Segment_ZERO_FIDELITY_no_con2.flange_b)
          annotation (Line(
            points={{-178,-29.8},{-218,-29.8},{-218,-30},{-226,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.flange,susp_6. wheel) annotation (Line(
            points={{-158,-19.8},{-158,21.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_6.hull,frame_susp_5)  annotation (Line(
            points={{-158,62.4},{-158,88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_b, belt_Segment_ZERO_FIDELITY_no_con5.flange_a)
          annotation (Line(
            points={{-138,-30.2},{-129,-30.2},{-129,-30},{-122,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con5.flange_b, road_Wheel_Assembly_4.frame_a)
          annotation (Line(
            points={{-102,-30},{-94,-30},{-94,-29.8},{-86,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(tensioner.hull, idler) annotation (Line(
            points={{-402,138.4},{-400,138.4},{-400,148},{-340,148},{-340,140}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(tensioner.wheel, idler_Pulley.flange) annotation (Line(
            points={{-396,97.6},{-396,92},{-340,92},{-340,98}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_b, idler_Pulley.frame_a)
          annotation (Line(
            points={{-234,154},{-330,154},{-330,118}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley1.flange,guide_2)  annotation (Line(
            points={{46,162},{46,212}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley2.flange,guide_1)  annotation (Line(
            points={{242,162},{242,204}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_a,guide_Wheel_Pulley3. frame_b)
          annotation (Line(
            points={{-214,154},{-156,154}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley3.flange,guide_3)  annotation (Line(
            points={{-136,164},{-136,204}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley2.frame_a,belt_Segment_ZERO_FIDELITY_no_con10. flange_b)
          annotation (Line(
            points={{262,152},{302,152}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con10.flange_a, sprocket1.frame_b)
          annotation (Line(
            points={{322,152},{329.8,152},{329.8,120}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con8.flange_a,guide_Wheel_Pulley2. frame_b)
          annotation (Line(
            points={{142,152},{222,152}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley1.frame_a,belt_Segment_ZERO_FIDELITY_no_con8. flange_b)
          annotation (Line(
            points={{66,152},{122,152}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con7.flange_a,guide_Wheel_Pulley1. frame_b)
          annotation (Line(
            points={{-34,152},{26,152}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con7.flange_b,guide_Wheel_Pulley3. frame_a)
          annotation (Line(
            points={{-54,152},{-114,152},{-114,154},{-116,154}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false,extent={{-460,-100},
                  {420,220}}), graphics), Icon(coordinateSystem(preserveAspectRatio=false,
                         extent={{-460,-100},{420,220}}),
                                               graphics={
              Text(
                extent={{-86,98},{48,64}},
                lineColor={0,0,255},
                textString="FULL"),
              Ellipse(extent={{-266,40},{-166,-60}},lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{-152,40},{-52,-60}}, lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{2,40},{102,-60}},    lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{122,40},{222,-60}},  lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Text(
                extent={{-198,130},{156,96}},
                lineColor={0,128,0},
                textString="VU"),
              Text(
                extent={{-238,66},{222,30}},
                lineColor={0,0,255},
                textString="Hydro_Pnuematic"),
              Ellipse(extent={{-300,200},{-234,138}},
                                                    lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{-40,200},{26,138}},  lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{190,200},{256,138}}, lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid)}));
      end Susp_Hydro_Pneumatic_6_Wheels;

      model Suspension_Full_Container
      "Implements a full suspension model inside a container"
        extends C2M2L_Ext.Testing_Blocks.Hull.Suspension_Interface;

        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_1(
          m_damper=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771; -0.25,
                  -20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195; -0.05,
              -15734.3095108594; -0.01,-13702.9943094687; 0,0;0.01, 6670;0.05,
              12904.4407273906;0.1, 14208.796253;0.125, 14744.2626991272;0.25,
              16805.5764160156;0.375, 18651.4111061096;0.5, 20815.203125;0.625,
              23349.2517433167;0.75, 26056.5549316406;0.875, 28667.5844154358; 1,30962;
              1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,35494.2829933167;
              1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676; -0.25,
                  -1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169; -0.05,
              -518.487198545312; -0.01,18.8136284481731; 0,0;0.01, 618.34740751998;0.05,
              738.3195499375;0.1, 923.340798;0.125, 1035.63191772461;0.25,
              1859.6598046875;0.375, 3201.07604614258;0.5, 5073.25375;0.625,
              7399.2298425293;0.75, 10046.1281640625;0.875, 12859.5825646973; 1,
              15698.16; 1.125,18467.783626709; 1.25,21156.1558984375; 1.375,
              23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=2500,
          offset_wheel=0,
          compression_scale=0.8)
          annotation (Placement(transformation(extent={{126,0},{166,40}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_2(
          m_damper=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771; -0.25,
                  -20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195; -0.05,
              -15734.3095108594; -0.01,-13702.9943094687; 0,0;0.01, 6670;0.05,
              12904.4407273906;0.1, 14208.796253;0.125, 14744.2626991272;0.25,
              16805.5764160156;0.375, 18651.4111061096;0.5, 20815.203125;0.625,
              23349.2517433167;0.75, 26056.5549316406;0.875, 28667.5844154358; 1,30962;
              1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,35494.2829933167;
              1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676; -0.25,
                  -1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169; -0.05,
              -518.487198545312; -0.01,18.8136284481731; 0,0;0.01, 618.34740751998;0.05,
              738.3195499375;0.1, 923.340798;0.125, 1035.63191772461;0.25,
              1859.6598046875;0.375, 3201.07604614258;0.5, 5073.25375;0.625,
              7399.2298425293;0.75, 10046.1281640625;0.875, 12859.5825646973; 1,
              15698.16; 1.125,18467.783626709; 1.25,21156.1558984375; 1.375,
              23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=2500,
          offset_wheel=0,
          compression_scale=0.8)
          annotation (Placement(transformation(extent={{46,0},{86,40}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
                                                                                 road_Wheel_Assembly_1(
          D_wheel=0.8,
          W_wheel=0.2,
          m_wheel=10,
          c_tire=1000000,
          d_tire=100000)
          annotation (Placement(transformation(extent={{166,-80},{126,-40}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
                                                                                 road_Wheel_Assembly_2(
          D_wheel=0.8,
          W_wheel=0.2,
          m_wheel=10,
          c_tire=1000000,
          d_tire=100000)
          annotation (Placement(transformation(extent={{86,-80},{46,-40}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                        belt_leading(
          c=10000000,
          d=1000000)
          annotation (Placement(transformation(extent={{234,-42},{254,-22}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                        belt_lower_1(
          c=10000000,
          d=1000000)
          annotation (Placement(transformation(extent={{96,-84},{116,-64}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                        belt_trailing(
          c=10000000,
          d=1000000)
          annotation (Placement(transformation(extent={{-252,-38},{-232,-18}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                        belt_upper_rear(
          c=10000000,
          d=1000000) annotation (Placement(transformation(extent={{-124,106},{-144,
                  126}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Idler_Pulley
                                                                          sprocket1(
          D_wheel=0.8,
          W_wheel=0.2,
          m_wheel=10)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=180,
              origin={340,96})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Idler_Pulley
          idler_Pulley(
          D_wheel=0.8,
          W_wheel=0.2,
          m_wheel=10,
          phi=1.5707963267949)
          annotation (Placement(transformation(extent={{-360,76},{-320,116}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_3(
          m_damper=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771; -0.25,
                  -20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195; -0.05,
              -15734.3095108594; -0.01,-13702.9943094687; 0,0;0.01, 6670;0.05,
              12904.4407273906;0.1, 14208.796253;0.125, 14744.2626991272;0.25,
              16805.5764160156;0.375, 18651.4111061096;0.5, 20815.203125;0.625,
              23349.2517433167;0.75, 26056.5549316406;0.875, 28667.5844154358; 1,30962;
              1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,35494.2829933167;
              1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676; -0.25,
                  -1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169; -0.05,
              -518.487198545312; -0.01,18.8136284481731; 0,0;0.01, 618.34740751998;0.05,
              738.3195499375;0.1, 923.340798;0.125, 1035.63191772461;0.25,
              1859.6598046875;0.375, 3201.07604614258;0.5, 5073.25375;0.625,
              7399.2298425293;0.75, 10046.1281640625;0.875, 12859.5825646973; 1,
              15698.16; 1.125,18467.783626709; 1.25,21156.1558984375; 1.375,
              23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=2500,
          offset_wheel=0,
          compression_scale=0.8)
          annotation (Placement(transformation(extent={{-34,0},{6,40}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
                                                                                 road_Wheel_Assembly_3(
          D_wheel=0.8,
          W_wheel=0.2,
          m_wheel=10,
          c_tire=1000000,
          d_tire=100000)
          annotation (Placement(transformation(extent={{6,-80},{-34,-40}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                        belt_lower_2(
          c=10000000,
          d=1000000)
          annotation (Placement(transformation(extent={{16,-84},{36,-64}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_4(
          m_damper=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771; -0.25,
                  -20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195; -0.05,
              -15734.3095108594; -0.01,-13702.9943094687; 0,0;0.01, 6670;0.05,
              12904.4407273906;0.1, 14208.796253;0.125, 14744.2626991272;0.25,
              16805.5764160156;0.375, 18651.4111061096;0.5, 20815.203125;0.625,
              23349.2517433167;0.75, 26056.5549316406;0.875, 28667.5844154358; 1,30962;
              1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,35494.2829933167;
              1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676; -0.25,
                  -1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169; -0.05,
              -518.487198545312; -0.01,18.8136284481731; 0,0;0.01, 618.34740751998;0.05,
              738.3195499375;0.1, 923.340798;0.125, 1035.63191772461;0.25,
              1859.6598046875;0.375, 3201.07604614258;0.5, 5073.25375;0.625,
              7399.2298425293;0.75, 10046.1281640625;0.875, 12859.5825646973; 1,
              15698.16; 1.125,18467.783626709; 1.25,21156.1558984375; 1.375,
              23867.181661377; 1.5,26855.39125],
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=2500,
          offset_wheel=0,
          compression_scale=1.0,
          rebound_scale=0.8)
          annotation (Placement(transformation(extent={{-114,0},{-74,40}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
                                                                                 road_Wheel_Assembly_4(
          D_wheel=0.8,
          W_wheel=0.2,
          m_wheel=10,
          c_tire=1000000,
          d_tire=100000)
          annotation (Placement(transformation(extent={{-74,-80},{-114,-40}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                        belt_lower_3(
          c=10000000,
          d=1000000)
          annotation (Placement(transformation(extent={{-64,-84},{-44,-64}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_5(
          m_damper=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771; -0.25,
                  -20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195; -0.05,
              -15734.3095108594; -0.01,-13702.9943094687; 0,0;0.01, 6670;0.05,
              12904.4407273906;0.1, 14208.796253;0.125, 14744.2626991272;0.25,
              16805.5764160156;0.375, 18651.4111061096;0.5, 20815.203125;0.625,
              23349.2517433167;0.75, 26056.5549316406;0.875, 28667.5844154358; 1,30962;
              1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,35494.2829933167;
              1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676; -0.25,
                  -1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169; -0.05,
              -518.487198545312; -0.01,18.8136284481731; 0,0;0.01, 618.34740751998;0.05,
              738.3195499375;0.1, 923.340798;0.125, 1035.63191772461;0.25,
              1859.6598046875;0.375, 3201.07604614258;0.5, 5073.25375;0.625,
              7399.2298425293;0.75, 10046.1281640625;0.875, 12859.5825646973; 1,
              15698.16; 1.125,18467.783626709; 1.25,21156.1558984375; 1.375,
              23867.181661377; 1.5,26855.39125],
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=2500,
          offset_wheel=0,
          compression_scale=1.0,
          rebound_scale=0.8)
          annotation (Placement(transformation(extent={{-194,0},{-154,40}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
                                                                                 road_Wheel_Assembly_5(
          D_wheel=0.8,
          W_wheel=0.2,
          m_wheel=10,
          c_tire=1000000,
          d_tire=100000)
          annotation (Placement(transformation(extent={{-154,-80},{-194,-40}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                        belt_lower_4(
          c=10000000,
          d=1000000)
          annotation (Placement(transformation(extent={{-144,-84},{-124,-64}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
                           guide_Wheel_Pulley(
          D_wheel=0.4,
          W_wheel=0.2,
          m_wheel=5) annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={0,126})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                        belt_upper_front(
          c=10000000,
          d=1000000) annotation (Placement(transformation(extent={{78,106},{58,126}})));
      equation
        connect(belt_trailing.flange_a,idler_Pulley. frame_b)
                                                            annotation (Line(
            points={{-252,-28},{-330,-28},{-330,76}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_lower_1.flange_b,road_Wheel_Assembly_1. frame_a)
                                                                    annotation (Line(
            points={{116,-74},{121,-74},{121,-69.8},{126,-69.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_2.frame_b,belt_lower_1. flange_a)
                                                                   annotation (Line(
            points={{86,-70.2},{91,-70.2},{91,-74},{96,-74}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_leading.flange_b, sprocket1.frame_a)    annotation (Line(
            points={{254,-32},{330,-32},{330,76}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.wheel,road_Wheel_Assembly_2. flange) annotation (Line(
            points={{66,-0.4},{66,-59.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.wheel,road_Wheel_Assembly_1. flange) annotation (Line(
            points={{146,-0.4},{146,-59.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_3.frame_b,belt_lower_2. flange_a)
                                                                   annotation (Line(
            points={{6,-70.2},{11,-70.2},{11,-74},{16,-74}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_3.wheel,road_Wheel_Assembly_3. flange) annotation (Line(
            points={{-14,-0.4},{-14,-59.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_4.frame_b,belt_lower_3. flange_a)
                                                                   annotation (Line(
            points={{-74,-70.2},{-69,-70.2},{-69,-74},{-64,-74}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_4.wheel,road_Wheel_Assembly_4. flange) annotation (Line(
            points={{-94,-0.4},{-94,-59.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_b,belt_lower_4. flange_a)
                                                                   annotation (Line(
            points={{-154,-70.2},{-149,-70.2},{-149,-74},{-144,-74}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_5.wheel,road_Wheel_Assembly_5. flange) annotation (Line(
            points={{-174,-0.4},{-174,-59.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_lower_4.flange_b,road_Wheel_Assembly_4. frame_a) annotation (
            Line(
            points={{-124,-74},{-119,-74},{-119,-69.8},{-114,-69.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_lower_3.flange_b,road_Wheel_Assembly_3. frame_a) annotation (
            Line(
            points={{-44,-74},{-39,-74},{-39,-69.8},{-34,-69.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_lower_2.flange_b,road_Wheel_Assembly_2. frame_a) annotation (
            Line(
            points={{36,-74},{41,-74},{41,-69.8},{46,-69.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_trailing.flange_b,road_Wheel_Assembly_5. frame_a) annotation (
            Line(
            points={{-232,-28},{-213,-28},{-213,-69.8},{-194,-69.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_leading.flange_a,road_Wheel_Assembly_1. frame_b) annotation (
            Line(
            points={{234,-32},{186,-32},{186,-70.2},{166,-70.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_Pulley.frame_a,belt_upper_rear. flange_b) annotation (Line(
            points={{-330,116},{-144,116}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_upper_rear.flange_a,guide_Wheel_Pulley. frame_b) annotation (
            Line(
            points={{-124,116},{-20,116}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley.frame_a,belt_upper_front. flange_b) annotation (
            Line(
            points={{20,116},{58,116}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_upper_front.flange_a, sprocket1.frame_b)
                                                             annotation (Line(
            points={{78,116},{330,116}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_Pulley.flange, idler) annotation (Line(
            points={{-340,96},{-340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley.flange, guide_2) annotation (Line(
            points={{0,126},{0,180}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_5.hull, frame_susp_6) annotation (Line(
            points={{-174,40.4},{-174,44},{-280,44},{-280,46},{-280,46},{-280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_4.hull, frame_susp_5) annotation (Line(
            points={{-94,40.4},{-150,40.4},{-150,90},{-200,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_3.hull, frame_susp_4) annotation (Line(
            points={{-14,40.4},{-70,40.4},{-70,90},{-120,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.hull, frame_susp_2) annotation (Line(
            points={{66,40.4},{132,40.4},{132,90},{200,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.hull, frame_susp_1) annotation (Line(
            points={{146,40.4},{210,40.4},{210,90},{280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(sprocket1.flange, sprocket) annotation (Line(
            points={{340,96},{340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false,extent={{-460,
                  -100},{420,220}}),
                               graphics), Icon(graphics={
              Text(
                extent={{-86,98},{48,64}},
                lineColor={0,0,255},
                textString="FULL")}));
      end Suspension_Full_Container;

      model Suspension_Full_Container_4_Wheels
      "Implements a full suspension model inside a container"
        extends C2M2L_Ext.Testing_Blocks.Hull.Suspension_Interface_2_Wheels;
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_1(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{300,-40},{260,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_leading(c=100000,
            d=10000)
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=90,
              origin={330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Drive_Sprocket_Pulley
          sprocket1(D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=270,
              origin={340,100})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Idler_Pulley
                                                                                    idler_Pulley(
          phi=1.5707963267949,
          m_wheel=10,
          D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-360,78},{-320,118}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con3
          annotation (Placement(transformation(extent={{-10,-40},{10,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_6(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-260,-40},{-300,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con6
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con9
          annotation (Placement(transformation(extent={{8,120},{-12,140}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_1(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{260,20},{300,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_2(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{-300,20},{-260,60}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_4
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-109,68},{-23,108}}),
              iconTransformation(extent={{-98,122},{-78,142}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_4(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{-86,22},{-46,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_4(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-46,-38},{-86,2}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_3(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{38,22},{78,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_3(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{78,-38},{38,2}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_3
                                                                annotation (Placement(
              transformation(rotation=0, extent={{15,72},{101,112}}),
              iconTransformation(extent={{32,124},{52,144}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con1
          annotation (Placement(transformation(extent={{154,-36},{174,-16}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con2
          annotation (Placement(transformation(extent={{-194,-36},{-174,-16}})));
      equation
        connect(road_Wheel_Assembly_1.frame_b,belt_leading. flange_a)
                                                                    annotation (Line(
            points={{300,-30.2},{330,-30.2},{330,40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_leading.flange_b,sprocket1. frame_a)    annotation (Line(
            points={{330,60},{330,80},{330.2,80}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_a,idler_Pulley. frame_b)
          annotation (Line(
            points={{-330,60},{-330,78}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_b,road_Wheel_Assembly_6. frame_a)
          annotation (Line(
            points={{-330,40},{-330,-29.8},{-300,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_b, idler_Pulley.frame_a)
          annotation (Line(
            points={{-12,130},{-330,130},{-330,118}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_Pulley.flange, idler) annotation (Line(
            points={{-340,98},{-340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(sprocket1.flange, sprocket) annotation (Line(
            points={{340.2,100},{340.2,120},{340,120},{340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_a, sprocket1.frame_b)
          annotation (Line(
            points={{8,130},{329.8,130},{329.8,120}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.wheel, road_Wheel_Assembly_1.flange) annotation (Line(
            points={{280,19.6},{280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.hull, frame_susp_1) annotation (Line(
            points={{280,60.4},{280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.wheel, road_Wheel_Assembly_6.flange) annotation (Line(
            points={{-280,19.6},{-280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.hull, frame_susp_6) annotation (Line(
            points={{-280,60.4},{-280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_b, road_Wheel_Assembly_3.frame_a)
          annotation (Line(
            points={{10,-30},{26,-30},{26,-27.8},{38,-27.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_3.frame_b, belt_Segment_ZERO_FIDELITY_no_con1.flange_a)
          annotation (Line(
            points={{78,-28.2},{118,-28.2},{118,-26},{154,-26}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con1.flange_b, road_Wheel_Assembly_1.frame_a)
          annotation (Line(
            points={{174,-26},{218,-26},{218,-29.8},{260,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_a, road_Wheel_Assembly_4.frame_b)
          annotation (Line(
            points={{-10,-30},{-12,-30},{-12,-28.2},{-46,-28.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_4.frame_a, belt_Segment_ZERO_FIDELITY_no_con2.flange_b)
          annotation (Line(
            points={{-86,-27.8},{-146,-27.8},{-146,-26},{-174,-26}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con2.flange_a, road_Wheel_Assembly_6.frame_b)
          annotation (Line(
            points={{-194,-26},{-228,-26},{-228,-30.2},{-260,-30.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_4.flange, susp_4.wheel) annotation (Line(
            points={{-66,-17.8},{-66,21.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_4.hull, frame_susp_4) annotation (Line(
            points={{-66,62.4},{-66,88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_susp_3, susp_3.hull) annotation (Line(
            points={{58,92},{58,92},{58,62.4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_3.wheel, road_Wheel_Assembly_3.flange) annotation (Line(
            points={{58,21.6},{58,21.6},{58,-17.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false,extent={{-460,
                  -100},{420,220}}),
                               graphics), Icon(coordinateSystem(preserveAspectRatio=
                  false, extent={{-460,-100},{420,220}}),
                                               graphics={
              Text(
                extent={{-86,98},{48,64}},
                lineColor={0,0,255},
                textString="FULL"),
              Ellipse(extent={{-154,40},{-54,-60}}, lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{0,40},{100,-60}},    lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid)}));
      end Suspension_Full_Container_4_Wheels;

      model Suspension_Full_Container_4_Wheels_1_guide
      "Implements a full suspension model inside a container"
        extends C2M2L_Ext.Testing_Blocks.Hull.Suspension_Interface_2_Wheels;
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_1(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{300,-40},{260,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_leading(c=100000,
            d=10000)
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=90,
              origin={330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Drive_Sprocket_Pulley
          sprocket1(D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=270,
              origin={340,100})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Idler_Pulley
                                                                                    idler_Pulley(
          phi=1.5707963267949,
          m_wheel=10,
          D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-360,78},{-320,118}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con3
          annotation (Placement(transformation(extent={{-10,-40},{10,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_6(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-260,-40},{-300,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con6
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con9
          annotation (Placement(transformation(extent={{-112,120},{-132,140}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_1(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{260,20},{300,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_2(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{-300,20},{-260,60}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_4
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-109,68},{-23,108}}),
              iconTransformation(extent={{-98,122},{-78,142}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_4(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{-86,22},{-46,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_4(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-46,-38},{-86,2}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_3(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{38,22},{78,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_3(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{78,-38},{38,2}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_3
                                                                annotation (Placement(
              transformation(rotation=0, extent={{15,72},{101,112}}),
              iconTransformation(extent={{32,124},{52,144}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con1
          annotation (Placement(transformation(extent={{154,-36},{174,-16}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con2
          annotation (Placement(transformation(extent={{-194,-36},{-174,-16}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con4
          annotation (Placement(transformation(extent={{152,120},{132,140}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley1(
          m_wheel=5,
          stateSelect=StateSelect.always,
          D_wheel=0.6)        annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={0,140})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_2(
            includeBearingConnector=true) annotation (Placement(transformation(extent={{-10,190},
                  {10,170}}),           iconTransformation(extent={{-30,180},{-10,160}})));
      equation
        connect(road_Wheel_Assembly_1.frame_b,belt_leading. flange_a)
                                                                    annotation (Line(
            points={{300,-30.2},{330,-30.2},{330,40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_leading.flange_b,sprocket1. frame_a)    annotation (Line(
            points={{330,60},{330,80},{330.2,80}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_a,idler_Pulley. frame_b)
          annotation (Line(
            points={{-330,60},{-330,78}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_b,road_Wheel_Assembly_6. frame_a)
          annotation (Line(
            points={{-330,40},{-330,-29.8},{-300,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_b, idler_Pulley.frame_a)
          annotation (Line(
            points={{-132,130},{-330,130},{-330,118}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_Pulley.flange, idler) annotation (Line(
            points={{-340,98},{-340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(sprocket1.flange, sprocket) annotation (Line(
            points={{340.2,100},{340.2,120},{340,120},{340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.wheel, road_Wheel_Assembly_1.flange) annotation (Line(
            points={{280,19.6},{280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.hull, frame_susp_1) annotation (Line(
            points={{280,60.4},{280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.wheel, road_Wheel_Assembly_6.flange) annotation (Line(
            points={{-280,19.6},{-280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.hull, frame_susp_6) annotation (Line(
            points={{-280,60.4},{-280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_b, road_Wheel_Assembly_3.frame_a)
          annotation (Line(
            points={{10,-30},{26,-30},{26,-27.8},{38,-27.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_3.frame_b, belt_Segment_ZERO_FIDELITY_no_con1.flange_a)
          annotation (Line(
            points={{78,-28.2},{118,-28.2},{118,-26},{154,-26}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con1.flange_b, road_Wheel_Assembly_1.frame_a)
          annotation (Line(
            points={{174,-26},{218,-26},{218,-29.8},{260,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_a, road_Wheel_Assembly_4.frame_b)
          annotation (Line(
            points={{-10,-30},{-12,-30},{-12,-28.2},{-46,-28.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_4.frame_a, belt_Segment_ZERO_FIDELITY_no_con2.flange_b)
          annotation (Line(
            points={{-86,-27.8},{-146,-27.8},{-146,-26},{-174,-26}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con2.flange_a, road_Wheel_Assembly_6.frame_b)
          annotation (Line(
            points={{-194,-26},{-228,-26},{-228,-30.2},{-260,-30.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_4.flange, susp_4.wheel) annotation (Line(
            points={{-66,-17.8},{-66,21.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_4.hull, frame_susp_4) annotation (Line(
            points={{-66,62.4},{-66,88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_susp_3, susp_3.hull) annotation (Line(
            points={{58,92},{58,92},{58,62.4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_3.wheel, road_Wheel_Assembly_3.flange) annotation (Line(
            points={{58,21.6},{58,21.6},{58,-17.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con4.flange_a, sprocket1.frame_b)
          annotation (Line(
            points={{152,130},{329.8,130},{329.8,120}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_a,guide_Wheel_Pulley1. frame_b)
          annotation (Line(
            points={{-112,130},{-20,130}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley1.flange,guide_2)  annotation (Line(
            points={{0,140},{0,180}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley1.frame_a, belt_Segment_ZERO_FIDELITY_no_con4.flange_b)
          annotation (Line(
            points={{20,130},{132,130}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false,extent={{-460,
                  -100},{420,220}}),
                               graphics), Icon(coordinateSystem(preserveAspectRatio=
                  false, extent={{-460,-100},{420,220}}),
                                               graphics={
              Text(
                extent={{-86,98},{48,64}},
                lineColor={0,0,255},
                textString="FULL"),
              Ellipse(extent={{-166,40},{-66,-60}}, lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{-12,40},{88,-60}},   lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{-54,200},{12,138}},  lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Text(
                extent={{-198,124},{156,90}},
                lineColor={0,128,0},
                textString="VU")}));
      end Suspension_Full_Container_4_Wheels_1_guide;

      model Suspension_Full_Container_6_Wheels
      "Implements a full suspension model inside a container"
        extends C2M2L_Ext.Testing_Blocks.Hull.Suspension_Interface_2_Wheels;
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_1(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{300,-40},{260,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_leading(c=100000,
            d=10000)
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=90,
              origin={330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Drive_Sprocket_Pulley
          sprocket1(D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=270,
              origin={340,100})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Idler_Pulley
                                                                                    idler_Pulley(
          phi=1.5707963267949,
          m_wheel=10,
          D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-360,78},{-320,118}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con3
          annotation (Placement(transformation(extent={{-10,-40},{10,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_6(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-260,-40},{-300,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con6
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con9
          annotation (Placement(transformation(extent={{8,120},{-12,140}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_1(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{260,20},{300,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_2(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{-300,20},{-260,60}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_4
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-109,68},{-23,108}}),
              iconTransformation(extent={{-112,122},{-92,142}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_4(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{-86,22},{-46,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_4(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-46,-40},{-86,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_3(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{38,22},{78,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_3(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{78,-40},{38,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_3
                                                                annotation (Placement(
              transformation(rotation=0, extent={{15,72},{101,112}}),
              iconTransformation(extent={{44,124},{64,144}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con1
          annotation (Placement(transformation(extent={{232,-40},{252,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con2
          annotation (Placement(transformation(extent={{-246,-40},{-226,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_5(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{140,20},{180,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_2(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{180,-40},{140,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_2
                                                                annotation (Placement(
              transformation(rotation=0, extent={{117,70},{203,110}}),
              iconTransformation(extent={{162,124},{182,144}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con4
          annotation (Placement(transformation(extent={{102,-40},{122,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_6(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{-178,22},{-138,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_5(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-138,-40},{-178,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_5
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-201,68},{-115,108}}),
              iconTransformation(extent={{-224,122},{-204,142}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con5
          annotation (Placement(transformation(extent={{-122,-40},{-102,-20}})));
      equation
        connect(road_Wheel_Assembly_1.frame_b,belt_leading. flange_a)
                                                                    annotation (Line(
            points={{300,-30.2},{330,-30.2},{330,40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_leading.flange_b,sprocket1. frame_a)    annotation (Line(
            points={{330,60},{330,80},{330.2,80}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_a,idler_Pulley. frame_b)
          annotation (Line(
            points={{-330,60},{-330,78}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_b,road_Wheel_Assembly_6. frame_a)
          annotation (Line(
            points={{-330,40},{-330,-29.8},{-300,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_b, idler_Pulley.frame_a)
          annotation (Line(
            points={{-12,130},{-330,130},{-330,118}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_Pulley.flange, idler) annotation (Line(
            points={{-340,98},{-340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(sprocket1.flange, sprocket) annotation (Line(
            points={{340.2,100},{340.2,120},{340,120},{340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_a, sprocket1.frame_b)
          annotation (Line(
            points={{8,130},{329.8,130},{329.8,120}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.wheel, road_Wheel_Assembly_1.flange) annotation (Line(
            points={{280,19.6},{280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.hull, frame_susp_1) annotation (Line(
            points={{280,60.4},{280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.wheel, road_Wheel_Assembly_6.flange) annotation (Line(
            points={{-280,19.6},{-280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.hull, frame_susp_6) annotation (Line(
            points={{-280,60.4},{-280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_b, road_Wheel_Assembly_3.frame_a)
          annotation (Line(
            points={{10,-30},{26,-30},{26,-29.8},{38,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con1.flange_b, road_Wheel_Assembly_1.frame_a)
          annotation (Line(
            points={{252,-30},{252,-29.8},{260,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_a, road_Wheel_Assembly_4.frame_b)
          annotation (Line(
            points={{-10,-30},{-12,-30},{-12,-30.2},{-46,-30.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con2.flange_a, road_Wheel_Assembly_6.frame_b)
          annotation (Line(
            points={{-246,-30},{-246,-30.2},{-260,-30.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_4.flange, susp_4.wheel) annotation (Line(
            points={{-66,-19.8},{-66,21.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_4.hull, frame_susp_4) annotation (Line(
            points={{-66,62.4},{-66,88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_susp_3, susp_3.hull) annotation (Line(
            points={{58,92},{58,92},{58,62.4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_3.wheel, road_Wheel_Assembly_3.flange) annotation (Line(
            points={{58,21.6},{58,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_2.frame_b, belt_Segment_ZERO_FIDELITY_no_con1.flange_a)
          annotation (Line(
            points={{180,-30.2},{220,-30.2},{220,-30},{232,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_susp_2,susp_5. hull) annotation (Line(
            points={{160,90},{160,60.4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_5.wheel,road_Wheel_Assembly_2. flange) annotation (Line(
            points={{160,19.6},{160,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_3.frame_b, belt_Segment_ZERO_FIDELITY_no_con4.flange_a)
          annotation (Line(
            points={{78,-30.2},{91,-30.2},{91,-30},{102,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con4.flange_b, road_Wheel_Assembly_2.frame_a)
          annotation (Line(
            points={{122,-30},{132,-30},{132,-29.8},{140,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_a, belt_Segment_ZERO_FIDELITY_no_con2.flange_b)
          annotation (Line(
            points={{-178,-29.8},{-218,-29.8},{-218,-30},{-226,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.flange,susp_6. wheel) annotation (Line(
            points={{-158,-19.8},{-158,21.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_6.hull,frame_susp_5)  annotation (Line(
            points={{-158,62.4},{-158,88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_b, belt_Segment_ZERO_FIDELITY_no_con5.flange_a)
          annotation (Line(
            points={{-138,-30.2},{-129,-30.2},{-129,-30},{-122,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con5.flange_b, road_Wheel_Assembly_4.frame_a)
          annotation (Line(
            points={{-102,-30},{-94,-30},{-94,-29.8},{-86,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false,extent={{-460,
                  -100},{420,220}}),
                               graphics), Icon(coordinateSystem(preserveAspectRatio=false,
                         extent={{-460,-100},{420,220}}),
                                               graphics={
              Text(
                extent={{-86,98},{48,64}},
                lineColor={0,0,255},
                textString="FULL"),
              Ellipse(extent={{-266,40},{-166,-60}},lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{-152,40},{-52,-60}}, lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{2,40},{102,-60}},    lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{122,40},{222,-60}},  lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Text(
                extent={{-196,124},{158,90}},
                lineColor={0,128,0},
                textString="VU")}));
      end Suspension_Full_Container_6_Wheels;

      model Suspension_Full_Container_6_Wheels_3_guides
      "Implements a full suspension model inside a container"
        extends C2M2L_Ext.Testing_Blocks.Hull.Suspension_Interface_2_Wheels;
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_1(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{300,-40},{260,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_leading(c=100000,
            d=10000)
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=90,
              origin={330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Drive_Sprocket_Pulley
          sprocket1(D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=270,
              origin={340,100})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Idler_Pulley
                                                                                    idler_Pulley(
          phi=1.5707963267949,
          m_wheel=10,
          D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-360,78},{-320,118}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con3
          annotation (Placement(transformation(extent={{-10,-40},{10,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_6(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-260,-40},{-300,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con6
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con9
          annotation (Placement(transformation(extent={{-258,128},{-278,148}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_1(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{260,20},{300,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_2(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{-300,20},{-260,60}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_4
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-109,68},{-23,108}}),
              iconTransformation(extent={{-112,122},{-92,142}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_4(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{-86,22},{-46,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_4(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-46,-40},{-86,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_3(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{38,22},{78,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_3(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{78,-40},{38,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_3
                                                                annotation (Placement(
              transformation(rotation=0, extent={{15,72},{101,112}}),
              iconTransformation(extent={{44,124},{64,144}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con1
          annotation (Placement(transformation(extent={{232,-40},{252,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con2
          annotation (Placement(transformation(extent={{-246,-40},{-226,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_5(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{140,20},{180,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_2(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{180,-40},{140,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_2
                                                                annotation (Placement(
              transformation(rotation=0, extent={{117,70},{203,110}}),
              iconTransformation(extent={{162,124},{182,144}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con4
          annotation (Placement(transformation(extent={{102,-40},{122,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_6(
          m_damper=1,
          offset_wheel=0,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          c_bar=3*2500)
          annotation (Placement(transformation(extent={{-178,22},{-138,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_5(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-138,-40},{-178,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_5
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-201,68},{-115,108}}),
              iconTransformation(extent={{-224,122},{-204,142}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con5
          annotation (Placement(transformation(extent={{-122,-40},{-102,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley1(
          m_wheel=5,
          stateSelect=StateSelect.always,
          D_wheel=0.6)        annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={2,146})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_2(
            includeBearingConnector=true) annotation (Placement(transformation(extent={{-8,206},
                  {12,186}}),           iconTransformation(extent={{-16,178},{4,158}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley2(
          m_wheel=5,
          stateSelect=StateSelect.always,
          D_wheel=0.6)        annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={198,146})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_1(
            includeBearingConnector=true) annotation (Placement(transformation(extent={{188,198},
                  {208,178}}),          iconTransformation(extent={{214,180},{234,160}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley3(
          m_wheel=5,
          stateSelect=StateSelect.always,
          D_wheel=0.6)        annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={-180,148})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_3(
            includeBearingConnector=true) annotation (Placement(transformation(extent={{-190,
                  198},{-170,178}}),    iconTransformation(extent={{-278,178},{-258,
                  158}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con7
          annotation (Placement(transformation(extent={{-78,126},{-98,146}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con8
          annotation (Placement(transformation(extent={{98,126},{78,146}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con10
          annotation (Placement(transformation(extent={{278,126},{258,146}})));
      equation
        connect(road_Wheel_Assembly_1.frame_b,belt_leading. flange_a)
                                                                    annotation (Line(
            points={{300,-30.2},{330,-30.2},{330,40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_leading.flange_b,sprocket1. frame_a)    annotation (Line(
            points={{330,60},{330,80},{330.2,80}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_a,idler_Pulley. frame_b)
          annotation (Line(
            points={{-330,60},{-330,78}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_b,road_Wheel_Assembly_6. frame_a)
          annotation (Line(
            points={{-330,40},{-330,-29.8},{-300,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_b, idler_Pulley.frame_a)
          annotation (Line(
            points={{-278,138},{-330,138},{-330,118}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_Pulley.flange, idler) annotation (Line(
            points={{-340,98},{-340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(sprocket1.flange, sprocket) annotation (Line(
            points={{340.2,100},{340.2,120},{340,120},{340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.wheel, road_Wheel_Assembly_1.flange) annotation (Line(
            points={{280,19.6},{280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.hull, frame_susp_1) annotation (Line(
            points={{280,60.4},{280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.wheel, road_Wheel_Assembly_6.flange) annotation (Line(
            points={{-280,19.6},{-280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.hull, frame_susp_6) annotation (Line(
            points={{-280,60.4},{-280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_b, road_Wheel_Assembly_3.frame_a)
          annotation (Line(
            points={{10,-30},{26,-30},{26,-29.8},{38,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con1.flange_b, road_Wheel_Assembly_1.frame_a)
          annotation (Line(
            points={{252,-30},{252,-29.8},{260,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_a, road_Wheel_Assembly_4.frame_b)
          annotation (Line(
            points={{-10,-30},{-12,-30},{-12,-30.2},{-46,-30.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con2.flange_a, road_Wheel_Assembly_6.frame_b)
          annotation (Line(
            points={{-246,-30},{-246,-30.2},{-260,-30.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_4.flange, susp_4.wheel) annotation (Line(
            points={{-66,-19.8},{-66,21.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_4.hull, frame_susp_4) annotation (Line(
            points={{-66,62.4},{-66,88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_susp_3, susp_3.hull) annotation (Line(
            points={{58,92},{58,92},{58,62.4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_3.wheel, road_Wheel_Assembly_3.flange) annotation (Line(
            points={{58,21.6},{58,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_2.frame_b, belt_Segment_ZERO_FIDELITY_no_con1.flange_a)
          annotation (Line(
            points={{180,-30.2},{220,-30.2},{220,-30},{232,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_susp_2,susp_5. hull) annotation (Line(
            points={{160,90},{160,60.4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_5.wheel,road_Wheel_Assembly_2. flange) annotation (Line(
            points={{160,19.6},{160,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_3.frame_b, belt_Segment_ZERO_FIDELITY_no_con4.flange_a)
          annotation (Line(
            points={{78,-30.2},{91,-30.2},{91,-30},{102,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con4.flange_b, road_Wheel_Assembly_2.frame_a)
          annotation (Line(
            points={{122,-30},{132,-30},{132,-29.8},{140,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_a, belt_Segment_ZERO_FIDELITY_no_con2.flange_b)
          annotation (Line(
            points={{-178,-29.8},{-218,-29.8},{-218,-30},{-226,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.flange,susp_6. wheel) annotation (Line(
            points={{-158,-19.8},{-158,21.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_6.hull,frame_susp_5)  annotation (Line(
            points={{-158,62.4},{-158,88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_b, belt_Segment_ZERO_FIDELITY_no_con5.flange_a)
          annotation (Line(
            points={{-138,-30.2},{-129,-30.2},{-129,-30},{-122,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con5.flange_b, road_Wheel_Assembly_4.frame_a)
          annotation (Line(
            points={{-102,-30},{-94,-30},{-94,-29.8},{-86,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley1.flange,guide_2)  annotation (Line(
            points={{2,146},{2,196}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley2.flange,guide_1)  annotation (Line(
            points={{198,146},{198,188}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_a,guide_Wheel_Pulley3. frame_b)
          annotation (Line(
            points={{-258,138},{-200,138}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley3.flange,guide_3)  annotation (Line(
            points={{-180,148},{-180,188}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley2.frame_a, belt_Segment_ZERO_FIDELITY_no_con10.flange_b)
          annotation (Line(
            points={{218,136},{258,136}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con10.flange_a, sprocket1.frame_b)
          annotation (Line(
            points={{278,136},{329.8,136},{329.8,120}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con8.flange_a, guide_Wheel_Pulley2.frame_b)
          annotation (Line(
            points={{98,136},{178,136}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley1.frame_a, belt_Segment_ZERO_FIDELITY_no_con8.flange_b)
          annotation (Line(
            points={{22,136},{78,136}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con7.flange_a, guide_Wheel_Pulley1.frame_b)
          annotation (Line(
            points={{-78,136},{-18,136}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con7.flange_b, guide_Wheel_Pulley3.frame_a)
          annotation (Line(
            points={{-98,136},{-158,136},{-158,138},{-160,138}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false,extent={{-460,
                  -100},{420,220}}),
                               graphics), Icon(coordinateSystem(preserveAspectRatio=false,
                         extent={{-460,-100},{420,220}}),
                                               graphics={
              Text(
                extent={{-86,98},{48,64}},
                lineColor={0,0,255},
                textString="FULL"),
              Ellipse(extent={{-266,40},{-166,-60}},lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{-152,40},{-52,-60}}, lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{2,40},{102,-60}},    lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{122,40},{222,-60}},  lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{-300,200},{-234,138}},
                                                    lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{-40,200},{26,138}},  lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{190,200},{256,138}}, lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Text(
                extent={{-196,126},{158,92}},
                lineColor={0,128,0},
                textString="VU")}));
      end Suspension_Full_Container_6_Wheels_3_guides;

      model Suspension_Full_Container_6_Wheels_3g_Hydro_pneumatic_wTensioner
      "Implements a full suspension model inside a container"
        extends
        C2M2L_CyPhy.Testing_Blocks.Hull.Suspension_Interface_2_Wheels_idler_frame;
        parameter Boolean is_left_side = false;
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_1(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{300,-40},{260,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_leading(c=100000,
            d=10000)
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=90,
              origin={330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Drive_Sprocket_Pulley
          sprocket1(D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=270,
              origin={340,100})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Idler_Pulley
                                                                                    idler_Pulley(
          phi=1.5707963267949,
          m_wheel=10,
          D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-360,78},{-320,118}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con3
          annotation (Placement(transformation(extent={{-10,-40},{10,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_6(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-260,-40},{-300,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con6
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_1(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{260,20},{300,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_2(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{-300,20},{-260,60}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_4
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-109,68},{-23,108}}),
              iconTransformation(extent={{-112,122},{-92,142}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_4(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{-86,22},{-46,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_4(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-46,-40},{-86,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_3(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{38,22},{78,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_3(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{78,-40},{38,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_3
                                                                annotation (Placement(
              transformation(rotation=0, extent={{15,72},{101,112}}),
              iconTransformation(extent={{44,124},{64,144}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con1
          annotation (Placement(transformation(extent={{232,-40},{252,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con2
          annotation (Placement(transformation(extent={{-246,-40},{-226,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_5(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{140,20},{180,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_2(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{180,-40},{140,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_2
                                                                annotation (Placement(
              transformation(rotation=0, extent={{117,70},{203,110}}),
              iconTransformation(extent={{162,124},{182,144}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con4
          annotation (Placement(transformation(extent={{102,-40},{122,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_6(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{-178,22},{-138,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_5(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-138,-40},{-178,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_5
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-201,68},{-115,108}}),
              iconTransformation(extent={{-224,122},{-204,142}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con5
          annotation (Placement(transformation(extent={{-122,-40},{-102,-20}})));

        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Rigid_Tensioner_Assembly
          tensioner(
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=-90,
              origin={-396,118})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con9
          annotation (Placement(transformation(extent={{-214,144},{-234,164}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley1(
          m_wheel=5,
          stateSelect=StateSelect.always,
          D_wheel=0.6)        annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={46,162})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley2(
          m_wheel=5,
          stateSelect=StateSelect.always,
          D_wheel=0.6)        annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={242,162})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley3(
          m_wheel=5,
          stateSelect=StateSelect.always,
          D_wheel=0.6)        annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={-136,164})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con7
          annotation (Placement(transformation(extent={{-34,142},{-54,162}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con8
          annotation (Placement(transformation(extent={{142,142},{122,162}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con10
          annotation (Placement(transformation(extent={{322,142},{302,162}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_2(
            includeBearingConnector=true) annotation (Placement(transformation(extent={{36,222},
                  {56,202}}),           iconTransformation(extent={{-16,178},{4,158}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_1(
            includeBearingConnector=true) annotation (Placement(transformation(extent={{232,214},
                  {252,194}}),          iconTransformation(extent={{214,180},{234,160}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_3(
            includeBearingConnector=true) annotation (Placement(transformation(extent={{-146,
                  214},{-126,194}}),    iconTransformation(extent={{-278,178},{-258,
                  158}})));
      equation
        connect(road_Wheel_Assembly_1.frame_b,belt_leading. flange_a)
                                                                    annotation (Line(
            points={{300,-30.2},{330,-30.2},{330,40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_leading.flange_b,sprocket1. frame_a)    annotation (Line(
            points={{330,60},{330,80},{330.2,80}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_a,idler_Pulley. frame_b)
          annotation (Line(
            points={{-330,60},{-330,78}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_b,road_Wheel_Assembly_6. frame_a)
          annotation (Line(
            points={{-330,40},{-330,-29.8},{-300,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(sprocket1.flange, sprocket) annotation (Line(
            points={{340.2,100},{340.2,120},{340,120},{340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.wheel, road_Wheel_Assembly_1.flange) annotation (Line(
            points={{280,19.6},{280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.hull, frame_susp_1) annotation (Line(
            points={{280,60.4},{280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.wheel, road_Wheel_Assembly_6.flange) annotation (Line(
            points={{-280,19.6},{-280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.hull, frame_susp_6) annotation (Line(
            points={{-280,60.4},{-280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_b, road_Wheel_Assembly_3.frame_a)
          annotation (Line(
            points={{10,-30},{26,-30},{26,-29.8},{38,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con1.flange_b, road_Wheel_Assembly_1.frame_a)
          annotation (Line(
            points={{252,-30},{252,-29.8},{260,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_a, road_Wheel_Assembly_4.frame_b)
          annotation (Line(
            points={{-10,-30},{-12,-30},{-12,-30.2},{-46,-30.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con2.flange_a, road_Wheel_Assembly_6.frame_b)
          annotation (Line(
            points={{-246,-30},{-246,-30.2},{-260,-30.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_4.flange, susp_4.wheel) annotation (Line(
            points={{-66,-19.8},{-66,21.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_4.hull, frame_susp_4) annotation (Line(
            points={{-66,62.4},{-66,88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_susp_3, susp_3.hull) annotation (Line(
            points={{58,92},{58,62.4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_3.wheel, road_Wheel_Assembly_3.flange) annotation (Line(
            points={{58,21.6},{58,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_2.frame_b, belt_Segment_ZERO_FIDELITY_no_con1.flange_a)
          annotation (Line(
            points={{180,-30.2},{220,-30.2},{220,-30},{232,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_susp_2,susp_5. hull) annotation (Line(
            points={{160,90},{160,60.4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_5.wheel,road_Wheel_Assembly_2. flange) annotation (Line(
            points={{160,19.6},{160,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_3.frame_b, belt_Segment_ZERO_FIDELITY_no_con4.flange_a)
          annotation (Line(
            points={{78,-30.2},{91,-30.2},{91,-30},{102,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con4.flange_b, road_Wheel_Assembly_2.frame_a)
          annotation (Line(
            points={{122,-30},{132,-30},{132,-29.8},{140,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_a, belt_Segment_ZERO_FIDELITY_no_con2.flange_b)
          annotation (Line(
            points={{-178,-29.8},{-218,-29.8},{-218,-30},{-226,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.flange,susp_6. wheel) annotation (Line(
            points={{-158,-19.8},{-158,21.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_6.hull,frame_susp_5)  annotation (Line(
            points={{-158,62.4},{-158,88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_b, belt_Segment_ZERO_FIDELITY_no_con5.flange_a)
          annotation (Line(
            points={{-138,-30.2},{-129,-30.2},{-129,-30},{-122,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con5.flange_b, road_Wheel_Assembly_4.frame_a)
          annotation (Line(
            points={{-102,-30},{-94,-30},{-94,-29.8},{-86,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(tensioner.hull, idler) annotation (Line(
            points={{-402,138.4},{-400,138.4},{-400,148},{-340,148},{-340,140}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(tensioner.wheel, idler_Pulley.flange) annotation (Line(
            points={{-396,97.6},{-396,92},{-340,92},{-340,98}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_b, idler_Pulley.frame_a)
          annotation (Line(
            points={{-234,154},{-330,154},{-330,118}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley1.flange,guide_2)  annotation (Line(
            points={{46,162},{46,212}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley2.flange,guide_1)  annotation (Line(
            points={{242,162},{242,204}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_a,guide_Wheel_Pulley3. frame_b)
          annotation (Line(
            points={{-214,154},{-156,154}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley3.flange,guide_3)  annotation (Line(
            points={{-136,164},{-136,204}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley2.frame_a,belt_Segment_ZERO_FIDELITY_no_con10. flange_b)
          annotation (Line(
            points={{262,152},{302,152}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con10.flange_a, sprocket1.frame_b)
          annotation (Line(
            points={{322,152},{329.8,152},{329.8,120}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con8.flange_a,guide_Wheel_Pulley2. frame_b)
          annotation (Line(
            points={{142,152},{222,152}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley1.frame_a,belt_Segment_ZERO_FIDELITY_no_con8. flange_b)
          annotation (Line(
            points={{66,152},{122,152}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con7.flange_a,guide_Wheel_Pulley1. frame_b)
          annotation (Line(
            points={{-34,152},{26,152}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con7.flange_b,guide_Wheel_Pulley3. frame_a)
          annotation (Line(
            points={{-54,152},{-114,152},{-114,154},{-116,154}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false,extent={{-460,-100},
                  {420,220}}), graphics), Icon(coordinateSystem(preserveAspectRatio=false,
                         extent={{-460,-100},{420,220}}),
                                               graphics={
              Text(
                extent={{-86,98},{48,64}},
                lineColor={0,0,255},
                textString="FULL"),
              Ellipse(extent={{-266,40},{-166,-60}},lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{-152,40},{-52,-60}}, lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{2,40},{102,-60}},    lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{122,40},{222,-60}},  lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Text(
                extent={{-198,130},{156,96}},
                lineColor={0,128,0},
                textString="VU"),
              Text(
                extent={{-238,66},{222,30}},
                lineColor={0,0,255},
                textString="Hydro_Pnuematic"),
              Ellipse(extent={{-300,200},{-234,138}},
                                                    lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{-40,200},{26,138}},  lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{190,200},{256,138}}, lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid)}));
      end Suspension_Full_Container_6_Wheels_3g_Hydro_pneumatic_wTensioner;

      model
      Suspension_Full_Container_6_Wheels_3g_Hydro_pneumatic_wTensioner_Decl
      "Implements a full suspension model inside a container"
        extends
        C2M2L_CyPhy.Testing_Blocks.Hull.Suspension_Interface_2_Wheels_idler_frame_Decl;
        parameter Boolean is_left_side = false;
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_1(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{300,-40},{260,0}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_leading(c=100000, d=10000, mass_length=1)
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=90,
              origin={330,50})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Drive_Sprocket_Pulley
          sprocket1(D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=270,
              origin={340,100})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Idler_Pulley
                                                                                     idler_Pulley(
          phi=1.5707963267949,
          m_wheel=10,
          D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-360,78},{-320,118}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con3(mass_length=1)
          annotation (Placement(transformation(extent={{-10,-40},{10,-20}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_6(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-260,-40},{-300,0}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con6(mass_length=1)
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-330,50})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_1(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{260,20},{300,60}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_2(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{-300,20},{-260,60}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_4
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-109,68},{-23,108}}),
              iconTransformation(extent={{-112,122},{-92,142}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_4(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{-86,22},{-46,62}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_4(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-46,-40},{-86,0}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_3(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{38,22},{78,62}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_3(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{78,-40},{38,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_3
                                                                annotation (Placement(
              transformation(rotation=0, extent={{15,72},{101,112}}),
              iconTransformation(extent={{44,124},{64,144}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con1(mass_length=1)
          annotation (Placement(transformation(extent={{232,-40},{252,-20}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con2(mass_length=1)
          annotation (Placement(transformation(extent={{-246,-40},{-226,-20}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_5(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{140,20},{180,60}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_2(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{180,-40},{140,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_2
                                                                annotation (Placement(
              transformation(rotation=0, extent={{117,70},{203,110}}),
              iconTransformation(extent={{162,124},{182,144}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con4(mass_length=1)
          annotation (Placement(transformation(extent={{102,-40},{122,-20}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_6(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{-178,22},{-138,62}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_5(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-138,-40},{-178,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_5
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-201,68},{-115,108}}),
              iconTransformation(extent={{-224,122},{-204,142}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con5(mass_length=1)
          annotation (Placement(transformation(extent={{-122,-40},{-102,-20}})));

        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Rigid_Tensioner_Assembly
          tensioner(
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=-90,
              origin={-396,118})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con9(mass_length=1)
          annotation (Placement(transformation(extent={{-214,144},{-234,164}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley1(
          m_wheel=5,
          stateSelect=StateSelect.always,
          D_wheel=0.6)        annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={46,162})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley2(
          m_wheel=5,
          stateSelect=StateSelect.always,
          D_wheel=0.6)        annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={242,162})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley3(
          m_wheel=5,
          stateSelect=StateSelect.always,
          D_wheel=0.6)        annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={-136,164})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con7(mass_length=1)
          annotation (Placement(transformation(extent={{-34,142},{-54,162}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con8(mass_length=1)
          annotation (Placement(transformation(extent={{142,142},{122,162}})));
        C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_w_Mass
          belt_Segment_ZERO_FIDELITY_no_con10(mass_length=1)
          annotation (Placement(transformation(extent={{322,142},{302,162}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_2(
            includeBearingConnector=true) annotation (Placement(transformation(extent={{36,222},
                  {56,202}}),           iconTransformation(extent={{-16,178},{4,158}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_1(
            includeBearingConnector=true) annotation (Placement(transformation(extent={{232,214},
                  {252,194}}),          iconTransformation(extent={{214,180},{234,160}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing guide_3(
            includeBearingConnector=true) annotation (Placement(transformation(extent={{-146,
                  214},{-126,194}}),    iconTransformation(extent={{-278,178},{-258,
                  158}})));
      equation
        connect(road_Wheel_Assembly_1.frame_b,belt_leading. flange_a)
                                                                    annotation (Line(
            points={{300,-30.2},{330,-30.2},{330,40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_leading.flange_b,sprocket1. frame_a)    annotation (Line(
            points={{330,60},{330,80},{330.2,80}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_a,idler_Pulley. frame_b)
          annotation (Line(
            points={{-330,60},{-330,78}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_b,road_Wheel_Assembly_6. frame_a)
          annotation (Line(
            points={{-330,40},{-330,-29.8},{-300,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(sprocket1.flange, sprocket) annotation (Line(
            points={{340.2,100},{340.2,120},{340,120},{340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.wheel, road_Wheel_Assembly_1.flange) annotation (Line(
            points={{280,19.6},{280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.hull, frame_susp_1) annotation (Line(
            points={{280,60.4},{280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.wheel, road_Wheel_Assembly_6.flange) annotation (Line(
            points={{-280,19.6},{-280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.hull, frame_susp_6) annotation (Line(
            points={{-280,60.4},{-280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_b, road_Wheel_Assembly_3.frame_a)
          annotation (Line(
            points={{10,-30},{26,-30},{26,-29.8},{38,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con1.flange_b, road_Wheel_Assembly_1.frame_a)
          annotation (Line(
            points={{252,-30},{252,-29.8},{260,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_a, road_Wheel_Assembly_4.frame_b)
          annotation (Line(
            points={{-10,-30},{-12,-30},{-12,-30.2},{-46,-30.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con2.flange_a, road_Wheel_Assembly_6.frame_b)
          annotation (Line(
            points={{-246,-30},{-246,-30.2},{-260,-30.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_4.flange, susp_4.wheel) annotation (Line(
            points={{-66,-19.8},{-66,21.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_4.hull, frame_susp_4) annotation (Line(
            points={{-66,62.4},{-66,88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_susp_3, susp_3.hull) annotation (Line(
            points={{58,92},{58,62.4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_3.wheel, road_Wheel_Assembly_3.flange) annotation (Line(
            points={{58,21.6},{58,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_2.frame_b, belt_Segment_ZERO_FIDELITY_no_con1.flange_a)
          annotation (Line(
            points={{180,-30.2},{220,-30.2},{220,-30},{232,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_susp_2,susp_5. hull) annotation (Line(
            points={{160,90},{160,60.4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_5.wheel,road_Wheel_Assembly_2. flange) annotation (Line(
            points={{160,19.6},{160,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_3.frame_b, belt_Segment_ZERO_FIDELITY_no_con4.flange_a)
          annotation (Line(
            points={{78,-30.2},{91,-30.2},{91,-30},{102,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con4.flange_b, road_Wheel_Assembly_2.frame_a)
          annotation (Line(
            points={{122,-30},{132,-30},{132,-29.8},{140,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_a, belt_Segment_ZERO_FIDELITY_no_con2.flange_b)
          annotation (Line(
            points={{-178,-29.8},{-218,-29.8},{-218,-30},{-226,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.flange,susp_6. wheel) annotation (Line(
            points={{-158,-19.8},{-158,21.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_6.hull,frame_susp_5)  annotation (Line(
            points={{-158,62.4},{-158,88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_b, belt_Segment_ZERO_FIDELITY_no_con5.flange_a)
          annotation (Line(
            points={{-138,-30.2},{-129,-30.2},{-129,-30},{-122,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con5.flange_b, road_Wheel_Assembly_4.frame_a)
          annotation (Line(
            points={{-102,-30},{-94,-30},{-94,-29.8},{-86,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(tensioner.hull, idler) annotation (Line(
            points={{-402,138.4},{-400,138.4},{-400,148},{-340,148},{-340,140}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(tensioner.wheel, idler_Pulley.flange) annotation (Line(
            points={{-396,97.6},{-396,92},{-340,92},{-340,98}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_b, idler_Pulley.frame_a)
          annotation (Line(
            points={{-234,154},{-330,154},{-330,118}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley1.flange,guide_2)  annotation (Line(
            points={{46,162},{46,212}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley2.flange,guide_1)  annotation (Line(
            points={{242,162},{242,204}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_a,guide_Wheel_Pulley3. frame_b)
          annotation (Line(
            points={{-214,154},{-156,154}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley3.flange,guide_3)  annotation (Line(
            points={{-136,164},{-136,204}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley2.frame_a,belt_Segment_ZERO_FIDELITY_no_con10. flange_b)
          annotation (Line(
            points={{262,152},{302,152}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con10.flange_a, sprocket1.frame_b)
          annotation (Line(
            points={{322,152},{329.8,152},{329.8,120}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con8.flange_a,guide_Wheel_Pulley2. frame_b)
          annotation (Line(
            points={{142,152},{222,152}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley1.frame_a,belt_Segment_ZERO_FIDELITY_no_con8. flange_b)
          annotation (Line(
            points={{66,152},{122,152}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con7.flange_a,guide_Wheel_Pulley1. frame_b)
          annotation (Line(
            points={{-34,152},{26,152}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con7.flange_b,guide_Wheel_Pulley3. frame_a)
          annotation (Line(
            points={{-54,152},{-114,152},{-114,154},{-116,154}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false,extent={{-460,
                  -100},{420,220}}),
                               graphics), Icon(coordinateSystem(preserveAspectRatio=false,
                         extent={{-460,-100},{420,220}}),
                                               graphics={
              Text(
                extent={{-86,98},{48,64}},
                lineColor={0,0,255},
                textString="FULL"),
              Ellipse(extent={{-266,40},{-166,-60}},lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{-152,40},{-52,-60}}, lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{2,40},{102,-60}},    lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{122,40},{222,-60}},  lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Text(
                extent={{-198,130},{156,96}},
                lineColor={0,128,0},
                textString="VU"),
              Text(
                extent={{-238,66},{222,30}},
                lineColor={0,0,255},
                textString="Hydro_Pnuematic"),
              Ellipse(extent={{-300,200},{-234,138}},
                                                    lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{-40,200},{26,138}},  lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{190,200},{256,138}}, lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid)}));
      end Suspension_Full_Container_6_Wheels_3g_Hydro_pneumatic_wTensioner_Decl;

      model Suspension_Full_Container_6_Wheels_Hydro_pneumatic
      "Implements a full suspension model inside a container"
        extends C2M2L_Ext.Testing_Blocks.Hull.Suspension_Interface_2_Wheels;
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_1(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{300,-40},{260,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_leading(c=100000,
            d=10000)
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=90,
              origin={330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Drive_Sprocket_Pulley
          sprocket1(D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=270,
              origin={340,100})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Idler_Pulley
                                                                                    idler_Pulley(
          phi=1.5707963267949,
          m_wheel=10,
          D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-360,78},{-320,118}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con3
          annotation (Placement(transformation(extent={{-10,-40},{10,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_6(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-260,-40},{-300,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con6
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con9
          annotation (Placement(transformation(extent={{8,120},{-12,140}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_1(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8)
          annotation (Placement(transformation(extent={{260,20},{300,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_2(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8)
          annotation (Placement(transformation(extent={{-300,20},{-260,60}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_4
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-109,68},{-23,108}}),
              iconTransformation(extent={{-112,122},{-92,142}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_4(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8)
          annotation (Placement(transformation(extent={{-86,22},{-46,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_4(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-46,-40},{-86,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_3(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8)
          annotation (Placement(transformation(extent={{38,22},{78,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_3(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{78,-40},{38,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_3
                                                                annotation (Placement(
              transformation(rotation=0, extent={{15,72},{101,112}}),
              iconTransformation(extent={{44,124},{64,144}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con1
          annotation (Placement(transformation(extent={{232,-40},{252,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con2
          annotation (Placement(transformation(extent={{-246,-40},{-226,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_5(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8)
          annotation (Placement(transformation(extent={{140,20},{180,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_2(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{180,-40},{140,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_2
                                                                annotation (Placement(
              transformation(rotation=0, extent={{117,70},{203,110}}),
              iconTransformation(extent={{162,124},{182,144}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con4
          annotation (Placement(transformation(extent={{102,-40},{122,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_6(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8)
          annotation (Placement(transformation(extent={{-178,22},{-138,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_5(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-138,-40},{-178,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_5
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-201,68},{-115,108}}),
              iconTransformation(extent={{-224,122},{-204,142}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con5
          annotation (Placement(transformation(extent={{-122,-40},{-102,-20}})));
      equation
        connect(road_Wheel_Assembly_1.frame_b,belt_leading. flange_a)
                                                                    annotation (Line(
            points={{300,-30.2},{330,-30.2},{330,40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_leading.flange_b,sprocket1. frame_a)    annotation (Line(
            points={{330,60},{330,80},{330.2,80}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_a,idler_Pulley. frame_b)
          annotation (Line(
            points={{-330,60},{-330,78}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_b,road_Wheel_Assembly_6. frame_a)
          annotation (Line(
            points={{-330,40},{-330,-29.8},{-300,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_b, idler_Pulley.frame_a)
          annotation (Line(
            points={{-12,130},{-330,130},{-330,118}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_Pulley.flange, idler) annotation (Line(
            points={{-340,98},{-340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(sprocket1.flange, sprocket) annotation (Line(
            points={{340.2,100},{340.2,120},{340,120},{340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_a, sprocket1.frame_b)
          annotation (Line(
            points={{8,130},{329.8,130},{329.8,120}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.wheel, road_Wheel_Assembly_1.flange) annotation (Line(
            points={{280,19.6},{280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.hull, frame_susp_1) annotation (Line(
            points={{280,60.4},{280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.wheel, road_Wheel_Assembly_6.flange) annotation (Line(
            points={{-280,19.6},{-280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.hull, frame_susp_6) annotation (Line(
            points={{-280,60.4},{-280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_b, road_Wheel_Assembly_3.frame_a)
          annotation (Line(
            points={{10,-30},{26,-30},{26,-29.8},{38,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con1.flange_b, road_Wheel_Assembly_1.frame_a)
          annotation (Line(
            points={{252,-30},{252,-29.8},{260,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_a, road_Wheel_Assembly_4.frame_b)
          annotation (Line(
            points={{-10,-30},{-12,-30},{-12,-30.2},{-46,-30.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con2.flange_a, road_Wheel_Assembly_6.frame_b)
          annotation (Line(
            points={{-246,-30},{-246,-30.2},{-260,-30.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_4.flange, susp_4.wheel) annotation (Line(
            points={{-66,-19.8},{-66,21.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_4.hull, frame_susp_4) annotation (Line(
            points={{-66,62.4},{-66,88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_susp_3, susp_3.hull) annotation (Line(
            points={{58,92},{58,92},{58,62.4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_3.wheel, road_Wheel_Assembly_3.flange) annotation (Line(
            points={{58,21.6},{58,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_2.frame_b, belt_Segment_ZERO_FIDELITY_no_con1.flange_a)
          annotation (Line(
            points={{180,-30.2},{220,-30.2},{220,-30},{232,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_susp_2,susp_5. hull) annotation (Line(
            points={{160,90},{160,60.4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_5.wheel,road_Wheel_Assembly_2. flange) annotation (Line(
            points={{160,19.6},{160,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_3.frame_b, belt_Segment_ZERO_FIDELITY_no_con4.flange_a)
          annotation (Line(
            points={{78,-30.2},{91,-30.2},{91,-30},{102,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con4.flange_b, road_Wheel_Assembly_2.frame_a)
          annotation (Line(
            points={{122,-30},{132,-30},{132,-29.8},{140,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_a, belt_Segment_ZERO_FIDELITY_no_con2.flange_b)
          annotation (Line(
            points={{-178,-29.8},{-218,-29.8},{-218,-30},{-226,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.flange,susp_6. wheel) annotation (Line(
            points={{-158,-19.8},{-158,21.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_6.hull,frame_susp_5)  annotation (Line(
            points={{-158,62.4},{-158,88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_b, belt_Segment_ZERO_FIDELITY_no_con5.flange_a)
          annotation (Line(
            points={{-138,-30.2},{-129,-30.2},{-129,-30},{-122,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con5.flange_b, road_Wheel_Assembly_4.frame_a)
          annotation (Line(
            points={{-102,-30},{-94,-30},{-94,-29.8},{-86,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false,extent={{-460,
                  -100},{420,220}}),
                               graphics), Icon(coordinateSystem(preserveAspectRatio=false,
                         extent={{-460,-100},{420,220}}),
                                               graphics={
              Text(
                extent={{-86,98},{48,64}},
                lineColor={0,0,255},
                textString="FULL"),
              Ellipse(extent={{-266,40},{-166,-60}},lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{-152,40},{-52,-60}}, lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{2,40},{102,-60}},    lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{122,40},{222,-60}},  lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Text(
                extent={{-198,130},{156,96}},
                lineColor={0,128,0},
                textString="VU"),
              Text(
                extent={{-238,66},{222,30}},
                lineColor={0,0,255},
                textString="Hydro_Pnuematic")}));
      end Suspension_Full_Container_6_Wheels_Hydro_pneumatic;

      model Suspension_Full_Container_6_Wheels_Hydro_pneumatic_wTensioner
      "Implements a full suspension model inside a container"
        extends
        C2M2L_CyPhy.Testing_Blocks.Hull.Suspension_Interface_2_Wheels_idler_frame;
        parameter Boolean is_left_side = false;
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_1(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{300,-40},{260,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_leading(c=100000,
            d=10000)
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=90,
              origin={330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Drive_Sprocket_Pulley
          sprocket1(D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=270,
              origin={340,100})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Idler_Pulley
                                                                                    idler_Pulley(
          phi=1.5707963267949,
          m_wheel=10,
          D_wheel=sprocket_diameter)
          annotation (Placement(transformation(extent={{-360,78},{-320,118}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con3
          annotation (Placement(transformation(extent={{-10,-40},{10,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_6(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-260,-40},{-300,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con6
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con9
          annotation (Placement(transformation(extent={{8,120},{-12,140}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_1(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{260,20},{300,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_2(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{-300,20},{-260,60}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_4
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-109,68},{-23,108}}),
              iconTransformation(extent={{-112,122},{-92,142}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_4(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{-86,22},{-46,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_4(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-46,-40},{-86,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_3(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{38,22},{78,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_3(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{78,-40},{38,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_3
                                                                annotation (Placement(
              transformation(rotation=0, extent={{15,72},{101,112}}),
              iconTransformation(extent={{44,124},{64,144}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con1
          annotation (Placement(transformation(extent={{232,-40},{252,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con2
          annotation (Placement(transformation(extent={{-246,-40},{-226,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_5(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{140,20},{180,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_2(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{180,-40},{140,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_2
                                                                annotation (Placement(
              transformation(rotation=0, extent={{117,70},{203,110}}),
              iconTransformation(extent={{162,124},{182,144}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con4
          annotation (Placement(transformation(extent={{102,-40},{122,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Hydropneumatic
          susp_6(
          offset_wheel=0,
          compression_scale=1,
          rebound_scale=0.6,
          m_arm=15,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{-178,22},{-138,62}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_5(D_wheel=wheel_diameter, crr=ground_context.crr)
          annotation (Placement(transformation(extent={{-138,-40},{-178,0}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_5
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-201,68},{-115,108}}),
              iconTransformation(extent={{-224,122},{-204,142}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con5
          annotation (Placement(transformation(extent={{-122,-40},{-102,-20}})));

        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Rigid_Tensioner_Assembly
          tensioner(
          is_left_side=is_left_side)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=-90,
              origin={-396,118})));
      equation
        connect(road_Wheel_Assembly_1.frame_b,belt_leading. flange_a)
                                                                    annotation (Line(
            points={{300,-30.2},{330,-30.2},{330,40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_leading.flange_b,sprocket1. frame_a)    annotation (Line(
            points={{330,60},{330,80},{330.2,80}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_a,idler_Pulley. frame_b)
          annotation (Line(
            points={{-330,60},{-330,78}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_b,road_Wheel_Assembly_6. frame_a)
          annotation (Line(
            points={{-330,40},{-330,-29.8},{-300,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_b, idler_Pulley.frame_a)
          annotation (Line(
            points={{-12,130},{-330,130},{-330,118}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(sprocket1.flange, sprocket) annotation (Line(
            points={{340.2,100},{340.2,120},{340,120},{340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_a, sprocket1.frame_b)
          annotation (Line(
            points={{8,130},{329.8,130},{329.8,120}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.wheel, road_Wheel_Assembly_1.flange) annotation (Line(
            points={{280,19.6},{280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.hull, frame_susp_1) annotation (Line(
            points={{280,60.4},{280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.wheel, road_Wheel_Assembly_6.flange) annotation (Line(
            points={{-280,19.6},{-280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.hull, frame_susp_6) annotation (Line(
            points={{-280,60.4},{-280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_b, road_Wheel_Assembly_3.frame_a)
          annotation (Line(
            points={{10,-30},{26,-30},{26,-29.8},{38,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con1.flange_b, road_Wheel_Assembly_1.frame_a)
          annotation (Line(
            points={{252,-30},{252,-29.8},{260,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_a, road_Wheel_Assembly_4.frame_b)
          annotation (Line(
            points={{-10,-30},{-12,-30},{-12,-30.2},{-46,-30.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con2.flange_a, road_Wheel_Assembly_6.frame_b)
          annotation (Line(
            points={{-246,-30},{-246,-30.2},{-260,-30.2}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_4.flange, susp_4.wheel) annotation (Line(
            points={{-66,-19.8},{-66,21.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_4.hull, frame_susp_4) annotation (Line(
            points={{-66,62.4},{-66,88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_susp_3, susp_3.hull) annotation (Line(
            points={{58,92},{58,62.4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_3.wheel, road_Wheel_Assembly_3.flange) annotation (Line(
            points={{58,21.6},{58,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_2.frame_b, belt_Segment_ZERO_FIDELITY_no_con1.flange_a)
          annotation (Line(
            points={{180,-30.2},{220,-30.2},{220,-30},{232,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(frame_susp_2,susp_5. hull) annotation (Line(
            points={{160,90},{160,60.4}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_5.wheel,road_Wheel_Assembly_2. flange) annotation (Line(
            points={{160,19.6},{160,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_3.frame_b, belt_Segment_ZERO_FIDELITY_no_con4.flange_a)
          annotation (Line(
            points={{78,-30.2},{91,-30.2},{91,-30},{102,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con4.flange_b, road_Wheel_Assembly_2.frame_a)
          annotation (Line(
            points={{122,-30},{132,-30},{132,-29.8},{140,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_a, belt_Segment_ZERO_FIDELITY_no_con2.flange_b)
          annotation (Line(
            points={{-178,-29.8},{-218,-29.8},{-218,-30},{-226,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.flange,susp_6. wheel) annotation (Line(
            points={{-158,-19.8},{-158,21.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_6.hull,frame_susp_5)  annotation (Line(
            points={{-158,62.4},{-158,88}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_b, belt_Segment_ZERO_FIDELITY_no_con5.flange_a)
          annotation (Line(
            points={{-138,-30.2},{-129,-30.2},{-129,-30},{-122,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con5.flange_b, road_Wheel_Assembly_4.frame_a)
          annotation (Line(
            points={{-102,-30},{-94,-30},{-94,-29.8},{-86,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(tensioner.hull, idler) annotation (Line(
            points={{-402,138.4},{-400,138.4},{-400,148},{-340,148},{-340,140}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(tensioner.wheel, idler_Pulley.flange) annotation (Line(
            points={{-396,97.6},{-396,92},{-340,92},{-340,98}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false,extent={{-460,-100},
                  {420,220}}), graphics), Icon(coordinateSystem(preserveAspectRatio=false,
                         extent={{-460,-100},{420,220}}),
                                               graphics={
              Text(
                extent={{-86,98},{48,64}},
                lineColor={0,0,255},
                textString="FULL"),
              Ellipse(extent={{-266,40},{-166,-60}},lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{-152,40},{-52,-60}}, lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{2,40},{102,-60}},    lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{122,40},{222,-60}},  lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Text(
                extent={{-198,130},{156,96}},
                lineColor={0,128,0},
                textString="VU"),
              Text(
                extent={{-238,66},{222,30}},
                lineColor={0,0,255},
                textString="Hydro_Pnuematic")}));
      end Suspension_Full_Container_6_Wheels_Hydro_pneumatic_wTensioner;

      model Suspension_Full_Container_Hydro_Pnuematic_Tensioner
      "Implements a full suspension model inside a container, with track tensioner"
        extends C2M2L_Ext.Testing_Blocks.Hull.Suspension_Interface_Tensioner;
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_1(
          D_wheel=wheel_diameter,
          stateSelect=StateSelect.always,
          c_tire=c_tire,
          d_tire=d_tire,
          m_wheel=m_roadwheel,
          W_wheel=wheel_width,
          Z_offset=wheel_offset,
          J_xxyyzz=moi_wheel)
          annotation (Placement(transformation(extent={{300,-40},{260,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_leading(
          f_initial=-15000,
          c=c_track,
          d=d_track)
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=90,
              origin={330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Drive_Sprocket_Pulley
          sprocket1(
          D_wheel=sprocket_diameter,
          stateSelect=StateSelect.always,
          c_tire=c_tire,
          d_tire=d_tire,
          crr=crr,
          m_wheel=m_sprocket,
          W_wheel=sprocket_width,
          J_xxyyzz=moi_sprocket)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=270,
              origin={340,100})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Idler_Pulley
                                                                                    idler_Pulley(
          stateSelect=StateSelect.always,
          D_wheel=idler_diameter,
          m_wheel=m_idler,
          W_wheel=idler_width,
          phi=1.5707963267949,
          J_xxyyzz=moi_idler)
          annotation (Placement(transformation(extent={{-360,20},{-320,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con3(
          f_initial=-tension,
          c=c_track,
          d=d_track)
          annotation (Placement(transformation(extent={{-10,-40},{10,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_6(
          D_wheel=wheel_diameter,
          stateSelect=StateSelect.always,
          c_tire=c_tire,
          d_tire=d_tire,
          m_wheel=m_roadwheel,
          W_wheel=wheel_width,
          Z_offset=wheel_offset,
          J_xxyyzz=moi_wheel)
          annotation (Placement(transformation(extent={{-260,-40},{-300,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con6(
          f_initial=-tension,
          c=c_track,
          d=d_track)
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-330,-10})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con9(
          f_initial=-tension,
          c=c_track,
          d=d_track)
          annotation (Placement(transformation(extent={{-100,120},{-120,140}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_1(
          bracket_height_CG=0,
          is_left_side=is_left_side,
          lever_arm_CG=0.221,
          offset_CG=0.10,
          m_arm=m_suspension,
          lever_arm_wheel=lever_arm_wheel,
          offset_wheel=offset_wheel,
          J_xxyyzz=moi_suspension,
          compression_scale=compression_scale,
          rebound_scale=rebound_scale)
          annotation (Placement(transformation(extent={{260,20},{300,60}})));

        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_2(
          bracket_height_CG=0,
          is_left_side=is_left_side,
          lever_arm_CG=0.221,
          offset_CG=0.10,
          m_arm=m_suspension,
          lever_arm_wheel=lever_arm_wheel,
          offset_wheel=offset_wheel,
          J_xxyyzz=moi_suspension,
          compression_scale=compression_scale,
          rebound_scale=rebound_scale)
          annotation (Placement(transformation(extent={{-300,20},{-260,60}})));

        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_3(
          bracket_height_CG=0,
          is_left_side=is_left_side,
          lever_arm_CG=0.221,
          offset_CG=0.10,
          m_arm=m_suspension,
          lever_arm_wheel=lever_arm_wheel,
          offset_wheel=offset_wheel,
          J_xxyyzz=moi_suspension,
          compression_scale=compression_scale,
          rebound_scale=rebound_scale)
          annotation (Placement(transformation(extent={{-220,20},{-180,60}})));

        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_4(
          bracket_height_CG=0,
          is_left_side=is_left_side,
          lever_arm_CG=0.221,
          offset_CG=0.10,
          m_arm=m_suspension,
          lever_arm_wheel=lever_arm_wheel,
          offset_wheel=offset_wheel,
          J_xxyyzz=moi_suspension,
          compression_scale=compression_scale,
          rebound_scale=rebound_scale)
          annotation (Placement(transformation(extent={{-140,20},{-100,60}})));

        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_5(
          bracket_height_CG=0,
          is_left_side=is_left_side,
          lever_arm_CG=0.221,
          offset_CG=0.10,
          m_arm=m_suspension,
          lever_arm_wheel=lever_arm_wheel,
          offset_wheel=offset_wheel,
          J_xxyyzz=moi_suspension,
          compression_scale=compression_scale,
          rebound_scale=rebound_scale)
          annotation (Placement(transformation(extent={{100,20},{140,60}})));

        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_6(
          bracket_height_CG=0,
          is_left_side=is_left_side,
          lever_arm_CG=0.221,
          offset_CG=0.10,
          m_arm=m_suspension,
          lever_arm_wheel=lever_arm_wheel,
          offset_wheel=offset_wheel,
          J_xxyyzz=moi_suspension,
          compression_scale=compression_scale,
          rebound_scale=rebound_scale)
          annotation (Placement(transformation(extent={{180,20},{220,60}})));

        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_2(
          D_wheel=wheel_diameter,
          stateSelect=StateSelect.always,
          c_tire=c_tire,
          d_tire=d_tire,
          m_wheel=m_roadwheel,
          W_wheel=wheel_width,
          Z_offset=wheel_offset,
          J_xxyyzz=moi_wheel)
          annotation (Placement(transformation(extent={{-180,-40},{-220,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_3(
          D_wheel=wheel_diameter,
          stateSelect=StateSelect.always,
          c_tire=c_tire,
          d_tire=d_tire,
          m_wheel=m_roadwheel,
          W_wheel=wheel_width,
          Z_offset=wheel_offset,
          J_xxyyzz=moi_wheel)
          annotation (Placement(transformation(extent={{-100,-40},{-140,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_4(
          D_wheel=wheel_diameter,
          stateSelect=StateSelect.always,
          c_tire=c_tire,
          d_tire=d_tire,
          m_wheel=m_roadwheel,
          W_wheel=wheel_width,
          Z_offset=wheel_offset,
          J_xxyyzz=moi_wheel)
          annotation (Placement(transformation(extent={{140,-40},{100,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_5(
          D_wheel=wheel_diameter,
          stateSelect=StateSelect.always,
          c_tire=c_tire,
          d_tire=d_tire,
          m_wheel=m_roadwheel,
          W_wheel=wheel_width,
          Z_offset=wheel_offset,
          J_xxyyzz=moi_wheel)
          annotation (Placement(transformation(extent={{220,-40},{180,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con1(
          f_initial=-tension,
          c=c_track,
          d=d_track)
          annotation (Placement(transformation(extent={{-250,-40},{-230,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con2(
          f_initial=-tension,
          c=c_track,
          d=d_track)
          annotation (Placement(transformation(extent={{-170,-40},{-150,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con4(
          f_initial=-tension,
          c=c_track,
          d=d_track)
          annotation (Placement(transformation(extent={{150,-40},{170,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con5(
          f_initial=-tension,
          c=c_track,
          d=d_track)
          annotation (Placement(transformation(extent={{230,-40},{250,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley(
          stateSelect=StateSelect.always,
          D_wheel=guide_diameter,
          W_wheel=guide_width,
          m_wheel=m_guide,
          J_xxyyzz=moi_guide)
                             annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={-220,140})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley1(
          stateSelect=StateSelect.always,
          D_wheel=guide_diameter,
          W_wheel=guide_width,
          m_wheel=m_guide,
          J_xxyyzz=moi_guide) annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={0,140})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley2(
          stateSelect=StateSelect.always,
          D_wheel=guide_diameter,
          W_wheel=guide_width,
          m_wheel=m_guide,
          J_xxyyzz=moi_guide) annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={220,140})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con7(
          f_initial=-tension,
          c=c_track,
          d=d_track)
          annotation (Placement(transformation(extent={{120,120},{100,140}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con8(
          f_initial=-tension,
          c=c_track,
          d=d_track)
          annotation (Placement(transformation(extent={{-270,120},{-290,140}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_Segment_ZERO_FIDELITY_no_con10(
          f_initial=-tension,
          c=c_track,
          d=d_track)
          annotation (Placement(transformation(extent={{300,120},{280,140}})));


        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Rigid_Tensioner_Assembly
          tensioner(
          is_left_side=is_left_side,
          lever_arm_wheel=lever_arm_idler,
          offset_wheel=offset_idler,
          initial_angle=initial_angle,
          lever_arm_tensioner=lever_arm_tensioner,
          m_arm=m_idler_arm,
          ideal_length=ideal_length,
          m_tensioner=m_tensioner,
          J_xxyyzz=moi_assembly,
          offset_tensioner=offset_tensioner)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=-90,
              origin={-380,100})));
      equation
        connect(road_Wheel_Assembly_1.frame_b,belt_leading. flange_a)
                                                                    annotation (Line(
            points={{300,-30.2},{330,-30.2},{330,40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_leading.flange_b,sprocket1. frame_a)    annotation (Line(
            points={{330,60},{330,80},{330.2,80}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_a,idler_Pulley. frame_b)
          annotation (Line(
            points={{-330,0},{-330,20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_b,road_Wheel_Assembly_6. frame_a)
          annotation (Line(
            points={{-330,-20},{-330,-29.8},{-300,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(sprocket1.flange, sprocket) annotation (Line(
            points={{340.2,100},{340.2,120},{340,120},{340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.wheel, road_Wheel_Assembly_1.flange) annotation (Line(
            points={{280,19.6},{280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.hull, frame_susp_1) annotation (Line(
            points={{280,60.4},{280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.wheel, road_Wheel_Assembly_6.flange) annotation (Line(
            points={{-280,19.6},{-280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.hull, frame_susp_6) annotation (Line(
            points={{-280,60.4},{-280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_6.frame_b, belt_Segment_ZERO_FIDELITY_no_con1.flange_a)
          annotation (Line(
            points={{-260,-30.2},{-255,-30.2},{-255,-30},{-250,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con1.flange_b, road_Wheel_Assembly_2.frame_a)
          annotation (Line(
            points={{-230,-30},{-225,-30},{-225,-29.8},{-220,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_2.frame_b, belt_Segment_ZERO_FIDELITY_no_con2.flange_a)
          annotation (Line(
            points={{-180,-30.2},{-176,-30.2},{-176,-30},{-170,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con2.flange_b, road_Wheel_Assembly_3.frame_a)
          annotation (Line(
            points={{-150,-30},{-145,-30},{-145,-29.8},{-140,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_3.frame_b, belt_Segment_ZERO_FIDELITY_no_con3.flange_a)
          annotation (Line(
            points={{-100,-30.2},{-55,-30.2},{-55,-30},{-10,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_b, road_Wheel_Assembly_4.frame_a)
          annotation (Line(
            points={{10,-30},{55,-30},{55,-29.8},{100,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_4.frame_b, belt_Segment_ZERO_FIDELITY_no_con4.flange_a)
          annotation (Line(
            points={{140,-30.2},{146,-30.2},{146,-30},{150,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con4.flange_b, road_Wheel_Assembly_5.frame_a)
          annotation (Line(
            points={{170,-30},{175,-30},{175,-29.8},{180,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_b, belt_Segment_ZERO_FIDELITY_no_con5.flange_a)
          annotation (Line(
            points={{220,-30.2},{228,-30.2},{228,-30},{230,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con5.flange_b, road_Wheel_Assembly_1.frame_a)
          annotation (Line(
            points={{250,-30},{256,-30},{256,-29.8},{260,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.flange, susp_6.wheel) annotation (Line(
            points={{200,-19.8},{200,19.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_4.flange, susp_5.wheel) annotation (Line(
            points={{120,-19.8},{120,19.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_3.flange, susp_4.wheel) annotation (Line(
            points={{-120,-19.8},{-120,19.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_2.flange, susp_3.wheel) annotation (Line(
            points={{-200,-19.8},{-200,19.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_3.hull, frame_susp_5) annotation (Line(
            points={{-200,60.4},{-200,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_4.hull, frame_susp_4) annotation (Line(
            points={{-120,60.4},{-120,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_5.hull, frame_susp_3) annotation (Line(
            points={{120,60.4},{120,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_6.hull, frame_susp_2) annotation (Line(
            points={{200,60.4},{200,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley.flange, guide_3) annotation (Line(
            points={{-220,140},{-220,180}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_b, guide_Wheel_Pulley.frame_a)
          annotation (Line(
            points={{-120,130},{-200,130}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_a, guide_Wheel_Pulley1.frame_b)
          annotation (Line(
            points={{-100,130},{-20,130}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley1.frame_a, belt_Segment_ZERO_FIDELITY_no_con7.flange_b)
          annotation (Line(
            points={{20,130},{100,130}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con7.flange_a, guide_Wheel_Pulley2.frame_b)
          annotation (Line(
            points={{120,130},{200,130}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley1.flange, guide_2) annotation (Line(
            points={{0,140},{0,180}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley2.flange, guide_1) annotation (Line(
            points={{220,140},{220,180}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con10.flange_b, guide_Wheel_Pulley2.frame_a)
          annotation (Line(
            points={{280,130},{240,130}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con10.flange_a, sprocket1.frame_b)
          annotation (Line(
            points={{300,130},{329.8,130},{329.8,120}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con8.flange_b, idler_Pulley.frame_a)
          annotation (Line(
            points={{-290,130},{-330,130},{-330,60}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con8.flange_a, guide_Wheel_Pulley.frame_b)
          annotation (Line(
            points={{-270,130},{-240,130}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(tensioner.wheel, idler_Pulley.flange) annotation (Line(
            points={{-380,79.6},{-380,40},{-340,40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(tensioner.hull, idler) annotation (Line(
            points={{-386,120.4},{-386,140},{-340,140}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=true, extent={{-460,
                  -100},{420,220}}),
                               graphics), Icon(graphics={
              Text(
                extent={{-86,98},{48,64}},
                lineColor={0,0,255},
                textString="FULL"),
              Text(
                extent={{-260,58},{200,22}},
                lineColor={0,0,255},
                textString="Hydro_Pnuematic"),
              Text(
                extent={{-200,136},{154,102}},
                lineColor={0,128,0},
                textString="VU")}));
      end Suspension_Full_Container_Hydro_Pnuematic_Tensioner;

      model Suspension_Full_Container_modified
      "Implements a full suspension model inside a container"
        extends C2M2L_Ext.Testing_Blocks.Hull.Suspension_Interface;
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_1(D_wheel=wheel_diameter, stateSelect=StateSelect.always)
          annotation (Placement(transformation(extent={{300,-40},{260,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment
                                                                                    belt_leading(f_initial=
             -tension)
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=90,
              origin={330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Drive_Sprocket_Pulley
          sprocket1(D_wheel=sprocket_diameter, stateSelect=StateSelect.always)
          annotation (Placement(transformation(extent={{-20,-20},{20,20}},
              rotation=270,
              origin={340,100})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Idler_Pulley
                                                                                    idler_Pulley(
          phi=1.5707963267949,
          m_wheel=10,
          D_wheel=sprocket_diameter,
          stateSelect=StateSelect.always)
          annotation (Placement(transformation(extent={{-360,78},{-320,118}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_ZERO_FIDELITY_no_con
          belt_Segment_ZERO_FIDELITY_no_con3
          annotation (Placement(transformation(extent={{-10,-40},{10,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_6(D_wheel=wheel_diameter, stateSelect=StateSelect.always)
          annotation (Placement(transformation(extent={{-260,-40},{-300,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_ZERO_FIDELITY_no_con
          belt_Segment_ZERO_FIDELITY_no_con6
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=270,
              origin={-330,50})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_ZERO_FIDELITY_no_con
          belt_Segment_ZERO_FIDELITY_no_con9
          annotation (Placement(transformation(extent={{-100,120},{-120,140}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_1(
          m_damper=1,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side,
          offset_wheel=0.1,
          c_bar=2500)
          annotation (Placement(transformation(extent={{260,20},{300,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_2(
          m_damper=1,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side,
          offset_wheel=0.1,
          c_bar=2500)
          annotation (Placement(transformation(extent={{-300,20},{-260,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_3(
          m_damper=1,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side,
          offset_wheel=0.1,
          c_bar=2500)
          annotation (Placement(transformation(extent={{-220,20},{-180,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_4(
          m_damper=1,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side,
          offset_wheel=0.1,
          c_bar=2500)
          annotation (Placement(transformation(extent={{-140,20},{-100,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_5(
          m_damper=1,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side,
          offset_wheel=0.1,
          c_bar=2500)
          annotation (Placement(transformation(extent={{100,20},{140,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.TrackSusp_SubAssembly_Torsion_Trans_Damper_Range
          susp_6(
          m_damper=1,
          compression_scale=1,
          f_v_table_max=[-1.5,-40549.296875; -1.375,-39333.6639671326; -1.25,-38569.0114746094;
              -1.125,-37648.2355918884; -1,-36157; -0.875,-33952.3727989197; -0.75,-31169.2145996094;
              -0.625,-28154.3177757263; -0.5,-25328.296875; -0.375,-22975.2301902771;
              -0.25,-20960.0524902344; -0.125,-18373.6989097595; -0.1,-17628.712195;
              -0.05,-15734.3095108594; -0.01,-13702.9943094687; 0,0; 0.01,6670; 0.05,
              12904.4407273906; 0.1,14208.796253; 0.125,14744.2626991272; 0.25,
              16805.5764160156; 0.375,18651.4111061096; 0.5,20815.203125; 0.625,
              23349.2517433167; 0.75,26056.5549316406; 0.875,28667.5844154358; 1,
              30962; 1.125,32835.3031654358; 1.25,34310.4299316406; 1.375,
              35494.2829933167; 1.5,36479.203125],
          f_v_table_min=[-1.5,-15539.8815625; -1.375,-13702.1994080353; -1.25,-11911.5988720703;
              -1.125,-10035.9884247589; -1,-8115.28; -0.875,-6282.0744355011; -0.75,-4693.13246582031;
              -0.625,-3471.63126838684; -0.5,-2660.2065625; -0.375,-2184.78026130676;
              -0.25,-1829.17367675781; -0.125,-1220.50627754211; -0.1,-1024.2495169;
              -0.05,-518.487198545312; -0.01,18.8136284481731; 0,0; 0.01,
              618.34740751998; 0.05,738.3195499375; 0.1,923.340798; 0.125,
              1035.63191772461; 0.25,1859.6598046875; 0.375,3201.07604614258; 0.5,
              5073.25375; 0.625,7399.2298425293; 0.75,10046.1281640625; 0.875,
              12859.5825646973; 1,15698.16; 1.125,18467.783626709; 1.25,
              21156.1558984375; 1.375,23867.181661377; 1.5,26855.39125],
          rebound_scale=0.6,
          m_arm=15,
          m_bar=11,
          lever_arm_damper=0.6,
          bracket_height_damper=0.05,
          offset_damper=-0.05,
          lever_arm_CG=0.5,
          bracket_height_CG=0,
          offset_CG=0,
          L_max=1,
          L_min=0.5,
          damper_top_mount_x=-1.2,
          damper_top_mount_y=0.1,
          damper_top_mount_z=-0.05,
          lever_arm_wheel=0.8,
          is_left_side=is_left_side,
          offset_wheel=0.1,
          c_bar=2500)
          annotation (Placement(transformation(extent={{180,20},{220,60}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_2(D_wheel=wheel_diameter, stateSelect=StateSelect.always)
          annotation (Placement(transformation(extent={{-180,-40},{-220,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_3(D_wheel=wheel_diameter, stateSelect=StateSelect.always)
          annotation (Placement(transformation(extent={{-100,-40},{-140,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_4(D_wheel=wheel_diameter, stateSelect=StateSelect.always)
          annotation (Placement(transformation(extent={{140,-40},{100,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Road_Wheel_Assembly
          road_Wheel_Assembly_5(D_wheel=wheel_diameter, stateSelect=StateSelect.always)
          annotation (Placement(transformation(extent={{220,-40},{180,0}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_ZERO_FIDELITY_no_con
          belt_Segment_ZERO_FIDELITY_no_con1
          annotation (Placement(transformation(extent={{-250,-40},{-230,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_ZERO_FIDELITY_no_con
          belt_Segment_ZERO_FIDELITY_no_con2
          annotation (Placement(transformation(extent={{-170,-40},{-150,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_ZERO_FIDELITY_no_con
          belt_Segment_ZERO_FIDELITY_no_con4
          annotation (Placement(transformation(extent={{150,-40},{170,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_ZERO_FIDELITY_no_con
          belt_Segment_ZERO_FIDELITY_no_con5
          annotation (Placement(transformation(extent={{230,-40},{250,-20}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley(
          D_wheel=guide_wheel_diameter,
          m_wheel=10,
          stateSelect=StateSelect.always)
                             annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={-220,140})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley1(
          D_wheel=guide_wheel_diameter,
          m_wheel=5,
          stateSelect=StateSelect.always)
                              annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={0,140})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Guide_Wheel_Pulley
          guide_Wheel_Pulley2(
          D_wheel=guide_wheel_diameter,
          m_wheel=5,
          stateSelect=StateSelect.always)
                              annotation (Placement(transformation(
              extent={{-20,-20},{20,20}},
              rotation=270,
              origin={220,140})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_ZERO_FIDELITY_no_con
          belt_Segment_ZERO_FIDELITY_no_con7
          annotation (Placement(transformation(extent={{120,120},{100,140}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_ZERO_FIDELITY_no_con
          belt_Segment_ZERO_FIDELITY_no_con8
          annotation (Placement(transformation(extent={{-270,120},{-290,140}})));
        C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Suspension.Belt_Segment_ZERO_FIDELITY_no_con
          belt_Segment_ZERO_FIDELITY_no_con10
          annotation (Placement(transformation(extent={{300,120},{280,140}})));

      equation
        connect(road_Wheel_Assembly_1.frame_b,belt_leading. flange_a)
                                                                    annotation (Line(
            points={{300,-30.2},{330,-30.2},{330,40}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_leading.flange_b,sprocket1. frame_a)    annotation (Line(
            points={{330,60},{330,80},{330.2,80}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_a,idler_Pulley. frame_b)
          annotation (Line(
            points={{-330,60},{-330,78}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con6.flange_b,road_Wheel_Assembly_6. frame_a)
          annotation (Line(
            points={{-330,40},{-330,-29.8},{-300,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idler_Pulley.flange, idler) annotation (Line(
            points={{-340,98},{-340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(sprocket1.flange, sprocket) annotation (Line(
            points={{340.2,100},{340.2,120},{340,120},{340,140}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.wheel, road_Wheel_Assembly_1.flange) annotation (Line(
            points={{280,19.6},{280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_1.hull, frame_susp_1) annotation (Line(
            points={{280,60.4},{280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.wheel, road_Wheel_Assembly_6.flange) annotation (Line(
            points={{-280,19.6},{-280,-19.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_2.hull, frame_susp_6) annotation (Line(
            points={{-280,60.4},{-280,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_6.frame_b, belt_Segment_ZERO_FIDELITY_no_con1.flange_a)
          annotation (Line(
            points={{-260,-30.2},{-255,-30.2},{-255,-30},{-250,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con1.flange_b, road_Wheel_Assembly_2.frame_a)
          annotation (Line(
            points={{-230,-30},{-225,-30},{-225,-29.8},{-220,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_2.frame_b, belt_Segment_ZERO_FIDELITY_no_con2.flange_a)
          annotation (Line(
            points={{-180,-30.2},{-176,-30.2},{-176,-30},{-170,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con2.flange_b, road_Wheel_Assembly_3.frame_a)
          annotation (Line(
            points={{-150,-30},{-145,-30},{-145,-29.8},{-140,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_3.frame_b, belt_Segment_ZERO_FIDELITY_no_con3.flange_a)
          annotation (Line(
            points={{-100,-30.2},{-55,-30.2},{-55,-30},{-10,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con3.flange_b, road_Wheel_Assembly_4.frame_a)
          annotation (Line(
            points={{10,-30},{55,-30},{55,-29.8},{100,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_4.frame_b, belt_Segment_ZERO_FIDELITY_no_con4.flange_a)
          annotation (Line(
            points={{140,-30.2},{146,-30.2},{146,-30},{150,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con4.flange_b, road_Wheel_Assembly_5.frame_a)
          annotation (Line(
            points={{170,-30},{175,-30},{175,-29.8},{180,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.frame_b, belt_Segment_ZERO_FIDELITY_no_con5.flange_a)
          annotation (Line(
            points={{220,-30.2},{228,-30.2},{228,-30},{230,-30}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con5.flange_b, road_Wheel_Assembly_1.frame_a)
          annotation (Line(
            points={{250,-30},{256,-30},{256,-29.8},{260,-29.8}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_5.flange, susp_6.wheel) annotation (Line(
            points={{200,-19.8},{200,19.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_4.flange, susp_5.wheel) annotation (Line(
            points={{120,-19.8},{120,19.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_3.flange, susp_4.wheel) annotation (Line(
            points={{-120,-19.8},{-120,19.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(road_Wheel_Assembly_2.flange, susp_3.wheel) annotation (Line(
            points={{-200,-19.8},{-200,19.6}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_3.hull, frame_susp_5) annotation (Line(
            points={{-200,60.4},{-200,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_4.hull, frame_susp_4) annotation (Line(
            points={{-120,60.4},{-120,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_5.hull, frame_susp_3) annotation (Line(
            points={{120,60.4},{120,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(susp_6.hull, frame_susp_2) annotation (Line(
            points={{200,60.4},{200,90}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley.flange, guide_3) annotation (Line(
            points={{-220,140},{-220,180}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_b, guide_Wheel_Pulley.frame_a)
          annotation (Line(
            points={{-120,130},{-200,130}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con9.flange_a, guide_Wheel_Pulley1.frame_b)
          annotation (Line(
            points={{-100,130},{-20,130}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley1.frame_a, belt_Segment_ZERO_FIDELITY_no_con7.flange_b)
          annotation (Line(
            points={{20,130},{100,130}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con7.flange_a, guide_Wheel_Pulley2.frame_b)
          annotation (Line(
            points={{120,130},{200,130}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley1.flange, guide_2) annotation (Line(
            points={{0,140},{0,180}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(guide_Wheel_Pulley2.flange, guide_1) annotation (Line(
            points={{220,140},{220,180}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con10.flange_b, guide_Wheel_Pulley2.frame_a)
          annotation (Line(
            points={{280,130},{240,130}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con10.flange_a, sprocket1.frame_b)
          annotation (Line(
            points={{300,130},{329.8,130},{329.8,120}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con8.flange_b, idler_Pulley.frame_a)
          annotation (Line(
            points={{-290,130},{-330,130},{-330,118}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(belt_Segment_ZERO_FIDELITY_no_con8.flange_a, guide_Wheel_Pulley.frame_b)
          annotation (Line(
            points={{-270,130},{-240,130}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=true, extent={{-460,
                  -100},{420,220}}),
                               graphics), Icon(graphics={
              Text(
                extent={{-86,98},{48,64}},
                lineColor={0,0,255},
                textString="FULL")}));
      end Suspension_Full_Container_modified;

      partial model Suspension_Interface_2_Wheels_idler_frame
      "Provides an interface that can wrap suspension implementations"

        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing sprocket(
            includeBearingConnector=true) annotation (Placement(transformation(extent={{330,150},
                  {350,130}}),           iconTransformation(extent={{340,120},{360,
                  100}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_6
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-323,70},{-237,110}}),
              iconTransformation(extent={{-330,120},{-310,140}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_1
                                                                 annotation (
            Placement(transformation(rotation=0, extent={{237,70},{323,110}}),
              iconTransformation(extent={{270,120},{290,140}})));
    protected
        parameter SI.Force max_brg_force = 1e6;
    public
        parameter Real crr=0.01 "coefficient of rolling resistance";
        parameter SI.Distance sprocket_diameter=0.6 "Diameter of sprocket";
        parameter SI.Distance wheel_diameter=0.6 "Diameter of road wheels";
        outer C2M2L_Ext.Testing_Blocks.Land.Terrains.Ground_Definition_2D ground_context
          annotation (Placement(transformation(extent={{-440,180},{-420,200}})));

        Modelica.Mechanics.MultiBody.Interfaces.Frame_a idler
          annotation (Placement(transformation(extent={{-356,124},{-324,156}}),
              iconTransformation(extent={{-410,96},{-378,128}})));
        annotation (Diagram(coordinateSystem(extent={{-460,-100},{420,220}},
                preserveAspectRatio=false,
              initialScale=0.2),           graphics), Icon(coordinateSystem(extent={{-460,
                  -100},{420,220}},      preserveAspectRatio=false,
              initialScale=0.2),                                    graphics={
              Polygon(
                points={{-406,156},{-274,200},{232,200},{366,156},{396,90},{332,-34},{
                    292,-60},{-336,-60},{-376,-34},{-438,98},{-406,156}},
                lineColor={175,175,175},
                pattern=LinePattern.Dash,
                smooth=Smooth.None,
                fillColor={250,240,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{300,160},{400,60}},  lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={215,215,215},
                fillPattern=FillPattern.Solid),
              Line(
                points={{-276,200},{-410,156}},
                color={175,175,175},
                smooth=Smooth.None,
                thickness=0.5),
              Line(
                points={{364,158},{240,198}},
                color={175,175,175},
                smooth=Smooth.None,
                thickness=0.5),
              Line(
                points={{398,96},{334,-34}},
                color={175,175,175},
                smooth=Smooth.None,
                thickness=0.5),
              Line(
                points={{-438,94},{-374,-36}},
                color={175,175,175},
                smooth=Smooth.None,
                thickness=0.5),
              Line(
                points={{292,-60},{-332,-60}},
                color={175,175,175},
                smooth=Smooth.None,
                thickness=0.5),
              Line(
                points={{230,200},{-272,200}},
                color={175,175,175},
                smooth=Smooth.None,
                thickness=0.5),
              Ellipse(extent={{-440,160},{-340,60}},lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{-380,40},{-280,-60}},lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{240,40},{340,-60}},  lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Polygon(
                points={{386,144},{382,150},{374,154},{368,156},{362,158},{360,154},{
                    352,154},{342,156},{338,158},{332,156},{324,152},{318,148},{314,
                    144},{314,140},{314,134},{312,130},{306,124},{302,122},{300,116},
                    {300,110},{300,102},{302,94},{308,92},{312,88},{316,82},{316,74},
                    {320,70},{328,66},{334,64},{338,62},{344,66},{350,66},{356,66},{
                    360,62},{368,64},{376,68},{382,72},{382,78},{386,84},{392,92},{
                    398,96},{400,102},{400,114},{398,122},{392,126},{388,134},{386,
                    144}},
                lineColor={128,128,128},
                smooth=Smooth.None,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid,
                lineThickness=0.5)}));
      end Suspension_Interface_2_Wheels_idler_frame;

      partial model Suspension_Interface_2_Wheels_idler_frame_Decl
      "Provides an interface that can wrap suspension implementations"

        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing sprocket(
            includeBearingConnector=true) annotation (Placement(transformation(extent={{330,150},
                  {350,130}}),           iconTransformation(extent={{340,120},{360,
                  100}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_6
                                                                annotation (Placement(
              transformation(rotation=0, extent={{-323,70},{-237,110}}),
              iconTransformation(extent={{-330,120},{-310,140}})));
        Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_susp_1
                                                                 annotation (
            Placement(transformation(rotation=0, extent={{237,70},{323,110}}),
              iconTransformation(extent={{270,120},{290,140}})));
    protected
        parameter SI.Force max_brg_force = 1e6;
    public
        parameter Real crr=0.01 "coefficient of rolling resistance";
        parameter SI.Distance sprocket_diameter=0.6 "Diameter of sprocket";
        parameter SI.Distance wheel_diameter=0.6 "Diameter of road wheels";
        outer C2M2L_Decl.Testing_Blocks.Land.Terrains.Ground_Definition_2D ground_context
          annotation (Placement(transformation(extent={{-440,180},{-420,200}})));

        Modelica.Mechanics.MultiBody.Interfaces.Frame_a idler
          annotation (Placement(transformation(extent={{-356,124},{-324,156}}),
              iconTransformation(extent={{-410,96},{-378,128}})));
        annotation (Diagram(coordinateSystem(extent={{-460,-100},{420,220}},
                preserveAspectRatio=false,
              initialScale=0.2),           graphics), Icon(coordinateSystem(extent={{-460,
                  -100},{420,220}},      preserveAspectRatio=false,
              initialScale=0.2),                                    graphics={
              Polygon(
                points={{-406,156},{-274,200},{232,200},{366,156},{396,90},{332,-34},{
                    292,-60},{-336,-60},{-376,-34},{-438,98},{-406,156}},
                lineColor={175,175,175},
                pattern=LinePattern.Dash,
                smooth=Smooth.None,
                fillColor={250,240,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{300,160},{400,60}},  lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={215,215,215},
                fillPattern=FillPattern.Solid),
              Line(
                points={{-276,200},{-410,156}},
                color={175,175,175},
                smooth=Smooth.None,
                thickness=0.5),
              Line(
                points={{364,158},{240,198}},
                color={175,175,175},
                smooth=Smooth.None,
                thickness=0.5),
              Line(
                points={{398,96},{334,-34}},
                color={175,175,175},
                smooth=Smooth.None,
                thickness=0.5),
              Line(
                points={{-438,94},{-374,-36}},
                color={175,175,175},
                smooth=Smooth.None,
                thickness=0.5),
              Line(
                points={{292,-60},{-332,-60}},
                color={175,175,175},
                smooth=Smooth.None,
                thickness=0.5),
              Line(
                points={{230,200},{-272,200}},
                color={175,175,175},
                smooth=Smooth.None,
                thickness=0.5),
              Ellipse(extent={{-440,160},{-340,60}},lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{-380,40},{-280,-60}},lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Ellipse(extent={{240,40},{340,-60}},  lineColor={128,128,128},
                lineThickness=0.5,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid),
              Polygon(
                points={{386,144},{382,150},{374,154},{368,156},{362,158},{360,154},{
                    352,154},{342,156},{338,158},{332,156},{324,152},{318,148},{314,
                    144},{314,140},{314,134},{312,130},{306,124},{302,122},{300,116},
                    {300,110},{300,102},{302,94},{308,92},{312,88},{316,82},{316,74},
                    {320,70},{328,66},{334,64},{338,62},{344,66},{350,66},{356,66},{
                    360,62},{368,64},{376,68},{382,72},{382,78},{386,84},{392,92},{
                    398,96},{400,102},{400,114},{398,122},{392,126},{388,134},{386,
                    144}},
                lineColor={128,128,128},
                smooth=Smooth.None,
                fillColor={245,225,255},
                fillPattern=FillPattern.Solid,
                lineThickness=0.5)}));
      end Suspension_Interface_2_Wheels_idler_frame_Decl;
    annotation (Icon(graphics={
            Ellipse(
              extent={{-56,-20},{-36,-40}},
              lineColor={0,0,0},
              fillColor={95,95,95},
              fillPattern=FillPattern.Solid),
            Ellipse(
              extent={{24,-20},{44,-40}},
              lineColor={0,0,0},
              fillColor={95,95,95},
              fillPattern=FillPattern.Solid),
            Rectangle(
              extent={{48,-18},{-28,16}},
              lineColor={0,0,0},
              fillPattern=FillPattern.Solid,
              fillColor={175,175,175}),
            Ellipse(
              extent={{-32,-20},{-12,-40}},
              lineColor={0,0,0},
              fillColor={95,95,95},
              fillPattern=FillPattern.Solid),
            Polygon(
              points={{-36,-18},{-60,-18},{-60,0},{-50,16},{-36,16},{-36,-18}},
              lineColor={0,0,0},
              smooth=Smooth.None,
              fillPattern=FillPattern.Solid,
              fillColor={175,175,175}),
            Polygon(
              points={{-52,-2},{-48,12},{-42,12},{-42,-2},{-52,-2}},
              lineColor={0,0,0},
              smooth=Smooth.None,
              fillColor={255,255,255},
              fillPattern=FillPattern.Solid),
            Ellipse(
              extent={{8,12},{28,-8}},
              lineColor={255,85,255},
              fillColor={215,215,215},
              fillPattern=FillPattern.Solid),
            Line(
              points={{18,42},{18,-38}},
              color={255,85,255},
              smooth=Smooth.None),
            Line(
              points={{-22,2},{58,2}},
              color={255,85,255},
              smooth=Smooth.None)}));
    end Hull;

    package Land_Decl
      extends Modelica.Icons.Package;

      model FTP_Driver "Federal Test Procedure"
        extends C2M2L_Decl.Testing_Blocks.Land.Driver_Base_Land(
        profile_data=[0.0,0.0;1.0,0.0;2.0,0.0;3.0,0.0;4.0,0.0;5.0,0.0;6.0,0.0;7.0,0.0;8.0,0.0;9.0,0.0;10.0,0.0;11.0,0.0;12.0,0.0;13.0,0.0;14.0,0.0;15.0,0.0;16.0,0.0;17.0,0.0;18.0,0.0;19.0,0.0;20.0,0.0;21.0,1.34112;22.0,2.637536;23.0,3.844544;24.0,5.14096;25.0,6.392672;26.0,7.554976;27.0,7.733792;28.0,8.091424;29.0,9.253728;30.0,9.700768;31.0,10.013696;32.0,10.0584;33.0,9.879584;34.0,9.61136;35.0,9.343136;36.0,9.119616;37.0,8.851392;38.0,7.59968;39.0,6.660896;40.0,6.660896;41.0,6.795008;42.0,6.92912;43.0,7.15264;44.0,7.644384;45.0,8.538464;46.0,9.432544;47.0,10.147808;48.0,10.237216;49.0,10.147808;50.0,10.103104;51.0,9.521952;52.0,8.49376;53.0,7.644384;54.0,7.063232;55.0,7.063232;56.0,7.912608;57.0,8.851392;58.0,9.656064;59.0,10.371328;60.0,10.818368;61.0,10.997184;62.0,11.131296;63.0,11.176;64.0,10.997184;65.0,10.95248;66.0,11.041888;67.0,11.086592;68.0,11.041888;69.0,10.997184;70.0,10.997184;71.0,11.220704;72.0,11.444224;73.0,11.488928;74.0,11.354816;75.0,11.131296;76.0,11.176;77.0,11.354816;78.0,11.62304;79.0,11.62304;80.0,11.488928;81.0,11.667744;82.0,11.935968;83.0,12.2936;84.0,12.785344;85.0,13.098272;86.0,13.321792;87.0,13.455904;88.0,13.590016;89.0,13.724128;90.0,13.724128;91.0,13.63472;92.0,13.590016;93.0,13.545312;94.0,13.590016;95.0,13.768832;96.0,13.590016;97.0,13.366496;98.0,13.18768;99.0,13.321792;100.0,13.545312;101.0,13.724128;102.0,13.813536;103.0,13.85824;104.0,13.813536;105.0,13.590016;106.0,13.321792;107.0,13.366496;108.0,13.500608;109.0,13.724128;110.0,13.947648;111.0,14.215872;112.0,14.394688;113.0,14.484096;114.0,14.394688;115.0,14.171168;116.0,12.785344;117.0,11.310112;118.0,9.83488;119.0,8.359648;120.0,6.884416;121.0,5.409184;122.0,3.933952;123.0,2.45872;124.0,0.983488;125.0,0.0;126.0,0.0;127.0,0.0;128.0,0.0;129.0,0.0;130.0,0.0;131.0,0.0;132.0,0.0;133.0,0.0;134.0,0.0;135.0,0.0;136.0,0.0;137.0,0.0;138.0,0.0;139.0,0.0;140.0,0.0;141.0,0.0;142.0,0.0;143.0,0.0;144.0,0.0;145.0,0.0;146.0,0.0;147.0,0.0;148.0,0.0;149.0,0.0;150.0,0.0;151.0,0.0;152.0,0.0;153.0,0.0;154.0,0.0;155.0,0.0;156.0,0.0;157.0,0.0;158.0,0.0;159.0,0.0;160.0,0.0;161.0,0.0;162.0,0.0;163.0,0.0;164.0,1.475232;165.0,2.950464;166.0,4.425696;167.0,5.900928;168.0,7.37616;169.0,8.851392;170.0,9.924288;171.0,10.863072;172.0,11.533632;173.0,11.801856;174.0,11.488928;175.0,11.220704;176.0,11.041888;177.0,11.176;178.0,11.265408;179.0,11.354816;180.0,11.533632;181.0,12.159488;182.0,11.84656;183.0,10.72896;184.0,10.147808;185.0,8.672576;186.0,7.912608;187.0,7.689088;188.0,8.091424;189.0,8.314944;190.0,8.9408;191.0,9.924288;192.0,10.95248;193.0,12.204192;194.0,13.63472;195.0,14.97584;196.0,16.182848;197.0,16.674592;198.0,17.568672;199.0,18.10512;200.0,18.820384;201.0,19.44624;202.0,20.161504;203.0,20.56384;204.0,20.921472;205.0,21.2344;206.0,21.2344;207.0,21.144992;208.0,21.100288;209.0,21.01088;210.0,21.01088;211.0,21.01088;212.0,21.01088;213.0,21.01088;214.0,21.100288;215.0,21.189696;216.0,21.413216;217.0,21.68144;218.0,21.949664;219.0,22.12848;220.0,22.352;221.0,22.620224;222.0,22.79904;223.0,23.02256;224.0,23.335488;225.0,23.782528;226.0,24.184864;227.0,24.408384;228.0,24.542496;229.0,24.5872;230.0,24.542496;231.0,24.408384;232.0,24.408384;233.0,24.497792;234.0,24.631904;235.0,24.81072;236.0,24.900128;237.0,25.078944;238.0,25.168352;239.0,25.302464;240.0,25.347168;241.0,25.347168;242.0,25.25776;243.0,25.25776;244.0,25.25776;245.0,25.25776;246.0,25.25776;247.0,25.25776;248.0,25.213056;249.0,25.078944;250.0,24.944832;251.0,24.631904;252.0,24.408384;253.0,24.229568;254.0,24.14016;255.0,24.006048;256.0,23.961344;257.0,24.095456;258.0,24.14016;259.0,24.184864;260.0,24.184864;261.0,24.050752;262.0,23.871936;263.0,23.69312;264.0,23.514304;265.0,23.290784;266.0,23.424896;267.0,23.24608;268.0,23.201376;269.0,23.111968;270.0,23.02256;271.0,23.067264;272.0,23.156672;273.0,23.290784;274.0,23.4696;275.0,23.69312;276.0,23.91664;277.0,24.14016;278.0,24.542496;279.0,24.766016;280.0,24.855424;281.0,25.03424;282.0,25.03424;283.0,24.944832;284.0,24.676608;285.0,24.36368;286.0,23.961344;287.0,23.4696;288.0,23.02256;289.0,23.02256;290.0,23.02256;291.0,22.843744;292.0,22.396704;293.0,22.352;294.0,22.396704;295.0,22.352;296.0,22.173184;297.0,22.12848;298.0,22.12848;299.0,22.12848;300.0,21.949664;301.0,21.726144;302.0,21.502624;303.0,21.100288;304.0,20.608544;305.0,20.1168;306.0,19.580352;307.0,19.043904;308.0,18.55216;309.0,18.015712;310.0,17.21104;311.0,16.54048;312.0,15.735808;313.0,15.109952;314.0,14.5288;315.0,14.08176;316.0,13.679424;317.0,13.63472;318.0,13.4112;319.0,12.96416;320.0,12.2936;321.0,11.086592;322.0,9.61136;323.0,8.985504;324.0,8.538464;325.0,8.27024;326.0,7.59968;327.0,6.92912;328.0,5.588;329.0,4.828032;330.0,3.57632;331.0,2.101088;332.0,0.625856;333.0,0.0;334.0,0.0;335.0,0.0;336.0,0.0;337.0,0.0;338.0,0.0;339.0,0.0;340.0,0.0;341.0,0.0;342.0,0.0;343.0,0.0;344.0,0.0;345.0,0.0;346.0,0.0;347.0,0.44704;348.0,1.922272;349.0,3.397504;350.0,4.872736;351.0,6.347968;352.0,7.733792;353.0,8.9408;354.0,10.0584;355.0,10.594848;356.0,11.265408;357.0,11.891264;358.0,12.561824;359.0,13.4112;360.0,13.768832;361.0,14.126464;362.0,14.349984;363.0,14.662912;364.0,15.020544;365.0,15.42288;366.0,15.467584;367.0,15.601696;368.0,15.556992;369.0,15.42288;370.0,15.512288;371.0,15.86992;372.0,16.09344;373.0,16.09344;374.0,16.09344;375.0,16.09344;376.0,16.09344;377.0,16.09344;378.0,16.138144;379.0,16.272256;380.0,16.31696;381.0,16.272256;382.0,16.09344;383.0,15.691104;384.0,15.244064;385.0,14.97584;386.0,14.037056;387.0,12.96416;388.0,11.488928;389.0,10.28192;390.0,9.074912;391.0,7.8232;392.0,6.48208;393.0,5.36448;394.0,3.889248;395.0,2.414016;396.0,0.938784;397.0,0.0;398.0,0.0;399.0,0.0;400.0,0.0;401.0,0.0;402.0,0.0;403.0,1.162304;404.0,2.637536;405.0,4.112768;406.0,5.588;407.0,7.063232;408.0,8.538464;409.0,10.013696;410.0,11.176;411.0,11.444224;412.0,12.2936;413.0,12.96416;414.0,13.4112;415.0,13.455904;416.0,13.4112;417.0,13.277088;418.0,13.098272;419.0,12.874752;420.0,12.51712;421.0,11.176;422.0,9.700768;423.0,8.225536;424.0,6.750304;425.0,5.275072;426.0,3.79984;427.0,2.324608;428.0,0.849376;429.0,0.0;430.0,0.0;431.0,0.0;432.0,0.0;433.0,0.0;434.0,0.0;435.0,0.0;436.0,0.0;437.0,0.0;438.0,0.0;439.0,0.0;440.0,0.0;441.0,0.0;442.0,0.0;443.0,0.0;444.0,0.0;445.0,0.0;446.0,0.0;447.0,0.0;448.0,1.475232;449.0,2.950464;450.0,4.425696;451.0,5.900928;452.0,7.37616;453.0,8.851392;454.0,10.326624;455.0,11.801856;456.0,12.427712;457.0,13.008864;458.0,14.08176;459.0,14.75232;460.0,15.020544;461.0,15.556992;462.0,15.691104;463.0,15.914624;464.0,16.138144;465.0,16.09344;466.0,16.138144;467.0,16.182848;468.0,16.09344;469.0,15.959328;470.0,16.09344;471.0,16.09344;472.0,15.914624;473.0,15.86992;474.0,15.825216;475.0,15.735808;476.0,15.735808;477.0,15.735808;478.0,15.735808;479.0,15.735808;480.0,15.735808;481.0,15.6464;482.0,15.691104;483.0,15.735808;484.0,15.86992;485.0,15.735808;486.0,15.6464;487.0,15.6464;488.0,15.6464;489.0,15.556992;490.0,15.467584;491.0,15.42288;492.0,14.97584;493.0,14.30528;494.0,13.455904;495.0,12.51712;496.0,11.39952;497.0,10.0584;498.0,8.851392;499.0,7.37616;500.0,5.900928;501.0,4.604512;502.0,3.218688;503.0,1.78816;504.0,0.44704;505.0,0.0;506.0,0.0;507.0,0.0;508.0,0.0;509.0,0.0;510.0,0.0;511.0,0.536448;512.0,1.56464;513.0,2.45872;514.0,2.90576;515.0,3.79984;516.0,4.291584;517.0,4.69392;518.0,5.319776;519.0,6.25856;520.0,7.15264;521.0,7.912608;522.0,8.49376;523.0,8.985504;524.0,9.38784;525.0,9.83488;526.0,10.28192;527.0,10.639552;528.0,10.95248;529.0,11.131296;530.0,11.176;531.0,11.176;532.0,11.176;533.0,11.176;534.0,11.176;535.0,11.176;536.0,11.444224;537.0,11.533632;538.0,11.62304;539.0,11.444224;540.0,11.265408;541.0,11.176;542.0,11.176;543.0,11.176;544.0,10.907776;545.0,10.326624;546.0,8.851392;547.0,7.37616;548.0,5.900928;549.0,4.425696;550.0,2.950464;551.0,1.475232;552.0,0.0;553.0,0.0;554.0,0.0;555.0,0.0;556.0,0.0;557.0,0.0;558.0,0.0;559.0,0.0;560.0,0.0;561.0,0.0;562.0,0.0;563.0,0.0;564.0,0.0;565.0,0.0;566.0,0.0;567.0,0.0;568.0,0.0;569.0,1.475232;570.0,2.950464;571.0,4.425696;572.0,5.81152;573.0,6.526784;574.0,7.15264;575.0,7.59968;576.0,7.59968;577.0,7.59968;578.0,7.8232;579.0,7.912608;580.0,7.912608;581.0,7.8232;582.0,7.59968;583.0,7.554976;584.0,7.420864;585.0,7.59968;586.0,7.644384;587.0,7.59968;588.0,7.420864;589.0,7.37616;590.0,7.37616;591.0,7.420864;592.0,7.59968;593.0,7.867904;594.0,8.27024;595.0,8.583168;596.0,9.030208;597.0,9.38784;598.0,9.432544;599.0,9.477248;600.0,9.656064;601.0,9.83488;602.0,10.013696;603.0,10.0584;604.0,10.0584;605.0,10.0584;606.0,10.147808;607.0,10.594848;608.0,11.220704;609.0,11.62304;610.0,11.84656;611.0,12.07008;612.0,11.667744;613.0,10.192512;614.0,8.71728;615.0,7.242048;616.0,5.766816;617.0,4.291584;618.0,2.816352;619.0,1.34112;620.0,0.0;621.0,0.0;622.0,0.0;623.0,0.0;624.0,0.0;625.0,0.0;626.0,0.0;627.0,0.0;628.0,0.0;629.0,0.0;630.0,0.0;631.0,0.0;632.0,0.0;633.0,0.0;634.0,0.0;635.0,0.0;636.0,0.0;637.0,0.0;638.0,0.0;639.0,0.0;640.0,0.0;641.0,0.0;642.0,0.0;643.0,0.0;644.0,0.0;645.0,0.0;646.0,0.89408;647.0,2.01168;648.0,3.486912;649.0,4.559808;650.0,5.588;651.0,6.25856;652.0,6.839712;653.0,7.8232;654.0,8.761984;655.0,9.38784;656.0,9.924288;657.0,10.416032;658.0,10.95248;659.0,11.310112;660.0,11.444224;661.0,11.62304;662.0,11.667744;663.0,11.712448;664.0,11.712448;665.0,11.801856;666.0,11.84656;667.0,11.84656;668.0,11.62304;669.0,11.39952;670.0,10.550144;671.0,9.566656;672.0,8.27024;673.0,7.331456;674.0,6.48208;675.0,5.185664;676.0,3.889248;677.0,2.592832;678.0,1.56464;679.0,0.89408;680.0,0.0;681.0,0.0;682.0,0.0;683.0,0.0;684.0,0.0;685.0,0.0;686.0,0.0;687.0,0.0;688.0,0.0;689.0,0.0;690.0,0.0;691.0,0.0;692.0,0.0;693.0,0.0;694.0,0.625856;695.0,1.475232;696.0,1.966976;697.0,2.90576;698.0,4.112768;699.0,5.051552;700.0,6.03504;701.0,6.526784;702.0,7.331456;703.0,7.465568;704.0,7.37616;705.0,7.37616;706.0,8.136128;707.0,8.583168;708.0,8.985504;709.0,9.61136;710.0,10.0584;711.0,10.0584;712.0,9.879584;713.0,10.147808;714.0,10.416032;715.0,10.50544;716.0,10.0584;717.0,9.656064;718.0,9.16432;719.0,8.04672;720.0,6.7056;721.0,5.36448;722.0,4.02336;723.0,2.771648;724.0,2.01168;725.0,1.34112;726.0,0.938784;727.0,0.22352;728.0,0.22352;729.0,1.430528;730.0,2.90576;731.0,4.291584;732.0,5.588;733.0,6.25856;734.0,7.15264;735.0,8.04672;736.0,8.761984;737.0,9.61136;738.0,10.326624;739.0,10.95248;740.0,11.39952;741.0,11.84656;742.0,12.114784;743.0,12.338304;744.0,12.472416;745.0,12.651232;746.0,12.785344;747.0,12.785344;748.0,12.651232;749.0,12.606528;750.0,12.51712;751.0,12.2936;752.0,11.980672;753.0,11.39952;754.0,10.50544;755.0,9.61136;756.0,8.49376;757.0,7.37616;758.0,6.660896;759.0,5.588;760.0,4.202176;761.0,2.771648;762.0,1.34112;763.0,0.67056;764.0,0.67056;765.0,0.22352;766.0,0.0;767.0,1.34112;768.0,2.816352;769.0,4.291584;770.0,5.766816;771.0,7.063232;772.0,7.8232;773.0,8.225536;774.0,8.71728;775.0,9.253728;776.0,9.83488;777.0,10.371328;778.0,11.176;779.0,11.84656;780.0,12.2936;781.0,12.51712;782.0,12.651232;783.0,12.919456;784.0,12.919456;785.0,12.919456;786.0,12.874752;787.0,12.74064;788.0,12.651232;789.0,12.651232;790.0,12.651232;791.0,12.606528;792.0,12.338304;793.0,12.2936;794.0,12.2936;795.0,12.2936;796.0,12.2936;797.0,12.2936;798.0,12.2936;799.0,12.338304;800.0,12.51712;801.0,12.74064;802.0,13.4112;803.0,13.85824;804.0,14.30528;805.0,14.75232;806.0,14.75232;807.0,15.020544;808.0,15.19936;809.0,15.333472;810.0,15.288768;811.0,15.19936;812.0,15.19936;813.0,15.154656;814.0,15.020544;815.0,14.797024;816.0,14.75232;817.0,14.5288;818.0,14.30528;819.0,14.260576;820.0,14.126464;821.0,14.08176;822.0,13.679424;823.0,13.4112;824.0,13.366496;825.0,13.366496;826.0,13.366496;827.0,13.366496;828.0,13.232384;829.0,13.18768;830.0,13.18768;831.0,13.098272;832.0,12.919456;833.0,12.606528;834.0,12.383008;835.0,12.07008;836.0,11.39952;837.0,10.594848;838.0,9.83488;839.0,9.16432;840.0,8.583168;841.0,8.583168;842.0,8.985504;843.0,9.343136;844.0,9.566656;845.0,9.83488;846.0,10.103104;847.0,10.371328;848.0,10.72896;849.0,11.176;850.0,11.62304;851.0,11.891264;852.0,11.891264;853.0,11.980672;854.0,12.07008;855.0,12.159488;856.0,12.427712;857.0,12.561824;858.0,12.874752;859.0,12.919456;860.0,12.96416;861.0,13.008864;862.0,12.96416;863.0,12.561824;864.0,12.2936;865.0,12.07008;866.0,11.533632;867.0,11.176;868.0,10.95248;869.0,11.086592;870.0,11.220704;871.0,11.39952;872.0,11.488928;873.0,11.712448;874.0,12.025376;875.0,12.2936;876.0,12.427712;877.0,12.695936;878.0,12.96416;879.0,13.053568;880.0,13.008864;881.0,12.96416;882.0,12.919456;883.0,12.74064;884.0,12.561824;885.0,12.51712;886.0,12.51712;887.0,12.338304;888.0,12.159488;889.0,11.891264;890.0,12.07008;891.0,12.2936;892.0,12.427712;893.0,12.51712;894.0,12.427712;895.0,12.51712;896.0,12.51712;897.0,12.51712;898.0,12.383008;899.0,12.248896;900.0,12.025376;901.0,11.891264;902.0,11.84656;903.0,11.84656;904.0,11.84656;905.0,11.757152;906.0,11.712448;907.0,11.712448;908.0,11.578336;909.0,11.444224;910.0,11.444224;911.0,11.578336;912.0,11.533632;913.0,11.39952;914.0,10.997184;915.0,10.50544;916.0,9.924288;917.0,9.656064;918.0,9.656064;919.0,9.700768;920.0,10.103104;921.0,10.460736;922.0,10.72896;923.0,10.818368;924.0,10.907776;925.0,11.131296;926.0,11.220704;927.0,11.265408;928.0,11.310112;929.0,11.39952;930.0,11.265408;931.0,11.176;932.0,11.176;933.0,11.176;934.0,11.041888;935.0,10.95248;936.0,10.863072;937.0,10.863072;938.0,10.95248;939.0,11.176;940.0,11.176;941.0,10.997184;942.0,10.997184;943.0,10.773664;944.0,10.95248;945.0,11.220704;946.0,11.444224;947.0,11.220704;948.0,10.72896;949.0,9.83488;950.0,8.985504;951.0,7.554976;952.0,6.079744;953.0,4.604512;954.0,3.12928;955.0,1.654048;956.0,0.178816;957.0,0.0;958.0,0.0;959.0,0.0;960.0,0.89408;961.0,2.369312;962.0,3.844544;963.0,5.319776;964.0,6.795008;965.0,7.8232;966.0,8.314944;967.0,8.9408;968.0,9.432544;969.0,9.83488;970.0,10.28192;971.0,10.95248;972.0,11.757152;973.0,12.2936;974.0,12.561824;975.0,12.695936;976.0,12.74064;977.0,12.74064;978.0,12.74064;979.0,12.383008;980.0,12.2936;981.0,12.159488;982.0,11.980672;983.0,11.84656;984.0,11.62304;985.0,11.488928;986.0,11.265408;987.0,10.72896;988.0,9.83488;989.0,9.61136;990.0,9.61136;991.0,9.745472;992.0,10.0584;993.0,10.28192;994.0,10.192512;995.0,10.192512;996.0,10.28192;997.0,10.147808;998.0,10.147808;999.0,10.147808;1000.0,10.50544;1001.0,10.72896;1002.0,10.997184;1003.0,11.086592;1004.0,11.220704;1005.0,11.39952;1006.0,11.444224;1007.0,11.39952;1008.0,11.176;1009.0,10.773664;1010.0,10.594848;1011.0,10.371328;1012.0,10.237216;1013.0,10.0584;1014.0,9.83488;1015.0,9.656064;1016.0,9.16432;1017.0,7.8232;1018.0,6.347968;1019.0,4.872736;1020.0,3.397504;1021.0,1.922272;1022.0,0.44704;1023.0,0.0;1024.0,0.0;1025.0,0.0;1026.0,0.0;1027.0,0.0;1028.0,0.0;1029.0,0.0;1030.0,0.0;1031.0,0.0;1032.0,0.0;1033.0,0.0;1034.0,0.0;1035.0,0.0;1036.0,0.0;1037.0,0.0;1038.0,0.0;1039.0,0.0;1040.0,0.0;1041.0,0.0;1042.0,0.0;1043.0,0.0;1044.0,0.0;1045.0,0.0;1046.0,0.0;1047.0,0.0;1048.0,0.0;1049.0,0.0;1050.0,0.0;1051.0,0.0;1052.0,0.0;1053.0,0.536448;1054.0,1.78816;1055.0,3.263392;1056.0,4.738624;1057.0,6.213856;1058.0,7.59968;1059.0,8.27024;1060.0,8.9408;1061.0,9.745472;1062.0,10.28192;1063.0,10.72896;1064.0,11.086592;1065.0,11.444224;1066.0,11.84656;1067.0,11.980672;1068.0,12.248896;1069.0,12.472416;1070.0,12.651232;1071.0,12.51712;1072.0,12.2936;1073.0,12.07008;1074.0,12.07008;1075.0,11.757152;1076.0,10.95248;1077.0,10.0584;1078.0,9.61136;1079.0,9.209024;1080.0,8.04672;1081.0,6.7056;1082.0,5.498592;1083.0,4.962144;1084.0,4.738624;1085.0,4.4704;1086.0,4.24688;1087.0,4.068064;1088.0,3.889248;1089.0,3.844544;1090.0,3.933952;1091.0,4.02336;1092.0,3.889248;1093.0,3.844544;1094.0,3.57632;1095.0,3.12928;1096.0,2.2352;1097.0,1.877568;1098.0,1.162304;1099.0,0.44704;1100.0,0.0;1101.0,0.044704;1102.0,0.268224;1103.0,0.715264;1104.0,1.609344;1105.0,3.084576;1106.0,4.4704;1107.0,5.722112;1108.0,6.25856;1109.0,6.48208;1110.0,7.15264;1111.0,8.091424;1112.0,8.9408;1113.0,9.38784;1114.0,9.477248;1115.0,9.521952;1116.0,9.566656;1117.0,9.700768;1118.0,10.0584;1119.0,10.28192;1120.0,10.639552;1121.0,10.95248;1122.0,11.176;1123.0,11.131296;1124.0,11.086592;1125.0,11.176;1126.0,11.354816;1127.0,11.533632;1128.0,11.62304;1129.0,11.801856;1130.0,11.891264;1131.0,12.025376;1132.0,12.07008;1133.0,12.07008;1134.0,12.07008;1135.0,12.025376;1136.0,11.980672;1137.0,11.980672;1138.0,11.84656;1139.0,11.801856;1140.0,11.62304;1141.0,11.39952;1142.0,10.997184;1143.0,10.50544;1144.0,9.61136;1145.0,8.9408;1146.0,7.8232;1147.0,7.15264;1148.0,6.25856;1149.0,4.783328;1150.0,3.308096;1151.0,1.832864;1152.0,0.357632;1153.0,0.0;1154.0,0.0;1155.0,0.0;1156.0,0.0;1157.0,0.0;1158.0,0.0;1159.0,0.0;1160.0,0.0;1161.0,0.0;1162.0,0.0;1163.0,0.0;1164.0,0.0;1165.0,0.0;1166.0,0.0;1167.0,0.0;1168.0,0.0;1169.0,0.938784;1170.0,2.414016;1171.0,3.889248;1172.0,5.36448;1173.0,6.839712;1174.0,8.314944;1175.0,9.432544;1176.0,10.28192;1177.0,10.50544;1178.0,10.28192;1179.0,10.0584;1180.0,8.9408;1181.0,7.465568;1182.0,5.990336;1183.0,4.515104;1184.0,3.039872;1185.0,1.56464;1186.0,0.089408;1187.0,0.0;1188.0,0.0;1189.0,0.0;1190.0,0.0;1191.0,0.0;1192.0,0.0;1193.0,0.0;1194.0,0.0;1195.0,0.0;1196.0,0.0;1197.0,0.089408;1198.0,0.67056;1199.0,1.56464;1200.0,2.90576;1201.0,4.380992;1202.0,5.36448;1203.0,5.766816;1204.0,5.81152;1205.0,5.632704;1206.0,5.722112;1207.0,5.856224;1208.0,5.856224;1209.0,6.25856;1210.0,6.92912;1211.0,7.59968;1212.0,8.314944;1213.0,8.806688;1214.0,9.38784;1215.0,9.61136;1216.0,9.745472;1217.0,9.745472;1218.0,9.61136;1219.0,9.477248;1220.0,9.61136;1221.0,9.745472;1222.0,9.83488;1223.0,9.790176;1224.0,9.700768;1225.0,9.61136;1226.0,9.61136;1227.0,9.566656;1228.0,8.985504;1229.0,8.71728;1230.0,8.583168;1231.0,8.761984;1232.0,8.851392;1233.0,8.9408;1234.0,8.71728;1235.0,7.8232;1236.0,6.92912;1237.0,5.81152;1238.0,4.4704;1239.0,3.57632;1240.0,2.68224;1241.0,1.78816;1242.0,1.1176;1243.0,0.312928;1244.0,0.0;1245.0,0.0;1246.0,0.0;1247.0,0.0;1248.0,0.0;1249.0,0.0;1250.0,0.0;1251.0,0.0;1252.0,0.44704;1253.0,0.44704;1254.0,0.44704;1255.0,0.44704;1256.0,0.44704;1257.0,0.715264;1258.0,1.34112;1259.0,1.78816;1260.0,2.2352;1261.0,2.816352;1262.0,3.57632;1263.0,4.4704;1264.0,4.69392;1265.0,4.24688;1266.0,3.79984;1267.0,3.397504;1268.0,3.933952;1269.0,4.91744;1270.0,6.25856;1271.0,7.59968;1272.0,8.71728;1273.0,9.38784;1274.0,9.745472;1275.0,9.924288;1276.0,10.28192;1277.0,10.550144;1278.0,10.773664;1279.0,10.95248;1280.0,10.95248;1281.0,10.72896;1282.0,10.50544;1283.0,10.50544;1284.0,10.50544;1285.0,10.50544;1286.0,10.50544;1287.0,10.50544;1288.0,10.72896;1289.0,10.773664;1290.0,10.95248;1291.0,11.041888;1292.0,11.176;1293.0,11.354816;1294.0,11.444224;1295.0,11.488928;1296.0,11.62304;1297.0,11.712448;1298.0,12.07008;1299.0,12.427712;1300.0,12.651232;1301.0,12.96416;1302.0,13.008864;1303.0,12.96416;1304.0,12.51712;1305.0,11.041888;1306.0,9.566656;1307.0,8.091424;1308.0,6.616192;1309.0,5.14096;1310.0,3.665728;1311.0,2.190496;1312.0,0.715264;1313.0,0.0;1314.0,0.0;1315.0,0.0;1316.0,0.0;1317.0,0.0;1318.0,0.0;1319.0,0.0;1320.0,0.0;1321.0,0.0;1322.0,0.0;1323.0,0.0;1324.0,0.0;1325.0,0.0;1326.0,0.0;1327.0,0.0;1328.0,0.0;1329.0,0.0;1330.0,0.0;1331.0,0.0;1332.0,0.0;1333.0,0.0;1334.0,0.0;1335.0,0.0;1336.0,0.0;1337.0,0.0;1338.0,0.67056;1339.0,2.145792;1340.0,3.621024;1341.0,5.096256;1342.0,5.900928;1343.0,6.750304;1344.0,7.510272;1345.0,8.180832;1346.0,8.71728;1347.0,9.074912;1348.0,9.521952;1349.0,9.790176;1350.0,9.879584;1351.0,10.013696;1352.0,9.83488;1353.0,9.656064;1354.0,9.432544;1355.0,9.16432;1356.0,8.9408;1357.0,8.761984;1358.0,8.27024;1359.0,7.8232;1360.0,7.37616;1361.0,6.92912;1362.0,6.25856;1363.0,4.91744;1364.0,3.57632;1365.0,2.324608;1366.0,1.1176;1367.0,0.0;1368.0,0.0;1369.0,0.0;1370.0,0.0;1371.0,0.0;1372.0,0.0;1373.0,0.0;1374.0,0.0;1375.0,0.0;1376.0,0.0;1377.0,0.0;1378.0,0.0;1379.0,0.0;1380.0,0.0;1381.0,0.0;1382.0,0.0;1383.0,0.0;1384.0,0.0;1385.0,0.0;1386.0,0.0;1387.0,0.0;1388.0,0.0;1389.0,0.0;1390.0,1.34112;1391.0,2.637536;1392.0,3.844544;1393.0,5.14096;1394.0,6.392672;1395.0,7.554976;1396.0,7.733792;1397.0,8.091424;1398.0,9.253728;1399.0,9.700768;1400.0,10.013696;1401.0,10.0584;1402.0,9.879584;1403.0,9.61136;1404.0,9.343136;1405.0,9.119616;1406.0,8.851392;1407.0,7.59968;1408.0,6.660896;1409.0,6.660896;1410.0,6.795008;1411.0,6.92912;1412.0,7.15264;1413.0,7.644384;1414.0,8.538464;1415.0,9.432544;1416.0,10.147808;1417.0,10.237216;1418.0,10.147808;1419.0,10.103104;1420.0,9.521952;1421.0,8.49376;1422.0,7.644384;1423.0,7.063232;1424.0,7.063232;1425.0,7.912608;1426.0,8.851392;1427.0,9.656064;1428.0,10.371328;1429.0,10.818368;1430.0,10.997184;1431.0,11.131296;1432.0,11.176;1433.0,10.997184;1434.0,10.95248;1435.0,11.041888;1436.0,11.086592;1437.0,11.041888;1438.0,10.997184;1439.0,10.997184;1440.0,11.220704;1441.0,11.444224;1442.0,11.488928;1443.0,11.354816;1444.0,11.131296;1445.0,11.176;1446.0,11.354816;1447.0,11.62304;1448.0,11.62304;1449.0,11.488928;1450.0,11.667744;1451.0,11.935968;1452.0,12.2936;1453.0,12.785344;1454.0,13.098272;1455.0,13.321792;1456.0,13.455904;1457.0,13.590016;1458.0,13.724128;1459.0,13.724128;1460.0,13.63472;1461.0,13.590016;1462.0,13.545312;1463.0,13.590016;1464.0,13.768832;1465.0,13.590016;1466.0,13.366496;1467.0,13.18768;1468.0,13.321792;1469.0,13.545312;1470.0,13.724128;1471.0,13.813536;1472.0,13.85824;1473.0,13.813536;1474.0,13.590016;1475.0,13.321792;1476.0,13.366496;1477.0,13.500608;1478.0,13.724128;1479.0,13.947648;1480.0,14.215872;1481.0,14.394688;1482.0,14.484096;1483.0,14.394688;1484.0,14.171168;1485.0,12.785344;1486.0,11.310112;1487.0,9.83488;1488.0,8.359648;1489.0,6.884416;1490.0,5.409184;1491.0,3.933952;1492.0,2.45872;1493.0,0.983488;1494.0,0.0;1495.0,0.0;1496.0,0.0;1497.0,0.0;1498.0,0.0;1499.0,0.0;1500.0,0.0;1501.0,0.0;1502.0,0.0;1503.0,0.0;1504.0,0.0;1505.0,0.0;1506.0,0.0;1507.0,0.0;1508.0,0.0;1509.0,0.0;1510.0,0.0;1511.0,0.0;1512.0,0.0;1513.0,0.0;1514.0,0.0;1515.0,0.0;1516.0,0.0;1517.0,0.0;1518.0,0.0;1519.0,0.0;1520.0,0.0;1521.0,0.0;1522.0,0.0;1523.0,0.0;1524.0,0.0;1525.0,0.0;1526.0,0.0;1527.0,0.0;1528.0,0.0;1529.0,0.0;1530.0,0.0;1531.0,0.0;1532.0,0.0;1533.0,1.475232;1534.0,2.950464;1535.0,4.425696;1536.0,5.900928;1537.0,7.37616;1538.0,8.851392;1539.0,9.924288;1540.0,10.863072;1541.0,11.533632;1542.0,11.801856;1543.0,11.488928;1544.0,11.220704;1545.0,11.041888;1546.0,11.176;1547.0,11.265408;1548.0,11.354816;1549.0,11.533632;1550.0,12.159488;1551.0,11.84656;1552.0,10.72896;1553.0,10.147808;1554.0,8.672576;1555.0,7.912608;1556.0,7.689088;1557.0,8.091424;1558.0,8.314944;1559.0,8.9408;1560.0,9.924288;1561.0,10.95248;1562.0,12.204192;1563.0,13.63472;1564.0,14.97584;1565.0,16.182848;1566.0,16.674592;1567.0,17.568672;1568.0,18.10512;1569.0,18.820384;1570.0,19.44624;1571.0,20.161504;1572.0,20.56384;1573.0,20.921472;1574.0,21.2344;1575.0,21.2344;1576.0,21.144992;1577.0,21.100288;1578.0,21.01088;1579.0,21.01088;1580.0,21.01088;1581.0,21.01088;1582.0,21.01088;1583.0,21.100288;1584.0,21.189696;1585.0,21.413216;1586.0,21.68144;1587.0,21.949664;1588.0,22.12848;1589.0,22.352;1590.0,22.620224;1591.0,22.79904;1592.0,23.02256;1593.0,23.335488;1594.0,23.782528;1595.0,24.184864;1596.0,24.408384;1597.0,24.542496;1598.0,24.5872;1599.0,24.542496;1600.0,24.408384;1601.0,24.408384;1602.0,24.497792;1603.0,24.631904;1604.0,24.81072;1605.0,24.900128;1606.0,25.078944;1607.0,25.168352;1608.0,25.302464;1609.0,25.347168;1610.0,25.347168;1611.0,25.25776;1612.0,25.25776;1613.0,25.25776;1614.0,25.25776;1615.0,25.25776;1616.0,25.25776;1617.0,25.213056;1618.0,25.078944;1619.0,24.944832;1620.0,24.631904;1621.0,24.408384;1622.0,24.229568;1623.0,24.14016;1624.0,24.006048;1625.0,23.961344;1626.0,24.095456;1627.0,24.14016;1628.0,24.184864;1629.0,24.184864;1630.0,24.050752;1631.0,23.871936;1632.0,23.69312;1633.0,23.514304;1634.0,23.290784;1635.0,23.424896;1636.0,23.24608;1637.0,23.201376;1638.0,23.111968;1639.0,23.02256;1640.0,23.067264;1641.0,23.156672;1642.0,23.290784;1643.0,23.4696;1644.0,23.69312;1645.0,23.91664;1646.0,24.14016;1647.0,24.542496;1648.0,24.766016;1649.0,24.855424;1650.0,25.03424;1651.0,25.03424;1652.0,24.944832;1653.0,24.676608;1654.0,24.36368;1655.0,23.961344;1656.0,23.4696;1657.0,23.02256;1658.0,23.02256;1659.0,23.02256;1660.0,22.843744;1661.0,22.396704;1662.0,22.352;1663.0,22.396704;1664.0,22.352;1665.0,22.173184;1666.0,22.12848;1667.0,22.12848;1668.0,22.12848;1669.0,21.949664;1670.0,21.726144;1671.0,21.502624;1672.0,21.100288;1673.0,20.608544;1674.0,20.1168;1675.0,19.580352;1676.0,19.043904;1677.0,18.55216;1678.0,18.015712;1679.0,17.21104;1680.0,16.54048;1681.0,15.735808;1682.0,15.109952;1683.0,14.5288;1684.0,14.08176;1685.0,13.679424;1686.0,13.63472;1687.0,13.4112;1688.0,12.96416;1689.0,12.2936;1690.0,11.086592;1691.0,9.61136;1692.0,8.985504;1693.0,8.538464;1694.0,8.27024;1695.0,7.59968;1696.0,6.92912;1697.0,5.588;1698.0,4.828032;1699.0,3.57632;1700.0,2.101088;1701.0,0.625856;1702.0,0.0;1703.0,0.0;1704.0,0.0;1705.0,0.0;1706.0,0.0;1707.0,0.0;1708.0,0.0;1709.0,0.0;1710.0,0.0;1711.0,0.0;1712.0,0.0;1713.0,0.0;1714.0,0.0;1715.0,0.0;1716.0,0.44704;1717.0,1.922272;1718.0,3.397504;1719.0,4.872736;1720.0,6.347968;1721.0,7.733792;1722.0,8.9408;1723.0,10.0584;1724.0,10.594848;1725.0,11.265408;1726.0,11.891264;1727.0,12.561824;1728.0,13.4112;1729.0,13.768832;1730.0,14.126464;1731.0,14.349984;1732.0,14.662912;1733.0,15.020544;1734.0,15.42288;1735.0,15.467584;1736.0,15.601696;1737.0,15.556992;1738.0,15.42288;1739.0,15.512288;1740.0,15.86992;1741.0,16.09344;1742.0,16.09344;1743.0,16.09344;1744.0,16.09344;1745.0,16.09344;1746.0,16.09344;1747.0,16.138144;1748.0,16.272256;1749.0,16.31696;1750.0,16.272256;1751.0,16.09344;1752.0,15.691104;1753.0,15.244064;1754.0,14.97584;1755.0,14.037056;1756.0,12.96416;1757.0,11.488928;1758.0,10.28192;1759.0,9.074912;1760.0,7.8232;1761.0,6.48208;1762.0,5.36448;1763.0,3.889248;1764.0,2.414016;1765.0,0.938784;1766.0,0.0;1767.0,0.0;1768.0,0.0;1769.0,0.0;1770.0,0.0;1771.0,0.0;1772.0,1.162304;1773.0,2.637536;1774.0,4.112768;1775.0,5.588;1776.0,7.063232;1777.0,8.538464;1778.0,10.013696;1779.0,11.176;1780.0,11.444224;1781.0,12.2936;1782.0,12.96416;1783.0,13.4112;1784.0,13.455904;1785.0,13.4112;1786.0,13.277088;1787.0,13.098272;1788.0,12.874752;1789.0,12.51712;1790.0,11.176;1791.0,9.700768;1792.0,8.225536;1793.0,6.750304;1794.0,5.275072;1795.0,3.79984;1796.0,2.324608;1797.0,0.849376;1798.0,0.0;1799.0,0.0;1800.0,0.0;1801.0,0.0;1802.0,0.0;1803.0,0.0;1804.0,0.0;1805.0,0.0;1806.0,0.0;1807.0,0.0;1808.0,0.0;1809.0,0.0;1810.0,0.0;1811.0,0.0;1812.0,0.0;1813.0,0.0;1814.0,0.0;1815.0,0.0;1816.0,0.0;1817.0,1.475232;1818.0,2.950464;1819.0,4.425696;1820.0,5.900928;1821.0,7.37616;1822.0,8.851392;1823.0,10.326624;1824.0,11.801856;1825.0,12.427712;1826.0,13.008864;1827.0,14.08176;1828.0,14.75232;1829.0,15.020544;1830.0,15.556992;1831.0,15.691104;1832.0,15.914624;1833.0,16.138144;1834.0,16.09344;1835.0,16.138144;1836.0,16.182848;1837.0,16.09344;1838.0,15.959328;1839.0,16.09344;1840.0,16.09344;1841.0,15.914624;1842.0,15.86992;1843.0,15.825216;1844.0,15.735808;1845.0,15.735808;1846.0,15.735808;1847.0,15.735808;1848.0,15.735808;1849.0,15.735808;1850.0,15.6464;1851.0,15.691104;1852.0,15.735808;1853.0,15.86992;1854.0,15.735808;1855.0,15.6464;1856.0,15.6464;1857.0,15.6464;1858.0,15.556992;1859.0,15.467584;1860.0,15.42288;1861.0,14.97584;1862.0,14.30528;1863.0,13.455904;1864.0,12.51712;1865.0,11.39952;1866.0,10.0584;1867.0,8.851392;1868.0,7.37616;1869.0,5.900928;1870.0,4.604512;1871.0,3.218688;1872.0,1.78816;1873.0,0.44704;1874.0,0.0;1875.0,0.0;1900.0,0.0]);
        annotation (Icon(graphics={Text(
                extent={{-120,130},{100,110}},
                lineColor={0,128,0},
                textString="FTP"), Line(
                points={{-130,140},{-126,168},{-118,178},{-114,140},{-104,140},{-102,
                    180},{-92,170},{-84,192},{-74,198},{-68,140},{-60,140},{-56,184},
                    {-52,140},{-44,182},{-40,140},{-32,192},{-24,184},{-20,140},{-12,
                    170},{-4,176},{0,164},{8,172},{16,170},{20,140},{28,140},{32,172},
                    {36,140},{44,186},{48,140},{56,166},{64,176},{66,140},{76,140},{
                    80,180},{88,168},{96,190},{104,196},{110,140}},
                color={255,0,0},
                smooth=Smooth.None)}));
      end FTP_Driver;

      model Road_Wheel_Load_Both_Sides
      "Simulates both road wheels connected to a mass simulating a vehicle"
      extends C2M2L_Decl.Interfaces.Test_Bed_C2M2L;

        Modelica.Mechanics.Rotational.Components.IdealRollingWheel idealRollingWheel(
                              radius=radius, useSupportT=false)
          annotation (Placement(transformation(extent={{-32,-10},{-12,10}})));
        SI.Conversions.NonSIunits.Velocity_kmh vehicleSpeed
        "promote velocity to the top level";
        output SI.Time Accel_20kph(start=-1, fixed=true)
        "Acceleration time to 20 kph";
        output SI.Time Accel_40kph(start=-1, fixed=true)
        "Acceleration time to 40 kph";
          parameter Boolean animation=world.enableAnimation
        "= true, show resultant force arrow";
        parameter Real Crr = 0.01 "Coefficient of rolling resistance";
        parameter SI.Area frontal_area = 16 "Frontal area of vehicle";
        parameter SI.Density air_density = 1.16 "Nominal air density";
        parameter Real Cd = 1.25
        "Vehicle drag factor (Cd) in F = 0.5 * Cd * rho * v^2 * A";
        parameter SI.Distance radius=0.4
        "Wheel radius used to convert rotational to translational motion";
        parameter SI.Mass vehicle_mass=20000 "Mass of the vehicle";
        parameter SI.Force static_resistance=vehicle_mass * 9.81 * Crr
        "Constant force required to move vehicle";
        parameter SI.TranslationalDampingConstant linear_resistance=0
        "Linear resistance coefficient (d) in f = d * vel";
        parameter Real quad_resistance=0.5 * Cd * air_density * frontal_area
        "Quadratic resistance coeff (dq) in f = dq * v * |v|";
        parameter Real grade=0 "% grade (100% = 45degree angle)";
        Modelica.Mechanics.Translational.Components.Mass mass(m=vehicle_mass, L=0,
          v(fixed=true),
          stateSelect=StateSelect.prefer)
          annotation (Placement(transformation(extent={{62,-10},{82,10}})));
        Modelica.Mechanics.Translational.Components.SupportFriction supportFriction(
          useSupport=true,
          f_pos=[0,static_resistance],
          peak=1,
          locked(fixed=true, start=true))
                     annotation (Placement(transformation(extent={{28,-10},{48,10}})));
        C2M2L_Decl.Mechanics.Translational.Components.Quadratic_Damper quadratic_damper(d=
              linear_resistance, dq=quad_resistance)
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=270,
              origin={54,-20})));

        Modelica.Mechanics.MultiBody.Interfaces.Frame_a mount if world.driveTrainMechanics3D annotation (Placement(
              transformation(
              extent={{-16,-16},{16,16}},
              rotation=90,
              origin={0,-100})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing rhs_flange(
            includeBearingConnector= world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{-112,30},{-92,50}})));
        Modelica.Mechanics.Translational.Components.Fixed fixed
          annotation (Placement(transformation(extent={{-32,-38},{-12,-18}})));
        Modelica.Mechanics.Translational.Sensors.ForceSensor forceSensor
          annotation (Placement(transformation(extent={{0,-10},{20,10}})));
        Modelica.Mechanics.MultiBody.Forces.WorldForce force(color={0,128,255},
            resolveInFrame=Modelica.Mechanics.MultiBody.Types.ResolveInFrameB.frame_b,
          animation=animation) if world.driveTrainMechanics3D
          annotation (Placement(transformation(
              extent={{-10,-10},{10,10}},
              rotation=180,
              origin={-76,48})));
        Modelica.Blocks.Sources.Constant const(k=0) annotation (Placement(
              transformation(
              extent={{-10,-10},{10,10}},
              rotation=180,
              origin={-30,90})));
        Modelica.Blocks.Sources.Constant const1(k=0) annotation (Placement(
              transformation(
              extent={{-10,-10},{10,10}},
              rotation=180,
              origin={-30,64})));

        outer Modelica.Mechanics.MultiBody.World world
          annotation (Placement(transformation(extent={{-100,80},{-80,100}})));
        Modelica.Mechanics.Translational.Sources.Force force1
          annotation (Placement(transformation(extent={{62,18},{82,38}})));
        Modelica.Blocks.Sources.RealExpression realExpression(y=-vehicle_mass*9.81*
              (grade / 100) * sin(pi /4)) annotation (Placement(transformation(extent={{10,20},{30,40}})));
        C2M2L_Decl.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_Bus
          annotation (Placement(transformation(extent={{-10,90},{10,110}})));

        Modelica.Mechanics.Rotational.Components.IdealGear idealGear(ratio=-1)
          annotation (Placement(transformation(extent={{-92,-30},{-72,-10}})));
        Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing lhs_flange(
            includeBearingConnector=world.driveTrainMechanics3D)
          annotation (Placement(transformation(extent={{-110,-50},{-90,-30}})));
        Modelica.Mechanics.MultiBody.Parts.FixedRotation lhs_location(
          rotationType=Modelica.Mechanics.MultiBody.Types.RotationTypes.RotationAxis,
          n={1,0,0},
          width=0.03,
          r={0.28,-1.3,0.41},
          angle=-90) if world.driveTrainMechanics3D
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=180,
              origin={-50,-100})));

        Modelica.Mechanics.MultiBody.Parts.FixedRotation rhs_location(
          rotationType=Modelica.Mechanics.MultiBody.Types.RotationTypes.RotationAxis,
          n={1,0,0},
          width=0.03,
          r={0.28,1.3,0.41},
          angle=90) if world.driveTrainMechanics3D
          annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=180,
              origin={-52,-80})));

        Modelica.Mechanics.Rotational.Components.Inertia rotational_inertia(J=1)
          annotation (Placement(transformation(extent={{-86,0},{-66,20}})));
        Modelica.Mechanics.Translational.Sensors.SpeedSensor speedSensor
          annotation (Placement(transformation(extent={{4,52},{24,72}})));
      equation
        vehicleSpeed = SI.Conversions.to_kmh(mass.v);

        when vehicleSpeed > 20 then
           Accel_20kph = time;
        end when;
        when vehicleSpeed > 40 then
           Accel_40kph = time;
        end when;

        connect(force.frame_b, rhs_flange.bearingFrame)
                                                    annotation (Line(
            points={{-86,48},{-94,48},{-94,40},{-102,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(idealRollingWheel.flangeT, forceSensor.flange_a) annotation (Line(
            points={{-12,0},{0,0}},
            color={0,127,0},
            smooth=Smooth.None));
        connect(forceSensor.f, force.force[2]) annotation (Line(
            points={{2,-11},{2,-20},{-4,-20},{-4,48},{-64,48}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(const.y, force.force[1]) annotation (Line(
            points={{-41,90},{-52,90},{-52,49.3333},{-64,49.3333}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(const1.y, force.force[3]) annotation (Line(
            points={{-41,64},{-52,64},{-52,46.6667},{-64,46.6667}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(forceSensor.flange_b, supportFriction.flange_a) annotation (Line(
            points={{20,0},{28,0}},
            color={0,127,0},
            smooth=Smooth.None));
        connect(supportFriction.flange_b, quadratic_damper.flange_a) annotation (Line(
            points={{48,0},{54,0},{54,-10}},
            color={0,127,0},
            smooth=Smooth.None));
        connect(supportFriction.flange_b, mass.flange_a) annotation (Line(
            points={{48,0},{62,0}},
            color={0,127,0},
            smooth=Smooth.None));
        connect(supportFriction.support, fixed.flange) annotation (Line(
            points={{38,-10},{8,-10},{8,-28},{-22,-28}},
            color={0,0,0},
            pattern=LinePattern.None,
            smooth=Smooth.None));
        connect(fixed.flange, quadratic_damper.flange_b) annotation (Line(
            points={{-22,-28},{22,-28},{22,-30},{54,-30}},
            color={0,127,0},
            smooth=Smooth.None));

        connect(force1.flange, mass.flange_b) annotation (Line(
            points={{82,28},{92,28},{92,0},{82,0}},
            color={0,127,0},
            smooth=Smooth.None));
        connect(realExpression.y, force1.f) annotation (Line(
            points={{31,30},{46,30},{46,28},{60,28}},
            color={0,0,127},
            smooth=Smooth.None));
        connect(lhs_flange.flange, idealGear.flange_a) annotation (Line(
            points={{-100,-40},{-94,-40},{-94,-20},{-92,-20}},
            color={135,135,135},
            thickness=0.5,
            smooth=Smooth.None));
        connect(mount, lhs_location.frame_a) annotation (Line(
            points={{0,-100},{-40,-100}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(lhs_location.frame_b, lhs_flange.bearingFrame) annotation (Line(
            points={{-60,-100},{-100,-100},{-100,-40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(mount, rhs_location.frame_a) annotation (Line(
            points={{0,-100},{-22,-100},{-22,-80},{-42,-80}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(rhs_location.frame_b, rhs_flange.bearingFrame) annotation (Line(
            points={{-62,-80},{-116,-80},{-116,8},{-102,8},{-102,40}},
            color={95,95,95},
            thickness=0.5,
            smooth=Smooth.None));
        connect(rotational_inertia.flange_a, rhs_flange.flange) annotation (Line(
            points={{-86,10},{-94,10},{-94,40},{-102,40}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(idealGear.flange_b, rotational_inertia.flange_a) annotation (Line(
            points={{-72,-20},{-80,-20},{-80,10},{-86,10}},
            color={0,0,0},
            smooth=Smooth.None));
        connect(idealRollingWheel.flangeT, speedSensor.flange) annotation (Line(
            points={{-12,0},{-8,0},{-8,62},{4,62}},
            color={0,127,0},
            smooth=Smooth.None));
        connect(speedSensor.v, driver_Bus.vehicle_speed) annotation (Line(
            points={{25,62},{40,62},{40,80},{0,80},{0,100}},
            color={0,0,127},
            smooth=Smooth.None), Text(
            string="%second",
            index=1,
            extent={{6,3},{6,3}}));
        connect(rotational_inertia.flange_b, idealRollingWheel.flangeR) annotation (
            Line(
            points={{-66,10},{-50,10},{-50,0},{-32,0}},
            color={0,0,0},
            smooth=Smooth.None));
        annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
                  -100},{100,100}}),
                            graphics), Icon(graphics={
              Ellipse(
                extent={{-40,-40},{-20,-60}},
                lineColor={0,0,255},
                fillColor={95,95,95},
                fillPattern=FillPattern.Solid),
              Ellipse(
                extent={{40,-40},{60,-60}},
                lineColor={0,0,255},
                fillColor={95,95,95},
                fillPattern=FillPattern.Solid),
              Rectangle(
                extent={{64,-38},{-12,-4}},
                lineColor={0,0,255},
                fillPattern=FillPattern.Solid,
                fillColor={175,175,175}),
              Ellipse(
                extent={{-16,-40},{4,-60}},
                lineColor={0,0,255},
                fillColor={95,95,95},
                fillPattern=FillPattern.Solid),
              Polygon(
                points={{-20,-38},{-44,-38},{-44,-20},{-34,-4},{-20,-4},{-20,-38}},
                lineColor={0,0,255},
                smooth=Smooth.None,
                fillPattern=FillPattern.Solid,
                fillColor={175,175,175}),
              Polygon(
                points={{-36,-22},{-32,-8},{-26,-8},{-26,-22},{-36,-22}},
                lineColor={0,0,255},
                smooth=Smooth.None,
                fillColor={255,255,255},
                fillPattern=FillPattern.Solid),
              Text(
                extent={{-64,88},{68,8}},
                lineColor={0,0,0},
                fillColor={175,175,175},
                fillPattern=FillPattern.Solid,
                textString="Road
Load"),       Rectangle(extent={{-100,100},{100,-100}}, lineColor={0,0,0})}));
      end Road_Wheel_Load_Both_Sides;

      model SC03_Driver
      "SC03 Driving Cycle, per 8/96 Final Rule - a Supplemental FTP Driving Schedule"
        extends C2M2L_Decl.Testing_Blocks.Land.Driver_Base_Land(
        profile_data=[0.0,0.0;1.0,0.0;2.0,0.0;3.0,0.0;4.0,0.0;5.0,0.0;6.0,0.0;7.0,0.0;8.0,0.0;9.0,0.0;10.0,0.0;11.0,0.0;12.0,0.0;13.0,0.0;14.0,0.0;15.0,0.0;16.0,0.0;17.0,0.0;18.0,0.0;19.0,0.402336;20.0,1.34112;21.0,1.296416;22.0,1.475232;23.0,1.56464;24.0,0.983488;25.0,0.625856;26.0,0.0;27.0,0.0;28.0,0.0;29.0,0.0;30.0,0.0;31.0,0.0;32.0,0.0;33.0,0.178816;34.0,1.475232;35.0,2.68224;36.0,3.57632;37.0,3.889248;38.0,4.4704;39.0,5.543296;40.0,6.169152;41.0,6.571488;42.0,6.616192;43.0,7.420864;44.0,8.180832;45.0,8.49376;46.0,8.583168;47.0,8.627872;48.0,8.806688;49.0,9.16432;50.0,9.38784;51.0,9.477248;52.0,9.656064;53.0,9.924288;54.0,10.639552;55.0,10.997184;56.0,10.863072;57.0,10.416032;58.0,10.147808;59.0,9.566656;60.0,9.119616;61.0,8.71728;62.0,8.002016;63.0,6.973824;64.0,5.230368;65.0,3.486912;66.0,3.218688;67.0,4.157472;68.0,5.766816;69.0,7.063232;70.0,7.242048;71.0,7.554976;72.0,8.180832;73.0,9.074912;74.0,9.656064;75.0,10.013696;76.0,10.28192;77.0,10.192512;78.0,9.879584;79.0,9.477248;80.0,8.71728;81.0,7.644384;82.0,6.303264;83.0,4.69392;84.0,3.397504;85.0,3.3528;86.0,4.4704;87.0,5.856224;88.0,6.303264;89.0,7.331456;90.0,8.761984;91.0,10.013696;92.0,11.041888;93.0,11.667744;94.0,11.533632;95.0,11.891264;96.0,12.427712;97.0,12.74064;98.0,12.919456;99.0,13.098272;100.0,13.18768;101.0,13.142976;102.0,13.142976;103.0,13.321792;104.0,13.545312;105.0,13.679424;106.0,13.63472;107.0,13.63472;108.0,13.455904;109.0,13.098272;110.0,12.695936;111.0,12.338304;112.0,11.980672;113.0,11.39952;114.0,10.594848;115.0,9.700768;116.0,8.627872;117.0,7.465568;118.0,6.437376;119.0,5.14096;120.0,3.531616;121.0,2.950464;122.0,4.202176;123.0,5.543296;124.0,6.616192;125.0,7.197344;126.0,8.627872;127.0,10.103104;128.0,11.39952;129.0,11.801856;130.0,11.935968;131.0,12.427712;132.0,13.142976;133.0,13.902944;134.0,14.5288;135.0,15.020544;136.0,15.467584;137.0,15.825216;138.0,16.138144;139.0,16.54048;140.0,16.853408;141.0,17.032224;142.0,17.121632;143.0,17.032224;144.0,16.898112;145.0,16.361664;146.0,15.556992;147.0,14.841728;148.0,14.484096;149.0,14.439392;150.0,14.439392;151.0,14.484096;152.0,14.484096;153.0,14.484096;154.0,14.5288;155.0,14.886432;156.0,15.378176;157.0,15.86992;158.0,16.361664;159.0,16.719296;160.0,16.98752;161.0,17.166336;162.0,17.21104;163.0,17.255744;164.0,17.166336;165.0,17.076928;166.0,16.764;167.0,16.495776;168.0,16.227552;169.0,15.556992;170.0,14.75232;171.0,14.037056;172.0,13.724128;173.0,13.545312;174.0,13.4112;175.0,13.098272;176.0,12.248896;177.0,11.220704;178.0,9.745472;179.0,7.689088;180.0,5.588;181.0,3.621024;182.0,2.01168;183.0,0.89408;184.0,0.44704;185.0,0.268224;186.0,0.0;187.0,0.0;188.0,0.0;189.0,0.0;190.0,0.0;191.0,0.0;192.0,0.0;193.0,0.0;194.0,0.0;195.0,0.0;196.0,0.0;197.0,0.0;198.0,0.0;199.0,0.0;200.0,0.0;201.0,0.0;202.0,0.0;203.0,0.0;204.0,0.0;205.0,0.44704;206.0,0.22352;207.0,1.162304;208.0,3.442208;209.0,5.498592;210.0,7.063232;211.0,7.733792;212.0,8.672576;213.0,10.416032;214.0,12.159488;215.0,13.85824;216.0,15.020544;217.0,15.288768;218.0,16.004032;219.0,16.674592;220.0,17.121632;221.0,17.523968;222.0,17.926304;223.0,18.283936;224.0,18.32864;225.0,18.060416;226.0,17.747488;227.0,17.479264;228.0,17.032224;229.0,16.406368;230.0,16.048736;231.0,16.048736;232.0,15.959328;233.0,15.601696;234.0,15.154656;235.0,14.573504;236.0,14.260576;237.0,13.902944;238.0,13.679424;239.0,13.545312;240.0,13.455904;241.0,13.366496;242.0,13.321792;243.0,13.321792;244.0,13.321792;245.0,13.321792;246.0,13.277088;247.0,13.277088;248.0,13.232384;249.0,12.695936;250.0,11.533632;251.0,10.192512;252.0,8.49376;253.0,6.25856;254.0,3.844544;255.0,1.832864;256.0,0.581152;257.0,0.0;258.0,0.0;259.0,0.0;260.0,0.0;261.0,0.0;262.0,0.0;263.0,0.0;264.0,0.0;265.0,0.0;266.0,0.0;267.0,0.0;268.0,0.0;269.0,0.0;270.0,0.0;271.0,0.0;272.0,0.0;273.0,0.0;274.0,0.0;275.0,0.0;276.0,0.0;277.0,0.0;278.0,0.0;279.0,0.0;280.0,0.0;281.0,0.044704;282.0,2.01168;283.0,4.068064;284.0,6.079744;285.0,8.136128;286.0,10.103104;287.0,11.712448;288.0,13.098272;289.0,14.349984;290.0,15.42288;291.0,16.451072;292.0,17.166336;293.0,17.8816;294.0,18.418048;295.0,18.730976;296.0,18.865088;297.0,19.088608;298.0,19.22272;299.0,19.356832;300.0,19.44624;301.0,19.535648;302.0,19.803872;303.0,20.295616;304.0,20.519136;305.0,20.921472;306.0,21.279104;307.0,21.547328;308.0,21.726144;309.0,21.770848;310.0,21.726144;311.0,21.90496;312.0,22.262592;313.0,22.57552;314.0,22.888448;315.0,23.290784;316.0,23.559008;317.0,23.871936;318.0,23.424896;319.0,24.36368;320.0,24.497792;321.0,24.497792;322.0,24.453088;323.0,24.274272;324.0,24.14016;325.0,24.050752;326.0,23.91664;327.0,23.827232;328.0,23.648416;329.0,23.514304;330.0,23.24608;331.0,23.067264;332.0,22.79904;333.0,22.486112;334.0,22.039072;335.0,21.502624;336.0,20.78736;337.0,19.490944;338.0,18.194528;339.0,16.629888;340.0,15.378176;341.0,14.037056;342.0,12.785344;343.0,10.818368;344.0,8.091424;345.0,5.498592;346.0,3.621024;347.0,2.145792;348.0,1.162304;349.0,0.938784;350.0,0.0;351.0,0.0;352.0,0.0;353.0,0.0;354.0,0.0;355.0,0.0;356.0,0.0;357.0,0.0;358.0,0.0;359.0,0.0;360.0,0.0;361.0,0.0;362.0,0.0;363.0,0.0;364.0,0.0;365.0,0.0;366.0,0.0;367.0,0.0;368.0,0.0;369.0,0.0;370.0,0.0;371.0,1.922272;372.0,4.068064;373.0,5.900928;374.0,7.286752;375.0,8.538464;376.0,9.343136;377.0,10.147808;378.0,11.086592;379.0,12.025376;380.0,12.874752;381.0,13.4112;382.0,13.590016;383.0,13.679424;384.0,13.813536;385.0,13.902944;386.0,13.768832;387.0,13.902944;388.0,14.08176;389.0,14.484096;390.0,14.797024;391.0,14.886432;392.0,14.931136;393.0,15.065248;394.0,15.244064;395.0,15.512288;396.0,15.6464;397.0,15.825216;398.0,16.004032;399.0,16.09344;400.0,16.182848;401.0,16.227552;402.0,16.272256;403.0,16.31696;404.0,16.495776;405.0,16.629888;406.0,16.674592;407.0,16.898112;408.0,17.076928;409.0,17.255744;410.0,17.345152;411.0,17.255744;412.0,17.389856;413.0,17.43456;414.0,17.345152;415.0,17.255744;416.0,17.032224;417.0,16.808704;418.0,16.808704;419.0,16.674592;420.0,16.54048;421.0,16.361664;422.0,16.182848;423.0,16.09344;424.0,16.09344;425.0,15.86992;426.0,15.42288;427.0,14.75232;428.0,13.85824;429.0,12.2936;430.0,10.103104;431.0,8.9408;432.0,8.49376;433.0,8.672576;434.0,8.583168;435.0,9.209024;436.0,10.237216;437.0,10.997184;438.0,11.39952;439.0,12.025376;440.0,12.204192;441.0,12.606528;442.0,13.232384;443.0,13.500608;444.0,13.724128;445.0,13.992352;446.0,14.171168;447.0,14.394688;448.0,14.5288;449.0,14.75232;450.0,14.841728;451.0,14.886432;452.0,14.797024;453.0,14.618208;454.0,14.439392;455.0,14.260576;456.0,14.08176;457.0,13.947648;458.0,13.768832;459.0,13.63472;460.0,13.500608;461.0,13.366496;462.0,13.500608;463.0,13.679424;464.0,13.813536;465.0,13.947648;466.0,14.215872;467.0,14.484096;468.0,14.5288;469.0,14.439392;470.0,14.439392;471.0,14.662912;472.0,14.707616;473.0,14.662912;474.0,14.662912;475.0,14.886432;476.0,14.931136;477.0,14.707616;478.0,14.707616;479.0,14.662912;480.0,14.707616;481.0,14.662912;482.0,14.662912;483.0,14.484096;484.0,14.126464;485.0,13.679424;486.0,13.545312;487.0,13.545312;488.0,13.321792;489.0,13.098272;490.0,12.919456;491.0,12.874752;492.0,13.098272;493.0,13.4112;494.0,13.500608;495.0,13.590016;496.0,13.724128;497.0,13.768832;498.0,13.321792;499.0,12.830048;500.0,12.919456;501.0,13.053568;502.0,13.142976;503.0,12.785344;504.0,12.07008;505.0,12.159488;506.0,11.891264;507.0,10.371328;508.0,9.477248;509.0,9.477248;510.0,9.298432;511.0,8.002016;512.0,5.900928;513.0,4.24688;514.0,2.861056;515.0,1.832864;516.0,1.1176;517.0,0.0;518.0,0.0;519.0,0.0;520.0,0.0;521.0,0.0;522.0,0.0;523.0,0.0;524.0,0.0;525.0,0.0;526.0,0.0;527.0,0.0;528.0,0.0;529.0,0.0;530.0,0.0;531.0,0.0;532.0,0.0;533.0,0.0;534.0,0.0;535.0,0.0;536.0,0.0;537.0,0.268224;538.0,1.475232;539.0,2.637536;540.0,3.978656;541.0,4.559808;542.0,4.649216;543.0,4.425696;544.0,4.425696;545.0,4.69392;546.0,5.051552;547.0,5.543296;548.0,5.722112;549.0,6.25856;550.0,6.526784;551.0,6.92912;552.0,7.59968;553.0,7.8232;554.0,8.091424;555.0,8.225536;556.0,8.27024;557.0,8.136128;558.0,8.27024;559.0,8.180832;560.0,8.136128;561.0,8.002016;562.0,7.912608;563.0,7.912608;564.0,7.733792;565.0,7.778496;566.0,7.510272;567.0,7.8232;568.0,7.912608;569.0,7.8232;570.0,7.867904;571.0,7.733792;572.0,7.778496;573.0,7.867904;574.0,7.867904;575.0,8.002016;576.0,8.04672;577.0,7.957312;578.0,7.912608;579.0,7.8232;580.0,7.912608;581.0,7.912608;582.0,8.091424;583.0,8.225536;584.0,8.583168;585.0,8.449056;586.0,8.04672;587.0,6.973824;588.0,5.945632;589.0,4.4704;590.0,3.442208;591.0,2.592832;592.0,1.654048;593.0,1.072896;594.0,0.0;595.0,0.0;596.0,0.0;597.0,0.0;598.0,0.0;599.0,0.0;600.0,0.0]);
        annotation (Icon(graphics={Text(
                extent={{-120,130},{100,110}},
                lineColor={0,128,0},
                textString="SC03"),Line(
                points={{-110,140},{-102,168},{-96,182},{-90,162},{-84,180},{-76,162},
                    {-56,194},{-52,188},{-44,194},{-36,140},{-28,140},{-24,194},{-12,
                    176},{-4,174},{2,140},{14,190},{26,206},{36,140},{48,140},{60,180},
                    {78,192},{81.9961,164.027},{91.9902,177.986},{110,140}},
                color={255,0,0},
                smooth=Smooth.None)}));
      end SC03_Driver;

      model Sunday_Driver "Default driver profile from C2M2L_Ext"
        extends C2M2L_Decl.Testing_Blocks.Land.Driver_Land_Profile;
        annotation (Icon(coordinateSystem(preserveAspectRatio=false, extent={{-100,-100},
                  {100,100}}), graphics={Text(
                extent={{-120,130},{100,110}},
                lineColor={0,128,0},
                textString="Sunday")}));
      end Sunday_Driver;

      package Terrains
        extends Modelica.Icons.Package;

        model Flat
          extends Ground_Definition_2D(road=[-10, 0; 1000, 0]);
        end Flat;

        model Ground_Definition_2D
          parameter Boolean show_trace=false   annotation (Dialog(group="Animation"));
          parameter Boolean show_road=false   annotation (Dialog(group="Animation"));
          parameter Boolean animation=world.enableAnimation   annotation (Dialog(group="Animation"));
          parameter SI.Radius wheel_rad=0.5 annotation (Dialog(group="Show trace", enable=show_trace));

          // This should be overwritten to make different terrains
          parameter Integer samples=25;

          parameter Real road[:,2]=[-10,0.0; 10.0,0.0] "road profile" annotation(HideResult=true);
          parameter Real crr = 0.01 "coefficient of rolling resistance";

      protected
          parameter Real p[:,2]=C2M2L_Ext.C2M2L_Component_Building_Blocks.Suspension.Contact_Models.roll_through(
              road,
              wheel_rad,
              samples) if                                                 show_trace;

          Modelica.Mechanics.MultiBody.Parts.FixedTranslation actual_road[size(road, 1) -
            1](
            r={{road[i + 1, 1] - road[i, 1],road[i + 1, 2] - road[i, 2],0} for i in 1:(size(
                road, 1) - 1)},
            color={if rem(i, 2) == 0 then {0,0,0} else {128,128,0} for i in 1:(size(
                road, 1))},
            each specularCoefficient=0,
            each animation=animation,
            each shapeType="box",
            each width=0.01,
            each height=6) if                 animation and show_road
            annotation (Placement(transformation(extent={{-34,32},{-14,52}})));

          Modelica.Mechanics.MultiBody.Parts.FixedTranslation center_line[size(p, 1) -
            3](
            r={{p[i + 2, 1] - p[i + 1, 1],p[i + 2, 2] - p[i + 1, 2],0} for i in 1:(size(p, 1)
                 - 3)},
            each width=0.2,
            color={if rem(i, 2) == 0 then {128,0,0} else {255,0,0} for i in 1:(size(p,
                1)) - 2},
            each specularCoefficient=0,
            each animation=animation) if animation and show_trace
            annotation (Placement(transformation(extent={{-33,-21},{-13,-1}})));

          Modelica.Mechanics.MultiBody.Parts.Fixed fixed1(animation=false, r={road[1, 1],
                road[1, 2],0}) if animation
            annotation (Placement(transformation(extent={{-72,14},{-52,34}})));
          Modelica.Mechanics.MultiBody.Parts.Fixed fixed2(animation=false, r={p[2, 1],p[
                2, 2],0}) if   animation and show_trace
            annotation (Placement(transformation(extent={{-72,14},{-52,34}})));
          outer Modelica.Mechanics.MultiBody.World world
            annotation (Placement(transformation(extent={{50,71},{70,91}})));
          Modelica.Mechanics.MultiBody.Parts.FixedTranslation actual_road_left_side[size(road,
            1) - 1](
            each specularCoefficient=0,
            each animation=animation,
            each shapeType="box",
            each height=10,
            each width=0.02,
            r={{road[i + 1, 1] - road[i, 1],road[i + 1, 2] - road[i, 2],0} for i in 1:
                (size(road, 1) - 1)},
            color={if rem(i, 2) == 0 then {0,104,10} else {1,166,17} for i in 1:(size(
                 road, 1))}) if               animation and show_road
            annotation (Placement(transformation(extent={{-4,54},{16,74}})));
          Modelica.Mechanics.MultiBody.Parts.FixedTranslation actual_road_right_side[size(road,
            1) - 1](
            each specularCoefficient=0,
            each animation=animation,
            each shapeType="box",
            each height=10,
            each width=0.02,
            r={{road[i + 1, 1] - road[i, 1],road[i + 1, 2] - road[i, 2],0} for i in 1:
                (size(road, 1) - 1)},
            color={if rem(i, 2) == 0 then {0,104,10} else {1,166,17} for i in 1:(size(
                 road, 1))}) if               animation and show_road
            annotation (Placement(transformation(extent={{-3,6},{17,26}})));
      public
          Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation(
              animation=false, r={0,0,8})
            annotation (Placement(transformation(extent={{-41,54},{-21,74}})));
          Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation1(
              animation=false, r={0,0,-8})
            annotation (Placement(transformation(extent={{-37,12},{-17,32}})));
        equation
          for i in 1:(size(road, 1) - 2) loop
            connect(actual_road[i].frame_b, actual_road[i + 1].frame_a);
            connect(actual_road_right_side[i].frame_b, actual_road_right_side[i + 1].frame_a);
            connect(actual_road_left_side[i].frame_b, actual_road_left_side[i + 1].frame_a);
          end for;
          if show_trace then
            for i in 1:(if samples > 0 then samples - 2 else size(road, 1) - 2) loop
              connect(center_line[i].frame_b, center_line[i + 1].frame_a);
            end for;
          end if;
          connect(actual_road[1].frame_a, fixed1.frame_b)
                                                         annotation (Line(
              points={{-34,42},{-44,42},{-44,24},{-52,24}},
              color={95,95,95},
              thickness=0.5,
              smooth=Smooth.None));
          connect(center_line[1].frame_a, fixed2.frame_b)
                                                         annotation (Line(
              points={{-33,-11},{-44,-11},{-44,24},{-52,24}},
              color={95,95,95},
              thickness=0.5,
              smooth=Smooth.None));
          connect(fixed1.frame_b, fixedTranslation.frame_a) annotation (Line(
              points={{-52,24},{-46,24},{-46,64},{-41,64}},
              color={95,95,95},
              thickness=0.5,
              smooth=Smooth.None));
          connect(fixedTranslation.frame_b, actual_road_left_side[1].frame_a)
            annotation (Line(
              points={{-21,64},{-4,64}},
              color={95,95,95},
              thickness=0.5,
              smooth=Smooth.None));
          connect(fixed1.frame_b, fixedTranslation1.frame_a) annotation (Line(
              points={{-52,24},{-45,24},{-45,22},{-37,22}},
              color={95,95,95},
              thickness=0.5,
              smooth=Smooth.None));
          connect(fixedTranslation1.frame_b, actual_road_right_side[1].frame_a)
            annotation (Line(
              points={{-17,22},{-10,22},{-10,16},{-3,16}},
              color={95,95,95},
              thickness=0.5,
              smooth=Smooth.None));
          annotation (
            defaultComponentName="ground_context",
            defaultComponentPrefixes="inner",
            Icon(coordinateSystem(
                preserveAspectRatio=true,
                extent={{-100,-100},{100,100}},
                grid={1,1}), graphics={
                Rectangle(
                  extent={{-100,100},{100,-100}},
                  lineColor={0,0,255},
                  fillColor={255,255,255},
                  fillPattern=FillPattern.Solid),
                Polygon(
                  points={{-90,-69},{90,-69},{90,-19},{80,-23},{70,-29},{60,-21},{50,-22},
                      {40,-28},{30,-23},{20,-21},{10,-18},{0,-22},{-10,-24},{-20,-31},{
                      -30,-29},{-40,-28},{-50,-18},{-60,-20},{-70,-26},{-81,-32},{-90,-27},
                      {-90,-69}},
                  smooth=Smooth.Bezier,
                  fillColor={255,170,85},
                  fillPattern=FillPattern.Solid,
                  pattern=LinePattern.None),
                Rectangle(
                  extent={{-90,-39},{90,-90}},
                  fillColor={255,170,85},
                  fillPattern=FillPattern.Solid,
                  pattern=LinePattern.None),
                Ellipse(
                  extent={{-20,20},{20,-20}},
                  fillColor={215,215,215},
                  fillPattern=FillPattern.Solid,
                  pattern=LinePattern.None,
                  origin={-50,1},
                  rotation=360),
                Ellipse(
                  extent={{-20,20},{20,-20}},
                  fillColor={135,135,135},
                  fillPattern=FillPattern.Solid,
                  pattern=LinePattern.None,
                  origin={-30,-9},
                  rotation=360),
                Ellipse(
                  extent={{-20,20},{20,-20}},
                  fillColor={95,95,95},
                  fillPattern=FillPattern.Solid,
                  pattern=LinePattern.None,
                  origin={-10,-4},
                  rotation=360),
                Ellipse(
                  extent={{-20,20},{20,-20}},
                  fillColor={0,0,0},
                  fillPattern=FillPattern.Solid,
                  pattern=LinePattern.None,
                  origin={10,1},
                  rotation=360),
                Text(
                  extent={{-250,140},{250,120}},
                  lineColor={0,0,255},
                  textString="%name")}),
            Diagram(coordinateSystem(
                preserveAspectRatio=false,
                extent={{-100,-100},{100,100}},
                grid={1,1}), graphics));
        end Ground_Definition_2D;

        model RoughRoad
          extends Ground_Definition_2D(road=[
            -10, 0;
              0, 0;
             25, 0;
             35, 1;
             50, 0;
             80, 0;
            100, 4;
            120, 4;
            130, 0;
           1000, 0]);
        end RoughRoad;

        model Test
          extends Icons.Simple_Test_Case_200x200;
          inner Ground_Definition_2D ground_context(
            show_trace=true,
            show_road=true,
            road=[-10,0.0; 0,5; 15,5; 20,10; 25,12; 50,15; 100,5])
            annotation (Placement(transformation(extent={{-140,120},{-120,140}})));
          inner Modelica.Mechanics.MultiBody.World world
            annotation (Placement(transformation(extent={{-180,121},{-160,141}})));
        end Test;
      end Terrains;

      model Urban_Driver "Urban Dynamometer Driving Schedule"
        extends C2M2L_Decl.Testing_Blocks.Land.Driver_Base_Land(
        profile_data=[0.0,0.0;1.0,0.0;2.0,0.0;3.0,0.0;4.0,0.0;5.0,0.0;6.0,0.0;7.0,0.0;8.0,0.0;9.0,0.0;10.0,0.0;11.0,0.0;12.0,0.0;13.0,0.0;14.0,0.0;15.0,0.0;16.0,0.0;17.0,0.0;18.0,0.0;19.0,0.0;20.0,0.0;21.0,1.34112;22.0,2.637536;23.0,3.844544;24.0,5.14096;25.0,6.392672;26.0,7.554976;27.0,7.733792;28.0,8.091424;29.0,9.253728;30.0,9.700768;31.0,10.013696;32.0,10.0584;33.0,9.879584;34.0,9.61136;35.0,9.343136;36.0,9.119616;37.0,8.851392;38.0,7.59968;39.0,6.660896;40.0,6.660896;41.0,6.795008;42.0,6.92912;43.0,7.15264;44.0,7.644384;45.0,8.538464;46.0,9.432544;47.0,10.147808;48.0,10.237216;49.0,10.147808;50.0,10.103104;51.0,9.521952;52.0,8.49376;53.0,7.644384;54.0,7.063232;55.0,7.063232;56.0,7.912608;57.0,8.851392;58.0,9.656064;59.0,10.371328;60.0,10.818368;61.0,10.997184;62.0,11.131296;63.0,11.176;64.0,10.997184;65.0,10.95248;66.0,11.041888;67.0,11.086592;68.0,11.041888;69.0,10.997184;70.0,10.997184;71.0,11.220704;72.0,11.444224;73.0,11.488928;74.0,11.354816;75.0,11.131296;76.0,11.176;77.0,11.354816;78.0,11.62304;79.0,11.62304;80.0,11.488928;81.0,11.667744;82.0,11.935968;83.0,12.2936;84.0,12.785344;85.0,13.098272;86.0,13.321792;87.0,13.455904;88.0,13.590016;89.0,13.724128;90.0,13.724128;91.0,13.63472;92.0,13.590016;93.0,13.545312;94.0,13.590016;95.0,13.768832;96.0,13.590016;97.0,13.366496;98.0,13.18768;99.0,13.321792;100.0,13.545312;101.0,13.724128;102.0,13.813536;103.0,13.85824;104.0,13.813536;105.0,13.590016;106.0,13.321792;107.0,13.366496;108.0,13.500608;109.0,13.724128;110.0,13.947648;111.0,14.215872;112.0,14.394688;113.0,14.484096;114.0,14.394688;115.0,14.171168;116.0,12.785344;117.0,11.310112;118.0,9.83488;119.0,8.359648;120.0,6.884416;121.0,5.409184;122.0,3.933952;123.0,2.45872;124.0,0.983488;125.0,0.0;126.0,0.0;127.0,0.0;128.0,0.0;129.0,0.0;130.0,0.0;131.0,0.0;132.0,0.0;133.0,0.0;134.0,0.0;135.0,0.0;136.0,0.0;137.0,0.0;138.0,0.0;139.0,0.0;140.0,0.0;141.0,0.0;142.0,0.0;143.0,0.0;144.0,0.0;145.0,0.0;146.0,0.0;147.0,0.0;148.0,0.0;149.0,0.0;150.0,0.0;151.0,0.0;152.0,0.0;153.0,0.0;154.0,0.0;155.0,0.0;156.0,0.0;157.0,0.0;158.0,0.0;159.0,0.0;160.0,0.0;161.0,0.0;162.0,0.0;163.0,0.0;164.0,1.475232;165.0,2.950464;166.0,4.425696;167.0,5.900928;168.0,7.37616;169.0,8.851392;170.0,9.924288;171.0,10.863072;172.0,11.533632;173.0,11.801856;174.0,11.488928;175.0,11.220704;176.0,11.041888;177.0,11.176;178.0,11.265408;179.0,11.354816;180.0,11.533632;181.0,12.159488;182.0,11.84656;183.0,10.72896;184.0,10.147808;185.0,8.672576;186.0,7.912608;187.0,7.689088;188.0,8.091424;189.0,8.314944;190.0,8.9408;191.0,9.924288;192.0,10.95248;193.0,12.204192;194.0,13.63472;195.0,14.97584;196.0,16.182848;197.0,16.674592;198.0,17.568672;199.0,18.10512;200.0,18.820384;201.0,19.44624;202.0,20.161504;203.0,20.56384;204.0,20.921472;205.0,21.2344;206.0,21.2344;207.0,21.144992;208.0,21.100288;209.0,21.01088;210.0,21.01088;211.0,21.01088;212.0,21.01088;213.0,21.01088;214.0,21.100288;215.0,21.189696;216.0,21.413216;217.0,21.68144;218.0,21.949664;219.0,22.12848;220.0,22.352;221.0,22.620224;222.0,22.79904;223.0,23.02256;224.0,23.335488;225.0,23.782528;226.0,24.184864;227.0,24.408384;228.0,24.542496;229.0,24.5872;230.0,24.542496;231.0,24.408384;232.0,24.408384;233.0,24.497792;234.0,24.631904;235.0,24.81072;236.0,24.900128;237.0,25.078944;238.0,25.168352;239.0,25.302464;240.0,25.347168;241.0,25.347168;242.0,25.25776;243.0,25.25776;244.0,25.25776;245.0,25.25776;246.0,25.25776;247.0,25.25776;248.0,25.213056;249.0,25.078944;250.0,24.944832;251.0,24.631904;252.0,24.408384;253.0,24.229568;254.0,24.14016;255.0,24.006048;256.0,23.961344;257.0,24.095456;258.0,24.14016;259.0,24.184864;260.0,24.184864;261.0,24.050752;262.0,23.871936;263.0,23.69312;264.0,23.514304;265.0,23.290784;266.0,23.424896;267.0,23.24608;268.0,23.201376;269.0,23.111968;270.0,23.02256;271.0,23.067264;272.0,23.156672;273.0,23.290784;274.0,23.4696;275.0,23.69312;276.0,23.91664;277.0,24.14016;278.0,24.542496;279.0,24.766016;280.0,24.855424;281.0,25.03424;282.0,25.03424;283.0,24.944832;284.0,24.676608;285.0,24.36368;286.0,23.961344;287.0,23.4696;288.0,23.02256;289.0,23.02256;290.0,23.02256;291.0,22.843744;292.0,22.396704;293.0,22.352;294.0,22.396704;295.0,22.352;296.0,22.173184;297.0,22.12848;298.0,22.12848;299.0,22.12848;300.0,21.949664;301.0,21.726144;302.0,21.502624;303.0,21.100288;304.0,20.608544;305.0,20.1168;306.0,19.580352;307.0,19.043904;308.0,18.55216;309.0,18.015712;310.0,17.21104;311.0,16.54048;312.0,15.735808;313.0,15.109952;314.0,14.5288;315.0,14.08176;316.0,13.679424;317.0,13.63472;318.0,13.4112;319.0,12.96416;320.0,12.2936;321.0,11.086592;322.0,9.61136;323.0,8.985504;324.0,8.538464;325.0,8.27024;326.0,7.59968;327.0,6.92912;328.0,5.588;329.0,4.828032;330.0,3.57632;331.0,2.101088;332.0,0.625856;333.0,0.0;334.0,0.0;335.0,0.0;336.0,0.0;337.0,0.0;338.0,0.0;339.0,0.0;340.0,0.0;341.0,0.0;342.0,0.0;343.0,0.0;344.0,0.0;345.0,0.0;346.0,0.0;347.0,0.44704;348.0,1.922272;349.0,3.397504;350.0,4.872736;351.0,6.347968;352.0,7.733792;353.0,8.9408;354.0,10.0584;355.0,10.594848;356.0,11.265408;357.0,11.891264;358.0,12.561824;359.0,13.4112;360.0,13.768832;361.0,14.126464;362.0,14.349984;363.0,14.662912;364.0,15.020544;365.0,15.42288;366.0,15.467584;367.0,15.601696;368.0,15.556992;369.0,15.42288;370.0,15.512288;371.0,15.86992;372.0,16.09344;373.0,16.09344;374.0,16.09344;375.0,16.09344;376.0,16.09344;377.0,16.09344;378.0,16.138144;379.0,16.272256;380.0,16.31696;381.0,16.272256;382.0,16.09344;383.0,15.691104;384.0,15.244064;385.0,14.97584;386.0,14.037056;387.0,12.96416;388.0,11.488928;389.0,10.28192;390.0,9.074912;391.0,7.8232;392.0,6.48208;393.0,5.36448;394.0,3.889248;395.0,2.414016;396.0,0.938784;397.0,0.0;398.0,0.0;399.0,0.0;400.0,0.0;401.0,0.0;402.0,0.0;403.0,1.162304;404.0,2.637536;405.0,4.112768;406.0,5.588;407.0,7.063232;408.0,8.538464;409.0,10.013696;410.0,11.176;411.0,11.444224;412.0,12.2936;413.0,12.96416;414.0,13.4112;415.0,13.455904;416.0,13.4112;417.0,13.277088;418.0,13.098272;419.0,12.874752;420.0,12.51712;421.0,11.176;422.0,9.700768;423.0,8.225536;424.0,6.750304;425.0,5.275072;426.0,3.79984;427.0,2.324608;428.0,0.849376;429.0,0.0;430.0,0.0;431.0,0.0;432.0,0.0;433.0,0.0;434.0,0.0;435.0,0.0;436.0,0.0;437.0,0.0;438.0,0.0;439.0,0.0;440.0,0.0;441.0,0.0;442.0,0.0;443.0,0.0;444.0,0.0;445.0,0.0;446.0,0.0;447.0,0.0;448.0,1.475232;449.0,2.950464;450.0,4.425696;451.0,5.900928;452.0,7.37616;453.0,8.851392;454.0,10.326624;455.0,11.801856;456.0,12.427712;457.0,13.008864;458.0,14.08176;459.0,14.75232;460.0,15.020544;461.0,15.556992;462.0,15.691104;463.0,15.914624;464.0,16.138144;465.0,16.09344;466.0,16.138144;467.0,16.182848;468.0,16.09344;469.0,15.959328;470.0,16.09344;471.0,16.09344;472.0,15.914624;473.0,15.86992;474.0,15.825216;475.0,15.735808;476.0,15.735808;477.0,15.735808;478.0,15.735808;479.0,15.735808;480.0,15.735808;481.0,15.6464;482.0,15.691104;483.0,15.735808;484.0,15.86992;485.0,15.735808;486.0,15.6464;487.0,15.6464;488.0,15.6464;489.0,15.556992;490.0,15.467584;491.0,15.42288;492.0,14.97584;493.0,14.30528;494.0,13.455904;495.0,12.51712;496.0,11.39952;497.0,10.0584;498.0,8.851392;499.0,7.37616;500.0,5.900928;501.0,4.604512;502.0,3.218688;503.0,1.78816;504.0,0.44704;505.0,0.0;506.0,0.0;507.0,0.0;508.0,0.0;509.0,0.0;510.0,0.0;511.0,0.536448;512.0,1.56464;513.0,2.45872;514.0,2.90576;515.0,3.79984;516.0,4.291584;517.0,4.69392;518.0,5.319776;519.0,6.25856;520.0,7.15264;521.0,7.912608;522.0,8.49376;523.0,8.985504;524.0,9.38784;525.0,9.83488;526.0,10.28192;527.0,10.639552;528.0,10.95248;529.0,11.131296;530.0,11.176;531.0,11.176;532.0,11.176;533.0,11.176;534.0,11.176;535.0,11.176;536.0,11.444224;537.0,11.533632;538.0,11.62304;539.0,11.444224;540.0,11.265408;541.0,11.176;542.0,11.176;543.0,11.176;544.0,10.907776;545.0,10.326624;546.0,8.851392;547.0,7.37616;548.0,5.900928;549.0,4.425696;550.0,2.950464;551.0,1.475232;552.0,0.0;553.0,0.0;554.0,0.0;555.0,0.0;556.0,0.0;557.0,0.0;558.0,0.0;559.0,0.0;560.0,0.0;561.0,0.0;562.0,0.0;563.0,0.0;564.0,0.0;565.0,0.0;566.0,0.0;567.0,0.0;568.0,0.0;569.0,1.475232;570.0,2.950464;571.0,4.425696;572.0,5.81152;573.0,6.526784;574.0,7.15264;575.0,7.59968;576.0,7.59968;577.0,7.59968;578.0,7.8232;579.0,7.912608;580.0,7.912608;581.0,7.8232;582.0,7.59968;583.0,7.554976;584.0,7.420864;585.0,7.59968;586.0,7.644384;587.0,7.59968;588.0,7.420864;589.0,7.37616;590.0,7.37616;591.0,7.420864;592.0,7.59968;593.0,7.867904;594.0,8.27024;595.0,8.583168;596.0,9.030208;597.0,9.38784;598.0,9.432544;599.0,9.477248;600.0,9.656064;601.0,9.83488;602.0,10.013696;603.0,10.0584;604.0,10.0584;605.0,10.0584;606.0,10.147808;607.0,10.594848;608.0,11.220704;609.0,11.62304;610.0,11.84656;611.0,12.07008;612.0,11.667744;613.0,10.192512;614.0,8.71728;615.0,7.242048;616.0,5.766816;617.0,4.291584;618.0,2.816352;619.0,1.34112;620.0,0.0;621.0,0.0;622.0,0.0;623.0,0.0;624.0,0.0;625.0,0.0;626.0,0.0;627.0,0.0;628.0,0.0;629.0,0.0;630.0,0.0;631.0,0.0;632.0,0.0;633.0,0.0;634.0,0.0;635.0,0.0;636.0,0.0;637.0,0.0;638.0,0.0;639.0,0.0;640.0,0.0;641.0,0.0;642.0,0.0;643.0,0.0;644.0,0.0;645.0,0.0;646.0,0.89408;647.0,2.01168;648.0,3.486912;649.0,4.559808;650.0,5.588;651.0,6.25856;652.0,6.839712;653.0,7.8232;654.0,8.761984;655.0,9.38784;656.0,9.924288;657.0,10.416032;658.0,10.95248;659.0,11.310112;660.0,11.444224;661.0,11.62304;662.0,11.667744;663.0,11.712448;664.0,11.712448;665.0,11.801856;666.0,11.84656;667.0,11.84656;668.0,11.62304;669.0,11.39952;670.0,10.550144;671.0,9.566656;672.0,8.27024;673.0,7.331456;674.0,6.48208;675.0,5.185664;676.0,3.889248;677.0,2.592832;678.0,1.56464;679.0,0.89408;680.0,0.0;681.0,0.0;682.0,0.0;683.0,0.0;684.0,0.0;685.0,0.0;686.0,0.0;687.0,0.0;688.0,0.0;689.0,0.0;690.0,0.0;691.0,0.0;692.0,0.0;693.0,0.0;694.0,0.625856;695.0,1.475232;696.0,1.966976;697.0,2.90576;698.0,4.112768;699.0,5.051552;700.0,6.03504;701.0,6.526784;702.0,7.331456;703.0,7.465568;704.0,7.37616;705.0,7.37616;706.0,8.136128;707.0,8.583168;708.0,8.985504;709.0,9.61136;710.0,10.0584;711.0,10.0584;712.0,9.879584;713.0,10.147808;714.0,10.416032;715.0,10.50544;716.0,10.0584;717.0,9.656064;718.0,9.16432;719.0,8.04672;720.0,6.7056;721.0,5.36448;722.0,4.02336;723.0,2.771648;724.0,2.01168;725.0,1.34112;726.0,0.938784;727.0,0.22352;728.0,0.22352;729.0,1.430528;730.0,2.90576;731.0,4.291584;732.0,5.588;733.0,6.25856;734.0,7.15264;735.0,8.04672;736.0,8.761984;737.0,9.61136;738.0,10.326624;739.0,10.95248;740.0,11.39952;741.0,11.84656;742.0,12.114784;743.0,12.338304;744.0,12.472416;745.0,12.651232;746.0,12.785344;747.0,12.785344;748.0,12.651232;749.0,12.606528;750.0,12.51712;751.0,12.2936;752.0,11.980672;753.0,11.39952;754.0,10.50544;755.0,9.61136;756.0,8.49376;757.0,7.37616;758.0,6.660896;759.0,5.588;760.0,4.202176;761.0,2.771648;762.0,1.34112;763.0,0.67056;764.0,0.67056;765.0,0.22352;766.0,0.0;767.0,1.34112;768.0,2.816352;769.0,4.291584;770.0,5.766816;771.0,7.063232;772.0,7.8232;773.0,8.225536;774.0,8.71728;775.0,9.253728;776.0,9.83488;777.0,10.371328;778.0,11.176;779.0,11.84656;780.0,12.2936;781.0,12.51712;782.0,12.651232;783.0,12.919456;784.0,12.919456;785.0,12.919456;786.0,12.874752;787.0,12.74064;788.0,12.651232;789.0,12.651232;790.0,12.651232;791.0,12.606528;792.0,12.338304;793.0,12.2936;794.0,12.2936;795.0,12.2936;796.0,12.2936;797.0,12.2936;798.0,12.2936;799.0,12.338304;800.0,12.51712;801.0,12.74064;802.0,13.4112;803.0,13.85824;804.0,14.30528;805.0,14.75232;806.0,14.75232;807.0,15.020544;808.0,15.19936;809.0,15.333472;810.0,15.288768;811.0,15.19936;812.0,15.19936;813.0,15.154656;814.0,15.020544;815.0,14.797024;816.0,14.75232;817.0,14.5288;818.0,14.30528;819.0,14.260576;820.0,14.126464;821.0,14.08176;822.0,13.679424;823.0,13.4112;824.0,13.366496;825.0,13.366496;826.0,13.366496;827.0,13.366496;828.0,13.232384;829.0,13.18768;830.0,13.18768;831.0,13.098272;832.0,12.919456;833.0,12.606528;834.0,12.383008;835.0,12.07008;836.0,11.39952;837.0,10.594848;838.0,9.83488;839.0,9.16432;840.0,8.583168;841.0,8.583168;842.0,8.985504;843.0,9.343136;844.0,9.566656;845.0,9.83488;846.0,10.103104;847.0,10.371328;848.0,10.72896;849.0,11.176;850.0,11.62304;851.0,11.891264;852.0,11.891264;853.0,11.980672;854.0,12.07008;855.0,12.159488;856.0,12.427712;857.0,12.561824;858.0,12.874752;859.0,12.919456;860.0,12.96416;861.0,13.008864;862.0,12.96416;863.0,12.561824;864.0,12.2936;865.0,12.07008;866.0,11.533632;867.0,11.176;868.0,10.95248;869.0,11.086592;870.0,11.220704;871.0,11.39952;872.0,11.488928;873.0,11.712448;874.0,12.025376;875.0,12.2936;876.0,12.427712;877.0,12.695936;878.0,12.96416;879.0,13.053568;880.0,13.008864;881.0,12.96416;882.0,12.919456;883.0,12.74064;884.0,12.561824;885.0,12.51712;886.0,12.51712;887.0,12.338304;888.0,12.159488;889.0,11.891264;890.0,12.07008;891.0,12.2936;892.0,12.427712;893.0,12.51712;894.0,12.427712;895.0,12.51712;896.0,12.51712;897.0,12.51712;898.0,12.383008;899.0,12.248896;900.0,12.025376;901.0,11.891264;902.0,11.84656;903.0,11.84656;904.0,11.84656;905.0,11.757152;906.0,11.712448;907.0,11.712448;908.0,11.578336;909.0,11.444224;910.0,11.444224;911.0,11.578336;912.0,11.533632;913.0,11.39952;914.0,10.997184;915.0,10.50544;916.0,9.924288;917.0,9.656064;918.0,9.656064;919.0,9.700768;920.0,10.103104;921.0,10.460736;922.0,10.72896;923.0,10.818368;924.0,10.907776;925.0,11.131296;926.0,11.220704;927.0,11.265408;928.0,11.310112;929.0,11.39952;930.0,11.265408;931.0,11.176;932.0,11.176;933.0,11.176;934.0,11.041888;935.0,10.95248;936.0,10.863072;937.0,10.863072;938.0,10.95248;939.0,11.176;940.0,11.176;941.0,10.997184;942.0,10.997184;943.0,10.773664;944.0,10.95248;945.0,11.220704;946.0,11.444224;947.0,11.220704;948.0,10.72896;949.0,9.83488;950.0,8.985504;951.0,7.554976;952.0,6.079744;953.0,4.604512;954.0,3.12928;955.0,1.654048;956.0,0.178816;957.0,0.0;958.0,0.0;959.0,0.0;960.0,0.89408;961.0,2.369312;962.0,3.844544;963.0,5.319776;964.0,6.795008;965.0,7.8232;966.0,8.314944;967.0,8.9408;968.0,9.432544;969.0,9.83488;970.0,10.28192;971.0,10.95248;972.0,11.757152;973.0,12.2936;974.0,12.561824;975.0,12.695936;976.0,12.74064;977.0,12.74064;978.0,12.74064;979.0,12.383008;980.0,12.2936;981.0,12.159488;982.0,11.980672;983.0,11.84656;984.0,11.62304;985.0,11.488928;986.0,11.265408;987.0,10.72896;988.0,9.83488;989.0,9.61136;990.0,9.61136;991.0,9.745472;992.0,10.0584;993.0,10.28192;994.0,10.192512;995.0,10.192512;996.0,10.28192;997.0,10.147808;998.0,10.147808;999.0,10.147808;1000.0,10.50544;1001.0,10.72896;1002.0,10.997184;1003.0,11.086592;1004.0,11.220704;1005.0,11.39952;1006.0,11.444224;1007.0,11.39952;1008.0,11.176;1009.0,10.773664;1010.0,10.594848;1011.0,10.371328;1012.0,10.237216;1013.0,10.0584;1014.0,9.83488;1015.0,9.656064;1016.0,9.16432;1017.0,7.8232;1018.0,6.347968;1019.0,4.872736;1020.0,3.397504;1021.0,1.922272;1022.0,0.44704;1023.0,0.0;1024.0,0.0;1025.0,0.0;1026.0,0.0;1027.0,0.0;1028.0,0.0;1029.0,0.0;1030.0,0.0;1031.0,0.0;1032.0,0.0;1033.0,0.0;1034.0,0.0;1035.0,0.0;1036.0,0.0;1037.0,0.0;1038.0,0.0;1039.0,0.0;1040.0,0.0;1041.0,0.0;1042.0,0.0;1043.0,0.0;1044.0,0.0;1045.0,0.0;1046.0,0.0;1047.0,0.0;1048.0,0.0;1049.0,0.0;1050.0,0.0;1051.0,0.0;1052.0,0.0;1053.0,0.536448;1054.0,1.78816;1055.0,3.263392;1056.0,4.738624;1057.0,6.213856;1058.0,7.59968;1059.0,8.27024;1060.0,8.9408;1061.0,9.745472;1062.0,10.28192;1063.0,10.72896;1064.0,11.086592;1065.0,11.444224;1066.0,11.84656;1067.0,11.980672;1068.0,12.248896;1069.0,12.472416;1070.0,12.651232;1071.0,12.51712;1072.0,12.2936;1073.0,12.07008;1074.0,12.07008;1075.0,11.757152;1076.0,10.95248;1077.0,10.0584;1078.0,9.61136;1079.0,9.209024;1080.0,8.04672;1081.0,6.7056;1082.0,5.498592;1083.0,4.962144;1084.0,4.738624;1085.0,4.4704;1086.0,4.24688;1087.0,4.068064;1088.0,3.889248;1089.0,3.844544;1090.0,3.933952;1091.0,4.02336;1092.0,3.889248;1093.0,3.844544;1094.0,3.57632;1095.0,3.12928;1096.0,2.2352;1097.0,1.877568;1098.0,1.162304;1099.0,0.44704;1100.0,0.0;1101.0,0.044704;1102.0,0.268224;1103.0,0.715264;1104.0,1.609344;1105.0,3.084576;1106.0,4.4704;1107.0,5.722112;1108.0,6.25856;1109.0,6.48208;1110.0,7.15264;1111.0,8.091424;1112.0,8.9408;1113.0,9.38784;1114.0,9.477248;1115.0,9.521952;1116.0,9.566656;1117.0,9.700768;1118.0,10.0584;1119.0,10.28192;1120.0,10.639552;1121.0,10.95248;1122.0,11.176;1123.0,11.131296;1124.0,11.086592;1125.0,11.176;1126.0,11.354816;1127.0,11.533632;1128.0,11.62304;1129.0,11.801856;1130.0,11.891264;1131.0,12.025376;1132.0,12.07008;1133.0,12.07008;1134.0,12.07008;1135.0,12.025376;1136.0,11.980672;1137.0,11.980672;1138.0,11.84656;1139.0,11.801856;1140.0,11.62304;1141.0,11.39952;1142.0,10.997184;1143.0,10.50544;1144.0,9.61136;1145.0,8.9408;1146.0,7.8232;1147.0,7.15264;1148.0,6.25856;1149.0,4.783328;1150.0,3.308096;1151.0,1.832864;1152.0,0.357632;1153.0,0.0;1154.0,0.0;1155.0,0.0;1156.0,0.0;1157.0,0.0;1158.0,0.0;1159.0,0.0;1160.0,0.0;1161.0,0.0;1162.0,0.0;1163.0,0.0;1164.0,0.0;1165.0,0.0;1166.0,0.0;1167.0,0.0;1168.0,0.0;1169.0,0.938784;1170.0,2.414016;1171.0,3.889248;1172.0,5.36448;1173.0,6.839712;1174.0,8.314944;1175.0,9.432544;1176.0,10.28192;1177.0,10.50544;1178.0,10.28192;1179.0,10.0584;1180.0,8.9408;1181.0,7.465568;1182.0,5.990336;1183.0,4.515104;1184.0,3.039872;1185.0,1.56464;1186.0,0.089408;1187.0,0.0;1188.0,0.0;1189.0,0.0;1190.0,0.0;1191.0,0.0;1192.0,0.0;1193.0,0.0;1194.0,0.0;1195.0,0.0;1196.0,0.0;1197.0,0.089408;1198.0,0.67056;1199.0,1.56464;1200.0,2.90576;1201.0,4.380992;1202.0,5.36448;1203.0,5.766816;1204.0,5.81152;1205.0,5.632704;1206.0,5.722112;1207.0,5.856224;1208.0,5.856224;1209.0,6.25856;1210.0,6.92912;1211.0,7.59968;1212.0,8.314944;1213.0,8.806688;1214.0,9.38784;1215.0,9.61136;1216.0,9.745472;1217.0,9.745472;1218.0,9.61136;1219.0,9.477248;1220.0,9.61136;1221.0,9.745472;1222.0,9.83488;1223.0,9.790176;1224.0,9.700768;1225.0,9.61136;1226.0,9.61136;1227.0,9.566656;1228.0,8.985504;1229.0,8.71728;1230.0,8.583168;1231.0,8.761984;1232.0,8.851392;1233.0,8.9408;1234.0,8.71728;1235.0,7.8232;1236.0,6.92912;1237.0,5.81152;1238.0,4.4704;1239.0,3.57632;1240.0,2.68224;1241.0,1.78816;1242.0,1.1176;1243.0,0.312928;1244.0,0.0;1245.0,0.0;1246.0,0.0;1247.0,0.0;1248.0,0.0;1249.0,0.0;1250.0,0.0;1251.0,0.0;1252.0,0.44704;1253.0,0.44704;1254.0,0.44704;1255.0,0.44704;1256.0,0.44704;1257.0,0.715264;1258.0,1.34112;1259.0,1.78816;1260.0,2.2352;1261.0,2.816352;1262.0,3.57632;1263.0,4.4704;1264.0,4.69392;1265.0,4.24688;1266.0,3.79984;1267.0,3.397504;1268.0,3.933952;1269.0,4.91744;1270.0,6.25856;1271.0,7.59968;1272.0,8.71728;1273.0,9.38784;1274.0,9.745472;1275.0,9.924288;1276.0,10.28192;1277.0,10.550144;1278.0,10.773664;1279.0,10.95248;1280.0,10.95248;1281.0,10.72896;1282.0,10.50544;1283.0,10.50544;1284.0,10.50544;1285.0,10.50544;1286.0,10.50544;1287.0,10.50544;1288.0,10.72896;1289.0,10.773664;1290.0,10.95248;1291.0,11.041888;1292.0,11.176;1293.0,11.354816;1294.0,11.444224;1295.0,11.488928;1296.0,11.62304;1297.0,11.712448;1298.0,12.07008;1299.0,12.427712;1300.0,12.651232;1301.0,12.96416;1302.0,13.008864;1303.0,12.96416;1304.0,12.51712;1305.0,11.041888;1306.0,9.566656;1307.0,8.091424;1308.0,6.616192;1309.0,5.14096;1310.0,3.665728;1311.0,2.190496;1312.0,0.715264;1313.0,0.0;1314.0,0.0;1315.0,0.0;1316.0,0.0;1317.0,0.0;1318.0,0.0;1319.0,0.0;1320.0,0.0;1321.0,0.0;1322.0,0.0;1323.0,0.0;1324.0,0.0;1325.0,0.0;1326.0,0.0;1327.0,0.0;1328.0,0.0;1329.0,0.0;1330.0,0.0;1331.0,0.0;1332.0,0.0;1333.0,0.0;1334.0,0.0;1335.0,0.0;1336.0,0.0;1337.0,0.0;1338.0,0.67056;1339.0,2.145792;1340.0,3.621024;1341.0,5.096256;1342.0,5.900928;1343.0,6.750304;1344.0,7.510272;1345.0,8.180832;1346.0,8.71728;1347.0,9.074912;1348.0,9.521952;1349.0,9.790176;1350.0,9.879584;1351.0,10.013696;1352.0,9.83488;1353.0,9.656064;1354.0,9.432544;1355.0,9.16432;1356.0,8.9408;1357.0,8.761984;1358.0,8.27024;1359.0,7.8232;1360.0,7.37616;1361.0,6.92912;1362.0,6.25856;1363.0,4.91744;1364.0,3.57632;1365.0,2.324608;1366.0,1.1176;1367.0,0.0;1368.0,0.0;1369.0,0.0]);
        annotation (Icon(graphics={Text(
                extent={{-120,130},{100,110}},
                lineColor={0,128,0},
                textString="Urban"),
                                   Line(
                points={{-110,140},{-106,168},{-92,178},{-90,140},{-80,140},{-72,180},
                    {-64,170},{-56,192},{-44,198},{-36,140},{-28,140},{-26,184},{-18,
                    140},{-14,182},{-8,140},{-2,192},{8,186},{10,140},{20,170},{28,
                    176},{34,164},{46,172},{60,170},{64,140},{72,140},{82,172},{92,
                    140},{104,188},{110,140}},
                color={255,0,0},
                smooth=Smooth.None)}));
      end Urban_Driver;

      model US06_Driver "US06 Driving Cycle, per 8/96 Final Rule"
        extends C2M2L_Decl.Testing_Blocks.Land.Driver_Base_Land(
        profile_data=[0.0,0.0;1.0,0.0;2.0,0.0;3.0,0.0;4.0,0.0;5.0,0.0;6.0,0.089408;7.0,0.312928;8.0,0.491744;9.0,0.759968;10.0,2.68224;11.0,6.213856;12.0,9.16432;13.0,11.488928;14.0,11.176;15.0,12.695936;16.0,14.439392;17.0,15.467584;18.0,16.31696;19.0,17.166336;20.0,17.836896;21.0,18.865088;22.0,19.580352;23.0,19.759168;24.0,19.401536;25.0,19.043904;26.0,18.015712;27.0,17.523968;28.0,17.166336;29.0,17.166336;30.0,17.523968;31.0,17.345152;32.0,17.345152;33.0,16.31696;34.0,14.439392;35.0,12.338304;36.0,9.968992;37.0,7.733792;38.0,5.14096;39.0,2.592832;40.0,0.536448;41.0,0.0;42.0,0.0;43.0,0.0;44.0,0.0;45.0,0.0;46.0,0.0;47.0,0.0;48.0,0.0;49.0,0.357632;50.0,4.112768;51.0,6.660896;52.0,8.136128;53.0,9.924288;54.0,12.159488;55.0,14.037056;56.0,15.109952;57.0,16.629888;58.0,18.239232;59.0,19.66976;60.0,20.697952;61.0,21.279104;62.0,22.12848;63.0,22.888448;64.0,23.69312;65.0,24.318976;66.0,24.855424;67.0,25.213056;68.0,25.078944;69.0,25.123648;70.0,24.944832;71.0,24.631904;72.0,24.318976;73.0,24.229568;74.0,24.318976;75.0,24.229568;76.0,23.91664;77.0,23.380192;78.0,23.24608;79.0,23.201376;80.0,23.156672;81.0,23.201376;82.0,23.24608;83.0,23.4696;84.0,23.871936;85.0,24.542496;86.0,25.391872;87.0,26.285952;88.0,27.090624;89.0,27.850592;90.0,28.699968;91.0,29.594048;92.0,30.309312;93.0,31.024576;94.0,31.471616;95.0,31.561024;96.0,31.605728;97.0,31.426912;98.0,30.488128;99.0,29.72816;100.0,29.012896;101.0,28.476448;102.0,27.94;103.0,27.26944;104.0,26.509472;105.0,25.794208;106.0,25.03424;107.0,24.36368;108.0,23.603712;109.0,22.888448;110.0,22.12848;111.0,21.45792;112.0,20.697952;113.0,19.66976;114.0,18.373344;115.0,17.345152;116.0,16.853408;117.0,16.361664;118.0,15.780512;119.0,13.4112;120.0,10.907776;121.0,8.851392;122.0,6.92912;123.0,4.828032;124.0,2.816352;125.0,1.430528;126.0,0.938784;127.0,0.536448;128.0,0.0;129.0,0.0;130.0,0.0;131.0,0.0;132.0,0.0;133.0,0.0;134.0,0.0;135.0,0.0;136.0,1.207008;137.0,4.112768;138.0,7.197344;139.0,10.147808;140.0,13.053568;141.0,15.288768;142.0,17.345152;143.0,19.22272;144.0,20.250912;145.0,20.921472;146.0,21.45792;147.0,22.12848;148.0,22.486112;149.0,23.02256;150.0,23.335488;151.0,23.514304;152.0,23.69312;153.0,24.050752;154.0,24.050752;155.0,24.050752;156.0,24.408384;157.0,25.168352;158.0,25.436576;159.0,25.973024;160.0,26.107136;161.0,26.643584;162.0,26.777696;163.0,26.911808;164.0,27.04592;165.0,26.688288;166.0,26.062432;167.0,25.973024;168.0,25.838912;169.0,25.615392;170.0,25.7048;171.0,25.302464;172.0,25.48128;173.0,25.302464;174.0,25.25776;175.0,25.123648;176.0,25.213056;177.0,25.302464;178.0,25.213056;179.0,25.078944;180.0,25.03424;181.0,24.989536;182.0,24.497792;183.0,24.229568;184.0,24.408384;185.0,23.335488;186.0,24.453088;187.0,24.900128;188.0,25.48128;189.0,25.92832;190.0,25.973024;191.0,26.554176;192.0,26.777696;193.0,27.26944;194.0,27.448256;195.0,27.671776;196.0,27.94;197.0,27.94;198.0,28.029408;199.0,27.805888;200.0,27.94;201.0,28.208224;202.0,28.029408;203.0,28.074112;204.0,28.16352;205.0,28.655264;206.0,28.565856;207.0,28.655264;208.0,28.744672;209.0,28.83408;210.0,29.012896;211.0,29.191712;212.0,29.50464;213.0,29.50464;214.0,29.683456;215.0,28.655264;216.0,28.431744;217.0,28.565856;218.0,28.655264;219.0,28.476448;220.0,28.744672;221.0,28.699968;222.0,28.565856;223.0,28.699968;224.0,28.342336;225.0,28.61056;226.0,28.565856;227.0,28.61056;228.0,28.521152;229.0,28.61056;230.0,28.297632;231.0,28.342336;232.0,28.565856;233.0,28.61056;234.0,28.744672;235.0,28.968192;236.0,29.102304;237.0,28.61056;238.0,28.699968;239.0,28.208224;240.0,28.476448;241.0,28.208224;242.0,28.476448;243.0,28.38704;244.0,28.16352;245.0,28.208224;246.0,28.16352;247.0,28.297632;248.0,28.342336;249.0,28.297632;250.0,27.94;251.0,27.94;252.0,28.118816;253.0,28.074112;254.0,27.805888;255.0,27.895296;256.0,27.850592;257.0,27.850592;258.0,27.895296;259.0,27.761184;260.0,27.94;261.0,28.074112;262.0,27.850592;263.0,27.850592;264.0,27.895296;265.0,27.671776;266.0,28.074112;267.0,28.074112;268.0,27.850592;269.0,28.074112;270.0,27.895296;271.0,27.761184;272.0,27.671776;273.0,27.627072;274.0,27.761184;275.0,27.761184;276.0,27.761184;277.0,27.71648;278.0,27.895296;279.0,27.805888;280.0,27.805888;281.0,27.895296;282.0,28.029408;283.0,27.984704;284.0,28.476448;285.0,28.744672;286.0,28.968192;287.0,29.102304;288.0,29.459936;289.0,29.549344;290.0,29.95168;291.0,30.041088;292.0,30.1752;293.0,30.532832;294.0,30.532832;295.0,30.756352;296.0,30.890464;297.0,31.024576;298.0,32.052768;299.0,32.231584;300.0,33.483296;301.0,32.455104;302.0,32.276288;303.0,32.276288;304.0,32.18688;305.0,32.4104;306.0,32.544512;307.0,32.499808;308.0,32.097472;309.0,31.918656;310.0,31.784544;311.0,31.784544;312.0,31.695136;313.0,31.73984;314.0,31.73984;315.0,31.829248;316.0,32.231584;317.0,32.455104;318.0,32.902144;319.0,33.438592;320.0,33.840928;321.0,34.556192;322.0,35.047936;323.0,35.450272;324.0,34.958528;325.0,33.97504;326.0,33.796224;327.0,34.153856;328.0,34.690304;329.0,34.86912;330.0,35.360864;331.0,35.53968;332.0,35.718496;333.0,35.718496;334.0,35.897312;335.0,35.897312;336.0,35.53968;337.0,35.53968;338.0,35.360864;339.0,35.182048;340.0,34.690304;341.0,34.19856;342.0,33.215072;343.0,32.455104;344.0,31.650432;345.0,30.219904;346.0,29.683456;347.0,29.817568;348.0,29.549344;349.0,29.459936;350.0,29.594048;351.0,29.549344;352.0,29.996384;353.0,30.130496;354.0,30.532832;355.0,30.532832;356.0,30.711648;357.0,30.488128;358.0,30.443424;359.0,30.39872;360.0,29.996384;361.0,29.683456;362.0,29.549344;363.0,29.370528;364.0,29.50464;365.0,29.683456;366.0,29.50464;367.0,29.638752;368.0,29.95168;369.0,30.1752;370.0,30.354016;371.0,30.443424;372.0,30.62224;373.0,30.801056;374.0,30.666944;375.0,31.024576;376.0,31.024576;377.0,31.024576;378.0,31.2928;379.0,31.471616;380.0,31.561024;381.0,31.695136;382.0,31.426912;383.0,31.561024;384.0,31.426912;385.0,31.158688;386.0,31.248096;387.0,31.337504;388.0,31.113984;389.0,30.979872;390.0,31.248096;391.0,31.158688;392.0,31.06928;393.0,31.248096;394.0,31.382208;395.0,31.382208;396.0,31.382208;397.0,31.73984;398.0,31.650432;399.0,31.695136;400.0,31.605728;401.0,31.695136;402.0,31.829248;403.0,31.873952;404.0,31.650432;405.0,31.829248;406.0,32.052768;407.0,32.142176;408.0,32.455104;409.0,32.320992;410.0,32.320992;411.0,32.231584;412.0,32.18688;413.0,32.142176;414.0,32.455104;415.0,32.544512;416.0,32.723328;417.0,32.231584;418.0,31.96336;419.0,31.695136;420.0,31.471616;421.0,31.51632;422.0,31.695136;423.0,31.382208;424.0,31.73984;425.0,31.382208;426.0,31.426912;427.0,30.890464;428.0,30.756352;429.0,30.488128;430.0,30.532832;431.0,30.488128;432.0,30.264608;433.0,30.085792;434.0,30.1752;435.0,30.219904;436.0,30.219904;437.0,30.041088;438.0,29.95168;439.0,29.638752;440.0,29.772864;441.0,29.594048;442.0,29.683456;443.0,29.459936;444.0,29.549344;445.0,29.28112;446.0,27.805888;447.0,27.805888;448.0,27.448256;449.0,27.314144;450.0,27.448256;451.0,27.314144;452.0,27.448256;453.0,27.448256;454.0,27.627072;455.0,27.627072;456.0,27.627072;457.0,27.627072;458.0,27.805888;459.0,27.627072;460.0,27.805888;461.0,27.984704;462.0,27.805888;463.0,27.984704;464.0,27.805888;465.0,27.984704;466.0,27.984704;467.0,28.16352;468.0,27.984704;469.0,27.805888;470.0,27.314144;471.0,26.59888;472.0,26.285952;473.0,25.391872;474.0,24.900128;475.0,24.184864;476.0,23.02256;477.0,21.994368;478.0,21.815552;479.0,21.279104;480.0,20.072096;481.0,18.55216;482.0,16.629888;483.0,15.467584;484.0,14.75232;485.0,13.053568;486.0,9.968992;487.0,7.912608;488.0,7.733792;489.0,6.25856;490.0,4.4704;491.0,2.68224;492.0,0.89408;493.0,0.0;494.0,0.0;495.0,0.0;496.0,0.0;497.0,0.0;498.0,0.0;499.0,0.0;500.0,0.0;501.0,0.089408;502.0,1.966976;503.0,4.515104;504.0,6.973824;505.0,9.298432;506.0,11.220704;507.0,12.383008;508.0,12.606528;509.0,11.980672;510.0,11.086592;511.0,10.013696;512.0,7.644384;513.0,5.051552;514.0,3.084576;515.0,3.3528;516.0,4.962144;517.0,6.884416;518.0,8.896096;519.0,10.818368;520.0,12.114784;521.0,12.74064;522.0,12.606528;523.0,11.444224;524.0,9.700768;525.0,7.733792;526.0,5.409184;527.0,3.3528;528.0,2.592832;529.0,1.072896;530.0,0.536448;531.0,0.849376;532.0,2.995168;533.0,5.275072;534.0,7.510272;535.0,9.700768;536.0,11.578336;537.0,12.383008;538.0,12.51712;539.0,12.114784;540.0,10.907776;541.0,9.030208;542.0,6.795008;543.0,4.157472;544.0,2.2352;545.0,1.296416;546.0,1.072896;547.0,3.755136;548.0,6.03504;549.0,7.957312;550.0,9.924288;551.0,11.712448;552.0,13.4112;553.0,13.321792;554.0,11.62304;555.0,9.521952;556.0,7.242048;557.0,5.096256;558.0,2.950464;559.0,1.162304;560.0,0.0;561.0,0.0;562.0,0.0;563.0,0.0;564.0,0.0;565.0,0.0;566.0,0.0;567.0,0.0;568.0,0.134112;569.0,2.861056;570.0,5.677408;571.0,8.583168;572.0,10.639552;573.0,12.606528;574.0,15.601696;575.0,16.764;576.0,18.015712;577.0,20.1168;578.0,22.307296;579.0,23.067264;580.0,22.888448;581.0,22.620224;582.0,22.307296;583.0,21.368512;584.0,19.937984;585.0,18.418048;586.0,16.898112;587.0,14.931136;588.0,12.51712;589.0,10.594848;590.0,8.404352;591.0,5.766816;592.0,2.771648;593.0,0.983488;594.0,0.0;595.0,0.0;596.0,0.0;597.0,0.0;598.0,0.0;599.0,0.0;600.0,0.0]);
        annotation (Icon(graphics={Text(
                extent={{-120,130},{100,110}},
                lineColor={0,128,0},
                textString="US06"),Line(
                points={{-110,140},{-102,168},{-96,162},{-90,140},{-80,140},{-72,180},
                    {-64,170},{-56,192},{-40,170},{-36,140},{-28,140},{-26,172},{-18,
                    178},{-12,172},{-8,190},{8,190},{12,198},{18,190},{36,190},{42,
                    184},{48,140},{54,156},{62,140},{70,156},{78,140},{84,156},{92,
                    140},{104,188},{110,140}},
                color={255,0,0},
                smooth=Smooth.None)}));
      end US06_Driver;
      annotation (Icon(graphics={
          Line(
            points={{-70,-58},{-40,42}},
            color={0,0,0},
            smooth=Smooth.None),
          Line(
            points={{50,-58},{18,42}},
            color={0,0,0},
            smooth=Smooth.None),
          Line(
            points={{-10,-58},{-10,-38}},
            color={0,0,0},
            smooth=Smooth.None),
          Line(
            points={{-10,-18},{-10,2}},
            color={0,0,0},
            smooth=Smooth.None),
          Line(
            points={{-10,22},{-10,42}},
            color={0,0,0},
            smooth=Smooth.None)}));
    end Land_Decl;

    model TorqueSourceFlangeWithBearing
      Modelica.Blocks.Sources.Trapezoid torque1(
        offset=0,
        rising=1,
        period=20,
        nperiod=1,
        width=8,
        startTime=0,
        amplitude=torque)
        annotation (Placement(transformation(extent={{-68,-8},{-48,12}})));
      Modelica.Mechanics.Rotational.Sources.Torque torque2
        annotation (Placement(transformation(extent={{-18,-8},{2,12}})));
      Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing flange(
          includeBearingConnector=true)
        annotation (Placement(transformation(extent={{110,-12},{130,8}}),
            iconTransformation(extent={{110,-12},{130,8}})));
      parameter Real torque=1 "Requested Torque";
  protected
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a
        annotation (Placement(transformation(extent={{76,-60},{108,-28}})));
    equation
      connect(torque2.flange, flange.flange) annotation (Line(
          points={{2,2},{62,2},{62,-2},{120,-2}},
          color={0,0,0},
          smooth=Smooth.None));
      connect(torque1.y,torque2. tau)
                                     annotation (Line(
          points={{-47,2},{-20,2}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(flange.bearingFrame, frame_a) annotation (Line(
          points={{120,-2},{98,-2},{98,-44},{92,-44}},
          color={135,135,135},
          thickness=0.5,
          smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-120,
                -120},{120,120}}), graphics), Icon(coordinateSystem(
              preserveAspectRatio=false, extent={{-120,-120},{120,120}}), graphics={
            Polygon(
              points={{96,10},{76,68},{47,37},{96,10}},
              lineColor={0,0,0},
              fillColor={0,0,0},
              fillPattern=FillPattern.Solid),
            Line(
              points={{-78,10},{-54,40},{-26,62},{8,72},{38,66},{58,54},{74,38},{86,
                  24},{96,10}},
              color={0,0,0},
              thickness=0.5),
            Text(
              extent={{-160,106},{160,80}},
              textString="%name",
              lineColor={0,0,255}),
            Line(points={{-44,-32},{-28,-18},{-6,-6},{14,-4},{32,-8},{46,-16},{58,
                  -26},{66,-36},{74,-48}},     color={0,0,0}),
            Text(
              extent={{-52,-19},{-131,-60}},
              lineColor={0,0,0},
              textString="tau")}));
    end TorqueSourceFlangeWithBearing;
  end Testing_Blocks;

  annotation (uses(C2M2L_Decl,
      Modelica(version="3.2.1")), version="1.1");
end C2M2L_CyPhy;
