#ifndef __GEOMOBJ_HXX__
#define __GEOMOBJ_HXX__

#include "point.hxx"
#include "bbox.hxx"

class ray;

class IntersectCache
{
public:
	point3d m_intpt;	// the intersection point
	double m_param;		// parameter of the intersection point on the RAY
	vector3d m_normal;	// normal is not normalized, according to Renderman Spec 3.2
	vector3d m_dPdu, m_dPdv;	// first derivative
	double m_u, m_v;
};

class GeomObj
{
public:
	GeomObj()	{ m_box=NULL; }
	virtual ~GeomObj() { if (m_box!=NULL) delete m_box; }

	const bbox *GetBoundingBox() const { return m_box; }
	// IntersectNearest is used by ray tracing,
	// it can't be const because derived GeomObj may have some cache value
	virtual int IntersectNearest(const ray &r, IntersectCache &ic) = 0;

	/*
	 * Fill in IntersectCache data using the intpt,
	 * this function does not check if the point is on the object or not,
	 * so it is the programmers' responsibility to ensure ic is the
	 * returned value of the same object.
	 */
	enum {
		EVAL_P = 1,
		EVAL_U = 2,
		EVAL_V = 4,
		EVAL_DPDU = 8,
		EVAL_DPDV = 0x10,
		EVAL_NORMAL = 0x20,
	};
	virtual void Eval(IntersectCache &ic, unsigned int flag) = 0;

	virtual void Evaluv(IntersectCache &ic, double u, double v, unsigned int flag) = 0;

protected:
	virtual void CalcBoundingBox() = 0;
	
protected:
	bbox *m_box;
};

extern double g_znear, g_zfar;

#endif /* __GEOMOBJ_HXX__ */