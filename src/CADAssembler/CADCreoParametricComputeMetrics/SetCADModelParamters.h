#ifndef SET_CAD_MODEL_PARAMTERS_H
#define SET_CAD_MODEL_PARAMTERS_H

#include <isis_application_exception.h>
#include "ComputeMetricsXML.hxx"

namespace isis
{

	void SetCADModelParamters(std::auto_ptr<CADComponentsType>  &in_CADComponents_ptr)
																		throw (isis::application_exception);

} // end namespace isis


#endif // end SET_CAD_MODEL_PARAMTERS_H