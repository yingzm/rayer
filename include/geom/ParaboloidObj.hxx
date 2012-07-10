#ifndef __PARABOLOIDOBJ_HXX__
#define __PARABOLOIDOBJ_HXX__

#include "GeomObj.hxx"

class ParaboloidObj : public GeomObj
{
public:
	ParaboloidObj(double rmax, double zmin, double zmax, double tmax);

	virtual int IntersectNearest(const ray &r, IntersectCache &ic);
	virtual void Eval(IntersectCache &ic, unsigned int flag);
	virtual void Evaluv(IntersectCache &ic, double u, double v, unsigned int flag);

protected:
	virtual void CalcBoundingBox();

private:
	double m_rmax, m_zmin, m_zmax, m_tmax;
};

#endif 