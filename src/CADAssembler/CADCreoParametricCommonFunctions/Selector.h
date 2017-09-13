

#ifndef CREATE_CREO_SELECTOR_H
#define CREATE_CREO_SELECTOR_H

#include <ProToolkit.h>
#include <ProFeature.h>
#include <ProReference.h>
#include <ProDtmPln.h>

#include "cc_LoggerBoost.h"

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace isis {
namespace creo {
namespace feature {

class Selector {
private:

	ProSelection m_self;
public:

	Selector( ProSelection in_selection );
	Selector( ProMdl in_model, ProFeature in_feature, ProType in_geom_type = PRO_TYPE_UNUSED );
	~Selector();

	ProSelection& operator*();
	ProSelection  operator&();
	ProSelection* operator->();

	ProValueData getValueData();
};


}
}
}

#endif // CREATE_CREO_SELECTOR_H