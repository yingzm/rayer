inline point3d::point3d()
{
	// EMPTY
}

inline point3d::point3d(double x, double y, double z)
{
	comp[0]=x; comp[1]=y; comp[2]=z;
}

inline point3d::point3d(const point4d &p)
{
	if (eq(p[3], 0.0)) {
		comp[0]= (p[0]==0) ? 0 : DBL_INFINITE;
		comp[1]= (p[1]==0) ? 0 : DBL_INFINITE;
		comp[2]= (p[2]==0) ? 0 : DBL_INFINITE;
	}
	else {
		comp[0] = p[0]/p[3];
		comp[1] = p[1]/p[3];
		comp[2] = p[2]/p[3];
	}
}

inline double *point3d::get_comp()
{
	return comp;
}

inline void point3d::set_comp(int i, double val)
{
	ASSERT(i>=0 && i<3);
	comp[i] = val;
}

inline void point3d::set_comp(double x, double y, double z)
{
	comp[0]=x; comp[1]=y; comp[2]=z;
}

	
inline double &point3d::operator[](int i)
{
	ASSERT(i>=0 && i<3);
	return comp[i];
}

inline const double &point3d::operator[](int i) const
{
	ASSERT(i>=0 && i<3);
	return comp[i];
}


inline const point3d &point3d::operator +=(const vector3d &v)
{
	comp[0] = comp[0]+v[0];
	comp[1] = comp[1]+v[1];
	comp[2] = comp[2]+v[2];

	return *this;
}

inline const point3d &point3d::operator *=(double k)
{
	comp[0]*=k; comp[1]*=k; comp[2]*=k;

	return *this;
}

inline const point3d &point3d::operator *=(const matrix4d &m)
{
	*this = m*(*this);
	
	return *this;
}

inline const point3d &point3d::operator /=(double k)
{
	comp[0]/=k; comp[1]/=k; comp[2]/=k;

	return *this;
}

inline double point3d::length() const
{
	return sqrt(sqrlength());
}

inline double point3d::sqrlength() const
{
	return (comp[0]*comp[0]+comp[1]*comp[1]+comp[2]*comp[2]);
}

	// friends
inline point3d operator -(const point3d &v)
{
	return point3d(-v.comp[0], -v.comp[1], -v.comp[2]);
}

inline bool operator ==(const point3d &v1, const point3d &v2)
{
	if ( eq(v1.comp[0], v2.comp[0]) &&
		eq(v1.comp[1], v2.comp[1]) &&
		eq(v1.comp[2], v2.comp[2]) ) 
		return true;
	else
		return false;
}

inline bool operator !=(const point3d &v1, const point3d &v2)
{
	return !(v1==v2);
}

inline point3d operator +(const point3d &v1, const point3d &v2)
{
	return point3d(v1.comp[0]+v2.comp[0], v1.comp[1]+v2.comp[1], v1.comp[2]+v2.comp[2]);
}

inline vector3d operator -(const point3d &v1, const point3d &v2)
{
	return vector3d(v1.comp[0]-v2.comp[0], v1.comp[1]-v2.comp[1], v1.comp[2]-v2.comp[2]);
}

inline point3d operator *(const point3d &v1, double k)
{
	return point3d(v1.comp[0]*k, v1.comp[1]*k, v1.comp[2]*k);
}

inline point3d operator *(double k, const point3d &v1)
{
	return point3d(v1.comp[0]*k, v1.comp[1]*k, v1.comp[2]*k);
}

inline point3d operator /(const point3d &v1, double k)
{
	return point3d(v1.comp[0]/k, v1.comp[1]/k, v1.comp[2]/k);
}

inline point3d operator+(const point3d &p, const vector3d &v)
{
	return point3d(p.comp[0]+v[0], p.comp[1]+v[1], p.comp[2]+v[2]);
}


inline point3d pmin(const point3d &v1, const point3d &v2)
{
	return point3d(min(v1.comp[0], v2.comp[0]),
		min(v1.comp[1], v2.comp[1]),
		min(v1.comp[2], v2.comp[2]));
}

inline point3d pmax(const point3d &v1, const point3d &v2)
{
	return point3d(max(v1.comp[0], v2.comp[0]),
		max(v1.comp[1], v2.comp[1]),
		max(v1.comp[2], v2.comp[2]));
}


inline point4d::point4d()
{
	// EMPTY
}

inline point4d::point4d(double x, double y, double z, double w)
{
	comp[0]=x; comp[1]=y; comp[2]=z; comp[3]=w;
}

inline point4d::point4d(const point3d &p)
{
	comp[0]=p[0]; comp[1]=p[1]; 
	comp[2]=p[2]; comp[3]=1;
}

inline double *point4d::get_comp()
{
	return comp;
}

inline void point4d::set_comp(int i, double val)
{
	ASSERT(i>=0 && i<4);
	comp[i] = val;
}

inline void point4d::set_comp(double x, double y, double z, double w)
{
	comp[0]=x; comp[1]=y; comp[2]=z; comp[3]=w;
}

	
inline double &point4d::operator[](int i)
{
	ASSERT(i>=0 && i<4);
	return comp[i];
}

inline const double &point4d::operator[](int i) const
{
	ASSERT(i>=0 && i<4);
	return comp[i];
}


