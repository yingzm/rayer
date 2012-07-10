#include "common.hxx"
#include "CylinderObj.hxx"
#include "ray.hxx"
#include "aabb.hxx"

CylinderObj::CylinderObj(double radius, double zmin, double zmax, double tmax)
{
	m_radius = radius; 
	if (zmin>zmax) {
		m_zmin = zmax;
		m_zmax = zmin;
	}
	else {
		m_zmin = zmin;
		m_zmax = zmax;
	}
	m_tmax = deg2rad(tmax);
	if (m_tmax>0.0) {
		m_tmax = angle_normalize(m_tmax);
	}
	else {
		m_tmax = -(2*M_PI-angle_normalize(m_tmax));
	}

	CalcBoundingBox();
}

int CylinderObj::IntersectNearest(const ray &r, IntersectCache &ic)
{
	const point3d &p = r.get_orig();
	const vector3d &v = r.get_dir();

	// get a quadric equation
	double a = v.x()*v.x()+v.y()*v.y(),
		b = 2*p.x()*v.x()+2*p.y()*v.y(),
		c = p.x()*p.x()+p.y()*p.y()-m_radius*m_radius;

	if (eq(a, 0)) {
		// TODO: linear function
		return 0;
	}
	else {
		double t[2];
		int n = solve_quadric(a, b, c, t);

		for (int i=0; i<n; ++i) {
                        if (t[i]<g_znear || t[i]>g_zfar)
				continue;

			point3d intp = p+t[i]*v;
			if (intp.z()<m_zmin || intp.z()>m_zmax)
				continue;

			double theta = atan2(intp.y(), intp.x());
			theta = angle_normalize(theta);
			if (m_tmax<0)
				theta = -(2*M_PI-theta);
			if (fabs(theta)>fabs(m_tmax))
				continue;

			vector3d n = intp-point3d(0, 0, intp.z());
			n.normalize();

			ic.m_intpt = intp;
			ic.m_param = t[i];
			ic.m_normal = n;
			ic.m_u = theta/m_tmax;
			ic.m_v = (intp.z()-m_zmin)/(m_zmax-m_zmin);

			return 1;
		}
		return 0;
	}
}

void CylinderObj::Eval(IntersectCache &ic, unsigned int flag)
{
	double u = ic.m_u, v= ic.m_v;

	if (flag & GeomObj::EVAL_DPDU) {
		ic.m_dPdu.set_comp(0, -m_radius*m_tmax*sin(m_tmax*u));
		ic.m_dPdu.set_comp(1, m_radius*m_tmax*cos(m_tmax*u));
		ic.m_dPdu.set_comp(2, 0);
	}

	if (flag &GeomObj::EVAL_DPDV) {
		ic.m_dPdv.set_comp(0, 0);
		ic.m_dPdv.set_comp(1, 0);
		ic.m_dPdv.set_comp(2, m_zmax-m_zmin);
	}
}

void CylinderObj::Evaluv(IntersectCache &ic, double u, double v, unsigned int flag)
{
}


void CylinderObj::CalcBoundingBox()
{
	m_box = new aabb(point3d(-m_radius, -m_radius, m_zmin),
		point3d(m_radius, m_radius, m_zmax));
}