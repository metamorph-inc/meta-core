#include "DeckConverter.h"

#include <string>
#include <numeric>
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
	// Calculix assigns convection and flux loads to a specified element face.
	// Which nodes are specified determine the selected face. The sum of the nodes
	// given is unique to a given face.
	// Face 1: 1-2-3, Face 2: 1-4-2, Face 3: 2-4-3, Face 4: 3-4-1
	void CalculixConverter::DetermineElementFace(std::vector<int> faceNodes, int &face)
	{
		int nodeSum = 0;
		nodeSum = std::accumulate( faceNodes.begin(), faceNodes.end(), 0 );
		if (nodeSum == 6)
			face = 1;
		else if (nodeSum == 7)
			face = 2;
		else if (nodeSum == 8)
			face = 3;
		else if (nodeSum == 9)
			face = 4;
	}

	void CalculixConverter::ConvertNastranDeck(isis_CADCommon::NastranDeck& nasDeck,
											   const std::string &abaqusFile )
	{
		isis_CADCommon::NastranDeckHelper deckHelper(nasDeck);

		std::ofstream calculix;

		// Determine type of analysis
		std::string analysisType;
		std::string elementType;
		bool structural = false;
		bool thermal = false;
		if (nasDeck.getSolverType() == "101")
		{
			analysisType = "*STATIC";
			elementType = "C3D10";
			structural = true;
		}
		else if (nasDeck.getSolverType() == "153")
		{
			analysisType = "*HEAT TRANSFER, STEADY STATE";
			elementType = "DC3D10";
			thermal = true;
		}

		calculix.open(((abaqusFile.size()>0)?(abaqusFile + "\\"):"" + nasDeck.getName() + ".inp").c_str());
	
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

		//********************************************************************************************************************

		calculix << "** ---------------------------------------------------\n";
		calculix << "** ---- *Material -----------------------------------\n";
		calculix << "** ---------------------------------------------------\n";
		for (std::map<int, isis_CADCommon::MAT1>::const_iterator ci = nasDeck.getMaterialData_MAT1().begin(); ci != nasDeck.getMaterialData_MAT1().end(); ci++)
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
		for (std::map<int, isis_CADCommon::MAT4>::const_iterator ci = nasDeck.getMaterialData_MAT4().begin(); ci != nasDeck.getMaterialData_MAT4().end(); ci++)
		{
			for (std::map<int, isis_CADCommon::PSolid>::const_iterator pi = nasDeck.getPsolidData().begin(); pi != nasDeck.getPsolidData().end(); pi++)
			{
				// Nastran uses MAT4 cards to specify coefficient of convection, where only H is defined,
				// with all other properties not listed. In Abaqus/Calculix this is a property of the setup,
				// not the material. So in Nastran the convection MAT4 card is a kind of 'dummy' card.
				// Abaqus requires all MAT4 cards to have all required properties. Since the convection MAT4
				// card doesn't apply to any PSOLIDs, it can be disregarded since the convection coefficient
				// is captured by the get_SurfaceConvection() helper function.
				if (pi->second.MID == ci->second.MID)
				{
					calculix << "*MATERIAL, NAME=MAT_MID_" << ci->first << "\n";
					if (ci->second.K != "")
						calculix << "*CONDUCTIVITY\n" << ci->second.K << "\n";
					if (ci->second.p != "")
						calculix << "*DENSITY\n" << ci->second.p << ",\n";
					if (ci->second.CP != "")
						calculix << "*SPECIFIC HEAT\n" << ci->second.CP << ",\n";
					calculix << "**\n";
				}
			}
		}
		//********************************************************************************************************************
		
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
			calculix << "*ELEMENT, TYPE=" << elementType << ", ELSET=" << psolidID << "\n";			// TODO: Change element type accordingly
			calculix << "**\n";
			
			for (std::map<int, isis_CADCommon::FEAElement>::const_iterator ret = nasDeck.getElementData().begin(); ret != nasDeck.getElementData().end(); ret++)
			{
				if (ret->second.PID != ci->first)
				{
					continue;
				}
				else
				{
					calculix << ret->second.EID;
					for (unsigned int i = 0; i < ret->second.GIDs.size(); i++)
					{
						calculix << ", " << ret->second.GIDs[i];
					}
					calculix << "\n";
				}
			}
			calculix << "**\n";		
		}

		calculix << "*ELSET, ELSET=ALL_3DSOLID\n";
		calculix << psolidList;
		calculix << "**\n";
		calculix << "*ELSET, ELSET=ALL_MASSIVE_ELEMENTS\n";
		calculix << "ALL_3DSOLID,\n";
		calculix << "**\n";
		calculix << "** ---------------------------------------------------\n";

		//********************************************************************************************************************
		
		calculix << "*STEP\n";
		calculix << analysisType << "\n";
		// TODO: NLPARAM - Time increment & max increment??

		//********************************************************************************************************************

		if (thermal == true)
		{
			bool defaultTemperature_set;
			double defaultTemperature;
			calculix << "** DEFAULT INITIAL TEMPERATURE \n";
			calculix << "*INITIAL CONDITIONS, TYPE=TEMPERATURE\n";
			deckHelper.getDefaultGridPointTemperature(defaultTemperature_set, defaultTemperature);
			if (defaultTemperature_set == true)
				calculix << "NS_TR_CID_" << nasDeck.getGridDisplacementCord_ID() << ", " << defaultTemperature << "\n";
			calculix << "**\n";

			calculix << "*NODE PRINT, NSET=NS_TR_CID_" << nasDeck.getGridDisplacementCord_ID() << "\n";
			calculix << "NT\n";

			// Boundary Constraints - Thermal (specified temperature)
			std::map<int,double> gridPointToSpecifiedTemperature_map;
			std::map<int,std::vector<int>> temperatureToNode_map;
			deckHelper.getSpecifiedGridPointTemperatures(gridPointToSpecifiedTemperature_map);
			if (gridPointToSpecifiedTemperature_map.size() > 0)
			{
				calculix << "**\n";
				calculix << "** SPECIFIED TEMPERATURE LOADS\n";
				calculix << "*BOUNDARY, OP=NEW\n";
				for each ( const std::pair<int, double> &i in gridPointToSpecifiedTemperature_map )
				{
					// Abaqus Heat Transfer DOF = 11
					calculix << i.first << ", " << "11" << "," << "11" << ", " << i.second << "\n";
					std::vector<int> nodeVec;
					if (temperatureToNode_map.count(i.second) == 1)
					{
						nodeVec = temperatureToNode_map[i.second];
						nodeVec.push_back(i.first);
						temperatureToNode_map[i.second] = nodeVec;
					}
					else
					{
						nodeVec.push_back(i.first);
						temperatureToNode_map[i.second] = nodeVec;
					}
				}

				calculix << "**\n";
				calculix << "** SET INITIAL TEMPERATURE FOR SPECIFIED TEMPERATURE LOADS\n";
				for (std::map<int, std::vector<int>>::iterator it = temperatureToNode_map.begin(); it != temperatureToNode_map.end(); ++it)
				{
					calculix << "*INITIAL CONDITIONS, TYPE=TEMPERATURE\n";
					for ( unsigned int i = 0; i < it->second.size(); i++ )
					{
						calculix << it->second[i] << ", " << it->first << "\n";
					}
				}
			}

			// Surface Heat Flux: support DFLUX only
			std::vector<isis_CADCommon::HeatFluxLoad> heatFluxLoads;
			deckHelper.getHeatFluxLoadsForBoundarySurfaces(heatFluxLoads);
			if (heatFluxLoads.size() > 0)
			{
				calculix << "**\n";
				calculix << "*DFLUX\n";
				std::map<int, isis_CADCommon::FEAElement> elementData = nasDeck.getElementData();
				for (std::vector<isis_CADCommon::HeatFluxLoad>::const_iterator hfi = heatFluxLoads.begin(); hfi != heatFluxLoads.end(); hfi++)
				{
					int face = 0;
					std::vector<int> faceNodes;
					int currentElement = hfi->elementIDThatContainsSurface;
					for (std::vector<int>::size_type gi = 0; gi != hfi->surfaceGridPointIDs.size()-3; gi++)
					{
						for (unsigned int i = 0; i < elementData[currentElement].GIDs.size(); i++)
						{
							if ( elementData[currentElement].GIDs[i] == hfi->surfaceGridPointIDs[gi] )
								faceNodes.push_back(i+1);  // Add 1: index is zero-based.
						}
					}
					DetermineElementFace(faceNodes, face);
					calculix << hfi->elementIDThatContainsSurface << ", " << "S" << face << ", " << hfi->Q0 << "\n";
				}
			}

			// Heat Generation (Body Heat Flux)
			std::vector<isis_CADCommon::VolumetricHeatGeneration> heatGenLoads;
			deckHelper.getVolumetricHeatGenerations(heatGenLoads);
			if (heatGenLoads.size() > 0)
			{
				calculix << "**\n";
				calculix << "*DFLUX\n";
				for (std::vector<isis_CADCommon::VolumetricHeatGeneration>::const_iterator bhfi = heatGenLoads.begin(); bhfi != heatGenLoads.end(); bhfi++)
				{
					for (std::vector<int>::size_type hgi = 0; hgi != bhfi->elementIDs.size(); hgi++)
					{
						calculix << bhfi->elementIDs[hgi] << ", " << "BF, " << bhfi->powerInputPerVolume << "\n";
					}
				}
			}

			// Surface Convection
			std::vector<isis_CADCommon::SurfaceConvection> surfaceConvection;
			deckHelper.getSurfaceConvectionConstraints(surfaceConvection);
			if (surfaceConvection.size() > 0)
			{
				calculix << "**\n";
				calculix << "*FILM\n";
				std::map<int, isis_CADCommon::FEAElement> elementData = nasDeck.getElementData();
				for (std::vector<isis_CADCommon::SurfaceConvection>::const_iterator sci = surfaceConvection.begin(); sci != surfaceConvection.end(); sci++)
				{
					int face = 0;
					std::vector<int> faceNodes;
					int currentElement = sci->elementIDThatContainsSurface;
					// Determine which element surface the grid points correspond to
					for (std::vector<int>::size_type gi = 0; gi != sci->surfaceGridPointIDs.size()-3; gi++)
					{
						for (unsigned int i = 0; i < elementData[currentElement].GIDs.size(); i++)
						{
							if ( elementData[currentElement].GIDs[i] == sci->surfaceGridPointIDs[gi] )
								faceNodes.push_back(i+1);  // Add 1: index is zero-based.
						}
					}
					DetermineElementFace(faceNodes, face);
					calculix << sci->elementIDThatContainsSurface << ", " << "F" << face << ", ";
					calculix << sci->ambientTemperature << ", " << sci->convectionCoefficient << "\n";
				}
			}
		}
		
		for (std::map<std::string, std::vector<std::pair<std::string, std::string>>>::const_iterator ci = nasDeck.getCaseControlData().subcases.begin(); 
			ci != nasDeck.getCaseControlData().subcases.end(); 
			ci++)
		{

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
			
			// Boundary Constraints - Structural
			// Nastran deck outputs specified temperatures as SPC loads, but under DOF 1, whereas Abaqus has DOF 11.
			// Since SPC is the same card for fixed structural constraints, if the input was a coupled analysis
			// you would need to know which SPC card to override the DOF to 11. At the moment it can be ambiguous
			// whether the SPC card refers to the DOF 1 (Force X-dir) or DOF 11 (thermal).
			if (nasDeck.getSpcData_SIDKey().size() > 0)
			{
				calculix << "*BOUNDARY, OP=NEW\n";

				for (std::multimap<int, isis_CADCommon::SPC>::const_iterator bi = nasDeck.getSpcData_SIDKey().begin(); bi != nasDeck.getSpcData_SIDKey().end(); bi++)
				{
					calculix << bi->second.G1 << ", " << bi->second.C1 << "," << bi->second.C1 << ", " << bi->second.D1 << "\n";
				}
			}

			// Force loads
			if (nasDeck.getForceLoadData().size() > 0)
			{
				calculix << "**\n";
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
			calculix << "**\n";
		}

		if (structural == true)
		{
			calculix << "*NODE FILE\n";
			calculix << "U\n";
			calculix << "*EL FILE\n";
			calculix << "S\n";
		}
		else if (thermal == true)
		{
			calculix << "*NODE FILE\n";
			calculix << "NT\n";
		}
		calculix << "*END STEP\n";
		
		/********************************************************************************************************************/


		// close file
		calculix.close();
	} // void CalculixConverter::ConvertNastranDeck

	
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
		for (std::map<int, isis_CADCommon::FEAElement>::const_iterator ci = nasDeck.getElementData().begin(); 
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

			for (size_t i = 0; i < ci->second.GIDs.size(); i++)
			{
				elmerMesh << " " << ci->second.GIDs[i];
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