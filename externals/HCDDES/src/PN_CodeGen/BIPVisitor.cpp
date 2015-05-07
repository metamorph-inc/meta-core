#include "Visitor.h"

#include "ConfigKeeper.h"

#include <set>
#include <sstream>

typedef std::set< BIP::Module > ModSet;
typedef std::set< BIP::PetriNet > PNSet; 
typedef std::set< BIP::Variable > VarSet;
typedef std::set< BIP::State > StateSet;
typedef std::set< BIP::Connector > ConnSet;
typedef std::set< BIP::Port > PortSet;
typedef std::set< BIP::Transition > TransSet;
typedef std::set< BIP::StateToTransition > StTSet;
typedef std::set< BIP::TransitionToState > TtSSet;

using namespace std;

BIPVisitor::BIPVisitor( BIP::RootFolder & brf, TimeStamp & ts ) : 
	_ts( ts )
{
	_contextTracker.showContext( ConfigKeeper::Inst().GetContextEnabled() );
	Context rfContext( _contextTracker, brf.name() );

	ModSet modSet = brf.Module_children();
	for ( ModSet::iterator modIter = modSet.begin(); modIter != modSet.end(); modIter++ )
	{
		Context modContext( _contextTracker, (*modIter).name() );
		(*modIter).Accept( *this );
	}
}

void BIPVisitor::HandleExterns( const BIP::Module & mod )
{
	_tb->AddExterns();
	PNSet pnSet = mod.PetriNet_children();
	for ( PNSet::iterator pnIter = pnSet.begin(); pnIter != pnSet.end(); pnIter++ )
	{
		Context pnCtxt( _contextTracker, (*pnIter).name() );
		
		_transMap = new map< string, bool >();
		_transVars = new map< string, vector< pair< string, string > > >();
		PortSet portSet = pnIter->Port_children();
		for ( PortSet::iterator portIter = portSet.begin(); portIter != portSet.end(); portIter++ )
		{
			Context portCtxt( _contextTracker, (*portIter).name() );
			ConnSet srcSet = (*portIter).srcPortToPort();
			ConnSet dstSet = (*portIter).dstPortToPort();
			if ( srcSet.size() )
			{
				_transMap->insert( make_pair( string( (*portIter).name() ), false ) );
			}
			else if ( dstSet.size() )
			{
				_transMap->insert( make_pair( string( (*portIter).name() ), true ) );
			}

			vector< pair< string, string > > varinfo;
			set< BIP::VariableReference > varRefs = (*portIter).VariableReference_children();
			for ( set< BIP::VariableReference >::iterator vrIter = varRefs.begin();
					vrIter != varRefs.end(); vrIter++ )
			{
				Context vrCtxt( _contextTracker, (*vrIter).name() );
				BIP::Variable var = vrIter->ref();
				varinfo.push_back( make_pair( var.name(), var.datatype() ) );
			}
			_transVars->insert( make_pair( string( (*portIter).name() ), varinfo ) );
		}


		TransSet transSet = pnIter->Transition_children();
		for ( TransSet::iterator transIter = transSet.begin(); transIter != transSet.end(); transIter++ )
		{
			Context transCtxt( _contextTracker, (*transIter).name() );
			if ( !(*_transMap)[ string( (*transIter).name() ) ] )
			{
				_tb->AddSendTransition( (*pnIter).name(), (*transIter).name(), (*_transVars)[ string( (*transIter).name() ) ] );
			}
		}

		delete _transMap;
		delete _transVars;
	}
	_tb->EndExterns();
}

void BIPVisitor::Visit_Module( const BIP::Module & mod ) {

	string modname( mod.name() );
	_tb = new PNTemplateBuilder( modname, _ts.GetDateTime() );

	HandleExterns( mod );

	// Define the traversal
	PNSet pnSet = mod.PetriNet_children();
	for ( PNSet::iterator pnIter = pnSet.begin(); pnIter != pnSet.end(); pnIter++ )
	{
		Context pnCtxt( _contextTracker, (*pnIter).name() );
		(*pnIter).Accept(*this);

		_varMap = new map< string, string >();
		VarSet vSet = pnIter->Variable_children();
		for ( VarSet::iterator vIter = vSet.begin(); vIter != vSet.end(); vIter++ )
		{
			Context varCtxt( _contextTracker, (*vIter).name() );
			(*vIter).Accept(*this);
		}
		_tb->AddGlobalVars( *_varMap );
		delete _varMap;

		_stateMap = new map< string, bool >();
		StateSet stSet = pnIter->State_children();
		for ( StateSet::iterator stIter = stSet.begin(); stIter != stSet.end(); stIter++ )
		{
			Context stCtxt( _contextTracker, (*stIter).name());
			(*stIter).Accept(*this);
		}
		_tb->AddStates( *_stateMap );
		delete _stateMap;

		_tb->AddStruct();
		_tb->AddSetup();

		_transMap = new map< string, bool >();
		_transVars = new map< string, vector< pair < string, string > > >();
		PortSet portSet = pnIter->Port_children();
		for ( PortSet::iterator portIter = portSet.begin(); portIter != portSet.end(); portIter++ )
		{
			Context portCtxt( _contextTracker, (*portIter).name() );
			(*portIter).Accept(*this);
		}

		TransSet transSet = pnIter->Transition_children();
		for ( TransSet::iterator transIter = transSet.begin(); transIter != transSet.end(); transIter++ )
		{
			Context transCtxt( _contextTracker, (*transIter).name() );
			(*transIter).Accept(*this);
		}

		delete _transMap;
		delete _transVars;

		_tb->ClosePetriNet();
	}

	PathMgr * pm = &(ConfigKeeper::Inst().GetPathMgr());
	_tb->WriteModuleFile( pm->GetHeaderFilename( modname ), pm->GetHeaderTemplate() ); // header
	_tb->WriteModuleFile( pm->GetCodeFilename( modname ), pm->GetCodeTemplate() ); // code
	
	delete (_tb);
}

void BIPVisitor::Visit_PetriNet( const BIP::PetriNet & pn ) {

	_tb->AddPetriNet( pn.name() );
}

void BIPVisitor::Visit_Variable( const BIP::Variable & v ) {

	_varMap->insert( make_pair( string(v.name()), string(v.datatype()) ));
}

void BIPVisitor::Visit_State( const BIP::State & st ) {

	_stateMap->insert( make_pair( string(st.name()), st.InitialeState() ));
}

void BIPVisitor::Visit_Port( const BIP::Port & p ) {

	ConnSet srcSet = p.srcPortToPort();
	ConnSet dstSet = p.dstPortToPort();

	if ( srcSet.size() )
	{
		_transMap->insert( make_pair( string( p.name() ), false ) );
	}
	else if ( dstSet.size() )
	{
		_transMap->insert( make_pair( string( p.name() ), true ) );
	}

	vector< pair< string, string > > varinfo;
	set< BIP::VariableReference > varRefs = p.VariableReference_children();
	for ( set< BIP::VariableReference >::iterator vrIter = varRefs.begin();
			vrIter != varRefs.end(); vrIter++ )
	{
		Context vrCtxt( _contextTracker, (*vrIter).name() );
		BIP::Variable var = vrIter->ref();
		varinfo.push_back( make_pair( var.name(), var.datatype() ) );
	}
	_transVars->insert( make_pair( string( p.name() ), varinfo ) );
}

void BIPVisitor::Visit_VariableReference( const BIP::VariableReference & vr ) {

}

void BIPVisitor::Visit_Transition( const BIP::Transition & t ) {

	StTSet sttSet = t.srcStateToTransition();
	vector< string > srcStates;
	for ( StTSet::iterator sttIter = sttSet.begin(); sttIter != sttSet.end(); sttIter++ )
	{
		ostringstream out;
		out << (*sttIter).name() << "_" << (*sttIter).uniqueId();
		Context sttCtxt( _contextTracker, out.str() );
		BIP::State st = sttIter->srcStateToTransition_end();
		srcStates.push_back( string( st.name() ) );
	}	
		
	TtSSet ttsSet = t.dstTransitionToState();
	vector< string > dstStates;
	for ( TtSSet::iterator ttsIter = ttsSet.begin(); ttsIter != ttsSet.end(); ttsIter++ )
	{
		ostringstream out;
		out << (*ttsIter).name() << "_" << (*ttsIter).uniqueId();
		Context ttsCtxt( _contextTracker, (*ttsIter).name() );
		BIP::State st = ttsIter->dstTransitionToState_end();
		dstStates.push_back( string( st.name() ) );
	}

	
	_tb->AddTransition( string( t.name() ), (*_transMap)[ string( t.name() ) ],
						(*_transVars)[ string( t.name() ) ], srcStates, dstStates, 
						string( t.guard() ), string( t.action() ) );
}

void BIPVisitor::Visit_TransitionToState( const BIP::TransitionToState & tts ) {

}

void BIPVisitor::Visit_StateToTransition( const BIP::StateToTransition & stt ) {

}

void BIPVisitor::Visit_Connector( const BIP::Connector & conn ) {

}