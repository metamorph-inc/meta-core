#include "stdafx.h"
#include "UdmApp.h"
#include "UdmConfig.h"
#include "Uml.h"
#include "UdmUtil.h"

#include "HierCycles.h"

#include <set>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

/*********************************************************************************/
/* Initialization function. The framework calls it before preparing the backend. */
/* Initialize here the settings in the config global object.					 */
/* Return 0 if successful.														 */
/*********************************************************************************/
int CUdmApp::Initialize()
{


	// TODO: Your initialization code comes here...
	return 0;
}



/* 
Remarks to CUdmApp::UdmMain(...):
0.	The p_backend points to an already open backend, and the framework 
	closes it automatically. DO NOT OPEN OR CLOSE IT!
	To commit changes use p_backend->CommitEditSequence().
	To abort changes use p_backend->AbortEditSequence().
	To save changes to a different file use p_backend->SaveAs() or p_backend->CloseAs().

1.	Focus is the currently open model.

2.	The possible values for param (from GME DecoratorLib.h component_startmode_enum):
	GME_MAIN_START			=   0,
	GME_BROWSER_START		=   1,
	GME_CONTEXT_START		=   2,
	GME_EMBEDDED_START		=   3,
	GME_MENU_START			=  16,
	GME_BGCONTEXT_START		=  18,
	GME_ICON_START			=  32,
	METAMODEL_CHECK_SYNTAX	= 101

 3. The framework catches all the exceptions and reports the error in a message box,
	clean up and close the transactions aborting the changes. You can override this 
	behavior by catching udm_exception. Use udm_exception::what() to form an error 
	message.
*/

/***********************************************/
/* Main entry point for Udm-based Interpreter  */
/***********************************************/

void CUdmApp::UdmMain(
					 Udm::DataNetwork* p_backend,		// Backend pointer(already open!)
					 Udm::Object focusObject,			// Focus object
					 set<Udm::Object> selectedObjects,	// Selected objects
					 long param)						// Parameters
{	

	// TODO: Your Code Comes Here...
	//AfxMessageBox("Building Cycle Graph for the current open model");

	if( focusObject == &Udm::_null )
	{
		AfxMessageBox("No current focus object!  Please open a Component model.");
	}

	//else if ( focusObject.type() == ESMoL::Component::meta ) // OK to go forward
	//{

	//}

	// Here goes --

	//    iterate over the selected objects
	else //if(!selectedObjects.empty())
	{
		//FullCycleList full_cycles;
		

		
		// Iterate set
		//for( set< Udm::Object >::iterator objIter=selectedObjects.begin();
		//	 objIter!=selectedObjects.end(); objIter++ )
		{
			ofstream out( "cycles.log", ios::out );
			string objName( UdmUtil::ExtractName(focusObject) );	
			out << "Processing selected object " << objName << endl;
			out.close();
			ESMoLCycles cycles;

	//    if the object in question is of type:
	//       Subsystem
	//           findHierCycles( subsys, cycles, path_name_list )
	//           getFullCycleList( cycles, cycle2componentmap )

			if ( focusObject.type() == ESMoL::Subsystem::meta )
			{

				clock_t t1, t2;
				t1 = clock();

				ESMoL::Subsystem subsys = ESMoL::Subsystem::Cast( focusObject );
				subsys.uniqueId();
				bool found = false;
				// container obj-> cycles_in_container[ cycle[ (cycle_member_obj1, obj1_full_pathname), ... ], cycle[...], ... ]
				
				found = cycles.findHierCycles( subsys );

				//if ( found )
				//{
				//	cycles.getFullCycleList( full_cycles );
				//}

				// Clear modification flags now that we're done
				cycles.clearModFlags( subsys );

				t2 = clock();
				double spanms = ( (t2-t1)* 1000 )/CLOCKS_PER_SEC;
				ostringstream msg;
				msg << "Elapsed time: " << spanms << std::endl;
				msg << "Components visited: " << cycles.numComponents() << std::endl;
				msg << "Components processed: " << cycles.numComponentProcessed() << std::endl;
				msg << "Elements processed: " << cycles.numElementsProcessed() << std::endl;
				msg << "Abstract cycles found: " << cycles.numAbstractCycles() << std::endl;
				msg << "Total cycles found: " << cycles.numTotalCycles() << std::endl;
				AfxMessageBox( msg.str().c_str() );
			}

	//       SubsystemRef
	//           findHierCycles( subsysref->ref(), cycles, path_name_list )
	//           getFullCycleList( cycles, cycle2componentmap )
			else if ( focusObject.type() == ESMoL::SubsystemRef::meta )
			{
				// is this even possible?
			}

	//       Component
	//           cycles = []
	//           iterate over SubSystemRefs
	//               findHierCycles( subsysref->ref(), cycles, path_name_list )
	//           build local graph with inputs, outputs, connections, & subsysrefs (using path edges)
	//           findAllCycles( localgraph, cycles )
	//           fill in path edges for next level
	//           getFullCycleList( cycles, cycle2componentmap )

			else if ( focusObject.type() == ESMoL::Component::meta )
			{
				clock_t t1, t2;
				t1 = clock();

				ESMoL::Component comp = ESMoL::Component::Cast( focusObject );
				comp.uniqueId();
				bool found = false;
				// container obj-> cycles_in_container[ cycle[ (cycle_member_obj1, obj1_full_pathname), ... ], cycle[...], ... ]

				found = cycles.findHierCycles( comp );

				//if ( found )
				//{
				//	cycles.getFullCycleList( full_cycles );
				//}

				// Clear modification flags now that we're done
				cycles.clearModFlags( comp );

				t2 = clock();
				double spanms = ( (t2-t1)* 1000 )/CLOCKS_PER_SEC;
				ostringstream msg;
				msg << "Elapsed time: " << spanms << std::endl;
				msg << "Components visited: " << cycles.numComponents() << std::endl;
				msg << "Components processed: " << cycles.numComponentProcessed() << std::endl;
				msg << "Elements processed: " << cycles.numElementsProcessed() << std::endl;
				AfxMessageBox( msg.str().c_str() );
			}

	//       ComponentRef
	//           same as component, but deref
			else if ( focusObject.type() == ESMoL::ComponentRef::meta )
			{
				// is this even possible?
			}

	//       SystemTypes
	//           cycles = []
	//           iterate over Components
	//               findCompCycles( comp, cycles, path_name_list )
	//           getFullCycleList( cycles, cycle2componentmap )
			else if ( focusObject.type() == ESMoL::SystemTypes::meta )
			{

			}

	//       LogicalArchitecture
	//           cycles = []
	//           iterate over ComponentRefs
	//               findCompCycles( compref->ref(), cycles, path_name_list )
	//           build local graph with inputs, outputs, connections, & componentrefs (using path edges)
	//           findAllCycles( localgraph, cycles )
	//           getFullCycleList( cycles, cycle2componentmap )

			else if ( focusObject.type() == ESMoL::System::meta )
			{

			}

	//       Anything else
	//           tough luck
			else
			{
				string msg = objName + string(" is wrong type for cycle detection.");
				AfxMessageBox( msg.c_str() );	
			}

		}
		
	}
	// Considerations:
	//     when making a cycle report, we need to keep track of the path down into the hierarchy (check)
	//     we need to keep a map of reported cycles to the object that contained them, so we can reconstruct

/************************** Demo Code - Delete It *********************/
	// Welcome


//#ifdef _DEBUG
//	// Displaying the name of the root object meta
//	Udm::Object rootObject=p_backend->GetRootObject();
//	string rootObjectName("Root Object's Class Name: ");
//	rootObjectName+=UdmUtil::ExtractName(rootObject.type());
//	AfxMessageBox(rootObjectName.c_str());
//
//	// Displaying the focus object
//	if(focusObject!=&Udm::_null)
//	{
//		string focusObjectName("Focus Object Name: ");
//		focusObjectName+=UdmUtil::ExtractName(focusObject);
//		AfxMessageBox(focusObjectName.c_str());
//	}
//
//	// Displaying selected objects
//	if(!selectedObjects.empty())
//	{
//		string selObjNames("Selected Objects:\r\n");
//		// Iterate set
//		for(set<Udm::Object>::iterator i=selectedObjects.begin();
//														i!=selectedObjects.end();i++)
//		{
//			selObjNames+=UdmUtil::ExtractName(*i);
//			selObjNames+="\r\n";
//		}
//		AfxMessageBox(selObjNames.c_str());	
//	}
//#endif
/****************************** Demo Code End *************************/
													
}

