#ifndef UDM_CONFIG_H
#define UDM_CONFIG_H

#include <string> // STL


#define _USE_CONSOLE


#include "CyberComposition.h"
 
#define META_NAMESPACE ::CyberComposition





struct _config
{
	// Global settings
	std::string metaPath;

};
extern _config config;

#endif /* UDM_CONFIG_H */
