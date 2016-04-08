	CyPhy2CAD_CSharp.dll



Revision History:
----------------


	  3/15/2016  	Export_All_Component_Points: Added the capability to export all component points by 
           		specifying a particular parameter in the Test Bench.  Typically, to export a component 
           		point there would need to be a metric in the Test Bench that pointed to a point in 
           		a referenced component (e.g. System Under Test).   With this change, if a parameter 
           		exists in the Test Bench with the name of Export_All_Component_Points, then all points
           		in the component models referenced by the Test Bench would be exported.  Exported means 
           		that CyPhy2CAD_CSharp would add instructions in CADAssembly.xml for the CreateAssembly 
           		program to compute the coordinates of the points and put the values in ComputedValues.xml.  
           		Note – Only the points in the component (Kind = Component) would be exported and not 
           		the points in CADModel (Kind = CADModel) unless those points were exposed at the component level. 

V2.0.0.0 ??/??/??	Added version to CyPhy2CAD_CSharp.dll.  ???Other changes???