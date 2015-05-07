#ifndef _ISIS_VARRESOLVE
#define _ISIS_VARRESOLVE

#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <boost/shared_ptr.hpp>


class VarResolve {
private:

	// VarResolve USES A TREE STRUCTURE TO RESOLVE VARIABLE SCOPE.  EACH NODE IS INDEXED
	// BY A STRING THAT IS THE NAME OF THE (STATE) SCOPE AT A PARTICULAR PATH AND LEVEL IN THE
	// SCOPE TREE.  THE NODE ITSELF IS A MAP OF STRING->NODE PAIRS, OR, MORE PRECISELY,
	// A MAP OF STRING->SMART-POINTER-TO-NODE PAIRS.

	// ONE PROBLEM IS THAT, WHEN THE SMART-POINTER TYPE IS DEFINED, THE NODE TYPE IS NOT YET DEFINED.
	// REALLY, AS THE TREE IS A SELF-REFERENTIAL DATA TYPE, AND THERE IS NO WAY (THAT I'M AWARE OF)
	// TO "DECLARE" A TYPEDEF, THE NODE TYPE CAN'T BE DEFINED BEFORE THE SMART-POINTER TYPE.  SO,
	// THE SMART-POINTER IS A SMART-POINTER-TO-VOID THAT MUST BE EXPLICITLY CAST TO
	// SMART-POINTER-TO-NODE WHEN NEEDED.
	typedef boost::shared_ptr< void > ResMapSPProxy;
	typedef std::map< std::string, ResMapSPProxy > ResMap;
	typedef boost::shared_ptr< ResMap > ResMapSP;

	typedef std::list< ResMapSP > ResMapSPList;
	typedef std::map< std::string, ResMapSPList > ResMapSPLookup;

public:
	typedef std::list< std::string > ScopePath;

	typedef std::pair< ScopePath, ScopePath > ScopeStructPair;
	typedef std::list< ScopeStructPair > ScopeStructPairList;

private:
	typedef std::list< ScopePath > ScopePathList;

	ResMapSP _rootResMapSP;
	ResMapSPLookup _resMapSPLookup;

	static ResMapSP resMapSPProxyCast( ResMapSPProxy resMapSPProxy ) {
		return boost::static_pointer_cast< ResMap, void >( resMapSPProxy );
	}

	// A VARIABLE REFERENCE (I.E. NAME) MAY REFER TO ONE OF SEVERAL DIFFERENT VARIABLES, ALL OF WHICH
	// HAVE THE SAME NAME, BUT A DIFFERENT SCOPE.  THE QUESTION IS "WHICH ONE IS BEING REFERRED TO?"
	// IN STATEFLOW, YOU CAN REFER TO ANY VARIABLE ANYWHERE IN THE STATE MACHINE, AND NEED ONLY USE SCOPE
	// (VIA NAMESPACE-DOT-NOTATION) TO DISAMBIGUATE THE REFERENCE.
	//
	// VarResolve FINDS WHICH VARIABLE IS BEING REFERRED TO BY CREATING IN INDEX INTO THE SCOPE TREE BASED
	// ON INDIVIDUAL SCOPE/VARIABLE NAMES.  THE INDEX MAPS THE NAME "FOO", FOR INSTANCE, TO A LIST OF
	// SMART-POINTER-TO-NODE, EACH OF WHICH POINTS TO A NODE OF VARIABLE OR SCOPE NAMED "FOO".
	//
	// SINCE THERE COULD BE SEVERAL SUCH NODES, ALL OF NAME "FOO", VarResolve THEN USES THE REMAINING NAMES
	// IN THE SCOPE-PATH OF THE VARIABLE TO CULL THIS LIST OF NODES UNTIL ONLY ONE IS LEFT.  FOR INSTANCE,
	// WITH "FOO/BAR/BOZ/...", VarResolve FIRST USES THE AFORE-MENTIONED INDEX TO GET A LIST OF
	// SMART-POINTER-TO-NODE AS BEFORE FOR "FOO", THEN ELIMINATES ANY OF THESE NODES THAT DO NOT HAVE A
	// CHILD SCOPE/VARIABLE OF NAME "BAR".  THEN IT ELIMINATES ALL OF THESE RESULTING NODES THAT DO NOT
	// HAVE A GRANDCHILD OF NAME "BOZ", AND SO ON.
	//
	// HOPEFULLY, YOU THEN END UP ONLY ONE NODE.  IF YOU END UP WITH MORE THAN ONE, YOU HAVE AN AMBIGUITY.
	// IF YOU END UP WITH NONE, YOU HAVE A BAD REFERENCE.
	ResMapSPList getResMapSPList( const ScopePath &scopePath );
	ScopeStructPairList getScopeStructPairList( const ScopePath &scopePath );
	static ScopeStructPairList cullScopeStructPairList(
	 const ScopeStructPairList &scopeStructPairList, const ScopePath &scopePath
	);

public:

	static ScopePath getScopePath( std::string scopePathString, std::string delim = "/" );
	static ScopePath getScopePath( const ResMapSP &resMapSP );

	VarResolve( void ) : _rootResMapSP( new ResMap ) { }

	void addScopePath( const ScopePath &scopePath, bool isLeaf = true );


	std::string resolveScope( const ScopePath &scopePath, const ScopePath &functionScopePath );

	void clear( void ) {
		_rootResMapSP = ResMapSP( new ResMap );
		_resMapSPLookup.clear();
	}

};

#endif
