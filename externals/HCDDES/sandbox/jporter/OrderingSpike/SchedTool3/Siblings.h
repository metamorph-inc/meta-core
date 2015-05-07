#ifndef Siblings_h__
#define Siblings_h__

// Interfaces to relate two objects in the following way:
// 
// The oldest sibling can have multiple siblings, each distinguished by a unique string
// The other siblings can only have one oldest sibling, but each stores its own name
// In practice the sibling string should be a type or category of objects

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class Sibling;
typedef boost::shared_ptr< Sibling > SiblingPtr;
typedef std::map< const std::string, SiblingPtr > SiblingMap;
class OldestSibling;
typedef boost::shared_ptr< OldestSibling > OldestSiblingPtr;

class OldestSibling
{
public:
	OldestSibling();

	void AddSibling( SiblingPtr p ) { _siblings.insert( std::make_pair( p->Category(), p ) ); }

protected:
	SiblingMap _siblings;

};


class Sibling : boost::enable_shared_from_this< Sibling >
{
public:

	Sibling();

	void setCategory( const std::string & cat ) { _category = cat; }
	const std::string Category() { return _category; }

	void setOldestSibling( OldestSiblingPtr p ) { _oldest = p; p->AddSibling( shared_from_this() ); }
	template< class T > getOldestSibling() { return boost::shared_dynamic_cast< T >( _oldest ); }

protected:

	OldestSiblingPtr _oldest;
	std::string _category;
};

#endif // Siblings_h__