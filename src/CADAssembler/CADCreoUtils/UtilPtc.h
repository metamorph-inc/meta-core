
#ifndef UTILITIES_PTC_H
#define UTILITIES_PTC_H

#include <ProToolkit.h>
#include <ProElement.h>
#include <ProItemerr.h>


void ProUtilFeatErrsWrite(
	char *function,
	ProError err,
	ProElement tree,
	ProErrorlist *err_list);


ProError write_selected_feature( void );
ProError write_all_feature( const std::string protocol_payload );

ProError write_selected_feature_as_xml( char* in_output_file );

ProError disassemble();

#endif // UTILITIES_PTC_H