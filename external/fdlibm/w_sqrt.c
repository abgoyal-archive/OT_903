
/* @(#)w_sqrt.c 1.3 95/01/18 */


#include "fdlibm.h"

#ifdef __STDC__
	double ieee_sqrt(double x)		/* wrapper sqrt */
#else
	double ieee_sqrt(x)			/* wrapper sqrt */
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_sqrt(x);
#else
	double z;
	z = __ieee754_sqrt(x);
	if(_LIB_VERSION == _IEEE_ || ieee_isnan(x)) return z;
	if(x<0.0) {
	    return __kernel_standard(x,x,26); /* ieee_sqrt(negative) */
	} else
	    return z;
#endif
}
