#include "common.hxx"
#include "ray.hxx"

ray operator *(const ray &r, const matrix4d &m)
{
	ray r0;

	point3d p2 = r.m_origin+r.m_dir;

	r0.m_origin = m*r.m_origin;
	p2 = m*p2;

	r0.m_dir = p2-r0.m_origin;
	r0.m_dir.normalize();

	return r0;

}