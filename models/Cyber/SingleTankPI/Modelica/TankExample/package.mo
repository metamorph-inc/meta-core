within ;
package TankExample 


extends Modelica.Icons.Package;


  model Tank
  Modelica.Blocks.Interfaces.RealInput u
    annotation (Placement(transformation(extent={{-116,-12},{-92,12}})));
  Modelica.Blocks.Math.Gain gain(k=2)
    annotation (Placement(transformation(extent={{-76,-7},{-62,7}})));
  Modelica.Blocks.Math.Add add(k2=-1)
    annotation (Placement(transformation(extent={{-40,-10},{-20,10}})));
  Modelica.Blocks.Continuous.Integrator integrator
    annotation (Placement(transformation(extent={{20,-10},{40,10}})));
  Modelica.Blocks.Math.Gain gain1(k=0.5)
    annotation (Placement(transformation(extent={{8,-47},{-6,-33}})));
  Modelica.Blocks.Interfaces.RealOutput y1 "Connector of Real output signal"
    annotation (Placement(transformation(extent={{96,-10},{116,10}})));
  equation
  connect(gain.u, u) annotation (Line(
      points={{-77.4,8.88178e-016},{-80,8.88178e-016},{-80,0},{-104,0}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(gain.y, add.u1) annotation (Line(
      points={{-61.3,8.88178e-016},{-56,8.88178e-016},{-56,0},{-52,0},{-52,6},{
          -42,6}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(add.y, integrator.u) annotation (Line(
      points={{-19,0},{18,0}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(integrator.y, gain1.u) annotation (Line(
      points={{41,0},{70,0},{70,-40},{9.4,-40}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(gain1.y, add.u2) annotation (Line(
      points={{-6.7,-40},{-52,-40},{-52,-6},{-42,-6}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(integrator.y, y1) annotation (Line(
      points={{41,0},{106,0}},
      color={0,0,127},
      smooth=Smooth.None));
  annotation (Diagram(coordinateSystem(preserveAspectRatio=true, extent={{-100,
            -100},{100,100}}),
                      graphics), Icon(coordinateSystem(extent={{-100,-100},{100,
            100}}, preserveAspectRatio=false), graphics={
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
    annotation (Placement(transformation(extent={{-40,20},{-20,40}})));
  Modelica.Blocks.Math.Gain gain1(k=0.4)
    annotation (Placement(transformation(extent={{-40,-40},{-20,-20}})));
  Modelica.Blocks.Continuous.Integrator integrator
    annotation (Placement(transformation(extent={{0,20},{20,40}})));
  Modelica.Blocks.Math.Add add
    annotation (Placement(transformation(extent={{40,-10},{60,10}})));
  Modelica.Blocks.Interfaces.RealOutput y1 "Connector of Real output signal"
    annotation (Placement(transformation(extent={{98,-10},{118,10}})));
  Modelica.Blocks.Interfaces.RealInput u
    annotation (Placement(transformation(extent={{-118,36},{-90,64}})));
  Modelica.Blocks.Math.Add add1(k2=-1)
    annotation (Placement(transformation(extent={{-76,-7},{-62,7}})));
  Modelica.Blocks.Interfaces.RealInput u1
    annotation (Placement(transformation(extent={{-118,-54},{-90,-26}})));
  equation
  connect(gain.y, integrator.u) annotation (Line(
      points={{-19,30},{-2,30}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(integrator.y, add.u1) annotation (Line(
      points={{21,30},{28,30},{28,6},{38,6}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(gain1.y, add.u2) annotation (Line(
      points={{-19,-30},{28,-30},{28,-6},{38,-6}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(add.y, y1) annotation (Line(
      points={{61,0},{108,0}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(add1.y, gain.u) annotation (Line(
      points={{-61.3,8.88178e-016},{-58,8.88178e-016},{-58,30},{-42,30}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(gain1.u, add1.y) annotation (Line(
      points={{-42,-30},{-58,-30},{-58,8.88178e-016},{-61.3,8.88178e-016}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(add1.u1, u) annotation (Line(
      points={{-77.4,4.2},{-78,4.2},{-78,4},{-80,4},{-80,50},{-104,50}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(add1.u2, u1) annotation (Line(
      points={{-77.4,-4.2},{-78,-4.2},{-78,-4},{-80,-4},{-80,-4},{-80,-40},{
          -104,-40}},
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
    annotation (Placement(transformation(extent={{-40,40},{-20,60}})));
  Tank tank annotation (Placement(transformation(extent={{20,40},{40,60}})));
  Modelica.Blocks.Sources.Constant const(k=10)
    annotation (Placement(transformation(extent={{-80,40},{-60,60}})));
  equation
  connect(tankController.y1, tank.u) annotation (Line(
      points={{-19.2,50},{19.6,50}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(tank.y1, tankController.u1) annotation (Line(
      points={{40.6,50},{50,50},{50,30},{-50,30},{-50,46},{-40.4,46}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(const.y, tankController.u) annotation (Line(
      points={{-59,50},{-50,50},{-50,55},{-40.4,55}},
      color={0,0,127},
      smooth=Smooth.None));
  annotation (
    Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,-100},{
            100,100}}),
            graphics),
    experiment(StopTime=10),
    __Dymola_experimentSetupOutput);
  end TankTestbench;

annotation (uses(Modelica(version="3.2")));
end TankExample;
