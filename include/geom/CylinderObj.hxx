#ifndef __CYLINDEROBJ_HXX__
#define __CYLINDEROBJ_HXX__

#include "GeomObj.hxx"

class CylinderObj : public GeomObj
{
public:
	CylinderObj(double radius, double zmin, double zmax, double tmax);
	virtual int IntersectNearest(const ray &r, IntersectCache &ic);
	virtual void Eval(IntersectCache &ic, unsigned int flag);
	virtual void Evaluv(IntersectCache &ic, double u, double v, unsigned int flag);

protected:
	virtual void CalcBoundingBox();

private:
	double m_radius, m_zmin, m_zmax, m_tmax;
	bool m_ccw;
};

#endif