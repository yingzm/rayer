#include "common.hxx"

#include "matrix.hxx"
#include "vector.hxx"
#include "point.hxx"

BEGIN_NAMESPACE

/*
 * Graphics GemsII, K.Wu, Fast Matrix Inversion 
 */
bool matrix4d::inverse()
{
	int i,j,k;               
	int pvt_i[4], pvt_j[4];            /* Locations of pivot elements */
	double pvt_val;                     /* Value of current pivot element */
	double hold;                        /* Temporary storage */
	double determinat;                  /* Determinant */
	
	determinat = 1.0f;
	for (k=0; k<4; k++)  {
		/** Locate k'th pivot element **/
		pvt_val=comp[k*4+k];            /** Initialize for search **/
		pvt_i[k]=k;
		pvt_j[k]=k;
		for (i=k; i<4; i++) {
			for (j=k; j<4; j++) {
				if (fabs(comp[i*4+j]) > fabs(pvt_val)) {
					pvt_i[k]=i;
					pvt_j[k]=j;
					pvt_val=comp[i*4+j];
				}
			}
		}
		
		/** Product of pivots, gives determinant when finished **/
		determinat*=pvt_val;
		if (fabs(determinat)<1e-7) {    
			return (false);  /** Matrix is singular (zero determinant). **/
		}
		
		/** "Interchange" rows (with sign change stuff) **/
		i=pvt_i[k];
		if (i!=k) {               /** If rows are different **/
			for (j=0; j<4; j++) {
				hold=-comp[k*4+j];
				comp[k*4+j]=comp[i*4+j];
				comp[i*4+j]=hold;
			}
		}
		
		/** "Interchange" columns **/
		j=pvt_j[k];
		if (j!=k) {              /** If columns are different **/
			for (i=0; i<4; i++) {
				hold=-comp[i*4+k];
				comp[i*4+k]=comp[i*4+j];
				comp[i*4+j]=hold;
			}
		}
		
		/** Divide column by minus pivot value **/
		for (i=0; i<4; i++) {
			if (i!=k) comp[i*4+k]/=( -pvt_val) ; 
		}
		
		/** Reduce the matrix **/
		for (i=0; i<4; i++) {
			hold = comp[i*4+k];
			for (j=0; j<4; j++) {
				if (i!=k && j!=k) comp[i*4+j]+=hold*comp[k*4+j];
			}
		}
		
		/** Divide row by pivot **/
		for (j=0; j<4; j++) {
			if (j!=k) comp[k*4+j]/=pvt_val;
		}
		
		/** Replace pivot by reciprocal (at last we can touch it). **/
		comp[k*4+k] = 1.0f/pvt_val;
	}
	
	/* That was most of the work, one final pass of row/column interchange */
	/* to finish */
	for (k=4-2; k>=0; k--) { /* Don't need to work with 1 by 1 corner*/
		i=pvt_j[k];            /* Rows to swap correspond to pivot COLUMN */
		if (i!=k) {            /* If rows are different */
			for(j=0; j<4; j++) {
				hold = comp[k*4+j];
				comp[k*4+j]=-comp[i*4+j];
				comp[i*4+j]=hold;
			}
		}
		
		j=pvt_i[k];           /* Columns to swap correspond to pivot ROW */
		if (j!=k)             /* If columns are different */
			for (i=0; i<4; i++) {
				hold=comp[i*4+k];
				comp[i*4+k]=-comp[i*4+j];
				comp[i*4+j]=hold;
			}
	}
	return (true);                          
}

void matrix4d::transpose()
{
/*
 *  0  1  2  3
 *  4  5  6  7
 *  8  9 10 11
 * 12 13 14 15 
 */
	swap(comp[1], comp[4]);
	swap(comp[2], comp[8]);
	swap(comp[6], comp[9]);
	swap(comp[3], comp[12]);
	swap(comp[7], comp[13]);
	swap(comp[11], comp[14]);
}

matrix4d identity_matrix()
{
	matrix4d m;
	m.comp[0]=1; m.comp[1]=0; m.comp[2]=0; m.comp[3]=0;
	m.comp[4]=0; m.comp[5]=1; m.comp[6]=0; m.comp[7]=0;
	m.comp[8]=0; m.comp[9]=0; m.comp[10]=1; m.comp[11]=0;
	m.comp[12]=0; m.comp[13]=0; m.comp[14]=0; m.comp[15]=1;

	return m;
}

matrix4d translate_matrix(const vector3d &v)
{
	matrix4d tr_mat = identity_matrix();
	tr_mat.comp[0*4+3] = v[0];
	tr_mat.comp[1*4+3] = v[1];
	tr_mat.comp[2*4+3] = v[2];

	return tr_mat;
}

matrix4d rotatex_matrix(double angle)
{
	matrix4d rx = identity_matrix();

	double cosangle = cos(angle), sinangle = sin(angle);
	rx[1][1] = cosangle; rx[1][2] = -sinangle;
	rx[2][1] = sinangle; rx[2][2] = cosangle;

	return rx;
}

matrix4d rotatey_matrix(double angle)
{
	matrix4d rx = identity_matrix();

	double cosangle = cos(angle), sinangle = sin(angle);
	rx[0][0] = cosangle; rx[0][2] = sinangle;
	rx[2][0] = -sinangle; rx[2][2] = cosangle;

	return rx;
}

matrix4d rotatez_matrix(double angle)
{
	matrix4d rx = identity_matrix();

	double cosangle = cos(angle), sinangle = sin(angle);
	rx[0][0] = cosangle; rx[0][1] = -sinangle;
	rx[1][0] = sinangle; rx[1][1] = cosangle;

	return rx;
}


matrix4d rotate_matrix(const vector3d &v, double angle)
{
	if (eq(v.y()*v.y()+v.z()*v.z(), 0)) {
		return rotatex_matrix(angle);
	}

	double zero[16];
	memset(zero, 0, sizeof(double)*16);

	matrix4d rx(zero), irx(zero), ry(zero), iry(zero), rz(zero);
	double a, b, c, d;
	a = v.x();
	b = v.y();
	c = v.z();
	d = sqrt(b*b+c*c);


	rx[0][0] = 1.0; rx[1][1] = c/d; rx[1][2] = -b/d;
	rx[2][1] = b/d; rx[2][2] = c/d; rx[3][3] = 1.0;
	irx[0][0] = 1.0; irx[1][1] = c/d; irx[1][2] = b/d;
	irx[2][1] = -b/d; irx[2][2] = c/d; irx[3][3] = 1.0;
	ry[0][0] = d; ry[0][2] = -a; ry[1][1] = 1.0;
	ry[2][0] = a; ry[2][2] = d; ry[3][3] = 1.0;
	iry[0][0] = d; iry[0][2] = a; iry[1][1] = 1.0;
	iry[2][0] = -a; iry[2][2] = d; iry[3][3] = 1.0;
	rz[0][0] = cos(angle); rz[0][1] = -sin(angle); rz[1][0] = sin(angle);
	rz[1][1] = cos(angle); rz[2][2] = 1.0; rz[3][3] = 1.0;

	return irx*iry*rz*ry*rx;
}

matrix4d scale_matrix(const vector3d &v)
{
	matrix4d tr_mat = identity_matrix();
	tr_mat.comp[0] = v[0];
	tr_mat.comp[5] = v[1];
	tr_mat.comp[10] = v[2];

	return tr_mat;
}

matrix4d coordinate_matrix(const point3d &origin, const vector3d &xaxis, const vector3d &yaxis)
{
	// TODO

	return matrix4d();
}

matrix4d inverse_matrix(const matrix4d &m)
{
	matrix4d _mat = m;
	_mat.inverse();

	return _mat;
}

matrix4d transpose_matrix(const matrix4d &m)
{
	matrix4d _m;
	
/*
 *  0  1  2  3
 *  4  5  6  7
 *  8  9 10 11
 * 12 13 14 15 
 */
	_m.comp[0]=m.comp[0]; _m.comp[1]=m.comp[4]; _m.comp[2]=m.comp[8]; _m.comp[3]=m.comp[12];
	_m.comp[4]=m.comp[1]; _m.comp[5]=m.comp[5]; _m.comp[6]=m.comp[9]; _m.comp[7]=m.comp[13];
	_m.comp[8]=m.comp[2]; _m.comp[9]=m.comp[6]; _m.comp[10]=m.comp[10]; _m.comp[11]=m.comp[14];
	_m.comp[12]=m.comp[3]; _m.comp[13]=m.comp[7]; _m.comp[14]=m.comp[11]; _m.comp[15]=m.comp[15];

	return _m;
}

matrix4d operator*(const matrix4d &m1, const matrix4d &m2)
{
	matrix4d m;

	m.comp[0]=m1.comp[0]*m2.comp[0]+m1.comp[1]*m2.comp[4]+m1.comp[2]*m2.comp[8]+m1.comp[3]*m2.comp[12];
	m.comp[1]=m1.comp[0]*m2.comp[1]+m1.comp[1]*m2.comp[5]+m1.comp[2]*m2.comp[9]+m1.comp[3]*m2.comp[13];
	m.comp[2]=m1.comp[0]*m2.comp[2]+m1.comp[1]*m2.comp[6]+m1.comp[2]*m2.comp[10]+m1.comp[3]*m2.comp[14];
	m.comp[3]=m1.comp[0]*m2.comp[3]+m1.comp[1]*m2.comp[7]+m1.comp[2]*m2.comp[11]+m1.comp[3]*m2.comp[15];
	
	m.comp[4]=m1.comp[4]*m2.comp[0]+m1.comp[5]*m2.comp[4]+m1.comp[6]*m2.comp[8]+m1.comp[7]*m2.comp[12];
	m.comp[5]=m1.comp[4]*m2.comp[1]+m1.comp[5]*m2.comp[5]+m1.comp[6]*m2.comp[9]+m1.comp[7]*m2.comp[13];
	m.comp[6]=m1.comp[4]*m2.comp[2]+m1.comp[5]*m2.comp[6]+m1.comp[6]*m2.comp[10]+m1.comp[7]*m2.comp[14];
	m.comp[7]=m1.comp[4]*m2.comp[3]+m1.comp[5]*m2.comp[7]+m1.comp[6]*m2.comp[11]+m1.comp[7]*m2.comp[15];

	m.comp[8]=m1.comp[8]*m2.comp[0]+m1.comp[9]*m2.comp[4]+m1.comp[10]*m2.comp[8]+m1.comp[11]*m2.comp[12];
	m.comp[9]=m1.comp[8]*m2.comp[1]+m1.comp[9]*m2.comp[5]+m1.comp[10]*m2.comp[9]+m1.comp[11]*m2.comp[13];
	m.comp[10]=m1.comp[8]*m2.comp[2]+m1.comp[9]*m2.comp[6]+m1.comp[10]*m2.comp[10]+m1.comp[11]*m2.comp[14];
	m.comp[11]=m1.comp[8]*m2.comp[3]+m1.comp[9]*m2.comp[7]+m1.comp[10]*m2.comp[11]+m1.comp[11]*m2.comp[15];

	m.comp[12]=m1.comp[12]*m2.comp[0]+m1.comp[13]*m2.comp[4]+m1.comp[14]*m2.comp[8]+m1.comp[15]*m2.comp[12];
	m.comp[13]=m1.comp[12]*m2.comp[1]+m1.comp[13]*m2.comp[5]+m1.comp[14]*m2.comp[9]+m1.comp[15]*m2.comp[13];
	m.comp[14]=m1.comp[12]*m2.comp[2]+m1.comp[13]*m2.comp[6]+m1.comp[14]*m2.comp[10]+m1.comp[15]*m2.comp[14];
	m.comp[15]=m1.comp[12]*m2.comp[3]+m1.comp[13]*m2.comp[7]+m1.comp[14]*m2.comp[11]+m1.comp[15]*m2.comp[15];

	return m;
}

point3d operator *(const matrix4d &m, const point3d &p)
{
	return point3d(m*point4d(p));
}

point4d operator *(const matrix4d &m, const point4d &p)
{
	point4d p2;

	p2[0] = m.comp[0]*p[0]+m.comp[1]*p[1]+m.comp[2]*p[2]+m.comp[3]*p[3];
	p2[1] = m.comp[4]*p[0]+m.comp[5]*p[1]+m.comp[6]*p[2]+m.comp[7]*p[3];
	p2[2] = m.comp[8]*p[0]+m.comp[9]*p[1]+m.comp[10]*p[2]+m.comp[11]*p[3];
	p2[3] = m.comp[12]*p[0]+m.comp[13]*p[1]+m.comp[14]*p[2]+m.comp[15]*p[3];

	return p2;
}

// OPTI: 
vector3d operator *(const matrix4d &m, const vector3d &v)
{
	vector3d v2;
	v2[0] = m.comp[0]*v[0]+m.comp[1]*v[1]+m.comp[2]*v[2]+m.comp[3];
	v2[1] = m.comp[4]*v[0]+m.comp[5]*v[1]+m.comp[6]*v[2]+m.comp[7];
	v2[2] = m.comp[8]*v[0]+m.comp[9]*v[1]+m.comp[10]*v[2]+m.comp[11];
	// assume no homogeneous content in matrix

	return v2;
}

END_NAMESPACE