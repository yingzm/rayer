#include "common.hxx"
#include "HyperboloidObj.hxx"

#include "ray.hxx"
#include "aabb.hxx"

HyperboloidObj::HyperboloidObj(const point3d &point1, const point3d &point2, double tmax)
{
	m_pt1 = point1; m_pt2 = point2;
	m_tmax = deg2rad(tmax);

	CalcBoundingBox();
}

int HyperboloidObj::IntersectNearest(const ray &r, IntersectCache &ic)
{
	double x0, y0, z0;
	double dx, dy, dz;
	x0 = r.get_orig().x(); y0 = r.get_orig().y(); z0 = r.get_orig().z();
	dx = r.get_dir().x(); dy = r.get_dir().y(); dz = r.get_dir().z();
	double x02=x0*x0, y02=y0*y0, z02=z0*z0;
	double dx2=dx*dx, dy2=dy*dy, dz2=dz*dz;

	double x1, y1, z1;
	double x2, y2, z2;
	x1 = m_pt1.x(); y1=m_pt1.y(); z1=m_pt1.z();
	x2 = m_pt2.x(); y2=m_pt2.y(); z2=m_pt2.z();
	double x12=x1*x1, y12=y1*y1, z12=z1*z1;
	double x22=x2*x2, y22=y2*y2, z22=z2*z2;

	double a, b, c;
	double f = (z2-z1)*(z2-z1);
	a = dx2+dy2+(-dz2*x12+2*dz2*x1*x2-dz2*x22-
		dz2*y12+2*dz2*y1*y2-dz2*y22)/f;
	b = 2*dx*x0+2*dy*y0+(-2*dz*x12*z0+4*dz*x1*x2*z0-
		2*dz*x22*z0-2*dz*y12*z0+4*dz*y1*y2*z0-
		2*dz*y22*z0+2*dz*x12*z1-4*dz*x1*x2*z1+
		2*dz*x22*z1+2*dz*y12*z1-4*dz*y1*y2*z1+
		2*dz*y22*z1)/f+(2*dz*x12-2*dz*x1*x2+2*dz*y12-2*dz*y1*y2)/(z2-z1);
	c = x02-x12+y02-y12+(-x12*z02+2*x1*x2*z02-
		x22*z02-y12*z02+2*y1*y2*z02-y22*z02+
		2*x12*z0*z1-4*x1*x2*z0*z1+2*x22*z0*z1+2*y12*z0*z1-
		4*y1*y2*z0*z1+2*y22*z0*z1-x12*z12+2*x1*x2*z12-
		x22*z12-y12*z12+2*y1*y2*z12-y22*z12)/f+
		(2*x12*z0-2*x1*x2*z0+2*y12*z0-2*y1*y2*z0-
		2*x12*z1+2*x1*x2*z1-2*y12*z1+2*y1*y2*z1)/(z2-z1);

	double t[2];
	int n = solve_quadric(a, b, c, t);
	
	for (int i=0; i<n; ++i) {
		if (t[i]<g_znear || t[i]>g_zfar)
			continue;

		point3d intp = r.get_orig()+t[i]*r.get_dir();

		double v=(intp.z()-z1)/(z2-z1);
		double xr = (1-v)*x1+v*x2, yr=(1-v)*y1+v*y2;

		double tx = (intp.x()*xr+intp.y()*yr)/(xr*xr+yr*yr),
			ty = (intp.y()*xr-intp.x()*yr)/(xr*xr+yr*yr);
		double theta = atan2(ty, tx);
		theta = angle_normalize(theta);
		if (theta>m_tmax)
			continue;

		// ok 
		ic.m_intpt = intp;
		ic.m_param = t[i];
		ic.m_u = theta/m_tmax;
		ic.m_v = v;

		return 1;
	}

	return 0;
}

void HyperboloidObj::Eval(IntersectCache &ic, unsigned int flag)
{
	double u = ic.m_u, v = ic.m_v;

	if (fabs(v-0.5)<0.001) {
		int a = 3;
	}

	if ( flag & (GeomObj::EVAL_DPDU | GeomObj::EVAL_NORMAL) ) {
		ic.m_dPdu.set_comp(0, -m_tmax*((1-v)*m_pt1.x()+v*m_pt2.x())*sin(m_tmax*u)-
			m_tmax*((1-v)*m_pt1.y()+v*m_pt2.y())*cos(m_tmax*u));
		ic.m_dPdu.set_comp(1, -m_tmax*((1-v)*m_pt1.y()+v*m_pt2.y())*sin(m_tmax*u)+
			m_tmax*((1-v)*m_pt1.x()+v*m_pt2.x())*cos(m_tmax*u));
		ic.m_dPdu.set_comp(2, 0);
	}

	if (flag & (GeomObj::EVAL_DPDV | GeomObj::EVAL_NORMAL) ) {
		ic.m_dPdv.set_comp(0, (-m_pt1.x()+m_pt2.x())*cos(m_tmax*u)-
			(-m_pt1.y()+m_pt2.y())*sin(m_tmax*u));
		ic.m_dPdv.set_comp(1, (-m_pt1.y()+m_pt2.y())*cos(m_tmax*u)+
			(-m_pt1.x()+m_pt2.x())*sin(m_tmax*u));
		ic.m_dPdv.set_comp(2, m_pt2.z()-m_pt1.z());
	}

	if (flag & GeomObj::EVAL_NORMAL) {
		ic.m_normal = ic.m_dPdv*ic.m_dPdu;
		ic.m_normal.normalize();
	}
}

void HyperboloidObj::Evaluv(IntersectCache &ic, double u, double v, unsigned int flag)
{
}

void HyperboloidObj::CalcBoundingBox()
{
	double radius = max(sqrt(m_pt1.x()*m_pt1.x()+m_pt1.y()*m_pt1.y()),
		sqrt(m_pt2.x()*m_pt2.x()+m_pt2.y()*m_pt2.y()));
	m_box = new aabb(point3d(-radius, -radius, min(m_pt1.z(), m_pt2.z())),
		point3d(radius, radius, max(m_pt1.z(), m_pt2.z())) );
}