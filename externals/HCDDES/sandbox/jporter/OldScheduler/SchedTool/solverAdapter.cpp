
#include "solverAdapter.h"
#include "cpSolver.h"

SolverAdapter::SolverAdapter(void)
{
}

SolverAdapter::~SolverAdapter(void)
{
}

SolverAdapter * SolverAdapter::GetSolver( std::string & solvername )
{
	if ( solvername == "Gecode" || solvername == "CP" || solvername == "Finite domain" || solvername == "FD" )
		return (SolverAdapter *) new CPSolver();

	if (solvername == "FDCO")
		return (SolverAdapter *) new CPSolver(true);

	return NULL;
}

