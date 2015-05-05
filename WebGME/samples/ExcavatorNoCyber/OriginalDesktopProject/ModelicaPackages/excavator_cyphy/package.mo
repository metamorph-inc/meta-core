within ;
package excavator_cyphy 
  package excavator

    model v009
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

      OpenHydraulics.Examples.Excavator.SubSystems.DigCycleSeq digCycleSeq
        annotation (Placement(transformation(extent={{372,-174},{414,-134}},
              rotation=0)));

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves
     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit bucketValve(
        sizeOfInputs=4,
        inputIndex=4,
        q_nom=0.003)
        annotation (Placement(transformation(extent={{298,-126},{328,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit armValve(
        sizeOfInputs=4,
        inputIndex=3,
        q_nom=0.01)
        annotation (Placement(transformation(extent={{256,-126},{286,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        sizeOfInputs=4,
        inputIndex=2,
        q_nom=0.01,
        q_fraction_A2T=0.4)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        sizeOfInputs=4,
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={320,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={306,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={264,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={280,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{326,-146},{346,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints
      MB.Joints.Assemblies.JointRRR bucket_link(
      rRod2_ib={-0.67,0.07,0},
      rRod1_ia={0.62,0.36,0},
      phi_guess=0,
      phi_offset=-45)   annotation (Placement(transformation(extent={{-2,281},{
                27,312}},   rotation=0)));

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init)
        annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
                238}})));
      cyPhy_Excavator.components.bucketCyl     bucketCyl1(bucket_s_init=
            bucket_s_init) annotation (Placement(transformation(rotation=0, extent={{142,302},
                {172,330}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
      cyPhy_Excavator.components.arm     arm1 annotation (Placement(transformation(
              rotation=0, extent={{70,260},{98,288}})));
      cyPhy_Excavator.components.bucket     bucket1 annotation (Placement(
            transformation(rotation=0, extent={{64,298},{104,332}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    cyPhy_Excavator.excavator.linearDistance linearDistance1 annotation (
        Placement(transformation(rotation=0, extent={{-40,204},{-20,224}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(boomValve.portLS1, armValve.portLS2)
        annotation (Line(
          points={{142,-108},{256,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(armValve.portLS1, bucketValve.portLS2)
        annotation (Line(
          points={{286,-108},{298,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(boomValve.portP1, armValve.portP2) annotation (Line(points={{142,
              -117},{256,-117}},
                              color={255,0,0}));
      connect(boomValve.portT1, armValve.portT2) annotation (Line(points={{142,
              -123},{256,-123}},
                              color={255,0,0}));
      connect(armValve.portP1, bucketValve.portP2) annotation (Line(points={{286,
              -117},{298,-117}}, color={255,0,0}));
      connect(armValve.portT1, bucketValve.portT2) annotation (Line(points={{286,
              -123},{298,-123}}, color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(bucketValve.portA, bucketLineA.port_a)
                                               annotation (Line(points={{305.2,
              -96},{306,-96},{306,-74}},
                                      color={255,0,0}));
      connect(bucketValve.portB, bucketLineB.port_a)
                                               annotation (Line(points={{320.8,
              -96},{320,-96},{320,-74}},
                                      color={255,0,0}));
      connect(bucketLineA.port_b,bucketCyl1.port_a)
                                              annotation (Line(points={{306,-54},
            {306,-54},{306,132},{306,306},{170,306},{170,305.027},{171.379,
            305.027}},          color={255,0,0}));
      connect(bucketLineB.port_b,bucketCyl1.port_b)
        annotation (Line(points={{320,-54},{320,-54},{320,326},{171.172,326},{
            171.172,326.216}},                     color={255,0,0}));
      connect(armValve.portA, armLineA.port_a)
                                            annotation (Line(points={{263.2,-96},
              {264,-96},{264,-74}},   color={255,0,0}));
      connect(armValve.portB, armLineB.port_a)
                                            annotation (Line(points={{278.8,-96},
              {280,-96},{280,-74}},   color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(bucketValve.y, armValve.u) annotation (Line(points={{297.1,-100.5},
              {286.9,-100.5}},
                             color={0,0,127}));
      connect(armValve.y, boomValve.u) annotation (Line(points={{255.1,-100.5},
              {142.9,-100.5}},
                            color={0,0,127}));
      connect(boomValve.y, swingValve.u) annotation (Line(points={{111.1,-100.5},{100.9,
              -100.5}},              color={0,0,127}));
      connect(bucketValve.portLS1, openTank.port) annotation (Line(
          points={{328,-108},{336,-108},{336,-126}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

      connect(digCycleSeq.y1, bucketValve.u) annotation (Line(
          points={{416.1,-154},{424,-154},{424,-100.5},{328.9,-100.5}},
          color={0,0,127},
          smooth=Smooth.None));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket_link.frame_a, arm1.frame_b1)
                                               annotation (Line(
          points={{-2,296.5},{-2,294},{-28,294},{-28,292},{-28,272.28},{70.28,
            272.28},{70.28,273.72}},
          color={0,0,0},
          thickness=0.5));
    connect(bucketCyl1.frame_b, bucket_link.frame_im)
                                                 annotation (Line(
          points={{156.069,328.789},{156,328.789},{156,350},{12,350},{12,312},{
            12.5,312}},
          color={0,0,0},
          thickness=0.5));
      connect(bucketCyl1.frame_a,arm1.frame_b)
                                           annotation (Line(
          points={{157.931,303.059},{156,303.059},{156,285.2},{97.44,285.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket1.frame_b, bucket_link.frame_b)
                                              annotation (Line(
          points={{64.4,318.4},{64.4,318},{52,318},{52,296.5},{27,296.5}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_b,arm1.frame_a)             annotation (Line(
          points={{87.2,187.28},{87.2,187.28},{87.2,261.12},{83.72,261.12}},
          color={95,95,95},
          thickness=0.5));
      connect(arm1.frame_b2,bucket1.frame_a)           annotation (Line(
          points={{84.28,286.6},{86,286.6},{86,299.7},{85.6,299.7}},
          color={95,95,95},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(armCyl1.port_b, armLineB.port_b) annotation (Line(
          points={{161.96,233.84},{280,233.84},{280,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(armCyl1.port_a, armLineA.port_b) annotation (Line(
          points={{161.96,213.84},{264,213.84},{264,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(arm1.frame_b3, armCyl1.frame_b) annotation (Line(
          points={{97.72,261.4},{147,261.4},{147,237.36}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boom1.frame_b3, armCyl1.frame_a) annotation (Line(
          points={{101.2,186.56},{149.38,186.56},{149.38,210.64}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(linearDistance1.frame_a, undercarriage.frame_a) annotation (Line(
        points={{-20,212},{-36,212},{-36,0},{86,0},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(linearDistance1.frame_b, bucket1.bucket_tip) annotation (Line(
        points={{-20,218},{-18,218},{-18,334},{90,334},{90,331.32}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(StopTime=20, Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009;

    model v009_redux
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

      OpenHydraulics.Examples.Excavator.SubSystems.DigCycleSeq digCycleSeq
        annotation (Placement(transformation(extent={{180,-80},{222,-40}},
              rotation=0)));

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        sizeOfInputs=4,
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
      lineBulkMod=100000000)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{152,-120},{172,-100}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
      lineBulkMod=100000000)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
    cyPhy_Excavator.components.undercarriage
                                    base1
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,40},{96,60}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

      connect(carriage1.frame_a,base1.frame_b)                  annotation (Line(
          points={{88,80},{88,72},{88,60}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, base1.frame_a) annotation (Line(
        points={{54,26},{86,26},{86,40}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(openTank.port, swingValve.portLS1) annotation (Line(
        points={{162,-100},{132,-100},{132,-108},{100,-108}},
        color={255,0,0},
        smooth=Smooth.None));
    connect(digCycleSeq.y1, swingValve.u) annotation (Line(
        points={{224.1,-60},{226,-60},{226,-92},{110,-92},{110,-100.5},{100.9,
            -100.5}},
        color={0,0,127},
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(StopTime=20, Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_redux;

    model v009_redux2
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

      OpenHydraulics.Examples.Excavator.SubSystems.DigCycleSeq digCycleSeq
        annotation (Placement(transformation(extent={{180,-80},{222,-40}},
              rotation=0)));

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        sizeOfInputs=4,
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
      lineBulkMod=100000000)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{152,-120},{172,-100}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
      lineBulkMod=100000000)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
    Modelica.Mechanics.Rotational.Components.Inertia inertia(J=1)
      annotation (Placement(transformation(extent={{-74,-50},{-54,-30}})));
    Modelica.Mechanics.Rotational.Components.Fixed fixed
      annotation (Placement(transformation(extent={{-104,-64},{-84,-44}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    // Top level connections

    connect(openTank.port, swingValve.portLS1) annotation (Line(
        points={{162,-100},{132,-100},{132,-108},{100,-108}},
        color={255,0,0},
        smooth=Smooth.None));
    connect(digCycleSeq.y1, swingValve.u) annotation (Line(
        points={{224.1,-60},{226,-60},{226,-92},{110,-92},{110,-100.5},{100.9,
            -100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(inertia.flange_b, swingMotor.flange_b) annotation (Line(
        points={{-54,-40},{-37.8,-40}},
        color={0,0,0},
        smooth=Smooth.None));
    connect(inertia.flange_a, fixed.flange) annotation (Line(
        points={{-74,-40},{-94,-40},{-94,-54}},
        color={0,0,0},
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(StopTime=20, Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_redux2;

    model v009_redux3
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

      // the valves

      // the LoadSensing pump and control circuitry

      // the lines and junctions

      // the connectors

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
    cyPhy_Excavator.components.undercarriage
                                    base1
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,40},{96,60}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    Modelica.Mechanics.Rotational.Sources.Torque torque
      annotation (Placement(transformation(extent={{2,74},{22,94}})));
    Modelica.Blocks.Sources.Ramp ramp(
      height=150,
      duration=10,
      startTime=2)
      annotation (Placement(transformation(extent={{-78,36},{-58,56}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(carriage1.frame_a,base1.frame_b)                  annotation (Line(
          points={{88,80},{88,72},{88,60}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    // Top level connections

    connect(fixed.frame_b, base1.frame_a) annotation (Line(
        points={{54,26},{86,26},{86,40}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(torque.flange, carriage1.axis) annotation (Line(
        points={{22,84},{50,84},{50,80},{78,80}},
        color={0,0,0},
        smooth=Smooth.None));
    connect(ramp.y, torque.tau) annotation (Line(
        points={{-57,46},{-30,46},{-30,84},{0,84}},
        color={0,0,127},
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(StopTime=20, Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_redux3;

    model v009_redux4
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

      OpenHydraulics.Examples.Excavator.SubSystems.DigCycleSeq digCycleSeq
        annotation (Placement(transformation(extent={{180,-80},{222,-40}},
              rotation=0)));

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        sizeOfInputs=4,
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          useSupport=false, w_fixed=15000)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
      lineBulkMod=100000000)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{152,-120},{172,-100}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
      lineBulkMod=100000000)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
    cyPhy_Excavator.components.undercarriage
                                    base1
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,40},{96,60}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

      connect(carriage1.frame_a,base1.frame_b)                  annotation (Line(
          points={{88,80},{88,72},{88,60}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, base1.frame_a) annotation (Line(
        points={{54,26},{86,26},{86,40}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(openTank.port, swingValve.portLS1) annotation (Line(
        points={{162,-100},{132,-100},{132,-108},{100,-108}},
        color={255,0,0},
        smooth=Smooth.None));
    connect(digCycleSeq.y1, swingValve.u) annotation (Line(
        points={{224.1,-60},{226,-60},{226,-92},{110,-92},{110,-100.5},{100.9,
            -100.5}},
        color={0,0,127},
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(StopTime=20, Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_redux4;

    model v009_redux5
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

      OpenHydraulics.Examples.Excavator.SubSystems.DigCycleSeq digCycleSeq
        annotation (Placement(transformation(extent={{264,-144},{306,-104}},
              rotation=0)));

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        sizeOfInputs=4,
        inputIndex=2,
        q_nom=0.01,
        q_fraction_A2T=0.4)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        sizeOfInputs=4,
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{214,-150},{234,-130}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    OpenHydraulics.Components.Sensors.PressureSensor pressureSensor annotation (
       Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=270,
          origin={214,130})));
    OpenHydraulics.Components.Sensors.PressureSensor pressureSensor1
      annotation (Placement(transformation(
          extent={{-10,10},{10,-10}},
          rotation=270,
          origin={136,62})));
    MB.Sensors.Distance distance annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=90,
          origin={52,128})));
    MB.Sensors.RelativePosition relativePosition annotation (Placement(
          transformation(
          extent={{-10,-10},{10,10}},
          rotation=90,
          origin={32,130})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(boomValve.y, swingValve.u) annotation (Line(points={{111.1,-100.5},{100.9,
              -100.5}},              color={0,0,127}));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(pressureSensor.port_a, boomcylR.port_a)  annotation (Line(
        points={{204,130},{188,130},{188,142.296},{148.94,142.296}},
        color={255,0,0},
        smooth=Smooth.None));
    connect(pressureSensor1.port_a, boomcylR.port_b)  annotation (Line(
        points={{146,62},{162,62},{162,117.704},{150.98,117.704}},
        color={255,0,0},
        smooth=Smooth.None));
    connect(distance.frame_a, carriage1.frame_b) annotation (Line(
        points={{52,118},{52,92},{78.8,92.2}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(distance.frame_b, boomCylL.frame_b)
                                            annotation (Line(
        points={{52,138},{52,164},{0.78,164},{0.78,137.333}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(relativePosition.frame_b, boomCylL.frame_b)
                                                    annotation (Line(
        points={{32,140},{32,164},{0.78,164},{0.78,137.333}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(relativePosition.frame_a, carriage1.frame_b) annotation (Line(
        points={{32,120},{32,92},{30,92},{78.8,92.2}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(boomValve.portLS1, openTank.port) annotation (Line(
        points={{142,-108},{224,-108},{224,-130}},
        color={255,0,0},
        smooth=Smooth.None));
    connect(boomValve.u, digCycleSeq.y1) annotation (Line(
        points={{142.9,-100.5},{340,-100.5},{340,-124},{308.1,-124}},
        color={0,0,127},
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(StopTime=20, Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_redux5;

    model v009_altDigSeq
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves
     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit bucketValve(
        sizeOfInputs=4,
        inputIndex=4,
        q_nom=0.003)
        annotation (Placement(transformation(extent={{298,-126},{328,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit armValve(
        sizeOfInputs=4,
        inputIndex=3,
        q_nom=0.01)
        annotation (Placement(transformation(extent={{256,-126},{286,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        sizeOfInputs=4,
        inputIndex=2,
        q_nom=0.01,
        q_fraction_A2T=0.4)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        sizeOfInputs=4,
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={320,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={306,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={264,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={280,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{326,-146},{346,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints
      MB.Joints.Assemblies.JointRRR bucket_link(
      rRod2_ib={-0.67,0.07,0},
      rRod1_ia={0.62,0.36,0},
      phi_guess=0,
      phi_offset=-45)   annotation (Placement(transformation(extent={{-2,281},{
                27,312}},   rotation=0)));

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init)
        annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
                238}})));
      cyPhy_Excavator.components.bucketCyl     bucketCyl1(bucket_s_init=
            bucket_s_init) annotation (Placement(transformation(rotation=0, extent={{142,302},
                {172,330}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
      cyPhy_Excavator.components.arm     arm1 annotation (Placement(transformation(
              rotation=0, extent={{70,260},{98,288}})));
      cyPhy_Excavator.components.bucket     bucket1 annotation (Placement(
            transformation(rotation=0, extent={{64,298},{104,332}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    cyPhy_Excavator.TestComponents.maxH_extend digCycleSeq
      annotation (Placement(transformation(extent={{412,-138},{432,-118}})));
    cyPhy_Excavator.excavator.linearDistance linearDistance
      annotation (Placement(transformation(extent={{-138,170},{-70,226}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(boomValve.portLS1, armValve.portLS2)
        annotation (Line(
          points={{142,-108},{256,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(armValve.portLS1, bucketValve.portLS2)
        annotation (Line(
          points={{286,-108},{298,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(boomValve.portP1, armValve.portP2) annotation (Line(points={{142,
              -117},{256,-117}},
                              color={255,0,0}));
      connect(boomValve.portT1, armValve.portT2) annotation (Line(points={{142,
              -123},{256,-123}},
                              color={255,0,0}));
      connect(armValve.portP1, bucketValve.portP2) annotation (Line(points={{286,
              -117},{298,-117}}, color={255,0,0}));
      connect(armValve.portT1, bucketValve.portT2) annotation (Line(points={{286,
              -123},{298,-123}}, color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(bucketValve.portA, bucketLineA.port_a)
                                               annotation (Line(points={{305.2,
              -96},{306,-96},{306,-74}},
                                      color={255,0,0}));
      connect(bucketValve.portB, bucketLineB.port_a)
                                               annotation (Line(points={{320.8,
              -96},{320,-96},{320,-74}},
                                      color={255,0,0}));
      connect(bucketLineA.port_b,bucketCyl1.port_a)
                                              annotation (Line(points={{306,-54},
            {306,-54},{306,132},{306,306},{170,306},{170,305.027},{171.379,
            305.027}},          color={255,0,0}));
      connect(bucketLineB.port_b,bucketCyl1.port_b)
        annotation (Line(points={{320,-54},{320,-54},{320,326},{171.172,326},{
            171.172,326.216}},                     color={255,0,0}));
      connect(armValve.portA, armLineA.port_a)
                                            annotation (Line(points={{263.2,-96},
              {264,-96},{264,-74}},   color={255,0,0}));
      connect(armValve.portB, armLineB.port_a)
                                            annotation (Line(points={{278.8,-96},
              {280,-96},{280,-74}},   color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(bucketValve.portLS1, openTank.port) annotation (Line(
          points={{328,-108},{336,-108},{336,-126}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket_link.frame_a, arm1.frame_b1)
                                               annotation (Line(
          points={{-2,296.5},{-2,294},{-28,294},{-28,292},{-28,272.28},{70.28,
            272.28},{70.28,273.72}},
          color={0,0,0},
          thickness=0.5));
    connect(bucketCyl1.frame_b, bucket_link.frame_im)
                                                 annotation (Line(
          points={{156.069,328.789},{156,328.789},{156,350},{12,350},{12,312},{
            12.5,312}},
          color={0,0,0},
          thickness=0.5));
      connect(bucketCyl1.frame_a,arm1.frame_b)
                                           annotation (Line(
          points={{157.931,303.059},{156,303.059},{156,285.2},{97.44,285.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket1.frame_b, bucket_link.frame_b)
                                              annotation (Line(
          points={{64.4,318.4},{64.4,318},{52,318},{52,296.5},{27,296.5}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_b,arm1.frame_a)             annotation (Line(
          points={{87.2,187.28},{87.2,187.28},{87.2,261.12},{83.72,261.12}},
          color={95,95,95},
          thickness=0.5));
      connect(arm1.frame_b2,bucket1.frame_a)           annotation (Line(
          points={{84.28,286.6},{86,286.6},{86,299.7},{85.6,299.7}},
          color={95,95,95},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(armCyl1.port_b, armLineB.port_b) annotation (Line(
          points={{161.96,233.84},{280,233.84},{280,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(armCyl1.port_a, armLineA.port_b) annotation (Line(
          points={{161.96,213.84},{264,213.84},{264,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(arm1.frame_b3, armCyl1.frame_b) annotation (Line(
          points={{97.72,261.4},{147,261.4},{147,237.36}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boom1.frame_b3, armCyl1.frame_a) annotation (Line(
          points={{101.2,186.56},{149.38,186.56},{149.38,210.64}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(digCycleSeq.y1, bucketValve.u) annotation (Line(
        points={{433,-128},{460,-128},{460,-86},{380,-86},{380,-100.5},{328.9,
            -100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(bucketValve.y, armValve.u) annotation (Line(
        points={{297.1,-100.5},{286.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(armValve.y, boomValve.u) annotation (Line(
        points={{255.1,-100.5},{142.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(boomValve.y, swingValve.u) annotation (Line(
        points={{111.1,-100.5},{106.55,-100.5},{106.55,-100.5},{100.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));

    connect(bucketCyl1.frame_a, linearDistance.frame_b) annotation (Line(
        points={{157.931,303.059},{44.9655,303.059},{44.9655,209.2},{-70,209.2}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));

    connect(boom1.frame_a, linearDistance.frame_a) annotation (Line(
        points={{85.6,164.72},{7.8,164.72},{7.8,192.4},{-70,192.4}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(StopTime=20, Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_altDigSeq;

    model v009_altDigSeq_maxHeight
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves
     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit bucketValve(
        sizeOfInputs=4,
        inputIndex=4,
        q_nom=0.003)
        annotation (Placement(transformation(extent={{298,-126},{328,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit armValve(
        sizeOfInputs=4,
        inputIndex=3,
        q_nom=0.01)
        annotation (Placement(transformation(extent={{256,-126},{286,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        sizeOfInputs=4,
        inputIndex=2,
        q_nom=0.01,
        q_fraction_A2T=0.4)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        sizeOfInputs=4,
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={320,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={306,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={264,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={280,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{326,-146},{346,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints
      MB.Joints.Assemblies.JointRRR bucket_link(
      rRod2_ib={-0.67,0.07,0},
      rRod1_ia={0.62,0.36,0},
      phi_guess=0,
      phi_offset=-45)   annotation (Placement(transformation(extent={{-2,281},{
                27,312}},   rotation=0)));

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init)
        annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
                238}})));
      cyPhy_Excavator.components.bucketCyl     bucketCyl1(bucket_s_init=
            bucket_s_init) annotation (Placement(transformation(rotation=0, extent={{142,302},
                {172,330}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
      cyPhy_Excavator.components.arm     arm1 annotation (Placement(transformation(
              rotation=0, extent={{70,260},{98,288}})));
      cyPhy_Excavator.components.bucket     bucket1 annotation (Placement(
            transformation(rotation=0, extent={{64,298},{104,332}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    cyPhy_Excavator.TestComponents.max_height digCycleSeq
      annotation (Placement(transformation(extent={{412,-138},{432,-118}})));
    cyPhy_Excavator.excavator.linearDistance linearDistance
      annotation (Placement(transformation(extent={{-70,212},{-2,268}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(boomValve.portLS1, armValve.portLS2)
        annotation (Line(
          points={{142,-108},{256,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(armValve.portLS1, bucketValve.portLS2)
        annotation (Line(
          points={{286,-108},{298,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(boomValve.portP1, armValve.portP2) annotation (Line(points={{142,
              -117},{256,-117}},
                              color={255,0,0}));
      connect(boomValve.portT1, armValve.portT2) annotation (Line(points={{142,
              -123},{256,-123}},
                              color={255,0,0}));
      connect(armValve.portP1, bucketValve.portP2) annotation (Line(points={{286,
              -117},{298,-117}}, color={255,0,0}));
      connect(armValve.portT1, bucketValve.portT2) annotation (Line(points={{286,
              -123},{298,-123}}, color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(bucketValve.portA, bucketLineA.port_a)
                                               annotation (Line(points={{305.2,
              -96},{306,-96},{306,-74}},
                                      color={255,0,0}));
      connect(bucketValve.portB, bucketLineB.port_a)
                                               annotation (Line(points={{320.8,
              -96},{320,-96},{320,-74}},
                                      color={255,0,0}));
      connect(bucketLineA.port_b,bucketCyl1.port_a)
                                              annotation (Line(points={{306,-54},
            {306,-54},{306,132},{306,306},{170,306},{170,305.027},{171.379,
            305.027}},          color={255,0,0}));
      connect(bucketLineB.port_b,bucketCyl1.port_b)
        annotation (Line(points={{320,-54},{320,-54},{320,326},{171.172,326},{
            171.172,326.216}},                     color={255,0,0}));
      connect(armValve.portA, armLineA.port_a)
                                            annotation (Line(points={{263.2,-96},
              {264,-96},{264,-74}},   color={255,0,0}));
      connect(armValve.portB, armLineB.port_a)
                                            annotation (Line(points={{278.8,-96},
              {280,-96},{280,-74}},   color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(bucketValve.portLS1, openTank.port) annotation (Line(
          points={{328,-108},{336,-108},{336,-126}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket_link.frame_a, arm1.frame_b1)
                                               annotation (Line(
          points={{-2,296.5},{-2,294},{-28,294},{-28,292},{-28,272.28},{70.56,
              272.28},{70.56,271.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucketCyl1.frame_b, bucket_link.frame_im)
                                                 annotation (Line(
          points={{156.069,328.789},{156,328.789},{156,350},{12,350},{12,312},{
            12.5,312}},
          color={0,0,0},
          thickness=0.5));
      connect(bucketCyl1.frame_a,arm1.frame_b)
                                           annotation (Line(
          points={{157.931,303.059},{156,303.059},{156,285.2},{97.44,285.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket1.frame_b, bucket_link.frame_b)
                                              annotation (Line(
          points={{64.4,318.4},{64.4,318},{52,318},{52,296.5},{27,296.5}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_b,arm1.frame_a)             annotation (Line(
          points={{87.2,187.28},{87.2,187.28},{87.2,261.12},{83.72,261.12}},
          color={95,95,95},
          thickness=0.5));
      connect(arm1.frame_b2,bucket1.frame_a)           annotation (Line(
          points={{84.28,286.6},{86,286.6},{86,299.7},{85.6,299.7}},
          color={95,95,95},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(armCyl1.port_b, armLineB.port_b) annotation (Line(
          points={{161.96,233.84},{280,233.84},{280,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(armCyl1.port_a, armLineA.port_b) annotation (Line(
          points={{161.96,213.84},{264,213.84},{264,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(arm1.frame_b3, armCyl1.frame_b) annotation (Line(
          points={{97.72,261.4},{147,261.4},{147,237.36}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boom1.frame_b3, armCyl1.frame_a) annotation (Line(
          points={{101.2,186.56},{149.38,186.56},{149.38,210.64}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(digCycleSeq.y1, bucketValve.u) annotation (Line(
        points={{433,-128},{460,-128},{460,-86},{380,-86},{380,-100.5},{328.9,
            -100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(bucketValve.y, armValve.u) annotation (Line(
        points={{297.1,-100.5},{286.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(armValve.y, boomValve.u) annotation (Line(
        points={{255.1,-100.5},{142.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(boomValve.y, swingValve.u) annotation (Line(
        points={{111.1,-100.5},{106.55,-100.5},{106.55,-100.5},{100.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));

    connect(bucketCyl1.frame_a,linearDistance. frame_b) annotation (Line(
        points={{157.931,303.059},{112.966,303.059},{112.966,251.2},{-2,251.2}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(boom1.frame_a,linearDistance. frame_a) annotation (Line(
        points={{85.6,164.72},{75.8,164.72},{75.8,234.4},{-2,234.4}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(StopTime=20, Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_altDigSeq_maxHeight;

    model v009_altDigSeq_bucket
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves
     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit bucketValve(
        sizeOfInputs=4,
        inputIndex=4,
        q_nom=0.003)
        annotation (Placement(transformation(extent={{298,-126},{328,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit armValve(
        sizeOfInputs=4,
        inputIndex=3,
        q_nom=0.01)
        annotation (Placement(transformation(extent={{256,-126},{286,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        sizeOfInputs=4,
        inputIndex=2,
        q_nom=0.01,
        q_fraction_A2T=0.4)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        sizeOfInputs=4,
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={320,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={306,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={264,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={280,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{326,-146},{346,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints
      MB.Joints.Assemblies.JointRRR bucket_link(
      rRod2_ib={-0.67,0.07,0},
      rRod1_ia={0.62,0.36,0},
      phi_guess=0,
      phi_offset=-45)   annotation (Placement(transformation(extent={{-2,281},{
                27,312}},   rotation=0)));

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init)
        annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
                238}})));
      cyPhy_Excavator.components.bucketCyl     bucketCyl1(bucket_s_init=
            bucket_s_init) annotation (Placement(transformation(rotation=0, extent={{142,302},
                {172,330}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
      cyPhy_Excavator.components.arm     arm1 annotation (Placement(transformation(
              rotation=0, extent={{70,260},{98,288}})));
      cyPhy_Excavator.components.bucket     bucket1 annotation (Placement(
            transformation(rotation=0, extent={{64,298},{104,332}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    cyPhy_Excavator.TestComponents.bucketCycle digCycleSeq
      annotation (Placement(transformation(extent={{412,-138},{432,-118}})));
    cyPhy_Excavator.excavator.linearDistance linearDistance
      annotation (Placement(transformation(extent={{-104,184},{-36,240}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(boomValve.portLS1, armValve.portLS2)
        annotation (Line(
          points={{142,-108},{256,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(armValve.portLS1, bucketValve.portLS2)
        annotation (Line(
          points={{286,-108},{298,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(boomValve.portP1, armValve.portP2) annotation (Line(points={{142,
              -117},{256,-117}},
                              color={255,0,0}));
      connect(boomValve.portT1, armValve.portT2) annotation (Line(points={{142,
              -123},{256,-123}},
                              color={255,0,0}));
      connect(armValve.portP1, bucketValve.portP2) annotation (Line(points={{286,
              -117},{298,-117}}, color={255,0,0}));
      connect(armValve.portT1, bucketValve.portT2) annotation (Line(points={{286,
              -123},{298,-123}}, color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(bucketValve.portA, bucketLineA.port_a)
                                               annotation (Line(points={{305.2,
              -96},{306,-96},{306,-74}},
                                      color={255,0,0}));
      connect(bucketValve.portB, bucketLineB.port_a)
                                               annotation (Line(points={{320.8,
              -96},{320,-96},{320,-74}},
                                      color={255,0,0}));
      connect(bucketLineA.port_b,bucketCyl1.port_a)
                                              annotation (Line(points={{306,-54},
            {306,-54},{306,132},{306,306},{170,306},{170,305.027},{171.379,
            305.027}},          color={255,0,0}));
      connect(bucketLineB.port_b,bucketCyl1.port_b)
        annotation (Line(points={{320,-54},{320,-54},{320,326},{171.172,326},{
            171.172,326.216}},                     color={255,0,0}));
      connect(armValve.portA, armLineA.port_a)
                                            annotation (Line(points={{263.2,-96},
              {264,-96},{264,-74}},   color={255,0,0}));
      connect(armValve.portB, armLineB.port_a)
                                            annotation (Line(points={{278.8,-96},
              {280,-96},{280,-74}},   color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(bucketValve.portLS1, openTank.port) annotation (Line(
          points={{328,-108},{336,-108},{336,-126}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket_link.frame_a, arm1.frame_b1)
                                               annotation (Line(
          points={{-2,296.5},{-2,294},{-28,294},{-28,292},{-28,272.28},{70.28,
            272.28},{70.28,273.72}},
          color={0,0,0},
          thickness=0.5));
    connect(bucketCyl1.frame_b, bucket_link.frame_im)
                                                 annotation (Line(
          points={{156.069,328.789},{156,328.789},{156,350},{12,350},{12,312},{
            12.5,312}},
          color={0,0,0},
          thickness=0.5));
      connect(bucketCyl1.frame_a,arm1.frame_b)
                                           annotation (Line(
          points={{157.931,303.059},{156,303.059},{156,285.2},{97.44,285.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket1.frame_b, bucket_link.frame_b)
                                              annotation (Line(
          points={{64.4,318.4},{64.4,318},{52,318},{52,296.5},{27,296.5}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_b,arm1.frame_a)             annotation (Line(
          points={{87.2,187.28},{87.2,187.28},{87.2,261.12},{83.72,261.12}},
          color={95,95,95},
          thickness=0.5));
      connect(arm1.frame_b2,bucket1.frame_a)           annotation (Line(
          points={{84.28,286.6},{86,286.6},{86,299.7},{85.6,299.7}},
          color={95,95,95},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(armCyl1.port_b, armLineB.port_b) annotation (Line(
          points={{161.96,233.84},{280,233.84},{280,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(armCyl1.port_a, armLineA.port_b) annotation (Line(
          points={{161.96,213.84},{264,213.84},{264,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(arm1.frame_b3, armCyl1.frame_b) annotation (Line(
          points={{97.72,261.4},{147,261.4},{147,237.36}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boom1.frame_b3, armCyl1.frame_a) annotation (Line(
          points={{101.2,186.56},{149.38,186.56},{149.38,210.64}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(digCycleSeq.y1, bucketValve.u) annotation (Line(
        points={{433,-128},{460,-128},{460,-86},{380,-86},{380,-100.5},{328.9,
            -100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(bucketValve.y, armValve.u) annotation (Line(
        points={{297.1,-100.5},{286.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(armValve.y, boomValve.u) annotation (Line(
        points={{255.1,-100.5},{142.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(boomValve.y, swingValve.u) annotation (Line(
        points={{111.1,-100.5},{106.55,-100.5},{106.55,-100.5},{100.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));

    connect(bucketCyl1.frame_a,linearDistance. frame_b) annotation (Line(
        points={{157.931,303.059},{78.966,303.059},{78.966,223.2},{-36,223.2}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(boom1.frame_a,linearDistance. frame_a) annotation (Line(
        points={{85.6,164.72},{41.8,164.72},{41.8,206.4},{-36,206.4}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(StopTime=20, Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_altDigSeq_bucket;

    model v009_altDigSeq_arm
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves
     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit bucketValve(
        sizeOfInputs=4,
        inputIndex=4,
        q_nom=0.003)
        annotation (Placement(transformation(extent={{298,-126},{328,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit armValve(
        sizeOfInputs=4,
        inputIndex=3,
        q_nom=0.01)
        annotation (Placement(transformation(extent={{256,-126},{286,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        sizeOfInputs=4,
        inputIndex=2,
        q_nom=0.01,
        q_fraction_A2T=0.4)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        sizeOfInputs=4,
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={320,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={306,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={264,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={280,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{326,-146},{346,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints
      MB.Joints.Assemblies.JointRRR bucket_link(
      rRod2_ib={-0.67,0.07,0},
      rRod1_ia={0.62,0.36,0},
      phi_guess=0,
      phi_offset=-45)   annotation (Placement(transformation(extent={{-2,281},{
                27,312}},   rotation=0)));

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init)
        annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
                238}})));
      cyPhy_Excavator.components.bucketCyl     bucketCyl1(bucket_s_init=
            bucket_s_init) annotation (Placement(transformation(rotation=0, extent={{142,302},
                {172,330}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
      cyPhy_Excavator.components.arm     arm1 annotation (Placement(transformation(
              rotation=0, extent={{70,260},{98,288}})));
      cyPhy_Excavator.components.bucket     bucket1 annotation (Placement(
            transformation(rotation=0, extent={{64,298},{104,332}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    cyPhy_Excavator.TestComponents.armCycle    digCycleSeq
      annotation (Placement(transformation(extent={{412,-138},{432,-118}})));
    cyPhy_Excavator.excavator.linearDistance linearDistance
      annotation (Placement(transformation(extent={{-104,198},{-36,254}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(boomValve.portLS1, armValve.portLS2)
        annotation (Line(
          points={{142,-108},{256,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(armValve.portLS1, bucketValve.portLS2)
        annotation (Line(
          points={{286,-108},{298,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(boomValve.portP1, armValve.portP2) annotation (Line(points={{142,
              -117},{256,-117}},
                              color={255,0,0}));
      connect(boomValve.portT1, armValve.portT2) annotation (Line(points={{142,
              -123},{256,-123}},
                              color={255,0,0}));
      connect(armValve.portP1, bucketValve.portP2) annotation (Line(points={{286,
              -117},{298,-117}}, color={255,0,0}));
      connect(armValve.portT1, bucketValve.portT2) annotation (Line(points={{286,
              -123},{298,-123}}, color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(bucketValve.portA, bucketLineA.port_a)
                                               annotation (Line(points={{305.2,
              -96},{306,-96},{306,-74}},
                                      color={255,0,0}));
      connect(bucketValve.portB, bucketLineB.port_a)
                                               annotation (Line(points={{320.8,
              -96},{320,-96},{320,-74}},
                                      color={255,0,0}));
      connect(bucketLineA.port_b,bucketCyl1.port_a)
                                              annotation (Line(points={{306,-54},
            {306,-54},{306,132},{306,306},{170,306},{170,305.027},{171.379,
            305.027}},          color={255,0,0}));
      connect(bucketLineB.port_b,bucketCyl1.port_b)
        annotation (Line(points={{320,-54},{320,-54},{320,326},{171.172,326},{
            171.172,326.216}},                     color={255,0,0}));
      connect(armValve.portA, armLineA.port_a)
                                            annotation (Line(points={{263.2,-96},
              {264,-96},{264,-74}},   color={255,0,0}));
      connect(armValve.portB, armLineB.port_a)
                                            annotation (Line(points={{278.8,-96},
              {280,-96},{280,-74}},   color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(bucketValve.portLS1, openTank.port) annotation (Line(
          points={{328,-108},{336,-108},{336,-126}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket_link.frame_a, arm1.frame_b1)
                                               annotation (Line(
          points={{-2,296.5},{-2,294},{-28,294},{-28,292},{-28,272.28},{70.56,
              272.28},{70.56,271.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucketCyl1.frame_b, bucket_link.frame_im)
                                                 annotation (Line(
          points={{156.069,328.789},{156,328.789},{156,350},{12,350},{12,312},{
            12.5,312}},
          color={0,0,0},
          thickness=0.5));
      connect(bucketCyl1.frame_a,arm1.frame_b)
                                           annotation (Line(
          points={{157.931,303.059},{156,303.059},{156,285.2},{97.44,285.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket1.frame_b, bucket_link.frame_b)
                                              annotation (Line(
          points={{64.4,318.4},{64.4,318},{52,318},{52,296.5},{27,296.5}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_b,arm1.frame_a)             annotation (Line(
          points={{87.2,187.28},{87.2,187.28},{87.2,261.12},{83.72,261.12}},
          color={95,95,95},
          thickness=0.5));
      connect(arm1.frame_b2,bucket1.frame_a)           annotation (Line(
          points={{84.28,286.6},{86,286.6},{86,299.7},{85.6,299.7}},
          color={95,95,95},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(armCyl1.port_b, armLineB.port_b) annotation (Line(
          points={{161.96,233.84},{280,233.84},{280,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(armCyl1.port_a, armLineA.port_b) annotation (Line(
          points={{161.96,213.84},{264,213.84},{264,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(arm1.frame_b3, armCyl1.frame_b) annotation (Line(
          points={{97.72,261.4},{147,261.4},{147,237.36}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boom1.frame_b3, armCyl1.frame_a) annotation (Line(
          points={{101.2,186.56},{149.38,186.56},{149.38,210.64}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(digCycleSeq.y1, bucketValve.u) annotation (Line(
        points={{433,-128},{460,-128},{460,-86},{380,-86},{380,-100.5},{328.9,
            -100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(bucketValve.y, armValve.u) annotation (Line(
        points={{297.1,-100.5},{286.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(armValve.y, boomValve.u) annotation (Line(
        points={{255.1,-100.5},{142.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(boomValve.y, swingValve.u) annotation (Line(
        points={{111.1,-100.5},{106.55,-100.5},{106.55,-100.5},{100.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));

    connect(bucketCyl1.frame_a, linearDistance.frame_b) annotation (Line(
        points={{157.931,303.059},{78.9655,303.059},{78.9655,237.2},{-36,237.2}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));

    connect(boom1.frame_a, linearDistance.frame_a) annotation (Line(
        points={{85.6,164.72},{41.8,164.72},{41.8,220.4},{-36,220.4}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(StopTime=20, Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_altDigSeq_arm;

    model v009_altDigSeq_boom
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves
     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit bucketValve(
        sizeOfInputs=4,
        inputIndex=4,
        q_nom=0.003)
        annotation (Placement(transformation(extent={{298,-126},{328,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit armValve(
        sizeOfInputs=4,
        inputIndex=3,
        q_nom=0.01)
        annotation (Placement(transformation(extent={{256,-126},{286,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        sizeOfInputs=4,
        inputIndex=2,
        q_nom=0.01,
        q_fraction_A2T=0.4)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        sizeOfInputs=4,
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={320,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={306,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={264,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={280,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{326,-146},{346,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints
      MB.Joints.Assemblies.JointRRR bucket_link(
      rRod2_ib={-0.67,0.07,0},
      rRod1_ia={0.62,0.36,0},
      phi_guess=0,
      phi_offset=-45)   annotation (Placement(transformation(extent={{-2,281},{
                27,312}},   rotation=0)));

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init)
        annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
                238}})));
      cyPhy_Excavator.components.bucketCyl     bucketCyl1(bucket_s_init=
            bucket_s_init) annotation (Placement(transformation(rotation=0, extent={{142,302},
                {172,330}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
      cyPhy_Excavator.components.arm     arm1 annotation (Placement(transformation(
              rotation=0, extent={{70,260},{98,288}})));
      cyPhy_Excavator.components.bucket     bucket1 annotation (Placement(
            transformation(rotation=0, extent={{64,298},{104,332}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    cyPhy_Excavator.TestComponents.boomCycle   digCycleSeq
      annotation (Placement(transformation(extent={{412,-138},{432,-118}})));
    cyPhy_Excavator.excavator.linearDistance linearDistance
      annotation (Placement(transformation(extent={{-104,198},{-36,254}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(boomValve.portLS1, armValve.portLS2)
        annotation (Line(
          points={{142,-108},{256,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(armValve.portLS1, bucketValve.portLS2)
        annotation (Line(
          points={{286,-108},{298,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(boomValve.portP1, armValve.portP2) annotation (Line(points={{142,
              -117},{256,-117}},
                              color={255,0,0}));
      connect(boomValve.portT1, armValve.portT2) annotation (Line(points={{142,
              -123},{256,-123}},
                              color={255,0,0}));
      connect(armValve.portP1, bucketValve.portP2) annotation (Line(points={{286,
              -117},{298,-117}}, color={255,0,0}));
      connect(armValve.portT1, bucketValve.portT2) annotation (Line(points={{286,
              -123},{298,-123}}, color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(bucketValve.portA, bucketLineA.port_a)
                                               annotation (Line(points={{305.2,
              -96},{306,-96},{306,-74}},
                                      color={255,0,0}));
      connect(bucketValve.portB, bucketLineB.port_a)
                                               annotation (Line(points={{320.8,
              -96},{320,-96},{320,-74}},
                                      color={255,0,0}));
      connect(bucketLineA.port_b,bucketCyl1.port_a)
                                              annotation (Line(points={{306,-54},
            {306,-54},{306,132},{306,306},{170,306},{170,305.027},{171.379,
            305.027}},          color={255,0,0}));
      connect(bucketLineB.port_b,bucketCyl1.port_b)
        annotation (Line(points={{320,-54},{320,-54},{320,326},{171.172,326},{
            171.172,326.216}},                     color={255,0,0}));
      connect(armValve.portA, armLineA.port_a)
                                            annotation (Line(points={{263.2,-96},
              {264,-96},{264,-74}},   color={255,0,0}));
      connect(armValve.portB, armLineB.port_a)
                                            annotation (Line(points={{278.8,-96},
              {280,-96},{280,-74}},   color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(bucketValve.portLS1, openTank.port) annotation (Line(
          points={{328,-108},{336,-108},{336,-126}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket_link.frame_a, arm1.frame_b1)
                                               annotation (Line(
          points={{-2,296.5},{-2,294},{-28,294},{-28,292},{-28,272.28},{70.56,
              272.28},{70.56,271.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucketCyl1.frame_b, bucket_link.frame_im)
                                                 annotation (Line(
          points={{156.069,328.789},{156,328.789},{156,350},{12,350},{12,312},{
            12.5,312}},
          color={0,0,0},
          thickness=0.5));
      connect(bucketCyl1.frame_a,arm1.frame_b)
                                           annotation (Line(
          points={{157.931,303.059},{156,303.059},{156,285.2},{97.44,285.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket1.frame_b, bucket_link.frame_b)
                                              annotation (Line(
          points={{64.4,318.4},{64.4,318},{52,318},{52,296.5},{27,296.5}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_b,arm1.frame_a)             annotation (Line(
          points={{87.2,187.28},{87.2,187.28},{87.2,261.12},{83.72,261.12}},
          color={95,95,95},
          thickness=0.5));
      connect(arm1.frame_b2,bucket1.frame_a)           annotation (Line(
          points={{84.28,286.6},{86,286.6},{86,299.7},{85.6,299.7}},
          color={95,95,95},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(armCyl1.port_b, armLineB.port_b) annotation (Line(
          points={{161.96,233.84},{280,233.84},{280,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(armCyl1.port_a, armLineA.port_b) annotation (Line(
          points={{161.96,213.84},{264,213.84},{264,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(arm1.frame_b3, armCyl1.frame_b) annotation (Line(
          points={{97.72,261.4},{147,261.4},{147,237.36}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boom1.frame_b3, armCyl1.frame_a) annotation (Line(
          points={{101.2,186.56},{149.38,186.56},{149.38,210.64}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(digCycleSeq.y1, bucketValve.u) annotation (Line(
        points={{433,-128},{460,-128},{460,-86},{380,-86},{380,-100.5},{328.9,
            -100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(bucketValve.y, armValve.u) annotation (Line(
        points={{297.1,-100.5},{286.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(armValve.y, boomValve.u) annotation (Line(
        points={{255.1,-100.5},{142.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(boomValve.y, swingValve.u) annotation (Line(
        points={{111.1,-100.5},{100.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));

    connect(bucketCyl1.frame_a, linearDistance.frame_b) annotation (Line(
        points={{157.931,303.059},{78.9655,303.059},{78.9655,237.2},{-36,237.2}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));

    connect(boom1.frame_a, linearDistance.frame_a) annotation (Line(
        points={{85.6,164.72},{41.8,164.72},{41.8,220.4},{-36,220.4}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(StopTime=20, Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_altDigSeq_boom;

    model linearDistance
    Modelica.Mechanics.MultiBody.Sensors.RelativePosition relativePosition
      annotation (Placement(transformation(
          extent={{10,-10},{-10,10}},
          rotation=270,
          origin={208,120})));
    Modelica.Blocks.Math.Product xsqrd annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=180,
          origin={154,142})));
    Modelica.Blocks.Math.Product ysqrd annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=180,
          origin={152,94})));
    Modelica.Blocks.Math.Add add
      annotation (Placement(transformation(extent={{118,106},{98,126}})));
    Modelica.Blocks.Math.Sqrt distance annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=180,
          origin={72,116})));
    Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation (
        Placement(transformation(rotation=0, extent={{486,-4},{554,52}})));
    Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b annotation (
        Placement(transformation(rotation=0, extent={{486,164},{554,220}})));
    Modelica.Blocks.Interfaces.RealOutput x_dist annotation (Placement(
          transformation(
          extent={{-37,-37},{37,37}},
          rotation=90,
          origin={67,361}), iconTransformation(
          extent={{-37,-37},{37,37}},
          rotation=90,
          origin={-14,362})));
    Modelica.Blocks.Interfaces.RealOutput y_dist annotation (Placement(
          transformation(
          extent={{-32,-32},{32,32}},
          rotation=90,
          origin={224,356}), iconTransformation(
          extent={{-32,-32},{32,32}},
          rotation=90,
          origin={370,358})));
    equation
    connect(relativePosition.r_rel[1], xsqrd.u2) annotation (Line(
        points={{196.333,120},{182,120},{182,148},{166,148}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(relativePosition.r_rel[1], xsqrd.u1) annotation (Line(
        points={{196.333,120},{182,120},{182,136},{166,136}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(relativePosition.r_rel[2], ysqrd.u2) annotation (Line(
        points={{197,120},{182,120},{182,100},{164,100}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(relativePosition.r_rel[2], ysqrd.u1) annotation (Line(
        points={{197,120},{182,120},{182,88},{164,88}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(xsqrd.y, add.u1) annotation (Line(
        points={{143,142},{132,142},{132,122},{120,122}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(add.u2, ysqrd.y) annotation (Line(
        points={{120,110},{130,110},{130,94},{141,94}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(add.y, distance.u) annotation (Line(
        points={{97,116},{84,116}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(frame_a, relativePosition.frame_a) annotation (Line(points={{520,24},
            {510,24},{510,110},{208,110}}, color={95,95,95}));
    connect(frame_b, relativePosition.frame_b) annotation (Line(points={{520,
            192},{510,192},{510,130},{208,130}}, color={95,95,95}));
    connect(relativePosition.r_rel[2], y_dist) annotation (Line(
        points={{197,120},{202,120},{202,356},{224,356}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(relativePosition.r_rel[1], x_dist) annotation (Line(
        points={{196.333,120},{192,120},{192,242},{67,242},{67,361}},
        color={0,0,127},
        smooth=Smooth.None));
    annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
            preserveAspectRatio=false), graphics),                         Icon(
          coordinateSystem(extent={{-160,-200},{520,360}}, preserveAspectRatio=
              false), graphics));
    end linearDistance;

    model v009_altDigSeq_swing
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves
     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit bucketValve(
        sizeOfInputs=4,
        inputIndex=4,
        q_nom=0.003)
        annotation (Placement(transformation(extent={{298,-126},{328,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit armValve(
        sizeOfInputs=4,
        inputIndex=3,
        q_nom=0.01)
        annotation (Placement(transformation(extent={{256,-126},{286,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        sizeOfInputs=4,
        inputIndex=2,
        q_nom=0.01,
        q_fraction_A2T=0.4)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        sizeOfInputs=4,
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={320,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={306,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={264,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={280,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{326,-146},{346,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints
      MB.Joints.Assemblies.JointRRR bucket_link(
      rRod2_ib={-0.67,0.07,0},
      rRod1_ia={0.62,0.36,0},
      phi_guess=0,
      phi_offset=-45)   annotation (Placement(transformation(extent={{-2,281},{
                27,312}},   rotation=0)));

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init)
        annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
                238}})));
      cyPhy_Excavator.components.bucketCyl     bucketCyl1(bucket_s_init=
            bucket_s_init) annotation (Placement(transformation(rotation=0, extent={{142,302},
                {172,330}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
      cyPhy_Excavator.components.arm     arm1 annotation (Placement(transformation(
              rotation=0, extent={{70,260},{98,288}})));
      cyPhy_Excavator.components.bucket     bucket1 annotation (Placement(
            transformation(rotation=0, extent={{64,298},{104,332}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    cyPhy_Excavator.TestComponents.swingCycle  digCycleSeq
      annotation (Placement(transformation(extent={{412,-138},{432,-118}})));
    cyPhy_Excavator.excavator.linearDistance linearDistance
      annotation (Placement(transformation(extent={{-104,198},{-36,254}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(boomValve.portLS1, armValve.portLS2)
        annotation (Line(
          points={{142,-108},{256,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(armValve.portLS1, bucketValve.portLS2)
        annotation (Line(
          points={{286,-108},{298,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(boomValve.portP1, armValve.portP2) annotation (Line(points={{142,
              -117},{256,-117}},
                              color={255,0,0}));
      connect(boomValve.portT1, armValve.portT2) annotation (Line(points={{142,
              -123},{256,-123}},
                              color={255,0,0}));
      connect(armValve.portP1, bucketValve.portP2) annotation (Line(points={{286,
              -117},{298,-117}}, color={255,0,0}));
      connect(armValve.portT1, bucketValve.portT2) annotation (Line(points={{286,
              -123},{298,-123}}, color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(bucketValve.portA, bucketLineA.port_a)
                                               annotation (Line(points={{305.2,
              -96},{306,-96},{306,-74}},
                                      color={255,0,0}));
      connect(bucketValve.portB, bucketLineB.port_a)
                                               annotation (Line(points={{320.8,
              -96},{320,-96},{320,-74}},
                                      color={255,0,0}));
      connect(bucketLineA.port_b,bucketCyl1.port_a)
                                              annotation (Line(points={{306,-54},
            {306,-54},{306,132},{306,306},{170,306},{170,305.027},{171.379,
            305.027}},          color={255,0,0}));
      connect(bucketLineB.port_b,bucketCyl1.port_b)
        annotation (Line(points={{320,-54},{320,-54},{320,326},{171.172,326},{
            171.172,326.216}},                     color={255,0,0}));
      connect(armValve.portA, armLineA.port_a)
                                            annotation (Line(points={{263.2,-96},
              {264,-96},{264,-74}},   color={255,0,0}));
      connect(armValve.portB, armLineB.port_a)
                                            annotation (Line(points={{278.8,-96},
              {280,-96},{280,-74}},   color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(bucketValve.portLS1, openTank.port) annotation (Line(
          points={{328,-108},{336,-108},{336,-126}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket_link.frame_a, arm1.frame_b1)
                                               annotation (Line(
          points={{-2,296.5},{-2,294},{-28,294},{-28,292},{-28,272.28},{70.56,
              272.28},{70.56,271.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucketCyl1.frame_b, bucket_link.frame_im)
                                                 annotation (Line(
          points={{156.069,328.789},{156,328.789},{156,350},{12,350},{12,312},{
            12.5,312}},
          color={0,0,0},
          thickness=0.5));
      connect(bucketCyl1.frame_a,arm1.frame_b)
                                           annotation (Line(
          points={{157.931,303.059},{156,303.059},{156,285.2},{97.44,285.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket1.frame_b, bucket_link.frame_b)
                                              annotation (Line(
          points={{64.4,318.4},{64.4,318},{52,318},{52,296.5},{27,296.5}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_b,arm1.frame_a)             annotation (Line(
          points={{87.2,187.28},{87.2,187.28},{87.2,261.12},{83.72,261.12}},
          color={95,95,95},
          thickness=0.5));
      connect(arm1.frame_b2,bucket1.frame_a)           annotation (Line(
          points={{84.28,286.6},{86,286.6},{86,299.7},{85.6,299.7}},
          color={95,95,95},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(armCyl1.port_b, armLineB.port_b) annotation (Line(
          points={{161.96,233.84},{280,233.84},{280,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(armCyl1.port_a, armLineA.port_b) annotation (Line(
          points={{161.96,213.84},{264,213.84},{264,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(arm1.frame_b3, armCyl1.frame_b) annotation (Line(
          points={{97.72,261.4},{147,261.4},{147,237.36}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boom1.frame_b3, armCyl1.frame_a) annotation (Line(
          points={{101.2,186.56},{149.38,186.56},{149.38,210.64}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(digCycleSeq.y1, bucketValve.u) annotation (Line(
        points={{433,-128},{460,-128},{460,-86},{380,-86},{380,-100.5},{328.9,
            -100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(bucketValve.y, armValve.u) annotation (Line(
        points={{297.1,-100.5},{286.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(armValve.y, boomValve.u) annotation (Line(
        points={{255.1,-100.5},{142.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(boomValve.y, swingValve.u) annotation (Line(
        points={{111.1,-100.5},{106.55,-100.5},{106.55,-100.5},{100.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));

    connect(bucketCyl1.frame_a, linearDistance.frame_b) annotation (Line(
        points={{157.931,303.059},{78.9655,303.059},{78.9655,237.2},{-36,237.2}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));

    connect(boom1.frame_a, linearDistance.frame_a) annotation (Line(
        points={{85.6,164.72},{41.8,164.72},{41.8,220.4},{-36,220.4}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(StopTime=20, Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_altDigSeq_swing;

    model v009_boomOnly_angleControl
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        q_nom=0.01,
        q_fraction_A2T=0.4)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{326,-146},{346,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    MB.Sensors.RelativeAngles relativeAngles annotation (Placement(
          transformation(
          extent={{-10,-10},{10,10}},
          rotation=90,
          origin={262,86})));
    Modelica.Blocks.Continuous.LimPID PID(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Ti=0.2,
      Td=0.2,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=4)
      annotation (Placement(transformation(extent={{350,14},{330,-6}})));
    Modelica.Blocks.Sources.Constant const1(k=0)
      annotation (Placement(transformation(extent={{306,-84},{286,-64}})));
    Modelica.Blocks.Sources.TimeTable timeTable(table=[0,-15*Modelica.Constants.pi
          /180; 10,-15*Modelica.Constants.pi/180; 10,51*Modelica.Constants.pi/
          180; 20,51*Modelica.Constants.pi/180; 20,51*Modelica.Constants.pi/180])
      annotation (Placement(transformation(extent={{426,-4},{406,16}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(openTank.port, boomValve.portLS1) annotation (Line(
        points={{336,-126},{336,-108},{142,-108}},
        color={255,0,0},
        smooth=Smooth.None));
    connect(relativeAngles.frame_a, undercarriage.frame_b) annotation (Line(
        points={{262,76},{176,76},{176,78},{88,78},{88,58}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(boom1.frame_b, relativeAngles.frame_b) annotation (Line(
        points={{87.2,187.28},{88,187.28},{88,208},{208,208},{208,96},{262,96}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));

    connect(const1.y, swingValve.u[1]) annotation (Line(
        points={{285,-74},{194,-74},{194,-84},{100.9,-84},{100.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(boomValve.u[1], PID.y) annotation (Line(
        points={{142.9,-100.5},{268,-100.5},{268,4},{329,4}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(relativeAngles.angles[3], PID.u_m) annotation (Line(
        points={{272.333,86},{340,86},{340,16}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID.u_s, timeTable.y) annotation (Line(
        points={{352,4},{378,4},{378,6},{405,6}},
        color={0,0,127},
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(StopTime=20, Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_boomOnly_angleControl;

    model v009_boomOnly_cylLenthControl
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        q_nom=0.01,
        q_fraction_A2T=0.4)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{326,-146},{346,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    Modelica.Blocks.Continuous.LimPID PID(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Td=0.2,
      Ti=0.5,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=4)
      annotation (Placement(transformation(extent={{350,14},{330,-6}})));
    Modelica.Blocks.Sources.Constant const1(k=0)
      annotation (Placement(transformation(extent={{306,-84},{286,-64}})));
    Modelica.Blocks.Sources.TimeTable timeTable(table=[0,2.48; 10,2.48; 10,3.73;
          20,3.73; 20,3.73])
      annotation (Placement(transformation(extent={{426,-6},{406,14}})));
    cyPhy_Excavator.excavator.linearDistance linearDistance
      annotation (Placement(transformation(extent={{-146,142},{-78,198}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(openTank.port, boomValve.portLS1) annotation (Line(
        points={{336,-126},{336,-108},{142,-108}},
        color={255,0,0},
        smooth=Smooth.None));

    connect(const1.y, swingValve.u[1]) annotation (Line(
        points={{285,-74},{194,-74},{194,-84},{100.9,-84},{100.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(boomValve.u[1], PID.y) annotation (Line(
        points={{142.9,-100.5},{268,-100.5},{268,4},{329,4}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID.u_s, timeTable.y) annotation (Line(
        points={{352,4},{405,4}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(boomcylR.cylLength, PID.u_m) annotation (Line(
        points={{115.62,129.148},{115.62,54},{340,54},{340,16}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(linearDistance.frame_b, boom1.frame_b) annotation (Line(
        points={{-78,181.2},{6,181.2},{6,187.28},{87.2,187.28}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(linearDistance.frame_a, carriage1.frame_b2) annotation (Line(
        points={{-78,164.4},{-24,164.4},{-24,150},{86,150},{86,99.2},{87.6,99.2}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));

      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(StopTime=20, Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_boomOnly_cylLenthControl;

    model v009_armControl
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit armValve(
        q_nom=0.01,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{256,-126},{286,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        q_nom=0.01,
        q_fraction_A2T=0.4,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6,
      sizeOfInputs=1)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={264,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={280,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{326,-146},{346,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init)
        annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
                238}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
      cyPhy_Excavator.components.arm     arm1 annotation (Placement(transformation(
              rotation=0, extent={{70,260},{98,288}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    Modelica.Blocks.Continuous.LimPID PID(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Td=0.5,
      k=3.5,
      Ti=0.001,
      controllerType=Modelica.Blocks.Types.SimpleController.P)
      annotation (Placement(transformation(extent={{320,72},{300,52}})));
    Modelica.Blocks.Sources.TimeTable timeTable(table=[0,2.5; 10,2.5; 10,3.7;
          20,3.7; 20,3.7])
      annotation (Placement(transformation(extent={{390,54},{370,74}})));
    Modelica.Blocks.Sources.Constant const1(k=0)
      annotation (Placement(transformation(extent={{340,-56},{320,-36}})));
    Modelica.Blocks.Continuous.LimPID PID1(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Ti=0.5,
      Td=0.5,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=3)
      annotation (Placement(transformation(extent={{394,-76},{374,-96}})));
    Modelica.Blocks.Sources.TimeTable timeTable1(table=[0,3.19; 10,3.19; 10,
          3.19; 20,3.19; 20,3.19])
      annotation (Placement(transformation(extent={{484,-96},{464,-76}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(boomValve.portLS1, armValve.portLS2)
        annotation (Line(
          points={{142,-108},{256,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(boomValve.portP1, armValve.portP2) annotation (Line(points={{142,
              -117},{256,-117}},
                              color={255,0,0}));
      connect(boomValve.portT1, armValve.portT2) annotation (Line(points={{142,
              -123},{256,-123}},
                              color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(armValve.portA, armLineA.port_a)
                                            annotation (Line(points={{263.2,-96},
              {264,-96},{264,-74}},   color={255,0,0}));
      connect(armValve.portB, armLineB.port_a)
                                            annotation (Line(points={{278.8,-96},
              {280,-96},{280,-74}},   color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_b,arm1.frame_a)             annotation (Line(
          points={{87.2,187.28},{87.2,187.28},{87.2,261.12},{83.72,261.12}},
          color={95,95,95},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(armCyl1.port_b, armLineB.port_b) annotation (Line(
          points={{161.96,233.84},{280,233.84},{280,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(armCyl1.port_a, armLineA.port_b) annotation (Line(
          points={{161.96,213.84},{264,213.84},{264,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(arm1.frame_b3, armCyl1.frame_b) annotation (Line(
          points={{97.72,261.4},{147,261.4},{147,237.36}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boom1.frame_b3, armCyl1.frame_a) annotation (Line(
          points={{101.2,186.56},{149.38,186.56},{149.38,210.64}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(openTank.port, armValve.portLS1) annotation (Line(
        points={{336,-126},{336,-108},{286,-108}},
        color={255,0,0},
        smooth=Smooth.None));
    connect(PID.u_s, timeTable.y) annotation (Line(
        points={{322,62},{352,62},{352,64},{369,64}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(swingValve.u[1], const1.y) annotation (Line(
        points={{100.9,-100.5},{100.9,-46},{319,-46}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID.y, boomValve.u[1]) annotation (Line(
        points={{299,62},{218,62},{218,-100.5},{142.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(boomcylR.cylLength, PID.u_m) annotation (Line(
        points={{115.62,129.148},{120,129.148},{120,84},{310,84},{310,74}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID1.u_s, timeTable1.y) annotation (Line(
        points={{396,-86},{463,-86}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(armCyl1.cylLength, PID1.u_m) annotation (Line(
        points={{130,232.56},{342,232.56},{342,218},{490,218},{490,26},{384,26},
            {384,-74}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID1.y, armValve.u[1]) annotation (Line(
        points={{373,-86},{332,-86},{332,-100.5},{286.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(
        StopTime=20,
        __Dymola_NumberOfIntervals=100,
        Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_armControl;

    model v009_control
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves
     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit bucketValve(
        q_nom=0.003,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{314,-126},{344,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit armValve(
        q_nom=0.01,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{256,-126},{286,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        q_nom=0.01,
        q_fraction_A2T=0.4,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6,
      sizeOfInputs=1)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={336,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={322,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={264,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={280,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{342,-146},{362,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // useful control parameters
    //--------------------------------------------------------
     parameter SI.Length arm_strokeLength = 1.64;
     parameter SI.Length arm_closedLength = 2.26;
     parameter SI.Length arm_maxLength = arm_closedLength + arm_strokeLength;
    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints
      MB.Joints.Assemblies.JointRRR bucket_link(
      rRod2_ib={-0.67,0.07,0},
      rRod1_ia={0.62,0.36,0},
      phi_guess=0,
      phi_offset=-45)   annotation (Placement(transformation(extent={{-28,313},
              {1,344}},     rotation=0)));

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init,
        closedLength=arm_closedLength)
        annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
                238}})));
      cyPhy_Excavator.components.bucketCyl     bucketCyl1(bucket_s_init=
            bucket_s_init) annotation (Placement(transformation(rotation=0, extent={{142,302},
                {172,330}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
      cyPhy_Excavator.components.arm     arm1 annotation (Placement(transformation(
              rotation=0, extent={{70,260},{98,288}})));
      cyPhy_Excavator.components.bucket     bucket1 annotation (Placement(
            transformation(rotation=0, extent={{64,298},{104,332}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    cyPhy_Excavator.excavator.linearDistance linearDistance1 annotation (
        Placement(transformation(rotation=0, extent={{-40,204},{-20,224}})));
    Modelica.Blocks.Continuous.LimPID PID(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Td=0.01,
      Ti=0.5,
      Ni=0.2,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=3)
      annotation (Placement(transformation(extent={{380,102},{360,82}})));
    Modelica.Blocks.Sources.TimeTable timeTable(table=[0,3.67; 1,3; 8,2.8; 10,
          2.8; 15,2.8; 15,3.67; 20,3.67])
      annotation (Placement(transformation(extent={{432,82},{412,102}})));
    Modelica.Blocks.Continuous.LimPID PID1(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Ti=0.5,
      Td=0.5,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=5/arm_maxLength)
      annotation (Placement(transformation(extent={{434,-36},{414,-56}})));
    Modelica.Blocks.Sources.TimeTable timeTable1(table=[0,2.3; 5,2.3; 10,3.8;
          15,3.8; 20,3.8])
      annotation (Placement(transformation(extent={{500,-56},{480,-36}})));
    Modelica.Blocks.Sources.Constant const1(k=0)
      annotation (Placement(transformation(extent={{380,-28},{360,-8}})));
    Modelica.Blocks.Continuous.LimPID PID2(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Ti=0.5,
      Td=0.5,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=1)
      annotation (Placement(transformation(extent={{426,-92},{406,-112}})));
    Modelica.Blocks.Sources.TimeTable timeTable2(table=[0,3.07; 10,3.07; 16,
          3.07; 16,3.18; 20,3.18])
      annotation (Placement(transformation(extent={{492,-112},{472,-92}})));
    Modelica.Blocks.Sources.Constant const
      annotation (Placement(transformation(extent={{844,78},{864,98}})));
    Modelica.Blocks.Sources.Ramp ramp(
      offset=25,
      startTime=5,
      duration=5,
      height=2000)
      annotation (Placement(transformation(extent={{10,284},{30,304}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(boomValve.portLS1, armValve.portLS2)
        annotation (Line(
          points={{142,-108},{256,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(armValve.portLS1, bucketValve.portLS2)
        annotation (Line(
          points={{286,-108},{302,-108},{314,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(boomValve.portP1, armValve.portP2) annotation (Line(points={{142,
              -117},{256,-117}},
                              color={255,0,0}));
      connect(boomValve.portT1, armValve.portT2) annotation (Line(points={{142,
              -123},{256,-123}},
                              color={255,0,0}));
      connect(armValve.portP1, bucketValve.portP2) annotation (Line(points={{286,
            -117},{302,-117},{314,-117}},
                                 color={255,0,0}));
      connect(armValve.portT1, bucketValve.portT2) annotation (Line(points={{286,
            -123},{302,-123},{314,-123}},
                                 color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(bucketValve.portA, bucketLineA.port_a)
                                               annotation (Line(points={{321.2,
            -96},{322,-96},{322,-74}},color={255,0,0}));
      connect(bucketValve.portB, bucketLineB.port_a)
                                               annotation (Line(points={{336.8,
            -96},{336,-96},{336,-74}},color={255,0,0}));
      connect(bucketLineA.port_b,bucketCyl1.port_a)
                                              annotation (Line(points={{322,-54},
            {322,-54},{322,132},{322,306},{186,306},{186,305.027},{171.379,
            305.027}},          color={255,0,0}));
      connect(bucketLineB.port_b,bucketCyl1.port_b)
        annotation (Line(points={{336,-54},{336,-54},{336,326},{171.172,326},{
            171.172,326.216}},                     color={255,0,0}));
      connect(armValve.portA, armLineA.port_a)
                                            annotation (Line(points={{263.2,-96},
              {264,-96},{264,-74}},   color={255,0,0}));
      connect(armValve.portB, armLineB.port_a)
                                            annotation (Line(points={{278.8,-96},
              {280,-96},{280,-74}},   color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(bucketValve.portLS1, openTank.port) annotation (Line(
          points={{344,-108},{352,-108},{352,-126}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket_link.frame_a, arm1.frame_b1)
                                               annotation (Line(
          points={{-28,328.5},{-28,294},{-28,292},{-28,272.28},{70.28,272.28},{70.28,
              273.72}},
          color={0,0,0},
          thickness=0.5));
    connect(bucketCyl1.frame_b, bucket_link.frame_im)
                                                 annotation (Line(
          points={{156.069,328.789},{156,328.789},{156,350},{12,350},{12,344},{
            -13.5,344}},
          color={0,0,0},
          thickness=0.5));
      connect(bucketCyl1.frame_a,arm1.frame_b)
                                           annotation (Line(
          points={{157.931,303.059},{156,303.059},{156,285.2},{97.44,285.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket1.frame_b, bucket_link.frame_b)
                                              annotation (Line(
          points={{64.4,318.4},{64.4,318},{52,318},{52,328.5},{1,328.5}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_b,arm1.frame_a)             annotation (Line(
          points={{87.2,187.28},{87.2,187.28},{87.2,261.12},{83.72,261.12}},
          color={95,95,95},
          thickness=0.5));
      connect(arm1.frame_b2,bucket1.frame_a)           annotation (Line(
          points={{84.28,286.6},{86,286.6},{86,299.7},{85.6,299.7}},
          color={95,95,95},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(armCyl1.port_b, armLineB.port_b) annotation (Line(
          points={{161.96,233.84},{280,233.84},{280,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(armCyl1.port_a, armLineA.port_b) annotation (Line(
          points={{161.96,213.84},{264,213.84},{264,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(arm1.frame_b3, armCyl1.frame_b) annotation (Line(
          points={{97.72,261.4},{147,261.4},{147,237.36}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boom1.frame_b3, armCyl1.frame_a) annotation (Line(
          points={{101.2,186.56},{149.38,186.56},{149.38,210.64}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(boom1.frame_a, linearDistance1.frame_a) annotation (Line(
        points={{85.6,164.72},{28.8,164.72},{28.8,212},{-20,212}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(bucketCyl1.frame_a, linearDistance1.frame_b) annotation (Line(
        points={{157.931,303.059},{114.966,303.059},{114.966,218},{-20,218}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(PID.u_s, timeTable.y) annotation (Line(
        points={{382,92},{411,92}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID.y, boomValve.u[1]) annotation (Line(
        points={{359,92},{208,92},{208,-100.5},{142.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(boomcylR.cylLength, PID.u_m) annotation (Line(
        points={{115.62,129.148},{160,129.148},{160,112},{370,112},{370,104}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(armCyl1.cylLength, PID1.u_m) annotation (Line(
        points={{130,232.56},{382,232.56},{382,246},{530,246},{530,54},{424,54},
            {424,-34}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID1.y, armValve.u[1]) annotation (Line(
        points={{413,-46},{306,-46},{306,-100.5},{286.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(swingValve.u[1], const1.y) annotation (Line(
        points={{100.9,-100.5},{100.9,-18},{359,-18}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID1.u_s, timeTable1.y) annotation (Line(
        points={{436,-46},{479,-46}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID2.u_s, timeTable2.y) annotation (Line(
        points={{428,-102},{471,-102}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(bucketValve.u[1], PID2.y) annotation (Line(
        points={{344.9,-100.5},{374.45,-100.5},{374.45,-102},{405,-102}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(bucketCyl1.cylLength, PID2.u_m) annotation (Line(
        points={{140.966,323.189},{118,323.189},{118,340},{544,340},{544,-76},{
            416,-76},{416,-90}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(ramp.y, bucket1.load) annotation (Line(
        points={{31,294},{46,294},{46,311.6},{64,311.6}},
        color={0,0,127},
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(StopTime=20, Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_control;

    model v009_max_distance
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves
     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit bucketValve(
        q_nom=0.003,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{314,-126},{344,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit armValve(
        q_nom=0.01,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{256,-126},{286,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        q_fraction_A2T=0.4,
      sizeOfInputs=1,
      inputIndex=1,
      q_nom=0.01)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6,
      sizeOfInputs=1)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={336,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={322,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={264,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={280,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{342,-146},{362,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints
      MB.Joints.Assemblies.JointRRR bucket_link(
      rRod2_ib={-0.67,0.07,0},
      rRod1_ia={0.62,0.36,0},
      phi_guess=0,
      phi_offset=-45)   annotation (Placement(transformation(extent={{-28,313},
              {1,344}},     rotation=0)));

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init)
        annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
                238}})));
      cyPhy_Excavator.components.bucketCyl     bucketCyl1(bucket_s_init=
            bucket_s_init) annotation (Placement(transformation(rotation=0, extent={{142,302},
                {172,330}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
      cyPhy_Excavator.components.arm     arm1 annotation (Placement(transformation(
              rotation=0, extent={{70,260},{98,288}})));
      cyPhy_Excavator.components.bucket     bucket1 annotation (Placement(
            transformation(rotation=0, extent={{64,298},{104,332}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    cyPhy_Excavator.excavator.linearDistance linearDistance1 annotation (
        Placement(transformation(rotation=0, extent={{-40,204},{-20,224}})));
    Modelica.Blocks.Continuous.LimPID PID(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Td=0.01,
      Ti=0.5,
      Ni=0.2,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=1)
      annotation (Placement(transformation(extent={{380,102},{360,82}})));
    Modelica.Blocks.Sources.TimeTable timeTable(table=[0,0.95; 8,0.95; 8,0.10;
          16,0.10; 16,0.99])
      annotation (Placement(transformation(extent={{432,82},{412,102}})));
    Modelica.Blocks.Continuous.LimPID PID1(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Ti=0.5,
      Td=0.5,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=1)
      annotation (Placement(transformation(extent={{434,-36},{414,-56}})));
    Modelica.Blocks.Sources.TimeTable timeTable1(table=[0,0.01; 12,0.01; 12,
          0.99; 16,0.99])
      annotation (Placement(transformation(extent={{502,-54},{482,-34}})));
    Modelica.Blocks.Sources.Constant const1(k=0)
      annotation (Placement(transformation(extent={{380,-28},{360,-8}})));
    Modelica.Blocks.Continuous.LimPID PID2(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Ti=0.5,
      Td=0.5,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=1)
      annotation (Placement(transformation(extent={{426,-92},{406,-112}})));
    Modelica.Blocks.Sources.TimeTable timeTable2(table=[0,0.01; 11,0.01; 11,0.5;
          14,0.5; 14,0.99])
      annotation (Placement(transformation(extent={{494,-112},{474,-92}})));
    Modelica.Blocks.Sources.Constant const
      annotation (Placement(transformation(extent={{844,78},{864,98}})));
    Modelica.Blocks.Sources.Ramp ramp(
      offset=25,
      duration=5,
      height=2000,
      startTime=12)
      annotation (Placement(transformation(extent={{10,284},{30,304}})));
    MB.Sensors.AbsoluteAngles absoluteAngles
      annotation (Placement(transformation(extent={{204,338},{224,358}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(boomValve.portLS1, armValve.portLS2)
        annotation (Line(
          points={{142,-108},{256,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(armValve.portLS1, bucketValve.portLS2)
        annotation (Line(
          points={{286,-108},{302,-108},{314,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(boomValve.portP1, armValve.portP2) annotation (Line(points={{142,
              -117},{256,-117}},
                              color={255,0,0}));
      connect(boomValve.portT1, armValve.portT2) annotation (Line(points={{142,
              -123},{256,-123}},
                              color={255,0,0}));
      connect(armValve.portP1, bucketValve.portP2) annotation (Line(points={{286,
            -117},{302,-117},{314,-117}},
                                 color={255,0,0}));
      connect(armValve.portT1, bucketValve.portT2) annotation (Line(points={{286,
            -123},{302,-123},{314,-123}},
                                 color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(bucketValve.portA, bucketLineA.port_a)
                                               annotation (Line(points={{321.2,
            -96},{322,-96},{322,-74}},color={255,0,0}));
      connect(bucketValve.portB, bucketLineB.port_a)
                                               annotation (Line(points={{336.8,
            -96},{336,-96},{336,-74}},color={255,0,0}));
      connect(bucketLineA.port_b,bucketCyl1.port_a)
                                              annotation (Line(points={{322,-54},
            {322,-54},{322,132},{322,306},{186,306},{186,305.027},{171.379,
            305.027}},          color={255,0,0}));
      connect(bucketLineB.port_b,bucketCyl1.port_b)
        annotation (Line(points={{336,-54},{338,-54},{338,326},{171.172,326},{
            171.172,326.216}},                     color={255,0,0}));
      connect(armValve.portA, armLineA.port_a)
                                            annotation (Line(points={{263.2,-96},
              {264,-96},{264,-74}},   color={255,0,0}));
      connect(armValve.portB, armLineB.port_a)
                                            annotation (Line(points={{278.8,-96},
              {280,-96},{280,-74}},   color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(bucketValve.portLS1, openTank.port) annotation (Line(
          points={{344,-108},{352,-108},{352,-126}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket_link.frame_a, arm1.frame_b1)
                                               annotation (Line(
          points={{-28,328.5},{-28,294},{-28,292},{-28,272.28},{70.28,272.28},{
            70.28,273.72}},
          color={0,0,0},
          thickness=0.5));
    connect(bucketCyl1.frame_b, bucket_link.frame_im)
                                                 annotation (Line(
          points={{156.069,328.789},{156,328.789},{156,350},{12,350},{12,344},{
            -13.5,344}},
          color={0,0,0},
          thickness=0.5));
      connect(bucketCyl1.frame_a,arm1.frame_b)
                                           annotation (Line(
          points={{157.931,303.059},{156,303.059},{156,285.2},{97.44,285.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket1.frame_b, bucket_link.frame_b)
                                              annotation (Line(
          points={{64.4,318.4},{64.4,318},{52,318},{52,328.5},{1,328.5}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_b,arm1.frame_a)             annotation (Line(
          points={{87.2,187.28},{87.2,187.28},{87.2,261.12},{83.72,261.12}},
          color={95,95,95},
          thickness=0.5));
      connect(arm1.frame_b2,bucket1.frame_a)           annotation (Line(
          points={{84.28,286.6},{86,286.6},{86,299.7},{85.6,299.7}},
          color={95,95,95},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(armCyl1.port_b, armLineB.port_b) annotation (Line(
          points={{161.96,233.84},{280,233.84},{280,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(armCyl1.port_a, armLineA.port_b) annotation (Line(
          points={{161.96,213.84},{264,213.84},{264,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(arm1.frame_b3, armCyl1.frame_b) annotation (Line(
          points={{97.72,261.4},{147,261.4},{147,237.36}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boom1.frame_b3, armCyl1.frame_a) annotation (Line(
          points={{101.2,186.56},{149.38,186.56},{149.38,210.64}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(PID.u_s, timeTable.y) annotation (Line(
        points={{382,92},{411,92}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID.y, boomValve.u[1]) annotation (Line(
        points={{359,92},{208,92},{208,-100.5},{142.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID1.y, armValve.u[1]) annotation (Line(
        points={{413,-46},{306,-46},{306,-100.5},{286.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(swingValve.u[1], const1.y) annotation (Line(
        points={{100.9,-100.5},{100.9,-18},{359,-18}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID1.u_s, timeTable1.y) annotation (Line(
        points={{436,-46},{458,-46},{458,-44},{481,-44}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID2.u_s, timeTable2.y) annotation (Line(
        points={{428,-102},{473,-102}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(bucketValve.u[1], PID2.y) annotation (Line(
        points={{344.9,-100.5},{374.45,-100.5},{374.45,-102},{405,-102}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(ramp.y, bucket1.load) annotation (Line(
        points={{31,294},{46,294},{46,311.6},{64,311.6}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(linearDistance1.frame_b, bucket1.bucket_tip) annotation (Line(
        points={{-20,218},{16,218},{16,248},{56,248},{56,342},{90,342},{90,
            331.32}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(linearDistance1.frame_a, undercarriage.frame_a) annotation (Line(
        points={{-20,212},{14,212},{14,214},{66,214},{66,36},{86,36},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(absoluteAngles.frame_a, bucket1.bucket_tip) annotation (Line(
        points={{204,348},{164,348},{164,366},{90,366},{90,331.32}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(boomcylR.cylLength_unit, PID.u_m) annotation (Line(
        points={{116.3,121.6},{243.15,121.6},{243.15,104},{370,104}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(armCyl1.cylLength_unit, PID1.u_m) annotation (Line(
        points={{128.64,226.32},{122,226.32},{122,200},{442,200},{442,-22},{424,
            -22},{424,-34}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(bucketCyl1.cylLength_unit, PID2.u_m) annotation (Line(
        points={{141.172,314.714},{128,314.714},{128,270},{510,270},{510,-78},{
            416,-78},{416,-90}},
        color={0,0,127},
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(
        StopTime=30,
        __Dymola_NumberOfIntervals=600,
        Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_max_distance;

    model v009_digSeq
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves
     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit bucketValve(
        q_nom=0.003,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{314,-126},{344,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit armValve(
        q_nom=0.01,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{256,-126},{286,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        q_fraction_A2T=0.4,
      sizeOfInputs=1,
      inputIndex=1,
      q_nom=0.01)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6,
      sizeOfInputs=1)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={336,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={322,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={264,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={280,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{342,-146},{362,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints
      MB.Joints.Assemblies.JointRRR bucket_link(
      rRod2_ib={-0.67,0.07,0},
      rRod1_ia={0.62,0.36,0},
      phi_guess=0,
      phi_offset=-45)   annotation (Placement(transformation(extent={{-28,313},
              {1,344}},     rotation=0)));

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init)
        annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
                238}})));
      cyPhy_Excavator.components.bucketCyl     bucketCyl1(bucket_s_init=
            bucket_s_init) annotation (Placement(transformation(rotation=0, extent={{142,302},
                {172,330}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
      cyPhy_Excavator.components.arm     arm1 annotation (Placement(transformation(
              rotation=0, extent={{70,260},{98,288}})));
      cyPhy_Excavator.components.bucket     bucket1 annotation (Placement(
            transformation(rotation=0, extent={{64,298},{104,332}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    cyPhy_Excavator.excavator.linearDistance linearDistance1 annotation (
        Placement(transformation(rotation=0, extent={{-40,204},{-20,224}})));
    Modelica.Blocks.Sources.Constant const
      annotation (Placement(transformation(extent={{844,78},{864,98}})));
    Modelica.Blocks.Sources.Ramp ramp(
      duration=5,
      offset=50,
      startTime=5,
      height=3000)
      annotation (Placement(transformation(extent={{10,284},{30,304}})));
    cyPhy_Excavator.TestComponents.DigCycleSeq digCycleSeq
      annotation (Placement(transformation(extent={{492,2},{402,68}})));
    MB.Sensors.AbsoluteAngles absoluteAngles
      annotation (Placement(transformation(extent={{214,338},{234,358}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(boomValve.portLS1, armValve.portLS2)
        annotation (Line(
          points={{142,-108},{256,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(armValve.portLS1, bucketValve.portLS2)
        annotation (Line(
          points={{286,-108},{302,-108},{314,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(boomValve.portP1, armValve.portP2) annotation (Line(points={{142,
              -117},{256,-117}},
                              color={255,0,0}));
      connect(boomValve.portT1, armValve.portT2) annotation (Line(points={{142,
              -123},{256,-123}},
                              color={255,0,0}));
      connect(armValve.portP1, bucketValve.portP2) annotation (Line(points={{286,
            -117},{302,-117},{314,-117}},
                                 color={255,0,0}));
      connect(armValve.portT1, bucketValve.portT2) annotation (Line(points={{286,
            -123},{302,-123},{314,-123}},
                                 color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(bucketValve.portA, bucketLineA.port_a)
                                               annotation (Line(points={{321.2,
            -96},{322,-96},{322,-74}},color={255,0,0}));
      connect(bucketValve.portB, bucketLineB.port_a)
                                               annotation (Line(points={{336.8,
            -96},{336,-96},{336,-74}},color={255,0,0}));
      connect(bucketLineA.port_b,bucketCyl1.port_a)
                                              annotation (Line(points={{322,-54},
            {322,-54},{322,132},{322,306},{186,306},{186,305.027},{171.379,
            305.027}},          color={255,0,0}));
      connect(bucketLineB.port_b,bucketCyl1.port_b)
        annotation (Line(points={{336,-54},{338,-54},{338,326},{171.172,326},{
            171.172,326.216}},                     color={255,0,0}));
      connect(armValve.portA, armLineA.port_a)
                                            annotation (Line(points={{263.2,-96},
              {264,-96},{264,-74}},   color={255,0,0}));
      connect(armValve.portB, armLineB.port_a)
                                            annotation (Line(points={{278.8,-96},
              {280,-96},{280,-74}},   color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(bucketValve.portLS1, openTank.port) annotation (Line(
          points={{344,-108},{352,-108},{352,-126}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket_link.frame_a, arm1.frame_b1)
                                               annotation (Line(
          points={{-28,328.5},{-28,294},{-28,292},{-28,272.28},{70.28,272.28},{
            70.28,273.72}},
          color={0,0,0},
          thickness=0.5));
    connect(bucketCyl1.frame_b, bucket_link.frame_im)
                                                 annotation (Line(
          points={{156.069,328.789},{156,328.789},{156,350},{12,350},{12,344},{
            -13.5,344}},
          color={0,0,0},
          thickness=0.5));
      connect(bucketCyl1.frame_a,arm1.frame_b)
                                           annotation (Line(
          points={{157.931,303.059},{156,303.059},{156,285.2},{97.44,285.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket1.frame_b, bucket_link.frame_b)
                                              annotation (Line(
          points={{64.4,318.4},{64.4,318},{52,318},{52,328.5},{1,328.5}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_b,arm1.frame_a)             annotation (Line(
          points={{87.2,187.28},{87.2,187.28},{87.2,261.12},{83.72,261.12}},
          color={95,95,95},
          thickness=0.5));
      connect(arm1.frame_b2,bucket1.frame_a)           annotation (Line(
          points={{84.28,286.6},{86,286.6},{86,299.7},{85.6,299.7}},
          color={95,95,95},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(armCyl1.port_b, armLineB.port_b) annotation (Line(
          points={{161.96,233.84},{280,233.84},{280,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(armCyl1.port_a, armLineA.port_b) annotation (Line(
          points={{161.96,213.84},{264,213.84},{264,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(arm1.frame_b3, armCyl1.frame_b) annotation (Line(
          points={{97.72,261.4},{147,261.4},{147,237.36}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boom1.frame_b3, armCyl1.frame_a) annotation (Line(
          points={{101.2,186.56},{149.38,186.56},{149.38,210.64}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(ramp.y, bucket1.load) annotation (Line(
        points={{31,294},{46,294},{46,311.6},{64,311.6}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(linearDistance1.frame_b, bucket1.bucket_tip) annotation (Line(
        points={{-20,218},{16,218},{16,248},{56,248},{56,342},{90,342},{90,
            331.32}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(linearDistance1.frame_a, undercarriage.frame_a) annotation (Line(
        points={{-20,212},{14,212},{14,214},{66,214},{66,36},{86,36},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(digCycleSeq.swing, swingValve.u[1]) annotation (Line(
        points={{399.3,54.8},{100.9,54.8},{100.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(digCycleSeq.boom, boomValve.u[1]) annotation (Line(
        points={{399.3,41.6},{160,41.6},{160,-100.5},{142.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(digCycleSeq.arm, armValve.u[1]) annotation (Line(
        points={{399.3,28.4},{296,28.4},{296,-100.5},{286.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(digCycleSeq.bucket, bucketValve.u[1]) annotation (Line(
        points={{399.3,15.2},{399.3,-100.5},{344.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(bucket1.bucket_tip, absoluteAngles.frame_a) annotation (Line(
        points={{90,331.32},{90,348},{214,348}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(
        StopTime=20,
        __Dymola_NumberOfIntervals=600,
        Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_digSeq;

    model v009_bucketControl
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves
     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit bucketValve(
        q_nom=0.003,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{314,-126},{344,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit armValve(
        q_nom=0.01,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{256,-126},{286,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        q_fraction_A2T=0.4,
      sizeOfInputs=1,
      inputIndex=1,
      q_nom=0.01)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6,
      sizeOfInputs=1)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={336,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={322,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={264,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={280,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{342,-146},{362,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints
      MB.Joints.Assemblies.JointRRR bucket_link(
      rRod2_ib={-0.67,0.07,0},
      rRod1_ia={0.62,0.36,0},
      phi_guess=0,
      phi_offset=-45)   annotation (Placement(transformation(extent={{-28,313},
              {1,344}},     rotation=0)));

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init)
        annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
                238}})));
      cyPhy_Excavator.components.bucketCyl     bucketCyl1(bucket_s_init=
            bucket_s_init) annotation (Placement(transformation(rotation=0, extent={{142,302},
                {172,330}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
      cyPhy_Excavator.components.arm     arm1 annotation (Placement(transformation(
              rotation=0, extent={{70,260},{98,288}})));
      cyPhy_Excavator.components.bucket     bucket1 annotation (Placement(
            transformation(rotation=0, extent={{64,298},{104,332}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    cyPhy_Excavator.excavator.linearDistance linearDistance1 annotation (
        Placement(transformation(rotation=0, extent={{-40,204},{-20,224}})));
    Modelica.Blocks.Continuous.LimPID PID(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Td=0.01,
      Ti=0.5,
      Ni=0.2,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=1)
      annotation (Placement(transformation(extent={{380,102},{360,82}})));
    Modelica.Blocks.Sources.TimeTable timeTable(table=[0,3.67])
      annotation (Placement(transformation(extent={{432,82},{412,102}})));
    Modelica.Blocks.Continuous.LimPID PID1(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Ti=0.5,
      Td=0.5,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=1)
      annotation (Placement(transformation(extent={{434,-36},{414,-56}})));
    Modelica.Blocks.Sources.TimeTable timeTable1(table=[0,2.91])
      annotation (Placement(transformation(extent={{500,-56},{480,-36}})));
    Modelica.Blocks.Sources.Constant const1(k=0)
      annotation (Placement(transformation(extent={{380,-28},{360,-8}})));
    Modelica.Blocks.Continuous.LimPID PID2(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Ti=0.5,
      Td=0.5,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=1)
      annotation (Placement(transformation(extent={{426,-92},{406,-112}})));
    Modelica.Blocks.Sources.TimeTable timeTable2(table=[0,2.31; 5,3.19])
      annotation (Placement(transformation(extent={{494,-112},{474,-92}})));
    Modelica.Blocks.Sources.Constant const
      annotation (Placement(transformation(extent={{844,78},{864,98}})));
    Modelica.Blocks.Sources.Ramp ramp(
      offset=25,
      duration=5,
      height=2000,
      startTime=12)
      annotation (Placement(transformation(extent={{10,284},{30,304}})));
    MB.Sensors.AbsoluteAngles absoluteAngles
      annotation (Placement(transformation(extent={{204,338},{224,358}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(boomValve.portLS1, armValve.portLS2)
        annotation (Line(
          points={{142,-108},{256,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(armValve.portLS1, bucketValve.portLS2)
        annotation (Line(
          points={{286,-108},{302,-108},{314,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(boomValve.portP1, armValve.portP2) annotation (Line(points={{142,
              -117},{256,-117}},
                              color={255,0,0}));
      connect(boomValve.portT1, armValve.portT2) annotation (Line(points={{142,
              -123},{256,-123}},
                              color={255,0,0}));
      connect(armValve.portP1, bucketValve.portP2) annotation (Line(points={{286,
            -117},{302,-117},{314,-117}},
                                 color={255,0,0}));
      connect(armValve.portT1, bucketValve.portT2) annotation (Line(points={{286,
            -123},{302,-123},{314,-123}},
                                 color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(bucketValve.portA, bucketLineA.port_a)
                                               annotation (Line(points={{321.2,
            -96},{322,-96},{322,-74}},color={255,0,0}));
      connect(bucketValve.portB, bucketLineB.port_a)
                                               annotation (Line(points={{336.8,
            -96},{336,-96},{336,-74}},color={255,0,0}));
      connect(bucketLineA.port_b,bucketCyl1.port_a)
                                              annotation (Line(points={{322,-54},
            {322,-54},{322,132},{322,306},{186,306},{186,305.027},{171.379,
            305.027}},          color={255,0,0}));
      connect(bucketLineB.port_b,bucketCyl1.port_b)
        annotation (Line(points={{336,-54},{338,-54},{338,326},{171.172,326},{
            171.172,326.216}},                     color={255,0,0}));
      connect(armValve.portA, armLineA.port_a)
                                            annotation (Line(points={{263.2,-96},
              {264,-96},{264,-74}},   color={255,0,0}));
      connect(armValve.portB, armLineB.port_a)
                                            annotation (Line(points={{278.8,-96},
              {280,-96},{280,-74}},   color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(bucketValve.portLS1, openTank.port) annotation (Line(
          points={{344,-108},{352,-108},{352,-126}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket_link.frame_a, arm1.frame_b1)
                                               annotation (Line(
          points={{-28,328.5},{-28,294},{-28,292},{-28,272.28},{70.28,272.28},{
            70.28,273.72}},
          color={0,0,0},
          thickness=0.5));
    connect(bucketCyl1.frame_b, bucket_link.frame_im)
                                                 annotation (Line(
          points={{156.069,328.789},{156,328.789},{156,350},{12,350},{12,344},{
            -13.5,344}},
          color={0,0,0},
          thickness=0.5));
      connect(bucketCyl1.frame_a,arm1.frame_b)
                                           annotation (Line(
          points={{157.931,303.059},{156,303.059},{156,285.2},{97.44,285.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket1.frame_b, bucket_link.frame_b)
                                              annotation (Line(
          points={{64.4,318.4},{64.4,318},{52,318},{52,328.5},{1,328.5}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_b,arm1.frame_a)             annotation (Line(
          points={{87.2,187.28},{87.2,187.28},{87.2,261.12},{83.72,261.12}},
          color={95,95,95},
          thickness=0.5));
      connect(arm1.frame_b2,bucket1.frame_a)           annotation (Line(
          points={{84.28,286.6},{86,286.6},{86,299.7},{85.6,299.7}},
          color={95,95,95},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(armCyl1.port_b, armLineB.port_b) annotation (Line(
          points={{161.96,233.84},{280,233.84},{280,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(armCyl1.port_a, armLineA.port_b) annotation (Line(
          points={{161.96,213.84},{264,213.84},{264,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(arm1.frame_b3, armCyl1.frame_b) annotation (Line(
          points={{97.72,261.4},{147,261.4},{147,237.36}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boom1.frame_b3, armCyl1.frame_a) annotation (Line(
          points={{101.2,186.56},{149.38,186.56},{149.38,210.64}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(PID.u_s, timeTable.y) annotation (Line(
        points={{382,92},{411,92}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID.y, boomValve.u[1]) annotation (Line(
        points={{359,92},{208,92},{208,-100.5},{142.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(boomcylR.cylLength, PID.u_m) annotation (Line(
        points={{115.62,129.148},{160,129.148},{160,112},{370,112},{370,104}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(armCyl1.cylLength, PID1.u_m) annotation (Line(
        points={{130,232.56},{108,232.56},{108,248},{528,248},{528,56},{424,56},
            {424,-34}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID1.y, armValve.u[1]) annotation (Line(
        points={{413,-46},{306,-46},{306,-100.5},{286.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(swingValve.u[1], const1.y) annotation (Line(
        points={{100.9,-100.5},{100.9,-18},{359,-18}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID1.u_s, timeTable1.y) annotation (Line(
        points={{436,-46},{479,-46}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID2.u_s, timeTable2.y) annotation (Line(
        points={{428,-102},{473,-102}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(bucketValve.u[1], PID2.y) annotation (Line(
        points={{344.9,-100.5},{374.45,-100.5},{374.45,-102},{405,-102}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(bucketCyl1.cylLength, PID2.u_m) annotation (Line(
        points={{140.966,323.189},{118,323.189},{118,340},{544,340},{544,-76},{
            416,-76},{416,-90}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(ramp.y, bucket1.load) annotation (Line(
        points={{31,294},{46,294},{46,311.6},{64,311.6}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(linearDistance1.frame_b, bucket1.bucket_tip) annotation (Line(
        points={{-20,218},{16,218},{16,248},{56,248},{56,342},{90,342},{90,
            331.32}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(linearDistance1.frame_a, undercarriage.frame_a) annotation (Line(
        points={{-20,212},{14,212},{14,214},{66,214},{66,36},{86,36},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(bucket1.bucket_tip, absoluteAngles.frame_a) annotation (Line(
        points={{90,331.32},{94,331.32},{94,356},{204,356},{204,348}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(
        StopTime=10,
        __Dymola_NumberOfIntervals=600,
        Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_bucketControl;

    model v009_dump
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=0.1
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=1.63
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.45
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves
     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit bucketValve(
        q_nom=0.003,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{314,-126},{344,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit armValve(
        q_nom=0.01,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{256,-126},{286,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        q_fraction_A2T=0.4,
      sizeOfInputs=1,
      inputIndex=1,
      q_nom=0.01)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6,
      sizeOfInputs=1)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={336,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={322,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={264,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={280,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{342,-146},{362,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints
      MB.Joints.Assemblies.JointRRR bucket_link(
      rRod2_ib={-0.67,0.07,0},
      rRod1_ia={0.62,0.36,0},
      phi_guess=0,
      phi_offset=-45)   annotation (Placement(transformation(extent={{-28,313},
              {1,344}},     rotation=0)));

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init)
        annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
                238}})));
      cyPhy_Excavator.components.bucketCyl     bucketCyl1(bucket_s_init=
            bucket_s_init) annotation (Placement(transformation(rotation=0, extent={{142,302},
                {172,330}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
      cyPhy_Excavator.components.arm     arm1 annotation (Placement(transformation(
              rotation=0, extent={{70,260},{98,288}})));
      cyPhy_Excavator.components.bucket     bucket1 annotation (Placement(
            transformation(rotation=0, extent={{64,298},{104,332}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    cyPhy_Excavator.excavator.linearDistance linearDistance1 annotation (
        Placement(transformation(rotation=0, extent={{-40,204},{-20,224}})));
    Modelica.Blocks.Continuous.LimPID PID(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Td=0.01,
      Ti=0.5,
      Ni=0.2,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
        k=1)
      annotation (Placement(transformation(extent={{380,102},{360,82}})));
    Modelica.Blocks.Sources.TimeTable timeTable(table=[0,0.01; 8,0.01; 8,0.7;
          10,0.7; 10,0.7])
      annotation (Placement(transformation(extent={{432,82},{412,102}})));
    Modelica.Blocks.Continuous.LimPID PID1(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Ti=0.5,
      Td=0.5,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=0.5)
      annotation (Placement(transformation(extent={{434,-36},{414,-56}})));
    Modelica.Blocks.Sources.TimeTable timeTable1(table=[0,0.99; 15,0.99; 15,0.1;
          15,0.1])
      annotation (Placement(transformation(extent={{502,-54},{482,-34}})));
    Modelica.Blocks.Sources.Constant const1(k=0)
      annotation (Placement(transformation(extent={{390,4},{370,24}})));
    Modelica.Blocks.Continuous.LimPID PID2(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Ti=0.5,
      Td=0.5,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=1)
      annotation (Placement(transformation(extent={{426,-92},{406,-112}})));
    Modelica.Blocks.Sources.TimeTable timeTable2(table=[0,0.9; 11,0.9])
      annotation (Placement(transformation(extent={{494,-112},{474,-92}})));
    Modelica.Blocks.Sources.Constant const
      annotation (Placement(transformation(extent={{844,78},{864,98}})));
    Modelica.Blocks.Sources.Ramp ramp(
      offset=25,
      duration=5,
      startTime=12,
      height=3000)
      annotation (Placement(transformation(extent={{10,284},{30,304}})));
    cyPhy_Excavator.components.BucketAngle bucketAngle annotation (Placement(
          transformation(rotation=0, extent={{192,340},{212,360}})));
    MB.Forces.Spring spring(c=100)
      annotation (Placement(transformation(extent={{368,330},{388,350}})));
    MB.Parts.Fixed fixed1(r={5,0,0}) annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=90,
          origin={436,302})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(boomValve.portLS1, armValve.portLS2)
        annotation (Line(
          points={{142,-108},{256,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(armValve.portLS1, bucketValve.portLS2)
        annotation (Line(
          points={{286,-108},{302,-108},{314,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(boomValve.portP1, armValve.portP2) annotation (Line(points={{142,
              -117},{256,-117}},
                              color={255,0,0}));
      connect(boomValve.portT1, armValve.portT2) annotation (Line(points={{142,
              -123},{256,-123}},
                              color={255,0,0}));
      connect(armValve.portP1, bucketValve.portP2) annotation (Line(points={{286,
            -117},{302,-117},{314,-117}},
                                 color={255,0,0}));
      connect(armValve.portT1, bucketValve.portT2) annotation (Line(points={{286,
            -123},{302,-123},{314,-123}},
                                 color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(bucketValve.portA, bucketLineA.port_a)
                                               annotation (Line(points={{321.2,
            -96},{322,-96},{322,-74}},color={255,0,0}));
      connect(bucketValve.portB, bucketLineB.port_a)
                                               annotation (Line(points={{336.8,
            -96},{336,-96},{336,-74}},color={255,0,0}));
      connect(bucketLineA.port_b,bucketCyl1.port_a)
                                              annotation (Line(points={{322,-54},
            {322,-54},{322,132},{322,306},{186,306},{186,305.027},{171.379,
            305.027}},          color={255,0,0}));
      connect(bucketLineB.port_b,bucketCyl1.port_b)
        annotation (Line(points={{336,-54},{338,-54},{338,326},{171.172,326},{
            171.172,326.216}},                     color={255,0,0}));
      connect(armValve.portA, armLineA.port_a)
                                            annotation (Line(points={{263.2,-96},
              {264,-96},{264,-74}},   color={255,0,0}));
      connect(armValve.portB, armLineB.port_a)
                                            annotation (Line(points={{278.8,-96},
              {280,-96},{280,-74}},   color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(bucketValve.portLS1, openTank.port) annotation (Line(
          points={{344,-108},{352,-108},{352,-126}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket_link.frame_a, arm1.frame_b1)
                                               annotation (Line(
          points={{-28,328.5},{-28,294},{-28,292},{-28,272.28},{70.28,272.28},{
            70.28,273.72}},
          color={0,0,0},
          thickness=0.5));
    connect(bucketCyl1.frame_b, bucket_link.frame_im)
                                                 annotation (Line(
          points={{156.069,328.789},{156,328.789},{156,350},{12,350},{12,344},{
            -13.5,344}},
          color={0,0,0},
          thickness=0.5));
      connect(bucketCyl1.frame_a,arm1.frame_b)
                                           annotation (Line(
          points={{157.931,303.059},{156,303.059},{156,285.2},{97.44,285.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket1.frame_b, bucket_link.frame_b)
                                              annotation (Line(
          points={{64.4,318.4},{64.4,318},{52,318},{52,328.5},{1,328.5}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_b,arm1.frame_a)             annotation (Line(
          points={{87.2,187.28},{87.2,187.28},{87.2,261.12},{83.72,261.12}},
          color={95,95,95},
          thickness=0.5));
      connect(arm1.frame_b2,bucket1.frame_a)           annotation (Line(
          points={{84.28,286.6},{86,286.6},{86,299.7},{85.6,299.7}},
          color={95,95,95},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(armCyl1.port_b, armLineB.port_b) annotation (Line(
          points={{161.96,233.84},{280,233.84},{280,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(armCyl1.port_a, armLineA.port_b) annotation (Line(
          points={{161.96,213.84},{264,213.84},{264,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(arm1.frame_b3, armCyl1.frame_b) annotation (Line(
          points={{97.72,261.4},{147,261.4},{147,237.36}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boom1.frame_b3, armCyl1.frame_a) annotation (Line(
          points={{101.2,186.56},{149.38,186.56},{149.38,210.64}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(PID.u_s, timeTable.y) annotation (Line(
        points={{382,92},{411,92}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID.y, boomValve.u[1]) annotation (Line(
        points={{359,92},{208,92},{208,-100.5},{142.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID1.y, armValve.u[1]) annotation (Line(
        points={{413,-46},{306,-46},{306,-100.5},{286.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(swingValve.u[1], const1.y) annotation (Line(
        points={{100.9,-100.5},{100.9,14},{369,14}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID1.u_s, timeTable1.y) annotation (Line(
        points={{436,-46},{458,-46},{458,-44},{481,-44}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID2.u_s, timeTable2.y) annotation (Line(
        points={{428,-102},{473,-102}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(bucketValve.u[1], PID2.y) annotation (Line(
        points={{344.9,-100.5},{374.45,-100.5},{374.45,-102},{405,-102}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(ramp.y, bucket1.load) annotation (Line(
        points={{31,294},{46,294},{46,311.6},{64,311.6}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(linearDistance1.frame_b, bucket1.bucket_tip) annotation (Line(
        points={{-20,218},{16,218},{16,248},{56,248},{56,342},{90,342},{90,
            331.32}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(linearDistance1.frame_a, undercarriage.frame_a) annotation (Line(
        points={{-20,212},{14,212},{14,214},{66,214},{66,36},{86,36},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
      connect(bucketAngle.frame_a,    bucket1.bucket_tip) annotation (Line(
          points={{192,350},{164,350},{164,366},{90,366},{90,331.32}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boomcylR.cylLength_unit, PID.u_m) annotation (Line(
          points={{116.3,121.6},{243.15,121.6},{243.15,104},{370,104}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(armCyl1.cylLength_unit, PID1.u_m) annotation (Line(
          points={{128.64,226.32},{122,226.32},{122,200},{442,200},{442,-22},{424,-22},
              {424,-34}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(bucketCyl1.cylLength_unit, PID2.u_m) annotation (Line(
          points={{141.172,314.714},{128,314.714},{128,270},{510,270},{510,-78},
            {416,-78},{416,-90}},
          color={0,0,127},
          smooth=Smooth.None));
    connect(spring.frame_b, fixed1.frame_b) annotation (Line(
        points={{388,340},{412,340},{412,312},{436,312}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(spring.frame_a, bucket1.bucket_tip) annotation (Line(
        points={{368,340},{230,340},{230,331.32},{90,331.32}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(
        StopTime=25,
        __Dymola_NumberOfIntervals=600,
        Tolerance=1e-008),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_dump;

    model v009_maxLoad
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=0.45
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.1
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.45
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit armValve(
        q_nom=0.01,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{256,-126},{286,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        q_fraction_A2T=0.4,
      sizeOfInputs=1,
      inputIndex=1,
      q_nom=0.01)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6,
      sizeOfInputs=1)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={264,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={280,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{342,-146},{362,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init)
        annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
                238}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
      cyPhy_Excavator.components.arm     arm1 annotation (Placement(transformation(
              rotation=0, extent={{70,260},{98,288}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    cyPhy_Excavator.excavator.linearDistance linearDistance1 annotation (
        Placement(transformation(rotation=0, extent={{-38,202},{-18,222}})));
    Modelica.Blocks.Continuous.LimPID PID(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Td=0.01,
      Ti=0.5,
      Ni=0.2,
        controllerType=Modelica.Blocks.Types.SimpleController.PI,
        k=2)
      annotation (Placement(transformation(extent={{380,102},{360,82}})));
    Modelica.Blocks.Sources.TimeTable timeTable(table=[0,0.36])
      annotation (Placement(transformation(extent={{434,82},{414,102}})));
    Modelica.Blocks.Continuous.LimPID PID1(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Ti=0.5,
      Td=0.5,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=0.5)
      annotation (Placement(transformation(extent={{436,-58},{416,-78}})));
    Modelica.Blocks.Sources.TimeTable timeTable1(table=[0,0.061])
      annotation (Placement(transformation(extent={{502,-54},{482,-34}})));
    Modelica.Blocks.Sources.Constant const1(k=0)
      annotation (Placement(transformation(extent={{390,4},{370,24}})));
    Modelica.Blocks.Sources.Constant const
      annotation (Placement(transformation(extent={{844,78},{864,98}})));
      Modelica.Blocks.Sources.Ramp ramp(
      duration=30,
      startTime=5,
      height=0,
      offset=-60000)
        annotation (Placement(transformation(extent={{224,296},{204,316}})));
    Modelica.Blocks.Sources.TimeTable timeTable2(table=[0,0; 5,0; 5,-1; 8,-1; 8,
          0])
      annotation (Placement(transformation(extent={{372,-72},{352,-52}})));
    Modelica.Blocks.Sources.TimeTable timeTable3(table=[0,0])
      annotation (Placement(transformation(extent={{214,-80},{194,-60}})));
      Modelica.Blocks.Sources.Ramp ramp1(
      startTime=5,
      duration=30,
      offset=0,
      height=-1)
        annotation (Placement(transformation(extent={{326,-182},{346,-162}})));
    cyPhy_Excavator.components.WorldForce force1
      annotation (Placement(transformation(extent={{130,314},{110,334}})));
    Modelica.Blocks.Sources.Constant const2(k=0)
      annotation (Placement(transformation(extent={{220,328},{200,348}})));
    Modelica.Blocks.Sources.Constant const3(k=0)
      annotation (Placement(transformation(extent={{244,260},{224,280}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(boomValve.portLS1, armValve.portLS2)
        annotation (Line(
          points={{142,-108},{256,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(boomValve.portP1, armValve.portP2) annotation (Line(points={{142,
              -117},{256,-117}},
                              color={255,0,0}));
      connect(boomValve.portT1, armValve.portT2) annotation (Line(points={{142,
              -123},{256,-123}},
                              color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(armValve.portA, armLineA.port_a)
                                            annotation (Line(points={{263.2,-96},
              {264,-96},{264,-74}},   color={255,0,0}));
      connect(armValve.portB, armLineB.port_a)
                                            annotation (Line(points={{278.8,-96},
              {280,-96},{280,-74}},   color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_b,arm1.frame_a)             annotation (Line(
          points={{87.2,187.28},{87.2,187.28},{87.2,261.12},{83.72,261.12}},
          color={95,95,95},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(armCyl1.port_b, armLineB.port_b) annotation (Line(
          points={{161.96,233.84},{280,233.84},{280,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(armCyl1.port_a, armLineA.port_b) annotation (Line(
          points={{161.96,213.84},{264,213.84},{264,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(arm1.frame_b3, armCyl1.frame_b) annotation (Line(
          points={{97.72,261.4},{147,261.4},{147,237.36}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boom1.frame_b3, armCyl1.frame_a) annotation (Line(
          points={{101.2,186.56},{149.38,186.56},{149.38,210.64}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(PID.u_s, timeTable.y) annotation (Line(
        points={{382,92},{413,92}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(swingValve.u[1], const1.y) annotation (Line(
        points={{100.9,-100.5},{100.9,14},{369,14}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID1.u_s, timeTable1.y) annotation (Line(
        points={{438,-68},{458,-68},{458,-44},{481,-44}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(linearDistance1.frame_a, undercarriage.frame_a) annotation (Line(
        points={{-18,210},{14,210},{14,214},{66,214},{66,36},{86,36},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
      connect(boomcylR.cylLength_unit, PID.u_m) annotation (Line(
          points={{116.3,121.6},{243.15,121.6},{243.15,104},{370,104}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(armCyl1.cylLength_unit, PID1.u_m) annotation (Line(
          points={{128.64,226.32},{122,226.32},{122,200},{442,200},{442,-56},{426,-56}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(openTank.port, armValve.portLS1) annotation (Line(
          points={{352,-126},{352,-108},{286,-108}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(linearDistance1.frame_b, arm1.frame_b2) annotation (Line(
          points={{-18,216},{34,216},{34,288},{84,288},{84.28,286.6}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(timeTable3.y, boomValve.u[1]) annotation (Line(
          points={{193,-70},{170,-70},{170,-100.5},{142.9,-100.5}},
          color={0,0,127},
          smooth=Smooth.None));
    connect(ramp1.y, armValve.u[1]) annotation (Line(
        points={{347,-172},{362,-172},{362,-168},{374,-168},{374,-100.5},{286.9,
            -100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(force1.frame_b, arm1.frame_b2) annotation (Line(
        points={{110,324},{98,324},{98,320},{84,320},{84,286},{84.28,286.6}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(const3.y, force1.zForce) annotation (Line(
        points={{223,270},{178,270},{178,318},{130.8,318}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(force1.xForce, const2.y) annotation (Line(
        points={{130.8,330},{164.4,330},{164.4,338},{199,338}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(force1.yForce, ramp.y) annotation (Line(
        points={{130.8,324},{194,324},{194,306},{203,306}},
        color={0,0,127},
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(
          StopTime=30,
          __Dymola_NumberOfIntervals=600,
          Tolerance=1e-008),
        experimentSetupOutput(textual=true, equdistant=false),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_maxLoad;

    model v009_maxSpring
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=0.45
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=0.1
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.45
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit armValve(
        q_nom=0.01,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{256,-126},{286,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        q_fraction_A2T=0.4,
      sizeOfInputs=1,
      inputIndex=1,
      q_nom=0.01)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6,
      sizeOfInputs=1)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={264,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={280,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{342,-146},{362,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init)
        annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
                238}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
      cyPhy_Excavator.components.arm     arm1 annotation (Placement(transformation(
              rotation=0, extent={{70,260},{98,288}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    cyPhy_Excavator.excavator.linearDistance linearDistance1 annotation (
        Placement(transformation(rotation=0, extent={{-38,202},{-18,222}})));
    Modelica.Blocks.Continuous.LimPID PID(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Td=0.01,
      Ti=0.5,
      Ni=0.2,
        controllerType=Modelica.Blocks.Types.SimpleController.PI,
        k=2)
      annotation (Placement(transformation(extent={{380,102},{360,82}})));
    Modelica.Blocks.Sources.TimeTable timeTable(table=[0,0.36])
      annotation (Placement(transformation(extent={{434,82},{414,102}})));
    Modelica.Blocks.Continuous.LimPID PID1(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Ti=0.5,
      Td=0.5,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=0.5)
      annotation (Placement(transformation(extent={{436,-58},{416,-78}})));
    Modelica.Blocks.Sources.TimeTable timeTable1(table=[0,0.061])
      annotation (Placement(transformation(extent={{502,-54},{482,-34}})));
    Modelica.Blocks.Sources.Constant const1(k=0)
      annotation (Placement(transformation(extent={{390,4},{370,24}})));
    Modelica.Blocks.Sources.Constant const
      annotation (Placement(transformation(extent={{844,78},{864,98}})));
    Modelica.Blocks.Sources.TimeTable timeTable2(table=[0,0; 5,0; 5,-1; 10,-1;
          10,0])
      annotation (Placement(transformation(extent={{372,-72},{352,-52}})));
    Modelica.Blocks.Sources.TimeTable timeTable3(table=[0,0])
      annotation (Placement(transformation(extent={{214,-80},{194,-60}})));
      MB.Parts.Fixed fixed1(r={7,0,0}) annotation (Placement(transformation(
            extent={{-10,-10},{10,10}},
            rotation=90,
            origin={288,260})));
      MB.Forces.SpringDamperParallel springDamperParallel(
        c=10,
        s_unstretched=3,
      d=10000000)
        annotation (Placement(transformation(extent={{182,264},{202,284}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(boomValve.portLS1, armValve.portLS2)
        annotation (Line(
          points={{142,-108},{256,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(boomValve.portP1, armValve.portP2) annotation (Line(points={{142,
              -117},{256,-117}},
                              color={255,0,0}));
      connect(boomValve.portT1, armValve.portT2) annotation (Line(points={{142,
              -123},{256,-123}},
                              color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(armValve.portA, armLineA.port_a)
                                            annotation (Line(points={{263.2,-96},
              {264,-96},{264,-74}},   color={255,0,0}));
      connect(armValve.portB, armLineB.port_a)
                                            annotation (Line(points={{278.8,-96},
              {280,-96},{280,-74}},   color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_b,arm1.frame_a)             annotation (Line(
          points={{87.2,187.28},{87.2,187.28},{87.2,261.12},{83.72,261.12}},
          color={95,95,95},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(armCyl1.port_b, armLineB.port_b) annotation (Line(
          points={{161.96,233.84},{280,233.84},{280,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(armCyl1.port_a, armLineA.port_b) annotation (Line(
          points={{161.96,213.84},{264,213.84},{264,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(arm1.frame_b3, armCyl1.frame_b) annotation (Line(
          points={{97.72,261.4},{147,261.4},{147,237.36}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boom1.frame_b3, armCyl1.frame_a) annotation (Line(
          points={{101.2,186.56},{149.38,186.56},{149.38,210.64}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(PID.u_s, timeTable.y) annotation (Line(
        points={{382,92},{413,92}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(swingValve.u[1], const1.y) annotation (Line(
        points={{100.9,-100.5},{100.9,14},{369,14}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID1.u_s, timeTable1.y) annotation (Line(
        points={{438,-68},{458,-68},{458,-44},{481,-44}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(linearDistance1.frame_a, undercarriage.frame_a) annotation (Line(
        points={{-18,210},{14,210},{14,214},{66,214},{66,36},{86,36},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
      connect(boomcylR.cylLength_unit, PID.u_m) annotation (Line(
          points={{116.3,121.6},{243.15,121.6},{243.15,104},{370,104}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(armCyl1.cylLength_unit, PID1.u_m) annotation (Line(
          points={{128.64,226.32},{122,226.32},{122,200},{442,200},{442,-56},{426,-56}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(openTank.port, armValve.portLS1) annotation (Line(
          points={{352,-126},{352,-108},{286,-108}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(linearDistance1.frame_b, arm1.frame_b2) annotation (Line(
          points={{-18,216},{34,216},{34,288},{84,288},{84.28,286.6}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(timeTable2.y, armValve.u[1]) annotation (Line(
          points={{351,-62},{320,-62},{320,-100.5},{286.9,-100.5}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(timeTable3.y, boomValve.u[1]) annotation (Line(
          points={{193,-70},{170,-70},{170,-100.5},{142.9,-100.5}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(arm1.frame_b2, springDamperParallel.frame_a) annotation (Line(
          points={{84.28,286.6},{132.14,286.6},{132.14,274},{182,274}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(springDamperParallel.frame_b, fixed1.frame_b) annotation (Line(
          points={{202,274},{246,274},{246,270},{288,270}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(
          StopTime=30,
          __Dymola_NumberOfIntervals=600,
          Tolerance=1e-008),
        experimentSetupOutput(textual=true, equdistant=false),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_maxSpring;

    model v009_cushionExperiments
      import cyPhy_Excavator = excavator_cyphy;
      extends Modelica.Icons.Example;

    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------
    extends OpenHydraulics.Interfaces.PartialFluidCircuit;

      //parameter SI.
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length arm_s_init=1.64
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      // the main components

     OpenHydraulics.Components.MotorsPumps.Motor swingMotor(Dconst=0.3)
        annotation (Placement(transformation(
            origin={-28,-40},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      // the valves
     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit bucketValve(
        q_nom=0.003,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{314,-126},{344,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit armValve(
        q_nom=0.01,
      sizeOfInputs=1,
      inputIndex=1)
        annotation (Placement(transformation(extent={{256,-126},{286,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit boomValve(
        q_fraction_A2T=0.4,
      sizeOfInputs=1,
      inputIndex=1,
      q_nom=0.01)
        annotation (Placement(transformation(extent={{112,-126},{142,-96}},
              rotation=0)));

     OpenHydraulics.Examples.Excavator.SubSystems.LSValveUnit swingValve(
        q_nom=0.01,
        q_fraction_A2T=0.6,
        q_fraction_B2T=0.6,
      sizeOfInputs=1)
        annotation (Placement(transformation(extent={{70,-126},{100,-96}},
              rotation=0)));

      // the LoadSensing pump and control circuitry
      Modelica.Mechanics.Rotational.Sources.ConstantSpeed constantSpeed(
          w_fixed=150,
          useSupport=false)
        annotation (Placement(transformation(extent={{100,-176},{120,-156}},
              rotation=0)));
      OpenHydraulics.Examples.Excavator.SubSystems.PowerUnit powerUnit
        annotation (Placement(transformation(extent={{134,-182},{166,-150}},
              rotation=0)));

      // the lines and junctions
      OpenHydraulics.Components.Lines.NJunction jB
        annotation (Placement(transformation(extent={{126,-34},{146,-14}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction jA
        annotation (Placement(transformation(extent={{110,-50},{130,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.Line boomLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={136,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line boomLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={120,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={336,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line bucketLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={322,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineA(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={264,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line armLineB(
        L=1,
        D=0.05,
        lineBulkMod=1e7)
        annotation (Placement(transformation(
            origin={280,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineA(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={78,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Components.Lines.Line swingLineB(
        L=2,
        D=0.03,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={94,-64},
            extent={{-10,-10},{10,10}},
            rotation=90)));

      OpenHydraulics.Basic.OpenTank openTank
        annotation (Placement(transformation(extent={{342,-146},{362,-126}},
              rotation=0)));

      // the connectors

      OpenHydraulics.Components.Lines.Line pumpLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={100,-140},
            extent={{-10,-10},{10,10}},
            rotation=180)));

      OpenHydraulics.Components.Lines.Line tankLine(
        D=0.05,
        L=2,
        lineBulkMod=1e8)
        annotation (Placement(transformation(
            origin={130,-134},
            extent={{-10,-10},{10,10}},
            rotation=180)));

    //--------------------------------------------------------
    // MechanicsBody
    //--------------------------------------------------------
    import MB = Modelica.Mechanics.MultiBody;

      //outer MB.World world;

      // the joints
      MB.Joints.Assemblies.JointRRR bucket_link(
      rRod2_ib={-0.67,0.07,0},
      rRod1_ia={0.62,0.36,0},
      phi_guess=0,
      phi_offset=-45)   annotation (Placement(transformation(extent={{-28,313},
              {1,344}},     rotation=0)));

      // joint friction

      // the outside interface

      // the actuators

      // the linkages

      inner MB.World world
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
            rotation=90,
            origin={-110,286})));
      cyPhy_Excavator.components.boomCylL boomCylL(boom_s_init=boom_s_init)
                                        annotation (Placement(transformation(
              rotation=0, extent={{-22,118},{12,138}})));
      cyPhy_Excavator.components.boomCylR boomcylR(boom_s_init=boom_s_init)
                                                  annotation (Placement(
            transformation(rotation=0, extent={{118,116},{152,144}})));
      cyPhy_Excavator.components.carriage     carriage1 annotation (Placement(
            transformation(rotation=0, extent={{78,80},{98,100}})));
      cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init)
        annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
                238}})));
      cyPhy_Excavator.components.bucketCyl     bucketCyl1(bucket_s_init=
            bucket_s_init) annotation (Placement(transformation(rotation=0, extent={{142,302},
                {172,330}})));
      cyPhy_Excavator.components.boom     boom1 annotation (Placement(
            transformation(rotation=0, extent={{66,164},{106,188}})));
      cyPhy_Excavator.components.arm     arm1 annotation (Placement(transformation(
              rotation=0, extent={{70,260},{98,288}})));
      cyPhy_Excavator.components.bucket     bucket1 annotation (Placement(
            transformation(rotation=0, extent={{64,298},{104,332}})));
    cyPhy_Excavator.components.undercarriage undercarriage
                                         annotation (Placement(transformation(
            rotation=0, extent={{76,38},{96,58}})));
    MB.Parts.Fixed fixed
      annotation (Placement(transformation(extent={{34,16},{54,36}})));
    cyPhy_Excavator.excavator.linearDistance linearDistance1 annotation (
        Placement(transformation(rotation=0, extent={{-40,204},{-20,224}})));
    Modelica.Blocks.Continuous.LimPID PID(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Td=0.01,
      Ti=0.5,
      Ni=0.2,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=1)
      annotation (Placement(transformation(extent={{380,102},{360,82}})));
    Modelica.Blocks.Sources.TimeTable timeTable(table=[0,0])
      annotation (Placement(transformation(extent={{432,82},{412,102}})));
    Modelica.Blocks.Continuous.LimPID PID1(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Ti=0.5,
      Td=0.5,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=1)
      annotation (Placement(transformation(extent={{434,-36},{414,-56}})));
    Modelica.Blocks.Sources.TimeTable timeTable1(table=[0,2.91])
      annotation (Placement(transformation(extent={{500,-56},{480,-36}})));
    Modelica.Blocks.Sources.Constant const1(k=0)
      annotation (Placement(transformation(extent={{384,-12},{364,8}})));
    Modelica.Blocks.Continuous.LimPID PID2(
      initType=Modelica.Blocks.Types.InitPID.NoInit,
      yMax=1,
      Ti=0.5,
      Td=0.5,
      controllerType=Modelica.Blocks.Types.SimpleController.P,
      k=1)
      annotation (Placement(transformation(extent={{426,-92},{406,-112}})));
    Modelica.Blocks.Sources.TimeTable timeTable2(table=[0,0])
      annotation (Placement(transformation(extent={{494,-112},{474,-92}})));
    Modelica.Blocks.Sources.Constant const
      annotation (Placement(transformation(extent={{844,78},{864,98}})));
    Modelica.Blocks.Sources.Ramp ramp(
      offset=25,
      duration=5,
      height=2000,
      startTime=12)
      annotation (Placement(transformation(extent={{10,284},{30,304}})));
    MB.Sensors.AbsoluteAngles absoluteAngles
      annotation (Placement(transformation(extent={{204,338},{224,358}})));
    Modelica.Blocks.Sources.Constant const2(k=-1)
      annotation (Placement(transformation(extent={{376,-46},{356,-26}})));
    Modelica.Blocks.Sources.Constant const3(k=0)
      annotation (Placement(transformation(extent={{424,-166},{404,-146}})));
    Modelica.Blocks.Sources.Constant const4(k=0)
      annotation (Placement(transformation(extent={{208,-88},{188,-68}})));
    equation
    //--------------------------------------------------------
    // HydraulicSubSyst equations
    //--------------------------------------------------------

      connect(swingValve.portLS1, boomValve.portLS2) annotation (Line(
          points={{100,-108},{112,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(boomValve.portLS1, armValve.portLS2)
        annotation (Line(
          points={{142,-108},{256,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(armValve.portLS1, bucketValve.portLS2)
        annotation (Line(
          points={{286,-108},{302,-108},{314,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(swingValve.portP1, boomValve.portP2) annotation (Line(points={{100,-117},
              {112,-117}},         color={255,0,0}));
      connect(swingValve.portT1, boomValve.portT2) annotation (Line(points={{100,-123},
              {112,-123}},         color={255,0,0}));
      connect(boomValve.portP1, armValve.portP2) annotation (Line(points={{142,
              -117},{256,-117}},
                              color={255,0,0}));
      connect(boomValve.portT1, armValve.portT2) annotation (Line(points={{142,
              -123},{256,-123}},
                              color={255,0,0}));
      connect(armValve.portP1, bucketValve.portP2) annotation (Line(points={{286,
            -117},{302,-117},{314,-117}},
                                 color={255,0,0}));
      connect(armValve.portT1, bucketValve.portT2) annotation (Line(points={{286,
            -123},{302,-123},{314,-123}},
                                 color={255,0,0}));
      connect(constantSpeed.flange, powerUnit.flange_a) annotation (Line(
            points={{120,-166},{134,-166}},
                                          color={0,0,0}));
      connect(powerUnit.portLS, swingValve.portLS2) annotation (Line(
          points={{140.4,-150},{140,-148},{140,-146},{56,-146},{56,-108},{70,-108}},
          color={255,0,0},
          pattern=LinePattern.Dash));

    connect(jA.port[3], boomCylL.port_a)      annotation (Line(points={{120,
            -39.3333},{120,-18},{-52,-18},{-52,136.8},{-20.3,136.8}},
                                           color={255,0,0}));
    connect(jB.port[2], boomcylR.port_b)       annotation (Line(points={{136,-24},
            {162,-24},{162,117.704},{150.98,117.704}},
                                      color={255,0,0}));
    connect(jB.port[3], boomCylL.port_b)      annotation (Line(points={{136,
            -23.3333},{136,-6},{-30,-6},{-30,98},{-30,119.467},{-20.98,119.467}},
                                                             color={255,0,0}));
      connect(boomValve.portB, boomLineB.port_a)
                                             annotation (Line(points={{134.8,-96},{136,
              -96},{136,-74}},          color={255,0,0}));
      connect(boomValve.portA, boomLineA.port_a)
                                             annotation (Line(points={{119.2,-96},{120,
              -96},{120,-74}},          color={255,0,0}));
      connect(boomLineA.port_b, jA.port[1])
                                        annotation (Line(points={{120,-54},{120,
            -40.6667}},      color={255,0,0}));
      connect(boomLineB.port_b, jB.port[1])
                                        annotation (Line(points={{136,-54},{136,
            -24.6667}},      color={255,0,0}));
      connect(bucketValve.portA, bucketLineA.port_a)
                                               annotation (Line(points={{321.2,
            -96},{322,-96},{322,-74}},color={255,0,0}));
      connect(bucketValve.portB, bucketLineB.port_a)
                                               annotation (Line(points={{336.8,
            -96},{336,-96},{336,-74}},color={255,0,0}));
      connect(bucketLineA.port_b,bucketCyl1.port_a)
                                              annotation (Line(points={{322,-54},
            {322,-54},{322,132},{322,306},{186,306},{186,305.027},{171.379,
            305.027}},          color={255,0,0}));
      connect(bucketLineB.port_b,bucketCyl1.port_b)
        annotation (Line(points={{336,-54},{338,-54},{338,326},{171.172,326},{
            171.172,326.216}},                     color={255,0,0}));
      connect(armValve.portA, armLineA.port_a)
                                            annotation (Line(points={{263.2,-96},
              {264,-96},{264,-74}},   color={255,0,0}));
      connect(armValve.portB, armLineB.port_a)
                                            annotation (Line(points={{278.8,-96},
              {280,-96},{280,-74}},   color={255,0,0}));
      connect(swingValve.portA, swingLineA.port_a)
                                              annotation (Line(points={{77.2,-96},{78,
              -96},{78,-74}},           color={255,0,0}));
      connect(swingValve.portB, swingLineB.port_a)
                                              annotation (Line(points={{92.8,-96},{94,
              -96},{94,-74}},           color={255,0,0}));
      connect(swingLineA.port_b, swingMotor.portB)
                                        annotation (Line(points={{78,-54},{-28,
              -54},{-28,-50}},   color={255,0,0}));
      connect(swingLineB.port_b, swingMotor.portA)
                                         annotation (Line(points={{94,-54},{94,
              -42},{-2,-42},{-2,-30},{-28,-30}},
                                 color={255,0,0}));
      connect(bucketValve.portLS1, openTank.port) annotation (Line(
          points={{344,-108},{352,-108},{352,-126}},
          color={255,0,0},
          pattern=LinePattern.Dash));
      connect(powerUnit.portT, tankLine.port_a) annotation (Line(points={{159.6,-150},
              {160,-150},{160,-134},{140,-134}}, color={255,0,0}));
      connect(tankLine.port_b, swingValve.portT2) annotation (Line(points={{120,-134},
              {66,-134},{66,-123},{70,-123}},          color={255,0,0}));
      connect(powerUnit.portP, pumpLine.port_a) annotation (Line(points={{150,-150},
              {150,-140},{110,-140}},  color={255,0,0}));
      connect(pumpLine.port_b, swingValve.portP2) annotation (Line(points={{90,-140},
              {60,-140},{60,-117},{70,-117}},          color={255,0,0}));

    connect(carriage1.frame_a, undercarriage.frame_b)           annotation (Line(
          points={{88,80},{88,58}},
          color={0,0,0},
          thickness=0.5));
    //--------------------------------------------------------
    // MechanicsBody equations
    //--------------------------------------------------------

    connect(boomCylL.frame_a, carriage1.frame_b)
                                            annotation (Line(
          points={{-0.92,118.4},{-0.92,92.2},{78.8,92.2}},
          color={0,0,0},
          thickness=0.5));
    connect(boom1.frame_b1, boomCylL.frame_b)
                                            annotation (Line(
          points={{66,164.96},{0.78,164.96},{0.78,137.333}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket_link.frame_a, arm1.frame_b1)
                                               annotation (Line(
          points={{-28,328.5},{-28,294},{-28,292},{-28,272.28},{70.28,272.28},{
            70.28,273.72}},
          color={0,0,0},
          thickness=0.5));
    connect(bucketCyl1.frame_b, bucket_link.frame_im)
                                                 annotation (Line(
          points={{156.069,328.789},{156,328.789},{156,350},{12,350},{12,344},{
            -13.5,344}},
          color={0,0,0},
          thickness=0.5));
      connect(bucketCyl1.frame_a,arm1.frame_b)
                                           annotation (Line(
          points={{157.931,303.059},{156,303.059},{156,285.2},{97.44,285.2}},
          color={0,0,0},
          thickness=0.5));
    connect(bucket1.frame_b, bucket_link.frame_b)
                                              annotation (Line(
          points={{64.4,318.4},{64.4,318},{52,318},{52,328.5},{1,328.5}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1.frame_b,arm1.frame_a)             annotation (Line(
          points={{87.2,187.28},{87.2,187.28},{87.2,261.12},{83.72,261.12}},
          color={95,95,95},
          thickness=0.5));
      connect(arm1.frame_b2,bucket1.frame_a)           annotation (Line(
          points={{84.28,286.6},{86,286.6},{86,299.7},{85.6,299.7}},
          color={95,95,95},
          thickness=0.5));
      connect(boom1.frame_a,       carriage1.frame_b2) annotation (Line(
          points={{85.6,164.72},{86,164.72},{86,99.2},{87.6,99.2}},
          color={95,95,95},
          thickness=0.5));

    // Top level connections

      connect(swingMotor.flange_b,carriage1.axis)      annotation (Line(
          points={{-37.8,-40},{-54,-40},{-54,80},{78,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(carriage1.frame_b1, boomcylR.frame_a)    annotation (Line(
          points={{97.4,92},{136.36,92},{136.36,118.435}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(armCyl1.port_b, armLineB.port_b) annotation (Line(
          points={{161.96,233.84},{280,233.84},{280,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(armCyl1.port_a, armLineA.port_b) annotation (Line(
          points={{161.96,213.84},{264,213.84},{264,-54}},
          color={255,0,0},
          smooth=Smooth.None));
      connect(arm1.frame_b3, armCyl1.frame_b) annotation (Line(
          points={{97.72,261.4},{147,261.4},{147,237.36}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(boom1.frame_b2, boomcylR.frame_b)    annotation (Line(
          points={{105.6,164.96},{138.06,164.96},{138.06,141.565}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(boom1.frame_b3, armCyl1.frame_a) annotation (Line(
          points={{101.2,186.56},{149.38,186.56},{149.38,210.64}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(jA.port[2], boomcylR.port_a)    annotation (Line(
          points={{120,-40},{188,-40},{188,142.296},{148.94,142.296}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(fixed.frame_b, undercarriage.frame_a)
                                          annotation (Line(
        points={{54,26},{86,26},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(PID.u_s, timeTable.y) annotation (Line(
        points={{382,92},{411,92}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(boomcylR.cylLength, PID.u_m) annotation (Line(
        points={{115.62,129.148},{160,129.148},{160,112},{370,112},{370,104}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(armCyl1.cylLength, PID1.u_m) annotation (Line(
        points={{130,232.56},{108,232.56},{108,248},{528,248},{528,56},{424,56},
            {424,-34}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(swingValve.u[1], const1.y) annotation (Line(
        points={{100.9,-100.5},{100.9,-2},{363,-2}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID1.u_s, timeTable1.y) annotation (Line(
        points={{436,-46},{479,-46}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(PID2.u_s, timeTable2.y) annotation (Line(
        points={{428,-102},{473,-102}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(bucketCyl1.cylLength, PID2.u_m) annotation (Line(
        points={{140.966,323.189},{118,323.189},{118,340},{544,340},{544,-76},{
            416,-76},{416,-90}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(ramp.y, bucket1.load) annotation (Line(
        points={{31,294},{46,294},{46,311.6},{64,311.6}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(linearDistance1.frame_b, bucket1.bucket_tip) annotation (Line(
        points={{-20,218},{16,218},{16,248},{56,248},{56,342},{90,342},{90,
            331.32}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(linearDistance1.frame_a, undercarriage.frame_a) annotation (Line(
        points={{-20,212},{14,212},{14,214},{66,214},{66,36},{86,36},{86,38}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(bucket1.bucket_tip, absoluteAngles.frame_a) annotation (Line(
        points={{90,331.32},{94,331.32},{94,356},{204,356},{204,348}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
      connect(armValve.u[1], const2.y) annotation (Line(
          points={{286.9,-100.5},{323.45,-100.5},{323.45,-36},{355,-36}},
          color={0,0,127},
          smooth=Smooth.None));
    connect(const3.y, bucketValve.u[1]) annotation (Line(
        points={{403,-156},{376,-156},{376,-100.5},{344.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(const4.y, boomValve.u[1]) annotation (Line(
        points={{187,-78},{166.5,-78},{166.5,-100.5},{142.9,-100.5}},
        color={0,0,127},
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}},
              preserveAspectRatio=false),
                          graphics),
        experiment(
        StopTime=10,
        __Dymola_NumberOfIntervals=600,
        Tolerance=1e-008,
        __Dymola_Algorithm="Esdirk45a"),
        experimentSetupOutput(textual=true),
                  Diagram(graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-160,-200},{520,360}}), graphics={Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,255},
              fillColor={255,128,0},
              fillPattern=FillPattern.Solid), Text(
              extent={{-100,26},{100,-14}},
              lineColor={0,0,255},
              fillColor={170,213,255},
              fillPattern=FillPattern.Solid,
              textString="%name")}));
    end v009_cushionExperiments;
  end excavator;


  package components

    model undercarriage

    Modelica.Mechanics.MultiBody.Parts.FixedTranslation undercarriage(
      lengthDirection={1,0,0},
      r={0,1.70,0},
      shapeType="modelica://OpenHydraulics/Resources/Shapes/Excavator/base.stl",
      color={0,0,0}) "object model for the base of the excavator"
        annotation (Placement(transformation(
            origin={190,147},
            extent={{-11,-13},{11,13}},
            rotation=90)));

    Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation (
        Placement(transformation(rotation=0, extent={{160,20},{180,40}})));
    Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b annotation (
        Placement(transformation(rotation=0, extent={{180,220},{200,240}})));
    Modelica.Mechanics.MultiBody.Sensors.CutTorque cutTorque annotation (
        Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=270,
          origin={190,184})));
    Modelica.Blocks.Interfaces.RealOutput tipping_torque
      annotation (Placement(transformation(extent={{70,178},{50,198}})));
    equation
    connect(frame_a, undercarriage.frame_a)
                                   annotation (Line(points={{170,30},{170,30},{
            170,108},{190,108},{190,136}},           color={95,95,95}));
    connect(frame_b, cutTorque.frame_a) annotation (Line(
        points={{190,230},{190,194}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(cutTorque.frame_b, undercarriage.frame_b) annotation (Line(
        points={{190,174},{190,158}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(tipping_torque, cutTorque.torque[3]) annotation (Line(
        points={{60,188},{120,188},{120,192},{179.667,192}},
        color={0,0,127},
        smooth=Smooth.None));
    annotation (Diagram(coordinateSystem(extent={{70,30},{270,230}},
            preserveAspectRatio=false), graphics),                     Icon(
          coordinateSystem(extent={{70,30},{270,230}})));
    end undercarriage;

    model bucket

      Body_variableMass                       bBucket(
        I_31=I_31_Bucket,
        I_32=I_32_Bucket,
        r_CM=r_CM_Bucket,
        m=m_Bucket,
        I_11=I_11_Bucket,
        I_22=I_22_Bucket,
        I_33=I_33_Bucket,
        I_21=I_21_Bucket) "mass/inertia properties for arm"
        annotation (Placement(transformation(
            origin={490,296},
            extent={{-10,-10},{10,10}},
            rotation=90)));
      Modelica.Mechanics.MultiBody.Parts.FixedTranslation bucketLink(
        color={0,0,255},
        width=0.1,
        r=r_Bucket,
      r_shape=r_shape,
      lengthDirection=lengthDirection,
      widthDirection=widthDirection,
        shapeType=CAD_Bucket) annotation (Placement(transformation(
            origin={411,293.5},
            extent={{-14.5,-11},{14.5,11}},
            rotation=90)));
      Modelica.Mechanics.MultiBody.Joints.Revolute bucketRevolute(
        a(start=bucket_a_start, fixed=if (bucket_initType == Modelica.Mechanics.MultiBody.Types.Init.SteadyState
               or bucket_initType == Modelica.Mechanics.MultiBody.Types.Init.VelocityAcceleration
               or bucket_initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration)
               then true else false),
        phi(fixed=if (bucket_initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocity
               or bucket_initType == Modelica.Mechanics.MultiBody.Types.Init.Position
               or bucket_initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration)
               then true else false, start=bucket_phi_start),
        w(fixed=if (bucket_initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocity
               or bucket_initType == Modelica.Mechanics.MultiBody.Types.Init.SteadyState
               or bucket_initType == Modelica.Mechanics.MultiBody.Types.Init.Velocity
               or bucket_initType == Modelica.Mechanics.MultiBody.Types.Init.VelocityAcceleration
               or bucket_initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration)
               then true else false, start=bucket_w_start),
        useAxisFlange=true)
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=90,
            origin={411,228})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b annotation (Placement(
            transformation(rotation=0, extent={{312,270},{332,290}}),
            iconTransformation(extent={{312,270},{332,290}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation (Placement(
            transformation(rotation=0, extent={{418,160},{438,180}}),
            iconTransformation(extent={{418,160},{438,180}})));

    // the bucket parameters
      parameter Modelica.Mechanics.MultiBody.Types.Init bucket_initType=
          Modelica.Mechanics.MultiBody.Types.Init.Free
      "Type of initialization (defines usage of start values below)"
        annotation (Dialog(group="Bucket",tab="Initialization"));
      parameter SI.Angle
        bucket_phi_start=0.34906585039887
      "Initial value of rotation angle phi (fixed or guess value)"
        annotation (Evaluate=false, Dialog(group="Bucket",tab="Initialization"));
        //bucket_phi_start=0
      parameter SI.AngularVelocity
        bucket_w_start =                                     0
      "Initial value of relative angular velocity w = der(phi)"
        annotation (Evaluate=false, Dialog(group="Bucket",tab="Initialization"));
      parameter SI.AngularAcceleration
        bucket_a_start =                                          0
      "Initial value of relative angular acceleration a = der(w)"
        annotation (Evaluate=false, Dialog(group="Bucket",tab="Initialization"));

    // Carriage Intertia Parameters
        SI.Mass m_Bucket(min=0, start = 1);
        parameter SI.MomentOfInertia I_11_Bucket=0.1304;
        parameter SI.MomentOfInertia I_22_Bucket=2.11;
        parameter SI.MomentOfInertia I_33_Bucket=2.191;
        parameter SI.MomentOfInertia I_21_Bucket=-0.214;
        parameter SI.MomentOfInertia I_31_Bucket=0;
        parameter SI.MomentOfInertia I_32_Bucket=0;
        parameter Real r_CM_Bucket[:]={0.5,-0.6,0};
    // CAD Parameters
        parameter String CAD_Bucket = "modelica://OpenHydraulics/Resources/Shapes/Excavator/bucket.stl";
        parameter Real r_Bucket[:] = {0.52,
                                          0.07,0};
        parameter Real r_shape[:] = {0,0,0};
        parameter Real lengthDirection[:] = r_Bucket - r_shape;
        parameter Real widthDirection[:] = {0,1,0};
    // Metric parameters
       parameter Real bucketTip[:] = {0.12, -1.35, 0};

       Modelica.Blocks.Interfaces.RealInput load
        annotation (Placement(transformation(extent={{300,220},{340,260}})));
      Modelica.Mechanics.Rotational.Components.Fixed fixed
        annotation (Placement(transformation(extent={{354,202},{374,222}})));
      Modelica.Mechanics.Rotational.Components.Damper damper(d=100000)
        annotation (Placement(transformation(extent={{364,218},{384,238}})));
      Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation(r=
            bucketTip, shapeType="box") annotation (Placement(transformation(
            extent={{-10,-10},{10,10}},
            rotation=90,
            origin={450,322})));
    Modelica.Mechanics.MultiBody.Interfaces.Frame_b bucket_tip annotation (
        Placement(transformation(
          extent={{-16,-16},{16,16}},
          rotation=90,
          origin={450,356})));
    equation
      m_Bucket = load;
      connect(bBucket.frame_a, bucketLink.frame_a)
                                            annotation (Line(
          points={{490,286},{490,279},{411,279}},
          color={0,0,0},
          thickness=0.5));
      connect(frame_b, bucketLink.frame_b) annotation (Line(points={{322,280},{
              322,327},{411,327},{411,308}},
                                    color={0,0,0}));
      connect(frame_a, bucketRevolute.frame_a) annotation (Line(points={{428,170},
              {411,170},{411,218}},
                               color={95,95,95}));
      connect(bucketLink.frame_a, bucketRevolute.frame_b) annotation (Line(
          points={{411,279},{411,238}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
      connect(fixed.flange, damper.flange_a) annotation (Line(
          points={{364,212},{364,228}},
          color={0,0,0},
          smooth=Smooth.None));
      connect(damper.flange_b, bucketRevolute.axis) annotation (Line(
          points={{384,228},{401,228}},
          color={0,0,0},
          smooth=Smooth.None));
      connect(bucketRevolute.frame_b, fixedTranslation.frame_a) annotation (Line(
          points={{411,238},{411,262},{450,262},{450,312}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(fixedTranslation.frame_b, bucket_tip) annotation (Line(
        points={{450,332},{450,356}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{320,160},{520,360}},
              preserveAspectRatio=false), graphics),                       Icon(
            coordinateSystem(extent={{320,160},{520,360}}, preserveAspectRatio=
                false), graphics));
    end bucket;

    model arm

      Modelica.Mechanics.MultiBody.Joints.Revolute armRevolute(
        a(start=arm_a_start, fixed=if (arm_initType == Modelica.Mechanics.MultiBody.Types.Init.SteadyState
               or arm_initType == Modelica.Mechanics.MultiBody.Types.Init.VelocityAcceleration
               or arm_initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration)
               then true else false),
        phi(fixed=if (arm_initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocity
               or arm_initType == Modelica.Mechanics.MultiBody.Types.Init.Position
               or arm_initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration)
               then true else false, start=arm_phi_start),
        w(fixed=if (arm_initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocity
               or arm_initType == Modelica.Mechanics.MultiBody.Types.Init.SteadyState
               or arm_initType == Modelica.Mechanics.MultiBody.Types.Init.Velocity
               or arm_initType == Modelica.Mechanics.MultiBody.Types.Init.VelocityAcceleration
               or arm_initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration)
               then true else false, start=arm_w_start),
      useAxisFlange=true)
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=90,
            origin={327,180})));

      Modelica.Mechanics.MultiBody.Parts.Body bArm(
        r_CM=r_CM_arm,
        I_11=I_11_arm,
        I_22=I_22_arm,
        I_33=I_33_arm,
        I_21=I_21_arm,
        I_31=0,
        I_32=0,
        animation=true,
        m=m_arm) "mass/inertia properties for arm"
        annotation (Placement(transformation(
            origin={272,160},
            extent={{-10,10},{10,-10}},
            rotation=270)));
      Modelica.Mechanics.MultiBody.Parts.FixedTranslation armLink(
        r=r_arm,
        animation=true,
        color={0,180,0},
        shapeType=CAD_arm,
      r_shape=r_shape,
      lengthDirection=lengthDirection,
      widthDirection=widthDirection)
      "object model for the arm of the excavator"
        annotation (Placement(transformation(extent={{-21.5,-11},{21.5,11}},
              rotation=90,
            origin={325.5,232})));
      Modelica.Mechanics.MultiBody.Parts.FixedTranslation arm2Link(
        r_shape={0,0,0},
        animation=true,
        color={0,180,0},
        r=linkMount,
        shapeType="cylinder",
        width=0.1) "object model for the arm of the excavator"
        annotation (Placement(transformation(extent={{-21.5,-8.5},{21.5,8.5}},
              rotation=90,
            origin={265.5,239.5})));
      Modelica.Mechanics.MultiBody.Parts.FixedTranslation arm1Link(
        r_shape={0,0,0},
        animation=true,
        color={0,180,0},
        r=BucketCylMount,
        shapeType="cylinder",
        width=0.1) "object model for the arm of the excavator"
        annotation (Placement(transformation(extent={{-17.5,-8},{17.5,8}},
              rotation=90,
            origin={414.5,228})));
      Modelica.Mechanics.MultiBody.Parts.FixedTranslation boom4LinkY(r=armCylMountY)
        annotation (Placement(transformation(
            origin={366.5,169},
            extent={{-20,-8.5},{20,8.5}},
            rotation=270)));
      Modelica.Mechanics.MultiBody.Parts.FixedTranslation boom4LinkX(r=armCylMountX)
        annotation (Placement(transformation(
            origin={389.5,141},
            extent={{-17.5,-9},{17.5,9}},
            rotation=0)));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b annotation (Placement(
            transformation(rotation=0, extent={{426,300},{446,320}}),
            iconTransformation(extent={{426,300},{446,320}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b1 annotation (
          Placement(transformation(rotation=0, extent={{232,218},{252,238}}),
            iconTransformation(extent={{232,218},{252,238}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b2 annotation (
          Placement(transformation(rotation=0, extent={{332,310},{352,330}}),
            iconTransformation(extent={{332,310},{352,330}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation (Placement(
            transformation(rotation=0, extent={{328,128},{348,148}}),
            iconTransformation(extent={{328,128},{348,148}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b3 annotation (
          Placement(transformation(rotation=0, extent={{428,130},{448,150}}),
            iconTransformation(extent={{428,130},{448,150}})));

     // the arm parameters
      parameter Modelica.Mechanics.MultiBody.Types.Init arm_initType=
          Modelica.Mechanics.MultiBody.Types.Init.Free
      "Type of initialization (defines usage of start values below)"
        annotation (Dialog(group="Arm",tab="Initialization"));
      parameter SI.Angle
        arm_phi_start=-1.3962634015955
      "Initial value of rotation angle phi (fixed or guess value)"
        annotation (Evaluate=false, Dialog(group="Arm",tab="Initialization"));
        //arm_phi_start=0
      parameter SI.AngularVelocity
        arm_w_start = 0
      "Initial value of relative angular velocity w = der(phi)"
        annotation (Evaluate=false, Dialog(group="Arm",tab="Initialization"));
      parameter SI.AngularAcceleration
        arm_a_start =                                          0
      "Initial value of relative angular acceleration a = der(w)"
        annotation (Evaluate=false, Dialog(group="Arm",tab="Initialization"));

    // Carriage Intertia Parameters
        parameter SI.Mass m_arm=700.5;
        parameter SI.MomentOfInertia I_11_arm=130.4;
        parameter SI.MomentOfInertia I_22_arm=2110;
        parameter SI.MomentOfInertia I_33_arm=2191;
        parameter SI.MomentOfInertia I_21_arm=-214;
        parameter SI.MomentOfInertia I_31_arm=0;
        parameter SI.MomentOfInertia I_32_arm=0;
        parameter Real r_CM_arm[:]={1.176,0.296,0};

    // Position -Vectors from arm boom pivot
      // Arm cylinder mount
      parameter Real armCylMountx = -0.920;
      parameter Real armCylMounty = 0.217;
      parameter Real armCylMountX[:] = {armCylMountx,0,0};
      parameter Real armCylMountY[:] = {0,armCylMounty,0};
      // bucket Linkage mount
      parameter Real linkMountx = 2.97;
      //parameter Real linkMounty = 0.13;
      parameter Real linkMounty = 0;

      parameter Real linkMount[:] = {linkMountx,linkMounty,0};
      // Bucket Linkage cylinder mount
      parameter Real BucketCylMountx = 0.49;
      //parameter Real BucketCylMounty = 0.96;
      parameter Real BucketCylMount_theta = 0.472;
      parameter Real BucketCylMounty = BucketCylMountx * cos(BucketCylMount_theta)/sin(BucketCylMount_theta);
      parameter Real BucketCylMount[ :] = {BucketCylMountx,BucketCylMounty,0};
    // CAD parameters
        parameter String CAD_arm = "modelica://OpenHydraulics/Resources/Shapes/Excavator/arm.stl";
        //parameter String CAD_arm= "modelica://OpenHydraulics/Resources/Shapes/Excavator/boom_ex_375.dxf";
        parameter Real r_armx = 3.654;
        parameter Real r_arm[:] = {r_armx,0,0}; //Position of bucket pivot
        parameter Real r_shape[:] = {0,0,0};
        parameter Real lengthDirection[:] = r_arm - r_shape;
        parameter Real widthDirection[:] = {0,1,0};

    Modelica.Mechanics.Rotational.Components.Fixed fixed
      annotation (Placement(transformation(extent={{278,134},{298,154}})));
    Modelica.Mechanics.Rotational.Components.Damper damper(d=10000)
      annotation (Placement(transformation(extent={{288,150},{308,170}})));
    Modelica.Mechanics.Rotational.Sensors.SpeedSensor speedSensor annotation (
        Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=180,
          origin={294,192})));
    Modelica.Blocks.Interfaces.RealOutput arm_ang_vel
      annotation (Placement(transformation(extent={{240,182},{220,202}})));
    equation
      connect(boom4LinkX.frame_a, boom4LinkY.frame_b)       annotation (Line(
          points={{372,141},{366.5,141},{366.5,149}},
          color={0,0,0},
          thickness=0.5));
      connect(bArm.frame_a, armLink.frame_a)
                                         annotation (Line(
          points={{272,170},{272,210.5},{325.5,210.5}},
          color={0,0,0},
          thickness=0.5));
      connect(armLink.frame_a, arm1Link.frame_a)
                                         annotation (Line(
          points={{325.5,210.5},{346,210.5},{346,210},{368,210},{376,210},{396,
              210},{396,210.5},{414.5,210.5}},
          color={0,0,0},
          thickness=0.5));
      connect(arm2Link.frame_a, armLink.frame_a)
                                         annotation (Line(
          points={{265.5,218},{265.5,210.5},{325.5,210.5}},
          color={0,0,0},
          thickness=0.5));
      connect(armRevolute.frame_b, boom4LinkY.frame_a) annotation (Line(
          points={{327,190},{366,190},{366,189},{366.5,189}},
          color={95,95,95},
          thickness=0.5));
      connect(armRevolute.frame_b, armLink.frame_a)  annotation (Line(
          points={{327,190},{326,190},{326,210.5},{325.5,210.5}},
          color={95,95,95},
          thickness=0.5));
      connect(frame_b, arm1Link.frame_b) annotation (Line(points={{436,310},{
              394,310},{394,245.5},{414.5,245.5}},
                                    color={0,0,0}));
      connect(frame_b1, arm2Link.frame_b) annotation (Line(points={{242,228},{
            242,261},{265.5,261}},
                            color={0,0,0}));
      connect(frame_b2, armLink.frame_b) annotation (Line(points={{342,320},{
              336,320},{336,292},{325.5,292},{325.5,253.5}},
                                    color={95,95,95}));
      connect(frame_a, armRevolute.frame_a) annotation (Line(points={{338,138},
              {338,138},{327,138},{327,170}},
                                         color={95,95,95}));
      connect(frame_b3, boom4LinkX.frame_b) annotation (Line(points={{438,140},
              {438,140},{407,140},{407,141}},
                                         color={0,0,0}));
    connect(fixed.flange, damper.flange_a) annotation (Line(
        points={{288,144},{288,160}},
        color={0,0,0},
        smooth=Smooth.None));
    connect(damper.flange_b, armRevolute.axis) annotation (Line(
        points={{308,160},{308,180},{317,180}},
        color={0,0,0},
        smooth=Smooth.None));
    connect(speedSensor.flange, armRevolute.axis) annotation (Line(
        points={{304,192},{316,192},{316,180},{317,180}},
        color={0,0,0},
        smooth=Smooth.None));
    connect(speedSensor.w, arm_ang_vel) annotation (Line(
        points={{283,192},{230,192}},
        color={0,0,127},
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{240,130},{440,330}},
              preserveAspectRatio=false), graphics),                       Icon(
            coordinateSystem(extent={{240,130},{440,330}}, preserveAspectRatio=false),
                        graphics));
    end arm;

    model boom

      Modelica.Mechanics.MultiBody.Joints.Revolute boomRevolute(
        a(start=boom_a_start, fixed=if (boom_initType == Modelica.Mechanics.MultiBody.Types.Init.SteadyState
               or boom_initType == Modelica.Mechanics.MultiBody.Types.Init.VelocityAcceleration
               or boom_initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration)
               then true else false),
        phi(fixed=if (boom_initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocity
               or boom_initType == Modelica.Mechanics.MultiBody.Types.Init.Position
               or boom_initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration)
               then true else false, start=boom_phi_start),
        w(fixed=if (boom_initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocity
               or boom_initType == Modelica.Mechanics.MultiBody.Types.Init.SteadyState
               or boom_initType == Modelica.Mechanics.MultiBody.Types.Init.Velocity
               or boom_initType == Modelica.Mechanics.MultiBody.Types.Init.VelocityAcceleration
               or boom_initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration)
               then true else false, start=boom_w_start),
      useAxisFlange=true)
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=90,
            origin={167,212})));

      // boom2LinkLeft is the vector from the carriage pivot of the boom to the left hydrualic cylinderder mount point.
      Modelica.Mechanics.MultiBody.Parts.FixedTranslation boom2LinkLeft(r=boomLCylMount)
      "from boom-lift pivot to lifting cylinder rod side pivot"
        annotation (Placement(transformation(
            origin={105,195.5},
            extent={{-28,-8.5},{28,8.5}},
            rotation=270)));
      Modelica.Mechanics.MultiBody.Parts.FixedTranslation boom3Link(r=armCylMount)
      "from boom-lift pivot to arm cylinder cap side pivot"
        annotation (Placement(transformation(extent={{-23.5,-7},{23.5,7}},
              rotation=90,
            origin={227.5,300})));
      Modelica.Mechanics.MultiBody.Parts.Body bBoom(
        r_CM=r_CM_Boom,
        I_11=I_11_Boom,
        I_22=I_22_Boom,
        I_33=I_33_Boom,
        I_21=I_21_Boom,
        I_31=I_31_Boom,
        I_32=I_32_Boom,
        m=m_Boom) "mass/inertia properties for boom"
        annotation (Placement(transformation(extent={{182,246},{202,266}},
              rotation=0)));
      Modelica.Mechanics.MultiBody.Parts.FixedTranslation boom(
        r=r_boom,
        color={155,0,0},
      shapeType=CAD_boom,
      r_shape=r_shape,
      lengthDirection=lengthDirection,
      widthDirection=widthDirection)
      "object model for the boom of the excavator"
        annotation (Placement(transformation(extent={{-29.5,-15},{29.5,15}},
              rotation=90,
            origin={168.5,286})));

      // boom2LinkRight is the vector from the carriage pivot of the boom to the right hydrualic cylinderder mount point.
      Modelica.Mechanics.MultiBody.Parts.FixedTranslation boom2LinkRight(r=boomRCylMount)
      "from boom-lift pivot to lifting cylinder rod side pivot"
        annotation (Placement(transformation(extent={{-22.5,-9.5},{22.5,9.5}},
              rotation=270,
            origin={233.5,186.5})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b annotation (Placement(
            transformation(rotation=0, extent={{166,344},{186,364}}),
            iconTransformation(extent={{166,344},{186,364}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b1 annotation (
          Placement(transformation(rotation=0, extent={{60,158},{80,178}}),
            iconTransformation(extent={{60,158},{80,178}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b2 annotation (
          Placement(transformation(rotation=0, extent={{258,158},{278,178}}),
            iconTransformation(extent={{258,158},{278,178}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b3 annotation (
          Placement(transformation(rotation=0, extent={{236,338},{256,358}}),
            iconTransformation(extent={{236,338},{256,358}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation (Placement(
            transformation(rotation=0, extent={{158,156},{178,176}}),
            iconTransformation(extent={{158,156},{178,176}})));

     // the boom parameters
      parameter Modelica.Mechanics.MultiBody.Types.Init boom_initType=
          Modelica.Mechanics.MultiBody.Types.Init.Free
      "Type of initialization (defines usage of start values below)"
        annotation (Dialog(group="Boom",tab="Initialization"));
      parameter SI.Angle
        boom_phi_start=0.87266462599716
      "Initial value of rotation angle phi (fixed or guess value)"
        annotation (Evaluate=false, Dialog(group="Boom",tab="Initialization"));
        //boom_phi_start=0
      parameter SI.AngularVelocity
        boom_w_start =                                     0
      "Initial value of relative angular velocity w = der(phi)"
        annotation (Evaluate=false, Dialog(group="Boom",tab="Initialization"));
      parameter SI.AngularAcceleration
        boom_a_start =                                          0
      "Initial value of relative angular acceleration a = der(w)"
        annotation (Evaluate=false, Dialog(group="Boom",tab="Initialization"));

    // Carriage Intertia Parameters
        parameter SI.Mass m_Boom=1307;
        parameter SI.MomentOfInertia I_11_Boom=390.5;
        parameter SI.MomentOfInertia I_22_Boom=7803;
        parameter SI.MomentOfInertia I_33_Boom=8060;
        parameter SI.MomentOfInertia I_21_Boom=-150.1;
        parameter SI.MomentOfInertia I_31_Boom=0;
        parameter SI.MomentOfInertia I_32_Boom=0;
        parameter Real r_CM_Boom[:]={3.44,0.65,0};
    // Position - Vectors from the boom carriage pivot.
      //Left cylinder mount
      parameter Real boomLCylMount[:]={2.85,1.18,-0.3};
      //Right cylinder mount
      parameter Real boomRCylMount[:] = {2.85,1.18,0.3};
      //Arm cylinder mount
      parameter Real armCylMount[:] = {4.22,1.33,0};
    // CAD Parameters
      parameter String CAD_boom = "modelica://OpenHydraulics/Resources/Shapes/Excavator/boom.stl";
      parameter Real r_boom[:] = {7.11,0,0};
      parameter Real r_shape[:] = {0,0,0};
      parameter Real lengthDirection[:] = r_boom - r_shape;
      parameter Real widthDirection[:] = {0,1,0};

    Modelica.Mechanics.MultiBody.Sensors.RelativeAngles BoomAngle
      annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=90,
          origin={120,284})));
    Modelica.Mechanics.Rotational.Components.Damper damper(d=100000)
      annotation (Placement(transformation(extent={{132,202},{152,222}})));
    Modelica.Mechanics.Rotational.Components.Fixed fixed
      annotation (Placement(transformation(extent={{122,186},{142,206}})));
    equation
      connect(bBoom.frame_a,boom. frame_a) annotation (Line(
          points={{182,256},{172,256},{172,256.5},{168.5,256.5}},
          color={0,0,0},
          thickness=0.5));
      connect(boomRevolute.frame_b,boom. frame_a) annotation (Line(
          points={{167,222},{168,222},{168,256.5},{168.5,256.5}},
          color={95,95,95},
          thickness=0.5));
      connect(boom2LinkLeft.frame_a, boomRevolute.frame_b) annotation (Line(
          points={{105,223.5},{136,223.5},{136,222},{167,222}},
          color={95,95,95},
          thickness=0.5));
      connect(boom3Link.frame_a, boomRevolute.frame_b) annotation (Line(
          points={{227.5,276.5},{227.5,266},{222,266},{222,256},{222,232},{168,
            232},{168,222},{167,222}},
          color={95,95,95},
          thickness=0.5));
      connect(boom2LinkRight.frame_a, boomRevolute.frame_b)  annotation (Line(
          points={{233.5,209},{233.5,224},{167,224},{167,222}},
          color={95,95,95},
          thickness=0.5));
      connect(frame_b, boom.frame_b) annotation (Line(points={{176,354},{156,
              354},{156,350},{156,335.5},{168.5,335.5},{168.5,315.5}},
                               color={95,95,95}));
      connect(frame_b1, boom2LinkLeft.frame_b)
        annotation (Line(points={{70,168},{105,168},{105,167.5}},
                                                                color={0,0,0}));
      connect(frame_b2, boom2LinkRight.frame_b) annotation (Line(points={{268,168},
            {268,162},{233.5,162},{233.5,164}},color={0,0,0}));
      connect(frame_b3, boom3Link.frame_b) annotation (Line(points={{246,348},{
              228,348},{228,323.5},{227.5,323.5}},
                                    color={0,0,0}));
      connect(frame_a, boomRevolute.frame_a) annotation (Line(points={{168,166},
            {168,202},{167,202}},
                               color={95,95,95}));
      connect(frame_b, frame_b) annotation (Line(
          points={{176,354},{176,354}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(BoomAngle.frame_a, boomRevolute.frame_a) annotation (Line(
        points={{120,274},{120,178},{168,178},{168,202},{167,202}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(BoomAngle.frame_b, boom.frame_b) annotation (Line(
        points={{120,294},{120,322},{168.5,322},{168.5,315.5}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(boomRevolute.axis, damper.flange_b) annotation (Line(
        points={{157,212},{152,212}},
        color={0,0,0},
        smooth=Smooth.None));
    connect(damper.flange_a, fixed.flange) annotation (Line(
        points={{132,212},{132,196}},
        color={0,0,0},
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{70,160},{270,360}},
              preserveAspectRatio=false), graphics),                      Icon(
            coordinateSystem(extent={{70,160},{270,360}}, preserveAspectRatio=
                false), graphics));
    end boom;

    model bucketCyl

      Modelica.Mechanics.MultiBody.Forces.LineForceWithMass cylBucket(
          lineShapeType="cylinder", lineShapeWidth=0.095)
      "hydraulic force in cylinder 2 (between boom and arm)"
        annotation (Placement(transformation(
            origin={367.5,204},
            extent={{15.5,-15},{-15.5,15}},
            rotation=180)));
      DoubleActingCylinder                                     bucketCyl(
        closedLength=2.30,
        boreDiameter=0.095,
        rodDiameter=0.05,
        s_init=bucket_s_init,
        pistonMass=5,
        initType=Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration,
        fixRodPressure=true,
        strokeLength=0.9)
        annotation (Placement(transformation(extent={{358,108},{378,128}},
              rotation=0)));

      parameter Modelica.SIunits.Length bucket_s_init=0.77
      "Initial position of bucket cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

      parameter SI.Length closedLength = 2.30;
      parameter SI.Diameter boreDiameter = 0.095;
      parameter SI.Diameter rodDiameter=0.05;
      parameter SI.Mass pistonMass = 5;
      parameter SI.Length strokeLength = 0.9;
      parameter SI.Length maxLength = closedLength + strokeLength;

      OpenHydraulics.Interfaces.FluidPort port_a annotation (Placement(
            transformation(rotation=0, extent={{469.5,-18.5},{498.5,18.5}}),
            iconTransformation(extent={{469.5,-18.5},{498.5,18.5}})));
      OpenHydraulics.Interfaces.FluidPort port_b annotation (Placement(
            transformation(rotation=0, extent={{467.5,261.5},{496.5,298.5}}),
            iconTransformation(extent={{467.5,261.5},{496.5,298.5}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation (
          Placement(transformation(rotation=0, extent={{339.5,-44.5},{368.5,
                -7.5}}), iconTransformation(extent={{339.5,-44.5},{368.5,-7.5}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b annotation (
          Placement(transformation(rotation=0, extent={{321.5,295.5},{350.5,
                332.5}}), iconTransformation(extent={{321.5,295.5},{350.5,332.5}})));
    Modelica.Mechanics.MultiBody.Sensors.Distance distance annotation (
        Placement(transformation(
          extent={{-10,10},{10,-10}},
          rotation=90,
          origin={300,240})));
    Modelica.Blocks.Interfaces.RealOutput cylLength
      annotation (Placement(transformation(extent={{200,230},{180,250}})));
    OpenHydraulics.Components.Sensors.PressureSensor pressureSensor(absolute=
          true)
      annotation (Placement(transformation(extent={{436,30},{456,50}})));
    Modelica.Blocks.Interfaces.RealOutput cylPressure_a
      annotation (Placement(transformation(extent={{490,34},{510,54}})));
    OpenHydraulics.Components.Sensors.PressureSensor pressureSensor1(
                                                                    absolute=
          true)
      annotation (Placement(transformation(extent={{436,170},{456,190}})));
    Modelica.Blocks.Interfaces.RealOutput cylPressure_b
      annotation (Placement(transformation(extent={{490,174},{510,194}})));
    Modelica.Blocks.Interfaces.RealOutput cylLength_unit
      annotation (Placement(transformation(extent={{202,118},{182,138}})));
    equation
      connect(bucketCyl.flange_a, cylBucket.flange_a) annotation (Line(
          points={{358,118},{340,118},{340,176},{358.2,176},{358.2,189}},
          color={0,127,0},
          smooth=Smooth.None));
      connect(bucketCyl.flange_b, cylBucket.flange_b) annotation (Line(
          points={{378,118},{394,118},{394,176},{376.8,176},{376.8,189}},
          color={0,127,0},
          smooth=Smooth.None));
      connect(port_a, bucketCyl.port_a) annotation (Line(points={{484,0},{484,
              12},{360,12},{360,110}},   color={255,0,0}));
      connect(port_b, bucketCyl.port_b) annotation (Line(points={{482,280},{482,
              88},{376,88},{376,110}},   color={255,0,0}));
      connect(frame_a, cylBucket.frame_a) annotation (Line(points={{354,-26},{
              306,-26},{306,204},{352,204}}, color={0,0,0}));
      connect(frame_b, cylBucket.frame_b) annotation (Line(points={{336,314},{
              336,262},{424,262},{424,204},{383,204}},
                                   color={0,0,0}));
    connect(distance.frame_b, cylBucket.frame_b) annotation (Line(
        points={{300,250},{320,250},{320,262},{424,262},{424,204},{383,204}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(distance.frame_a, cylBucket.frame_a) annotation (Line(
        points={{300,230},{304,230},{304,204},{352,204}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(cylLength, distance.distance) annotation (Line(
        points={{190,240},{289,240}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(pressureSensor.y, cylPressure_a)
                                           annotation (Line(
        points={{453,40},{474,40},{474,44},{500,44}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(pressureSensor1.y, cylPressure_b)
                                           annotation (Line(
        points={{453,180},{474,180},{474,184},{500,184}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(pressureSensor1.port_a, bucketCyl.port_b) annotation (Line(
        points={{446,170},{446,140},{482,140},{482,88},{376,88},{376,110}},
        color={255,0,0},
        smooth=Smooth.None));
    connect(pressureSensor.port_a, bucketCyl.port_a) annotation (Line(
        points={{446,30},{446,12},{360,12},{360,110}},
        color={255,0,0},
        smooth=Smooth.None));
    connect(bucketCyl.strokeLength_unit, cylLength_unit) annotation (Line(
        points={{357,126.6},{277.5,126.6},{277.5,128},{192,128}},
        color={0,0,127},
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{200,-40},{490,330}},
              preserveAspectRatio=false), graphics),                       Icon(
            coordinateSystem(extent={{200,-40},{490,330}}, preserveAspectRatio=
                false), graphics));
    end bucketCyl;

    model armCyl

      Modelica.Mechanics.MultiBody.Forces.LineForceWithMass cylArm(
          lineShapeType="cylinder", lineShapeWidth=0.095)
      "hydraulic force in cylinder 2 (between boom and arm)"
        annotation (Placement(transformation(extent={{223,213},{243,193}},
              rotation=0)));
      DoubleActingCylinder                                     armCyl(
        strokeLength=strokeLength,
        closedLength=closedLength,
        boreDiameter=boreDiameter,
        rodDiameter=rodDiameter,
        pistonMass=pistonMass,
        s_init=arm_s_init,
        initType=Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration,
        fixHeadPressure=true,
      useCushionHead=true)
        annotation (Placement(transformation(extent={{226,152},{246,172}},
              rotation=0)));

      parameter Modelica.SIunits.Length arm_s_init=0.71
      "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter SI.Length closedLength = 2.26;
      parameter SI.Diameter boreDiameter = 0.145;
      parameter SI.Diameter rodDiameter=0.05;
      parameter SI.Mass pistonMass = 10;
      parameter SI.Length strokeLength = 1.64;
      parameter SI.Length maxLength = closedLength + strokeLength;

      OpenHydraulics.Interfaces.FluidPort port_a annotation (Placement(
            transformation(rotation=0, extent={{338,-9.5},{358,25.5}}),
            iconTransformation(extent={{338,-9.5},{358,25.5}})));
      OpenHydraulics.Interfaces.FluidPort port_b annotation (Placement(
            transformation(rotation=0, extent={{338,240.5},{358,275.5}}),
            iconTransformation(extent={{338,240.5},{358,275.5}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation (
          Placement(transformation(rotation=0, extent={{264,-49.5},{284,-14.5}}),
            iconTransformation(extent={{264,-49.5},{284,-14.5}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b annotation (
          Placement(transformation(rotation=0, extent={{250,284.5},{270,319.5}}),
            iconTransformation(extent={{250,284.5},{270,319.5}})));
    Modelica.Mechanics.MultiBody.Sensors.Distance distance annotation (
        Placement(transformation(
          extent={{-10,10},{10,-10}},
          rotation=90,
          origin={202,242})));
    Modelica.Blocks.Interfaces.RealOutput cylLength
      annotation (Placement(transformation(extent={{170,232},{150,252}})));
    OpenHydraulics.Components.Sensors.PressureSensor pressureSensor(absolute=
          true)
      annotation (Placement(transformation(extent={{306,86},{326,106}})));
    Modelica.Blocks.Interfaces.RealOutput cylPressure_a
      annotation (Placement(transformation(extent={{360,90},{380,110}})));
    OpenHydraulics.Components.Sensors.PressureSensor pressureSensor1(
                                                                    absolute=
          true)
      annotation (Placement(transformation(extent={{306,168},{326,188}})));
    Modelica.Blocks.Interfaces.RealOutput cylPressure_b
      annotation (Placement(transformation(extent={{360,172},{380,192}})));
    Modelica.Blocks.Interfaces.RealOutput cylLength_unit
      annotation (Placement(transformation(extent={{162,154},{142,174}})));
    equation
      connect(cylArm.flange_a, armCyl.flange_a) annotation (Line(
          points={{227,193},{227,192},{208,192},{208,162},{226,162}},
          color={0,127,0},
          smooth=Smooth.None));
      connect(armCyl.flange_b, cylArm.flange_b) annotation (Line(
          points={{246,162},{268,162},{268,192},{239,192},{239,193}},
          color={0,127,0},
          smooth=Smooth.None));
      connect(port_a, armCyl.port_a) annotation (Line(points={{348,8},{348,130},
              {228,130},{228,154}}, color={255,0,0}));
      connect(port_b, armCyl.port_b) annotation (Line(points={{348,258},{348,
              144},{244,144},{244,154}}, color={255,0,0}));
      connect(frame_a, cylArm.frame_a) annotation (Line(points={{274,-32},{182,
              -32},{182,203},{223,203}}, color={0,0,0}));
      connect(frame_b, cylArm.frame_b) annotation (Line(points={{260,302},{304,
              302},{304,203},{243,203}}, color={0,0,0}));
    connect(distance.frame_a, cylArm.frame_a) annotation (Line(
        points={{202,232},{202,203},{223,203}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(distance.frame_b, cylArm.frame_b) annotation (Line(
        points={{202,252},{304,252},{304,203},{243,203}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(distance.distance, cylLength) annotation (Line(
        points={{191,242},{160,242}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(pressureSensor.y, cylPressure_a)
                                           annotation (Line(
        points={{323,96},{344,96},{344,100},{370,100}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(pressureSensor.port_a, armCyl.port_a) annotation (Line(
        points={{316,86},{316,58},{348,58},{348,130},{228,130},{228,154}},
        color={255,0,0},
        smooth=Smooth.None));
    connect(pressureSensor1.y, cylPressure_b)
                                           annotation (Line(
        points={{323,178},{344,178},{344,182},{370,182}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(pressureSensor1.port_a, armCyl.port_b) annotation (Line(
        points={{316,168},{348,168},{348,144},{244,144},{244,154}},
        color={255,0,0},
        smooth=Smooth.None));
      connect(armCyl.strokeLength_unit, cylLength_unit) annotation (Line(
          points={{225,170.6},{192.5,170.6},{192.5,164},{152,164}},
          color={0,0,127},
          smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{160,-40},{360,310}},
              preserveAspectRatio=false), graphics),                       Icon(
            coordinateSystem(extent={{160,-40},{360,310}}, preserveAspectRatio=
                false), graphics));
    end armCyl;

    model carriage

      Modelica.Mechanics.MultiBody.Joints.Revolute swingRevolute(
        n={0,1,0},
        useAxisFlange=true,
        a(start=swing_a_start, fixed=if ((swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.SteadyState
               or (swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.VelocityAcceleration
               or (swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration)
               then true else false),
        phi(fixed=if ((swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocity
               or (swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.Position
               or (swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration)
               then true else false, start=swing_phi_start),
        w(fixed=if ((swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocity
               or (swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.SteadyState
               or (swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.Velocity
               or (swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.VelocityAcceleration
               or (swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration)
               then true else false, start=swing_w_start))
      "revolute joint (plus motor) representing the swing function"
        annotation (Placement(transformation(
            origin={154,118},
            extent={{-10,-10},{10,10}},
            rotation=90)));
      Modelica.Mechanics.MultiBody.Parts.FixedTranslation carriage(
        color={255,255,0},
      r=r_carriage,
      shapeType=CAD_carriage,
        r_shape=r_shape,
        lengthDirection=lengthDirection,
        widthDirection=widthDirection)
      "object model for the carriage of the excavator"
        annotation (Placement(transformation(
            origin={154.5,221},
            extent={{-26,-13.5},{26,13.5}},
            rotation=90)));

      Modelica.Mechanics.MultiBody.Parts.FixedTranslation boom1LinkRight(r=
            boomLinkRight)
      "from swing motor to lifting cylinder cap side pivot"
        annotation (Placement(transformation(
            origin={224.5,167},
            extent={{-20,-8.5},{20,8.5}},
            rotation=90)));
      Modelica.Mechanics.MultiBody.Parts.FixedTranslation boom1LinkLeft(r=
            boomLinkLeft) "from swing motor to lifting cylinder cap side pivot"
        annotation (Placement(transformation(
            origin={86.5,169},
            extent={{-20,-8.5},{20,8.5}},
            rotation=90)));
      Modelica.Mechanics.MultiBody.Parts.Body bCarriage(
        m=m_carriage,
        I_11=I_11_carriage,
        I_22=I_22_carriage,
        I_33=I_33_carriage,
        I_21=I_21_carriage,
        I_31=I_31_carriage,
        I_32=I_32_carriage,
        r_CM=r_CM_carriage) "mass/inertia properties for carriage"
        annotation (Placement(transformation(extent={{179,185},{199,205}},
              rotation=0)));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b annotation (Placement(
            transformation(rotation=0, extent={{58,192},{78,212}}),
            iconTransformation(extent={{58,192},{78,212}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b1 annotation (
          Placement(transformation(rotation=0, extent={{244,190},{264,210}}),
            iconTransformation(extent={{244,190},{264,210}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b2 annotation (
          Placement(transformation(rotation=0, extent={{146,262},{166,282}}),
            iconTransformation(extent={{146,262},{166,282}})));
      Modelica.Mechanics.Rotational.Interfaces.Flange_a axis annotation (Placement(
            transformation(rotation=0, extent={{50,70},{70,90}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation (Placement(
            transformation(rotation=0, extent={{150,70},{170,90}})));

    // the swing parameters
      parameter Modelica.Mechanics.MultiBody.Types.Init swing_initType=
          Modelica.Mechanics.MultiBody.Types.Init.PositionVelocity
      "Type of initialization (defines usage of start values below)"
        annotation (Dialog(group="Swing",tab="Initialization"));
          //Modelica.Mechanics.MultiBody.Types.Init.Free
      parameter SI.Angle
        swing_phi_start=0
      "Initial value of rotation angle phi (fixed or guess value)"
        annotation (Evaluate=false, Dialog(group="Swing",tab="Initialization"));
      parameter SI.AngularVelocity
        swing_w_start =                                     0
      "Initial value of relative angular velocity w = der(phi)"
        annotation (Evaluate=false, Dialog(group="Swing",tab="Initialization"));
      parameter SI.AngularAcceleration
        swing_a_start =                                          0
      "Initial value of relative angular acceleration a = der(w)"
        annotation (Evaluate=false, Dialog(group="Swing",tab="Initialization"));

    // Carriage Intertia Parameters
        parameter SI.Mass m_carriage=11312.161;
        parameter SI.MomentOfInertia I_11_carriage=9667;
        parameter SI.MomentOfInertia I_22_carriage=34850;
        parameter SI.MomentOfInertia I_33_carriage=30686;
        parameter SI.MomentOfInertia I_21_carriage=-3756;
        parameter SI.MomentOfInertia I_31_carriage=0;
        parameter SI.MomentOfInertia I_32_carriage=0;
        parameter Real r_CM_carriage[:]={-2.555,1.169,0};

    // Position Parameters
        parameter Real boomLinkRight[:] = {0.655,0.703,0.296};// "from swing motor to left boom cylinder cap side pivot"
        parameter Real boomLinkLeft[:] = {0.655,0.703,-0.296};// "from swing motor to right boom cylinder cap side pivot"
    // CAD parameters
        parameter String CAD_carriage = "modelica://OpenHydraulics/Resources/Shapes/Excavator/carriage.stl";
        parameter Real r_carriage[:] = {-0.164,1.428,0};
        parameter Real r_shape[:] = {0,0,0};
        parameter Real lengthDirection[:] = {1,0,0};
        parameter Real widthDirection[:] = {0,1,0};

    equation
      connect(boom1LinkRight.frame_a, swingRevolute.frame_b)    annotation (Line(
          points={{224.5,147},{218,147},{218,142},{154,142},{154,128}},
          color={0,0,0},
          thickness=0.5));
      connect(carriage.frame_a,swingRevolute. frame_b)
                                                  annotation (Line(
          points={{154.5,195},{154,195},{154,184},{154,128}},
          color={0,0,0},
          thickness=0.5));
      connect(bCarriage.frame_a,carriage. frame_a) annotation (Line(
          points={{179,195},{166,195},{154.5,195}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1LinkLeft.frame_a, swingRevolute.frame_b)
                                              annotation (Line(
          points={{86.5,149},{95,149},{95,142},{154,142},{154,128}},
          color={0,0,0},
          thickness=0.5));
      connect(frame_b, boom1LinkLeft.frame_b) annotation (Line(points={{68,202},
              {68,201},{86.5,201},{86.5,189}},
                                         color={0,0,0}));
      connect(frame_b1, boom1LinkRight.frame_b) annotation (Line(points={{254,200},
              {224,200},{224,187},{224.5,187}},color={0,0,0}));
      connect(frame_b2, carriage.frame_b) annotation (Line(points={{156,272},{
              156,252},{154.5,252},{154.5,247}},
                                     color={95,95,95}));
      connect(axis, swingRevolute.axis) annotation (Line(points={{60,80},{114,
              80},{114,118},{144,118}},
                             color={0,0,0}));
      connect(frame_a, swingRevolute.frame_a) annotation (Line(points={{160,80},
              {154,80},{154,108}},    color={0,0,0}));
      annotation (Diagram(coordinateSystem(extent={{60,80},{260,280}},
              preserveAspectRatio=false), graphics),                     Icon(
            coordinateSystem(extent={{60,80},{260,280}}, preserveAspectRatio=
                false), graphics));
    end carriage;

    model boomCylR

      Modelica.Mechanics.MultiBody.Forces.LineForceWithMass cylBoomRight(
          lineShapeWidth=0.095)
      "hydraulic force in cylinder 1 (between carriage and boom)"
        annotation (Placement(transformation(
            origin={185,75},
            extent={{-10,10},{10,-10}},
            rotation=90)));
      DoubleActingCylinder                                     boomCylRight(
        closedLength= closedLength,
        boreDiameter=boreDiameter,
        rodDiameter=rodDiameter,
        pistonMass=pistonMass,
        s_init=boom_s_init,
        initType=Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration,
        fixRodPressure=true,
        strokeLength=strokeLength)
        annotation (Placement(transformation(extent={{10,10},{-10,-10}},
              rotation=270,
            origin={236,70})));

      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      OpenHydraulics.Interfaces.FluidPort port_a annotation (Placement(
            transformation(rotation=0, extent={{282,164.5},{302,187.5}}),
            iconTransformation(extent={{282,164.5},{302,187.5}})));
      OpenHydraulics.Interfaces.FluidPort port_b annotation (Placement(
            transformation(rotation=0, extent={{294,-37.5},{314,-14.5}}),
            iconTransformation(extent={{294,-37.5},{314,-14.5}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation (
          Placement(transformation(rotation=0, extent={{208,-31.5},{228,-8.5}}),
            iconTransformation(extent={{208,-31.5},{228,-8.5}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b annotation (
          Placement(transformation(rotation=0, extent={{218,158.5},{238,181.5}}),
            iconTransformation(extent={{218,158.5},{238,181.5}})));

    // Cylinder Parameters
        parameter SI.Length closedLength = 2.48;
        parameter SI.Diameter boreDiameter = 0.135;
        parameter SI.Diameter rodDiameter=0.05;
        parameter SI.Mass pistonMass = 10;
        parameter SI.Length strokeLength = 1.25;
        //parameter SI.Length maxLength = closedLength + strokeLength;

    Modelica.Mechanics.MultiBody.Sensors.Distance distance annotation (
        Placement(transformation(
          extent={{-10,10},{10,-10}},
          rotation=90,
          origin={132,68})));
    Modelica.Blocks.Interfaces.RealOutput cylLength
      annotation (Placement(transformation(extent={{106,58},{86,78}})));
    OpenHydraulics.Components.Sensors.PressureSensor pressureSensor(absolute=
          true)
      annotation (Placement(transformation(extent={{256,78},{276,98}})));
    OpenHydraulics.Components.Sensors.PressureSensor pressureSensor1
      annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=180,
          origin={270,52})));
    Modelica.Blocks.Interfaces.RealOutput cylPressure_b
      annotation (Placement(transformation(extent={{310,82},{330,102}})));
    Modelica.Mechanics.Translational.Sensors.ForceSensor forceSensor
      annotation (Placement(transformation(extent={{224,82},{204,102}})));
    Modelica.Blocks.Interfaces.RealOutput maxLength = closedLength + strokeLength
      annotation (Placement(transformation(extent={{110,140},{90,160}})));
    Modelica.Blocks.Interfaces.RealOutput cylPressure_a
      annotation (Placement(transformation(extent={{310,16},{330,36}})));
    Modelica.Blocks.Interfaces.RealOutput cylLength_unit
      annotation (Placement(transformation(extent={{110,-4},{90,16}})));
    equation
      connect(boomCylRight.flange_a, cylBoomRight.flange_a) annotation (Line(
          points={{236,60},{236,38},{198,38},{198,69},{195,69}},
          color={0,127,0},
          smooth=Smooth.None));
      connect(port_a, boomCylRight.port_a) annotation (Line(points={{292,176},{
            292,62},{244,62}},   color={255,0,0}));
      connect(port_b, boomCylRight.port_b) annotation (Line(points={{304,-26},{
            304,46},{304,78},{244,78}},                              color={255,
              0,0}));
      connect(frame_a, cylBoomRight.frame_a) annotation (Line(points={{218,-20},
            {186,-20},{186,65},{185,65}},   color={0,0,0}));
      connect(frame_b, cylBoomRight.frame_b) annotation (Line(points={{228,170},
            {228,152},{214,152},{210,152},{185,152},{185,85}},   color={0,0,0}));
      connect(port_a, port_a) annotation (Line(
          points={{292,176},{292,176},{292,176}},
          color={255,0,0},
          smooth=Smooth.None));
    connect(distance.distance, cylLength)   annotation (Line(
        points={{121,68},{96,68}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(distance.frame_b, cylBoomRight.frame_b) annotation (Line(
        points={{132,78},{132,100},{185,100},{185,85}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(distance.frame_a, cylBoomRight.frame_a) annotation (Line(
        points={{132,58},{132,44},{186,44},{186,65},{185,65}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(port_b, pressureSensor.port_a) annotation (Line(
        points={{304,-26},{304,78},{266,78}},
        color={255,0,0},
        smooth=Smooth.None));
    connect(port_a, pressureSensor1.port_a) annotation (Line(
        points={{292,176},{292,62},{270,62}},
        color={255,0,0},
        smooth=Smooth.None));
    connect(pressureSensor.y, cylPressure_b)
                                           annotation (Line(
        points={{273,88},{294,88},{294,92},{320,92}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(forceSensor.flange_a, boomCylRight.flange_b) annotation (Line(
        points={{224,92},{236,92},{236,80}},
        color={0,127,0},
        smooth=Smooth.None));
    connect(forceSensor.flange_b, cylBoomRight.flange_b) annotation (Line(
        points={{204,92},{195,92},{195,81}},
        color={0,127,0},
        smooth=Smooth.None));
    connect(pressureSensor1.y, cylPressure_a) annotation (Line(
        points={{263,52},{264,52},{264,26},{320,26}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(boomCylRight.strokeLength_unit, cylLength_unit) annotation (Line(
        points={{227.4,59},{227.4,6},{100,6}},
        color={0,0,127},
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{110,-40},{310,190}},
              preserveAspectRatio=false), graphics), Icon(coordinateSystem(
              extent={{110,-40},{310,190}}, preserveAspectRatio=false),
            graphics));
    end boomCylR;

    model boomCylL

      Modelica.Mechanics.MultiBody.Forces.LineForceWithMass cylBoomLeft(
          lineShapeWidth=0.095)
      "hydraulic force in cylinder 1 (between carriage and boom)"
        annotation (Placement(transformation(
            origin={109,117},
            extent={{-10,-10},{10,10}},
            rotation=90)));
      DoubleActingCylinder                                     boomCylLeft(
        strokeLength=strokeLength,
        closedLength=closedLength,
        boreDiameter=boreDiameter,
        rodDiameter=rodDiameter,
        pistonMass=pistonMass,
        s_init=boom_s_init)
        annotation (Placement(transformation(
            origin={72,122},
            extent={{-10,10},{10,-10}},
            rotation=90)));
      parameter Modelica.SIunits.Length boom_s_init=1.19
      "Initial position of boom cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));

    // Cylinder Parameters
        parameter SI.Length closedLength = 2.48;
        parameter SI.Diameter boreDiameter = 0.135;
        parameter SI.Diameter rodDiameter=0.05;
        parameter SI.Mass pistonMass = 10;
        parameter SI.Length strokeLength = 1.25;

      OpenHydraulics.Interfaces.FluidPort port_a annotation (Placement(
            transformation(rotation=0, extent={{-10,217},{10,247}}),
            iconTransformation(extent={{-10,217},{10,247}})));
      OpenHydraulics.Interfaces.FluidPort port_b annotation (Placement(
            transformation(rotation=0, extent={{-14,-43},{6,-13}}),
            iconTransformation(extent={{-14,-43},{6,-13}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation (
          Placement(transformation(rotation=0, extent={{104,-59},{124,-29}}),
            iconTransformation(extent={{104,-59},{124,-29}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b annotation (
          Placement(transformation(rotation=0, extent={{114,225},{134,255}}),
            iconTransformation(extent={{114,225},{134,255}})));
    Modelica.Mechanics.MultiBody.Sensors.Distance distance annotation (
        Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=90,
          origin={152,118})));
    Modelica.Blocks.Interfaces.RealOutput cylLength
      annotation (Placement(transformation(extent={{190,106},{210,126}})));
    OpenHydraulics.Components.Sensors.PressureSensor pressureSensor(absolute=
          true)
      annotation (Placement(transformation(extent={{36,82},{16,102}})));
    Modelica.Blocks.Interfaces.RealOutput cylPressure_b
      annotation (Placement(transformation(extent={{-12,76},{-42,106}})));
    OpenHydraulics.Components.Sensors.PressureSensor pressureSensor1(
                                                                    absolute=
          true)
      annotation (Placement(transformation(extent={{34,154},{14,174}})));
    Modelica.Blocks.Interfaces.RealOutput cylPressure_a
      annotation (Placement(transformation(extent={{-10,148},{-38,176}})));
    Modelica.Blocks.Interfaces.RealOutput cylLength_unit
      annotation (Placement(transformation(extent={{190,42},{210,62}})));
    equation
      connect(boomCylLeft.flange_b, cylBoomLeft.flange_b) annotation (Line(
          points={{72,132},{92,132},{92,123},{99,123}},
          color={0,127,0},
          smooth=Smooth.None));
      connect(boomCylLeft.flange_a, cylBoomLeft.flange_a) annotation (Line(
          points={{72,112},{64,112},{64,104},{90,104},{90,111},{99,111}},
          color={0,127,0},
          smooth=Smooth.None));
      connect(port_a, boomCylLeft.port_a) annotation (Line(points={{0,232},{54,
            232},{54,114},{64,114}},   color={255,0,0}));
      connect(port_b, boomCylLeft.port_b) annotation (Line(points={{-4,-28},{50,
            -28},{50,130},{64,130}},   color={255,0,0}));
      connect(frame_a, cylBoomLeft.frame_a) annotation (Line(points={{114,-44},
              {108,-44},{108,107},{109,107}}, color={0,0,0}));
      connect(frame_b, cylBoomLeft.frame_b) annotation (Line(points={{124,240},
              {108,240},{108,127},{109,127}}, color={0,0,0}));
    connect(distance.frame_b, cylBoomLeft.frame_b) annotation (Line(
        points={{152,128},{152,152},{108,152},{108,127},{109,127}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(distance.frame_a, cylBoomLeft.frame_a) annotation (Line(
        points={{152,108},{152,74},{108,74},{108,107},{109,107}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(distance.distance, cylLength)   annotation (Line(
        points={{163,118},{178,118},{178,116},{200,116}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(pressureSensor.y, cylPressure_b)
                                           annotation (Line(
        points={{19,92},{-4,92},{-4,91},{-27,91}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(pressureSensor.port_a, boomCylLeft.port_b) annotation (Line(
        points={{26,82},{38,82},{38,130},{64,130}},
        color={255,0,0},
        smooth=Smooth.None));
    connect(cylPressure_a, pressureSensor1.y) annotation (Line(
        points={{-24,162},{-4,162},{-4,164},{17,164}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(pressureSensor1.port_a, boomCylLeft.port_a) annotation (Line(
        points={{24,154},{54,154},{54,114},{64,114}},
        color={255,0,0},
        smooth=Smooth.None));
    connect(boomCylLeft.strokeLength_unit, cylLength_unit) annotation (Line(
        points={{80.6,111},{80.6,52},{200,52}},
        color={0,0,127},
        smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(extent={{-10,-50},{190,250}},
              preserveAspectRatio=false), graphics), Icon(coordinateSystem(
              extent={{-10,-50},{190,250}}, preserveAspectRatio=false),
            graphics={
            Text(
              extent={{8,242},{70,218}},
              lineColor={0,0,255},
              textString="PortA"),
            Text(
              extent={{18,-8},{66,-62}},
              lineColor={0,0,255},
              textString="PortB"),
            Rectangle(
              extent={{-10,250},{190,-50}},
              lineColor={0,0,0},
              lineThickness=0.5),
            Text(
              extent={{-4,132},{116,78}},
              lineColor={0,0,0},
              lineThickness=0.5,
              fillColor={0,0,255},
              fillPattern=FillPattern.Solid,
              textString="boomCylL")}));
    end boomCylL;

    model LSValveUnit

      // sizing parameters
      parameter SI.VolumeFlowRate q_nom = 0.001 "Nominal flow rate at dp_nom"
        annotation(Dialog(tab="Sizing"));
      parameter SI.Pressure dp_nom = 3e5 "Nominal dp for metering curve"
        annotation(Dialog(tab="Sizing"));

      parameter Real q_fraction_P2A = 1 "Fraction of nominal flow rate"
        annotation(Dialog(tab="Sizing",group="Advanced Metering"));
      parameter Real q_fraction_P2B = 1 "Fraction of nominal flow rate"
        annotation(Dialog(tab="Sizing",group="Advanced Metering"));
      parameter Real q_fraction_A2T = 1 "Fraction of nominal flow rate"
        annotation(Dialog(tab="Sizing",group="Advanced Metering"));
      parameter Real q_fraction_B2T = 1 "Fraction of nominal flow rate"
        annotation(Dialog(tab="Sizing",group="Advanced Metering"));

      // dynamic response parameters
      parameter SI.Frequency bandwidth = 10 "Bandwidth of 2nd order response"
        annotation(Dialog(tab="Dynamics"));
      parameter Real dampCoeff = 1 "Damping coefficient of 2nd order response"
        annotation(Dialog(tab="Dynamics"));

      // initialization parameters
      parameter Boolean fixPressureA = false
      "True if pressure at port is fixed"
      annotation(Dialog(tab="Initialization",group="Pressures"));
      parameter Boolean fixPressureB = false
      "True if pressure at port is fixed"
      annotation(Dialog(tab="Initialization",group="Pressures"));

      // the components

      OpenHydraulics.Components.Valves.ShuttleValve shuttleValve(q_nom=0.01)
        annotation (Placement(transformation(
            origin={0,30},
            extent={{10,-10},{-10,10}},
            rotation=270)));

      // configuration components
      OpenHydraulics.Components.Valves.DirectionalValves.V4_3CCLSHydAntiCavitation
        valve4_3pos_wRelief(
        bandwidth=bandwidth,
        dampCoeff=dampCoeff,
        q_nom=q_nom,
        dp_nom=dp_nom,
        q_fraction_P2A=q_fraction_P2A,
        q_fraction_P2B=q_fraction_P2B,
        q_fraction_A2T=q_fraction_A2T,
        q_fraction_B2T=q_fraction_B2T,
        j1(V=0))
        annotation (
        Dialog(tab="Config Options"), Placement(transformation(extent={{-10,
                -8},{10,12}}, rotation=0)));

      OpenHydraulics.Components.Lines.NJunction j1(n_ports=3)
        annotation (Placement(transformation(extent={{-14,-50},{6,-30}},
              rotation=0)));

      OpenHydraulics.Components.Lines.NJunction j2(n_ports=3)
        annotation (Placement(transformation(extent={{-6,-90},{14,-70}},
              rotation=0)));

      // the connectors
      OpenHydraulics.Interfaces.FluidPort portA(p(start=p_init, fixed=
              fixPressureA))
        annotation (Placement(transformation(extent={{-58,94},{-46,106}},
              rotation=0)));

      OpenHydraulics.Interfaces.FluidPort portB(p(start=p_init, fixed=
              fixPressureB))
        annotation (Placement(transformation(extent={{46,94},{58,106}},
              rotation=0)));

      OpenHydraulics.Interfaces.FluidPort portP2
        annotation (Placement(transformation(extent={{-106,-46},{-94,-34}},
              rotation=0)));
      OpenHydraulics.Interfaces.FluidPort portT2
        annotation (Placement(transformation(extent={{-106,-86},{-94,-74}},
              rotation=0)));
      OpenHydraulics.Interfaces.FluidPort portP1
        annotation (Placement(transformation(extent={{94,-46},{106,-34}},
              rotation=0)));
      OpenHydraulics.Interfaces.FluidPort portT1
        annotation (Placement(transformation(extent={{94,-86},{106,-74}},
              rotation=0)));
      OpenHydraulics.Interfaces.FluidPort portLS1
        annotation (Placement(transformation(extent={{94,14},{106,26}},
              rotation=0)));
      OpenHydraulics.Interfaces.FluidPort portLS2
        annotation (Placement(transformation(extent={{-106,14},{-94,26}},
              rotation=0)));
      parameter Integer sizeOfInputs=1 "Size of input array";
      parameter Integer inputIndex=1 "Index of command for this valve";

      extends OpenHydraulics.Interfaces.PartialFluidComponent;
      Modelica.Blocks.Interfaces.RealInput control annotation (Placement(
            transformation(
            extent={{-20,-20},{20,20}},
            rotation=180,
            origin={110,80})));
    equation
      connect(valve4_3pos_wRelief.portLS, shuttleValve.PortA) annotation (Line(
            points={{0,10},{0,20},{-1.83697e-015,20}},color={255,0,0}));
      connect(portLS1, shuttleValve.PortB) annotation (Line(points={{100,20},
              {60,20},{60,50},{0,50},{0,40},{1.83697e-015,40}},  color={255,0,
              0}));
      connect(shuttleValve.PortC, portLS2) annotation (Line(points={{-8,30},{
              -60,30},{-60,20},{-100,20}}, color={255,0,0}));
      connect(portP2, j1.port[1]) annotation (Line(points={{-100,-40},{-4,-40},
            {-4,-40.6667}}, color={255,0,0}));
      connect(portP1, j1.port[2]) annotation (Line(points={{100,-40},{48,-40},{48,-40},
              {-4,-40}},                color={255,0,0}));
      connect(portT2, j2.port[1]) annotation (Line(points={{-100,-80},{4,-80},{
            4,-80.6667}},  color={255,0,0}));
      connect(portT1, j2.port[2]) annotation (Line(points={{100,-80},{4,-80},{4,-80}},
                           color={255,0,0}));
      connect(j2.port[3], valve4_3pos_wRelief.portT)
        annotation (Line(points={{4,-79.3333},{4,-6}},
                                                     color={255,0,0}));
      connect(valve4_3pos_wRelief.portA, portA) annotation (Line(points={{-4,
              10},{-4,12},{-52,12},{-52,100}}, color={255,0,0}));
      connect(valve4_3pos_wRelief.portB, portB) annotation (Line(points={{4,
              10},{4,12},{52,12},{52,100}}, color={255,0,0}));
      connect(valve4_3pos_wRelief.portP, j1.port[3]) annotation (Line(points={{-4,-6},
            {-4,-39.3333}},          color={255,0,0}));
      connect(valve4_3pos_wRelief.control, control) annotation (Line(
          points={{11,2},{56,2},{56,80},{110,80}},
          color={0,0,127},
          smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{
                -100,-100},{100,100}}),
                          graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-100,-100},{100,100}}), graphics={
            Text(
              extent={{62,24},{96,14}},
              lineColor={255,0,0},
              textString="LS"),
            Text(
              extent={{-94,24},{-60,14}},
              lineColor={255,0,0},
              textString="LS"),
            Text(
              extent={{72,-34},{98,-44}},
              lineColor={255,0,0},
              textString="P"),
            Text(
              extent={{-92,-36},{-66,-46}},
              lineColor={255,0,0},
              textString="P"),
            Text(
              extent={{70,-74},{96,-84}},
              lineColor={255,0,0},
              textString="T"),
            Text(
              extent={{-94,-76},{-68,-86}},
              lineColor={255,0,0},
              textString="T"),
            Text(
              extent={{-66,90},{-38,76}},
              lineColor={255,0,0},
              textString="A"),
            Text(
              extent={{38,90},{68,76}},
              lineColor={255,0,0},
              textString="B"),
            Text(
              extent={{-100,20},{100,-20}},
              lineColor={0,0,0},
              textString="Valve"),
            Rectangle(extent={{-100,100},{100,-100}}, lineColor={0,0,0}),
            Text(
              extent={{-100,-20},{100,-60}},
              lineColor={0,0,0},
              textString="Unit"),
            Text(
              extent={{-100,80},{100,40}},
              lineColor={0,0,255},
              textString="%name")}));
    end LSValveUnit;

    model carriage_CAD

      Modelica.Mechanics.MultiBody.Joints.Revolute swingRevolute(
        n={0,1,0},
        useAxisFlange=true,
        a(start=swing_a_start, fixed=if ((swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.SteadyState
               or (swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.VelocityAcceleration
               or (swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration)
               then true else false),
        phi(fixed=if ((swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocity
               or (swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.Position
               or (swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration)
               then true else false, start=swing_phi_start),
        w(fixed=if ((swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocity
               or (swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.SteadyState
               or (swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.Velocity
               or (swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.VelocityAcceleration
               or (swing_initType) == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration)
               then true else false, start=swing_w_start))
      "revolute joint (plus motor) representing the swing function"
        annotation (Placement(transformation(
            origin={154,118},
            extent={{-10,-10},{10,10}},
            rotation=90)));
      Modelica.Mechanics.MultiBody.Parts.FixedTranslation carriage(
        widthDirection={0,1,0},
        r_shape={0,0,0},
        color={255,255,0},
      r=r_carriage,
      shapeType=CAD_carriage,
        lengthDirection={1,0,0})
      "object model for the carriage of the excavator"
        annotation (Placement(transformation(
            origin={154.5,221},
            extent={{-26,-13.5},{26,13.5}},
            rotation=90)));

      Modelica.Mechanics.MultiBody.Parts.FixedTranslation boom1LinkRight(r=
            boomLinkRight)
      "from swing motor to lifting cylinder cap side pivot"
        annotation (Placement(transformation(
            origin={224.5,167},
            extent={{-20,-8.5},{20,8.5}},
            rotation=90)));
      Modelica.Mechanics.MultiBody.Parts.FixedTranslation boom1LinkLeft(r=
            boomLinkLeft) "from swing motor to lifting cylinder cap side pivot"
        annotation (Placement(transformation(
            origin={86.5,169},
            extent={{-20,-8.5},{20,8.5}},
            rotation=90)));
      Modelica.Mechanics.MultiBody.Parts.Body bCarriage(
        m=m_carriage,
        I_11=I_11_carriage,
        I_22=I_22_carriage,
        I_33=I_33_carriage,
        I_21=I_21_carriage,
        I_31=I_31_carriage,
        I_32=I_32_carriage,
        r_CM=r_CM_carriage) "mass/inertia properties for carriage"
        annotation (Placement(transformation(extent={{179,185},{199,205}},
              rotation=0)));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b annotation (Placement(
            transformation(rotation=0, extent={{58,192},{78,212}}),
            iconTransformation(extent={{58,192},{78,212}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b1 annotation (
          Placement(transformation(rotation=0, extent={{244,190},{264,210}}),
            iconTransformation(extent={{244,190},{264,210}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b2 annotation (
          Placement(transformation(rotation=0, extent={{146,262},{166,282}}),
            iconTransformation(extent={{146,262},{166,282}})));
      Modelica.Mechanics.Rotational.Interfaces.Flange_a axis annotation (Placement(
            transformation(rotation=0, extent={{50,70},{70,90}})));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation (Placement(
            transformation(rotation=0, extent={{150,70},{170,90}})));

    // the swing parameters
      parameter Modelica.Mechanics.MultiBody.Types.Init swing_initType=
          Modelica.Mechanics.MultiBody.Types.Init.PositionVelocity
      "Type of initialization (defines usage of start values below)"
        annotation (Dialog(group="Swing",tab="Initialization"));
          //Modelica.Mechanics.MultiBody.Types.Init.Free
      parameter SI.Angle
        swing_phi_start=0
      "Initial value of rotation angle phi (fixed or guess value)"
        annotation (Evaluate=false, Dialog(group="Swing",tab="Initialization"));
      parameter SI.AngularVelocity
        swing_w_start =                                     0
      "Initial value of relative angular velocity w = der(phi)"
        annotation (Evaluate=false, Dialog(group="Swing",tab="Initialization"));
      parameter SI.AngularAcceleration
        swing_a_start =                                          0
      "Initial value of relative angular acceleration a = der(w)"
        annotation (Evaluate=false, Dialog(group="Swing",tab="Initialization"));

    // Carriage Intertia Parameters
        parameter SI.Mass m_carriage=11312.161;
        parameter SI.MomentOfInertia I_11_carriage=9667;
        parameter SI.MomentOfInertia I_22_carriage=34850;
        parameter SI.MomentOfInertia I_33_carriage=30686;
        parameter SI.MomentOfInertia I_21_carriage=-3756;
        parameter SI.MomentOfInertia I_31_carriage=0;
        parameter SI.MomentOfInertia I_32_carriage=0;
        parameter Real r_CM_carriage[:]={-2.555,1.169,0};

    // Position Parameters
        parameter Real boomLinkRight[:] = {0.655,0.703,0.296};// "from swing motor to left boom cylinder cap side pivot"
        parameter Real boomLinkLeft[:] = {0.655,0.703,-0.296};// "from swing motor to right boom cylinder cap side pivot"
    // CAD parameters
        parameter String CAD_carriage = "modelica://OpenHydraulics/Resources/Shapes/Excavator/upper_body.dxf";
        parameter Real r_carriage[:] = {-0.164,1.428,0};
        //parameter Real r_carriage[:] = {0,0,0};

    equation
      connect(boom1LinkRight.frame_a, swingRevolute.frame_b)    annotation (Line(
          points={{224.5,147},{218,147},{218,142},{154,142},{154,128}},
          color={0,0,0},
          thickness=0.5));
      connect(carriage.frame_a,swingRevolute. frame_b)
                                                  annotation (Line(
          points={{154.5,195},{154,195},{154,184},{154,128}},
          color={0,0,0},
          thickness=0.5));
      connect(bCarriage.frame_a,carriage. frame_a) annotation (Line(
          points={{179,195},{166,195},{154.5,195}},
          color={0,0,0},
          thickness=0.5));
      connect(boom1LinkLeft.frame_a, swingRevolute.frame_b)
                                              annotation (Line(
          points={{86.5,149},{95,149},{95,142},{154,142},{154,128}},
          color={0,0,0},
          thickness=0.5));
      connect(frame_b, boom1LinkLeft.frame_b) annotation (Line(points={{68,202},
              {68,201},{86.5,201},{86.5,189}},
                                         color={0,0,0}));
      connect(frame_b1, boom1LinkRight.frame_b) annotation (Line(points={{254,200},
              {224,200},{224,187},{224.5,187}},color={0,0,0}));
      connect(frame_b2, carriage.frame_b) annotation (Line(points={{156,272},{
              156,252},{154.5,252},{154.5,247}},
                                     color={95,95,95}));
      connect(axis, swingRevolute.axis) annotation (Line(points={{60,80},{114,
              80},{114,118},{144,118}},
                             color={0,0,0}));
      connect(frame_a, swingRevolute.frame_a) annotation (Line(points={{160,80},
              {154,80},{154,108}},    color={0,0,0}));
      annotation (Diagram(coordinateSystem(extent={{60,80},{260,280}},
              preserveAspectRatio=false), graphics),                     Icon(
            coordinateSystem(extent={{60,80},{260,280}}, preserveAspectRatio=
                false), graphics));
    end carriage_CAD;

    model Body_variableMass
    "Rigid body with mass, inertia tensor and one frame connector (12 potential states)"

      import SI = Modelica.SIunits;
      import C = Modelica.Constants;
      import Modelica.Math.*;
      import Modelica.Mechanics.MultiBody.Types;
      import Modelica.Mechanics.MultiBody.Frames;
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a
      "Coordinate system fixed at body"
        annotation (Placement(transformation(extent={{-116,-16},{-84,16}},
              rotation=0)));
      parameter Boolean animation=true
      "= true, if animation shall be enabled (show cylinder and sphere)";
      parameter SI.Position r_CM[3](start={0,0,0})
      "Vector from frame_a to center of mass, resolved in frame_a";
      //parameter SI.Mass m(min=0, start = 1) "Mass of rigid body";
      SI.Mass m(min=0, start = 1) "Mass of rigid body";
      parameter SI.Inertia I_11(min=0) = 0.001
      " (1,1) element of inertia tensor"
        annotation (Dialog(group=
              "Inertia tensor (resolved in center of mass, parallel to frame_a)"));
      parameter SI.Inertia I_22(min=0) = 0.001
      " (2,2) element of inertia tensor"
        annotation (Dialog(group=
              "Inertia tensor (resolved in center of mass, parallel to frame_a)"));
      parameter SI.Inertia I_33(min=0) = 0.001
      " (3,3) element of inertia tensor"
        annotation (Dialog(group=
              "Inertia tensor (resolved in center of mass, parallel to frame_a)"));
      parameter SI.Inertia I_21(min=-C.inf)=0
      " (2,1) element of inertia tensor"                                         annotation (
          Dialog(group=
              "Inertia tensor (resolved in center of mass, parallel to frame_a)"));
      parameter SI.Inertia I_31(min=-C.inf)=0
      " (3,1) element of inertia tensor"                                         annotation (
          Dialog(group=
              "Inertia tensor (resolved in center of mass, parallel to frame_a)"));
      parameter SI.Inertia I_32(min=-C.inf)=0
      " (3,2) element of inertia tensor"                                         annotation (
          Dialog(group=
              "Inertia tensor (resolved in center of mass, parallel to frame_a)"));

      SI.Position r_0[3](start={0,0,0}, each stateSelect=if enforceStates then
                  StateSelect.always else StateSelect.avoid)
      "Position vector from origin of world frame to origin of frame_a"
        annotation(Dialog(tab="Initialization", showStartAttribute=true));
      SI.Velocity v_0[3](start={0,0,0}, each stateSelect=if enforceStates then StateSelect.always else
                  StateSelect.avoid)
      "Absolute velocity of frame_a, resolved in world frame (= der(r_0))"
        annotation(Dialog(tab="Initialization", showStartAttribute=true));
      SI.Acceleration a_0[3](start={0,0,0})
      "Absolute acceleration of frame_a resolved in world frame (= der(v_0))"
        annotation(Dialog(tab="Initialization", showStartAttribute=true));

      parameter Boolean angles_fixed = false
      "= true, if angles_start are used as initial values, else as guess values"
        annotation(Evaluate=true, choices(__Dymola_checkBox=true), Dialog(tab="Initialization"));
      parameter SI.Angle angles_start[3]={0,0,0}
      "Initial values of angles to rotate frame_a around 'sequence_start' axes into frame_b"
        annotation (Dialog(tab="Initialization"));
      parameter Types.RotationSequence sequence_start={1,2,3}
      "Sequence of rotations to rotate frame_a into frame_b at initial time"
        annotation (Evaluate=true, Dialog(tab="Initialization"));

      parameter Boolean w_0_fixed = false
      "= true, if w_0_start are used as initial values, else as guess values"
        annotation(Evaluate=true, choices(__Dymola_checkBox=true), Dialog(tab="Initialization"));
      parameter SI.AngularVelocity w_0_start[3]={0,0,0}
      "Initial or guess values of angular velocity of frame_a resolved in world frame"
        annotation (Dialog(tab="Initialization"));

      parameter Boolean z_0_fixed = false
      "= true, if z_0_start are used as initial values, else as guess values"
        annotation(Evaluate=true, choices(__Dymola_checkBox=true), Dialog(tab="Initialization"));
      parameter SI.AngularAcceleration z_0_start[3]={0,0,0}
      "Initial values of angular acceleration z_0 = der(w_0)"
        annotation (Dialog(tab="Initialization"));

      parameter SI.Diameter sphereDiameter=world.defaultBodyDiameter
      "Diameter of sphere"   annotation (Dialog(
          tab="Animation",
          group="if animation = true",
          enable=animation));
      input Types.Color sphereColor=Modelica.Mechanics.MultiBody.Types.Defaults.BodyColor
      "Color of sphere"   annotation (Dialog(
          tab="Animation",
          group="if animation = true",
          enable=animation));
      parameter SI.Diameter cylinderDiameter=sphereDiameter/Types.Defaults.
          BodyCylinderDiameterFraction "Diameter of cylinder"
        annotation (Dialog(
          tab="Animation",
          group="if animation = true",
          enable=animation));
      input Types.Color cylinderColor=sphereColor "Color of cylinder"
        annotation (Dialog(
          tab="Animation",
          group="if animation = true",
          enable=animation));
      input Types.SpecularCoefficient specularCoefficient = world.defaultSpecularCoefficient
      "Reflection of ambient light (= 0: light is completely absorbed)"
        annotation (Dialog(
          tab="Animation",
          group="if animation = true",
          enable=animation));
      parameter Boolean enforceStates=false
      " = true, if absolute variables of body object shall be used as states (StateSelect.always)"
        annotation (Evaluate=true,Dialog(tab="Advanced"));
      parameter Boolean useQuaternions=true
      " = true, if quaternions shall be used as potential states otherwise use 3 angles as potential states"
        annotation (Evaluate=true,Dialog(tab="Advanced"));
      parameter Types.RotationSequence sequence_angleStates={1,2,3}
      " Sequence of rotations to rotate world frame into frame_a around the 3 angles used as potential states"
         annotation (Evaluate=true, Dialog(tab="Advanced", enable=not
              useQuaternions));

      final parameter SI.Inertia I[3, 3]=[I_11, I_21, I_31; I_21, I_22, I_32;
          I_31, I_32, I_33] "inertia tensor";
      final parameter Frames.Orientation R_start=Modelica.Mechanics.MultiBody.Frames.axesRotations(
          sequence_start, angles_start, zeros(3))
      "Orientation object from world frame to frame_a at initial time";
      final parameter SI.AngularAcceleration z_a_start[3]=Frames.resolve2(R_start, z_0_start)
      "Initial values of angular acceleration z_a = der(w_a), i.e., time derivative of angular velocity resolved in frame_a";

      SI.AngularVelocity w_a[3](start=Frames.resolve2(R_start, w_0_start),
                                fixed=fill(w_0_fixed,3),
                                each stateSelect=if enforceStates then (if useQuaternions then
                                StateSelect.always else StateSelect.never) else StateSelect.avoid)
      "Absolute angular velocity of frame_a resolved in frame_a";
      SI.AngularAcceleration z_a[3](start=Frames.resolve2(R_start, z_0_start),fixed=fill(z_0_fixed,3))
      "Absolute angular acceleration of frame_a resolved in frame_a";
      SI.Acceleration g_0[3] "Gravity acceleration resolved in world frame";

  protected
      outer Modelica.Mechanics.MultiBody.World world;

      // Declarations for quaternions (dummies, if quaternions are not used)
      parameter Frames.Quaternions.Orientation Q_start=Frames.to_Q(R_start)
      "Quaternion orientation object from world frame to frame_a at initial time";
      Frames.Quaternions.Orientation Q(start=Q_start, each stateSelect=if
            enforceStates then (if useQuaternions then StateSelect.prefer else
            StateSelect.never) else StateSelect.avoid)
      "Quaternion orientation object from world frame to frame_a (dummy value, if quaternions are not used as states)";

      // Declaration for 3 angles
      parameter SI.Angle phi_start[3]=if sequence_start[1] ==
          sequence_angleStates[1] and sequence_start[2] == sequence_angleStates[2]
           and sequence_start[3] == sequence_angleStates[3] then angles_start else
           Frames.axesRotationsAngles(R_start, sequence_angleStates)
      "Potential angle states at initial time";
      SI.Angle phi[3](start=phi_start, each stateSelect=if enforceStates then (if
            useQuaternions then StateSelect.never else StateSelect.always) else
            StateSelect.avoid)
      "Dummy or 3 angles to rotate world frame into frame_a of body";
      SI.AngularVelocity phi_d[3](each stateSelect=if enforceStates then (if
            useQuaternions then StateSelect.never else StateSelect.always) else
            StateSelect.avoid) "= der(phi)";
      SI.AngularAcceleration phi_dd[3] "= der(phi_d)";

      // Declarations for animation
      Modelica.Mechanics.MultiBody.Visualizers.Advanced.Shape cylinder(
        shapeType="cylinder",
        color=cylinderColor,
        specularCoefficient=specularCoefficient,
        length=if Modelica.Math.Vectors.length(r_CM) > sphereDiameter/2 then
            Modelica.Math.Vectors.length(r_CM) - (if cylinderDiameter > 1.1*
            sphereDiameter then sphereDiameter/2 else 0) else 0,
        width=cylinderDiameter,
        height=cylinderDiameter,
        lengthDirection=r_CM,
        widthDirection={0,1,0},
        r=frame_a.r_0,
        R=frame_a.R) if world.enableAnimation and animation;
      Modelica.Mechanics.MultiBody.Visualizers.Advanced.Shape sphere(
        shapeType="sphere",
        color=sphereColor,
        specularCoefficient=specularCoefficient,
        length=sphereDiameter,
        width=sphereDiameter,
        height=sphereDiameter,
        lengthDirection={1,0,0},
        widthDirection={0,1,0},
        r_shape=r_CM - {1,0,0}*sphereDiameter/2,
        r=frame_a.r_0,
        R=frame_a.R) if world.enableAnimation and animation and sphereDiameter > 0;
    initial equation
      if angles_fixed then
        // Initialize positional variables
        if not Connections.isRoot(frame_a.R) then
          // frame_a.R is computed somewhere else
          zeros(3) = Frames.Orientation.equalityConstraint(frame_a.R, R_start);
        elseif useQuaternions then
          // frame_a.R is computed from quaternions Q
          zeros(3) = Frames.Quaternions.Orientation.equalityConstraint(Q, Q_start);
        else
          // frame_a.R is computed from the 3 angles 'phi'
          phi = phi_start;
        end if;
      end if;

    equation
      if enforceStates then
        Connections.root(frame_a.R);
      else
        Connections.potentialRoot(frame_a.R);
      end if;
      r_0 = frame_a.r_0;

      if not Connections.isRoot(frame_a.R) then
        // Body does not have states
        // Dummies
        Q = {0,0,0,1};
        phi = zeros(3);
        phi_d = zeros(3);
        phi_dd = zeros(3);
      elseif useQuaternions then
        // Use Quaternions as states (with dynamic state selection)
        frame_a.R = Frames.from_Q(Q, Frames.Quaternions.angularVelocity2(Q, der(Q)));
        {0} = Frames.Quaternions.orientationConstraint(Q);

        // Dummies
        phi = zeros(3);
        phi_d = zeros(3);
        phi_dd = zeros(3);
      else
        // Use Cardan angles as states
        phi_d = der(phi);
        phi_dd = der(phi_d);
        frame_a.R = Frames.axesRotations(sequence_angleStates, phi, phi_d);

        // Dummies
        Q = {0,0,0,1};
      end if;

      // gravity acceleration at center of mass resolved in world frame
      g_0 = world.gravityAcceleration(frame_a.r_0 + Frames.resolve1(frame_a.R,
        r_CM));

      // translational kinematic differential equations
      v_0 = der(frame_a.r_0);
      a_0 = der(v_0);

      // rotational kinematic differential equations
      w_a = Frames.angularVelocity2(frame_a.R);
      z_a = der(w_a);

      /* Newton/Euler equations with respect to center of mass
            a_CM = a_a + cross(z_a, r_CM) + cross(w_a, cross(w_a, r_CM));
            f_CM = m*(a_CM - g_a);
            t_CM = I*z_a + cross(w_a, I*w_a);
       frame_a.f = f_CM
       frame_a.t = t_CM + cross(r_CM, f_CM);
    Inserting the first three equations in the last two results in:
  */
      frame_a.f = m*(Frames.resolve2(frame_a.R, a_0 - g_0) + cross(z_a, r_CM) +
        cross(w_a, cross(w_a, r_CM)));
      frame_a.t = I*z_a + cross(w_a, I*w_a) + cross(r_CM, frame_a.f);
      annotation (
        Icon(coordinateSystem(
            preserveAspectRatio=true,
            extent={{-100,-100},{100,100}},
            grid={1,1}), graphics={
            Rectangle(
              extent={{-100,30},{-3,-31}},
              lineColor={0,0,0},
              fillPattern=FillPattern.HorizontalCylinder,
              fillColor={0,127,255}),
            Text(
              extent={{150,-100},{-150,-70}},
              lineColor={0,0,0},
              textString="m=%m"),
            Text(
              extent={{-150,110},{150,70}},
              textString="%name",
              lineColor={0,0,255}),
            Ellipse(
              extent={{-20,60},{100,-60}},
              lineColor={0,0,0},
              fillPattern=FillPattern.Sphere,
              fillColor={0,127,255})}),
        Diagram(coordinateSystem(
            preserveAspectRatio=true,
            extent={{-100,-100},{100,100}},
            grid={1,1}), graphics),
        Documentation(info="<HTML>
<p>
<b>Rigid body</b> with mass and inertia tensor.
All parameter vectors have to be resolved in frame_a.
The <b>inertia tensor</b> has to be defined with respect to a
coordinate system that is parallel to frame_a with the
origin at the center of mass of the body.
</p>
<p>
By default, this component is visualized by a <b>cylinder</b> located
between frame_a and the center of mass and by a <b>sphere</b> that has
its center at the center of mass. If the cylinder length is smaller as
the radius of the sphere, e.g., since frame_a is located at the
center of mass, the cylinder is not displayed. Note, that
the animation may be switched off via parameter animation = <b>false</b>.
</p>
<IMG src=\"modelica://Modelica/Resources/Images/MultiBody/Body.png\" ALT=\"Parts.Body\">
<p>
<b>States of Body Components</b>
</p>
<p>
Every body has potential states. If possible a tool will select
the states of joints and not the states of bodies because this is
usually the most efficient choice. In this case the position, orientation,
velocity and angular velocity of frame_a of the body will be computed
by the component that is connected to frame_a. However, if a body is moving
freely in space, variables of the body have to be used as states. The potential
states of the body are:
</p>
<ul>
<li> The <b>position vector</b> frame_a.r_0 from the origin of the
     world frame to the origin of frame_a of the body, resolved in
     the world frame and the <b>absolute velocity</b> v_0 of the origin of
     frame_a, resolved in the world frame (= der(frame_a.r_0)).</li>
</li>
<li> If parameter <b>useQuaternions</b> in the \"Advanced\" menu
     is <b>true</b> (this is the default), then <b>4 quaternions</b>
     are potential states. Additionally, the coordinates of the
     absolute angular velocity vector of the
     body are 3 potential states.<br>
     If <b>useQuaternions</b> in the \"Advanced\" menu
     is <b>false</b>, then <b>3 angles</b> and the derivatives of
     these angles are potential states. The orientation of frame_a
     is computed by rotating the world frame along the axes defined
     in parameter vector \"sequence_angleStates\" (default = {1,2,3}, i.e.,
     the Cardan angle sequence) around the angles used as potential states.
     For example, the default is to rotate the x-axis of the world frame
     around angles[1], the new y-axis around angles[2] and the new z-axis
     around angles[3], arriving at frame_a.
 </li>
</ul>
<p>
The quaternions have the slight disadvantage that there is a
non-linear constraint equation between the 4 quaternions.
Therefore, at least one non-linear equation has to be solved
during simulation. A tool might, however, analytically solve this
simple constraint equation. Using the 3 angles as states has the
disadvantage that there is a singular configuration in which a
division by zero will occur. If it is possible to determine in advance
for an application class that this singular configuration is outside
of the operating region, the 3 angles might be used as potential
states by setting <b>useQuaternions</b> = <b>false</b>.
</p>
<p>
In text books about 3-dimensional mechanics often 3 angles and the
angular velocity are used as states. This is not the case here, since
3 angles and their derivatives are used as potential states
(if useQuaternions = false). The reason
is that for real-time simulation the discretization formula of the
integrator might be \"inlined\" and solved together with the body equations.
By appropriate symbolic transformation the performance is
drastically increased if angles and their
derivatives are used as states, instead of angles and the angular
velocity.
</p>
<p>
Whether or not variables of the body are used as states is usually
automatically selected by the Modelica translator. If parameter
<b>enforceStates</b> is set to <b>true</b> in the \"Advanced\" menu,
then body variables are forced to be used as states according
to the setting of parameters \"useQuaternions\" and
\"sequence_angleStates\".
</p>
</HTML>"));
    end Body_variableMass;

    block LimPID_varyGain
    "P, PI, PD, and PID controller with limited output, anti-windup compensation and setpoint weighting"
      import Modelica.Blocks.Types.InitPID;
      import Modelica.Blocks.Types.SimpleController;
      extends Modelica.Blocks.Interfaces.SVcontrol;
      output Real controlError = u_s - u_m
      "Control error (set point - measurement)";

      parameter Modelica.Blocks.Types.SimpleController controllerType=
             Modelica.Blocks.Types.SimpleController.PID "Type of controller";

      //-------------------------------------------------------------------
       parameter Real k(min=0, unit="1") = 1/Lmax "Gain of controller";
      //-------------------------------------------------------------------

      parameter SI.Time Ti(min=Modelica.Constants.small, start=0.5)
      "Time constant of Integrator block"
         annotation(Dialog(enable=controllerType==SimpleController.PI or
                                  controllerType==SimpleController.PID));
      parameter SI.Time Td(min=0, start=0.1)
      "Time constant of Derivative block"
           annotation(Dialog(enable=controllerType==SimpleController.PD or
                                    controllerType==SimpleController.PID));
      parameter Real yMax(start=1) "Upper limit of output";
      parameter Real yMin=-yMax "Lower limit of output";
      parameter Real wp(min=0) = 1
      "Set-point weight for Proportional block (0..1)";
      parameter Real wd(min=0) = 0
      "Set-point weight for Derivative block (0..1)"
           annotation(Dialog(enable=controllerType==SimpleController.PD or
                                    controllerType==SimpleController.PID));
      parameter Real Ni(min=100*Modelica.Constants.eps) = 0.9
      "Ni*Ti is time constant of anti-windup compensation"
         annotation(Dialog(enable=controllerType==SimpleController.PI or
                                  controllerType==SimpleController.PID));
      parameter Real Nd(min=100*Modelica.Constants.eps) = 10
      "The higher Nd, the more ideal the derivative block"
           annotation(Dialog(enable=controllerType==SimpleController.PD or
                                    controllerType==SimpleController.PID));
      parameter Modelica.Blocks.Types.InitPID initType= Modelica.Blocks.Types.InitPID.DoNotUse_InitialIntegratorState
      "Type of initialization (1: no init, 2: steady state, 3: initial state, 4: initial output)"
                                         annotation(Evaluate=true,
          Dialog(group="Initialization"));
      parameter Boolean limitsAtInit = true
      "= false, if limits are ignored during initializiation"
        annotation(Evaluate=true, Dialog(group="Initialization",
                           enable=controllerType==SimpleController.PI or
                                  controllerType==SimpleController.PID));
      parameter Real xi_start=0
      "Initial or guess value value for integrator output (= integrator state)"
        annotation (Dialog(group="Initialization",
                    enable=controllerType==SimpleController.PI or
                           controllerType==SimpleController.PID));
      parameter Real xd_start=0
      "Initial or guess value for state of derivative block"
        annotation (Dialog(group="Initialization",
                             enable=controllerType==SimpleController.PD or
                                    controllerType==SimpleController.PID));
      parameter Real y_start=0 "Initial value of output"
        annotation(Dialog(enable=initType == InitPID.InitialOutput, group=
              "Initialization"));

      Modelica.Blocks.Math.Add addP(k1=wp, k2=-1)
        annotation (Placement(transformation(extent={{-80,40},{-60,60}}, rotation=
               0)));
      Modelica.Blocks.Math.Add addD(k1=wd, k2=-1) if with_D
        annotation (Placement(transformation(extent={{-80,-10},{-60,10}},
              rotation=0)));
      Modelica.Blocks.Math.Gain P(k=1)
                         annotation (Placement(transformation(extent={{-40,40},{
                -20,60}}, rotation=0)));
      Modelica.Blocks.Continuous.Integrator I(
        k=1/Ti,
        y_start=xi_start,
        initType=if initType == InitPID.SteadyState then InitPID.SteadyState else
            if initType == InitPID.InitialState or initType == InitPID.DoNotUse_InitialIntegratorState
             then InitPID.InitialState else InitPID.NoInit) if with_I
        annotation (Placement(transformation(extent={{-40,-60},{-20,-40}},
              rotation=0)));
      Modelica.Blocks.Continuous.Derivative D(
        k=Td,
        T=max([Td/Nd,1.e-14]),
        x_start=xd_start,
        initType=if initType == InitPID.SteadyState or initType == InitPID.InitialOutput
             then InitPID.SteadyState else if initType == InitPID.InitialState
             then InitPID.InitialState else InitPID.NoInit) if with_D
        annotation (Placement(transformation(extent={{-40,-10},{-20,10}},
              rotation=0)));
      Modelica.Blocks.Math.Gain gainPID(k=k)
                                    annotation (Placement(transformation(extent={
                {30,-10},{50,10}}, rotation=0)));
      Modelica.Blocks.Math.Add3 addPID
                              annotation (Placement(transformation(
              extent={{0,-10},{20,10}}, rotation=0)));
      Modelica.Blocks.Math.Add3 addI(k2=-1) if with_I
                                             annotation (Placement(
            transformation(extent={{-80,-60},{-60,-40}}, rotation=0)));
      Modelica.Blocks.Math.Add addSat(k1=+1, k2=-1) if with_I
        annotation (Placement(transformation(
            origin={80,-50},
            extent={{-10,-10},{10,10}},
            rotation=270)));
      Modelica.Blocks.Math.Gain gainTrack(k=1/(k*Ni)) if with_I
        annotation (Placement(transformation(extent={{40,-80},{20,-60}}, rotation=
               0)));
      Modelica.Blocks.Nonlinear.Limiter limiter(
        uMax=yMax,
        uMin=yMin,
        limitsAtInit=limitsAtInit)
        annotation (Placement(transformation(extent={{70,-10},{90,10}}, rotation=
                0)));
  protected
      parameter Boolean with_I = controllerType==SimpleController.PI or
                                 controllerType==SimpleController.PID annotation(Evaluate=true, HideResult=true);
      parameter Boolean with_D = controllerType==SimpleController.PD or
                                 controllerType==SimpleController.PID annotation(Evaluate=true, HideResult=true);
  public
      Modelica.Blocks.Sources.Constant Dzero(k=0) if not with_D
        annotation (Placement(transformation(extent={{-30,20},{-20,30}}, rotation=
               0)));
      Modelica.Blocks.Sources.Constant Izero(k=0) if not with_I
        annotation (Placement(transformation(extent={{10,-55},{0,-45}}, rotation=
                0)));
      Modelica.Blocks.Interfaces.RealInput
                Lmax "Lmax"
        annotation (Placement(transformation(extent={{-20,-20},{20,20}},
            rotation=270,
            origin={41,99}), iconTransformation(
            extent={{-20,-20},{20,20}},
            rotation=90,
            origin={-81,-120})));
    initial equation
      if initType==InitPID.InitialOutput then
         y = y_start;
      end if;
    equation
      assert(yMax >= yMin, "LimPID: Limits must be consistent. However, yMax (=" + String(yMax) +
                           ") < yMin (=" + String(yMin) + ")");
      if initType == InitPID.InitialOutput and (y_start < yMin or y_start > yMax) then
          Modelica.Utilities.Streams.error("LimPID: Start value y_start (=" + String(y_start) +
             ") is outside of the limits of yMin (=" + String(yMin) +") and yMax (=" + String(yMax) + ")");
      end if;
      assert(limitsAtInit or not limitsAtInit and y >= yMin and y <= yMax,
             "LimPID: During initialization the limits have been switched off.\n" +
             "After initialization, the output y (=" + String(y) +
             ") is outside of the limits of yMin (=" + String(yMin) +") and yMax (=" + String(yMax) + ")");

      connect(u_s, addP.u1) annotation (Line(points={{-120,0},{-96,0},{-96,56},{
              -82,56}}, color={0,0,127}));
      connect(u_s, addD.u1) annotation (Line(points={{-120,0},{-96,0},{-96,6},{
              -82,6}}, color={0,0,127}));
      connect(u_s, addI.u1) annotation (Line(points={{-120,0},{-96,0},{-96,-42},{
              -82,-42}}, color={0,0,127}));
      connect(addP.y, P.u) annotation (Line(points={{-59,50},{-42,50}}, color={0,
              0,127}));
      connect(addD.y, D.u)
        annotation (Line(points={{-59,0},{-42,0}}, color={0,0,127}));
      connect(addI.y, I.u) annotation (Line(points={{-59,-50},{-42,-50}}, color={
              0,0,127}));
      connect(P.y, addPID.u1) annotation (Line(points={{-19,50},{-10,50},{-10,8},
              {-2,8}}, color={0,0,127}));
      connect(D.y, addPID.u2)
        annotation (Line(points={{-19,0},{-2,0}}, color={0,0,127}));
      connect(I.y, addPID.u3) annotation (Line(points={{-19,-50},{-10,-50},{-10,
              -8},{-2,-8}}, color={0,0,127}));
      connect(addPID.y, gainPID.u)
        annotation (Line(points={{21,0},{28,0}}, color={0,0,127}));
      connect(gainPID.y, addSat.u2) annotation (Line(points={{51,0},{60,0},{60,
              -20},{74,-20},{74,-38}}, color={0,0,127}));
      connect(gainPID.y, limiter.u)
        annotation (Line(points={{51,0},{68,0}}, color={0,0,127}));
      connect(limiter.y, addSat.u1) annotation (Line(points={{91,0},{94,0},{94,
              -20},{86,-20},{86,-38}}, color={0,0,127}));
      connect(limiter.y, y)
        annotation (Line(points={{91,0},{110,0}}, color={0,0,127}));
      connect(addSat.y, gainTrack.u) annotation (Line(points={{80,-61},{80,-70},{
              42,-70}}, color={0,0,127}));
      connect(gainTrack.y, addI.u3) annotation (Line(points={{19,-70},{-88,-70},{
              -88,-58},{-82,-58}}, color={0,0,127}));
      connect(u_m, addP.u2) annotation (Line(
          points={{0,-120},{0,-92},{-92,-92},{-92,44},{-82,44}},
          color={0,0,127},
          thickness=0.5));
      connect(u_m, addD.u2) annotation (Line(
          points={{0,-120},{0,-92},{-92,-92},{-92,-6},{-82,-6}},
          color={0,0,127},
          thickness=0.5));
      connect(u_m, addI.u2) annotation (Line(
          points={{0,-120},{0,-92},{-92,-92},{-92,-50},{-82,-50}},
          color={0,0,127},
          thickness=0.5));
      connect(Dzero.y, addPID.u2) annotation (Line(points={{-19.5,25},{-14,25},{
              -14,0},{-2,0}}, color={0,0,127}));
      connect(Izero.y, addPID.u3) annotation (Line(points={{-0.5,-50},{-10,-50},{
              -10,-8},{-2,-8}}, color={0,0,127}));
      annotation (defaultComponentName="PID",
        Icon(coordinateSystem(
            preserveAspectRatio=false,
            extent={{-100,-100},{100,100}},
            grid={1,1}), graphics={
            Line(points={{-80,78},{-80,-90}}, color={192,192,192}),
            Polygon(
              points={{-80,90},{-88,68},{-72,68},{-80,90}},
              lineColor={192,192,192},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-90,-80},{82,-80}}, color={192,192,192}),
            Polygon(
              points={{90,-80},{68,-72},{68,-88},{90,-80}},
              lineColor={192,192,192},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Line(points={{-80,-80},{-80,50},{-80,-20},{30,60},{80,60}}, color={0,
                  0,127}),
            Text(
              extent={{-20,-20},{80,-60}},
              lineColor={192,192,192},
              textString="PID"),
            Text(
              extent={{-168,-129},{-98,-144}},
              lineColor={0,0,255},
              textString="Lmax
")}),   Documentation(info="<HTML>
<p>
Via parameter <b>controllerType</b> either <b>P</b>, <b>PI</b>, <b>PD</b>,
or <b>PID</b> can be selected. If, e.g., PI is selected, all components belonging to the
D-part are removed from the block (via conditional declarations).
The example model
<a href=\"modelica://Modelica.Blocks.Examples.PID_Controller\">Modelica.Blocks.Examples.PID_Controller</a>
demonstrates the usage of this controller.
Several practical aspects of PID controller design are incorporated
according to chapter 3 of the book:
</p>

<dl>
<dt>&Aring;str&ouml;m K.J., and H&auml;gglund T.:</dt>
<dd> <b>PID Controllers: Theory, Design, and Tuning</b>.
     Instrument Society of America, 2nd edition, 1995.
</dd>
</dl>

<p>
Besides the additive <b>proportional, integral</b> and <b>derivative</b>
part of this controller, the following features are present:
</p>
<ul>
<li> The output of this controller is limited. If the controller is
     in its limits, anti-windup compensation is activated to drive
     the integrator state to zero. </li>
<li> The high-frequency gain of the derivative part is limited
     to avoid excessive amplification of measurement noise.</li>
<li> Setpoint weighting is present, which allows to weight
     the setpoint in the proportional and the derivative part
     independantly from the measurement. The controller will respond
     to load disturbances and measurement noise independantly of this setting
     (parameters wp, wd). However, setpoint changes will depend on this
     setting. For example, it is useful to set the setpoint weight wd
     for the derivative part to zero, if steps may occur in the
     setpoint signal.</li>
</ul>

<p>
The parameters of the controller can be manually adjusted by performing
simulations of the closed loop system (= controller + plant connected
together) and using the following strategy:
</p>

<ol>
<li> Set very large limits, e.g., yMax = Modelica.Constants.inf</li>
<li> Select a <b>P</b>-controller and manually enlarge parameter <b>k</b>
     (the total gain of the controller) until the closed-loop response
     cannot be improved any more.</li>
<li> Select a <b>PI</b>-controller and manually adjust parameters
     <b>k</b> and <b>Ti</b> (the time constant of the integrator).
     The first value of Ti can be selected, such that it is in the
     order of the time constant of the oscillations occuring with
     the P-controller. If, e.g., vibrations in the order of T=10 ms
     occur in the previous step, start with Ti=0.01 s.</li>
<li> If you want to make the reaction of the control loop faster
     (but probably less robust against disturbances and measurement noise)
     select a <b>PID</b>-Controller and manually adjust parameters
     <b>k</b>, <b>Ti</b>, <b>Td</b> (time constant of derivative block).</li>
<li> Set the limits yMax and yMin according to your specification.</li>
<li> Perform simulations such that the output of the PID controller
     goes in its limits. Tune <b>Ni</b> (Ni*Ti is the time constant of
     the anti-windup compensation) such that the input to the limiter
     block (= limiter.u) goes quickly enough back to its limits.
     If Ni is decreased, this happens faster. If Ni=infinity, the
     anti-windup compensation is switched off and the controller works bad.</li>
</ol>

<p>
<b>Initialization</b>
</p>

<p>
This block can be initialized in different
ways controlled by parameter <b>initType</b>. The possible
values of initType are defined in
<a href=\"modelica://Modelica.Blocks.Types.InitPID\">Modelica.Blocks.Types.InitPID</a>.
This type is identical to
<a href=\"modelica://Modelica.Blocks.Types.Init\">Types.Init</a>,
with the only exception that the additional option
<b>DoNotUse_InitialIntegratorState</b> is added for
backward compatibility reasons (= integrator is initialized with
InitialState whereas differential part is initialized with
NoInit which was the initialization in version 2.2 of the Modelica
standard library).
</p>

<p>
Based on the setting of initType, the integrator (I) and derivative (D)
blocks inside the PID controller are initialized according to the following table:
</p>

<table border=1 cellspacing=0 cellpadding=2>
  <tr><td valign=\"top\"><b>initType</b></td>
      <td valign=\"top\"><b>I.initType</b></td>
      <td valign=\"top\"><b>D.initType</b></td></tr>

  <tr><td valign=\"top\"><b>NoInit</b></td>
      <td valign=\"top\">NoInit</td>
      <td valign=\"top\">NoInit</td></tr>

  <tr><td valign=\"top\"><b>SteadyState</b></td>
      <td valign=\"top\">SteadyState</td>
      <td valign=\"top\">SteadyState</td></tr>

  <tr><td valign=\"top\"><b>InitialState</b></td>
      <td valign=\"top\">InitialState</td>
      <td valign=\"top\">InitialState</td></tr>

  <tr><td valign=\"top\"><b>InitialOutput</b><br>
          and initial equation: y = y_start</td>
      <td valign=\"top\">NoInit</td>
      <td valign=\"top\">SteadyState</td></tr>

  <tr><td valign=\"top\"><b>DoNotUse_InitialIntegratorState</b></td>
      <td valign=\"top\">InitialState</td>
      <td valign=\"top\">NoInit</td></tr>
</table>

<p>
In many cases, the most useful initial condition is
<b>SteadyState</b> because initial transients are then no longer
present. If initType = InitPID.SteadyState, then in some
cases difficulties might occur. The reason is the
equation of the integrator:
</p>

<pre>
   <b>der</b>(y) = k*u;
</pre>

<p>
The steady state equation \"der(x)=0\" leads to the condition that the input u to the
integrator is zero. If the input u is already (directly or indirectly) defined
by another initial condition, then the initialization problem is <b>singular</b>
(has none or infinitely many solutions). This situation occurs often
for mechanical systems, where, e.g., u = desiredSpeed - measuredSpeed and
since speed is both a state and a derivative, it is natural to
initialize it with zero. As sketched this is, however, not possible.
The solution is to not initialize u_m or the variable that is used
to compute u_m by an algebraic equation.
</p>

<p>
If parameter <b>limitAtInit</b> = <b>false</b>, the limits at the
output of this controller block are removed from the initialization problem which
leads to a much simpler equation system. After initialization has been
performed, it is checked via an assert whether the output is in the
defined limits. For backward compatibility reasons
<b>limitAtInit</b> = <b>true</b>. In most cases it is best
to use <b>limitAtInit</b> = <b>false</b>.
</p>
</HTML>
"),     Diagram(coordinateSystem(
            preserveAspectRatio=false,
            extent={{-100,-100},{100,100}},
            grid={1,1}), graphics));
    end LimPID_varyGain;

    model Revolute_withDamping
    "Revolute joint (1 rotational degree-of-freedom, 2 potential states, optional axis flange)"

      import SI = Modelica.SIunits;

      Modelica.Mechanics.Rotational.Interfaces.Flange_a axis if useAxisFlange
      "1-dim. rotational flange that drives the joint"
        annotation (Placement(transformation(extent={{10,90},{-10,110}}, rotation=
               0)));
      Modelica.Mechanics.Rotational.Interfaces.Flange_b support if useAxisFlange
      "1-dim. rotational flange of the drive support (assumed to be fixed in the world frame, NOT in the joint)"
        annotation (Placement(transformation(extent={{-70,90},{-50,110}},
              rotation=0)));

      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a
      "Coordinate system fixed to the joint with one cut-force and cut-torque"
        annotation (Placement(transformation(extent={{-116,-16},{-84,16}},
              rotation=0)));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b
      "Coordinate system fixed to the joint with one cut-force and cut-torque"
        annotation (Placement(transformation(extent={{84,-16},{116,16}},
              rotation=0)));

      parameter Boolean useAxisFlange=false "= true, if axis flange is enabled"
        annotation(Evaluate=true, HideResult=true, choices(__Dymola_checkBox=true));
      parameter Boolean animation=true
      "= true, if animation shall be enabled (show axis as cylinder)";
      parameter Modelica.Mechanics.MultiBody.Types.Axis n={0,0,1}
      "Axis of rotation resolved in frame_a (= same as in frame_b)"
        annotation (Evaluate=true);
      constant SI.Angle phi_offset=0
      "Relative angle offset (angle = phi_offset + phi)";
      parameter SI.Distance cylinderLength=world.defaultJointLength
      "Length of cylinder representing the joint axis"
        annotation (Dialog(tab="Animation", group="if animation = true", enable=animation));
      parameter SI.Distance cylinderDiameter=world.defaultJointWidth
      "Diameter of cylinder representing the joint axis"
        annotation (Dialog(tab="Animation", group="if animation = true", enable=animation));
      input Modelica.Mechanics.MultiBody.Types.Color cylinderColor=Modelica.Mechanics.MultiBody.Types.Defaults.JointColor
      "Color of cylinder representing the joint axis"
        annotation (Dialog(tab="Animation", group="if animation = true", enable=animation));
      input Modelica.Mechanics.MultiBody.Types.SpecularCoefficient
        specularCoefficient =                                                            world.defaultSpecularCoefficient
      "Reflection of ambient light (= 0: light is completely absorbed)"
        annotation (Dialog(tab="Animation", group="if animation = true", enable=animation));
      parameter StateSelect stateSelect=StateSelect.prefer
      "Priority to use joint angle phi and w=der(phi) as states"   annotation(Dialog(tab="Advanced"));

      SI.Angle phi(start=0, final stateSelect=stateSelect)
      "Relative rotation angle from frame_a to frame_b"
         annotation (unassignedMessage="
The rotation angle phi of a revolute joint cannot be determined.
Possible reasons:
- A non-zero mass might be missing on either side of the parts
  connected to the revolute joint.
- Too many StateSelect.always are defined and the model
  has less degrees of freedom as specified with this setting
  (remove all StateSelect.always settings).
");
      SI.AngularVelocity w(start=0, stateSelect=stateSelect)
      "First derivative of angle phi (relative angular velocity)";
      SI.AngularAcceleration a(start=0)
      "Second derivative of angle phi (relative angular acceleration)";
      SI.Torque tau "Driving torque in direction of axis of rotation";
      SI.Angle angle "= phi_offset + phi";

  protected
      outer Modelica.Mechanics.MultiBody.World world;
      parameter Real e[3](each final unit="1")=Modelica.Math.Vectors.normalize(
                                           n,0.0)
      "Unit vector in direction of rotation axis, resolved in frame_a (= same as in frame_b)";
      Modelica.Mechanics.MultiBody.Frames.Orientation R_rel
      "Relative orientation object from frame_a to frame_b or from frame_b to frame_a";
      Modelica.Mechanics.MultiBody.Visualizers.Advanced.Shape cylinder(
      shapeType="cylinder",
      color=cylinderColor,
      specularCoefficient=specularCoefficient,
      length=cylinderLength,
      width=cylinderDiameter,
      height=cylinderDiameter,
      lengthDirection=e,
      widthDirection={0,1,0},
      r_shape=-e*(cylinderLength/2),
      r=frame_a.r_0,
      R=frame_a.R) if world.enableAnimation and animation;

  protected
      Modelica.Mechanics.Rotational.Components.Fixed fixed
      "support flange is fixed to ground"
        annotation (Placement(transformation(extent={{-70,70},{-50,90}})));
      Modelica.Mechanics.Rotational.Interfaces.InternalSupport internalAxis(tau=tau)
        annotation (Placement(transformation(extent={{-10,90},{10,70}})));
      Modelica.Mechanics.Rotational.Sources.ConstantTorque constantTorque(
        tau_constant=0) if not useAxisFlange
        annotation (Placement(transformation(extent={{40,70},{20,90}})));
  public
    Modelica.Mechanics.Rotational.Components.Damper damper(d=1)
      annotation (Placement(transformation(extent={{-41,40},{-21,60}})));
    equation
      Connections.branch(frame_a.R, frame_b.R);

      assert(cardinality(frame_a) > 0,
        "Connector frame_a of revolute joint is not connected");
      assert(cardinality(frame_b) > 0,
        "Connector frame_b of revolute joint is not connected");

      angle = phi_offset + phi;
      w = der(phi);
      a = der(w);

      // relationships between quantities of frame_a and of frame_b
      frame_b.r_0 = frame_a.r_0;

      if rooted(frame_a.R) then
      R_rel = Modelica.Mechanics.MultiBody.Frames.planarRotation(
            e,
            phi_offset + phi,
            w);
      frame_b.R = Modelica.Mechanics.MultiBody.Frames.absoluteRotation(frame_a.R,
        R_rel);
      frame_a.f = -Modelica.Mechanics.MultiBody.Frames.resolve1(R_rel, frame_b.f);
      frame_a.t = -Modelica.Mechanics.MultiBody.Frames.resolve1(R_rel, frame_b.t);
      else
      R_rel = Modelica.Mechanics.MultiBody.Frames.planarRotation(
            -e,
            phi_offset + phi,
            w);
      frame_a.R = Modelica.Mechanics.MultiBody.Frames.absoluteRotation(frame_b.R,
        R_rel);
      frame_b.f = -Modelica.Mechanics.MultiBody.Frames.resolve1(R_rel, frame_a.f);
      frame_b.t = -Modelica.Mechanics.MultiBody.Frames.resolve1(R_rel, frame_a.t);
      end if;

      // d'Alemberts principle
      tau = -frame_b.t*e;

      // Connection to internal connectors
      phi = internalAxis.phi;

      connect(fixed.flange, support) annotation (Line(
          points={{-60,80},{-60,100}},
          color={0,0,0},
          smooth=Smooth.None));
      connect(internalAxis.flange, axis) annotation (Line(
          points={{0,80},{0,100}},
          color={0,0,0},
          smooth=Smooth.None));
      connect(constantTorque.flange, internalAxis.flange) annotation (Line(
          points={{20,80},{0,80}},
          color={0,0,0},
          smooth=Smooth.None));
    connect(support, damper.flange_a) annotation (Line(
        points={{-60,100},{-51,100},{-51,99},{-41,99},{-41,50}},
        color={0,0,0},
        smooth=Smooth.None));
    connect(axis, damper.flange_b) annotation (Line(
        points={{0,100},{-21,100},{-21,50}},
        color={0,0,0},
        smooth=Smooth.None));
      annotation (
        Icon(coordinateSystem(
            preserveAspectRatio=true,
            extent={{-100,-100},{100,100}},
            grid={1,1}), graphics={
            Rectangle(
              extent={{-100,-60},{-30,60}},
              lineColor={0,0,0},
              fillPattern=FillPattern.HorizontalCylinder,
              fillColor={192,192,192}),
            Rectangle(
              extent={{30,-60},{100,60}},
              lineColor={0,0,0},
              fillPattern=FillPattern.HorizontalCylinder,
              fillColor={192,192,192}),
            Rectangle(extent={{-100,59},{-30,-60}}, lineColor={0,0,0}),
            Rectangle(extent={{30,60},{100,-60}}, lineColor={0,0,0}),
            Text(
              extent={{-90,14},{-54,-11}},
              lineColor={128,128,128},
              textString="a"),
            Text(
              extent={{51,11},{87,-14}},
              lineColor={128,128,128},
              textString="b"),
            Line(
              visible=useAxisFlange,
              points={{-20,80},{-20,60}},
              color={0,0,0}),
            Line(
              visible=useAxisFlange,
              points={{20,80},{20,60}},
              color={0,0,0}),
            Rectangle(
              visible=useAxisFlange,
              extent={{-10,100},{10,50}},
              lineColor={0,0,0},
              fillPattern=FillPattern.VerticalCylinder,
              fillColor={192,192,192}),
            Polygon(
              visible=useAxisFlange,
              points={{-10,30},{10,30},{30,50},{-30,50},{-10,30}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Rectangle(
              extent={{-30,11},{30,-10}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Polygon(
              visible=useAxisFlange,
              points={{10,30},{30,50},{30,-50},{10,-30},{10,30}},
              lineColor={0,0,0},
              fillColor={192,192,192},
              fillPattern=FillPattern.Solid),
            Text(
              extent={{-150,-110},{150,-80}},
              lineColor={0,0,0},
              textString="n=%n"),
            Text(
              visible=useAxisFlange,
              extent={{-150,-155},{150,-115}},
              textString="%name",
              lineColor={0,0,255}),
            Line(
              visible=useAxisFlange,
              points={{-20,70},{-60,70},{-60,60}},
              color={0,0,0},
              smooth=Smooth.None),
            Line(
              visible=useAxisFlange,
              points={{20,70},{50,70},{50,60}},
              color={0,0,0},
              smooth=Smooth.None),
            Line(
              visible=useAxisFlange,
              points={{-90,100},{-30,100}},
              color={0,0,0}),
            Line(
              visible=useAxisFlange,
              points={{-30,100},{-50,80}},
              color={0,0,0}),
            Line(
              visible=useAxisFlange,
              points={{-49,100},{-70,80}},
              color={0,0,0}),
            Line(
              visible=useAxisFlange,
              points={{-70,100},{-90,80}},
              color={0,0,0}),
            Text(
              visible=not useAxisFlange,
              extent={{-150,70},{150,110}},
              textString="%name",
              lineColor={0,0,255})}),
        Diagram(coordinateSystem(
            preserveAspectRatio=false,
            extent={{-100,-100},{100,100}},
            grid={1,1}), graphics),
        Documentation(info="<html>

<p>
Joint where frame_b rotates around axis n which is fixed in frame_a.
The two frames coincide when the rotation angle \"phi = 0\".
</p>

<p>
Optionally, two additional 1-dimensional mechanical flanges
(flange \"axis\" represents the driving flange and
flange \"support\" represents the bearing) can be enabled via
parameter <b>useAxisFlange</b>. The enabled axis flange can be
driven with elements of the
<a href=\"modelica://Modelica.Mechanics.Rotational\">Modelica.Mechanics.Rotational</a>
library.

</p>

<p>
In the \"Advanced\" menu it can be defined via parameter <b>stateSelect</b>
that the rotation angle \"phi\" and its derivative shall be definitely
used as states by setting stateSelect=StateSelect.always.
Default is StateSelect.prefer to use the joint angle and its
derivative as preferred states. The states are usually selected automatically.
In certain situations, especially when closed kinematic loops are present,
it might be slightly more efficient, when using the StateSelect.always setting.
</p>
<p>
If a <b>planar loop</b> is present, e.g., consisting of 4 revolute joints
where the joint axes are all parallel to each other, then there is no
longer a unique mathematical solution and the symbolic algorithms will
fail. Usually, an error message will be printed pointing out this
situation. In this case, one revolute joint of the loop has to be replaced
by a Joints.RevolutePlanarLoopConstraint joint. The
effect is that from the 5 constraints of a usual revolute joint,
3 constraints are removed and replaced by appropriate known
variables (e.g., the force in the direction of the axis of rotation is
treated as known with value equal to zero; for standard revolute joints,
this force is an unknown quantity).
</p>

<p>
In the following figure the animation of a revolute
joint is shown. The light blue coordinate system is
frame_a and the dark blue coordinate system is
frame_b of the joint. The black arrow is parameter
vector \"n\" defining the translation axis
(here: n = {0,0,1}, phi.start = 45<sup>o</sup>).
</p>

<IMG src=\"modelica://Modelica/Resources/Images/MultiBody/Joints/Revolute.png\">

</html>
"));
    end Revolute_withDamping;

    model DoubleActingCylinder
      import Modelica.Constants.pi;

      // the parameters
      parameter SI.Length boreDiameter = 0.05 "Bore diameter"
        annotation (Dialog(tab="Sizing",group="Dimensions"));
      parameter SI.Length rodDiameter = 0.01 "Rod diameter"
        annotation (Dialog(tab="Sizing",group="Dimensions"));
      parameter SI.Length strokeLength = 0.1 "Stroke length of the cylinder"
        annotation (Dialog(tab="Sizing",group="Dimensions"));
      parameter SI.Length closedLength = 0.3
      "Total length of cylinder fully retracted"
        annotation (Dialog(tab="Sizing",group="Dimensions"));
      parameter SI.VolumeFlowRate q_nom = 0.01
      "Nominal flow rate for in/outlet"
        annotation (Dialog(tab="Sizing",group="Hydraulics"));
      parameter SI.Pressure dp_nom = 1e4 "Nominal pressure drop for q_nom"
        annotation (Dialog(tab="Sizing",group="Hydraulics"));
      parameter SI.AbsolutePressure maxPressure = 3e7 "Maximum rated pressure"
        annotation (Dialog(tab="Sizing",group="Hydraulics"));

      // dynamics parameters
      parameter SI.Mass pistonMass = 0 "Mass of the piston and rod"
        annotation (Dialog(tab="Dynamics"));
      parameter Real damping( final unit="N/ (m/s)",
        final min=0) = 1e4 "damping between piston and cylinder [N/ (m/s)]"
        annotation (Dialog(tab="Dynamics"));

      parameter SI.Distance endOfTravelDistance = 0.01
      "Maximum distance beyond the end of travel point"
        annotation (Dialog(tab="Dynamics",group="End-of-travel"));
      parameter Real stopStiffness(
        final unit="N/m",
        final min=0) = 1e9 "stiffness at impact"
        annotation(Dialog(tab="Dynamics",group="End-of-travel"));
      parameter Real stopDamping(
        final unit="N.s/m",
        final min=-1000) = 1e12 "damping at impact"
        annotation(Dialog(tab="Dynamics",group="End-of-travel"));

      // cushion parameters
      parameter Boolean useCushionHead = true
      "false = constant restriction with q_nom & dp_nom"
        annotation(Evaluate=true, Dialog(tab="Cushions",group="Head Cushion"));
      parameter Real cushionTableHead[:, :]=[0,0.001;0.001,0.001;0.029,0.01;0.03,1]
      "Cushion flow rate (1st col = s_rel; 2nd col = fraction of q_nom)"
        annotation(Dialog(tab="Cushions",group="Head Cushion",enable=useCushionHead));
      parameter Modelica.Blocks.Types.Smoothness smoothnessHead=
        Modelica.Blocks.Types.Smoothness.LinearSegments
      "smoothness of table interpolation"
        annotation(Dialog(tab="Cushions",group="Head Cushion",enable=useCushionHead));
      parameter Boolean useCushionRod = true
      "false = constant restriction with q_nom & dp_nom"
        annotation(Evaluate=true, Dialog(tab="Cushions",group="Rod Cushion"));
      parameter Real cushionTableRod[:, :]=[0,0.001;0.001,0.001;0.029,0.01;0.03,1]
      "Cushion flow rate (1st col = s_rel; 2nd col = fraction of q_nom)"
        annotation(Dialog(tab="Cushions",group="Rod Cushion",enable=useCushionRod));
      parameter Modelica.Blocks.Types.Smoothness smoothnessRod=
        Modelica.Blocks.Types.Smoothness.LinearSegments
      "smoothness of table interpolation"
        annotation(Dialog(tab="Cushions",group="Rod Cushion",enable=useCushionRod));

      // sealing parameters
      parameter SI.Length L_A2B = 0.01
      "Length of seal between chambers A and B"
        annotation (Dialog(tab="Seals",group="Piston"));
      parameter SI.Diameter D_A2B = 1e-5
      "Hydraulic diameter of seal between chambers A and B"
        annotation (Dialog(tab="Seals",group="Piston"));
      parameter SI.Length L_A2Env = 0.01
      "Length of seal between chamber A and Environment"
        annotation (Dialog(tab="Seals",group="Piston"));
      parameter SI.Diameter D_A2Env = 0
      "Hydraulic diameter of seal between chamber A and Environment"
        annotation (Dialog(tab="Seals",group="Piston"));
      parameter SI.Length L_B2Env = 0.01
      "Length of seal between chamber B and Environment"
        annotation (Dialog(tab="Seals",group="Piston"));
      parameter SI.Diameter D_B2Env = 0
      "Hydraulic diameter of seal between chamber B and Environment"
        annotation (Dialog(tab="Seals",group="Piston"));

      // initialization parameters
      parameter Modelica.Mechanics.MultiBody.Types.Init initType=
          Modelica.Mechanics.MultiBody.Types.Init.Free
      "Type of initialization (defines usage of start values below)"
        annotation (Dialog(tab="Initialization",group="Mechanical"));
      parameter SI.Distance s_init = 0 "Initial position >0 and <stroke"
        annotation (Dialog(tab="Initialization",group="Mechanical"));
      parameter SI.Velocity v_init = 0 "Initial velocity"
        annotation (Dialog(tab="Initialization",group="Mechanical"));
      parameter SI.Acceleration a_init = 0 "Initial acceleration"
        annotation (Dialog(tab="Initialization",group="Mechanical"));
      parameter Boolean fixHeadPressure = false
      "Initialize the pressure at the head side"
        annotation (Dialog(tab="Initialization",group="Fluid"));
      parameter Boolean fixRodPressure = false
      "Initialize the pressure at the rod side"
        annotation (Dialog(tab="Initialization",group="Fluid"));

      // the connectors
      OpenHydraulics.Interfaces.FluidPort port_a
        annotation (Placement(transformation(extent={{-90,-90},{-70,-70}},
              rotation=0)));
      OpenHydraulics.Interfaces.FluidPort port_b
        annotation (Placement(transformation(extent={{90,-90},{70,-70}},
              rotation=0)));

      // the components
      OpenHydraulics.Basic.FluidPower2MechTrans cylinderChamberHead(
        A=pi/4*boreDiameter^2,
        stopStiffness=stopStiffness,
        stopDamping=stopDamping,
        n_ports=3,
        p_init=p_init,
        maxPressure=maxPressure*10)
        annotation (Placement(transformation(extent={{-50,-10},{-30,10}},
              rotation=0)));

      OpenHydraulics.Basic.FluidPower2MechTrans cylinderChamberRod(
        A=pi/4*(boreDiameter^2 - rodDiameter^2),
        stopStiffness=stopStiffness,
        stopDamping=stopDamping,
        n_ports=3,
        p_init=p_init,
        maxPressure=maxPressure*10)
        annotation (Placement(transformation(extent={{30,-10},{50,10}},
              rotation=0)));
      OpenHydraulics.Components.Cylinders.BaseClasses.CylinderCushion cushionHead(
        cushionTable=if useCushionHead then cushionTableHead else [0,0.001;
            strokeLength/1000,1; 1,1],
        smoothness=smoothnessHead,
        q_nom=q_nom,
        dp_nom=dp_nom,
        dp_relief=maxPressure*0.9)
        annotation (Placement(transformation(extent={{-50,-60},{-30,-40}},
              rotation=0)));
      OpenHydraulics.Components.Cylinders.BaseClasses.CylinderCushion cushionRod(
        cushionTable=if useCushionRod then cushionTableRod else [0,0.001;
            strokeLength/1000,1; 1,1],
        smoothness=smoothnessHead,
        q_nom=q_nom,
        dp_nom=dp_nom,
        dp_relief=maxPressure*0.9)
        annotation (Placement(transformation(extent={{30,-60},{50,-40}},
              rotation=0)));
      OpenHydraulics.Basic.LaminarRestriction leakage_Head2Rod(L=L_A2B, D=D_A2B)
        annotation (Placement(transformation(extent={{-10,-30},{10,-10}},
              rotation=0)));
      Modelica.Mechanics.Translational.Components.Mass piston(
                                                          m=pistonMass)
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=0)));
      Modelica.Mechanics.Translational.Components.Rod cylinder(
                                                    L=strokeLength)
        annotation (Placement(transformation(extent={{-10,70},{10,90}},
              rotation=0)));
      Modelica.Mechanics.Translational.Components.Rod rod(
                                               L=closedLength)
        annotation (Placement(transformation(extent={{70,-10},{90,10}},
              rotation=0)));
      Modelica.Mechanics.Translational.Interfaces.Flange_a flange_a
      "(left) driving flange (flange axis directed INTO cut plane, e. g. from left to right)"
        annotation (Placement(transformation(extent={{-110,-10},{-90,10}},
              rotation=0)));
      Modelica.Mechanics.Translational.Interfaces.Flange_b flange_b
      "(right) driven flange (flange axis directed OUT OF cut plane, i. e. from right to left)"
        annotation (Placement(transformation(extent={{90,-10},{110,10}},
              rotation=0)));
      Modelica.Mechanics.Translational.Components.Damper damper(
                                                     d=damping)
        annotation (Placement(transformation(extent={{-48,58},{-28,78}},
              rotation=0)));
      OpenHydraulics.Basic.ConstPressureSource envSinkA
        annotation (Placement(transformation(
            origin={-90,-20},
            extent={{-10,-10},{10,10}},
            rotation=270)));
      OpenHydraulics.Basic.ConstPressureSource envSinkB
        annotation (Placement(transformation(
            origin={90,-20},
            extent={{-10,-10},{10,10}},
            rotation=90)));
      OpenHydraulics.Basic.LaminarRestriction leakage_Head2Env(L=L_A2Env, D=D_A2Env)
        annotation (Placement(transformation(extent={{-74,-30},{-54,-10}},
              rotation=0)));
      OpenHydraulics.Basic.LaminarRestriction leakage_Rod2Env(L=L_B2Env, D=D_B2Env)
        annotation (Placement(transformation(
            origin={64,-20},
            extent={{-10,-10},{10,10}},
            rotation=180)));
      OpenHydraulics.Components.Lines.NJunction jA(n_ports=2)
        annotation (Placement(transformation(extent={{-50,-90},{-30,-70}},
              rotation=0)));
      OpenHydraulics.Components.Lines.NJunction jB(n_ports=2)
        annotation (Placement(transformation(extent={{30,-90},{50,-70}},
              rotation=0)));

      extends OpenHydraulics.Interfaces.PartialFluidComponent;

  protected
      outer OpenHydraulics.Circuits.Environment environment;

  public
    Modelica.Blocks.Interfaces.RealOutput strokeLength_unit= cylinderChamberHead.s_rel/strokeLength annotation (Placement(transformation(extent={{-100,76},{-120,96}})));
    initial equation
      assert(cylinderChamberHead.s_rel>=0,"Initial position is smaller than zero");
      assert(cylinderChamberRod.s_rel>=0,"Initial position is larger than strokeLength");

      // state initialization
      if initType == Modelica.Mechanics.MultiBody.Types.Init.Position then
        cylinderChamberHead.s_rel = s_init;
      elseif initType == Modelica.Mechanics.MultiBody.Types.Init.Velocity then
        cylinderChamberHead.v_rel = v_init;
      elseif initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocity then
        cylinderChamberHead.s_rel = s_init;
        cylinderChamberHead.v_rel = v_init;
      elseif initType == Modelica.Mechanics.MultiBody.Types.Init.VelocityAcceleration then
        cylinderChamberHead.v_rel = v_init;
        piston.a = a_init;
      elseif initType == Modelica.Mechanics.MultiBody.Types.Init.SteadyState then
        cylinderChamberHead.v_rel = 0;
        piston.a = a_init;
      elseif initType == Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration then
        cylinderChamberHead.s_rel = s_init;
        cylinderChamberHead.v_rel = v_init;
        piston.a = a_init;
      elseif initType == Modelica.Mechanics.MultiBody.Types.Init.Free then
        // nothing
      else
        assert(true,"Invalid initialization type in FluidPower2MechTrans");
      end if;

      if fixHeadPressure then
        cylinderChamberHead.p_vol = p_init;
      end if;
      if fixRodPressure then
        cylinderChamberRod.p_vol = p_init;
      end if;

    equation
      when cushionRod.reliefValve.valvePositionSteadyState>0 then
        Modelica.Utilities.Streams.print("\nWARNING: Cylinder exceeds maximum pressure at the rod end.");
        Modelica.Utilities.Streams.print("         This could just be due to end-of-travel behavior.");
        Modelica.Utilities.Streams.print("         (time = "+String(time)+")");
      end when;
      when cushionHead.reliefValve.valvePositionSteadyState>0 then
        Modelica.Utilities.Streams.print("\nWARNING: Cylinder exceeds maximum pressure at the head end.");
        Modelica.Utilities.Streams.print("         This could just be due to end-of-travel behavior.");
        Modelica.Utilities.Streams.print("         (time = "+String(time)+")");
      end when;

      connect(cylinderChamberHead.flange_b, piston.flange_a)
        annotation (Line(points={{-30,0},{-10,0}}, color={0,127,0}));
      connect(rod.flange_a, piston.flange_b)
        annotation (Line(points={{70,0},{70,20},{62,20},{62,22},{58,22},{58,20},
              {14,20},{14,0},{10,0}}, color={0,127,0}));
      connect(piston.flange_b, cylinderChamberRod.flange_a)
        annotation (Line(points={{10,0},{30,0}}, color={0,127,0}));
      connect(cylinder.flange_b, cylinderChamberRod.flange_b)
        annotation (Line(points={{10,80},{60,80},{60,0},{50,0}}, color={0,127,0}));
      connect(cylinderChamberHead.flange_a, cylinder.flange_a)
        annotation (Line(points={{-50,0},{-60,0},{-60,80},{-10,80}}, color={0,
              127,0}));
      connect(cylinderChamberHead.flange_a, flange_a)
        annotation (Line(points={{-50,0},{-100,0}}, color={0,127,0}));
      connect(rod.flange_b, flange_b)
        annotation (Line(points={{90,0},{100,0}}, color={0,127,0}));
      connect(flange_a, damper.flange_a)
        annotation (Line(points={{-100,0},{-80,0},{-80,68},{-62,68},{-62,70},{
              -58,70},{-58,68},{-48,68}}, color={0,127,0}));
      connect(damper.flange_b,cylinderChamberHead. flange_b)
        annotation (Line(points={{-28,68},{-14,68},{-14,0},{-30,0}}, color={0,
              127,0}));
      connect(envSinkA.port, leakage_Head2Env.port_a)
        annotation (Line(points={{-80,-20},{-78,-20},{-74,-20}},
            color={255,0,0}));
      connect(envSinkB.port, leakage_Rod2Env.port_a)
        annotation (Line(points={{80,-20},{74,-20}}, color={255,0,0}));
      connect(port_a,jA. port[1]) annotation (Line(points={{-80,-80},{-40,-80},
              {-40,-80.5}}, color={255,0,0}));
      connect(port_b,jB. port[1]) annotation (Line(points={{80,-80},{40,-80},{
              40,-80.5}}, color={255,0,0}));

      connect(cylinderChamberHead.port[1], cushionHead.port_a) annotation (Line(
            points={{-40,-0.75},{-40,-40}}, color={255,0,0}));

      connect(cushionHead.port_b, jA.port[2]) annotation (Line(points={{-40,-60},
              {-40,-79.5}}, color={255,0,0}));
      connect(cylinderChamberHead.flange_a, cushionHead.flange_a) annotation (Line(
            points={{-50,0},{-50,-50}}, color={0,127,0}));
      connect(cylinderChamberHead.flange_b, cushionHead.flange_b) annotation (Line(
            points={{-30,0},{-30,-50}}, color={0,127,0}));
      connect(cylinderChamberRod.flange_a, cushionRod.flange_a) annotation (Line(
            points={{30,0},{30,-50}}, color={0,127,0}));
      connect(cylinderChamberRod.flange_b, cushionRod.flange_b) annotation (Line(
            points={{50,0},{50,-50}}, color={0,127,0}));
      connect(cushionRod.port_b, jB.port[2]) annotation (Line(points={{40,-60},
              {40,-79.5}}, color={255,0,0}));
      connect(cylinderChamberRod.port[1], cushionRod.port_a) annotation (Line(
            points={{40,-0.75},{40,-40}}, color={255,0,0}));
      connect(leakage_Head2Rod.port_b, cylinderChamberRod.port[2]) annotation (Line(
            points={{10,-20},{38,-20},{38,0},{40,0},{40,-0.05}}, color={255,0,0}));
      connect(leakage_Head2Rod.port_a, cylinderChamberHead.port[2]) annotation (Line(
            points={{-10,-20},{-38,-20},{-38,0},{-40,0},{-40,-0.05}}, color={
              255,0,0}));
      connect(leakage_Head2Env.port_b, cylinderChamberHead.port[3]) annotation (Line(
            points={{-54,-20},{-42,-20},{-42,0},{-40,0},{-40,0.65}}, color={255,
              0,0}));
      connect(leakage_Rod2Env.port_b, cylinderChamberRod.port[3]) annotation (Line(
            points={{54,-20},{42,-20},{42,0},{40,0},{40,0.65}}, color={255,0,0}));
      annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
                -100},{100,100}}),
                          graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-100,-100},{100,100}}), graphics={
            Rectangle(
              extent={{-90,80},{90,-90}},
              lineColor={255,255,255},
              fillColor={255,255,255},
              fillPattern=FillPattern.Solid),
            Rectangle(
              extent={{-90,40},{90,-40}},
              lineColor={0,0,0},
              fillColor={255,255,255},
              fillPattern=FillPattern.Solid),
            Rectangle(
              extent={{100,10},{0,-10}},
              lineColor={0,0,0},
              fillColor={255,255,255},
              fillPattern=FillPattern.Solid),
            Rectangle(
              extent={{-18,39},{0,-39}},
              lineColor={0,0,0},
              fillColor={255,255,255},
              fillPattern=FillPattern.Solid),
            Line(points={{-80,-40},{-80,-40},{-80,-40},{-80,-80}}, color={255,0,
                  0}),
            Line(points={{80,-40},{80,-40},{80,-40},{80,-78}}, color={255,0,0}),
            Polygon(
              points={{-88,-40},{-80,-30},{-72,-40},{-88,-40}},
              lineColor={0,0,0},
              fillColor={0,0,0},
              fillPattern=FillPattern.Solid),
            Polygon(
              points={{72,-40},{80,-30},{88,-40},{72,-40}},
              lineColor={0,0,0},
              fillColor={0,0,0},
              fillPattern=FillPattern.Solid),
            Text(
              extent={{-64,-56},{-34,-96}},
              lineColor={0,0,0},
              fillColor={0,0,0},
              fillPattern=FillPattern.Solid,
              textString="A"),
            Text(
              extent={{34,-56},{64,-96}},
              lineColor={0,0,0},
              fillColor={0,0,0},
              fillPattern=FillPattern.Solid,
              textString="B"),
            Text(
              extent={{0,84},{0,60}},
              lineColor={0,0,255},
              fillColor={0,0,0},
              fillPattern=FillPattern.Solid,
              textString="%name"),
            Rectangle(
              extent={{-34,18},{16,-18}},
              lineColor={0,0,0},
              fillColor={255,255,255},
              fillPattern=FillPattern.Solid),
            Line(points={{-22,-26},{8,34}}, color={0,0,0}),
            Polygon(
              points={{8,34},{-8,18},{4,12},{8,34}},
              lineColor={0,0,0},
              fillColor={0,0,0},
              fillPattern=FillPattern.Solid)}));
    end DoubleActingCylinder;

    model BucketAngle
    Modelica.Mechanics.MultiBody.Sensors.AbsoluteAngles absoluteAngles
        annotation (Placement(transformation(extent={{272,250},{292,270}})));
    Modelica.Blocks.Interfaces.RealOutput BucketAngle
      annotation (Placement(transformation(extent={{400,250},{420,270}})));
    Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation (
        Placement(transformation(rotation=0, extent={{190,250},{210,270}})));
    equation
    connect(frame_a, absoluteAngles.frame_a) annotation (Line(points={{200,260},
            {202,260},{272,260}}, color={95,95,95}));
    connect(absoluteAngles.angles[3], BucketAngle) annotation (Line(
        points={{293,260.667},{354,260.667},{354,260},{410,260}},
        color={0,0,127},
        smooth=Smooth.None));
    annotation (Diagram(coordinateSystem(extent={{200,160},{400,360}},
            preserveAspectRatio=false), graphics), Icon(coordinateSystem(extent=
             {{200,160},{400,360}})));
    end BucketAngle;

    model WorldForce
    "External force acting at frame_b, defined by 3 input signals and resolved in frame world, frame_b or frame_resolve"

      import SI = Modelica.SIunits;
      extends Modelica.Mechanics.MultiBody.Interfaces.PartialOneFrame_b;
      Modelica.Mechanics.MultiBody.Interfaces.Frame_resolve frame_resolve if
      resolveInFrame == Modelica.Mechanics.MultiBody.Types.ResolveInFrameB.frame_resolve
      "The input signals are optionally resolved in this frame"
        annotation (Placement(transformation(
            origin={0,-100},
            extent={{-16,-16},{16,16}},
            rotation=270)));

      parameter Boolean animation=true "= true, if animation shall be enabled";
      parameter Modelica.Mechanics.MultiBody.Types.ResolveInFrameB resolveInFrame=
        Modelica.Mechanics.MultiBody.Types.ResolveInFrameB.world
      "Frame in which input force is resolved (1: world, 2: frame_b, 3: frame_resolve)";
      parameter Real N_to_m(unit="N/m") = world.defaultN_to_m
      "Force arrow scaling (length = force/N_to_m)"
        annotation (Dialog(group="if animation = true", enable=animation));
      input SI.Diameter diameter=world.defaultArrowDiameter
      "Diameter of force arrow"   annotation (Dialog(group="if animation = true", enable=animation));
      input Modelica.Mechanics.MultiBody.Types.Color color=Modelica.Mechanics.MultiBody.Types.Defaults.ForceColor
      "Color of arrow"
        annotation (Dialog(group="if animation = true", enable=animation));
      input Modelica.Mechanics.MultiBody.Types.SpecularCoefficient specularCoefficient=world.defaultSpecularCoefficient
      "Reflection of ambient light (= 0: light is completely absorbed)"
        annotation (Dialog(group="if animation = true", enable=animation));

  protected
      SI.Position f_in_m[3]=frame_b.f/N_to_m
      "Force mapped from N to m for animation";
      Modelica.Mechanics.MultiBody.Visualizers.Advanced.Arrow arrow(
      diameter=diameter,
      color=color,
      specularCoefficient=specularCoefficient,
      R=frame_b.R,
      r=frame_b.r_0,
      r_tail=f_in_m,
      r_head=-f_in_m) if world.enableAnimation and animation;

  public
      Modelica.Mechanics.MultiBody.Forces.Internal.BasicWorldForce basicWorldForce(
        resolveInFrame=resolveInFrame)
        annotation (Placement(transformation(extent={{-10,-10},{10,10}})));
  protected
      Modelica.Mechanics.MultiBody.Interfaces.ZeroPosition zeroPosition if not (
      resolveInFrame == Modelica.Mechanics.MultiBody.Types.ResolveInFrameB.frame_resolve)
        annotation (Placement(transformation(extent={{20,-40},{40,-20}})));
  public
      Modelica.Blocks.Interfaces.RealInput xForce
        annotation (Placement(transformation(extent={{-128,40},{-88,80}})));
      Modelica.Blocks.Interfaces.RealInput yForce
        annotation (Placement(transformation(extent={{-128,-20},{-88,20}})));
      Modelica.Blocks.Interfaces.RealInput zForce
        annotation (Placement(transformation(extent={{-128,-80},{-88,-40}})));
    equation
      connect(basicWorldForce.frame_b, frame_b) annotation (Line(
          points={{10,0},{100,0}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));

      //connect(basicWorldForce.force, force) annotation (Line(points={{-12,0},{-120,0}}, color={0,0,127}, smooth=Smooth.None));
      connect(basicWorldForce.frame_resolve, frame_resolve) annotation (Line(
          points={{0,-10},{0,-100}},
          color={95,95,95},
          pattern=LinePattern.Dot,
          smooth=Smooth.None));
      connect(zeroPosition.frame_resolve, basicWorldForce.frame_resolve)
        annotation (Line(
          points={{20,-30},{0,-30},{0,-10}},
          color={95,95,95},
          pattern=LinePattern.Dot,
          smooth=Smooth.None));
      connect(xForce, basicWorldForce.force[1]) annotation (Line(
          points={{-108,60},{-56,60},{-56,-1.33333},{-12,-1.33333}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(yForce, basicWorldForce.force[2]) annotation (Line(
          points={{-108,0},{-12,0}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(zForce, basicWorldForce.force[3]) annotation (Line(
          points={{-108,-60},{-53,-60},{-53,1.33333},{-12,1.33333}},
          color={0,0,127},
          smooth=Smooth.None));
      annotation (defaultComponentName="force",
        Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,-100},{100,
                100}}),     graphics),
        Icon(coordinateSystem(preserveAspectRatio=true, extent={{-100,-100},{100,
                100}}), graphics={
            Text(
              extent={{-100,-46},{100,-76}},
              lineColor={192,192,192},
              textString="resolve"),
            Polygon(
              points={{-100,10},{50,10},{50,31},{94,0},{50,-31},{50,-10},{-100,-10},
                  {-100,10}},
              lineColor={0,0,0},
              fillColor={0,0,0},
              fillPattern=FillPattern.Solid),
            Text(
              extent={{-150,85},{150,45}},
              textString="%name",
              lineColor={0,0,255}),
            Line(
              points={{0,-10},{0,-95}},
              color={95,95,95},
              pattern=LinePattern.Dot)}),
        Documentation(info="<HTML>

<p>
The <b>3</b> signals of the <b>force</b> connector are interpreted
as the x-, y- and z-coordinates of a <b>force</b> acting at the frame
connector to which frame_b of this component is attached.
Via parameter <b>resolveInFrame</b> it is defined, in which frame these
coordinates shall be resolved:
</p>

<table border=1 cellspacing=0 cellpadding=2>
<tr><th><b>Types.ResolveInFrameB.</b></th><th><b>Meaning</b></th></tr>
<tr><td valign=\"top\">world</td>
    <td valign=\"top\">Resolve input force in world frame (= default)</td></tr>

<tr><td valign=\"top\">frame_b</td>
    <td valign=\"top\">Resolve input force in frame_b</td></tr>

<tr><td valign=\"top\">frame_resolve</td>
    <td valign=\"top\">Resolve input force in frame_resolve (frame_resolve must be connected)</td></tr>
</table>

<p>
If resolveInFrame = Types.ResolveInFrameB.frame_resolve, the force coordinates
are with respect to the frame, that is connected to <b>frame_resolve</b>.
<p>

<p>
If force={100,0,0}, and for all parameters the default setting is used,
then the interpretation is that a force of 100 N is acting along the positive
x-axis of frame_b.
</p>

<p>
Note, the cut-torque in frame_b (frame_b.t) is always set to zero.
Conceptually, a force and torque acts on the world frame in such a way that
the force and torque balance between world.frame_b and frame_b is fulfilled.
For efficiency reasons, this reaction torque is, however, not computed.
</p>

<p>
This force component is by default visualized as an arrow
acting at the connector to which it is connected. The diameter
and color of the arrow can be defined via
variables <b>diameter</b> and <b>color</b>. The arrow
points in the direction defined by the
force signal. The length of the arrow is proportional
to the length of the force vector using parameter
<b>N_to_m</b> as scaling factor. For example, if N_to_m = 100 N/m,
then a force of 350 N is displayed as an arrow of length 3.5 m.
</p>
<p>
An example how to use this model is given in the
following figure:
</p>

<IMG src=\"modelica://Modelica/Resources/Images/MultiBody/Forces/WorldForce1.png\">

<p>
This leads to the following animation
</p>

<IMG src=\"modelica://Modelica/Resources/Images/MultiBody/Forces/WorldForce2.png\">

</HTML>
"));
    end WorldForce;

    model CombiTable1Ds
    "Table look-up in one dimension (matrix/file) with one input and n outputs"

      import Modelica.Blocks.Types;
      parameter Boolean tableOnFile=false
      "true, if table is defined on file or in function usertab"
        annotation(Dialog(group="table data definition"));
      parameter Real table[:, :]=fill(0.0,0,2)
      "table matrix (grid = first column; e.g., table=[0,2])"
           annotation(Dialog(group="table data definition", enable = not tableOnFile));
      parameter String tableName="NoName"
      "table name on file or in function usertab (see docu)"
           annotation(Dialog(group="table data definition", enable = tableOnFile));
      parameter String fileName="NoName" "file where matrix is stored"
           annotation(Dialog(group="table data definition", enable = tableOnFile,
                             __Dymola_loadSelector(filter="Text files (*.txt);;Matlab files (*.mat)",
                             caption="Open file in which table is present")));
      parameter Integer columns[:]=2:size(table, 2)
      "columns of table to be interpolated"
      annotation(Dialog(group="table data interpretation"));
      parameter Modelica.Blocks.Types.Smoothness smoothness=Types.Smoothness.LinearSegments
      "smoothness of table interpolation"
      annotation(Dialog(group="table data interpretation"));
      extends Modelica.Blocks.Interfaces.SIMO(final nout=size(columns, 1));

  protected
      Integer tableID;

      function tableInit
      "Initialize 1-dim. table defined by matrix (for details see: Modelica/Resources/C-Sources/ModelicaTables.h)"
        input String tableName;
        input String fileName;
        input Real table[ :, :];
        input Modelica.Blocks.Types.Smoothness smoothness;
        output Integer tableID;
      external "C" tableID = ModelicaTables_CombiTable1D_init(
                     tableName, fileName, table, size(table, 1), size(table, 2),
                     smoothness);
        annotation(Library="ModelicaExternalC");
      end tableInit;

      function tableIpo
      "Interpolate 1-dim. table defined by matrix (for details see: Modelica/Resources/C-Sources/ModelicaTables.h)"
        input Integer tableID;
        input Integer icol;
        input Real u;
        output Real value;
      external "C" value =
                         ModelicaTables_CombiTable1D_interpolate(tableID, icol, u);
        annotation(Library="ModelicaExternalC");
      end tableIpo;

  public
    Modelica.Blocks.Interfaces.RealOutput y1
      annotation (Placement(transformation(extent={{114,-20},{154,20}})));
    equation
      if tableOnFile then
        assert(tableName<>"NoName", "tableOnFile = true and no table name given");
      end if;
      if not tableOnFile then
        assert(size(table,1) > 0 and size(table,2) > 0, "tableOnFile = false and parameter table is an empty matrix");
      end if;

      for i in 1:nout loop
        y[i] = if not tableOnFile and size(table,1)==1 then
                 table[1, columns[i]] else tableIpo(tableID, columns[i], u);
      end for;
      when initial() then
        tableID=tableInit(if tableOnFile then tableName else "NoName",
                          if tableOnFile then fileName else "NoName", table, smoothness);
      end when;
    connect(y[1], y1) annotation (Line(
        points={{110,0},{134,0}},
        color={0,0,127},
        smooth=Smooth.None));
      annotation (
        Documentation(info="<html>
<p>
<b>Linear interpolation</b> in <b>one</b> dimension of a <b>table</b>.
Via parameter <b>columns</b> it can be defined how many columns of the
table are interpolated. If, e.g., icol={2,4}, it is assumed that one input
and 2 output signals are present and that the first output interpolates
via column 2 and the second output interpolates via column 4 of the
table matrix.
</p>
<p>
The grid points and function values are stored in a matrix \"table[i,j]\",
where the first column \"table[:,1]\" contains the grid points and the
other columns contain the data to be interpolated. Example:
</p>
<pre>
   table = [0,  0;
            1,  1;
            2,  4;
            4, 16]
   If, e.g., the input u = 1.0, the output y =  1.0,
       e.g., the input u = 1.5, the output y =  2.5,
       e.g., the input u = 2.0, the output y =  4.0,
       e.g., the input u =-1.0, the output y = -1.0 (i.e., extrapolation).
</pre>
<ul>
<li> The interpolation is <b>efficient</b>, because a search for a new interpolation
     starts at the interval used in the last call.</li>
<li> If the table has only <b>one row</b>, the table value is returned,
     independent of the value of the input signal.</li>
<li> If the input signal <b>u</b> is <b>outside</b> of the defined <b>interval</b>, i.e.,
     u &gt; table[size(table,1),1] or u &lt; table[1,1], the corresponding
     value is also determined by linear
     interpolation through the last or first two points of the table.</li>
<li> The grid values (first column) have to be <b>strict</b>
     monotonically increasing.</li>
</ul>
<p>
The table matrix can be defined in the following ways:
</p>
<ol>
<li> Explicitly supplied as <b>parameter matrix</b> \"table\",
     and the other parameters have the following values:
<pre>
   tableName is \"NoName\" or has only blanks,
   fileName  is \"NoName\" or has only blanks.
</pre></li>
<li> <b>Read</b> from a <b>file</b> \"fileName\" where the matrix is stored as
      \"tableName\". Both ASCII and binary file format is possible.
      (the ASCII format is described below).
      It is most convenient to generate the binary file from Matlab
      (Matlab 4 storage format), e.g., by command
<pre>
   save tables.mat tab1 tab2 tab3 -V4
</pre>
      when the three tables tab1, tab2, tab3 should be
      used from the model.</li>
<li>  Statically stored in function \"usertab\" in file \"usertab.c\".
      The matrix is identified by \"tableName\". Parameter
      fileName = \"NoName\" or has only blanks.</li>
</ol>
<p>
Table definition methods (1) and (3) do <b>not</b> allocate dynamic memory,
and do not access files, whereas method (2) does. Therefore (1) and (3)
are suited for hardware-in-the-loop simulation (e.g., with dSpace hardware).
When the constant \"NO_FILE\" is defined, all parts of the
source code of method (2) are removed by the C-preprocessor, such that
no dynamic memory allocation and no access to files takes place.
</p>
<p>
If tables are read from an ASCII-file, the file need to have the
following structure (\"-----\" is not part of the file content):
</p>
<pre>
-----------------------------------------------------
#1
double tab1(5,2)   # comment line
  0   0
  1   1
  2   4
  3   9
  4  16
double tab2(5,2)   # another comment line
  0   0
  2   2
  4   8
  6  18
  8  32
-----------------------------------------------------
</pre>
<p>
Note, that the first two characters in the file need to be
\"#1\". Afterwards, the corresponding matrix has to be declared
with type, name and actual dimensions. Finally, in successive
rows of the file, the elements of the matrix have to be given.
Several matrices may be defined one after another.
</p>
</HTML>
"),     Icon(coordinateSystem(preserveAspectRatio=false,extent={{-100,-100},{
              100,100}}),
                        graphics={
            Line(points={{-60,40},{-60,-40},{60,-40},{60,40},{30,40},{30,-40},{-30,
                  -40},{-30,40},{-60,40},{-60,20},{60,20},{60,0},{-60,0},{-60,-20},
                  {60,-20},{60,-40},{-60,-40},{-60,40},{60,40},{60,-40}}, color={
                  0,0,0}),
            Line(points={{0,40},{0,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-60,40},{-30,20}},
              lineColor={0,0,0},
              fillColor={255,255,0},
              fillPattern=FillPattern.Solid),
            Rectangle(
              extent={{-60,20},{-30,0}},
              lineColor={0,0,0},
              fillColor={255,255,0},
              fillPattern=FillPattern.Solid),
            Rectangle(
              extent={{-60,0},{-30,-20}},
              lineColor={0,0,0},
              fillColor={255,255,0},
              fillPattern=FillPattern.Solid),
            Rectangle(
              extent={{-60,-20},{-30,-40}},
              lineColor={0,0,0},
              fillColor={255,255,0},
              fillPattern=FillPattern.Solid)}),
        Diagram(coordinateSystem(preserveAspectRatio=false,extent={{-100,-100},
              {100,100}}),  graphics={
            Rectangle(
              extent={{-60,60},{60,-60}},
              fillColor={235,235,235},
              fillPattern=FillPattern.Solid,
              lineColor={0,0,255}),
            Line(points={{-100,0},{-58,0}}, color={0,0,255}),
            Line(points={{60,0},{100,0}}, color={0,0,255}),
            Text(
              extent={{-100,100},{100,64}},
              textString="1 dimensional linear table interpolation",
              lineColor={0,0,255}),
            Line(points={{-54,40},{-54,-40},{54,-40},{54,40},{28,40},{28,-40},{-28,
                  -40},{-28,40},{-54,40},{-54,20},{54,20},{54,0},{-54,0},{-54,-20},
                  {54,-20},{54,-40},{-54,-40},{-54,40},{54,40},{54,-40}}, color={
                  0,0,0}),
            Line(points={{0,40},{0,-40}}, color={0,0,0}),
            Rectangle(
              extent={{-54,40},{-28,20}},
              lineColor={0,0,0},
              fillColor={255,255,0},
              fillPattern=FillPattern.Solid),
            Rectangle(
              extent={{-54,20},{-28,0}},
              lineColor={0,0,0},
              fillColor={255,255,0},
              fillPattern=FillPattern.Solid),
            Rectangle(
              extent={{-54,0},{-28,-20}},
              lineColor={0,0,0},
              fillColor={255,255,0},
              fillPattern=FillPattern.Solid),
            Rectangle(
              extent={{-54,-20},{-28,-40}},
              lineColor={0,0,0},
              fillColor={255,255,0},
              fillPattern=FillPattern.Solid),
            Text(
              extent={{-52,56},{-34,44}},
              textString="u",
              lineColor={0,0,255}),
            Text(
              extent={{-22,54},{2,42}},
              textString="y[1]",
              lineColor={0,0,255}),
            Text(
              extent={{4,54},{28,42}},
              textString="y[2]",
              lineColor={0,0,255}),
            Text(
              extent={{0,-40},{32,-54}},
              textString="columns",
              lineColor={0,0,255})}));
    end CombiTable1Ds;
  end components;


  extends Modelica.Icons.Package;
  import SI = Modelica.SIunits;
  import Cv = Modelica.SIunits.Conversions;
  import NonSI = Modelica.SIunits.Conversions.NonSIunits;


  package TestComponents
    model DigCycleSeq
      import Modelica.Constants.pi;

      // the parameters
      parameter Modelica.SIunits.Frequency bandwidth=10;
      parameter Modelica.SIunits.Time startTime=1
      "Time before excavator starts moving";
      parameter Real swingAmplitude = -1 "Amplitude of swing command";
      parameter Real boomAmplitude1 = -0.9 "Amplitude of first boom command";
      parameter Real boomAmplitude2 = 0.5 "Amplitude of second boom command";
      parameter Real armAmplitude1 = -1 "Amplitude of arm command";
      parameter Real armAmplitude2 = 0.3 "Amplitude of bucket command";
    //  parameter Real armAmplitude2 = 1 "Amplitude of bucket command";
      parameter Real bucketAmplitude1 = -0.7 "Amplitude of bucket command";
      parameter Real bucketAmplitude2 = 0.45 "Amplitude of bucket command";
      parameter Real bucketAmplitude3 = -1 "Amplitude of bucket command";

      // the components
      Modelica.Blocks.Tables.CombiTable1D swingTimeTable(                   table=[0,
            0; 1,0; 13,0; 14,swingAmplitude; 17,swingAmplitude; 18,0; 20,0])
        annotation (Placement(transformation(
            origin={-30,60},
            extent={{10,-10},{-10,10}},
            rotation=180)));
      Modelica.Blocks.Tables.CombiTable1D boomTimeTable(
                               table=[0,0; 1,boomAmplitude1; 2,boomAmplitude1;
            3,0; 7,0; 8,boomAmplitude2; 11.5,boomAmplitude2; 12.5,0; 20,0])
        annotation (Placement(transformation(
            origin={-30,20},
            extent={{10,-10},{-10,10}},
            rotation=180)));
      Modelica.Blocks.Tables.CombiTable1D armTimeTable(table=[0,0;0.5,0; 1,
            armAmplitude1; 2,armAmplitude1; 3,0; 4.5,armAmplitude2; 6.5,
            armAmplitude2; 7.5,0; 20,0])
        annotation (Placement(transformation(
            origin={-30,-20},
            extent={{10,-10},{-10,10}},
            rotation=180)));
      Modelica.Blocks.Tables.CombiTable1D bucketTimeTable(table=[0,0;0.5,0; 1,bucketAmplitude1; 3,bucketAmplitude1; 4,
            0; 5,0; 6,bucketAmplitude2; 7,bucketAmplitude2; 9,0; 16,0; 16.3,
            bucketAmplitude3; 17,bucketAmplitude3; 18,0; 20,0])
        annotation (Placement(transformation(
            origin={-30,-60},
            extent={{10,-10},{-10,10}},
            rotation=180)));

      Modelica.Blocks.Continuous.SecondOrder swingFilter(w=2*pi*bandwidth)
        annotation (Placement(transformation(extent={{0,50},{20,70}},
              rotation=0)));
      Modelica.Blocks.Continuous.SecondOrder boomFilter(w=2*pi*bandwidth)
        annotation (Placement(transformation(extent={{0,10},{20,30}},
              rotation=0)));
      Modelica.Blocks.Continuous.SecondOrder armFilter(w=2*pi*bandwidth)
        annotation (Placement(transformation(extent={{0,-30},{20,-10}},
              rotation=0)));
      Modelica.Blocks.Continuous.SecondOrder bucketFilter(w=2*pi*bandwidth)
        annotation (Placement(transformation(extent={{0,-70},{20,-50}},
              rotation=0)));

      // auxiliary components
  protected
      SI.Time t "Input to tables";

  public
      Modelica.Blocks.Interfaces.RealOutput swing
      "Connector of Real output signal"
        annotation (Placement(transformation(extent={{96,50},{116,70}})));
      Modelica.Blocks.Interfaces.RealOutput boom
      "Connector of Real output signal"
        annotation (Placement(transformation(extent={{96,10},{116,30}})));
      Modelica.Blocks.Interfaces.RealOutput arm
      "Connector of Real output signal"
        annotation (Placement(transformation(extent={{96,-30},{116,-10}})));
      Modelica.Blocks.Interfaces.RealOutput bucket
      "Connector of Real output signal"
        annotation (Placement(transformation(extent={{96,-70},{116,-50}})));
    equation
      t = if noEvent(time>startTime) then time-startTime else 0;
      swingTimeTable.u[1] = t;
      boomTimeTable.u[1] = t;
      armTimeTable.u[1] = t;
      bucketTimeTable.u[1] = t;

      connect(swingTimeTable.y[1], swingFilter.u)
        annotation (Line(points={{-19,60},{-2,60}}, color={0,0,127}));
      connect(boomTimeTable.y[1], boomFilter.u)
        annotation (Line(points={{-19,20},{-2,20}}, color={0,0,127}));
      connect(armTimeTable.y[1], armFilter.u)
        annotation (Line(points={{-19,-20},{-2,-20}}, color={0,0,127}));
      connect(bucketTimeTable.y[1],bucketFilter. u)
        annotation (Line(points={{-19,-60},{-2,-60}}, color={0,0,127}));

      connect(swingFilter.y, swing) annotation (Line(
          points={{21,60},{106,60}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(boomFilter.y, boom) annotation (Line(
          points={{21,20},{106,20}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(armFilter.y, arm) annotation (Line(
          points={{21,-20},{106,-20}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(bucketFilter.y, bucket) annotation (Line(
          points={{21,-60},{106,-60}},
          color={0,0,127},
          smooth=Smooth.None));
      annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{
                -100,-100},{100,100}}),
                          graphics),
                           Icon(coordinateSystem(preserveAspectRatio=false,
              extent={{-100,-100},{100,100}}), graphics={
            Rectangle(
              extent={{-100,100},{100,-100}},
              lineColor={0,0,0},
              fillColor={255,255,255},
              fillPattern=FillPattern.Solid),
            Text(
              extent={{-74,60},{36,20}},
              lineColor={0,0,0},
              textString="Dig "),
            Text(
              extent={{-74,20},{36,-20}},
              lineColor={0,0,0},
              textString="Cycle"),
            Text(
              extent={{36,-12},{96,-28}},
              lineColor={0,0,0},
              fillColor={255,255,255},
              fillPattern=FillPattern.Solid,
              textString="Arm"),
            Text(
              extent={{96,28},{36,12}},
              lineColor={0,0,0},
              fillColor={255,255,255},
              fillPattern=FillPattern.Solid,
              textString="Boom"),
            Text(
              extent={{96,68},{36,52}},
              lineColor={0,0,0},
              fillColor={255,255,255},
              fillPattern=FillPattern.Solid,
              textString="Swing"),
            Text(
              extent={{36,-52},{96,-68}},
              lineColor={0,0,0},
              fillColor={255,255,255},
              fillPattern=FillPattern.Solid,
              textString="Bucket"),
            Text(
              extent={{-74,-20},{36,-60}},
              lineColor={0,0,0},
              textString="Traj")}));
    end DigCycleSeq;

    model maxH_extend
    extends OpenHydraulics.Examples.Excavator.SubSystems.DigCycleSeq(
      swingTimeTable(table=[0,0; 1,0; 10,0; 17,0; 20,0]),
      boomTimeTable(table=[0,0; 1,0; 10,-1; 12,0; 20,0]),
      armTimeTable(table=[0,0; 0.5,0; 1,-1; 3,0; 5,0; 6,0; 6.5,0; 7.5,0; 20,0]),
      bucketTimeTable(table=[0,0; 0.5,0; 1,0; 3,0; 4,0; 5,0; 6,0; 20,0]));

    annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
              -100},{100,100}}),      graphics));
    end maxH_extend;

    model max_height
    extends OpenHydraulics.Examples.Excavator.SubSystems.DigCycleSeq(
      swingTimeTable(table=[0,0; 1,0; 10,0; 17,0; 20,0]),
      boomTimeTable(table=[0,0; 1,0.5; 2,0; 20,0]),
      armTimeTable(table=[0,0; 0.5,0; 1,-0.5; 3,0; 5,0; 6,0; 6.5,0; 7.5,0; 20,0]),
      bucketTimeTable(table=[0,0; 0.5,0; 1,0; 3,0; 4,0; 5,-0.5; 6,0; 20,0]));

    annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
              -100},{100,100}}),      graphics));
    end max_height;

    model bucketCycle
    extends OpenHydraulics.Examples.Excavator.SubSystems.DigCycleSeq(
      swingTimeTable(table=[0,0; 1,0; 10,0; 17,0; 20,0]),
      boomTimeTable(table=[0,0; 1,0; 2,0; 20,0]),
      armTimeTable(table=[0,0; 0.5,0; 1,0; 3,0; 5,0; 6,0; 6.5,0; 7.5,0; 20,0]),
      bucketTimeTable(table=[0,0; 0.5,0; 1,-1; 3.5,0; 4,0; 5,0; 6,1; 9.06,0; 12,
            0; 14,-1; 16,0; 20,0]));

    annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
              -100},{100,100}}),      graphics));
    end bucketCycle;

    model armCycle
    extends OpenHydraulics.Examples.Excavator.SubSystems.DigCycleSeq(
      swingTimeTable(table=[0,0; 1,0; 10,0; 17,0; 20,0]),
      boomTimeTable(table=[0,0; 1,0; 2,0; 20,0]),
      armTimeTable(table=[0,0; 0.5,0; 1,0.5; 2.5,0; 5,0; 6,-0.5; 10,0; 15,0; 20,
            0]),
      bucketTimeTable(table=[0,0; 0.5,0; 1,0; 3.5,0; 4,0; 5,0; 6,0; 9.06,0; 12,
            0; 14,0; 16,0; 20,0]));

    annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
              -100},{100,100}}),      graphics));
    end armCycle;

    model boomCycle
    extends OpenHydraulics.Examples.Excavator.SubSystems.DigCycleSeq(
      swingTimeTable(table=[0,0; 1,0; 10,0; 17,0; 20,0]),
      boomTimeTable(table=[0,0; 1,-1; 3,0; 8,0.5; 11,0; 20,0]),
      armTimeTable(table=[0,0; 0.5,0; 1,0; 2.5,0; 5,0; 6,0; 10,0; 15,0; 20,0]),
      bucketTimeTable(table=[0,0; 0.5,0; 1,0; 3.5,0; 4,0; 5,0; 6,0; 9.06,0; 12,
            0; 14,0; 16,0; 20,0]));

    annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
              -100},{100,100}}),      graphics));
    end boomCycle;

    model swingCycle
    extends OpenHydraulics.Examples.Excavator.SubSystems.DigCycleSeq(
      swingTimeTable(table=[0,0; 1,1]),
      boomTimeTable(table=[0,0; 1,1; 3,0; 8,0; 11,0; 20,0]),
      armTimeTable(table=[0,0; 0.5,0; 1,1; 2.5,0; 5,0; 6,0; 10,0; 15,0; 20,0]),
      bucketTimeTable(table=[0,0; 0.5,0; 1,1; 3.5,0; 4,0; 5,0; 6,0; 9.06,0; 12,
            0; 14,0; 16,0; 20,0]));

    annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
              -100},{100,100}}),      graphics));
    end swingCycle;
  end TestComponents;


  annotation (uses(Modelica(version="3.2"), OpenHydraulics(version="1.0")),
    experiment,
    __Dymola_experimentSetupOutput);
end excavator_cyphy;
