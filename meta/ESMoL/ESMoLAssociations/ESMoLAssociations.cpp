
#include <set>

#include <UdmBase.h>
#include "Uml.h"

using namespace Uml;
using namespace std;

int main( int argc, char **argv ) {



	if ( argc < 2 )
	{
		std::cout << "Usage: ESMoLAssociations <uml-diagram-filename>" << std::endl;
		exit(1);
	}

	std::string filename( argv[1] );
	cout << "Opening file " << filename << " for processing..." << endl;

	try {
		Udm::SmartDataNetwork sdn( Uml::diagram ); 
		sdn.OpenExisting( filename, "", Udm::CHANGES_PERSIST_ALWAYS );
		
		//Get the root folder
		Uml::Diagram udiag = Uml::Diagram::Cast(sdn.GetRootObject());
		
		Class CompRef, MessageRef;
		Class CommMapping, Dependency, ExecutionAssignment, Connector; // association classes

		// First, find the important classes
		cout << "Collecting classes..." << endl;
		set< Class > classes = udiag.Class_kind_children();
		for ( set< Class >::iterator pCl = classes.begin(); 
			pCl != classes.end(); pCl++ ) {

				if ( string(pCl->name()) == string("ComponentRef") )
					CompRef = *pCl;

				if ( string(pCl->name()) == string("MessageRef") )
					MessageRef = *pCl;
				
				if ( string(pCl->name()) == string("CommMapping") )
					CommMapping = *pCl;

				if ( string(pCl->name()) == string("Dependency") )
					Dependency = *pCl;

				if ( string(pCl->name()) == string("ExecutionAssignment") )
					ExecutionAssignment = *pCl;

				if ( string(pCl->name()) == string("Connector") )
					Connector = *pCl;
		}
		
		// CommMapping
		cout << "Handling CommMapping..." << endl;
		if ( CommMapping ) {

			set< AssociationRole > cmars = CommMapping.associationRoles();
			if ( cmars.size() > 0 )
			{
				cout << "CommMapping already has additional associations -- please check the model. " << endl;
			}
			else {

				// Create something
				Association cm1 = Association::Create( udiag );
				AssociationRole ar1 = AssociationRole::Create( cm1 );
				ar1.name() = string( "srcCommMapping__rp_helper" );
				ar1.target() = CompRef;
				ar1.max() = 1;
				ar1.min() = 0;
				CompRef.associationRoles() += ar1;
				AssociationRole ar2 = AssociationRole::Create( cm1 );
				ar2.name() = string( "cmd" );
				ar2.target() = CommMapping;
				ar2.max() = 1;
				ar2.min() = 0;
				CommMapping.associationRoles() += ar2;

				Association cm2 = Association::Create( udiag );
				AssociationRole ar3 = AssociationRole::Create( cm2 );
				ar3.name() = string( "cms" );
				ar3.target() = CommMapping;
				ar3.max() = 1;
				ar3.min() = 0;
				CommMapping.associationRoles() += ar3;
				AssociationRole ar4 = AssociationRole::Create( cm2 );
				ar4.name() = string( "dstCommMapping__rp_helper" );
				ar4.target() = CompRef;
				ar4.max() = 1;
				ar4.min() = 0;
				CompRef.associationRoles() += ar4;
			}
			
		}
		else {

			std::cout << "CommMapping association class was not found in the model!" << std::endl;
		}

		// Dependency
		cout << "Handling Dependency... " << endl;
		if ( Dependency ) {

			set< AssociationRole > cmars = Dependency.associationRoles();
			if ( cmars.size() > 0 )
			{
				cout << "Dependency already has additional associations -- please check the model. " << endl;
			}
			else {

				// Create something
				Association cm1 = Association::Create( udiag );
				AssociationRole ar1 = AssociationRole::Create( cm1 );
				ar1.name() = string( "srcDependency__rp_helper" );
				ar1.target() = CompRef;
				ar1.max() = 1;
				ar1.min() = 0;
				CompRef.associationRoles() += ar1;
				AssociationRole ar2 = AssociationRole::Create( cm1 );
				ar2.name() = string( "dpd" );
				ar2.target() = Dependency;
				ar2.max() = 1;
				ar2.min() = 0;
				Dependency.associationRoles() += ar2;

				Association cm2 = Association::Create( udiag );
				AssociationRole ar3 = AssociationRole::Create( cm2 );
				ar3.name() = string( "dps" );
				ar3.target() = Dependency;
				ar3.max() = 1;
				ar3.min() = 0;
				Dependency.associationRoles() += ar3;
				AssociationRole ar4 = AssociationRole::Create( cm2 );
				ar4.name() = string( "dstDependency__rp_helper" );
				ar4.target() = CompRef;
				ar4.max() = 1;
				ar4.min() = 0;
				CompRef.associationRoles() += ar4;
			}
			
		}
		else {

			std::cout << "Dependency association class was not found in the model!" << std::endl;
		}

		
		// Execution Assignment
		cout << "Handling ExecutionAssignment..." << endl;
		if ( ExecutionAssignment ) {

			set< AssociationRole > exars = ExecutionAssignment.associationRoles();
			if ( exars.size() > 0 )
			{
				cout << "ExecutionAssignment already has additional associations -- please check the model. " << endl;
			}
			else {

				// Create something
				Association cm1 = Association::Create( udiag );
				AssociationRole ar1 = AssociationRole::Create( cm1 );
				ar1.name() = string( "eas" );
				ar1.target() = ExecutionAssignment;
				ar1.max() = 1;
				ar1.min() = 0;
				ExecutionAssignment.associationRoles() += ar1;
				AssociationRole ar2 = AssociationRole::Create( cm1 );
				ar2.name() = string( "dstExecutionAssignment__rp_helper" );
				ar2.target() = CompRef;
				ar2.max() = 1;
				ar2.min() = 0;
				CompRef.associationRoles() += ar2;

				/* Association cm2 = Association::Create( udiag );
				AssociationRole ar3 = AssociationRole::Create( cm2 );
				ar3.name() = string( "srcExecutionAssignment__rp_helper" );
				ar3.target() = CompRef;
				ar3.max() = 1;
				ar3.min() = 0;
				Dependency.associationRoles() += ar3;
				AssociationRole ar4 = AssociationRole::Create( cm2 );
				ar4.name() = string( "ead" );
				ar4.target() = ExecutionAssignment;
				ar4.max() = 1;
				ar4.min() = 0;
				CompRef.associationRoles() += ar4; */
			}
			
		}
		else {

			std::cout << "ExecutionAssignment association class was not found in the model!" << std::endl;
		}

		// Connector
		cout << "Handling Connector..." << endl;
		if ( Connector ) {

			set< AssociationRole > cmars = Connector.associationRoles();
			if ( cmars.size() > 0 )
			{
				cout << "Connector already has additional associations -- please check the model. " << endl;
			}
			else {

				// Create something
				Association cm1 = Association::Create( udiag );
				AssociationRole ar1 = AssociationRole::Create( cm1 );
				ar1.name() = string( "srcConnector__rp_helper" );
				ar1.target() = MessageRef;
				ar1.max() = 1;
				ar1.min() = 0;
				MessageRef.associationRoles() += ar1;
				AssociationRole ar2 = AssociationRole::Create( cm1 );
				ar2.name() = string( "cnd" );
				ar2.target() = Connector;
				ar2.max() = 1;
				ar2.min() = 0;
				Connector.associationRoles() += ar2;

				Association cm2 = Association::Create( udiag );
				AssociationRole ar3 = AssociationRole::Create( cm2 );
				ar3.name() = string( "cns" );
				ar3.target() = Connector;
				ar3.max() = 1;
				ar3.min() = 0;
				Connector.associationRoles() += ar3;
				AssociationRole ar4 = AssociationRole::Create( cm2 );
				ar4.name() = string( "dstConnector__rp_helper" );
				ar4.target() = MessageRef;
				ar4.max() = 1;
				ar4.min() = 0;
				MessageRef.associationRoles() += ar4;
			}
			
		}
		else {

			std::cout << "Connector association class was not found in the model!" << std::endl;
		}

		// We did make changes
		cout << "Closing." << endl;
		sdn.CloseWithUpdate();

	} catch( udm_exception &ex )	{
		std::cout << "Exception occured in udm" << std::endl;
		std::cout << ex.what() << std::endl;
	}

	return 0;
}