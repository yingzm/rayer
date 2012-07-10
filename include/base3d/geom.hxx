#ifndef __GEOM_HXX__
#define __GEOM_HXX__

#include "point.hxx"
#include "unit_vector.hxx"

double angle_between(const vector3d &v1, const vector3d &v2);

double triangle_area(const point3d &p1, const point3d &p2, const point3d &p3);

#define POLYGON_ALPHA_EPSILON 0.001

bool pt_in_triangle(double &c1, double &c2, double &c3,
		   const point3d &p1, const point3d &p2, const point3d &p3, 
		   const point3d &p);


inline double angle_between(const vector3d &v1, const vector3d &v2)
{
	double angle = acos(normalize(v1)%normalize(v2));

	return  angle;
}


#endif /* __GEOM_HXX__ */