
#include "Selector.h"
#include <stdexcpt.h>

#include <ProElemId.h>
#include <ProFeatType.h>
#include <ProFeatForm.h>
#include <ProExtrude.h>
#include <ProValue.h>
#include <ProSurface.h>
#include <ProQuilt.h>
#include <ProDtmPln.h>
#include <ProDtmAxis.h>
#include <ProSolidify.h>
#include <ProSolid.h>
#include <ProDtmCsys.h>
#include <ProUtil.h>
#include <UtilTree.h>
#include <ProFeature.h>
#include <UtilPtc.h>
#include <ProModelitem.h>

#define PRO_VALUE_TYPE_NIL static_cast<ProValueDataType>(-1)

namespace isis {
namespace creo {
namespace feature {


ProError select_geom_action (ProGeomitem* geom_handle, ProError status, ProAppData  app_data) 
{
	if (status != PRO_TK_NO_ERROR) return status;
	ProModelitem* item = reinterpret_cast<ProModelitem*>(app_data);
	*item = *geom_handle; 
	return PRO_TK_NO_ERROR;
}

			
Selector::Selector( ProSelection selection ) 
	: m_log_f(log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY)),
	  m_log_cf(log4cpp::Category::getInstance(LOGCAT_CONSOLEANDLOGFILE)),
	  m_self(selection)  { }

/**
For different feature types different geometries are appropriate:
DTM_PLN : PRO_TYPE_SURFACE
DTM_AXIS : PRO_TYPE_AXIS

Rather than ProModelitemByNameInit() maybe one of the following would be a better choice:
ProGeomitemFeatureGet();
ProGeomitemToSurface();
ProSurfaceDataGet();
ProFeatureSelectionGet();
ProFeatureGeomitemVisit();  // *I think this is the correct approach*
ProSurfaceDataGet();
*/
Selector::Selector( ProMdl in_model, ProFeature in_feature, ProType in_geom_type )
	: m_log_f(log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY)),
	  m_log_cf(log4cpp::Category::getInstance(LOGCAT_CONSOLEANDLOGFILE))
{
	ProError status;
	ProModelitem model_item;
	if (in_geom_type == PRO_TYPE_UNUSED) {
		model_item = static_cast<ProModelitem>(in_feature);
	} else {
		ProAppData select_geom_data = static_cast<ProAppData>(&model_item);
		switch( status = ProFeatureGeomitemVisit(&in_feature, in_geom_type, 
			select_geom_action, NULL, select_geom_data) ) 
		{
		case PRO_TK_NO_ERROR: break;
		case PRO_TK_BAD_INPUTS:
			m_log_cf.warnStream() << "one or more arguments was invalid.";
			break;
		default:
			m_log_cf.warnStream() 
				<< "the action function returned a value other than PRO_TK_NO_ERROR and visiting stopped. " 
				<< status;
		}
	}
	m_log_cf.infoStream() << "model selected: "
		<< " id = " << model_item.id << " type = " << model_item.type;

    switch( status = ProSelectionAlloc(NULL, &model_item, &m_self) ) {
	case PRO_TK_NO_ERROR: break;
    case PRO_TK_BAD_INPUTS:
		m_log_cf.errorStream() << "the allocation failed due to bad inputs.";
		m_log_cf.errorStream() << " component-path: NULL " 
		          << " model-item: " << model_item.id;
		throw std::runtime_error("selector allocation failed");
	default: 
		m_log_cf.errorStream() << "could not allocate selection: " << status;
		throw std::runtime_error("selector allocation failed");
	}
}
Selector::~Selector() {
	switch( ProSelectionFree( &m_self ) ) {
	case PRO_TK_NO_ERROR: break;
	}
}
ProSelection& Selector::operator*() {
	return m_self;
}
ProSelection Selector::operator&() {
	return m_self;
}
ProSelection* Selector::operator->() {
	return &m_self;
}
ProValueData Selector::getValueData() {
	ProValueData data;
	data.type = PRO_VALUE_TYPE_SELECTION;
	data.v.r = m_self;
	return data;
}

			
} // feature
} // creo
} // isis