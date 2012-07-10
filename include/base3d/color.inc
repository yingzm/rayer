inline color3d::color3d()
{
	// EMPTY
}
inline color3d::color3d(double c1, double c2, double c3)
{
	comp[0] = c1; comp[1] = c2; comp[2] = c3;
}

inline color3d::color3d(const color4d &c)
{
	comp[0] = c.comp[0]; comp[1] = c.comp[1]; comp[2] = c.comp[2];
}

inline double *color3d::get_comp()
{
	return comp;
}

inline void color3d::set_comp(int i, double val)
{
	ASSERT(i>=0 && i<3);
	comp[i] = val;
}

inline void color3d::set_comp(double r, double g, double b)
{
	comp[0]=r; comp[1]=g; comp[2]=b;
}

	
inline double &color3d::operator[](int i)
{
	ASSERT(i>=0 && i<3);
	return comp[i];
}

inline const double &color3d::operator[](int i) const
{
	ASSERT(i>=0 && i<3);
	return comp[i];
}


inline const color3d &color3d::operator +=(const color3d &c)
{
	comp[0] += c.comp[0];
	comp[1] += c.comp[1];
	comp[2] += c.comp[2];

	return *this;
}

inline const color3d &color3d::operator -=(const color3d &c)
{
	comp[0] -= c.comp[0];
	comp[1] -= c.comp[1];
	comp[2] -= c.comp[2];

	return *this;
}

inline const color3d &color3d::operator *=(const color3d &c)
{
	comp[0] *= c.comp[0];
	comp[1] *= c.comp[1];
	comp[2] *= c.comp[2];

	return *this;
}

inline const color3d &color3d::operator *=(double d)
{
	comp[0] *= d;
	comp[1] *= d;
	comp[2] *= d;

	return *this;
}

inline const color3d &color3d::operator /=(double d)
{
	comp[0] /= d;
	comp[1] /= d;
	comp[2] /= d;

	return *this;
}

// friends
inline bool operator ==(const color3d &c1, const color3d &c2)
{
	if ( c1.comp[0]==c2.comp[0] &&
		c1.comp[1]==c2.comp[1] &&
		c1.comp[2]==c2.comp[2] )
		return true;
	else
		return false;
}

inline bool operator !=(const color3d &c1, const color3d &c2)
{
	return !(c1==c2);
}

inline color3d operator +(const color3d &c1, const color3d &c2)
{
	return color3d(c1.comp[0]+c2.comp[0], c1.comp[1]+c2.comp[1], c1.comp[2]+c2.comp[2]); 
}

inline color3d operator -(const color3d &c1, const color3d &c2)
{
	return color3d(c1.comp[0]-c2.comp[0], c1.comp[1]-c2.comp[1], c1.comp[2]-c2.comp[2]); 
}

inline color3d operator *(const color3d &c1, const color3d &c2)
{
	return color3d(c1.comp[0]*c2.comp[0], c1.comp[1]*c2.comp[1], c1.comp[2]*c2.comp[2]); 
}

inline color3d operator *(const color3d &c1, double k)
{
	return color3d(c1.comp[0]*k, c1.comp[1]*k, c1.comp[2]*k); 
}

inline color3d operator *(double k, const color3d &c1)
{
	return (c1*k);
}

inline color3d operator /(const color3d &c1, double k)
{
	return color3d(c1.comp[0]/k, c1.comp[1]/k, c1.comp[2]/k); 
}

inline color3d cmin(const color3d &c1, const color3d &c2)
{
	return color3d(min(c1.r(), c2.r()),
		min(c1.g(), c2.g()),
		min(c1.b(), c2.b()));
}

inline color3d cmax(const color3d &c1, const color3d &c2)
{
	return color3d(max(c1.r(), c2.r()),
		max(c1.g(), c2.g()),
		max(c1.b(), c2.b()));
}


////////////////////////////// color4d ///////////////////////////////////////
inline color4d::color4d()
{
	// EMPTY
}
inline color4d::color4d(double c1, double c2, double c3, double c4)
{
	comp[0] = c1; comp[1] = c2; comp[2] = c3; comp[3] = c4;
}

inline color4d::color4d(const color3d &c)
{
	comp[0] = c.comp[0]; comp[1] = c.comp[1]; comp[2] = c.comp[2];
}


inline double *color4d::get_comp()
{
	return comp;
}

inline void color4d::set_comp(int i, double val)
{
	ASSERT(i>=0 && i<4);
	comp[i] = val;
}

inline void color4d::set_comp(double r, double g, double b, double a)
{
	comp[0]=r; comp[1]=g; comp[2]=b; comp[3]=a;
}

	
inline double &color4d::operator[](int i)
{
	ASSERT(i>=0 && i<4);
	return comp[i];
}

inline const double &color4d::operator[](int i) const
{
	ASSERT(i>=0 && i<4);
	return comp[i];
}


inline const color4d &color4d::operator +=(const color4d &c)
{
	comp[0] += c.comp[0];
	comp[1] += c.comp[1];
	comp[2] += c.comp[2];
	comp[3] += c.comp[3];

	return *this;
}

inline const color4d &color4d::operator -=(const color4d &c)
{
	comp[0] -= c.comp[0];
	comp[1] -= c.comp[1];
	comp[2] -= c.comp[2];
	comp[3] -= c.comp[3];

	return *this;
}

inline const color4d &color4d::operator *=(const color4d &c)
{
	comp[0] *= c.comp[0];
	comp[1] *= c.comp[1];
	comp[2] *= c.comp[2];
	comp[3] *= c.comp[3];

	return *this;
}

inline const color4d &color4d::operator *=(double d)
{
	comp[0] *= d;
	comp[1] *= d;
	comp[2] *= d;
	comp[3] *= d;

	return *this;
}

inline const color4d &color4d::operator /=(double d)
{
	comp[0] /= d;
	comp[1] /= d;
	comp[2] /= d;
	comp[3] /= d;

	return *this;
}

// friends
inline bool operator ==(const color4d &c1, const color4d &c2)
{
	if ( c1.comp[0]==c2.comp[0] &&
		c1.comp[1]==c2.comp[1] &&
		c1.comp[2]==c2.comp[2] &&
		c1.comp[3]==c2.comp[3] )
		return true;
	else
		return false;
}

inline bool operator !=(const color4d &c1, const color4d &c2)
{
	return !(c1==c2);
}

inline color4d operator +(const color4d &c1, const color4d &c2)
{
	return color4d(c1.comp[0]+c2.comp[0], c1.comp[1]+c2.comp[1], c1.comp[2]+c2.comp[2], c1.comp[3]+c2.comp[3]); 
}

inline color4d operator -(const color4d &c1, const color4d &c2)
{
	return color4d(c1.comp[0]-c2.comp[0], c1.comp[1]-c2.comp[1], c1.comp[2]-c2.comp[2], c1.comp[3]-c2.comp[3]); 
}

inline color4d operator *(const color4d &c1, const color4d &c2)
{
	return color4d(c1.comp[0]*c2.comp[0], c1.comp[1]*c2.comp[1], c1.comp[2]*c2.comp[2], c1.comp[3]*c2.comp[3]); 
}

inline color4d operator *(const color4d &c1, double k)
{
	return color4d(c1.comp[0]*k, c1.comp[1]*k, c1.comp[2]*k, c1.comp[3]*k); 
}

inline color4d operator *(double k, const color4d &c1)
{
	return (c1*k);
}

inline color4d operator /(const color4d &c1, double k)
{
	return color4d(c1.comp[0]/k, c1.comp[1]/k, c1.comp[2]/k, c1.comp[3]/k); 
}

