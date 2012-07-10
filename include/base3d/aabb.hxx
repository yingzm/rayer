#ifndef __AABB_HXX__
#define __AABB_HXX__

#include "bbox.hxx"
#include "ray.hxx"

BEGIN_NAMESPACE

class aabb : public bbox
{
friend aabb operator |(const aabb &bx1, const aabb &bx2);
friend aabb operator &(const aabb &bx1, const aabb &bx2);

public:
	aabb();
	aabb(const point3d &min, const point3d &max);

	const bbox &operator &=(const point3d &p);

	bool isempty() const;

	bool isin(const point3d &pnt) const;
	
	bool intersect(const ray &r) const;

public:
	static const aabb null_box;

private:
	point3d m_min;
	point3d m_max;

	const point3d &get_min() const { return m_min; }
	const point3d &get_max() const { return m_max; }
};

aabb operator |(const aabb &bx1, const aabb &bx2);
// NOTICE: the aabb after & is perhaps an empty aabb
aabb operator &(const aabb &bx1, const aabb &bx2);

inline bool aabb::isempty() const
{ 
	if (ge(m_min[0], m_max[0]) || ge(m_min[1], m_max[1]) || ge(m_min[2], m_max[2])) 
		return true;
	else
		return false;
}

END_NAMESPACE

#endif /* __AABB_HXX__ */

