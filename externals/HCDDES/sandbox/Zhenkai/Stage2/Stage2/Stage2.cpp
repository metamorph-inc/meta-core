#include <iostream>
#include <string>
using namespace std;

#include <UdmBase.h>
#include <Semantics.h>

#include "Stage2.h"


int main(int argc, char *argv[])
{
	int i;
	string inputFileName;

	// parse the command line
	for (i = 1; i < argc; i++)
		if (!strcmp("-f", argv[i]))
			inputFileName = argv[i + 1];

	Udm::SmartDataNetwork sdn(Semantics::diagram);

	try {
		sdn.OpenExisting(inputFileName);

		// Get the root folder
		Semantics::RootFolder rootFolder = Semantics::RootFolder::Cast(sdn.GetRootObject());
		
		Generate_C_Code(rootFolder);

	}
	catch (udm_exception &e) {
		cout << e.what() << endl;
		exit(1);
	}
	catch ( ... ) {
		cout << "Unkown exception in Stage 2!" << endl;
		exit(2);
	}
	


}
