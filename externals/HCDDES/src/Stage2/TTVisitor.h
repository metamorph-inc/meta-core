#ifndef __TTVISITOR_H__
#define __TTVISITOR_H__

#include <vector>
#include <numeric>
#include <sstream>

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



class TrueTimeVisitor : public AbstractVisitor {
protected:
	static bool _initialized;   // Has the visitor been initialized

	int SignalSize( const std::string &name );
	std::string SignalName( const std::string &type, const int &callIndex );

public:
	// Constructors and Destructors
	TrueTimeVisitor( Semantics::RootFolder &rootFolder, TimeStamp &timeStamp );
	TrueTimeVisitor();
	virtual ~TrueTimeVisitor()		{ }

	// Factory Method
	static AbstractVisitor * GetTrueTimeVisitor( Semantics::RootFolder & rf, TimeStamp & ts ) {
		return new TrueTimeVisitor( rf, ts );
	}

	// Visitor Methods
	virtual void Visit_RootFolder( const Semantics::RootFolder & rootFolder );
	virtual void Visit_Deployment( const Semantics::Deployment & deployment ) { }
	virtual void Visit_Node( const Semantics::Node & node ) { }
	virtual void Visit_LocalDependency( const Semantics::LocalDependency & dependency ) { }
	virtual void Visit_Task( const Semantics::Task & task ) { }
	virtual void Visit_Device( const Semantics::Device & device ) { }
	virtual void Visit_IChan( const Semantics::IChan & iChan ) { }
	virtual void Visit_OChan( const Semantics::OChan & oChan) { }
	virtual void Visit_TTBus( const Semantics::TTBus & bus ) { }
	virtual void Visit_CANBus( const Semantics::CANBus & bus ) { }
	virtual void Visit_Plant( const Semantics::Plant & plant ) { }
	virtual void Visit_Signal( const Semantics::Signal & signal, bool isParameter=false ) { }
	void Visit_QualifiedMessageStructure( const QualifiedMessage & qualifiedMessage, 
										  const Semantics::Task & task ) { }
	void Visit_Signal_Member( const Semantics::Signal & signal ) { }
	virtual void Visit_Component( const Semantics::Component & )				{ }
	virtual void Visit_TimingConstraint( const Semantics::TimingConstraint & )	{ }
	virtual void Visit_CommMedium( const Semantics::CommMedium & )				{ }
	void Visit_DummyMessage( Semantics::Msg & dummyMsg ) { }
};


class TrueTimeSimulinkVisitor : public TrueTimeVisitor
{
protected:

	typedef struct {

		unsigned int sigsize;
		unsigned int sigIdx;
		string portname;

	} ioport_struct;

	typedef vector< ioport_struct > spec_list_type;

	std::map< std::string, TTNetwork >  _networkMap;           // Map of network names to TTNetwork structs

	std::string		_baseName;
	std::string		_directoryName;
	bool			_metamode;

	void DetermineAnalogIO( const Semantics::Node & node, spec_list_type & input_spec, spec_list_type & output_spec );

	unsigned long sum_sig_sizes( const spec_list_type & specs ) {

		unsigned long sum = 0;
		for ( spec_list_type::const_iterator specIter = specs.begin(); specIter != specs.end(); specIter++ )
		{
			sum += (*specIter).sigsize;
		}
		return sum;
	}
	
	std::string spec_size_str( const spec_list_type & specs ) {

		std::ostringstream out;
		out << "[";
		for ( spec_list_type::const_iterator specIter = specs.begin(); specIter != specs.end(); specIter++ )
		{
			out << " " << (*specIter).sigsize;
		}
		out << " ]";

		return out.str();
	}

public:

	// Constructors and Destructors
	TrueTimeSimulinkVisitor();
	virtual ~TrueTimeSimulinkVisitor() { }

	const std::map< std::string, TTNetwork >  & getNetworkMap() { return _networkMap; }

	// Visitor Methods
	virtual void Visit_RootFolder( const Semantics::RootFolder & rootFolder );
	virtual void Visit_Deployment( const Semantics::Deployment & deployment );
	virtual void Visit_Node( const Semantics::Node & node );
	virtual void Visit_LocalDependency( const Semantics::LocalDependency & dependency ) { }
	virtual void Visit_Task( const Semantics::Task & task ) { }
	virtual void Visit_Device( const Semantics::Device & device ) { }
	virtual void Visit_IChan( const Semantics::IChan & iChan ) { }
	virtual void Visit_OChan( const Semantics::OChan & oChan) { }
	virtual void Visit_TTBus( const Semantics::TTBus & bus );
	virtual void Visit_CANBus( const Semantics::CANBus & bus );
	virtual void Visit_Plant( const Semantics::Plant & plant ) { }
	virtual void Visit_Signal( const Semantics::Signal & signal, bool isParameter=false ) { }
	void Visit_QualifiedMessageStructure( const QualifiedMessage & qualifiedMessage, 
										  const Semantics::Task & task );
	void Visit_Signal_Member( const Semantics::Signal & signal );
	virtual void Visit_Component( const Semantics::Component & )				{ }
	virtual void Visit_TimingConstraint( const Semantics::TimingConstraint & )	{ }
	virtual void Visit_CommMedium( const Semantics::CommMedium & )				{ }
	void Visit_DummyMessage( Semantics::Msg & dummyMsg );
	void SetUpDictionary();
	void WriteTemplate( std::string & template_filename, std::string & output_filename );
	void Gen_Deployment_Script( NodeVector & nodeVector, BusSet  & buses );
};

class TrueTimeSourceVisitor : public TrueTimeVisitor
{
protected:

	std::vector< std::string > _SchedHeaderLines;
	std::map< std::string, std::set< std::string > > _BusHeaderLines;

	const std::map< std::string, TTNetwork >  & _networkMap;   // Map of network names to TTNetwork structs

	int                                 _analogIn, _analogOut; // For counting # of node analog IO

	std::set< std::string > _already_included;                 // keep track of includes already made

	Semantics::CommMedium GetMessageBus( Semantics::Msg & m );

public:
	// Constructors and Destructors
	TrueTimeSourceVisitor( const std::map< std::string, TTNetwork >  & nwMap );
	virtual ~TrueTimeSourceVisitor()  { }


	// Visitor Methods
	virtual void Visit_RootFolder( const Semantics::RootFolder & rootFolder );
	virtual void Visit_Deployment( const Semantics::Deployment & deployment );
	virtual void Visit_Node( const Semantics::Node & node );
	virtual void Visit_LocalDependency( const Semantics::LocalDependency & dependency );
	virtual void Visit_Task( const Semantics::Task & task );
	virtual void Visit_Device( const Semantics::Device & device ) { }
	virtual void Visit_IChan( const Semantics::IChan & iChan );
	virtual void Visit_OChan( const Semantics::OChan & oChan);
	virtual void Visit_TTBus( const Semantics::TTBus & bus ) { }
	virtual void Visit_CANBus( const Semantics::CANBus & bus ) { }
	virtual void Visit_Plant( const Semantics::Plant & plant ) { }
	virtual void Visit_Signal( const Semantics::Signal & signal, bool isParameter=false );
	void Visit_QualifiedMessageStructure( const QualifiedMessage & qualifiedMessage, 
										  const Semantics::Task & task );
	void Visit_Signal_Member( const Semantics::Signal & signal );
	virtual void Visit_Component( const Semantics::Component & )				{ }
	virtual void Visit_TimingConstraint( const Semantics::TimingConstraint & )	{ }
	virtual void Visit_CommMedium( const Semantics::CommMedium & )				{ }
	void Visit_DummyMessage( Semantics::Msg & dummyMsg );
};


/***************************************************************************************/


#endif // __TTVISITOR_H__

