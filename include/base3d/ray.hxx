#ifndef __RAY_HXX__
#define __RAY_HXX__

#include "point.hxx"
#include "vector.hxx"
#include "matrix.hxx"

BEGIN_NAMESPACE

class ray
{
public:
	const point3d &get_orig() const { return m_origin; }
	const vector3d &get_dir() const { return m_dir; }

	void set_orig(const point3d &orig) { m_origin = orig; }
	void set_dir(const vector3d &dir) { m_dir = dir; }

	friend ray operator *(const ray &r, const matrix4d &m);
	
private:
	point3d m_origin;
	vector3d m_dir;
};

END_NAMESPACE

#endif /* __RAY_HXX__ */
