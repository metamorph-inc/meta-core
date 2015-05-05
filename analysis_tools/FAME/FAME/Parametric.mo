within FAME;

package Parametric
  "Attenuator elements to be used for various 'external approach' modifications."

  partial model BaseParametricFault
    replaceable type ParamType = Real;
    parameter ParamType u "input signal";
    output ParamType y "output signal";
    input Real amount(min=0.0, max=1.0, start=0, unit="1");
  end BaseParametricFault;

  partial model BaseParametricFaultAmountAsParameter
    replaceable type ParamType = Real;
    parameter ParamType u "input signal";
    output ParamType y "output signal";
    parameter Real amount(min=0.0, max=1.0, start=0, unit="1");
  end BaseParametricFaultAmountAsParameter;

end Parametric;
