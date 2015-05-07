#ifndef DESMAP_H
#define DESMAP_H
#include <afxtempl.h>
#include "uml.h"
#include "UmlExt.h"
#include "DesertIface.h"
#include "DesertIfaceBack.h"

#include "BackIface.h"
#include "desertdll.h"
using namespace DesertIface;

typedef map<DesertBase, long > UdmDesertMap;
typedef const pair<DesertBase const, long> UdmDesertMapItem;

//we need the inverse map, too
//Desert functions are supposed to generate unique IDs

typedef map<long, DesertBase> DesertUdmMap;
typedef const pair<long const, DesertBase > DesertUdmMapItem;



typedef set<DesertIface::Element> UdmElementSet;
typedef set<DesertIface::Member> UdmMemberSet;

void DoMap(DesertBase &db, UdmDesertMap & _map, DesertUdmMap &inv_des_map, long id);
long GetID(DesertBase &db, UdmDesertMap & _map);
DesertBase &GetObject(long id, DesertUdmMap & _map);

bool CreateDesertSpace(Space &sp, DesertIface::Element &e, UdmDesertMap &des_map, DesertUdmMap &inv_des_map, UdmElementSet &elements, bool root);

bool CreateCustomDomain(
		CustomDomain &cd, 
		DesertIface::CustomMember &mb, 
		UdmDesertMap &des_map, 
		DesertUdmMap &inv_des_map,
		UdmMemberSet &mb_set,
		bool root);

bool CreateDesertConstraintSet(ConstraintSet &cs, UdmDesertMap &des_map, DesertUdmMap &inv_des_map );
bool CreateElementRelations(DesertSystem &ds, UdmDesertMap &des_map, DesertUdmMap &inv_des_map);
bool CreateMemberRelations(DesertSystem &ds, UdmDesertMap &des_map, DesertUdmMap &inv_des_map);

bool CreateConstraints(DesertSystem &ds, UdmDesertMap &des_map, DesertUdmMap &inv_des_map);
bool CreateCustomDomains(DesertSystem &ds, UdmDesertMap &des_map, DesertUdmMap &inv_des_map, UdmMemberSet &mb_set );

bool CreateNaturalDomains(DesertSystem &ds, UdmDesertMap& des_map, DesertUdmMap &inv_des_map);
bool CreateVariableProperties(UdmDesertMap &des_map, DesertUdmMap &inv_des_map, UdmElementSet& elements);
bool CreateConstantProperties(UdmDesertMap &des_map, DesertUdmMap &inv_des_map, UdmElementSet& elements, UdmMemberSet &c_members);
bool CreateAssignments(UdmDesertMap& des_map, DesertUdmMap &inv_des_map, UdmElementSet& elements, UdmMemberSet &c_members);
bool CreateSimpleFormulas(DesertSystem &ds, UdmDesertMap& des_map, DesertUdmMap &inv_des_map);




#endif //DESMAP_H