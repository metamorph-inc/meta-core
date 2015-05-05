// processed by FAME Modelica Library Fault-Augmentor [version 19011]

within Modelica.Fluid.Examples;

package HeatExchanger
 "Demo of a heat exchanger model"
 extends Modelica.Icons.ExamplesPackage;

 model HeatExchangerSimulation
  "simulation for the heat exchanger model"
  extends Modelica.Icons.Example;

 // locally defined classes in HeatExchangerSimulation
  replaceable   package Medium = Modelica.Media.Water.ConstantPropertyLiquidWater;

 // components of HeatExchangerSimulation
  Modelica.Fluid.Examples.HeatExchanger.BaseClasses.BasicHX HEX(c_wall=500,use_T_start=true,nNodes=20,length=2,m_flow_start_1=0.2,m_flow_start_2=0.2,k_wall=100,energyDynamics=Modelica.Fluid.Types.Dynamics.FixedInitial,massDynamics=Modelica.Fluid.Types.Dynamics.SteadyStateInitial,s_wall=0.005,crossArea_1=4.5e-4,crossArea_2=4.5e-4,perimeter_1=0.075,perimeter_2=0.075,area_h_1=0.075*2*20,area_h_2=0.075*2*20,rho_wall=900,redeclare package Medium_1 = Medium,redeclare package Medium_2 = Medium,redeclare model HeatTransfer_1 = Modelica.Fluid.Pipes.BaseClasses.HeatTransfer.ConstantFlowHeatTransfer(alpha0=1000),redeclare model HeatTransfer_2 = Modelica.Fluid.Pipes.BaseClasses.HeatTransfer.ConstantFlowHeatTransfer(alpha0=200),Twall_start=300,dT=10,T_start_1=304,T_start_2=300) annotation(Placement(transformation(extent={{-26,-14},{34,46}},rotation=0)));
  Modelica.Fluid.Sources.Boundary_pT ambient2(nPorts=1,p=1e5,T=280,redeclare package Medium = Medium) annotation(Placement(transformation(extent={{82,-28},{62,-8}},rotation=0)));
  Modelica.Fluid.Sources.Boundary_pT ambient1(nPorts=1,p=1e5,T=300,redeclare package Medium = Medium) annotation(Placement(transformation(extent={{82,24},{62,44}},rotation=0)));
  Modelica.Fluid.Sources.MassFlowSource_T massFlowRate2(nPorts=1,m_flow=0.2,T=360,redeclare package Medium = Medium,use_m_flow_in=true,use_T_in=false,use_X_in=false) annotation(Placement(transformation(extent={{-66,24},{-46,44}},rotation=0)));
  Modelica.Fluid.Sources.MassFlowSource_T massFlowRate1(nPorts=1,T=300,m_flow=0.5,redeclare package Medium = Medium) annotation(Placement(transformation(extent={{-66,-10},{-46,10}},rotation=0)));
  Modelica.Blocks.Sources.Ramp Ramp1(startTime=50,duration=5,height=-1,offset=0.5) annotation(Placement(transformation(extent={{-98,24},{-78,44}},rotation=0)));
  inner Modelica.Fluid.System system annotation(Placement(transformation(extent={{60,70},{80,90}},rotation=0)));

 // algorithms and equations of HeatExchangerSimulation
 equation
  connect(massFlowRate1.ports[1],HEX.port_a1) annotation(Line(points={{-46,0},{-40,0},{-40,15.4},{-29,15.4}},color={0,127,255}));
  connect(HEX.port_b1,ambient1.ports[1]) annotation(Line(points={{37,15.4},{48.5,15.4},{48.5,34},{62,34}},color={0,127,255}));
  connect(Ramp1.y,massFlowRate2.m_flow_in) annotation(Line(points={{-77,34},{-74,34},{-74,42},{-66,42}},color={0,0,127}));
  connect(massFlowRate2.ports[1],HEX.port_b2) annotation(Line(points={{-46,34},{-40,34},{-40,29.8},{-29,29.8}},color={0,127,255},smooth=Smooth.None));
  connect(HEX.port_a2,ambient2.ports[1]) annotation(Line(points={{37,2.2},{42,2},{50,2},{50,-18},{62,-18}},color={0,127,255},smooth=Smooth.None));

 annotation(Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics),experiment(StopTime=100,Tolerance=1e-005),Documentation(info="<html>
<img src=\"modelica://Modelica/Resources/Images/Fluid/Examples/HeatExchanger.png\" border=\"1\">
</html>"));
 end HeatExchangerSimulation;

 package BaseClasses
  "Additional models for heat exchangers"
  extends Modelica.Icons.BasesPackage;

  model BasicHX
   "Simple heat exchanger model"

  // locally defined classes in BasicHX
   replaceable    package Medium_1 = Modelica.Media.Water.StandardWater constrainedby Modelica.Media.Interfaces.PartialMedium;
   replaceable    package Medium_2 = Modelica.Media.Water.StandardWater constrainedby Modelica.Media.Interfaces.PartialMedium;
   replaceable    model HeatTransfer_1 = Modelica.Fluid.Pipes.BaseClasses.HeatTransfer.IdealFlowHeatTransfer constrainedby Modelica.Fluid.Pipes.BaseClasses.HeatTransfer.PartialFlowHeatTransfer;
   replaceable    model HeatTransfer_2 = Modelica.Fluid.Pipes.BaseClasses.HeatTransfer.IdealFlowHeatTransfer constrainedby Modelica.Fluid.Pipes.BaseClasses.HeatTransfer.PartialFlowHeatTransfer;
   replaceable    model FlowModel_1 = Modelica.Fluid.Pipes.BaseClasses.FlowModels.DetailedPipeFlow constrainedby Modelica.Fluid.Pipes.BaseClasses.FlowModels.PartialStaggeredFlowModel;
   replaceable    model FlowModel_2 = Modelica.Fluid.Pipes.BaseClasses.FlowModels.DetailedPipeFlow constrainedby Modelica.Fluid.Pipes.BaseClasses.FlowModels.PartialStaggeredFlowModel;
      model _famefaults_port_a1 = FAME.Dampers.FluidWithConnectEquations;
      model _famefaults_port_a2 = FAME.Dampers.FluidWithConnectEquations;
      model _famefaults_port_b1 = FAME.Dampers.FluidWithConnectEquations;
      model _famefaults_port_b2 = FAME.Dampers.FluidWithConnectEquations;

  // components of BasicHX
   final parameter Modelica.SIunits.Area area_h=(area_h_1+area_h_2)/(2) "Heat transfer area";
   parameter Medium_2.Temperature T_start_2=(if use_T_start then Medium_2.T_default else Medium_2.temperature_phX((p_a_start2+p_b_start2)/(2),h_start_2,X_start_2)) "Start value of temperature" annotation(Evaluate=true,Dialog(tab="Initialization",group="Fluid 2",enable=use_T_start));
   parameter Medium_1.Temperature T_start_1=(if use_T_start then Medium_1.T_default else Medium_1.temperature_phX((p_a_start1+p_b_start1)/(2),h_start_1,X_start_1)) "Start value of temperature" annotation(Evaluate=true,Dialog(tab="Initialization",group="Fluid 1",enable=use_T_start));
   parameter Boolean use_HeatTransfer=false "= true to use the HeatTransfer_1/_2 model";
   parameter Medium_2.MassFraction X_start_2[Medium_2.nX]=Medium_2.X_default "Start value of mass fractions m_i/m" annotation(Dialog(tab="Initialization",group="Fluid 2",enable=Medium_2.nXi>0));
   parameter Boolean allowFlowReversal=system.allowFlowReversal "allow flow reversal, false restricts to design direction (port_a -> port_b)" annotation(Dialog(tab="Assumptions"),Evaluate=true);
   parameter Medium_1.MassFraction X_start_1[Medium_1.nX]=Medium_1.X_default "Start value of mass fractions m_i/m" annotation(Dialog(tab="Initialization",group="Fluid 1",enable=Medium_1.nXi>0));
   parameter Integer nNodes(min=1)=2 "Spatial segmentation";
   parameter Modelica.SIunits.ThermalConductivity k_wall "Thermal conductivity of wall material" annotation(Dialog(group="Solid material properties"));
   parameter Medium_2.SpecificEnthalpy h_start_2=(if use_T_start then Medium_2.specificEnthalpy_pTX((p_a_start2+p_b_start2)/(2),T_start_2,X_start_2[1:Medium_2.nXi]) else Medium_2.h_default) "Start value of specific enthalpy" annotation(Evaluate=true,Dialog(tab="Initialization",group="Fluid 2",enable=not use_T_start));
   parameter Medium_2.AbsolutePressure p_b_start2=Medium_2.p_default "Start value of pressure" annotation(Dialog(tab="Initialization",group="Fluid 2"));
   parameter Medium_1.SpecificEnthalpy h_start_1=(if use_T_start then Medium_1.specificEnthalpy_pTX((p_a_start1+p_b_start1)/(2),T_start_1,X_start_1[1:Medium_1.nXi]) else Medium_1.h_default) "Start value of specific enthalpy" annotation(Evaluate=true,Dialog(tab="Initialization",group="Fluid 1",enable=not use_T_start));
   parameter Types.Dynamics energyDynamics=system.energyDynamics "Formulation of energy balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
   Modelica.Fluid.Pipes.DynamicPipe pipe_2(redeclare final package Medium = Medium_2,final nNodes=nNodes,final allowFlowReversal=allowFlowReversal,final energyDynamics=energyDynamics,final massDynamics=massDynamics,final momentumDynamics=momentumDynamics,final length=length,final isCircular=false,final diameter=0,final use_HeatTransfer=use_HeatTransfer,redeclare final model HeatTransfer = HeatTransfer_2,final use_T_start=use_T_start,final T_start=T_start_2,final h_start=h_start_2,final X_start=X_start_2,final m_flow_start=m_flow_start_2,final perimeter=perimeter_2,final crossArea=crossArea_2,final p_a_start=p_a_start1,final p_b_start=p_b_start2,final roughness=roughness_2,redeclare final model FlowModel = FlowModel_2) annotation(Placement(transformation(extent={{20,88},{-40,28}},rotation=0)));
   parameter Medium_2.AbsolutePressure p_a_start2=Medium_2.p_default "Start value of pressure" annotation(Dialog(tab="Initialization",group="Fluid 2"));
   Modelica.Fluid.Pipes.DynamicPipe pipe_1(redeclare final package Medium = Medium_1,final isCircular=false,final diameter=0,final nNodes=nNodes,final allowFlowReversal=allowFlowReversal,final energyDynamics=energyDynamics,final massDynamics=massDynamics,final momentumDynamics=momentumDynamics,final length=length,final use_HeatTransfer=use_HeatTransfer,redeclare final model HeatTransfer = HeatTransfer_1,final use_T_start=use_T_start,final T_start=T_start_1,final h_start=h_start_1,final X_start=X_start_1,final m_flow_start=m_flow_start_1,final perimeter=perimeter_1,final crossArea=crossArea_1,final roughness=roughness_1,redeclare final model FlowModel = FlowModel_1) annotation(Placement(transformation(extent={{-40,-80},{20,-20}},rotation=0)));
   parameter Medium_1.AbsolutePressure p_a_start1=Medium_1.p_default "Start value of pressure" annotation(Dialog(tab="Initialization",group="Fluid 1"));
   parameter Medium_1.AbsolutePressure p_b_start1=Medium_1.p_default "Start value of pressure" annotation(Dialog(tab="Initialization",group="Fluid 1"));
   parameter Modelica.SIunits.SpecificHeatCapacity c_wall "Specific heat capacity of wall material" annotation(Dialog(tab="General",group="Solid material properties"));
   parameter Modelica.SIunits.Area crossArea_1 "Cross sectional area" annotation(Dialog(tab="General",group="Fluid 1"));
   parameter Modelica.SIunits.Length length(min=0) "Length of flow path for both fluids";
   parameter Boolean use_T_start=true "Use T_start if true, otherwise h_start" annotation(Evaluate=true,Dialog(tab="Initialization"));
   parameter Modelica.SIunits.Length roughness_1=2.5e-5 "Absolute roughness of pipe (default = smooth steel pipe)" annotation(Dialog(tab="General",group="Fluid 1"));
   parameter Modelica.SIunits.Length roughness_2=2.5e-5 "Absolute roughness of pipe (default = smooth steel pipe)" annotation(Dialog(tab="General",group="Fluid 2"));
   Modelica.Fluid.Interfaces.FluidPort_a port_a1(redeclare final package Medium = Medium_1) annotation(Placement(transformation(extent={{-120,-12},{-100,8}},rotation=0)));
   FAME.Dampers.FluidWithConnectEquations _famefault_port_a1(amount=0.0,port_b(redeclare final package Medium = Medium_1),port_a(redeclare final package Medium = Medium_1));
   Modelica.Fluid.Interfaces.FluidPort_a port_a2(redeclare final package Medium = Medium_2) annotation(Placement(transformation(extent={{100,-56},{120,-36}},rotation=0)));
   FAME.Dampers.FluidWithConnectEquations _famefault_port_a2(amount=0.0,port_b(redeclare final package Medium = Medium_2),port_a(redeclare final package Medium = Medium_2));
   final parameter Modelica.SIunits.Mass m_wall=rho_wall*area_h*s_wall "Wall mass";
   parameter Medium_2.MassFlowRate m_flow_start_2=system.m_flow_start "Start value of mass flow rate" annotation(Evaluate=true,Dialog(tab="Initialization",group="Fluid 2"));
   Modelica.SIunits.HeatFlowRate Q_flow_1 "Total heat flow rate of pipe 1";
   parameter Medium_1.MassFlowRate m_flow_start_1=system.m_flow_start "Start value of mass flow rate" annotation(Evaluate=true,Dialog(tab="Initialization",group="Fluid 1"));
   parameter Types.Dynamics massDynamics=system.massDynamics "Formulation of mass balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
   Modelica.SIunits.HeatFlowRate Q_flow_2 "Total heat flow rate of pipe 2";
   parameter Modelica.SIunits.Area area_h_2 "Heat transfer area" annotation(Dialog(tab="General",group="Fluid 2"));
   parameter Modelica.SIunits.Area area_h_1 "Heat transfer area" annotation(Dialog(tab="General",group="Fluid 1"));
   parameter Modelica.SIunits.Length s_wall(min=0) "Wall thickness";
   parameter Modelica.SIunits.Length perimeter_1 "Flow channel perimeter" annotation(Dialog(tab="General",group="Fluid 1"));
   parameter Modelica.SIunits.Area crossArea_2 "Cross sectional area" annotation(Dialog(tab="General",group="Fluid 2"));
   parameter Modelica.SIunits.Temperature Twall_start "Start value of wall temperature" annotation(Dialog(tab="Initialization",group="Wall"));
   parameter Modelica.SIunits.Density rho_wall "Density of wall material" annotation(Dialog(tab="General",group="Solid material properties"));
   parameter Modelica.SIunits.Temperature dT "Start value for pipe_1.T - pipe_2.T" annotation(Dialog(tab="Initialization",group="Wall"));
   Modelica.Fluid.Interfaces.FluidPort_b port_b1(redeclare final package Medium = Medium_1) annotation(Placement(transformation(extent={{100,-12},{120,8}},rotation=0)));
   FAME.Dampers.FluidWithConnectEquations _famefault_port_b1(amount=0.0,port_b(redeclare final package Medium = Medium_1),port_a(redeclare final package Medium = Medium_1));
   outer Modelica.Fluid.System system "System properties";
   Modelica.Fluid.Interfaces.FluidPort_b port_b2(redeclare final package Medium = Medium_2) annotation(Placement(transformation(extent={{-120,36},{-100,56}},rotation=0)));
   FAME.Dampers.FluidWithConnectEquations _famefault_port_b2(amount=0.0,port_b(redeclare final package Medium = Medium_2),port_a(redeclare final package Medium = Medium_2));
   parameter Modelica.SIunits.Length perimeter_2 "Flow channel perimeter" annotation(Dialog(tab="General",group="Fluid 2"));
   parameter Types.Dynamics momentumDynamics=system.momentumDynamics "Formulation of momentum balance, if pressureLoss options available" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
   Modelica.Fluid.Examples.HeatExchanger.BaseClasses.WallConstProps wall(rho_wall=rho_wall,c_wall=c_wall,T_start=Twall_start,k_wall=k_wall,dT=dT,s=s_wall,energyDynamics=energyDynamics,n=nNodes,area_h=(crossArea_1+crossArea_2)/(2)) annotation(Placement(transformation(extent={{-29,-23},{9,35}},rotation=0)));

  // algorithms and equations of BasicHX
  equation
   Q_flow_1 = sum(pipe_1.heatTransfer.Q_flows);
   Q_flow_2 = sum(pipe_2.heatTransfer.Q_flows);
   connect(pipe_2.port_b,_famefault_port_b2.port_b) annotation(Line(points={{-40,58},{-76,58},{-76,46},{-110,46}},color={0,127,255},thickness=0.5));
   connect(pipe_1.port_b,_famefault_port_b1.port_b) annotation(Line(points={{20,-50},{42,-50},{42,-2},{110,-2}},color={0,127,255},thickness=0.5));
   connect(pipe_1.port_a,_famefault_port_a1.port_b) annotation(Line(points={{-40,-50},{-75.3,-50},{-75.3,-2},{-110,-2}},color={0,127,255},thickness=0.5));
   connect(pipe_2.port_a,_famefault_port_a2.port_b) annotation(Line(points={{20,58},{65,58},{65,-46},{110,-46}},color={0,127,255},thickness=0.5));
   connect(wall.heatPort_b,pipe_1.heatPorts) annotation(Line(points={{-10,-8.5},{-10,-36.8},{-9.7,-36.8}},color={191,0,0},smooth=Smooth.None));
   connect(pipe_2.heatPorts[nNodes:-1:1],wall.heatPort_a[1:nNodes]) annotation(Line(points={{-10.3,44.8},{-10.3,31.7},{-10,31.7},{-10,20.5}},color={127,0,0},smooth=Smooth.None));
   connect(port_a1,_famefault_port_a1.port_a);
   connect(port_a2,_famefault_port_a2.port_a);
   connect(port_b1,_famefault_port_b1.port_a);
   connect(port_b2,_famefault_port_b2.port_a);

  annotation(Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}},grid={1,1}),graphics),Icon(coordinateSystem(preserveAspectRatio=false,extent={{-100,-100},{100,100}},grid={1,1}),graphics={Rectangle(extent={{-100,-26},{100,-30}},lineColor={0,0,0},fillColor={95,95,95},fillPattern=FillPattern.Forward),Rectangle(extent={{-100,30},{100,26}},lineColor={0,0,0},fillColor={95,95,95},fillPattern=FillPattern.Forward),Rectangle(extent={{-100,60},{100,30}},lineColor={0,0,0},fillPattern=FillPattern.HorizontalCylinder,fillColor={0,63,125}),Rectangle(extent={{-100,-30},{100,-60}},lineColor={0,0,0},fillPattern=FillPattern.HorizontalCylinder,fillColor={0,63,125}),Rectangle(extent={{-100,26},{100,-26}},lineColor={0,0,0},fillPattern=FillPattern.HorizontalCylinder,fillColor={0,128,255}),Text(extent={{-150,110},{150,70}},lineColor={0,0,255},textString="%name"),Line(points={{30,-85},{-60,-85}},color={0,128,255},smooth=Smooth.None),Polygon(points={{20,-70},{60,-85},{20,-100},{20,-70}},lineColor={0,128,255},smooth=Smooth.None,fillColor={0,128,255},fillPattern=FillPattern.Solid),Line(points={{30,77},{-60,77}},color={0,128,255},smooth=Smooth.None),Polygon(points={{-50,92},{-90,77},{-50,62},{-50,92}},lineColor={0,128,255},smooth=Smooth.None,fillColor={0,128,255},fillPattern=FillPattern.Solid)}),Documentation(info="<html>
<p>Simple model of a heat exchanger consisting of two pipes and one wall in between.
For both fluids geometry parameters, such as heat transfer area and cross section as well as heat transfer and pressure drop correlations may be chosen.
The flow scheme may be concurrent or counterflow, defined by the respective flow directions of the fluids entering the component.
The design flow direction with positive m_flow variables is counterflow.</p>
</html>"));
  end BasicHX;

  model WallConstProps
   "Pipe wall with capacitance, assuming 1D heat conduction and constant material properties"

  // locally defined classes in WallConstProps
      model _famefaults_heatPort_b = FAME.Dampers.ThermalWithoutConnectEquations;
      model _famefaults_heatPort_a = FAME.Dampers.ThermalWithoutConnectEquations;

  // components of WallConstProps
   Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_a heatPort_b[n] "Thermal port" annotation(Placement(transformation(extent={{-20,-40},{20,-60}},rotation=0)));
   FAME.Dampers.ThermalWithoutConnectEquations _famefault_heatPort_b[n](each amount=0.0);
   parameter Modelica.SIunits.Area area_h "Heat transfer area";
   Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_a heatPort_a[n] "Thermal port" annotation(Placement(transformation(extent={{-20,40},{20,60}},rotation=0)));
   FAME.Dampers.ThermalWithoutConnectEquations _famefault_heatPort_a[n](each amount=0.0);
   parameter Integer n(min=1)=1 "Segmentation perpendicular to heat conduction";
   parameter Modelica.SIunits.ThermalConductivity k_wall "Thermal conductivity of wall material";
   parameter Modelica.SIunits.Mass m[n]=fill(rho_wall*area_h*s/(n),n) "Distribution of wall mass";
   parameter Modelica.SIunits.Density rho_wall "Density of wall material";
   parameter Modelica.SIunits.Temperature dT "Start value for port_b.T - port_a.T";
   Modelica.SIunits.Temperature Ta[n](each start=T_start-0.5*dT);
   Modelica.SIunits.Temperature T[n](start=ones(n)*T_start,stateSelect=StateSelect.prefer) "Wall temperature";
   outer Modelica.Fluid.System system;
   parameter Types.Dynamics energyDynamics=system.energyDynamics "Formulation of energy balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
   Modelica.SIunits.Temperature Tb[n](each start=T_start+0.5*dT);
   parameter Modelica.SIunits.Length s "Wall thickness";
   parameter Modelica.SIunits.SpecificHeatCapacity c_wall "Specific heat capacity of wall material";
   parameter Modelica.SIunits.Temperature T_start "Wall temperature start value";

  // algorithms and equations of WallConstProps
  initial equation
   if energyDynamics==Types.Dynamics.SteadyState then
    der(T) = zeros(n);
   elseif energyDynamics==Types.Dynamics.FixedInitial then
    T = ones(n)*T_start;
   end if;
  equation
   for i in 1:n loop
    assert(m[i]>0,"Wall has negative dimensions");
    if energyDynamics==Types.Dynamics.SteadyState then
     0 = _famefault_heatPort_a[i].port_b.Q_flow+_famefault_heatPort_b[i].port_b.Q_flow;
    else
     c_wall*m[i]*der(T[i]) = heatPort_a[i].Q_flow+heatPort_b[i].Q_flow;
    end if;
    _famefault_heatPort_a[i].port_b.Q_flow = k_wall/(s)*(Ta[i]-T[i])*area_h/(n);
    _famefault_heatPort_b[i].port_b.Q_flow = k_wall/(s)*(Tb[i]-T[i])*area_h/(n);
   end for;
   Ta = _famefault_heatPort_a.port_b.T;
   Tb = _famefault_heatPort_b.port_b.T;
   for _fame_lhs_0 in 1:n loop
    connect(heatPort_b[_fame_lhs_0],_famefault_heatPort_b[_fame_lhs_0].port_a);
   end for;
   for _fame_lhs_0 in 1:n loop
    connect(heatPort_a[_fame_lhs_0],_famefault_heatPort_a[_fame_lhs_0].port_a);
   end for;

  annotation(Icon(coordinateSystem(preserveAspectRatio=false,extent={{-100,-100},{100,100}}),graphics={Rectangle(extent={{-100,40},{100,-40}},lineColor={0,0,0},fillColor={95,95,95},fillPattern=FillPattern.Forward),Text(extent={{-82,18},{76,-18}},lineColor={0,0,0},textString="%name")}),Documentation(revisions="<html>
<ul>
<li><i>04 Mar 2006</i>
    by Katrin Pr&ouml;l&szlig;:<br>
       Model added to the Fluid library</li>
</ul>
</html>",info="<html>
Simple model of circular (or any other closed shape) wall to be used for pipe (or duct) models. Heat conduction is regarded one dimensional, capacitance is lumped at the arithmetic mean temperature. The spatial discretization (parameter <code>n</code>) is meant to correspond to a connected fluid model discretization.
</html>"));
  end WallConstProps;
 end BaseClasses;
end HeatExchanger;
