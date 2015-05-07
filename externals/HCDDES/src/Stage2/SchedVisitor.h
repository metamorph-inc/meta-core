#ifndef __SCHEDVISITOR_H__
#define __SCHEDVISITOR_H__


/*** Included Header Files ***/
#include "utils.h"
#include "ctemplate/template.h"
#include "VisitorFactory.h"
#include "AbstractVisitor.h"


/***************************************************************************************/


class SchedVisitor: public AbstractVisitor {
protected:
	static bool						_initialized;
	//ctemplate::TemplateDictionary	_dict;
	std::string GetQualifiedName(const Semantics::Task & task );

	Semantics::InputDevice			_curInputDevice;
	Semantics::OutputDevice			_curOutputDevice;
	Semantics::Node					_curNode;

public:
	SchedVisitor( Semantics::RootFolder &rootFolder, TimeStamp &timeStamp );
	virtual ~SchedVisitor()			{ }

	static AbstractVisitor * GetSchedVisitor( Semantics::RootFolder & rf, TimeStamp & ts ) {
		return new SchedVisitor( rf, ts );
	}

	// Overridden Methods
	virtual void Visit_RootFolder(const Semantics::RootFolder & );
	virtual void Visit_Deployment(const Semantics::Deployment & );

	virtual void Visit_Node(const Semantics::Node & )					{ }
	virtual void Visit_TTBus(const Semantics::TTBus & )					{ }
	virtual void Visit_CANBus(const Semantics::CANBus & )					{ }
	virtual void Visit_TimingConstraint(const Semantics::TimingConstraint & ) { }
	virtual void Visit_CommMedium(const Semantics::CommMedium & )		{ }
	virtual void Visit_Msg(const Semantics::Msg & );
	virtual void Visit_Signal(const Semantics::Signal & )				{ }
	virtual void Visit_Task(const Semantics::Task & );
	virtual void Visit_InputDevice( const Semantics::InputDevice & );
	virtual void Visit_OutputDevice( const Semantics::OutputDevice & );
	virtual void Visit_Component(const Semantics::Component & )			{ }

	virtual void Visit_Acquisition(const Semantics::Acquisition & );
	virtual void Visit_Actuation(const Semantics::Actuation & );
	virtual void Visit_LocalDependency(const Semantics::LocalDependency & );

};


/***************************************************************************************/


#endif // __SCHEDVISITOR_H__

