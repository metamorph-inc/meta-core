#include "IndexMerge.h"

ID_TYPE MakeCombinedID( ID_TYPE parentid, ID_TYPE childid ) {

	return (( (unsigned long long) parentid ) << 32 )
		+ (unsigned long long) childid;
}