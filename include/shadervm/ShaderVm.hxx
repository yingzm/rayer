#ifndef __SHADERVM_HXX__
#define __SHADERVM_HXX__

#include "AcceStruct.hxx"
#include "color.hxx"
#include "sli.hxx"
#include "ShaderInst.hxx"
#include "FileManager.hxx"
#include "SimpleMemory.hxx"

class ray;
class GeomObj;
class Light;


class CoordSys
{
public:
	enum {
		RASTER,
		NDC,
		SCREEN,
		CAMERA,
		WORLD,
		OBJECT,
		SHADER
	};
	string name;
	matrix4d mat;
	matrix4d invmat;
};

class ROption
{
public:
	string display_name;
	enum DisplayType{
		DISPLAY_FILE,
		DISPLAY_RGBA,
	} ;
	DisplayType display_type;
	DisplayType display_mode;

	// camera option
	unsigned int format_xres, format_yres;
	double format_par;	// pixel aspect ratio;

	double frameaspectratio;

	double cam_projection;
	double cam_fov;
	double cam_znear, cam_zfar;
	double screen_left, screen_right, screen_bot, screen_top;

	unsigned int pixelsamples_x, pixelsamples_y;

	bool exposure_enable;
	double exposure_gain, exposure_gamma;
};



class ShaderVm
{
public:
	ShaderVm() {}
	~ShaderVm() {}

	void Begin();
	void End();

	void FrameBegin(int frame);
	void FrameEnd();
	
	/* 
	 * Core rendering function, trace a ray and return the color
	 */
	void Trace(const ray &r, color3d &ci, color3d &oi);

	/*
	 * Set global environment
	 */
	int AddLightSource(ShaderInst *obj);
	void AddAreaLight(ShaderInst *obj, GeomObj *go, const matrix4d &m);
	void SetLightOnOff(int light, bool onoff);

	void AddGeomObj(GeomObj *go, RAttribute *si, const matrix4d &m);

	void SetMemi(unsigned int offset, int val);
	void SetMemf(unsigned int offset, double val);
	void SetMemp(unsigned int offset, const point3d &p);
	void SetMemv(unsigned int offset, const vector3d &p);
	void SetMemc(unsigned int offset, const color3d &c);
	void SetMemm(unsigned int offset, const matrix4d &m);

	void SetMemi(unsigned int space, unsigned int offset, int val);
	void SetMemf(unsigned int space, unsigned int offset, double val);
	void SetMemp(unsigned int space, unsigned int offset, const point3d &p);
	void SetMemv(unsigned int space, unsigned int offset, const vector3d &p);
	void SetMemc(unsigned int space, unsigned int offset, const color3d &c);
	void SetMemm(unsigned int space, unsigned int offset, const matrix4d &m);

	int GetMemi(unsigned int offset);
	double GetMemf(unsigned int offset);
	point3d GetMemp(unsigned int offset);
	vector3d GetMemv(unsigned int offset);
	color3d GetMemc(unsigned int offset);
	matrix4d GetMemm(unsigned int offset);
	char *GetMems(unsigned int offset);

	int GetMemi(unsigned int space, unsigned int offset);
	double GetMemf(unsigned int space, unsigned int offset);
	point3d GetMemp(unsigned int space, unsigned int offset);
	vector3d GetMemv(unsigned int space, unsigned int offset);
	color3d GetMemc(unsigned int space, unsigned int offset);
	matrix4d GetMemm(unsigned int space, unsigned int offset);

	int *GetMemAddri(unsigned addrcode);
	double *GetMemAddrf(unsigned addrcode);
	point3d *GetMemAddrp(unsigned addrcode);
	vector3d *GetMemAddrv(unsigned addrcode);
	color3d *GetMemAddrc(unsigned addrcode);
	matrix4d *GetMemAddrm(unsigned addrcode);






	int QueryShaderParamPos(const string &paramname);

	void SetCoordSys(int i, const matrix4d &m);

	ShaderFileManager m_shaderfm;
	ROption m_options;
private:
	unsigned int m_reg[16];		// 16 registers
	char *m_mem[16];	// pointer to the space register refers
	SimpleMemory m_datamem;
	SimpleMemory m_codemem;
	SimpleMemory m_stackmem;
	SimpleMemory m_localenvstackmem;

	AcceStruct m_accestruct;

	bool m_framebegin;
	vector<Light *> m_globallights;
	vector<Light *> m_lights;

	ShaderInst m_defaultsurface;

	IntersectInfo m_ii;

	ShaderInst *m_curshader;
	void CalcAfterEval();

	/*
	 * shader vm core
	 */
	int LoadShader(ShaderInst *);
	int PreExecShader(ShaderInst *si);
	void SetEnvironment(ShaderInst *);
	int ExecShader();

	int ExecInst();		// execute an instruction

	/*
	 * call c functions 
	 */
	int m_callparamindex;
	int GetCallParamEvalduv();
	double *GetCallParamf();
	point3d *GetCallParamp();
	vector3d *GetCallParamv();
	color3d *GetCallParamc();
	matrix4d *GetCallParamm();
	char *GetCallParams();
	double *GetCallParamfa(int *n);
	color3d *GetCallParamca(int *n);
	void CallReturn();
	void CallReturnf(double f);
	void CallReturnp(const point3d &f);
	void CallReturnv(const vector3d &v);
	void CallReturnc(const color3d &c);
	void CallReturnm(const matrix4d &m);
	
	void CallCFunc(int i);


	/*
	 * 
	 */
	void *GetAddr(int addrcode);
	int AddrSpace(int addrcode);
	int AddrOffset(int addrcode);
	unsigned int AddrCode(unsigned int space, unsigned int offset);

	int AddString(const char *s);


	color3d CalcAmbientLight();
	color3d CalcDiffuseLight();
	color3d CalcSpecularLight();

	/*
	 * context switch when shader change
	 */
	void PushAllContext();
	void PopAllContext();
	stack<IntersectInfo> m_iistack;

	/*
	 * Evaluate Du(x), Dv(x)
	 */
	vector3d Duv(int variable);
	vector3d Dvv(int variable);
	double Duf(int variable);
	double Dvf(int variable);

	class DerivValues
	{
	public:
		// don't need a type indicator
		double u_flt;
		color3d u_clr;
		vector3d u_vtr;
	};
	map<int, DerivValues> m_derivu;
	map<int, DerivValues> m_derivv;
	int m_mode;	// mode==0, normal mode, mode==1, evaluate mode
	int m_codeoffset;	// the relative codeoffset if m_mode is evaluate mode


	const CoordSys &GetCoordSys(const char *name) const;
	vector<CoordSys> m_coordsys;

	void SubStack(int i) { m_reg[REG_LSTOP]-=i; m_mem[REG_LSTOP]-=i; }
	void AddStack(int i) { m_reg[REG_LSTOP]+=i; m_mem[REG_LSTOP]+=i; }

	color3d _specularbrdf(vector3d L, vector3d N, vector3d V, double roughness);

	FileManager m_texturefm;
};

inline int ShaderVm::AddrSpace(int addrcode)
{
	return (addrcode>>24)&0xf;
}

inline int ShaderVm::AddrOffset(int addrcode)
{
	return (addrcode&0xffffff);
}

inline unsigned int ShaderVm::AddrCode(unsigned int space, unsigned int offset)
{
	return ((space&0xf)<<24)+(offset&0xffffff);
}

inline int ShaderVm::GetMemi(unsigned int space, unsigned int offset)
{
	return *(int *)(m_mem[space] + offset);
}

inline double ShaderVm::GetMemf(unsigned int space, unsigned int offset)
{
	return *(double *)(m_mem[space]+offset);
}

inline point3d ShaderVm::GetMemp(unsigned int space, unsigned int offset)
{
	return *(point3d *)(m_mem[space]+offset);
}

inline vector3d ShaderVm::GetMemv(unsigned int space, unsigned int offset)
{
	return *(vector3d *)(m_mem[space]+offset);
}

inline color3d ShaderVm::GetMemc(unsigned int space, unsigned int offset)
{
	return *(color3d *)(m_mem[space]+offset);
}

inline matrix4d ShaderVm::GetMemm(unsigned int space, unsigned int offset)
{
	return *(matrix4d *)(m_mem[space]+offset);
}

inline int ShaderVm::GetMemi(unsigned int offset)
{
	return *(int *)(m_mem[AddrSpace(offset)]+AddrOffset(offset));
}

inline double ShaderVm::GetMemf(unsigned int offset)
{
	return *(double *)(m_mem[AddrSpace(offset)]+AddrOffset(offset));
}

inline point3d ShaderVm::GetMemp(unsigned int offset)
{
	return *(point3d *)(m_mem[AddrSpace(offset)]+AddrOffset(offset));	
}

inline vector3d ShaderVm::GetMemv(unsigned int offset)
{
	return *(vector3d *)(m_mem[AddrSpace(offset)]+AddrOffset(offset));
}

inline color3d ShaderVm::GetMemc(unsigned int offset)
{
	return *(color3d *)(m_mem[AddrSpace(offset)]+AddrOffset(offset));
}

inline matrix4d ShaderVm::GetMemm(unsigned int offset)
{
	return *(matrix4d *)(m_mem[AddrSpace(offset)]+AddrOffset(offset));
}

inline char *ShaderVm::GetMems(unsigned int offset)
{
	int offreal = GetMemi(offset);
	return (char *)(m_mem[AddrSpace(offreal)]+AddrOffset(offreal));
}

inline int ShaderVm::GetCallParamEvalduv()
{
	if (m_reg[REG_CS]+m_callparamindex>=m_reg[REG_LSTOP])
		return 0;
	
	int *mem = (int *)(m_codemem.GetMem(0)+m_reg[REG_NP]-4);
	if (*(mem-1)!=PUSHF && *(mem-1)!=PUSHV)
		return 0;

	return *mem;
}

inline double *ShaderVm::GetCallParamf()
{
	if (m_reg[REG_CS]+m_callparamindex>=m_reg[REG_LSTOP])
		return NULL;

	int tmp = m_callparamindex;
	m_callparamindex += 8;

	return (double *)(m_mem[REG_CS]+tmp);
}

inline point3d *ShaderVm::GetCallParamp()
{
	if (m_reg[REG_CS]+m_callparamindex>=m_reg[REG_LSTOP])
		return NULL;

	int tmp = m_callparamindex;
	m_callparamindex += 24;

	return (point3d *)(m_mem[REG_CS]+tmp);
}

inline vector3d *ShaderVm::GetCallParamv()
{
	if (m_reg[REG_CS]+m_callparamindex>=m_reg[REG_LSTOP])
		return NULL;

	int tmp = m_callparamindex;
	m_callparamindex += 24;

	return (vector3d *)(m_mem[REG_CS]+tmp);
}

inline color3d *ShaderVm::GetCallParamc()
{
	if (m_reg[REG_CS]+m_callparamindex>=m_reg[REG_LSTOP])
		return NULL;

	int tmp = m_callparamindex;
	m_callparamindex += 24;

	return (color3d  *)(m_mem[REG_CS]+tmp);
}


inline matrix4d *ShaderVm::GetCallParamm()
{
	if (m_reg[REG_CS]+m_callparamindex>=m_reg[REG_LSTOP])
		return NULL;

	int tmp = m_callparamindex;
	m_callparamindex += 8*16;

	return (matrix4d *)(m_mem[REG_CS]+tmp);
}

inline char *ShaderVm::GetCallParams()
{
	if (m_reg[REG_CS]+m_callparamindex>=m_reg[REG_LSTOP])
		return NULL;

	int addr = *(int *)(m_mem[REG_CS]+m_callparamindex);

	m_callparamindex += 4;


	return GetMems(addr);
}

inline double *ShaderVm::GetCallParamfa(int *n)
{
	// TODO
	return NULL;
}

inline color3d *ShaderVm::GetCallParamca(int *n)
{
	// TODO
	return NULL;
}

inline void ShaderVm::SetMemf(unsigned int offset, double val)
{
	*(double *)(m_mem[AddrSpace(offset)]+AddrOffset(offset)) = val;
}

inline void ShaderVm::SetMemi(unsigned int offset, int val)
{
	*(int *)(m_mem[AddrSpace(offset)]+AddrOffset(offset)) = val;
}

inline void ShaderVm::SetMemp(unsigned int offset, const point3d &p)
{
	*((point3d *)(m_mem[AddrSpace(offset)]+AddrOffset(offset))) = p;
}

inline void ShaderVm::SetMemv(unsigned int offset, const vector3d &p)
{
	*((vector3d *)(m_mem[AddrSpace(offset)]+AddrOffset(offset))) = p;
}

inline void ShaderVm::SetMemc(unsigned int offset, const color3d &c)
{
	*((color3d *)(m_mem[AddrSpace(offset)]+AddrOffset(offset))) = c;
}

inline	void ShaderVm::SetMemm(unsigned int offset, const matrix4d &m)
{
	*((matrix4d *)(m_mem[AddrSpace(offset)]+AddrOffset(offset))) = m;
}

inline void ShaderVm::SetMemi(unsigned int space, unsigned int offset, int val)
{
	*((int *)(m_mem[space]+offset)) = val;
}

inline void ShaderVm::SetMemf(unsigned int space, unsigned int offset, double val)
{
	*((double *)(m_mem[space]+offset)) = val;
}

inline void ShaderVm::SetMemp(unsigned int space, unsigned int offset, const point3d &p)
{
	*((point3d *)(m_mem[space]+offset)) = p;
}

inline void ShaderVm::SetMemv(unsigned int space, unsigned int offset, const vector3d &p)
{
	*((vector3d *)(m_mem[space]+offset)) = p;
}

inline void ShaderVm::SetMemc(unsigned int space, unsigned int offset, const color3d &c)
{
	*((color3d *)(m_mem[space]+offset)) = c;
}

inline void ShaderVm::SetMemm(unsigned int space, unsigned int offset, const matrix4d &m)
{
	*((matrix4d *)(m_mem[space]+offset)) = m;
}

inline double *ShaderVm::GetMemAddrf(unsigned addrcode)
{
	return (double *)(m_mem[AddrSpace(addrcode)]+AddrOffset(addrcode));
}

inline point3d *ShaderVm::GetMemAddrp(unsigned addrcode)
{
	return (point3d *)(m_mem[AddrSpace(addrcode)]+AddrOffset(addrcode));
}

inline vector3d *ShaderVm::GetMemAddrv(unsigned addrcode)
{
	return (vector3d *)(m_mem[AddrSpace(addrcode)]+AddrOffset(addrcode));
}

inline color3d *ShaderVm::GetMemAddrc(unsigned addrcode)
{
	return (color3d *)(m_mem[AddrSpace(addrcode)]+AddrOffset(addrcode));
}

inline matrix4d *ShaderVm::GetMemAddrm(unsigned addrcode)
{
	return (matrix4d *)(m_mem[AddrSpace(addrcode)]+AddrOffset(addrcode));
}

inline int *ShaderVm::GetMemAddri(unsigned addrcode)
{
	return (int *)(m_mem[AddrSpace(addrcode)]+AddrOffset(addrcode));
}

inline void ShaderVm::CallReturn()
{
	m_reg[REG_LSTOP] = m_reg[REG_CS];
	m_mem[REG_LSTOP] = m_mem[REG_CS];
}

inline	void ShaderVm::CallReturnf(double f)
{
	SetMemf(REG_CS, 0, f);
	m_reg[REG_LSTOP] = m_reg[REG_CS]+8;
	m_mem[REG_LSTOP] = m_mem[REG_CS]+8;
}

inline void ShaderVm::CallReturnp(const point3d &p)
{
	SetMemp(REG_CS, 0, p);
	m_reg[REG_LSTOP] = m_reg[REG_CS]+24;
	m_mem[REG_LSTOP] = m_mem[REG_CS]+24;
}

inline void ShaderVm::CallReturnv(const vector3d &v)
{
	SetMemv(REG_CS, 0, v);
	m_reg[REG_LSTOP] = m_reg[REG_CS]+24;
	m_mem[REG_LSTOP] = m_mem[REG_CS]+24;
}

inline void ShaderVm::CallReturnc(const color3d &c)
{
	SetMemc(REG_CS, 0, c);
	m_reg[REG_LSTOP] = m_reg[REG_CS]+24;
	m_mem[REG_LSTOP] = m_mem[REG_CS]+24;
}

inline void ShaderVm::CallReturnm(const matrix4d &m)
{
	SetMemm(REG_CS, 0, m);
	m_reg[REG_LSTOP] = m_reg[REG_CS]+16*8;
	m_mem[REG_LSTOP] = m_mem[REG_CS]+16*8;
}



#endif /* __SHADERVM_HXX__ */
