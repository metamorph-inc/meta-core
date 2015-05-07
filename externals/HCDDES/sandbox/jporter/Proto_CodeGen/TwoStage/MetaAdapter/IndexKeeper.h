#ifndef INDEX_KEEPER_H
#define INDEX_KEEPER_H

#include <map>
#include <Semantics.h>
#include <sstream>

class IndexKeeper;

// Store references to the sending objects

// Utility Exception
class lookupErr { 
public:
	lookupErr( unsigned long long idx ) {
		std::ostringstream errmsg;
		errmsg << "Failed lookup of index " << idx << ".";
		_what = errmsg.str();
	}
	std::string _what;
};

// Forward maps for later interpreters: ESMoL_elt_id -> Semantics_elt_id
typedef std::map< unsigned long long, unsigned long long > lookup_map;
typedef std::map< unsigned long long, std::pair< unsigned long long, unsigned long long > > lookup_pair_map;
typedef std::map< unsigned long long, std::vector< unsigned long long > > lookup_triple_map;
typedef std::map< unsigned long long, Udm::Object > object_map;

// Actually keep the indices
class IndexKeeper
{
protected:
	IndexKeeper(void) { }
	static IndexKeeper * _inst;

public:
	static IndexKeeper * inst();
	virtual ~IndexKeeper(void) { }

	bool seenIdx( unsigned long long eidx ) { return (_allidx.find( eidx ) != _allidx.end()); }
	bool seenPairIdx( unsigned long long eidx ) { return (_allpairidx.find(eidx) != _allpairidx.end()); }
	bool seenTripleIdx( unsigned long long eidx ) { return  (_alltripleidx.find(eidx) != _alltripleidx.end()); }

	// Store an object
	template <class T>
	void store( unsigned long long eidx, unsigned long long sidx, T & obj )
	{
		_allidx[ eidx ] = sidx;
		_allobj[ sidx ] = Udm::Object::Cast( obj ); // widen to universal type
	}

	// Store two objects
	template <class T1, class T2>
	void store_pair( unsigned long long eidx, unsigned long long sidx1, T1 & obj1,
					 unsigned long long sidx2, T2 & obj2 )
	{
		_allpairidx[ eidx ] = make_pair( sidx1, sidx2 );
		_allobj[ sidx1 ] = Udm::Object::Cast( obj1 );
		_allobj[ sidx2 ] = Udm::Object::Cast( obj2 );
	}

	// Store three objects (now we're really pushing it)
	template <class T1, class T2, class T3>
	void store_triple( unsigned long long eidx, unsigned long long sidx1, T1 & obj1,
		unsigned long long sidx2, T2 & obj2, unsigned long long sidx3, T3 & obj3 )
	{
		vector<unsigned long long> v;
		v.push_back( sidx1 );
		v.push_back( sidx2 );
		v.push_back( sidx3 );
		_alltripleidx[ eidx ] = v;
		_allobj[ sidx1 ] = Udm::Object::Cast( obj1 );
		_allobj[ sidx2 ] = Udm::Object::Cast( obj2 );
		_allobj[ sidx3 ] = Udm::Object::Cast( obj3 );
	}

	// Look up the associated Semantics part for an ESMoL class
	template <class T>
	T lookup( unsigned long long eidx ) throw (lookupErr)
	{ 
		lookup_map::iterator idx = _allidx.find(eidx); 
		if ( idx == _allidx.end() ) 
			throw lookupErr(eidx);
		object_map::iterator it = _allobj.find( idx->second );
		if ( it == _allobj.end() )
			throw lookupErr(idx->second);
		return T::Cast( it->second );
	}

	template <class T1, class T2>
	typename std::pair< T1, T2 > lookup_pair( unsigned long long eidx ) throw (lookupErr)
	{
		lookup_pair_map::iterator idx = _allpairidx.find(eidx); 
		if ( idx == _allpairidx.end() ) 
			throw lookupErr(eidx);
		object_map::iterator it1 = _allobj.find( (idx->second).first );
		if ( it1 == _allobj.end() )
			throw lookupErr( (idx->second).first );
		object_map::iterator it2 = _allobj.find( (idx->second).second );
		if ( it2 == _allobj.end() )
			throw lookupErr( (idx->second).second );
		return std::make_pair( T1::Cast(it1->second), T2::Cast(it2->second) );
	}
	
	template <class T1, class T2, class T3>
	struct triple
	{
		T1 elt1;
		T2 elt2;
		T3 elt3;
	};

	template <class T1, class T2, class T3>
	typename triple< T1, T2, T3 > lookup_triple( unsigned long long eidx ) throw (lookupErr)
	{
		lookup_triple_map::iterator idx = _alltripleidx.find(eidx); 
		if ( idx == _alltripleidx.end() ) 
			throw lookupErr(eidx);

		object_map::iterator it1 = _allobj.find( (idx->second)[0] );
		if ( it1 == _allobj.end() )
			throw lookupErr( (idx->second)[0] );
		object_map::iterator it2 = _allobj.find( (idx->second)[1] );
		if ( it2 == _allobj.end() )
			throw lookupErr( (idx->second)[1] );
		object_map::iterator it3 = _allobj.find( (idx->second)[2] );
		if ( it3 == _allobj.end() )
			throw lookupErr( (idx->second)[2] );

		triple< T1, T2, T3 > tr;
		tr.elt1 = T1::Cast(it1->second);
		t2.elt2 = T2::Cast(it2->second);
		t3.elt3 = T3::Cast(it3->second);
		return tr;
	}

private:
	lookup_map _allidx;
	lookup_pair_map _allpairidx;
	lookup_triple_map _alltripleidx;

	object_map _allobj;
};

#endif // INDEX_KEEPER_H