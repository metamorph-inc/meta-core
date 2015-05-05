within ;
package TankExample 


extends Modelica.Icons.Package;


  model Tank

  Modelica.Blocks.Interfaces.RealInput u
    annotation (Placement(transformation(extent={{-116,50},{-92,74}})));
  Modelica.Blocks.Math.Gain gain(k=2)
    annotation (Placement(transformation(extent={{-38,54},{-24,68}})));
  Modelica.Blocks.Math.Add add(k2=-1)
    annotation (Placement(transformation(extent={{-4,50},{16,70}})));
  Modelica.Blocks.Continuous.Integrator integrator
    annotation (Placement(transformation(extent={{34,52},{54,72}})));
  Modelica.Blocks.Math.Gain gain1(k=.5)
    annotation (Placement(transformation(extent={{10,12},{-4,26}})));
  Modelica.Blocks.Interfaces.RealOutput y1 "Connector of Real output signal"
    annotation (Placement(transformation(extent={{96,52},{116,72}})));
  equation
  connect(gain.u, u) annotation (Line(
      points={{-39.4,61},{-48.7,61},{-48.7,62},{-104,62}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(gain.y, add.u1) annotation (Line(
      points={{-23.3,61},{-14.65,61},{-14.65,66},{-6,66}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(add.y, integrator.u) annotation (Line(
      points={{17,60},{24,60},{24,62},{32,62}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(integrator.y, gain1.u) annotation (Line(
      points={{55,62},{66,62},{66,20},{38,20},{38,19},{11.4,19}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(gain1.y, add.u2) annotation (Line(
      points={{-4.7,19},{-4.7,36.5},{-6,36.5},{-6,54}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(integrator.y, y1) annotation (Line(
      points={{55,62},{106,62}},
      color={0,0,127},
      smooth=Smooth.None));
  annotation (Diagram(coordinateSystem(preserveAspectRatio=true, extent={{-100,
            -100},{100,100}}),
                      graphics), Icon(coordinateSystem(extent={{-100,-100},{100,100}},
                   preserveAspectRatio=true),  graphics={
        Ellipse(
          extent={{-48,-52},{60,-68}},
          lineColor={0,0,0},
          fillColor={0,0,255},
          fillPattern=FillPattern.Solid),
        Rectangle(extent={{-48,68},{60,-60}}, lineColor={0,0,0}),
        Ellipse(
          extent={{-48,74},{60,62}},
          lineColor={0,0,0},
          fillColor={255,255,255},
          fillPattern=FillPattern.Solid),
        Rectangle(
          extent={{-48,-10},{60,-60}},
          lineColor={0,0,255},
          fillPattern=FillPattern.Solid,
          fillColor={0,0,255}),
        Ellipse(
          extent={{-48,-2},{60,-18}},
          lineColor={0,0,255},
          fillPattern=FillPattern.Solid,
          fillColor={0,128,255})}));
  end Tank;


  model TankController

  Modelica.Blocks.Math.Gain gain(k=2)
    annotation (Placement(transformation(extent={{-50,54},{-34,70}})));
  Modelica.Blocks.Math.Gain gain1(k=.4)
    annotation (Placement(transformation(extent={{0,-6},{20,14}})));
  Modelica.Blocks.Continuous.Integrator integrator
    annotation (Placement(transformation(extent={{-2,52},{18,72}})));
  Modelica.Blocks.Math.Add add
    annotation (Placement(transformation(extent={{40,32},{60,52}})));
  Modelica.Blocks.Interfaces.RealOutput y1 "Connector of Real output signal"
    annotation (Placement(transformation(extent={{96,32},{116,52}})));
  Modelica.Blocks.Interfaces.RealInput u
    annotation (Placement(transformation(extent={{-118,38},{-90,66}})));
  Modelica.Blocks.Math.Add add1(k2=-1)
    annotation (Placement(transformation(extent={{-76,8},{-62,22}})));
  Modelica.Blocks.Interfaces.RealInput u1
    annotation (Placement(transformation(extent={{-118,-28},{-90,0}})));
  equation
  connect(gain.y, integrator.u) annotation (Line(
      points={{-33.2,62},{-4,62}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(integrator.y, add.u1) annotation (Line(
      points={{19,62},{28,62},{28,48},{38,48}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(gain1.y, add.u2) annotation (Line(
      points={{21,4},{28,4},{28,36},{38,36}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(add.y, y1) annotation (Line(
      points={{61,42},{106,42}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(add1.y, gain.u) annotation (Line(
      points={{-61.3,15},{-61.3,23.5},{-51.6,23.5},{-51.6,62}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(gain1.u, add1.y) annotation (Line(
      points={{-2,4},{-26,4},{-26,15},{-61.3,15}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(add1.u1, u) annotation (Line(
      points={{-77.4,19.2},{-77.4,35.6},{-104,35.6},{-104,52}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(add1.u2, u1) annotation (Line(
      points={{-77.4,10.8},{-77.4,-2.6},{-104,-2.6},{-104,-14}},
      color={0,0,127},
      smooth=Smooth.None));
  annotation (Diagram(coordinateSystem(preserveAspectRatio=true, extent={{-100,
            -100},{100,100}}),
                      graphics), Icon(coordinateSystem(extent={{-100,-100},{100,
            100}}, preserveAspectRatio=false), graphics={Ellipse(extent={{-100,
              100},{100,-100}}, lineColor={0,0,0}), Text(
          extent={{-80,80},{80,-80}},
          lineColor={0,0,0},
          fillColor={0,0,255},
          fillPattern=FillPattern.Solid,
          textString="PI
controller")}));
  end TankController;


  model TankTestbench
  extends Modelica.Icons.Example;
  TankController tankController
    annotation (Placement(transformation(extent={{-28,42},{-8,62}})));
  Tank tank annotation (Placement(transformation(extent={{24,40},{44,60}})));
  Modelica.Blocks.Sources.Constant const(k=10)
    annotation (Placement(transformation(extent={{-90,38},{-70,58}})));
  equation
  connect(tankController.y1, tank.u) annotation (Line(
      points={{-8.8,56.2},{8.2,56.2},{8.2,56.2},{26.8,56.2}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(tank.y1, tankController.u1) annotation (Line(
      points={{42.8,56.2},{56,56.2},{56,26},{22,28},{-4,20},{-26.4,50.4}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(const.y, tankController.u) annotation (Line(
      points={{-69,48},{-48,48},{-48,57.2},{-27.4,57.2}},
      color={0,0,127},
      smooth=Smooth.None));
  annotation (
    Diagram(graphics),
    experiment(StopTime=10),
    __Dymola_experimentSetupOutput);
  end TankTestbench;


annotation (uses(Modelica(version="3.2")));
end TankExample;
