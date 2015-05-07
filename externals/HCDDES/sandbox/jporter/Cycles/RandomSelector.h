
#ifndef _RANDOM_SELECTOR_H
#define _RANDOM_SELECTOR_H

#include <vector>
#include <boost/random/mersenne_twister.hpp>

class RandomSelector
{
public:
	RandomSelector(); 
	virtual ~RandomSelector() { }

	long chooseInt(long minint, long maxint);
	std::vector< long > chooseInts(long minint, long maxint, unsigned long count);
	double chooseDouble( double minreal, double maxreal );
	double chooseFromList( const std::vector< double > & numlist );
	bool choose( double prob );

protected:
	boost::mt19937 _rng;
};

#endif // _RANDOM_SELECTOR_H