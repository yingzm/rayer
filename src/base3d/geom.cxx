#include "common.hxx"
#include "geom.hxx"


double triangle_area(const point3d &p1, const point3d &p2, const point3d &p3)
{
	vector3d v1, v2, a1;
	v1 = p2-p1;
	v2 = p3-p1;
	a1 = v1*v2;

	return a1.length()/2;
}

bool pt_in_triangle(double &c1, double &c2, double &c3,
		   const point3d &p1, const point3d &p2, const point3d &p3, 
		   const point3d &p)
{
	vector3d v1, v2, v3;
	v1 = p1-p;
	v2 = p2-p;
	v3 = p3-p;
	vector3d n1(v1), n2(v2), n3(v3);
	n1.normalize(); n2.normalize(); n3.normalize();
	
	double alpha=0;
	alpha += acos(n1%n2);
	alpha += acos(n2%n3);
	alpha += acos(n3%n1);
	if (fabs(alpha-2*M_PI)>=POLYGON_ALPHA_EPSILON)
		return false;
	
	vector3d ar;
	double a1, a2, a3;
	ar = v2*v3;
	a1 = ar.length()/2;
	ar = v3*v1;
	a2 = ar.length()/2;
	ar = v1*v2;
	a3 = ar.length()/2;

	double area = a1+a2+a3;
	c1 = a1/area; c2 = a2/area; c3 = a3/area;

	return true;
}
