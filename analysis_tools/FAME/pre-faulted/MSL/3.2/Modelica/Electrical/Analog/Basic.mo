// processed by FAME Modelica Library Fault-Augmentor [version 19011]

within Modelica.Electrical.Analog;

package Basic
 "Basic electrical components"
 extends Modelica.Icons.Package;

 model Ground
  "Ground node"

 // components of Ground
  Interfaces.Pin p annotation(Placement(transformation(origin={0,100},extent={{10,-10},{-10,10}},rotation=270)));

 // algorithms and equations of Ground
 equation
  p.v = 0;

 annotation(Documentation(info="<html>
<p>Ground of an electrical circuit. The potential at the ground node is zero. Every electrical circuit has to contain at least one ground object.</p>
</html>",revisions="<html>
<ul>
<li><i> 1998   </i>
       by Christoph Clauss<br> initially implemented<br>
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={2,2}),graphics={Line(points={{-60,50},{60,50}},color={0,0,255}),Line(points={{-40,30},{40,30}},color={0,0,255}),Line(points={{-20,10},{20,10}},color={0,0,255}),Line(points={{0,90},{0,50}},color={0,0,255}),Text(extent={{-144,-19},{156,-59}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={2,2}),graphics={Line(points={{-60,50},{60,50}},thickness=0.5,color={0,0,255}),Line(points={{-40,30},{40,30}},thickness=0.5,color={0,0,255}),Line(points={{-20,10},{20,10}},thickness=0.5,color={0,0,255}),Line(points={{0,96},{0,50}},thickness=0.5,color={0,0,255}),Text(extent={{-24,-38},{22,-6}},textString="p.v=0",lineColor={0,0,255})}));
 end Ground;

 model Resistor
  "Ideal linear electrical resistor"

 // locally defined classes in Resistor
    model _famefaults_heatPort = FAME.Dampers.ThermalWithoutConnectEquations;
    model _famefaults_n = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n__p = FAME.Bridges.Electrical;

 // components of Resistor
  Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_a heatPort() if useHeatPort annotation(Placement(transformation(extent={{-10,-110},{10,-90}}),iconTransformation(extent={{-10,-110},{10,-90}})));
  FAME.Dampers.ThermalWithoutConnectEquations _famefault_heatPort(amount=0.0,port_b(T(start=T)=T_heatPort,Q_flow=-LossPower)) if useHeatPort;
  parameter Modelica.SIunits.Temperature T_ref=300.15 "Reference temperature";
  Modelica.SIunits.Resistance R_actual "Actual resistance = R*(1 + alpha*(T_heatPort - T_ref))";
  Modelica.Electrical.Analog.Interfaces.NegativePin n "Negative pin" annotation(Placement(transformation(extent={{110,-10},{90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n(amount=0.0);
  parameter Boolean useHeatPort=false "=true, if HeatPort is enabled" annotation(Evaluate=true,HideResult=true,choices(__Dymola_checkBox=true));
  Modelica.SIunits.Current i "Current flowing from pin p to pin n";
  Modelica.SIunits.Power LossPower "Loss power leaving component via HeatPort";
  Modelica.SIunits.Voltage v "Voltage drop between the two pins (= p.v - n.v)";
  parameter Modelica.SIunits.Temperature T=T_ref "Fixed device temperature if useHeatPort = false" annotation(Dialog(enable=not useHeatPort));
  Modelica.Electrical.Analog.Interfaces.PositivePin p "Positive pin (potential p.v > n.v for positive voltage drop v)" annotation(Placement(transformation(extent={{-110,-10},{-90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p(amount=0.0);
  parameter Modelica.SIunits.Resistance R(start=1) "Resistance at temperature T_ref";
  parameter Modelica.SIunits.LinearTemperatureCoefficient alpha=0 "Temperature coefficient of resistance (R_actual = R*(1 + alpha*(T_heatPort - T_ref))";
  Modelica.SIunits.Temperature T_heatPort "Temperature of HeatPort";
  FAME.Bridges.Electrical _famefault_n__p(amount=0.0);

 // algorithms and equations of Resistor
 equation
  assert((1+alpha*(T_heatPort-T_ref))>=Modelica.Constants.eps,"Temperature outside scope of model!");
  R_actual = R*(1+alpha*(T_heatPort-T_ref));
  v = R_actual*i;
  LossPower = v*i;
  v = _famefault_p.port_b.v-_famefault_n.port_b.v;
  0 = _famefault_p.port_b.i+_famefault_n.port_b.i;
  i = _famefault_p.port_b.i;
  if not useHeatPort then
   T_heatPort = T;
  end if;
  connect(heatPort,_famefault_heatPort.port_a);
  connect(n,_famefault_n.port_a);
  connect(p,_famefault_p.port_a);
  connect(n,_famefault_n__p.port_a);
  connect(p,_famefault_n__p.port_b);

 annotation(Documentation(info="<html>
<p>The linear resistor connects the branch voltage <i>v</i> with the branch current <i>i</i> by <i>i*R = v</i>. The Resistance <i>R</i> is allowed to be positive, zero, or negative.</p>
</html>",revisions="<html>
<ul>
<li><i> August 07, 2009   </i>
       by Anton Haumer<br> temperature dependency of resistance added<br>
       </li>
<li><i> March 11, 2009   </i>
       by Christoph Clauss<br> conditional heat port added<br>
       </li>
<li><i> 1998   </i>
       by Christoph Clauss<br> initially implemented<br>
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={2,2}),graphics={Rectangle(extent={{-70,30},{70,-30}},lineColor={0,0,255},fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{-90,0},{-70,0}},color={0,0,255}),Line(points={{70,0},{90,0}},color={0,0,255}),Text(extent={{-144,-40},{142,-72}},lineColor={0,0,0},textString="R=%R"),Line(visible=useHeatPort,points={{0,-100},{0,-30}},color={127,0,0},smooth=Smooth.None,pattern=LinePattern.Dot),Text(extent={{-152,87},{148,47}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={2,2}),graphics={Rectangle(extent={{-70,30},{70,-30}},lineColor={0,0,255}),Line(points={{-96,0},{-70,0}},color={0,0,255}),Line(points={{70,0},{96,0}},color={0,0,255})}));
 end Resistor;

 model HeatingResistor
  "Temperature dependent electrical resistor"

 // locally defined classes in HeatingResistor
    model _famefaults_heatPort = FAME.Dampers.ThermalWithoutConnectEquations;
    model _famefaults_n = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n__p = FAME.Bridges.Electrical;

 // components of HeatingResistor
  Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_a heatPort() if useHeatPort annotation(Placement(transformation(extent={{-10,-110},{10,-90}}),iconTransformation(extent={{-10,-110},{10,-90}})));
  FAME.Dampers.ThermalWithoutConnectEquations _famefault_heatPort(amount=0.0,port_b(T(start=T)=T_heatPort,Q_flow=-LossPower)) if useHeatPort;
  parameter Modelica.SIunits.Temperature T_ref=300.15 "Reference temperature";
  parameter Modelica.SIunits.Resistance R_ref(start=1) "Resistance at temperature T_ref";
  Modelica.Electrical.Analog.Interfaces.NegativePin n "Negative pin" annotation(Placement(transformation(extent={{110,-10},{90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n(amount=0.0);
  parameter Boolean useHeatPort=true "=true, if HeatPort is enabled" annotation(Evaluate=true,HideResult=true,choices(__Dymola_checkBox=true));
  Modelica.SIunits.Current i "Current flowing from pin p to pin n";
  Modelica.SIunits.Power LossPower "Loss power leaving component via HeatPort";
  Modelica.SIunits.Voltage v "Voltage drop between the two pins (= p.v - n.v)";
  parameter Modelica.SIunits.Temperature T=T_ref "Fixed device temperature if useHeatPort = false" annotation(Dialog(enable=not useHeatPort));
  Modelica.Electrical.Analog.Interfaces.PositivePin p "Positive pin (potential p.v > n.v for positive voltage drop v)" annotation(Placement(transformation(extent={{-110,-10},{-90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p(amount=0.0);
  parameter Modelica.SIunits.LinearTemperatureCoefficient alpha=0 "Temperature coefficient of resistance (R = R_ref*(1 + alpha*(heatPort.T - T_ref))";
  Modelica.SIunits.Resistance R "Resistance = R_ref*(1 + alpha*(T_heatPort - T_ref))";
  Modelica.SIunits.Temperature T_heatPort "Temperature of HeatPort";
  FAME.Bridges.Electrical _famefault_n__p(amount=0.0);

 // algorithms and equations of HeatingResistor
 equation
  assert((1+alpha*(T_heatPort-T_ref))>=Modelica.Constants.eps,"Temperature outside scope of model!");
  R = R_ref*(1+alpha*(T_heatPort-T_ref));
  v = R*i;
  LossPower = v*i;
  v = _famefault_p.port_b.v-_famefault_n.port_b.v;
  0 = _famefault_p.port_b.i+_famefault_n.port_b.i;
  i = _famefault_p.port_b.i;
  if not useHeatPort then
   T_heatPort = T;
  end if;
  connect(heatPort,_famefault_heatPort.port_a);
  connect(n,_famefault_n.port_a);
  connect(p,_famefault_p.port_a);
  connect(n,_famefault_n__p.port_a);
  connect(p,_famefault_n__p.port_b);

 annotation(__Dymola_structurallyIncomplete=true,Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics={Line(points={{-110,20},{-85,20}},color={160,160,164}),Polygon(points={{-95,23},{-85,20},{-95,17},{-95,23}},lineColor={160,160,164},fillColor={160,160,164},fillPattern=FillPattern.Solid),Line(points={{90,20},{115,20}},color={160,160,164}),Line(points={{-125,0},{-115,0}},color={160,160,164}),Line(points={{-120,-5},{-120,5}},color={160,160,164}),Text(extent={{-110,25},{-90,45}},lineColor={160,160,164},textString="i"),Polygon(points={{105,23},{115,20},{105,17},{105,23}},lineColor={160,160,164},fillColor={160,160,164},fillPattern=FillPattern.Solid),Line(points={{115,0},{125,0}},color={160,160,164}),Text(extent={{90,45},{110,25}},lineColor={160,160,164},textString="i")}),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics={Line(points={{-90,0},{-70,0}},color={0,0,255}),Line(points={{70,0},{90,0}},color={0,0,255}),Rectangle(extent={{-70,30},{70,-30}},lineColor={0,0,255},fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{-52,-50},{48,50}},color={0,0,255}),Polygon(points={{40,52},{50,42},{54,56},{40,52}},lineColor={0,0,255},fillColor={0,0,255},fillPattern=FillPattern.Solid),Line(visible=useHeatPort,points={{0,-100},{0,-30}},color={127,0,0},smooth=Smooth.None,pattern=LinePattern.Dot),Text(extent={{-156,109},{144,69}},textString="%name",lineColor={0,0,255})}),Documentation(info="<html>
<p>This is a model for an electrical resistor where the generated heat is dissipated to the environment via connector <b>heatPort</b> and where the resistance R is temperature dependent according to the following equation:</p>
<pre>    R = R_ref*(1 + alpha*(heatPort.T - T_ref))</pre>
<p><b>alpha</b> is the <b>temperature coefficient of resistance</b>, which is often abbreviated as <b>TCR</b>. In resistor catalogues, it is usually defined as <b>X [ppm/K]</b> (parts per million, similarly to per centage) meaning <b>X*1.e-6 [1/K]</b>. Resistors are available for 1 .. 7000 ppm/K, i.e., alpha = 1e-6 .. 7e-3 1/K;</p>
<p>Via parameter <b>useHeatPort</b> the heatPort connector can be enabled and disabled (default = enabled). If it is disabled, the generated heat is transported implicitly to an internal temperature source with a fixed temperature of T_ref.</p><p>If the heatPort connector is enabled, it must be connected.</p>
</html>",revisions="<html>
<ul>
<li><i> August 07, 2009   </i>
       by Anton Haumer<br> temperature dependency of resistance added<br>
       </li>
<li><i> March 11, 2009   </i>
       by Christoph Clauss<br> conditional heat port added<br>
       </li>
<li><i> 2002   </i>
       by Anton Haumer<br> initially implemented<br>
       </li>
</ul>
</html>"));
 end HeatingResistor;

 model Conductor
  "Ideal linear electrical conductor"

 // locally defined classes in Conductor
    model _famefaults_heatPort = FAME.Dampers.ThermalWithoutConnectEquations;
    model _famefaults_n = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n__p = FAME.Bridges.Electrical;

 // components of Conductor
  Modelica.SIunits.Conductance G_actual "Actual conductance = G_ref/(1 + alpha*(T_heatPort - T_ref))";
  Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_a heatPort() if useHeatPort annotation(Placement(transformation(extent={{-10,-110},{10,-90}}),iconTransformation(extent={{-10,-110},{10,-90}})));
  FAME.Dampers.ThermalWithoutConnectEquations _famefault_heatPort(amount=0.0,port_b(T(start=T)=T_heatPort,Q_flow=-LossPower)) if useHeatPort;
  parameter Modelica.SIunits.Temperature T_ref=300.15 "Reference temperature";
  parameter Modelica.SIunits.Conductance G(start=1) "Conductance at temperature T_ref";
  Modelica.Electrical.Analog.Interfaces.NegativePin n "Negative pin" annotation(Placement(transformation(extent={{110,-10},{90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n(amount=0.0);
  parameter Boolean useHeatPort=false "=true, if HeatPort is enabled" annotation(Evaluate=true,HideResult=true,choices(__Dymola_checkBox=true));
  Modelica.SIunits.Current i "Current flowing from pin p to pin n";
  Modelica.SIunits.Power LossPower "Loss power leaving component via HeatPort";
  Modelica.SIunits.Voltage v "Voltage drop between the two pins (= p.v - n.v)";
  parameter Modelica.SIunits.Temperature T=T_ref "Fixed device temperature if useHeatPort = false" annotation(Dialog(enable=not useHeatPort));
  Modelica.Electrical.Analog.Interfaces.PositivePin p "Positive pin (potential p.v > n.v for positive voltage drop v)" annotation(Placement(transformation(extent={{-110,-10},{-90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p(amount=0.0);
  parameter Modelica.SIunits.LinearTemperatureCoefficient alpha=0 "Temperature coefficient of conductance (G_actual = G_ref/(1 + alpha*(T_heatPort - T_ref))";
  Modelica.SIunits.Temperature T_heatPort "Temperature of HeatPort";
  FAME.Bridges.Electrical _famefault_n__p(amount=0.0);

 // algorithms and equations of Conductor
 equation
  assert((1+alpha*(T_heatPort-T_ref))>=Modelica.Constants.eps,"Temperature outside scope of model!");
  G_actual = G/(1+alpha*(T_heatPort-T_ref));
  i = G_actual*v;
  LossPower = v*i;
  v = _famefault_p.port_b.v-_famefault_n.port_b.v;
  0 = _famefault_p.port_b.i+_famefault_n.port_b.i;
  i = _famefault_p.port_b.i;
  if not useHeatPort then
   T_heatPort = T;
  end if;
  connect(heatPort,_famefault_heatPort.port_a);
  connect(n,_famefault_n.port_a);
  connect(p,_famefault_p.port_a);
  connect(p,_famefault_n__p.port_a);
  connect(n,_famefault_n__p.port_b);

 annotation(Documentation(info="<html>
<p>The linear conductor connects the branch voltage <i>v</i> with the branch current <i>i</i> by <i>i = v*G</i>. The Conductance <i>G</i> is allowed to be positive, zero, or negative.</p>
</html>",revisions="<html>
<ul>
<li><i> August 07, 2009   </i>
       by Anton Haumer<br> temperature dependency of conductance added<br>
       </li>
<li><i> March 11, 2009   </i>
       by Christoph Clauss<br> conditional heat port added<br>
       </li>
<li><i> 1998   </i>
       by Christoph Clauss<br> initially implemented<br>
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={2,2}),graphics={Rectangle(extent={{-70,30},{70,-30}},fillColor={255,255,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Rectangle(extent={{-70,30},{70,-30}},lineColor={0,0,255}),Line(points={{-90,0},{-70,0}},color={0,0,255}),Line(points={{70,0},{90,0}},color={0,0,255}),Line(visible=useHeatPort,points={{0,-100},{0,-30}},color={127,0,0},smooth=Smooth.None,pattern=LinePattern.Dot),Text(extent={{-152,87},{148,47}},textString="%name",lineColor={0,0,255}),Text(extent={{-144,-38},{142,-70}},lineColor={0,0,0},textString="G=%G")}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={2,2}),graphics={Line(points={{-96,0},{-70,0}},color={0,0,255}),Line(points={{70,0},{96,0}},color={0,0,255}),Rectangle(extent={{-70,30},{70,-30}},lineColor={0,0,255})}));
 end Conductor;

 model Capacitor
  "Ideal linear electrical capacitor"

 // locally defined classes in Capacitor
    model _famefaults_p = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n__p = FAME.Bridges.Electrical;

 // components of Capacitor
  Modelica.SIunits.Voltage v "Voltage drop between the two pins (= p.v - n.v)";
  parameter Modelica.SIunits.Capacitance C(start=1) "Capacitance";
  Modelica.Electrical.Analog.Interfaces.PositivePin p "Positive pin (potential p.v > n.v for positive voltage drop v)" annotation(Placement(transformation(extent={{-110,-10},{-90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p(amount=0.0);
  Modelica.Electrical.Analog.Interfaces.NegativePin n "Negative pin" annotation(Placement(transformation(extent={{110,-10},{90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n(amount=0.0);
  Modelica.SIunits.Current i "Current flowing from pin p to pin n";
  FAME.Bridges.Electrical _famefault_n__p(amount=0.0);

 // algorithms and equations of Capacitor
 equation
  i = C*der(v);
  v = _famefault_p.port_b.v-_famefault_n.port_b.v;
  0 = _famefault_p.port_b.i+_famefault_n.port_b.i;
  i = _famefault_p.port_b.i;
  connect(p,_famefault_p.port_a);
  connect(n,_famefault_n.port_a);
  connect(p,_famefault_n__p.port_a);
  connect(n,_famefault_n__p.port_b);

 annotation(Documentation(info="<html>
<p>The linear capacitor connects the branch voltage <i>v</i> with the branch current <i>i</i> by <i>i = C * dv/dt</i>. The Capacitance <i>C</i> is allowed to be positive, zero, or negative.</p>
</html>",revisions="<html>
<ul>
<li><i> 1998   </i>
       by Christoph Clauss<br> initially implemented<br>
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={2,2}),graphics={Line(points={{-14,28},{-14,-28}},thickness=0.5,color={0,0,255}),Line(points={{14,28},{14,-28}},thickness=0.5,color={0,0,255}),Line(points={{-90,0},{-14,0}},color={0,0,255}),Line(points={{14,0},{90,0}},color={0,0,255}),Text(extent={{-136,-60},{136,-92}},lineColor={0,0,0},textString="C=%C"),Text(extent={{-150,85},{150,45}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={2,2}),graphics={Line(points={{-20,40},{-20,-40}},thickness=0.5,color={0,0,255}),Line(points={{20,40},{20,-40}},thickness=0.5,color={0,0,255}),Line(points={{-96,0},{-20,0}},color={0,0,255}),Line(points={{20,0},{96,0}},color={0,0,255})}));
 end Capacitor;

 model Inductor
  "Ideal linear electrical inductor"

 // locally defined classes in Inductor
    model _famefaults_p = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n__p = FAME.Bridges.Electrical;

 // components of Inductor
  Modelica.SIunits.Voltage v "Voltage drop between the two pins (= p.v - n.v)";
  Modelica.Electrical.Analog.Interfaces.PositivePin p "Positive pin (potential p.v > n.v for positive voltage drop v)" annotation(Placement(transformation(extent={{-110,-10},{-90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p(amount=0.0);
  parameter Modelica.SIunits.Inductance L(start=1) "Inductance";
  Modelica.Electrical.Analog.Interfaces.NegativePin n "Negative pin" annotation(Placement(transformation(extent={{110,-10},{90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n(amount=0.0);
  Modelica.SIunits.Current i "Current flowing from pin p to pin n";
  FAME.Bridges.Electrical _famefault_n__p(amount=0.0);

 // algorithms and equations of Inductor
 equation
  L*der(i) = v;
  v = _famefault_p.port_b.v-_famefault_n.port_b.v;
  0 = _famefault_p.port_b.i+_famefault_n.port_b.i;
  i = _famefault_p.port_b.i;
  connect(p,_famefault_p.port_a);
  connect(n,_famefault_n.port_a);
  connect(p,_famefault_n__p.port_a);
  connect(n,_famefault_n__p.port_b);

 annotation(Documentation(info="<html>
<p>The linear inductor connects the branch voltage <i>v</i> with the branch current <i>i</i> by <i>v = L * di/dt</i>. The Inductance <i>L</i> is allowed to be positive, zero, or negative.</p>
</html>",revisions="<html>
<ul>
<li><i> 1998   </i>
       by Christoph Clauss<br> initially implemented<br>
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={2,2}),graphics={Ellipse(extent={{-60,-15},{-30,15}},lineColor={0,0,255}),Ellipse(extent={{-30,-15},{0,15}},lineColor={0,0,255}),Ellipse(extent={{0,-15},{30,15}},lineColor={0,0,255}),Ellipse(extent={{30,-15},{60,15}},lineColor={0,0,255}),Rectangle(extent={{-60,-30},{60,0}},lineColor={255,255,255},fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{60,0},{90,0}},color={0,0,255}),Line(points={{-90,0},{-60,0}},color={0,0,255}),Text(extent={{-138,-60},{144,-94}},lineColor={0,0,0},textString="L=%L"),Text(extent={{-152,79},{148,39}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={2,2}),graphics={Ellipse(extent={{-60,-15},{-30,15}},lineColor={0,0,255}),Ellipse(extent={{-30,-15},{0,15}},lineColor={0,0,255}),Ellipse(extent={{0,-15},{30,15}},lineColor={0,0,255}),Ellipse(extent={{30,-15},{60,15}},lineColor={0,0,255}),Rectangle(extent={{-60,-30},{60,0}},lineColor={255,255,255},fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{60,0},{96,0}},color={0,0,255}),Line(points={{-96,0},{-60,0}},color={0,0,255})}));
 end Inductor;

 model SaturatingInductor
  "Simple model of an inductor with saturation"

 // locally defined classes in SaturatingInductor
    model _famefaults_p = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n__p = FAME.Bridges.Electrical;

 // components of SaturatingInductor
  Modelica.SIunits.Inductance Lact(start=Lzer);
  Modelica.SIunits.Voltage v "Voltage drop between the two pins (= p.v - n.v)";
  parameter Modelica.SIunits.Inductance Lzer(start=2*Lnom) "Inductance near current=0";
  parameter Modelica.SIunits.Current Inom(start=1) "Nominal current";
  Modelica.Electrical.Analog.Interfaces.PositivePin p "Positive pin (potential p.v > n.v for positive voltage drop v)" annotation(Placement(transformation(extent={{-110,-10},{-90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p(amount=0.0);
  Modelica.Electrical.Analog.Interfaces.NegativePin n "Negative pin" annotation(Placement(transformation(extent={{110,-10},{90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n(amount=0.0);
  Modelica.SIunits.MagneticFlux Psi;
  parameter Modelica.SIunits.Inductance Linf(start=Lnom/(2)) "Inductance at large currents";
  parameter Modelica.SIunits.Inductance Lnom(start=1) "Nominal inductance at Nominal current";
  Modelica.SIunits.Current i "Current flowing from pin p to pin n";
  FAME.Bridges.Electrical _famefault_n__p(amount=0.0);
 protected
  parameter Modelica.SIunits.Current Ipar(start=Inom/(10),fixed=false);

 // algorithms and equations of SaturatingInductor
 initial equation
  Lnom-Linf = (Lzer-Linf)*Ipar/(Inom)*(Modelica.Constants.pi/(2)-Modelica.Math.atan(Ipar/(Inom)));
 equation
  assert(Lzer>(Lnom+Modelica.Constants.eps),"Lzer (= "+String(Lzer)+") has to be > Lnom (= "+String(Lnom)+")");
  assert(Linf<(Lnom-Modelica.Constants.eps),"Linf (= "+String(Linf)+") has to be < Lnom (= "+String(Lnom)+")");
  (Lact-Linf)*i/(Ipar) = (Lzer-Linf)*noEvent(Modelica.Math.atan(i/(Ipar)));
  Psi = Lact*i;
  v = der(Psi);
  v = _famefault_p.port_b.v-_famefault_n.port_b.v;
  0 = _famefault_p.port_b.i+_famefault_n.port_b.i;
  i = _famefault_p.port_b.i;
  connect(p,_famefault_p.port_a);
  connect(n,_famefault_n.port_a);
  connect(p,_famefault_n__p.port_a);
  connect(n,_famefault_n__p.port_b);

 annotation(Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics={Ellipse(extent={{-60,-15},{-30,15}},lineColor={0,0,255}),Ellipse(extent={{-30,-15},{0,15}},lineColor={0,0,255}),Ellipse(extent={{0,-15},{30,15}},lineColor={0,0,255}),Ellipse(extent={{30,-15},{60,15}},lineColor={0,0,255}),Rectangle(extent={{-60,-20},{62,0}},lineColor={255,255,255},fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{60,0},{90,0}},color={0,0,255}),Line(points={{-90,0},{-60,0}},color={0,0,255}),Rectangle(extent={{-60,-10},{60,-20}},lineColor={0,0,0},fillPattern=FillPattern.Sphere,fillColor={0,0,255}),Text(extent={{-148,-50},{152,-80}},lineColor={0,0,0},textString="Lnom=%Lnom"),Text(extent={{-148,91},{152,51}},textString="%name",lineColor={0,0,255})}),Documentation(info="<html>
<p>This model approximates the behaviour of an inductor with the influence of saturation, i.e., the value of the inductance depends on the current flowing through the inductor. The inductance decreases as current increases.</p><p>The parameters are:</p>
<ul>
<li>Inom...nominal current</li>
<li>Lnom...nominal inductance at nominal current</li>
<li>Lzer...inductance near current = 0; Lzer has to be greater than Lnom</li>
<li>Linf...inductance at large currents; Linf has to be less than Lnom </li>
</ul>
</html>",revisions="<html>
<dl>
<p>
  <dt><b>Main Author:</b></dt>
  <dd>
  <a href=\"http://www.haumer.at/\">Anton Haumer</a><br>
  Technical Consulting &amp; Electrical Engineering<br>
  A-3423 St.Andrae-Woerdern<br>Austria<br>
  email: <a href=\"mailto:a.haumer@haumer.at\">a.haumer@haumer.at</a>
  </dd>
</p>
<p>
  <dt><b>Release Notes:</b></dt>
  <dd>
  <ul>
  <li> May 27, 2004: Implemented by Anton Haumer</li>
  </ul>
  </dd>
</dl>
</html>"),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics={Ellipse(extent={{-60,-15},{-30,15}},lineColor={0,0,255}),Ellipse(extent={{-30,-15},{0,15}},lineColor={0,0,255}),Ellipse(extent={{0,-15},{30,15}},lineColor={0,0,255}),Ellipse(extent={{30,-15},{60,15}},lineColor={0,0,255}),Rectangle(extent={{-60,-20},{62,0}},lineColor={255,255,255},fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{60,0},{96,0}},color={0,0,255}),Line(points={{-96,0},{-60,0}},color={0,0,255}),Rectangle(extent={{-60,-10},{60,-20}},lineColor={0,0,0},fillPattern=FillPattern.Sphere,fillColor={0,0,255})}));
 end SaturatingInductor;

 model Transformer
  "Transformer with two ports"

 // locally defined classes in Transformer
    model _famefaults_n1 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p2 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p1 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n2 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n2__p2 = FAME.Bridges.Electrical;
    model _famefaults_p1__p2 = FAME.Bridges.Electrical;
    model _famefaults_n1__p2 = FAME.Bridges.Electrical;
    model _famefaults_n2__p1 = FAME.Bridges.Electrical;
    model _famefaults_n1__n2 = FAME.Bridges.Electrical;
    model _famefaults_n1__p1 = FAME.Bridges.Electrical;

 // components of Transformer
  Modelica.SIunits.Voltage v1 "Voltage drop over the left port";
  Modelica.Electrical.Analog.Interfaces.NegativePin n1 "Negative pin of the left port" annotation(Placement(transformation(extent={{-90,-60},{-110,-40}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n1(amount=0.0);
  parameter Modelica.SIunits.Inductance L1(start=1) "Primary inductance";
  parameter Modelica.SIunits.Inductance L2(start=1) "Secondary inductance";
  Modelica.Electrical.Analog.Interfaces.PositivePin p2 "Positive pin of the right port (potential p2.v > n2.v for positive voltage drop v2)" annotation(Placement(transformation(extent={{110,40},{90,60}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p2(amount=0.0);
  Modelica.Electrical.Analog.Interfaces.PositivePin p1 "Positive pin of the left port (potential p1.v > n1.v for positive voltage drop v1)" annotation(Placement(transformation(extent={{-110,40},{-90,60}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p1(amount=0.0);
  Modelica.SIunits.Voltage v2 "Voltage drop over the right port";
  Modelica.Electrical.Analog.Interfaces.NegativePin n2 "Negative pin of the right port" annotation(Placement(transformation(extent={{90,-60},{110,-40}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n2(amount=0.0);
  parameter Modelica.SIunits.Inductance M(start=1) "Coupling inductance";
  Modelica.SIunits.Current i2 "Current flowing from pos. to neg. pin of the right port";
  Modelica.SIunits.Current i1 "Current flowing from pos. to neg. pin of the left port";
  FAME.Bridges.Electrical _famefault_n2__p2(amount=0.0);
  FAME.Bridges.Electrical _famefault_p1__p2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n1__p2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n2__p1(amount=0.0);
  FAME.Bridges.Electrical _famefault_n1__n2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n1__p1(amount=0.0);

 // algorithms and equations of Transformer
 equation
  v1 = L1*der(i1)+M*der(i2);
  v2 = M*der(i1)+L2*der(i2);
  v1 = _famefault_p1.port_b.v-_famefault_n1.port_b.v;
  v2 = _famefault_p2.port_b.v-_famefault_n2.port_b.v;
  0 = _famefault_p1.port_b.i+_famefault_n1.port_b.i;
  0 = _famefault_p2.port_b.i+_famefault_n2.port_b.i;
  i1 = _famefault_p1.port_b.i;
  i2 = _famefault_p2.port_b.i;
  connect(n1,_famefault_n1.port_a);
  connect(p2,_famefault_p2.port_a);
  connect(p1,_famefault_p1.port_a);
  connect(n2,_famefault_n2.port_a);
  connect(p2,_famefault_n2__p2.port_a);
  connect(n2,_famefault_n2__p2.port_b);
  connect(p2,_famefault_p1__p2.port_a);
  connect(p1,_famefault_p1__p2.port_b);
  connect(p2,_famefault_n1__p2.port_a);
  connect(n1,_famefault_n1__p2.port_b);
  connect(n2,_famefault_n2__p1.port_a);
  connect(p1,_famefault_n2__p1.port_b);
  connect(n2,_famefault_n1__n2.port_a);
  connect(n1,_famefault_n1__n2.port_b);
  connect(p1,_famefault_n1__p1.port_a);
  connect(n1,_famefault_n1__p1.port_b);

 annotation(Documentation(info="<html>
<p>The transformer is a two port. The left port voltage <i>v1</i>, left port current <i>i1</i>, right port voltage <i>v2</i> and right port current <i>i2</i> are connected by the following relation:</p>
<pre>         | v1 |         | L1   M  |  | i1&#39;; |
         |    |    =    |         |  |     |
         | v2 |         | M    L2 |  | i2&#39;; |</pre>
<p><i>L1</i>, <i>L2</i>, and <i>M</i> are the primary, secondary, and coupling inductances respectively.</p>
</html>",revisions="<html>
<ul>
<li><i> 1998   </i>
       by Christoph Clauss<br> initially implemented<br>
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Ellipse(extent={{-45,-50},{-20,-25}},lineColor={0,0,255}),Ellipse(extent={{-45,-25},{-20,0}},lineColor={0,0,255}),Ellipse(extent={{-45,0},{-20,25}},lineColor={0,0,255}),Ellipse(extent={{-45,25},{-20,50}},lineColor={0,0,255}),Rectangle(extent={{-72,-60},{-33,60}},lineColor={255,255,255},fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{-90,50},{-32,50}},color={0,0,255}),Line(points={{-90,-50},{-32,-50}},color={0,0,255}),Ellipse(extent={{20,-50},{45,-25}},lineColor={0,0,255}),Ellipse(extent={{20,-25},{45,0}},lineColor={0,0,255}),Ellipse(extent={{20,0},{45,25}},lineColor={0,0,255}),Ellipse(extent={{20,25},{45,50}},lineColor={0,0,255}),Rectangle(extent={{33,-60},{72,60}},lineColor={255,255,255},fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{32,50},{90,50}},color={0,0,255}),Line(points={{32,-50},{90,-50}},color={0,0,255}),Text(extent={{-103,16},{-60,-10}},textString="L1",lineColor={0,0,0},pattern=LinePattern.Dot),Text(extent={{64,15},{105,-9}},textString="L2",lineColor={0,0,0},pattern=LinePattern.Dot),Text(extent={{-27,-71},{33,-97}},textString="M",lineColor={0,0,0}),Text(extent={{-153,113},{147,73}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Ellipse(extent={{-45,-50},{-20,-25}},lineColor={0,0,255}),Ellipse(extent={{-45,-25},{-20,0}},lineColor={0,0,255}),Ellipse(extent={{-45,0},{-20,25}},lineColor={0,0,255}),Ellipse(extent={{-45,25},{-20,50}},lineColor={0,0,255}),Rectangle(extent={{-72,-60},{-33,60}},lineColor={255,255,255},fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{-96,50},{-32,50}},color={0,0,255}),Line(points={{-96,-50},{-32,-50}},color={0,0,255}),Ellipse(extent={{20,-50},{45,-25}},lineColor={0,0,255}),Ellipse(extent={{20,-25},{45,0}},lineColor={0,0,255}),Ellipse(extent={{20,0},{45,25}},lineColor={0,0,255}),Ellipse(extent={{20,25},{45,50}},lineColor={0,0,255}),Rectangle(extent={{33,-60},{72,60}},lineColor={255,255,255},fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{32,50},{96,50}},color={0,0,255}),Line(points={{32,-50},{96,-50}},color={0,0,255})}));
 end Transformer;

 model M_Transformer
  "Generic transformer with free number of inductors"

 // locally defined classes in M_Transformer
    model _famefaults_p = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n__p = FAME.Bridges.Electrical;

 // components of M_Transformer
  Modelica.SIunits.Voltage v[N];
  Modelica.Electrical.Analog.Interfaces.PositivePin p[N] "Positive pin" annotation(extent=[-80, -40; -62, 40],Placement(transformation(extent={{-80,-40},{-62,40}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p[N](each amount=0.0);
  Modelica.SIunits.Inductance Lm[N,N];
  Modelica.Electrical.Analog.Interfaces.NegativePin n[N] "Negative pin" annotation(extent=[62, -40; 80, 40],Placement(transformation(extent={{62,-40},{80,40}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n[N](each amount=0.0);
  parameter Modelica.SIunits.Inductance L[dimL]={1,0.1,0.2,2,0.3,3} "inductances and coupling inductances";
  parameter Integer N(final min=1)=3 "number of inductors";
  Modelica.SIunits.Current i[N];
  FAME.Bridges.Electrical _famefault_n__p[N,N](each amount=0.0);
 protected
  parameter Integer dimL=div(N*(N+1),2);

 // algorithms and equations of M_Transformer
 algorithm
    for s in 1:N loop
      for z in 1:N loop
        Lm[z,s]:=(if z>=s then L[(s-1)*N+z-div((s-1)*s,2)] else Lm[s,z]);
      end for;
    end for;
 equation
  for j in 1:N loop
   v[j] = _famefault_p[j].port_b.v-_famefault_n[j].port_b.v;
   0 = _famefault_p[j].port_b.i+_famefault_n[j].port_b.i;
   i[j] = _famefault_p[j].port_b.i;
  end for;
  v = Lm*der(i);
  for _fame_lhs_0 in 1:N loop
   connect(p[_fame_lhs_0],_famefault_p[_fame_lhs_0].port_a);
  end for;
  for _fame_lhs_0 in 1:N loop
   connect(n[_fame_lhs_0],_famefault_n[_fame_lhs_0].port_a);
  end for;
  for _fame_lhs_0 in 1:N, _fame_rhs_0 in 1:N loop
   connect(p[_fame_lhs_0],_famefault_n__p[_fame_rhs_0,_fame_lhs_0].port_a);
  end for;
  for _fame_lhs_0 in 1:N, _fame_rhs_0 in 1:N loop
   connect(n[_fame_lhs_0],_famefault_n__p[_fame_lhs_0,_fame_rhs_0].port_b);
  end for;

 annotation(Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics={Ellipse(extent={{-36,24},{-18,42}},lineColor={0,0,255}),Ellipse(extent={{18,24},{36,42}},lineColor={0,0,255}),Ellipse(extent={{0,24},{18,42}},lineColor={0,0,255}),Ellipse(extent={{-18,24},{0,42}},lineColor={0,0,255}),Rectangle(extent={{42,24},{-44,34}},lineColor={0,0,255},pattern=LinePattern.None,fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{-36,34},{-62,34}},color={0,0,255},pattern=LinePattern.None),Line(points={{62,34},{36,34}},color={0,0,255},pattern=LinePattern.None),Ellipse(extent={{-36,8},{-18,26}},lineColor={0,0,255}),Ellipse(extent={{18,8},{36,26}},lineColor={0,0,255}),Ellipse(extent={{0,8},{18,26}},lineColor={0,0,255}),Ellipse(extent={{-18,8},{0,26}},lineColor={0,0,255}),Rectangle(extent={{42,6},{-44,18}},lineColor={0,0,255},pattern=LinePattern.None,fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{-36,18},{-62,18}},color={0,0,255},pattern=LinePattern.None),Line(points={{62,18},{36,18}},color={0,0,255},pattern=LinePattern.None),Line(points={{-36,-29},{-62,-29}},color={0,0,255},pattern=LinePattern.None),Ellipse(extent={{-36,-40},{-18,-22}},lineColor={0,0,255}),Ellipse(extent={{-18,-40},{0,-22}},lineColor={0,0,255}),Ellipse(extent={{0,-40},{18,-22}},lineColor={0,0,255}),Ellipse(extent={{18,-40},{36,-22}},lineColor={0,0,255}),Line(points={{62,-29},{36,-29}},color={0,0,255},pattern=LinePattern.None),Rectangle(extent={{42,-42},{-44,-30}},lineColor={0,0,255},pattern=LinePattern.None,fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{0,8},{0,-18}},color={0,0,255},pattern=LinePattern.Dot),Text(extent={{-150,103},{150,63}},textString="%name",lineColor={0,0,255})},Ellipse(extent=[-36, 24; -18, 42]),Ellipse(extent=[18, 24; 36, 42]),Ellipse(extent=[0, 24; 18, 42]),Ellipse(extent=[-18, 24; 0, 42]),Rectangle(extent=[42, 24; -44, 34],fillColor={255,255,255}),Line(points=[-36, 34; -62, 34]),Line(points=[62, 34; 36, 34]),Ellipse(extent=[-36, 8; -18, 26]),Ellipse(extent=[18, 8; 36, 26]),Ellipse(extent=[0, 8; 18, 26]),Ellipse(extent=[-18, 8; 0, 26]),Rectangle(extent=[42, 6; -44, 18],fillColor={255,255,255}),Line(points=[-36, 18; -62, 18]),Line(points=[62, 18; 36, 18]),Line(points=[-36, -29; -62, -29]),Ellipse(extent=[-36, -40; -18, -22]),Ellipse(extent=[-18, -40; 0, -22]),Ellipse(extent=[0, -40; 18, -22]),Ellipse(extent=[18, -40; 36, -22]),Line(points=[62, -29; 36, -29]),Rectangle(extent=[42, -42; -44, -30],fillColor={255,255,255}),Line(points=[0, 8; 0, -18])),Diagram(coordinateSystem(preserveAspectRatio=false,extent={{-100,-100},{100,100}}),graphics),Documentation(info="<html>
<p>The model <i>M_Transformer</i> is a model of a transformer with the posibility to choose the number of inductors. Inside the model, an inductance matrix is built based on the inductance of the inductors and the coupling inductances between the inductors given as a parameter vector from the user of the model.</p>

<p>An example shows that approach:<br>
The user chooses a model with <b>three</b> inductors, that means the parameter <i><b>N</b></i> has to be <b>3</b>. Then he has to specify the inductances of the three inductors and the three coupling inductances. The coupling inductances are no real existing devices, but effects that occur between two inductors. The inductivities (main diagonal of the inductance matrix) and the coupling inductivities have to be specified in the parameter vector <i>L</i>. The length <i>dimL</i> of the parameter vector is calculated as follows: <i><b>dimL=(N*(N+1))/2</b></i></p>

<p>The following example shows how the parameter vector is used to fill in the inductance matrix. To specify the inductance matrix of a three inductances transformer (<i>N=3</i>):<br>

<img
 src=\"modelica://Modelica/Resources/Images/Electrical/Analog/Basic/M_Transformer-eq.png\"
 alt=\"L_m\">

<br>the user has to allocate the parameter vector <i>L[6] </i>, since <i>Nv=(N*(N+1))/2=(3*(3+1))/2=6</i>. The parameter vector must be filled like this: <i>L=[1,0.1,0.2,2,0.3,3] </i>.</p>
<p>Inside the model, two loops are used to fill the inductance matrix to guarantee that it is filled in a symmetric way.</p>
</html>",revisions="
<html>
<table border=\"1\" cellspacing=\"0\" cellpadding=\"2\">
    <tr>
      <th>Version</th>
      <th>Revision</th>
      <th>Date</th>
      <th>Author</th>
      <th>Comment</th>
    </tr>
   <tr>
      <td valign=\"top\"></td>
      <td valign=\"top\">4163</td>
      <td valign=\"top\">2010-09-11</td>
      <td valign=\"top\">Dietmar Winkler</td>
      <td valign=\"top\">Documentation corrected according to documentation guidelines.</td>
    </tr>
    <tr>
      <td valign=\"top\"></td>
      <td valign=\"top\"></td>
      <td valign=\"top\">2008-11-24</td>
      <td valign=\"top\">Kristin Majetta</td>
      <td valign=\"top\">Documentation added.</td>
    </tr>
    <tr>
      <td valign=\"top\"></td>
      <td valign=\"top\"></td>
      <td valign=\"top\">2008-11-16</td>
      <td valign=\"top\">Kristin Majetta</td>
      <td valign=\"top\">Initially implemented</td>
    </tr>
</table>
</html>
"));
 end M_Transformer;

 model Gyrator
  "Gyrator"

 // locally defined classes in Gyrator
    model _famefaults_n1 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p2 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p1 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n2 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n2__p2 = FAME.Bridges.Electrical;
    model _famefaults_n2__p1 = FAME.Bridges.Electrical;
    model _famefaults_n1__n2 = FAME.Bridges.Electrical;
    model _famefaults_p1__p2 = FAME.Bridges.Electrical;
    model _famefaults_n1__p2 = FAME.Bridges.Electrical;
    model _famefaults_n1__p1 = FAME.Bridges.Electrical;

 // components of Gyrator
  Modelica.SIunits.Voltage v1 "Voltage drop over the left port";
  Modelica.Electrical.Analog.Interfaces.NegativePin n1 "Negative pin of the left port" annotation(Placement(transformation(extent={{-90,-60},{-110,-40}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n1(amount=0.0);
  Modelica.Electrical.Analog.Interfaces.PositivePin p2 "Positive pin of the right port (potential p2.v > n2.v for positive voltage drop v2)" annotation(Placement(transformation(extent={{110,40},{90,60}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p2(amount=0.0);
  parameter Modelica.SIunits.Conductance G2(start=1) "Gyration conductance";
  Modelica.Electrical.Analog.Interfaces.PositivePin p1 "Positive pin of the left port (potential p1.v > n1.v for positive voltage drop v1)" annotation(Placement(transformation(extent={{-110,40},{-90,60}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p1(amount=0.0);
  parameter Modelica.SIunits.Conductance G1(start=1) "Gyration conductance";
  Modelica.SIunits.Voltage v2 "Voltage drop over the right port";
  Modelica.Electrical.Analog.Interfaces.NegativePin n2 "Negative pin of the right port" annotation(Placement(transformation(extent={{90,-60},{110,-40}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n2(amount=0.0);
  Modelica.SIunits.Current i2 "Current flowing from pos. to neg. pin of the right port";
  Modelica.SIunits.Current i1 "Current flowing from pos. to neg. pin of the left port";
  FAME.Bridges.Electrical _famefault_n2__p2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n2__p1(amount=0.0);
  FAME.Bridges.Electrical _famefault_n1__n2(amount=0.0);
  FAME.Bridges.Electrical _famefault_p1__p2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n1__p2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n1__p1(amount=0.0);

 // algorithms and equations of Gyrator
 equation
  i1 = G2*v2;
  i2 = (-G1)*v1;
  v1 = _famefault_p1.port_b.v-_famefault_n1.port_b.v;
  v2 = _famefault_p2.port_b.v-_famefault_n2.port_b.v;
  0 = _famefault_p1.port_b.i+_famefault_n1.port_b.i;
  0 = _famefault_p2.port_b.i+_famefault_n2.port_b.i;
  i1 = _famefault_p1.port_b.i;
  i2 = _famefault_p2.port_b.i;
  connect(n1,_famefault_n1.port_a);
  connect(p2,_famefault_p2.port_a);
  connect(p1,_famefault_p1.port_a);
  connect(n2,_famefault_n2.port_a);
  connect(n2,_famefault_n2__p2.port_a);
  connect(p2,_famefault_n2__p2.port_b);
  connect(n2,_famefault_n2__p1.port_a);
  connect(p1,_famefault_n2__p1.port_b);
  connect(n2,_famefault_n1__n2.port_a);
  connect(n1,_famefault_n1__n2.port_b);
  connect(p2,_famefault_p1__p2.port_a);
  connect(p1,_famefault_p1__p2.port_b);
  connect(p2,_famefault_n1__p2.port_a);
  connect(n1,_famefault_n1__p2.port_b);
  connect(p1,_famefault_n1__p1.port_a);
  connect(n1,_famefault_n1__p1.port_b);

 annotation(Documentation(info="<html>
<p>A gyrator is a two-port element defined by the following equations:</p>
<pre>    i1 =  G2 * v2
    i2 = -G1 * v1</pre>
<p>where the constants <i>G1</i>, <i>G2</i> are called the gyration conductance.</p>
</html>",revisions="<html>
<ul>
<li><i> 1998   </i>
       by Christoph Clauss<br> initially implemented<br>
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Rectangle(extent={{-60,60},{60,-60}},fillColor={255,255,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{-90,50},{-60,50}},color={0,0,255}),Line(points={{-90,-50},{-60,-50}},color={0,0,255}),Line(points={{60,50},{90,50}},color={0,0,255}),Line(points={{60,-50},{90,-50}},color={0,0,255}),Line(points={{-40,30},{40,30}},color={0,0,255}),Line(points={{-20,-20},{20,-20}},color={0,0,255}),Polygon(points={{30,34},{40,30},{30,26},{30,34}},fillColor={0,0,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Polygon(points={{-26,-19},{-16,-15},{-16,-23},{-26,-19}},fillColor={0,0,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{-5,10},{-10,-10}},color={0,0,0}),Line(points={{9,10},{4,-9}},color={0,0,0}),Line(points={{-14,9},{16,10}},color={0,0,0}),Text(extent={{-29,59},{30,30}},textString="G1",lineColor={0,0,0}),Text(extent={{-29,-29},{29,-58}},textString="G2",lineColor={0,0,0}),Text(extent={{-156,117},{144,77}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Rectangle(extent={{-60,60},{60,-60}},lineColor={0,0,255}),Line(points={{-96,50},{-60,50}},color={0,0,255}),Line(points={{-96,-50},{-60,-50}},color={0,0,255}),Line(points={{60,50},{96,50}},color={0,0,255}),Line(points={{60,-50},{96,-50}},color={0,0,255}),Line(points={{-40,30},{40,30}},color={0,0,255}),Line(points={{-21,-22},{19,-22}},color={0,0,255}),Polygon(points={{30,34},{40,30},{30,26},{30,34}},fillColor={0,0,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Polygon(points={{-20,-30},{-10,-26},{-10,-34},{-20,-30}},fillColor={0,0,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{-4,5},{-6,-5}},thickness=1,color={0,0,255}),Line(points={{3,5},{1,-5}},thickness=1,color={0,0,255}),Line(points={{-8,5},{7,5}},thickness=1,color={0,0,255}),Text(extent={{-20,50},{20,35}},textString="G1",lineColor={0,0,255}),Text(extent={{-20,-35},{20,-50}},textString="G2",lineColor={0,0,255})}));
 end Gyrator;

 model EMF
  "Electromotoric force (electric/mechanic transformer)"

 // locally defined classes in EMF
    model _famefaults_support = FAME.Dampers.RotationalWithConnectEquations;
    model _famefaults_p = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_flange = FAME.Dampers.RotationalWithoutConnectEquations;
    model _famefaults_n = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n__p = FAME.Bridges.Electrical;

 // components of EMF
  Modelica.SIunits.AngularVelocity w "Angular velocity of flange relative to support";
  parameter Boolean useSupport=false "= true, if support flange enabled, otherwise implicitly grounded" annotation(Evaluate=true,HideResult=true,choices(__Dymola_checkBox=true));
  Modelica.SIunits.Voltage v "Voltage drop between the two pins";
  Modelica.Mechanics.Rotational.Interfaces.Support support if useSupport "Support/housing of emf shaft" annotation(Placement(transformation(extent={{-110,-10},{-90,10}})));
  FAME.Dampers.RotationalWithConnectEquations _famefault_support(amount=0.0) if useSupport;
  Modelica.Electrical.Analog.Interfaces.PositivePin p annotation(Placement(transformation(origin={0,100},extent={{-10,-10},{10,10}},rotation=90)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p(amount=0.0);
  Modelica.Mechanics.Rotational.Interfaces.Flange_b flange annotation(Placement(transformation(extent={{90,-10},{110,10}},rotation=0)));
  FAME.Dampers.RotationalWithoutConnectEquations _famefault_flange(amount=0.0);
  Modelica.Electrical.Analog.Interfaces.NegativePin n annotation(Placement(transformation(origin={0,-100},extent={{-10,-10},{10,10}},rotation=90)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n(amount=0.0);
  parameter Modelica.SIunits.ElectricalTorqueConstant k(start=1) "Transformation coefficient";
  Modelica.SIunits.Angle phi "Angle of shaft flange with respect to support (= flange.phi - support.phi)";
  Modelica.SIunits.Current i "Current flowing from positive to negative pin";
  FAME.Bridges.Electrical _famefault_n__p(amount=0.0);
 protected
  Modelica.Mechanics.Rotational.Interfaces.InternalSupport internalSupport(tau=-flange.tau) annotation(Placement(transformation(extent={{-90,-10},{-70,10}})));
  Modelica.Mechanics.Rotational.Components.Fixed fixed if not useSupport annotation(Placement(transformation(extent={{-90,-20},{-70,0}})));

 // algorithms and equations of EMF
 equation
  v = _famefault_p.port_b.v-_famefault_n.port_b.v;
  0 = _famefault_p.port_b.i+_famefault_n.port_b.i;
  i = _famefault_p.port_b.i;
  phi = _famefault_flange.port_b.phi-internalSupport.phi;
  w = der(phi);
  k*w = v;
  _famefault_flange.port_b.tau = (-k)*i;
  connect(internalSupport.flange,_famefault_support.port_b) annotation(Line(points={{-80,0},{-100,0}},color={0,0,0},smooth=Smooth.None));
  connect(internalSupport.flange,fixed.flange) annotation(Line(points={{-80,0},{-80,-10}},color={0,0,0},smooth=Smooth.None));
  connect(support,_famefault_support.port_a);
  connect(p,_famefault_p.port_a);
  connect(flange,_famefault_flange.port_a);
  connect(n,_famefault_n.port_a);
  connect(p,_famefault_n__p.port_a);
  connect(n,_famefault_n__p.port_b);

 annotation(defaultComponentName="emf",Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Rectangle(extent={{-85,10},{-36,-10}},lineColor={0,0,0},fillPattern=FillPattern.HorizontalCylinder,fillColor={192,192,192}),Line(points={{0,90},{0,40}},color={0,0,255}),Rectangle(extent={{35,10},{100,-10}},lineColor={0,0,0},fillPattern=FillPattern.HorizontalCylinder,fillColor={192,192,192}),Ellipse(extent={{-40,40},{40,-40}},fillColor={255,255,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{0,-90},{0,-40}},color={0,0,255}),Text(extent={{0,-50},{199,-90}},textString="%name",lineColor={0,0,255}),Text(extent={{0,80},{189,46}},lineColor={160,160,164},textString="k=%k"),Line(visible=not useSupport,points={{-100,-30},{-40,-30}},color={0,0,0}),Line(visible=not useSupport,points={{-100,-50},{-80,-30}},color={0,0,0}),Line(visible=not useSupport,points={{-80,-50},{-60,-30}},color={0,0,0}),Line(visible=not useSupport,points={{-60,-50},{-40,-30}},color={0,0,0}),Line(visible=not useSupport,points={{-70,-30},{-70,-10}},color={0,0,0})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Polygon(points={{-17,95},{-20,85},{-23,95},{-17,95}},lineColor={160,160,164},fillColor={160,160,164},fillPattern=FillPattern.Solid),Line(points={{-20,110},{-20,85}},color={160,160,164}),Text(extent={{-40,110},{-30,90}},lineColor={160,160,164},textString="i"),Line(points={{9,75},{19,75}},color={192,192,192}),Line(points={{-20,-110},{-20,-85}},color={160,160,164}),Polygon(points={{-17,-100},{-20,-110},{-23,-100},{-17,-100}},lineColor={160,160,164},fillColor={160,160,164},fillPattern=FillPattern.Solid),Text(extent={{-40,-110},{-30,-90}},lineColor={160,160,164},textString="i"),Line(points={{8,-79},{18,-79}},color={192,192,192}),Line(points={{14,80},{14,70}},color={192,192,192})}),Documentation(info="<html>
<p>EMF transforms electrical energy into rotational mechanical energy. It is used as basic building block of an electrical motor. The mechanical connector flange can be connected to elements of the Modelica.Mechanics.Rotational library. flange.tau is the cut-torque, flange.phi is the angle at the rotational connection.</p>
</html>",revisions="<html>
<ul>
<li><i> 1998   </i>
       by Martin Otter<br> initially implemented<br>
       </li>
</ul>
</html>"));
 end EMF;

 model TranslationalEMF
  "Electromotoric force (electric/mechanic transformer)"

 // locally defined classes in TranslationalEMF
    model _famefaults_support = FAME.Dampers.TranslationalWithConnectEquations;
    model _famefaults_p = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_flange = FAME.Dampers.TranslationalWithoutConnectEquations;
    model _famefaults_n = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n__p = FAME.Bridges.Electrical;

 // components of TranslationalEMF
  parameter Boolean useSupport=false "= true, if support flange enabled, otherwise implicitly grounded" annotation(Evaluate=true,HideResult=true,choices(__Dymola_checkBox=true));
  Modelica.SIunits.Voltage v "Voltage drop between the two pins";
  Modelica.Mechanics.Translational.Interfaces.Support support if useSupport "Support/housing" annotation(Placement(transformation(extent={{-110,-10},{-90,10}})));
  FAME.Dampers.TranslationalWithConnectEquations _famefault_support(amount=0.0) if useSupport;
  Modelica.SIunits.Position s "Position of flange relative to support";
  Modelica.SIunits.Velocity vel "Velocity of flange relative to support";
  Modelica.Electrical.Analog.Interfaces.PositivePin p annotation(Placement(transformation(origin={0,100},extent={{-10,-10},{10,10}},rotation=90)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p(amount=0.0);
  Modelica.Mechanics.Translational.Interfaces.Flange_b flange annotation(Placement(transformation(extent={{90,-10},{110,10}},rotation=0)));
  FAME.Dampers.TranslationalWithoutConnectEquations _famefault_flange(amount=0.0);
  Modelica.Electrical.Analog.Interfaces.NegativePin n annotation(Placement(transformation(origin={0,-100},extent={{-10,-10},{10,10}},rotation=90)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n(amount=0.0);
  parameter Modelica.SIunits.ElectricalForceConstant k(start=1) "Transformation coefficient";
  Modelica.SIunits.Current i "Current flowing from positive to negative pin";
  FAME.Bridges.Electrical _famefault_n__p(amount=0.0);
 protected
  Modelica.Mechanics.Translational.Interfaces.InternalSupport internalSupport(f=-flange.f) annotation(Placement(transformation(extent={{-90,-10},{-70,10}})));
  Modelica.Mechanics.Translational.Components.Fixed fixed if not useSupport annotation(Placement(transformation(extent={{-90,-20},{-70,0}})));

 // algorithms and equations of TranslationalEMF
 equation
  v = _famefault_p.port_b.v-_famefault_n.port_b.v;
  0 = _famefault_p.port_b.i+_famefault_n.port_b.i;
  i = _famefault_p.port_b.i;
  s = _famefault_flange.port_b.s-internalSupport.s;
  vel = der(s);
  k*vel = v;
  _famefault_flange.port_b.f = (-k)*i;
  connect(internalSupport.flange,_famefault_support.port_b) annotation(Line(points={{-80,0},{-90,0},{-90,0},{-100,0}},color={0,127,0},smooth=Smooth.None));
  connect(internalSupport.flange,fixed.flange) annotation(Line(points={{-80,0},{-80,-10}},color={0,127,0},smooth=Smooth.None));
  connect(support,_famefault_support.port_a);
  connect(p,_famefault_p.port_a);
  connect(flange,_famefault_flange.port_a);
  connect(n,_famefault_n.port_a);
  connect(p,_famefault_n__p.port_a);
  connect(n,_famefault_n__p.port_b);

 annotation(defaultComponentName="emf",Icon(coordinateSystem(preserveAspectRatio=false,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Rectangle(extent={{-90,51},{-40,-50}},lineColor={135,135,135},fillColor={135,135,135},fillPattern=FillPattern.HorizontalCylinder),Rectangle(extent={{-21,20},{90,-20}},lineColor={135,135,135},fillColor={135,135,135},fillPattern=FillPattern.Solid),Text(extent={{0,80},{148,44}},lineColor={160,160,164},textString="k=%k"),Line(points={{-30,49},{-30,80},{0,80},{0,91}},color={0,0,255}),Line(points={{20,-49},{20,-80},{0,-80},{0,-89},{0,-90}},color={0,0,255}),Ellipse(extent={{-21,50},{9,-50}},lineColor={0,0,255}),Ellipse(extent={{2,50},{32,-50}},lineColor={0,0,255}),Ellipse(extent={{-43,50},{-13,-50}},lineColor={0,0,255}),Rectangle(extent={{-4,20},{-1,-20}},lineColor={135,135,135},fillColor={135,135,135},fillPattern=FillPattern.Solid),Rectangle(extent={{7,20},{10,-20}},lineColor={135,135,135},fillColor={135,135,135},fillPattern=FillPattern.Solid),Rectangle(extent={{-14,20},{-11,-20}},lineColor={135,135,135},fillColor={135,135,135},fillPattern=FillPattern.Solid),Rectangle(extent={{19,20},{44,-20}},lineColor={135,135,135},fillColor={135,135,135},fillPattern=FillPattern.Solid),Line(visible=not useSupport,points={{-100,-70},{-40,-70}},color={0,0,0}),Line(visible=not useSupport,points={{-100,-90},{-80,-70}},color={0,0,0}),Line(visible=not useSupport,points={{-80,-90},{-60,-70}},color={0,0,0}),Line(visible=not useSupport,points={{-60,-90},{-40,-70}},color={0,0,0}),Line(visible=not useSupport,points={{-70,-70},{-70,-50}},color={0,0,0}),Text(extent={{0,-50},{199,-90}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Polygon(points={{-17,95},{-20,85},{-23,95},{-17,95}},lineColor={160,160,164},fillColor={160,160,164},fillPattern=FillPattern.Solid),Line(points={{-20,110},{-20,85}},color={160,160,164}),Text(extent={{-40,110},{-30,90}},lineColor={160,160,164},textString="i"),Line(points={{9,75},{19,75}},color={192,192,192}),Line(points={{-20,-110},{-20,-85}},color={160,160,164}),Polygon(points={{-17,-100},{-20,-110},{-23,-100},{-17,-100}},lineColor={160,160,164},fillColor={160,160,164},fillPattern=FillPattern.Solid),Text(extent={{-40,-110},{-30,-90}},lineColor={160,160,164},textString="i"),Line(points={{8,-79},{18,-79}},color={192,192,192}),Line(points={{14,80},{14,70}},color={192,192,192}),Polygon(points={{140,3},{150,0},{140,-3},{140,3},{140,3}},lineColor={0,0,0},fillColor={0,0,0},fillPattern=FillPattern.Solid)}),Window(x=0.21,y=0,width=0.65,height=0.66),Documentation(info="<html>
<p>EMF transforms electrical energy into translational mechanical energy. It is used as basic building block of an electrical linear motor. The mechanical connector flange can be connected to elements of the Modelica.Mechanics.Translational library. flange.f is the cut-force, flange.s is the distance at the translational connection.</p>
</html>",revisions="<html>
<ul>
<li><i>  </i>
       </li>
<li><i> 2009   </i>
       by Anton Haumer<br> initially implemented<br>
       </li>
</ul>
</html>"));
 end TranslationalEMF;

 model VCV
  "Linear voltage-controlled voltage source"

 // locally defined classes in VCV
    model _famefaults_n1 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p2 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p1 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n2 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p1__p2 = FAME.Bridges.Electrical;
    model _famefaults_n2__p2 = FAME.Bridges.Electrical;
    model _famefaults_n1__p2 = FAME.Bridges.Electrical;
    model _famefaults_n2__p1 = FAME.Bridges.Electrical;
    model _famefaults_n1__p1 = FAME.Bridges.Electrical;
    model _famefaults_n1__n2 = FAME.Bridges.Electrical;

 // components of VCV
  Modelica.SIunits.Voltage v1 "Voltage drop over the left port";
  Modelica.Electrical.Analog.Interfaces.NegativePin n1 "Negative pin of the left port" annotation(Placement(transformation(extent={{-90,-60},{-110,-40}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n1(amount=0.0);
  Modelica.Electrical.Analog.Interfaces.PositivePin p2 "Positive pin of the right port (potential p2.v > n2.v for positive voltage drop v2)" annotation(Placement(transformation(extent={{110,40},{90,60}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p2(amount=0.0);
  Modelica.Electrical.Analog.Interfaces.PositivePin p1 "Positive pin of the left port (potential p1.v > n1.v for positive voltage drop v1)" annotation(Placement(transformation(extent={{-110,40},{-90,60}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p1(amount=0.0);
  Modelica.SIunits.Voltage v2 "Voltage drop over the right port";
  Modelica.Electrical.Analog.Interfaces.NegativePin n2 "Negative pin of the right port" annotation(Placement(transformation(extent={{90,-60},{110,-40}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n2(amount=0.0);
  Modelica.SIunits.Current i2 "Current flowing from pos. to neg. pin of the right port";
  parameter Real gain(start=1) "Voltage gain";
  Modelica.SIunits.Current i1 "Current flowing from pos. to neg. pin of the left port";
  FAME.Bridges.Electrical _famefault_p1__p2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n2__p2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n1__p2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n2__p1(amount=0.0);
  FAME.Bridges.Electrical _famefault_n1__p1(amount=0.0);
  FAME.Bridges.Electrical _famefault_n1__n2(amount=0.0);

 // algorithms and equations of VCV
 equation
  v2 = v1*gain;
  i1 = 0;
  v1 = _famefault_p1.port_b.v-_famefault_n1.port_b.v;
  v2 = _famefault_p2.port_b.v-_famefault_n2.port_b.v;
  0 = _famefault_p1.port_b.i+_famefault_n1.port_b.i;
  0 = _famefault_p2.port_b.i+_famefault_n2.port_b.i;
  i1 = _famefault_p1.port_b.i;
  i2 = _famefault_p2.port_b.i;
  connect(n1,_famefault_n1.port_a);
  connect(p2,_famefault_p2.port_a);
  connect(p1,_famefault_p1.port_a);
  connect(n2,_famefault_n2.port_a);
  connect(p2,_famefault_p1__p2.port_a);
  connect(p1,_famefault_p1__p2.port_b);
  connect(p2,_famefault_n2__p2.port_a);
  connect(n2,_famefault_n2__p2.port_b);
  connect(p2,_famefault_n1__p2.port_a);
  connect(n1,_famefault_n1__p2.port_b);
  connect(p1,_famefault_n2__p1.port_a);
  connect(n2,_famefault_n2__p1.port_b);
  connect(p1,_famefault_n1__p1.port_a);
  connect(n1,_famefault_n1__p1.port_b);
  connect(n2,_famefault_n1__n2.port_a);
  connect(n1,_famefault_n1__n2.port_b);

 annotation(Documentation(info="<html>
<p>The linear voltage-controlled voltage source is a TwoPort. The right port voltage v2 is controlled by the left port voltage v1 via</p>
<pre>    v2 = v1 * gain. </pre>
<p>The left port current is zero. Any voltage gain can be chosen.</p>
</html>",revisions="<html>
<ul>
<li><i> 1998   </i>
       by Christoph Clauss<br> initially implemented<br>
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Rectangle(extent={{-70,70},{70,-70}},fillColor={255,255,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{-90,50},{-30,50}},color={0,0,255}),Line(points={{-30,-50},{-90,-50}},color={0,0,255}),Line(points={{100,50},{30,50},{30,-50},{100,-50}},color={0,0,255}),Ellipse(extent={{10,20},{50,-20}},lineColor={0,0,255}),Line(points={{-20,60},{20,60}},color={0,0,255}),Polygon(points={{20,60},{10,63},{10,57},{20,60}},fillColor={0,0,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Text(extent={{-148,-81},{152,-121}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Line(points={{-96,50},{-31,50}},color={0,0,255}),Line(points={{-30,-50},{-96,-50}},color={0,0,255}),Line(points={{96,50},{30,50},{30,-50},{96,-50}},color={0,0,255}),Ellipse(extent={{10,20},{50,-20}},lineColor={0,0,255}),Rectangle(extent={{-70,70},{70,-70}},lineColor={0,0,255}),Line(points={{-20,60},{20,60}},color={0,0,255}),Polygon(points={{20,60},{10,63},{10,57},{20,60}},fillColor={0,0,255},fillPattern=FillPattern.Solid,lineColor={0,0,255})}));
 end VCV;

 model VCC
  "Linear voltage-controlled current source"

 // locally defined classes in VCC
    model _famefaults_n1 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p2 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p1 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n2 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n1__p1 = FAME.Bridges.Electrical;
    model _famefaults_n2__p1 = FAME.Bridges.Electrical;
    model _famefaults_p1__p2 = FAME.Bridges.Electrical;
    model _famefaults_n1__n2 = FAME.Bridges.Electrical;
    model _famefaults_n1__p2 = FAME.Bridges.Electrical;
    model _famefaults_n2__p2 = FAME.Bridges.Electrical;

 // components of VCC
  Modelica.SIunits.Voltage v1 "Voltage drop over the left port";
  Modelica.Electrical.Analog.Interfaces.NegativePin n1 "Negative pin of the left port" annotation(Placement(transformation(extent={{-90,-60},{-110,-40}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n1(amount=0.0);
  parameter Modelica.SIunits.Conductance transConductance(start=1) "Transconductance";
  Modelica.Electrical.Analog.Interfaces.PositivePin p2 "Positive pin of the right port (potential p2.v > n2.v for positive voltage drop v2)" annotation(Placement(transformation(extent={{110,40},{90,60}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p2(amount=0.0);
  Modelica.Electrical.Analog.Interfaces.PositivePin p1 "Positive pin of the left port (potential p1.v > n1.v for positive voltage drop v1)" annotation(Placement(transformation(extent={{-110,40},{-90,60}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p1(amount=0.0);
  Modelica.SIunits.Voltage v2 "Voltage drop over the right port";
  Modelica.Electrical.Analog.Interfaces.NegativePin n2 "Negative pin of the right port" annotation(Placement(transformation(extent={{90,-60},{110,-40}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n2(amount=0.0);
  Modelica.SIunits.Current i2 "Current flowing from pos. to neg. pin of the right port";
  Modelica.SIunits.Current i1 "Current flowing from pos. to neg. pin of the left port";
  FAME.Bridges.Electrical _famefault_n1__p1(amount=0.0);
  FAME.Bridges.Electrical _famefault_n2__p1(amount=0.0);
  FAME.Bridges.Electrical _famefault_p1__p2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n1__n2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n1__p2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n2__p2(amount=0.0);

 // algorithms and equations of VCC
 equation
  i2 = v1*transConductance;
  i1 = 0;
  v1 = _famefault_p1.port_b.v-_famefault_n1.port_b.v;
  v2 = _famefault_p2.port_b.v-_famefault_n2.port_b.v;
  0 = _famefault_p1.port_b.i+_famefault_n1.port_b.i;
  0 = _famefault_p2.port_b.i+_famefault_n2.port_b.i;
  i1 = _famefault_p1.port_b.i;
  i2 = _famefault_p2.port_b.i;
  connect(n1,_famefault_n1.port_a);
  connect(p2,_famefault_p2.port_a);
  connect(p1,_famefault_p1.port_a);
  connect(n2,_famefault_n2.port_a);
  connect(p1,_famefault_n1__p1.port_a);
  connect(n1,_famefault_n1__p1.port_b);
  connect(p1,_famefault_n2__p1.port_a);
  connect(n2,_famefault_n2__p1.port_b);
  connect(p1,_famefault_p1__p2.port_a);
  connect(p2,_famefault_p1__p2.port_b);
  connect(n1,_famefault_n1__n2.port_a);
  connect(n2,_famefault_n1__n2.port_b);
  connect(n1,_famefault_n1__p2.port_a);
  connect(p2,_famefault_n1__p2.port_b);
  connect(n2,_famefault_n2__p2.port_a);
  connect(p2,_famefault_n2__p2.port_b);

 annotation(Documentation(info="<html>
<p>The linear voltage-controlled current source is a TwoPort. The right port current i2 is controlled by the left port voltage v1 via</p>
<pre>    i2 = v1 * transConductance. </pre>
<p>The left port current is zero. Any transConductance can be chosen.</p>
</html>",revisions="<html>
<ul>
<li><i> 1998   </i>
       by Christoph Clauss<br> initially implemented<br>
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Rectangle(extent={{-70,70},{70,-70}},fillColor={255,255,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{-90,50},{-30,50}},color={0,0,255}),Line(points={{-30,-50},{-90,-50}},color={0,0,255}),Ellipse(extent={{10,20},{50,-20}},lineColor={0,0,255}),Line(points={{-20,60},{20,60}},color={0,0,255}),Polygon(points={{20,60},{10,63},{10,57},{20,60}},fillColor={0,0,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{90,50},{30,50},{30,20}},color={0,0,255}),Line(points={{91,-50},{30,-50},{30,-20}},color={0,0,255}),Line(points={{10,0},{50,0}},color={0,0,255}),Text(extent={{-143,-82},{157,-122}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Line(points={{-96,50},{-30,50}},color={0,0,255}),Line(points={{-30,-50},{-96,-50}},color={0,0,255}),Ellipse(extent={{10,20},{50,-20}},lineColor={0,0,255}),Rectangle(extent={{-70,70},{70,-70}},lineColor={0,0,255}),Line(points={{-20,60},{20,60}},color={0,0,255}),Polygon(points={{20,60},{10,63},{10,57},{20,60}},fillColor={0,0,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{96,50},{30,50},{30,20}},color={0,0,255}),Line(points={{96,-50},{30,-50},{30,-20}},color={0,0,255}),Line(points={{10,0},{50,0}},color={0,0,255})}));
 end VCC;

 model CCV
  "Linear current-controlled voltage source"

 // locally defined classes in CCV
    model _famefaults_n1 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p2 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p1 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n2 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n1__n2 = FAME.Bridges.Electrical;
    model _famefaults_n2__p2 = FAME.Bridges.Electrical;
    model _famefaults_n2__p1 = FAME.Bridges.Electrical;
    model _famefaults_n1__p2 = FAME.Bridges.Electrical;
    model _famefaults_n1__p1 = FAME.Bridges.Electrical;
    model _famefaults_p1__p2 = FAME.Bridges.Electrical;

 // components of CCV
  Modelica.SIunits.Voltage v1 "Voltage drop over the left port";
  Modelica.Electrical.Analog.Interfaces.NegativePin n1 "Negative pin of the left port" annotation(Placement(transformation(extent={{-90,-60},{-110,-40}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n1(amount=0.0);
  parameter Modelica.SIunits.Resistance transResistance(start=1) "Transresistance";
  Modelica.Electrical.Analog.Interfaces.PositivePin p2 "Positive pin of the right port (potential p2.v > n2.v for positive voltage drop v2)" annotation(Placement(transformation(extent={{110,40},{90,60}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p2(amount=0.0);
  Modelica.Electrical.Analog.Interfaces.PositivePin p1 "Positive pin of the left port (potential p1.v > n1.v for positive voltage drop v1)" annotation(Placement(transformation(extent={{-110,40},{-90,60}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p1(amount=0.0);
  Modelica.SIunits.Voltage v2 "Voltage drop over the right port";
  Modelica.Electrical.Analog.Interfaces.NegativePin n2 "Negative pin of the right port" annotation(Placement(transformation(extent={{90,-60},{110,-40}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n2(amount=0.0);
  Modelica.SIunits.Current i2 "Current flowing from pos. to neg. pin of the right port";
  Modelica.SIunits.Current i1 "Current flowing from pos. to neg. pin of the left port";
  FAME.Bridges.Electrical _famefault_n1__n2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n2__p2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n2__p1(amount=0.0);
  FAME.Bridges.Electrical _famefault_n1__p2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n1__p1(amount=0.0);
  FAME.Bridges.Electrical _famefault_p1__p2(amount=0.0);

 // algorithms and equations of CCV
 equation
  v2 = i1*transResistance;
  v1 = 0;
  v1 = _famefault_p1.port_b.v-_famefault_n1.port_b.v;
  v2 = _famefault_p2.port_b.v-_famefault_n2.port_b.v;
  0 = _famefault_p1.port_b.i+_famefault_n1.port_b.i;
  0 = _famefault_p2.port_b.i+_famefault_n2.port_b.i;
  i1 = _famefault_p1.port_b.i;
  i2 = _famefault_p2.port_b.i;
  connect(n1,_famefault_n1.port_a);
  connect(p2,_famefault_p2.port_a);
  connect(p1,_famefault_p1.port_a);
  connect(n2,_famefault_n2.port_a);
  connect(n2,_famefault_n1__n2.port_a);
  connect(n1,_famefault_n1__n2.port_b);
  connect(n2,_famefault_n2__p2.port_a);
  connect(p2,_famefault_n2__p2.port_b);
  connect(n2,_famefault_n2__p1.port_a);
  connect(p1,_famefault_n2__p1.port_b);
  connect(n1,_famefault_n1__p2.port_a);
  connect(p2,_famefault_n1__p2.port_b);
  connect(n1,_famefault_n1__p1.port_a);
  connect(p1,_famefault_n1__p1.port_b);
  connect(p2,_famefault_p1__p2.port_a);
  connect(p1,_famefault_p1__p2.port_b);

 annotation(Documentation(info="<html>
<p>The linear current-controlled voltage source is a TwoPort. The right port voltage v2 is controlled by the left port current i1 via</p>
<pre>    v2 = i1 * transResistance. </pre>
<p>The left port voltage is zero. Any transResistance can be chosen.</p>
</html>",revisions="<html>
<ul>
<li><i> 1998   </i>
       by Christoph Clauss<br> initially implemented<br>
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Rectangle(extent={{-70,70},{70,-70}},fillColor={255,255,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{100,50},{30,50},{30,-50},{100,-50}},color={0,0,255}),Ellipse(extent={{10,20},{50,-20}},lineColor={0,0,255}),Line(points={{-20,60},{20,60}},color={0,0,255}),Polygon(points={{20,60},{10,63},{10,57},{20,60}},fillColor={0,0,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{-90,50},{-20,50},{-20,-50},{-90,-50}},color={0,0,255}),Text(extent={{-140,-82},{160,-122}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Ellipse(extent={{10,20},{50,-20}},lineColor={0,0,255}),Rectangle(extent={{-70,70},{70,-70}},lineColor={0,0,255}),Line(points={{-20,60},{20,60}},color={0,0,255}),Polygon(points={{20,60},{10,63},{10,57},{20,60}},fillColor={0,0,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{96,50},{30,50},{30,-50},{96,-50}},color={0,0,255}),Line(points={{-96,50},{-30,50},{-30,-50},{-96,-50}},color={0,0,255})}));
 end CCV;

 model CCC
  "Linear current-controlled current source"

 // locally defined classes in CCC
    model _famefaults_n1 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p2 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p1 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n2 = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n1__p1 = FAME.Bridges.Electrical;
    model _famefaults_p1__p2 = FAME.Bridges.Electrical;
    model _famefaults_n2__p1 = FAME.Bridges.Electrical;
    model _famefaults_n1__p2 = FAME.Bridges.Electrical;
    model _famefaults_n1__n2 = FAME.Bridges.Electrical;
    model _famefaults_n2__p2 = FAME.Bridges.Electrical;

 // components of CCC
  Modelica.SIunits.Voltage v1 "Voltage drop over the left port";
  Modelica.Electrical.Analog.Interfaces.NegativePin n1 "Negative pin of the left port" annotation(Placement(transformation(extent={{-90,-60},{-110,-40}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n1(amount=0.0);
  Modelica.Electrical.Analog.Interfaces.PositivePin p2 "Positive pin of the right port (potential p2.v > n2.v for positive voltage drop v2)" annotation(Placement(transformation(extent={{110,40},{90,60}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p2(amount=0.0);
  Modelica.Electrical.Analog.Interfaces.PositivePin p1 "Positive pin of the left port (potential p1.v > n1.v for positive voltage drop v1)" annotation(Placement(transformation(extent={{-110,40},{-90,60}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p1(amount=0.0);
  Modelica.SIunits.Voltage v2 "Voltage drop over the right port";
  Modelica.Electrical.Analog.Interfaces.NegativePin n2 "Negative pin of the right port" annotation(Placement(transformation(extent={{90,-60},{110,-40}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n2(amount=0.0);
  Modelica.SIunits.Current i2 "Current flowing from pos. to neg. pin of the right port";
  parameter Real gain(start=1) "Current gain";
  Modelica.SIunits.Current i1 "Current flowing from pos. to neg. pin of the left port";
  FAME.Bridges.Electrical _famefault_n1__p1(amount=0.0);
  FAME.Bridges.Electrical _famefault_p1__p2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n2__p1(amount=0.0);
  FAME.Bridges.Electrical _famefault_n1__p2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n1__n2(amount=0.0);
  FAME.Bridges.Electrical _famefault_n2__p2(amount=0.0);

 // algorithms and equations of CCC
 equation
  i2 = i1*gain;
  v1 = 0;
  v1 = _famefault_p1.port_b.v-_famefault_n1.port_b.v;
  v2 = _famefault_p2.port_b.v-_famefault_n2.port_b.v;
  0 = _famefault_p1.port_b.i+_famefault_n1.port_b.i;
  0 = _famefault_p2.port_b.i+_famefault_n2.port_b.i;
  i1 = _famefault_p1.port_b.i;
  i2 = _famefault_p2.port_b.i;
  connect(n1,_famefault_n1.port_a);
  connect(p2,_famefault_p2.port_a);
  connect(p1,_famefault_p1.port_a);
  connect(n2,_famefault_n2.port_a);
  connect(p1,_famefault_n1__p1.port_a);
  connect(n1,_famefault_n1__p1.port_b);
  connect(p1,_famefault_p1__p2.port_a);
  connect(p2,_famefault_p1__p2.port_b);
  connect(p1,_famefault_n2__p1.port_a);
  connect(n2,_famefault_n2__p1.port_b);
  connect(n1,_famefault_n1__p2.port_a);
  connect(p2,_famefault_n1__p2.port_b);
  connect(n1,_famefault_n1__n2.port_a);
  connect(n2,_famefault_n1__n2.port_b);
  connect(p2,_famefault_n2__p2.port_a);
  connect(n2,_famefault_n2__p2.port_b);

 annotation(Documentation(info="<html>
<p>The linear current-controlled current source is a TwoPort. The right port current i2 is controlled by the left port current i1 via</p>
<pre>    i2 = i1 * gain. </pre>
<p>The left port voltage is zero. Any current gain can be chosen.</p>
</html>",revisions="<html>
<ul>
<li><i> 1998   </i>
       by Christoph Clauss<br> initially implemented<br>
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Rectangle(extent={{-70,70},{70,-70}},fillColor={255,255,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{-100,50},{-30,50},{-30,-50},{-100,-50}},color={0,0,255}),Ellipse(extent={{10,20},{50,-20}},lineColor={0,0,255}),Line(points={{-20,60},{20,60}},color={0,0,255}),Polygon(points={{20,60},{10,63},{10,57},{20,60}},fillColor={0,0,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{90,50},{30,50},{30,20}},color={0,0,255}),Line(points={{91,-50},{30,-50},{30,-20}},color={0,0,255}),Line(points={{10,0},{50,0}},color={0,0,255}),Text(extent={{-146,-76},{154,-116}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Ellipse(extent={{10,20},{50,-20}},lineColor={0,0,255}),Rectangle(extent={{-70,70},{70,-70}},lineColor={0,0,255}),Line(points={{-20,60},{20,60}},color={0,0,255}),Polygon(points={{20,60},{10,63},{10,57},{20,60}},fillColor={0,0,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{96,50},{30,50},{30,20}},color={0,0,255}),Line(points={{96,-50},{30,-50},{30,-20}},color={0,0,255}),Line(points={{10,0},{50,0}},color={0,0,255}),Line(points={{-96,50},{-30,50},{-30,-50},{-96,-50}},color={0,0,255})}));
 end CCC;

 model OpAmp
  "Simple nonideal model of an OpAmp with limitation"

 // locally defined classes in OpAmp
    model _famefaults_VMin = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_in_n = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_VMax = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_in_p = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_out = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_out__VMin = FAME.Bridges.Electrical;
    model _famefaults_in_p__out = FAME.Bridges.Electrical;
    model _famefaults_in_n__out = FAME.Bridges.Electrical;
    model _famefaults_out__VMax = FAME.Bridges.Electrical;
    model _famefaults_in_p__VMin = FAME.Bridges.Electrical;
    model _famefaults_in_n__VMin = FAME.Bridges.Electrical;
    model _famefaults_VMax__VMin = FAME.Bridges.Electrical;
    model _famefaults_in_n__in_p = FAME.Bridges.Electrical;
    model _famefaults_in_p__VMax = FAME.Bridges.Electrical;
    model _famefaults_in_n__VMax = FAME.Bridges.Electrical;

 // components of OpAmp
  Modelica.SIunits.Voltage vin "input voltagae";
  Modelica.Electrical.Analog.Interfaces.NegativePin VMin "Negative output voltage limitation" annotation(Placement(transformation(extent={{-10,-80},{10,-60}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_VMin(amount=0.0);
  parameter Real Slope(start=1) "Slope of the out.v/vin characteristic at vin=0";
  Modelica.Electrical.Analog.Interfaces.NegativePin in_n "Negative pin of the input port" annotation(Placement(transformation(extent={{-90,40},{-110,60}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_in_n(amount=0.0);
  Modelica.Electrical.Analog.Interfaces.PositivePin VMax "Positive output voltage limitation" annotation(Placement(transformation(extent={{-10,60},{10,80}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_VMax(amount=0.0);
  Modelica.Electrical.Analog.Interfaces.PositivePin in_p "Positive pin of the input port" annotation(Placement(transformation(extent={{-110,-60},{-90,-40}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_in_p(amount=0.0);
  Modelica.Electrical.Analog.Interfaces.PositivePin out "Output pin" annotation(Placement(transformation(extent={{110,-10},{90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_out(amount=0.0);
  FAME.Bridges.Electrical _famefault_out__VMin(amount=0.0);
  FAME.Bridges.Electrical _famefault_in_p__out(amount=0.0);
  FAME.Bridges.Electrical _famefault_in_n__out(amount=0.0);
  FAME.Bridges.Electrical _famefault_out__VMax(amount=0.0);
  FAME.Bridges.Electrical _famefault_in_p__VMin(amount=0.0);
  FAME.Bridges.Electrical _famefault_in_n__VMin(amount=0.0);
  FAME.Bridges.Electrical _famefault_VMax__VMin(amount=0.0);
  FAME.Bridges.Electrical _famefault_in_n__in_p(amount=0.0);
  FAME.Bridges.Electrical _famefault_in_p__VMax(amount=0.0);
  FAME.Bridges.Electrical _famefault_in_n__VMax(amount=0.0);
 protected
  Real f "auxiliary variable";
  Real absSlope;

 // algorithms and equations of OpAmp
 equation
  _famefault_in_p.port_b.i = 0;
  _famefault_in_n.port_b.i = 0;
  _famefault_VMax.port_b.i = 0;
  _famefault_VMin.port_b.i = 0;
  vin = _famefault_in_p.port_b.v-_famefault_in_n.port_b.v;
  f = 2/(_famefault_VMax.port_b.v-_famefault_VMin.port_b.v);
  absSlope = smooth(0,(if Slope<0 then -Slope else Slope));
  _famefault_out.port_b.v = (_famefault_VMax.port_b.v+_famefault_VMin.port_b.v)/(2)+absSlope*vin/(1+absSlope*smooth(0,(if (f*vin)<0 then (-f)*vin else f*vin)));
  connect(VMin,_famefault_VMin.port_a);
  connect(in_n,_famefault_in_n.port_a);
  connect(VMax,_famefault_VMax.port_a);
  connect(in_p,_famefault_in_p.port_a);
  connect(out,_famefault_out.port_a);
  connect(out,_famefault_out__VMin.port_a);
  connect(VMin,_famefault_out__VMin.port_b);
  connect(out,_famefault_in_p__out.port_a);
  connect(in_p,_famefault_in_p__out.port_b);
  connect(out,_famefault_in_n__out.port_a);
  connect(in_n,_famefault_in_n__out.port_b);
  connect(out,_famefault_out__VMax.port_a);
  connect(VMax,_famefault_out__VMax.port_b);
  connect(VMin,_famefault_in_p__VMin.port_a);
  connect(in_p,_famefault_in_p__VMin.port_b);
  connect(VMin,_famefault_in_n__VMin.port_a);
  connect(in_n,_famefault_in_n__VMin.port_b);
  connect(VMin,_famefault_VMax__VMin.port_a);
  connect(VMax,_famefault_VMax__VMin.port_b);
  connect(in_p,_famefault_in_n__in_p.port_a);
  connect(in_n,_famefault_in_n__in_p.port_b);
  connect(in_p,_famefault_in_p__VMax.port_a);
  connect(VMax,_famefault_in_p__VMax.port_b);
  connect(in_n,_famefault_in_n__VMax.port_a);
  connect(VMax,_famefault_in_n__VMax.port_b);

 annotation(Documentation(info="<html>
<p>The OpAmp is a simle nonideal model with a smooth out.v = f(vin) characteristic, where &quot;vin = in_p.v - in_n.v&quot;. The characteristic is limited by VMax.v and VMin.v. Its slope at vin=0 is the parameter Slope, which must be positive. (Therefore, the absolute value of Slope is taken into calculation.)</p>
</html>",revisions="<html>
<ul>
<li><i> 2000   </i>
       by Christoph Clauss<br> initially implemented<br>
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Polygon(points={{60,0},{-60,70},{-60,-70},{60,0}},fillColor={255,255,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{-45,-10},{-20,-10},{-14,-9},{-11,-7},{-9,7},{-6,9},{0,10},{20,10}},color={0,0,255}),Line(points={{0,35},{0,80}},color={0,0,255}),Line(points={{0,-35},{0,-80}},color={0,0,255}),Line(points={{-90,50},{-60,50}},color={0,0,255}),Line(points={{-90,-50},{-60,-50}},color={0,0,255}),Line(points={{60,0},{90,0}},color={0,0,255}),Line(points={{-48,32},{-28,32}},color={0,0,255}),Line(points={{-39,-20},{-39,-41}},color={0,0,255}),Line(points={{-50,-31},{-28,-31}},color={0,0,255}),Text(extent={{-149,138},{151,98}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Polygon(points={{60,0},{-60,70},{-60,-70},{60,0}},fillColor={255,255,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{-45,-10},{-20,-10},{-14,-9},{-11,-7},{-9,7},{-6,9},{0,10},{20,10}},color={0,0,255}),Line(points={{0,35},{0,70}},color={0,0,255}),Line(points={{0,-35},{0,-70}},color={0,0,255}),Line(points={{-96,50},{-60,50}},color={0,0,255}),Line(points={{-96,-50},{-60,-50}},color={0,0,255}),Line(points={{60,0},{96,0}},color={0,0,255}),Line(points={{-55,50},{-45,50}},color={0,0,255}),Line(points={{-50,-45},{-50,-55}},color={0,0,255}),Line(points={{-55,-50},{-45,-50}},color={0,0,255}),Text(extent={{-112,-93},{-91,-73}},lineColor={160,160,164},textString="in_p.i=0"),Polygon(points={{120,3},{110,0},{120,-3},{120,3}},lineColor={0,0,0},fillPattern=FillPattern.HorizontalCylinder,fillColor={160,160,164}),Line(points={{111,0},{136,0}},color={0,0,0}),Text(extent={{118,2},{135,17}},lineColor={0,0,0},textString="i2"),Text(extent={{-111,60},{-85,80}},lineColor={160,160,164},textString="in_n.i=0"),Line(points={{-100,-35},{-100,23},{-100,24}},color={160,160,164},arrow={Arrow.None,Arrow.Filled}),Text(extent={{-97,-16},{-74,4}},lineColor={160,160,164},textString="vin")}));
 end OpAmp;

 model OpAmpDetailed
  "Detailed model of an operational amplifier"

  import SI = Modelica.SIunits;

 // locally defined classes in OpAmpDetailed

  function FCNiout_limit
   "Internal limitation function"

  // components of FCNiout_limit
   input SI.Voltage v_source;
   input SI.Voltage v_out;
   input SI.Resistance Rout;
   input SI.Current Imaxsi_val;
   input SI.Current Imaxso_val;
   output SI.Current result;

  // algorithms and equations of FCNiout_limit
  algorithm
     if v_out>(v_source+Rout*Imaxsi_val) then
       result:=Imaxsi_val;
     elseif v_out<(v_source-Rout*Imaxso_val) then
       result:=-Imaxso_val;
     else
       result:=(v_out-v_source)/(Rout);
     end if;
     return;

  annotation(Documentation(info="<html>
<p>Internal limitation function, designed for OpAmpDetailed, not for purpose of external usage.</p>
</html>"));
  end FCNiout_limit;

  function FCNq_sum_limit
   "Internal limitation function"

  // components of FCNq_sum_limit
   input SI.Voltage q_sum;
   input SI.Voltage q_sum_ltf;
   input SI.Voltage v_pos;
   input SI.Voltage v_neg;
   input SI.Voltage vcp;
   input SI.Voltage vcm;
   output SI.Voltage result;

  // algorithms and equations of FCNq_sum_limit
  algorithm
     if (q_sum>(v_pos-vcp)) and (q_sum_ltf>=(v_pos-vcp)) then
       result:=v_pos-vcp;
     elseif (q_sum<(v_neg+vcm)) and (q_sum_ltf<=(v_neg+vcm)) then
       result:=v_neg+vcm;
     else
       result:=q_sum;
     end if;
     return;

  annotation(Documentation(info="<html>
<p>Internal limitation function, designed for OpAmpDetailed, not for purpose of external usage.</p>
</html>"));
  end FCNq_sum_limit;
    model _famefaults_p_supply = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_m_supply = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_outp = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_m = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_m__outp = FAME.Bridges.Electrical;
    model _famefaults_outp__p_supply = FAME.Bridges.Electrical;
    model _famefaults_m_supply__outp = FAME.Bridges.Electrical;
    model _famefaults_outp__p = FAME.Bridges.Electrical;
    model _famefaults_m__p_supply = FAME.Bridges.Electrical;
    model _famefaults_m__m_supply = FAME.Bridges.Electrical;
    model _famefaults_m__p = FAME.Bridges.Electrical;
    model _famefaults_m_supply__p_supply = FAME.Bridges.Electrical;
    model _famefaults_p__p_supply = FAME.Bridges.Electrical;
    model _famefaults_m_supply__p = FAME.Bridges.Electrical;

 // components of OpAmpDetailed
  parameter Modelica.SIunits.Frequency fz=5.0e6 "Zero frequency";
  Modelica.SIunits.Current i_out;
  Modelica.SIunits.Voltage v_out;
  parameter Modelica.SIunits.Voltage Vos=1.0e-3 "Input offset voltage";
  parameter Modelica.SIunits.Voltage vcm=0.0 "Correction value for limiting by msupply";
  parameter Modelica.SIunits.Voltage vcp=0.0 "Correction value for limiting by p_supply";
  final parameter Real Avcm_val=Avd0_val/(10.0^(CMRR/(20.0)))/(2.0) "common mode gain";
  final parameter Real Avd0_val=10.0^(Avd0/(20.0)) "differential mode gain";
  final parameter Modelica.SIunits.Current I1=Ib+Ios/(2.0) "Current of internal source I1";
  final parameter Modelica.SIunits.Current I2=Ib-Ios/(2.0) "Current of internal source I2";
  Modelica.SIunits.Current i_c3;
  parameter Real CMRR=90.0 "Common-mode rejection [dB]";
  parameter Real Avd0=106.0 "Differential amplifier [dB]";
  final parameter Modelica.SIunits.VoltageSlope sr_p_val=abs(sr_p) "Value of slew rate for increase";
  parameter Modelica.SIunits.Capacitance Cin=1.4e-12 "Input capacitance";
  parameter Modelica.SIunits.Time Ts=0.0000012 "sampling time";
  Modelica.SIunits.Current i_vos;
  Modelica.Electrical.Analog.Interfaces.PositivePin p_supply "Positive output voltage limitation" annotation(Placement(transformation(extent={{-8,65},{9,82}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p_supply(amount=0.0);
  parameter Modelica.SIunits.Resistance Rcm=2.0e9 "Input resistance (common mode)";
  Modelica.Electrical.Analog.Interfaces.NegativePin m_supply "Negative output voltage limitation" annotation(Placement(transformation(extent={{-9,-83},{9,-65}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_m_supply(amount=0.0);
  constant Real Pi=3.141592654;
  Modelica.SIunits.Voltage q_sum;
  final parameter Modelica.SIunits.VoltageSlope sr_m_val=-abs(sr_m) "Negative alue of slew rate for increase";
  Modelica.SIunits.Current i_4;
  Modelica.SIunits.Current i_r2;
  Modelica.SIunits.Current i_3;
  Modelica.SIunits.Voltage q_sum_help;
  Modelica.SIunits.Voltage v_source;
  parameter Modelica.SIunits.Current Ib=80.0e-9 "Input bias current";
  final parameter Modelica.SIunits.Voltage vcm_abs=abs(vcm) "Positive correction value for limiting by msupply";
  parameter Modelica.SIunits.Current Ios=20.0e-9 "Input offset current";
  Modelica.SIunits.Voltage v_pos;
  Modelica.SIunits.Voltage v_in;
  parameter Modelica.SIunits.Resistance Rout=75.0 "Output resistance";
  Modelica.SIunits.Voltage q_fp1;
  parameter Modelica.SIunits.Resistance Rdm=2.0e6 "Input resistance (differential input mode)";
  Modelica.Electrical.Analog.Interfaces.PositivePin outp "Output pin" annotation(Placement(transformation(extent={{110,-10},{90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_outp(amount=0.0);
  Modelica.SIunits.Voltage v_3;
  Modelica.SIunits.Voltage v_4;
  Modelica.SIunits.Voltage v_neg;
  parameter Modelica.SIunits.Current Imaxsi=25.0e-3 "Maximal output current (sink current)";
  parameter Modelica.SIunits.Current Imaxso=25.0e-3 "Maximal output current (source current)";
  parameter Modelica.SIunits.Frequency fp4=100.0e6 "Pole frequency";
  parameter Modelica.SIunits.Frequency fp2=2.0e6 "Pole frequency";
  parameter Modelica.SIunits.Frequency fp3=20.0e6 "Pole frequency";
  final parameter Modelica.SIunits.Voltage vcp_abs=abs(vcp) "Positive correction value for limiting by p_supply";
  parameter Modelica.SIunits.VoltageSlope sr_p=0.5e6 "Slew rate for increase";
  Modelica.Electrical.Analog.Interfaces.NegativePin m "Negative pin of the input port" annotation(Placement(transformation(extent={{-90,40},{-111,61}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_m(amount=0.0);
  parameter Modelica.SIunits.Frequency fp1=5.0 "Dominant pole";
  final parameter Modelica.SIunits.Current Imaxsi_val=abs(Imaxsi) "Orientation into outp";
  Real q_fr1;
  parameter Modelica.SIunits.VoltageSlope sr_m=0.5e6 "Slew rate for decrease";
  Modelica.Electrical.Analog.Interfaces.PositivePin p "Positive pin of the input port" annotation(Placement(transformation(extent={{-111,-61},{-90,-40}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p(amount=0.0);
  Real q_fr3;
  Real q_fr2;
  final parameter Modelica.SIunits.Current Imaxso_val=abs(Imaxso) "Orientation out outp";
  Modelica.SIunits.Voltage v_vos;
  Modelica.SIunits.Voltage x "auxiliary variable for slew rate";
  FAME.Bridges.Electrical _famefault_m__outp(amount=0.0);
  FAME.Bridges.Electrical _famefault_outp__p_supply(amount=0.0);
  FAME.Bridges.Electrical _famefault_m_supply__outp(amount=0.0);
  FAME.Bridges.Electrical _famefault_outp__p(amount=0.0);
  FAME.Bridges.Electrical _famefault_m__p_supply(amount=0.0);
  FAME.Bridges.Electrical _famefault_m__m_supply(amount=0.0);
  FAME.Bridges.Electrical _famefault_m__p(amount=0.0);
  FAME.Bridges.Electrical _famefault_m_supply__p_supply(amount=0.0);
  FAME.Bridges.Electrical _famefault_p__p_supply(amount=0.0);
  FAME.Bridges.Electrical _famefault_m_supply__p(amount=0.0);

 // algorithms and equations of OpAmpDetailed
 initial equation
  v_source = q_fp1;
  x = 0;
 equation
  assert(Rout>0.0,"Rout must be > 0.0.");
  v_pos = _famefault_p_supply.port_b.v;
  v_neg = _famefault_m_supply.port_b.v;
  _famefault_p.port_b.i = i_vos;
  _famefault_m.port_b.i = i_4-i_r2-i_c3;
  0 = i_3+i_r2+i_c3-i_vos;
  _famefault_p.port_b.v-_famefault_m.port_b.v = v_vos+v_in;
  v_4 = _famefault_m.port_b.v;
  v_3 = _famefault_p.port_b.v-v_vos;
  v_vos = Vos;
  i_3 = I1+v_3/(Rcm);
  v_in = Rdm*i_r2;
  i_c3 = Cin*der(v_in);
  i_4 = I2+v_4/(Rcm);
  der(q_fr1) = 2.0*Pi*fp2*(v_in-q_fr1);
  q_fr2+1.0/(2.0*Pi*fp3)*der(q_fr2) = q_fr1+1.0/(2.0*Pi*fz)*der(q_fr1);
  der(q_fr3) = 2.0*Pi*fp4*(q_fr2-q_fr3);
  q_sum = Avd0_val*q_fr3+Avcm_val*(v_3+v_4);
  q_sum_help = FCNq_sum_limit(q_sum,q_fp1,v_pos,v_neg,vcp_abs,vcm_abs);
  der(q_fp1) = 2.0*Pi*fp1*(q_sum_help-q_fp1);
  der(x) = (q_fp1-v_source)/(Ts);
  der(v_source) = smooth(0,noEvent((if der(x)>sr_p_val then sr_p_val else (if der(x)<sr_m_val then sr_m_val else der(x)))));
  v_out = _famefault_outp.port_b.v;
  i_out = _famefault_outp.port_b.i;
  i_out = FCNiout_limit(v_source,v_out,Rout,Imaxsi_val,Imaxso_val);
  _famefault_p_supply.port_b.i = 0;
  _famefault_m_supply.port_b.i = 0;
  connect(p_supply,_famefault_p_supply.port_a);
  connect(m_supply,_famefault_m_supply.port_a);
  connect(outp,_famefault_outp.port_a);
  connect(m,_famefault_m.port_a);
  connect(p,_famefault_p.port_a);
  connect(outp,_famefault_m__outp.port_a);
  connect(m,_famefault_m__outp.port_b);
  connect(outp,_famefault_outp__p_supply.port_a);
  connect(p_supply,_famefault_outp__p_supply.port_b);
  connect(outp,_famefault_m_supply__outp.port_a);
  connect(m_supply,_famefault_m_supply__outp.port_b);
  connect(outp,_famefault_outp__p.port_a);
  connect(p,_famefault_outp__p.port_b);
  connect(m,_famefault_m__p_supply.port_a);
  connect(p_supply,_famefault_m__p_supply.port_b);
  connect(m,_famefault_m__m_supply.port_a);
  connect(m_supply,_famefault_m__m_supply.port_b);
  connect(m,_famefault_m__p.port_a);
  connect(p,_famefault_m__p.port_b);
  connect(p_supply,_famefault_m_supply__p_supply.port_a);
  connect(m_supply,_famefault_m_supply__p_supply.port_b);
  connect(p_supply,_famefault_p__p_supply.port_a);
  connect(p,_famefault_p__p_supply.port_b);
  connect(m_supply,_famefault_m_supply__p.port_a);
  connect(p,_famefault_m_supply__p.port_b);

 annotation(Documentation(info="<html>
<p>The OpAmpDetailed model is a general operational amplifier model. The emphasis is on separating each important data sheet parameter into a sub-circuit independent of the other parameters. The model is broken down into five functional stages <b>input</b>, <b>frequency response</b>, <b>gain</b>, <b>slew rate</b> and an <b>output</b> stage. Each stage contains data sheet parameters to be modeled. This partitioning and the modelling of the separate submodels are based on the description in <b>[CP92]</b>.</p>
<p>Using <b>[CP92]</b> Joachim Haase (Fraunhofer Institute for Integrated Circuits, Design Automation Division) transfered 2001 operational amplifier models into VHDL-AMS. Now one of these models, the model &quot;amp(macro)&quot; was transferred into Modelica.</p>
<dl><dt><b>Reference:</b> </dt>
<dd><b>[CP92]</b> Conelly, J.A.; Choi, P.: Macromodelling with SPICE. Englewood Cliffs: Prentice-Hall, 1992 </dd>
</dl></html>",revisions="<html>
<ul>
<li><i>  </i>
       </li>
<li><i> Juni 17, 2009   </i>
       by Susann Wolf<br> initially implemented<br>
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Polygon(points={{60,0},{-60,70},{-60,-70},{60,0}},fillColor={255,255,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{0,35},{0,80}},color={0,0,255}),Line(points={{0,-35},{0,-80}},color={0,0,255}),Line(points={{-90,50},{-60,50}},color={0,0,255}),Line(points={{-90,-50},{-60,-50}},color={0,0,255}),Line(points={{60,0},{90,0}},color={0,0,255}),Line(points={{-48,32},{-28,32}},color={0,0,255}),Line(points={{-39,-20},{-39,-41}},color={0,0,255}),Line(points={{-50,-31},{-28,-31}},color={0,0,255}),Text(extent={{-151,141},{149,101}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Polygon(points={{60,0},{-60,70},{-60,-70},{60,0}},fillColor={255,255,255},fillPattern=FillPattern.Solid,lineColor={0,0,255}),Line(points={{0,35},{0,70}},color={0,0,255}),Line(points={{0,-35},{0,-70}},color={0,0,255}),Line(points={{-96,50},{-60,50}},color={0,0,255}),Line(points={{-96,-50},{-60,-50}},color={0,0,255}),Line(points={{60,0},{96,0}},color={0,0,255}),Line(points={{-55,50},{-45,50}},color={0,0,255}),Line(points={{-50,-45},{-50,-55}},color={0,0,255}),Line(points={{-55,-50},{-45,-50}},color={0,0,255}),Polygon(points={{120,3},{110,0},{120,-3},{120,3}},lineColor={0,0,0},fillPattern=FillPattern.HorizontalCylinder,fillColor={160,160,164}),Line(points={{111,0},{136,0}},color={0,0,0}),Text(extent={{114,2},{131,17}},lineColor={0,0,0},textString="i2"),Line(points={{-100,-35},{-100,23},{-100,24}},color={160,160,164},arrow={Arrow.None,Arrow.Filled}),Text(extent={{-97,-16},{-74,4}},lineColor={160,160,164},textString="vin")}));
 end OpAmpDetailed;

 model VariableResistor
  "Ideal linear electrical resistor with variable resistance"

 // locally defined classes in VariableResistor
    model _famefaults_heatPort = FAME.Dampers.ThermalWithoutConnectEquations;
    model _famefaults_n = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n__p = FAME.Bridges.Electrical;

 // components of VariableResistor
  Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_a heatPort() if useHeatPort annotation(Placement(transformation(extent={{-10,-110},{10,-90}}),iconTransformation(extent={{-10,-110},{10,-90}})));
  FAME.Dampers.ThermalWithoutConnectEquations _famefault_heatPort(amount=0.0,port_b(T(start=T)=T_heatPort,Q_flow=-LossPower)) if useHeatPort;
  parameter Modelica.SIunits.Temperature T_ref=300.15 "Reference temperature";
  Modelica.SIunits.Resistance R_actual "Actual resistance = R*(1 + alpha*(T_heatPort - T_ref))";
  Modelica.Electrical.Analog.Interfaces.NegativePin n "Negative pin" annotation(Placement(transformation(extent={{110,-10},{90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n(amount=0.0);
  parameter Boolean useHeatPort=false "=true, if HeatPort is enabled" annotation(Evaluate=true,HideResult=true,choices(__Dymola_checkBox=true));
  Modelica.SIunits.Current i "Current flowing from pin p to pin n";
  Modelica.SIunits.Power LossPower "Loss power leaving component via HeatPort";
  Modelica.SIunits.Voltage v "Voltage drop between the two pins (= p.v - n.v)";
  parameter Modelica.SIunits.Temperature T=T_ref "Fixed device temperature if useHeatPort = false" annotation(Dialog(enable=not useHeatPort));
  Modelica.Electrical.Analog.Interfaces.PositivePin p "Positive pin (potential p.v > n.v for positive voltage drop v)" annotation(Placement(transformation(extent={{-110,-10},{-90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p(amount=0.0);
  parameter Modelica.SIunits.LinearTemperatureCoefficient alpha=0 "Temperature coefficient of resistance (R_actual = R*(1 + alpha*(T_heatPort - T_ref))";
  Modelica.Blocks.Interfaces.RealInput R annotation(Placement(transformation(origin={0,110},extent={{-20,-20},{20,20}},rotation=270)));
  Modelica.SIunits.Temperature T_heatPort "Temperature of HeatPort";
  FAME.Bridges.Electrical _famefault_n__p(amount=0.0);

 // algorithms and equations of VariableResistor
 equation
  assert((1+alpha*(T_heatPort-T_ref))>=Modelica.Constants.eps,"Temperature outside scope of model!");
  R_actual = R*(1+alpha*(T_heatPort-T_ref));
  v = R_actual*i;
  LossPower = v*i;
  v = _famefault_p.port_b.v-_famefault_n.port_b.v;
  0 = _famefault_p.port_b.i+_famefault_n.port_b.i;
  i = _famefault_p.port_b.i;
  if not useHeatPort then
   T_heatPort = T;
  end if;
  connect(heatPort,_famefault_heatPort.port_a);
  connect(n,_famefault_n.port_a);
  connect(p,_famefault_p.port_a);
  connect(n,_famefault_n__p.port_a);
  connect(p,_famefault_n__p.port_b);

 annotation(Documentation(info="<html>
<p>The linear resistor connects the branch voltage <i>v</i> with the branch current <i>i</i> by 
<br><i><b>i*R = v</b></i>
<br>The Resistance <i>R</i> is given as input signal.
<br><br><b>Attention!!!</b><br>It is recommended that the R signal should not cross the zero value. Otherwise depending on the surrounding circuit the probability of singularities is high.</p>
</html>",revisions="<html>
<ul>
<li><i> August 07, 2009   </i>
       by Anton Haumer<br> temperature dependency of resistance added<br>
       </li>
<li><i> March 11, 2009   </i>
       by Christoph Clauss<br> conditional heat port added<br>
       </li>
<li><i>June 7, 2004   </i>
       by Christoph Clauss<br>changed, docu added<br>
       </li>
<li><i>April 30, 2004</i>
       by Anton Haumer<br>implemented.
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={2,2}),graphics={Line(points={{-90,0},{-70,0}},color={0,0,255}),Rectangle(extent={{-70,30},{70,-30}},lineColor={0,0,255},fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{70,0},{90,0}},color={0,0,255}),Line(points={{0,90},{0,30}},color={0,0,255}),Text(extent={{-148,-41},{152,-81}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={2,2}),graphics={Rectangle(extent={{-70,30},{70,-30}},lineColor={0,0,255},fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{-96,0},{-70,0}},color={0,0,255}),Line(points={{0,90},{0,30}},color={0,0,255}),Line(points={{70,0},{96,0}},color={0,0,255})}));
 end VariableResistor;

 model VariableConductor
  "Ideal linear electrical conductor with variable conductance"

 // locally defined classes in VariableConductor
    model _famefaults_heatPort = FAME.Dampers.ThermalWithoutConnectEquations;
    model _famefaults_n = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_p = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n__p = FAME.Bridges.Electrical;

 // components of VariableConductor
  Modelica.SIunits.Conductance G_actual "Actual conductance = G/(1 + alpha*(T_heatPort - T_ref))";
  Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_a heatPort() if useHeatPort annotation(Placement(transformation(extent={{-10,-110},{10,-90}}),iconTransformation(extent={{-10,-110},{10,-90}})));
  FAME.Dampers.ThermalWithoutConnectEquations _famefault_heatPort(amount=0.0,port_b(T(start=T)=T_heatPort,Q_flow=-LossPower)) if useHeatPort;
  parameter Modelica.SIunits.Temperature T_ref=300.15 "Reference temperature";
  Modelica.Blocks.Interfaces.RealInput G annotation(Placement(transformation(origin={0,110},extent={{-20,-20},{20,20}},rotation=270)));
  Modelica.Electrical.Analog.Interfaces.NegativePin n "Negative pin" annotation(Placement(transformation(extent={{110,-10},{90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n(amount=0.0);
  parameter Boolean useHeatPort=false "=true, if HeatPort is enabled" annotation(Evaluate=true,HideResult=true,choices(__Dymola_checkBox=true));
  Modelica.SIunits.Current i "Current flowing from pin p to pin n";
  Modelica.SIunits.Power LossPower "Loss power leaving component via HeatPort";
  Modelica.SIunits.Voltage v "Voltage drop between the two pins (= p.v - n.v)";
  parameter Modelica.SIunits.Temperature T=T_ref "Fixed device temperature if useHeatPort = false" annotation(Dialog(enable=not useHeatPort));
  Modelica.Electrical.Analog.Interfaces.PositivePin p "Positive pin (potential p.v > n.v for positive voltage drop v)" annotation(Placement(transformation(extent={{-110,-10},{-90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p(amount=0.0);
  parameter Modelica.SIunits.LinearTemperatureCoefficient alpha=0 "Temperature coefficient of conductance (G_actual = G/(1 + alpha*(T_heatPort - T_ref))";
  Modelica.SIunits.Temperature T_heatPort "Temperature of HeatPort";
  FAME.Bridges.Electrical _famefault_n__p(amount=0.0);

 // algorithms and equations of VariableConductor
 equation
  assert((1+alpha*(T_heatPort-T_ref))>=Modelica.Constants.eps,"Temperature outside scope of model!");
  G_actual = G/(1+alpha*(T_heatPort-T_ref));
  i = G_actual*v;
  LossPower = v*i;
  v = _famefault_p.port_b.v-_famefault_n.port_b.v;
  0 = _famefault_p.port_b.i+_famefault_n.port_b.i;
  i = _famefault_p.port_b.i;
  if not useHeatPort then
   T_heatPort = T;
  end if;
  connect(heatPort,_famefault_heatPort.port_a);
  connect(n,_famefault_n.port_a);
  connect(p,_famefault_p.port_a);
  connect(n,_famefault_n__p.port_a);
  connect(p,_famefault_n__p.port_b);

 annotation(Documentation(info="<html>
<p>The linear conductor connects the branch voltage <i>v</i> with the branch current <i>i</i> by
<br><i><b>i = G*v</b></i>
<br>The Conductance <i>G</i> is given as input signal.
<br><br><b>Attention!!!</b>
<br>It is recommended that the G signal should not cross the zero value. Otherwise depending on the surrounding circuit the probability of singularities is high.</p>
</html>",revisions="<html>
<ul>
<li><i> August 07, 2009   </i>
       by Anton Haumer<br> temperature dependency of conductance added<br>
       </li>
<li><i> March 11, 2009   </i>
       by Christoph Clauss<br> conditional heat port added<br>
       </li>
<li><i>June 7, 2004   </i>
       by Christoph Clauss<br> implemented<br>
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={2,2}),graphics={Line(points={{-90,0},{-70,0}},color={0,0,255}),Rectangle(extent={{-70,30},{70,-30}},lineColor={0,0,255},fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{70,0},{90,0}},color={0,0,255}),Line(points={{0,90},{0,30}},color={0,0,255}),Text(extent={{-148,-41},{152,-81}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={2,2}),graphics={Line(points={{0,90},{0,30}},color={0,0,255}),Line(points={{-96,0},{-70,0}},color={0,0,255}),Line(points={{70,0},{96,0}},color={0,0,255}),Rectangle(extent={{-70,30},{70,-30}},lineColor={0,0,255},fillColor={255,255,255},fillPattern=FillPattern.Solid)}));
 end VariableConductor;

 model VariableCapacitor
  "Ideal linear electrical capacitor with variable capacitance"

 // locally defined classes in VariableCapacitor
    model _famefaults_p = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n__p = FAME.Bridges.Electrical;

 // components of VariableCapacitor
  Modelica.SIunits.Voltage v "Voltage drop between the two pins (= p.v - n.v)";
  Modelica.SIunits.ElectricCharge Q;
  Modelica.Blocks.Interfaces.RealInput C annotation(Placement(transformation(origin={0,110},extent={{-20,-20},{20,20}},rotation=270)));
  Modelica.Electrical.Analog.Interfaces.PositivePin p "Positive pin (potential p.v > n.v for positive voltage drop v)" annotation(Placement(transformation(extent={{-110,-10},{-90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p(amount=0.0);
  Modelica.Electrical.Analog.Interfaces.NegativePin n "Negative pin" annotation(Placement(transformation(extent={{110,-10},{90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n(amount=0.0);
  parameter Modelica.SIunits.Capacitance Cmin=Modelica.Constants.eps "lower bound for variable capacitance";
  Modelica.SIunits.Current i "Current flowing from pin p to pin n";
  FAME.Bridges.Electrical _famefault_n__p(amount=0.0);

 // algorithms and equations of VariableCapacitor
 equation
  assert(C>=0,"Capacitance C (= "+String(C)+") has to be >= 0!");
  Q = noEvent(max(C,Cmin))*v;
  i = der(Q);
  v = _famefault_p.port_b.v-_famefault_n.port_b.v;
  0 = _famefault_p.port_b.i+_famefault_n.port_b.i;
  i = _famefault_p.port_b.i;
  connect(p,_famefault_p.port_a);
  connect(n,_famefault_n.port_a);
  connect(p,_famefault_n__p.port_a);
  connect(n,_famefault_n__p.port_b);

 annotation(Documentation(info="<html>
<p>The linear capacitor connects the branch voltage <i>v</i> with the branch current <i>i</i> by
<br><i><b>i = dQ/dt</b></i> with <i><b>Q = C * v</b></i><b> </b>.
<br>The capacitance <i>C</i> is given as input signal. 
It is required that C &ge; 0, otherwise an assertion is raised. To avoid a variable index system, 
C = Cmin, if 0 &le; C &lt; Cmin, where Cmin is a parameter with default value Modelica.Constants.eps.</p>
</html>",revisions="<html>
<ul>
<li><i>June 7, 2004   </i>
       by Christoph Clauss<br>changed, docu added<br>
       </li>
<li><i>April 30, 2004</i>
       by Anton Haumer<br>implemented.
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics={Line(points={{-90,0},{-14,0}},color={0,0,255}),Line(points={{14,0},{90,0}},color={0,0,255}),Line(points={{0,90},{0,30}},color={0,0,255}),Line(points={{-14,28},{-14,-28}},thickness=0.5,color={0,0,255}),Line(points={{14,28},{14,-28}},thickness=0.5,color={0,0,255}),Text(extent={{-144,-43},{156,-83}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics={Line(points={{-96,0},{-14,0}},color={0,0,255}),Line(points={{14,0},{96,0}},color={0,0,255}),Line(points={{0,90},{0,30}},color={0,0,255}),Line(points={{-14,28},{-14,-28}},thickness=0.5,color={0,0,255}),Line(points={{14,28},{14,-28}},thickness=0.5,color={0,0,255})}));
 end VariableCapacitor;

 model VariableInductor
  "Ideal linear electrical inductor with variable inductance"

 // locally defined classes in VariableInductor
    model _famefaults_p = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n = FAME.Dampers.ElectricalWithoutConnectEquations;
    model _famefaults_n__p = FAME.Bridges.Electrical;

 // components of VariableInductor
  Modelica.SIunits.Voltage v "Voltage drop between the two pins (= p.v - n.v)";
  Modelica.Electrical.Analog.Interfaces.PositivePin p "Positive pin (potential p.v > n.v for positive voltage drop v)" annotation(Placement(transformation(extent={{-110,-10},{-90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_p(amount=0.0);
  Modelica.Blocks.Interfaces.RealInput L annotation(Placement(transformation(origin={0,108},extent={{-20,-20},{20,20}},rotation=270)));
  Modelica.Electrical.Analog.Interfaces.NegativePin n "Negative pin" annotation(Placement(transformation(extent={{110,-10},{90,10}},rotation=0)));
  FAME.Dampers.ElectricalWithoutConnectEquations _famefault_n(amount=0.0);
  parameter Modelica.SIunits.Inductance Lmin=Modelica.Constants.eps "lower bound for variable inductance";
  Modelica.SIunits.MagneticFlux Psi;
  Modelica.SIunits.Current i "Current flowing from pin p to pin n";
  FAME.Bridges.Electrical _famefault_n__p(amount=0.0);

 // algorithms and equations of VariableInductor
 equation
  assert(L>=0,"Inductance L_ (= "+String(L)+") has to be >= 0!");
  Psi = noEvent(max(L,Lmin))*i;
  v = der(Psi);
  v = _famefault_p.port_b.v-_famefault_n.port_b.v;
  0 = _famefault_p.port_b.i+_famefault_n.port_b.i;
  i = _famefault_p.port_b.i;
  connect(p,_famefault_p.port_a);
  connect(n,_famefault_n.port_a);
  connect(p,_famefault_n__p.port_a);
  connect(n,_famefault_n__p.port_b);

 annotation(Documentation(info="<html>
<p>The linear inductor connects the branch voltage <i>v</i> with the branch current <i>i</i> by
<br><i><b>v = d Psi/dt </b></i>with <i><b>Psi = L * i </b></i>.
<br>The inductance <i>L</i> is as input signal.
It is required that L &ge; 0, otherwise an assertion is raised. To avoid a variable index system, L = Lmin, if 0 &le; L &lt; Lmin, where Lmin is a parameter with default value Modelica.Constants.eps.</p>
</html>",revisions="<html>
<ul>
<li><i>June 7, 2004   </i>
       by Christoph Clauss<br>changed, docu added<br>
       </li>
<li><i>April 30, 2004</i>
       by Anton Haumer<br>implemented.
       </li>
</ul>
</html>"),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics={Line(points={{-90,0},{-60,0}},color={0,0,255}),Line(points={{60,0},{90,0}},color={0,0,255}),Line(points={{0,90},{0,8}},color={0,0,255}),Ellipse(extent={{-60,-15},{-30,15}},lineColor={0,0,255}),Ellipse(extent={{-30,-15},{0,15}},lineColor={0,0,255}),Ellipse(extent={{0,-15},{30,15}},lineColor={0,0,255}),Ellipse(extent={{30,-15},{60,15}},lineColor={0,0,255}),Rectangle(extent={{-60,-30},{60,0}},lineColor={255,255,255},fillColor={255,255,255},fillPattern=FillPattern.Solid),Text(extent={{-146,-47},{154,-87}},textString="%name",lineColor={0,0,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics={Line(points={{-96,0},{-60,0}},color={0,0,255}),Line(points={{0,90},{0,8}},color={0,0,255}),Ellipse(extent={{-60,-15},{-30,15}},lineColor={0,0,255}),Ellipse(extent={{-30,-15},{0,15}},lineColor={0,0,255}),Ellipse(extent={{0,-15},{30,15}},lineColor={0,0,255}),Ellipse(extent={{30,-15},{60,15}},lineColor={0,0,255}),Rectangle(extent={{-60,-30},{62,0}},lineColor={255,255,255},fillColor={255,255,255},fillPattern=FillPattern.Solid),Line(points={{60,0},{96,0}},color={0,0,255})}));
 end VariableInductor;

annotation(Documentation(info="<html>
<p>This package contains very basic analog electrical components such as resistor, conductor, condensator, inductor, and the ground (which is needed in each electrical circuit description. Furthermore, controled sources, coupling components, and some improved (but newertheless basic) are in this package.</p>
</html>",revisions="<html>
<dl>
<dt>
<b>Main Authors:</b>
<dd>
Christoph Clau&szlig;
    &lt;<a href=\"mailto:Christoph.Clauss@eas.iis.fraunhofer.de\">Christoph.Clauss@eas.iis.fraunhofer.de</a>&gt;<br>
    Andr&eacute; Schneider
    &lt;<a href=\"mailto:Andre.Schneider@eas.iis.fraunhofer.de\">Andre.Schneider@eas.iis.fraunhofer.de</a>&gt;<br>
    Fraunhofer Institute for Integrated Circuits<br>
    Design Automation Department<br>
    Zeunerstra&szlig;e 38<br>
    D-01069 Dresden<br>
<p>
</dl>
</html>"));
end Basic;
