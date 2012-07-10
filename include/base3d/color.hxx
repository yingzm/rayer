#ifndef __COLOR_HXX__
#define __COLOR_HXX__

BEGIN_NAMESPACE

class color4d;

class color3d
{
public:
	typedef enum {
		clamp_cut,
		clamp_scale
	} clamp_mode;
public:
	color3d();
	color3d(double c1, double c2, double c3);
	color3d(const color4d &c);

	double *get_comp();
	void set_comp(int i, double val);
	void set_comp(double x, double y, double z);	
	double &operator[](int i);
	const double &operator[](int i) const;

	double r() const { return comp[0]; }
	double g() const { return comp[1]; }
	double b() const { return comp[2]; }

	void clamp(clamp_mode cm);

	const color3d &operator +=(const color3d &c);
	const color3d &operator -=(const color3d &c);
	const color3d &operator *=(const color3d &c);
	const color3d &operator *=(double d);
	const color3d &operator /=(double d);

	friend class color4d;
	
	friend bool operator ==(const color3d &c1, const color3d &c2);
	friend bool operator !=(const color3d &c1, const color3d &c2);
	friend color3d operator +(const color3d &c1, const color3d &c2);
	friend color3d operator -(const color3d &c1, const color3d &c2);
	friend color3d operator *(const color3d &c1, const color3d &c2);
	friend color3d operator *(const color3d &c1, double k);
	friend color3d operator *(double k, const color3d &c1);
	friend color3d operator /(const color3d &c1, double k);
	friend color3d clamp(const color3d &c, clamp_mode m);

	friend color3d cmin(const color3d &c1, const color3d &c2);
	friend color3d cmax(const color3d &c1, const color3d &c2);

public:
	static const color3d black;
	static const color3d white;
	static const color3d red;
	static const color3d green;
	static const color3d blue;
	static const color3d yellow;

private:
	double comp[3];
};

class color4d
{
public:
	typedef enum {
		clamp_cut,
		clamp_scale
	} clamp_mode;
public:
	color4d();
	color4d(double c1, double c2, double c3, double c4);
	color4d(const color3d &c);

	double *get_comp();
	void set_comp(int i, double val);
	void set_comp(double x, double y, double z, double a);	
	double &operator[](int i);
	const double &operator[](int i) const;

	const color4d &operator +=(const color4d &c);
	const color4d &operator -=(const color4d &c);
	const color4d &operator *=(const color4d &c);
	const color4d &operator *=(double d);
	const color4d &operator /=(double d);

	friend class color3d;

	friend bool operator ==(const color4d &c1, const color4d &c2);
	friend bool operator !=(const color4d &c1, const color4d &c2);
	friend color4d operator +(const color4d &c1, const color4d &c2);
	friend color4d operator -(const color4d &c1, const color4d &c2);
	friend color4d operator *(const color4d &c1, const color4d &c2);
	friend color4d operator *(const color4d &c1, double k);
	friend color4d operator *(double k, const color4d &c1);
	friend color4d operator /(const color4d &c1, double k);
	friend color4d clamp(const color4d &c, clamp_mode m);

public:
	static const color4d black;
	static const color4d white;
	static const color4d red;
	static const color4d green;
	static const color4d blue;
	static const color4d yellow;

private:
	double comp[4];
};

#include "color.inc"

END_NAMESPACE

#endif /* __COLOR_HXX__ */
