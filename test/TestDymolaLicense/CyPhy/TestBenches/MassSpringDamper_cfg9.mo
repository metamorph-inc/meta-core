within CyPhy.TestBenches;
model MassSpringDamper_cfg9
  //Parameters
  parameter Real f_const=5;
  parameter Real mass_start_acc=10;

  //Metrics
  Real MaximumSpeed=0 "PostProcessing : ";

  //Environments

  //ComponentAssemblies
   CyPhy.ComponentAssemblies.MassSpringDamper MassSpringDamper(
mass_start_acc=mass_start_acc) annotation(Placement(transformation(origin={45,-30}, extent={{-15,-15},{15,15}})));

  //TestComponents
  CyPhy.TestComponents.ConstantForce ConstantForce(
__CyPhy__f_const=f_const) annotation(choicesAllMatching=true, Placement(transformation(origin={15,-15}, extent={{-15,-15},{15,15}})));
  CyPhy.TestComponents.floor floor annotation(choicesAllMatching=true, Placement(transformation(origin={15,-15}, extent={{-15,-15},{15,15}})));
  CyPhy.TestComponents.roof roof annotation(choicesAllMatching=true, Placement(transformation(origin={15,-15}, extent={{-15,-15},{15,15}})));
  CyPhy.TestComponents.SpeedSensor SpeedSensor annotation(choicesAllMatching=true, Placement(transformation(origin={15,-15}, extent={{-15,-15},{15,15}})));
equation
  connect(ConstantForce.flange, MassSpringDamper.MassPort) annotation(Line(points = {{0.0,0.0},{0.1,0.1}}));
  connect(ConstantForce.flange, SpeedSensor.flange) annotation(Line(points = {{0.0,0.0},{0.1,0.1}}));
  connect(floor.flange, MassSpringDamper.DamperPort) annotation(Line(points = {{0.0,0.0},{0.1,0.1}}));
  connect(MassSpringDamper.SpringPort, roof.flange) annotation(Line(points = {{0.0,0.0},{0.1,0.1}}));

 // Annotations
annotation (Documentation(info="<HTML><p>Generated test bench from CyPhy using the META tools. Tool Version: 21529:21530M Interpreter Version: CyPhy2Modelica_v2, Version=1.0.0.0, Culture=neutral, PublicKeyToken=d71a19957c0c6396</p></HTML>"),experiment(StartTime=0, StopTime=10, Algorithm="dassl", Tolerance=0.0001, NumberOfIntervals=100),
Icon(coordinateSystem(preserveAspectRatio=true, extent={{0,-60},{90,0}}),
  graphics={
    Line(
      points={{15,-33},{25.5,-45},{43.5,-15}},
      color={0,255,0},
      smooth=Smooth.None,
      thickness=0.5),
    Line(
      points={{45,-45},{75,-15}},
      color={255,0,0},
      smooth=Smooth.None,
      thickness=0.5),
    Line(
      points={{45,-15},{75,-45}},
      color={255,0,0},
      smooth=Smooth.None,
      thickness=0.5)}),
  Diagram(coordinateSystem(preserveAspectRatio=true, extent={{0,-60},{90,0}})));
end MassSpringDamper_cfg9;
