#include "common.hxx"

#include "view.hxx"

BEGIN_NAMESPACE

inline void view::adjust_viewup()
{
	if (is_perpendicular(m_viewdir, m_viewup))
		return;

	if (is_biparallel(m_viewdir, m_viewup)) {
		printf("Coincide of view direction and view up direction(%f, %f, %f).\n",
			m_viewup[0], m_viewup[1], m_viewup[2]);
	}

	// or up direction need adjust, to be perpendicular with m_viewdir
	unit_vector3d viewright;
	viewright = (m_viewdir*m_viewup);

	m_viewup = viewright*m_viewdir;
}

END_NAMESPACE