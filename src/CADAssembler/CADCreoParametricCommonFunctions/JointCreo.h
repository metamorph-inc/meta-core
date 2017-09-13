

#include <ProToolkit.h>
#include <ProFeature.h>
#include "cc_Joint.h"

namespace isis {
namespace cad {
namespace creo {

/**
Extract the information from a Creo feature,
datum carried by a feature to produce a joint.
*/
Joint convert_datum( ProFeature& in_datum );
Joint extract_marker( ProGeomitem& in_geom_item );
Joint extract_marker( ProSelection& in_selection );

/**
Extract the information from a set of features.
Calls convert_datum for each feature.
*/
std::vector<Joint> convert_datum_set( std::vector<ProFeature> in_datums );

/**
Extract the information from a single Creo selection
and produce a single joint.
/**
Given a set of Creo datum features, infer an aggregate joint.
*/
Joint infer_joint(std::vector<ProFeature> datum_set); 

} // creo
} // cad
} // isis
