#include "common.hxx"
#include "RGeomNode.hxx"
#include "GeomObj.hxx"
/*
RAttribute::RAttribute()
{
	surface = displacement = atmosphere = NULL;
}
*/
void RAttribute::Copy(const RAttribute &ra)
{
	if (&ra==this)
		return;

	this->color = ra.color;
	this->opacity = ra.opacity;
	this->matte = ra.matte;

	this->surface = ra.surface;
	this->displacement = ra.displacement;
	this->atmosphere = ra.atmosphere;

	this->shadingrate = ra.shadingrate;
	this->sides = ra.sides;
	this->shadinginterpolation = ra.shadinginterpolation;
	this->orientation = ra.orientation;
	this->geometricapproximation_type = ra.geometricapproximation_type;
	this->geometricapproximation_value = ra.geometricapproximation_value;
}

RGeomNode::RGeomNode()
{
	m_geomobj = NULL;
	m_globalmatrix = identity_matrix();
}

RGeomNode::~RGeomNode()
{
	delete m_geomobj;
}