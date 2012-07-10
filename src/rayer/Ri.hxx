#ifndef __RI_HXX__
#define __RI_HXX__

#include "color.hxx"
#include "matrix.hxx"
#include "ShaderInst.hxx"
#include "ShaderVm.hxx"
#include "RGeomNode.hxx"



/*
 * RenderMan Interface Standard Include File
 * (for ANSI Standard C)
 */

/* Definitions of Abstract Types used in RI */
typedef short RtBoolean;
typedef int RtInt;
typedef double RtFloat;
typedef char *RtToken;
typedef RtFloat RtColor[3];
typedef RtFloat RtPoint[3];
typedef RtFloat RtVector[3];
typedef RtFloat RtNormal[3];
typedef RtFloat RtHpoint[4];
typedef RtFloat RtMatrix[4][4];
typedef RtFloat RtBasis[4][4];
typedef RtFloat RtBound[6];
typedef char *RtString;
typedef void *RtPointer;
#define RtVoid void
typedef RtFloat (*RtFilterFunc)(RtFloat, RtFloat, RtFloat, RtFloat);
typedef RtVoid (*RtErrorHandler)(RtInt, RtInt, char *);
typedef RtVoid (*RtProcSubdivFunc)(RtPointer, RtFloat);
typedef RtVoid (*RtProcFreeFunc)(RtPointer);
typedef RtVoid (*RtArchiveCallback)(RtToken, char *, ...);
typedef RtPointer RtObjectHandle;
typedef RtPointer RtLightHandle;

typedef RtPointer RtContextHandle;
/* Extern Declarations for Predefined RI Data Structures */
#define RI_FALSE 0
#define RI_TRUE (! RI_FALSE)
#define RI_INFINITY 1.0e38
#define RI_EPSILON 1.0e-10
#define RI_NULL ((RtToken)0)

extern RtToken RI_FRAMEBUFFER, RI_FILE;
extern RtToken RI_RGB, RI_RGBA, RI_RGBZ, RI_RGBAZ, RI_A, RI_Z, RI_AZ;
extern RtToken RI_PERSPECTIVE, RI_ORTHOGRAPHIC;
extern RtToken RI_HIDDEN, RI_PAINT;
extern RtToken RI_CONSTANT, RI_SMOOTH;
extern RtToken RI_FLATNESS, RI_FOV;
extern RtToken RI_AMBIENTLIGHT, RI_POINTLIGHT, RI_DISTANTLIGHT,
RI_SPOTLIGHT;
extern RtToken RI_INTENSITY, RI_LIGHTCOLOR, RI_FROM, RI_TO, RI_CONEANGLE,
RI_CONEDELTAANGLE, RI_BEAMDISTRIBUTION;
extern RtToken RI_MATTE, RI_METAL, RI_SHINYMETAL,
RI_PLASTIC, RI_PAINTEDPLASTIC;
extern RtToken RI_KA, RI_KD, RI_KS, RI_ROUGHNESS, RI_KR,
RI_TEXTURENAME, RI_SPECULARCOLOR;
extern RtToken RI_DEPTHCUE, RI_FOG, RI_BUMPY;
extern RtToken RI_MINDISTANCE, RI_MAXDISTANCE, RI_BACKGROUND,
RI_DISTANCE, RI_AMPLITUDE;
extern RtToken RI_RASTER, RI_SCREEN, RI_CAMERA, RI_WORLD, RI_OBJECT;
extern RtToken RI_INSIDE, RI_OUTSIDE, RI_LH, RI_RH;
extern RtToken RI_P, RI_PZ, RI_PW, RI_N, RI_NP,
RI_CS, RI_OS, RI_S, RI_T, RI_ST;
extern RtToken RI_BILINEAR, RI_BICUBIC;
extern RtToken RI_LINEAR, RI_CUBIC;
extern RtToken RI_PRIMITIVE, RI_INTERSECTION, RI_UNION, RI_DIFFERENCE;
extern RtToken RI_PERIODIC, RI_NONPERIODIC, RI_CLAMP, RI_BLACK;
extern RtToken RI_IGNORE, RI_PRINT, RI_ABORT, RI_HANDLER;
extern RtToken RI_COMMENT, RI_STRUCTURE, RI_VERBATIM;
extern RtToken RI_IDENTIFIER, RI_NAME, RI_SHADINGGROUP;
extern RtToken RI_WIDTH, RI_CONSTANTWIDTH;
extern RtBasis RiBezierBasis, RiBSplineBasis, RiCatmullRomBasis,
RiHermiteBasis, RiPowerBasis;

#define RI_BEZIERSTEP ((RtInt)3)
#define RI_BSPLINESTEP ((RtInt)1)
#define RI_CATMULLROMSTEP ((RtInt)1)
#define RI_HERMITESTEP ((RtInt)2)
#define RI_POWERSTEP ((RtInt)4)

class Ri
{
public:

RtInt RiLastError;
/* Declarations of All the RenderMan Interface Subroutines */
// RtFloat RiGaussianFilter(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth);
// RtFloat RiBoxFilter(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth);
// RtFloat RiTriangleFilter(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth);
// RtFloat RiCatmullRomFilter(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth);
// RtFloat RiSincFilter(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth);

// RtVoid RiErrorIgnore(RtInt code, RtInt severity, char *msg);
// RtVoid RiErrorPrint(RtInt code, RtInt severity, char *msg);
// RtVoid RiErrorAbort(RtInt code, RtInt severity, char *msg);
// RtVoid RiProcDelayedReadArchive(RtPointer data, RtFloat detail);
// RtVoid RiProcRunProgram(RtPointer data, RtFloat detail);
// RtVoid RiProcDynamicLoad(RtPointer data, RtFloat detail);
// RtContextHandle RiGetContext(void);
// RtVoid RiContext(RtContextHandle);
// RtToken RiDeclare(char *name, char *declaration);

RtVoid RiBegin(RtToken name);
RtVoid RiEnd(void);
RtVoid RiFrameBegin(RtInt frame);
RtVoid RiFrameEnd(void);
RtVoid RiWorldBegin(void);
RtVoid RiWorldEnd(void);


RtVoid RiFormat(RtInt xres, RtInt yres, RtFloat aspect);
RtVoid RiFrameAspectRatio(RtFloat aspect);
RtVoid RiScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top);
// RtVoid RiCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax);
RtVoid RiProjection(RtToken name, ...);
// RtVoid RiProjectionV(RtToken name, RtInt n,RtToken tokens[],RtPointer parms[]);
RtVoid RiClipping(RtFloat hither, RtFloat yon);
// RtVoid RiClippingPlane(RtFloat x, RtFloat y, RtFloat z,	
//		       RtFloat nx, RtFloat ny, RtFloat nz);
// RtVoid RiShutter(RtFloat min, RtFloat max);

// RtVoid RiPixelVariance(RtFloat variation),
RtVoid RiPixelSamples(RtFloat xsamples, RtFloat ysamples);
// RtVoid RiPixelFilter(RtFilterFunc filterfunc, RtFloat xwidth, RtFloat ywidth);
RtVoid RiExposure(RtFloat gain, RtFloat gamma);
// RtVoid RiImager(RtToken name, ...);
// RtVoid RiImagerV(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[]);
// RtVoid RiQuantize(RtToken type, RtInt one, RtInt min, RtInt max, RtFloat ampl);
RtVoid RiDisplay(char *name, RtToken type, RtToken mode, ...);
// RtVoid RiDisplayV(char *name, RtToken type, RtToken mode,
// 	RtInt n, RtToken tokens[], RtPointer parms[]);

// RtVoid RiHider(RtToken type, ...);
// RtVoid RiHiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer parms[]);
// RtVoid RiColorSamples(RtInt n, RtFloat nRGB[], RtFloat RGBn[]);
// RtVoid RiRelativeDetail(RtFloat relativedetail);
// RtVoid RiOption(RtToken name, ...);
// RtVoid RiOptionV(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[]);


RtVoid RiAttributeBegin(void);
RtVoid RiAttributeEnd(void);
RtVoid RiColor(RtColor color);
RtVoid RiOpacity(RtColor color);
// RtVoid RiTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2,
// 	RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);

RtLightHandle RiLightSource(RtToken name, ...);
// RtLightHandle RiLightSourceV(RtToken name,RtInt n,RtToken tokens[],RtPointer parms[]);
// RtLightHandle RiAreaLightSource(RtToken name, ...);
// RtLightHandle RiAreaLightSourceV(RtToken name, RtInt n, 
//	RtToken tokens[], RtPointer parms[]);

RtVoid RiIlluminate(RtLightHandle light, RtBoolean onoff);
RtVoid RiSurface(RtToken name, ...);
// RtVoid RiSurfaceV(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[]);
// RtVoid RiAtmosphere(RtToken name, ...);
// RtVoid RiAtmosphereV(RtToken name,RtInt n,RtToken tokens[],RtPointer parms[]);
// RtVoid RiInterior(RtToken name, ...);
// RtVoid RiInteriorV(RtToken name,RtInt n,RtToken tokens[],RtPointer parms[]);
// RtVoid RiExterior(RtToken name, ...);
// RtVoid RiExteriorV(RtToken name,RtInt n,RtToken tokens[],RtPointer parms[]);
// RtVoid RiShadingRate(RtFloat size);
// RtVoid RiShadingInterpolation(RtToken type);
// RtVoid RiMatte(RtBoolean onoff);

// RtVoid RiBound(RtBound bound);
// RtVoid RiDetail(RtBound bound);
// RtVoid RiDetailRange(RtFloat minvis, RtFloat lowtran, 
//	RtFloat uptran, RtFloat maxvis);
// RtVoid RiGeometricApproximation(RtToken type, RtFloat value);
// RtVoid RiOrientation(RtToken orientation);
// RtVoid RiReverseOrientation(void);
// RtVoid RiSides(RtInt sides);

RtVoid RiIdentity(void);
RtVoid RiTransform(RtMatrix transform);
RtVoid RiConcatTransform(RtMatrix transform);
RtVoid RiPerspective(RtFloat fov);
RtVoid RiTranslate(RtFloat dx, RtFloat dy, RtFloat dz);
RtVoid RiRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
RtVoid RiScale(RtFloat sx, RtFloat sy, RtFloat sz);
// RtVoid RiSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1,
//	RtFloat dx2, RtFloat dy2, RtFloat dz2);
// RtVoid RiDeformation(RtToken name, ...);
// RtVoid RiDeformationV(RtToken name,RtInt n,RtToken tokens[],RtPointer parms[]);
RtVoid RiDisplacement(RtToken name, ...);
// RtVoid RiDisplacementV(RtToken name,RtInt n,
//	RtToken tokens[],RtPointer parms[]);
// RtVoid RiCoordinateSystem(RtToken space);
// RtVoid RiCoordSysTransform(RtToken space);


// RtPoint *RiTransformPoints(RtToken fromspace, RtToken tospace, RtInt n,
//	RtPoint points[]);

RtVoid RiTransformBegin(void);
RtVoid RiTransformEnd(void);

// RtVoid RiAttribute(RtToken name, ...);
// RtVoid RiAttributeV(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[]);

RtVoid RiPolygon(RtInt nverts, ...);
// RtVoid RiPolygonV(RtInt nverts, RtInt n, RtToken tokens[], RtPointer parms[]);
// RtVoid RiGeneralPolygon(RtInt nloops, RtInt nverts[], ...);
// RtVoid RiGeneralPolygonV(RtInt nloops, RtInt nverts[],
//	RtInt n, RtToken tokens[], RtPointer parms[]);
RtVoid RiPointsPolygons(RtInt npolys, RtInt nverts[], RtInt verts[], ...);
// RtVoid RiPointsPolygonsV(RtInt npolys, RtInt nverts[], RtInt verts[],
// 	RtInt n, RtToken tokens[], RtPointer parms[]);
// RtVoid RiPointsGeneralPolygons(RtInt npolys, RtInt nloops[], RtInt nverts[],
// 	RtInt verts[], ...);
// RtVoid RiPointsGeneralPolygonsV(RtInt npolys, RtInt nloops[], RtInt nverts[],
//	RtInt verts[], RtInt n, RtToken tokens[], RtPointer parms[]);
// RtVoid RiBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep);
// RtVoid RiPatch(RtToken type, ...);
// RtVoid RiPatchV(RtToken type, RtInt n, RtToken tokens[], RtPointer parms[]);
// RtVoid RiPatchMesh(RtToken type, RtInt nu, RtToken uwrap,
// 	RtInt nv, RtToken vwrap, ...);
// RtVoid RiPatchMeshV(RtToken type, RtInt nu, RtToken uwrap,
//	RtInt nv, RtToken vwrap,
//	RtInt n, RtToken tokens[], RtPointer parms[]);
// RtVoid RiNuPatch(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin,
//	RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[],
//	RtFloat vmin, RtFloat vmax, ...);
// RtVoid RiNuPatchV(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin,
//	RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[],
//	RtFloat vmin, RtFloat vmax,
//	RtInt n, RtToken tokens[], RtPointer parms[]);
// RtVoid RiTrimCurve(RtInt nloops, RtInt ncurves[], RtInt order[],
//	RtFloat knot[], RtFloat min[], RtFloat max[], RtInt n[],
//	RtFloat u[], RtFloat v[], RtFloat w[]);

RtVoid RiSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax, ...);
// RtVoid RiSphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax,
//	RtInt n, RtToken tokens[], RtPointer parms[]);
RtVoid RiCone(RtFloat height, RtFloat radius, RtFloat tmax, ...);
// RtVoid RiConeV(RtFloat height, RtFloat radius, RtFloat tmax,
//	RtInt n, RtToken tokens[], RtPointer parms[]);
RtVoid RiCylinder(RtFloat radius,RtFloat zmin,RtFloat zmax,RtFloat tmax, ...);
// RtVoid RiCylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax,
//	RtInt n, RtToken tokens[], RtPointer parms[]);
RtVoid RiHyperboloid(RtPoint point1, RtPoint point2, RtFloat tmax, ...);
// RtVoid RiHyperboloidV(RtPoint point1, RtPoint point2, RtFloat tmax,
//	RtInt n, RtToken tokens[], RtPointer parms[]);
RtVoid RiParaboloid(RtFloat rmax,RtFloat zmin,RtFloat zmax,RtFloat tmax, ...);
// RtVoid RiParaboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat tmax,
//	RtInt n, RtToken tokens[], RtPointer parms[]);
RtVoid RiDisk(RtFloat height, RtFloat radius, RtFloat tmax, ...);
// RtVoid RiDiskV(RtFloat height, RtFloat radius, RtFloat tmax,
//	RtInt n, RtToken tokens[], RtPointer parms[]);
RtVoid RiTorus(RtFloat majrad, RtFloat minrad, RtFloat phimin,
	RtFloat phimax, RtFloat tmax, ...);
// RtVoid RiTorusV(RtFloat majrad,RtFloat minrad,
//	RtFloat phimin, RtFloat phimax, RtFloat tmax,
//	RtInt n, RtToken tokens[], RtPointer parms[]);

// RtVoid RiBlobby(RtInt nleaf, RtInt ncode, RtInt code[],
//	RtInt nflt, RtFloat flt[],
//	RtInt nstr, RtToken str[], ...);

// RtVoid RiBlobbyV(RtInt nleaf, RtInt ncode, RtInt code[],
//	RtInt nflt, RtFloat flt[],
//	RtInt nstr, RtToken str[],
//	RtInt n , RtToken tokens[], RtPointer parms[]);

// RtVoid RiCurves(RtToken type, RtInt ncurves,
//	RtInt nvertices[], RtToken wrap, ...);
// RtVoid RiCurvesV(RtToken type, RtInt ncurves, RtInt nvertices[], RtToken wrap,
//	RtInt n, RtToken tokens[], RtPointer parms[]);
// RtVoid RiPoints(RtInt nverts,...);
// RtVoid RiPointsV(RtInt nverts, RtInt n, RtToken tokens[], RtPointer parms[]);
// RtVoid RiSubdivisionMesh(RtToken mask, RtInt nf, RtInt nverts[],
//	RtInt verts[],
//	RtInt ntags, RtToken tags[], RtInt numargs[],
//	RtInt intargs[], RtFloat floatargs[], ...);
// RtVoid RiSubdivisionMeshV(RtToken mask, RtInt nf, RtInt nverts[],
//	RtInt verts[], RtInt ntags, RtToken tags[],
//	RtInt nargs[], RtInt intargs[],
//	RtFloat floatargs[], RtInt n,
//	RtToken tokens[], RtPointer *parms);


// RtVoid RiProcedural(RtPointer data, RtBound bound,
//	RtVoid (*subdivfunc)(RtPointer, RtFloat),
//	RtVoid (*freefunc)(RtPointer)),
//	RiGeometry(RtToken type, ...),
// RtVoid RiGeometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer parms[]);

// RtVoid RiSolidBegin(RtToken operation);
// RtVoid RiSolidEnd(void) ;


// RtObjectHandle RiObjectBegin(void);
// RtVoid RiObjectEnd(void);
// RtVoid RiObjectInstance(RtObjectHandle handle);
// RtVoid RiMotionBegin(RtInt n, ...);
// RtVoid RiMotionBeginV(RtInt n, RtFloat times[]);
// RtVoid RiMotionEnd(void) ;


// RtVoid RiMakeTexture(char *pic, char *tex, RtToken swrap, RtToken twrap,
//	RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...);
// RtVoid RiMakeTextureV(char *pic, char *tex, RtToken swrap, RtToken twrap,
//	RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth,
//	RtInt n, RtToken tokens[], RtPointer parms[]);
// RtVoid RiMakeBump(char *pic, char *tex, RtToken swrap, RtToken twrap,
//	RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...);
// RtVoid RiMakeBumpV(char *pic, char *tex, RtToken swrap, RtToken twrap,
//	RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth,
//	RtInt n, RtToken tokens[], RtPointer parms[]);
// RtVoid RiMakeLatLongEnvironment(char *pic, char *tex,
//	RtFilterFunc filterfunc,
//	RtFloat swidth, RtFloat twidth, ...);
// RtVoid RiMakeLatLongEnvironmentV(char *pic, char *tex,
//	RtFilterFunc filterfunc,
//	RtFloat swidth, RtFloat twidth,
//	RtInt n, RtToken tokens[], RtPointer parms[]);
// RtVoid RiMakeCubeFaceEnvironment(char *px, char *nx, char *py, char *ny,
//	char *pz, char *nz, char *tex, RtFloat fov,
//	RtFilterFunc filterfunc, RtFloat swidth, RtFloat ywidth, ...);
// RtVoid RiMakeCubeFaceEnvironmentV(char *px, char *nx, char *py, char *ny,
//	char *pz, char *nz, char *tex, RtFloat fov,
//	RtFilterFunc filterfunc, RtFloat swidth, RtFloat ywidth,
//	RtInt n, RtToken tokens[], RtPointer parms[]);
// RtVoid RiMakeShadow(char *pic, char *tex, ...);
// RtVoid RiMakeShadowV(char *pic, char *tex,
//	RtInt n, RtToken tokens[], RtPointer parms[]);

// RtVoid RiArchiveRecord(RtToken type, char *format, ...),
// RtVoid RiReadArchive(RtToken name, RtArchiveCallback callback, ...);
// RtVoid RiReadArchiveV(RtToken name, RtArchiveCallback callback,
//	RtInt n, RtToken tokens[], RtPointer parms[]);

// RtVoid RiErrorHandler(RtErrorHandler handler);
/*
Error Codes
1 - 10 System and File Errors
11 - 20 Program Limitations
21 - 40 State Errors
41 - 60 Parameter and Protocol Errors
61 - 80 Execution Errors
*/
#define RIE_NOERROR ((RtInt)0)
#define RIE_NOMEM ((RtInt)1) /* Out of memory */
#define RIE_SYSTEM ((RtInt)2) /* Miscellaneous system error */
#define RIE_NOFILE ((RtInt)3) /* File nonexistent */
#define RIE_BADFILE ((RtInt)4) /* Bad file format */
#define RIE_VERSION ((RtInt)5) /* File version mismatch */
#define RIE_DISKFULL ((RtInt)6) /* Target disk is full */
#define RIE_INCAPABLE ((RtInt)11) /* Optional RI feature */
#define RIE_UNIMPLEMENT ((RtInt)12) /* Unimplemented feature */
#define RIE_LIMIT ((RtInt)13) /* Arbitrary program limit */
#define RIE_BUG ((RtInt)14) /* Probably a bug in renderer */
#define RIE_NOTSTARTED ((RtInt)23) /* RiBegin not called */
#define RIE_NESTING ((RtInt)24) /* Bad begin-end nesting */
#define RIE_NOTOPTIONS ((RtInt)25) /* Invalid state for options */
#define RIE_NOTATTRIBS ((RtInt)26) /* Invalid state for attribs */
#define RIE_NOTPRIMS ((RtInt)27) /* Invalid state for primitives */
#define RIE_ILLSTATE ((RtInt)28) /* Other invalid state */
#define RIE_BADMOTION ((RtInt)29) /* Badly formed motion block */
#define RIE_BADSOLID ((RtInt)30) /* Badly formed solid block */
#define RIE_BADTOKEN ((RtInt)41) /* Invalid token for request */
#define RIE_RANGE ((RtInt)42) /* Parameter out of range */
#define RIE_CONSISTENCY ((RtInt)43) /* Parameters inconsistent */
#define RIE_BADHANDLE ((RtInt)44) /* Bad object/light handle */
#define RIE_NOSHADER ((RtInt)45) /* Can¡¯t load requested shader */
#define RIE_MISSINGDATA ((RtInt)46) /* Required parameters not provided */
#define RIE_SYNTAX ((RtInt)47) /* Declare type syntax error */
#define RIE_MATH ((RtInt)61) /* Zerodivide, noninvert matrix, etc. */
/* Error severity levels */
#define RIE_INFO ((RtInt)0) /* Rendering stats and other info */
#define RIE_WARNING ((RtInt)1) /* Something seems wrong, maybe okay */
#define RIE_ERROR ((RtInt)2) /* Problem. Results may be wrong */
#define RIE_SEVERE ((RtInt)3) /* So bad you should probably abort */

private:
	vector<ShaderInst *> m_shaders;
	ShaderVm *m_sv;		// shader virtual machine
	color3d *m_fb;		// frame buffer for output color
	color3d *m_opfb;	// frame buffer for opacity

	RAttribute m_attr;	// current attribute
	matrix4d m_trans;	// current transformation

	stack<RAttribute> m_attrstack;
	stack<matrix4d> m_transstack;

	vector<CoordSys> m_coordsys;	// internal coordinate system and
					// named coordinate system


	RAttribute m_default_obj_attribute;

	int m_framebegin;	// use to hack when FrameBegin is missing

	ray GenerateRay(int px, int py, int sx, int sy);
	void Render();

	void ConvertParameters(vector<ShaderParam> *pl, ShaderFile *sf, ShaderInst *si);
};



#endif /* __RI_HXX__ */