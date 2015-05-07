This package will generate a Java source file from a MatLab Stateflow model stored in an ".mdl" file.

REQUIREMENTS
------------
GME (for use the the GME backend in UDM)

**IMPORTANT**
If you don't have VS2005 Service Pack 1 installed, you may need to install the package found
at the following URL:

http://www.microsoft.com/downloads/details.aspx?FamilyID=200b2fd9-ae1a-4a14-984d-389c36f85647&displaylang=en

The title of the page is "Microsoft Visual C++ 2005 SP1 Redistributable Package (x86)"

-----

This package contains the following executables:

Executable:  MDL2MGA slsf_model.mdl [ecsl_dp_model.<xml|mga>]
-------------------------------------------------------------
With one argument, MDL2MGA renders an ESMoL model that is analogous to the MatLab model in the ".mdl" file.
The file rendered that contains this ESMoL model has the same name as the ".mdl" file, but with a ".xml" entension.

With two arguments, MDL2MGA will render the ESMoL model into the file specified by the second argument.
This file must end in either ".xml" (to use the UDM DOM backend) or ".mga" (to use the GME backend).
Only a ".mga" file is viewable in GME (the ECLSP_DP paradigm must be registered in GME to do this).


Executable:  ESMoL2Java.bat <ecsl_dp_model.<xml|mga>>
-------------------------------------------------------
This will render a ".java" Java source file from the ESMoL model in the specified file.



To run the test from the ESMoL2Java directory:
------------------------------------------------
Execute: ..\bin\MDL2MGA fixedphone.mdl
Renders: fixedphone.xml

Execute: SF2J.bat fixedphone.xml
Renders: TopLevel.java

Execute: <path-to-java-compiler>\javac TopLevel.java State.java
If this compiles without error, the test has completed successfully.

