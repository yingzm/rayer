inline unit_vector3d::unit_vector3d()
{
	// EMPTY
}

inline unit_vector3d::unit_vector3d(double x, double y, double z)
{
	comp[0]=x; comp[1]=y; comp[2]=z;
	normalize();
}

inline double *unit_vector3d::get_comp()
{
	return comp;
}

inline void unit_vector3d::set_comp(int i, double val)
{
	ASSERT(i>=0 && i<3);
	comp[i] = val;
}

inline void unit_vector3d::set_comp(double x, double y, double z)
{
	comp[0]=x; comp[1]=y; comp[2]=z;
}

	
inline double &unit_vector3d::operator[](int i)
{
	ASSERT(i>=0 && i<3);
	return comp[i];
}

inline const double &unit_vector3d::operator[](int i) const
{
	ASSERT(i>=0 && i<3);
	return comp[i];
}

inline const unit_vector3d &unit_vector3d::operator +=(const unit_vector3d &v)
{
	comp[0] = comp[0]+v.comp[0];
	comp[1] = comp[1]+v.comp[1];
	comp[2] = comp[2]+v.comp[2];

	return *this;
}

inline const unit_vector3d &unit_vector3d::operator -=(const unit_vector3d &v)
{
	comp[0] = comp[0]-v.comp[0];
	comp[1] = comp[1]-v.comp[1];
	comp[2] = comp[2]-v.comp[2];

	return *this;
}

inline const unit_vector3d &unit_vector3d::operator *=(const unit_vector3d &v)
{
	*this = (*this)*v;

	return *this;
}

// friends
inline unit_vector3d operator -(const unit_vector3d &v)
{
	return unit_vector3d(-v.comp[0], -v.comp[1], -v.comp[2]);
}

inline bool operator ==(const unit_vector3d &v1, const unit_vector3d &v2)
{
	if ( eq(v1.comp[0], v2.comp[0]) &&
		eq(v1.comp[0], v2.comp[0]) &&
		eq(v1.comp[0], v2.comp[0]) ) 
		return true;
	else
		return false;
}

inline bool operator !=(const unit_vector3d &v1, const unit_vector3d &v2)
{
	return !(v1==v2);
}

inline unit_vector3d operator +(const unit_vector3d &v1, const unit_vector3d &v2)
{
	return unit_vector3d(v1.comp[0]+v2.comp[0], v1.comp[1]+v2.comp[1], v1.comp[2]+v2.comp[2]);
}

inline unit_vector3d operator -(const unit_vector3d &v1, const unit_vector3d &v2)
{
	return unit_vector3d(v1.comp[0]-v2.comp[0], v1.comp[1]-v2.comp[1], v1.comp[2]-v2.comp[2]);
}

inline unit_vector3d operator *(const unit_vector3d &v1, const unit_vector3d &v2)
{
	return unit_vector3d(v1.comp[1]*v2.comp[2]-v2.comp[1]*v1.comp[2],
		v2.comp[0]*v1.comp[2]-v1.comp[0]*v2.comp[2],
		v1.comp[0]*v2.comp[1]-v2.comp[0]*v1.comp[1]);
}

inline double operator %(const unit_vector3d &v1, const unit_vector3d &v2)
{
	return v1.comp[0]*v2.comp[0]+v1.comp[1]*v2.comp[1]+v1.comp[2]*v2.comp[2];
}

/*
unit_vector3d min(const unit_vector3d &v1, const unit_vector3d &v2)
{
	return unit_vector3d(min(v1.comp[0], v2.comp[0]),
		min(v1.comp[1], v2.comp[1]),
		min(v1.comp[2], v2.comp[2]));
}

unit_vector3d max(const unit_vector3d &v1, const unit_vector3d &v2)
{
	return unit_vector3d(max(v1.comp[0], v2.comp[0]),
		max(v1.comp[1], v2.comp[1]),
		max(v1.comp[2], v2.comp[2]));
}
*/

inline double unit_vector3d::length() const
{
	return sqrt(comp[0]*comp[0]+comp[1]*comp[1]+comp[2]*comp[2]);
}

inline bool eq(const unit_vector3d &v1, const unit_vector3d &v2, double tol)
{
	if (eq(v1[0], v2[0], tol) && eq(v1[1], v2[1], tol) && eq(v1[2], v2[2], tol))
		return true;
	else
		return false;
}

inline bool is_parallel(const unit_vector3d &v1, const unit_vector3d &v2, double tol)
{
	return eq(v1, v2, tol);
}


inline bool is_biparallel(const unit_vector3d &v1, const unit_vector3d &v2, double tol)
{
	return (eq(v1, v2, tol) || eq(v1, -v2, tol));	
}

inline bool is_antiparallel(const unit_vector3d &v1, const unit_vector3d &v2, double tol)
{
	return eq(v1, -v2, tol);
}

inline bool is_perpendicular(const unit_vector3d &v1, const unit_vector3d &v2, double tol)
{
	double cosangle;
	cosangle = v1%v2;

	if (eq(cosangle, 0, tol))
		return true;
	else
		return false;
}

