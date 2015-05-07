#ifndef EDPCG_H
#define EDPCG_H

#include "ESMoL.h"
#include <vector>

typedef struct _ltStr {
	bool operator()(const string& a, const string& b) const {
		return a.compare(b) < 0;
	}
} LtStr;

#define BEGIN_ITERATE_CCOLL(Cl, coll) \
  for(vector<Cl>::const_iterator Cl##_i=coll##.begin(); Cl##_i!=coll##.end(); Cl##_i++) \
  { \
    const Cl& Cl##_el = *Cl##_i;

#define END_ITERATE_CCOLL() \
  }

#define BEGIN_ITERATE_C(Cl,obj) \
{ \
  const vector<Cl>& Cl##_s = obj.Cl##_kind_children(); \
  BEGIN_ITERATE_CCOLL(Cl, Cl##_s)

#define END_ITERATE_C() \
  END_ITERATE_CCOLL() \
}



#define BEGIN_ITERATE_CSET(Cl, coll) \
  for(set<Cl>::const_iterator Cl##_i=coll##.begin(); Cl##_i!=coll##.end(); Cl##_i++) \
  { \
    const Cl& Cl##_el = *Cl##_i;

#define END_ITERATE_CSET() \
  }


#endif
