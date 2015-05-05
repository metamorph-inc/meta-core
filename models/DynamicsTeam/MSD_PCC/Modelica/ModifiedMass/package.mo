within ;
package ModifiedMass 
  model MassInitial
    extends Modelica.Mechanics.Translational.Components.Mass(m=massa);
    parameter Real massa = 1;
    parameter Real a_start = -1;
    parameter Real v_start = 0;
  initial equation
    a = a_start;
    v = v_start;
  end MassInitial;


annotation (uses(Modelica(version="3.2")));
end ModifiedMass;
