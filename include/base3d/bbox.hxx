#ifndef __BBOX_HXX__
#define __BBOX_HXX__

class point3d;
class ray;

BEGIN_NAMESPACE

class bbox
{
public:
//	virtual bbox operator |(const bbox &bx)=0;
//	virtual bbox operator &(const bbox &bx)=0;

	virtual const bbox &operator &=(const point3d &p)=0;

	virtual bool isin(const point3d &pnt) const=0;

	virtual bool intersect(const ray &r) const=0;
};

END_NAMESPACE



#endif /* __BBOX_HXX__  */
