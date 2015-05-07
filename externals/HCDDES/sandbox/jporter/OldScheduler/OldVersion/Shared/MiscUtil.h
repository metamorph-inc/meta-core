#ifndef _MISC_UTIL_H_
#define _MISC_UTIL_H_

#include <sstream>

namespace MiscUtil
{
// LCM Utility function
int LCM( int a, int b );

// Stolen directly from Stroustrop himself 
// http://www.research.att.com/~bs/bs_faq2.html#int-to-string
std::string itos(int i);	// convert int to string

};

#endif // _MISC_UTIL_H_
