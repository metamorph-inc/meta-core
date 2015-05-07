/// Implementations for Constraint System class

#include "ConstraintSys.h"

#include "stdafx.h"
#include <sstream>

// Inputs: constraint problem instance and variable map (instance names -> constraint variable index)
void ConstraintSys::CreateEqualityConstraints( const ConstraintProblem & cp, map< string, int > & vmap )
{
	set< EqualityConstraint > eqcstrs = cp.EqualityConstraint_kind_children();
	for( set< EqualityConstraint >::iterator pEqCstr = eqcstrs.begin(); pEqCstr != eqcstrs.end(); pEqCstr++ )
	{
		vector< string > names;

		set< XPort > xps = pEqCstr->XPort_kind_children();
		vector< int > xindices;
		for ( set< XPort >::iterator pX = xps.begin(); pX != xps.end(); pX++ )
		{
			//pA->GetAdjacentObjects( CSched::meta_FDVar );
			//vector<Udm::ObjectImpl*> sw_ois = s_bulb_i->__impl()->getAssociation(LampDiagram::Bulb::meta_dst, Udm::TARGETFROMPEER);
			//cout << " number of peers(meta_dst) :" << (long) sw_ois.size() << endl;
			//Switch peer_switch(*sw_ois.begin());
			vector<Udm::ObjectImpl*> cvars = pX->__impl()->getAssociation( CSched::XPort::meta_srcMember );
			// We'll constrain this to one connection per port at some point
			// TODO: Add constraint for that
			FDVar v( *cvars.begin() );

			xindices.push_back( vmap[ v.name() ]);
			ostringstream tmpstr;
			tmpstr << v.name() << " " << vmap[ v.name() ];
			names.push_back( tmpstr.str().c_str() );
		}

		set< YPort > yps = pEqCstr->YPort_kind_children();
		vector< int > yindices;
		for ( set< YPort >::iterator pY = yps.begin(); pY != yps.end(); pY++ )
		{
			vector<Udm::ObjectImpl*> cvars = pY->__impl()->getAssociation( CSched::YPort::meta_srcMember );
			// We'll constrain this to one connection per port at some point
			// TODO: Add constraint for that
			FDVar v( *cvars.begin() );

			yindices.push_back( vmap[ v.name() ] );
			ostringstream tmpstr;
			tmpstr << v.name() << " " << vmap[ v.name() ];
			names.push_back( tmpstr.str().c_str() );
		}

		/*ostringstream outStr;
		outStr << "Creating equality constraint for " << endl;
		for ( vector< string >::iterator pName = names.begin(); pName != names.end(); pName++ )
		{
			outStr << *pName << endl;
		}
		AfxMessageBox( outStr.str().c_str() );*/
		if ( xindices.size() == yindices.size() && xindices.size() > 0 )
			AddEqConstraint(xindices, yindices, (const int) pEqCstr->Offset() );
	}
}

// Inputs: constraint problem instance and variable map (instance names -> constraint variable index)
void ConstraintSys::CreateInequalityConstraints( const ConstraintProblem & cp, map< string, int > & vmap )
{
	set< InequalityConstraint > ineqcstrs = cp.InequalityConstraint_kind_children();
	for( set< InequalityConstraint >::iterator pIneqCstr = ineqcstrs.begin(); pIneqCstr != ineqcstrs.end(); pIneqCstr++ )
	{

		set< XPort > xps = pIneqCstr->XPort_kind_children();
		vector< int > xindices;
		for ( set< XPort >::iterator pX = xps.begin(); pX != xps.end(); pX++ )
		{
			//pA->GetAdjacentObjects( CSched::meta_FDVar );
			//vector<Udm::ObjectImpl*> sw_ois = s_bulb_i->__impl()->getAssociation(LampDiagram::Bulb::meta_dst, Udm::TARGETFROMPEER);
			//cout << " number of peers(meta_dst) :" << (long) sw_ois.size() << endl;
			//Switch peer_switch(*sw_ois.begin());
			vector<Udm::ObjectImpl*> cvars = pX->__impl()->getAssociation( CSched::XPort::meta_srcMember );
			// We'll constrain this to one connection per port at some point
			// TODO: Add constraint for that
			FDVar v( *cvars.begin() );

			xindices.push_back( vmap[ v.name() ]);
		}

		set< YPort > yps = pIneqCstr->YPort_kind_children();
		vector< int > yindices;
		for ( set< YPort >::iterator pY = yps.begin(); pY != yps.end(); pY++ )
		{
			vector<Udm::ObjectImpl*> cvars = pY->__impl()->getAssociation( CSched::YPort::meta_srcMember );
			// We'll constrain this to one connection per port at some point
			// TODO: Add constraint for that
			FDVar v( *cvars.begin() );

			yindices.push_back( vmap[ v.name() ] );
		}

		AddIneqConstraint( xindices, yindices, (const int) pIneqCstr->Offset() );
	}
}

// Inputs: constraint problem instance and variable map (instance names -> constraint variable index)
void ConstraintSys::CreateRangeConstraints( const ConstraintProblem & cp, map< string, int > & vmap )
{
	// Create local constraints for individual variables
	set< FDVar > vars = cp.FDVar_kind_children();
	for ( set< FDVar >::iterator pV = vars.begin(); pV != vars.end(); pV++ )
	{
		AddRangeConstraint( vmap[ pV->name() ], (unsigned int) pV->LowerBound(), 
			                (unsigned int) pV->UpperBound() );

		// Interval size constraint
		if ( pV->Size() > 0 )
		{
			AddRangeConstraint( vmap[ pV->name() ], (unsigned int) cp.LowerBound(),
							(unsigned int) cp.UpperBound() - (pV->Size()-1) );
		}
	}

	set< RangeConstraint > rcstrs = cp.RangeConstraint_kind_children();
	for( set< RangeConstraint >::iterator pRCstr = rcstrs.begin(); pRCstr != rcstrs.end(); pRCstr++ )
	{
		vector<Udm::ObjectImpl*> cvars = pRCstr->__impl()->getAssociation( CSched::RangeConstraint::meta_srcMember );
		
		// TODO: Just one for now
		FDVar v( *cvars.begin() );

		// Get the max of the lower bounds
		unsigned int CstrObjLbound = (unsigned int) pRCstr->LowerBound();
		unsigned int VarObjLbound = (unsigned int) v.LowerBound();
		unsigned int Lbound = (unsigned int) (CstrObjLbound > VarObjLbound) ? CstrObjLbound : VarObjLbound;

		// Get the min of the upper bounds (include the size)
		unsigned int CstrObjUbound = (unsigned int) pRCstr->UpperBound() - v.Size();
		unsigned int VarObjUbound = (unsigned int) v.UpperBound() - v.Size();
		unsigned int Ubound = (unsigned int) (CstrObjUbound < VarObjUbound) ? CstrObjUbound : VarObjUbound;		

		AddRangeConstraint( vmap[ v.name() ], Lbound, Ubound );
	}
	
}

// Inputs: constraint problem instance and variable map (instance names -> constraint variable index)
void ConstraintSys::CreateSerialConstraints( const ConstraintProblem & cp, map< string, int > & vmap )
{
	set< SerialConstraint > sercstrs = cp.SerialConstraint_kind_children();
	for ( set< SerialConstraint >::iterator pSerCstr = sercstrs.begin(); pSerCstr != sercstrs.end(); pSerCstr++ )
	{
		vector<Udm::ObjectImpl*> cvars = pSerCstr->__impl()->getAssociation( CSched::SerialConstraint::meta_srcMember );

		vector< pair< int, int > > varlist;

		// Make an entry for everything that's connected to this constraint
		vector<Udm::ObjectImpl*>::iterator pEnds;
		for ( pEnds = cvars.begin(); pEnds != cvars.end(); pEnds++ )
		{
			FDVar v( *pEnds );
			varlist.push_back( make_pair( vmap[ v.name() ], (unsigned int) v.Size() ) ); 
		}
		
		AddSerialConstraint( varlist );
	}
}
