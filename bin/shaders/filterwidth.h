/************************************************************************
 * filterwidth.h - Some handy macros for filter size estimation
 *                 for antialiasing of shaders.
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 *
 * $Revision: 1.3 $    $Date: 2000/11/14 05:53:41 $
 *
 ************************************************************************/



#ifndef FILTERWIDTH_H
#define FILTERWIDTH_H 1

/* Define metrics for estimating filter widths, if none has already
 * been defined.  This is crucial for antialiasing.
 */
#ifndef MINFILTWIDTH
#  define MINFILTWIDTH 1.0e-6
#endif


/* The filterwidth macro takes a float and returns the approximate 
 * amount that the float changes from pixel to adjacent pixel.
 */
#define filterwidth(x)  max (abs(Du(x)*du) + abs(Dv(x)*dv), MINFILTWIDTH)


/* The filterwidthp macro is similar to filterwidth, but is for 
 * point data.
 */
#define filterwidthp(p) max (sqrt(area(p)), MINFILTWIDTH)



/* Given a function g, its known average g_avg, its high frequency
 * feature size, and a filter width, fade to the average as we approach
 * the Nyquist limit.
 */
#define fadeout(g,g_avg,featuresize,fwidth) \
        mix (g, g_avg, smoothstep(.2,.6,fwidth/featuresize))


#endif /* FILTERWIDTH_H */
