within ;
package TwoTankPackage
  extends Modelica.Icons.Package;

  model TankAStateController

  public
  Real mode;
  Real state;
  input Modelica.Blocks.Interfaces.RealInput inp annotation (Placement(transformation(rotation=0, extent={{-112,
              -10},{-92,10}})));
  output Modelica.Blocks.Interfaces.RealOutput dgain annotation (Placement(transformation(rotation=0, extent={{96,-10},
              {116,10}})));

  equation
    // mode 0 ON - HIGH
    // mode 1 MEDIUM
    // mode 2 OFF

    if ( mode < 1)  and ( inp < 2) then
      dgain =  0.5;
      mode =  1;
      state = 4;
    elseif ( mode < 2)  and ( mode > 0)  and ( inp > 2) then  // mode 1
      dgain =  1;
      mode =  0;
      state = 3;
    elseif ( mode < 2)  and ( mode > 0)  and ( inp < 0) then  // mode 1
      dgain =  0;
      mode =  2;
      state = 2;
    elseif ( mode > 1)  and ( inp > 2) then
      dgain =  0.5;
      mode =  1;
      state = 1;
    else
      dgain =  1;
      mode =  0;
      state = 0;
    end if;

    annotation (Diagram(coordinateSystem(preserveAspectRatio=true, extent={{-100,
            -100},{100,100}}),
                      graphics), Icon(coordinateSystem(extent={{-100,-100},{100,
            100}}, preserveAspectRatio=false), graphics={Ellipse(extent={{-100,
              100},{100,-100}}, lineColor={0,0,0}), Text(
          extent={{-80,80},{80,-80}},
          lineColor={0,0,0},
          fillColor={0,0,255},
          fillPattern=FillPattern.Solid,
          textString="SF
controller")}));
  end TankAStateController;

  model TankAStateController2

  public
  discrete Integer mode;

  Real state;
  input Modelica.Blocks.Interfaces.RealInput inp annotation (Placement(transformation(rotation=0, extent={{-112,
              -10},{-92,10}})));
  output Modelica.Blocks.Interfaces.RealOutput dgain annotation (Placement(transformation(rotation=0, extent={{96,-10},
              {116,10}})));

  algorithm
    when (pre(mode) == 0) and (inp < 2) then
  //    reinit(dgain, 1);
      mode := 1;
      dgain := 0.5;
    end when;
    when (pre(mode) == 1) and (inp > 2) then
  //    reinit(dgain, 0.5);
      mode := 0;
      dgain := 1;
    end when;
    when (pre(mode) == 1) and (inp < 0) then
  //    reinit(dgain, 0.5);
      mode := 2;
      dgain := 0;
    end when;

    when (pre(mode) == 2) and (inp > 2) then
  //    reinit(dgain, 0.5);
      mode := 1;
      dgain := 0.5;
    end when;

  initial equation
    dgain = 1;

  equation
    // mode 0 ON - HIGH
    // mode 1 MEDIUM
    // mode 2 OFF

    state = 0;

    /*if (pre(mode) == 0) and (inp < 2) then
    mode = 1;
    
  elseif (pre(mode) == 1) and (inp > 2) then
    mode = 0;
    //dgain = 1;           
  elseif (pre(mode) == 1) and (inp < 0) then
    mode = 2;
  elseif (pre(mode) == 2) and (inp > 2) then
    mode = 1;
  else
    mode = 0;
  end if;*/

  annotation (Diagram(coordinateSystem(preserveAspectRatio=true, extent={{-100,
            -100},{100,100}}),
                      graphics), Icon(coordinateSystem(extent={{-100,-100},{100,
            100}}, preserveAspectRatio=false), graphics={Ellipse(extent={{-100,
              100},{100,-100}}, lineColor={0,0,0}), Text(
          extent={{-80,80},{80,-80}},
          lineColor={0,0,0},
          fillColor={0,0,255},
          fillPattern=FillPattern.Solid,
          textString="SF
controller")}));
  end TankAStateController2;

public
  model TankA
    parameter Real y_start = 0
      "Initial tank level at the beginning of the simulation";
    Modelica.Blocks.Math.Gain gain(k=2)
      annotation (Placement(transformation(extent={{-52,76},{-42,86}})));
    Modelica.Blocks.Math.Add add(k2=-1)
      annotation (Placement(transformation(extent={{-24,70},{-14,80}})));
    Modelica.Blocks.Math.Gain gain1(k=0.1)
      annotation (Placement(transformation(extent={{-14,42},{-24,52}})));
    Modelica.Blocks.Continuous.Integrator integrator
      annotation (Placement(transformation(extent={{8,72},{16,80}})));
    Modelica.Blocks.Interfaces.RealOutput TankALevel
      annotation (Placement(transformation(extent={{98,68},{110,80}})));
    Modelica.Blocks.Interfaces.RealOutput TankAOutput(start=y_start)
      annotation (Placement(transformation(rotation=0, extent={{98,-30},{118,
              -10}})));
    Modelica.Blocks.Interfaces.RealInput u
      annotation (Placement(transformation(extent={{-120,62},{-88,94}})));
  equation
    connect(add.u1, gain.y) annotation (Line(
        points={{-25,78},{-32,78},{-32,81},{-41.5,81}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(add.u2, gain1.y) annotation (Line(
        points={{-25,72},{-28,72},{-28,47},{-24.5,47}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(add.y, integrator.u) annotation (Line(
        points={{-13.5,75},{0.25,75},{0.25,76},{7.2,76}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(gain1.u, integrator.y) annotation (Line(
        points={{-13,47},{25.5,47},{25.5,76},{16.4,76}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(integrator.y, TankALevel) annotation (Line(
        points={{16.4,76},{32,76},{32,74},{104,74}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(gain1.y, TankAOutput) annotation (Line(
        points={{-24.5,47},{-24.5,-20},{82,-20},{82,-20},{108,-20},{108,-20}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(gain.u, u) annotation (Line(
        points={{-53,81},{-63.5,81},{-63.5,78},{-104,78}},
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
  end TankA;

  model TankB
    parameter Real y_start = 0
      "Initial tank level at the beginning of the simulation";
    Modelica.Blocks.Math.Add3 add3_1(k3=-1)
      annotation (Placement(transformation(extent={{0,10},{10,20}})));
    Modelica.Blocks.Math.Gain gain2(k=2)
      annotation (Placement(transformation(extent={{-18,14},{-8,24}})));
    Modelica.Blocks.Math.Gain gain3(k=0.3)
      annotation (Placement(transformation(extent={{0,-26},{-10,-16}})));
    Modelica.Blocks.Continuous.Integrator integrator1
      annotation (Placement(transformation(extent={{22,4},{30,12}})));
    Modelica.Blocks.Interfaces.RealInput u2 annotation (Placement(
          transformation(rotation=0, extent={{-114,62},{-94,82}})));
    Modelica.Blocks.Interfaces.RealInput u annotation (Placement(transformation(
            rotation=0, extent={{-112,4},{-92,24}})));
    Modelica.Blocks.Interfaces.RealOutput TankBLevel(start=y_start) annotation (
       Placement(transformation(rotation=0, extent={{98,-2},{118,18}})));
  equation
    connect(gain3.u, integrator1.y) annotation (Line(
        points={{1,-21},{37.5,-21},{37.5,8},{30.4,8}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(gain3.y, add3_1.u3) annotation (Line(
        points={{-10.5,-21},{-10.5,-7.5},{-1,-7.5},{-1,11}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(add3_1.u1, gain2.y) annotation (Line(
        points={{-1,19},{-7.5,19}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(add3_1.y, integrator1.u) annotation (Line(
        points={{10.5,15},{16.25,15},{16.25,8},{21.2,8}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(u2, add3_1.u2) annotation (Line(points={{-104,72},{-104,62.5},{-1,
            62.5},{-1,15}},
                      color={0,0,127}));
    connect(u, gain2.u)
      annotation (Line(points={{-102,14},{-19,14},{-19,19}},color={0,0,127}));
    connect(TankBLevel, integrator1.y) annotation (Line(points={{108,8},{46.2,8},
            {30.4,8}},          color={0,0,127}));
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
  end TankB;
  annotation (uses(Modelica(version="3.2")));
end TwoTankPackage;
