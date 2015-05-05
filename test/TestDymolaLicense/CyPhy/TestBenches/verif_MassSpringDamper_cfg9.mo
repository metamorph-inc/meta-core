within CyPhy.TestBenches;
model verif_MassSpringDamper_cfg9
  extends CyPhy.TestBenches.MassSpringDamper_cfg9;

    // Requirement Definition
    type Requirement = enumeration(
      success,
      unknown,
      violated);

    // Limit-Checks definitions
    Requirement Limit1(start = Requirement.unknown, fixed=true) " Max absolute value limit-check on MassSpringDamper.Spring_1__Spring_1.flange_a.f";

    // Requirements-Checks
equation
    // Limit-Checks equations
    if Limit1 == Requirement.violated or MassSpringDamper.Spring_1__Spring_1.flange_a.f > 12 or MassSpringDamper.Spring_1__Spring_1.flange_a.f < -12 then
      Limit1 = Requirement.violated;
    else
      Limit1 = Requirement.unknown;
    end if;


    // Requirement-Metrics Checks equations
end verif_MassSpringDamper_cfg9;
