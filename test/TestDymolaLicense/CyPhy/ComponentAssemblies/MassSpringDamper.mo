within CyPhy.ComponentAssemblies;
model MassSpringDamper
  //Parameters
  parameter Real mass_start_acc=10;

  //Metrics
  //Environments

  //ComponentAssemblies

  //Components
  replaceable
    CyPhy.Components.MSD.Components.Dampers.Damper_1
    Damper_1__Damper_1(
d=1) constrainedby
    Modelica.Mechanics.Translational.Components.Damper
    annotation(choicesAllMatching=true, Placement(transformation(origin={15,-15}, extent={{-15,-15},{15,15}})));
  replaceable
    CyPhy.Components.MSD.Components.Masses.Mass_5
    Mass_1__Mass_5(
    __CyPhy__a_start=mass_start_acc,
    massa=5) constrainedby
    ModifiedMass.MassInitial
    annotation(choicesAllMatching=true, Placement(transformation(origin={15,-15}, extent={{-15,-15},{15,15}})));
  replaceable
    CyPhy.Components.MSD.Components.Springs.Spring_1
    Spring_1__Spring_1(
c=1) constrainedby
    Modelica.Mechanics.Translational.Components.Spring
    annotation(choicesAllMatching=true, Placement(transformation(origin={15,-15}, extent={{-15,-15},{15,15}})));

  //Connectors
  Modelica.Mechanics.Translational.Interfaces.Flange_a DamperPort annotation(Placement(transformation(origin={98,-28}, extent={{-20,-20},{20,20}})));
  Modelica.Mechanics.Translational.Interfaces.Flange_a MassPort annotation(Placement(transformation(origin={252,-99}, extent={{-20,-20},{20,20}})));
  Modelica.Mechanics.Translational.Interfaces.Flange_a SpringPort annotation(Placement(transformation(origin={99,-169}, extent={{-20,-20},{20,20}})));
equation
  connect(Damper_1__Damper_1.flange_a, DamperPort) annotation(Line(points = {{0.0,0.0},{0.1,0.1}}));
  connect(Damper_1__Damper_1.flange_b, Mass_1__Mass_5.flange_b) annotation(Line(points = {{0.0,0.0},{0.1,0.1}}));
  connect(Mass_1__Mass_5.flange_a, MassPort) annotation(Line(points = {{0.0,0.0},{0.1,0.1}}));
  connect(Mass_1__Mass_5.flange_b, Spring_1__Spring_1.flange_b) annotation(Line(points = {{0.0,0.0},{0.1,0.1}}));
  connect(Spring_1__Spring_1.flange_a, SpringPort) annotation(Line(points = {{0.0,0.0},{0.1,0.1}}));

 // Annotations
annotation (Icon(coordinateSystem(preserveAspectRatio=true,  extent={{0,-169},{252,0}}),
graphics={
Rectangle(extent={{0,0},{252,-169}},
lineColor={0,0,0},
fillColor={250,250,255},
fillPattern=FillPattern.Solid),
Polygon(points={{-40,-40},{-40,0},{0,0},{-40,-40}},
smooth=Smooth.None,
fillColor={0,127,0},
fillPattern=FillPattern.Solid,
pattern=LinePattern.None,
origin={0,40},
rotation=0,
lineColor={0,127,0}),
Polygon(points={{-40,-40},{-40,0},{0,0},{-40,-40}},
smooth=Smooth.None,
fillColor={0,127,0},
fillPattern=FillPattern.Solid,
pattern=LinePattern.None,
origin={252,-209},
rotation=180,
lineColor={0,127,0}),
Rectangle(
extent={{-40,50},{292,90}},
lineColor={0,127,0},
fillColor={0,127,0},
fillPattern=FillPattern.Solid),
Text(
extent={{-40,50},{292,90}},
lineColor={255,255,255},
textString="%name",
fontName="Comic Sans MS",
textStyle={TextStyle.Bold})}),
Diagram(coordinateSystem(
preserveAspectRatio=true,  extent={{0,-169},{252,0}})));
end MassSpringDamper;
