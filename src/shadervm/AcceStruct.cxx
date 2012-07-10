#include "common.hxx"
#include "AcceStruct.hxx"

#include "RGeomNode.hxx"
#include "ray.hxx"

bool AcceStruct::IntersectNearest(const ray &r, IntersectInfo &ii)
{
	vector<RGeomNode *>::iterator it = m_geomnode.begin();

	RGeomNode *gn = NULL;
	IntersectCache ic, icnearest;
	icnearest.m_param = DBL_MAX;
	ray r0;

	for (; it<m_geomnode.end(); ++it) {
		GeomObj *go = (*it)->GetGeomObj();
		matrix4d m = (*it)->GetMatrix();
		m.inverse();
		r0 = r*m;
		const bbox *box = go->GetBoundingBox();
		if (box->intersect(r0)) {
			if (go->IntersectNearest(r0, ic)) {
				// the returned ic.m_param is local parameter, and
				// can't be used as oclusion culling
				vector3d v = (*it)->GetMatrix()*ic.m_intpt-r.get_orig();
				ic.m_param = v%r.get_dir();
				if(ic.m_param<icnearest.m_param) {
					icnearest = ic;
					gn = *it;		
				}
			}
		}
	}

	if (gn==NULL) {
		ii.m_node=NULL;
		return false;
	}

	ii.m_node = gn;
	ii.m_ic = icnearest;

	return true;
}

void AcceStruct::AddGeomObj(GeomObj *go, RAttribute *si, const matrix4d &m)
{
	RGeomNode *gn = new RGeomNode();
	gn->SetGeomObj(go);
	gn->SetAttribute(si);
	gn->SetMatrix(m);
	m_geomnode.push_back(gn);
}

void AcceStruct::RemoveAll()
{
	int n = m_geomnode.size();
	for (int i=0; i<n; ++i) {
		RGeomNode *gn = m_geomnode[i];
		delete gn;
	}
	m_geomnode.erase(m_geomnode.begin(), m_geomnode.end());
}