/* ctestgen_main.cpp generated on Tue Jun 19 13:55:29 2012
 */

#include "ctestgen.h"
#include "InputFileRegistry.h"
#include "UdmDom.h"
//#include "UdmGme.h"
#include "RTTGenerator.h"
//#include "GReATUtils.h"
//#include <direct.h>

#include <boost/filesystem.hpp>

// Extern global temporary root object(s).
void usage()
{
	std::cout << "Usage:  Start.exe FileType=\"input.xml\";\"output.xml\" !\"Udm\\SFC.xsd\"  or\n\tStart -d\n\n";
}

int main( int argc, char* argv[])
{
	InputFileRegistry ifr;
	ifr.registerFile( "FileType=\"input.xml\";\"output.xml\" !\"Udm\\SFC.xsd\" ");
	if( argc== 1)
	{
		usage( );
		return -1;
	}
	else
		if( ( argc== 2)&& ( std::string( argv[ 1])== "-d"))
			std::cout << "Executing with default arguments." << std::endl;
		else
			for( int i= 1; i<argc; ++i)
			{
				if( false== ifr.registerFile( argv[ i]))
					throw InputFileEx( std::string( "Invalid input file expression: ") + argv[ i]);
			}
	try
	{
		// Initialize RTT
		boost::filesystem::path cwd = boost::filesystem::current_path();
		UdmUtil::copy_assoc_map udm_copyMap;
		map<int, int> rtt_copyIdMap;
		std::string rttname("RTT_");

		std::string stem = boost::filesystem::path(  ifr.getFileName( "FileType" )  ).filename().stem().string();
		rttname.append( stem );

		std::string fullMGAName = (  cwd / boost::filesystem::path( rttname )  ).string();
		fullMGAName.append( ".xml" );
		RTTGenerator::Instance( fullMGAName, rttname );
		// 
		std::vector< Udm::StaticDataNetworkSpecifier> dnsvec;
		// Open FileType rwc
		Udm::SmartDataNetwork sdnSFC_fec( SFC::diagram);
		sdnSFC_fec.OpenExisting( ifr.getFileName( "FileType"), UseXSD()( ifr.getFileName( "FileType")) ? ifr.getXsdName( "FileType" ) : "SFC", Udm::CHANGES_LOST_DEFAULT);

		std::string fullCopyPath = boost::filesystem::absolute(   cwd / boost::filesystem::path(  ifr.getCopyName( "FileType" )  )   ).string();
		std::string copyName = boost::filesystem::path(  ifr.getCopyName( "FileType" )  ).filename().string();
		RTTGenerator::Instance()->generateDataNetwork( sdnSFC_fec.testdn()->uniqueId(), fullCopyPath, copyName );
		Udm::StaticDataNetworkSpecifier sdns_SFC_fed( ifr.getFileName( "FileType"), &sdnSFC_fec);
		dnsvec.push_back( sdns_SFC_fed);
		// Create the project
		Udm::StaticUdmProject prj( dnsvec, ctest::diagram);
		Udm::DataNetwork& sFC_ref_fee= prj.GetDataNetwork( ifr.getFileName( "FileType"));
		Packets_t ins_1;
		// get objects from DNs
		SFC::Project rootSFC_fef= SFC::Project::Cast( sFC_ref_fee.GetRootObject());
		// Add objects to input packets.
		ins_1.push_back( rootSFC_fef);
		// Get access to temporary root object(s).
		// transformation
		Start_0 start_0;
		start_0( ins_1);
		// Close the project
		prj.Close();
		sdnSFC_fec.SaveAs( ifr.getCopyName( "FileType"));
		sdnSFC_fec.CloseNoUpdate();
		// close RTT model
		RTTGenerator::Instance()->close();
	}
	catch( udm_exception &e)
	{
		cout << e.what() << endl;
	}
	return 0;
}

