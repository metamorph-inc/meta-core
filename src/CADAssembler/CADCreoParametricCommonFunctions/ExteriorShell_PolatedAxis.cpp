#include "ExteriorShell.h"

namespace isis {
	namespace hydrostatic {

		/**
		a function which determines if the subject value is in the axis range.
		*/
		bool PolatedAxis::isInRange(const double subject) const {
			if (m_axis.empty()) return false;
			if (subject < m_axis.front()) return false;
			if (m_axis.back() < subject) return false;
			return true;
		}
		/**
		returns the lower index and the interpolation ratio.
		*/
		inline PolatedAxis::inter_type locate(const double subject, const axis_type axis) {
			if (axis.empty()) throw  false;
			int below = 0;
			int above = (int)axis.size() - 1;
			if (subject == axis[above]) {
				return ::boost::make_tuple(::boost::make_tuple(above, above), 0.0);
			}
			while( below + 1 < above ) {
				const int split = std::div(below + above, 2).quot;
				const double candidate = axis[split];
				if (subject == candidate) {
					return ::boost::make_tuple(::boost::make_tuple(split,split), 0.0);
				}
				if (subject < candidate) {
					above = split;
				} else {
					below = split;
				}
			}
			const double ratio = (subject - axis[below])/(axis[above] - axis[below]);
			return ::boost::make_tuple(::boost::make_tuple(below, above), ratio);
		}
		/** 
		returns a tuple (index, ratio) over the axis
		*/
		PolatedAxis::inter_type PolatedAxis::interpolate(const double key) const {
			return locate(key, m_axis);
		}
		/** 
		returns a tuple (index, ratio) over all axies
		*/
		PolatedAxis::extra_type PolatedAxis::extrapolate(const double key) const {
			return locate(key, m_axis);
		}

	
	} // namespace hydrostatic 
} // namespace isis 