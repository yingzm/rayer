inline vector3d::vector3d()
{
	// EMPTY
}


inline vector3d::vector3d(double x, double y, double z)
{
	comp[0]=x; comp[1]=y; comp[2]=z;
}

inline double *vector3d::get_comp()
{
	return comp;
}

inline void vector3d::set_comp(int i, double val)
{
	ASSERT(i>=0 && i<3);
	comp[i] = val;
}

inline void vector3d::set_comp(double x, double y, double z)
{
	comp[0]=x; comp[1]=y; comp[2]=z;
}

	
inline double &vector3d::operator[](int i)
{
	ASSERT(i>=0 && i<3);
	return comp[i];
}

inline 	const double &vector3d::operator[](int i) const
{
	ASSERT(i>=0 && i<3);
	return comp[i];	
}

inline const vector3d &vector3d::operator +=(const vector3d &v)
{
	comp[0] = comp[0]+v.comp[0];
	comp[1] = comp[1]+v.comp[1];
	comp[2] = comp[2]+v.comp[2];

	return *this;
}

inline const vector3d &vector3d::operator -=(const vector3d &v)
{
	comp[0] = comp[0]-v.comp[0];
	comp[1] = comp[1]-v.comp[1];
	comp[2] = comp[2]-v.comp[2];

	return *this;
}

inline const vector3d &vector3d::operator *=(const vector3d &v)
{
	*this = (*this)*v;

	return *this;
}

inline const vector3d &vector3d::operator *=(double k)
{
	comp[0]*=k; comp[1]*=k; comp[2]*=k;

	return *this;
}

inline const vector3d &vector3d::operator *=(const matrix4d &m)
{
	*this = m*(*this);
	
	return *this;
}

inline const vector3d &vector3d::operator /=(double k)
{
	comp[0]/=k; comp[1]/=k; comp[2]/=k;

	return *this;
}

inline double vector3d::length() const
{
	return sqrt(sqrlength());
}

inline double vector3d::sqrlength() const
{
	return (comp[0]*comp[0]+comp[1]*comp[1]+comp[2]*comp[2]);
}

	// friends
inline vector3d operator -(const vector3d &v)
{
	return vector3d(-v.comp[0], -v.comp[1], -v.comp[2]);
}

inline bool operator ==(const vector3d &v1, const vector3d &v2)
{
	if ( eq(v1.comp[0], v2.comp[0]) &&
		eq(v1.comp[0], v2.comp[0]) &&
		eq(v1.comp[0], v2.comp[0]) ) 
		return true;
	else
		return false;
}

inline bool operator !=(const vector3d &v1, const vector3d &v2)
{
	return !(v1==v2);
}

inline vector3d operator +(const vector3d &v1, const vector3d &v2)
{
	return vector3d(v1.comp[0]+v2.comp[0], v1.comp[1]+v2.comp[1], v1.comp[2]+v2.comp[2]);
}

inline vector3d operator -(const vector3d &v1, const vector3d &v2)
{
	return vector3d(v1.comp[0]-v2.comp[0], v1.comp[1]-v2.comp[1], v1.comp[2]-v2.comp[2]);
}

inline vector3d operator *(const vector3d &v1, const vector3d &v2)
{
	return vector3d(v1.comp[1]*v2.comp[2]-v2.comp[1]*v1.comp[2],
		v2.comp[0]*v1.comp[2]-v1.comp[0]*v2.comp[2],
		v1.comp[0]*v2.comp[1]-v2.comp[0]*v1.comp[1]);
}

inline vector3d operator *(const vector3d &v1, double k)
{
	return vector3d(v1.comp[0]*k, v1.comp[1]*k, v1.comp[2]*k);
}

inline vector3d operator *(double k, const vector3d &v1)
{
	return vector3d(v1.comp[0]*k, v1.comp[1]*k, v1.comp[2]*k);
}

inline double operator %(const vector3d &v1, const vector3d &v2)
{
	return v1.comp[0]*v2.comp[0]+v1.comp[1]*v2.comp[1]+v1.comp[2]*v2.comp[2];
}

inline vector3d operator /(const vector3d &v1, double k)
{
	return vector3d(v1.comp[0]/k, v1.comp[1]/k, v1.comp[2]/k);
}



inline vector3d vmin(const vector3d &v1, const vector3d &v2)
{
	return vector3d(min(v1.comp[0], v2.comp[0]),
		min(v1.comp[1], v2.comp[1]),
		min(v1.comp[2], v2.comp[2]));
}

inline vector3d vmax(const vector3d &v1, const vector3d &v2)
{
	return vector3d(max(v1.comp[0], v2.comp[0]),
		max(v1.comp[1], v2.comp[1]),
		max(v1.comp[2], v2.comp[2]));
}


inline bool eq(const vector3d &v1, const vector3d &v2, double tol)
{
	if (eq(v1[0], v2[0], tol) && eq(v1[1], v2[1], tol) && eq(v1[2], v2[2], tol))
		return true;
	else
		return false;
}

