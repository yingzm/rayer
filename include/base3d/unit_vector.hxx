#ifndef __UNIT_VECTOR_HXX__
#define __UNIT_VECTOR_HXX__

#include "vector.hxx"
#include "tolerance.hxx"
#include "math.hxx"

BEGIN_NAMESPACE

class unit_vector3d
{
public:
	unit_vector3d();
	unit_vector3d(double x, double y, double z);

	double *get_comp();
	void set_comp(int i, double val);
	void set_comp(double x, double y, double z);
	double &operator[](int i);
	const double &operator[] (int i) const;

	const unit_vector3d &operator +=(const unit_vector3d &v);
	const unit_vector3d &operator -=(const unit_vector3d &v);
	const unit_vector3d &operator *=(const unit_vector3d &v);


	// friends
	friend unit_vector3d operator -(const unit_vector3d &v);
	friend bool operator ==(const unit_vector3d &v1, const unit_vector3d &v2);
	friend bool operator !=(const unit_vector3d &v1, const unit_vector3d &v2);
	friend unit_vector3d operator +(const unit_vector3d &v1, const unit_vector3d &v2);
	friend unit_vector3d operator -(const unit_vector3d &v1, const unit_vector3d &v2);
	friend unit_vector3d operator *(const unit_vector3d &v1, const unit_vector3d &v2);
	friend double operator %(const unit_vector3d &v1, const unit_vector3d &v2);

//	friend unit_vector3d min(const unit_vector3d &v1, const unit_vector3d &v2);
//	friend unit_vector3d max(const unit_vector3d &v1, const unit_vector3d &v2);
public:
	static const unit_vector3d zero;
	static const unit_vector3d unit_x;
	static const unit_vector3d unit_y;
	static const unit_vector3d unit_z;
private:
	double comp[3];

	double length() const;
	void normalize();
};

unit_vector3d operator -(const unit_vector3d &v);
bool operator ==(const unit_vector3d &v1, const unit_vector3d &v2);
bool operator !=(const unit_vector3d &v1, const unit_vector3d &v2);
unit_vector3d operator +(const unit_vector3d &v1, const unit_vector3d &v2);
unit_vector3d operator -(const unit_vector3d &v1, const unit_vector3d &v2);
unit_vector3d operator *(const unit_vector3d &v1, const unit_vector3d &v2);
double operator %(const unit_vector3d &v1, const unit_vector3d &v2);
unit_vector3d normalize(const vector3d &v);

bool eq(const unit_vector3d &v1, const unit_vector3d &v2, double tol=DOUBLE_ZERO_TOLERANCE);

bool is_parallel(const unit_vector3d &v1, const unit_vector3d &v2, double tol=DOUBLE_ZERO_TOLERANCE);
bool is_biparallel(const unit_vector3d &v1, const unit_vector3d &v2, double tol=DOUBLE_ZERO_TOLERANCE);
bool is_antiparallel(const unit_vector3d &v1, const unit_vector3d &v2, double tol=DOUBLE_ZERO_TOLERANCE);
bool is_perpendicular(const unit_vector3d &v1, const unit_vector3d &v2, double tol=DOUBLE_ZERO_TOLERANCE);

//unit_vector3d min(const unit_vector3d &v1, const unit_vector3d &v2);
//unit_vector3d max(const unit_vector3d &v1, const unit_vector3d &v2);


#include "unit_vector.inc"

END_NAMESPACE

#endif /* __UNIT_VECTOR_HXX__ */
