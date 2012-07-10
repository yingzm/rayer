#ifndef __SPHEREOBJ_HXX__
#define __SPHEREOBJ_HXX__

#include "GeomObj.hxx"

class SphereObj : public GeomObj
{
public:
	SphereObj(double radius, double zmin, double zmax, double tmax);
	virtual int IntersectNearest(const ray &r, IntersectCache &ic);
	virtual void Eval(IntersectCache &ic, unsigned int flag);
	virtual void Evaluv(IntersectCache &ic, double u, double v, unsigned int flag);

protected:
	virtual void CalcBoundingBox();

	double m_radius, m_zmin, m_zmax, m_tmax;
	double m_phimin, m_phimax;
};

#endif