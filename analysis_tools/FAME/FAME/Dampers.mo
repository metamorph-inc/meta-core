within FAME;

package Dampers "Damper elements to be used for various 'power loss' faults"

  model Constants
    constant Real verySmall = 1.0e-7;
    constant Real veryLarge = 1.0e7;
  end Constants;

  partial model Electrical
    input Real amount(min=0.0, max=1.0);
    encapsulated type Modes = enumeration(
        Nominal,
        OpenConnection,
        ShortToGround,
        BadConnection);
    parameter Modes mode = Modes.Nominal;
  end Electrical;

  partial model ElectricalAmountAsParameter
    parameter Real amount(min=0.0, max=1.0, start=0.0, unit="1");
    encapsulated type Modes = enumeration(
        Nominal,
        OpenConnection,
        ShortToGround,
        BadConnection);
    parameter Modes mode = Modes.Nominal;
  end ElectricalAmountAsParameter;

  model ElectricalWithConnectEquations "This is essentially a conductor with a resistance that varies
    with the damping factor, being 0 at a damping factor of 0, and
    infinite at a damping factor of 1."
    extends Electrical;
    Modelica.Electrical.Analog.Interfaces.Pin port_a;
    Modelica.Electrical.Analog.Interfaces.Pin port_b;
  protected
    Modelica.SIunits.Resistance R;
  equation
    if mode == Modes.Nominal then
      0 = port_a.i + port_b.i;
      port_a.v - port_b.v = port_b.i * R;
      R=0;
    elseif mode == Modes.OpenConnection then
      0 = port_a.i + port_b.i;
      port_a.v - port_b.v = port_b.i * R;
      R = Constants.veryLarge;
    elseif mode == Modes.ShortToGround then
      port_a.v - port_b.v=0;
      R*(port_a.i + port_b.i) = port_b.v;
      R = 1/(1/max(Constants.verySmall,1-amount)-1);
    else // BadConnection
      0 = port_a.i + port_b.i;
      port_a.v - port_b.v = port_b.i * R;
      R=(1/max(Constants.verySmall,1-amount)-1);
    end if;
  end ElectricalWithConnectEquations;

  model ElectricalWithoutConnectEquations "This is essentially a conductor with a resistance that varies
    with the damping factor, being 0 at a damping factor of 0, and
    infinite at a damping factor of 1."
    extends Electrical;
    Modelica.Electrical.Analog.Interfaces.Pin port_a;
    record iconnector
       input Modelica.SIunits.Voltage v;
       input Modelica.SIunits.Current i;
    end iconnector;
    iconnector port_b;
  protected
    Modelica.SIunits.Resistance R;
  equation
    if mode == Modes.Nominal then
      0 = port_a.i - port_b.i;
      port_a.v - port_b.v = -port_b.i * R;
      R=0;
    elseif mode == Modes.OpenConnection then
      0 = port_a.i - port_b.i;
      port_a.v - port_b.v = -port_b.i * R;
      R = Constants.veryLarge;
    elseif mode == Modes.ShortToGround then
      port_a.v - port_b.v=0;
      R*(port_a.i - port_b.i) = port_b.v;
      R = 1/(1/max(Constants.verySmall,1-amount)-1);
    else // BadConnection
      0 = port_a.i - port_b.i;
      port_a.v - port_b.v = -port_b.i * R;
      R=(1/max(Constants.verySmall,1-amount)-1);
    end if;
  end ElectricalWithoutConnectEquations;

  model ElectricalWithConnectEquationsAasP
    "This is essentially a conductor with a resistance that varies
    with the damping factor, being 0 at a damping factor of 0, and
    infinite at a damping factor of 1."
    extends ElectricalAmountAsParameter;
    Modelica.Electrical.Analog.Interfaces.Pin port_a;
    Modelica.Electrical.Analog.Interfaces.Pin port_b;
  protected
    Modelica.SIunits.Resistance R;
  equation
    if mode == Modes.Nominal then
      0 = port_a.i + port_b.i;
      port_a.v - port_b.v = port_b.i * R;
      R=0;
    elseif mode == Modes.OpenConnection then
      0 = port_a.i + port_b.i;
      port_a.v - port_b.v = port_b.i * R;
      R = Constants.veryLarge;
    elseif mode == Modes.ShortToGround then
      port_a.v - port_b.v=0;
      R*(port_a.i + port_b.i) = port_b.v;
      R = 1/(1/max(Constants.verySmall,1-amount)-1);
    else // BadConnection
      0 = port_a.i + port_b.i;
      port_a.v - port_b.v = port_b.i * R;
      R=(1/max(Constants.verySmall,1-amount)-1);
    end if;
  end ElectricalWithConnectEquationsAasP;

  model ElectricalWithoutConnectEquationsAasP
    "This is essentially a conductor with a resistance that varies
    with the damping factor, being 0 at a damping factor of 0, and
    infinite at a damping factor of 1."
    extends ElectricalAmountAsParameter;
    Modelica.Electrical.Analog.Interfaces.Pin port_a;
    record iconnector
       input Modelica.SIunits.Voltage v;
       input Modelica.SIunits.Current i;
    end iconnector;
    iconnector port_b;
  protected
    Modelica.SIunits.Resistance R;
  equation
    if mode == Modes.Nominal then
      0 = port_a.i - port_b.i;
      port_a.v - port_b.v = -port_b.i * R;
      R=0;
    elseif mode == Modes.OpenConnection then
      0 = port_a.i - port_b.i;
      port_a.v - port_b.v = -port_b.i * R;
      R = Constants.veryLarge;
    elseif mode == Modes.ShortToGround then
      port_a.v - port_b.v=0;
      R*(port_a.i - port_b.i) = port_b.v;
      R = 1/(1/max(Constants.verySmall,1-amount)-1);
    else // BadConnection
      0 = port_a.i - port_b.i;
      port_a.v - port_b.v = -port_b.i * R;
      R=(1/max(Constants.verySmall,1-amount)-1);
    end if;
  end ElectricalWithoutConnectEquationsAasP;

  partial model Rotational
    input Real amount(min=0.0, max=1.0);
    encapsulated type Modes = enumeration(
        Nominal,
        Stick,
        Broken);
    parameter Modes mode = Modes.Nominal;
  end Rotational;

  partial model RotationalAmountAsParameter
    parameter Real amount(min=0.0, max=1.0, start=0.0, unit="1");
    encapsulated type Modes = enumeration(
        Nominal,
        Stick,
        Broken);
    parameter Modes mode = Modes.Nominal;
  end RotationalAmountAsParameter;

  model RotationalWithConnectEquations
    extends Rotational;
    Modelica.Mechanics.Rotational.Interfaces.Flange_a port_a;
    Modelica.Mechanics.Rotational.Interfaces.Flange_a port_b;
  equation
    if mode == Modes.Nominal then
       port_b.tau + port_a.tau=0;
       port_b.phi = port_a.phi;
    elseif mode == Modes.Stick then
      port_b.tau + port_a.tau = (1/max(Constants.verySmall,1-amount)-1)*der(port_a.phi);
      port_b.phi = port_a.phi;
    else // Broken
      port_b.tau + port_a.tau = 0;
      port_a.phi - port_b.phi = (1/max(Constants.verySmall,0)-1)* port_a.tau;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              visible=useHeatPort,
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end RotationalWithConnectEquations;

  model RotationalWithoutConnectEquations
    extends Rotational;
    Modelica.Mechanics.Rotational.Interfaces.Flange_a port_a;
    record iconnector
      input Modelica.SIunits.Angle phi;
      input Modelica.SIunits.Torque tau;
    end iconnector;
    iconnector port_b;
  equation
    if mode == Modes.Nominal then
      port_a.tau-port_b.tau=0;
      port_a.phi=port_b.phi;
    elseif mode == Modes.Stick then
      -port_b.tau + port_a.tau = (1/max(Constants.verySmall,1-amount)-1)*der(port_a.phi);
      port_b.phi = port_a.phi;
    else // Broken
      -port_b.tau + port_a.tau = 0;
      port_a.phi - port_b.phi = (1/max(Constants.verySmall,0)-1)* port_a.tau;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              visible=useHeatPort,
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end RotationalWithoutConnectEquations;

  model RotationalWithConnectEquationsAasP
    extends RotationalAmountAsParameter;
    Modelica.Mechanics.Rotational.Interfaces.Flange_a port_a;
    Modelica.Mechanics.Rotational.Interfaces.Flange_a port_b;
  equation
    if mode == Modes.Nominal then
       port_b.tau + port_a.tau=0;
       port_b.phi = port_a.phi;
    elseif mode == Modes.Stick then
      port_b.tau + port_a.tau = (1/max(Constants.verySmall,1-amount)-1)*der(port_a.phi);
      port_b.phi = port_a.phi;
    else
      port_b.tau + port_a.tau = 0;
      port_a.phi - port_b.phi = (1/max(Constants.verySmall,0)-1)* port_a.tau;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              visible=useHeatPort,
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end RotationalWithConnectEquationsAasP;

  model RotationalWithoutConnectEquationsAasP
    extends RotationalAmountAsParameter;
    Modelica.Mechanics.Rotational.Interfaces.Flange_a port_a;
    record iconnector
      input Modelica.SIunits.Angle phi;
      input Modelica.SIunits.Torque tau;
    end iconnector;
    iconnector port_b;
  equation
    if mode == Modes.Nominal then
      port_a.tau-port_b.tau=0;
      port_a.phi=port_b.phi;
    elseif mode == Modes.Stick then
      -port_b.tau + port_a.tau = (1/max(Constants.verySmall,1-amount)-1)*der(port_a.phi);
      port_b.phi = port_a.phi;
    else
      -port_b.tau + port_a.tau = 0;
      port_a.phi - port_b.phi = (1/max(Constants.verySmall,0)-1)* port_a.tau;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              visible=useHeatPort,
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end RotationalWithoutConnectEquationsAasP;

  partial model Translational
    input Real amount(min=0.0, max=1.0);
    encapsulated type Modes = enumeration(
        Nominal,
        Stick,
        Broken);
    parameter Modes mode = Modes.Nominal;
  end Translational;

  partial model TranslationalAmountAsParameter
    parameter Real amount(min=0.0, max=1.0, start=0.0, unit="1");
    encapsulated type Modes = enumeration(
        Nominal,
        Stick,
        Broken);
    parameter Modes mode = Modes.Nominal;
  end TranslationalAmountAsParameter;

  model TranslationalWithoutConnectEquations
    extends Translational;
    Modelica.Mechanics.Translational.Interfaces.Flange_a port_a;
    record iconnector
      input Modelica.SIunits.Position s;
      input Modelica.SIunits.Force f;
    end iconnector;
    iconnector port_b;
  equation
    if mode == Modes.Nominal then
      port_a.f-port_b.f=0;
      port_a.s=port_b.s;
    elseif mode == Modes.Stick then
      -port_b.f + port_a.f = (1/max(Constants.verySmall,1-amount)-1)*der(port_a.s);
      port_b.s = port_a.s;
    else
      -port_b.f + port_a.f = 0;
      port_a.s - port_b.s = (1/max(Constants.verySmall,0)-1)* port_a.f;
    end if;
  end TranslationalWithoutConnectEquations;

  model TranslationalWithConnectEquations
    extends Translational;
    Modelica.Mechanics.Translational.Interfaces.Flange_a port_a;
    Modelica.Mechanics.Translational.Interfaces.Flange_b port_b;
  equation
    if mode == Modes.Nominal then
     port_b.f + port_a.f=0;
     port_b.s = port_a.s;
    elseif mode == Modes.Stick then
      port_b.f + port_a.f = (1/max(Constants.verySmall,1-amount)-1)*der(port_a.s);
      port_b.s = port_a.s;
    else
      port_b.f + port_a.f = 0;
      port_a.s - port_b.s = (1/max(Constants.verySmall,0)-1)* port_a.f;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              visible=useHeatPort,
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end TranslationalWithConnectEquations;

  model TranslationalWithoutConnectEquationsAasP
    extends TranslationalAmountAsParameter;
    Modelica.Mechanics.Translational.Interfaces.Flange_a port_a;
    record iconnector
      input Modelica.SIunits.Position s;
      input Modelica.SIunits.Force f;
    end iconnector;
    iconnector port_b;
  equation
    if mode == Modes.Nominal then
      port_a.f-port_b.f=0;
      port_a.s=port_b.s;
    elseif mode == Modes.Stick then
      -port_b.f + port_a.f = (1/max(Constants.verySmall,1-amount)-1)*der(port_a.s);
      port_b.s = port_a.s;
    else
      -port_b.f + port_a.f = 0;
      port_a.s - port_b.s = (1/max(Constants.verySmall,0)-1)* port_a.f;
    end if;
  end TranslationalWithoutConnectEquationsAasP;

  model TranslationalWithConnectEquationsAasP
    extends TranslationalAmountAsParameter;
    Modelica.Mechanics.Translational.Interfaces.Flange_a port_a;
    Modelica.Mechanics.Translational.Interfaces.Flange_b port_b;
  equation
    if mode == Modes.Nominal then
     port_b.f + port_a.f=0;
     port_b.s = port_a.s;
    elseif mode == Modes.Stick then
      port_b.f + port_a.f = (1/max(Constants.verySmall,1-amount)-1)*der(port_a.s);
      port_b.s = port_a.s;
    else
      port_b.f + port_a.f = 0;
      port_a.s - port_b.s = (1/max(Constants.verySmall,0)-1)* port_a.f;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              visible=useHeatPort,
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end TranslationalWithConnectEquationsAasP;

  partial model Fluid
    input Real amount(min=0.0, max=1.0);
    encapsulated type Modes = enumeration(
        Nominal,
        Leak);
    parameter Modes mode = Modes.Nominal;
    parameter Real eps = 1.0e-7;
  end Fluid;

  partial model FluidAmountAsParameter
    parameter Real amount(min=0.0, max=1.0, start=0.0, unit="1");
    encapsulated type Modes = enumeration(
        Nominal,
        Leak);
    parameter Modes mode = Modes.Nominal;
    parameter Real eps = 1.0e-7;
  end FluidAmountAsParameter;

  model FluidWithConnectEquations
    extends Fluid;
    Modelica.Fluid.Interfaces.FluidPort_a port_a;
    Modelica.Fluid.Interfaces.FluidPort_a port_b;
  equation
    if mode == Modes.Nominal then
      amount = 0.0;
    end if;

    port_a.p = port_b.p;

    //linear loss of mass flow
    port_a.m_flow + port_b.m_flow = (1.0/max(eps, 1.0 - amount)-1.0)*port_a.p;

    port_a.Xi_outflow = inStream(port_b.Xi_outflow);
    port_b.Xi_outflow = inStream(port_a.Xi_outflow);

    port_a.C_outflow = inStream(port_b.C_outflow);
    port_b.C_outflow = inStream(port_a.C_outflow);

    port_a.h_outflow = inStream(port_b.h_outflow);
    port_b.h_outflow = inStream(port_a.h_outflow);

     annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                     extent={{-100,-100},{100,100}},
                                     grid={1,1}),
                    graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{60,40},{80,40}}, color={0,0,0}),
                              Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
                              Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
                              Rectangle(extent={{-50,-10},{40,-70}},
                                        lineColor={0,0,0},
                                        fillColor={192,192,192},
                                        fillPattern=FillPattern.Solid),
                              Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
                              Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
                              Line(points={{40,-40},{80,-40}}, color={0,0,0}),
                              Line(points={{80,40},{80,-40}}, color={0,0,0}),
                              Line(points={{-90,0},{-80,0}}, color={0,0,0}),
                              Line(points={{80,0},{90,0}}, color={0,0,0}),
                              Text(extent={{-150,-144},{150,-104}},
                                   lineColor={0,0,0},
                                   textString="amount=%amount"),
                              Text(extent={{-190,110},{190,70}},
                                   lineColor={0,0,255},
                                   textString="%name"),
                              Line(visible=useHeatPort,
                                   points={{-100,-100},{-100,-55},{-5,-55}},
                                   color={191,0,0},
                                   pattern=LinePattern.Dot,
                                   smooth=Smooth.None)}));
   end FluidWithConnectEquations;

  model FluidWithConnectEquationsAasP
    extends FluidAmountAsParameter;
    Modelica.Fluid.Interfaces.FluidPort_a port_a;
    Modelica.Fluid.Interfaces.FluidPort_a port_b;
  equation
    if mode == Modes.Nominal then
      amount = 0.0;
    end if;

    port_a.p = port_b.p;

    //linear loss of mass flow
    port_a.m_flow + port_b.m_flow = (1.0/max(eps, 1.0 - amount)-1.0)*port_a.p;

    port_a.Xi_outflow = inStream(port_b.Xi_outflow);
    port_b.Xi_outflow = inStream(port_a.Xi_outflow);

    port_a.C_outflow = inStream(port_b.C_outflow);
    port_b.C_outflow = inStream(port_a.C_outflow);

    port_a.h_outflow = inStream(port_b.h_outflow);
    port_b.h_outflow = inStream(port_a.h_outflow);

     annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                     extent={{-100,-100},{100,100}},
                                     grid={1,1}),
                    graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{60,40},{80,40}}, color={0,0,0}),
                              Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
                              Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
                              Rectangle(extent={{-50,-10},{40,-70}},
                                        lineColor={0,0,0},
                                        fillColor={192,192,192},
                                        fillPattern=FillPattern.Solid),
                              Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
                              Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
                              Line(points={{40,-40},{80,-40}}, color={0,0,0}),
                              Line(points={{80,40},{80,-40}}, color={0,0,0}),
                              Line(points={{-90,0},{-80,0}}, color={0,0,0}),
                              Line(points={{80,0},{90,0}}, color={0,0,0}),
                              Text(extent={{-150,-144},{150,-104}},
                                   lineColor={0,0,0},
                                   textString="amount=%amount"),
                              Text(extent={{-190,110},{190,70}},
                                   lineColor={0,0,255},
                                   textString="%name"),
                              Line(visible=useHeatPort,
                                   points={{-100,-100},{-100,-55},{-5,-55}},
                                   color={191,0,0},
                                   pattern=LinePattern.Dot,
                                   smooth=Smooth.None)}));
   end FluidWithConnectEquationsAasP;

  partial model FluidHeat
    input Real amount(min=0.0, max=1.0);
    encapsulated type Modes = enumeration(
        Nominal,
        Leak,
        Obstruction);
    parameter Modes mode = Modes.Nominal;
    parameter Modelica.Thermal.FluidHeatFlow.Media.Medium medium
      "Medium in the connector";
  end FluidHeat;

  partial model FluidHeatAmountAsParameter
    parameter Real amount(min=0.0, max=1.0, start=0.0, unit="1");
    encapsulated type Modes = enumeration(
        Nominal,
        Leak,
        Obstruction);
    parameter Modes mode = Modes.Nominal;
    parameter Modelica.Thermal.FluidHeatFlow.Media.Medium medium
      "Medium in the connector";
  end FluidHeatAmountAsParameter;

  model FluidHeatWithoutConnectEquations
    extends FluidHeat;
    Modelica.Thermal.FluidHeatFlow.Interfaces.FlowPort_a port_a(medium=medium);
    record iconnector
      parameter Modelica.Thermal.FluidHeatFlow.Media.Medium medium
        "Medium in the connector";
      input Modelica.SIunits.Pressure p;
      input Modelica.SIunits.MassFlowRate m_flow;
      input Modelica.SIunits.SpecificEnthalpy h;
      input Modelica.SIunits.EnthalpyFlowRate H_flow;
    end iconnector;
    iconnector port_b(medium=medium);
  equation
    if mode == Modes.Nominal then
      port_b.p = port_a.p;
      -port_b.m_flow + port_a.m_flow =0;
      -port_b.H_flow + port_a.H_flow = 0;
      port_b.h - port_a.h = 0;
    elseif mode == Modes.Leak then
      port_b.p = port_a.p;
      -port_b.m_flow + port_a.m_flow = (1/max(Constants.verySmall,1-amount)-1)* port_a.p;
      -port_b.H_flow + port_a.H_flow = 0;
      port_b.h - port_a.h = 0;
    else
      port_a.p - port_b.p = (1/max(Constants.verySmall,1-amount)-1)*port_a.m_flow*abs(port_a.m_flow);
      -port_b.m_flow + port_a.m_flow=0;
      //no heat loss assumption
      -port_b.H_flow + port_a.H_flow = 0;
      port_b.h - port_a.h = 0;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              visible=useHeatPort,
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end FluidHeatWithoutConnectEquations;

  model FluidHeatWithConnectEquations
    extends FluidHeat;
    Modelica.Thermal.FluidHeatFlow.Interfaces.FlowPort_a port_a(medium=medium);
    Modelica.Thermal.FluidHeatFlow.Interfaces.FlowPort_b port_b(medium=medium);
  equation
    if mode == Modes.Nominal then
      port_b.p = port_a.p;
      port_b.m_flow + port_a.m_flow=0;
      port_b.H_flow + port_a.H_flow = 0;
      port_b.h - port_a.h = 0;
    elseif mode == Modes.Leak then
      port_b.p = port_a.p;
      port_b.m_flow + port_a.m_flow = (1/max(Constants.verySmall,1-amount)-1)* port_a.p;
      port_b.H_flow + port_a.H_flow = 0;
      port_b.h - port_a.h = 0;
    else
      port_a.p - port_b.p = (1/max(Constants.verySmall,1-amount)-1)*port_a.m_flow*abs(port_a.m_flow);
      port_b.m_flow + port_a.m_flow=0;
      //no heat loss assumption
      port_b.H_flow + port_a.H_flow = 0;
      port_b.h - port_a.h = 0;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              visible=useHeatPort,
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end FluidHeatWithConnectEquations;

  model FluidHeatWithoutConnectEquationsAasP
    extends FluidHeatAmountAsParameter;
    Modelica.Thermal.FluidHeatFlow.Interfaces.FlowPort_a port_a(medium=medium);
    record iconnector
      parameter Modelica.Thermal.FluidHeatFlow.Media.Medium medium
        "Medium in the connector";
      input Modelica.SIunits.Pressure p;
      input Modelica.SIunits.MassFlowRate m_flow;
      input Modelica.SIunits.SpecificEnthalpy h;
      input Modelica.SIunits.EnthalpyFlowRate H_flow;
    end iconnector;
    iconnector port_b(medium=medium);
  equation
    if mode == Modes.Nominal then
      port_b.p = port_a.p;
      -port_b.m_flow + port_a.m_flow =0;
      -port_b.H_flow + port_a.H_flow = 0;
      port_b.h - port_a.h = 0;
    elseif mode == Modes.Leak then
      port_b.p = port_a.p;
      -port_b.m_flow + port_a.m_flow = (1/max(Constants.verySmall,1-amount)-1)* port_a.p;
      -port_b.H_flow + port_a.H_flow = 0;
      port_b.h - port_a.h = 0;
    else
      port_a.p - port_b.p = (1/max(Constants.verySmall,1-amount)-1)*port_a.m_flow*abs(port_a.m_flow);
      -port_b.m_flow + port_a.m_flow=0;
      //no heat loss assumption
      -port_b.H_flow + port_a.H_flow = 0;
      port_b.h - port_a.h = 0;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              visible=useHeatPort,
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end FluidHeatWithoutConnectEquationsAasP;

  model FluidHeatWithConnectEquationsAasP
    extends FluidHeatAmountAsParameter;
    Modelica.Thermal.FluidHeatFlow.Interfaces.FlowPort_a port_a(medium=medium);
    Modelica.Thermal.FluidHeatFlow.Interfaces.FlowPort_b port_b(medium=medium);
  equation
    if mode == Modes.Nominal then
      port_b.p = port_a.p;
      port_b.m_flow + port_a.m_flow=0;
      port_b.H_flow + port_a.H_flow = 0;
      port_b.h - port_a.h = 0;
    elseif mode == Modes.Leak then
      port_b.p = port_a.p;
      port_b.m_flow + port_a.m_flow = (1/max(Constants.verySmall,1-amount)-1)* port_a.p;
      port_b.H_flow + port_a.H_flow = 0;
      port_b.h - port_a.h = 0;
    else
      port_a.p - port_b.p = (1/max(Constants.verySmall,1-amount)-1)*port_a.m_flow*abs(port_a.m_flow);
      port_b.m_flow + port_a.m_flow=0;
      //no heat loss assumption
      port_b.H_flow + port_a.H_flow = 0;
      port_b.h - port_a.h = 0;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              visible=useHeatPort,
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end FluidHeatWithConnectEquationsAasP;

  partial model Hydraulic
    input Real amount(min=0.0, max=1.0);
    connector Hydraulic_Port
      flow Modelica.SIunits.VolumeFlowRate V;
      Modelica.SIunits.AbsolutePressure P;
    end Hydraulic_Port;
    encapsulated type Modes = enumeration(
        Nominal,
        Leak,
        Rupture);
    parameter Modes mode = Modes.Nominal;
  end Hydraulic;

  partial model HydraulicAmountAsParameter
    parameter Real amount(min=0.0, max=1.0, start=0.0, unit="1");
    connector Hydraulic_Port
      flow Modelica.SIunits.VolumeFlowRate V;
      Modelica.SIunits.AbsolutePressure P;
    end Hydraulic_Port;
    encapsulated type Modes = enumeration(
        Nominal,
        Leak,
        Rupture);
    parameter Modes mode = Modes.Nominal;
  end HydraulicAmountAsParameter;

  model HydraulicWithoutConnectEquations
    extends Hydraulic;
    Hydraulic_Port port_a;
    record iconnector
      input Modelica.SIunits.VolumeFlowRate V;
      input Modelica.SIunits.AbsolutePressure P;
    end iconnector;
    iconnector port_b;
  equation
    if mode == Modes.Nominal then
      port_a.V - port_b.V=0;
      port_a.P = port_b.P;
    elseif mode == Modes.Leak then
      port_a.V - port_b.V = (1/max(Constants.verySmall,1-amount)-1)*port_a.P;
      port_a.P = port_b.P;
    else
       port_a.V - port_b.V = 0;
       port_a.P - port_b.P = Constants.veryLarge*port_a.V;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              visible=useHeatPort,
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end HydraulicWithoutConnectEquations;

  model HydraulicWithConnectEquations
    extends Hydraulic;
    Hydraulic_Port port_a;
    Hydraulic_Port port_b;
  equation
    if mode == Modes.Nominal then
      connect(port_a,port_b);
    elseif mode == Modes.Leak then
      port_a.V + port_b.V = (1/max(Constants.verySmall,1-amount)-1)*port_a.P;
      port_a.P = port_b.P;
    else
       port_a.V + port_b.V = 0;
       port_a.P - port_b.P = Constants.veryLarge*port_a.V;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end HydraulicWithConnectEquations;

  model HydraulicWithoutConnectEquationsAasP
    extends HydraulicAmountAsParameter;
    Hydraulic_Port port_a;
    record iconnector
      input Modelica.SIunits.VolumeFlowRate V;
      input Modelica.SIunits.AbsolutePressure P;
    end iconnector;
    iconnector port_b;
  equation
    if mode == Modes.Nominal then
      port_a.V - port_b.V=0;
      port_a.P = port_b.P;
    elseif mode == Modes.Leak then
      port_a.V - port_b.V = (1/max(Constants.verySmall,1-amount)-1)*port_a.P;
      port_a.P = port_b.P;
    else
       port_a.V - port_b.V = 0;
       port_a.P - port_b.P = Constants.veryLarge*port_a.V;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              visible=useHeatPort,
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end HydraulicWithoutConnectEquationsAasP;

  model HydraulicWithConnectEquationsAasP
    extends HydraulicAmountAsParameter;
    Hydraulic_Port port_a;
    Hydraulic_Port port_b;
  equation
    if mode == Modes.Nominal then
      connect(port_a,port_b);
    elseif mode == Modes.Leak then
      port_a.V + port_b.V = (1/max(Constants.verySmall,1-amount)-1)*port_a.P;
      port_a.P = port_b.P;
    else
       port_a.V + port_b.V = 0;
       port_a.P - port_b.P = Constants.veryLarge*port_a.V;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end HydraulicWithConnectEquationsAasP;

  partial model Thermal
    input Real amount(min=0.0, max=1.0);
    encapsulated type Modes = enumeration(
        Nominal,
        Leak,
        Rupture);
    parameter Modes mode = Modes.Nominal;
  end Thermal;

  partial model ThermalAmountAsParameter
    parameter Real amount(min=0.0, max=1.0, start=0.0, unit="1");
    encapsulated type Modes = enumeration(
        Nominal,
        Leak,
        Rupture);
    parameter Modes mode = Modes.Nominal;
  end ThermalAmountAsParameter;

  model ThermalWithoutConnectEquations
    extends Thermal;
    Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_a port_a;
    record iconnector
      input Modelica.SIunits.Temperature T;
      input Modelica.SIunits.HeatFlowRate Q_flow;
    end iconnector;
    iconnector port_b;
  equation
    if mode == Modes.Nominal then
      port_a.Q_flow - port_b.Q_flow =0;
      port_a.T = port_b.T;
    elseif mode == Modes.Leak then
      port_a.Q_flow - port_b.Q_flow = (1/max(Constants.verySmall,1-amount)-1)*port_a.T;
      port_a.T = port_b.T;
    else
      port_a.Q_flow - port_b.Q_flow = 0;
      port_a.T - port_b.T = Constants.veryLarge*port_a.Q_flow;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              visible=useHeatPort,
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end ThermalWithoutConnectEquations;

  model ThermalWithConnectEquations
    extends Thermal;
    Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_a port_a;
    Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_b port_b;
  equation
    if mode == Modes.Nominal then
      port_a.Q_flow + port_b.Q_flow =0;
      port_a.T = port_b.T;
    elseif mode == Modes.Leak then
      port_a.Q_flow + port_b.Q_flow = (1/max(Constants.verySmall,1-amount)-1)*port_a.T;
      port_a.T = port_b.T;
    else
      port_a.Q_flow + port_b.Q_flow = 0;
      port_a.T - port_b.T = Constants.veryLarge*port_a.Q_flow;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              visible=useHeatPort,
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end ThermalWithConnectEquations;

  model ThermalWithoutConnectEquationsAasP
    extends ThermalAmountAsParameter;
    Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_a port_a;
    record iconnector
      input Modelica.SIunits.Temperature T;
      input Modelica.SIunits.HeatFlowRate Q_flow;
    end iconnector;
    iconnector port_b;
  equation
    if mode == Modes.Nominal then
      port_a.Q_flow - port_b.Q_flow =0;
      port_a.T = port_b.T;
    elseif mode == Modes.Leak then
      port_a.Q_flow - port_b.Q_flow = (1/max(Constants.verySmall,1-amount)-1)*port_a.T;
      port_a.T = port_b.T;
    else
      port_a.Q_flow - port_b.Q_flow = 0;
      port_a.T - port_b.T = Constants.veryLarge*port_a.Q_flow;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              visible=useHeatPort,
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end ThermalWithoutConnectEquationsAasP;

  model ThermalWithConnectEquationsAasP
    extends ThermalAmountAsParameter;
    Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_a port_a;
    Modelica.Thermal.HeatTransfer.Interfaces.HeatPort_b port_b;
  equation
    if mode == Modes.Nominal then
      port_a.Q_flow + port_b.Q_flow =0;
      port_a.T = port_b.T;
    elseif mode == Modes.Leak then
      port_a.Q_flow + port_b.Q_flow = (1/max(Constants.verySmall,1-amount)-1)*port_a.T;
      port_a.T = port_b.T;
    else
      port_a.Q_flow + port_b.Q_flow = 0;
      port_a.T - port_b.T = Constants.veryLarge*port_a.Q_flow;
    end if;
    annotation (Icon(coordinateSystem(preserveAspectRatio=true,
                                      extent={{-100,-100},{100,100}},
                                      grid={1,1}),
                     graphics={Line(points={{-80,40},{-60,40},{-45,10},{-15,70},{15,10},{45,70},{
                  60,40},{80,40}}, color={0,0,0}),
            Line(points={{-80,40},{-80,-40}}, color={0,0,0}),
            Line(points={{-80,-40},{-50,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-50,-10},{40,-70}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-50,-10},{70,-10}}, color={0,0,0}),
            Line(points={{-50,-70},{70,-70}}, color={0,0,0}),
            Line(points={{40,-40},{80,-40}}, color={0,0,0}),
            Line(points={{80,40},{80,-40}}, color={0,0,0}),
            Line(points={{-90,0},{-80,0}}, color={0,0,0}),
            Line(points={{80,0},{90,0}}, color={0,0,0}),
            Text(
              extent={{-150,-144},{150,-104}},
              lineColor={0,0,0},
              textString="amount=%amount"),
            Text(
              extent={{-190,110},{190,70}},
              lineColor={0,0,255},
              textString="%name"),
            Line(
              visible=useHeatPort,
              points={{-100,-100},{-100,-55},{-5,-55}},
              color={191,0,0},
              pattern=LinePattern.Dot,
              smooth=Smooth.None)}));
  end ThermalWithConnectEquationsAasP;

end Dampers;
