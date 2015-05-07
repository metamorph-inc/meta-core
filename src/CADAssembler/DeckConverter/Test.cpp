// Test.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <direct.h>
#include <vector>
#include <io.h>     // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include <Windows.h>
#include "../CADCommonFunctions/Nastran.h"
#include "DeckConverter.h"

bool DirectoryExists( const char* absolutePath ){

    if( _access( absolutePath, 0 ) == 0 ){

        struct stat status;
        stat( absolutePath, &status );

        return (status.st_mode & S_IFDIR) != 0;
    }
    return false;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "-i <Nastran Input Deck File>   -o <Output Directory>" << std::endl;
		return 0;
	}

	try
	{
		std::string nasFile;
		std::string abaqusFile;
		for (int i = 1; i < argc; i++)
		{
			std::string anArg = argv[i];

			if (anArg == "-i")
			{
				nasFile = argv[i+1];
			}
			if (anArg == "-o")
			{
				abaqusFile = argv[i+1];
			}

		}
		
		std::string::size_type pos = nasFile.find(".nas");
		if (pos == std::string::npos)
			std::cout << "Incorrect File Name: " << nasFile << std::endl;
		else
		{	
			if (!DirectoryExists(abaqusFile.c_str()))
			{
				char* a_cwd = _getcwd(NULL, 0);
				std::string s_cwd(a_cwd);
				free(a_cwd);
				std::cout << "Directory specified does not exist relative to working directory." << std::endl;
				std::cout << "Placing INP file in working directory (" << s_cwd << ")" << std::endl;
			}
			isis_CADCommon::NastranDeck nasDeck;
			nasDeck.ReadNastranDeck(nasFile);
			
			isis::CalculixConverter converter;
			//converter.ConvertNastranDeck(commonDS, abaqusFile);

			//isis::ElmerConverter converter;
			converter.ConvertNastranDeck(nasDeck, abaqusFile);

			// Create csv file mapping PSolids to associated elements for use in post-processing.
			std::ofstream psolid_element_map;
			psolid_element_map.open("PSolid_Element_Map.csv");
			for (std::map<int, isis_CADCommon::PSolid>::const_iterator pi = nasDeck.getPsolidData().begin(); pi != nasDeck.getPsolidData().end(); pi++)
			{
				std::vector<int> psolid;
				std::vector<int> elements;
				psolid.push_back(pi->first);
				nasDeck.FindElementsFromPSolids(psolid, elements);
				psolid_element_map << psolid[0] << ", ";
				for (int e=0; e != elements.size(); e++)
				{
					psolid_element_map << elements[e] << ", ";
				}
				psolid_element_map << '\n';
			}
			psolid_element_map.close();
			
		}
	}
	catch(isis::application_exception& e)
	{
		std::cout << "ISIS Application Exception: " << e.what() << std::endl;
	}

	return 0;
}

