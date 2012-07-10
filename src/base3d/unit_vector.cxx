#include "common.hxx"

#include "unit_vector.hxx"

BEGIN_NAMESPACE

const unit_vector3d unit_vector3d::zero(0.0, 0.0, 0.0);
const unit_vector3d unit_vector3d::unit_x(1.0, 0.0, 0.0);
const unit_vector3d unit_vector3d::unit_y(0.0, 1.0, 0.0);
const unit_vector3d unit_vector3d::unit_z(0.0, 0.0, 1.0);


void unit_vector3d::normalize()
{
	double len = length();
	if (eq(len, 0.0)) {
		comp[0] = comp[1] = comp[2] = 0.0;
		return;
	}

	comp[0]/=len; comp[1]/=len; comp[2]/=len;
}

unit_vector3d normalize(const vector3d &v)
{
	return unit_vector3d(v[0], v[1], v[2]);
}


END_NAMESPACE