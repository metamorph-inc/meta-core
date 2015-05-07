#include "RandomSelector.h"

#include <ctime>
#include <set>

#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/uniform_01.hpp>

using namespace std;
using namespace boost;

RandomSelector::RandomSelector() : _rng(time(0)) { }

long RandomSelector::chooseInt(long minint, long maxint)
{
	if ( minint == maxint ) return minint;

	uniform_smallint<> range(minint, maxint);
	variate_generator<mt19937 &, uniform_smallint<> > gen(_rng, range);
	return gen();
}

std::vector< long > RandomSelector::chooseInts(long minint, long maxint, unsigned long count)
{
	unsigned long idx;
	std::set< long > vals;
	
	while ( vals.size() < count )
	{
		vals.clear();
		for ( idx = 0; idx < count; idx++ )
		{
			vals.insert(chooseInt(minint, maxint));
		}
		// If we have any duplicates, then start again
	}
	
	std::vector< long > final(vals.begin(), vals.end());
	return final;

}

double RandomSelector::chooseDouble( double minreal, double maxreal )
{
	if ( minreal == maxreal ) return minreal;

	uniform_real<double> range(minreal, maxreal);
	variate_generator<mt19937 &, uniform_real<double> > gen(_rng, range);
	return gen();
}

double RandomSelector::chooseFromList( const std::vector< double > & numlist )
{
	return numlist[chooseInt(0,numlist.size()-1)];
}

bool RandomSelector::choose( double prob )
{
	if ( prob <= 0.0 ) return false;
	if ( prob >= 1.0 ) return true;

	uniform_real<double> range(0.0, 1.0);
	variate_generator< mt19937 &, uniform_real<double> > gen(_rng, range);

	return (gen() <= prob);
}