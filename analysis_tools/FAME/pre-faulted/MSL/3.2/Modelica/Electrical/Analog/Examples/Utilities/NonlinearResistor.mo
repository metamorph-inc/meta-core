// processed by FAME Modelica Library Fault-Augmentor [version 19011]

within Modelica.Electrical.Analog.Examples.Utilities;

model NonlinearResistor
 "Chua's resistor"

// locally defined classes in NonlinearResistor
  model _famefaults_p = FAME.Dampers.ElectricalWithoutConnectEquations;
  model _famefaults_n = FAME.Dampers.ElectricalWithoutConnectEquations;
  model _famefaults_n__p = FAME.Bridges.Electrical;

// components of NonlinearResistor
 Modelica.SIunits.Voltage v "Voltage drop between the two pins (= p.v - n.v)";
 parameter Modelica.SIunits.Conductance Ga "Conductance in inner voltage range";
 parameter Modelica.SIunits.Conductance Gb "Conductance in outer voltage range";
 parameter Modelica.SIunits.Voltage Ve "Inner voltage range limit";
 Modelica.Electrical.Analog.Interfaces.PositivePin p "Positive pin (potential p.v > n.v for positive voltage drop v)" annotation(Placement(transformation(extent={{-110,-10},{-90,10}},rotation=0)));
 FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p(amount=0.0);
 Modelica.Electrical.Analog.Interfaces.NegativePin n "Negative pin" annotation(Placement(transformation(extent={{110,-10},{90,10}},rotation=0)));
 FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n(amount=0.0);
 Modelica.SIunits.Current i "Current flowing from pin p to pin n";
 FAME.Bridges.Electrical _famefault_n__p(amount=0.0);

// algorithms and equations of NonlinearResistor
equation
 i = (if v<-Ve then Gb*(v+Ve)-Ga*Ve else (if v>Ve then Gb*(v-Ve)+Ga*Ve else Ga*v));
 v = _famefault_p.port_b.v-_famefault_n.port_b.v;
 0 = _famefault_p.port_b.i+_famefault_n.port_b.i;
 i = _famefault_p.port_b.i;
 connect(p,_famefault_p.port_a);
 connect(n,_famefault_n.port_a);
 connect(n,_famefault_n__p.port_a);
 connect(p,_famefault_n__p.port_b);

annotation(Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Rectangle(extent={{-70,30},{70,-30}},lineColor={0,0,255},fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{-90,0},{-70,0}},color={0,0,255}),Line(points={{70,0},{90,0}},color={0,0,255}),Line(points={{-50,-60},{50,60}},color={0,0,255}),Polygon(points={{50,60},{38,52},{44,46},{50,60}},fillColor={0,0,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Text(extent={{-150,97},{150,67}},lineColor={0,0,255},textString="%name")}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Rectangle(extent={{-70,30},{70,-30}},lineColor={0,0,255}),Line(points={{-90,0},{-70,0}},color={0,0,255}),Line(points={{70,0},{90,0}},color={0,0,255}),Line(points={{-50,-60},{50,60}},color={0,0,255}),Polygon(points={{50,60},{38,52},{44,46},{50,60}},fillColor={0,0,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Text(extent={{-100,100},{100,70}},textString="%name",lineColor={0,0,255})}),Documentation(info="<html>
<p>This is the only nonlinear component for Chua&#39;;s circuit. It is a piecewise linear resistor with both an inner and an outer range, which includes the inner one. The slopes of both ranges are given by parameters. The resistance caracteristic is continuous. For Chua&#39;;s circuit both slopes have to be chosen to be negative.</p>
</html>"));
end NonlinearResistor;
