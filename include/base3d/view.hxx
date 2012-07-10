#ifndef __VIEW_HXX__
#define __VIEW_HXX__

#include "math.hxx"
#include "point.hxx"
#include "vector.hxx"
#include "unit_vector.hxx"

BEGIN_NAMESPACE

class view
{
typedef enum {
	ortho,
	perspective
} projection_mode;
public:
	view();

	void set_projection(projection_mode proj);
	void set_screen(unsigned int *scr);
	void set_fov(double fov);
	void set_ori(const point3d &ori);
	void set_viewdir(const unit_vector3d &dir);
	void set_viewup(const unit_vector3d &dir);
	void set_znear(double d);
	void set_zfar(double d);

	projection_mode get_projection();
	void get_screen(unsigned int *scr);
	double get_fov();
	point3d get_ori();
	unit_vector3d get_viewdir();
	unit_vector3d get_viewup();
	double get_znear();
	double get_zfar();

protected:
	projection_mode m_projection;
	unsigned int m_screen[4];
	double m_fov;	// y direction fov, upper edge to lower edge, 
			// for perspective only
	double m_znear, m_zfar;
	point3d m_ori;
	unit_vector3d m_viewdir;
	unit_vector3d m_viewup;

	void adjust_viewup();
};

inline view::view()
: m_ori(0, 0, 5), m_viewdir(0, 0, -1), m_viewup(0, 1, 0)
{
	m_projection = perspective;
	m_screen[0] = m_screen[2] = -1; m_screen[1] = m_screen[3] = 1;
	m_fov = deg2rad(60);
	m_znear = 0.1; m_zfar = 10000;
}

inline void view::set_projection(projection_mode proj)
{
	m_projection = proj;
}

inline void view::set_screen(unsigned int *scr)
{
	m_screen[0] = scr[0]; m_screen[1] = scr[1];
	m_screen[2] = scr[2]; m_screen[3] = scr[3];
}

inline void view::set_fov(double fov)
{
	if (fov<=0 || fov>=M_PI) {
		printf("Invalid field of view(%f) in view::set_fov.\n", fov);
	}

	m_fov = fov;
}

inline void view::set_ori(const point3d &ori)
{
	m_ori = ori;
}

inline void view::set_viewdir(const unit_vector3d &dir)
{
	m_viewdir = dir;

	adjust_viewup();
}

inline void view::set_viewup(const unit_vector3d &dir)
{
	m_viewup = dir;

	adjust_viewup();
}

inline void view::set_znear(double d)
{
	m_znear = d;
}

inline void view::set_zfar(double d)
{
	m_zfar = d;
}

inline view::projection_mode view::get_projection()
{
	return m_projection;
}

inline void view::get_screen(unsigned int *scr)
{
	scr[0] = m_screen[0]; scr[1] = m_screen[1];
	scr[2] = m_screen[2]; scr[3] = m_screen[3];
}

inline double view::get_fov()
{
	return m_fov;
}

inline point3d view::get_ori()
{
	return m_ori;
}

inline unit_vector3d view::get_viewdir()
{
	return m_viewdir;
}

inline unit_vector3d view::get_viewup()
{
	return m_viewup;
}

inline double view::get_znear()
{
	return m_znear;
}

inline double view::get_zfar()
{
	return m_zfar;
}


END_NAMESPACE

#endif /* __VIEW_HXX */
