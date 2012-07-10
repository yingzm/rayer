#include "common.hxx"
#include "Ri.hxx"

#include "ray.hxx"

#include "ShaderVm.hxx"
#include "ShaderInst.hxx"

#include "SphereObj.hxx"
#include "ConeObj.hxx"
#include "TorusObj.hxx"
#include "CylinderObj.hxx"
#include "DiskObj.hxx"
#include "HyperboloidObj.hxx"
#include "ParaboloidObj.hxx"
#include "PolygonObj.hxx"
#include "ShaderFile.hxx"

#include "tiffio.h"

#include <time.h>

RtVoid Ri::RiBegin(RtToken name)
{
	printf("@<Ri::RiBegin>: Begin rendering %s\n", name);

	// default values for global attributes and options
	m_fb = NULL;
	m_opfb = NULL;
	m_framebegin = 0;


	m_default_obj_attribute.color = color3d(1.0, 1.0, 1.0);
	m_default_obj_attribute.opacity = color3d(1.0, 1.0, 1.0);
	m_default_obj_attribute.matte = 0;
	m_default_obj_attribute.surface = NULL;
	m_default_obj_attribute.displacement = NULL;
	m_default_obj_attribute.atmosphere = NULL;
	m_default_obj_attribute.shadingrate = 1.0f;
	m_default_obj_attribute.shadinginterpolation = RAttribute::SHADERINTP_CONSTANT;
	m_default_obj_attribute.sides = 1;
	m_default_obj_attribute.orientation = RAttribute::ORIENTATION_OUTSIDE;
	m_default_obj_attribute.geometricapproximation_type = 0;
	m_default_obj_attribute.geometricapproximation_value = 1.0f;

	CoordSys cs;
	cs.mat = identity_matrix();
	cs.invmat = identity_matrix();
	cs.name = string("raster"); m_coordsys.push_back(cs);
	cs.name = string("NDC"); m_coordsys.push_back(cs);
	cs.name = string("screen"); m_coordsys.push_back(cs);
	cs.name = string("camera"); m_coordsys.push_back(cs);
	cs.name = string("world"); m_coordsys.push_back(cs);
	cs.name = string("object"); m_coordsys.push_back(cs);
	cs.name = string("shader"); m_coordsys.push_back(cs);

	m_attr = m_default_obj_attribute;
	m_trans = identity_matrix();

	m_sv = new ShaderVm();
	m_sv->Begin();
}

RtVoid Ri::RiEnd()
{
	m_sv->End();

	delete m_sv;

	int n = m_shaders.size();
	for (int i=0; i<n; ++i) {
		ShaderInst *si = m_shaders[i];
		delete si;
	}
}

RtVoid Ri::RiFrameBegin(RtInt frame)
{
	m_sv->FrameBegin(frame);

	m_framebegin=1;

	if (m_fb==NULL) {
		int size = m_sv->m_options.format_xres*m_sv->m_options.format_yres;
		m_fb = new color3d[size];
		// allocate memory for frame buffer
		if (m_fb==NULL) {
			printf("!<Ri::RiFrameBegin>: Failed to allocate %ld memory for frame buffer",
				size*sizeof(color3d));
			return;
		}
		m_opfb = new color3d[size];
		if (m_opfb==NULL) {
			printf("!<Ri::RiFrameBegin>: Failed to allocate %ld memory for opacity frame buffer",
				size*sizeof(color3d));
			return;
		}
		for (int i=0; i<size; ++i) {
			m_opfb[i] = color3d::white;
		}
	}

	/*
	 * If the format dimension is different from screen window, 
	 * then adjust screen window by y direction.
	 */
	double ratio = (double)m_sv->m_options.format_xres/m_sv->m_options.format_yres;
	double ratiosw = (double)(m_sv->m_options.screen_right-m_sv->m_options.screen_left)/
		(m_sv->m_options.screen_top-m_sv->m_options.screen_bot);

	if (!eq(ratio, ratiosw)) {
		m_sv->m_options.screen_left *= ratio;
		m_sv->m_options.screen_right *= ratio;
	}
}

RtVoid Ri::RiFrameEnd()
{
	m_sv->FrameEnd();

	Render();

	delete [] m_fb;
	delete [] m_opfb;
}

RtVoid Ri::RiWorldBegin()
{
	// FrameBegin may be ignored if there is only one frame
	if (m_framebegin==0) {
		RiFrameBegin(0);
		m_framebegin=0;	// dirty hack, FrameBegin will modify framebegin
	}

	// TODO: setup coordinate system
	matrix4d m = identity_matrix();

	m_sv->SetCoordSys(3, m);
	m_sv->SetCoordSys(4, m_trans);
}

RtVoid Ri::RiWorldEnd()
{
	if (m_framebegin==0)
		RiFrameEnd();
}


RtVoid Ri::RiFormat(RtInt xres, RtInt yres, RtFloat aspect)
{
	m_sv->m_options.format_xres = xres;
	m_sv->m_options.format_yres = yres;
	m_sv->m_options.format_par = aspect;

}

RtVoid Ri::RiFrameAspectRatio(RtFloat frameaspectratio)
{
	m_sv->m_options.frameaspectratio = frameaspectratio;
}

RtVoid Ri::RiScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	m_sv->m_options.screen_left = left;
	m_sv->m_options.screen_right = right;
	m_sv->m_options.screen_bot = bot;
	m_sv->m_options.screen_top = top;
}

RtVoid Ri::RiProjection(RtToken name, ...)
{
	if (strcmp(name, RI_PERSPECTIVE)==0) {
		m_sv->m_options.cam_projection = 1;
		// get fov
		va_list vl;
		va_start(vl, name);

		vector<ShaderParam> *pl = va_arg(vl, vector<ShaderParam> *);

		if ( (pl!=NULL) && (pl->size()>0) ) {
			vector<ShaderParam>::iterator it = pl->begin();
			ShaderParam &p = *it;
			if ( p.GetName().compare("fov")==0 ) {
				double fov;
				p.ConvertTo(VAR_FLOAT);
				fov = p.Getd();
				m_sv->m_options.cam_fov = deg2rad(fov);
			}
		}

		va_end(vl);
	}
	else if (strcmp(name, RI_ORTHOGRAPHIC)==0) {
		m_sv->m_options.cam_projection = 0;
	}

}

RtVoid Ri::RiDisplay(char *name, RtToken type, RtToken mode, ...)
{
	m_sv->m_options.display_name = name;
	if (strcmp(type, RI_FILE)!=0) {
		printf("?<Ri::RiDisplay> type is not \"file\", using file instead\n");
	}
	m_sv->m_options.display_type = ROption::DISPLAY_FILE;
	if (strcmp(mode, RI_RGB)!=0) {
		printf("?<Ri::RiDisplay> type is not \"rgb\", using rgb instead\n");
	}
	m_sv->m_options.display_mode = ROption::DISPLAY_RGBA;
}


RtVoid Ri::RiAttributeBegin()
{
	m_transstack.push(m_trans);
	m_attrstack.push(m_attr);
}

RtVoid Ri::RiAttributeEnd()
{
	m_attr = m_attrstack.top();
	m_trans = m_transstack.top();
	m_transstack.pop();
	m_attrstack.pop();
}


RtVoid Ri::RiColor(RtColor clr)
{
	m_attr.color[0] = clr[0];
	m_attr.color[1] = clr[1];
	m_attr.color[2] = clr[2];
}

RtVoid Ri::RiOpacity(RtColor clr)
{
	m_attr.opacity[0] = clr[0];
	m_attr.opacity[1] = clr[1];
	m_attr.opacity[2] = clr[2];
}

RtLightHandle Ri::RiLightSource(RtToken name, ...)
{
	va_list arg;
	va_start(arg, name);

	ShaderInst *si = new ShaderInst;
	si->name = name;
	si->mat = m_trans;


	ShaderFile *sf = m_sv->m_shaderfm.GetShaderFile(si->name);
	if (sf==NULL) {
		printf("!<Ri::RiLightSource> can't load light shader %s\n", si->name.c_str());
		delete si;
		return 0;
	}

	int handle = va_arg(arg, int);
	vector<ShaderParam> *pl = va_arg(arg, vector<ShaderParam> *);
	if (pl!=NULL) {
		for (int i=0; i<pl->size(); ++i) {
			ShaderParam &sp = (*pl)[i];
			
			int size, offset, type;
			string paramname;

			if (sf->GetParamDesc(sp.GetName(), offset, type)) {
				sp.SetSize(variable_dword((variable_t)type));
				sp.SetOffset(offset);
				sp.ConvertTo(type);
				si->params.push_back(sp);
			}
			
		}
	}
	m_shaders.push_back(si);
	m_sv->AddLightSource(si);

	va_end(arg);

	return NULL;
}

RtVoid Ri::RiIlluminate(RtLightHandle light, RtBoolean onoff)
{
	m_sv->SetLightOnOff((int)(light), onoff);
}

RtVoid Ri::RiSurface(RtToken name, ...)
{
	va_list arg;
	va_start(arg, name);

	vector<ShaderParam> *pl = va_arg(arg, vector<ShaderParam> *);

	ShaderInst *si = new ShaderInst;
	si->name = name;
	si->mat = m_trans;

	ShaderFile *sf = m_sv->m_shaderfm.GetShaderFile(si->name);
	if (sf==NULL) {
		printf("!<Ri::RiSurface> can't load surface shader %s\n", si->name.c_str());
		delete si;
		return;
	}

	if (pl!=NULL) {
		ConvertParameters(pl, sf, si);
	}

	m_shaders.push_back(si);

	m_attr.surface = si;

	va_end(arg);
}

RtVoid Ri::RiDisplacement(RtToken name, ...)
{
	va_list arg;
	va_start(arg, name);

	vector<ShaderParam> *pl = va_arg(arg, vector<ShaderParam> *);

	ShaderInst *si = new ShaderInst;
	si->name = name;
	si->mat = m_trans;

	ShaderFile *sf = m_sv->m_shaderfm.GetShaderFile(si->name);
	if (sf==NULL) {
		printf("!<Ri::RiDisplacement> can't load displacement shader %s\n", si->name.c_str());
		delete si;
		return;
	}

	if (pl!=NULL) {
		ConvertParameters(pl, sf, si);
	}

	m_shaders.push_back(si);

	m_attr.displacement = si;

	va_end(arg);
}

void Ri::ConvertParameters(vector<ShaderParam> *pl, ShaderFile *sf, ShaderInst *si)
{
	for (int i=0; i<pl->size(); ++i) {
		ShaderParam &sp = (*pl)[i];
		
		int size, offset, type;
		string paramname;

		if (sf->GetParamDesc(sp.GetName(), offset, type)) {
			sp.SetSize(variable_dword((variable_t)type));
			sp.SetOffset(offset);
			sp.ConvertTo(type);
			si->params.push_back(sp);
		}
		
	}
}


RtVoid Ri::RiIdentity(void)
{
	m_trans = identity_matrix();
}

RtVoid Ri::RiTransform(RtMatrix transform)
{
	m_trans.set_row(0, (double *)transform[0]);
	m_trans.set_row(1, (double *)transform[1]);
	m_trans.set_row(2, (double *)transform[2]);
	m_trans.set_row(3, (double *)transform[3]);
}

RtVoid Ri::RiConcatTransform(RtMatrix transform)
{
	matrix4d trans;
	trans.set_row(0, (double *)transform[0]);
	trans.set_row(1, (double *)transform[1]);
	trans.set_row(2, (double *)transform[2]);
	trans.set_row(3, (double *)transform[3]);

	m_trans *= trans;
}

RtVoid Ri::RiPerspective(RtFloat fov)
{
	m_sv->m_options.cam_fov = deg2rad(fov);
}

RtVoid Ri::RiTranslate(RtFloat dx, RtFloat dy, RtFloat dz)
{
	m_trans *= translate_matrix(vector3d(dx, dy, dz));
}

RtVoid Ri::RiRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
{
	m_trans *= rotate_matrix(vector3d(dx, dy, dz), deg2rad(angle));
}


RtVoid Ri::RiScale(RtFloat sx, RtFloat sy, RtFloat sz)
{
	m_trans *= scale_matrix(vector3d(sx, sy, sz));
}

RtVoid Ri::RiTransformBegin(void)
{
	m_transstack.push(m_trans);
}

RtVoid Ri::RiTransformEnd(void)
{
	m_trans = m_transstack.top();
	m_transstack.pop();
}

extern double g_znear, g_far;
RtVoid Ri::RiClipping(RtFloat hither, RtFloat yon)
{
	m_sv->m_options.cam_znear = hither; m_sv->m_options.cam_zfar = yon;

	g_znear = hither; g_zfar = yon;
}

RtVoid Ri::RiPixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	m_sv->m_options.pixelsamples_x = xsamples;
	m_sv->m_options.pixelsamples_y = ysamples;
}

RtVoid Ri::RiExposure(RtFloat gain, RtFloat gamma)
{
	if (!eq(gain, 1.0) && !eq(gamma, 1.0)) {
		m_sv->m_options.exposure_enable = true;
		m_sv->m_options.exposure_gain = gain;
		m_sv->m_options.exposure_gamma = gamma;
	}
	else
		m_sv->m_options.exposure_enable = false;
}

RtVoid Ri::RiSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax, ...)
{
	SphereObj *so = new SphereObj(radius, zmin, zmax, tmax);

	m_sv->AddGeomObj(so, &m_attr, m_trans);
}

RtVoid Ri::RiCone(RtFloat height, RtFloat radius, RtFloat tmax, ...)
{
	ConeObj *so = new ConeObj(height, radius, tmax);

	m_sv->AddGeomObj(so, &m_attr, m_trans);
}

RtVoid Ri::RiCylinder(RtFloat radius,RtFloat zmin,RtFloat zmax,RtFloat tmax, ...)
{
	CylinderObj *co = new CylinderObj(radius, zmin, zmax, tmax);

	m_sv->AddGeomObj(co, &m_attr, m_trans);
}

RtVoid Ri::RiDisk(RtFloat height, RtFloat radius, RtFloat tmax, ...)
{
	DiskObj *dobj = new DiskObj(height, radius, tmax);

	m_sv->AddGeomObj(dobj, &m_attr, m_trans);
}

RtVoid Ri::RiTorus(RtFloat majrad, RtFloat minrad, RtFloat phimin,
	RtFloat phimax, RtFloat tmax, ...)
{
	TorusObj *to = new TorusObj(majrad, minrad, phimin, phimax, tmax);

	m_sv->AddGeomObj(to, &m_attr, m_trans);
}

RtVoid Ri::RiHyperboloid(RtPoint pt1, RtPoint pt2, RtFloat tmax, ...)
{
	HyperboloidObj *ho = new HyperboloidObj(point3d(pt1[0], pt1[1], pt1[2]),
		point3d(pt2[0], pt2[1], pt2[2]), tmax);

	m_sv->AddGeomObj(ho, &m_attr, m_trans);
}

RtVoid Ri::RiParaboloid(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat tmax, ...)
{
	ParaboloidObj *po = new ParaboloidObj(rmax, zmin, zmax, tmax);

	m_sv->AddGeomObj(po, &m_attr, m_trans);
}

RtVoid Ri::RiPolygon(RtInt nverts, ...)
{
	va_list arg;
	va_start(arg, nverts);

	vector<ShaderParam> *pl = va_arg(arg, vector<ShaderParam> *);
	if (pl==NULL) {
		printf("!<Ri::RiPolygon> Missing polygon parameters\n");
		return;
	}
	va_end(arg);

	point3d *p=NULL;
	vector3d *n=NULL;
	double *st=NULL;
	
	vector<ShaderParam>::iterator it = pl->begin();
	for (; it<pl->end(); ++it) {
		ShaderParam &sp = *it;

		const string &name =sp.GetName();
		if (name.compare("P")==0)
			p = (point3d *)sp.GetValuePtr();
		else if (name.compare("N")==0)
			n = (vector3d *)sp.GetValuePtr();
		else if (name.compare("st")==0)
			st = (double *)sp.GetValuePtr();
	}

	int numpapol = nverts;
	int *poses = new int[nverts];
	for (int i=0; i<numpapol; ++i)
		poses[i] = i;

	PolygonObj *po = new PolygonObj(nverts, p, n, st, 1, &numpapol, poses);
	m_sv->AddGeomObj(po, &m_attr, m_trans);
	delete [] poses;
}

RtVoid Ri::RiPointsPolygons(RtInt npolys, RtInt nverts[], RtInt verts[], ...)
{
	va_list arg;
	va_start(arg, verts);

	vector<ShaderParam> *pl = va_arg(arg, vector<ShaderParam> *);
	if (pl==NULL) {
		printf("!<Ri::RiPolygon> Missing polygon parameters\n");
		return;
	}
	va_end(arg);

	point3d *p=NULL;
	vector3d *n=NULL;
	double *st=NULL;
	int totalnverts = 0;
	
	vector<ShaderParam>::iterator it = pl->begin();
	for (; it<pl->end(); ++it) {
		ShaderParam &sp = *it;

		const string &name =sp.GetName();
		if (name.compare("P")==0) {
			p = (point3d *)sp.GetValuePtr();
			totalnverts = sp.GetItemNum();
		}
		else if (name.compare("N")==0)
			n = (vector3d *)sp.GetValuePtr();
		else if (name.compare("st")==0)
			st = (double *)sp.GetValuePtr();
	}

	PolygonObj *po = new PolygonObj(totalnverts, p, n, st, npolys, nverts, verts);
	m_sv->AddGeomObj(po, &m_attr, m_trans);
}

ray Ri::GenerateRay(int px, int py, int sx, int sy)
{
	double x = px + (double)sx/m_sv->m_options.pixelsamples_x-0.5;
	double y = (m_sv->m_options.format_yres-1-py) + (double)sy/m_sv->m_options.pixelsamples_y-0.5;

	ray r;
	// generate ray
	if (m_sv->m_options.cam_projection==0) {
		// orthographic projection
		double tx = x/(m_sv->m_options.format_xres-1);
		double ty = y/(m_sv->m_options.format_yres-1);
		point3d orig;
		orig.set_comp(m_sv->m_options.screen_left+(m_sv->m_options.screen_right-m_sv->m_options.screen_left)*tx,
			m_sv->m_options.screen_bot+(m_sv->m_options.screen_top-m_sv->m_options.screen_bot)*ty,
			0);
		r.set_orig(orig);
		r.set_dir(vector3d(0, 0, 1));	// notice that camera space is left-handed
	}
	else {
		// perspective projection
		double tx = x/(m_sv->m_options.format_xres-1)*2-1.0;
		double ty = y/(m_sv->m_options.format_yres-1)*2-1.0;

		r.set_orig(point3d(0, 0, 0));
		double sinfov = sin(m_sv->m_options.cam_fov/2);
		double ratio = (double)m_sv->m_options.format_xres/m_sv->m_options.format_yres;
		vector3d v;
		v.set_comp(1, ty*sinfov);
		v.set_comp(0, tx*sinfov*ratio);
		v.set_comp(2, cos(m_sv->m_options.cam_fov/2));
		v.normalize();
		r.set_dir(v);
	}

	return r;
}

void Ri::Render()
{
	int px, py, sx, sy;

	time_t starttime;
	time(&starttime);

	ray r;
	int idx=0;
	for (py=0; py<m_sv->m_options.format_yres; ++py) {
		for (px=0; px<m_sv->m_options.format_xres; ++px) {
			color3d pixelcolor = color3d::black;
			color3d pixelopacity = color3d::black;
			for (sy=0; sy<m_sv->m_options.pixelsamples_y; ++sy) {
				for (sx = 0; sx<m_sv->m_options.pixelsamples_x; ++sx) {
					r = GenerateRay(px, py, sx, sy);
					color3d cs, os;
					m_sv->Trace(r, cs, os);
					cs.clamp(color3d::clamp_cut);
					os.clamp(color3d::clamp_cut);
					pixelcolor += cs;
					pixelopacity += os;
				}
			}

			pixelcolor /= m_sv->m_options.pixelsamples_x*m_sv->m_options.pixelsamples_y;
			if (m_sv->m_options.exposure_enable) {
				double invgamma = 1/m_sv->m_options.exposure_gamma;
				m_fb[idx].set_comp(0, pow(pixelcolor.r()*m_sv->m_options.exposure_gain, invgamma));
				m_fb[idx].set_comp(1, pow(pixelcolor.g()*m_sv->m_options.exposure_gain, invgamma));
				m_fb[idx].set_comp(2, pow(pixelcolor.b()*m_sv->m_options.exposure_gain, invgamma));
			}
			else
				m_fb[idx] = pixelcolor;
			m_opfb[idx] = pixelopacity/(m_sv->m_options.pixelsamples_x*m_sv->m_options.pixelsamples_y);
			idx++;
		}
		printf("@<Ri::Render>: Done %3u%%\r", int((double)py/(m_sv->m_options.format_yres-1)*100));
	}

	time_t endtime;
	time(&endtime);

	printf("\nElapsed time: %ds\n", endtime-starttime);

	// save buffer to tiff file
	TIFF *tif =TIFFOpen(m_sv->m_options.display_name.c_str(), "w");

	unsigned int rowsperstrip = (unsigned int)(-1);
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, m_sv->m_options.format_yres);
	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, m_sv->m_options.format_xres);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 
		rowsperstrip = TIFFDefaultStripSize(tif, rowsperstrip));
//	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, m_sv->m_options.format_yres);

	TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	TIFFSetField(tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
	TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

	TIFFSetField(tif, TIFFTAG_XRESOLUTION, 150.0);
	TIFFSetField(tif, TIFFTAG_YRESOLUTION, 150.0);
	TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
	TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_PACKBITS);

	int slsize = TIFFScanlineSize(tif);
	unsigned char *scanline = (unsigned char *)_TIFFmalloc(slsize);

	int jj=0;
	for (int i=0; i<m_sv->m_options.format_yres; ++i) {
		for (int j=0; j<slsize;) {
			scanline[j++] = (unsigned char)(m_fb[jj].r()*255);
			scanline[j++] = (unsigned char)(m_fb[jj].g()*255);
			scanline[j++] = (unsigned char)(m_fb[jj++].b()*255);
//			scanline[j++] = 255;
		}

		if (TIFFWriteScanline(tif, scanline, i)<0) {
			printf("!<Ri::Render>: Failed exec TIFFWriteScanline\n");
		}

/*		int strip = 0;
		int stripsize = TIFFStripSize(tif);
		int height = m_sv->m_options.format_yres,
			width = m_sv->m_options.format_xres;
		if (TIFFWriteEncodedStrip(tif, strip, scanline+i*width, stripsize) < 0)
			break;
		strip++;
*/	}

	_TIFFfree(scanline);

	TIFFClose(tif);
}


// constant strings

RtToken RI_FRAMEBUFFER="framebuffer", RI_FILE="file";
RtToken RI_RGB="rgb", RI_RGBA="rgba", RI_RGBZ="rgbz", RI_RGBAZ="rgbaz", 
	RI_A="a", RI_Z="z", RI_AZ="az";
RtToken RI_PERSPECTIVE="perspective", RI_ORTHOGRAPHIC="orthographic";
RtToken RI_HIDDEN="hidden", RI_PAINT="paint";
RtToken RI_CONSTANT="constant", RI_SMOOTH="smooth";
RtToken RI_FLATNESS="flatness", RI_FOV="fov";
RtToken RI_AMBIENTLIGHT="ambientlight", RI_POINTLIGHT="pointlight", 
	RI_DISTANTLIGHT="distantlight", RI_SPOTLIGHT="spotlight";
RtToken RI_INTENSITY="intensity", RI_LIGHTCOLOR="lightcolor", 
	RI_FROM="from", RI_TO="to", RI_CONEANGLE="coneangle",
	RI_CONEDELTAANGLE="conedeltaangle", 
	RI_BEAMDISTRIBUTION="beamdistribution";
RtToken RI_MATTE="matte", RI_METAL="metal", RI_SHINYMETAL="shinymetal",
	RI_PLASTIC="plastic", RI_PAINTEDPLASTIC="paintedplastic";
RtToken RI_P="P", RI_PZ="PZ", RI_PW="PW", RI_N="N", RI_NP="NP",
	RI_CS="Cs", RI_OS="Os", RI_S="S", RI_T="T", RI_ST="ST";
RtToken RI_BILINEAR="bilinear", RI_BICUBIC="bicubic";
RtToken RI_LINEAR="linear", RI_CUBIC="cubic";
RtToken RI_PRIMITIVE="primitive", RI_INTERSECTION="intersection", 
	RI_UNION="union", RI_DIFFERENCE="difference";
RtToken RI_PERIODIC="periodic", RI_NONPERIODIC="nonperiodic", 
	RI_CLAMP="clamp", RI_BLACK="black";
RtToken RI_IGNORE="ignore", RI_PRINT="print", RI_ABORT="abort", 
	RI_HANDLER="handler";
RtToken RI_COMMENT="comment", RI_STRUCTURE="structure", RI_VERBATIM="vertatim";
RtToken RI_IDENTIFIER="identifier", RI_NAME="name", 
	RI_SHADINGGROUP="shadinggroup";
RtToken RI_WIDTH="width", RI_CONSTANTWIDTH="constantwidth";


