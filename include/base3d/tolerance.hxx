#ifndef __TOLERANCE_HXX__
#define __TOLERANCE_HXX__

#include "math.hxx"

BEGIN_NAMESPACE

const double DOUBLE_ZERO_TOLERANCE=1e-12;
const double DOUBLE_TOLERANCE=DBL_EPSILON;
const double DOUBLE_SQRT_TOLERANCE = 1.1e-8;

const float FLOAT_ZERO_TOLERANCE=1e-6f;
const float FLOAT_TOLERANCE=FLT_EPSILON;
const float FLOAT_SQRT_TOLERANCE = 3.0e-4f;

/*
 *	I planned to use is_equal, less_than, less_equal, greater_than, greater_equal
 *	as the function names, but less_equal and greater_equal coincide with sgi STL
 *	library, and I have no good ways to solve it.
 */
bool eq(double d1, double d2, double tol=DOUBLE_ZERO_TOLERANCE);
bool lt(double d1, double d2, double tol=DOUBLE_ZERO_TOLERANCE);
bool le(double d1, double d2, double tol=DOUBLE_ZERO_TOLERANCE);
bool gt(double d1, double d2, double tol=DOUBLE_ZERO_TOLERANCE);
bool ge(double d1, double d2, double tol=DOUBLE_ZERO_TOLERANCE);

bool eq(float f1, float f2, float tol=FLOAT_ZERO_TOLERANCE);
bool lt(float f1, float f2, float tol=FLOAT_ZERO_TOLERANCE);
bool le(float f1, float f2, float tol=FLOAT_ZERO_TOLERANCE);
bool gt(float f1, float f2, float tol=FLOAT_ZERO_TOLERANCE);
bool ge(float f1, float f2, float tol=FLOAT_ZERO_TOLERANCE);

// inline functions
// double
inline bool eq(double d1, double d2, double tol)
{
	if ((d1-tol)<d2 && (d1+tol)>d2)
		return true;
	else
		return false;
}

inline bool lt(double d1, double d2, double tol)
{
	if (d1<d2-tol)
		return true;
	else
		return false;
}

inline bool le(double d1, double d2, double tol)
{
	return !gt(d1, d2, tol);
}

inline bool gt(double d1, double d2, double tol)
{
	if (d1>d2+tol)
		return true;
	else
		return false;
}

inline bool ge(double d1, double d2, double tol)
{
	return !lt(d1, d2, tol);
}


// float
inline bool eq(float f1, float f2, float tol)
{
	if ((f1-tol)<f2 && (f1+tol)>f2)
		return true;
	else
		return false;
}

inline bool lt(float f1, float f2, float tol)
{
	if (f1<f2-tol)
		return true;
	else
		return false;
}

inline bool le(float d1, float d2, float tol)
{
	return !gt(d1, d2, tol);
}

inline bool gt(float f1, float f2, float tol)
{
	if (f1>f2+tol)
		return true;
	else
		return false;
}

inline bool ge(float f1, float f2, float tol)
{
	return !lt(f1, f2, tol);
}

END_NAMESPACE

#endif /* __TOLERANCE_HXX__ */
