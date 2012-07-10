#include "common.hxx"
#include "aabb.hxx"

BEGIN_NAMESPACE

const aabb aabb::null_box(point3d::maximum, point3d::minimum);

aabb::aabb()
{
	m_min = point3d::maximum;
	m_max = point3d::minimum;
}

aabb::aabb(const point3d &min, const point3d &max)
{
	m_min = min;
	m_max = max;
}


bool aabb::isin(const point3d &pnt) const
{
	if (ge(pnt[0], m_min[0]) && le(pnt[0], m_max[0]) &&
		ge(pnt[1], m_min[1]) && le(pnt[1], m_max[1]) &&
		ge(pnt[2], m_min[2]) && le(pnt[2], m_max[2]))
		return true;
	else
		return false;
}

/*
 * Efficient Bounding Box Intersection, by Brain Smits
 * 
 * Ray Tracing News, Volume 15, Number 1
 * http://www.acm.org/tog/resources/RTNews/html/rtnv15n1.html
 */

bool aabb::intersect(const ray &r) const
{
	double intervalMin=-DBL_MAX, intervalMax=DBL_MAX;
	double t1, t2;
	const point3d &origin = r.get_orig();
	const vector3d &dir = r.get_dir();
	vector3d invdir;
	invdir.set_comp(1/dir.x(), 1/dir.y(), 1/dir.z());

	for (int i=0; i<3; ++i) {
		t1 = (m_min[i] - origin[i]) * invdir[i];
		t2 = (m_max[i] - origin[i]) * invdir[i];

		if (invdir[i] > 0) {
			if (t1 > intervalMin) intervalMin = t1;
			if (t2 < intervalMax) intervalMax = t2;
			if (intervalMin > intervalMax) 
				return false;
		}
		else if (invdir[i] < 0) {
			if (t2 > intervalMin) intervalMin = t2;
			if (t1 < intervalMax) intervalMax = t1;
			if (intervalMin > intervalMax) 
				return false;
		}
		else {
			if (origin[i]<m_min[i] || origin[i]>m_max[i])
				return false;
		}
	}

	if (ge(intervalMin, 0))
		return true;
	else
		return false;
}

const bbox &aabb::operator &=(const point3d &p)
{
	m_min = pmin(m_min, p);
	m_max = pmax(m_max, p);

	return *this;
}

aabb operator |(const aabb &bx1, const aabb &bx2)
{
	point3d mmin, mmax;
	mmin = pmin(bx1.get_min(), bx2.get_min());
	mmax = pmax(bx1.get_max(), bx2.get_max());

	return aabb(mmin, mmax);
}

aabb operator &(const aabb &bx1, const aabb &bx2)
{
	point3d mmin, mmax;
	mmin = pmax(bx1.get_min(), bx2.get_min());
	mmax = pmin(bx1.get_max(), bx2.get_max());

	return aabb(mmin, mmax);
}

END_NAMESPACE