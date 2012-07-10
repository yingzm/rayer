#ifndef __POINT_HXX__
#define __POINT_HXX__

#include "vector.hxx"
#include "tolerance.hxx"
#include "math.hxx"
#include "matrix.hxx"

BEGIN_NAMESPACE

class point4d;

class point3d
{
public:
	point3d();
	point3d(double x, double y, double z);
	point3d(const point4d &p);

	double x() const { return comp[0]; }
	double y() const { return comp[1]; }
	double z() const { return comp[2]; }

	double *get_comp() ;
	void set_comp(int i, double val);
	void set_comp(double x, double y, double z);
	double &operator[](int i);
	const double &operator[](int i) const;

	const point3d &operator +=(const vector3d &v);
	const point3d &operator *=(double k);
	const point3d &operator *=(const matrix4d &m);
	const point3d &operator /=(double k);

	// square distance from point to origin
	double length() const;
	// distance from point to origin
	double sqrlength() const;

	// friends
	friend point3d operator -(const point3d &v);
	friend bool operator ==(const point3d &v1, const point3d &v2);
	friend bool operator !=(const point3d &v1, const point3d &v2);
	friend point3d operator +(const point3d &v1, const point3d &v2);
	friend vector3d operator -(const point3d &v1, const point3d &v2);
	friend point3d operator *(const point3d &v1, double k);
	friend point3d operator *(double k, const point3d &v1);
	friend point3d operator /(const point3d &v1, double k);
	friend point3d operator+(const point3d &p, const vector3d &v);

	// @@ I want to use min and max as the name, but it coincide with
	// template min and max.
	friend point3d pmin(const point3d &v1, const point3d &v2);
	friend point3d pmax(const point3d &v1, const point3d &v2);
public:
	static const point3d zero;
	static const point3d unit_x;
	static const point3d unit_y;
	static const point3d unit_z;
	static const point3d minimum;
	static const point3d maximum;
private:
	double comp[3];
};

point3d operator -(const point3d &v);
bool operator ==(const point3d &v1, const point3d &v2);
bool operator !=(const point3d &v1, const point3d &v2);
vector3d operator -(const point3d &v1, const point3d &v2);
point3d operator *(const point3d &v1, double k);
point3d operator *(double k, const point3d &v1);
point3d operator /(const point3d &v1, double k);
point3d operator+(const point3d &p, const vector3d &v);
point3d pmin(const point3d &v1, const point3d &v2);
point3d pmax(const point3d &v1, const point3d &v2);


class point4d
{
public:
	point4d();
	point4d(double x, double y, double z, double w);
	point4d(const point3d &p);

	double *get_comp() ;
	double get_comp(int i) const;
	void set_comp(int i, double val);
	void set_comp(double x, double y, double z, double w);
	double &operator[](int i);
	const double &operator[](int i) const;


public:
	static const point4d zero;
	static const point4d unit_x;
	static const point4d unit_y;
	static const point4d unit_z;
	static const point4d minimum;
	static const point4d maximum;
private:
	double comp[4];
};

#include "point.inc"

END_NAMESPACE

#endif /* __POINT_HXX__ */
