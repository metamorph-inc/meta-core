//
// Misc type definitions
//
//

#ifndef _IBSATYPES_H_
#define _IBSATYPES_H_

#include <set>
#include <vector>
#include <map>
#include <list>

typedef unsigned long long idx_t;

typedef std::set< idx_t > IndexSet;
typedef std::vector< idx_t > IndexVector;
typedef std::pair< idx_t, idx_t > IndexPair;
typedef std::list< idx_t > IndexList;

#endif // _IBSATYPES_H_
