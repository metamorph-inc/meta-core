#include "Cyber2SFC-gr_userinc.h"

template <class T>
bool TransitionOrder( const T& lhs, const T& rhs) {
	string l = lhs.Order();
	string r = rhs.Order();
	return atoi(l.c_str()) < atoi(r.c_str());
}

template <class BASE, class T>
bool TransitionOrder_caster( const BASE& lhs, const BASE& rhs) {
	T lhs_casted= T::Cast( lhs);
	T rhs_casted= T::Cast( rhs);
	return TransitionOrder( lhs_casted, rhs_casted);
}

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
bool RevTransitionOrder( const T& lhs, const T& rhs) {
	string l = lhs.Order();
	string r = rhs.Order();
	return atoi(l.c_str()) > atoi(r.c_str());
}

template <class BASE, class T>
bool RevTransitionOrder_caster( const BASE& lhs, const BASE& rhs) {
	T lhs_casted= T::Cast( lhs);
	T rhs_casted= T::Cast( rhs);
	return RevTransitionOrder( lhs_casted, rhs_casted);
}

template <class T>
bool DEOrder( const T& lhs, const T& rhs) {
	return static_cast< __int64 >( lhs.Port() ) < static_cast< __int64 >( rhs.Port() );
}

template <class BASE, class T>
bool DEOrder_caster( const BASE& lhs, const BASE& rhs) {
	T lhs_casted= T::Cast( lhs);
	T rhs_casted= T::Cast( rhs);
	return DEOrder( lhs_casted, rhs_casted);
}

template <class T>
bool StateOrder( const T& lhs, const T& rhs) {
	string l = lhs.Order();
	string r = rhs.Order();
	return atoi(l.c_str()) < atoi(r.c_str());

}

template <class BASE, class T>
bool StateOrder_caster( const BASE& lhs, const BASE& rhs) {
	T lhs_casted= T::Cast( lhs);
	T rhs_casted= T::Cast( rhs);
	return StateOrder( lhs_casted, rhs_casted);
}

template <class T>
bool StateRevOrder( const T& lhs, const T& rhs) {
	string l = lhs.Order();
	string r = rhs.Order();
	return atoi(l.c_str()) > atoi(r.c_str());

}

template <class BASE, class T>
bool StateRevOrder_caster( const BASE& lhs, const BASE& rhs) {
	T lhs_casted= T::Cast( lhs);
	T rhs_casted= T::Cast( rhs);
	return StateRevOrder( lhs_casted, rhs_casted);
}

