#ifndef __CONSTRAINTMGR_H_INCL__
#define __CONSTRAINTMGR_H_INCL__

#include <map>

#include "MiscUtil.h"
#include "IndexMgr.h"
#include "CSched.h"

/**
 * TODO: Add class description
 * 
 * @author   jporter
 */
class ConstraintMgr
{
private:

public:
    // Constructor
    ConstraintMgr( Udm::DataNetwork* p_backend, TTSystem & sys );
    ConstraintMgr( ConstraintProblem & cp );
	void SetInstanceModelRef( InstanceModel & im ); 

    // Destructor
    virtual ~ConstraintMgr();

    // I hope we made sure all the instance names were unique ;-)
	void AddTaskInstance( const std::string & tskName, int wcet, int upper_bound );
    void AddMsgInstance( const std::string & msgName, int dur, int upper_bound );

    // Constraint of the form xInst = yInst + offset
    void AddEqualityConstraint( const std::string & firstInst, const std::string & secondInst, const int offset );

    // Constraint of the form ltInst <= gtInst + offset
    void AddInequalityConstraint( const std::string & ltInst, const std::string & gtInst, const int offset );

    // Global constraint for serializing instances
	void AddSerialConstraint( const std::string & usedFor, std::vector< std::string > & instNames );

    // Solve the constraint problem
	bool SolveConstraints( std::string & stats = std::string("nostats"), int maxsln = 10 );

    // Output Functions
	void UpdateSolutions();
    bool GetSolutionNames( std::vector< string > & solutions );

    // One for Matlab
    void DrawSchedule( const std::string & prefix, const std::string & sln_name );

    // One for Text files
    void WriteSchedule( const std::string & prefix, const std::string & sln_name );

    // One for instance model
    void UpdateInstanceModel( const std::string & sln_name );

    // One for building dependencies for second stage
    // TODO: figure out what needs to be done

protected:
    std::map< std::string, FDVar >      _inst_varmap;
    std::map< std::string, int >        _inst_nummap;
    ConstraintProblem                   _cp;
    int                                 _varcount;
    std::map< std::string, Solution >   _solutions;

    // Copy constructor
    // TODO: Uncomment the copy constructor when you need it.
    //ConstraintMgr(const ConstraintMgr& src);

    // Assignment operator
    // TODO: Uncomment the assignment operator when you need it.
    //ConstraintMgr& operator=(const ConstraintMgr& src);
};

// Constructor implementation
// inline ConstraintMgr::ConstraintMgr( Udm::DataNetwork* p_backend, TTSystem & sys ) : _varcount(0)
// {
//     /// Create the proper constraints
//     RootFolder rootObj = RootFolder::Cast(p_backend->GetRootObject());
//
//     // First make a constraint model
//     _cp = ConstraintProblem::Create( rootObj );
//     _cp.name() = string(sys.name()) + "Constraints";
// }
//
// inline ConstraintMgr::ConstraintMgr( ConstraintProblem & cp ) : _varcount(0)
// {
//     _cp = cp;
//
//     std::set< FDVar > vars = _cp.FDVar_children();
//     std::set< FDVar >::iterator pVar;
//     for ( pVar = vars.begin(); pVar != vars.end(); pVar++ )
//     {
//         _inst_varmap[ pVar->name() ] = fdv;
//         _inst_nummap[ pVar->name() ] = _varcount++;
//     }
// }

inline void ConstraintMgr::SetInstanceModelRef( InstanceModel & im )
{
    // Save a reference to the instance model that will be used to create these constraints
    // We'll need it later
    InstanceRef ir = InstanceRef::Create( _cp );
    ir.ref() = im;
    ir.name() = im.name();

    
}

// Destructor implementation
inline ConstraintMgr::~ConstraintMgr()
{
}

// TODO: Uncomment the copy constructor when you need it.
//inline ConstraintMgr::ConstraintMgr(const ConstraintMgr& src)
//{
//   // TODO: copy
//}

// TODO: Uncomment the assignment operator when you need it.
//inline ConstraintMgr& ConstraintMgr::operator=(const ConstraintMgr& rhs)
//{
//   if (this == &rhs) {
//      return *this;
//   }
//
//   // TODO: assignment
//
//   return *this;
//}

#endif // __CONSTRAINTMGR_H_INCL__

