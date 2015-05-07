#include "MiscUtil.h"

// LCM Utility function
int MiscUtil::LCM( int a, int b )
{
    int r, atmp = a, btmp = b;
	// First find the GCD
	while( btmp > 0 )
	{
		r = atmp;
        atmp = btmp;
		btmp = r % btmp;
	}

	// Then use it to get the LCM
	return ( a * b ) / atmp;
}

// Stolen directly from Stroustrop himself 
// http://www.research.att.com/~bs/bs_faq2.html#int-to-string
std::string MiscUtil::itos(int i)	// convert int to string
{
	std::stringstream s;
	s << i;
	return s.str();
}