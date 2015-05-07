within ;
package driveLine_2
  model driveLine_2
    type Requirement = enumeration(
        success,
        unknown,
        violated);
    Requirement speedReq(start = Requirement.unknown, fixed = true);
    Requirement backRollReq(start = Requirement.unknown, fixed = true);
    Requirement clutchLockedReq(start = Requirement.unknown, fixed = true);// for clutch.cgeo>=0.477 the requirement is satisfied for the "nominal" values of the parameters
    Requirement brakingDistanceReq(start = Requirement.unknown, fixed = true); // for brake.cgeo>=0.482 the requirement is satisfied for the "nominal" values of the parameters

    Modelica.Mechanics.Rotational.Components.Inertia inertia(phi(start = 0, fixed = true, displayUnit = "deg"), w(start = 0, fixed = true, displayUnit = "rad/s"), J = 2.8) annotation(Placement(transformation(extent = {{-200,-10},{-180,10}}, rotation = 0)));
    Modelica.Mechanics.Rotational.Sources.TorqueStep torqueStep(offsetTorque = 0, useSupport = false, startTime = 0.5, stepTorque = 450) annotation(Placement(transformation(extent = {{-234,-10},{-214,10}})));
    Modelica.Mechanics.Rotational.Components.Clutch clutch(fn_max = 2000, cgeo = 0.5) annotation(Placement(transformation(extent = {{-158,-10},{-138,10}})));
    Modelica.Mechanics.Rotational.Components.IdealGear idealGear(ratio = 1.4) annotation(Placement(transformation(extent = {{-108,-10},{-88,10}})));
    Modelica.Mechanics.Rotational.Components.Inertia inertia2(phi(start = 0, fixed = true), w(start = 0, fixed = true), J = 0.1) annotation(Placement(transformation(extent = {{-66,-10},{-46,10}}, rotation = 0)));
    Modelica.Mechanics.Rotational.Components.SpringDamper spring(d = 10 ^ 4, c = 500) annotation(Placement(transformation(extent = {{-16,-10},{4,10}}, rotation = 0)));
    Modelica.Blocks.Sources.Ramp ramp1(duration = 2, startTime = 4) annotation(Placement(transformation(extent = {{-222,56},{-202,76}})));
    Road_Wheel_Load_Both_Sides road_Wheel_Load_Both_Sides(vehicle_mass = 3000, grade = 2,brake.cgeo=0.5) annotation(Placement(transformation(extent = {{36,-10},{56,10}})));
    Modelica.Blocks.Sources.Ramp ramp2(              startTime = 70, height = 1,
      duration=5)                                                                annotation(Placement(transformation(extent = {{-2,70},{18,90}})));

  //  Real T_forward;
  //  Real T_stuck(start=1e10);
  protected
    Real T(start=1e10);

  equation
  //  der(T_forward)=0;
  //  der(T_stuck)=0;

  //  when road_Wheel_Load_Both_Sides.brake.mode == road_Wheel_Load_Both_Sides.brake.Forward then
  //    reinit(T_forward,time);
  //  end when;
  //    when road_Wheel_Load_Both_Sides.brake.mode == road_Wheel_Load_Both_Sides.brake.Stuck then
  //    reinit(T_stuck,time);
  //  end when;

    if road_Wheel_Load_Both_Sides.brake.mode == road_Wheel_Load_Both_Sides.brake.Stuck then
      T = time;
    else
      T=1e10;
    end if;

  //  if pre(brakingDistanceReq) == Requirement.success or (T_stuck-T_forward)<=2  then
  //    brakingDistanceReq = Requirement.success;
  //  else
  //    brakingDistanceReq = Requirement.unknown;
  //  end if;

    if pre(brakingDistanceReq) == Requirement.success or (T-ramp2.startTime)<=2 then
      brakingDistanceReq = Requirement.success;
    else
      brakingDistanceReq = Requirement.unknown;
    end if;

    if pre(speedReq) == Requirement.success or road_Wheel_Load_Both_Sides.vehicle_speed >= 30 and time <= 14 then
      speedReq = Requirement.success;
    else if time > 14 then
	   speedReq = Requirement.violated;
	   else
	   speedReq = Requirement.unknown;
	 end if;
       end if;
// 1 and 1/3 ft
    if pre(backRollReq) == Requirement.violated or road_Wheel_Load_Both_Sides.mass.s < (-0.41) then
      backRollReq = Requirement.violated;
    else
      backRollReq = Requirement.unknown;
    end if;

    if pre(clutchLockedReq) == Requirement.success or  clutch.mode==clutch.Stuck then
      clutchLockedReq = Requirement.success;
    else
      clutchLockedReq = Requirement.unknown;
    end if;

    connect(torqueStep.flange,inertia.flange_a) annotation(Line(points = {{-214,0},{-200,0}}, color = {0,0,0}, smooth = Smooth.None));
    connect(clutch.flange_b,idealGear.flange_a) annotation(Line(points = {{-138,0},{-108,0}}, color = {0,0,0}, smooth = Smooth.None));
    connect(inertia.flange_b,clutch.flange_a) annotation(Line(points = {{-180,0},{-158,0}}, color = {0,0,0}, smooth = Smooth.None));
    connect(inertia2.flange_b,spring.flange_a) annotation(Line(points = {{-46,0},{-16,0}}, color = {0,0,0}, smooth = Smooth.None));
    connect(idealGear.flange_b,inertia2.flange_a) annotation(Line(points = {{-88,0},{-66,0}}, color = {0,0,0}, smooth = Smooth.None));
    connect(ramp1.y,clutch.f_normalized) annotation(Line(points = {{-201,66},{-148,66},{-148,11}}, color = {0,0,127}, smooth = Smooth.None));
    connect(spring.flange_b,road_Wheel_Load_Both_Sides.rhs_flange) annotation(Line(points = {{4,0},{36,0}}, color = {0,0,0}, smooth = Smooth.None));
    connect(ramp2.y,road_Wheel_Load_Both_Sides.brake_command) annotation(Line(points = {{19,80},{32,80},{32,64},{41.3,64},{41.3,10.3}}, color = {0,0,127}, smooth = Smooth.None));
    annotation(Diagram(coordinateSystem(extent = {{-240,-120},{240,140}}, preserveAspectRatio = false), graphics), Icon(coordinateSystem(extent = {{-240,-120},{240,140}})), uses(Modelica(version = "3.2")));
  end driveLine_2;

  model driveLine_2_QR
    type Requirement = enumeration(
        success,
        unknown,
        violated);
    Requirement speedReq(start = Requirement.unknown, fixed = true);
    Requirement backRollReq(start = Requirement.unknown, fixed = true);
    Requirement clutchLockedReq(start = Requirement.unknown, fixed = true);// for clutch.cgeo>=0.477 the requirement is satisfied for the "nominal" values of the parameters
    Requirement brakingDistanceReq(start = Requirement.unknown, fixed = true); // for brake.cgeo>=0.482 the requirement is satisfied for the "nominal" values of the parameters

    Modelica.Mechanics.Rotational.Components.Inertia inertia(phi(start = 0, fixed = true, displayUnit = "deg"), w(start = 0, fixed = true, displayUnit = "rad/s"), J = 2.8) annotation(Placement(transformation(extent = {{-200,-10},{-180,10}}, rotation = 0)));
    Modelica.Mechanics.Rotational.Sources.TorqueStep torqueStep(offsetTorque = 0, useSupport = false, startTime = 0.5, stepTorque = 450) annotation(Placement(transformation(extent = {{-234,-10},{-214,10}})));
    Modelica.Mechanics.Rotational.Components.Clutch clutch(fn_max = 2000, cgeo = 0.5) annotation(Placement(transformation(extent = {{-158,-10},{-138,10}})));
    Modelica.Mechanics.Rotational.Components.IdealGear idealGear(ratio = 1.4) annotation(Placement(transformation(extent = {{-108,-10},{-88,10}})));
    Modelica.Mechanics.Rotational.Components.Inertia inertia2(phi(start = 0, fixed = true), w(start = 0, fixed = true), J = 0.1) annotation(Placement(transformation(extent = {{-66,-10},{-46,10}}, rotation = 0)));
    Modelica.Mechanics.Rotational.Components.SpringDamper spring(d = 10 ^ 4, c = 500) annotation(Placement(transformation(extent = {{-16,-10},{4,10}}, rotation = 0)));
    Modelica.Blocks.Sources.Ramp ramp1(duration = 2, startTime = 4) annotation(Placement(transformation(extent = {{-222,56},{-202,76}})));
    Road_Wheel_Load_Both_Sides road_Wheel_Load_Both_Sides(vehicle_mass = 3000, grade = 2,brake.cgeo=0.5) annotation(Placement(transformation(extent = {{36,-10},{56,10}})));
    Modelica.Blocks.Sources.Ramp ramp2(              startTime = 70, height = 1,
      duration=5)                                                                annotation(Placement(transformation(extent = {{-2,70},{18,90}})));


  equation
  //  der(T_forward)=0;
  //  der(T_stuck)=0;

  //  when road_Wheel_Load_Both_Sides.brake.mode == road_Wheel_Load_Both_Sides.brake.Forward then
  //    reinit(T_forward,time);
  //  end when;
  //    when road_Wheel_Load_Both_Sides.brake.mode == road_Wheel_Load_Both_Sides.brake.Stuck then
  //    reinit(T_stuck,time);
  //  end when;

  //  if pre(brakingDistanceReq) == Requirement.success or (T_stuck-T_forward)<=2  then
  //    brakingDistanceReq = Requirement.success;
  //  else
  //    brakingDistanceReq = Requirement.unknown;
  //  end if;

    if pre(brakingDistanceReq) == Requirement.success or 
      (time <= (ramp2.startTime + 2) 
       and road_Wheel_Load_Both_Sides.brake.mode == road_Wheel_Load_Both_Sides.brake.Stuck) then
      brakingDistanceReq = Requirement.success;
    else
      brakingDistanceReq = Requirement.unknown;
    end if;

    if pre(speedReq) == Requirement.success or road_Wheel_Load_Both_Sides.vehicle_speed >= 30 and time <= 14 then
      speedReq = Requirement.success;
    else if time > 14 then
	   speedReq = Requirement.violated;
	   else
	   speedReq = Requirement.unknown;
	 end if;
       end if;
// 1 and 1/3 ft
    if pre(backRollReq) == Requirement.violated or road_Wheel_Load_Both_Sides.mass.s < (-0.41) then
      backRollReq = Requirement.violated;
    else
      backRollReq = Requirement.unknown;
    end if;

    if pre(clutchLockedReq) == Requirement.success or clutch.mode==clutch.Stuck then
      clutchLockedReq = Requirement.success;
    else
      clutchLockedReq = Requirement.unknown;
    end if;

    connect(torqueStep.flange,inertia.flange_a) annotation(Line(points = {{-214,0},{-200,0}}, color = {0,0,0}, smooth = Smooth.None));
    connect(clutch.flange_b,idealGear.flange_a) annotation(Line(points = {{-138,0},{-108,0}}, color = {0,0,0}, smooth = Smooth.None));
    connect(inertia.flange_b,clutch.flange_a) annotation(Line(points = {{-180,0},{-158,0}}, color = {0,0,0}, smooth = Smooth.None));
    connect(inertia2.flange_b,spring.flange_a) annotation(Line(points = {{-46,0},{-16,0}}, color = {0,0,0}, smooth = Smooth.None));
    connect(idealGear.flange_b,inertia2.flange_a) annotation(Line(points = {{-88,0},{-66,0}}, color = {0,0,0}, smooth = Smooth.None));
    connect(ramp1.y,clutch.f_normalized) annotation(Line(points = {{-201,66},{-148,66},{-148,11}}, color = {0,0,127}, smooth = Smooth.None));
    connect(spring.flange_b,road_Wheel_Load_Both_Sides.rhs_flange) annotation(Line(points = {{4,0},{36,0}}, color = {0,0,0}, smooth = Smooth.None));
    connect(ramp2.y,road_Wheel_Load_Both_Sides.brake_command) annotation(Line(points = {{19,80},{32,80},{32,64},{41.3,64},{41.3,10.3}}, color = {0,0,127}, smooth = Smooth.None));
    annotation(Diagram(coordinateSystem(extent = {{-240,-120},{240,140}}, preserveAspectRatio = false), graphics), Icon(coordinateSystem(extent = {{-240,-120},{240,140}})), uses(Modelica(version = "3.2")));
  end driveLine_2_QR;

  model Road_Wheel_Load_Both_Sides
    "Simulates both road wheels connected to a mass simulating a vehicle"
    import SI = Modelica.SIunits;
    Modelica.Mechanics.Rotational.Components.IdealRollingWheel idealRollingWheel(radius = radius, useSupportT = false) annotation(Placement(transformation(extent = {{-32,-10},{-12,10}})));
    SI.Conversions.NonSIunits.Velocity_kmh vehicleSpeed
      "promote velocity to the top level";
    parameter Real Crr = 0.01 "Coefficient of rolling resistance";
    parameter SI.Area frontal_area = 16 "Frontal area of vehicle";
    parameter SI.Density air_density = 1.16 "Nominal air density";
    parameter Real Cd = 1.25
      "Vehicle drag factor (Cd) in F = 0.5 * Cd * rho * v^2 * A";
    parameter SI.Distance radius = 0.2
      "Wheel radius used to convert rotational to translational motion";
    parameter SI.Mass vehicle_mass = 30000 "Mass of the vehicle";
    parameter SI.Force static_resistance = vehicle_mass * 9.81 * Crr
      "Constant force required to move vehicle";
    parameter SI.TranslationalDampingConstant linear_resistance = 0
      "Linear resistance coefficient (d) in f = d * vel";
    parameter Real quad_resistance = 0.5 * Cd * air_density * frontal_area
      "Quadratic resistance coeff (dq) in f = dq * v * |v|";
    parameter Real grade = 0 "% grade (100% = 45degree angle)";
    Modelica.Mechanics.Translational.Components.Mass mass(m = vehicle_mass, L = 0, v(fixed = true), stateSelect = StateSelect.always) annotation(Placement(transformation(extent = {{62,-10},{82,10}})));
    Modelica.Mechanics.Translational.Components.SupportFriction supportFriction(useSupport = true, f_pos = [0,static_resistance], peak = 1, locked(fixed = true, start = true)) annotation(Placement(transformation(extent = {{28,-10},{48,10}})));
    Quadratic_Damper quadratic_damper(d = linear_resistance, dq = quad_resistance) annotation(Placement(transformation(extent = {{-10,-10},{10,10}}, rotation = 270, origin = {54,-20})));
    Modelica.Mechanics.Rotational.Interfaces.Flange_a rhs_flange annotation(Placement(transformation(extent = {{-110,-10},{-90,10}})));
    Modelica.Mechanics.Translational.Components.Fixed fixed annotation(Placement(transformation(extent = {{28,-50},{48,-30}})));
    Modelica.Mechanics.Translational.Sensors.ForceSensor forceSensor annotation(Placement(transformation(extent = {{0,-10},{20,10}})));
    Modelica.Mechanics.Translational.Sources.Force force1 annotation(Placement(transformation(extent = {{62,18},{82,38}})));
    Modelica.Blocks.Sources.RealExpression realExpression(y = -vehicle_mass * 9.81 * grade / 100 * sin(Modelica.Constants.pi / 4)) annotation(Placement(transformation(extent = {{32,18},{52,38}})));
    Modelica.Mechanics.Rotational.Components.Brake brake(fn_max = 100000, locked(fixed = true, start = true)) annotation(Placement(transformation(extent = {{-60,-10},{-40,10}})));
    Modelica.Mechanics.Rotational.Components.Inertia rotational_inertia(J = 1) annotation(Placement(transformation(extent = {{-90,-10},{-70,10}})));
    Modelica.Mechanics.Translational.Sensors.SpeedSensor speedSensor annotation(Placement(transformation(extent = {{4,50},{24,70}})));
    Modelica.Blocks.Interfaces.RealOutput vehicle_speed annotation(Placement(transformation(extent = {{-10,-10},{10,10}}, rotation = 90, origin = {30,106})));
    Modelica.Blocks.Interfaces.RealInput brake_command annotation(Placement(transformation(extent = {{-11,-11},{11,11}}, rotation = 270, origin = {-47,103})));
  equation
    vehicleSpeed = SI.Conversions.to_kmh(mass.v);
    connect(idealRollingWheel.flangeT,forceSensor.flange_a) annotation(Line(points = {{-12,0},{0,0}}, color = {0,127,0}, smooth = Smooth.None));
    connect(forceSensor.flange_b,supportFriction.flange_a) annotation(Line(points = {{20,0},{28,0}}, color = {0,127,0}, smooth = Smooth.None));
    connect(supportFriction.flange_b,quadratic_damper.flange_a) annotation(Line(points = {{48,0},{54,0},{54,-10}}, color = {0,127,0}, smooth = Smooth.None));
    connect(supportFriction.flange_b,mass.flange_a) annotation(Line(points = {{48,0},{62,0}}, color = {0,127,0}, smooth = Smooth.None));
    connect(supportFriction.support,fixed.flange) annotation(Line(points = {{38,-10},{38,-40}}, color = {0,0,0}, pattern = LinePattern.None, smooth = Smooth.None));
    connect(fixed.flange,quadratic_damper.flange_b) annotation(Line(points = {{38,-40},{54,-40},{54,-30}}, color = {0,127,0}, smooth = Smooth.None));
    connect(force1.flange,mass.flange_b) annotation(Line(points = {{82,28},{92,28},{92,0},{82,0}}, color = {0,127,0}, smooth = Smooth.None));
    connect(realExpression.y,force1.f) annotation(Line(points = {{53,28},{60,28}}, color = {0,0,127}, smooth = Smooth.None));
    connect(brake.flange_b,idealRollingWheel.flangeR) annotation(Line(points = {{-40,0},{-32,0}}, color = {0,0,0}, smooth = Smooth.None));
    connect(rotational_inertia.flange_b,brake.flange_a) annotation(Line(points = {{-70,0},{-60,0}}, color = {0,0,0}, smooth = Smooth.None));
    connect(idealRollingWheel.flangeT,speedSensor.flange) annotation(Line(points = {{-12,0},{-8,0},{-8,60},{4,60}}, color = {0,127,0}, smooth = Smooth.None));
    connect(rotational_inertia.flange_a,rhs_flange) annotation(Line(points = {{-90,0},{-100,0}}, color = {0,0,0}, smooth = Smooth.None));
    vehicle_speed = Modelica.SIunits.Conversions.to_kmh(speedSensor.v);
    // connect(speedSensor.v, vehicle_speed) annotation (Line(
    //     points={{25,60},{30,60},{30,106}},
    //     color={0,0,127},
    //      smooth=Smooth.None));
    connect(brake.f_normalized,brake_command) annotation(Line(points = {{-50,11},{-50,103},{-47,103}}, color = {0,0,127}, smooth = Smooth.None));
    annotation(Diagram(coordinateSystem(preserveAspectRatio = false, extent = {{-100,-100},{100,100}}), graphics), Icon(graphics={  Ellipse(extent = {{-40,-40},{-20,-60}}, lineColor = {0,0,255}, fillColor = {95,95,95},
              fillPattern =                                                                                                    FillPattern.Solid),Ellipse(extent = {{40,-40},{60,-60}}, lineColor = {0,0,255}, fillColor = {95,95,95},
              fillPattern =                                                                                                    FillPattern.Solid),Rectangle(extent = {{64,-38},{-12,-4}}, lineColor = {0,0,255},
              fillPattern =                                                                                                    FillPattern.Solid, fillColor = {175,175,175}),Ellipse(extent = {{-16,-40},{4,-60}}, lineColor = {0,0,255}, fillColor = {95,95,95},
              fillPattern =                                                                                                    FillPattern.Solid),Polygon(points = {{-20,-38},{-44,-38},{-44,-20},{-34,-4},{-20,-4},{-20,-38}}, lineColor = {0,0,255}, smooth = Smooth.None,
              fillPattern =                                                                                                    FillPattern.Solid, fillColor = {175,175,175}),Polygon(points = {{-36,-22},{-32,-8},{-26,-8},{-26,-22},{-36,-22}}, lineColor = {0,0,255}, smooth = Smooth.None, fillColor = {255,255,255},
              fillPattern =                                                                                                    FillPattern.Solid),Text(extent = {{-64,88},{68,8}}, lineColor = {0,0,0}, fillColor = {175,175,175},
              fillPattern =                                                                                                    FillPattern.Solid, textString = "Road
Load"),Rectangle(extent = {{-100,100},{100,-100}}, lineColor = {0,0,0})}));
  end Road_Wheel_Load_Both_Sides;

  model Quadratic_Damper
    "1D translational damper with a linear resistance (d*v) and quadratic (dq * v * |v|)"
    import SI = Modelica.SIunits;
    extends
      Modelica.Mechanics.Translational.Interfaces.PartialCompliantWithRelativeStates;
    parameter SI.TranslationalDampingConstant d(final min = 0, start = 0)
      "Damping constant";
    parameter Real dq(final min = 0, start = 0, unit = "N.s2/m2")
      "Quadratic damping constant (d) in f = d * v * |v|";
  equation
    f = d * v_rel + dq * v_rel * abs(v_rel);
    annotation(Documentation(info = "<html>
<p>
<i>Linear, velocity dependent damper</i> element. It can be either connected
between a sliding mass and the housing (model Fixed), or
between two sliding masses.
</p>

</HTML>
"), Icon(coordinateSystem(preserveAspectRatio = true, extent = {{-100,-100},{100,100}}, grid = {2,2}), graphics={  Line(points = {{-90,0},{-60,0}}, color = {0,0,0}),Line(points = {{-60,-30},{-60,30}}, color = {0,0,0}),Line(points = {{-60,-30},{60,-30}}, color = {0,0,0}),Line(points = {{-60,30},{60,30}}, color = {0,0,0}),Rectangle(extent = {{-60,30},{30,-30}}, lineColor = {0,0,0}, fillColor = {192,192,192},
              fillPattern =                                                                                                    FillPattern.Solid),Line(points = {{30,0},{90,0}}, color = {0,0,0}),Polygon(points = {{50,-90},{20,-80},{20,-100},{50,-90}}, lineColor = {128,128,128}, fillColor = {128,128,128},
              fillPattern =                                                                                                    FillPattern.Solid),Line(points = {{-60,-90},{20,-90}}, color = {0,0,0}),Text(extent = {{-150,90},{150,50}}, textString = "%name", lineColor = {0,0,255}),Text(extent = {{-150,-45},{150,-75}}, lineColor = {0,0,0}, textString = "d=%d"),Line(visible = useHeatPort, points = {{-100,-100},{-100,-20},{-14,-20}}, color = {191,0,0}, pattern = LinePattern.Dot, smooth = Smooth.None)}), Diagram(coordinateSystem(preserveAspectRatio = true, extent = {{-100,-100},{100,100}}, grid = {2,2}), graphics = {Line(points=  {{-90,0},{-60,0}}, color=  {0,0,0}),Line(points=  {{-60,-30},{-60,30}}, color=  {0,0,0}),Line(points=  {{-60,-30},{60,-30}}, color=  {0,0,0}),Line(points=  {{-60,30},{60,30}}, color=  {0,0,0}),Rectangle(extent=  {{-60,30},{30,-30}}, lineColor=  {0,0,0}, fillColor=  {192,192,192}, fillPattern=  FillPattern.Solid),Line(points=  {{30,0},{90,0}}, color=  {0,0,0}),Line(points=  {{-50,60},{50,60}}, color=  {128,128,128}),Polygon(points=  {{50,63},{60,60},{50,57},{50,63}}, lineColor=  {128,128,128}, fillColor=  {128,128,128}, fillPattern=  FillPattern.Solid),Text(extent=  {{-58,68},{42,78}}, lineColor=  {128,128,128}, textString=  "der(s_rel)")}));
  end Quadratic_Damper;

  model driveLine_2_initial
    Modelica.Mechanics.Rotational.Components.Inertia inertia2(phi(start = 0, fixed = true), w(start = 0, fixed = true), J = 0.1) annotation(Placement(transformation(extent = {{-52,-30},{-32,-10}}, rotation = 0)));
    Modelica.Mechanics.Rotational.Components.SpringDamper spring(d = 10 ^ 4, c = 500) annotation(Placement(transformation(extent = {{-4,-30},{16,-10}}, rotation = 0)));
    Road_Wheel_Load_Both_Sides_initial road_Wheel_Load_Both_Sides_initial(grade = 2) annotation(Placement(transformation(extent = {{30,-30},{50,-10}})));
  equation
    connect(inertia2.flange_b,spring.flange_a) annotation(Line(points = {{-32,-20},{-4,-20}}, color = {0,0,0}, smooth = Smooth.None));
    connect(spring.flange_b,road_Wheel_Load_Both_Sides_initial.rhs_flange) annotation(Line(points = {{16,-20},{30,-20}}, color = {0,0,0}, smooth = Smooth.None));
    annotation(Diagram(coordinateSystem(preserveAspectRatio = false, extent = {{-100,-100},{100,100}}), graphics));
  end driveLine_2_initial;

  model Road_Wheel_Load_Both_Sides_initial
    "Simulates both road wheels connected to a mass simulating a vehicle"
    import SI = Modelica.SIunits;
    Modelica.Mechanics.Rotational.Components.IdealRollingWheel idealRollingWheel(radius = radius, useSupportT = false) annotation(Placement(transformation(extent = {{-44,-10},{-24,10}})));
    SI.Conversions.NonSIunits.Velocity_kmh vehicleSpeed
      "promote velocity to the top level";
    parameter Real Crr = 0.01 "Coefficient of rolling resistance";
    parameter SI.Area frontal_area = 16 "Frontal area of vehicle";
    parameter SI.Density air_density = 1.16 "Nominal air density";
    parameter Real Cd = 1.25
      "Vehicle drag factor (Cd) in F = 0.5 * Cd * rho * v^2 * A";
    parameter SI.Distance radius = 0.2
      "Wheel radius used to convert rotational to translational motion";
    parameter SI.Mass vehicle_mass = 30000 "Mass of the vehicle";
    parameter SI.Force static_resistance = vehicle_mass * 9.81 * Crr
      "Constant force required to move vehicle";
    parameter SI.TranslationalDampingConstant linear_resistance = 0
      "Linear resistance coefficient (d) in f = d * vel";
    parameter Real quad_resistance = 0.5 * Cd * air_density * frontal_area
      "Quadratic resistance coeff (dq) in f = dq * v * |v|";
    parameter Real grade = 0 "% grade (100% = 45degree angle)";
    Modelica.Mechanics.Translational.Components.Mass mass(m = vehicle_mass, L = 0, v(fixed = true), stateSelect = StateSelect.always) annotation(Placement(transformation(extent = {{62,-10},{82,10}})));
    Modelica.Mechanics.Translational.Components.SupportFriction supportFriction(useSupport = true, f_pos = [0,static_resistance], peak = 1, locked(fixed = true, start = true)) annotation(Placement(transformation(extent = {{28,-10},{48,10}})));
    Modelica.Mechanics.Rotational.Interfaces.Flange_a rhs_flange annotation(Placement(transformation(extent = {{-110,-10},{-90,10}})));
    Modelica.Mechanics.Translational.Components.Fixed fixed annotation(Placement(transformation(extent = {{28,-50},{48,-30}})));
    Modelica.Mechanics.Translational.Sensors.ForceSensor forceSensor annotation(Placement(transformation(extent = {{0,-10},{20,10}})));
    Modelica.Mechanics.Translational.Sources.Force force1 annotation(Placement(transformation(extent = {{62,18},{82,38}})));
    Modelica.Blocks.Sources.RealExpression realExpression(y = -vehicle_mass * 9.81 * grade / 100 * sin(Modelica.Constants.pi / 4)) annotation(Placement(transformation(extent = {{32,18},{52,38}})));
    Modelica.Mechanics.Rotational.Components.Inertia rotational_inertia(J = 1) annotation(Placement(transformation(extent = {{-90,-10},{-70,10}})));
    Modelica.Mechanics.Translational.Sensors.SpeedSensor speedSensor annotation(Placement(transformation(extent = {{4,50},{24,70}})));
    Modelica.Blocks.Interfaces.RealOutput vehicle_speed annotation(Placement(transformation(extent = {{-10,-10},{10,10}}, rotation = 90, origin = {30,106})));
  equation
    vehicleSpeed = SI.Conversions.to_kmh(mass.v);
    connect(idealRollingWheel.flangeT,forceSensor.flange_a) annotation(Line(points = {{-24,0},{0,0}}, color = {0,127,0}, smooth = Smooth.None));
    connect(forceSensor.flange_b,supportFriction.flange_a) annotation(Line(points = {{20,0},{28,0}}, color = {0,127,0}, smooth = Smooth.None));
    connect(supportFriction.flange_b,mass.flange_a) annotation(Line(points = {{48,0},{62,0}}, color = {0,127,0}, smooth = Smooth.None));
    connect(supportFriction.support,fixed.flange) annotation(Line(points = {{38,-10},{38,-40}}, color = {0,0,0}, pattern = LinePattern.None, smooth = Smooth.None));
    connect(force1.flange,mass.flange_b) annotation(Line(points = {{82,28},{92,28},{92,0},{82,0}}, color = {0,127,0}, smooth = Smooth.None));
    connect(realExpression.y,force1.f) annotation(Line(points = {{53,28},{60,28}}, color = {0,0,127}, smooth = Smooth.None));
    connect(idealRollingWheel.flangeT,speedSensor.flange) annotation(Line(points = {{-24,0},{-8,0},{-8,60},{4,60}}, color = {0,127,0}, smooth = Smooth.None));
    connect(rotational_inertia.flange_a,rhs_flange) annotation(Line(points = {{-90,0},{-100,0}}, color = {0,0,0}, smooth = Smooth.None));
    vehicle_speed = Modelica.SIunits.Conversions.to_kmh(speedSensor.v);
    // connect(speedSensor.v, vehicle_speed) annotation (Line(
    //     points={{25,60},{30,60},{30,106}},
    //     color={0,0,127},
    //      smooth=Smooth.None));
    connect(rotational_inertia.flange_b,idealRollingWheel.flangeR) annotation(Line(points = {{-70,0},{-44,0}}, color = {0,0,0}, smooth = Smooth.None));
    annotation(Diagram(coordinateSystem(preserveAspectRatio = false, extent = {{-100,-100},{100,100}}), graphics), Icon(graphics={  Ellipse(extent = {{-40,-40},{-20,-60}}, lineColor = {0,0,255}, fillColor = {95,95,95},
              fillPattern =                                                                                                    FillPattern.Solid),Ellipse(extent = {{40,-40},{60,-60}}, lineColor = {0,0,255}, fillColor = {95,95,95},
              fillPattern =                                                                                                    FillPattern.Solid),Rectangle(extent = {{64,-38},{-12,-4}}, lineColor = {0,0,255},
              fillPattern =                                                                                                    FillPattern.Solid, fillColor = {175,175,175}),Ellipse(extent = {{-16,-40},{4,-60}}, lineColor = {0,0,255}, fillColor = {95,95,95},
              fillPattern =                                                                                                    FillPattern.Solid),Polygon(points = {{-20,-38},{-44,-38},{-44,-20},{-34,-4},{-20,-4},{-20,-38}}, lineColor = {0,0,255}, smooth = Smooth.None,
              fillPattern =                                                                                                    FillPattern.Solid, fillColor = {175,175,175}),Polygon(points = {{-36,-22},{-32,-8},{-26,-8},{-26,-22},{-36,-22}}, lineColor = {0,0,255}, smooth = Smooth.None, fillColor = {255,255,255},
              fillPattern =                                                                                                    FillPattern.Solid),Text(extent = {{-64,88},{68,8}}, lineColor = {0,0,0}, fillColor = {175,175,175},
              fillPattern =                                                                                                    FillPattern.Solid, textString = "Road
Load"),Rectangle(extent = {{-100,100},{100,-100}}, lineColor = {0,0,0})}));
  end Road_Wheel_Load_Both_Sides_initial;

  model simplifiedEquations
    import SI = Modelica.SIunits;
    parameter Real Crr = 0.01 "Coefficient of rolling resistance";
    parameter SI.Area frontal_area = 16 "Frontal area of vehicle";
    parameter SI.Density air_density = 1.16 "Nominal air density";
    parameter Real Cd = 1.25
      "Vehicle drag factor (Cd) in F = 0.5 * Cd * rho * v^2 * A";
    parameter SI.Distance radius = 0.2
      "Wheel radius used to convert rotational to translational motion";
    parameter SI.Mass vehicle_mass = 30000 "Mass of the vehicle";
    parameter SI.Force static_resistance = vehicle_mass * 9.81 * Crr
      "Constant force required to move vehicle";
    parameter SI.TranslationalDampingConstant linear_resistance = 0
      "Linear resistance coefficient (d) in f = d * vel";
    parameter Real quad_resistance = 0.5 * Cd * air_density * frontal_area
      "Quadratic resistance coeff (dq) in f = dq * v * |v|";
    parameter Real grade = 2 "% grade (100% = 45degree angle)";
    parameter Real d = 10 ^ 4;
    parameter Real c = 500;
    parameter Real J1 = 0.1;
    parameter Real J2 = 1;
    Real x;
    Real theta_a;
    Real theta_b;
    Real v = der(x);
    Real omega_a = der(theta_a);
    Real omega_b = der(theta_b);
    Real a = der(v);
    Real tau_b;
    Real F = vehicle_mass * 9.81 * grade / 100 * sin(Modelica.Constants.pi / 4);
    //Real F_T;
    // Real F1;
    // Real tau_J2;
    Real vehicleSpeed;
  equation
    J1 * der(omega_a) = tau_b;
    tau_b = c * (theta_b - theta_a) + d * (omega_b - omega_a);
    theta_b * radius = x;
    -tau_b + radius * (F - vehicle_mass * der(v) - static_resistance) = J2 * der(omega_b);
    vehicleSpeed = -SI.Conversions.to_kmh(v);
  end simplifiedEquations;
  annotation(uses(Modelica(version = "3.2")));
end driveLine_2;
