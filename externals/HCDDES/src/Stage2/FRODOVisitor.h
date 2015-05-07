#ifndef __FRODOVISITOR_H__
#define __FRODOVISITOR_H__


/*** Included Header Files ***/
#include "VisitorFactory.h"
#include "AbstractVisitor.h"


/***************************************************************************************/


class FRODOVisitor : public AbstractVisitor {
private:
	//enum MsgType { TRANSMIT, RECEIVE, PERIPHERAL, LOCAL_SEND, LOCAL_RECV };
	enum MsgDirection { SEND, RECEIVE };
	enum MsgContext { BUS, PERIPHERAL, LOCAL, DUMMY };
	enum MsgTimed { SCHEDULED, UNSCHEDULED };
	struct QualifiedTTMsg {
		Semantics::Msg ttMsg;
		Semantics::Channel channel;
		MsgDirection msgDirect;
		MsgContext msgCtxt;
		MsgTimed msgTimed;
		bool generateStruct;
	};
	struct QualifiedTTMsgCompare {
		bool operator()( const QualifiedTTMsg &m1, const QualifiedTTMsg &m2 ) {
			std::string nm1 = m1.ttMsg.name();
			std::string nm2 = m2.ttMsg.name();
			return nm1 < nm2;
		}
	};
	// A few type definitions
	typedef std::map< double, QualifiedTTMsg > QualifiedTTMsgScheduleMap;
	typedef std::map< double, Semantics::Task > TTTaskScheduleMap;
	typedef std::set< QualifiedTTMsg, QualifiedTTMsgCompare > QualifiedTTMsgSet;
	typedef std::map< std::string, std::string > ConfigParameterMap;
	typedef std::set< std::string > MsgNameSet;

	// Class 
	TTTaskScheduleMap			_ttTaskScheduleMap;
	std::string					_platformType;
	QualifiedTTMsgScheduleMap	_qualifiedTTMsgScheduleMap;
	QualifiedTTMsgSet			_qualifiedTTMsgSet;
	MsgNameSet					_msgNameKeeper;

		
protected:
	static bool					_initialized;

public:
	// Constructors and Destructors
	FRODOVisitor( Semantics::RootFolder &rootFolder, TimeStamp & timeStamp ) :
		AbstractVisitor( rootFolder, timeStamp ) { }
	virtual ~FRODOVisitor()		{ }

	static AbstractVisitor * GetFRODOVisitor( Semantics::RootFolder &rootFolder, TimeStamp &timeStamp ) {
		return new FRODOVisitor( rootFolder, timeStamp );
	}

	virtual void Visit_RootFolder( const Semantics::RootFolder & );
	void Visit_Deployment( const Semantics::Deployment & );
	void Visit_Node( const Semantics::Node &node );
	void Visit_QualifiedTTMsg( const QualifiedTTMsg &qualifiedTTMsg );
	void ProcessQualifiedTTMsgScheduleMap( void );
	void ScheduleTTMsg( const Semantics::Msg &, int index, unsigned long long scheduleTime, const std::string & ); 
	void SchedulePeripheral( const QualifiedTTMsg qualifiedTTMsg, int index, unsigned long long scheduleTime );
	void Visit_QualifiedTTMsg_Structure( const QualifiedTTMsg & );
	void Visit_TTTask( const Semantics::Task & );
	void BuildInitSection( TaskVector & tasks );
	void GetQualifiedTTMsgs( const Semantics::Task &ttTask );
	void ProcessTTTaskScheduleMap( void );
	void Visit_Signal_Member( const Semantics::Signal & );
	void Visit_TTTask_Subsystem( const Semantics::Task & );
	void Visit_Signal( const Semantics::Signal &, bool isParameter = false );
	static ConfigParameterMap GetConfigParameterMap( const std::string &config );
};


/***************************************************************************************/


#endif // __FRODOVISITOR_H__

