#include "common.hxx"

#include "point.hxx"

BEGIN_NAMESPACE

const point3d point3d::zero(0.0, 0.0, 0.0);
const point3d point3d::unit_x(1.0, 0.0, 0.0);
const point3d point3d::unit_y(0.0, 1.0, 0.0);
const point3d point3d::unit_z(0.0, 0.0, 1.0);
const point3d point3d::minimum(-DBL_MAX, -DBL_MAX, -DBL_MAX);
const point3d point3d::maximum(DBL_MAX, DBL_MAX, DBL_MAX);

const point4d point4d::zero(0.0, 0.0, 0.0, 1.0);
const point4d point4d::unit_x(1.0, 0.0, 0.0, 1.0);
const point4d point4d::unit_y(0.0, 1.0, 0.0, 1.0);
const point4d point4d::unit_z(0.0, 0.0, 1.0, 1.0);
const point4d point4d::minimum(-DBL_MAX, -DBL_MAX, -DBL_MAX, 1.0);
const point4d point4d::maximum(DBL_MAX, DBL_MAX, DBL_MAX, 1.0);
/*
point3d operator *(const point3d &p, const matrix4d &m)
{
	double x, y, z;
	double x0, y0, z0, w0;

	x = p.x(); y = p.y(); z = p.z();

	x0 = x*m[0][0]+y*m[0][1]+z*m[0][2]+m[0][3];
	y0 = x*m[1][0]+y*m[1][1]+z*m[1][2]+m[1][3];
	z0 = x*m[2][0]+y*m[2][1]+z*m[2][2]+m[2][3];
	w0 = x*m[3][0]+y*m[3][1]+z*m[3][2]+m[3][3];

	if (eq(w0, 0)) {
		x0 = y0 = z0 = DBL_MAX;
	}

	if (eq(w0, 1)) 
		return point3d(x0, y0, z0);
	else
		return point3d(x0/w0, y0/w0, z0/w0);
}
*/

END_NAMESPACE