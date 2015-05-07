#include "TTPGenerator.h"

#include "GenBuilder.h"

#define BUFF_SIZE	4096

using namespace std;
using namespace EsMoL;

TTPGenerator::TTPGenerator(void)
{

}

TTPGenerator::~TTPGenerator(void)
{

}

void TTPGenerator::AddAppNode( Node & n )
{
	_nodenames.push_back( (const std::string & ) n.name() );
}

void TTPGenerator::AddTaskComponent( Task & t, Component & c )
{

}

void TTPGenerator::AddComponentPort( Component & c, IOPort & p )
{

}

void TTPGenerator::AddTaskMessage( Task & t, Message & m )
{

}

void TTPGenerator::AddBusMsg( Bus & b, Message & m )
{

}

// Build the diretories, command files, 
// and everything else for this platform
void TTPGenerator::Generate( ConfigKeeper * ck )
{
	bool ret = false;
	
	std::list<string > ttp_cmds;

	ret = MakeBuildScripts(appName);
	string fn = releaseDir + string("ttp_plan.py");
	sprintf_s(buff, sizeof(buff), TTP_PLAN_PATH, releaseDir.c_str(), fn.c_str(), appName.c_str());
	ttp_cmds.push_back(buff);

	google::Template * tpl = google::Template::GetTemplate("TTPTemplates\\ttp_plan.tpl", google::DO_NOT_STRIP);
	ofstream ttp_plan;
	ttp_plan.open(fn.c_str());
	std::string output;
	tpl->Expand(&output, ck->GetConfig() );
	ttp_plan << output;
	cout << endl << "Invoking the TTP toolchain ..." << endl;
	
	std::list<string>::iterator it;
	for (it = ttp_cmds.begin(); it != ttp_cmds.end(); it++) {
		if (system((*it).c_str())) {
			ret = false;
			break;
		}
	}

	// Iterate over all of the nodes and do the following:
	for ( list< Node >::iterator pNode = _nodenames.begin(); pNode != _nodenames.end(); pNode++ )
	{
		bool ret = true;

		char buff[BUFF_SIZE];

		std::string node_nm((*pNode).name());
		
		string nodeDir = _releaseDir + "\\" + node_nm + "\\";
		int r = _mkdir(nodeDir.c_str());
		if (r  && errno != EEXIST) {
			perror("Unable to create node directory in release dir");
			return false;
		}

		string nodeBuildDir = _buildDir + "\\" + node_nm + "\\";
		r = _mkdir(nodeBuildDir.c_str());
		if (r  && errno != EEXIST) {
			perror("Unable to create node directory in build dir");
			return false;
		}

		AddBuildLine(node_nm, (*pNode).CPU());

		string fn = nodeDir + string("ttp_build_") + node_nm +string(".py");
		sprintf_s(buff, sizeof(buff), TTP_BUILD_PATH, nodeDir.c_str(), fn.c_str());
		ttp_cmds.push_back(buff);


	}
}

bool TTPGenerator::AddBuildLine(const string& hname, const string& hno)
{
	char buff[BUFF_SIZE];
	ofstream batch;
	string fn;

	fn = buildDir + string(FNAME_MAKE_ALL);
	batch.open(fn.c_str(), ios::app);
	sprintf_s(buff, sizeof(buff), MAKE_ALL_ADDHOST, hname.c_str(), hno.c_str());
	batch << buff;
	batch.close();

	return true;
}

bool TTPGenerator::MakeBuildScripts(const std::string & appName)
{
	char buff[BUFF_SIZE];
	ofstream batch;
	string fn;

	fn = buildDir + string(FNAME_MAKE_ALL);
	batch.open(fn.c_str());
	sprintf_s(buff, sizeof(buff), BATCH_MAKE_ALL);
	batch << buff;
	batch.close();

	fn = buildDir + string(FNAME_MAKE);
	batch.open(fn.c_str());
	sprintf_s(buff, sizeof(buff), BATCH_MAKE);
	batch << buff;
	batch.close();

	fn = buildDir + string(FNAME_PRJ_SETUP);
	batch.open(fn.c_str());
	sprintf_s(buff, sizeof(buff), BATCH_PRJ_SETUP);
	batch << buff;
	batch.close();

	fn = buildDir + string(FNAME_COPY_NODE_TO_DDB);
	batch.open(fn.c_str());
	sprintf_s(buff, sizeof(buff), BATCH_COPY_NODE_TO_DDB, appName.c_str());
	batch << buff;
	batch.close();

	return true;
}

