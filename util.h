/* SM Util 
* General utility
*/ 

#ifndef ___SM_LIB_UTIL
#define ___SM_LIB_UTIL

#include <cmath>

namespace util {
static double lerp(const double a, const double b, double t){ return a + (b - a) * t; }
};


#endif
