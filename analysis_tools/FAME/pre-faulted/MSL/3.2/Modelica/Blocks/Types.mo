// processed by FAME Modelica Library Fault-Augmentor [version 19011]

within Modelica.Blocks;

package Types
 "Library of constants and types with choices, especially to build menus"
 extends Modelica.Icons.Package;

 type Smoothness = enumeration(LinearSegments, ContinuousDerivative)  "Enumeration defining the smoothness of table interpolation"
;

 type Extrapolation = enumeration(HoldLastPoint, LastTwoPoints, Periodic)  "Enumeration defining the extrapolation of time table interpolation"
;

 type Init = enumeration(NoInit, SteadyState, InitialState, InitialOutput)  "Enumeration defining initialization of a block"
 annotation(Evaluate=true);

 type InitPID = enumeration(NoInit, SteadyState, InitialState, InitialOutput, DoNotUse_InitialIntegratorState)  "Enumeration defining initialization of PID and LimPID blocks"
 annotation(Documentation(info="<html>
<p>
This initialization type is identical to Types.Init and has just one
additional option <b>DoNotUse_InitialIntegratorState</b>. This option
is introduced in order that the default initialization for the
Continuous.PID and Continuous.LimPID blocks are backward
compatible. In Modelica 2.2, the integrators have been initialized
with their given states where as the D-part has not been initialized.
The option \"DoNotUse_InitialIntegratorState\" leads to this
initialization definition.
</p>

</html>"),Evaluate=true);

 type SimpleController = enumeration(P, PI, PD, PID)  "Enumeration defining P, PI, PD, or PID simple controller type"
 annotation(Evaluate=true);

 type AnalogFilter = enumeration(CriticalDamping, Bessel, Butterworth, ChebyshevI)  "Enumeration defining the method of filtering"
 annotation(Evaluate=true);

 type FilterType = enumeration(LowPass, HighPass, BandPass, BandStop)  "Enumeration of analog filter types (low, high, band pass or band stop filter"
 annotation(Evaluate=true);

annotation(Documentation(info="<HTML>
<p>
In this package <b>types</b> and <b>constants</b> are defined that are used
in library Modelica.Blocks. The types have additional annotation choices
definitions that define the menus to be built up in the graphical
user interface when the type is used as parameter in a declaration.
</p>
</HTML>"));
end Types;
