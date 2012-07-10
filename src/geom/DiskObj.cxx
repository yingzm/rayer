#include "common.hxx"
#include "DiskObj.hxx"

#include "ray.hxx"
#include "unit_vector.hxx"
#include "aabb.hxx"


DiskObj::DiskObj(double height, double radius, double tmax)
{
	m_height = height;
	m_radius = radius;
	m_tmax = deg2rad(tmax);
	if (m_tmax>0.0) {
		m_tmax = angle_normalize(m_tmax);
	}
	else {
		m_tmax = -(2*M_PI-angle_normalize(m_tmax));
	}

	CalcBoundingBox();
}

int DiskObj::IntersectNearest(const ray &r, IntersectCache &ic)
{
	const point3d &orig = r.get_orig();
	const vector3d &dir = r.get_dir();

	if (eq(dir.z(), 0)) { 
		// ray paralell with disk
		return 0;
	}
	else {
		double t = (m_height-orig.z())/dir.z();
		if (t<g_znear || t>g_zfar)
			return 0;	// the intersection is at the other end of ray

		point3d intp = orig+t*dir;

		double radius2 = intp.x()*intp.x()+intp.y()*intp.y();
		if (radius2>m_radius*m_radius)
			return 0;

		double theta = atan2(intp.y(), intp.x());
		theta = angle_normalize(theta);
		if (m_tmax<0)
			theta = -(2*M_PI-theta);
		if (fabs(theta)>fabs(m_tmax))
			return 0;

		ic.m_intpt = intp;
		ic.m_param = t;
		ic.m_u = theta/m_tmax;
		ic.m_v = 1.0-sqrt(radius2/(m_radius*m_radius));
		return 1;
	}
	return 0;
}

void DiskObj::Eval(IntersectCache &ic, unsigned int flag)
{
	ic.m_normal = vector3d::unit_z;

	if (flag&GeomObj::EVAL_DPDU) {
		ic.m_dPdu.set_comp(0, -m_tmax*m_radius*(1-ic.m_v)*sin(m_tmax*ic.m_u));
		ic.m_dPdu.set_comp(1, m_tmax*m_radius*(1-ic.m_v)*cos(m_tmax*ic.m_u));
		ic.m_dPdu.set_comp(0, 0.0);
	}

	if (flag&GeomObj::EVAL_DPDV) {
		ic.m_dPdv.set_comp(0, -m_radius*cos(m_tmax*ic.m_u));
		ic.m_dPdu.set_comp(0, -m_radius*sin(m_tmax*ic.m_u));
		ic.m_dPdv.set_comp(0, 0.0);
	}
}

void DiskObj::Evaluv(IntersectCache &ic, double u, double v, unsigned int flag)
{
	ic.m_u = u;
	ic.m_v = v;
	ic.m_normal = vector3d::unit_z;

	if (flag&GeomObj::EVAL_DPDU) {
		ic.m_dPdu.set_comp(0, -m_tmax*m_radius*(1-v)*sin(m_tmax*u));
		ic.m_dPdu.set_comp(1, m_tmax*m_radius*(1-v)*cos(m_tmax*u));
		ic.m_dPdu.set_comp(0, 0.0);
	}

	if (flag&GeomObj::EVAL_DPDV) {
		ic.m_dPdv.set_comp(0, -m_radius*cos(m_tmax*u));
		ic.m_dPdu.set_comp(0, -m_radius*sin(m_tmax*u));
		ic.m_dPdv.set_comp(0, 0.0);
	}
}

void DiskObj::CalcBoundingBox()
{
	m_box = new aabb(point3d(-m_radius, -m_radius, m_height), 
		point3d(m_radius, m_radius, m_height));
}