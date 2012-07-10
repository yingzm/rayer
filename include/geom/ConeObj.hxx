#ifndef __CONEOBJ_HXX__
#define __CONEOBJ_HXX__

#include "GeomObj.hxx"

class ConeObj : public GeomObj
{
public:
	ConeObj(double height, double radius, double tmax);
	virtual int IntersectNearest(const ray &r, IntersectCache &ic);
	virtual void Eval(IntersectCache &ic, unsigned int flag);
	virtual void Evaluv(IntersectCache &ic, double u, double v, unsigned int flag);

protected:
	virtual void CalcBoundingBox();

private:
	double m_radius, m_height, m_tmax;
};

#endif 