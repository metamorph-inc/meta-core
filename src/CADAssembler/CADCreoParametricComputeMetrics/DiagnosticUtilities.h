#ifndef DIAGNOSTIC_UTILITIES_H
#define DIAGNOSTIC_UTILITIES_H

//#include <isis_ptc_toolkit_functions.h>
//#include <isis_application_exception.h>
//#include <isis_include_ptc_headers.h>
//#include <list>
#include <iostream>
#include "ComputeMetricsXML.hxx"

namespace isis
{


void stream_CADComponentsTree( std::auto_ptr<CADComponentsType>  &in_CADComponents_ptr,
								std::ostream &out_Stream);


} // END namespace isis

#endif // DIAGNOSTIDIAGNOSTIC_UTILITIES_HC_UTILITIES