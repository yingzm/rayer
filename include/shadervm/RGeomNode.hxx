#ifndef __RGEOMNODE_HXX__
#define __RGEOMNODE_HXX__

#include "matrix.hxx"
#include "color.hxx"
#include "ShaderInst.hxx"

class GeomObj;

typedef int Rint;
typedef double Rfloat;
typedef unsigned char Ru8;

#define RFLOAT_EPSILON DBL_EPSILON
#define RFLOAT_INFINITY DBL_INFINITE

class RAttribute
{
public:
	// Color attributes
	color4d color;
	color4d opacity;
	Ru8 matte;
	// Shaders
	ShaderInst *surface;
	ShaderInst *displacement;
	ShaderInst *atmosphere;
	// Other shading attributes
	Rfloat shadingrate;
	enum {
		SHADERINTP_CONSTANT,
		SHADERINTP_SMOOTH
	};
	Ru8 shadinginterpolation;
	// Sideness
	Ru8 sides;
	enum {
		ORIENTATION_OUTSIDE,
		ORIENTATION_INSIDE
	};
	Ru8 orientation;
	// Renderer Control
	Ru8 geometricapproximation_type;
	Rfloat geometricapproximation_value;

	// RAttribute();
	void Copy(const RAttribute &ra);
};


class RGeomNode
{
public:
	RGeomNode();
	~RGeomNode();

	void SetGeomObj(GeomObj *go) { m_geomobj = go; }
	void SetAttribute(RAttribute *so) { m_attr.Copy(*so); }
	void SetMatrix(const matrix4d &m) { m_globalmatrix = m; }

	GeomObj *GetGeomObj() { return m_geomobj; }
	RAttribute *GetAttribute() { return &m_attr; }
	const matrix4d &GetMatrix() { return m_globalmatrix; }

private:
	GeomObj *m_geomobj;
	RAttribute m_attr;
	matrix4d m_globalmatrix;
};

#endif /* __RGEOMNODE_HXX__ */