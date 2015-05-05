within excavator_cyphy;
package excavator2

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
  annotation (Diagram(coordinateSystem(extent={{-160,-200},{520,360}})), Icon(
        coordinateSystem(extent={{-160,-200},{520,360}})));
  end linearDistance;

  model v009_control
    import cyPhy_Excavator = excavator_cyphy;
    extends Modelica.Icons.Example;

  //--------------------------------------------------------
  // HydraulicSubSyst equations
  //--------------------------------------------------------
  extends OpenHydraulics.Interfaces.PartialFluidCircuit;

    //parameter SI.
    parameter Modelica.SIunits.Length boom_s_init=1.6
      "Initial position of boom cylinder"
      annotation(Dialog(tab="Initialization",group="Actuator Positions"));
    parameter Modelica.SIunits.Length arm_s_init=1
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
    cyPhy_Excavator.excavator2.components.boomCylL
                                        boomCylL(boom_s_init=boom_s_init)
                                      annotation (Placement(transformation(
            rotation=0, extent={{-22,118},{12,138}})));
    cyPhy_Excavator.excavator2.components.boomCylR
                                        boomcylR(boom_s_init=boom_s_init)
                                                annotation (Placement(
          transformation(rotation=0, extent={{118,116},{152,144}})));
    cyPhy_Excavator.excavator2.components.carriage
                                            carriage1 annotation (Placement(
          transformation(rotation=0, extent={{78,80},{98,100}})));
    cyPhy_Excavator.excavator2.components.armCyl
                                          armCyl1(arm_s_init=arm_s_init,
        strokeLength=2)
      annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
              238}})));
    cyPhy_Excavator.excavator2.components.bucketCyl
                                             bucketCyl1(bucket_s_init=
          bucket_s_init, strokeLength=1.5)
                         annotation (Placement(transformation(rotation=0, extent={{142,302},
              {172,330}})));
    cyPhy_Excavator.excavator2.components.boom
                                        boom1 annotation (Placement(
          transformation(rotation=0, extent={{66,164},{106,188}})));
    cyPhy_Excavator.excavator2.components.arm
                                       arm1 annotation (Placement(transformation(
            rotation=0, extent={{70,260},{98,288}})));
    cyPhy_Excavator.excavator2.components.bucket
                                          bucket1 annotation (Placement(
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
    Td=0.5,
    Ti=0.001,
    controllerType=Modelica.Blocks.Types.SimpleController.P,
    k=3)
    annotation (Placement(transformation(extent={{380,102},{360,82}})));
  Modelica.Blocks.Sources.TimeTable timeTable(table=[0,2.5; 10,2.5; 10,4; 20,4;
          20,4])
    annotation (Placement(transformation(extent={{430,82},{410,102}})));
  Modelica.Blocks.Continuous.LimPID PID1(
    initType=Modelica.Blocks.Types.InitPID.NoInit,
    yMax=1,
    Ti=0.5,
    Td=0.5,
    controllerType=Modelica.Blocks.Types.SimpleController.P,
    k=1)
    annotation (Placement(transformation(extent={{434,-36},{414,-56}})));
  Modelica.Blocks.Sources.TimeTable timeTable1(table=[0,3.48; 10,3.48; 10,3.48;
          20,3.48; 20,3.48])
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
  Modelica.Blocks.Sources.TimeTable timeTable2(table=[0,3.07; 10,3.07; 10,
        3.07; 20,3.07; 20,3.07])
    annotation (Placement(transformation(extent={{492,-112},{472,-92}})));
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
            -40.6667}},    color={255,0,0}));
    connect(boomLineB.port_b, jB.port[1])
                                      annotation (Line(points={{136,-54},{136,
            -24.6667}},    color={255,0,0}));
    connect(bucketValve.portA, bucketLineA.port_a)
                                             annotation (Line(points={{321.2,
          -96},{322,-96},{322,-74}},color={255,0,0}));
    connect(bucketValve.portB, bucketLineB.port_a)
                                             annotation (Line(points={{336.8,
          -96},{336,-96},{336,-74}},color={255,0,0}));
    connect(bucketLineA.port_b,bucketCyl1.port_a)
                                            annotation (Line(points={{322,-54},
            {322,-54},{322,132},{322,306},{186,306},{186,305.027},{171.379,
            305.027}},        color={255,0,0}));
    connect(bucketLineB.port_b,bucketCyl1.port_b)
      annotation (Line(points={{336,-54},{336,-54},{336,326},{171.172,326},{
            171.172,326.216}},                   color={255,0,0}));
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
  connect(boom1.frame_a, linearDistance1.frame_a) annotation (Line(
      points={{85.6,164.72},{28.8,164.72},{28.8,212},{-20,212}},
      color={95,95,95},
      thickness=0.5,
      smooth=Smooth.None));
  connect(bucketCyl1.frame_a, linearDistance1.frame_b) annotation (Line(
      points={{157.931,303.059},{36.9655,303.059},{36.9655,218},{-20,218}},
      color={95,95,95},
      thickness=0.5,
      smooth=Smooth.None));
  connect(PID.u_s, timeTable.y) annotation (Line(
      points={{382,92},{409,92}},
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
    equation
    connect(frame_a, undercarriage.frame_a)
                                   annotation (Line(points={{170,30},{170,30},{
            170,108},{190,108},{190,136}},           color={95,95,95}));
    connect(frame_b, undercarriage.frame_b)
      annotation (Line(points={{190,230},{190,158}}, color={0,0,0}));
    annotation (Diagram(coordinateSystem(extent={{70,30},{270,230}})), Icon(
          coordinateSystem(extent={{70,30},{270,230}})));
    end undercarriage;

    model bucket

      Modelica.Mechanics.MultiBody.Parts.Body bBucket(
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
        shapeType=CAD_Bucket,
        lengthDirection={0,0,-1},
        r_shape={0.014,-2.359,0.88})
                              annotation (Placement(transformation(
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
               then true else false, start=bucket_w_start))
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
        parameter SI.Mass m_Bucket=50;
        parameter SI.MomentOfInertia I_11_Bucket=0.1304;
        parameter SI.MomentOfInertia I_22_Bucket=2.11;
        parameter SI.MomentOfInertia I_33_Bucket=2.191;
        parameter SI.MomentOfInertia I_21_Bucket=-0.214;
        parameter SI.MomentOfInertia I_31_Bucket=0;
        parameter SI.MomentOfInertia I_32_Bucket=0;
        parameter Real r_CM_Bucket[:]={0.5,-0.6,0};
    // CAD Parameters
        parameter String CAD_Bucket = "modelica://OpenHydraulics/Resources/Shapes/Excavator/bucket.dxf";
        //parameter Real r_Bucket[:] = {0.52,0.07,0};
        parameter Real r_Bucket[:] = {0.636,-0.043,0};

    equation
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
               then true else false, start=arm_w_start))
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
            origin={288,160},
            extent={{-10,10},{10,-10}},
            rotation=270)));
      Modelica.Mechanics.MultiBody.Parts.FixedTranslation armLink(
        r=r_arm,
        animation=true,
        color={0,180,0},
        shapeType=CAD_arm,
        lengthDirection={-1,0,0},
        r_shape={3.455,0,0.302}) "object model for the arm of the excavator"
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
          Placement(transformation(rotation=0, extent={{234,200},{254,220}}),
            iconTransformation(extent={{234,200},{254,220}})));
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
      parameter Real armCylMountX[:] = {-1.089,0,0};
      parameter Real armCylMountY[:] = {0,0.636,0};
      // bucket Linkage mount
      //parameter Real linkMount[:] = {1.233,.177,0};
      parameter Real linkMount[:] = {2.96,0.061,0};
      // Bucket Linkage cylinder mount
      //parameter Real BucketCylMount[ :] = {0.49,0.96,0};
      parameter Real BucketCylMount[ :] = {0.177,1.233,0};
    // CAD parameters
        parameter String CAD_arm = "modelica://OpenHydraulics/Resources/Shapes/Excavator/boom_ex_375.dxf";
        parameter Real r_arm[:] = {3.455,0,0};

    equation
      connect(boom4LinkX.frame_a, boom4LinkY.frame_b)       annotation (Line(
          points={{372,141},{366.5,141},{366.5,149}},
          color={0,0,0},
          thickness=0.5));
      connect(bArm.frame_a, armLink.frame_a)
                                         annotation (Line(
          points={{288,170},{288,210.5},{325.5,210.5}},
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
      connect(frame_b1, arm2Link.frame_b) annotation (Line(points={{244,210},{
              244,261},{265.5,261}},
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
               then true else false, start=boom_w_start))
        annotation (Placement(transformation(extent={{-10,-10},{10,10}},
              rotation=90,
            origin={167,202})));

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
            origin={229.5,300})));
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
        shapeType= CAD_boom,
        color={155,0,0},
        r_shape=r_shape,
        lengthDirection=lengthDirection,
        widthDirection=widthDirection)
        "object model for the boom of the excavator"
        annotation (Placement(transformation(extent={{-29.5,-15},{29.5,15}},
              rotation=90,
            origin={170.5,286})));

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
        boom_phi_start = 0
        "Initial value of rotation angle phi (fixed or guess value)"
        annotation (Evaluate=false, Dialog(group="Boom",tab="Initialization"));
        //boom_phi_start=0.87266462599716
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
      parameter Real boomLCylMount[:]={3.424,1.047,-0.59};
      //Right cylinder mount
      parameter Real boomRCylMount[:] = {3.424,1.047,0.59};
      //Arm cylinder mount
      parameter Real armCylMount[:] = {3.131,1.874,0};
      //  parameter Real armCylMount[:] = {4.22,1.33,0};

    // CAD Parameters
      parameter String CAD_boom = "modelica://OpenHydraulics/Resources/Shapes/Excavator/boom.dxf";
      parameter Real r_boom[:] = {6.567,0,0};
      parameter Real r_shape[:] = {-2.058,4.704,-0.6};
      parameter Real lengthDirection[:] = {-1,0,0};
      parameter Real widthDirection[:] = {0,-1,0};

    Modelica.Mechanics.MultiBody.Sensors.RelativeAngles BoomAngle
      annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=90,
          origin={128,274})));
    equation
      connect(bBoom.frame_a,boom. frame_a) annotation (Line(
          points={{182,256},{172,256},{172,256.5},{170.5,256.5}},
          color={0,0,0},
          thickness=0.5));
      connect(boomRevolute.frame_b,boom. frame_a) annotation (Line(
          points={{167,212},{168,212},{168,256.5},{170.5,256.5}},
          color={95,95,95},
          thickness=0.5));
      connect(boom2LinkLeft.frame_a, boomRevolute.frame_b) annotation (Line(
          points={{105,223.5},{136,223.5},{136,212},{167,212}},
          color={95,95,95},
          thickness=0.5));
      connect(boom3Link.frame_a, boomRevolute.frame_b) annotation (Line(
          points={{229.5,276.5},{229.5,266},{222,266},{222,256},{222,232},{168,232},
              {168,212},{167,212}},
          color={95,95,95},
          thickness=0.5));
      connect(boom2LinkRight.frame_a, boomRevolute.frame_b)  annotation (Line(
          points={{233.5,209},{233.5,212},{196,212},{167,212}},
          color={95,95,95},
          thickness=0.5));
      connect(frame_b, boom.frame_b) annotation (Line(points={{176,354},{156,354},{156,
              350},{156,335.5},{170.5,335.5},{170.5,315.5}},
                               color={95,95,95}));
      connect(frame_b1, boom2LinkLeft.frame_b)
        annotation (Line(points={{70,168},{105,168},{105,167.5}},
                                                                color={0,0,0}));
      connect(frame_b2, boom2LinkRight.frame_b) annotation (Line(points={{268,168},
            {268,162},{233.5,162},{233.5,164}},color={0,0,0}));
      connect(frame_b3, boom3Link.frame_b) annotation (Line(points={{246,348},{228,348},
              {228,323.5},{229.5,323.5}},
                                    color={0,0,0}));
      connect(frame_a, boomRevolute.frame_a) annotation (Line(points={{168,166},
            {168,192},{167,192}},
                               color={95,95,95}));
      connect(frame_b, frame_b) annotation (Line(
          points={{176,354},{176,354}},
          color={95,95,95},
          thickness=0.5,
          smooth=Smooth.None));
    connect(BoomAngle.frame_a, boomRevolute.frame_a) annotation (Line(
        points={{128,264},{128,178},{168,178},{168,192},{167,192}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(BoomAngle.frame_b, boom.frame_b) annotation (Line(
        points={{128,284},{128,322},{170.5,322},{170.5,315.5}},
        color={95,95,95},
        thickness=0.5,
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
      OpenHydraulics.Components.Cylinders.DoubleActingCylinder bucketCyl(
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
      OpenHydraulics.Components.Cylinders.DoubleActingCylinder armCyl(
        strokeLength=strokeLength,
        closedLength=closedLength,
        boreDiameter=boreDiameter,
        rodDiameter=rodDiameter,
        pistonMass=pistonMass,
        s_init=arm_s_init,
        initType=Modelica.Mechanics.MultiBody.Types.Init.PositionVelocityAcceleration,
        fixHeadPressure=true)
        annotation (Placement(transformation(extent={{226,152},{246,172}},
              rotation=0)));

      parameter Modelica.SIunits.Length arm_s_init=1
        "Initial position of arm cylinder"
        annotation(Dialog(tab="Initialization",group="Actuator Positions"));
      parameter SI.Length closedLength = 2.48;
      parameter SI.Diameter boreDiameter = 0.135;
      parameter SI.Diameter rodDiameter=0.05;
      parameter SI.Mass pistonMass = 10;
      parameter SI.Length strokeLength = 2;
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
        widthDirection={1,0,0},
        lengthDirection={0,1,0},
        r_shape={-3.334,0,-1.636})
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
        parameter Real r_CM_carriage[:]={-0.942,0.970,0};

    // Position Parameters
        //parameter Real boomLinkRight[:] = {0.655,0.703,0.296};// "from swing motor to left boom cylinder cap side pivot"
        //parameter Real boomLinkLeft[:] = {0.655,0.703,-0.296};// "from swing motor to right boom cylinder cap side pivot"
        parameter Real boomLinkRight[:] = {2.11,0.825,0.514};
        parameter Real boomLinkLeft[:] = {2.11,0.825,-0.514};

    // CAD parameters
        parameter String CAD_carriage = "modelica://OpenHydraulics/Resources/Shapes/Excavator/upper_body.dxf";
        parameter Real r_carriage[:] = {1.102,1.457,0};//Location of revolute
        //parameter Real r_carriage[:] = {0,0,0};
        parameter Real r_shape[:] = {3.334,0,-1.636};
        parameter Real lengthDirection[:] = {0,1,0};
        parameter Real widthDirection[:] = {1,0,0};

      Modelica.Mechanics.MultiBody.Parts.Body bCarriage(
        m=m_carriage,
        I_11=I_11_carriage,
        I_22=I_22_carriage,
        I_33=I_33_carriage,
        I_21=I_21_carriage,
        I_31=I_31_carriage,
        I_32=I_32_carriage,
        r_CM=r_CM_carriage) "mass/inertia properties for carriage"
        annotation (Placement(transformation(extent={{189,185},{209,205}},
              rotation=0)));
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
      connect(bCarriage.frame_a,carriage. frame_a) annotation (Line(
          points={{189,195},{154.5,195}},
          color={0,0,0},
          thickness=0.5));
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
      OpenHydraulics.Components.Cylinders.DoubleActingCylinder boomCylRight(
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
        parameter SI.Length strokeLength = 2;
        parameter SI.Length maxLength = closedLength + strokeLength;

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
    Modelica.Blocks.Interfaces.RealOutput cylPressure
      annotation (Placement(transformation(extent={{310,82},{330,102}})));
    Modelica.Mechanics.Translational.Sensors.ForceSensor forceSensor
      annotation (Placement(transformation(extent={{224,82},{204,102}})));
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
    connect(pressureSensor.y, cylPressure) annotation (Line(
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
      OpenHydraulics.Components.Cylinders.DoubleActingCylinder boomCylLeft(
        strokeLength=strokeLength,
        closedLength=closedLength,
        boreDiameter=boreDiameter,
        rodDiameter=rodDiameter,
        pistonMass=pistonMass,
        s_init=boom_s_init)
        annotation (Placement(transformation(
            origin={58,122},
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
        parameter SI.Length strokeLength = 2;

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
    equation
      connect(boomCylLeft.flange_b, cylBoomLeft.flange_b) annotation (Line(
          points={{58,132},{92,132},{92,123},{99,123}},
          color={0,127,0},
          smooth=Smooth.None));
      connect(boomCylLeft.flange_a, cylBoomLeft.flange_a) annotation (Line(
          points={{58,112},{64,112},{64,104},{90,104},{90,111},{99,111}},
          color={0,127,0},
          smooth=Smooth.None));
      connect(port_a, boomCylLeft.port_a) annotation (Line(points={{0,232},{26,
              232},{26,114},{50,114}}, color={255,0,0}));
      connect(port_b, boomCylLeft.port_b) annotation (Line(points={{-4,-28},{20,
              -28},{20,130},{50,130}}, color={255,0,0}));
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
  end components;

  model v009_redux
    import cyPhy_Excavator = excavator_cyphy;
    extends Modelica.Icons.Example;

  //--------------------------------------------------------
  // HydraulicSubSyst equations
  //--------------------------------------------------------
  extends OpenHydraulics.Interfaces.PartialFluidCircuit;

    //parameter SI.
    parameter Modelica.SIunits.Length boom_s_init=1.75
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
    cyPhy_Excavator.excavator2.components.carriage
                                            carriage1 annotation (Placement(
          transformation(rotation=0, extent={{78,80},{98,100}})));
    cyPhy_Excavator.components.armCyl     armCyl1(arm_s_init=arm_s_init,
        strokeLength=2)
      annotation (Placement(transformation(rotation=0, extent={{130,210},{164,
              238}})));
    cyPhy_Excavator.components.bucketCyl     bucketCyl1(bucket_s_init=
          bucket_s_init) annotation (Placement(transformation(rotation=0, extent={{142,302},
              {172,330}})));
    cyPhy_Excavator.excavator2.components.boom
                                        boom1 annotation (Placement(
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
    Td=0.5,
    Ti=0.001,
    controllerType=Modelica.Blocks.Types.SimpleController.P,
    k=3)
    annotation (Placement(transformation(extent={{380,102},{360,82}})));
  Modelica.Blocks.Sources.TimeTable timeTable(table=[0,2.5; 10,2.5; 10,3.7;
        20,3.7; 20,3.7])
    annotation (Placement(transformation(extent={{430,82},{410,102}})));
  Modelica.Blocks.Continuous.LimPID PID1(
    initType=Modelica.Blocks.Types.InitPID.NoInit,
    yMax=1,
    Ti=0.5,
    Td=0.5,
    controllerType=Modelica.Blocks.Types.SimpleController.P,
    k=1)
    annotation (Placement(transformation(extent={{434,-36},{414,-56}})));
  Modelica.Blocks.Sources.TimeTable timeTable1(table=[0,3.19; 10,3.19; 10,
        3.19; 20,3.19; 20,3.19])
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
  Modelica.Blocks.Sources.TimeTable timeTable2(table=[0,3.07; 10,3.07; 10,
        3.07; 20,3.07; 20,3.07])
    annotation (Placement(transformation(extent={{492,-112},{472,-92}})));
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
            -40.6667}},    color={255,0,0}));
    connect(boomLineB.port_b, jB.port[1])
                                      annotation (Line(points={{136,-54},{136,
            -24.6667}},    color={255,0,0}));
    connect(bucketValve.portA, bucketLineA.port_a)
                                             annotation (Line(points={{321.2,
          -96},{322,-96},{322,-74}},color={255,0,0}));
    connect(bucketValve.portB, bucketLineB.port_a)
                                             annotation (Line(points={{336.8,
          -96},{336,-96},{336,-74}},color={255,0,0}));
    connect(bucketLineA.port_b,bucketCyl1.port_a)
                                            annotation (Line(points={{322,-54},
            {322,-54},{322,132},{322,306},{186,306},{186,305.027},{171.379,
            305.027}},        color={255,0,0}));
    connect(bucketLineB.port_b,bucketCyl1.port_b)
      annotation (Line(points={{336,-54},{336,-54},{336,326},{171.172,326},{
            171.172,326.216}},                   color={255,0,0}));
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
  connect(boom1.frame_a, linearDistance1.frame_a) annotation (Line(
      points={{85.6,164.72},{28.8,164.72},{28.8,212},{-20,212}},
      color={95,95,95},
      thickness=0.5,
      smooth=Smooth.None));
  connect(bucketCyl1.frame_a, linearDistance1.frame_b) annotation (Line(
      points={{157.931,303.059},{36.9655,303.059},{36.9655,218},{-20,218}},
      color={95,95,95},
      thickness=0.5,
      smooth=Smooth.None));
  connect(PID.u_s, timeTable.y) annotation (Line(
      points={{382,92},{409,92}},
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
end excavator2;
