/* 
* ScamlaTester.cpp : 
*	demo code to show how to use DesertManager interface
*	User must have the Scamla package installed before running the test
*/

#include "DesertManager.h"
#include <iostream>

int main(int argc, char *argv[])
{
	try
	{
		DesertManager dm;
		//register the importer scamla2desert.exe which must be in system path
		dm.registerImporter("scamla2desert.exe"); 
		//register the exporter desert2scamla.exe which must be in system path
		dm.registerExporter("desert2scamla.exe"); 
		//run scamla2desert.exe on input model SCAMLA_Example.mga to generate the DesertIface xml file
		//SCAMLA_Example.mga must exist
		dm.importModel("SCAMLA_Example.mga");

		//Demo to show how to use addConstraint
		//The constraint context and corresponding component path is only valid for Scamla_example.mga
		//Suppose the component Application\RadioService_Alternative\RS1 fails, the constraint:
		// constraint c3() { 
		//   not(self.implementedBy() = children("RS1"))
		// }"
		// can be injected to rule out the configurations that use component RS1
		// the parent of the above constraint should be Application\RadioService_Alternative
		//if(RS1 fails)
		dm.addConstraint("testconstraint", "constraint c3() { not(self.implementedBy() = children(\"RS1\"))}", "Application\\RadioService_Alternative");

		//Apply constraint with name "C3" (only valid for Scamla_example.mga)
		dm.applyConstraint("C3");

		//Apply all constraints
	//	dm.applyAllConstraints();
		
		//run desert to generate the DesertIfaceBack xml file 
		int cfgs = dm.runDesert();	
		//For all configurations, generate the corresponding scamla models
		dm.exportAll();		
		//Generate a scamla model with specific configuration index(e.g. 1)
		//dm.exportModel(1);
	}
	catch(Desert_exception &e)
	{
		std::cout<<e.what()<<std::endl;
	}
	return 0;
}

