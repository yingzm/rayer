#ifndef __TORUSOBJ_HXX__
#define __TORUSOBJ_HXX__

#include "GeomObj.hxx"

class TorusObj : public GeomObj
{
public:
	TorusObj(double majrad, double minrad, double phimin, double phimax, double tmax);

	virtual int IntersectNearest(const ray &r, IntersectCache &ic);
	virtual void Eval(IntersectCache &ic, unsigned int flag);
	virtual void Evaluv(IntersectCache &ic, double u, double v, unsigned int flag);

protected:
	virtual void CalcBoundingBox();

private:
	double m_majrad, m_minrad, m_phimin, m_phimax, m_tmax;
};

#endif 