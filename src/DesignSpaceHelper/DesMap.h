#ifndef DESMAP_H
#define DESMAP_H
#include <afxtempl.h>
#include "uml.h"
#include "UmlExt.h"
#include "DesertIface.h"
#include "DesertIfaceBack.h"

#include "BackIface.h"
#include "desertdll.h"

#include <functional>

typedef map<DesertIface::DesertBase, long > UdmDesertMap;
typedef const pair<DesertIface::DesertBase const, long> UdmDesertMapItem;

//we need the inverse map, too
//Desert functions are supposed to generate unique IDs

typedef map<long, DesertIface::DesertBase> DesertUdmMap;
typedef const pair<long const, DesertIface::DesertBase > DesertUdmMapItem;



typedef set<DesertIface::Element> UdmElementSet;
typedef set<DesertIface::Member> UdmMemberSet;

void DoMap(DesertIface::DesertBase &db, UdmDesertMap & _map, DesertUdmMap &inv_des_map, long id);
long GetID(DesertIface::DesertBase &db, UdmDesertMap & _map);
DesertIface::DesertBase &GetObject(long id, DesertUdmMap & _map);

bool CreateDesertSpace(DesertIface::Space &sp, DesertIface::Element &e, UdmDesertMap &des_map, DesertUdmMap &inv_des_map, UdmElementSet &elements, bool root, std::function<void(short)> StepInState);

bool CreateCustomDomain(
		DesertIface::CustomDomain &cd,
		DesertIface::CustomMember &mb, 
		UdmDesertMap &des_map, 
		DesertUdmMap &inv_des_map,
		UdmMemberSet &mb_set,
		bool root, std::function<void(short)> StepInState);

bool CreateDesertConstraintSet(DesertIface::ConstraintSet &cs, UdmDesertMap &des_map, DesertUdmMap &inv_des_map);
bool CreateElementRelations(DesertIface::DesertSystem &ds, UdmDesertMap &des_map, DesertUdmMap &inv_des_map, std::function<void(short)> StepInState);
bool CreateMemberRelations(DesertIface::DesertSystem &ds, UdmDesertMap &des_map, DesertUdmMap &inv_des_map, std::function<void(short)> StepInState);

bool CreateConstraints(DesertIface::DesertSystem &ds, UdmDesertMap &des_map, DesertUdmMap &inv_des_map);
bool CreateCustomDomains(DesertIface::DesertSystem &ds, UdmDesertMap &des_map, DesertUdmMap &inv_des_map, UdmMemberSet &mb_set, std::function<void(short)> StepInState);

bool CreateNaturalDomains(DesertIface::DesertSystem &ds, UdmDesertMap& des_map, DesertUdmMap &inv_des_map, std::function<void(short)> StepInState);
template<typename Functor>
bool CreateVariableProperties(UdmDesertMap& des_map, DesertUdmMap &inv_des_map, UdmElementSet& elements, Functor& f) {
	return CreateVariableProperties(des_map, inv_des_map, elements, static_cast<std::function<void(short)> >(f));
}
bool CreateVariableProperties(UdmDesertMap& des_map, DesertUdmMap &inv_des_map, UdmElementSet& elements, std::function<void(short)> StepInState);
bool CreateConstantProperties(UdmDesertMap &des_map, DesertUdmMap &inv_des_map, UdmElementSet& elements, UdmMemberSet &c_members, std::function<void(short)> StepInState);
bool CreateAssignments(UdmDesertMap& des_map, DesertUdmMap &inv_des_map, UdmElementSet& elements, UdmMemberSet &c_members, std::function<void(short)> StepInState);
bool CreateSimpleFormulas(DesertIface::DesertSystem &ds, UdmDesertMap& des_map, DesertUdmMap &inv_des_map);

bool CreateCustomFormulas(DesertIface::DesertSystem &ds, UdmDesertMap &des_map, DesertUdmMap &inv_des_map);



#endif //DESMAP_H