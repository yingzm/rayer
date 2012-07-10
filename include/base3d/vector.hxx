#ifndef __VECTOR_HXX__
#define __VECTOR_HXX__

#include "tolerance.hxx"
#include "math.hxx"
#include "matrix.hxx"

BEGIN_NAMESPACE

class vector3d
{
public:
	vector3d();
	vector3d(double x, double y, double z);

	double *get_comp();
	void set_comp(int i, double val);
	void set_comp(double x, double y, double z);
	double &operator[](int i);
	const double &operator[](int i) const;

	double x() const { return comp[0]; }
	double y() const { return comp[1]; }
	double z() const { return comp[2]; }

	const vector3d &operator +=(const vector3d &v);
	const vector3d &operator -=(const vector3d &v);
	const vector3d &operator *=(const vector3d &v);
	const vector3d &operator *=(double k);
	const vector3d &operator *=(const matrix4d &m);
	const vector3d &operator /=(double k);


	double length() const;
	double sqrlength() const;

	void normalize();

	// friends
	friend vector3d operator -(const vector3d &v);
	friend bool operator ==(const vector3d &v1, const vector3d &v2);
	friend bool operator !=(const vector3d &v1, const vector3d &v2);
	friend vector3d operator +(const vector3d &v1, const vector3d &v2);
	friend vector3d operator -(const vector3d &v1, const vector3d &v2);
	friend vector3d operator *(const vector3d &v1, const vector3d &v2);
	friend vector3d operator *(const vector3d &v1, double k);
	friend vector3d operator *(double k, const vector3d &v1);
	friend double operator %(const vector3d &v1, const vector3d &v2);
	friend vector3d operator /(const vector3d &v1, double k);

	friend vector3d vmin(const vector3d &v1, const vector3d &v2);
	friend vector3d vmax(const vector3d &v1, const vector3d &v2);
public:
	static const vector3d zero;
	static const vector3d unit_x;
	static const vector3d unit_y;
	static const vector3d unit_z;
	static const vector3d minimum;
	static const vector3d maximum;
private:
	double comp[3];
};

vector3d operator -(const vector3d &v);
bool operator ==(const vector3d &v1, const vector3d &v2);
bool operator !=(const vector3d &v1, const vector3d &v2);
vector3d operator +(const vector3d &v1, const vector3d &v2);
vector3d operator -(const vector3d &v1, const vector3d &v2);
vector3d operator *(const vector3d &v1, const vector3d &v2);
vector3d operator *(const vector3d &v1, double k);
vector3d operator *(double k, const vector3d &v1);
double operator %(const vector3d &v1, const vector3d &v2);
vector3d operator /(const vector3d &v1, double k);
/*
vector3d min(const vector3d &v1, const vector3d &v2);
vector3d max(const vector3d &v1, const vector3d &v2);
*/

bool eq(const vector3d &v1, const vector3d &v2, double tol=DOUBLE_ZERO_TOLERANCE);

bool is_parallel(const vector3d &v1, const vector3d &v2, double tol=DOUBLE_ZERO_TOLERANCE);
bool is_antiparallel(const vector3d &v1, const vector3d &v2, double tol=DOUBLE_ZERO_TOLERANCE);
bool is_biparallel(const vector3d &v1, const vector3d &v2, double tol=DOUBLE_ZERO_TOLERANCE);
bool is_perpendicular(const vector3d &v1, const vector3d &v2, double tol=DOUBLE_ZERO_TOLERANCE);

#include "vector.inc"

END_NAMESPACE

#endif /* __VECTOR_HXX__ */
