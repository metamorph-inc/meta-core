
#include "ExteriorShell.h"

namespace isis {
	namespace buoyancy {
		void ExteriorShelladdDisplacement( const double displacement );
			void addHeelAngle( const double heel_angle );
			void addTrimAngle( const double trim_angle );

		Buoyancy ExteriorShell::computeBuoyancy();

	} // namespace buoyancy
} // namespace isis
