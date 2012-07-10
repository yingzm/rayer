#ifndef __HYPERBOLOIDOBJ_HXX__
#define __HYPERBOLOIDOBJ_HXX__

#include "GeomObj.hxx"

class HyperboloidObj : public GeomObj
{
public:
	HyperboloidObj(const point3d &point1, const point3d &point2, double tmax);

	virtual int IntersectNearest(const ray &r, IntersectCache &ic);
	virtual void Eval(IntersectCache &ic, unsigned int flag);
	virtual void Evaluv(IntersectCache &ic, double u, double v, unsigned int flag);

protected:
	virtual void CalcBoundingBox();

private:
	point3d m_pt1, m_pt2;
	double m_tmax;
};

#endif 