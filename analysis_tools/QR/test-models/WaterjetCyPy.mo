within ;
package WaterjetCyPy
  import SI =
            Modelica.SIunits;
    extends Modelica.Icons.Package;

  model Test_Driver
    extends C2M2L_Decl.Icons.Test_Driver;

    Modelica.Blocks.Sources.Ramp vehicle_speed(
      height=10,
      duration=3,
      startTime=0.25)
      annotation (Placement(transformation(extent={{-100,-10},{-80,10}})));
    Modelica.Blocks.Sources.TimeTable steer_command(startTime=0, table=[0,0])
      annotation (Placement(transformation(extent={{-100,20},{-80,40}})));
    Modelica.Blocks.Sources.Ramp reverse_command(
      duration=1,
      height=1,
      startTime=3500)
      annotation (Placement(transformation(extent={{-100,80},{-80,100}})));
    Modelica.Blocks.Sources.Ramp thrust_demand(duration=2, height=0.5)
      annotation (Placement(transformation(extent={{-100,50},{-80,70}})));
    C2M2L_Decl.Interfaces.Context_Interfaces.Driver.Driver_Bus
                         waterjet_Control_Bus
      annotation (Placement(transformation(extent={{90,-10},{110,10}})));
  equation
    connect(vehicle_speed.y, waterjet_Control_Bus.vehicle_speed) annotation (Line(
        points={{-79,0},{100,0}},
        color={0,0,127},
        smooth=Smooth.None), Text(
        string="%second",
        index=1,
        extent={{6,3},{6,3}}));
    connect(steer_command.y, waterjet_Control_Bus.steer_signal) annotation (Line(
        points={{-79,30},{-46,30},{-46,4},{100,4},{100,0}},
        color={0,0,127},
        smooth=Smooth.None), Text(
        string="%second",
        index=1,
        extent={{6,3},{6,3}}));
    connect(thrust_demand.y, waterjet_Control_Bus.acceleratorPedalPosition)
      annotation (Line(
        points={{-79,60},{-38,60},{-38,8},{100,8},{100,0}},
        color={0,0,127},
        smooth=Smooth.None), Text(
        string="%second",
        index=1,
        extent={{6,3},{6,3}}));
    connect(reverse_command.y, waterjet_Control_Bus.brakePedalPosition)
      annotation (Line(
        points={{-79,90},{-24,90},{-24,14},{100,14},{100,0}},
        color={0,0,127},
        smooth=Smooth.None), Text(
        string="%second",
        index=1,
        extent={{6,3},{6,3}}));
    annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{
              -100,-100},{100,100}}),
                        graphics));
  end Test_Driver;

  model Waterjet "water jet with a simple thrust model"
    //extends Modelica.Mechanics.MultiBody.Interfaces.PartialOneFrame_a;
    extends C2M2L_Decl.Containers.Mechanical_Models.Case_1_Shaft(redeclare
        C2M2L_Decl.Environments.Lumped_Thermal_Fixed_HTC_Infinite env(redeclare
          package Medium =
          C2M2L_Decl.Media.Incompressible.Essotherm650_Const_Prop,          fixed_T=
            373.15));

  extends C2M2L_Decl.Interfaces.Delivered_C2M2L;
  //parameters from instance sheet
   parameter SI.MomentOfInertia MOI_rotation = 1.22
      "rotational moment of inertia";
   parameter SI.Force max_thrust_forward_max_knots = 1000
      "max thrust whilst vehicle is moving";
   parameter SI.Force max_thrust_forward_zero_knots = 2000
      "max thrust whilst vehicle is stationary";
   parameter SI.Power max_input_power = 100000 "max operating power";
   parameter SI.Torque max_input_torque = 1000 "max operating torque";
   parameter C2M2L_Decl.cml_extended_physicals.Fraction reverse_thrust_factor=0.43
      "Percentage of forward thrust when in reverse";
  //  parameter Boolean steer_port_only = false "Waterjet steers to port side only";
  //  parameter Boolean steer_starboard_only = false "Waterjet steers to starboard side only";
  //  parameter Boolean steer_port_and_starboard = true "Waterjet can steer to port or starboard side";
  //  parameter Angle max_steer_angle = 45 "Maximum steer angle for waterjet [deg]";

    C2M2L_Decl.Interfaces.Context_Interfaces.Driver.Driver_Bus driver_bus
      annotation (Placement(transformation(extent={{-10,194},{10,214}})));
    Modelica.Mechanics.Rotational.Components.Inertia inertia(J=1, stateSelect=
          StateSelect.always)
      annotation (Placement(transformation(extent={{-180,-10},{-160,10}})));
    Modelica.Mechanics.MultiBody.Interfaces.Frame_a mount_01
      annotation (Placement(transformation(extent={{184,-14},{216,18}})));
    Modelica.Mechanics.MultiBody.Sensors.AbsoluteVelocity absoluteVelocity1(
        resolveInFrame=Modelica.Mechanics.MultiBody.Types.ResolveInFrameA.frame_a)
      annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=180,
          origin={150,-140})));
    Modelica.Mechanics.MultiBody.Forces.WorldForceAndTorque forceAndTorque(
        resolveInFrame=Modelica.Mechanics.MultiBody.Types.ResolveInFrameB.frame_b)
      annotation (Placement(transformation(extent={{140,68},{160,88}})));
    Modelica.Blocks.Sources.Constant const[3](k={0,0,0})
      annotation (Placement(transformation(extent={{100,120},{120,140}})));
    Modelica.Blocks.Sources.Constant const1(k=0)
      annotation (Placement(transformation(extent={{60,90},{80,110}})));
    Modelica.Blocks.Sources.Constant const2(k=0)
      annotation (Placement(transformation(extent={{60,40},{80,60}})));
    Modelica.Mechanics.Rotational.Sensors.SpeedSensor speedSensor
      annotation (Placement(transformation(extent={{-140,-114},{-120,-94}})));
    C2M2L_Decl.C2M2L_Component_Building_Blocks.Amphibious.Waterjet.Waterjet_Thrust_Calculator
      waterjet_Thrust_Calculator(
      max_thrust_zero=max_thrust_forward_zero_knots,
      max_thrust_forward=max_thrust_forward_max_knots,
      max_torque=max_input_torque,
      max_rot_speed=max_input_speed,
      no_thrust_rot_speed=no_thrust_rot_speed,
      max_forward_speed=max_forward_speed)
      annotation (Placement(transformation(extent={{-60,-120},{-40,-100}})));
    Modelica.Mechanics.Rotational.Sources.Torque torqueStep
      annotation (Placement(transformation(extent={{-100,-10},{-120,10}})));
    Modelica.Blocks.Logical.LessThreshold greaterThreshold
      annotation (Placement(transformation(extent={{-82,118},{-62,138}})));
    C2M2L_Decl.MSL_Decl.Blocks.Logical.TriggeredTrapezoid triggeredTrapezoid(
      offset=1,
      rising=1,
      amplitude=-(reverse_thrust_factor + 1))
      annotation (Placement(transformation(extent={{-50,118},{-30,138}})));
    Modelica.Blocks.Math.Product product1
      annotation (Placement(transformation(extent={{24,64},{44,84}})));
    parameter SI.AngularVelocity max_input_speed=98.43
      "maximum rotational speed";
    parameter SI.AngularVelocity no_thrust_rot_speed=26
      "rotational speed that gives no thrust";
    parameter SI.Velocity max_forward_speed=10
      "forward speed at which max_thrust_forward_max_knots is specified";
  equation
    connect(inertia.flange_a, brg_01.flange) annotation (Line(
        points={{-180,0},{-200,0}},
        color={0,0,0},
        smooth=Smooth.None));
    connect(absoluteVelocity1.frame_a, mount_01)
                                               annotation (Line(
        points={{160,-140},{180,-140},{180,2},{200,2}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(forceAndTorque.frame_b, mount_01)
                                            annotation (Line(
        points={{160,78},{180,78},{180,2},{200,2}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(const.y, forceAndTorque.torque) annotation (Line(
        points={{121,130},{130,130},{130,84},{138,84}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(const1.y, forceAndTorque.force[2]) annotation (Line(
        points={{81,100},{94.5,100},{94.5,72},{138,72}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(const2.y, forceAndTorque.force[3]) annotation (Line(
        points={{81,50},{122,50},{122,73.3333},{138,73.3333}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(waterjet_Thrust_Calculator.vehicle_speed, absoluteVelocity1.v[1])
      annotation (Line(
        points={{-62,-116},{-80,-116},{-80,-140},{126,-140},{126,-139.333},{139,
            -139.333}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(torqueStep.flange, inertia.flange_b) annotation (Line(
        points={{-120,0},{-160,0}},
        color={0,0,0},
        smooth=Smooth.None));
    connect(waterjet_Thrust_Calculator.torque, torqueStep.tau) annotation (Line(
        points={{-39,-104},{-20,-104},{-20,0},{-98,0}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(speedSensor.flange, inertia.flange_b) annotation (Line(
        points={{-140,-104},{-150,-104},{-150,0},{-160,0}},
        color={0,0,0},
        smooth=Smooth.None));
    connect(speedSensor.w, waterjet_Thrust_Calculator.shaft_speed) annotation (
        Line(
        points={{-119,-104},{-62,-104}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(greaterThreshold.u, driver_bus.brakePedalPosition) annotation (Line(
        points={{-84,128},{-96,128},{-96,170},{-4,170},{-4,204},{0,204}},
        color={0,0,127},
        smooth=Smooth.None), Text(
        string="%second",
        index=1,
        extent={{6,3},{6,3}}));
    connect(triggeredTrapezoid.u, greaterThreshold.y) annotation (Line(
        points={{-52,128},{-61,128}},
        color={255,0,255},
        smooth=Smooth.None));
    connect(product1.y, forceAndTorque.force[1]) annotation (Line(
        points={{45,74},{92,74},{92,70.6667},{138,70.6667}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(waterjet_Thrust_Calculator.thrust, product1.u2) annotation (Line(
        points={{-39,-116},{4,-116},{4,68},{22,68}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(triggeredTrapezoid.y, product1.u1) annotation (Line(
        points={{-29,128},{-4,128},{-4,80},{22,80}},
        color={0,0,127},
        smooth=Smooth.None));
    annotation (Diagram(coordinateSystem(preserveAspectRatio=true, extent={{-200,
              -320},{200,280}}),
                        graphics), Icon(coordinateSystem(preserveAspectRatio=true,
            extent={{-200,-320},{200,280}}),
                                        graphics={
          Line(
            points={{-132,-48},{-26,50},{-2,64},{24,68},{124,68},{142,58},{142,
                -44},{128,-54},{32,-54},{20,-54},{12,-60},{-132,-60},{-132,-46}},
            color={0,0,255},
            smooth=Smooth.None),
          Rectangle(
            extent={{-112,16},{152,4}},
            lineColor={0,0,255},
            fillColor={0,0,255},
            fillPattern=FillPattern.Solid),
          Rectangle(
            extent={{24,68},{22,-54}},
            lineColor={0,0,255},
            fillColor={0,0,255},
            fillPattern=FillPattern.Solid),
          Rectangle(
            extent={{94,68},{100,-54}},
            lineColor={0,0,255},
            fillColor={0,128,255},
            fillPattern=FillPattern.HorizontalCylinder),
          Rectangle(
            extent={{110,68},{116,-54}},
            lineColor={0,0,255},
            fillColor={0,128,255},
            fillPattern=FillPattern.HorizontalCylinder),
          Rectangle(
            extent={{130,62},{134,-50}},
            lineColor={0,0,255},
            fillColor={0,128,255},
            fillPattern=FillPattern.HorizontalCylinder),
          Rectangle(
            extent={{80,-6},{142,26}},
            lineColor={0,0,255},
            fillPattern=FillPattern.Solid,
            fillColor={0,0,255})}),
      Documentation(info="<html>
<p>Notes:</p>
<p><ul>
<li>Waterjet outputs directional force based on input shaft power and steeing/reverse signal</li>
<li>user must defin (by 0 or 1 input) if waterjet steers to port sode only, starboard side only or both.</li>
<li>user must define max steering input angle. A steering signal input of 1 will steer to that maximum angle.</li>
<li>if a reverse signal is recived, a thrust reduction factor will be applied to the forward thrust ans the direction will be reversed</li>
</ul></p>
</html>"));
  end Waterjet;

 model Test_Bench_Waterjet
  extends C2M2L_Decl.Icons.Simple_Test_Case;

  inner C2M2L_Decl.MSL_Decl.Fluid.System system;

  inner C2M2L_Decl.Environments.Lumped_Thermal_Fixed_HTC env
    annotation (Placement(transformation(extent={{60,60},{80,80}})));
  inner Modelica.Mechanics.MultiBody.World world(gravityType=Modelica.Mechanics.MultiBody.Types.GravityTypes.NoGravity,
      enableAnimation=false)
    annotation (Placement(transformation(extent={{-100,-40},{-80,-20}})));
  Modelica.Mechanics.Rotational.Sources.ConstantTorque constantSpeed(
      tau_constant=500)
    annotation (Placement(transformation(extent={{-98,0},{-78,20}})));
  Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing flange(
      includeBearingConnector=true)
    annotation (Placement(transformation(extent={{-60,0},{-40,20}})));
  Test_Driver test_Driver
    annotation (Placement(transformation(extent={{-40,60},{-20,80}})));
  replaceable
      C2M2L_Decl.C2M2L_Delivered_Component_Implementations.Amphibious.Waterjet.Waterjet
      waterjet
    annotation (Placement(transformation(extent={{-20,-22},{20,38}})));
  Modelica.Mechanics.MultiBody.Joints.Prismatic prismatic
    annotation (Placement(transformation(extent={{46,0},{66,20}})));
  Modelica.Mechanics.MultiBody.Parts.Body body(m=20000) annotation (Placement(
        transformation(
        extent={{-10,-10},{10,10}},
        rotation=90,
        origin={40,30})));
  Modelica.Mechanics.MultiBody.Parts.Fixed fixed annotation (Placement(
        transformation(
        extent={{-10,-10},{10,10}},
        rotation=90,
        origin={80,0})));
 equation

  connect(waterjet.brg_01, flange)               annotation (Line(
      points={{-20,10},{-50,10}},
      color={135,135,135},
      thickness=0.5,
      smooth=Smooth.None));
  connect(fixed.frame_b, prismatic.frame_b) annotation (Line(
      points={{80,10},{66,10}},
      color={95,95,95},
      thickness=0.5,
      smooth=Smooth.None));
  connect(test_Driver.waterjet_Control_Bus, waterjet.driver_bus)
    annotation (Line(
      points={{-20,70},{0,70},{0,30.4}},
      color={170,213,255},
      thickness=0.5,
      smooth=Smooth.None));
  connect(constantSpeed.flange, flange.flange) annotation (Line(
      points={{-78,10},{-50,10}},
      color={0,0,0},
      smooth=Smooth.None));
  connect(prismatic.frame_a, waterjet.mount_01) annotation (Line(
      points={{46,10},{33,10},{33,10.2},{20,10.2}},
      color={95,95,95},
      thickness=0.5,
      smooth=Smooth.None));
  connect(body.frame_a, waterjet.mount_01) annotation (Line(
      points={{40,20},{40,10.2},{20,10.2}},
      color={95,95,95},
      thickness=0.5,
      smooth=Smooth.None));
  annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
              -100},{100,100}}),
                      graphics),
    experiment(StopTime=30, Interval=0.01),
    __Dymola_experimentSetupOutput,
    Commands(file="Scripts/Waterjet_Instance_Test.mos" "Waterjet_Instance_Test"));
 end Test_Bench_Waterjet;

  package Components
  model Body
      "Rigid body with mass, inertia tensor and one frame connector (12 potential states)"

    import Modelica.Mechanics.MultiBody.Types;
    import Modelica.Mechanics.MultiBody.Frames;
    import Modelica.SIunits.Conversions.to_unit1;

    Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a
        "Coordinate system fixed at body"
                                        annotation (Placement(transformation(
            extent={{-116,-16},{-84,16}}, rotation=0)));
    parameter Boolean animation=true
        "= true, if animation shall be enabled (show cylinder and sphere)";
    parameter SI.Position r_CM[3](start={0,0,0})
        "Vector from frame_a to center of mass, resolved in frame_a";
    parameter SI.Mass m(min=0, start=1) "Mass of rigid body";
    parameter SI.Inertia I_11(min=0) = 0.001 "(1,1) element of inertia tensor"
      annotation (Dialog(group=
            "Inertia tensor (resolved in center of mass, parallel to frame_a)"));
    parameter SI.Inertia I_22(min=0) = 0.001 "(2,2) element of inertia tensor"
      annotation (Dialog(group=
            "Inertia tensor (resolved in center of mass, parallel to frame_a)"));
    parameter SI.Inertia I_33(min=0) = 0.001 "(3,3) element of inertia tensor"
      annotation (Dialog(group=
            "Inertia tensor (resolved in center of mass, parallel to frame_a)"));
    parameter SI.Inertia I_21 = 0 "(2,1) element of inertia tensor"
                                        annotation (Dialog(group=
            "Inertia tensor (resolved in center of mass, parallel to frame_a)"));
    parameter SI.Inertia I_31 = 0 "(3,1) element of inertia tensor"
                                        annotation (Dialog(group=
            "Inertia tensor (resolved in center of mass, parallel to frame_a)"));
    parameter SI.Inertia I_32 = 0 "(3,2) element of inertia tensor"
                                        annotation (Dialog(group=
            "Inertia tensor (resolved in center of mass, parallel to frame_a)"));

    SI.Position r_0[3](start={0,0,0}, each stateSelect=if enforceStates then
          StateSelect.always else StateSelect.avoid)
        "Position vector from origin of world frame to origin of frame_a"
      annotation (Dialog(tab="Initialization",showStartAttribute=true));
    SI.Velocity v_0[3](start={0,0,0}, each stateSelect=if enforceStates then
          StateSelect.always else StateSelect.avoid)
        "Absolute velocity of frame_a, resolved in world frame (= der(r_0))"
      annotation (Dialog(tab="Initialization",showStartAttribute=true));
    SI.Acceleration a_0[3](start={0,0,0})
        "Absolute acceleration of frame_a resolved in world frame (= der(v_0))"
      annotation (Dialog(tab="Initialization",showStartAttribute=true));

    parameter Boolean angles_fixed=false
        "= true, if angles_start are used as initial values, else as guess values"
      annotation (
      Evaluate=true,
      choices(checkBox=true),
      Dialog(tab="Initialization"));
    parameter SI.Angle angles_start[3]={0,0,0}
        "Initial values of angles to rotate frame_a around 'sequence_start' axes into frame_b"
      annotation (Dialog(tab="Initialization"));
    parameter Types.RotationSequence sequence_start={1,2,3}
        "Sequence of rotations to rotate frame_a into frame_b at initial time"
      annotation (Evaluate=true, Dialog(tab="Initialization"));

    parameter Boolean w_0_fixed=false
        "= true, if w_0_start are used as initial values, else as guess values"
      annotation (
      Evaluate=true,
      choices(checkBox=true),
      Dialog(tab="Initialization"));
    parameter SI.AngularVelocity w_0_start[3]={0,0,0}
        "Initial or guess values of angular velocity of frame_a resolved in world frame"
      annotation (Dialog(tab="Initialization"));

    parameter Boolean z_0_fixed=false
        "= true, if z_0_start are used as initial values, else as guess values"
      annotation (
      Evaluate=true,
      choices(checkBox=true),
      Dialog(tab="Initialization"));
    parameter SI.AngularAcceleration z_0_start[3]={0,0,0}
        "Initial values of angular acceleration z_0 = der(w_0)"
      annotation (Dialog(tab="Initialization"));

    parameter SI.Diameter sphereDiameter=world.defaultBodyDiameter
        "Diameter of sphere"
                           annotation (Dialog(
        tab="Animation",
        group="if animation = true",
        enable=animation));
    input Types.Color sphereColor=Modelica.Mechanics.MultiBody.Types.Defaults.BodyColor
        "Color of sphere"
                        annotation (Dialog(
        colorSelector=true,
        tab="Animation",
        group="if animation = true",
        enable=animation));
    parameter SI.Diameter cylinderDiameter=sphereDiameter/Types.Defaults.BodyCylinderDiameterFraction
        "Diameter of cylinder"
                             annotation (Dialog(
        tab="Animation",
        group="if animation = true",
        enable=animation));
    input Types.Color cylinderColor=sphereColor "Color of cylinder" annotation (
       Dialog(
        colorSelector=true,
        tab="Animation",
        group="if animation = true",
        enable=animation));
    input Types.SpecularCoefficient specularCoefficient=world.defaultSpecularCoefficient
        "Reflection of ambient light (= 0: light is completely absorbed)"
      annotation (Dialog(
        tab="Animation",
        group="if animation = true",
        enable=animation));
    parameter Boolean enforceStates=false
        "= true, if absolute variables of body object shall be used as states (StateSelect.always)"
      annotation (Evaluate=true,Dialog(tab="Advanced"));
    parameter Boolean useQuaternions=true
        "= true, if quaternions shall be used as potential states otherwise use 3 angles as potential states"
      annotation (Evaluate=true,Dialog(tab="Advanced"));
    parameter Types.RotationSequence sequence_angleStates={1,2,3}
        "Sequence of rotations to rotate world frame into frame_a around the 3 angles used as potential states"
      annotation (Evaluate=true, Dialog(tab="Advanced", enable=not
            useQuaternions));

    final parameter SI.Inertia I[3, 3]=[I_11, I_21, I_31; I_21, I_22, I_32;
        I_31, I_32, I_33] "inertia tensor";
    final parameter Frames.Orientation R_start=
        Modelica.Mechanics.MultiBody.Frames.axesRotations(
          sequence_start,
          angles_start,
          zeros(3))
        "Orientation object from world frame to frame_a at initial time";
    final parameter SI.AngularAcceleration z_a_start[3]=Frames.resolve2(R_start,
        z_0_start)
        "Initial values of angular acceleration z_a = der(w_a), i.e., time derivative of angular velocity resolved in frame_a";

    SI.AngularVelocity w_a[3](
      start=Frames.resolve2(R_start, w_0_start),
      fixed=fill(w_0_fixed, 3),
      each stateSelect=if enforceStates then (if useQuaternions then
          StateSelect.always else StateSelect.never) else StateSelect.avoid)
        "Absolute angular velocity of frame_a resolved in frame_a";
    SI.AngularAcceleration z_a[3](start=Frames.resolve2(R_start, z_0_start),
        fixed=fill(z_0_fixed, 3))
        "Absolute angular acceleration of frame_a resolved in frame_a";
    SI.Acceleration g_0[3] "Gravity acceleration resolved in world frame";

    protected
    outer Modelica.Mechanics.MultiBody.World world;

    // Declarations for quaternions (dummies, if quaternions are not used)
    parameter Frames.Quaternions.Orientation Q_start=Frames.to_Q(R_start)
        "Quaternion orientation object from world frame to frame_a at initial time";
    Frames.Quaternions.Orientation Q(start=Q_start, each stateSelect=if
          enforceStates then (if useQuaternions then StateSelect.prefer else
          StateSelect.never) else StateSelect.avoid)
        "Quaternion orientation object from world frame to frame_a (dummy value, if quaternions are not used as states)";

    // Declaration for 3 angles
    parameter SI.Angle phi_start[3]=if sequence_start[1] ==
        sequence_angleStates[1] and sequence_start[2] == sequence_angleStates[2]
         and sequence_start[3] == sequence_angleStates[3] then angles_start
         else Frames.axesRotationsAngles(R_start, sequence_angleStates)
        "Potential angle states at initial time";
    SI.Angle phi[3](start=phi_start, each stateSelect=if enforceStates then (
          if useQuaternions then StateSelect.never else StateSelect.always)
           else StateSelect.avoid)
        "Dummy or 3 angles to rotate world frame into frame_a of body";
    SI.AngularVelocity phi_d[3](each stateSelect=if enforceStates then (if
          useQuaternions then StateSelect.never else StateSelect.always) else
          StateSelect.avoid) "= der(phi)";
    SI.AngularAcceleration phi_dd[3] "= der(phi_d)";

    // Declarations for animation
    Modelica.Mechanics.MultiBody.Visualizers.Advanced.Shape cylinder(
      shapeType="cylinder",
      color=cylinderColor,
      specularCoefficient=specularCoefficient,
      length=if Modelica.Math.Vectors.length(r_CM) > sphereDiameter/2 then
          Modelica.Math.Vectors.length(r_CM) - (if cylinderDiameter > 1.1*
          sphereDiameter then sphereDiameter/2 else 0) else 0,
      width=cylinderDiameter,
      height=cylinderDiameter,
      lengthDirection = to_unit1(r_CM),
      widthDirection={0,1,0},
      r=frame_a.r_0,
      R=frame_a.R) if world.enableAnimation and animation;
    Modelica.Mechanics.MultiBody.Visualizers.Advanced.Shape sphere(
      shapeType="sphere",
      color=sphereColor,
      specularCoefficient=specularCoefficient,
      length=sphereDiameter,
      width=sphereDiameter,
      height=sphereDiameter,
      lengthDirection={1,0,0},
      widthDirection={0,1,0},
      r_shape=r_CM - {1,0,0}*sphereDiameter/2,
      r=frame_a.r_0,
      R=frame_a.R) if world.enableAnimation and animation and sphereDiameter >
      0;
  initial equation
    if angles_fixed then
      // Initialize positional variables
      if not Connections.isRoot(frame_a.R) then
        // frame_a.R is computed somewhere else
        zeros(3) = Frames.Orientation.equalityConstraint(frame_a.R, R_start);
      elseif useQuaternions then
        // frame_a.R is computed from quaternions Q
        zeros(3) = Frames.Quaternions.Orientation.equalityConstraint(Q, Q_start);
      else
        // frame_a.R is computed from the 3 angles 'phi'
        phi = phi_start;
      end if;
    end if;

  equation
    if enforceStates then
      Connections.root(frame_a.R);
    else
      Connections.potentialRoot(frame_a.R);
    end if;
    r_0 = frame_a.r_0;

    if not Connections.isRoot(frame_a.R) then
      // Body does not have states
      // Dummies
      Q = {0,0,0,1};
      phi = zeros(3);
      phi_d = zeros(3);
      phi_dd = zeros(3);
    elseif useQuaternions then
      // Use Quaternions as states (with dynamic state selection)
      frame_a.R = Frames.from_Q(Q, Frames.Quaternions.angularVelocity2(Q, der(Q)));
      {0} = Frames.Quaternions.orientationConstraint(Q);

      // Dummies
      phi = zeros(3);
      phi_d = zeros(3);
      phi_dd = zeros(3);
    else
      // Use Cardan angles as states
      phi_d = der(phi);
      phi_dd = der(phi_d);
      frame_a.R = Frames.axesRotations(
          sequence_angleStates,
          phi,
          phi_d);

      // Dummies
      Q = {0,0,0,1};
    end if;

    // gravity acceleration at center of mass resolved in world frame
    g_0 = world.gravityAcceleration(frame_a.r_0 + Frames.resolve1(frame_a.R,
      r_CM));

    // translational kinematic differential equations
    v_0 = der(frame_a.r_0);
    a_0 = der(v_0);

    // rotational kinematic differential equations
    w_a = Frames.angularVelocity2(frame_a.R);
    z_a = der(w_a);

    /* Newton/Euler equations with respect to center of mass
            a_CM = a_a + cross(z_a, r_CM) + cross(w_a, cross(w_a, r_CM));
            f_CM = m*(a_CM - g_a);
            t_CM = I*z_a + cross(w_a, I*w_a);
       frame_a.f = f_CM
       frame_a.t = t_CM + cross(r_CM, f_CM);
    Inserting the first three equations in the last two results in:
  */
    frame_a.f = m*(Frames.resolve2(frame_a.R, a_0 - g_0) + cross(z_a, r_CM) +
      cross(w_a, cross(w_a, r_CM)));
    frame_a.t = I*z_a + cross(w_a, I*w_a) + cross(r_CM, frame_a.f);
    annotation (Icon(coordinateSystem(
          preserveAspectRatio=true,
          extent={{-100,-100},{100,100}}), graphics={
          Rectangle(
            extent={{-100,30},{-3,-31}},
            lineColor={0,24,48},
            fillPattern=FillPattern.HorizontalCylinder,
            fillColor={0,127,255},
            radius=10),
          Text(
            extent={{150,-100},{-150,-70}},
            lineColor={0,0,0},
            textString="m=%m"),
          Text(
            extent={{-150,110},{150,70}},
            textString="%name",
            lineColor={0,0,255}),
          Ellipse(
            extent={{-20,60},{100,-60}},
            lineColor={0,24,48},
            fillPattern=FillPattern.Sphere,
            fillColor={0,127,255})}), Documentation(info="<HTML>
<p>
<b>Rigid body</b> with mass and inertia tensor.
All parameter vectors have to be resolved in frame_a.
The <b>inertia tensor</b> has to be defined with respect to a
coordinate system that is parallel to frame_a with the
origin at the center of mass of the body.
</p>
<p>
By default, this component is visualized by a <b>cylinder</b> located
between frame_a and the center of mass and by a <b>sphere</b> that has
its center at the center of mass. If the cylinder length is smaller as
the radius of the sphere, e.g., since frame_a is located at the
center of mass, the cylinder is not displayed. Note, that
the animation may be switched off via parameter animation = <b>false</b>.
</p>
<p>
<IMG src=\"modelica://Modelica/Resources/Images/Mechanics/MultiBody/Body.png\" ALT=\"Parts.Body\">
</p>

<p>
<b>States of Body Components</b>
</p>
<p>
Every body has potential states. If possible a tool will select
the states of joints and not the states of bodies because this is
usually the most efficient choice. In this case the position, orientation,
velocity and angular velocity of frame_a of the body will be computed
by the component that is connected to frame_a. However, if a body is moving
freely in space, variables of the body have to be used as states. The potential
states of the body are:
</p>
<ul>
<li> The <b>position vector</b> frame_a.r_0 from the origin of the
     world frame to the origin of frame_a of the body, resolved in
     the world frame and the <b>absolute velocity</b> v_0 of the origin of
     frame_a, resolved in the world frame (= der(frame_a.r_0)).
</li>
<li> If parameter <b>useQuaternions</b> in the \"Advanced\" menu
     is <b>true</b> (this is the default), then <b>4 quaternions</b>
     are potential states. Additionally, the coordinates of the
     absolute angular velocity vector of the
     body are 3 potential states.<br>
     If <b>useQuaternions</b> in the \"Advanced\" menu
     is <b>false</b>, then <b>3 angles</b> and the derivatives of
     these angles are potential states. The orientation of frame_a
     is computed by rotating the world frame along the axes defined
     in parameter vector \"sequence_angleStates\" (default = {1,2,3}, i.e.,
     the Cardan angle sequence) around the angles used as potential states.
     For example, the default is to rotate the x-axis of the world frame
     around angles[1], the new y-axis around angles[2] and the new z-axis
     around angles[3], arriving at frame_a.
 </li>
</ul>
<p>
The quaternions have the slight disadvantage that there is a
non-linear constraint equation between the 4 quaternions.
Therefore, at least one non-linear equation has to be solved
during simulation. A tool might, however, analytically solve this
simple constraint equation. Using the 3 angles as states has the
disadvantage that there is a singular configuration in which a
division by zero will occur. If it is possible to determine in advance
for an application class that this singular configuration is outside
of the operating region, the 3 angles might be used as potential
states by setting <b>useQuaternions</b> = <b>false</b>.
</p>
<p>
In text books about 3-dimensional mechanics often 3 angles and the
angular velocity are used as states. This is not the case here, since
3 angles and their derivatives are used as potential states
(if useQuaternions = false). The reason
is that for real-time simulation the discretization formula of the
integrator might be \"inlined\" and solved together with the body equations.
By appropriate symbolic transformation the performance is
drastically increased if angles and their
derivatives are used as states, instead of angles and the angular
velocity.
</p>
<p>
Whether or not variables of the body are used as states is usually
automatically selected by the Modelica translator. If parameter
<b>enforceStates</b> is set to <b>true</b> in the \"Advanced\" menu,
then body variables are forced to be used as states according
to the setting of parameters \"useQuaternions\" and
\"sequence_angleStates\".
</p>
</HTML>"));
  end Body;

  model Prismatic
      "Prismatic joint (1 translational degree-of-freedom, 2 potential states, optional axis flange)"
       import Modelica.Mechanics.MultiBody.Frames;
    extends Modelica.Mechanics.MultiBody.Interfaces.PartialElementaryJoint;
    Modelica.Mechanics.Translational.Interfaces.Flange_a axis if useAxisFlange
        "1-dim. translational flange that drives the joint"
      annotation (Placement(transformation(extent={{90,50},{70,70}}, rotation=0)));
    Modelica.Mechanics.Translational.Interfaces.Flange_b support if useAxisFlange
        "1-dim. translational flange of the drive drive support (assumed to be fixed in the world frame, NOT in the joint)"
      annotation (Placement(transformation(extent={{-30,50},{-50,70}}, rotation=
             0)));

    parameter Boolean useAxisFlange=false "= true, if axis flange is enabled"
      annotation(Evaluate=true, HideResult=true, choices(checkBox=true));
    parameter Boolean animation=true "= true, if animation shall be enabled";
    parameter Modelica.Mechanics.MultiBody.Types.Axis n={1,0,0}
        "Axis of translation resolved in frame_a (= same as in frame_b)"
      annotation (Evaluate=true);
    constant SI.Position s_offset=0
        "Relative distance offset (distance between frame_a and frame_b = s_offset + s)"
      annotation (Evaluate=false);
    parameter Modelica.Mechanics.MultiBody.Types.Axis boxWidthDirection={0,1,0}
        "Vector in width direction of box, resolved in frame_a"
      annotation (Evaluate=true, Dialog(tab="Animation", group=
            "if animation = true", enable=animation));
    parameter SI.Distance boxWidth=world.defaultJointWidth
        "Width of prismatic joint box"
      annotation (Dialog(tab="Animation", group="if animation = true", enable=animation));
    parameter SI.Distance boxHeight=boxWidth "Height of prismatic joint box"
      annotation (Dialog(tab="Animation", group="if animation = true", enable=animation));
    input Modelica.Mechanics.MultiBody.Types.Color boxColor=Modelica.Mechanics.MultiBody.Types.Defaults.JointColor
        "Color of prismatic joint box"
      annotation (Dialog(colorSelector=true, tab="Animation", group="if animation = true", enable=animation));
    input Modelica.Mechanics.MultiBody.Types.SpecularCoefficient specularCoefficient = world.defaultSpecularCoefficient
        "Reflection of ambient light (= 0: light is completely absorbed)"
      annotation (Dialog(tab="Animation", group="if animation = true", enable=animation));
    parameter StateSelect stateSelect=StateSelect.prefer
        "Priority to use distance s and v=der(s) as states"
                                                          annotation(Dialog(tab="Advanced"));
    final parameter Real e[3](each final unit="1")=
       Modelica.Math.Vectors.normalizeWithAssert(n)
        "Unit vector in direction of prismatic axis n";

    SI.Position s(start=0, final stateSelect=stateSelect)
        "Relative distance between frame_a and frame_b"
      annotation (unassignedMessage="
The relative distance s of a prismatic joint cannot be determined.
Possible reasons:
- A non-zero mass might be missing on either side of the parts
  connected to the prismatic joint.
- Too many StateSelect.always are defined and the model
  has less degrees of freedom as specified with this setting
  (remove all StateSelect.always settings).
");

    SI.Velocity v(start=0,final stateSelect=stateSelect)
        "First derivative of s (relative velocity)";
    SI.Acceleration a(start=0) "Second derivative of s (relative acceleration)";
    SI.Force f "Actuation force in direction of joint axis";

    protected
    Modelica.Mechanics.MultiBody.Visualizers.Advanced.Shape box(
      shapeType="box",
      color=boxColor,
      specularCoefficient=specularCoefficient,
      length=if noEvent(abs(s + s_offset) > 1.e-6) then s + s_offset else 1.e-6,
      width=boxWidth,
      height=boxHeight,
      lengthDirection=e,
      widthDirection=boxWidthDirection,
      r=frame_a.r_0,
      R=frame_a.R) if world.enableAnimation and animation;
    Modelica.Mechanics.Translational.Components.Fixed fixed
      annotation (Placement(transformation(extent={{-50,30},{-30,50}})));
    Modelica.Mechanics.Translational.Interfaces.InternalSupport internalAxis(f = f)
      annotation (Placement(transformation(extent={{70,50},{90,30}})));
    Modelica.Mechanics.Translational.Sources.ConstantForce constantForce(f_constant=0) if not useAxisFlange
      annotation (Placement(transformation(extent={{40,30},{60,50}})));
  equation
    v = der(s);
    a = der(v);

    // relationships between kinematic quantities of frame_a and of frame_b
    frame_b.r_0 = frame_a.r_0 + Frames.resolve1(frame_a.R, e*(s_offset + s));
    frame_b.R = frame_a.R;

    // Force and torque balance
    zeros(3) = frame_a.f + frame_b.f;
    zeros(3) = frame_a.t + frame_b.t + cross(e*(s_offset + s), frame_b.f);

    // d'Alemberts principle
    f = -e*frame_b.f;

    // Connection to internal connectors
    s = internalAxis.s;

    connect(fixed.flange, support) annotation (Line(
        points={{-40,40},{-40,60}},
        color={0,127,0},
        smooth=Smooth.None));
    connect(internalAxis.flange, axis)    annotation (Line(
        points={{80,40},{80,60}},
        color={0,127,0},
        smooth=Smooth.None));
    connect(constantForce.flange, internalAxis.flange)    annotation (Line(
        points={{60,40},{80,40}},
        color={0,127,0},
        smooth=Smooth.None));
    annotation (
      Icon(coordinateSystem(
          preserveAspectRatio=true,
          extent={{-100,-100},{100,100}}), graphics={
          Rectangle(
            extent={{-100,-50},{-30,41}},
            pattern=LinePattern.None,
            fillColor={192,192,192},
            fillPattern=FillPattern.Solid,
            lineColor={0,0,255}),
          Rectangle(
            extent={{-100,40},{-30,50}},
            pattern=LinePattern.None,
            fillColor={0,0,0},
            fillPattern=FillPattern.Solid,
            lineColor={0,0,255}),
          Rectangle(
            extent={{-30,-30},{100,20}},
            pattern=LinePattern.None,
            fillColor={192,192,192},
            fillPattern=FillPattern.Solid,
            lineColor={0,0,255}),
          Rectangle(
            extent={{-30,20},{100,30}},
            pattern=LinePattern.None,
            fillColor={0,0,0},
            fillPattern=FillPattern.Solid,
            lineColor={0,0,255}),
          Line(points={{-30,-50},{-30,50}}, color={0,0,0}),
          Line(points={{100,-30},{100,21}}, color={0,0,0}),
          Text(
            extent={{60,12},{96,-13}},
            lineColor={128,128,128},
            textString="b"),
          Text(
            extent={{-95,13},{-60,-9}},
            lineColor={128,128,128},
            textString="a"),
          Text(
            visible=useAxisFlange,
            extent={{-150,-135},{150,-95}},
            textString="%name",
            lineColor={0,0,255}),
          Text(
            extent={{-150,-90},{150,-60}},
            lineColor={0,0,0},
            textString="n=%n"),
          Rectangle(
            visible=useAxisFlange,
            extent={{90,30},{100,70}},
            pattern=LinePattern.None,
            fillColor={192,192,192},
            fillPattern=FillPattern.Solid,
            lineColor={0,0,255}),
          Text(
            visible=not useAxisFlange,
            extent={{-150,60},{150,100}},
            textString="%name",
            lineColor={0,0,255})}),
      Documentation(info="<HTML>
<p>
Joint where frame_b is translated along axis n which is fixed in frame_a.
The two frames coincide when the relative distance \"s = 0\".
</p>

<p>
Optionally, two additional 1-dimensional mechanical flanges
(flange \"axis\" represents the driving flange and
flange \"support\" represents the bearing) can be enabled via
parameter <b>useAxisFlange</b>. The enabled axis flange can be
driven with elements of the
<a href=\"modelica://Modelica.Mechanics.Translational\">Modelica.Mechanics.Translational</a>
library.

</p>

<p>
In the \"Advanced\" menu it can be defined via parameter <b>stateSelect</b>
that the relative distance \"s\" and its derivative shall be definitely
used as states by setting stateSelect=StateSelect.always.
Default is StateSelect.prefer to use the relative distance and its
derivative as preferred states. The states are usually selected automatically.
In certain situations, especially when closed kinematic loops are present,
it might be slightly more efficient, when using the StateSelect.always setting.
</p>

<p>
In the following figure the animation of a prismatic
joint is shown. The light blue coordinate system is
frame_a and the dark blue coordinate system is
frame_b of the joint. The black arrow is parameter
vector \"n\" defining the translation axis
(here: n = {1,1,0}).
</p>

<p>
<IMG src=\"modelica://Modelica/Resources/Images/Mechanics/MultiBody/Joints/Prismatic.png\">
</p>

</html>"));
  end Prismatic;

  model Fixed "Frame fixed in the world frame at a given position"
    import Modelica.Mechanics.MultiBody.Types;
    import Modelica.SIunits.Conversions.to_unit1;

    Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b
        "Coordinate system fixed in the world frame"
      annotation (Placement(transformation(extent={{84,-16},{116,16}}, rotation=
             0)));

    parameter Boolean animation=true "= true, if animation shall be enabled";
    parameter SI.Position r[3]={0,0,0}
        "Position vector from world frame to frame_b, resolved in world frame";
    parameter Modelica.Mechanics.MultiBody.Types.ShapeType shapeType="cylinder"
        "Type of shape"                                                                         annotation (
       Dialog(
        tab="Animation",
        group="if animation = true",
        enable=animation));
    parameter SI.Position r_shape[3]={0,0,0}
        "Vector from world frame to shape origin, resolved in world frame"
      annotation (Dialog(
        tab="Animation",
        group="if animation = true",
        enable=animation));
    parameter Modelica.Mechanics.MultiBody.Types.Axis lengthDirection = to_unit1(r - r_shape)
        "Vector in length direction of shape, resolved in world frame"
      annotation (Dialog(
        tab="Animation",
        group="if animation = true",
        enable=animation));
    parameter Modelica.Mechanics.MultiBody.Types.Axis widthDirection={0,1,0}
        "Vector in width direction of shape, resolved in world frame"
      annotation (Dialog(
        tab="Animation",
        group="if animation = true",
        enable=animation));
    parameter SI.Length length=Modelica.Math.Vectors.length(r - r_shape)
        "Length of shape"
                        annotation (Dialog(
        tab="Animation",
        group="if animation = true",
        enable=animation));
    parameter SI.Distance width=length/world.defaultWidthFraction
        "Width of shape"
                       annotation (Dialog(
        tab="Animation",
        group="if animation = true",
        enable=animation));
    parameter SI.Distance height=width "Height of shape" annotation (Dialog(
        tab="Animation",
        group="if animation = true",
        enable=animation));
    parameter Modelica.Mechanics.MultiBody.Types.ShapeExtra extra=0.0
        "Additional parameter for cone, pipe etc. (see docu of Visualizers.Advanced.Shape)"
      annotation (Dialog(
        tab="Animation",
        group="if animation = true",
        enable=animation));
    input Modelica.Mechanics.MultiBody.Types.Color color=Modelica.Mechanics.MultiBody.Types.Defaults.RodColor
        "Color of shape"
                       annotation (Dialog(
        colorSelector=true,
        tab="Animation",
        group="if animation = true",
        enable=animation));
    input Modelica.Mechanics.MultiBody.Types.SpecularCoefficient specularCoefficient=world.defaultSpecularCoefficient
        "Reflection of ambient light (= 0: light is completely absorbed)"
      annotation (Dialog(
        tab="Animation",
        group="if animation = true",
        enable=animation));

    protected
    outer Modelica.Mechanics.MultiBody.World world;
    Modelica.Mechanics.MultiBody.Visualizers.Advanced.Shape shape(
      shapeType=shapeType,
      color=color,
      specularCoefficient=specularCoefficient,
      length=length,
      width=width,
      height=height,
      lengthDirection=lengthDirection,
      widthDirection=widthDirection,
      extra=extra,
      r_shape=r_shape,
      r=zeros(3),
      R=Modelica.Mechanics.MultiBody.Frames.nullRotation()) if world.enableAnimation and animation;
  equation
    Connections.root(frame_b.R);
    frame_b.r_0 = r;
    frame_b.R = Modelica.Mechanics.MultiBody.Frames.nullRotation();
    annotation (
      Icon(coordinateSystem(
          preserveAspectRatio=true,
          extent={{-100,-100},{100,100}}), graphics={
          Rectangle(
            extent={{-90,90},{90,-90}},
            lineColor={255,255,255},
            fillColor={255,255,255},
            fillPattern=FillPattern.Solid),
          Text(
            extent={{150,145},{-150,105}},
            textString="%name",
            lineColor={0,0,255}),
          Line(points={{0,100},{0,-100}}, color={0,0,0}),
          Line(points={{0,-80},{-100,-20}}, color={0,0,0}),
          Line(points={{0,-40},{-100,20}}, color={0,0,0}),
          Line(points={{0,0},{-100,60}}, color={0,0,0}),
          Line(points={{0,40},{-100,100}}, color={0,0,0}),
          Line(points={{0,0},{100,0}}, color={0,0,0}),
          Text(
            extent={{-150,-105},{150,-135}},
            lineColor={0,0,0},
            textString="r=%r")}),
      Documentation(info="<html>
<p>
Element consisting of a frame (frame_b) that is fixed in the world
frame at a given position defined by parameter vector <b>r</b>
(vector from origin of world frame to frame_b, resolved in the
world frame).
</p>
<p>
By default, this component is visualized by a cylinder connecting the
world frame and frame_b of this components, as shown in the figure below.
Note, that the visualized world frame on the left side and
Fixed.frame_b on the right side are not part of the
component animation and that the animation may be switched off via parameter
animation = <b>false</b>.
</p>
<p>
<IMG src=\"modelica://Modelica/Resources/Images/Mechanics/MultiBody/Fixed.png\" ALT=\"Parts.Fixed\">
</p>

</html>"),
      Diagram(coordinateSystem(
          preserveAspectRatio=true,
          extent={{-100,-100},{100,100}}), graphics={
          Line(points={{0,100},{0,-100}}, color={0,0,0}),
          Line(points={{0,-80},{-100,-20}}, color={0,0,0}),
          Line(points={{0,-40},{-100,20}}, color={0,0,0}),
          Line(points={{0,0},{-100,60}}, color={0,0,0}),
          Line(points={{0,40},{-100,100}}, color={0,0,0}),
          Line(points={{0,0},{100,0}}, color={0,0,0})}));
  end Fixed;

  model ConstantTorque "Constant torque, not dependent on speed"
    extends Modelica.Mechanics.Rotational.Interfaces.PartialTorque;
    parameter Modelica.SIunits.Torque tau_constant
        "Constant torque (if negative, torque is acting as load)";
    Modelica.SIunits.Torque tau
        "Accelerating torque acting at flange (= -flange.tau)";
  equation
    tau = -flange.tau;
    tau = tau_constant;
    annotation (Icon(
        coordinateSystem(
          preserveAspectRatio=true,
          extent={{-100,-100},{100,100}}),
          graphics={
            Line(
              points={{-98,0},{100,0}},
              color={0,0,127}),
            Text(
              extent={{-124.0,-40.0},{120.0,-16.0}},
              textString="%tau_constant")}), Documentation(info="<HTML>
<p>
Model of constant torque, not dependent on angular velocity of flange.<br>
Positive torque acts accelerating.
</p>
</HTML>"));
  end ConstantTorque;

  connector FlangeWithBearing
      "Connector consisting of 1-dim. rotational flange and its bearing frame"
    parameter Boolean includeBearingConnector=false
        "= true, if bearing frame connector is present, otherwise not present";
    Modelica.Mechanics.Rotational.Interfaces.Flange_a flange
        "1-dim. rotational flange";
    Modelica.Mechanics.MultiBody.Interfaces.Frame bearingFrame if
      includeBearingConnector
        "3-dim. frame in which the 1-dim. shaft is mounted";

    annotation (
      defaultComponentName="flange",
      Icon(coordinateSystem(
          preserveAspectRatio=true,
          extent={{-100,-100},{100,100}}), graphics={
          Rectangle(
            extent={{-20,1},{20,-1}},
            lineColor={135,135,135},
            lineThickness=0.5),
          Rectangle(
            extent={{-100,100},{100,-100}},
            lineColor={255,255,255},
            fillColor={255,255,255},
            fillPattern=FillPattern.Solid),
          Rectangle(
            extent={{-100,25},{100,-25}},
            lineColor={64,64,64},
            fillPattern=FillPattern.HorizontalCylinder,
            fillColor={192,192,192}),
          Line(points={{-80,60},{80,60}}, color={0,0,0}),
          Line(points={{-80,-60},{80,-60}}, color={0,0,0}),
          Line(points={{0,100},{0,60}}, color={0,0,0}),
          Line(points={{0,-60},{0,-100}}, color={0,0,0}),
          Rectangle(extent={{-100,100},{100,-100}}, lineColor={135,135,135}),
          Rectangle(extent={{-100,25},{100,-25}}, lineColor={64,64,64})}),
      Diagram(coordinateSystem(
          preserveAspectRatio=true,
          extent={{-100,-100},{100,100}}), graphics={
          Line(
            points={{-50,-40},{50,-40}},
            color={0,0,0},
            thickness=0.5),
          Line(
            points={{-50,40},{50,40}},
            color={0,0,0},
            thickness=0.5),
          Text(
            extent={{-158,-66},{158,-124}},
            lineColor={0,0,0},
            lineThickness=0.5,
            textString="%name"),
          Rectangle(
            extent={{-60,60},{60,-60}},
            lineColor={255,255,255},
            lineThickness=0.5,
            fillColor={255,255,255},
            fillPattern=FillPattern.Solid),
          Rectangle(
            extent={{-60,15},{60,-15}},
            lineColor={64,64,64},
            fillPattern=FillPattern.HorizontalCylinder,
            fillColor={192,192,192}),
          Line(points={{0,60},{0,40}}, color={0,0,0}),
          Line(points={{0,-40},{0,-60}}, color={0,0,0}),
          Line(points={{-50,40},{50,40}}, color={0,0,0}),
          Line(points={{-50,-40},{50,-40}}, color={0,0,0}),
          Rectangle(extent={{-60,60},{60,-60}}, lineColor={135,135,135}),
          Rectangle(extent={{-60,15},{60,-15}}, lineColor={64,64,64})}),
      Documentation(info="<html>
<p>
This hierarchical connector models a 1-dim. rotational flange
connector and its optional bearing defined by a 3-dim. frame connector.
If a connection to the subconnectors should be clearly visible,
connect first an  instance of
<a href=\"modelica://Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearingAdaptor\">FlangeWithBearingAdaptor</a>
to the FlangeWithBearing connector.
</p>
</html>"));

  end FlangeWithBearing;
  end Components;
  annotation (uses(Modelica(version="3.2.1")));
  model TorqueSourceFlangeWithBearing
    parameter Real torque = 1;

    Modelica.Mechanics.Rotational.Sources.Torque torque2
      annotation (Placement(transformation(extent={{-38,-8},{-18,12}})));
    Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing flange(
        includeBearingConnector=true)
      annotation (Placement(transformation(extent={{90,-12},{110,8}}),
          iconTransformation(extent={{110,-12},{130,8}})));
  protected
    Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a
      annotation (Placement(transformation(extent={{56,-60},{88,-28}})));
  public
    Modelica.Blocks.Sources.Constant const(k = torque)
      annotation (Placement(transformation(extent={{-78,-10},{-58,10}})));
  equation
    connect(torque2.flange,flange. flange) annotation (Line(
        points={{-18,2},{42,2},{42,-2},{100,-2}},
        color={0,0,0},
        smooth=Smooth.None));
    connect(flange.bearingFrame,frame_a)  annotation (Line(
        points={{100,-2},{78,-2},{78,-44},{72,-44}},
        color={135,135,135},
        thickness=0.5,
        smooth=Smooth.None));
    connect(const.y, torque2.tau) annotation (Line(
        points={{-57,0},{-48,0},{-48,2},{-40,2}},
        color={0,0,127},
        smooth=Smooth.None));
    annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
              -100},{100,100}}), graphics));
  end TorqueSourceFlangeWithBearing;
end WaterjetCyPy;
