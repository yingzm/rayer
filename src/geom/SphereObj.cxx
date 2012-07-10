#include "common.hxx"
#include "SphereObj.hxx"
#include "ray.hxx"
#include "aabb.hxx"

SphereObj::SphereObj(double radius, double zmin, double zmax, double tmax)
{
	m_radius = radius;
	m_zmin = zmin;
	m_zmax = zmax;
	m_tmax = deg2rad(tmax);
	if (m_tmax>0.0) {
		m_tmax = angle_normalize(m_tmax);
	}
	else {
		m_tmax = -(2*M_PI-angle_normalize(m_tmax));
	}

	if (zmin<-radius)
		m_phimin = deg2rad(-90);
	else
		m_phimin = asin(zmin/radius);

	if (zmax>radius)
		m_phimax = deg2rad(90);
	else
		m_phimax = asin(zmax/radius);

	CalcBoundingBox();
}

int SphereObj::IntersectNearest(const ray &r, IntersectCache &ic)
{
	const point3d &p = r.get_orig();
	const vector3d &v = r.get_dir();

	double a, b, c;
	a = v.sqrlength();
	b = 2*(v.x()*p.x()+v.y()*p.y()+v.z()*p.z());
	c = p.sqrlength()-m_radius*m_radius;


	if (eq(a, 0)) {
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

			vector3d n(intp.x(), intp.y(), intp.z());
			n.normalize();

			ic.m_intpt = intp;
			ic.m_param = t[i];
			ic.m_normal = n;
			ic.m_u = theta/m_tmax;
			double phi = asin(intp.z()/m_radius);
			ic.m_v = (phi-m_phimin)/(m_phimax-m_phimin);

			return 1;
		}
		return 0;
	}
}

void SphereObj::Eval(IntersectCache &ic, unsigned int flag)
{
	double theta = ic.m_u*m_tmax;
	double phi = m_phimin+ic.m_v*(m_phimax-m_phimin);
	if (flag&GeomObj::EVAL_DPDU) {
		ic.m_dPdu.set_comp(0, -m_tmax*m_radius*sin(theta)*cos(phi));
		ic.m_dPdu.set_comp(1, m_tmax*m_radius*cos(theta)*cos(phi));
		ic.m_dPdu.set_comp(2, 0.0);
	}

	if (flag&GeomObj::EVAL_DPDV) {
		ic.m_dPdv.set_comp(0, -(m_phimax-m_phimin)*m_radius*cos(theta)*sin(phi));
		ic.m_dPdv.set_comp(1, -(m_phimax-m_phimin)*m_radius*sin(theta)*sin(phi));
		ic.m_dPdv.set_comp(2, (m_phimax-m_phimin)*m_radius*cos(phi));
	}
}

void SphereObj::Evaluv(IntersectCache &ic, double u, double v, unsigned int flag)
{
	double iu = floor(u);
	u = u-iu;
	double iv = floor(v);
	v = v-iv;
	ASSERT(u>=0.0 && u<=1.0);
	ASSERT(v>=0.0 && v<=1.0);

	double phi = m_phimin +v*(m_phimax-m_phimin);
	double theta = u*m_tmax;
	double cosphi = cos(phi), sinphi = sin(phi);
	double costheta = cos(theta), sintheta = sin(theta);

	ic.m_u = u;
	ic.m_v = v;

	if (flag&GeomObj::EVAL_P) {
		ic.m_intpt.set_comp(0, m_radius*costheta*cosphi);
		ic.m_intpt.set_comp(1, m_radius*sintheta*cosphi);
		ic.m_intpt.set_comp(2, m_radius*sinphi);
	}

	if (flag&GeomObj::EVAL_NORMAL) {
		ic.m_normal = ic.m_intpt-point3d::zero;
		ic.m_normal.normalize();
	}

	if (flag&GeomObj::EVAL_DPDU) {
		ic.m_dPdu.set_comp(0, -m_tmax*m_radius*sintheta*cosphi);
		ic.m_dPdu.set_comp(1, m_tmax*m_radius*costheta*cosphi);
		ic.m_dPdu.set_comp(2, 0.0);
	}

	if (flag&GeomObj::EVAL_DPDV) {
		ic.m_dPdv.set_comp(0, -(m_phimax-m_phimin)*m_radius*costheta*sinphi);
		ic.m_dPdv.set_comp(1, -(m_phimax-m_phimin)*m_radius*sintheta*sinphi);
		ic.m_dPdv.set_comp(2, (m_phimax-m_phimin)*m_radius*cosphi);
	}
}

void SphereObj::CalcBoundingBox()
{
	m_box = new aabb(point3d(-m_radius, -m_radius, m_zmin), 
		point3d(m_radius, m_radius, m_zmax));
}