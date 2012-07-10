#include "common.hxx"
#include "PolygonObj.hxx"
#include "ray.hxx"
#include "geom.hxx"
#include "aabb.hxx"

PolygonObj::PolygonObj(int nump, const point3d *pos, vector3d *nml, double *tex,
		int numpol, int *numpapol, int *poses)
{
	if (nump==0 || pos==NULL || m_numpol==0 || numpapol==NULL || poses==NULL) {
		return ;
	}

	int i;
	m_nump = nump;

	m_pos = new point3d[nump];
	for (i=0; i<m_nump; ++i)
		m_pos[i] = pos[i];
	if (nml!=NULL) {
		m_nml = new unit_vector3d[nump];
		for (i=0; i<m_nump; ++i)
			m_nml[i] = normalize(nml[i]);
	}
	else
		m_nml = NULL;
	if (tex!=NULL) {
		m_tex = new double[nump*2];
		memcpy(m_tex, tex, sizeof(double)*2);
	}
	else
		m_tex = NULL;

	m_numpol = numpol;
	m_numpapol = new int[m_numpol];
	memcpy(m_numpapol, numpapol, sizeof(int)*numpol);
	int totalnumpos=0;
	for (i=0; i<numpol; ++i)
		totalnumpos += numpapol[i];
	m_poses = new int[totalnumpos];
	memcpy(m_poses, poses, sizeof(int)*totalnumpos);

	CalcBoundingBox();
}

PolygonObj::~PolygonObj()
{
	delete [] m_pos;
	delete [] m_nml;
	delete [] m_tex;
	
	delete [] m_numpapol;
	delete [] m_poses;
}

int PolygonObj::IntersectNearest(const ray &r, IntersectCache &ic)
{
	double dist, neardist;
	int nearpol;
	int *nearpolidx=0;
	point3d nearp, _intp;
	vector3d nearnormal;
	neardist = 1e10;
	
	int cnump;
	int *cposidx=m_poses;
	bool intesct;
	for (int i=0; i<m_numpol; i++) {
		cnump = m_numpapol[i];
		intesct = IntersectPolygon(_intp, r, cnump, cposidx);
		if (!intesct) {
			cposidx+=cnump;
			continue;
		}
		dist = (_intp-r.get_orig()).length();
		if (dist>=g_znear && dist<=g_zfar && dist<neardist) {
			neardist=dist;
			nearp = _intp;
			nearpol = i;
			nearpolidx=cposidx;
			nearnormal = _lastnormal;
		}
		cposidx+=cnump;
	}
	if (nearpolidx==0)
		return 0;

	_lastintpol = nearpol;
	_lastpolidx = nearpolidx;
	ic.m_intpt = nearp;
	ic.m_param = neardist;
	_lastnormal = nearnormal;

	return 1;
}
	
void PolygonObj::Eval(IntersectCache &ic, unsigned int flag)
{
	// get normal
	if (m_nml==NULL) {
		ic.m_normal = _lastnormal;
		return;
	} 
	
	double x=0, y=0, z=0;
	int i;
	for (i=0; i<m_numpapol[_lastintpol]; i++) {
		x+= m_nml[*_lastpolidx][0];
		y+= m_nml[*_lastpolidx][1];
		z+= m_nml[*_lastpolidx][2];
		_lastpolidx++;
	}
	x/=m_numpapol[_lastintpol];
	y/=m_numpapol[_lastintpol];
	z/=m_numpapol[_lastintpol];

	ic.m_normal = vector3d(x, y, z);

	// get st
	if (!m_tex) {
		ic.m_u = ic.m_intpt.x();
		ic.m_v = ic.m_intpt.y();
		return;
	}
	
	int nnump = m_numpapol[_lastintpol];

	int *pposes = m_poses;
	for (i=0; i<_lastintpol; i++)
		pposes+=m_numpapol[i];
	// get areas of all sub-triangle
	double c1=0, c2=0, c3=0;
	int ii, iii;
	bool rt;
	for (i=0; i<nnump; i++) {
		ii=(i+1)%nnump;
		iii=(i+2)%nnump;
		rt = pt_in_triangle(c1, c2, c3, 
			m_pos[pposes[i]], m_pos[pposes[ii]], m_pos[pposes[iii]], ic.m_intpt);
		if (rt) {
			break;	
		}
	}

	ic.m_u = m_tex[pposes[i]*2]*c1+m_tex[pposes[ii]*2]*c2+m_tex[pposes[iii]*2]*c3;
	ic.m_v = m_tex[pposes[i]*2+1]*c1+m_tex[pposes[ii]*2+1]*c2+m_tex[pposes[iii]*2+1]*c3;
}

void PolygonObj::Evaluv(IntersectCache &ic, double u, double v, unsigned int flag)
{
}

void PolygonObj::CalcBoundingBox()
{
	m_box = new aabb();

	for (int i=0; i<m_nump; ++i) {
		(*m_box) &= m_pos[i];
	}
}

bool PolygonObj::IntersectPolygon(point3d &intp, const ray &r, int numpos, int *posidx)
{
	if (numpos<3)
		return false;

	// get plane normal
	vector3d dir1(m_pos[posidx[2]]-m_pos[posidx[1]]);
	vector3d dir2(m_pos[posidx[0]]-m_pos[posidx[1]]), N;
	N = dir1*dir2;
	N.normalize();

	double d;
	d = -N%(m_pos[posidx[0]]-point3d(0, 0, 0));

	// find t first	
	if (eq(N%r.get_dir(), 0))
		return false;
	double t = -(N%(r.get_orig()-point3d(0, 0, 0))+d)/(N%r.get_dir());
	if (t<g_znear || t>g_zfar)
		return false;

	// thus find intersection with  plane
	vector3d _dir = r.get_dir();
	intp =  r.get_orig()+_dir*t;
	_lastnormal = N;

	// check if intersection is in polygon
	double alpha, totalalpha=0;
	int ii;
	for (int i=0; i<numpos; i++) {
		ii=(i+1)%numpos;
		vector3d dd1(m_pos[posidx[i]]-intp), dd2(m_pos[posidx[ii]]-intp);
		dd1.normalize(); dd2.normalize();
		alpha = acos(dd1%dd2);
		totalalpha += alpha;
	}

	if (fabs(totalalpha-2*M_PI)<POLYGON_ALPHA_EPSILON)
		return true;
	else
		return false;
}
