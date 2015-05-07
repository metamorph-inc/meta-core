#include "ConfigKeeper.h"

ConfigKeeper::ConfigKeeper(void)
{
}

ConfigKeeper::~ConfigKeeper(void)
{
}

// It was nice to have these inline, but it doesn't work with the
// forward declarations :-(
NodeConfig * AppConfig::NewNode() 
{ 
	return new NodeConfig( this->NewSection("HOST_SECTION") );  
}

TaskConfig * NodeConfig::NewTask() 
{ 
	return new TaskConfig( this->NewSection("TASK_SECTION") );
}

CompConfig * TaskConfig::NewComp() 
{ 
	return new CompConfig( this->NewSection("COMP_SECTION") );
}

MsgConfig * CompConfig::NewMsg() 
{ 
	return new MsgConfig( this->NewSection("MSG_SECTION") );
}

