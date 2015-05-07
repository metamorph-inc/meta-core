#ifndef __INDEXKEEPER_H__
#define __INDEXKEEPER_H__


/*** Included Header Files ***/
#include <map>
#include <sstream>
#include <Semantics.h>

#include "IndexMerge.h"


/*** Class Predefinitions ***/
class IndexKeeper;


// Forward maps for later interpreters: ESMoL_elt_id -> Semantics_elt_id
typedef std::map< ID_TYPE, ID_TYPE > LookupMap;
typedef std::map< ID_TYPE, std::pair< ID_TYPE, ID_TYPE > > LookupPairMap;
typedef std::map< ID_TYPE, std::vector< ID_TYPE > > LookupTripleMap;
typedef std::map< ID_TYPE, Udm::Object > ObjectMap;


/***************************************************************************************/


class LookupErr { 
public:
	std::string _what;
	LookupErr( const ID_TYPE &index ) {
		std::ostringstream errmsg;
		errmsg << "Failed lookup of index " << index << ".";
		this->_what = errmsg.str();
	}
};


/***************************************************************************************/


class IndexKeeper {
private:
	LookupMap						_allIndex;
	LookupPairMap					_allPairIndex;
	//LookupTripleMap					_allTripleIndex;
	ObjectMap						_allObjects;

protected:
	IndexKeeper(void)				{ }
	static IndexKeeper				*_inst;

public:
	// Constructors and Destructors
	virtual ~IndexKeeper(void)		{ }

	static IndexKeeper *Inst();
	bool SeenIndex( const ID_TYPE &index ) { 
		return (this->_allIndex.find( index ) != this->_allIndex.end()); }
	bool SeenPairIndex( const ID_TYPE &index ) { 
		return (this->_allPairIndex.find(index) != this->_allPairIndex.end()); }
	//bool SeenTripleIndex( const ID_TYPE &index ) {
	//	return (this->_allTripleIndex.find(index) != this->_allTripleIndex.end()); }

	// Store an object
	template <class T>
	void Store( ID_TYPE eIndex, ID_TYPE sIndex, T &obj ) {
		// Store the sIndex value into the eIndex slot
		this->_allIndex[ eIndex ] = sIndex;
		// Widen to universal type
		this->_allObjects[ sIndex ] = Udm::Object::Cast( obj );
	}

	// Store two objects
	template <class T1, class T2>
	void StorePair( const ID_TYPE &eIndex, const ID_TYPE &sIndex1, T1 &obj1,
					const ID_TYPE &sIndex2, T2 &obj2 ) {
		// Insert sIndex1 and sIndex2 into the allPair map
		this->_allPairIndex[ eIndex ] = std::make_pair( sIndex1, sIndex2 );
		// Widen to universal types
		this->_allObjects[ sIndex1 ] = Udm::Object::Cast( obj1 );
		this->_allObjects[ sIndex2 ] = Udm::Object::Cast( obj2 );
	}

	// Store three objects (now we're really pushing it)
	//template <class T1, class T2, class T3>
	//void StoreTriple( const ID_TYPE &eIndex, const ID_TYPE &sIndex1, T1 &obj1,
	//	const ID_TYPE &sIndex2, T2 &obj2, const ID_TYPE &sIndex3, T3 &obj3 ) {
		// Push vector of values into the all triple index
	//	vector<ID_TYPE> vec;
	//	vec.push_back( sIndex1 );
	//	vec.push_back( sIndex2 );
	//	vec.push_back( sIndex3 );
	//	this->_allTripleIndex[ eIndex ] = vec;
		// Widen to universal types
	//	this->_allObjects[ sIndex1 ] = Udm::Object::Cast( obj1 );
	//	this->_allObjects[ sIndex2 ] = Udm::Object::Cast( obj2 );
	//	this->_allObjects[ sIndex3 ] = Udm::Object::Cast( obj3 );
	//}

	// Look up the associated Semantics part for an ESMoL class
	template <class T>
	T Lookup( const ID_TYPE &eIndex ) throw (LookupErr) {
		LookupMap::iterator indexIter = this->_allIndex.find( eIndex ); 
		// If the index is not found, throw an exception
		if ( indexIter == this->_allIndex.end() ) throw LookupErr( eIndex );
		// Otherwise, find the associated object
		ObjectMap::iterator objIter = this->_allObjects.find( indexIter->second );
		// If not found in object map, throw an exception
		if ( objIter == this->_allObjects.end() ) throw LookupErr( indexIter->second );
		// Otherwise, all is good...
		return T::Cast( objIter->second );
	}

	template <class T1, class T2>
	typename std::pair< T1, T2 > LookupPair( const ID_TYPE &eIndex ) throw (LookupErr) {
		// Try to find the index in the all pair map
		LookupPairMap::iterator pairIter = this->_allPairIndex.find( eIndex );
		// If not found, throw an exception
		if ( pairIter == this->_allPairIndex.end() ) throw LookupErr( eIndex );
		// Try to find first item in the object map
		ObjectMap::iterator objectIter1 = this->_allObjects.find( ( pairIter->second).first );
		// If not found, throw an exception
		if ( objectIter1 == this->_allObjects.end() ) throw LookupErr( (pairIter->second).first );
		// Try to find second item in the object map
		ObjectMap::iterator objectIter2 = this->_allObjects.find( (pairIter->second).second );
		// If not found, throw an exception
		if ( objectIter2 == this->_allObjects.end() ) throw LookupErr( (pairIter->second).second );
		// Otherwise, all is good...
		return std::make_pair( T1::Cast( objectIter1->second ), T2::Cast( objectIter2->second ) );
	}

	// Quick and Dirty Triple template structure
	//template <class T1, class T2, class T3>
	//struct Triple {
	//	T1 element1;
	//	T2 element2;
	//	T3 element3;
	//};

	//template <class T1, class T2, class T3>
	//typename Triple< T1, T2, T3 > LookupTriple( const ID_TYPE &eIndex ) throw (LookupErr) {
		// Get the triple index
	//	LookupTripleMap::iterator tripleIter = this->_allTripleIndex.find( eIndex ); 
	//	if ( tripleIter == this->_allTripleIndex.end() ) throw lookupErr(eidx);
		
		// Get the first object
	//	ObjectMap::iterator objIter1 = this->_allObjects.find( (tripleIter->second)[0] );
	//	if ( objIter1 == this->_allObjects.end() ) throw LookupErr( (tripleIter->second)[0] );
		// Get the second object
	//	ObjectMap::iterator objIter2 = this->_allObjects.find( (tripleIter->second)[1] );
	//	if ( objIter2 == this->_allObjects.end() ) throw LookupErr( (tripleIter->second)[1] );
		// Get the third object
	//	ObjectMap::iterator objIter3 = this->_allObjects.find( (tripleIter->second)[2] );
	//	if ( objIter3 == this->_allObjects.end() ) throw LookupErr( (tripleIter->second)[2] );

		// Create the return triple
	//	Triple< T1, T2, T3 > trip;
	//	trip.element1 = T1::Cast( objIter1->second );
	//	trip.element2 = T2::Cast( objIter2->second );
	//	trip.element3 = T3::Cast( objIter3->second );
	//	return trip;
	//}
};


/***************************************************************************************/


#endif // __INDEXKEEPER_H__

