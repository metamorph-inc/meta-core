#ifndef ESMOLHELPER_HPP_
#define ESMOLHELPER_HPP_

#include "SLSF.h"

	template< typename PORTTYPE >
	struct PortSorter {
		bool operator()( const PORTTYPE &port1, const PORTTYPE &port2 ) const {
			return static_cast< int >( port1.Number() ) < static_cast< int >( port2.Number() );
		}
	};

	typedef PortSorter< SLSF::InputPort > InputPortSorter;
	typedef PortSorter< SLSF::OutputPort > OutputPortSorter;

	typedef std::set< SLSF::InputPort, InputPortSorter > InputPortSet;
	typedef std::set< SLSF::OutputPort, OutputPortSorter > OutputPortSet;

	typedef std::set< SLSF::Parameter > ParameterSet;

#endif