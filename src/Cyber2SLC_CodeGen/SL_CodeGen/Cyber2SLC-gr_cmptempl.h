#include "Cyber2SLC-gr_userinc.h"

template <class T>
bool StatementOrder( const T& lhs, const T& rhs) {
	__int64 l = lhs.statementIndex();
__int64 r = rhs.statementIndex();
return (l < r);
}

template <class BASE, class T>
bool StatementOrder_caster( const BASE& lhs, const BASE& rhs) {
	T lhs_casted= T::Cast( lhs);
	T rhs_casted= T::Cast( rhs);
	return StatementOrder( lhs_casted, rhs_casted);
}

template <class T>
bool PortOrder( const T& lhs, const T& rhs) {
	__int64 l = lhs.Number();
__int64 r = rhs.Number();
return l < r;

}

template <class BASE, class T>
bool PortOrder_caster( const BASE& lhs, const BASE& rhs) {
	T lhs_casted= T::Cast( lhs);
	T rhs_casted= T::Cast( rhs);
	return PortOrder( lhs_casted, rhs_casted);
}

template <class T>
bool TopologicalSort( const T& lhs, const T& rhs) {
	__int64 lp = lhs.Priority();
__int64 rp = rhs.Priority();
return lp < rp;
}

template <class BASE, class T>
bool TopologicalSort_caster( const BASE& lhs, const BASE& rhs) {
	T lhs_casted= T::Cast( lhs);
	T rhs_casted= T::Cast( rhs);
	return TopologicalSort( lhs_casted, rhs_casted);
}

template <class T>
bool UniqueIdSort( const T& lhs, const T& rhs) {
	__int64 l = lhs.uniqueId();
__int64 r = rhs.uniqueId();
return (l < r);
}

template <class BASE, class T>
bool UniqueIdSort_caster( const BASE& lhs, const BASE& rhs) {
	T lhs_casted= T::Cast( lhs);
	T rhs_casted= T::Cast( rhs);
	return UniqueIdSort( lhs_casted, rhs_casted);
}

template <class T>
bool PRCompare( const T& lhs, const T& rhs) {
	std::string lhsString = lhs.name();
std::string rhsString = rhs.name();
return (lhsString < rhsString);
}

template <class BASE, class T>
bool PRCompare_caster( const BASE& lhs, const BASE& rhs) {
	T lhs_casted= T::Cast( lhs);
	T rhs_casted= T::Cast( rhs);
	return PRCompare( lhs_casted, rhs_casted);
}

