#ifndef __POLYGONOBJ_HXX__
#define __POLYGONOBJ_HXX__

#include "point.hxx"
#include "unit_vector.hxx"
#include "vector.hxx"

#include "GeomObj.hxx"

class PolygonObj : public GeomObj
{
public:
	int m_nump;	// number of vertices
	point3d *m_pos;	// the shared position
	unit_vector3d *m_nml;	// the shared normal
	double *m_tex;	// the shared s,t coordinates
	int m_numpol;	// number of polygons
	int *m_numpapol;	// number of vertices for each polygon
	int *m_poses;	// vertices indexes for each polygon

	int _lastintpol;	// last intersected polygon, used for 
	int *_lastpolidx;
	vector3d _lastnormal;
public:
	PolygonObj(int nump, const point3d *pos, vector3d *nml, double *tex,
		int numpol, int *numpapol, int *poses);
	~PolygonObj();
	virtual int IntersectNearest(const ray &r, IntersectCache &ic);
	virtual void Eval(IntersectCache &ic, unsigned int flag);
	virtual void Evaluv(IntersectCache &ic, double u, double v, unsigned int flag);

protected:
	virtual void CalcBoundingBox();

private:
	bool IntersectPolygon(point3d &intp, const ray &r, int numpos, int *posidx);
};

#endif