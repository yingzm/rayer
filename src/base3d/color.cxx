#include "common.hxx"
#include "color.hxx"

const color3d color3d::black(0.0, 0.0, 0.0);
const color3d color3d::red(1.0, 0.0, 0.0);
const color3d color3d::green(0.0, 1.0, 0.0);
const color3d color3d::blue(0.0, 0.0, 1.0);
const color3d color3d::white(1.0, 1.0, 1.0);

void color3d::clamp(clamp_mode cm)
{
	switch (cm) {
	case clamp_cut:
		if (comp[0]>1.0)
			comp[0] = 1.0;
		if (comp[1]>1.0)
			comp[1] = 1.0;
		if (comp[2]>1.0)
			comp[2] = 1.0;
		if (comp[0]<0.0)
			comp[0] = 0.0;
		if (comp[1]<0.0)
			comp[1] = 0.0;
		if (comp[2]<0.0)
			comp[2] = 0.0;
		break;
	case clamp_scale:
		// TODO
		break;
	}
}

color3d clamp(const color3d &c, color3d::clamp_mode cm)
{
	color3d c0 = c;
	c0.clamp(cm);

	return c0;
}