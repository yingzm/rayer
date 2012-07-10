#include "common.hxx"
#include "ShaderVm.hxx"
#include "TextureFile.hxx"

#include "geom.hxx"

void ShaderVm::CallCFunc(int func)
{
	double *f0, *f1, *f2, f;
	point3d *p0, *p1, *p2, p;
	vector3d *v0, *v1, v;
	color3d *c0, *c1, c;
	char *s0, *s1;
	int i;

	m_callparamindex = 0;

	switch(func) {
/////////////////////////////////////////////////////////////////////////
// Mathematical functions
	case 0: // float radians(float degrees)
		f0 = GetCallParamf();
		CallReturnf(deg2rad(*f0));
		break;
	case 1: // float degrees(float radians)
		f0 = GetCallParamf();
		CallReturnf(rad2deg(*f0));
		break;
	case 2: // float sin(float a)
		f0 = GetCallParamf();
		CallReturnf(sin(*f0));
		break;
	case 3: // float asin(float a)
		f0 = GetCallParamf();
		CallReturnf(asin(*f0));
		break;
	case 4: // float cos(float x)
		f0 = GetCallParamf();
		CallReturnf(cos(*f0));
		break;
	case 5: // float acos(float a)
		f0 = GetCallParamf();
		CallReturnf(acos(*f0));
		break;
	case 6: // float tan(float a)
		f0 = GetCallParamf();
		CallReturnf(tan(*f0));
		break;
	case 7: // float atan(float a)
		f0 = GetCallParamf();
		CallReturnf(atan(*f0));
		break;
	case 8: // float atan(float y, float x)
		f0 = GetCallParamf();
		f1 = GetCallParamf();
		CallReturnf(atan2(*f0, *f1));
		break;
	case 9: // float pow(float x, f)
		f0 = GetCallParamf();
		f1 = GetCallParamf();
		CallReturnf(pow(*f0, *f1));
		break;
	case 10: // float exp(float x);
		f0 = GetCallParamf();
		CallReturnf(exp(*f0));
		break;
	case 11: // float sqrt(float x)
		f0 = GetCallParamf();
		CallReturnf(sqrt(*f0));
		break;
	case 12: // float inversesqrt(float x)
		f0 = GetCallParamf();
		CallReturnf(1/sqrt(*f0));
		break;
	case 13: // float log(float x)
		f0 = GetCallParamf();
		CallReturnf(log(*f0));
		break;
	case 14: // float log(float x, float base)
		f0 = GetCallParamf();
		f1 = GetCallParamf();
		CallReturnf(log(*f0)/log(*f1));
		break;
	case 15: // float mod(float a, float b)
		f0 = GetCallParamf();
		f1 = GetCallParamf();
		{
			int n = floor((*f0)/(*f1));
			CallReturnf((*f0)-n*(*f1));
		}
		break;
	case 16: // float abs(x)
		f0 = GetCallParamf();
		CallReturnf(fabs(*f0));
		break;
	case 17: // float sign(float x)
		f0 = GetCallParamf();

		CallReturnf( ((*f0)>0.0)? 1.0 : ( ((*f0)<0.0) ? -1.0 : 0.0) );
		break;
	case 18:	// float min(float a, b, ...)
		f0 = GetCallParamf();
		while ( (f1 = GetCallParamf())!=NULL)
			*f0 = min(*f0, *f1);

		CallReturnf(*f0);
		break;
	case 19: // point min(point a, b, ...)
	case 20: // vector min(vector a, b, ...)
	case 21: // normal min(normal a, b, ...)
	case 22: // color min(color a, b, ...)
		p0 = GetCallParamp();
		while ( (p1 = GetCallParamp())!=NULL)
			*p0 = pmin(*p0, *p1);

		CallReturnp(*p0);
		break;
	case 23:	// float max(float a, b, ...)
		f0 = GetCallParamf();
		while ( (f1 = GetCallParamf())!=NULL)
			*f0 = max(*f0, *f1);

		CallReturnf(*f0);
		break;
	case 24: // point max(point a, b, ...)
	case 25: // vector max(vector a, b, ...)
	case 26: // normal max(normal a, b, ...)
	case 27: // color max(color a, b, ...)
		p0 = GetCallParamp();
		while ( (p1 = GetCallParamp())!=NULL)
			*p0 = pmax(*p0, *p1);

		CallReturnp(*p0);
		break;
	case 28: // float clamp(float a, min, max)
		f0 = GetCallParamf();
		f1 = GetCallParamf();
		f2 = GetCallParamf();
		CallReturnf(clamp(*f0, *f1, *f2));
		break;
	case 29: // point clamp(point a, min, max)
	case 30: // vector clamp(vector a, min, max)
	case 31: // normal clamp(normal a, min, max)
	case 32: // color clamp(color a, min, max)
		p0 = GetCallParamp();
		p1 = GetCallParamp();
		p2 = GetCallParamp();
		p.set_comp(0, clamp(p0->x(), p1->x(), p2->x()));
		p.set_comp(1, clamp(p0->y(), p1->y(), p2->y()));
		p.set_comp(2, clamp(p0->z(), p1->z(), p2->z()));
		CallReturnp(p);
		break;
	case 33:	// float mix(float x,y, float alpha)
		f0 = GetCallParamf();
		f1 = GetCallParamf();
		f2 = GetCallParamf();

		CallReturnf((*f0)*(1-(*f2))+(*f1)*(*f2));
		break;
	case 34:	// point mix(point x, y, float alpha)
	case 35:	// vector mix(vector x, y; float alpha)
	case 36:	// normal mix(normal x, y; float alpha)
	case 37:	// color mix(color x, y; float alpha)
		p0 = GetCallParamp();
		p1 = GetCallParamp();
		f0 = GetCallParamf();

		CallReturnp((*p0)*(1-(*f0))+(*p1)*(*f0));
		break;
	case 38: // float floor(float x)
		f0 = GetCallParamf();
		CallReturnf(floor(*f0));
		break;
	case 39: // float ceil(float x)
		f0 = GetCallParamf();
		CallReturnf(ceil(*f0));
		break;
	case 40: // float round(float x)
		f0 = GetCallParamf();
		CallReturnf(((*f0)>0) ? floor((*f0)+0.5) : ceil((*f0)-0.5));
		break;
	case 41:	// float step(float min, value)
		f0 = GetCallParamf();
		f1 = GetCallParamf();

		CallReturnf( ((*f1)<(*f0))? 0: 1 );
		break;
	case 42:	// float smoothstep(float min, max, value)
		f0 = GetCallParamf();
		f1 = GetCallParamf();
		f2 = GetCallParamf();

		if ((*f2)<=(*f0))
			CallReturnf(0.0);
		else if ((*f2)>=(*f1))
			CallReturnf(1.0);
		else {
			double f = ((*f2)-(*f0))/((*f1)-(*f0));
			CallReturnf(-2*f*f*f+3*f*f);	// TODO: Hermite interpolation
		}
		break;
	case 45: // float spline(float value; float f1, f2, ..., fn, fn1)
		{
			vector<double> values;
			f0 = GetCallParamf();
			while ( (f1=GetCallParamf())!=0 ) {
				values.push_back(*f1);
			}
			int n = values.size();
			double *ptr = new double[n];
			for (int j=0; j<n; ++j)
				ptr[j] = values[j];
			CallReturnf(spline_catmullrom(*f0, n, ptr));
			delete [] ptr;
		}
		break;
	case 46: // float spline(string basis; float value; float f1, f2, ..., fn, fn1)
		{
			vector<double> values;
			s0 = GetCallParams();
			f0 = GetCallParamf();
			while ( (f1=GetCallParamf())!=0 ) {
				values.push_back(*f1);
			}
			int n = values.size();
			double *ptr = new double[n];
			for (int j=0; j<n; ++j) 
				ptr[j] = values[j];
			if (strcmp(s0, "catmull-rom")==0)
				CallReturnf(spline_catmullrom(*f0, n, ptr));
			else if (strcmp(s0, "bezier")==0) 
				CallReturnf(spline_bezier(*f0, n, ptr));
			else if (strcmp(s0, "bspline")==0)
				CallReturnf(spline_bspline(*f0, n, ptr));
			else if (strcmp(s0, "hermite")==0)
				CallReturnf(spline_hermite(*f0, n, ptr));
			else if (strcmp(s0, "linear")==0)
				CallReturnf(spline_linear(*f0, n, ptr));
		}
		break;
	case 47: // float spline(float value; float fvals[])
		{
			f0 = GetCallParamf();
			int n;
			f1 = GetCallParamfa(&n);

			CallReturnf(spline_catmullrom(*f0, n, f1));
		}
		break;
	case 48: // float spline(string basis, float value; float fvals[])
		{
			s0 = GetCallParams();
			f0 = GetCallParamf();
			int n;
			f1 = GetCallParamfa(&n);

			if (strcmp(s0, "catmull-rom")==0)
				CallReturnf(spline_catmullrom(*f0, n, f1));
			else if (strcmp(s0, "bezier")==0) 
				CallReturnf(spline_bezier(*f0, n, f1));
			else if (strcmp(s0, "bspline")==0)
				CallReturnf(spline_bspline(*f0, n, f1));
			else if (strcmp(s0, "hermite")==0)
				CallReturnf(spline_hermite(*f0, n, f1));
			else if (strcmp(s0, "linear")==0)
				CallReturnf(spline_linear(*f0, n, f1));
		}
		break;
	case 49: // color spline(float value; color f1, f2, ..., fn, fn1)
	case 53: // point spline(float value; point p1, p2, ..., pn, pn1)
	case 57: // vector spline(float value; vector v1, v2, ..., vn, vn1)
		{
			vector<color3d> values;
			c0 = GetCallParamc();
			while ( (c1=GetCallParamc())!=0 ) {
				values.push_back(*c1);
			}
			int n = values.size();
			double *ptr = new double[n];
			color3d rc;
			for (int k=0; k<3; ++k) {
				for (int j=0; j<n; ++j)
					ptr[j] = values[j][k];
				rc[k] = spline_catmullrom(*f0, n, ptr);
			}
			CallReturnc(rc);
				
			delete [] ptr;
		}
		break;
	case 50: // color spline(string basis, float value; color f1, f2, ..., fn, fn1)
	case 54: // point spline(string basis; point p1, p2, ..., pn, pn1)
	case 58: // vector spline(string basis; float value; vector v1, v2, ..., vn, vn1)
		{
			s0 = GetCallParams();
			vector<color3d> values;
			c0 = GetCallParamc();
			while ( (c1=GetCallParamc())!=0 ) {
				values.push_back(*c1);
			}
			int n = values.size();
			double *ptr = new double[n];
			color3d rc;
			for (int k=0; k<3; ++k) {
				for (int j=0; j<n; ++j)
					ptr[j] = values[j][k];
				if (strcmp(s0, "catmull-rom")==0)
					rc[k] = spline_catmullrom(*f0, n, ptr);
				else if (strcmp(s0, "bezier")==0) 
					rc[k] = spline_bezier(*f0, n, ptr);
				else if (strcmp(s0, "bspline")==0)
					rc[k] = spline_bspline(*f0, n, ptr);
				else if (strcmp(s0, "hermite")==0)
					rc[k] = spline_hermite(*f0, n, ptr);
				else if (strcmp(s0, "linear")==0)
					rc[k] = spline_linear(*f0, n, ptr);
			}
			CallReturnc(rc);
				
			delete [] ptr;
		}
		break;
	case 51: // color spline(float value; color cvals[])
	case 55: // point spline(float value; point pvals[])
	case 59: // vector spline(float value; vector vvals[]);
		{
			f0 = GetCallParamf();
			int n;
			c0 = GetCallParamca(&n);

			double *ptr = new double[n];
			color3d rc;
			for (int k=0; k<3; ++k) {
				for (int j=0; j<n; ++j)
					ptr[j] = c0[j][k];
				rc[k] = spline_catmullrom(*f0, n, ptr);
			}
			CallReturnc(rc);
				
			delete [] ptr;
		}
		break;
	case 52: // color spline(string basis; float value; color cvals[])
	case 56: // point spline(string basis; float value; point pvals[])
	case 60: // vector spline(string basis; float value; vector vvals[]);
		{
			s0 = GetCallParams();
			f0 = GetCallParamf();
			int n;
			c0 = GetCallParamca(&n);

			double *ptr = new double[n];
			color3d rc;
			for (int k=0; k<3; ++k) {
				for (int j=0; j<n; ++j)
					ptr[j] = c0[j][k];
				if (strcmp(s0, "catmull-rom")==0)
					rc[k] = spline_catmullrom(*f0, n, ptr);
				else if (strcmp(s0, "bezier")==0) 
					rc[k] = spline_bezier(*f0, n, ptr);
				else if (strcmp(s0, "bspline")==0)
					rc[k] = spline_bspline(*f0, n, ptr);
				else if (strcmp(s0, "hermite")==0)
					rc[k] = spline_hermite(*f0, n, ptr);
				else if (strcmp(s0, "linear")==0)
					rc[k] = spline_linear(*f0, n, ptr);
			}
			CallReturnc(rc);
				
			delete [] ptr;
		}
		break;
	case 61: // float Du(float p)
		i = GetCallParamEvalduv();

		CallReturnf(Duf(i));
		break;
	case 62: // float Dv(float p)
		i = GetCallParamEvalduv();
		
		CallReturnf(Dvf(i));
		break;
	case 64: // color Du(color p)
	case 67: // vector Du(point p)
	case 70: // vector Du(vector p)
		i = GetCallParamEvalduv();
		CallReturnv(Duv(i));
		break;
	case 65: // color Dv(color p)
	case 68: // vector Dv(point p)
	case 71: // vector Dv(vector p)
		i = GetCallParamEvalduv();
		CallReturnv(Dvv(i));
		break;
	case 76:	// float noise(float v)
		f0 = GetCallParamf();

		CallReturnf(noise(*f0));
		break;
	case 77:	// float noise(float u, v)
		f0 = GetCallParamf();
		f1 = GetCallParamf();

		CallReturnf(noise(*f0, *f1));
		break;
	case 78:	// float noise(point u)
		p0 = GetCallParamp();
		
		CallReturnf(noise(p0->x(), p0->y(), p0->z()));
		break;
	case 85:	// point noise(float u, v)
		f0 = GetCallParamf();
		f1 = GetCallParamf();

		p.set_comp(0, noise(*f0, *f1));
		p.set_comp(1, noise((*f0)+0.3, (*f1)+0.3));
		p.set_comp(2, noise((*f0)+0.6, (*f1)+0.6));
		CallReturnp(p);
		break;

////////////////////////////////////////////////////////////////////////
	case 124:	// float xcomp(point P)
		p0 = GetCallParamp();
		CallReturnf(p0->x());
		break;
	case 125:
		p0 = GetCallParamp();
		CallReturnf(p0->y());
		break;
	case 126:
		p0 = GetCallParamp();
		CallReturnf(p0->z());
		break;
	case 142:	// float length(vector v)
		v0 = GetCallParamv();

		CallReturnf(v0->length());
		break;
	case 143:	// vector normalize(vector V)
		v0 = GetCallParamv();
		v = *v0;
		v.normalize();

		CallReturnv(v);
		break;
	case 147:	// float area(point P)
		i = GetCallParamEvalduv();

		f = ((Duv(i)*m_ii.m_du)*(Dvv(i)*m_ii.m_dv)).length();

		CallReturnf(f);
		break;
	case 148:	// vector faceforward(vector N, I)
		v0 = GetCallParamv();
		v1 = GetCallParamv();

		if ((*v0)%(*v1)>0)
			CallReturnv(-(*v0));
		else
			CallReturnv((*v0));
		
		break;
	case 154:	// point transform(string tospace, point p)
		s0 = GetCallParams();
		p0 = GetCallParamp();
		{
			const CoordSys &m1 = GetCoordSys(s0);
			p = m1.invmat*(*p0);
			CallReturnp(p);
		}
		break;
	case 155:	// point transform(string fromspace, tospace; point p)
		s0 = GetCallParams();
		s1 = GetCallParams();
		p0 = GetCallParamp();

		{
		const CoordSys &m0 = GetCoordSys(s0);
		const CoordSys &m1 = GetCoordSys(s1);
		p = m1.invmat*m0.mat*(*p0);
		CallReturnp(p);
		}

		break;
	case 158:	// vector vtransform(string tospace; vector v)
		s0 = GetCallParams();
		v0 = GetCallParamv();
		{
			const CoordSys &m1 = GetCoordSys(s0);
			v = m1.invmat*(*v0);
			CallReturnv(v);
		}
		break;
	case 167:	// vector calculatenormal(point P)
		i = GetCallParamEvalduv();

		{
		vector3d v00 = Duv(i);
		vector3d v01 = Dvv(i);
		v = v00*v01;
		v.normalize();
		}
		
		CallReturnv(v);
		break;
	case 183:	// color ambient();
		CallReturnc(CalcAmbientLight());
		break;
	case 184:
		CallReturnc(CalcDiffuseLight());
		break;
	case 185:
		CallReturnc(CalcSpecularLight());
		break;

	case 191:	// color texture(string name);
		s0 = GetCallParams();
		{

		TextureFile *tf = m_texturefm.GetTextureFile(string(s0));
		if (tf!=NULL)
			c = tf->GetAt(GetMemf(REG_LE, SLE_s), GetMemf(REG_LE, SLE_t));
		else
			c = color3d::black;
		}


		CallReturnc(c);
		break;
	case 205:	// solar(vector v, float angle)
		v0 = GetCallParamv();
		f0 = GetCallParamf();

		SetMemv(REG_LE, LLE_L, (*v0));
		m_reg[REG_FLG] |= FLAG_CMP;
		CallReturn();
		break;
	case 206:	// illuminate(point p)
		p0 = GetCallParamp();
		p = GetMemp(REG_LE, LLE_P);

		SetMemv(REG_LE, LLE_L, p-(*p0));
		m_reg[REG_FLG] |= FLAG_CMP;
		CallReturn();
		break;
	case 207:	// illuminate(point p, vector axis, float angle)
		p0 = GetCallParamp();
		v0 = GetCallParamv();
		f0 = GetCallParamf();
		p = GetMemp(REG_LE, LLE_P);

		v = p-(*p0);
		SetMemv(REG_LE, LLE_L, v);
		if (angle_between(v, (*v0))<(*f0)) {
			m_reg[REG_FLG] |= FLAG_CMP;
		}
		else
			m_reg[REG_FLG] &= ~FLAG_CMP;

		CallReturn();
		break;
	default:
		printf("!<NOT IMPLEMTNTED CFUNC %d>\n", func);
		break;
	}
}