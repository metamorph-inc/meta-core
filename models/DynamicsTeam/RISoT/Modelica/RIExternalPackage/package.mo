within ;
package RIExternalPackage
                          extends Modelica.Icons.Package;

  model DoubleSeriesResistor
  parameter Real DoubleR = 1;
    Modelica.Electrical.Analog.Basic.Resistor resistor(R=DoubleR)
      annotation (Placement(transformation(extent={{-54,-10},{-34,10}})));
    Modelica.Electrical.Analog.Basic.Resistor resistor1(R=DoubleR)
      annotation (Placement(transformation(extent={{36,-10},{56,10}})));
    Modelica.Electrical.Analog.Interfaces.PositivePin p
      annotation (Placement(transformation(extent={{-110,-10},{-90,10}})));
    Modelica.Electrical.Analog.Interfaces.NegativePin n
      annotation (Placement(transformation(extent={{90,-10},{110,10}})));
  equation
    connect(p, resistor.p)     annotation (Line(
        points={{-100,0},{-54,0}},
        color={0,0,255},
        smooth=Smooth.None));
    connect(resistor.n, resistor1.p) annotation (Line(
        points={{-34,0},{36,0}},
        color={0,0,255},
        smooth=Smooth.None));
    connect(resistor1.n, n)     annotation (Line(
        points={{56,0},{100,0}},
        color={0,0,255},
        smooth=Smooth.None));
    annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
              -100},{100,100}}), graphics));
  end DoubleSeriesResistor;
  annotation (uses(Modelica(version="3.2")));
end RIExternalPackage;
