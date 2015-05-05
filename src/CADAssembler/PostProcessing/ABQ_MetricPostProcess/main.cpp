// junk_udm_64bit.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h"
#include <iostream>

#include <odb_API.h>
#include "AnalyzeODB.h"
#include <map>
#include <fstream>


string gPath;
string gConfigID;
map<string, vector<Metric>> gMetricPairs;
map<string, string> gElementSets;
map<string, Material> gMaterials;
std::ofstream gFile;

using namespace std;




void Usage()
{
	cout << "Usage:" << endl;
	cout << "-f <ODB_File_Path>" << endl;
	cout << "-e <Element Sets>" << endl;
	cout << "-c <Config ID>" << endl;
	cout << "-m <MetricID:Name Pairs>" << endl;
	cout << "-p <Material Properties>" << endl;
}

float main(int argc, char* argv[])
{
	float maxVonMises = 1;

	gFile.open("FEAStressOutput.txt");	

	try
	{
		if (argc < 5)
		{
			cout << "Insufficient # of arguments provided!" << endl;
			Usage();
			return 0;
		}

		gPath = "";
		string elementNames,
			metricPairs,
			materialProperties;		
		
		for (int i = 1; i < argc; i++)
		{
			string anArg = argv[i];

			if (anArg == "-f")
			{
				gPath = argv[i+1];
			}
			else if (anArg == "-e")
			{
				elementNames = argv[i+1];
				ParseElementIDs(elementNames);
			}
			else if (anArg == "-c")
			{
				gConfigID = argv[i+1];
			}
			else if (anArg == "-m")
			{
				metricPairs = argv[i+1];
				ParseMetricIDs(metricPairs);
			}
			else if (anArg == "-p")
			{
				materialProperties = argv[i+1];
				ParseMaterials(materialProperties);
			}			
		}
		maxVonMises = AnalyzeFEAResults();
		WriteMetricsFile();
	}
	catch (odb_Exception& e)
	{
		std::string msg = e.ErrMsg().text();
		int errorN = e.ErrorNo();
		std::cout << "ODB Exception: " << msg.c_str() << " ErrorNo: " << errorN << std::endl;

		double var;
		std::cin >> var;
	}
	catch (analyzer_exception& e)
	{
		std::cout << "Analyzer Exception: " << e.what();
	}
	catch (...)
	{
		std::string msg = "None Abaqus related error!";
		std::cout<< "None ODB exception!" << std::endl;
	}

	gFile.close();
	return maxVonMises;
}

