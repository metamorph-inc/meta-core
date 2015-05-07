#ifndef __OILVISITOR_H__
#define __OILVISITOR_H__

#include "VisitorFactory.h"
#include "AbstractVisitor.h"
#include "OIL.h"

class OILVisitor : public AbstractVisitor {
protected:
	static bool	_initialized;

	void ValidateExecInfo(const Semantics::ExecInfo& exec_info);
	bool MakeOIL(const Semantics::Node& ecu);
	void MakeOIL(const Semantics::Node& ecu, const OIL::CPU& cpu);
	void MakeOIL(const Semantics::OSEKParameters& e_os, const OIL::CPU& cpu);
	void MakeOIL(const Semantics::Task& task, const OIL::CPU& cpu);
	void MakeOILCom(const Semantics::OSEKParameters& e_com, const OIL::CPU& cpu);


public:
	OILVisitor( Semantics::RootFolder &rootFolder, TimeStamp & timeStamp ) :
		AbstractVisitor( rootFolder, timeStamp ) { }
	virtual ~OILVisitor()		{ }

	static AbstractVisitor * GetOILVisitor( Semantics::RootFolder &rootFolder, TimeStamp &timeStamp ) {
		return new OILVisitor( rootFolder, timeStamp );
	}

	virtual void Visit_RootFolder( const Semantics::RootFolder & );
};

#endif // __OILVISITOR_H__