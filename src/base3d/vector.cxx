#include "common.hxx"

#include "vector.hxx"
#include "unit_vector.hxx"

BEGIN_NAMESPACE

const vector3d vector3d::zero(0.0, 0.0, 0.0);
const vector3d vector3d::unit_x(1.0, 0.0, 0.0);
const vector3d vector3d::unit_y(0.0, 1.0, 0.0);
const vector3d vector3d::unit_z(0.0, 0.0, 1.0);
const vector3d vector3d::minimum(-DBL_MAX, -DBL_MAX, -DBL_MAX);
const vector3d vector3d::maximum(DBL_MAX, DBL_MAX, DBL_MAX);

void vector3d::normalize()
{
	double len = length();
	if (eq(len, 0.0)) {
		comp[0] = comp[1] = comp[2] = 0.0;
	}
	else {
		comp[0]/=len; comp[1]/=len; comp[2]/=len;
	}
}

bool is_parallel(const vector3d &v1, const vector3d &v2, double tol)
{
	return (is_parallel(normalize(v1), normalize(v2), tol));
}

bool is_antiparallel(const vector3d &v1, const vector3d &v2, double tol)
{
	return is_antiparallel(normalize(v1), normalize(v2), tol);
}

bool is_biparallel(const vector3d &v1, const vector3d &v2, double tol)
{
	return (is_biparallel(normalize(v1), normalize(v2), tol));
}

bool is_perpendicular(const vector3d &v1, const vector3d &v2, double tol)
{
	return is_perpendicular(normalize(v1), normalize(v2), tol);
}



END_NAMESPACE