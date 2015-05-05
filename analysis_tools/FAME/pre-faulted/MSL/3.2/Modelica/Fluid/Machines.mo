// processed by FAME Modelica Library Fault-Augmentor [version 19011]

within Modelica.Fluid;

package Machines
 "Devices for converting between energy held in a fluid and mechanical energy"
 extends Modelica.Icons.VariantsPackage;

 model SweptVolume
  "varying cylindric volume depending on the postition of the piston"

  import Modelica.Constants.pi;
  import Modelica.Fluid.Types;
  import Modelica.Fluid.Types.Dynamics;
  import Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables;

 // locally defined classes in SweptVolume
  replaceable   package Medium = Modelica.Media.Interfaces.PartialMedium "Medium in the component" annotation(choicesAllMatching=true);
  replaceable   model HeatTransfer = Modelica.Fluid.Vessels.BaseClasses.HeatTransfer.IdealHeatTransfer constrainedby Modelica.Fluid.Vessels.BaseClasses.HeatTransfer.PartialVesselHeatTransfer;
    model _famefaults_heatPort = FAME.Dampers.ThermalWithConnectEquations;
    model _famefaults_flange = FAME.Dampers.TranslationalWithoutConnectEquations;

 // components of SweptVolume
  Modelica.SIunits.EnergyFlowRate ports_E_flow[nPorts] "flow of kinetic and potential energy at device boundary";
  Medium.MassFlowRate sum_ports_mXi_flow[Medium.nXi] "Substance mass flows through ports";
  Medium.MassFlowRate ports_mXi_flow[nPorts,Medium.nXi];
  Modelica.SIunits.HeatFlowRate Qb_flow "Heat flow across boundaries or energy source/sink";
  parameter Types.Dynamics energyDynamics=system.energyDynamics "Formulation of energy balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
  Modelica.SIunits.MassFlowRate mbXi_flow[Medium.nXi] "Substance mass flows across boundaries";
  parameter Medium.AbsolutePressure p_start=system.p_start "Start value of pressure" annotation(Dialog(tab="Initialization"));
  Modelica.SIunits.Area portAreas[nPorts]={Modelica.Constants.pi/(4)*portsData_diameter[i]^(2) for i in 1:nPorts};
  HeatTransfer heatTransfer(redeclare final package Medium = Medium,surfaceAreas={pistonCrossArea+2*sqrt(pistonCrossArea*pi)*(flange.s+clearance/(pistonCrossArea))},final n=1,final use_k=use_HeatTransfer,final states={medium.state}) annotation(Placement(transformation(extent={{-10,-10},{30,30}},rotation=90,origin={-50,-10})));
  Medium.EnthalpyFlowRate ports_H_flow[nPorts];
  final parameter Types.Dynamics traceDynamics=massDynamics "Formulation of trace substance balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
  Modelica.SIunits.Mass mXi[Medium.nXi] "Masses of independent components in the fluid";
  Medium.Density portDensities[nPorts] "densites of the fluid at the device boudary";
  outer Modelica.Fluid.System system "System properties";
  parameter Integer nPorts=0 "Number of ports" annotation(Evaluate=true,Dialog(connectorSizing=true,tab="General",group="Ports"));
  Medium.ExtraPropertyFlowRate mbC_flow[Medium.nC] "Trace substance mass flows across boundaries";
  Modelica.SIunits.Mass mC[Medium.nC] "Masses of trace substances in the fluid";
  final parameter Types.Dynamics substanceDynamics=massDynamics "Formulation of substance balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
  Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_a heatPort if use_HeatTransfer annotation(Placement(transformation(extent={{-110,-10},{-90,10}})));
  FAME.Dampers.ThermalWithConnectEquations _famefault_heatPort(amount=0.0) if use_HeatTransfer;
  parameter Medium.ExtraProperty C_start[Medium.nC](quantity=Medium.extraPropertiesNames)=fill(0,Medium.nC) "Start value of trace substances" annotation(Dialog(tab="Initialization",enable=Medium.nC>0));
  parameter Boolean use_portsData=true "= false to neglect pressure loss and kinetic energy" annotation(Evaluate=true,Dialog(tab="General",group="Ports"));
  parameter Boolean use_HeatTransfer=false "= true to use the HeatTransfer model" annotation(Dialog(tab="Assumptions",group="Heat transfer"));
  Medium.ExtraProperty C[Medium.nC] "Trace substance mixture content";
  Modelica.Mechanics.Translational.Interfaces.Flange_b flange "translation flange for piston" annotation(Placement(transformation(extent={{-10,90},{10,110}},rotation=0)));
  FAME.Dampers.TranslationalWithoutConnectEquations _famefault_flange(amount=0.0);
  Medium.AbsolutePressure vessel_ps_static[nPorts] "static pressures inside the vessel at the height of the corresponding ports, zero flow velocity";
  Modelica.SIunits.Energy U "Internal energy of fluid";
  parameter Medium.MassFraction X_start[Medium.nX]=Medium.X_default "Start value of mass fractions m_i/m" annotation(Dialog(tab="Initialization",enable=Medium.nXi>0));
  Modelica.SIunits.Volume V "fluid volume";
  Medium.ExtraPropertyFlowRate sum_ports_mC_flow[Medium.nC] "Trace substance mass flows through ports";
  parameter Medium.Temperature T_start=(if use_T_start then system.T_start else Medium.temperature_phX(p_start,h_start,X_start)) "Start value of temperature" annotation(Dialog(tab="Initialization",enable=use_T_start));
  parameter Boolean use_T_start=true "= true, use T_start, otherwise h_start" annotation(Dialog(tab="Initialization"),Evaluate=true);
  Real ports_penetration[nPorts] "penetration of port with fluid, depending on fluid level and port diameter";
  parameter Modelica.SIunits.MassFlowRate m_flow_small(min=0)=system.m_flow_small "Regularization range at zero mass flow rate" annotation(Dialog(tab="Advanced",group="Port properties",enable=stiffCharacteristicForEmptyPort));
  parameter Types.Dynamics massDynamics=system.massDynamics "Formulation of mass balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
  Modelica.SIunits.EnthalpyFlowRate Hb_flow "Enthalpy flow across boundaries or energy source/sink";
  Modelica.SIunits.Power Wb_flow "Work flow across boundaries or source term";
  Modelica.SIunits.MassFlowRate mb_flow "Mass flows across boundaries";
  Modelica.SIunits.Mass m "Mass of fluid";
  parameter Modelica.SIunits.Volume clearance "remaining volume at zero piston stroke";
  Modelica.SIunits.Velocity portVelocities[nPorts] "velocities of fluid flow at device boundary";
  parameter Modelica.SIunits.Area pistonCrossArea "cross sectional area of pistion";
  Medium.BaseProperties medium(preferredMediumStates=true,p(start=p_start),h(start=h_start),T(start=T_start),Xi(start=X_start[1:Medium.nXi]));
  Modelica.Fluid.Vessels.BaseClasses.VesselFluidPorts_b ports[nPorts](redeclare each package Medium = Medium) "Fluid inlets and outlets" annotation(Placement(transformation(extent={{-40,-10},{40,10}},origin={0,-100})));
  Real s[nPorts](each start=fluidLevel_max) "curve parameters for port flows vs. port pressures; for further details see, Modelica Tutorial: Ideal switching devices";
  final input Modelica.SIunits.Volume fluidVolume=V "Volume";
  Medium.ExtraPropertyFlowRate ports_mC_flow[nPorts,Medium.nC];
  parameter Medium.SpecificEnthalpy h_start=(if use_T_start then Medium.specificEnthalpy_pTX(p_start,T_start,X_start) else Medium.h_default) "Start value of specific enthalpy" annotation(Dialog(tab="Initialization",enable=not use_T_start));
  parameter Modelica.Fluid.Vessels.BaseClasses.VesselPortsData portsData[nPorts] if use_portsData "Data of inlet/outlet ports" annotation(Dialog(tab="General",group="Ports",enable=use_portsData));
 protected
  Real mC_scaled[Medium.nC](min=fill(Modelica.Constants.eps,Medium.nC)) "Scaled masses of trace substances in the fluid";
  parameter Modelica.SIunits.Area vesselArea=Modelica.Constants.inf "Area of the vessel used to relate to cross flow area of ports";
  input Modelica.SIunits.Height fluidLevel=0 "level of fluid in the vessel for treating heights of ports";
  parameter Modelica.SIunits.Height fluidLevel_max=1 "maximum level of fluid in the vessel";
  Modelica.Blocks.Interfaces.RealInput portsData_zeta_in_internal[nPorts]=portsData.zeta_in if use_portsData and (nPorts>0);
  Modelica.Blocks.Interfaces.RealInput portsData_zeta_out[nPorts];
  Modelica.Blocks.Interfaces.RealInput portsData_height[nPorts];
  Modelica.Blocks.Interfaces.RealInput portsData_diameter[nPorts];
  Modelica.Blocks.Interfaces.RealInput portsData_zeta_in[nPorts];
  Modelica.Blocks.Interfaces.RealInput portsData_zeta_out_internal[nPorts]=portsData.zeta_out if use_portsData and (nPorts>0);
  parameter Boolean initialize_p=not Medium.singleState "= true to set up initial equations for pressure";
  Modelica.Blocks.Interfaces.RealInput portsData_height_internal[nPorts]=portsData.height if use_portsData and (nPorts>0);
  Modelica.Blocks.Interfaces.RealInput portsData_diameter_internal[nPorts]=portsData.diameter if use_portsData and (nPorts>0);

 // algorithms and equations of SweptVolume
 initial equation
  if energyDynamics==Dynamics.FixedInitial then
   if (Medium.ThermoStates==IndependentVariables.ph) or (Medium.ThermoStates==IndependentVariables.phX) then
    medium.h = h_start;
   else
    medium.T = T_start;
   end if;
  elseif energyDynamics==Dynamics.SteadyStateInitial then
   if (Medium.ThermoStates==IndependentVariables.ph) or (Medium.ThermoStates==IndependentVariables.phX) then
    der(medium.h) = 0;
   else
    der(medium.T) = 0;
   end if;
  end if;
  if massDynamics==Dynamics.FixedInitial then
   if initialize_p then
    medium.p = p_start;
   end if;
  elseif massDynamics==Dynamics.SteadyStateInitial then
   if initialize_p then
    der(medium.p) = 0;
   end if;
  end if;
  if substanceDynamics==Dynamics.FixedInitial then
   medium.Xi = X_start[1:Medium.nXi];
  elseif substanceDynamics==Dynamics.SteadyStateInitial then
   der(medium.Xi) = zeros(Medium.nXi);
  end if;
  if traceDynamics==Dynamics.FixedInitial then
   mC_scaled = m*C_start[1:Medium.nC]./(Medium.C_nominal);
  elseif traceDynamics==Dynamics.SteadyStateInitial then
   der(mC_scaled) = zeros(Medium.nC);
  end if;
 equation
  assert(_famefault_flange.port_b.s>=0,"Piston stroke (given by flange.s) must not be smaller than zero!");
  V = clearance+_famefault_flange.port_b.s*pistonCrossArea;
  0 = _famefault_flange.port_b.f+(medium.p-system.p_ambient)*pistonCrossArea;
  Wb_flow = medium.p*pistonCrossArea*(-der(_famefault_flange.port_b.s));
  for i in 1:nPorts loop
   vessel_ps_static[i] = medium.p;
  end for;
  assert(not (energyDynamics<>Dynamics.SteadyState) and (massDynamics==Dynamics.SteadyState) or Medium.singleState,"Bad combination of dynamics options and Medium not conserving mass if fluidVolume is fixed.");
  m = fluidVolume*medium.d;
  mXi = m*medium.Xi;
  U = m*medium.u;
  mC = m*C;
  if energyDynamics==Dynamics.SteadyState then
   0 = Hb_flow+Qb_flow+Wb_flow;
  else
   der(U) = Hb_flow+Qb_flow+Wb_flow;
  end if;
  if massDynamics==Dynamics.SteadyState then
   0 = mb_flow;
  else
   der(m) = mb_flow;
  end if;
  if substanceDynamics==Dynamics.SteadyState then
   zeros(Medium.nXi) = mbXi_flow;
  else
   der(mXi) = mbXi_flow;
  end if;
  if traceDynamics==Dynamics.SteadyState then
   zeros(Medium.nC) = mbC_flow;
  else
   der(mC_scaled) = mbC_flow./(Medium.C_nominal);
  end if;
  mC = mC_scaled.*Medium.C_nominal;
  mb_flow = sum(ports.m_flow);
  mbXi_flow = sum_ports_mXi_flow;
  mbC_flow = sum_ports_mC_flow;
  Hb_flow = sum(ports_H_flow)+sum(ports_E_flow);
  Qb_flow = heatTransfer.Q_flows[1];
  for i in 1:nPorts loop
   assert(cardinality(ports[i])<=1,"
each ports[i] of volume can at most be connected to one component.
If two or more connections are present, ideal mixing takes
place with these connections, which is usually not the intention
of the modeller. Increase nPorts to add an additional port.
");
  end for;
  assert(fluidLevel<=fluidLevel_max,"Vessel is overflowing (fluidLevel > fluidLevel_max = "+String(fluidLevel)+")");
  assert(fluidLevel>((-1e-6)*fluidLevel_max),"Fluid level (= "+String(fluidLevel)+") is below zero meaning that the solution failed.");
  connect(portsData_diameter,portsData_diameter_internal);
  connect(portsData_height,portsData_height_internal);
  connect(portsData_zeta_in,portsData_zeta_in_internal);
  connect(portsData_zeta_out,portsData_zeta_out_internal);
  if not use_portsData then
   portsData_diameter = zeros(nPorts);
   portsData_height = zeros(nPorts);
   portsData_zeta_in = zeros(nPorts);
   portsData_zeta_out = zeros(nPorts);
  end if;
  for i in 1:nPorts loop
   if use_portsData then
    portDensities[i] = noEvent(Medium.density(Medium.setState_phX(vessel_ps_static[i],actualStream(ports[i].h_outflow),actualStream(ports[i].Xi_outflow))));
    portVelocities[i] = smooth(0,ports[i].m_flow/(portAreas[i])/(portDensities[i]));
    ports_penetration[i] = Modelica.Fluid.Utilities.regStep(fluidLevel-portsData_height[i]-0.1*portsData_diameter[i],1,1e-3,0.1*portsData_diameter[i]);
   else
    portDensities[i] = medium.d;
    portVelocities[i] = 0;
    ports_penetration[i] = 1;
   end if;
   if fluidLevel>=portsData_height[i] then
    if use_portsData then
     ports[i].p = vessel_ps_static[i]+0.5/(portAreas[i]^(2))*Modelica.Fluid.Utilities.regSquare2(ports[i].m_flow,m_flow_small,(portsData_zeta_in[i]-1+portAreas[i]^(2)/(vesselArea^(2)))/(portDensities[i])*ports_penetration[i],(portsData_zeta_out[i]+1-portAreas[i]^(2)/(vesselArea^(2)))/(medium.d)/(ports_penetration[i]));
    else
     ports[i].p = vessel_ps_static[i];
    end if;
    s[i] = fluidLevel-portsData_height[i];
   elseif (s[i]>0) or (portsData_height[i]>=fluidLevel_max) then
    ports[i].p = vessel_ps_static[i];
    s[i] = ports[i].m_flow;
   else
    ports[i].m_flow = 0;
    s[i] = (ports[i].p-vessel_ps_static[i])/(Medium.p_default)*(portsData_height[i]-fluidLevel);
   end if;
   ports[i].h_outflow = medium.h;
   ports[i].Xi_outflow = medium.Xi;
   ports[i].C_outflow = C;
   ports_H_flow[i] = ports[i].m_flow*actualStream(ports[i].h_outflow) "Enthalpy flow";
   ports_E_flow[i] = ports[i].m_flow*(0.5*portVelocities[i]*portVelocities[i]+system.g*portsData_height[i]) "Flow of kinetic and potential energy";
   ports_mXi_flow[i,:] = ports[i].m_flow*actualStream(ports[i].Xi_outflow) "Component mass flow";
   ports_mC_flow[i,:] = ports[i].m_flow*actualStream(ports[i].C_outflow) "Trace substance mass flow";
  end for;
  for i in 1:Medium.nXi loop
   sum_ports_mXi_flow[i] = sum(ports_mXi_flow[:,i]);
  end for;
  for i in 1:Medium.nC loop
   sum_ports_mC_flow[i] = sum(ports_mC_flow[:,i]);
  end for;
  connect(_famefault_heatPort.port_b,heatTransfer.heatPorts[1]) annotation(Line(points={{-100,0},{-87,0},{-87,8.88178e-016},{-74,8.88178e-016}},color={191,0,0},smooth=Smooth.None));
  connect(heatPort,_famefault_heatPort.port_a);
  connect(flange,_famefault_flange.port_a);

 annotation(Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics),Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics={Rectangle(extent={{-50,36},{50,-90}},lineColor={0,0,255},pattern=LinePattern.None,lineThickness=1,fillColor={170,213,255},fillPattern=FillPattern.Solid),Polygon(points={{-52,62},{-48,62},{-48,-30},{-52,-30},{-52,62}},lineColor={95,95,95},smooth=Smooth.None,fillColor={135,135,135},fillPattern=FillPattern.Backward),Polygon(points={{48,60},{52,60},{52,-34},{48,-34},{48,60}},lineColor={95,95,95},smooth=Smooth.None,fillColor={135,135,135},fillPattern=FillPattern.Backward),Rectangle(extent={{-48,40},{48,30}},lineColor={95,95,95},fillColor={135,135,135},fillPattern=FillPattern.Forward),Rectangle(extent={{-6,92},{6,40}},lineColor={95,95,95},fillColor={135,135,135},fillPattern=FillPattern.Forward),Polygon(points={{-48,-90},{48,-90},{48,70},{52,70},{52,-94},{-52,-94},{-52,70},{-48,70},{-48,-90}},lineColor={95,95,95},smooth=Smooth.None,fillColor={135,135,135},fillPattern=FillPattern.Backward),Line(visible=use_HeatTransfer,points={{-100,0},{-52,0}},smooth=Smooth.None,color={198,0,0})}),Documentation(info="<html>
<p> Mixing volume with varying size. The size of the volume is given by:</p>
<ul>
  <li>cross sectional piston area</li>
  <li>piston stroke given by the flange position s</li>
  <li>clearance (volume at flang position = 0)</li>
</ul>
<p>Losses are neglected. The shaft power is completely converted into mechanical work on the fluid.</p>

<p> The flange position has to be equal or greater than zero. Otherwise the simulation stops. The force of the flange results from the pressure difference between medium and ambient pressure and the cross sectional piston area. For using the component, a top level instance of the ambient model with the inner attribute is needed.</p>
<p> The pressure at both fluid ports equals the medium pressure in the volume. No suction nor discharge valve is included in the model.</p>
<p>The thermal port is directly connected to the medium. The temperature of the thermal port equals the medium temperature. The heat capacity of the cylinder and the piston are not includes in the model.</p>
</html>",revisions="<html>
<ul>
<li><i>29 Oct 2007</i>
    by Carsten Heinrich:<br>
       Model added to the Fluid library</li>
</ul>
</html>"));
 end SweptVolume;

 model Pump
  "Centrifugal pump with mechanical connector for the shaft"

  import Modelica.Constants;
  import Modelica.Fluid.Types;
  import Modelica.Fluid.Types.Dynamics;
  import Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables;
  import Modelica.SIunits.Conversions.NonSIunits.*;

 // locally defined classes in Pump
  replaceable   package Medium = Modelica.Media.Interfaces.PartialMedium "Medium in the component" annotation(choicesAllMatching=true);
  replaceable   function flowCharacteristic = PumpCharacteristics.baseFlow "Head vs. V_flow characteristic at nominal speed and density" annotation(Dialog(group="Characteristics"),choicesAllMatching=true);
  replaceable   function powerCharacteristic = PumpCharacteristics.quadraticPower(V_flow_nominal={0,0,0},W_nominal={0,0,0}) "Power consumption vs. V_flow at nominal speed and density" annotation(Dialog(group="Characteristics",enable=use_powerCharacteristic),choicesAllMatching=true);
  replaceable   function efficiencyCharacteristic = PumpCharacteristics.constantEfficiency(eta_nominal=0.8) constrainedby PumpCharacteristics.baseEfficiency;
  replaceable   model HeatTransfer = Modelica.Fluid.Vessels.BaseClasses.HeatTransfer.IdealHeatTransfer constrainedby Modelica.Fluid.Vessels.BaseClasses.HeatTransfer.PartialVesselHeatTransfer;
    model _famefaults_heatPort = FAME.Dampers.ThermalWithConnectEquations;
    model _famefaults_shaft = FAME.Dampers.RotationalWithoutConnectEquations;

 // components of Pump
  Modelica.SIunits.MassFlowRate m_flow=port_a.m_flow "Mass flow rate (total)";
  Real eta "Global Efficiency";
  Modelica.SIunits.MassFlowRate m_flow_single=m_flow/(nParallel) "Mass flow rate (single pump)";
  Modelica.SIunits.Pressure NPSPa=assertPositiveDifference(port_a.p,Medium.saturationPressure(Medium.temperature(state_a)),"Cavitation occurs at the pump inlet") if show_NPSHa "Net Positive Suction Pressure available";
  parameter Boolean show_NPSHa=false "= true to compute Net Positive Suction Head available" annotation(Dialog(tab="Advanced",group="Diagnostics"));
  parameter Modelica.SIunits.Conversions.NonSIunits.AngularVelocity_rpm N_nominal "Nominal rotational speed for flow characteristic" annotation(Dialog(group="Characteristics"));
  Modelica.SIunits.HeatFlowRate Qb_flow "Heat flow across boundaries or energy source/sink";
  parameter Types.Dynamics energyDynamics=Types.Dynamics.SteadyState "Formulation of energy balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
  Modelica.SIunits.MassFlowRate mbXi_flow[Medium.nXi] "Substance mass flows across boundaries";
  final parameter Medium.AbsolutePressure p_start=p_b_start "Start value of pressure" annotation(Dialog(tab="Initialization"));
  Modelica.SIunits.Height head=dp_pump/(rho*g) "Pump head";
  Medium.Density rho=medium.d;
  HeatTransfer heatTransfer(redeclare final package Medium = Medium,final n=1,surfaceAreas={4*Modelica.Constants.pi*(3/(4)*V/(Modelica.Constants.pi))^(2/(3))},final states={medium.state},final use_k=use_HeatTransfer) annotation(Placement(transformation(extent={{-10,-10},{30,30}},rotation=180,origin={50,-10})));
  Modelica.SIunits.Angle phi "Shaft angle";
  Medium.ThermodynamicState state_a=Medium.setState_phX(port_a.p,inStream(port_a.h_outflow),inStream(port_a.Xi_outflow)) if show_NPSHa "state for medium inflowing through port_a";
  final parameter Types.Dynamics traceDynamics=massDynamics "Formulation of trace substance balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
  Modelica.SIunits.Power W_total=W_single*nParallel "Power Consumption (total)";
  Modelica.SIunits.AngularVelocity omega "Shaft angular velocity";
  Modelica.SIunits.Mass mXi[Medium.nXi] "Masses of independent components in the fluid";
  Medium.Density rho_in=Medium.density(state_a) if show_NPSHa "Liquid density at the inlet port_a";
  Modelica.SIunits.VolumeFlowRate V_flow_single(start=m_flow_start/(rho_nominal)/(nParallel))=V_flow/(nParallel) "Volume flow rate (single pump)";
  outer Modelica.Fluid.System system "System properties";
  parameter Boolean use_powerCharacteristic=false "Use powerCharacteristic (vs. efficiencyCharacteristic)" annotation(Evaluate=true,Dialog(group="Characteristics"));
  Medium.ExtraPropertyFlowRate mbC_flow[Medium.nC] "Trace substance mass flows across boundaries";
  Modelica.SIunits.Mass mC[Medium.nC] "Masses of trace substances in the fluid";
  final parameter Types.Dynamics substanceDynamics=massDynamics "Formulation of substance balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
  Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_a heatPort if use_HeatTransfer annotation(Placement(transformation(extent={{30,-70},{50,-50}})));
  FAME.Dampers.ThermalWithConnectEquations _famefault_heatPort(amount=0.0) if use_HeatTransfer;
  Modelica.SIunits.Pressure NPDPa=assertPositiveDifference(port_b.p,Medium.saturationPressure(medium.T),"Cavitation occurs in the pump") if show_NPSHa "Net Positive Discharge Pressure available";
  parameter Medium.ExtraProperty C_start[Medium.nC](quantity=Medium.extraPropertiesNames)=fill(0,Medium.nC) "Start value of trace substances" annotation(Dialog(tab="Initialization",enable=Medium.nC>0));
  parameter Boolean use_HeatTransfer=false "= true to use a HeatTransfer model, e.g., for a housing" annotation(Dialog(tab="Assumptions",group="Heat transfer"));
  Medium.ExtraProperty C[Medium.nC] "Trace substance mixture content";
  parameter Boolean allowFlowReversal=system.allowFlowReversal "= true to allow flow reversal, false restricts to design direction (port_a -> port_b)" annotation(Dialog(tab="Assumptions"),Evaluate=true);
  Modelica.SIunits.Conversions.NonSIunits.AngularVelocity_rpm N(start=N_nominal) "Shaft rotational speed";
  Modelica.SIunits.Energy U "Internal energy of fluid";
  parameter Medium.MassFraction X_start[Medium.nX]=Medium.X_default "Start value of mass fractions m_i/m" annotation(Dialog(tab="Initialization",enable=Medium.nXi>0));
  parameter Medium.MassFlowRate m_flow_start=1 "Guess value of m_flow = port_a.m_flow" annotation(Dialog(tab="Initialization"));
  parameter Modelica.SIunits.Volume V=0 "Volume inside the pump" annotation(Dialog(tab="Assumptions"),Evaluate=true);
  parameter Medium.AbsolutePressure p_b_start=p_a_start "Guess value for outlet pressure" annotation(Dialog(tab="Initialization"));
  parameter Medium.Temperature T_start=(if use_T_start then system.T_start else Medium.temperature_phX(p_start,h_start,X_start)) "Start value of temperature" annotation(Dialog(tab="Initialization",enable=use_T_start));
  parameter Boolean use_T_start=true "= true, use T_start, otherwise h_start" annotation(Dialog(tab="Initialization"),Evaluate=true);
  parameter Medium.AbsolutePressure p_a_start=system.p_start "Guess value for inlet pressure" annotation(Dialog(tab="Initialization"));
  Modelica.SIunits.Pressure dp_pump=port_b.p-port_a.p "Pressure increase";
  final parameter Modelica.SIunits.Acceleration g=system.g;
  parameter Types.Dynamics massDynamics=Types.Dynamics.SteadyState "Formulation of mass balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
  Modelica.SIunits.EnthalpyFlowRate Hb_flow "Enthalpy flow across boundaries or energy source/sink";
  Modelica.SIunits.Power Wb_flow "Work flow across boundaries or source term";
  Modelica.Fluid.Interfaces.FluidPort_b port_b(m_flow(start=-m_flow_start,max=(if allowFlowReversal and not checkValve then Constants.inf else 0)),redeclare package Medium = Medium,p(start=p_b_start)) "Fluid connector b (positive design flow direction is from port_a to port_b)" annotation(Placement(transformation(extent={{110,-10},{90,10}},rotation=0),iconTransformation(extent={{110,-10},{90,10}})));
  Modelica.SIunits.MassFlowRate mb_flow "Mass flows across boundaries";
  Modelica.SIunits.Mass m "Mass of fluid";
  Modelica.Fluid.Interfaces.FluidPort_a port_a(m_flow(start=m_flow_start,min=(if allowFlowReversal and not checkValve then -Constants.inf else 0)),redeclare package Medium = Medium,p(start=p_a_start)) "Fluid connector a (positive design flow direction is from port_a to port_b)" annotation(Placement(transformation(extent={{-110,-10},{-90,10}},rotation=0)));
  Modelica.Mechanics.Rotational.Interfaces.Flange_a shaft annotation(Placement(transformation(extent={{-10,90},{10,110}},rotation=0)));
  FAME.Dampers.RotationalWithoutConnectEquations _famefault_shaft(amount=0.0);
  Medium.BaseProperties medium(preferredMediumStates=true,p(start=p_start),h(start=h_start),T(start=T_start),Xi(start=X_start[1:Medium.nXi]));
  Modelica.SIunits.VolumeFlowRate V_flow=m_flow/(rho) "Volume flow rate (total)";
  Modelica.SIunits.Length NPSHa=NPSPa/(rho_in*system.g) if show_NPSHa "Net Positive Suction Head available";
  parameter Boolean checkValve=false "= true to prevent reverse flow" annotation(Dialog(tab="Assumptions"),Evaluate=true);
  final input Modelica.SIunits.Volume fluidVolume=V "Volume";
  Real s(start=m_flow_start) "Curvilinear abscissa for the flow curve in parametric form (either mass flow rate or head)";
  parameter Medium.Density rho_nominal=Medium.density_pTX(Medium.p_default,Medium.T_default,Medium.X_default) "Nominal fluid density for characteristic" annotation(Dialog(group="Characteristics"));
  parameter Integer nParallel(min=1)=1 "Number of pumps in parallel" annotation(Dialog(group="Characteristics"));
  Modelica.SIunits.Power W_single "Power Consumption (single pump)";
  parameter Medium.SpecificEnthalpy h_start=(if use_T_start then Medium.specificEnthalpy_pTX(p_start,T_start,X_start) else Medium.h_default) "Start value of specific enthalpy" annotation(Dialog(tab="Initialization",enable=not use_T_start));
 protected
  Real mC_scaled[Medium.nC](min=fill(Modelica.Constants.eps,Medium.nC)) "Scaled masses of trace substances in the fluid";
  parameter Boolean port_a_exposesState=false "= true if port_a exposes the state of a fluid volume";
  parameter Boolean showDesignFlowDirection=true "= false to hide the arrow in the model icon";
  constant Modelica.SIunits.Height unitHead=1;
  constant Modelica.SIunits.MassFlowRate unitMassFlowRate=1;
  parameter Boolean port_b_exposesState=(energyDynamics<>Types.Dynamics.SteadyState) or (massDynamics<>Types.Dynamics.SteadyState) "= true if port_b.p exposes the state of a fluid volume";
  parameter Boolean initialize_p=not Medium.singleState "= true to set up initial equations for pressure";

 // algorithms and equations of Pump
 initial equation
  if energyDynamics==Dynamics.FixedInitial then
   if (Medium.ThermoStates==IndependentVariables.ph) or (Medium.ThermoStates==IndependentVariables.phX) then
    medium.h = h_start;
   else
    medium.T = T_start;
   end if;
  elseif energyDynamics==Dynamics.SteadyStateInitial then
   if (Medium.ThermoStates==IndependentVariables.ph) or (Medium.ThermoStates==IndependentVariables.phX) then
    der(medium.h) = 0;
   else
    der(medium.T) = 0;
   end if;
  end if;
  if massDynamics==Dynamics.FixedInitial then
   if initialize_p then
    medium.p = p_start;
   end if;
  elseif massDynamics==Dynamics.SteadyStateInitial then
   if initialize_p then
    der(medium.p) = 0;
   end if;
  end if;
  if substanceDynamics==Dynamics.FixedInitial then
   medium.Xi = X_start[1:Medium.nXi];
  elseif substanceDynamics==Dynamics.SteadyStateInitial then
   der(medium.Xi) = zeros(Medium.nXi);
  end if;
  if traceDynamics==Dynamics.FixedInitial then
   mC_scaled = m*C_start[1:Medium.nC]./(Medium.C_nominal);
  elseif traceDynamics==Dynamics.SteadyStateInitial then
   der(mC_scaled) = zeros(Medium.nC);
  end if;
 equation
  phi = _famefault_shaft.port_b.phi;
  omega = der(phi);
  N = Modelica.SIunits.Conversions.to_rpm(omega);
  W_single = omega*_famefault_shaft.port_b.tau;
  assert(not (energyDynamics<>Dynamics.SteadyState) and (massDynamics==Dynamics.SteadyState) or Medium.singleState,"Bad combination of dynamics options and Medium not conserving mass if fluidVolume is fixed.");
  m = fluidVolume*medium.d;
  mXi = m*medium.Xi;
  U = m*medium.u;
  mC = m*C;
  if energyDynamics==Dynamics.SteadyState then
   0 = Hb_flow+Qb_flow+Wb_flow;
  else
   der(U) = Hb_flow+Qb_flow+Wb_flow;
  end if;
  if massDynamics==Dynamics.SteadyState then
   0 = mb_flow;
  else
   der(m) = mb_flow;
  end if;
  if substanceDynamics==Dynamics.SteadyState then
   zeros(Medium.nXi) = mbXi_flow;
  else
   der(mXi) = mbXi_flow;
  end if;
  if traceDynamics==Dynamics.SteadyState then
   zeros(Medium.nC) = mbC_flow;
  else
   der(mC_scaled) = mbC_flow./(Medium.C_nominal);
  end if;
  mC = mC_scaled.*Medium.C_nominal;
  if not checkValve then
   head = (N/(N_nominal))^(2)*flowCharacteristic(V_flow_single*N_nominal/(N));
   s = 0;
  elseif s>0 then
   head = (N/(N_nominal))^(2)*flowCharacteristic(V_flow_single*N_nominal/(N));
   V_flow_single = s*unitMassFlowRate/(rho);
  else
   head = (N/(N_nominal))^(2)*flowCharacteristic(0)-s*unitHead;
   V_flow_single = 0;
  end if;
  if use_powerCharacteristic then
   W_single = (N/(N_nominal))^(3)*rho/(rho_nominal)*powerCharacteristic(V_flow_single*N_nominal/(N));
   eta = dp_pump*V_flow_single/(W_single);
  else
   eta = efficiencyCharacteristic(V_flow_single*N_nominal/(N));
   W_single = dp_pump*V_flow_single/(eta);
  end if;
  Wb_flow = W_total;
  Qb_flow = heatTransfer.Q_flows[1];
  Hb_flow = port_a.m_flow*actualStream(port_a.h_outflow)+port_b.m_flow*actualStream(port_b.h_outflow);
  port_a.h_outflow = medium.h;
  port_b.h_outflow = medium.h;
  port_b.p = medium.p "outlet pressure is equal to medium pressure, which includes Wb_flow";
  mb_flow = port_a.m_flow+port_b.m_flow;
  mbXi_flow = port_a.m_flow*actualStream(port_a.Xi_outflow)+port_b.m_flow*actualStream(port_b.Xi_outflow);
  port_a.Xi_outflow = medium.Xi;
  port_b.Xi_outflow = medium.Xi;
  mbC_flow = port_a.m_flow*actualStream(port_a.C_outflow)+port_b.m_flow*actualStream(port_b.C_outflow);
  port_a.C_outflow = C;
  port_b.C_outflow = C;
  connect(heatTransfer.heatPorts[1],_famefault_heatPort.port_b) annotation(Line(points={{40,-34},{40,-60}},color={127,0,0},smooth=Smooth.None));
  connect(heatPort,_famefault_heatPort.port_a);
  connect(shaft,_famefault_shaft.port_a);

 annotation(Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics={Rectangle(extent={{-10,100},{10,78}},lineColor={0,0,0},fillPattern=FillPattern.VerticalCylinder,fillColor={95,95,95})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics),Documentation(info="<HTML>
<p>This model describes a centrifugal pump (or a group of <code>nParallel</code> pumps) with a mechanical rotational connector for the shaft, to be used when the pump drive has to be modelled explicitly. In the case of <code>nParallel</code> pumps, the mechanical connector is relative to a single pump.
<p>The model extends <code>PartialPump</code>
 </HTML>",revisions="<html>
<ul>
<li><i>31 Oct 2005</i>
    by <a href=\"mailto:francesco.casella@polimi.it\">Francesco Casella</a>:<br>
       Model added to the Fluid library</li>
</ul>
</html>"));
 end Pump;

 model ControlledPump
  "Centrifugal pump with ideally controlled mass flow rate"

  import Modelica.SIunits.Conversions.NonSIunits.AngularVelocity_rpm;
  import Modelica.Constants;
  import Modelica.Fluid.Types;
  import Modelica.Fluid.Types.Dynamics;
  import Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables;
  import Modelica.SIunits.Conversions.NonSIunits.*;

 // locally defined classes in ControlledPump
  replaceable   package Medium = Modelica.Media.Interfaces.PartialMedium "Medium in the component" annotation(choicesAllMatching=true);
  replaceable   function flowCharacteristic = PumpCharacteristics.baseFlow "Head vs. V_flow characteristic at nominal speed and density" annotation(Dialog(group="Characteristics"),choicesAllMatching=true);
  replaceable   function powerCharacteristic = PumpCharacteristics.quadraticPower(V_flow_nominal={0,0,0},W_nominal={0,0,0}) "Power consumption vs. V_flow at nominal speed and density" annotation(Dialog(group="Characteristics",enable=use_powerCharacteristic),choicesAllMatching=true);
  replaceable   function efficiencyCharacteristic = PumpCharacteristics.constantEfficiency(eta_nominal=0.8) constrainedby PumpCharacteristics.baseEfficiency;
  replaceable   model HeatTransfer = Modelica.Fluid.Vessels.BaseClasses.HeatTransfer.IdealHeatTransfer constrainedby Modelica.Fluid.Vessels.BaseClasses.HeatTransfer.PartialVesselHeatTransfer;
    model _famefaults_heatPort = FAME.Dampers.ThermalWithConnectEquations;

 // components of ControlledPump
  Modelica.SIunits.MassFlowRate m_flow=port_a.m_flow "Mass flow rate (total)";
  Real eta "Global Efficiency";
  Modelica.SIunits.MassFlowRate m_flow_single=m_flow/(nParallel) "Mass flow rate (single pump)";
  Modelica.SIunits.Pressure NPSPa=assertPositiveDifference(port_a.p,Medium.saturationPressure(Medium.temperature(state_a)),"Cavitation occurs at the pump inlet") if show_NPSHa "Net Positive Suction Pressure available";
  parameter Boolean show_NPSHa=false "= true to compute Net Positive Suction Head available" annotation(Dialog(tab="Advanced",group="Diagnostics"));
  parameter Modelica.SIunits.Conversions.NonSIunits.AngularVelocity_rpm N_nominal=1500 "Nominal rotational speed for flow characteristic" annotation(Dialog(group="Characteristics"));
  final parameter Modelica.SIunits.VolumeFlowRate V_flow_op=m_flow_nominal/(rho_nominal) "operational volume flow rate according to nominal values";
  Modelica.SIunits.HeatFlowRate Qb_flow "Heat flow across boundaries or energy source/sink";
  parameter Types.Dynamics energyDynamics=Types.Dynamics.SteadyState "Formulation of energy balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
  Modelica.SIunits.MassFlowRate mbXi_flow[Medium.nXi] "Substance mass flows across boundaries";
  final parameter Medium.AbsolutePressure p_start=p_b_start "Start value of pressure" annotation(Dialog(tab="Initialization"));
  Modelica.SIunits.Height head=dp_pump/(rho*g) "Pump head";
  Medium.Density rho=medium.d;
  HeatTransfer heatTransfer(redeclare final package Medium = Medium,final n=1,surfaceAreas={4*Modelica.Constants.pi*(3/(4)*V/(Modelica.Constants.pi))^(2/(3))},final states={medium.state},final use_k=use_HeatTransfer) annotation(Placement(transformation(extent={{-10,-10},{30,30}},rotation=180,origin={50,-10})));
  Modelica.Blocks.Interfaces.RealInput m_flow_set if use_m_flow_set "Prescribed mass flow rate" annotation(Placement(transformation(extent={{-20,-20},{20,20}},rotation=-90,origin={-50,82})));
  Medium.ThermodynamicState state_a=Medium.setState_phX(port_a.p,inStream(port_a.h_outflow),inStream(port_a.Xi_outflow)) if show_NPSHa "state for medium inflowing through port_a";
  final parameter Types.Dynamics traceDynamics=massDynamics "Formulation of trace substance balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
  Modelica.SIunits.Power W_total=W_single*nParallel "Power Consumption (total)";
  parameter Medium.AbsolutePressure p_b_nominal "Nominal outlet pressure, fixed if not control_m_flow and not use_p_set";
  parameter Medium.AbsolutePressure p_a_nominal "Nominal inlet pressure for predefined pump characteristics";
  Modelica.SIunits.Mass mXi[Medium.nXi] "Masses of independent components in the fluid";
  Medium.Density rho_in=Medium.density(state_a) if show_NPSHa "Liquid density at the inlet port_a";
  Modelica.SIunits.VolumeFlowRate V_flow_single(start=m_flow_start/(rho_nominal)/(nParallel))=V_flow/(nParallel) "Volume flow rate (single pump)";
  outer Modelica.Fluid.System system "System properties";
  parameter Boolean use_p_set=false "= true to use input signal p_set instead of p_b_nominal" annotation(Dialog(enable=not control_m_flow));
  Modelica.Blocks.Interfaces.RealInput p_set if use_p_set "Prescribed outlet pressure" annotation(Placement(transformation(extent={{-20,-20},{20,20}},rotation=-90,origin={50,82})));
  final parameter Modelica.SIunits.Height head_op=(p_b_nominal-p_a_nominal)/(rho_nominal*g) "operational pump head according to nominal values";
  parameter Boolean use_powerCharacteristic=false "Use powerCharacteristic (vs. efficiencyCharacteristic)" annotation(Evaluate=true,Dialog(group="Characteristics"));
  Medium.ExtraPropertyFlowRate mbC_flow[Medium.nC] "Trace substance mass flows across boundaries";
  Modelica.SIunits.Mass mC[Medium.nC] "Masses of trace substances in the fluid";
  final parameter Types.Dynamics substanceDynamics=massDynamics "Formulation of substance balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
  Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_a heatPort if use_HeatTransfer annotation(Placement(transformation(extent={{30,-70},{50,-50}})));
  FAME.Dampers.ThermalWithConnectEquations _famefault_heatPort(amount=0.0) if use_HeatTransfer;
  Modelica.SIunits.Pressure NPDPa=assertPositiveDifference(port_b.p,Medium.saturationPressure(medium.T),"Cavitation occurs in the pump") if show_NPSHa "Net Positive Discharge Pressure available";
  parameter Medium.ExtraProperty C_start[Medium.nC](quantity=Medium.extraPropertiesNames)=fill(0,Medium.nC) "Start value of trace substances" annotation(Dialog(tab="Initialization",enable=Medium.nC>0));
  parameter Boolean use_HeatTransfer=false "= true to use a HeatTransfer model, e.g., for a housing" annotation(Dialog(tab="Assumptions",group="Heat transfer"));
  Medium.ExtraProperty C[Medium.nC] "Trace substance mixture content";
  parameter Boolean allowFlowReversal=system.allowFlowReversal "= true to allow flow reversal, false restricts to design direction (port_a -> port_b)" annotation(Dialog(tab="Assumptions"),Evaluate=true);
  Modelica.SIunits.Conversions.NonSIunits.AngularVelocity_rpm N(start=N_nominal) "Shaft rotational speed";
  Modelica.SIunits.Energy U "Internal energy of fluid";
  parameter Medium.MassFraction X_start[Medium.nX]=Medium.X_default "Start value of mass fractions m_i/m" annotation(Dialog(tab="Initialization",enable=Medium.nXi>0));
  parameter Medium.MassFlowRate m_flow_start=1 "Guess value of m_flow = port_a.m_flow" annotation(Dialog(tab="Initialization"));
  parameter Modelica.SIunits.Volume V=0 "Volume inside the pump" annotation(Dialog(tab="Assumptions"),Evaluate=true);
  parameter Medium.AbsolutePressure p_b_start=p_a_start "Guess value for outlet pressure" annotation(Dialog(tab="Initialization"));
  parameter Medium.MassFlowRate m_flow_nominal "Nominal mass flow rate, fixed if control_m_flow and not use_m_flow_set";
  parameter Medium.Temperature T_start=(if use_T_start then system.T_start else Medium.temperature_phX(p_start,h_start,X_start)) "Start value of temperature" annotation(Dialog(tab="Initialization",enable=use_T_start));
  parameter Boolean use_T_start=true "= true, use T_start, otherwise h_start" annotation(Dialog(tab="Initialization"),Evaluate=true);
  parameter Medium.AbsolutePressure p_a_start=system.p_start "Guess value for inlet pressure" annotation(Dialog(tab="Initialization"));
  Modelica.SIunits.Pressure dp_pump=port_b.p-port_a.p "Pressure increase";
  final parameter Modelica.SIunits.Acceleration g=system.g;
  parameter Boolean use_m_flow_set=false "= true to use input signal m_flow_set instead of m_flow_nominal" annotation(Dialog(enable=control_m_flow));
  parameter Types.Dynamics massDynamics=Types.Dynamics.SteadyState "Formulation of mass balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
  Modelica.SIunits.EnthalpyFlowRate Hb_flow "Enthalpy flow across boundaries or energy source/sink";
  Modelica.SIunits.Power Wb_flow "Work flow across boundaries or source term";
  Modelica.Fluid.Interfaces.FluidPort_b port_b(m_flow(start=-m_flow_start,max=(if allowFlowReversal and not checkValve then Constants.inf else 0)),redeclare package Medium = Medium,p(start=p_b_start)) "Fluid connector b (positive design flow direction is from port_a to port_b)" annotation(Placement(transformation(extent={{110,-10},{90,10}},rotation=0),iconTransformation(extent={{110,-10},{90,10}})));
  Modelica.SIunits.MassFlowRate mb_flow "Mass flows across boundaries";
  Modelica.SIunits.Mass m "Mass of fluid";
  Modelica.Fluid.Interfaces.FluidPort_a port_a(m_flow(start=m_flow_start,min=(if allowFlowReversal and not checkValve then -Constants.inf else 0)),redeclare package Medium = Medium,p(start=p_a_start)) "Fluid connector a (positive design flow direction is from port_a to port_b)" annotation(Placement(transformation(extent={{-110,-10},{-90,10}},rotation=0)));
  Medium.BaseProperties medium(preferredMediumStates=true,p(start=p_start),h(start=h_start),T(start=T_start),Xi(start=X_start[1:Medium.nXi]));
  Modelica.SIunits.VolumeFlowRate V_flow=m_flow/(rho) "Volume flow rate (total)";
  Modelica.SIunits.Length NPSHa=NPSPa/(rho_in*system.g) if show_NPSHa "Net Positive Suction Head available";
  parameter Boolean checkValve=false "= true to prevent reverse flow" annotation(Dialog(tab="Assumptions"),Evaluate=true);
  final input Modelica.SIunits.Volume fluidVolume=V "Volume";
  Real s(start=m_flow_start) "Curvilinear abscissa for the flow curve in parametric form (either mass flow rate or head)";
  parameter Medium.Density rho_nominal=Medium.density_pTX(Medium.p_default,Medium.T_default,Medium.X_default) "Nominal fluid density for characteristic" annotation(Dialog(group="Characteristics"));
  parameter Integer nParallel(min=1)=1 "Number of pumps in parallel" annotation(Dialog(group="Characteristics"));
  Modelica.SIunits.Power W_single "Power Consumption (single pump)";
  parameter Boolean control_m_flow=true "= false to control outlet pressure port_b.p instead of m_flow" annotation(Evaluate=true);
  parameter Medium.SpecificEnthalpy h_start=(if use_T_start then Medium.specificEnthalpy_pTX(p_start,T_start,X_start) else Medium.h_default) "Start value of specific enthalpy" annotation(Dialog(tab="Initialization",enable=not use_T_start));
 protected
  Modelica.Blocks.Interfaces.RealInput p_set_internal "Needed to connect to conditional connector";
  Real mC_scaled[Medium.nC](min=fill(Modelica.Constants.eps,Medium.nC)) "Scaled masses of trace substances in the fluid";
  parameter Boolean port_a_exposesState=false "= true if port_a exposes the state of a fluid volume";
  parameter Boolean showDesignFlowDirection=true "= false to hide the arrow in the model icon";
  constant Modelica.SIunits.Height unitHead=1;
  constant Modelica.SIunits.MassFlowRate unitMassFlowRate=1;
  parameter Boolean port_b_exposesState=(energyDynamics<>Types.Dynamics.SteadyState) or (massDynamics<>Types.Dynamics.SteadyState) "= true if port_b.p exposes the state of a fluid volume";
  parameter Boolean initialize_p=not Medium.singleState "= true to set up initial equations for pressure";
  Modelica.Blocks.Interfaces.RealInput m_flow_set_internal "Needed to connect to conditional connector";

 // algorithms and equations of ControlledPump
 initial equation
  if energyDynamics==Dynamics.FixedInitial then
   if (Medium.ThermoStates==IndependentVariables.ph) or (Medium.ThermoStates==IndependentVariables.phX) then
    medium.h = h_start;
   else
    medium.T = T_start;
   end if;
  elseif energyDynamics==Dynamics.SteadyStateInitial then
   if (Medium.ThermoStates==IndependentVariables.ph) or (Medium.ThermoStates==IndependentVariables.phX) then
    der(medium.h) = 0;
   else
    der(medium.T) = 0;
   end if;
  end if;
  if massDynamics==Dynamics.FixedInitial then
   if initialize_p then
    medium.p = p_start;
   end if;
  elseif massDynamics==Dynamics.SteadyStateInitial then
   if initialize_p then
    der(medium.p) = 0;
   end if;
  end if;
  if substanceDynamics==Dynamics.FixedInitial then
   medium.Xi = X_start[1:Medium.nXi];
  elseif substanceDynamics==Dynamics.SteadyStateInitial then
   der(medium.Xi) = zeros(Medium.nXi);
  end if;
  if traceDynamics==Dynamics.FixedInitial then
   mC_scaled = m*C_start[1:Medium.nC]./(Medium.C_nominal);
  elseif traceDynamics==Dynamics.SteadyStateInitial then
   der(mC_scaled) = zeros(Medium.nC);
  end if;
 equation
  if control_m_flow then
   m_flow = m_flow_set_internal;
  else
   dp_pump = p_set_internal-port_a.p;
  end if;
  if not use_m_flow_set then
   m_flow_set_internal = m_flow_nominal;
  end if;
  if not use_p_set then
   p_set_internal = p_b_nominal;
  end if;
  connect(m_flow_set,m_flow_set_internal);
  connect(p_set,p_set_internal);
  assert(not (energyDynamics<>Dynamics.SteadyState) and (massDynamics==Dynamics.SteadyState) or Medium.singleState,"Bad combination of dynamics options and Medium not conserving mass if fluidVolume is fixed.");
  m = fluidVolume*medium.d;
  mXi = m*medium.Xi;
  U = m*medium.u;
  mC = m*C;
  if energyDynamics==Dynamics.SteadyState then
   0 = Hb_flow+Qb_flow+Wb_flow;
  else
   der(U) = Hb_flow+Qb_flow+Wb_flow;
  end if;
  if massDynamics==Dynamics.SteadyState then
   0 = mb_flow;
  else
   der(m) = mb_flow;
  end if;
  if substanceDynamics==Dynamics.SteadyState then
   zeros(Medium.nXi) = mbXi_flow;
  else
   der(mXi) = mbXi_flow;
  end if;
  if traceDynamics==Dynamics.SteadyState then
   zeros(Medium.nC) = mbC_flow;
  else
   der(mC_scaled) = mbC_flow./(Medium.C_nominal);
  end if;
  mC = mC_scaled.*Medium.C_nominal;
  if not checkValve then
   head = (N/(N_nominal))^(2)*flowCharacteristic(V_flow_single*N_nominal/(N));
   s = 0;
  elseif s>0 then
   head = (N/(N_nominal))^(2)*flowCharacteristic(V_flow_single*N_nominal/(N));
   V_flow_single = s*unitMassFlowRate/(rho);
  else
   head = (N/(N_nominal))^(2)*flowCharacteristic(0)-s*unitHead;
   V_flow_single = 0;
  end if;
  if use_powerCharacteristic then
   W_single = (N/(N_nominal))^(3)*rho/(rho_nominal)*powerCharacteristic(V_flow_single*N_nominal/(N));
   eta = dp_pump*V_flow_single/(W_single);
  else
   eta = efficiencyCharacteristic(V_flow_single*N_nominal/(N));
   W_single = dp_pump*V_flow_single/(eta);
  end if;
  Wb_flow = W_total;
  Qb_flow = heatTransfer.Q_flows[1];
  Hb_flow = port_a.m_flow*actualStream(port_a.h_outflow)+port_b.m_flow*actualStream(port_b.h_outflow);
  port_a.h_outflow = medium.h;
  port_b.h_outflow = medium.h;
  port_b.p = medium.p "outlet pressure is equal to medium pressure, which includes Wb_flow";
  mb_flow = port_a.m_flow+port_b.m_flow;
  mbXi_flow = port_a.m_flow*actualStream(port_a.Xi_outflow)+port_b.m_flow*actualStream(port_b.Xi_outflow);
  port_a.Xi_outflow = medium.Xi;
  port_b.Xi_outflow = medium.Xi;
  mbC_flow = port_a.m_flow*actualStream(port_a.C_outflow)+port_b.m_flow*actualStream(port_b.C_outflow);
  port_a.C_outflow = C;
  port_b.C_outflow = C;
  connect(heatTransfer.heatPorts[1],_famefault_heatPort.port_b) annotation(Line(points={{40,-34},{40,-60}},color={127,0,0},smooth=Smooth.None));
  connect(heatPort,_famefault_heatPort.port_a);

 annotation(defaultComponentName="pump",Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics={Text(visible=use_p_set,extent={{82,108},{176,92}},textString="p_set"),Text(visible=use_m_flow_set,extent={{-20,108},{170,92}},textString="m_flow_set")}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics),Documentation(info="<HTML>
<p>
This model describes a centrifugal pump (or a group of <code>nParallel</code> pumps)
with ideally controlled mass flow rate or pressure.
</p>
<p>
Nominal values are used to predefine an exemplary pump characteristics and to define the operation of the pump.
The input connectors <code>m_flow_set</code> or <code>p_set</code> can optionally be enabled to provide time varying set points.
</p>
<p>
Use this model if the pump characteristics is of secondary interest.
The actual characteristics can be configured later on for the appropriate rotational speed N.
Then the model can be replaced with a Pump with rotational shaft or with a PrescribedPump.
</p>
</HTML>",revisions="<html>
<ul>
<li><i>15 Dec 2008</i>
    by Ruediger Franke</a>:<br>
       Model added to the Fluid library</li>
</ul>
</html>"));
 end ControlledPump;

 model PrescribedPump
  "Centrifugal pump with ideally controlled speed"

  import Modelica.Constants;
  import Modelica.Fluid.Types;
  import Modelica.Fluid.Types.Dynamics;
  import Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables;
  import Modelica.SIunits.Conversions.NonSIunits.*;

 // locally defined classes in PrescribedPump
  replaceable   package Medium = Modelica.Media.Interfaces.PartialMedium "Medium in the component" annotation(choicesAllMatching=true);
  replaceable   function flowCharacteristic = PumpCharacteristics.baseFlow "Head vs. V_flow characteristic at nominal speed and density" annotation(Dialog(group="Characteristics"),choicesAllMatching=true);
  replaceable   function powerCharacteristic = PumpCharacteristics.quadraticPower(V_flow_nominal={0,0,0},W_nominal={0,0,0}) "Power consumption vs. V_flow at nominal speed and density" annotation(Dialog(group="Characteristics",enable=use_powerCharacteristic),choicesAllMatching=true);
  replaceable   function efficiencyCharacteristic = PumpCharacteristics.constantEfficiency(eta_nominal=0.8) constrainedby PumpCharacteristics.baseEfficiency;
  replaceable   model HeatTransfer = Modelica.Fluid.Vessels.BaseClasses.HeatTransfer.IdealHeatTransfer constrainedby Modelica.Fluid.Vessels.BaseClasses.HeatTransfer.PartialVesselHeatTransfer;
    model _famefaults_heatPort = FAME.Dampers.ThermalWithConnectEquations;

 // components of PrescribedPump
  Modelica.SIunits.MassFlowRate m_flow=port_a.m_flow "Mass flow rate (total)";
  Real eta "Global Efficiency";
  Modelica.SIunits.MassFlowRate m_flow_single=m_flow/(nParallel) "Mass flow rate (single pump)";
  Modelica.SIunits.Pressure NPSPa=assertPositiveDifference(port_a.p,Medium.saturationPressure(Medium.temperature(state_a)),"Cavitation occurs at the pump inlet") if show_NPSHa "Net Positive Suction Pressure available";
  parameter Boolean show_NPSHa=false "= true to compute Net Positive Suction Head available" annotation(Dialog(tab="Advanced",group="Diagnostics"));
  parameter Modelica.SIunits.Conversions.NonSIunits.AngularVelocity_rpm N_nominal "Nominal rotational speed for flow characteristic" annotation(Dialog(group="Characteristics"));
  Modelica.SIunits.HeatFlowRate Qb_flow "Heat flow across boundaries or energy source/sink";
  parameter Types.Dynamics energyDynamics=Types.Dynamics.SteadyState "Formulation of energy balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
  Modelica.SIunits.MassFlowRate mbXi_flow[Medium.nXi] "Substance mass flows across boundaries";
  final parameter Medium.AbsolutePressure p_start=p_b_start "Start value of pressure" annotation(Dialog(tab="Initialization"));
  Modelica.SIunits.Height head=dp_pump/(rho*g) "Pump head";
  Medium.Density rho=medium.d;
  HeatTransfer heatTransfer(redeclare final package Medium = Medium,final n=1,surfaceAreas={4*Modelica.Constants.pi*(3/(4)*V/(Modelica.Constants.pi))^(2/(3))},final states={medium.state},final use_k=use_HeatTransfer) annotation(Placement(transformation(extent={{-10,-10},{30,30}},rotation=180,origin={50,-10})));
  Medium.ThermodynamicState state_a=Medium.setState_phX(port_a.p,inStream(port_a.h_outflow),inStream(port_a.Xi_outflow)) if show_NPSHa "state for medium inflowing through port_a";
  final parameter Types.Dynamics traceDynamics=massDynamics "Formulation of trace substance balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
  Modelica.SIunits.Power W_total=W_single*nParallel "Power Consumption (total)";
  parameter Modelica.SIunits.Conversions.NonSIunits.AngularVelocity_rpm N_const=N_nominal "Constant rotational speed" annotation(Dialog(enable=not use_N_in));
  Modelica.SIunits.Mass mXi[Medium.nXi] "Masses of independent components in the fluid";
  Medium.Density rho_in=Medium.density(state_a) if show_NPSHa "Liquid density at the inlet port_a";
  Modelica.SIunits.VolumeFlowRate V_flow_single(start=m_flow_start/(rho_nominal)/(nParallel))=V_flow/(nParallel) "Volume flow rate (single pump)";
  outer Modelica.Fluid.System system "System properties";
  parameter Boolean use_powerCharacteristic=false "Use powerCharacteristic (vs. efficiencyCharacteristic)" annotation(Evaluate=true,Dialog(group="Characteristics"));
  Medium.ExtraPropertyFlowRate mbC_flow[Medium.nC] "Trace substance mass flows across boundaries";
  Modelica.SIunits.Mass mC[Medium.nC] "Masses of trace substances in the fluid";
  final parameter Types.Dynamics substanceDynamics=massDynamics "Formulation of substance balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
  Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_a heatPort if use_HeatTransfer annotation(Placement(transformation(extent={{30,-70},{50,-50}})));
  FAME.Dampers.ThermalWithConnectEquations _famefault_heatPort(amount=0.0) if use_HeatTransfer;
  Modelica.SIunits.Pressure NPDPa=assertPositiveDifference(port_b.p,Medium.saturationPressure(medium.T),"Cavitation occurs in the pump") if show_NPSHa "Net Positive Discharge Pressure available";
  parameter Medium.ExtraProperty C_start[Medium.nC](quantity=Medium.extraPropertiesNames)=fill(0,Medium.nC) "Start value of trace substances" annotation(Dialog(tab="Initialization",enable=Medium.nC>0));
  parameter Boolean use_HeatTransfer=false "= true to use a HeatTransfer model, e.g., for a housing" annotation(Dialog(tab="Assumptions",group="Heat transfer"));
  Medium.ExtraProperty C[Medium.nC] "Trace substance mixture content";
  parameter Boolean allowFlowReversal=system.allowFlowReversal "= true to allow flow reversal, false restricts to design direction (port_a -> port_b)" annotation(Dialog(tab="Assumptions"),Evaluate=true);
  Modelica.SIunits.Conversions.NonSIunits.AngularVelocity_rpm N(start=N_nominal) "Shaft rotational speed";
  Modelica.SIunits.Energy U "Internal energy of fluid";
  parameter Medium.MassFraction X_start[Medium.nX]=Medium.X_default "Start value of mass fractions m_i/m" annotation(Dialog(tab="Initialization",enable=Medium.nXi>0));
  parameter Medium.MassFlowRate m_flow_start=1 "Guess value of m_flow = port_a.m_flow" annotation(Dialog(tab="Initialization"));
  parameter Modelica.SIunits.Volume V=0 "Volume inside the pump" annotation(Dialog(tab="Assumptions"),Evaluate=true);
  parameter Medium.AbsolutePressure p_b_start=p_a_start "Guess value for outlet pressure" annotation(Dialog(tab="Initialization"));
  parameter Medium.Temperature T_start=(if use_T_start then system.T_start else Medium.temperature_phX(p_start,h_start,X_start)) "Start value of temperature" annotation(Dialog(tab="Initialization",enable=use_T_start));
  parameter Boolean use_T_start=true "= true, use T_start, otherwise h_start" annotation(Dialog(tab="Initialization"),Evaluate=true);
  parameter Medium.AbsolutePressure p_a_start=system.p_start "Guess value for inlet pressure" annotation(Dialog(tab="Initialization"));
  Modelica.SIunits.Pressure dp_pump=port_b.p-port_a.p "Pressure increase";
  final parameter Modelica.SIunits.Acceleration g=system.g;
  parameter Types.Dynamics massDynamics=Types.Dynamics.SteadyState "Formulation of mass balance" annotation(Evaluate=true,Dialog(tab="Assumptions",group="Dynamics"));
  Modelica.SIunits.EnthalpyFlowRate Hb_flow "Enthalpy flow across boundaries or energy source/sink";
  Modelica.SIunits.Power Wb_flow "Work flow across boundaries or source term";
  Modelica.Fluid.Interfaces.FluidPort_b port_b(m_flow(start=-m_flow_start,max=(if allowFlowReversal and not checkValve then Constants.inf else 0)),redeclare package Medium = Medium,p(start=p_b_start)) "Fluid connector b (positive design flow direction is from port_a to port_b)" annotation(Placement(transformation(extent={{110,-10},{90,10}},rotation=0),iconTransformation(extent={{110,-10},{90,10}})));
  Modelica.SIunits.MassFlowRate mb_flow "Mass flows across boundaries";
  Modelica.SIunits.Mass m "Mass of fluid";
  Modelica.Fluid.Interfaces.FluidPort_a port_a(m_flow(start=m_flow_start,min=(if allowFlowReversal and not checkValve then -Constants.inf else 0)),redeclare package Medium = Medium,p(start=p_a_start)) "Fluid connector a (positive design flow direction is from port_a to port_b)" annotation(Placement(transformation(extent={{-110,-10},{-90,10}},rotation=0)));
  Medium.BaseProperties medium(preferredMediumStates=true,p(start=p_start),h(start=h_start),T(start=T_start),Xi(start=X_start[1:Medium.nXi]));
  Modelica.SIunits.VolumeFlowRate V_flow=m_flow/(rho) "Volume flow rate (total)";
  Modelica.SIunits.Length NPSHa=NPSPa/(rho_in*system.g) if show_NPSHa "Net Positive Suction Head available";
  parameter Boolean checkValve=false "= true to prevent reverse flow" annotation(Dialog(tab="Assumptions"),Evaluate=true);
  final input Modelica.SIunits.Volume fluidVolume=V "Volume";
  Real s(start=m_flow_start) "Curvilinear abscissa for the flow curve in parametric form (either mass flow rate or head)";
  parameter Medium.Density rho_nominal=Medium.density_pTX(Medium.p_default,Medium.T_default,Medium.X_default) "Nominal fluid density for characteristic" annotation(Dialog(group="Characteristics"));
  parameter Integer nParallel(min=1)=1 "Number of pumps in parallel" annotation(Dialog(group="Characteristics"));
  Modelica.SIunits.Power W_single "Power Consumption (single pump)";
  parameter Boolean use_N_in=false "Get the rotational speed from the input connector";
  Modelica.Blocks.Interfaces.RealInput N_in(unit="1/min") if use_N_in "Prescribed rotational speed" annotation(Placement(transformation(extent={{-20,-20},{20,20}},rotation=-90,origin={0,100}),iconTransformation(extent={{-20,-20},{20,20}},rotation=-90,origin={0,100})));
  parameter Medium.SpecificEnthalpy h_start=(if use_T_start then Medium.specificEnthalpy_pTX(p_start,T_start,X_start) else Medium.h_default) "Start value of specific enthalpy" annotation(Dialog(tab="Initialization",enable=not use_T_start));
 protected
  Real mC_scaled[Medium.nC](min=fill(Modelica.Constants.eps,Medium.nC)) "Scaled masses of trace substances in the fluid";
  parameter Boolean port_a_exposesState=false "= true if port_a exposes the state of a fluid volume";
  Modelica.Blocks.Interfaces.RealInput N_in_internal(unit="1/min") "Needed to connect to conditional connector";
  parameter Boolean showDesignFlowDirection=true "= false to hide the arrow in the model icon";
  constant Modelica.SIunits.Height unitHead=1;
  constant Modelica.SIunits.MassFlowRate unitMassFlowRate=1;
  parameter Boolean port_b_exposesState=(energyDynamics<>Types.Dynamics.SteadyState) or (massDynamics<>Types.Dynamics.SteadyState) "= true if port_b.p exposes the state of a fluid volume";
  parameter Boolean initialize_p=not Medium.singleState "= true to set up initial equations for pressure";

 // algorithms and equations of PrescribedPump
 initial equation
  if energyDynamics==Dynamics.FixedInitial then
   if (Medium.ThermoStates==IndependentVariables.ph) or (Medium.ThermoStates==IndependentVariables.phX) then
    medium.h = h_start;
   else
    medium.T = T_start;
   end if;
  elseif energyDynamics==Dynamics.SteadyStateInitial then
   if (Medium.ThermoStates==IndependentVariables.ph) or (Medium.ThermoStates==IndependentVariables.phX) then
    der(medium.h) = 0;
   else
    der(medium.T) = 0;
   end if;
  end if;
  if massDynamics==Dynamics.FixedInitial then
   if initialize_p then
    medium.p = p_start;
   end if;
  elseif massDynamics==Dynamics.SteadyStateInitial then
   if initialize_p then
    der(medium.p) = 0;
   end if;
  end if;
  if substanceDynamics==Dynamics.FixedInitial then
   medium.Xi = X_start[1:Medium.nXi];
  elseif substanceDynamics==Dynamics.SteadyStateInitial then
   der(medium.Xi) = zeros(Medium.nXi);
  end if;
  if traceDynamics==Dynamics.FixedInitial then
   mC_scaled = m*C_start[1:Medium.nC]./(Medium.C_nominal);
  elseif traceDynamics==Dynamics.SteadyStateInitial then
   der(mC_scaled) = zeros(Medium.nC);
  end if;
 equation
  connect(N_in,N_in_internal);
  if not use_N_in then
   N_in_internal = N_const;
  end if;
  N = max(N_in_internal,1e-3) "Rotational speed";
  assert(not (energyDynamics<>Dynamics.SteadyState) and (massDynamics==Dynamics.SteadyState) or Medium.singleState,"Bad combination of dynamics options and Medium not conserving mass if fluidVolume is fixed.");
  m = fluidVolume*medium.d;
  mXi = m*medium.Xi;
  U = m*medium.u;
  mC = m*C;
  if energyDynamics==Dynamics.SteadyState then
   0 = Hb_flow+Qb_flow+Wb_flow;
  else
   der(U) = Hb_flow+Qb_flow+Wb_flow;
  end if;
  if massDynamics==Dynamics.SteadyState then
   0 = mb_flow;
  else
   der(m) = mb_flow;
  end if;
  if substanceDynamics==Dynamics.SteadyState then
   zeros(Medium.nXi) = mbXi_flow;
  else
   der(mXi) = mbXi_flow;
  end if;
  if traceDynamics==Dynamics.SteadyState then
   zeros(Medium.nC) = mbC_flow;
  else
   der(mC_scaled) = mbC_flow./(Medium.C_nominal);
  end if;
  mC = mC_scaled.*Medium.C_nominal;
  if not checkValve then
   head = (N/(N_nominal))^(2)*flowCharacteristic(V_flow_single*N_nominal/(N));
   s = 0;
  elseif s>0 then
   head = (N/(N_nominal))^(2)*flowCharacteristic(V_flow_single*N_nominal/(N));
   V_flow_single = s*unitMassFlowRate/(rho);
  else
   head = (N/(N_nominal))^(2)*flowCharacteristic(0)-s*unitHead;
   V_flow_single = 0;
  end if;
  if use_powerCharacteristic then
   W_single = (N/(N_nominal))^(3)*rho/(rho_nominal)*powerCharacteristic(V_flow_single*N_nominal/(N));
   eta = dp_pump*V_flow_single/(W_single);
  else
   eta = efficiencyCharacteristic(V_flow_single*N_nominal/(N));
   W_single = dp_pump*V_flow_single/(eta);
  end if;
  Wb_flow = W_total;
  Qb_flow = heatTransfer.Q_flows[1];
  Hb_flow = port_a.m_flow*actualStream(port_a.h_outflow)+port_b.m_flow*actualStream(port_b.h_outflow);
  port_a.h_outflow = medium.h;
  port_b.h_outflow = medium.h;
  port_b.p = medium.p "outlet pressure is equal to medium pressure, which includes Wb_flow";
  mb_flow = port_a.m_flow+port_b.m_flow;
  mbXi_flow = port_a.m_flow*actualStream(port_a.Xi_outflow)+port_b.m_flow*actualStream(port_b.Xi_outflow);
  port_a.Xi_outflow = medium.Xi;
  port_b.Xi_outflow = medium.Xi;
  mbC_flow = port_a.m_flow*actualStream(port_a.C_outflow)+port_b.m_flow*actualStream(port_b.C_outflow);
  port_a.C_outflow = C;
  port_b.C_outflow = C;
  connect(heatTransfer.heatPorts[1],_famefault_heatPort.port_b) annotation(Line(points={{40,-34},{40,-60}},color={127,0,0},smooth=Smooth.None));
  connect(heatPort,_famefault_heatPort.port_a);

 annotation(defaultComponentName="pump",Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics={Text(visible=use_N_in,extent={{14,98},{178,82}},textString="N_in [rpm]")}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics),Documentation(info="<HTML>
<p>This model describes a centrifugal pump (or a group of <code>nParallel</code> pumps) with prescribed speed, either fixed or provided by an external signal.
<p>The model extends <code>PartialPump</code>
<p>If the <code>N_in</code> input connector is wired, it provides rotational speed of the pumps (rpm); otherwise, a constant rotational speed equal to <code>n_const</code> (which can be different from <code>N_nominal</code>) is assumed.</p>
</HTML>",revisions="<html>
<ul>
<li><i>31 Oct 2005</i>
    by <a href=\"mailto:francesco.casella@polimi.it\">Francesco Casella</a>:<br>
       Model added to the Fluid library</li>
</ul>
</html>"));
 end PrescribedPump;

 package BaseClasses
  "Base classes used in the Machines package (only of interest to build new component models)"
  extends Modelica.Icons.BasesPackage;

  partial model PartialPump
   "Base model for centrifugal pumps"
   extends Modelica.Fluid.Interfaces.PartialTwoPort(port_b_exposesState=(energyDynamics<>Types.Dynamics.SteadyState) or (massDynamics<>Types.Dynamics.SteadyState),port_a(p(start=p_a_start),m_flow(start=m_flow_start,min=(if allowFlowReversal and not checkValve then -Constants.inf else 0))),port_b(p(start=p_b_start),m_flow(start=-m_flow_start,max=(if allowFlowReversal and not checkValve then Constants.inf else 0))));
   extends Modelica.Fluid.Interfaces.PartialLumpedVolume(final fluidVolume=V,energyDynamics=Types.Dynamics.SteadyState,massDynamics=Types.Dynamics.SteadyState,final p_start=p_b_start);

   import Modelica.SIunits.Conversions.NonSIunits.*;
   import Modelica.Constants;

  // locally defined classes in PartialPump
   replaceable    function flowCharacteristic = PumpCharacteristics.baseFlow "Head vs. V_flow characteristic at nominal speed and density" annotation(Dialog(group="Characteristics"),choicesAllMatching=true);
   replaceable    function powerCharacteristic = PumpCharacteristics.quadraticPower(V_flow_nominal={0,0,0},W_nominal={0,0,0}) "Power consumption vs. V_flow at nominal speed and density" annotation(Dialog(group="Characteristics",enable=use_powerCharacteristic),choicesAllMatching=true);
   replaceable    function efficiencyCharacteristic = PumpCharacteristics.constantEfficiency(eta_nominal=0.8) constrainedby PumpCharacteristics.baseEfficiency;
   replaceable    model HeatTransfer = Modelica.Fluid.Vessels.BaseClasses.HeatTransfer.IdealHeatTransfer constrainedby Modelica.Fluid.Vessels.BaseClasses.HeatTransfer.PartialVesselHeatTransfer;

  // components of PartialPump
   parameter Medium.AbsolutePressure p_a_start=system.p_start "Guess value for inlet pressure" annotation(Dialog(tab="Initialization"));
   parameter Medium.AbsolutePressure p_b_start=p_a_start "Guess value for outlet pressure" annotation(Dialog(tab="Initialization"));
   parameter Medium.MassFlowRate m_flow_start=1 "Guess value of m_flow = port_a.m_flow" annotation(Dialog(tab="Initialization"));
   parameter Integer nParallel(min=1)=1 "Number of pumps in parallel" annotation(Dialog(group="Characteristics"));
   parameter AngularVelocity_rpm N_nominal "Nominal rotational speed for flow characteristic" annotation(Dialog(group="Characteristics"));
   parameter Medium.Density rho_nominal=Medium.density_pTX(Medium.p_default,Medium.T_default,Medium.X_default) "Nominal fluid density for characteristic" annotation(Dialog(group="Characteristics"));
   parameter Boolean use_powerCharacteristic=false "Use powerCharacteristic (vs. efficiencyCharacteristic)" annotation(Evaluate=true,Dialog(group="Characteristics"));
   parameter Boolean checkValve=false "= true to prevent reverse flow" annotation(Dialog(tab="Assumptions"),Evaluate=true);
   parameter SI.Volume V=0 "Volume inside the pump" annotation(Dialog(tab="Assumptions"),Evaluate=true);
   parameter Boolean use_HeatTransfer=false "= true to use a HeatTransfer model, e.g., for a housing" annotation(Dialog(tab="Assumptions",group="Heat transfer"));
   HeatTransfer heatTransfer(redeclare final package Medium = Medium,final n=1,surfaceAreas={4*Modelica.Constants.pi*(3/(4)*V/(Modelica.Constants.pi))^(2/(3))},final states={medium.state},final use_k=use_HeatTransfer) annotation(Placement(transformation(extent={{-10,-10},{30,30}},rotation=180,origin={50,-10})));
   Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_a heatPort if use_HeatTransfer annotation(Placement(transformation(extent={{30,-70},{50,-50}})));
   final parameter SI.Acceleration g=system.g;
   Medium.Density rho=medium.d;
   SI.Pressure dp_pump=port_b.p-port_a.p "Pressure increase";
   SI.Height head=dp_pump/(rho*g) "Pump head";
   SI.MassFlowRate m_flow=port_a.m_flow "Mass flow rate (total)";
   SI.MassFlowRate m_flow_single=m_flow/(nParallel) "Mass flow rate (single pump)";
   SI.VolumeFlowRate V_flow=m_flow/(rho) "Volume flow rate (total)";
   SI.VolumeFlowRate V_flow_single(start=m_flow_start/(rho_nominal)/(nParallel))=V_flow/(nParallel) "Volume flow rate (single pump)";
   AngularVelocity_rpm N(start=N_nominal) "Shaft rotational speed";
   SI.Power W_single "Power Consumption (single pump)";
   SI.Power W_total=W_single*nParallel "Power Consumption (total)";
   Real eta "Global Efficiency";
   Real s(start=m_flow_start) "Curvilinear abscissa for the flow curve in parametric form (either mass flow rate or head)";
   parameter Boolean show_NPSHa=false "= true to compute Net Positive Suction Head available" annotation(Dialog(tab="Advanced",group="Diagnostics"));
   Medium.ThermodynamicState state_a=Medium.setState_phX(port_a.p,inStream(port_a.h_outflow),inStream(port_a.Xi_outflow)) if show_NPSHa "state for medium inflowing through port_a";
   Medium.Density rho_in=Medium.density(state_a) if show_NPSHa "Liquid density at the inlet port_a";
   SI.Length NPSHa=NPSPa/(rho_in*system.g) if show_NPSHa "Net Positive Suction Head available";
   SI.Pressure NPSPa=assertPositiveDifference(port_a.p,Medium.saturationPressure(Medium.temperature(state_a)),"Cavitation occurs at the pump inlet") if show_NPSHa "Net Positive Suction Pressure available";
   SI.Pressure NPDPa=assertPositiveDifference(port_b.p,Medium.saturationPressure(medium.T),"Cavitation occurs in the pump") if show_NPSHa "Net Positive Discharge Pressure available";
  protected
   constant SI.Height unitHead=1;
   constant SI.MassFlowRate unitMassFlowRate=1;

  // algorithms and equations of PartialPump
  equation
   if not checkValve then
    head = (N/(N_nominal))^(2)*flowCharacteristic(V_flow_single*N_nominal/(N));
    s = 0;
   elseif s>0 then
    head = (N/(N_nominal))^(2)*flowCharacteristic(V_flow_single*N_nominal/(N));
    V_flow_single = s*unitMassFlowRate/(rho);
   else
    head = (N/(N_nominal))^(2)*flowCharacteristic(0)-s*unitHead;
    V_flow_single = 0;
   end if;
   if use_powerCharacteristic then
    W_single = (N/(N_nominal))^(3)*rho/(rho_nominal)*powerCharacteristic(V_flow_single*N_nominal/(N));
    eta = dp_pump*V_flow_single/(W_single);
   else
    eta = efficiencyCharacteristic(V_flow_single*N_nominal/(N));
    W_single = dp_pump*V_flow_single/(eta);
   end if;
   Wb_flow = W_total;
   Qb_flow = heatTransfer.Q_flows[1];
   Hb_flow = port_a.m_flow*actualStream(port_a.h_outflow)+port_b.m_flow*actualStream(port_b.h_outflow);
   port_a.h_outflow = medium.h;
   port_b.h_outflow = medium.h;
   port_b.p = medium.p "outlet pressure is equal to medium pressure, which includes Wb_flow";
   mb_flow = port_a.m_flow+port_b.m_flow;
   mbXi_flow = port_a.m_flow*actualStream(port_a.Xi_outflow)+port_b.m_flow*actualStream(port_b.Xi_outflow);
   port_a.Xi_outflow = medium.Xi;
   port_b.Xi_outflow = medium.Xi;
   mbC_flow = port_a.m_flow*actualStream(port_a.C_outflow)+port_b.m_flow*actualStream(port_b.C_outflow);
   port_a.C_outflow = C;
   port_b.C_outflow = C;
   connect(heatTransfer.heatPorts[1],heatPort) annotation(Line(points={{40,-34},{40,-60}},color={127,0,0},smooth=Smooth.None));

  annotation(Icon(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics={Rectangle(extent={{-100,46},{100,-46}},lineColor={0,0,0},fillColor={0,127,255},fillPattern=FillPattern.HorizontalCylinder),Polygon(points={{-48,-60},{-72,-100},{72,-100},{48,-60},{-48,-60}},lineColor={0,0,255},pattern=LinePattern.None,fillColor={0,0,0},fillPattern=FillPattern.VerticalCylinder),Ellipse(extent={{-80,80},{80,-80}},lineColor={0,0,0},fillPattern=FillPattern.Sphere,fillColor={0,100,199}),Polygon(points={{-28,30},{-28,-30},{50,-2},{-28,30}},lineColor={0,0,0},pattern=LinePattern.None,fillPattern=FillPattern.HorizontalCylinder,fillColor={255,255,255})}),Diagram(coordinateSystem(preserveAspectRatio=true,extent={{-100,-100},{100,100}}),graphics),Documentation(info="<HTML>
<p>This is the base model for pumps.
<p>The model describes a centrifugal pump, or a group of <code>nParallel</code> identical pumps. The pump model is based on the theory of kinematic similarity: the pump characteristics are given for nominal operating conditions (rotational speed and fluid density), and then adapted to actual operating condition, according to the similarity equations.

<p><b>Pump characteristics</b></p>
<p> The nominal hydraulic characteristic (head vs. volume flow rate) is given by the the replaceable function <code>flowCharacteristic</code>.
<p> The pump energy balance can be specified in two alternative ways:
<ul>
<li><code>use_powerCharacteristic = false</code> (default option): the replaceable function <code>efficiencyCharacteristic</code> (efficiency vs. volume flow rate in nominal conditions) is used to determine the efficiency, and then the power consumption.
    The default is a constant efficiency of 0.8.</li>
<li><code>use_powerCharacteristic = true</code>: the replaceable function <code>powerCharacteristic</code> (power consumption vs. volume flow rate in nominal conditions) is used to determine the power consumption, and then the efficiency.
    Use <code>powerCharacteristic</code> to specify a non-zero power consumption for zero flow rate.
</ul>
<p>
Several functions are provided in the package <code>PumpCharacteristics</code> to specify the characteristics as a function of some operating points at nominal conditions.
<p>Depending on the value of the <code>checkValve</code> parameter, the model either supports reverse flow conditions, or includes a built-in check valve to avoid flow reversal.
</p>
<p>It is possible to take into account the mass and energy storage of the fluid inside the pump by specifying its volume <code>V</code>, and by selecting appropriate dynamic mass and energy balance assumptions (see below);
this is recommended to avoid singularities in the computation of the outlet enthalpy in case of zero flow rate.
If zero flow rate conditions are always avoided, this dynamic effect can be neglected by leaving the default value <code>V = 0</code>, thus avoiding fast state variables in the model.
</p>

<p><b>Dynamics options</b></p>
<p>
Steady-state mass and energy balances are assumed per default, neglecting the holdup of fluid in the pump; this configuration works well if the flow rate is always positive.
Dynamic mass and energy balance can be used by setting the corresponding dynamic parameters. This is recommended to avoid singularities at zero or reversing mass flow rate. If the initial conditions imply non-zero mass flow rate, it is possible to use the <code>SteadyStateInitial</code> condition, otherwise it is recommended to use <code>FixedInitial</code> in order to avoid undetermined initial conditions.
</p>

<p><b>Heat transfer</b></p>
<p>
The boolean paramter <code>use_HeatTransfer</code> can be set to true if heat exchanged with the environment
should be taken into account or to model a housing. This might be desirable if a pump with realistic
<code>powerCharacteristic</code> for zero flow operates while a valve prevents fluid flow.
</p>

<p><b>Diagnostics of Cavitation</b></p>
<p>The boolean parameter show_NPSHa can set true to compute the Net Positive Suction Head available and check for cavitation,
provided a two-phase medium model is used.
</p>
</HTML>",revisions="<html>
<ul>
<li><i>Dec 2008</i>
    by R&uuml;diger Franke:<br>
    <ul>
    <li>Replaced simplified mass and energy balances with rigorous formulation (base class PartialLumpedVolume)</li>
    <li>Introduced optional HeatTransfer model defining Qb_flow</li>
    <li>Enabled events when the checkValve is operating to support the opening of a discrete valve before port_a</li>
    </ul></li>
<li><i>31 Oct 2005</i>
    by <a href=\"mailto:francesco.casella@polimi.it\">Francesco Casella</a>:<br>
       Model added to the Fluid library</li>
</ul>
</html>"));
  end PartialPump;

  package PumpCharacteristics
   "Functions for pump characteristics"

   import NonSI = Modelica.SIunits.Conversions.NonSIunits;

   partial function baseFlow
    "Base class for pump flow characteristics"
    extends Modelica.Icons.Function;

   // components of baseFlow
    input SI.VolumeFlowRate V_flow "Volumetric flow rate";
    output SI.Height head "Pump head";
   end baseFlow;

   partial function basePower
    "Base class for pump power consumption characteristics"
    extends Modelica.Icons.Function;

   // components of basePower
    input SI.VolumeFlowRate V_flow "Volumetric flow rate";
    output SI.Power consumption "Power consumption";
   end basePower;

   partial function baseEfficiency
    "Base class for efficiency characteristics"
    extends Modelica.Icons.Function;

   // components of baseEfficiency
    input SI.VolumeFlowRate V_flow "Volumetric flow rate";
    output Real eta "Efficiency";
   end baseEfficiency;

   function linearFlow
    "Linear flow characteristic"
    extends baseFlow;

   // components of linearFlow
    input SI.VolumeFlowRate V_flow_nominal[2] "Volume flow rate for two operating points (single pump)" annotation(Dialog);
    input SI.Height head_nominal[2] "Pump head for two operating points" annotation(Dialog);
   protected
    Real c[2]=Modelica.Math.Matrices.solve([ones(2), V_flow_nominal],head_nominal) "Coefficients of linear head curve";

   // algorithms and equations of linearFlow
   algorithm
      head:=c[1]+V_flow*c[2];
   end linearFlow;

   function quadraticFlow
    "Quadratic flow characteristic"
    extends baseFlow;

   // components of quadraticFlow
    input SI.VolumeFlowRate V_flow_nominal[3] "Volume flow rate for three operating points (single pump)" annotation(Dialog);
    input SI.Height head_nominal[3] "Pump head for three operating points" annotation(Dialog);
   protected
    Real V_flow_nominal2[3]={V_flow_nominal[1]^(2),V_flow_nominal[2]^(2),V_flow_nominal[3]^(2)} "Squared nominal flow rates";
    Real c[3]=Modelica.Math.Matrices.solve([ones(3), V_flow_nominal, V_flow_nominal2],head_nominal) "Coefficients of quadratic head curve";

   // algorithms and equations of quadraticFlow
   algorithm
      head:=c[1]+V_flow*c[2]+V_flow^(2)*c[3];
   end quadraticFlow;

   function polynomialFlow
    "Polynomial flow characteristic"
    extends baseFlow;

   // components of polynomialFlow
    input SI.VolumeFlowRate V_flow_nominal[:] "Volume flow rate for N operating points (single pump)" annotation(Dialog);
    input SI.Height head_nominal[:] "Pump head for N operating points" annotation(Dialog);
   protected
    Integer N=size(V_flow_nominal,1) "Number of nominal operating points";
    Real V_flow_nominal_pow[N,N]={{V_flow_nominal[i]^(j-1) for j in 1:N} for i in 1:N} "Rows: different operating points; columns: increasing powers";
    Real c[N]=Modelica.Math.Matrices.solve(V_flow_nominal_pow,head_nominal) "Coefficients of polynomial head curve";

   // algorithms and equations of polynomialFlow
   algorithm
      head:=sum(V_flow^(i-1)*c[i] for i in 1:N);
   end polynomialFlow;

   function constantEfficiency
    "Constant efficiency characteristic"
    extends baseEfficiency;

   // components of constantEfficiency
    input Real eta_nominal "Nominal efficiency" annotation(Dialog);

   // algorithms and equations of constantEfficiency
   algorithm
      eta:=eta_nominal;
   end constantEfficiency;

   function linearPower
    "Linear power consumption characteristic"
    extends basePower;

   // components of linearPower
    input SI.VolumeFlowRate V_flow_nominal[2] "Volume flow rate for two operating points (single pump)" annotation(Dialog);
    input SI.Power W_nominal[2] "Power consumption for two operating points" annotation(Dialog);
   protected
    Real c[2]=Modelica.Math.Matrices.solve([ones(3), V_flow_nominal],W_nominal) "Coefficients of linear power consumption curve";

   // algorithms and equations of linearPower
   algorithm
      consumption:=c[1]+V_flow*c[2];
   end linearPower;

   function quadraticPower
    "Quadratic power consumption characteristic"
    extends basePower;

   // components of quadraticPower
    input SI.VolumeFlowRate V_flow_nominal[3] "Volume flow rate for three operating points (single pump)" annotation(Dialog);
    input SI.Power W_nominal[3] "Power consumption for three operating points" annotation(Dialog);
   protected
    Real V_flow_nominal2[3]={V_flow_nominal[1]^(2),V_flow_nominal[2]^(2),V_flow_nominal[3]^(2)} "Squared nominal flow rates";
    Real c[3]=Modelica.Math.Matrices.solve([ones(3), V_flow_nominal, V_flow_nominal2],W_nominal) "Coefficients of quadratic power consumption curve";

   // algorithms and equations of quadraticPower
   algorithm
      consumption:=c[1]+V_flow*c[2]+V_flow^(2)*c[3];
   end quadraticPower;
  end PumpCharacteristics;

  function assertPositiveDifference
   extends Modelica.Icons.Function;

  // components of assertPositiveDifference
   input SI.Pressure p;
   input SI.Pressure p_sat;
   input String message;
   output SI.Pressure dp;

  // algorithms and equations of assertPositiveDifference
  algorithm
     dp:=p-p_sat;
     assert(p>=p_sat,message);
  end assertPositiveDifference;
 end BaseClasses;

annotation(Documentation(info="<html>

</html>"));
end Machines;
