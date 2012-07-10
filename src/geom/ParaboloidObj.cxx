#include "common.hxx"
#include "ParaboloidObj.hxx"

#include "ray.hxx"
#include "aabb.hxx"

ParaboloidObj::ParaboloidObj(double rmax, double zmin, double zmax, double tmax)
{
	m_rmax = rmax; 
	if (zmin>zmax) {
		m_zmin = zmax; m_zmax = zmin;
	}
	else {
		m_zmin = zmin; m_zmax = zmax; 
	}
	m_tmax = deg2rad(tmax);

	CalcBoundingBox();
}

int ParaboloidObj::IntersectNearest(const ray &r, IntersectCache &ic)
{
	double x0, y0, z0;
	double dx, dy, dz;
	x0 = r.get_orig().x(); y0 = r.get_orig().y(); z0 = r.get_orig().z();
	dx = r.get_dir().x(); dy = r.get_dir().y(); dz = r.get_dir().z();

	double a, b, c;
	a = dx*dx+dy*dy;
	b = 2*dx*x0+2*dy*y0-dz*m_rmax*m_rmax/m_zmax;
	c = x0*x0+y0*y0-m_rmax*m_rmax*z0/m_zmax;

	double t[2];
	int n = solve_quadric(a, b, c, t);

	for (int i=0; i<n; ++i) {
		if (t[i]<g_znear || t[i]>g_zfar)
			continue;

		point3d intp = r.get_orig()+t[i]*r.get_dir();
		if (intp.z()<m_zmin || intp.z()>m_zmax)
			continue;

		double theta = atan2(intp.y(), intp.x());
		theta = angle_normalize(theta);
		if (theta>m_tmax)
			continue;

		// ok
		ic.m_intpt = intp;
		ic.m_param = t[i];
		ic.m_u = theta/m_tmax;
		ic.m_v = intp.z()/(m_zmax-m_zmin);

		return 1;
	}

	return 0;
}

void ParaboloidObj::Eval(IntersectCache &ic, unsigned int flag)
{
	double u = ic.m_u, v = ic.m_v;

	double sqval = sqrt(v*(m_zmax-m_zmin)/m_zmax);
	if (flag&(GeomObj::EVAL_DPDU|GeomObj::EVAL_NORMAL)) {
		if (eq(v, 0.0)) {
			ic.m_dPdu = vector3d::zero;
		}
		else {
			ic.m_dPdu.set_comp(0, -m_rmax*m_tmax*sqval*sin(m_tmax*u));
			ic.m_dPdu.set_comp(1, m_rmax*m_tmax*sqval*cos(m_tmax*u));
			ic.m_dPdu.set_comp(2, 0);
		}
	}

	if (flag & (GeomObj::EVAL_DPDV|GeomObj::EVAL_NORMAL) ) {
		if (eq(v, 0.0)) {
			ic.m_dPdv = vector3d::zero;
		}
		else {
			ic.m_dPdv.set_comp(0, m_rmax*(m_zmax-m_zmin)*cos(m_tmax*u)/(2*m_zmax*sqval));
			ic.m_dPdv.set_comp(1, m_rmax*(m_zmax-m_zmin)*sin(m_tmax*u)/(2*m_zmax*sqval));
			ic.m_dPdv.set_comp(2, m_zmax-m_zmin);
		}
	}

	if (flag & (GeomObj::EVAL_NORMAL) ) {
		if (eq(v, 0.0)) {
			ic.m_normal = -vector3d::unit_y;
		}
		else {
			ic.m_normal = ic.m_dPdv*ic.m_dPdu;
			ic.m_normal.normalize();
		}
	}
}

void ParaboloidObj::Evaluv(IntersectCache &ic, double u, double v, unsigned int flag)
{
}

void ParaboloidObj::CalcBoundingBox()
{
	m_box = new aabb(point3d(-m_rmax, -m_rmax, m_zmin), point3d(m_rmax, m_rmax, m_zmax));
}