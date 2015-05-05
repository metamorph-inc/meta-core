within FAME;

package Bridges
  "Bridge elements to be used for various 'external approach' modifications."

  model Electrical
    input Real amount(min=0.0, max=1.0);
    Modelica.Electrical.Analog.Interfaces.PositivePin port_a;
    Modelica.Electrical.Analog.Interfaces.NegativePin port_b;
    final type Modes = enumeration(Nominal, Shorted);
    parameter Modes mode = Modes.Nominal;
  protected
    Modelica.SIunits.Voltage v;
    Modelica.SIunits.Current i;
  equation
    v = port_a.v - port_b.v;
    i = port_a.i;
    0 = port_a.i + port_b.i;
    v = (1 - amount) * 1.0e10 * i;
    annotation
      (Icon(coordinateSystem
            (preserveAspectRatio=true,
        extent={{-100,-100},{100,100}},
        grid={2,2}), graphics={
          Rectangle(
            extent={{-70,30},{70,-30}},
            fillColor={255,255,255},
            fillPattern=FillPattern.Solid,
            lineColor={0,0,255}),
          Rectangle(extent={{-70,30},{70,-30}}, lineColor={0,0,255}),
          Line(points={{-90,0},{-70,0}}, color={0,0,255}),
          Line(points={{70,0},{90,0}}, color={0,0,255}),
          Text(
            extent={{-152,87},{148,47}},
            textString="%name",
            lineColor={0,0,255}),
          Text(
            extent={{-144,-38},{142,-70}},
            lineColor={0,0,0},
            textString="bridging=%bridging")}));
  end Electrical;

  model ElectricalAmountAsParameter
    parameter Real amount(min=0.0, max=1.0, start=0.0, unit="1");
    Modelica.Electrical.Analog.Interfaces.PositivePin port_a;
    Modelica.Electrical.Analog.Interfaces.NegativePin port_b;
    final type Modes = enumeration(Nominal, Shorted);
    parameter Modes mode = Modes.Nominal;
  protected
    Modelica.SIunits.Voltage v;
    Modelica.SIunits.Current i;
  equation
    v = port_a.v - port_b.v;
    i = port_a.i;
    0 = port_a.i + port_b.i;
    v = (1 - amount) * 1.0e10 * i;
    annotation
      (Icon(coordinateSystem
            (preserveAspectRatio=true,
        extent={{-100,-100},{100,100}},
        grid={2,2}), graphics={
          Rectangle(
            extent={{-70,30},{70,-30}},
            fillColor={255,255,255},
            fillPattern=FillPattern.Solid,
            lineColor={0,0,255}),
          Rectangle(extent={{-70,30},{70,-30}}, lineColor={0,0,255}),
          Line(points={{-90,0},{-70,0}}, color={0,0,255}),
          Line(points={{70,0},{90,0}}, color={0,0,255}),
          Text(
            extent={{-152,87},{148,47}},
            textString="%name",
            lineColor={0,0,255}),
          Text(
            extent={{-144,-38},{142,-70}},
            lineColor={0,0,0},
            textString="bridging=%bridging")}));
  end ElectricalAmountAsParameter;

end Bridges;
