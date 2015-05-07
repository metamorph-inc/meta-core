#include "PathMgr.h"

#include "boost/filesystem.hpp"

using namespace boost::filesystem;

PathMgr::PathMgr( const std::string & lang, const std::string & ext, 
			      const std::string & tmpl_path )
{
	
	// create a directory with name ext + "time stamp"
	TimeStamp ts;
	_file_path = ext + std::string("_") + ts.GetDateTime( true );
	std::cout << "Writing output files to directory " << _file_path << std::endl;

	create_directory( path( _file_path ) );

	if ( lang == std::string( "c" ) )
	{
		_hdr_tmpl = std::string( "pn_ccode_h.tpl" );
		_code_tmpl = std::string( "pn_ccode_src.tpl" );
		_hdr_ext = std::string( ".h" );
		_code_ext = std::string( ".c" );
	}
	else if ( lang == std::string( "c++" ) )
	{
		_hdr_tmpl = std::string( "pn_cppcode_h.tpl" );
		_code_tmpl = std::string( "pn_cppcode_src.tpl" );
		_hdr_ext = std::string( ".hpp" );
		_code_ext = std::string( ".cpp" );
	} 

	_hdr_tmpl = tmpl_path + std::string("\\") + _hdr_tmpl;
	_code_tmpl = tmpl_path + std::string("\\") + _code_tmpl;
}