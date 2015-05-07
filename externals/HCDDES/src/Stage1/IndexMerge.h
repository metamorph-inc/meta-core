#ifndef _INDEX_MERGE_H_
#define _INDEX_MERGE_H_

// This needs to stay compatible with the uniqueID in UDM
// but avoid collisions with UDM IDs, so we use a bigger
// type to represent the IDs
typedef unsigned long long ID_TYPE;

// message id utility function
ID_TYPE MakeCombinedID( ID_TYPE parentid, ID_TYPE childid );

#endif // _INDEX_MERGE_H_