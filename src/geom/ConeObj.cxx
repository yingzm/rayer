#include "common.hxx"
#include "ConeObj.hxx"

#include "ray.hxx"
#include "aabb.hxx"

double g_znear, g_zfar;

ConeObj::ConeObj(double height, double radius, double tmax)
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

int ConeObj::IntersectNearest(const ray &r, IntersectCache &ic)
{
	double x0, y0, z0;
	double dx, dy, dz;
	x0 = r.get_orig().x(); y0 = r.get_orig().y(); z0 = r.get_orig().z();
	dx = r.get_dir().x(); dy = r.get_dir().y(); dz = r.get_dir().z();

	double r2 = m_radius*m_radius, h2 = m_height*m_height;
	double a, b, c;
	c = -r2+x0*x0+y0*y0+(2*r2*z0)/m_height-(r2*z0*z0)/h2;
	b = (2*dz*r2)/m_height+2*dx*x0+2*dy*y0-(2*dz*r2*z0)/h2;
	a = dx*dx+dy*dy-dz*dz*r2/h2;

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

			point3d intp = r.get_orig()+t[i]*r.get_dir();
			if ( (m_height>0) && (intp.z()<0 || intp.z()>m_height) ||
				(m_height<0) && (intp.z()>0 || intp.z()<m_height) )
				continue;

			double theta = atan2(intp.y(), intp.x());
			theta = angle_normalize(theta);
			if (m_tmax<0)
				theta = -(2*M_PI-theta);
			if (fabs(theta)>fabs(m_tmax))
				continue;

			vector3d v1(intp.x(), intp.y(), 0);
			v1.normalize();
			vector3d v2(0, 0, m_radius/m_height);
			vector3d n = v1+v2;
			n.normalize();

			ic.m_intpt = intp;
			ic.m_param = t[i];
			ic.m_normal = n;
			ic.m_u = theta/m_tmax;
			ic.m_v = intp.z()/m_height;

			return 1;
		}

		return 0;
	}
}

void ConeObj::Eval(IntersectCache &ic, unsigned int flag)
{
	double v = ic.m_v, u = ic.m_u;

	if (flag&GeomObj::EVAL_DPDU) {
		ic.m_dPdu.set_comp(0, -m_radius*m_tmax*(1-v)*sin(m_tmax*u));
		ic.m_dPdu.set_comp(1, m_radius*m_tmax*(1-v)*cos(m_tmax*u));
		ic.m_dPdu.set_comp(2, 0);
	}

	if (flag&GeomObj::EVAL_DPDV) {
		ic.m_dPdv.set_comp(0, -m_radius*cos(m_tmax*u));
		ic.m_dPdv.set_comp(1, -m_radius*sin(m_tmax*u));
		ic.m_dPdv.set_comp(2, m_height);
	}
}

void ConeObj::Evaluv(IntersectCache &ic, double u, double v, unsigned int flag)
{
}

void ConeObj::CalcBoundingBox()
{
	m_box = new aabb(point3d(-m_radius, -m_radius, min(0.0, m_height)), 
		point3d(m_radius, m_radius, max(0.0, m_height)));
}