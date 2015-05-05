
#include "Disassembler.h"
#include <ProSolid.h>
#include <ProModelitem.h>
#include <iostream>
#include <ProFeatType.h>
#include <ProAsmcomp.h>
#include <ProUtil.h>

#include <CommonDefinitions.h>
#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>

namespace isis {
namespace creo {
namespace assembly {

struct Appdata {
	int level;
	ProAsmcomppath path;
};

ProError visit_action( ProFeature* in_feature, ProError in_status, ProAppData in_app_data ); 
ProError filter_action( ProFeature* in_feature, ProAppData in_app_data );


ProError visit_component_action( ProFeature* in_feature, ProError in_status, Appdata in_app_data ) {
	log4cpp::Category& log_f = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
	log4cpp::Category& log_cf = log4cpp::Category::getInstance(LOGCAT_CONSOLEANDLOGFILE);

	ProError rs;
	ProMdl model;
	switch( rs = ProAsmcompMdlGet(in_feature, &model) ) {
	case PRO_TK_NO_ERROR: break;
	case PRO_TK_BAD_INPUTS:
		log_cf.errorStream() << "The specified feature is not a valid component feature.";
		return PRO_TK_GENERAL_ERROR;
	case PRO_TK_E_NOT_FOUND:
		log_cf.errorStream() << "Assembly component model is not a solid or is not in memory.";
		return PRO_TK_GENERAL_ERROR;
	default:
		log_cf.errorStream() << "unexpected error: " << rs;
		return PRO_TK_GENERAL_ERROR;
	}
	ProSolid solid = static_cast<ProSolid>(model);

	ProMdlType component_type;

	switch( rs = ProMdlTypeGet(model, &component_type) ) {
	case PRO_TK_NO_ERROR: break;
	case PRO_TK_BAD_INPUTS:
		log_cf.errorStream() << "The input argument is invalid.";
		return PRO_TK_GENERAL_ERROR;
	case PRO_TK_GENERAL_ERROR:
		log_cf.errorStream() << "The information could not be obtained.";
		return PRO_TK_GENERAL_ERROR;
	default:
		log_cf.errorStream() << "unexpected error: " << rs;
		return PRO_TK_GENERAL_ERROR;
	}

	ProAsmcomp component = static_cast<ProAsmcomp>(*in_feature);

	Appdata app_data; 
	app_data.level = in_app_data.level+1;
	app_data.path.owner = static_cast<ProSolid>(component.owner);
	app_data.path.owner = in_app_data.path.owner;
	app_data.path.table_num = in_app_data.path.table_num + 1;
	for( int ix=0; ix < app_data.path.table_num; ++ix ) {
		app_data.path.comp_id_table[ix] = in_app_data.path.comp_id_table[ix];
	}
	app_data.path.comp_id_table[app_data.path.table_num] = in_feature->id;

    switch( component_type ) {
	case PRO_ASSEMBLY:
		{
		ProAppData* app_data_vp = reinterpret_cast<ProAppData*>(&app_data);
		switch( rs = ProSolidFeatVisit(solid, visit_action, filter_action, app_data_vp) ) {
		case PRO_TK_NO_ERROR: break;
		case PRO_TK_E_NOT_FOUND:
			log_cf.errorStream() << "filter produced no component features.";
			break;
		default:
			log_cf.errorStream() << "Any other value is the value returned "
				<< " by the action function (visiting stopped)." << rs;
		}
		}
		break;
	case PRO_PART:
		{
			ProMdl self;
			switch( rs = ProModelitemMdlGet(in_feature, &self) ) {
			case PRO_TK_NO_ERROR: break;
			case PRO_TK_BAD_INPUTS:
				log_cf.errorStream() << "feature has no model. " << in_feature->id;
			}
			char assembly_name[PRO_NAME_SIZE];
			{
				ProName wname;
				switch( rs = ProMdlNameGet(self, wname) ) {
				case PRO_TK_NO_ERROR: break;
				case PRO_TK_BAD_INPUTS:
					log_cf.errorStream() << "The input argument is invalid. " << in_feature->id;
					throw std::runtime_error("selector feature name failed");
				case PRO_TK_E_NOT_FOUND:
					log_cf.errorStream() << "The specified item does not have a name.  ";
					break;
				default:
					log_cf.errorStream() << "ill specified error:  " << rs;
					throw std::runtime_error("selector feature name failed");
				}
				ProWstringToString(assembly_name, wname);
			}
			char model_name[PRO_PATH_SIZE];
			{
				wchar_t* wname;
				ProBoolean is_modifiable;
				switch( rs = ProMdlCommonnameGet(self, &wname, &is_modifiable) ) {
				case PRO_TK_NO_ERROR: break;
				case PRO_TK_BAD_INPUTS:
					log_cf.errorStream() << "The input argument is invalid. " << in_feature->id;
					throw std::runtime_error("selector feature name failed");
				case PRO_TK_E_NOT_FOUND:
					log_cf.errorStream() << "The specified item does not have a name.  ";
					break;
				default:
					log_cf.errorStream() << "ill specified error:  " << rs;
					throw std::runtime_error("selector feature name failed");
				}
				ProWstringToString(model_name, wname);
				ProWstringFree(wname);
			}
			char item_name[PRO_NAME_SIZE];
			{
				ProName wname;
				switch( rs = ProModelitemNameGet(in_feature, wname) ) {
				case PRO_TK_NO_ERROR: break;
				case PRO_TK_BAD_INPUTS:
					log_cf.errorStream() << "The input argument is invalid. " << in_feature->id;
					throw std::runtime_error("selector feature name failed");
				case PRO_TK_E_NOT_FOUND:
					log_cf.errorStream() << "The specified item does not have a name.  ";
					break;
				default:
					log_cf.errorStream() << "ill specified error:  " << rs;
					throw std::runtime_error("selector feature name failed");
				}
				ProWstringToString(item_name, wname);
			}

			// If you request the transformation from the member to the
			// assembly, this is PRO_B_TRUE. Otherwise, this is PRO_B_FALSE.
			ProBoolean   bottom_up = PRO_B_TRUE;
			ProMatrix    transformation;
			// retrieve the transformation matrix, given the component path.
			switch( rs = ProAsmcomppathTrfGet(&app_data.path, bottom_up, transformation) ) {
			case PRO_TK_NO_ERROR: break;
			case PRO_TK_BAD_INPUTS:
				log_cf.errorStream() << "transform get, the input argument is invalid: " 
					<< app_data.path.comp_id_table;
				return PRO_TK_GENERAL_ERROR;
			case PRO_TK_GENERAL_ERROR:
				log_cf.errorStream() << "The specified component path was wrong, or contained missing members.";
				return PRO_TK_GENERAL_ERROR;
			default:
				log_cf.errorStream() << "unexpected error: " << rs;
				return PRO_TK_GENERAL_ERROR;
			}

			log_f.infoStream() << "transformation: " << assembly_name 
				<< " : " << model_name << " : " << item_name;
			log_f.infoStream() << "[";
			for( int ix=0; ix < 4; ++ix ) {
				log_f.infoStream() << "\n\t[";
				for( int jx=0; jx < 4; ++jx ) {
					log_f.infoStream() << '\t' << transformation[ix][jx];
				}
				log_f.infoStream() << "]";
			}
			log_f.infoStream() << "]\n";
			return PRO_TK_NO_ERROR;
		} 
	default:
		{
		log_cf.errorStream() << "unknown-component-type: " << component_type;
		}
	}
	return PRO_TK_NO_ERROR;
}

/* ProAssemblyDynPosGet(); */
ProError visit_datum_plane_action( ProFeature* in_feature, ProError in_status, Appdata in_app_data ) {
	log4cpp::Category& log_f = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
	log4cpp::Category& log_cf = log4cpp::Category::getInstance(LOGCAT_CONSOLEANDLOGFILE);

	ProError rs = PRO_TK_NO_ERROR;
	/*
	switch( rs = ProAssemblyDynPosGet() ) {
	}
	*/
	return PRO_TK_NO_ERROR;
}

/**
The visiting function. If it returns anything other than PRO_TK_NO_ERROR, visiting stops.
*/
ProError visit_action( ProFeature* in_feature, ProError in_status, ProAppData in_app_data_raw ) {
	log4cpp::Category& log_f = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
	log4cpp::Category& log_cf = log4cpp::Category::getInstance(LOGCAT_CONSOLEANDLOGFILE);

	if (in_feature == NULL) {
		log_cf.errorStream() << "NULL feature";
		return PRO_TK_CONTINUE;
	}
	Appdata* in_app_data = static_cast<Appdata*>(in_app_data_raw);
	ProError rs;

    ProFeattype feat_type;
	switch( rs = ProFeatureTypeGet(in_feature, &feat_type) ) {
	case PRO_TK_NO_ERROR: break;
	case PRO_TK_BAD_INPUTS:
		log_cf.errorStream() << "feature type get, the input argument is invalid.";
		return PRO_TK_GENERAL_ERROR;
	default:
		log_cf.errorStream() << "unexpected error: " << rs;
		return PRO_TK_GENERAL_ERROR;
	}
	switch( feat_type ) {
	case PRO_FEAT_COMPONENT: 
		log_f.infoStream() << "component : " << in_feature->id;
        return visit_component_action( in_feature, in_status, *in_app_data );

	case PRO_FEAT_DATUM:
		log_f.infoStream() << "datum-plane : " << in_feature->id;
        return visit_datum_plane_action( in_feature, in_status, *in_app_data );

	case PRO_FEAT_DATUM_AXIS:
		log_f.infoStream() << "datum-axis : " << in_feature->id;
        return(PRO_TK_NO_ERROR);
	case PRO_FEAT_DATUM_POINT:
		log_f.infoStream() << "datum-point : " << in_feature->id;
        return(PRO_TK_NO_ERROR);
	default:
		log_cf.errorStream() << "feature type : " << feat_type;
	}
	return PRO_TK_CONTINUE;
}

/**
The filter function. If NULL, all features in the specified 
solid are visited using the action function. 
*/
ProError filter_action( ProFeature* in_feature, ProAppData in_app_data ) {
	log4cpp::Category& log_f = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
	log4cpp::Category& log_cf = log4cpp::Category::getInstance(LOGCAT_CONSOLEANDLOGFILE);

   ProError rs;

   ProFeattype feat_type;
	switch( rs = ProFeatureTypeGet(in_feature, &feat_type) ) {
	case PRO_TK_NO_ERROR: break;
	case PRO_TK_BAD_INPUTS:
		log_cf.errorStream() << "the input argument is invalid.";
		return PRO_TK_GENERAL_ERROR;
	default:
		log_cf.errorStream() << "unexpected error: " << rs;
		return PRO_TK_GENERAL_ERROR;
	}
	switch( feat_type ) {
	case PRO_FEAT_COMPONENT: 
		log_f.infoStream() << "component : " << in_feature->id;
        return(PRO_TK_NO_ERROR);
	case PRO_FEAT_DATUM:
		log_f.infoStream() << "datum-plane : " << in_feature->id;
        return(PRO_TK_NO_ERROR);
	case PRO_FEAT_DATUM_AXIS:
		log_f.infoStream() << "datum-axis : " << in_feature->id;
        return(PRO_TK_NO_ERROR);
	case PRO_FEAT_DATUM_POINT:
		log_f.infoStream() << "datum-point : " << in_feature->id;
        return(PRO_TK_NO_ERROR);
	default:
		log_cf.errorStream() << "feature type : " << feat_type;
	}
    return(PRO_TK_CONTINUE);
}

ProError disassemble(ProSolid in_solid) {
	log4cpp::Category& log_f = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
	log4cpp::Category& log_cf = log4cpp::Category::getInstance(LOGCAT_CONSOLEANDLOGFILE);

	ProError rs;

    // The application data passed to the filter and visiting functions.
    Appdata app_data;
	app_data.level = 1;
	app_data.path.owner = in_solid;
	app_data.path.table_num = -1;
	app_data.path.comp_id_table[0] = 0;

    // visits all the features in the specified solid.
    // <B>NOTE:</B> Internal features are also visited. 
    // Use <B>ProFeatureVisibilityGet()</B> to filter/skip them if required.
    ProAppData* app_data_vp = reinterpret_cast<ProAppData*>(&app_data);
	switch( rs = ProSolidFeatVisit(in_solid, visit_action, filter_action, app_data_vp) ) {
	case PRO_TK_NO_ERROR: break;
	case PRO_TK_E_NOT_FOUND:
		log_cf.errorStream() << "Either no features exist, "
			<< " or they were all filtered out by the filter function.";
		break;
	default:
		log_cf.errorStream() << "Any other value is the value returned "
			<< " by the action function (visiting stopped)." << rs;
	}
	return PRO_TK_NO_ERROR;
}

} // assembly
} // creo
} // isis
