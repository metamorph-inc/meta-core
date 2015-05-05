#include "DeckConverter.h"

#include <string>
#include <fstream>
#include <set>
#include "string_utils.h"
#include "../CADCommonFunctions/GraphicsFunctions.h"

std::string isis::DeckConverterBase::StaticStep = "STATIC1,STATICS,SESTATIC,NLSTATIC";
std::string isis::DeckConverterBase::FreqStep = "MODES,OLDMODES,SEMODES";
std::string isis::DeckConverterBase::BuckleStep = "BUCKLING,SEBUCKL";
std::string isis::DeckConverterBase::SteadyStep = "DFREQ,SEDFREQ";
std::string isis::DeckConverterBase::DynamicStep = "DTRAN,SEDTRAN";

namespace isis
{
	void CalculixConverter::ConvertNastranDeck(isis_CADCommon::NastranDeck& nasDeck,
											   const std::string &abaqusFile )
	{
		std::ofstream calculix;
		//calculix.open((nasDeck.Name + ".inp").c_str());
		//calculix.open((abaqusFile + ".inp").c_str());
		calculix.open((abaqusFile + "//" + nasDeck.getName() + ".inp").c_str());
	
		calculix << "** ---------------------------------------------------\n";
		calculix << "** ---- *NODE ----------------------------------------\n";
		calculix << "** ---------------------------------------------------\n";
		calculix << "*HEADING\n";
		calculix << "Model: " << nasDeck.getName() << "\n";
		calculix << "*NODE\n";
		calculix << "** node ID, X, Y, Z\n";

		for (std::map<int, isis_CADCommon::GridPoint>::const_iterator ci = nasDeck.getGridPointData().begin(); ci != nasDeck.getGridPointData().end(); ci++)
		{
			calculix<< ci->first << ", " << ci->second.point.x << ", " << ci->second.point.y << ", " << ci->second.point.z << "\n";
		}

		calculix << "** ---------------------------------------------------\n";
		calculix << "** ---- *TRANSFORM -----------------------------------\n";
		calculix << "** ---------------------------------------------------\n";	


		for (std::set<int>::const_iterator ci = nasDeck.getDisplaceCord_Elements_KEYS().begin(); ci != nasDeck.getDisplaceCord_Elements_KEYS().end(); ci++)
		{
			std::map<int, isis_CADCommon::CoordSystem>::const_iterator ei = nasDeck.getCoordSystems().find(*ci);

			if (ei != nasDeck.getCoordSystems().end())
			{
				std::string system = ei->second.system;
				std::string setname;
				to_string(setname, *ci);
				setname.insert(0, "NS_TR_CID_");
				calculix << "** following nodes have displacement coordinate systems\n";
				calculix << "** defined using " << system << " with CID = " << *ci << "\n";
				calculix << "**\n";

				if (system == "CORD2R")
				{
					calculix << "*TRANSFORM, TYPE=R, NSET=" << setname << "\n";
					calculix << ei->second.C.x << ", " << ei->second.C.y << ", " << ei->second.C.z << ", ";
				
					std::vector<double> Z, X;
					X.push_back(ei->second.C.x - ei->second.A.x);		// x
					X.push_back(ei->second.C.y - ei->second.A.y);		// y
					X.push_back(ei->second.C.z - ei->second.A.z);		// z

					Z.push_back(ei->second.B.x - ei->second.A.x);		// x
					Z.push_back(ei->second.B.y - ei->second.A.y);		// y
					Z.push_back(ei->second.B.z - ei->second.A.z);		// z

					std::vector<double> ZCrossX = isis_CADCommon::CrossProduct_3D(Z, X);
					calculix << ei->second.A.x+ZCrossX[0] << ", " << ei->second.A.y+ZCrossX[1] << ", " << ei->second.A.z+ZCrossX[2] << "\n";
				}
				else if (system == "CORD2C")
				{
					calculix << "*TRANSFORM, TYPE=C, NSET=" << setname << "\n";
					calculix << ei->second.A.x << ", " << ei->second.A.y << ", " << ei->second.A.z << ", ";
					calculix << ei->second.B.x << ", " << ei->second.B.y << ", " << ei->second.B.z << "\n";
				}
				else if (system == "CORD2S")
				{
					calculix << "*TRANSFORM, TYPE=S, NSET=" << setname << "\n";
					calculix << ei->second.A.x << ", " << ei->second.A.y << ", " << ei->second.A.z << ", ";
					calculix << ei->second.B.x << ", " << ei->second.B.y << ", " << ei->second.B.z << "\n";
				}

				calculix << "**\n";
				calculix << "*NSET, NSET=" << setname << "\n";
				std::multimap<int,int>::iterator it;
				std::pair<std::multimap<int,int>::iterator, std::multimap<int,int>::iterator> ret;
				ret = nasDeck.getDisplaceCord_Elements().equal_range(*ci);
				int count;
				for (it=ret.first, count = 1; it!=ret.second; it++, count++)
				{
					calculix << it->second << ", ";

					if (count % 5 == 0)
						calculix << "\n";
				}
				calculix << "\n**\n";
			}
		}

		/********************************************************************************************************************/

		calculix << "** ---------------------------------------------------\n";
		calculix << "** ---- *Material -----------------------------------\n";
		calculix << "** ---------------------------------------------------\n";
		for (std::map<int, isis_CADCommon::Material>::const_iterator ci = nasDeck.getMaterialData().begin(); ci != nasDeck.getMaterialData().end(); ci++)
		{
			calculix << "*MATERIAL, NAME=MAT_MID_" << ci->first << "\n";
			calculix << "*ELASTIC\n";
			calculix << ci->second.Young << ", " << ci->second.Poisson << "\n";
			if (ci->second.Density != "")
				calculix << "*DENSITY\n" << ci->second.Density << ",\n";
			if (ci->second.Thermal != "")
			{
				calculix << "*EXPANSION, ZERO=" << ci->second.RefTemp << "\n";
				calculix << ci->second.Thermal << ",\n";
			}
			calculix << "**\n";
		}
		/********************************************************************************************************************/

		calculix << "** ---------------------------------------------------\n";
		calculix << "** ---- SOLID elements -------------------------------\n";
		calculix << "** ---------------------------------------------------\n";
		std::string psolidList;
		for (std::map<int, isis_CADCommon::PSolid>::const_iterator ci = nasDeck.getPsolidData().begin(); ci != nasDeck.getPsolidData().end(); ci++)
		{
			std::string psolidID;
			to_string(psolidID, ci->first);
			psolidID.insert(0, "PSOLID_");
			psolidList += psolidID + ",\n";
			calculix << "*SOLID SECTION, ELSET=" << psolidID << ", MATERIAL=MAT_MID_" << ci->second.MID << "\n";
			calculix << "**\n";
			calculix << "*ELEMENT, TYPE=C3D10, ELSET=" << psolidID << "\n";			// TODO: Change element type accordingly
			calculix << "**\n";

			std::map<int, isis_CADCommon::SolidElement>::iterator it;
			std::pair<std::map<int, isis_CADCommon::SolidElement>::iterator,std::map<int, isis_CADCommon::SolidElement>::iterator> ret;
			ret = nasDeck.getElementData().equal_range(ci->first);
			for (it = ret.first; it != ret.second; it++)
			{
				calculix << it->second.EID;

				for (unsigned int i = 0; i < it->second.GID.size(); i++)
				{
					calculix << ", " << it->second.GID[i];
				}
				calculix << "\n";
			}
			calculix << "**\n";
		}

		calculix << "*ELSET, ELSET=ALL_3DSOLID\n";
		calculix << psolidList;
		calculix << "**\n";
		calculix << "*ELSET, ELSET=ALL_MASSIVE_ELEMENTS\n";
		calculix << "ALL_3DSOLID,\n";

		calculix << "** ---------------------------------------------------\n";
		/********************************************************************************************************************/

		for (std::map<std::string, std::vector<std::pair<std::string, std::string>>>::const_iterator ci = nasDeck.getCaseControlData().subcases.begin(); 
			ci != nasDeck.getCaseControlData().subcases.end(); 
			ci++)
		{
			calculix << "*STEP\n";
			if (nasDeck.getSolverType() == "101")
				calculix << "*STATIC\n";

			if (nasDeck.getCaseControlData().commands.find("DISPLACEMENT") != nasDeck.getCaseControlData().commands.end())
			{
				calculix << "*NODE PRINT, NSET=NS_TR_CID_" << nasDeck.getGridDisplacementCord_ID() << "\n";
				calculix << "U\n";
			}
			if (nasDeck.getCaseControlData().commands.find("SPCFORCES") != nasDeck.getCaseControlData().commands.end())
			{
				calculix << "*NODE PRINT, NSET=NS_TR_CID_" << nasDeck.getGridDisplacementCord_ID() << "\n";
				calculix << "RF\n";
			}


			calculix << "*EL PRINT, ELSET=ALL_3DSOLID\n";
			calculix << "S\n";
			calculix << "** ------------------------------------\n";
			calculix << "** ------ LOADCASE data for SUBCASE " << ci->first << "\n";
			calculix << "** ------------------------------------\n";

			calculix << "**LOADCASE, NAME=STEP_1_SOL_101_SUBCASE_" << ci->first<< "\n";
			calculix << "**LOADCASE\n";
			if (nasDeck.getSpcData().size() > 0)
			{
				calculix << "*BOUNDARY, OP=NEW\n";

				for (std::multimap<int, isis_CADCommon::SPC>::const_iterator bi = nasDeck.getSpcData().begin(); bi != nasDeck.getSpcData().end(); bi++)
				{
					calculix << bi->second.G1 << ", " << bi->second.C1 << "," << bi->second.C1 << ", " << bi->second.D1 << "\n";
				}
			}

			if (nasDeck.getForceLoadData().size() > 0)
			{
				calculix << "*CLOAD,  OP=NEW\n";
				for (std::multimap<int, isis_CADCommon::Force>::const_iterator ai = nasDeck.getForceLoadData().begin(); ai != nasDeck.getForceLoadData().end(); ai++)
				{
					if (atof(ai->second.N1.c_str()) != 0)
						calculix << ai->second.GID << ", 1, " << ai->second.N1 << "\n";

					if (atof(ai->second.N2.c_str()) != 0)
						calculix << ai->second.GID << ", 2, " << ai->second.N2 << "\n";

					if (atof(ai->second.N3.c_str()) != 0)
						calculix << ai->second.GID << ", 3, " << ai->second.N3 << "\n";
				}
			}

			calculix << "**END LOADCASE\n";
			calculix << "*NODE FILE\n";
			calculix << "U\n";
			calculix << "*EL FILE\n";
			calculix << "S\n";
			calculix << "*END STEP\n";
		}
		/********************************************************************************************************************/


		// close file
		calculix.close();
	}


// class ElmerConverter	
	void ElmerConverter::ConvertNastranDeck(isis_CADCommon::NastranDeck& nasDeck, 
											const std::string& outputDirectory)
	{
		std::ofstream elmerMesh;
		elmerMesh.open(outputDirectory + "//" + "mesh.header");
		elmerMesh << nasDeck.getGridPointData().size() << " " << nasDeck.getElementData().size() << " 0"  << "\n";			//nodes elements boundary-elements
		
		elmerMesh << nasDeck.getElementTypeCount() << "\n";																	//nof_types

		if (nasDeck.getHexaCount() > 0)																						//type-code nof_elements
		{
			elmerMesh << "820 " << nasDeck.getHexaCount() << "\n";
		}
		if (nasDeck.getPentaCount() > 0)																						//type-code nof_elements
		{
			elmerMesh << "715 " << nasDeck.getPentaCount() << "\n";
		}
		if (nasDeck.getTetraCount() > 0)																						//type-code nof_elements
		{
			elmerMesh << "510 " << nasDeck.getTetraCount() << "\n";
		}
		elmerMesh.close();


		elmerMesh.open(outputDirectory + "//" + "mesh.elements");
		for (std::map<int, isis_CADCommon::SolidElement>::const_iterator ci = nasDeck.getElementData().begin(); 
			ci != nasDeck.getElementData().end();
			ci++)
		{
			elmerMesh << ci->second.EID << " " << ci->second.PID << " ";

			if (ci->second.Type == isis_CADCommon::CHEXA)
				elmerMesh << "820";
			else if (ci->second.Type == isis_CADCommon::CPENTA)
				elmerMesh << "715";
			else if (ci->second.Type == isis_CADCommon::CTETRA)
				elmerMesh << "510";

			for (size_t i = 0; i < ci->second.GID.size(); i++)
			{
				elmerMesh << " " << ci->second.GID[i];
			}
			elmerMesh << "\n";
		}
		elmerMesh.close();

		elmerMesh.open(outputDirectory + "//" + "mesh.nodes");
		for (std::map<int, isis_CADCommon::GridPoint>::const_iterator ci = nasDeck.getGridPointData().begin(); ci != nasDeck.getGridPointData().end(); ci++)
		{
			elmerMesh << ci->second.ID << " -1 " << ci->second.point.x << " " << ci->second.point.y << " " << ci->second.point.z << "\n";
		}
		elmerMesh.close();

		elmerMesh.open(outputDirectory + "//" + "mesh.boundary");
		elmerMesh.close();
	}


} // namespace