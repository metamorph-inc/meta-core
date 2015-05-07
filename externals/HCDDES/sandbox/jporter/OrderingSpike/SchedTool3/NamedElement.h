#ifndef NamedElement_h__
#define NamedElement_h__

class NamedElement
{
public:
	NamedElement() { }
	NamedElement( const std::string & name ) : _elt_name( name ) { }
	NamedElement( const char * name ) : _elt_name( name ) { }
	~NamedElement() { }

	virtual const std::string Name() { return _elt_name; }

protected:

	std::string _elt_name;

private:
};

#endif // NamedElement_h__