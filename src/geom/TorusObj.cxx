#include "common.hxx"
#include "TorusObj.hxx"

#include "ray.hxx"
#include "math.hxx"
#include "aabb.hxx"

TorusObj::TorusObj(double majrad, double minrad, double phimin, double phimax, double tmax)
{
	m_majrad = majrad;
	m_minrad = minrad; 
	phimin = deg2rad(phimin);
	phimax = deg2rad(phimax);
	if (phimin>phimax) {
		m_phimin = phimax;
		m_phimax = phimin;
	}
	else {
		m_phimin = phimin;
		m_phimax = phimax;
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

int TorusObj::IntersectNearest(const ray &ry, IntersectCache &ic)
{
	double x0, y0, z0;
	double dx, dy, dz;
	double R, r;
	x0 = ry.get_orig().x(); y0 = ry.get_orig().y(); z0 = ry.get_orig().z();
	dx = ry.get_dir().x(); dy = ry.get_dir().y(); dz = ry.get_dir().z();
	R = m_majrad;
	r = m_minrad;

	double x02=x0*x0, x03=x02*x0, x04=x03*x0;
	double y02=y0*y0, y03=y02*y0, y04=y03*y0;
	double z02=z0*z0, z03=z02*z0, z04=z03*z0;
	double dx2=dx*dx, dx3=dx2*dx, dx4=dx3*dx;
	double dy2=dy*dy, dy3=dy2*dy, dy4=dy3*dy;
	double dz2=dz*dz, dz3=dz2*dz, dz4=dz3*dz;
	double R2=R*R, R3=R2*R, R4=R3*R;
	double r2=r*r, r3=r2*r, r4=r3*r;
	double a, b, c, d, e;
	a = -dx4-2*dx2*dy2-dy4-2*dx2*dz2-2*dy2*dz2-dz4;
	b = -4*dx3*x0-4*dx*dy2*x0-4*dx*dz2*x0-4*dx2*dy*y0-
		4*dy3*y0-4*dy*dz2*y0-4*dx2*dz*z0-4*dy2*dz*z0-4*dz3*z0;
	c = 2*dx2*r2+2*dy2*r2+2*dz2*r2+2*dx2*R2+2*dy2*R2-2*dz2*R2-
		6*dx2*x02-2*dy2*x02-2*dz2*x02-8*dx*dy*x0*y0-
		2*dx2*y02-6*dy2*y02-2*dz2*y02-8*dx*dz*x0*z0-
		8*dy*dz*y0*z0-2*dx2*z02-2*dy2*z02-6*dz2*z02;
	d = 4*dx*r2*x0+4*dx*R2*x0-4*dx*x03+4*dy*r2*y0+4*dy*R2*y0-
		4*dy*x02*y0-4*dx*x0*y02-4*dy*y03+4*dz*r2*z0-4*dz*R2*z0-
		4*dz*x02*z0-4*dz*y02*z0-4*dx*x0*z02-4*dy*y0*z02-4*dz*z03;
	e = -r4+2*r2*R2-R4+2*r2*x02+2*R2*x02-x04+2*r2*y02+2*R2*y02-
		2*x02*y02-y04+2*r2*z02-2*R2*z02-2*x02*z02-2*y02*z02-z04;

	double rts[4], rterr[4];
	int n = solve_quartic(a, b, c, d, e, rts, rterr);

	// sort
	int i,j;
	for (i=0; i<n; ++i)
	{
		for (j=i+1; j<n; ++j) {
			if (rts[i]>rts[j]) {
				double tmp = rts[j];
				rts[j] = rts[i];
				rts[i] = tmp;
			}
		}
	}

	point3d intp;
	double phi, theta;
	for (i=0; i<n; ++i) {
		if (rts[i]<g_znear || rts[i]>g_zfar)
			continue;

		intp = ry.get_orig()+rts[i]*ry.get_dir();

		theta = atan2(intp.y(), intp.x());
		theta = angle_normalize(theta);
		if (m_tmax<0)
			theta = -(2*M_PI-theta);
		if (fabs(theta)>fabs(m_tmax))
			continue;

		double rr = intp.x()/cos(theta)-R;
		phi = asin(intp.z()/r);
		if (rr<0 && phi>0)
			phi = M_PI-phi;
		else if (rr<0 && phi<0)
			phi = M_PI+phi;
		else if (rr>0 && phi<0)
			phi = M_PI*2+phi;

		if (phi<m_phimin || phi>m_phimax)
			continue;

		// ok, this is a solution
		double t = rts[i];
		vector3d vxy(intp.x(), intp.y(), 0);
		vxy.normalize();
		double dist = sqrt(intp.x()*intp.x()+intp.y()*intp.y());
		vector3d vz(0, 0, intp.z()/(dist-R));
		vector3d N = vxy+vz;
		N.normalize();

		ic.m_intpt = intp;
		ic.m_param = t;
		ic.m_normal = N;
		ic.m_u = theta/m_tmax;
		ic.m_v = (phi-m_phimin)/(m_phimax-m_phimin);

		return 1;
	}

	return 0;
}

void TorusObj::Eval(IntersectCache &ic, unsigned int flag)
{

}

void TorusObj::Evaluv(IntersectCache &ic, double u, double v, unsigned int flag)
{
}

void TorusObj::CalcBoundingBox()
{
	double r = m_majrad+m_minrad;
	m_box = new aabb(point3d(-r, -r, -m_minrad), point3d(r, r, m_minrad));
}