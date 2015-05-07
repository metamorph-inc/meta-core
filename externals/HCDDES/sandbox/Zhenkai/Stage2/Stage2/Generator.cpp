/***********************************/
/* More like a C style programming */
/***********************************/

//*****************************************************************************************************/
/* If there is no RTOS, every node runs a whole main function, and the template should be main.tpl    */
/* If there is an RTOS, just need to create tasks that run on the RTOS, and the messages between them */
/******************************************************************************************************/
#ifndef _RTOS_
#define TEMPLATE_NAME "main.tpl"
#else
#define TEMPLATE_NAME "FRODO.tpl"
#endif

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include <Semantics.h>
#include <ctemplate/template.h>
#include "Generator.h"
#include "Processors.h"

void Visit_Node(Semantics::Node &node)
{

	ctemplate::TemplateDictionary mainDictionary("main");

	string processor = node.processormodel();

	string outputCodeFileName = processor + "_main.c";
	string templatePath = LIBRARY_PATH;
	string processorPath = templatePath + "Processors/" + processor + "/";

	if (processor == "LPC2146") {
		mainDictionary.AddSectionDictionary("LPC2146_INCLUDE");
		LPC2146_Variables(mainDictionary);
	}
	else
		return;

	// Use templates to initialize the processor
	ctemplate::TemplateDictionary *init_processor_dict = mainDictionary.AddIncludeDictionary("PROCESSOR_INITIALIZATION");
	init_processor_dict->SetFilename(processorPath + "initialization.tpl");

	// IODevice
	set<Semantics::Device> deviceSet = node.integrates();

	string deviceType;
	set<Semantics::Device>::iterator itDeviceSet;
	for (itDeviceSet = deviceSet.begin(); itDeviceSet != deviceSet.end(); ++itDeviceSet) {
		deviceType = itDeviceSet->devicetype();
		if (deviceType == "")
			cout << "Need a device type" << endl;
	}

	ofstream outputCodeFile(outputCodeFileName.c_str());
	string output;
	ctemplate::ExpandTemplate(templatePath + TEMPLATE_NAME, ctemplate::DO_NOT_STRIP, &mainDictionary, &output);
	outputCodeFile << output;
}

void Visit_Deployment(Semantics::Deployment &deployment)
{
	set<Semantics::Node> nodeSet = deployment.Node_children();

	set<Semantics::Node>::iterator itNodeSet;
	for (itNodeSet = nodeSet.begin(); itNodeSet != nodeSet.end(); ++itNodeSet)
		Visit_Node(*itNodeSet);
}

void Generate_C_Code(Semantics::RootFolder &rootFolder)
{
	set<Semantics::Deployment> deploymentSet = rootFolder.Deployment_children();

	set<Semantics::Deployment>::iterator itDeploymentSet;
	for (itDeploymentSet = deploymentSet.begin(); itDeploymentSet != deploymentSet.end(); ++itDeploymentSet)
		Visit_Deployment(*itDeploymentSet);
}



