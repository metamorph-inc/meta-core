#ifndef _PATH_MGR_H_
#define _PATH_MGR_H_

#include "utils.h"

// The path manager does the following:

// It is created with the language option string and an extension string

// It provides the names of the template files
// It provides filenames for writing output files (including path) based on
//  a provided module name string

class PathMgr 
{
public:
	PathMgr( const std::string & lang, const std::string & ext, 
			 const std::string & tmpl_path );

	virtual ~PathMgr() { }

	std::string GetHeaderTemplate() { return _hdr_tmpl; }
	std::string GetCodeTemplate() { return _code_tmpl; }

	std::string GetHeaderFilename( const std::string & modname ) 
	{ return _file_path + std::string("\\") + modname + _hdr_ext; }

	std::string GetCodeFilename( const std::string & modname ) 
	{ return _file_path + std::string("\\") + modname + _code_ext; }


private:
	std::string _hdr_tmpl;
	std::string _code_tmpl;
	std::string _file_path;
	std::string _hdr_ext;
	std::string _code_ext;

};



#endif // _PATH_MGR_H_