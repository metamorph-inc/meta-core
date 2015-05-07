#include "VarResolve.hpp"

VarResolve::ScopePath VarResolve::getScopePath( std::string scopePathString, std::string delim ) {

	ScopePath scopePath;

	const std::string::size_type delimLength = delim.length();

	while( !scopePathString.empty() ) {

		std::string::size_type delimPos = scopePathString.find( delim );
		scopePath.push_back(  scopePathString.substr( 0, delimPos )  );
		if ( delimPos != std::string::npos )	scopePathString = scopePathString.substr( delimPos + delimLength );
		else									scopePathString.clear();

	}

	return scopePath;
}

VarResolve::ScopePath VarResolve::getScopePath( const ResMapSP &resMapSP ) {

	ScopePath scopePath;

	ResMapSP curResMapSP = resMapSP;
	ResMap::iterator crhItr = curResMapSP->find( ".." );
	while( crhItr != curResMapSP->end() ) {
		ResMapSP parentResMapSP = resMapSPProxyCast( (*crhItr).second );

		ResMap::iterator prhItr;
		
		for( prhItr = parentResMapSP->begin() ; prhItr != parentResMapSP->end() ; ++prhItr ) {
			if (  resMapSPProxyCast( (*prhItr).second )  ==  curResMapSP  ) break;
		}

		scopePath.push_front( (*prhItr).first );

		curResMapSP = parentResMapSP;
		crhItr = curResMapSP->find( ".." );
	}

	return scopePath;
}


/* THIS METHOD SHOULD ONLY BE INVOKED FOR VARIABLE DEFINITIONS, NOT REFERENCES. */
/* THEREFORE, NO PROVISION FOR STRUCTURE MEMBER REFERENCES IS NEEDED.           */
void VarResolve::addScopePath( const ScopePath &scopePath, bool isLeaf ) {

	ResMapSP curResMapSP = _rootResMapSP;

	for( ScopePath curScopePath = scopePath ; !curScopePath.empty() ; curScopePath.pop_front() ) {

		const std::string &nodeName = curScopePath.front();

		ResMap &curResMap = *curResMapSP;
		ResMap::iterator rshItr = curResMap.find( nodeName );
		if ( rshItr == curResMap.end() ) {

			/* ADD A NEW NODE TO THE SCOPE TREE WITH "nodeName" AS ITS NAME */
			ResMapSP newResMapSP( new ResMap );
			curResMap[ nodeName ] = newResMapSP;

			/* ADD A PARENT REFERENCE TO THE NEW ResMap */
			(*newResMapSP)[ ".." ] = curResMapSP;

			/* ADD THE NEW NODE TO THE LIST OF NODES THAT START WITH "nameName" */
			_resMapSPLookup[ nodeName ].push_back( newResMapSP );

			curResMapSP = newResMapSP;
		} else {
			curResMapSP = resMapSPProxyCast( (*rshItr).second );
		}

		if (  curScopePath.size() == 1  &&  isLeaf  &&  (*curResMapSP).find( ".leaf." ) == (*curResMapSP).end()  ) {
			(*curResMapSP)[ ".leaf." ] = ResMapSP(  static_cast< ResMap * >( 0 )  );
		}

	}
}


std::string VarResolve::resolveScope( const ScopePath &scopePath, const ScopePath &functionScopePath ) {

	ScopeStructPairList scopeStructPairList = getScopeStructPairList( scopePath );
	if ( scopeStructPairList.empty() ) return "";
	
	if ( scopeStructPairList.size() > 1 ) scopeStructPairList =
	 cullScopeStructPairList( scopeStructPairList, functionScopePath );

	if ( scopeStructPairList.size() == 0 ) return "";
	if ( scopeStructPairList.size() > 1 ) return ".ambiguous.";

	ScopeStructPair curScopeStructPair = scopeStructPairList.back();

	ScopePath::iterator scpItr = curScopeStructPair.first.begin();
	std::string varName( *scpItr );
	while( ++scpItr != curScopeStructPair.first.end() ) varName += '_' + *scpItr;

	for( scpItr = curScopeStructPair.second.begin() ; scpItr != curScopeStructPair.second.end() ; (void)++scpItr ) {
		varName += '.' + *scpItr;
	}
	
	return varName;
}

/* GET FULLY-QUALIFIED NAME OF VARIABLE, AND SPLIT INTO QUALIFIED VARIABLE AND STRUCTURE MEMBER REFERENCES */
VarResolve::ScopeStructPairList VarResolve::getScopeStructPairList( const ScopePath &scopePath ) {

	ResMapSPList curResMapSPList;
	ScopeStructPairList scopeStructPairList;

	ScopePath::const_iterator scpItr = scopePath.begin();
	std::string nodeName = *scpItr;
	(void)++scpItr;

	{
		ResMapSPLookup::iterator rslItr = _resMapSPLookup.find( nodeName );
		if ( rslItr == _resMapSPLookup.end() ) return scopeStructPairList;

		curResMapSPList = (*rslItr).second;
	}

	for( ResMapSPList::iterator crlItr = curResMapSPList.begin() ; crlItr != curResMapSPList.end() ; (void)++crlItr ) {
		ResMapSP resMapSP = resMapSPProxyCast( *crlItr );
		ResMap::iterator rshItr = resMapSP->find( ".leaf." );
		if ( rshItr != resMapSP->end() ) {
			ScopePath curScopePath = getScopePath( resMapSP );
			ScopePath structPath( scpItr, scopePath.end() );
			scopeStructPairList.push_back(  ScopeStructPair( curScopePath, structPath )  );
		}
	}

	while( scpItr != scopePath.end() ) {

		nodeName = *scpItr;
		(void)++scpItr;

		ResMapSPList nxtResMapSPList;
		for( ResMapSPList::iterator crlItr = curResMapSPList.begin() ; crlItr != curResMapSPList.end() ; (void)++crlItr ) {
			ResMap::iterator rshItr = (*crlItr)->find( nodeName );
			if ( rshItr != (*crlItr)->end() ) {
				ResMapSP resMapSP = resMapSPProxyCast( (*rshItr).second );
				nxtResMapSPList.push_back( resMapSP );
				rshItr = resMapSP->find( ".leaf." );
				if ( rshItr != resMapSP->end() ) {
					ScopePath curScopePath = getScopePath( resMapSP );
					ScopePath structPath( scpItr, scopePath.end() );
					scopeStructPairList.push_back(  ScopeStructPair( curScopePath, structPath )  );
				}
			}
		}

		curResMapSPList = nxtResMapSPList;
		if ( curResMapSPList.empty() ) return scopeStructPairList;
	}

	return scopeStructPairList;
}


VarResolve::ScopeStructPairList VarResolve::cullScopeStructPairList(
 const ScopeStructPairList &scopeStructPairList, const ScopePath &scopePath
) {

	// NO POINT IN CULLING AN EMPTY LIST OR A LIST WITH ONLY ONE ScopePath
	if ( scopeStructPairList.size() <= 1 ) return scopeStructPairList;

	// TRAVERSE scopePathList, REMOVE ANY ScopePath's THAT AREN'T ON scopePath
	ScopeStructPairList curScopeStructPairList = scopeStructPairList;

	typedef std::list< ScopePath::iterator > ScopePathIteratorList;
	ScopePathIteratorList scopePathIteratorList;
	for( ScopeStructPairList::iterator splItr = curScopeStructPairList.begin() ; splItr != curScopeStructPairList.end() ; ++splItr ) {
		scopePathIteratorList.push_back( splItr->first.begin() );
	}

	ScopePath cullScopePath = scopePath;

	while( curScopeStructPairList.size() > 1 && !cullScopePath.empty() ) {

		ScopeStructPairList::iterator splItr = curScopeStructPairList.begin();
		ScopePathIteratorList::iterator silItr = scopePathIteratorList.begin();

		while( splItr != curScopeStructPairList.end() ) {

			if ( *silItr == (*splItr).first.end() || **silItr != cullScopePath.front() ) {

				ScopeStructPairList::iterator tmpSplItr = splItr;
				++splItr;
				curScopeStructPairList.erase( tmpSplItr );

				ScopePathIteratorList::iterator tmpSilItr = silItr;
				++silItr;
				scopePathIteratorList.erase( tmpSilItr );

			} else {

				++(*silItr);

				++splItr;
				++silItr;

			}

		}
		cullScopePath.pop_front();
	}

	// EMPTY LIST OR A LIST WITH ONLY ONE ScopePath NEEDS NO FURTHER CULLING
	if ( curScopeStructPairList.size() <= 1 ) return curScopeStructPairList;


	// TAKE THE SHORTEST ScopePath('S)
	ScopeStructPairList::iterator curSplItr = curScopeStructPairList.begin();
	ScopeStructPairList::iterator nxtSplItr = curSplItr; ++nxtSplItr;

	while( nxtSplItr != curScopeStructPairList.end() ) {
		if ( curSplItr->first.size() < nxtSplItr->first.size() ) {
			ScopeStructPairList::iterator tmpSplItr = nxtSplItr;
			++nxtSplItr;
			curScopeStructPairList.erase( tmpSplItr );
		} else {
			if ( curSplItr->first.size() > nxtSplItr->first.size() ) curScopeStructPairList.erase( curSplItr );
			curSplItr = nxtSplItr;
			++nxtSplItr;
		}
	}

	return curScopeStructPairList;
}
