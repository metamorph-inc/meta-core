within CyPhy.Components;
package MSD
extends Modelica.Icons.Package;
  package Components
  extends Modelica.Icons.Package;
    package Dampers
    extends Modelica.Icons.Package;
      model Damper_1
        extends Modelica.Mechanics.Translational.Components.Damper(
          d=1);
      end Damper_1;
      model Damper_10
        extends Modelica.Mechanics.Translational.Components.Damper(
          d=10);
      end Damper_10;
      model Damper_2
        extends Modelica.Mechanics.Translational.Components.Damper(
          d=2);
      end Damper_2;
      model Damper_3
        extends Modelica.Mechanics.Translational.Components.Damper(
          d=3);
      end Damper_3;
      model Damper_4
        extends Modelica.Mechanics.Translational.Components.Damper(
          d=4);
      end Damper_4;
      model Damper_5
        extends Modelica.Mechanics.Translational.Components.Damper(
          d=5);
      end Damper_5;
      model Damper_6
        extends Modelica.Mechanics.Translational.Components.Damper(
          d=6);
      end Damper_6;
      model Damper_7
        extends Modelica.Mechanics.Translational.Components.Damper(
          d=7);
      end Damper_7;
      model Damper_8
        extends Modelica.Mechanics.Translational.Components.Damper(
          d=8);
      end Damper_8;
      model Damper_9
        extends Modelica.Mechanics.Translational.Components.Damper(
          d=9);
      end Damper_9;
    end Dampers;
    package Masses
    extends Modelica.Icons.Package;
      model Mass_10
        parameter Real __CyPhy__a_start=10;
        extends ModifiedMass.MassInitial(
          a_start=__CyPhy__a_start,
          massa=10,
          v_start=0);
      end Mass_10;
      model Mass_2
        parameter Real __CyPhy__a_start=10;
        extends ModifiedMass.MassInitial(
          a_start=__CyPhy__a_start,
          massa=2,
          v_start=0);
      end Mass_2;
      model Mass_3
        parameter Real __CyPhy__a_start=10;
        extends ModifiedMass.MassInitial(
          a_start=__CyPhy__a_start,
          massa=3,
          v_start=0);
      end Mass_3;
      model Mass_4
        parameter Real __CyPhy__a_start=10;
        extends ModifiedMass.MassInitial(
          a_start=__CyPhy__a_start,
          massa=4,
          v_start=0);
      end Mass_4;
      model Mass_5
        parameter Real __CyPhy__a_start=10;
        extends ModifiedMass.MassInitial(
          a_start=__CyPhy__a_start,
          massa=5,
          v_start=0);
      end Mass_5;
      model Mass_6
        parameter Real __CyPhy__a_start=10;
        extends ModifiedMass.MassInitial(
          a_start=__CyPhy__a_start,
          massa=6,
          v_start=0);
      end Mass_6;
      model Mass_7
        parameter Real __CyPhy__a_start=10;
        extends ModifiedMass.MassInitial(
          a_start=__CyPhy__a_start,
          massa=7,
          v_start=0);
      end Mass_7;
      model Mass_8
        parameter Real __CyPhy__a_start=10;
        extends ModifiedMass.MassInitial(
          a_start=__CyPhy__a_start,
          massa=8,
          v_start=0);
      end Mass_8;
      model Mass_9
        parameter Real __CyPhy__a_start=10;
        extends ModifiedMass.MassInitial(
          a_start=__CyPhy__a_start,
          massa=9,
          v_start=0);
      end Mass_9;
    end Masses;
    package Springs
    extends Modelica.Icons.Package;
      model Spring_1
        extends Modelica.Mechanics.Translational.Components.Spring(
          c=1);
      end Spring_1;
      model Spring_10
        extends Modelica.Mechanics.Translational.Components.Spring(
          c=10);
      end Spring_10;
      model Spring_2
        extends Modelica.Mechanics.Translational.Components.Spring(
          c=2);
      end Spring_2;
      model Spring_3
        extends Modelica.Mechanics.Translational.Components.Spring(
          c=3);
      end Spring_3;
      model Spring_4
        extends Modelica.Mechanics.Translational.Components.Spring(
          c=4);
      end Spring_4;
      model Spring_5
        extends Modelica.Mechanics.Translational.Components.Spring(
          c=5);
      end Spring_5;
      model Spring_6
        extends Modelica.Mechanics.Translational.Components.Spring(
          c=6);
      end Spring_6;
      model Spring_7
        extends Modelica.Mechanics.Translational.Components.Spring(
          c=7);
      end Spring_7;
      model Spring_8
        extends Modelica.Mechanics.Translational.Components.Spring(
          c=8);
      end Spring_8;
      model Spring_9
        extends Modelica.Mechanics.Translational.Components.Spring(
          c=9);
      end Spring_9;
    end Springs;
  end Components;
end MSD;
