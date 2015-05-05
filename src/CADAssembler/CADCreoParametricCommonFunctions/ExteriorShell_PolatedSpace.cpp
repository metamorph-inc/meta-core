#include "ExteriorShell.h"

namespace isis {
namespace hydrostatic {

/** 
returns a tuple indicating if the values are in the ranges specified by the axis.
*/
::boost::tuple<bool,bool,bool> PolatedSpace::isInRange(const double depth, const double heel, const double trim) {
	return ::boost::make_tuple(m_depth_axis.isInRange(depth),
								m_heel_axis.isInRange(heel),
								m_trim_axis.isInRange(trim) );
}
/** 
f(x,y,z) = [f(Q111)(x2-x)(y2-y)(z2-z)
			f(Q112)(x2-x)(y2-y)(z-z1)
			...
			f(Q221)(x-x1)(y-y1)(z2-z)
			f(Q221)(x-x1)(y-y1)(z-z1)]
			/ [(x2-x1)(y2-y1)(z2-z1)]

In all the cases of interest here X2-X1 = 1 and X-X1 = XR (& X2-X = 1-XR)
*/
::std::vector< const weighted_index_type > generateWeightedIndexVector( 
		const PolatedAxis::inter_type it_0, 
		const PolatedAxis::inter_type it_1, 
		const PolatedAxis::inter_type it_2) 
{
	const int ir_0_0 = it_0.get<0>().get<0>();
	const int ir_0_1 = it_0.get<0>().get<1>();

	const int ir_1_0 = it_1.get<0>().get<0>();
	const int ir_1_1 = it_1.get<0>().get<1>();

	const int ir_2_0 = it_2.get<0>().get<0>();
	const int ir_2_1 = it_2.get<0>().get<1>();

	const double v_0 = it_0.get<1>();
	const double w_0 = 1.0 - v_0;
		
	const double v_1 = it_1.get<1>();
	const double w_1 = 1.0 - v_1;

	const double v_2 = it_2.get<1>();
	const double w_2 = 1.0 - v_2;

	::std::vector< const weighted_index_type > result;
	result.push_back( ::boost::make_tuple(ir_0_0, ir_1_0, ir_2_0, v_0 + v_1 + v_2) );
	result.push_back( ::boost::make_tuple(ir_0_0, ir_1_0, ir_2_1, v_0 + v_1 + w_2) );
	result.push_back( ::boost::make_tuple(ir_0_0, ir_1_1, ir_2_0, v_0 + w_1 + v_2) );
	result.push_back( ::boost::make_tuple(ir_0_0, ir_1_1, ir_2_1, v_0 + w_1 + w_2) );
	result.push_back( ::boost::make_tuple(ir_0_1, ir_1_0, ir_2_0, w_0 + v_1 + v_2) );
	result.push_back( ::boost::make_tuple(ir_0_1, ir_1_0, ir_2_1, w_0 + v_1 + w_2) );
	result.push_back( ::boost::make_tuple(ir_0_1, ir_1_1, ir_2_0, w_0 + w_1 + v_2) );
	result.push_back( ::boost::make_tuple(ir_0_1, ir_1_1, ir_2_1, w_0 + w_1 + w_2) );
	return result;
};

/**
Accumulate the weighted results.
- center-of-hydrostatic vectors.
- water-line depth.
*/
struct ResultAverage {
	ResultAverage( PolatedSpace::array_result_type marray_result )
		: m_marray_result(marray_result) 
	{ }
	void operator()( weighted_index_type weight) { 
		Result wip = m_marray_result[weight.get<0>()][weight.get<1>()][weight.get<2>()];
		const double ratio = weight.get<3>();
		{
			cob_type cob_wip = wip.cob;
			m_value.cob += (cob_wip * ratio);
		}
		{
			depth_type depth_wip = wip.depth;
			m_value.depth += (depth_wip * ratio);
		}
	}
 
	const PolatedSpace::array_result_type m_marray_result;
	Result m_value;
};


/**
returns a tuple indicating if the values are in the ranges specified by the axis.
*/
Result PolatedSpace::interpolate(const double in_depth, const double in_heel, const double in_trim) {
	const PolatedAxis::inter_type depth_ix = m_depth_axis.interpolate(in_depth);
	const PolatedAxis::inter_type heel_ix = m_heel_axis.interpolate(in_heel);
	const PolatedAxis::inter_type trim_ix = m_trim_axis.interpolate(in_trim);

	::std::vector< const weighted_index_type > index_list = generateWeightedIndexVector( depth_ix, heel_ix, trim_ix );
	ResultAverage result = for_each(index_list.begin(), index_list.end(), ResultAverage(m_marray_result));
	return result.m_value;
}
		
/**
returns a tuple indicating if the values are in the ranges specified by the axis.
*/
Result PolatedSpace::extrapolate(const double displacement, const double heel, const double trim) {
	return interpolate( displacement, heel, trim );
}

std::ostream& PolatedSpace::print(std::ostream& out_stream) const {
	std::vector<double>::const_iterator heel_it;
	int heel_ix = 0;
	for (heel_it = m_heel_axis.begin();  heel_it != m_heel_axis.end(); ++heel_it, ++heel_ix) {

		std::vector<double>::const_iterator trim_it;
		int trim_ix = 0;
		for (trim_it = m_trim_axis.begin();  trim_it != m_trim_axis.end(); ++trim_it, ++trim_ix) {

			std::vector<double>::const_iterator displ_it;
			int displ_ix = 0;
			for (displ_it = m_depth_axis.begin();  
				displ_it != m_depth_axis.end(); ++displ_it, ++displ_ix)
			{
				out_stream 
					<< "      [" << '\t' << "heel" << " : " << '\t' << "trim" << " : " << '\t' << "displacement" << "] " << '\n'
					<< "index [" << '\t' << heel_ix << " : " << '\t' << trim_ix << " : " << '\t' << displ_ix << "] " << '\n'
					<< "value [" << '\t' << *heel_it << " : " << '\t' << *trim_it << " : " << '\t' << *displ_it << "] " << std::endl;
				
				out_stream 
					<< "cob: " << m_marray_result[heel_ix][trim_ix][displ_ix].cob << '\n'
					<< "depth: " << m_marray_result[heel_ix][trim_ix][displ_ix].depth << '\n'
					<< std::endl;
			}
		}
	}
	return out_stream;
}

std::ostream& operator<< (std::ostream& out_stream, const PolatedSpace& in_hydro) {
	in_hydro.print(out_stream);
	return out_stream;
}


} // namespace hydrostatic 
} // namespace isis 