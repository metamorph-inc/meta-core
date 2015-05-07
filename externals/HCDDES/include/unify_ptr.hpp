#ifndef _ISIS_UNIFY_PTR
#define _ISIS_UNIFY_PTR

#include <boost/shared_ptr.hpp>

template< typename T >
class unify_base;

template< typename T >
class unify_ptr;

template< typename T >
class unify_object;

template< typename T > 
class unify_base {

public:
	typedef boost::shared_ptr< unify_base<T> > UnifySP;

	virtual T &getObject( void ) = 0;

	virtual void setUnifySP( const UnifySP &unifySP ) = 0;
	virtual UnifySP getUnifySP( void ) = 0;
	virtual unify_ptr<T> getPrimary( unify_ptr<T> &candidate ) = 0;
};


template< typename T >
class unify_ptr : public unify_base< T > {
	typedef boost::shared_ptr< unify_base<T> > UnifySP;

private:
	UnifySP _unifySP;

	void normalize( void ) {
		getUnifySP()->getUnifySP()->getPrimary( *this );
	}

	unify_ptr( unify_object<T> *uo ) : _unifySP( uo ) { }

public:
	unify_ptr( void ) : _unifySP(   new unify_ptr<T>(  new unify_object<T>()  )   ) { }
	unify_ptr( const T &object ) : _unifySP(   new unify_ptr<T>(  new unify_object<T>( object )  )   ) { }

	void unify( unify_ptr<T> &rhs ) {
		normalize();
		rhs.normalize();
		if ( getUnifySP().get() == rhs.getUnifySP().get() ) return;

		rhs.getUnifySP()->setUnifySP( getUnifySP() );
		rhs.setUnifySP( getUnifySP() );
	}

	virtual T &getObject( void ) {
		normalize();
		return getUnifySP()->getUnifySP()->getObject();
	}

	virtual void setUnifySP( const UnifySP &unifySP ) { _unifySP = unifySP; }
	virtual UnifySP getUnifySP( void ) { return _unifySP; }
	virtual unify_ptr<T> getPrimary( unify_ptr<T> &candidate ) {
		unify_ptr<T> primary = getUnifySP()->getPrimary(
		 *(  boost::static_pointer_cast< unify_ptr<T>, unify_base<T> >( candidate.getUnifySP() )  )
		);
		candidate.setUnifySP( primary.getUnifySP() );
		return primary;
	}
};


template < typename T >
class unify_object : public unify_base< T > {
private:
	T _object;

public:
	typedef boost::shared_ptr< unify_base<T> > UnifySP;

	unify_object( void ) { }
	unify_object( const T &object ) : _object( object ) { }

	virtual T &getObject( void ) { return _object; }

	virtual void setUnifySP( const UnifySP &unifySP ) { }
	virtual UnifySP getUnifySP( void ) {
		return UnifySP(  static_cast< unify_base<T> * >( 0 )  );
	}
	virtual unify_ptr<T> getPrimary( unify_ptr<T> &candidate ) { return candidate; }
};

#endif
