#ifndef __FRODO2_VISITOR_H__
#define __FRODO2_VISITOR_H__


/*** Included Header Files ***/
#include "utils.h"
#include "ctemplate/template.h"
#include "VisitorFactory.h"
#include "AbstractVisitor.h"


/***************************************************************************************/



struct TTNetwork {
	int			id;
	std::map< std::string, int > *nodes;
};



class Frodo2Visitor : public AbstractVisitor {
protected:
	static bool						_initialized;			// Has the visitor been initialized
	bool							_pass;					// Is this the second pass
//	int								_analogIn, _analogOut;	// For counting # of node analog IO
	std::map< std::string, TTNetwork >	_networkMap;		// Map of network names to TTNetwork structs

	int SignalSize( const std::string &name );
	std::string SignalName( const std::string &type, const int &callIndex );
//	void DetermineAnalogIO( const Semantics::Node &node, int &inputs, int &outputs );

public:
	// Constructors and Destructors
	Frodo2Visitor( Semantics::RootFolder &rootFolder, TimeStamp &timeStamp );
	virtual ~Frodo2Visitor()		{ }

	// Factory Method
	static AbstractVisitor * GetFrodo2Visitor( Semantics::RootFolder & rf, TimeStamp & ts ) {
		return new Frodo2Visitor( rf, ts );
	}

	// Visitor Methods
	virtual void Visit_RootFolder( const Semantics::RootFolder &rootFolder );
	virtual void Visit_Deployment( const Semantics::Deployment &deployment );
	virtual void Visit_Node( const Semantics::Node &node );
	virtual void Visit_LocalDependency( const Semantics::LocalDependency &dependency );
	virtual void Visit_Task( const Semantics::Task &task );
	virtual void Visit_Device( const Semantics::Device &device );
	virtual void Visit_IChan( const Semantics::IChan &iChan );
	virtual void Visit_OChan( const Semantics::OChan &oChan);
	virtual void Visit_TTBus( const Semantics::TTBus &bus );
	virtual void Visit_CANBus( const Semantics::CANBus & bus );
	virtual void Visit_Plant( const Semantics::Plant &plant );
	virtual void Visit_Signal( const Semantics::Signal &signal, bool isParameter=false );
	void Visit_QualifiedMessageStructure( const QualifiedMessage &qualifiedMessage, 
										  const Semantics::Task &task );
	void Visit_Signal_Member( const Semantics::Signal &signal );
	virtual void Visit_Component( const Semantics::Component & )				{ }
	virtual void Visit_TimingConstraint( const Semantics::TimingConstraint & )	{ }
	virtual void Visit_CommMedium( const Semantics::CommMedium & )				{ }
};


/***************************************************************************************/


#endif // __FRODO2_VISITOR_H__

