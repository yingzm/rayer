#ifndef __SHADERVAR_HXX__
#define __SHADERVAR_HXX__

#include "point.hxx"
#include "vector.hxx"
#include "color.hxx"
#include "matrix.hxx"

#include "sli.hxx"

class ShaderParam
{
public:
	ShaderParam();
	ShaderParam(const ShaderParam &sp);
	~ShaderParam();

	void ConvertTo(int type);


	void SetName(const string &name);
	void SetSize(int size);
	void SetOffset(int offset);

	void Setd(double d);
	void Setp(const point3d &p);
	void Setv(const vector3d &v);
	void Setn(const vector3d &v);
	void Setc(const color3d &c);
	void Setm(const matrix4d &m);
	void Sets(const string &s);

	void Setda(int n, double *d);
	void Setpa(int n, point3d *p);
	void Setva(int n, vector3d *v);
	void Setna(int n, vector3d *v);
	void Setca(int n, color3d *c);
	void Setma(int n, matrix4d *m);
	void Setsa(int n, string *s);

	const string &GetName() const;
	int GetSize() const;
	int GetAllSize() const;
	int GetItemSize() const;
	int GetItemNum() const;
	int GetOffset() const;
	int GetType() const;
	void *GetValuePtr();
	double Getd() const;
	point3d Getp() const;
	vector3d Getv() const;
	vector3d Getn() const;
	color3d Getc() const;
	matrix4d Getm() const;
	string Gets() const;
	double Getda(int i) const;
	point3d Getpa(int i) const;
	vector3d Getva(int i) const;
	vector3d Getna(int i) const;
	color3d Getca(int i) const;
	matrix4d Getma(int i) const;
	string Getsa(int i) const;

private:
	string m_name;
	variable_t m_type;
	unsigned int m_sz;	// low 16 bit is the size of each item
				// high 16 bit is the number of items if i1s array
	unsigned int m_offset;	// offset in the parameter list, counted in bytes
	void *m_value;

	int GetDoubleArray(const char *s, double *&arr);
	void DestroyValue();
};

inline void ShaderParam::SetName(const string &name)
{
	m_name = name;
}

inline void ShaderParam::SetSize(int size)
{
	m_sz = size;
}

inline void ShaderParam::SetOffset(int offset)
{
	m_offset = offset;
}

inline void ShaderParam::Setd(double d)
{
	if (m_type!=VAR_FLOAT) {
		DestroyValue();
		m_value = new double;
	}

	*(double *)m_value = d;
	m_type = VAR_FLOAT;
	m_sz = sizeof(double);
}

inline void ShaderParam::Setp(const point3d &p)
{
	if (m_type!=VAR_POINT) {
		DestroyValue();
		m_value = new point3d;
	}

	*(point3d *)m_value = p;
	m_type = VAR_POINT;
	m_sz = sizeof(point3d);
}

inline void ShaderParam::Setv(const vector3d &v)
{
	if (m_type!=VAR_VECTOR) {
		DestroyValue();
		m_value = new vector3d;
	}

	*(vector3d *)m_value = v;
	m_type = VAR_VECTOR;
	m_sz = sizeof(vector3d);
}

inline void ShaderParam::Setn(const vector3d &v)
{
	if (m_type!=VAR_NORMAL) {
		DestroyValue();
		m_value = new vector3d;
	}

	*(vector3d *)m_value = v;
	m_type = VAR_NORMAL;
	m_sz = sizeof(vector3d);
}

inline void ShaderParam::Setc(const color3d &c)
{
	if (m_type!=VAR_COLOR) {
		DestroyValue();
		m_value = new color3d;
	}

	*(color3d *)m_value = c;
	m_type = VAR_COLOR;
	m_sz = sizeof(color3d);
}

inline void ShaderParam::Setm(const matrix4d &m)
{
	if (m_type !=VAR_MATRIX) {
		DestroyValue();
		m_value = new matrix4d;
	}

	*(matrix4d *)m_value = m;
	m_type = VAR_MATRIX;
	m_sz = sizeof(matrix4d);
}

inline void ShaderParam::Sets(const string &s)
{
	if (m_type != VAR_STRING) {
		DestroyValue();
		m_value = new string;
	}

	*(string *)m_value = s;
	m_type = VAR_STRING;
	m_sz = sizeof(string);
}

inline void ShaderParam::Setda(int n, double *d)
{
	if ( (m_type != (VAR_FLOAT | VAR_ARRAY)) ||
		(m_type==(VAR_FLOAT | VAR_ARRAY) && GetItemNum()==n )) {
		DestroyValue();
		m_value = new double[n];
	}

	memcpy(m_value, d, sizeof(double)*n);
	m_type = variable_t(VAR_FLOAT | VAR_ARRAY);
	m_sz = (n<<16)+sizeof(double);
}

inline void ShaderParam::Setpa(int n, point3d *p)
{
	if ( (m_type != (VAR_POINT | VAR_ARRAY)) ||
		(m_type==(VAR_POINT | VAR_ARRAY) && GetItemNum()==n )) {
		DestroyValue();
		m_value = new point3d[n];
	}

	memcpy(m_value, p, sizeof(point3d)*n);
	m_type = variable_t(VAR_POINT | VAR_ARRAY);
	m_sz = (n<<16)+sizeof(point3d);
}

inline void ShaderParam::Setva(int n, vector3d *v)
{
	if ( (m_type != (VAR_VECTOR | VAR_ARRAY)) ||
		(m_type==(VAR_VECTOR | VAR_ARRAY) && GetItemNum()==n )) {
		DestroyValue();
		m_value = new vector3d[n];
	}

	memcpy(m_value, v, sizeof(vector3d)*n);
	m_type = variable_t(VAR_VECTOR | VAR_ARRAY);
	m_sz = (n<<16)+sizeof(vector3d);
}

inline void ShaderParam::Setna(int n, vector3d *v)
{
	if ( (m_type != (VAR_NORMAL | VAR_ARRAY)) ||
		(m_type==(VAR_NORMAL | VAR_ARRAY) && GetItemNum()==n )) {
		DestroyValue();
		m_value = new vector3d[n];
	}

	memcpy(m_value, v, sizeof(vector3d)*n);
	m_type = variable_t(VAR_FLOAT | VAR_ARRAY);
	m_sz = (n<<16)+sizeof(vector3d);
}

inline void ShaderParam::Setca(int n, color3d *c)
{
	if ( (m_type != (VAR_COLOR | VAR_ARRAY)) ||
		(m_type==(VAR_COLOR | VAR_ARRAY) && GetItemNum()==n )) {
		DestroyValue();
		m_value = new color3d[n];
	}

	memcpy(m_value, c, sizeof(color3d)*n);
	m_type = variable_t(VAR_COLOR | VAR_ARRAY);
	m_sz = (n<<16)+sizeof(color3d);
}

inline void ShaderParam::Setma(int n, matrix4d *m)
{
	if ( (m_type != (VAR_MATRIX | VAR_ARRAY)) ||
		(m_type==(VAR_MATRIX | VAR_ARRAY) && GetItemNum()==n )) {
		DestroyValue();
		m_value = new matrix4d[n];
	}

	memcpy(m_value, m, sizeof(matrix4d)*n);
	m_type = variable_t(VAR_MATRIX | VAR_ARRAY);
	m_sz = (n<<16)+sizeof(matrix4d);
}

inline void ShaderParam::Setsa(int n, string *s)
{
	if ( (m_type != (VAR_STRING | VAR_ARRAY)) ||
		(m_type==(VAR_STRING | VAR_ARRAY) && GetItemNum()==n )) {
		DestroyValue();
		m_value = new string[n];
	}

	for (int i=0; i<n; ++i) 
		((string *)m_value)[i] = s[i];
	m_type = variable_t(VAR_STRING | VAR_ARRAY);
	m_sz = (n<<16)+sizeof(string);
}

inline const string &ShaderParam::GetName() const
{
	return m_name;
}

inline int ShaderParam::GetSize() const
{
	return m_sz;
}

inline int ShaderParam::GetAllSize() const
{
	int numitem = GetItemNum();
	int itemsize = GetItemSize();

	if (numitem==0)
		return itemsize;
	else
		return numitem*itemsize;
}

inline int ShaderParam::GetItemSize() const
{
	return (m_sz&0xffff);
}

inline int ShaderParam::GetItemNum() const
{
	return (m_sz>>16);
}

inline int ShaderParam::GetOffset() const
{
	return m_offset;
}

inline int ShaderParam::GetType() const
{
	return m_type;
}

inline void *ShaderParam::GetValuePtr()
{
	return m_value;
}

inline double ShaderParam::Getd() const
{
	if (m_type!=VAR_FLOAT) {
		printf("!<ShaderParam::Getd> Invalid argument\n");
		return 0.0;
	}

	return *(double *)m_value;
}

inline point3d ShaderParam::Getp() const
{
	if (m_type!=VAR_POINT) {
		printf("!<ShaderParam::Getp> Invalid argument\n");
		return point3d(0, 0, 0);
	}

	return *(point3d *)m_value;
}

inline vector3d ShaderParam::Getv() const
{
	if (m_type!=VAR_VECTOR) {
		printf("!<ShaderParam::Getv> Invalid argument\n");
		return vector3d(0, 0, 0);
	}

	return *(vector3d *)m_value;
}

inline vector3d ShaderParam::Getn() const
{
	if (m_type!=VAR_NORMAL) {
		printf("!<ShaderParam::Getn> Invalid argument\n");
		return vector3d(0, 0, 0);
	}

	return *(vector3d *)m_value;

}

inline color3d ShaderParam::Getc() const
{
	if (m_type!=VAR_COLOR) {
		printf("!<ShaderParam::Getc> Invalid argument\n");
		return color3d(0, 0, 0);
	}

	return *(color3d *)m_value;

}

inline matrix4d ShaderParam::Getm() const
{
	if (m_type!=VAR_MATRIX) {
		printf("!<ShaderParam::Getm> Invalid argument\n");
		return matrix4d();
	}

	return *(matrix4d *)m_value;

}

inline string ShaderParam::Gets() const
{
	if (m_type!=VAR_STRING) {
		printf("!<ShaderParam::Gets> Invalid argument\n");
		return string("");
	}

	return *(string*)m_value;
}

inline	double ShaderParam::Getda(int i) const
{
	if (m_type!= (VAR_FLOAT|VAR_ARRAY) ) {
		printf("!<ShaderParam::Getda> Invalid argument\n");
		return 0;
	}
	if (i>=GetItemNum()) {
		printf("!<ShaderParam::Getda> Index overflow\n");
		return 0;
	}

	return ((double *)m_value)[i];
}

inline point3d ShaderParam::Getpa(int i) const
{
	if (m_type!= (VAR_POINT|VAR_ARRAY) ) {
		printf("!<ShaderParam::Getpa> Invalid argument\n");
		return point3d::zero;
	}
	if (i>=GetItemNum()) {
		printf("!<ShaderParam::Getpa> Index overflow\n");
		return point3d::zero;
	}

	return ((point3d *)m_value)[i];
}

inline vector3d ShaderParam::Getva(int i) const
{
	if (m_type!= (VAR_VECTOR|VAR_ARRAY) ) {
		printf("!<ShaderParam::Getva> Invalid argument\n");
		return vector3d::zero;
	}
	if (i>=GetItemNum()) {
		printf("!<ShaderParam::Getva> Index overflow\n");
		return vector3d::zero;
	}

	return ((vector3d *)m_value)[i];
}

inline vector3d ShaderParam::Getna(int i) const
{
	if (m_type!= (VAR_NORMAL|VAR_ARRAY) ) {
		printf("!<ShaderParam::Getna> Invalid argument\n");
		return vector3d::zero;
	}
	if (i>=GetItemNum()) {
		printf("!<ShaderParam::Getna> Index overflow\n");
		return vector3d::zero;
	}

	return ((vector3d *)m_value)[i];
}

inline color3d ShaderParam::Getca(int i) const
{
	if (m_type!= (VAR_COLOR|VAR_ARRAY) ) {
		printf("!<ShaderParam::Getca> Invalid argument\n");
		return color3d::black;
	}
	if (i>=GetItemNum()) {
		printf("!<ShaderParam::Getca> Index overflow\n");
		return color3d::black;
	}

	return ((color3d *)m_value)[i];
}

inline matrix4d ShaderParam::Getma(int i) const
{
	if (m_type!= (VAR_MATRIX|VAR_ARRAY) ) {
		printf("!<ShaderParam::Getma> Invalid argument\n");
		return matrix4d();
	}
	if (i>=GetItemNum()) {
		printf("!<ShaderParam::Getma> Index overflow\n");
		return matrix4d();
	}

	return ((matrix4d *)m_value)[i];
}

inline string ShaderParam::Getsa(int i) const
{
	if (m_type!= (VAR_STRING|VAR_ARRAY) ) {
		printf("!<ShaderParam::Getsa> Invalid argument\n");
		return string("");
	}
	if (i>=GetItemNum()) {
		printf("!<ShaderParam::Getsa> Index overflow\n");
		return string("");
	}

	return ((string *)m_value)[i];
}




#endif /* __SHADERVAR_HXX__ */