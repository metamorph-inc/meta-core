#ifndef _SCHED_VISITOR_H_
#define _SCHED_VISITOR_H_

#include "google/template.h"
#include "Stage2_VisitorFactory.h"
#include "Stage2_AbstractVisitor.h"

class Stage2_SchedVisitor: public Stage2_AbstractVisitor {

protected:
	static bool _initialized;
	
	//google::TemplateDictionary _dict;

	std::string getQualifiedName(const Semantics::Task & t );

public:
	Stage2_SchedVisitor( Semantics::RootFolder &, timeStamp & ts );
	virtual ~Stage2_SchedVisitor() { }

	static Stage2_AbstractVisitor * getSchedVisitor( Semantics::RootFolder & rf, timeStamp & ts ) {
		return new Stage2_SchedVisitor( rf, ts );
	}

	virtual void Visit_RootFolder(const Semantics::RootFolder & );
	virtual void Visit_Deployment(const Semantics::Deployment & );

	virtual void Visit_Node(const Semantics::Node & ) { }
	virtual void Visit_TTTask(const Semantics::TTTask & );

	virtual void Visit_TTBus(const Semantics::TTBus & ) { }
	virtual void Visit_TTMsg(const Semantics::TTMsg & );
	
	
	virtual void Visit_DeploymentRef(const Semantics::DeploymentRef & ) { }
	virtual void Visit_TimingConstraint(const Semantics::TimingConstraint & ) { }
	virtual void Visit_Deployment_Behavioral(const Semantics::Deployment_Behavioral & ) { }
	
	virtual void Visit_CommMedium(const Semantics::CommMedium & ) { }
	virtual void Visit_Msg(const Semantics::Msg & ) { }
	
	virtual void Visit_Signal(const Semantics::Signal & ) { }
	virtual void Visit_Task(const Semantics::Task & ) { }
	virtual void Visit_Component(const Semantics::Component & ) { }

};

#endif // _SCHED_VISITOR_H_