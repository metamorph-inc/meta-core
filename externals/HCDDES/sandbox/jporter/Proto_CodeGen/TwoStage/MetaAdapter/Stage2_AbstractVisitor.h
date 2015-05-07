#ifndef STAGE2_ABSTRACTVISITOR
#define STAGE2_ABSTRACTVISITOR

#include "Semantics.h"
#include "utils.h"
#include "google/template.h"
#include "boost/lexical_cast.hpp"

#include <set>
#include <map>
#include <list>
#include <iostream>
#include <fstream>

class Stage2_AbstractVisitor : public Semantics::Visitor {

public:
	typedef std::set< Semantics::Component >     ComponentSet;
	typedef std::set< Semantics::Deployment >    DeploymentSet;
	typedef std::set< Semantics::Msg >           MsgSet;
	typedef std::set< Semantics::Signal >        SignalSet;
	typedef std::set< Semantics::TTMsg >         TTMsgSet;
	typedef std::set< Semantics::TTTask >        TTTaskSet;
	typedef std::set< Semantics::Task >		     TaskSet;
	typedef std::set< Semantics::Node >		     NodeSet;
	typedef std::set< Semantics::TTBus >	     TTBusSet;
	typedef std::set< Semantics::CommInterface > IfaceSet;
	typedef std::set< Semantics::BChan >         BChanSet;
	typedef std::set< Semantics::Channel >       ChannelSet;

	// Useful relations
	typedef std::set< Semantics::Acquisition > AcquisitionSet;
	typedef std::set< Semantics::Actuation >   ActuationSet;
	typedef std::set< Semantics::Receives >    ReceivesSet;
	typedef std::set< Semantics::Transmits >   TransmitsSet;

	typedef std::list< std::string > StringList;

	class SignalNumSorter {
	public:
		bool operator()( const Semantics::Signal &s1, const Semantics::Signal &s2 ) {
			return s1.num() < s2.num();
		}
	};
	typedef std::set< Semantics::Signal, SignalNumSorter > SortedSignalNumSet;

	struct SignalFnumSorter {
		bool operator()( const Semantics::Signal &s1, const Semantics::Signal &s2 ) {
			return s1.fnum() < s2.fnum();
		}
	};

	typedef std::set< Semantics::Signal, SignalFnumSorter > SortedSignalFnumSet;


	typedef std::vector< google::TemplateDictionary * > TemplateDictionaryStack;

	typedef std::set< double > ScheduleTimeSet;

protected:
	std::string _name;
	timeStamp _datetime;
	std::string _filepath;

	bool setupDir(); // Create directory for generated files and set path

	TemplateDictionaryStack _templateDictionaryStack;

	void addSectionDictionary( const std::string &sectionName ) {
		_templateDictionaryStack.push_back(
		 getTemplateDictionary().AddSectionDictionary( sectionName )
		);
	}

	void popSectionDictionary( void ) {
		if ( _templateDictionaryStack.size() > 1 ) {
			_templateDictionaryStack.pop_back();
		} else {
			std::cerr << "WARNING:  ATTEMPT TO POP MAIN DICTIONARY" << std::endl;
		}
	}

	google::TemplateDictionary &getTemplateDictionary( void ) {
		return *_templateDictionaryStack.back();
	}

private:
	void addTemplateDictionary( google::TemplateDictionary *templateDictionary ) {
		_templateDictionaryStack.push_back( templateDictionary );
	}

protected:
	google::TemplateDictionary _emptyDictionary;
	google::TemplateDictionary *_mainDictionary;

	void clearDictionary( void ) {
		delete _mainDictionary;
		_mainDictionary = _emptyDictionary.MakeCopy( "MAIN" );
		_templateDictionaryStack.clear();
		addTemplateDictionary( _mainDictionary );
	}

	google::TemplateDictionary &getMainDictionary( void ) { return *_mainDictionary; }

	static ScheduleTimeSet getScheduleTimeSet( const std::string &scheduleTimes );

public:
	Stage2_AbstractVisitor( Semantics::RootFolder &, timeStamp & datetime );
	virtual ~Stage2_AbstractVisitor() { }

	bool WriteFile();

	static StringList split( const std::string &str, const std::string &separator = "," );
};

#endif