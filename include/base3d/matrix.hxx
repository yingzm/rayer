#ifndef __MATRIX_HXX__
#define __MATRIX_HXX__

BEGIN_NAMESPACE

#include "math.hxx"

class vector3d;
class point3d;
class point4d;

/*
 *  0  1  2  3
 *  4  5  6  7
 *  8  9 10 11
 * 12 13 14 15 
 */

class matrix4d
{
public:
	matrix4d();
	matrix4d(double *mtx);

	double *get_comp();
	void set_comp(int row, int col, double value);

	double *get_row(int row);
	void set_row(int row, double *val);
	
	bool inverse();
	void transpose();

	const matrix4d &operator*=(const matrix4d &m);
	const double *operator[](int i) const;
	double *operator[](int i);

	// I ever want to have a const static matrix4d value of identity_matrix,
	// but i can't initialize it using my constructors.
	friend matrix4d identity_matrix();
	friend matrix4d translate_matrix(const vector3d &v);
	friend matrix4d rotatex_matrix(double angle);
	friend matrix4d rotatey_matrix(double angle);
	friend matrix4d rotatez_matrix(double angle);
	friend matrix4d rotate_matrix(const vector3d &v, double angle);
	friend matrix4d scale_matrix(const vector3d &v);
	friend matrix4d coordinate_matrix(const point3d &origin, const vector3d &xaxis, const vector3d &yaxis);
	friend matrix4d inverse_matrix(const matrix4d &m);
	friend matrix4d transpose_matrix(const matrix4d &m);

	friend matrix4d operator*(const matrix4d &m1, const matrix4d &m2);
	friend point3d operator *(const matrix4d &m, const point3d &p);
	friend point4d operator *(const matrix4d &m, const point4d &p);
	friend vector3d operator *(const matrix4d &m, const vector3d &v);
public:
	

private:
	// row first component
	double comp[16];
};

inline matrix4d::matrix4d()
{
	// EMPTY
}

inline matrix4d::matrix4d(double *mtx)
{
	memcpy(comp, mtx, sizeof(double)*16);
}

inline double *matrix4d::get_comp()
{
	return comp;
}

inline void matrix4d::set_comp(int row, int col, double value)
{
	ASSERT(row>=0 && row<4 && col>=0 && col<4);
	comp[row*4+col] = value;
}

inline double *matrix4d::get_row(int row)
{
	return comp+row*4;
}

inline void matrix4d::set_row(int row, double *val)
{
	double *p = comp+row*4;
	p[0] = val[0];
	p[1] = val[1];
	p[2] = val[2];
	p[3] = val[3];
}


inline const matrix4d &matrix4d::operator*=(const matrix4d &m)
{
	(*this) = (*this)*m;

	return *this;
}

inline const double *matrix4d::operator[](int i) const
{
	ASSERT(i>=0 && i<4);	

	return comp+i*4;
}

inline double *matrix4d::operator[](int i)
{
	ASSERT(i>=0 && i<4);	

	return comp+i*4;
}


END_NAMESPACE

#endif /* __MATRIX_HXX__ */
