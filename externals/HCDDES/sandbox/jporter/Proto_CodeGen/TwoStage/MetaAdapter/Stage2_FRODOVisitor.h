#ifndef _FRODO_VISITOR_H_
#define _FRODO_VISITOR_H_

#include "Stage2_VisitorFactory.h"
#include "Stage2_AbstractVisitor.h"

class Stage2_FRODOVisitor : public Stage2_AbstractVisitor {

private:
	std::string _platformType;

	typedef std::map< double, Semantics::TTTask > TTTaskScheduleMap;
	TTTaskScheduleMap _ttTaskScheduleMap;

	enum MsgType { TRANSMIT, RECEIVE, PERIPHERAL };
	struct QualifiedTTMsg {
		Semantics::TTMsg ttMsg;
		Semantics::Channel channel;
		MsgType msgType;
	};

	typedef std::map< double, QualifiedTTMsg > QualifiedTTMsgScheduleMap;
	QualifiedTTMsgScheduleMap _qualifiedTTMsgScheduleMap;

	struct QualifiedTTMsgCompare {
		bool operator()( const QualifiedTTMsg &m1, const QualifiedTTMsg &m2 ) {
			return m1.ttMsg < m2.ttMsg;
		}
	};

	typedef std::set< QualifiedTTMsg, QualifiedTTMsgCompare > QualifiedTTMsgSet;
	typedef std::map< std::string, std::string > ConfigParameterMap;

protected:
	static bool _initialized;

public:
	Stage2_FRODOVisitor( Semantics::RootFolder &, timeStamp & ts );
	virtual ~Stage2_FRODOVisitor() { }

	static Stage2_AbstractVisitor * getFRODOVisitor( Semantics::RootFolder & rf, timeStamp & ts ) {
		return new Stage2_FRODOVisitor( rf, ts );
	}

	virtual void Visit_RootFolder( const Semantics::RootFolder & );
	void Visit_Deployment( const Semantics::Deployment & );

	void Visit_Node( const Semantics::Node &node );

	void Visit_QualifiedTTMsg( const QualifiedTTMsg &qualifiedTTMsg );
	void processQualifiedTTMsgScheduleMap( void );
	void scheduleTTMsg( const Semantics::TTMsg &, int, double, const std::string & ); 
	void schedulePeripheral( const QualifiedTTMsg qualifiedTTMsg, int index, double scheduleTime );
	void Visit_QualifiedTTMsg_Structure( const QualifiedTTMsg & );

	void Visit_TTTask( const Semantics::TTTask & );
	QualifiedTTMsgSet getQualifiedTTMsgs( const Semantics::TTTask &ttTask );
	void processTTTaskScheduleMap( void );

	void Visit_Signal_Member( const Semantics::Signal & );
	void Visit_TTTask_Subsystem( const Semantics::TTTask & );
	void Visit_Signal( const Semantics::Signal &, bool isParameter = false );

	static ConfigParameterMap getConfigParameterMap( const std::string &config );
};

#endif // _FRODO_VISITOR_H_