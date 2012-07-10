#include "common.hxx"
#include "RayEngine.hxx"
#include "parsefunc.hxx"

#include "color.hxx"

const char *TOKEN_STR[] = {
	"ArchiveRecord",
	"AreaLightSource",
	"Atmosphere",
	"Attribute",
	"AttributeBegin",
	"AttributeEnd",
	"Basis",
	"Begin",
	"Blobby",
	"Bound",
	"Clipping",
	"ClippingPlane",
	"Color",
	"ColorSamples",
	"ConcatTransform",
	"Cone",
	"Context",
	"CoordinateSystem",
	"CoordSysTransform",
	"CropWindow",
	"Curves",
	"Cylinder",
	"Declare",
	"DepthOfField",
	"Detail",
	"DetailRange",
	"Disk",
	"Displacement",
	"Display",
	"End",
	"ErrorHandler",
	"Exposure",
	"Exterior",
	"Format",
	"FrameAspectRatio",
	"FrameBegin",
	"FrameEnd",
	"GeneralPolygon",
	"GeometricApproximation",
	"Geometry",
	"GetContext",
	"Hider",
	"Hyperboloid",
	"Identity",
	"Illuminate",
	"Imager",
	"Interior",
	"LightSource",
	"MakeCubeFaceEnvironment",
	"MakeLatLongEnvironment",
	"MakeShadow",
	"MakeTexture",
	"Matte",
	"MotionBegin",
	"MotionEnd",
	"NuPatch",
	"ObjectBegin",
	"ObjectEnd",
	"ObjectInstance",
	"Opacity",
	"Option",
	"Orientation",
	"Paraboloid",
	"Patch",
	"PatchMesh",
	"Perspective",
	"PixelFilter",
	"PixelSamples",
	"PixelVariance",
	"Points",
	"PointsGeneralPolygon",
	"PointsPolygons",
	"Polygon",
	"Procedural",
	"Projection",
	"Quantize",
	"ReadArchive",
	"RelativeDetail",
	"ReverseOrientation",
	"Rotate",
	"Scale",
	"ScreenWindow",
	"ShadingInterpolation",
	"ShadingRate",
	"Shutter",
	"Sides",
	"Skew",
	"SolidBegin",
	"SolidEnd",
	"Sphere",
	"SubdivisionMesh",
	"Surface",
	"TextureCoordinates",
	"Torus",
	"Transform",
	"TransformBegin",
	"TransformEnd",
	"TransformPoints",
	"Translate",
	"TrimCurve",
	"WorldBegin",
	"WorldEnd"
};

enum {
	TK_ARCHIVERECORD,
	TK_AREALIGHTSOURCE,
	TK_ATMOSPHERE,
	TK_ATTRIBUTE,
	TK_ATTRIBUTEBEGIN,
	TK_ATTRIBUTEEND,
	TK_BASIS,
	TK_BEGIN,
	TK_BLOBBY,
	TK_BOUND,
	TK_CLIPPING,
	TK_CLIPPINGPLANE,
	TK_COLOR,
	TK_COLORSAMPLES,
	TK_CONCATTRANSFORM,
	TK_CONE,
	TK_CONTEXT,
	TK_COORDINATESYSTEM,
	TK_COORDSYSTRANSFORM,
	TK_CROPWINDOW,
	TK_CURVES,
	TK_CYLINDER,
	TK_DECLARE,
	TK_DEPTHOFFIELD,
	TK_DETAIL,
	TK_DETAILRANGE,
	TK_DISK,
	TK_DISPLACEMENT,
	TK_DISPLAY,
	TK_END,
	TK_ERRORHANDLER,
	TK_EXPOSURE,
	TK_EXTERIOR,
	TK_FORMAT,
	TK_FRAMEASPECTRATIO,
	TK_FRAMEBEGIN,
	TK_FRAMEEND,
	TK_GENERALPOLYGON,
	TK_GEOMETRICAPPROXIMATION,
	TK_GEOMETRY,
	TK_GETCONTEXT,
	TK_HIDER,
	TK_HYPERBOLOID,
	TK_IDENTITY,
	TK_ILLUMINATE,
	TK_IMAGER,
	TK_INTERIOR,
	TK_LIGHTSOURCE,
	TK_MAKECUBEFACEENVIRONMENT,
	TK_MAKELATLONGENVIRONMENT,
	TK_MAKESHADOW,
	TK_MAKETEXTURE,
	TK_MATTE,
	TK_MOTIONBEGIN,
	TK_MOTIONEND,
	TK_NUPATCH,
	TK_OBJECTBEGIN,
	TK_OBJECTEND,
	TK_OBJECTINSTANCE,
	TK_OPACITY,
	TK_OPTION,
	TK_ORIENTATION,
	TK_PARABOLOID,
	TK_PATCH,
	TK_PATCHMESH,
	TK_PERSPECTIVE,
	TK_PIXELFILTER,
	TK_PIXELSAMPLES,
	TK_PIXELVARIANCE,
	TK_POINTS,
	TK_POINTSGENERALPOLYGONS,
	TK_POINTSPOLYGONS,
	TK_POLYGON,
	TK_PROCEDURAL,
	TK_PROJECTION,
	TK_QUANTIZE,
	TK_READARCHIVE,
	TK_RELATIVEDETAIL,
	TK_REVERSEORIENTATION,
	TK_ROTATE,
	TK_SCALE,
	TK_SCREENWINDOW,
	TK_SHADINGINTERPOLATION,
	TK_SHADINGRATE,
	TK_SHUTTER,
	TK_SIDES,
	TK_SKEW,
	TK_SOLIDBEGIN,
	TK_SOLIDEND,
	TK_SPHERE,
	TK_SUBDIVISIONMESH,
	TK_SURFACE,
	TK_TEXTURECOORDINATES,
	TK_TORUS,
	TK_TRANSFORM,
	TK_TRANSFORMBEGIN,
	TK_TRANSFORMEND,
	TK_TRANSFORMPOINTS,
	TK_TRANSLATE,
	TK_TRIMCURVE,
	TK_WORLDBEGIN,
	TK_WORLDEND,
	NUMBEROFTOKEN
};



int RayEngine::ReadRib(const string &filename)
{
	FILE *fp = fopen(filename.c_str(), "rt");

	if (fp==NULL) {
		printf("!<RayEngine::ReadRib>: Failed to open %s\n", filename.c_str());
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	long filelen = ftell(fp);
	m_filebuf = new char[filelen+1];
	if (m_filebuf==NULL) {
		printf("!<RayEngine::ReadRib>: Failed to allocate memory size %ld", filelen);
		fclose(fp);
		return -1;
	}

	fseek(fp, 0, SEEK_SET);

	long readlen;
	if ( (readlen = fread(m_filebuf, 1, filelen, fp)) == 0) {
		printf("!<RayEngine::ReadRib>: Failed to read file\n");
		fclose(fp);
		return -1;
	}

	fclose(fp);

	// dangerous conversion
	m_ri.RiBegin((RtToken)filename.c_str());

	m_fileptr = m_filebuf;
	m_fileend = m_filebuf+readlen;
	*m_fileend = '\0';
	m_curline = 1;

	int i, i0, i1, i2;
	int *iarr, *iarr2;
	double d0, d1, d2, d3, d4;
	char s0[256], s1[256], s2[256];
	RtColor c0;
	RtMatrix m0;
	vector<ShaderParam> p0;
	RtPoint pt0, pt1;
	int token;

	while ( (token = ReadToken())>=0) {
		m_curcmd = string(m_buffer);
		switch (token) {
		case TK_BEGIN:
			m_ri.RiBegin("unused");
			break;
		case TK_END:
			m_ri.RiEnd();
			break;
		case TK_FRAMEBEGIN:
			if (ReadInt(&i0)<0) {
				PrintSimpleError();
			}
			else
				m_ri.RiFrameBegin(i0);
			break;
		case TK_FRAMEEND:
			m_ri.RiFrameEnd();
			break;
		case TK_WORLDBEGIN:
			m_ri.RiWorldBegin();
			break;
		case TK_WORLDEND:
			m_ri.RiWorldEnd();
			break;
		case TK_FORMAT:
			if ( (ReadInt(&i0)<0) || (ReadInt(&i1)<0) || (ReadDouble(&d0)) ) {
				PrintSimpleError();
			}
			else
				m_ri.RiFormat(i0, i1, d0);
			break;
		case TK_FRAMEASPECTRATIO:
			if (ReadDouble(&d0)<0)
				PrintSimpleError();
			else
				m_ri.RiFrameAspectRatio(d0);
			break;
		case TK_SCREENWINDOW:
			if ( (ReadDouble(&d0)<0) || (ReadDouble(&d1)<0) ||
				(ReadDouble(&d2)<0) || (ReadDouble(&d3)<0) )
				PrintSimpleError();
			else
				m_ri.RiScreenWindow(d0, d1, d2, d3);
			break;
		case TK_PROJECTION:
			if ( (ReadString(s0)<0) || (ReadParameter(p0)<0) )
				PrintSimpleError();
			else
				m_ri.RiProjection(s0, &p0);
			break;
		case TK_DISPLAY:
			if ( (ReadString(s0)<0) || (ReadString(s1)<0) || (ReadString(s2)<0) )
				PrintSimpleError();
			else
				m_ri.RiDisplay(s0, s1, s2);
			break;
		case TK_DEPTHOFFIELD:
			if ( (ReadDouble(&d0)<0) || (ReadDouble(&d1)<0) || (ReadDouble(&d2)<0) )
				PrintSimpleError();
			printf("?<RayEngine::ReadRib> Depthoffield is not supported\n");
			break;
		case TK_SHUTTER:
			if ( (ReadDouble(&d0)<0) || (ReadDouble(&d1)<0) )
				PrintSimpleError();
			printf("?<RayEngine::ReadRib> Shutter is not supported\n");
			break;
		case TK_DECLARE:
			if ( (ReadString(s0)<0) || (ReadString(s1)<0) )
				PrintSimpleError();
			printf("?<RayEngine::ReadRib> Declare is not supported\n");
			break;
		case TK_ATTRIBUTE:
			if ( (ReadString(s0)<0) || (ReadParameter(p0)<0) )
				PrintSimpleError();
			printf("?<RayEngine::ReadRib> Atribute is not supported\n");
			break;
		case TK_ATTRIBUTEBEGIN:
			m_ri.RiAttributeBegin();
			break;
		case TK_ATTRIBUTEEND:
			m_ri.RiAttributeEnd();
			break;
		case TK_COLOR:
			if ( ReadColor(c0)<0 ) 
				PrintSimpleError();
			else
				m_ri.RiColor(c0);
			break;
		case TK_OPACITY:
			if ( ReadColor(c0)<0 )
				PrintSimpleError();
			else
				m_ri.RiOpacity(c0);
			break;
		case TK_LIGHTSOURCE:
			if ( (ReadString(s0)<0) || (ReadInt(&i0)<0) ||
				(ReadParameter(p0)<0) )
				PrintSimpleError();
			else
				m_ri.RiLightSource(s0, i0, &p0);
			break;
		case TK_ILLUMINATE:
			if ( (ReadInt(&i0)<0) || (ReadInt(&i1)<0) )
				PrintSimpleError();
			else
				m_ri.RiIlluminate((RtLightHandle)i0, i1);
			break;
		case TK_SURFACE:
			if ( (ReadString(s0)<0) || (ReadParameter(p0)<0) )
				PrintSimpleError();
			else
				m_ri.RiSurface(s0, &p0);
			break;
		case TK_DISPLACEMENT:
			if ( (ReadString(s0)<0) || (ReadParameter(p0)<0) )
				PrintSimpleError();
			else
				m_ri.RiDisplacement(s0, &p0);
			break;
		case TK_IMAGER:
			if ( (ReadString(s0)<0) || (ReadParameter(p0)<0) )
				PrintSimpleError();
			printf("?<RayEngine::ReadRib> Imager is not supported\n");
			break;
		case TK_IDENTITY:
			m_ri.RiIdentity();
			break;
		case TK_TRANSFORM:
			if ( ReadMatrix(m0)<0 )
				PrintSimpleError();
			else
				m_ri.RiTransform(m0);
			break;
		case TK_CONCATTRANSFORM:
			if ( ReadMatrix(m0)<0 )
				PrintSimpleError();
			else
				m_ri.RiConcatTransform(m0);
			break;
		case TK_PERSPECTIVE:
			if ( ReadDouble(&d0)<0 )
				PrintSimpleError();
			else
				m_ri.RiPerspective(d0);
			break;
		case TK_TRANSLATE:
			if ( (ReadDouble(&d0)<0) || (ReadDouble(&d1)<0) ||
                                (ReadDouble(&d2)<0) )
				PrintSimpleError();
			else
				m_ri.RiTranslate(d0, d1, d2);
			break;
		case TK_ROTATE:
			if ( (ReadDouble(&d0)<0) || (ReadDouble(&d1)<0) ||
                                (ReadDouble(&d2)<0) || (ReadDouble(&d3)<0) )
				PrintSimpleError();
			else
				m_ri.RiRotate(d0, d1, d2, d3);
			break;
		case TK_SCALE:
			if ( (ReadDouble(&d0)<0) || (ReadDouble(&d1)<0) ||
                                (ReadDouble(&d2)<0) )
				PrintSimpleError();
			else
				m_ri.RiScale(d0, d1, d2);
			break;
		case TK_TRANSFORMBEGIN:
			m_ri.RiTransformBegin();
			break;
		case TK_TRANSFORMEND:
			m_ri.RiTransformEnd();
			break;
		case TK_CLIPPING:
			if ( (ReadDouble(&d0)<0) || (ReadDouble(&d1)<0) )
				PrintSimpleError();
			m_ri.RiClipping(d0, d1);
			break;
		case TK_SHADINGINTERPOLATION:
			if ( (ReadString(s0)<0) )
				PrintSimpleError();
			printf("?<RayEngine::ReadRib> ShadingInterpolation is not implemented.\n");
			break;
		case TK_SHADINGRATE:
			if ( (ReadDouble(&d0)<0) )
				PrintSimpleError();
			printf("?<RayEngine::ReadRib> ShadingRate is not implemented.\n");
			break;
		case TK_ORIENTATION:
			if ( (ReadString(s0)<0) )
				PrintSimpleError();
			printf("?<RayEngine::ReadRib> Orientation is not implemented.\n");
			break;
		case TK_REVERSEORIENTATION:
			printf("?<RayEngine::ReadRib> ReverseOrientation is not implemented.\n");
			break;
		case TK_SIDES:
			if ( (ReadDouble(&d0)<0) )
				PrintSimpleError();
			printf("?<RayEngine::ReadRib> Sides is not supported\n");
			break;
		case TK_PIXELSAMPLES:
			if ( (ReadDouble(&d0)<0) || (ReadDouble(&d1)<0) )
				PrintSimpleError();
			m_ri.RiPixelSamples(d0, d1);
			break;
		case TK_EXPOSURE:
			if ( (ReadDouble(&d0)<0) || (ReadDouble(&d1)<0) )
				PrintSimpleError();
			m_ri.RiExposure(d0, d1);
			break;
		case TK_SPHERE:
			if ( (ReadDouble(&d0)<0) || (ReadDouble(&d1)<0) ||
                                (ReadDouble(&d2)<0) || (ReadDouble(&d3)<0) )
				PrintSimpleError();
			else
				m_ri.RiSphere(d0, d1, d2, d3);
			break;
		case TK_CONE:
			if ( (ReadDouble(&d0)<0) || (ReadDouble(&d1)<0) ||
                                (ReadDouble(&d2)<0) )
				PrintSimpleError();
			else
				m_ri.RiCone(d0, d1, d2);
			break;
		case TK_CYLINDER:
			if ( (ReadDouble(&d0)<0) || (ReadDouble(&d1)<0) ||
                                (ReadDouble(&d2)<0) || (ReadDouble(&d3)<0) )
				PrintSimpleError();
			else
				m_ri.RiCylinder(d0, d1, d2, d3);
			break;
		case TK_DISK:
			if ( (ReadDouble(&d0)<0) || (ReadDouble(&d1)<0) ||
                                (ReadDouble(&d2)<0) )
				PrintSimpleError();
			else
				m_ri.RiDisk(d0, d1, d2);
			break;
		case TK_TORUS:
			if ( (ReadDouble(&d0)<0) || (ReadDouble(&d1)<0) ||
                                (ReadDouble(&d2)<0) || (ReadDouble(&d3)<0) ||
				(ReadDouble(&d4)<0) )
				PrintSimpleError();
			else
				m_ri.RiTorus(d0, d1, d2, d3, d4);
			break;
		case TK_HYPERBOLOID:
			if ( (ReadDouble(pt0)<0) || (ReadDouble(pt0+1)<0) ||
				(ReadDouble(pt0+2)<0) || (ReadDouble(pt1)<0) ||
				(ReadDouble(pt1+1)<0) || (ReadDouble(pt1+2)<0) ||
				(ReadDouble(&d0)<0) )
				PrintSimpleError();
			else
				m_ri.RiHyperboloid(pt0, pt1, d0);
			break;
		case TK_PARABOLOID:
			if ( (ReadDouble(&d0)<0) || (ReadDouble(&d1)<0) ||
				(ReadDouble(&d2)<0) || (ReadDouble(&d3)<0) )
				PrintSimpleError();
			else
				m_ri.RiParaboloid(d0, d1, d2, d3);
			break;
		case TK_POLYGON:
			if ( ReadParameter(p0)<0) { 
				PrintSimpleError();
				break;
			}

			for (i=0; i<p0.size(); ++i) {
				ShaderParam &sp = p0[i];
				if (sp.GetName().compare("P")==0) {
					sp.ConvertTo(VAR_POINT|VAR_ARRAY);
					i0 = sp.GetItemNum();
					break;
				}
			}
			// check other values to be coincide with number of P
			for (i=0; i<p0.size(); ++i) {
				ShaderParam &sp = p0[i];
				if (sp.GetName().compare("N")==0) {
					sp.ConvertTo(VAR_VECTOR|VAR_ARRAY);
					if (sp.GetItemNum()!=i0)
						printf("!<RayEngine::ReadRib> number of normal is not correct.\n");
				}
				else if (sp.GetName().compare("st")==0) {
					sp.ConvertTo(VAR_FLOAT|VAR_ARRAY);
					if (sp.GetItemNum()!=i0*2)
						printf("!<RayEngine::ReadRib> number of st is not correct.\n");
				}
				else if (sp.GetName().compare("P")==0)
					continue;
				else
					printf("!<RayEngine::ReadRib> %s not implemented", sp.GetName().c_str());
			}
			m_ri.RiPolygon(i0, &p0);
			break;
		case TK_GENERALPOLYGON:
			break;
		case TK_POINTSPOLYGONS:
			if ( ((i0 = ReadArrayi(iarr))<0) || ((i1 = ReadArrayi(iarr2))<0) ||
				(ReadParameter(p0)<0) ) {
				PrintSimpleError();
				break;
			}
			for (i=0; i<p0.size(); ++i) {
				ShaderParam &sp = p0[i];
				if (sp.GetName().compare("P")==0) {
					sp.ConvertTo(VAR_POINT|VAR_ARRAY);
					i2 = sp.GetItemNum();
					break;
				}
			}
			// check other values to be coincide with number of P
			for (i=0; i<p0.size(); ++i) {
				ShaderParam &sp = p0[i];
				if (sp.GetName().compare("N")==0) {
					sp.ConvertTo(VAR_VECTOR|VAR_ARRAY);
					if (sp.GetItemNum()!=i2)
						printf("!<RayEngine::ReadRib> number of normal is not correct.\n");
				}
				else if (sp.GetName().compare("st")==0) {
					sp.ConvertTo(VAR_FLOAT|VAR_ARRAY);
					if (sp.GetItemNum()!=i2*2)
						printf("!<RayEngine::ReadRib> number of st is not correct.\n");
				}
				else if (sp.GetName().compare("P")==0)
					continue;
				else
					printf("!<RayEngine::ReadRib> %s not implemented", sp.GetName().c_str());
			}
			m_ri.RiPointsPolygons(i0, iarr, iarr2, &p0);
			delete [] iarr;
			delete [] iarr2;
			break;
		case TK_POINTSGENERALPOLYGONS:
			break;
		default:
			if (m_fileptr<m_fileend)
				printf("!> Sorry %s is not implemented yet\n", m_buffer);
			break;
		}
	}

	delete [] m_filebuf;

	m_ri.RiEnd();
}

int RayEngine::ReadInt(int *i)
{
	if (ReadWord()<0)
		return -1;

	if (!IsValidInteger(m_buffer))
		return -1;

	*i = atoi(m_buffer);
	return 0;
}

int RayEngine::ReadDouble(double *d)
{
	if (ReadWord()<0)
		return -1;

	if (!IsValidDouble(m_buffer))
		return -1;

	*d = atof(m_buffer);
	return 0;
}

int RayEngine::ReadString(char *s)
{
	SkipWhiteSpace();

	*s = '\0';

	if (*m_fileptr!='\"')
		return -1;

	// TODO: add backspace sequence
	NextChar();
	int ccount = 0;
	while(*m_fileptr!='\"') {
		if (*m_fileptr=='\n') {
			printf("?<RayEngine::ReadString>: Missing closing \"?\n");
			break;
		}
		ccount++;
		if (ccount>=256) {
			printf("!!<RayEngine::ReadString>: String size > 256!\n");
			break;
		}
		*s = *m_fileptr;
		s++; NextChar();
	}

	*s = '\0';
	NextChar();

	return 0;
}

int RayEngine::ReadMatrix(RtMatrix m)
{
	double d;

	SkipWhiteSpace();
	if (*m_fileptr=='[')
		NextChar();

	if (ReadDouble(&d)<0)
		return -1;
	m[0][0] = d;
	if (ReadDouble(&d)<0)
		return -1;
	m[0][1] = d;
	if (ReadDouble(&d)<0)
		return -1;
	m[0][2] = d;
	if (ReadDouble(&d)<0)
		return -1;
	m[0][3] = d;

	if (ReadDouble(&d)<0)
		return -1;
	m[1][0] = d;
	if (ReadDouble(&d)<0)
		return -1;
	m[1][1] = d;
	if (ReadDouble(&d)<0)
		return -1;
	m[1][2] = d;
	if (ReadDouble(&d)<0)
		return -1;
	m[1][3] = d;

	if (ReadDouble(&d)<0)
		return -1;
	m[2][0] = d;
	if (ReadDouble(&d)<0)
		return -1;
	m[2][1] = d;
	if (ReadDouble(&d)<0)
		return -1;
	m[2][2] = d;
	if (ReadDouble(&d)<0)
		return -1;
	m[2][3] = d;

	if (ReadDouble(&d)<0)
		return -1;
	m[3][0] = d;
	if (ReadDouble(&d)<0)
		return -1;
	m[3][1] = d;
	if (ReadDouble(&d)<0)
		return -1;
	m[3][2] = d;
	if (ReadDouble(&d)<0)
		return -1;
	m[3][3] = d;

	SkipWhiteSpace();
	if (*m_fileptr==']')
		NextChar();

	return 0;
}

int RayEngine::ReadArrayi(int *&buf)
{
	char s[1024];
	ReadStringOrValue(s);

	vector<int> iarray;
	const char *p = s;
	int i;
	int offset;
	while ( (p = get_int(p, &i)) != NULL) {
		iarray.push_back(i);
	}

	if (iarray.size()>0) {
		buf = new int[iarray.size()];
		for (i=0; i<iarray.size(); ++i) {
			buf[i] = iarray[i];
		}
	}

	return iarray.size();
}

int RayEngine::ReadArrayd(double *&buf)
{
	char s[1024];
	ReadStringOrValue(s);

	vector<double> darray;
	const char *p = s;
	double d;
	int offset;
	while ( (p = get_double(p, &d)) != NULL) {
		p += offset;
		darray.push_back(d);
	}

	if (darray.size()>0) {
		buf = new double[darray.size()];
		for (int i=0; i<darray.size(); ++i) {
			buf[i] = darray[i];
		}
	}

	return darray.size();
}

int RayEngine::ReadColor(RtColor c)
{
	double d;
	SkipWhiteSpace();
	if (*m_fileptr=='[')
		NextChar();

	if (ReadDouble(&d)<0)
		return -1;
	c[0] = d;
	if (ReadDouble(&d)<0)
		return -1;
	c[1] = d;
	if (ReadDouble(&d)<0)
		return -1;
	c[2] = d;
	SkipWhiteSpace();
	if (*m_fileptr==']')
		NextChar();
	return 0;
}

int RayEngine::ReadToken()
{
	if (ReadWord()<0)
		return -1;

	/* OPTI: token search can be optimized using hash */
	for (int i=0; i<NUMBEROFTOKEN; ++i) {
		if (strcmp(m_buffer, TOKEN_STR[i])==0)
			return i;
	}

	printf("!<RayEngine::ReadToken> %s is not defined by Renderman.\n", m_buffer);

	return -1;
}

void RayEngine::SkipWhiteSpace()
{
	while ( m_fileptr<m_fileend && IsWhiteSpace(*m_fileptr))
		NextChar();
}

bool RayEngine::IsWhiteSpace(char c)
{
	if (c=='\t' || c=='\n' || c==' ')
		return true;
	else
		return false;
}

int RayEngine::ReadWord()
{
	SkipWhiteSpace();

	// skip comment
	while (1) {
		while (m_fileptr<m_fileend && *m_fileptr=='#') {
			while (m_fileptr<m_fileend && *m_fileptr!='\n')
				NextChar();
			NextChar();
		}

		SkipWhiteSpace();	// skip the whitespace after a comment line
		if (m_fileptr>=m_fileend)
			break;
		if (*m_fileptr!='#')
			break;
	}

	bool valid = false;
	char *ptr = m_buffer;
	while ( m_fileptr<m_fileend ) {
		if (IsWhiteSpace(*m_fileptr))
			break;
		valid = true;
		*ptr = *m_fileptr;
		++ptr;
		NextChar();
	}

	*ptr = '\0';

	return valid ? 0 : -1;
}

int RayEngine::ReadParameter(vector<ShaderParam> &p)
{
	char s0[256], s1[1024];

	p.erase(p.begin(), p.end());
	int i = p.size();

	while (1) {
		if (!SkipWhiteSpaceNoReturn())
			break;

		ReadString(s0);

		ReadStringOrValue(s1);

		ShaderParam par;

		par.SetName(string(s0));

		par.Sets(string(s1));

		p.push_back(par);
	}

	return 0;
}

bool RayEngine::SkipWhiteSpaceNoReturn()
{
	while (m_fileptr<m_fileend && IsWhiteSpace(*m_fileptr)) {
		if (*m_fileptr=='\n')
			return  false;
		NextChar();
	}

	return true;
}

int RayEngine::ReadStringOrValue(char *s)
{
	SkipWhiteSpace();

	*s = '\0';

	bool isstring=false, isarray=false;

	if (*m_fileptr=='[') {
		isarray = true;
		NextChar();
	}
	SkipWhiteSpace();
	if (*m_fileptr=='\"') {
		isstring=true;
		NextChar();
	}

	// TODO: add backspace sequence
	int ccount = 0;
	while(1) {
		if ( (isstring && *m_fileptr=='\"') ||
			(isarray && *m_fileptr==']') ||
			(!isstring && !isarray && IsWhiteSpace(*m_fileptr)) )
			break;
		if (*m_fileptr=='\n') {
			printf("?<RayEngine::ReadStringOrValue>: Missing closing \"?\n");
			break;
		}
		ccount++;
		if (ccount>=1024) {
			printf("!!<RayEngine::ReadStringOrValue> string size bigger than 1024!\n");
			break;
		}
		*s = *m_fileptr;
		s++; NextChar();
	}

	*s = '\0';

	if (isstring)		// advance to ignore \"
		NextChar();
	if (isarray) {		// ignore ']'
		SkipWhiteSpace();
		NextChar();
	}

	return 0;
}

bool RayEngine::IsValidInteger(const char *s) const
{
	const char *p = s;

	while (*p!='\0') {
		if (!isdigit(*p))
			return false;
		p++;
	}

	return true;
}

bool RayEngine::IsValidDouble(const char *s) const
{
	const char *p = s;

	bool havedot=false;
	bool havee=false;

	if (*p=='+' || *p=='-')
		p++;

	while (*p!='\0') {
		if ( !isdigit(*p) && *p!='-' && *p!='+' &&
			*p!='e' && *p!='E' && *p!='.')
			return false;
		p++;
	}

	return true;
}

int RayEngine::Create()
{
	return 0;
}

void RayEngine::Destroy()
{}

void RayEngine::PrintSimpleError()
{
	printf("!<RayEngine::ReadRib> %d: %s command error\n", m_curline, m_curcmd.c_str());
}