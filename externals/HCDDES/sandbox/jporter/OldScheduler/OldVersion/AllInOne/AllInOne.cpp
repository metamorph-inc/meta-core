
// Main for AllInOne scheduler

// Code stolen directly from lamp model example in UDM documentation.
//
#include "CSched.h"

#ifdef _WIN32
UDM_USE_MGA
#endif

UDM_USE_DOM

#include <Uml.h>
#include <UdmStatic.h>
#include <UmlExt.h>
#include <UdmBase.h>
//#include <UdmInt.h>
//#include <cint_string.h>
//#include <UdmCint.h>

#include "InstanceMgr.h"

using namespace CSched;

void myDisplay(char *msg)
{
fprintf(stdout,"myDisplay: %s",msg);
}

#define CLINE "-Ic:\\EiC\\include -D_EiC"

int main(int argc, char *argv[])
{
	/*
	*
	*/
	if(argc < 2)
	{
		cout << "Usage: CSched <Scheduler Model File>\n";
		return -1;
	}
	try
	{
		Udm::SmartDataNetwork nw(diagram);
		//nw.CreateNew(argv[1],"LampDiagram", RootFolder::meta);
		nw.OpenExisting( argv[1], "CSched" );

		// First question -- how do we get the back end pointer from the nw object?
		{
			RootFolder rrr = RootFolder::Cast( nw.GetRootObject() );
            set< TTSystem > systems = rrr.TTSystem_children();
            set< TTSystem >::iterator pSys;
            for ( pSys = systems.begin(); pSys != systems.end(); pSys++ )
            {
                InstanceMgr imgr( &nw, *pSys ); // Create instances and such
                if ( !imgr.GetSolutions( pSys->name() ) ) // Draw solutions
				{
					cout << "No solutions found for model " << pSys->name() << "." << endl;
				}
            }
		}
		nw.CloseNoUpdate();
        //nw.CloseWithUpdate();
	}
	catch(udm_exception &e)
	{
		cout << "exception: " << e.what() << endl;
		return -1;

	}

	return 0;
}