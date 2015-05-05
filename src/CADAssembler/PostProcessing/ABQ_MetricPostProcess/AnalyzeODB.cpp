#include "AnalyzeODB.h"


#include <odb_API.h>
#include <odb_NumericalProblemSummary.h>
#include <dia_OdbEnums.h>
#include <odb_SequenceXplStatus.h>
#include <odb_SequenceContactDiagnostic.h>
#include <odb_SectionTypes.h>
#include <iostream>
#include <fstream>


extern string gPath;
extern string gConfigID;
extern map<string, vector<Metric>> gMetricPairs;	// [CompID, Metrics]
extern map <string, string> gElementSets;			// [CompID, ElementSetID]
extern map <string, Material> gMaterials;			// [CompID, Material]
extern std::ofstream gFile;

#define STRESS_COMP	"S"
#define DISP_COMP	"U"



float AnalyzeFEAResults()
{
	float maxVonMises = 1;
	map<string, StructuralResults> results;
	if (!FindStressFromODB(results))
		return maxVonMises;
	
	if (results.size() > 0)
		maxVonMises = results.begin()->second.mises;
	for (map<string, StructuralResults>::const_iterator ci = results.begin(); ci != results.end(); ci++)
	{
		gFile << "\n\n[" << ci->first << "] Metrics Value\n";

		float tmpMises = ci->second.mises;
		if (tmpMises > maxVonMises)
			maxVonMises = tmpMises;

		map<string, Material>::const_iterator di = gMaterials.find(ci->first);

		if (di != gMaterials.end())
		{
			float FOS = 0, tensileFOS = 0, pressFOS = 0, shearFOS = 0;

			float materialVal, partVal;

			materialVal = di->second.mises, partVal = ci->second.mises;
			if (partVal != 0)
			{
				tensileFOS = abs(materialVal/partVal);
				gFile<< "tensile Factor Of Safety: " << tensileFOS <<"\n";
			}

			materialVal = di->second.bearing, partVal = ci->second.press;
			if (partVal != 0)
			{
				pressFOS = abs(materialVal/partVal);
				gFile<< "press Factor Of Safety: " << pressFOS <<"\n";
			}

			materialVal = di->second.shear, partVal = ci->second.tresca;
			if (partVal != 0)
			{
				shearFOS = abs(materialVal/partVal);
				gFile<< "shear Factor Of Safety: " << shearFOS <<"\n";
			}

			FOS = min(tensileFOS, pressFOS);
			FOS = min(FOS, shearFOS);

			map<string, vector<Metric>>::iterator i = gMetricPairs.find(ci->first);
			if (i != gMetricPairs.end())
			{
				vector<Metric>& metrics = i->second;
				for (unsigned int i = 0; i < metrics.size(); i++)
				{
					string tmp;
					int type = metrics[i].type;
					switch (type)
					{
					case FACTOR_OF_SAFETY:
						{
							to_string(tmp, FOS);
							metrics[i].MetricValue = tmp;
						}
						break;
					case RESULT_QUALITY:
						metrics[i].MetricValue = "Low";
						break;
					case PART_MAX_VON_MISES:
						{
							to_string(tmp, ci->second.mises);
							metrics[i].MetricValue = tmp;
						}
						break;
					case PART_MAX_BEARING:
						{
							to_string(tmp, ci->second.press);
							metrics[i].MetricValue = tmp;
						}
						break;
					case PART_MAX_SHEAR:
						{
							to_string(tmp, ci->second.tresca);
							metrics[i].MetricValue = tmp;
						}
						break;
					}				
					metrics[i].MetricValue;
				}
			}
			
		}
	}

	return maxVonMises;
}

bool FindStressFromODB(map<string, StructuralResults>& results)
{
	cout << gPath << endl;
	if (gPath.find(".odb") == std::string::npos)
	{
		cout << "Invalid ODB file name!" << endl;
		return 0;
	}

	odb_initializeAPI();
	odb_Odb& odb = openOdb(gPath.c_str(), true);
	bool isClosed = odb.isClosed();
//		odb_String eType = "ALL ELEMENT";
	odb_Assembly& assembly = odb.rootAssembly();
		
	odb_Instance& instance = assembly.instances()["PART-1-1"];

	map <string, string>::const_iterator ci = gElementSets.begin();
	for (; ci != gElementSets.end(); ci++)
	{
		gFile << "[" << ci->first << ":" << ci->second << "]\n";
		StructuralResults result;

		odb_Set& cadComponentElementSet = instance.elementSets()[ci->second.c_str()];		
		odb_StepRepository& sRep1 = odb.steps();        
		odb_StepRepositoryIT sIter1 (sRep1);

		float mises = 0, 
			tresca = 0, 
			press = 0,
			maxPrinciple = 0, 
			minPrinciple = 0, 
			midPrinciple = 0, 
			maxInPlane = 0,
			minInPlane = 0,
			outPlane = 0,
			maxDisplacement = 0;

		for (sIter1.first(); !sIter1.isDone(); sIter1.next())
		{
			odb_Step& step = sRep1[sIter1.currentKey()];
			cout<<"Processing Step: "<<step.name().CStr()<<endl;
			//file<<"\nProcessing Step: "<<step.name().CStr()<<"\n";
			odb_SequenceFrame& frameSequence = step.frames();
			int numFrames = frameSequence.size();   

			odb_Frame& frame = frameSequence[numFrames-1];			// last frame
			//file << "Frame -  " << " Description[" << frame.description().CStr() << "]   LoadCase[" << frame.loadCase().name().CStr() << "]\n";

			// GETTING STRESS VALUES
			odb_FieldOutput& fieldOutput = frame.fieldOutputs()[STRESS_COMP];

			//cout<< " Total StressField:" << fieldOutput.values().size() << endl;
			odb_FieldOutput stressOutput = fieldOutput.getSubset(cadComponentElementSet);
			odb_SequenceInvariant invariants = stressOutput.validInvariants();
			int invariantSize = invariants.size();
			//file<< "ValidInvariants Size:" << invariantSize << "\n";
			bool bmises = invariants.isMember(odb_Enum::MISES),
				btresca = invariants.isMember(odb_Enum::TRESCA),
				bpress = invariants.isMember(odb_Enum::PRESS),
				bmaxPrinciple = invariants.isMember(odb_Enum::MAX_PRINCIPAL),
				bminPrinciple = invariants.isMember(odb_Enum::MIN_PRINCIPAL),
				bmidPrinciple = invariants.isMember(odb_Enum::MID_PRINCIPAL),
				bmaxInPlane = invariants.isMember(odb_Enum::MAX_INPLANE_PRINCIPAL),
				bminInPlane = invariants.isMember(odb_Enum::MIN_INPLANE_PRINCIPAL),
				boutPlane = invariants.isMember(odb_Enum::OUTOFPLANE_PRINCIPAL);

#if 0
			file<<bmises << " " << btresca 
				<< " " << bpress 
				<< " " << bmaxPrinciple 
				<< " " << bminPrinciple 
				<< " " << bmidPrinciple 
				<< " " << bmaxInPlane
				<< " " << bminInPlane
				<< " " << boutPlane
				<< "\n";
#endif
				
			const odb_SequenceFieldValue& seqVal = stressOutput.values();
			int numValues = seqVal.size();
			int totalValues = fieldOutput.values().size();


			cout << "Stress Fields for ELSET[" << ci->second << "]: " << numValues << "		TOTAL Stress Fields Size: " << totalValues << endl;
				
			result.hasMises = bmises;
			result.hasTresca = btresca;
			result.hasPress = bpress;

			for (int i = 0; i < numValues; i++)
			{
				float tmp = 0;
				const odb_FieldValue val = seqVal[i];
				if (bmises)
				{
					tmp = val.mises();
					//file<< "Mises: " << tmp;
					if (tmp > mises)
						mises = tmp;
				}
				
				tmp = 0;
				//file << "		";
				if (bpress)
				{
					tmp = val.press();
					//file<< "Pressure: " << tmp;
					if (tmp > press)
						press = tmp;
				}

				tmp = 0;
				//file << "		";
				if (btresca)
				{
					tmp = val.tresca();
					//file<< "Tresca: " << tmp;
					if (tmp > tresca)
						tresca = tmp;
				}

				tmp = 0;
#if 0					//file << "		";
				if (bmaxPrinciple)
				{
					tmp = val.maxPrincipal();
					//file<< "MaxPrinciple: " << tmp;
					if (tmp > maxPrinciple)
						maxPrinciple = tmp;
				}

				tmp = 0;
				//file << "		";
				if (bminPrinciple)
				{
					tmp = val.minPrincipal();
				//	file << "MinPrinciple: " << tmp;
					if (tmp < minPrinciple)
						minPrinciple = tmp;
				}
					
				tmp = 0;
				//file << "		";
				if (bmidPrinciple)
				{
					tmp = val.midPrincipal();
					//file << "MidPrinciple: " << tmp;
				}

				tmp = 0;
				//file << "		";
				if (bmaxInPlane)
				{
					tmp = val.maxInPlanePrincipal();
					//file << "MaxInPlane: " << tmp;
					file << tmp << "\n";
					if (tmp > maxInPlane)
						maxInPlane = tmp;
				}
					
				tmp = 0;
				//file << "		";
				if (bminInPlane)
				{
					tmp = val.minInPlanePrincipal();
					//file << "MinInPlane: " << tmp;
					if (tmp < minInPlane)
						minInPlane = tmp;
				}

				tmp = 0;
				//file << "		";
				if (boutPlane)
				{
					tmp = val.outOfPlanePrincipal();
				//	file << "OutOfPlane: " << tmp;
				}
#endif
				//file<<"\n";
			}

			
			// GETTING MAX DISPLACEMENT VALUE
			odb_FieldOutput& displacement_fieldOutput = frame.fieldOutputs()[DISP_COMP];
			odb_FieldOutput displacementOutput = displacement_fieldOutput.getSubset(cadComponentElementSet);
			odb_SequenceInvariant displacementInvariants = displacementOutput.validInvariants();
			invariantSize = displacementInvariants.size();
			bool bMag = displacementInvariants.isMember(odb_Enum::MAGNITUDE);
			const odb_SequenceFieldValue& displacementVal= displacementOutput.values();
			numValues = displacementVal.size();
				
			result.hasDisplacement = bMag;

			for (int i = 0; i < numValues; i++)
			{
				float tmp = 0;
				const odb_FieldValue val = displacementVal[i];
				if (bMag)
				{
					tmp = val.magnitude();
					//file<< "MaxDisplacement: " << tmp;
					if (tmp > maxDisplacement)
						maxDisplacement = tmp;
				}
			}

		} //end step
		result.mises = mises;
		result.press = press;
		result.tresca = tresca;
		result.maxDisplacement = maxDisplacement;

		results[ci->first] = result;
		gFile<< "Mises Max: " << mises << " Tresca Max: " << tresca << "	Press Max: " << press << "\n";

	} // end elementSet

	return 1;
}


void WriteMetricsFile()
{
	// write metrics dashboard file
	std::ofstream file;
	file.open("FEAResultOut.xml");

	if (file.is_open())
	{
		file<<"<RootObjectType>\n";
		file<<"<Configurations>\n";
		file<<"<Configuration"<< " ID=\"" << gConfigID << "\" Name=\"\"" << ">\n";
		for (map<string, vector<Metric>>::const_iterator ci = gMetricPairs.begin(); ci != gMetricPairs.end(); ci++)
		{
			const vector<Metric>& metrics = ci->second;
			int size = metrics.size();
			for (unsigned int i = 0; i < size; i++)
			{
				file<<"<ConfigMetric DefID=\"" << metrics[i].MetricID << " Value=\"" << metrics[i].MetricValue << "\"/>\n";
			}
		}
		file<<"</Configuration>\n";
		file<<"</Configurations>\n";
		file<<"</RootObjectType>\n";
		file.close();
	}


	file.open("FEAResultOut.csv");
	if (file.is_open())
	{
		file<<"ConfigurationID,ComponentID,MetricID,MetricName,Value"<<"\n";
		file<<gConfigID << ">\n";
		for (map<string, vector<Metric>>::const_iterator ci = gMetricPairs.begin(); ci != gMetricPairs.end(); ci++)
		{
			file<<","<<ci->first<<"\n";
			const vector<Metric>& metrics = ci->second;
			int size = metrics.size();
			for (unsigned int i = 0; i < size; i++)
			{
				file<<",," << metrics[i].MetricID << "," << metrics[i].type <<","<< metrics[i].MetricValue << "\n";
			}
		}

		file.close();
	}
}


// helper
void ParseElementIDs(string elementIDs)
{
	vector<string> tokens;
	Tokenizer(elementIDs, tokens, ",");
	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		unsigned int pos = tokens[i].find(":");
		if (pos == string::npos)
		{
			throw analyzer_exception("CommandLine (-e) parsing error: Missing Element Set ID."); // throw exception
		}
		else
		{			
			gElementSets[tokens[i].substr(0, pos)] = tokens[i].substr(pos+1);		// map[ComponentID] = ElementID;
		}
	}
}

// [CompID]MType:MID,MType:MID,MType:MID
void ParseMetricIDs(string metricIDs)
{
	vector<string> tokens;
	Tokenizer(metricIDs, tokens, "[");
	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		string tmp = tokens[i];
		unsigned int begin = tmp.find("]");
		vector<Metric> metricsVec;

		if (begin == string::npos)
		{
			throw analyzer_exception("CommandLine (-p) parsing error: Missing Component ID."); // throw exception
		}
		else
		{
			string id = tmp.substr(0, begin);
			tmp = tmp.substr(begin+1);
			vector<string> pairs;
			Tokenizer(tmp, pairs, ",");

			for (unsigned int k = 0; k < pairs.size(); k++)
			{			
				Metric metric;
				begin = pairs[k].find(":");
				if (begin == string::npos)
				{
					throw analyzer_exception("CommandLine (-p) parsing error: Missing metric type encountered."); // throw exception
				}
				else
				{
					string type = pairs[k].substr(0, begin);
					if (type == "FactorOfSafety")
						metric.type = FACTOR_OF_SAFETY;
					else if (type == "QualityIndicator")
						metric.type = RESULT_QUALITY;
					else if (type == "Mises")
						metric.type = PART_MAX_VON_MISES;
					else if (type == "Bearing")
						metric.type = PART_MAX_BEARING;
					else if (type == "Shear")
						metric.type = PART_MAX_SHEAR;
					else if (type == "Displacement")
						metric.type = DISPLACEMENT;
					else
					{
						throw analyzer_exception("CommandLine (-m) parsing error: Incorrect metric type encounctered [" + type + "]"); // throw exception;
					}
					metric.MetricID = pairs[k].substr(begin+1);
				}
				metricsVec.push_back(metric);
			}
			gMetricPairs[id] = metricsVec;
		}
	}
}

void ParseMaterials(string materialProps)
{
	vector<string> tokens;
	Tokenizer(materialProps, tokens, "[");
	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		string tmp = tokens[i];
		unsigned int begin = tmp.find("]");
		if (begin == string::npos)
		{
			throw analyzer_exception("CommandLine (-p) parsing error: Missing Component ID."); // throw exception
		}
		else
		{
			string id = tmp.substr(0, begin);
			tmp = tmp.substr(begin+1);
			vector<string> pairs;
			Tokenizer(tmp, pairs, ",");

			Material material;
			for (unsigned int k = 0; k < pairs.size(); k++)
			{
				begin = pairs[k].find(":");
				if (begin == string::npos)
				{
					throw analyzer_exception("CommandLine (-p) parsing error: Missing material property type."); // throw exception
				}
				else
				{
					string type = pairs[k].substr(0, begin);
					float value = atof(pairs[k].substr(begin+1).c_str());
					if (type == "Bearing")
						material.bearing = value;
					else if (type == "Mises")
						material.mises = value;
					else if (type == "Shear")
						material.shear = value;
					else
					{
						throw analyzer_exception("CommandLine (-p) parsing error: Incorrect material property type encounctered [" + type + "]");	// throw exception
					}
				}
			}
			gMaterials[id] = material;
		}
	}
}


void Tokenizer(const string& str, vector<string>& tokens, const string& delimiters)
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}
