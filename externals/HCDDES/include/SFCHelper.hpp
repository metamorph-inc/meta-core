#ifndef SFC_HELPER_
#define SFC_HELPER_

#include <vector>
#include <set>

namespace SFC {

	struct StatementSorter {
		bool operator()( const SFC::Statement &s1, const SFC::Statement &s2 ) const {
			return static_cast< int >( s1.statementIndex() ) < static_cast< int >( s2.statementIndex() );
		}
	};

	typedef std::set< SFC::Declaration, StatementSorter > DeclarationSet;

	struct ArgSorter {
		bool operator()( const SFC::Arg &a1, const SFC::Arg &a2 ) const {
			return static_cast< int >( a1.argIndex() ) < static_cast< int >( a2.argIndex() );
		}
	};

	typedef std::set< SFC::Arg, ArgSorter > ArgSet;

	typedef std::set< SFC::Struct > StructSet;

	typedef std::vector< SFC::LocalVar > LocalVarVector;
}
#endif
