
#include "Disassembler.h"
#include <ProSolid.h>
#include <ProModelitem.h>
#include <iostream>
#include <ProFeatType.h>
#include <ProAsmcomp.h>
#include <ProUtil.h>

#include "cc_LoggerBoost.h"

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
	
	

	ProError rs;
	ProMdl model;
	switch( rs = ProAsmcompMdlGet(in_feature, &model) ) {
	case PRO_TK_NO_ERROR: break;
	case PRO_TK_BAD_INPUTS:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "The specified feature is not a valid component feature.";
		return PRO_TK_GENERAL_ERROR;
	case PRO_TK_E_NOT_FOUND:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "Assembly component model is not a solid or is not in memory.";
		return PRO_TK_GENERAL_ERROR;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "unexpected error: " << rs;
		return PRO_TK_GENERAL_ERROR;
	}
	ProSolid solid = static_cast<ProSolid>(model);

	ProMdlType component_type;

	switch( rs = ProMdlTypeGet(model, &component_type) ) {
	case PRO_TK_NO_ERROR: break;
	case PRO_TK_BAD_INPUTS:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "The input argument is invalid.";
		return PRO_TK_GENERAL_ERROR;
	case PRO_TK_GENERAL_ERROR:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "The information could not be obtained.";
		return PRO_TK_GENERAL_ERROR;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "unexpected error: " << rs;
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
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "filter produced no component features.";
			break;
		default:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "Any other value is the value returned "
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
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "feature has no model. " << in_feature->id;
			}
			char assembly_name[PRO_NAME_SIZE];
			{
				ProName wname;
				switch( rs = ProMdlNameGet(self, wname) ) {
				case PRO_TK_NO_ERROR: break;
				case PRO_TK_BAD_INPUTS:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "The input argument is invalid. " << in_feature->id;
					throw std::runtime_error("selector feature name failed");
				case PRO_TK_E_NOT_FOUND:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "The specified item does not have a name.  ";
					break;
				default:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "ill specified error:  " << rs;
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
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "The input argument is invalid. " << in_feature->id;
					throw std::runtime_error("selector feature name failed");
				case PRO_TK_E_NOT_FOUND:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "The specified item does not have a name.  ";
					break;
				default:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "ill specified error:  " << rs;
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
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "The input argument is invalid. " << in_feature->id;
					throw std::runtime_error("selector feature name failed");
				case PRO_TK_E_NOT_FOUND:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "The specified item does not have a name.  ";
					break;
				default:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "ill specified error:  " << rs;
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
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "transform get, the input argument is invalid: " 
					<< app_data.path.comp_id_table;
				return PRO_TK_GENERAL_ERROR;
			case PRO_TK_GENERAL_ERROR:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "The specified component path was wrong, or contained missing members.";
				return PRO_TK_GENERAL_ERROR;
			default:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "unexpected error: " << rs;
				return PRO_TK_GENERAL_ERROR;
			}

			isis_LOG(lg, isis_FILE, isis_INFO) << "transformation: " << assembly_name 
				<< " : " << model_name << " : " << item_name;
			isis_LOG(lg, isis_FILE, isis_INFO) << "[";
			for( int ix=0; ix < 4; ++ix ) {
				isis_LOG(lg, isis_FILE, isis_INFO) << "\n\t[";
				for( int jx=0; jx < 4; ++jx ) {
					isis_LOG(lg, isis_FILE, isis_INFO) << '\t' << transformation[ix][jx];
				}
				isis_LOG(lg, isis_FILE, isis_INFO) << "]";
			}
			isis_LOG(lg, isis_FILE, isis_INFO) << "]\n";
			return PRO_TK_NO_ERROR;
		} 
	default:
		{
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "unknown-component-type: " << component_type;
		}
	}
	return PRO_TK_NO_ERROR;
}

/* ProAssemblyDynPosGet(); */
ProError visit_datum_plane_action( ProFeature* in_feature, ProError in_status, Appdata in_app_data ) {
	
	

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
	
	

	if (in_feature == NULL) {
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "NULL feature";
		return PRO_TK_CONTINUE;
	}
	Appdata* in_app_data = static_cast<Appdata*>(in_app_data_raw);
	ProError rs;

    ProFeattype feat_type;
	switch( rs = ProFeatureTypeGet(in_feature, &feat_type) ) {
	case PRO_TK_NO_ERROR: break;
	case PRO_TK_BAD_INPUTS:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "feature type get, the input argument is invalid.";
		return PRO_TK_GENERAL_ERROR;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "unexpected error: " << rs;
		return PRO_TK_GENERAL_ERROR;
	}
	switch( feat_type ) {
	case PRO_FEAT_COMPONENT: 
		isis_LOG(lg, isis_FILE, isis_INFO) << "component : " << in_feature->id;
        return visit_component_action( in_feature, in_status, *in_app_data );

	case PRO_FEAT_DATUM:
		isis_LOG(lg, isis_FILE, isis_INFO) << "datum-plane : " << in_feature->id;
        return visit_datum_plane_action( in_feature, in_status, *in_app_data );

	case PRO_FEAT_DATUM_AXIS:
		isis_LOG(lg, isis_FILE, isis_INFO) << "datum-axis : " << in_feature->id;
        return(PRO_TK_NO_ERROR);
	case PRO_FEAT_DATUM_POINT:
		isis_LOG(lg, isis_FILE, isis_INFO) << "datum-point : " << in_feature->id;
        return(PRO_TK_NO_ERROR);
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "feature type : " << feat_type;
	}
	return PRO_TK_CONTINUE;
}

/**
The filter function. If NULL, all features in the specified 
solid are visited using the action function. 
*/
ProError filter_action( ProFeature* in_feature, ProAppData in_app_data ) {
	
	

   ProError rs;

   ProFeattype feat_type;
	switch( rs = ProFeatureTypeGet(in_feature, &feat_type) ) {
	case PRO_TK_NO_ERROR: break;
	case PRO_TK_BAD_INPUTS:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "the input argument is invalid.";
		return PRO_TK_GENERAL_ERROR;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "unexpected error: " << rs;
		return PRO_TK_GENERAL_ERROR;
	}
	switch( feat_type ) {
	case PRO_FEAT_COMPONENT: 
		isis_LOG(lg, isis_FILE, isis_INFO) << "component : " << in_feature->id;
        return(PRO_TK_NO_ERROR);
	case PRO_FEAT_DATUM:
		isis_LOG(lg, isis_FILE, isis_INFO) << "datum-plane : " << in_feature->id;
        return(PRO_TK_NO_ERROR);
	case PRO_FEAT_DATUM_AXIS:
		isis_LOG(lg, isis_FILE, isis_INFO) << "datum-axis : " << in_feature->id;
        return(PRO_TK_NO_ERROR);
	case PRO_FEAT_DATUM_POINT:
		isis_LOG(lg, isis_FILE, isis_INFO) << "datum-point : " << in_feature->id;
        return(PRO_TK_NO_ERROR);
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "feature type : " << feat_type;
	}
    return(PRO_TK_CONTINUE);
}

ProError disassemble(ProSolid in_solid) {
	
	

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
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "Either no features exist, "
			<< " or they were all filtered out by the filter function.";
		break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "Any other value is the value returned "
			<< " by the action function (visiting stopped)." << rs;
	}
	return PRO_TK_NO_ERROR;
}

} // assembly
} // creo
} // isis
