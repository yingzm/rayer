#ifndef __ACCESTRUCT_HXX__
#define __ACCESTRUCT_HXX__

#include "GeomObj.hxx"
#include "color.hxx"
#include "ray.hxx"

class GeomObj;
class RAttribute;
class RGeomNode;
class ray;
class matrix4d;

class IntersectInfo
{
public:
	ray m_ray;
	RGeomNode *m_node;
	IntersectCache m_ic;
	vector3d m_I;		// incident ray direction
	double m_du, m_dv, m_s, m_t;
};

class AcceStruct
{
public:
	~AcceStruct() { RemoveAll(); }
	virtual bool IntersectNearest(const ray &r, IntersectInfo &ii);

	void AddGeomObj(GeomObj *go, RAttribute *si, const matrix4d &m);

	void RemoveAll();

private:
	vector<RGeomNode *> m_geomnode;
};

#endif /* __ACCESTRUCT_HXX__ */
