#if 0

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		odb_initializeAPI();
		odb_Odb& odb = openOdb("C:\\Users\\yaod\\Desktop\\junk_udm_64bit\\Analysis\\Chassis_Damper_FEA.odb", true);


#if 0

		/*
		NumericalProblemSummary:
			- Negative Eigen Values
			- Numerical Singularities
			- Zero Pivots
		*/
		odb_DiagnosticData diagnosticData = odb.diagnosticData();
		odb_NumericalProblemSummary npSummary = diagnosticData.numericalProblemSummary();
		//npSummary.numberOfNegativeEigenvalues();
		//npSummary.numberOfNumericalSingularities();
		//npSummary.numberOfZeroPivots();

		odb_SequenceDiagnosticStep& stepSeq = diagnosticData.steps();
		odb_DiagnosticStep& step = stepSeq[0];

		int xpl = step.XplStatus().size();
		int contact = step.contactDiagnostics().size();
		double eLen = step.characteristicElementLength();

		odb_SequenceAnalysisWarning analysisSeq= diagnosticData.analysisWarnings();
		int size = analysisSeq.size();

		for (int i = 0; i < size; i++)
		{
			std::cout<< "Warning [" << i << "]: " << analysisSeq[i].description().CStr() << std::endl;
			odb_AnalysisWarning& analysisWarning = analysisSeq[i];

			std::string description = analysisWarning.description().CStr();
			int size2 = analysisWarning.elementDescriptions().size();
			for (int j = 0; j < size2; j++)
			{
				std::string tmp = analysisWarning.elementDescriptions()[i].CStr();
			}
		//	dia_MetadataTable = analysisWarning.getMetadata();

			odb_SequenceAuxiliaryData auxData = analysisWarning.data();
			int category = analysisWarning.category().categoryEnum;
			int auxSize = auxData.size();

			for (int j = 0; j < auxSize; j++)
			{
				odb_AuxiliaryData& data = auxData[j];
				odb_SequenceInt labels = data.elementLabels();

				int Lsize = 0;
				Lsize= data.elementInstanceNames().size();
				bool val = data.hasValues();
				Lsize = labels.size();
			}
		}


#if 0
//		odb_SequenceAnalysisError errorSeq = diagnosticData.analysisErrors();
//		int size2 = errorSeq.size();
//		for (int i = 0; i < size2; i++)
		{
//			std::cout<< "Error [" << i << "]: " << errorSeq[i].description().CStr() << std::endl;
		}



#endif

#endif
		bool isClosed = odb.isClosed();
		odb_String eType = "ALL ELEMENT";
		odb_Assembly& assembly = odb.rootAssembly();
		
		std::ofstream file;
		file.open("ElementPrintout.txt");
		

		// Find Instance:ElementSets
		file<<"\nInstance:ElementSets\n";
		file<<"======================================\n";
		odb_InstanceRepositoryIT instIter2(assembly.instances());
		for (instIter2.first(); !instIter2.isDone(); instIter2.next())
		 {
			string key = instIter2.currentKey().CStr();
			odb_Instance& instance1 = 
				assembly.instances()[instIter2.currentKey()];

			odb_Enum::odb_DimensionEnum instanceType = 
				instance1.embeddedSpace();

			const odb_SequenceElement& all = instance1.elements();
			int size = all.size();
			file << "	ElementSet Keys:\n";
			odb_SetRepositoryIT setItE( instance1.elementSets() );
			for (setItE.first(); !setItE.isDone(); setItE.next())
				file<< "	" << setItE.currentKey().CStr() << "\n";
		}


		// Find RootAssembly:ElementSets
		file<<"\nRootAssembly::ElementSets\n";
		file<<"======================================\n";
		file << "	ElementSet Keys:\n";
		odb_SetRepositoryIT setItE(assembly.elementSets());
		for (setItE.first(); !setItE.isDone(); setItE.next())
			file << "	" << setItE.currentKey().CStr() << "\n";

		odb_Set& distorted = assembly.elementSets()["WarnElemDistorted"];		
		odb_Set& warped = assembly.elementSets()["WarnElemWarped"];
		file<< "	Distorted: " << distorted.size() << " Warped: " << warped.size() <<"\n";



		odb_Instance& instance = assembly.instances()["PART-1-1"];
		odb_Set& cadComponentElements = instance.elementSets()["PSHELL_1"];

		odb_StepRepository& sRep1 = odb.steps();        
		odb_StepRepositoryIT sIter1 (sRep1);
		for (sIter1.first(); !sIter1.isDone(); sIter1.next())
		{

			odb_Step& step = sRep1[sIter1.currentKey()];
			cout<<"Processing Step: "<<step.name().CStr()<<endl;
			file<<"\nProcessing Step: "<<step.name().CStr()<<"\n";
			odb_SequenceFrame& frameSequence = step.frames();
			int numFrames = frameSequence.size();   

			//for (int f = 0; f < numFrames; f++) 
			{
				  odb_Frame& frame = frameSequence[numFrames-1]; 
				  file << "Frame -  " << " Description[" << frame.description().CStr() << "]   LoadCase[" << frame.loadCase().name().CStr() << "]\n";

				 // odb_FieldOutputRepository& fieldOutputRep = frame.fieldOutputs();
				 // odb_FieldOutputRepositoryIT fieldIter( fieldOutputRep );
				 //  cout << endl << "Frame[" << f << "]" << endl;
				 // for (fieldIter.first(); !fieldIter.isDone(); fieldIter.next())
				 // {
				//	  odb_FieldOutput& field = fieldOutputRep[fieldIter.currentKey()];
				//	  cout << field.name().CStr() << ":" << field.description().CStr() << endl;
				 // }

				  // //////////////////////

				  odb_FieldOutput& fieldOutput = frame.fieldOutputs()["S"];
				  cout<< " Total StressField:" << fieldOutput.values().size() << endl;
				  odb_FieldOutput stressOutput = fieldOutput.getSubset(cadComponentElements);
				  odb_SequenceInvariant invariants = stressOutput.validInvariants();
				  int invariantSize = invariants.size();
				  file<< "ValidInvariants Size:" << invariantSize << "\n";
				  bool bmises = invariants.isMember(odb_Enum::MISES),
					  btresca = invariants.isMember(odb_Enum::TRESCA),
					  bpress = invariants.isMember(odb_Enum::PRESS),
					  bmaxPrinciple = invariants.isMember(odb_Enum::MAX_PRINCIPAL),
					  bminPrinciple = invariants.isMember(odb_Enum::MIN_PRINCIPAL),
					  bmidPrinciple = invariants.isMember(odb_Enum::MID_PRINCIPAL),
					  bmaxInPlane = invariants.isMember(odb_Enum::MAX_INPLANE_PRINCIPAL),
					  bminInPlane = invariants.isMember(odb_Enum::MIN_INPLANE_PRINCIPAL),
					  boutPlane = invariants.isMember(odb_Enum::OUTOFPLANE_PRINCIPAL);

				  float mises = 0, 
					  tresca = 0, 
					  press = 0,
					  maxPrinciple = 0, 
					  minPrinciple = 0, 
					  midPrinciple = 0, 
					  maxInPlane = 0,
					  minInPlane = 0,
					  outPlane = 0;

				  file<<bmises << " " << btresca 
					  << " " << bpress 
					  << " " << bmaxPrinciple 
					  << " " << bminPrinciple 
					  << " " << bmidPrinciple 
					  << " " << bmaxInPlane
					  << " " << bminInPlane
					  << " " << boutPlane
					  << "\n";

				  const odb_SequenceFieldValue& seqVal = stressOutput.values();
				  int numValues = seqVal.size();
				  cout << "Stress Fields for ELSET:" << numValues << endl;
				  for (int i = 0; i < numValues; i++)
				  {
					  float tmp;
					  const odb_FieldValue val = seqVal[i];
					  if (bmises)
					  {
						file<< "Mises: " << val.mises();
					  }
					  
					  file << "		";
					  if (bpress)
					  {
						file<< "Pressure: " << val.press();
					  }

					  file << "		";
					  if (bmaxPrinciple)
					  {
						  file<< "MaxPrinciple: " <<val.maxPrincipal();
					  }

					  file << "		";
					  if (bminPrinciple)
					  {
						  file << "MinPrinciple: " << val.minPrincipal();
					  }
						
					  file << "		";
					  if (bmidPrinciple)
					  {
						  file << "MidPrinciple: " << val.midPrincipal();
					  }

					  file << "		";
					  if (bmaxInPlane)
					  {
						  file << "MaxInPlane: " << val.maxInPlanePrincipal();
					  }
						   
					  file << "		";
					  if (bminInPlane)
					  {
						  file << "MinInPlane: " << val.minInPlanePrincipal();
					  }

					  file << "		";
					  if (boutPlane)
					  {
						  file << "OutOfPlane: " << val.outOfPlanePrincipal();
					  }

					  file<<"\n";
				  }
			}
		}


		// Printing out SectionCategories
#if 0
		// RootAssembly:SectionCategories
		file<<"\nRootAssembly:SectionCategories\n";
		file<<"=========================\n";		
		
		odb_SequenceSectionAssignment& aSecAssign = assembly.sectionAssignments();
		int size = aSecAssign.size();
		for (int i = 0; i < size; i++)
		{
			odb_SectionAssignment assign = aSecAssign[i];

			odb_Set& regions = assign.region();
			file<<"	RegionName: "<< regions.name().CStr() << " RegionType: " << regions.type() << "\n";

			//odb_Section& section = assign.section();
			//file<<section.subTypeIdentifier() << " || " <<section.typeIdentifier() << " || " << section.name().CStr() << " || " << assign.sectionName().CStr();
		}

		// Instance:SectionCategories
		file<<"\nInstance:SectionCategories\n";
		file<<"=========================\n";

		odb_InstanceRepositoryIT instIter(assembly.instances());
		for (instIter.first(); !instIter.isDone(); instIter.next())
		 {
			string key = instIter.currentKey().CStr();
			odb_Instance& instance = 
				assembly.instances()[instIter.currentKey()];

			odb_SequenceSectionAssignment& iSecAssign = instance.sectionAssignments();
			size = iSecAssign.size();
			for (int i = 0; i < size; i++)
			{
				odb_SectionAssignment assign = iSecAssign[i];

				odb_Set& regions = assign.region();
				file<< "	SectionName: " << assign.sectionName().CStr() << "	RegionName: "<< regions.name().CStr() << " RegionType: " << regions.type() << "\n";

				//odb_Section& section = assign.section();
				//file<<section.subTypeIdentifier() << " || " <<section.typeIdentifier() << " || " << section.name().CStr() << " || " << assign.sectionName().CStr();
			}
		}
#endif



	/*
		odb_InstanceRepositoryIT instIter(assembly.instances());
		for (instIter.first(); !instIter.isDone(); instIter.next())
		 {
			odb_Instance& instance1 = 
				assembly.instances()[instIter.currentKey()];
			odb_Enum::odb_DimensionEnum instanceType = 
				instance1.embeddedSpace();
    
			const odb_SequenceElement& elementList = 
				instance1.elements();
			int elementListSize = elementList.size();
			std::cout << "Element Connectivity Data" << std::endl;
			std::cout << "Element Label : constituent node labels ..." 
				<< std::endl;
			int numNodes = 0;
			for (int e=0; e<elementListSize; e++) {
				const odb_Element element = elementList[e];
				int elementLabel = element.label();
			//	std::cout << elementLabel <<" : ";
				odb_String elementType = element.type();				
			//	std::cout << "||Type: " << elementType.CStr() << " ||CategoryDescription: " << element.sectionCategory().description().CStr() << " ||CategoryName: " << element.sectionCategory().name().CStr() << std::endl;

				//const int* const conn = 
				//	element.connectivity(numNodes);
				//for (int j=0; j<numNodes; j++)
				 // std::cout << "  " << conn[j];
			//	std::cout << std::endl;

				file<<elementLabel<<"||		"<<elementType.CStr()<< "||		"<<element.sectionCategory().description().CStr()<<"||		"<<element.sectionCategory().name().CStr()<<"\n";
			//	odb_SequenceSectionPoint points = element.sectionCategory().sectionPoints();

			}
		}
			*/
						file.close();

		
		/*
		for (instIter.first(); !instIter.isDone(); instIter.next())
		 {
			odb_Instance& instance1 = 
				assembly.instances()[instIter.currentKey()];

			cout << "Element set keys:" << endl;
			odb_SetRepositoryIT setItE( instance1.elementSets() );
			for (setItE.first(); !setItE.isDone(); setItE.next())
				cout << setItE.currentKey().CStr() << endl;

			cout << endl << endl << " Instance Section Assignments: " << endl;

			odb_SequenceSectionAssignment& secRepo =  instance1.sectionAssignments();
			int size = secRepo.size();
			for (int i = 0; i < size; i++)
			{
				 odb_SectionAssignment& assigment = secRepo[i];
				 cout<< " assigment: " << assigment.sectionName().CStr() << endl;
			}
		}
		*/

		/*
		for (instIter.first(); !instIter.isDone(); instIter.next()) 
		{
			const odb_Instance& instance = instIter.currentValue();      
			odb_SequenceSectionAssignment sectionAssignmentSeq = 
				instance.sectionAssignments();  
			int sects = sectionAssignmentSeq.size();
			cout << "Instance : " << instance.name().CStr() << endl;
			for (int s = 0; s < sects; ++s) {
			odb_SectionAssignment sa = sectionAssignmentSeq[s];
			odb_Section aSection = sa.section();

			odb_String sectionName = sa.sectionName();
				cout << "  Section : " << sectionName.CStr() << endl; 
			odb_Set set = sa.region();
			const odb_SequenceElement& elements = set.elements();
			int size = elements.size();
				cout << "  Elements associated with this section : " 
					 << endl;
			for (int e = 0; e< size; ++e)
				cout << elements[e].label() << endl;	  
			} 
		}
		*/

		/*
		for (instIter.first(); !instIter.isDone(); instIter.next())
		 {
			odb_Instance& instance = 
				assembly.instances()[instIter.currentKey()];		
			
			
			odb_Set& center = instance.nodeSets()["PUNCH"];
			odb_FieldOutput& fieldU = lastFrame.fieldOutputs()["U"];
			odb_FieldOutput centerDisp = fieldU.getSubset(center);
			const odb_SequenceFieldValue& centerValues = 
				centerDisp.values();    
			const odb_FieldValue val = centerValues.value(0);
			const float* const data = val.data(numComp);
			cout << " Node: " << val.nodeLabel() << endl;
			cout << " U = ";
			for (int comp=0;comp<numComp;comp++)
			  cout << data[comp] << "  ";   
			cout << endl;

		}
		*/

		/*
		odb_SectionCategoryRepositoryIT sIter(odb.sectionCategories());
		for (sIter.first(); !sIter.isDone(); sIter.next())
		{
			cout << "Section Name : " << sIter.currentKey().CStr() << endl;
		}

		odb_SectionApi sectionApi;
		odb.extendApi(odb_Enum::odb_SECTION,sectionApi); 
		odb_SectionContainer&  sectionContainer = sectionApi.sections();
		odb_SectionContainerIT scIT(sectionContainer);
		for (scIT.first(); !scIT.isDone(); scIT.next()) {
			cout << "Section Name : " << scIT.currentKey().CStr() << endl;
			const odb_Section& sect = scIT.currentValue();
			sect.subTypeIdentifier();
		}
		*/


	//	double var;
	//	std::cout << "" << std::endl;
	//	std::cin >> var;
	}
	catch (odb_Exception& e)
	{
		std::string msg = e.ErrMsg().text();
		int errorN = e.ErrorNo();
		std::cout << "ODB Exception: " << msg.c_str() << " ErrorNo: " << errorN << std::endl;

		double var;
		std::cin >> var;
	}
	catch (...)
	{
		std::string msg = "None Abaqus related error!";
		std::cout<< "None ODB exception!" << std::endl;
	}
	return 0;
}


#endif