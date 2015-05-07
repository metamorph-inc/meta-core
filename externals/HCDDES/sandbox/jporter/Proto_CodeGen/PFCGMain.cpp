// Code originally by Peter Volgyesi

#include <direct.h>
#include <errno.h>
#include <string>
#include <google/template.h>

#include "UdmEngine.hpp"

#include "PF_CodeGen.h"


// TODO : refactor these to arguments/instance members/whatever
string rootDir;
string appName;
google::TemplateDictionary dict("PF_CodeGen");

const std::string PFCG_VERSION = "0.1";

void printVer()
{
	std::cerr << "FRED Platform Code Generator v" << PFCG_VERSION << " \n(c) 2003-2008 Vanderbilt University\nInstitute for Software Integrated Systems\n\n";
}

int main(int argc, char *argv[])
{
	
	

	printVer();
	if (argc == 1 || argc > 3)
	{

		cerr << "Usage: PF_CodeGen.exe [output_directory] <ESMoL.mga>" << endl;
		exit(1);
	}

	string esmolFile;

	if (argc == 2)
	{
		rootDir = _getcwd(NULL, _MAX_PATH);
		rootDir += "\\platform\\";
		esmolFile = argv[1];
	}
	else if (argc == 3)
	{
		rootDir = argv[1];
		esmolFile = argv[2];
	}

	cout << "Generating code from " << esmolFile << " in " << rootDir << endl << endl;

	int r = _mkdir(rootDir.c_str());
	if (r  && errno != EEXIST) {
		perror("Unable to create output directory");
		exit(1);
	}

	UdmEngine::open( esmolFile );

	size_t appNameBegin = esmolFile.find_last_of("\\");
	if (appNameBegin < 0) 
		appNameBegin = 0;
	else
		appNameBegin++;

	size_t appNameEnd = esmolFile.find_last_of(".");
	if (appNameEnd < 0) appNameEnd = esmolFile.length();
	appName = esmolFile.substr(appNameBegin, appNameEnd - appNameBegin);

	const ESMoL::RootFolder& rf = UdmEngine::getRootFolder();

	MakeAll(rf);

	return 0;
}

void MakeAll(const ESMoL::RootFolder &rf)
{
	// Create Task directories
	int taskno = 0;
	std::vector<ESMoL::DesignFolder> dfs = rf.DesignFolder_children();
	for (std::vector<ESMoL::DesignFolder>::iterator dfi = dfs.begin(); dfi != dfs.end(); dfi++) {
		std::vector<ESMoL::Deployment> ds = (*dfi).Deployment_children();
		for (std::vector<ESMoL::Deployment>::iterator di = ds.begin(); di != ds.end(); di++) {
			std::vector<ESMoL::Task> ts = (*di).Task_children();
			for (std::vector<ESMoL::Task>::iterator ti = ts.begin(); ti != ts.end(); ti++) {
				google::TemplateDictionary *taskDict = dict.AddSectionDictionary("TASK");
				taskDict->SetIntValue("NUMBER", taskno++);
				taskDict->SetValue("NAME", (const std::string)ti->name());
				taskDict->SetValue("COMMENT", (const std::string)ti->Comment());
				taskDict->SetValue("TASKTYPE", (const std::string)ti->TaskType());
				taskDict->SetIntValue("PRIORITY", (int)ti->Priority());
				taskDict->SetIntValue("ACTIVATION", (int)ti->Activation());
				if (ti->AutoStart()) {
					taskDict->SetValue("AUTOSTART", "");
				}
				taskDict->SetValue("PREEMPTION", (const std::string)ti->Preemption());
				if (ti->Cyclic()) {
					taskDict->SetValue("CYCLIC", "");
					taskDict->SetIntValue("CYCLETIME", (int)ti->CycleTime());
				}

			}
		}
	}

	// Temp
	google::Template *tpl = google::Template::GetTemplate("robostix.tpl", google::DO_NOT_STRIP);
	string output;
	bool error_free = tpl->Expand(&output, &dict);
	std::cout << output;
}
