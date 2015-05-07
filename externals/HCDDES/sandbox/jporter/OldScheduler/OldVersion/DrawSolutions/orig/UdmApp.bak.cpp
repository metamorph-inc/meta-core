// UdmApp.cpp: implementation of the CUdmApp class.
// This file was automatically generated as UdmApp.cpp
// by UDM Interpreter Wizard on Tuesday, April 10, 2007 20:25:13



#include "stdafx.h"
#include "UdmApp.h"
#include "UdmConfig.h"
#include "Uml.h"

#include <sstream>
#include <fstream>
#include <iomanip>
#include <map>

using namespace TTSched;

/*********************************************************************************/
/* Initialization function. The framework calls it before preparing the backend. */
/* Initialize here the settings in the config global object.					  */
/* Return 0 if successful.														  */
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

/************************** Demo Code - Delete It *********************/
	// Welcome
	//AfxMessageBox("Hello Udm-based Interpreter World!");

#ifdef _DEBUG_NO
	// Displaying the name of the root object meta
	Udm::Object rootObject=p_backend->GetRootObject();
	string rootObjectName("Root Object's Class Name: ");
	rootObjectName+=ExtractName(rootObject.type());
	AfxMessageBox(rootObjectName.c_str());

	// Displaying the focus object
	string focusObjectName("Focus Object Name: ");
	focusObjectName+=ExtractName(focusObject);
	AfxMessageBox(focusObjectName.c_str());

	// Displaying selected objects
	string selObjNames("Selected Objects:\r\n");
	// Iterate set
	for(set<Udm::Object>::iterator i=selectedObjects.begin();
													i!=selectedObjects.end();i++)
	{
		selObjNames+=ExtractName(*i);
		selObjNames+="\r\n";
	}
	AfxMessageBox(selObjNames.c_str());	
#endif
/****************************** Demo Code End *************************/

	// Displaying selected objects
	string selObjName; //("Selected Objects:\r\n");
	// Iterate set

	if ( selectedObjects.begin() != selectedObjects.end() )
	{
		
		for(set<Udm::Object>::iterator i=selectedObjects.begin();
														i!=selectedObjects.end();i++)
		{
			selObjName=ExtractName(*i);
			//selObjName+="\r\n";
			Solution s;
			try {
				s = Solution::Cast( *i );
			} catch (...) {
				ostringstream errmsg;
				errmsg << ExtractName(*i) << " is not a solution." << endl;
				errmsg << "Please select a solution from a constraint model.";
				AfxMessageBox( errmsg.str().c_str() );
			}

			ConstraintProblem cp = ConstraintProblem::Cast( s.parent() );
			InstanceRef ir = cp.InstanceRef_child();
			InstanceModel im = ir.ref();
			int global_period = (int) im.HyperPeriod();
			
			// First, build an index of all of the answers in the solution
			set< Assignment > anslist = s.Assignment_children();
			set< Assignment >::iterator pAns;
			map< string, int > answers;
			for ( pAns = anslist.begin(); pAns != anslist.end(); pAns++ )
			{
				answers[ pAns->name() ] = (int) pAns->Value();
			}

			// We'll use these a bit
			set< Processor > procs = im.Processor_children();
			set< Processor >::iterator pProc;

			set< Bus > buses = im.Bus_children();
			set< Bus >::iterator pBus;

			// Count colors and store durations
			int colorcount = 0;
			map< string, int > durations;
			map< string, int > colors;
			for ( pProc = procs.begin(); pProc != procs.end(); pProc++ )
			{
				// First get all of the durations (and colors) ready from the tasks on the processor
				set< Task > tasks = pProc->Task_children();
				set< Task >::iterator pTsk;
				for ( pTsk = tasks.begin(); pTsk != tasks.end(); pTsk++ )
				{
					
					durations[ pTsk->name() ] = (int) pTsk->WCExecTime();
					pair< map<string,int>::iterator, bool > r = colors.insert( make_pair( pTsk->name(), colorcount ) ); // No overwrite
					if (r.second == true) colorcount++; // only increment if insert was successful
				}
			}

			for ( pBus = buses.begin(); pBus != buses.end(); pBus++ )
			{
				set< Message > msgs = pBus->Message_children();
				set< Message >::iterator pMsg;
				for ( pMsg = msgs.begin(); pMsg != msgs.end(); pMsg++ )
				{
					durations[ pMsg->name() ] = (int) pMsg->MsgLength();
					pair< map<string,int>::iterator, bool > r = colors.insert( make_pair( pMsg->name(), colorcount ) );
					if ( r.second == true ) colorcount++;
				}
			}


			// Iterate over the processors in the instance model
			//   Create one line for each processor -- 
			//   Put the task boxes on the line, with a different color for
			//   each task type.  Save an index with instance name and rect coords
			int yoffset = 1;
			int ywidth = 5;
			int yskip = 10;
			map< string, vector< string > > instance_grid;
			map< string, vector< int > > taskboxes; // Task id, (x,y,x+width,y+height, color)
			map< string, int > yoffsetmap;  // Processor/Bus name -> y offset of tasks on that line
			for ( pProc = procs.begin(); pProc != procs.end(); pProc++ )
			{
				TaskInstances ti = pProc->TaskInstances_child();

				// For the box, we need a line offset (for processor),
				// fixed width for the line, start time, and duration
				//multimap< int, pair< string, string > > tasksort;  // Sort the tasks according to their offset
				
				vector< string >  instances_per_proc;
				set< TaskInstance > instances = ti.TaskInstance_children();
				set< TaskInstance >::iterator pInst;
				for ( pInst = instances.begin(); pInst != instances.end(); pInst++ )
				{
					instances_per_proc.push_back( pInst->name() );
					vector< int > vals(5);
					vals[0] = (int) answers[ pInst->name() ] + 1;
					vals[1] = yoffset;
					vals[2] = (int) durations[ pInst->RefName() ];
					vals[3] = ywidth;
					vals[4] = colors[ pInst->RefName() ];
					taskboxes[ pInst->name() ] = vals;
				}
				instance_grid[ pProc->name() ] = instances_per_proc;
				yoffsetmap[ pProc->name() ] = yoffset;
				yoffset += ywidth + yskip;
			}

			// Iterate over the buses in the instance model
			//   Create one line for each bus -- 
			//   Put the msg boxes on the line, with a different color for
			//   each msg type.  Save an index with instance name and rect coords
			for ( pBus = buses.begin(); pBus != buses.end(); pBus++ )
			{
		
				MsgInstances ti = pBus->MsgInstances_child();
				vector< string >  instances_per_proc;
				set< MsgInstance > instances = ti.MsgInstance_children();
				set< MsgInstance >::iterator pInst;
				for ( pInst = instances.begin(); pInst != instances.end(); pInst++ )
				{

					instances_per_proc.push_back( pInst->name() );
					vector< int > vals(5);
					vals[0] = (int) answers[ pInst->name() ] + 1;
					vals[1] = yoffset;
					vals[2] = (int) durations[ pInst->RefName() ];
					vals[3] = ywidth;
					vals[4] = colors[ pInst->RefName() ];
					taskboxes[ pInst->name() ] = vals;
				}
				instance_grid[ pBus->name() ] = instances_per_proc;
				yoffsetmap[ pBus->name() ] = yoffset;
				yoffset += ywidth + yskip;
			}

			// Now get the dependencies, look up the names from the box
			// index sets created above, and draw lines between the boxes.
			// Start with the different connectors in the dependency model.
			// Gather up all of the connectors and plot one arrow per pair.
			vector< vector< double > > lineends;
			for ( pBus = buses.begin(); pBus != buses.end(); pBus++ )
			{
				set< Dependencies > deps = pBus->Dependencies_children();
				set< Dependencies >::iterator pDep = deps.begin();  // We had better have only one!
				set< SenderInst > senders = pDep->SenderInst_children();
				set< ReceiverInst > receivers = pDep->ReceiverInst_children();

				set< SenderInst >::iterator pSndr;
				set< ReceiverInst >::iterator pRcvr;

				for ( pSndr = senders.begin(); pSndr != senders.end(); pSndr++ )
				{
					vector< double > sline(4);
					// Look up sending end and message end, use the instance name to get the box, and find the center
					TaskInstRef sref = pSndr->srcSenderInst_end();
					MsgInstRef mref = pSndr->dstSenderInst_end();
					TaskInstance sndtask = sref.ref();
					MsgInstance sndmsg = mref.ref();

					vector< int > sndbox = taskboxes[ sndtask.name() ];
					vector< int > msgbox = taskboxes[ sndmsg.name() ];

					sline[0] = sndbox[0] + ( sndbox[2] / 2.0 ); // send box x-center
					sline[1] = sndbox[1] + ( sndbox[3] / 2.0 ); // send box y-center
					sline[2] = msgbox[0] + ( msgbox[2] / 2.0 ); // msg box x-center
					sline[3] = msgbox[1] + ( msgbox[3] / 2.0 ); // msg box y-center

					lineends.push_back( sline );
				}

				for ( pRcvr = receivers.begin(); pRcvr != receivers.end(); pRcvr++ )
				{
					vector< double > rline(4);
					// Look up sending end and message end, use the instance name to get the box, and find the center
					MsgInstRef mref = pRcvr->srcReceiverInst_end();
					TaskInstRef rref = pRcvr->dstReceiverInst_end();
					MsgInstance sndmsg = mref.ref();
					TaskInstance rcvtask = rref.ref();
					vector< int > msgbox = taskboxes[ sndmsg.name() ];
					vector< int > rcvbox = taskboxes[ rcvtask.name() ];
					
					rline[0] = msgbox[0] + ( msgbox[2] / 2.0 ); // msg box x-center
					rline[1] = msgbox[1] + ( msgbox[3] / 2.0 ); // msg box y-center
					rline[2] = rcvbox[0] + ( rcvbox[2] / 2.0 ); // send box x-center
					rline[3] = rcvbox[1] + ( rcvbox[3] / 2.0 ); // send box y-center

					lineends.push_back( rline );
				}
			}
			
			// Create a file here with the proper name for the processor
			string pfname = selObjName;
			pfname += ".m";

			ofstream fout;
			fout.open( pfname.c_str() );

			fout << "cm = colormap;" << endl;
			fout << "cs = size(cm,1);" << endl << endl;

			map< string, vector< string > >::iterator pInst;
			
			// First the boxes
			for ( pInst = instance_grid.begin(); pInst != instance_grid.end(); pInst++ )
			{
				vector<string>::iterator pList;
				for ( pList = (pInst->second).begin(); pList != (pInst->second).end(); pList++ )
				{	
					fout << "rectangle( 'Position', [";
					fout << taskboxes[ *pList ][0] << ",";
					fout << taskboxes[ *pList ][1] << ",";
					fout << taskboxes[ *pList ][2] << ",";
					fout << taskboxes[ *pList ][3] << "], 'FaceColor', ";
					fout << "cm( floor( " << (double(taskboxes[ *pList ][4])/double(colorcount)) << " * cs )+1, :) );" << endl;
				}
			}
			fout << endl;

			// Iterate over the processors
			//   Get the y offset for this processor
			//   Make a text box for the processor name, place it on the upper part of the line
			//   Iterate over the tasks -- place all of the tasks on the line, in an appropriately colored box
			//   Repeat for buses
			int procxoffset = -10;
			int minxoffset = procxoffset;
			int xskip = 7;
			int xstart = -3;
			int procyoffset = 4;
			int taskyoffset = 1;
			for ( pProc = procs.begin(); pProc != procs.end(); pProc++ )
			{
				fout << "text( " << procxoffset << ", " << yoffsetmap[ pProc->name() ]+procyoffset << ", '" << pProc->name() << "', ";
				//fout << "', 'BackgroundColor', cm( floor( " << (double(color[pProc->name()])/double(colorcount)) << " * cs )+1, :), ";
				fout << "'EdgeColor', [0 0 0], 'HorizontalAlignment', 'center' );" << endl;
				// First get all of the durations (and colors) ready from the tasks on the processor
				int xoffset = xstart;
				set< Task > tasks = pProc->Task_children();
				set< Task >::iterator pTsk;
				for ( pTsk = tasks.begin(); pTsk != tasks.end(); pTsk++ )
				{
					fout << "text( " << xoffset << ", " << yoffsetmap[ pProc->name() ]+taskyoffset << ", '" << pTsk->name() << "', ";
					fout << "'BackgroundColor', cm( floor( " << (double(colors[pTsk->name()])/double(colorcount)) << " * cs )+1, :), ";
					fout << "'EdgeColor', [0 0 0], 'HorizontalAlignment', 'center' );" << endl;
					xoffset -= xskip;
				}
				minxoffset = ( xoffset < minxoffset ) ? xoffset : minxoffset;
			}

			for ( pBus = buses.begin(); pBus != buses.end(); pBus++ )
			{
				fout << "text( " << procxoffset << ", " << yoffsetmap[ pBus->name() ]+procyoffset << ", '" << pBus->name() << "', ";
				//fout << "', 'BackgroundColor', cm( floor( " << (double(color[pBus->name()])/double(colorcount)) << " * cs )+1, :), ";
				fout << "'EdgeColor', [0 0 0], 'HorizontalAlignment', 'center' );" << endl;
				set< Message > msgs = pBus->Message_children();
				set< Message >::iterator pMsg;
				int xoffset = xstart;
				for ( pMsg = msgs.begin(); pMsg != msgs.end(); pMsg++ )
				{
					fout << "text( " << xoffset << ", " << yoffsetmap[ pBus->name() ]+taskyoffset << ", '" << pMsg->name() << "', ";
					fout << "'BackgroundColor', cm( floor( " << (double(colors[pMsg->name()])/double(colorcount)) << " * cs )+1, :), ";
					fout << "'EdgeColor', [0 0 0], 'HorizontalAlignment', 'center' );" << endl;
					xoffset -= xskip;
				}
				minxoffset = ( xoffset < minxoffset ) ? xoffset : minxoffset;
			}

			// Readjust the axis for the new text
			fout << "v = axis();" << endl;
			fout << "axis( [ " << minxoffset << " v(2) v(3) v(4)] );" << endl;

			// Now the lines
			//vector< vector< double > >::iterator pLine;
			/*for ( pLine = lineends.begin(); pLine != lineends.end(); pLine++ )
			{
				fout << "line( [" << (*pLine)[0] << ", " << (*pLine)[2] << "], ";
				fout << "[ " << (*pLine)[1] << ", " << (*pLine)[3] << "], 'Marker', 'x', 'Color', 'k' );" << endl;
			}*/

			fout.close();



		}
		AfxMessageBox(selObjName.c_str());	
	}
	else
	{
		AfxMessageBox( "Please select a solution item to process." );
	}

}



#ifdef _DEBUG
/*****************************************************/
/* Debug time helper function. If the object has an  */
/* attribute called "name", this function retreives  */
/* it to help you to find it in the model during the */
/* application development.	Usualy every GME Object	 */
/* has a "name" attribute. If an object hapens not	 */
/* to have it,function retreives <no name specified>.*/
/*****************************************************/
string CUdmApp::ExtractName(Udm::Object ob)
{
	Uml::Class cls= ob.type();				
	set<Uml::Attribute> attrs=cls.attributes();		
	
	// Adding parent attributes
	set<Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	for(set<Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		if(string(ai->type())=="String")
		{
			string str=ai->name();
			if(str=="name")
			{
				string value=ob.getStringAttr(*ai);
				if(value.empty())value="<empty string>";
				return value;
			}			
		}				
	}	
	return string("<no name specified>");
}
#endif
