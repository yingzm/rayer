#ifndef __DISKOBJ_HXX__
#define __DISKOBJ_HXX__

#include "GeomObj.hxx"

class DiskObj : public GeomObj
{
public:
	DiskObj(double height, double radius, double tmax);
	virtual int IntersectNearest(const ray &r, IntersectCache &ic);
	virtual void Eval(IntersectCache &ic, unsigned int flag);
	virtual void Evaluv(IntersectCache &ic, double u, double v, unsigned int flag);

protected:
	virtual void CalcBoundingBox();

private:
	double m_height, m_radius, m_tmax;
};

#endif