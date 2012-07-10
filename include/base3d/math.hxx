#ifndef __MATH_HXX__
#define __MATH_HXX__

/** TODO: using climits */
#include <float.h>	

BEGIN_NAMESPACE

#ifndef M_PI
#define M_PI	3.1415926535898
#endif

/*
#define DBL_EPSILON	2.2204460492503131e-016
#define DBL_MAX		1.7976931348623158e+308
#define FLT_EPSILON	1.192092896e-07f
*/

#define DBL_INFINITE	DBL_MAX

double rad2deg(double r);
double deg2rad(double r);

double clamp(double x, double minx, double maxx);

double angle_normalize(double angle);

/**
 * solve quadric equation a*x*x+b*x+c=0
 * Input: a,b,c are corresponding constants, a must NOT be zero
 *	  t1, and t2 are two solution
 * Output: the number of solution
 */
int solve_quadric(double a, double b, double c, double t[2]);

int solve_quartic(double a, double b, double c, double d, double e, double rts[4], double rterr[4]);

double noise(double t);
double noise(double t0, double t1);
double noise(double t0, double t1, double t2);
double noise(double t0, double t1, double t2, double t3);

double spline_catmullrom(double value, int n, double *pts);
double spline_bezier(double value, int n, double *pts);
double spline_bspline(double value, int n, double *pts);
double spline_hermite(double value, int n, double *pts);
double spline_linear(double value, int n, double *pts);

#include "math.inc"

END_NAMESPACE

#endif /* __MATH_HXX__ */

