#ifndef __ABSTRACTVISITOR_H__
#define __ABSTRACTVISITOR_H__


/*** Included Header Files ***/
#include "Semantics.h"
#include "utils.h"
#include "ctemplate/template.h"
#include "boost/lexical_cast.hpp"


/***************************************************************************************/


class AbstractVisitor : public Semantics::Visitor {
public:
	// Message type used for some shared functions
	enum MessageType { TRANSMIT, RECEIVE, PERIPHERAL };
	struct QualifiedMessage {
		Semantics::Msg		message;
		Semantics::Channel	channel;
		MessageType			type;
	};
	struct QualifiedMessageCompare {
		bool operator()( const QualifiedMessage &m1, const QualifiedMessage &m2 ) {
			return m1.message < m2.message;
		}
	};
	// Whole lot of typedefs (useful relations)
	typedef std::set< QualifiedMessage, QualifiedMessageCompare > QualifiedMessageSet;
	typedef std::set< Semantics::Component >			ComponentSet;
	typedef std::vector< Semantics::Deployment >		DeploymentVector;
	typedef std::set< Semantics::Msg >					MsgSet;
	typedef std::vector< Semantics::Msg >				MsgVector;
	typedef std::set< Semantics::Signal	>				SignalSet;
	typedef std::set< Semantics::Msg >					TTMsgSet;
	typedef std::set< Semantics::Task >					TTTaskSet;
	typedef std::set< Semantics::Task >					TaskSet;
	typedef std::vector< Semantics::Task >				TaskVector;
	typedef std::set< Semantics::CommInterface >		CommInterfaceSet;
	typedef std::set< Semantics::CommMedium >			CommMediumSet;
	typedef std::set< Semantics::Device >				DeviceSet;
	typedef std::vector< Semantics::Device >			DeviceVector;
	typedef std::vector< Semantics::Node >				NodeVector;
	typedef std::vector< Semantics::TTBus >				TTBusVector;
	typedef std::vector< Semantics::CANBus >			CANBusVector;
	typedef std::set< Semantics::CommMedium >			BusSet;
	typedef std::set< Semantics::CommInterface >		IfaceSet;
	typedef std::set< Semantics::BChan >				BChanSet;
	typedef std::set< Semantics::IChan >				IChanSet;
	typedef std::set< Semantics::OChan >				OChanSet;
	typedef std::set< Semantics::Channel >				ChannelSet;
	typedef std::set< Semantics::Acquisition >			AcquisitionSet;
	typedef std::set< Semantics::Actuation >			ActuationSet;
	typedef std::vector< Semantics::Actuation >			ActuationVector;
	typedef std::set< Semantics::Receives >				ReceivesSet;
	typedef std::set< Semantics::Transmits >			TransmitsSet;
	typedef std::set< Semantics::LocalDependency >		LocalDepSet;
	typedef std::set< Semantics::Plant >				PlantSet;
	typedef std::set< Semantics::ExecInfo >				ExecInfoSet;
	typedef std::set< Semantics::LocalDependency >		LocalDependencySet;
	typedef std::vector< Semantics::LocalDependency >	LocalDependencyVector;
	typedef std::set< Semantics::DummyDependency >		DummyDependencySet;
	typedef std::list< std::string >					StringList;
	typedef std::vector< ctemplate::TemplateDictionary * >	TemplateDictionaryStack;
	typedef std::set< double >						ScheduleTimeSet;
private:
	void addTemplateDictionary( ctemplate::TemplateDictionary *templateDictionary ) {
		_templateDictionaryStack.push_back( templateDictionary );
	}

protected:
	std::string					_name;
	TimeStamp					_datetime;
	std::string					_filepath;
	TemplateDictionaryStack		_templateDictionaryStack;
	ctemplate::TemplateDictionary		_emptyDictionary;
	ctemplate::TemplateDictionary		*_mainDictionary;

	void ClearDictionary( void ) {
		// Delete the main dictionary
		delete this->_mainDictionary;
		// Create an empty dictionary
		this->_mainDictionary = this->_emptyDictionary.MakeCopy( "MAIN" );
		this->_templateDictionaryStack.clear();
		addTemplateDictionary( this->_mainDictionary );
	}
	ctemplate::TemplateDictionary& GetMainDictionary( void ) { return *(this->_mainDictionary); }
	bool SetupDir();
	void AddSectionDictionary( const std::string &sectionName ) {
		this->_templateDictionaryStack.push_back(
			this->GetTemplateDictionary().AddSectionDictionary( ctemplate::TemplateString(sectionName) )
		);
	}
	void PopSectionDictionary( void ) {
		if ( _templateDictionaryStack.size() > 1 ) {
			this->_templateDictionaryStack.pop_back();
		} else {
			std::cerr << "WARNING: ATTEMPT TO POP MAIN DICTIONARY" << std::endl;
		}
	}
	ctemplate::TemplateDictionary& GetTemplateDictionary( void ) {
		return *(this->_templateDictionaryStack.back());
	}

	static ScheduleTimeSet GetScheduleTimeSet( const std::string &scheduleTimes );

	bool HasTTSched( const Semantics::Executable & ea );
	std::string GetTTSched( const Semantics::Executable & ea );
	bool HasSampPeriod( const Semantics::Executable & ea );
	double GetSampPeriod( const Semantics::Executable & ea );
	double GetDesiredOffset( const Semantics::Executable & ea );
	bool HasMinPeriod( const Semantics::Executable & ea );
	double GetMinPeriod( const Semantics::Executable & ea );
	bool HasMaxDuration( const Semantics::Executable & ea );
	double GetMaxDuration( const Semantics::Executable & ea );

	QualifiedMessageSet GetQualifiedMessages( const Semantics::Task &task );

public:
	class SignalCallIndexSorter {
	public:
		bool operator()( const Semantics::Signal &s1, const Semantics::Signal &s2 ) {
			return s1.callIndex() < s2.callIndex();
		}
	};
	typedef std::set< Semantics::Signal, SignalCallIndexSorter > SortedSignal_ByCallIndex_Set;

	struct SignalMsgIndexSorter {
		bool operator()( const Semantics::Signal &s1, const Semantics::Signal &s2 ) {
			return s1.msgIndex() < s2.msgIndex();
		}
	};
	typedef std::set< Semantics::Signal, SignalMsgIndexSorter >	SortedSignal_ByMsgIndex_Set;

	struct IChanIndexSorter {
		bool operator()( const Semantics::IChan & c1, const Semantics::IChan & c2 ) {
			return c1.chanIndex() < c2.chanIndex();
		}
	};
	typedef std::set< Semantics::IChan, IChanIndexSorter > SortedIChan_ByChanIndex_Set;
	typedef std::vector< Semantics::IChan > IChanVector;

	struct OChanIndexSorter {
		bool operator()( const Semantics::OChan & c1, const Semantics::OChan & c2 ) {
			return c1.chanIndex() < c2.chanIndex();
		}
	};
	typedef std::set< Semantics::OChan, OChanIndexSorter > SortedOChan_ByChanIndex_Set;
	typedef std::vector< Semantics::OChan > OChanVector;

	AbstractVisitor( Semantics::RootFolder &rootFolder, TimeStamp &datetime );
	AbstractVisitor();
	virtual ~AbstractVisitor()	{ }

	bool WriteFile();
	static StringList Split( const std::string &str, const std::string &separator ="," );
	
};


/***************************************************************************************/


#endif //__ABSTRACTVISITOR_H__

