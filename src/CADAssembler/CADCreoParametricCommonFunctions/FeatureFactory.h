

#ifndef CREATE_CREO_FEATURE_H
#define CREATE_CREO_FEATURE_H

#include <ProToolkit.h>
#include <ProFeature.h>
#include <ProReference.h>
#include <ProDtmPln.h>

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace isis { 
namespace creo { 
namespace feature { 
namespace create {

ProError Csys( ProFeature& out_feature, ProMdl in_model, ProSelection in_selection, 
	std::string in_name );

ProError Csys_default( ProFeature& out_feature, ProMdl in_model, ProSelection in_selection, 
	std::string in_name );

ProError Point( ProFeature& out_feature, ProMdl in_model, ProSelection in_selection, 
	std::string in_name,  ProFeature in_csys, Pro3dPnt in_coords );

ProError Plane( ProFeature& out_feature, ProMdl in_model, ProSelection in_selection, 
	std::string in_name, ProDtmplnConstrType in_type );

ProError Axis( ProFeature& out_feature, ProMdl in_model, ProSelection in_selection, 
	std::string in_name, ProFeature in_datum_plane_a, ProFeature in_datum_plane_b );

ProError Plane_Pivot( ProFeature& out_feature, ProMdl in_model, ProSelection in_selection, 
	std::string in_name, ProFeature in_datum_axis, ProFeature in_datum_plane, 
	double in_angle ) ;

ProError Plane_Offset( ProFeature& out_feature, ProMdl in_model, ProSelection in_selection, 
	std::string in_name, ProDtmplnFlipDir in_flip_dir, ProFeature in_datum_plane, double in_depth ) ;

ProError Plane_Offset_adjust( ProSelection in_selection, ProFeature in_datum_plane, double in_depth ) ;
ProError Plane_Angle_adjust( ProSelection in_selection, ProFeature in_datum_plane, double in_angle ) ;

ProError Solidify_Truncate( ProFeature& out_feature, ProMdl in_model, ProSelection in_selection, 
	std::string in_name, ProFeature in_datum_plane ) ;	

} 
} 
} 
}

#endif // CREATE_CREO_FEATURE_H