within CyPhy.TestComponents;
model ConstantForce
  parameter Real __CyPhy__f_const=5;
  extends Modelica.Mechanics.Translational.Sources.ConstantForce(
    f_constant=__CyPhy__f_const);
end ConstantForce;
