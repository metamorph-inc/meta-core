// CyPhy2Desert.cpp : Defines the entry point for the console application.
//
#include "CyPhy2Desert.h"

UDM_USE_DOM
UDM_USE_MGA

void usage()
{
	std::cout << "Usage: cyphy2desert.exe <CyPhyML mga File Name> [<Desert xml File Name>] "<< std::endl;
}

int main(int argc, char *argv[])
{
	if( argc== 1)
	{
		usage( );
		return -1;
	}

	std::string cyphy_file = argv[1];
	std::string desert_file = cyphy_file;

	size_t extPos = cyphy_file.find(".mga");
	if(extPos == string::npos)
	{
		extPos = cyphy_file.find(".MGA");
		if(extPos == string::npos)
		{
			std::cout<<"Input file must be MGA file. Aborting."<<std::endl;
			return -1;
		}
	}

	if(argc == 3)
	{
		desert_file = argv[2];
		extPos = desert_file.find(".xml");
		if(extPos == string::npos) desert_file += ".xml";
	}
	else desert_file.replace(extPos,4,"_Desert.xml");

	cout << "Converting " << cyphy_file << " ...... ";

	try
	{
		Udm::SmartDataNetwork cyphy(CyPhyML::diagram);
		cyphy.OpenExisting(argv[1],"CyPhyML", Udm::CHANGES_LOST_DEFAULT);

		CyPhyML::RootFolder cyphy_rf = CyPhyML::RootFolder::Cast(cyphy.GetRootObject());

		Udm::SmartDataNetwork desert(DesertIface::diagram);
		{	
			desert.CreateNew(desert_file, "DesertIface.xsd", DesertIface::DesertSystem::meta,Udm::CHANGES_LOST_DEFAULT);
			DesertIface::DesertSystem desert_top = DesertIface::DesertSystem::Cast(desert.GetRootObject());

			map<CyPhyML::DesignEntity, DesertIface::Element> com2elem;
			CyPhy2Desert c2d(desert_top);
			c2d.generateDesert(cyphy_rf);
		}
		desert.CloseWithUpdate();
		cyphy.CloseWithUpdate();
	
		cout<<desert_file<<endl;
		cout<<"Done."<<endl;
	}
	catch(udm_exception &e)
	{
		cout<<e.what()<<endl;
		return 1;
	}

	return 0;
}

