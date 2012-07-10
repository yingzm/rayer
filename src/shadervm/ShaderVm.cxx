#include "common.hxx"
#include "ShaderVm.hxx"
#include "AcceStruct.hxx"
#include "ray.hxx"
#include "ShaderInst.hxx"
#include "Light.hxx"
#include "matrix.hxx"
#include "RGeomNode.hxx"
#include "ShaderFile.hxx"
#include "TextureFile.hxx"

#define R_FLG	(m_reg[REG_FLG])
#define R_NP	(m_reg[REG_NP])
#define R_LE	(m_reg[REG_LE])
#define R_LP	(m_reg[REG_LP])
#define R_LS	(m_reg[REG_LS])
#define R_CS	(m_reg[REG_CS])
#define R_LSTOP	(m_reg[REG_LSTOP])
#define R_DATAS	(m_reg[REG_DATAS])
#define R_DATAE (m_reg[REG_DATAE])
#define R_CODES	(m_reg[REG_CODES])
#define R_CODEE	(m_reg[REG_CODEE])
#define R_LES	(m_reg[REG_LES])

void ShaderVm::Trace(const ray &r, color3d &ci, color3d &oi)
{
	ci = color3d::black;
	oi = color3d::white;

	m_ii.m_ray = r;
	if (m_accestruct.IntersectNearest(r, m_ii)) {
		// TODO: the flag should not be 0
		m_ii.m_node->GetGeomObj()->Eval(m_ii.m_ic, 0xff);	

		CalcAfterEval();

		ShaderInst *si = m_ii.m_node->GetAttribute()->displacement;
		if (si!=NULL) {
			if (LoadShader(si)>=0) {
				PreExecShader(si);
				if (ExecShader()>=0) {
					m_ii.m_ic.m_intpt = GetMemp(REG_LE, DLE_P);
					m_ii.m_ic.m_normal = GetMemv(REG_LE, DLE_N);
				}
			}
		}

		si = m_ii.m_node->GetAttribute()->surface; 
		if (si==NULL)
			si = m_ii.m_node->GetAttribute()->surface = &m_defaultsurface;

		if (LoadShader(si)>=0) {
			PreExecShader(si);
			if (ExecShader()>=0) {
				ci = GetMemc(REG_LE, SLE_Ci);
				oi = GetMemc(REG_LE, SLE_Oi);
			}
		}
	}

	return ; 
}

void ShaderVm::FrameBegin(int frame)
{
	m_framebegin = true;
	m_accestruct.RemoveAll();
	int n = m_lights.size();
	for (int i=0; i<n; ++i) {
		Light *l = m_lights[i];
		delete l;
	}
	m_lights.erase(m_lights.begin(), m_lights.end());
	n = m_globallights.size();
	for (i=0; i<n; ++i) {
		m_lights.push_back(m_globallights[i]);
	}
}

void ShaderVm::FrameEnd()
{
	m_framebegin = false;
}


void ShaderVm::SetLightOnOff( int light, bool onoff)
{
	if (light>=(int)m_lights.size()) {
		printf("!<ShaderVm::SetLightOnOff>: Invalid light handle %d\n", light);
		return;
	}

	Light *lt = m_lights[light];
	lt->on = onoff;
}

void ShaderVm::AddGeomObj(GeomObj *go, RAttribute *si, const matrix4d &m)
{
	m_accestruct.AddGeomObj(go, si, m);
}

int ShaderVm::ExecShader()
{
	if (m_mode==0) {	// normal mode
		while (ExecInst()>=0)
			;
	}
	else {
		while ((R_NP-R_CODES)<m_codeoffset && ExecInst()>=0)
			;
	}
	
	return 0;
}

int ShaderVm::QueryShaderParamPos(const string &paramname)
{
	return 0;
}

int ShaderVm::LoadShader(ShaderInst *si)
{
	ShaderFile *sf;
	if (si->file!=NULL)
		sf = si->file;
	else
		sf = si->file = m_shaderfm.GetShaderFile(si->name.c_str());
	if (sf==NULL)
		return -1;

	// setup lp with respect to the shader type
	switch (sf->GetType()) {
	case SHADER_SURFACE:
                R_LP = R_LE+SLE_SIZE*4;
		break;
	case SHADER_LIGHT:
		R_LP = R_LE+LLE_SIZE*4;
		break;
	case SHADER_DISPLACEMENT:
		R_LP = R_LE+DLE_SIZE*4;
		break;
	case SHADER_VOLUME:
		R_LP = R_LE+VLE_SIZE*4;
		break;
	case SHADER_IMAGER:
		R_LP = R_LE+ILE_SIZE*4;
		break;
	}
	char *stackmem = m_stackmem.GetMem(0);
	m_mem[REG_LP] = stackmem+R_LP;

	// setup ls & cs with respect to the shader
	int memsize = sf->GetParamDefaultSize();
	R_LS = R_LP+sf->GetParamDefaultSize();
	R_CS = R_LSTOP = R_LS;
	m_mem[REG_CS] = m_mem[REG_LSTOP] = m_mem[REG_LS] = stackmem+R_LS;

	// load param default
	const char *mem = sf->GetParamDefault();
	memcpy(m_mem[REG_LP], mem, memsize);

	// load data
	memsize = sf->GetDataSize();
	mem = sf->GetData();
	m_mem[REG_DATAS] = m_datamem.GetMem(0)+R_DATAS;
	memcpy(m_mem[REG_DATAS], mem, memsize);
	R_DATAE = R_DATAS+memsize;
	
	// load code
	memsize = sf->GetCodeSize();
	mem = sf->GetCode();
	m_mem[REG_CODES] = m_codemem.GetMem(0)+R_CODES;
	memcpy(m_mem[REG_CODES], mem, memsize);
	R_CODEE = R_CODES+memsize;
	R_NP = R_CODES;

	m_curshader = si;
	if (m_mode==0) {
		m_derivu.erase(m_derivu.begin(), m_derivu.end());
		m_derivv.erase(m_derivv.begin(), m_derivv.end());
	}

	return 0;
}

void ShaderVm::Begin()
{
	m_options.display_mode = ROption::DISPLAY_RGBA;
	m_options.display_name = "untitled.tiff";
	m_options.display_type = ROption::DISPLAY_FILE;
	m_options.format_par = 1.0;
	m_options.format_xres = 640;
	m_options.format_yres = 480;
	m_options.frameaspectratio = 4/3;
	m_options.screen_bot = -1;
	m_options.screen_left = -4/3;
	m_options.screen_right = 4/3;
	m_options.screen_top = 1;
	m_options.cam_projection = 0;
	m_options.cam_fov = deg2rad(90);
	m_options.cam_znear = RFLOAT_EPSILON; g_znear = RFLOAT_EPSILON;
	m_options.cam_zfar = RFLOAT_INFINITY; g_zfar = RFLOAT_INFINITY;
	m_options.pixelsamples_x = m_options.pixelsamples_y = 2;
	m_options.exposure_enable = false; 
	m_options.exposure_gain = 1.0;
	m_options.exposure_gamma = 1.0;
	// TODO default pixel samples should be 2
	// m_option.pixelsamples_x = 2;
	// m_option.pixelsamples_y = 2;

	// 7 internal coordinate system
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

	m_defaultsurface.name = "defaultsurface";

	m_stackmem.Reserve(4096*4);
	m_datamem.Reserve(4096*4);
	m_codemem.Reserve(8192*4);
	m_localenvstackmem.Reserve(256*4);

	memset(m_reg, 0, 16*sizeof(int));
	memset(m_mem, 0, 16*sizeof(int));
	m_mem[REG_LE] = m_stackmem.GetMem(0);
	m_mem[REG_DATAS] = m_datamem.GetMem(0);
	m_mem[REG_CODES] = m_codemem.GetMem(0);

	m_mode = 0;
	m_codeoffset = 0;

	m_framebegin = false;
}

void ShaderVm::End()
{
}


int ShaderVm::ExecInst()
{
#define OPCODE_0 *(int *)(codemem+R_NP)
#define OPCODE_1 *(int *)(codemem+R_NP+4)
#define OPCODE_2 *(int *)(codemem+R_NP+8)
#define OPCODE_3 *(int *)(codemem+R_NP+12)

	double f0, f1;
	double *ff;	// double destination
	vector3d v0, v1, v2;
	vector3d *dv;
	color3d c0;
	char *s0, *s1;
	int i0, i1;
	int *ds;
	char *codemem = m_codemem.GetMem(0);


	switch (OPCODE_0) {
	case NOP:
		R_NP += 4;
		break;
	case END:
		return -1;
	case MOVF:
		ff = GetMemAddrf(OPCODE_1);
		*ff = GetMemf(OPCODE_2);

		R_NP += 12;
		break;
	case MOVV:
		dv = GetMemAddrv(OPCODE_1);
		*dv = GetMemv(OPCODE_2);

		R_NP += 12;
		break;
	case MOVS:
		ds = GetMemAddri(OPCODE_1);
		*ds = GetMemi(OPCODE_2);

		R_NP += 12;
		break;
	case ADDR:
		i0 = OPCODE_1;
		i1 = OPCODE_2;

		m_reg[i0] += i1; m_mem[i0] += i1;
		// ERROR!
		m_reg[REG_CS] += i1; m_mem[REG_CS] += i1;

		R_NP += 12;
		break;
	case SUBR:
		i0 = OPCODE_1;
		i1 = OPCODE_2;

		m_reg[i0] -= i1; m_mem[i0] -= i1;

		// ERROR!
		m_reg[REG_CS] -= i1; m_mem[REG_CS] -= i1;

		R_NP += 12;
		break;
	case ADDF:
		ff = GetMemAddrf(OPCODE_1);
		*ff = GetMemf(OPCODE_2)+GetMemf(OPCODE_3);

		R_NP += 16;
		break;
	case ADDV:
		dv = GetMemAddrv(OPCODE_1);
		*dv = GetMemv(OPCODE_2)+GetMemv(OPCODE_3);

		R_NP += 16;
		break;
	case SUBF:
		ff = GetMemAddrf(OPCODE_1);
		*ff = GetMemf(OPCODE_2)-GetMemf(OPCODE_3);

		R_NP += 16;
		break;
	case SUBV:
		dv = GetMemAddrv(OPCODE_1);
		*dv = GetMemv(OPCODE_2)-GetMemv(OPCODE_3);

		R_NP += 16;
		break;
	case MULF:
		ff = GetMemAddrf(OPCODE_1);
		*ff = GetMemf(OPCODE_2)*GetMemf(OPCODE_3);

		R_NP += 16;
		break;
	case MULV:
		dv = GetMemAddrv(OPCODE_1);
		*dv = GetMemv(OPCODE_2) * GetMemf(OPCODE_3);

		R_NP += 16;
		break;
	case MULVV:
		dv = GetMemAddrv(OPCODE_1);
		v0 = GetMemv(OPCODE_2);
		v1 = GetMemv(OPCODE_3);
		dv->set_comp(v0.x()*v1.x(), v0.y()*v1.y(), v0.z()*v1.z());

		R_NP += 16;
		break;
	case DIVF:
		ff = GetMemAddrf(OPCODE_1);
		*ff = GetMemf(OPCODE_2)/GetMemf(OPCODE_3);

		R_NP += 16;
		break;
	case DIVV:
		dv = GetMemAddrv(OPCODE_1);
		*dv = GetMemv(OPCODE_2)/GetMemf(OPCODE_3);

		R_NP += 16;
		break;
	case DOTV:
		ff = GetMemAddrf(OPCODE_1);
		*ff = GetMemv(OPCODE_2)%GetMemv(OPCODE_3);

		R_NP += 16;
		break;
	case NEGF:
		ff = GetMemAddrf(OPCODE_1);
		*ff = -(*ff);

		R_NP += 8;
		break;
	case NEGV:
		dv = GetMemAddrv(OPCODE_1);
		*dv = - (*dv);

		R_NP += 8;
		break;
	case NEG:
		if (R_FLG & FLAG_CMP)
			R_FLG &= ~FLAG_CMP;
		else
			R_FLG |= FLAG_CMP;
		R_NP += 1;
		break;
	case CRSV:
		dv = GetMemAddrv(OPCODE_1);
		*dv = GetMemv(OPCODE_2)*GetMemv(OPCODE_3);

		R_NP += 16;
		break;
	case JMP:
		R_NP += OPCODE_1;
		break;
	case JT:
		if (R_FLG&FLAG_CMP)
			R_NP += OPCODE_1;
		else
			R_NP += 8;

		break;
	case JF:
		if (!(R_FLG&FLAG_CMP))
			R_NP += OPCODE_1;
		else
			R_NP += 8;
		break;
	case LTF:
		f0 = GetMemf(OPCODE_1);
		f1 = GetMemf(OPCODE_2);

		if ( f0<f1 )
			R_FLG |= FLAG_CMP;
		else
			R_FLG &= ~FLAG_CMP;

		R_NP += 12;
		break;
	case LEF:
		f0 = GetMemf(OPCODE_1);
		f1 = GetMemf(OPCODE_2);

		if ( f0<=f1 )
			R_FLG |= FLAG_CMP;
		else
			R_FLG &= ~FLAG_CMP;
		R_NP += 12;
		break;
	case GTF:
		f0 = GetMemf(OPCODE_1);
		f1 = GetMemf(OPCODE_2);

		if ( f0>f1 )
			R_FLG |= FLAG_CMP;
		else
			R_FLG &= ~FLAG_CMP;
		R_NP += 12;
		break;
	case GEF:
		f0 = GetMemf(OPCODE_1);
		f1 = GetMemf(OPCODE_2);

		if ( f0>=f1 )
			R_FLG |= FLAG_CMP;
		else
			R_FLG &= ~FLAG_CMP;
		R_NP += 12;
		break;
	case NEF:
		f0 = GetMemf(OPCODE_1);
		f1 = GetMemf(OPCODE_2);

		if ( f0!=f1 )
			R_FLG |= FLAG_CMP;
		else
			R_FLG &= ~FLAG_CMP;
		R_NP += 12;
		break;
	case EQF:
		f0 = GetMemf(OPCODE_1);
		f1 = GetMemf(OPCODE_2);

		if ( f0==f1 )
			R_FLG |= FLAG_CMP;
		else
			R_FLG &= ~FLAG_CMP;
		R_NP += 12;
		break;
	case NEV:
		v0 = GetMemv(OPCODE_1);
		v1 = GetMemv(OPCODE_2);

		if (v0!=v1)
			R_FLG |= FLAG_CMP;
		else
			R_FLG &= ~FLAG_CMP;

		R_NP += 12;
		break;
	case EQV:
		v0 = GetMemv(OPCODE_1);
		v1 = GetMemv(OPCODE_2);
		if (v0==v1)
			R_FLG |= FLAG_CMP;
		else
			R_FLG &= ~FLAG_CMP;

		R_NP += 12;
		break;
	case NES:
		s0 = GetMems(OPCODE_1);
		s1 = GetMems(OPCODE_2);
		if (strcmp(s0, s1)==0)
			R_FLG &= ~FLAG_CMP;
		else
			R_FLG |= FLAG_CMP;

		R_NP += 12;
		break;
	case EQS:
		s0 = GetMems(OPCODE_1);
		s1 = GetMems(OPCODE_2);
		if (strcmp(s0, s1)==0) 
			R_FLG |= FLAG_CMP;
		else
			R_FLG &= ~FLAG_CMP;

		R_NP += 12;
		break;
	case PUSHF:
		f0 = GetMemf(OPCODE_1);
		SetMemf(REG_LSTOP, 0, f0);
		R_LSTOP += 8; m_mem[REG_LSTOP] += 8;

		R_NP += 8;
		break;
	case PUSHV:
		v0 = GetMemv(OPCODE_1);
		SetMemv(REG_LSTOP, 0, v0);
		R_LSTOP += 24; m_mem[REG_LSTOP] +=24;

		R_NP += 8;
		break;
	case POPF:
		ff = GetMemAddrf(OPCODE_1);
		R_LSTOP -= 8; m_mem[REG_LSTOP] -=8;
		*ff = GetMemf(REG_LSTOP, 0);
		
		R_NP += 8;
		break;
	case POPV:
		dv = GetMemAddrv(OPCODE_1);
		R_LSTOP -= 24; m_mem[REG_LSTOP] -= 24;
		*dv = GetMemv(REG_LSTOP, 0);

		ASSERT(R_LSTOP==R_CS);

		R_NP += 8;
		break;
	case CCALL:
		CallCFunc(OPCODE_1);

		R_NP += 8;
		break;
	case PUSHS:
		i0 = OPCODE_1;
		SetMemi(REG_LSTOP, 0, i0);
		R_LSTOP += 4; m_mem[REG_LSTOP] += 4;

		R_NP += 8;
		break;
	default:
		printf("!<ShaderVm::ExecInst> Unknown instruction\n");
		break;
	}

	return 0;
}


color3d ShaderVm::CalcAmbientLight()
{
	color3d clr(color3d::black);

	// setup light environment

	int n = (int)m_lights.size();
	vector<Light *>::iterator it = m_lights.begin();
	for (; it<m_lights.end(); ++it) {
		if ((*it)->type==Light::NONAMBIENT)
			continue;
		ShaderInst *si= (*it)->m_shader;
		PushAllContext();
		int rt = LoadShader(si);
		if (rt<0) {
			printf("!<ShaderVm::CalcAmbientLight> Failed to load shader %s\n",
				si->name.c_str());
			PopAllContext();
			continue;
		}

		SetMemv(REG_LE, LLE_L, vector3d::zero);
		SetMemc(REG_LE, LLE_Cl, color3d::black);
		PreExecShader(si);
		rt = ExecShader();
		if (rt<0) {
			PopAllContext();
			continue;
		}

		clr += GetMemc(REG_LE, LLE_Cl);

		PopAllContext();
	}


	return clr;
}

/*
 * color diffuse(normal N)
 * {
 *	color C = 0;
 *	illuminance (P, N, PI/2)
 *		C += Cl*normalize(L).N
 *	return C;
 * }
 */
color3d ShaderVm::CalcDiffuseLight()
{
	color3d clr(color3d::black);
	vector3d *N = GetCallParamv();

	// setup light environment

	int n = (int)m_lights.size();
	vector<Light *>::iterator it = m_lights.begin();
	for (; it<m_lights.end(); ++it) {
		if ( (*it)->type == Light::AMBIENT )
			continue;

		// save some common context
		point3d _P = GetMemp(REG_LE, SLE_P);
		vector3d _N = GetMemv(REG_LE, SLE_N);
		PushAllContext();
		ShaderInst *si = (*it)->m_shader;
		int rt = LoadShader(si);

		if (rt<0) {
			printf("!<ShaderVm::CalcDiffuseLight> Failed to load shader %s\n",
				si->name.c_str());
			PopAllContext();
			continue;
		}

		SetMemp(REG_LE, LLE_P, _P);
		SetMemv(REG_LE, LLE_N, _N);
		SetMemv(REG_LE, LLE_L, vector3d::zero);
		SetMemc(REG_LE, LLE_Cl, color3d::black);
		PreExecShader(si);
		rt = ExecShader();
		if (rt<0) {
			PopAllContext();
			continue;
		}

		color3d clr0 = GetMemc(REG_LE, LLE_Cl);
		vector3d L = GetMemv(REG_LE, LLE_L);
		L.normalize();
		PopAllContext();

		// !! Renderman Specification indicate the following equations
		// clr += clr0*(L%(*N));
		// but it won't work because when L and N goes in the oposite 
		// direction, it will have a negative dot value
		// TODO: add sides option
		clr += clr0*max(0.0, -L%(*N));
	}

	return clr;
}

color3d ShaderVm::CalcSpecularLight()
{
	color3d clr(color3d::black);

	vector3d *N = GetCallParamv();
	vector3d *V = GetCallParamv();
	double *roughness = GetCallParamf();

	// setup light environment

	int n = (int)m_lights.size();
	vector<Light *>::iterator it = m_lights.begin();
	for (; it<m_lights.end(); ++it) {
		if ( (*it)->type == Light::AMBIENT)
			continue;
		point3d _P = GetMemp(REG_LE, SLE_P);
		vector3d _N = GetMemv(REG_LE, SLE_N);
		SetMemv(REG_LE, LLE_L, vector3d::zero);
		SetMemc(REG_LE, LLE_Cl, color3d::black);
		PushAllContext();
		ShaderInst *si = (*it)->m_shader;
		int rt = LoadShader(si);
		if (rt<0) {
			printf("!<ShaderVm::CalcAmbientLight> Failed to load shader %s\n",
				si->name.c_str());
			PopAllContext();
			continue;
		}

		SetMemp(REG_LE, LLE_P, _P);
		SetMemv(REG_LE, LLE_N, _N);
		PreExecShader(si);
		rt = ExecShader();
		if (rt<0) {
			PopAllContext();
			continue;
		}

		color3d clr0 = GetMemc(REG_LE, LLE_Cl);
		vector3d L = GetMemv(REG_LE, LLE_L);
		L.normalize();
		PopAllContext();
		if ((-L)%(*N)>0)
			clr += clr0*_specularbrdf(-L, (*N), *V, *roughness);
	}

	return clr;
}

void ShaderVm::PushAllContext()
{
	// save registers to local environment stack
	char *p = m_localenvstackmem.GetMem(0);
	memcpy(p+m_reg[REG_LES], m_reg, sizeof(int)*16);	// copy 16 registers

        m_reg[REG_LES] += sizeof(int)*16;

	// setup new 
	m_reg[REG_LE] = m_reg[REG_LSTOP]; m_mem[REG_LE] = m_mem[REG_LSTOP];
	m_reg[REG_DATAS] = m_reg[REG_DATAE];
	m_reg[REG_CODES] = m_reg[REG_CODEE];
}

void ShaderVm::PopAllContext()
{
	// restore register values
	m_reg[REG_LES] -= sizeof(int)*16;
	char *p = m_localenvstackmem.GetMem(0);
	memcpy(m_reg, p+m_reg[REG_LES], sizeof(int)*16);

	m_mem[REG_LE] = m_stackmem.GetMem(0)+m_reg[REG_LE];
	m_mem[REG_LP] = m_stackmem.GetMem(0)+m_reg[REG_LP];
	m_mem[REG_LS] = m_stackmem.GetMem(0)+m_reg[REG_LS];
	m_mem[REG_CS] = m_stackmem.GetMem(0)+m_reg[REG_CS];
	m_mem[REG_LSTOP] = m_stackmem.GetMem(0)+m_reg[REG_LSTOP];

	m_mem[REG_DATAS] = m_datamem.GetMem(0)+m_reg[REG_DATAS];
	m_mem[REG_CODES] = m_codemem.GetMem(0)+m_reg[REG_CODES];
}

void ShaderVm::SetCoordSys(int i, const matrix4d &m)
{
	if (i<7) {
		m_coordsys[i].mat = m;
		m_coordsys[i].invmat = m;
		m_coordsys[i].invmat.inverse();
	}
	else {
		printf("?<ShaderVm::SetMatrix> No named coordinate system implemented.\n");
	}
}

int ShaderVm::AddLightSource(ShaderInst *obj)
{
	Light *lp = new Light;
	lp->m_shader = obj;
	lp->on = true;
	lp->shadowon = true;
	if (obj->name.compare("ambientlight")==0)
		lp->type = Light::AMBIENT;
	else
		lp->type = Light::NONAMBIENT;

	if (m_framebegin)
		m_lights.push_back(lp);
	else
		m_globallights.push_back(lp);

	return (int)(m_lights.size()-1);
}

// internel specularprdf function, from Renderman Specification p.153
color3d ShaderVm::_specularbrdf(vector3d L, vector3d N, vector3d V, double roughness)
{
	vector3d H = L+V;
	H.normalize();
	// Hey, this is different from Renderman Specification
	double v = pow(max(0.0, N%H), 10/roughness);

	return color3d(v, v, v);
}

int ShaderVm::PreExecShader(ShaderInst *si)
{
	SetEnvironment(si);

	SetCoordSys(6, si->mat);

	// setup parameters
	vector<ShaderParam> &splist = si->params;

	vector<ShaderParam>::iterator it  = splist.begin();
	for (; it<splist.end(); ++it) {
		ShaderParam &sp = *it;
		if (sp.GetType() != VAR_STRING) {
			void *dst = m_mem[REG_LP]+sp.GetOffset();
			void *src = sp.GetValuePtr();
			memcpy(dst, src, sp.GetAllSize());
		}
		else {
			void *dst = m_mem[REG_LP] + sp.GetOffset();
			int addrcode = AddString(sp.Gets().c_str());
			*(int *)dst = addrcode;
		}
	}

	return 0;
}

int ShaderVm::AddString(const char *s)
{
	const int REMAINING[] = {0, 3, 2, 1};
	int l = strlen(s)+1;
	int r = REMAINING[l%4];

	char *mem = m_datamem.GetMem(0)+m_reg[REG_DATAE];

	strcpy(mem, s);

	int tmp = R_DATAE;
	R_DATAE += l+r;

	return DATA_ADDR(tmp);
}

const CoordSys &ShaderVm::GetCoordSys(const char *name) const
{
	int n = m_coordsys.size();
	for (int i=0; i<n; ++i) {
		const CoordSys &cs = m_coordsys[i];
		if (cs.name.compare(name)==0) {
			return cs;
		}
	}

	return m_coordsys[3];
}

#define EVAL_ALL (0xff)

vector3d ShaderVm::Duv(int variable)
{
	// find if the Duv statement has been evaluated or not
	map<int, DerivValues>::iterator it = m_derivu.find(R_NP-R_CODES);
	if ( it!=m_derivu.end() ) {
		return it->second.u_vtr;
	}

	vector3d v(GetMemv(variable));

	m_mode = 1;
	m_codeoffset = R_NP-R_CODES;
	
	GeomObj *go = m_ii.m_node->GetGeomObj();
	ShaderInst *si = m_curshader;

	m_iistack.push(m_ii);
	// TODO not eval all
	go->Evaluv(m_ii.m_ic, m_ii.m_ic.m_u+m_ii.m_du, m_ii.m_ic.m_v, EVAL_ALL);

	CalcAfterEval();

	PushAllContext();

	LoadShader(si);

	PreExecShader(si);
	ExecShader();
	
	vector3d vdu = GetMemv(variable);
	PopAllContext();

	m_ii = m_iistack.top();
	m_iistack.pop();

	DerivValues dv;
	dv.u_vtr = (vdu-v)/m_ii.m_dv;
	m_derivu[m_codeoffset] = dv;


	m_mode = 0;
	m_codeoffset = 0;

	return dv.u_vtr;
}

vector3d ShaderVm::Dvv(int variable)
{
	// find if the Duv statement has been evaluated or not
	map<int, DerivValues>::iterator it = m_derivv.find(R_NP-R_CODES);
	if ( it!=m_derivv.end() ) {
		return it->second.u_vtr;
	}

	vector3d v(GetMemv(variable));

	m_mode = 1;
	m_codeoffset = R_NP-R_CODES;
	
	GeomObj *go = m_ii.m_node->GetGeomObj();
	ShaderInst *si = m_curshader;

	m_iistack.push(m_ii);
	// TODO not eval all
	go->Evaluv(m_ii.m_ic, m_ii.m_ic.m_u, m_ii.m_ic.m_v+m_ii.m_dv, EVAL_ALL);

	CalcAfterEval();

	PushAllContext();

	LoadShader(si);

	PreExecShader(si);
	ExecShader();
	
	vector3d vdu = GetMemv(variable);
	PopAllContext();

	m_ii = m_iistack.top();
	m_iistack.pop();

	DerivValues dv;
	dv.u_vtr = (vdu-v)/m_ii.m_dv;
	m_derivv[m_codeoffset] = dv;

	m_mode = 0;
	m_codeoffset = 0;


	return dv.u_vtr;
}

double ShaderVm::Duf(int variable)
{
	// find if the Duv statement has been evaluated or not
	map<int, DerivValues>::iterator it = m_derivu.find(R_NP-R_CODES);
	if ( it!=m_derivu.end() ) {
		return it->second.u_flt;
	}

	double v(GetMemf(variable));

	m_mode = 1;
	m_codeoffset = R_NP-R_CODES;
	
	GeomObj *go = m_ii.m_node->GetGeomObj();
	ShaderInst *si = m_curshader;

	m_iistack.push(m_ii);
	// TODO not eval all
	go->Evaluv(m_ii.m_ic, m_ii.m_ic.m_u+m_ii.m_du, m_ii.m_ic.m_v, EVAL_ALL);

	CalcAfterEval();

	PushAllContext();

	LoadShader(si);

	PreExecShader(si);
	ExecShader();
	
	double vdu = GetMemf(variable);
	PopAllContext();

	m_ii = m_iistack.top();
	m_iistack.pop();

	DerivValues dv;
	dv.u_flt = (vdu-v)/m_ii.m_dv;
	m_derivu[m_codeoffset] = dv;


	m_mode = 0;
	m_codeoffset = 0;

	return dv.u_flt;
}

double ShaderVm::Dvf(int variable)
{
	// find if the Duv statement has been evaluated or not
	map<int, DerivValues>::iterator it = m_derivv.find(R_NP-R_CODES);
	if ( it!=m_derivv.end() ) {
		return it->second.u_flt;
	}

	double v = GetMemf(variable);

	m_mode = 1;
	m_codeoffset = R_NP-R_CODES;
	
	GeomObj *go = m_ii.m_node->GetGeomObj();
	ShaderInst *si = m_curshader;

	m_iistack.push(m_ii);
	// TODO not eval all
	go->Evaluv(m_ii.m_ic, m_ii.m_ic.m_u, m_ii.m_ic.m_v+m_ii.m_dv, EVAL_ALL);

	CalcAfterEval();

	PushAllContext();

	LoadShader(si);

	PreExecShader(si);
	ExecShader();
	
	double vdu = GetMemf(variable);
	PopAllContext();

	m_ii = m_iistack.top();
	m_iistack.pop();

	DerivValues dv;
	dv.u_flt = (vdu-v)/m_ii.m_dv;
	m_derivv[m_codeoffset] = dv;

	m_mode = 0;
	m_codeoffset = 0;


	return dv.u_flt;
}

void ShaderVm::CalcAfterEval()
{
	IntersectCache *ic = &m_ii.m_ic;
	const matrix4d &m = m_ii.m_node->GetMatrix();
	// transform intp and normal to "camera" coordinate system
	ic->m_intpt *= m;
	m_ii.m_I = ic->m_intpt-m_ii.m_ray.get_orig();
	matrix4d m2 = m;
	m2.inverse();
	m2.transpose();
	ic->m_normal *= m2;
	ic->m_normal.normalize();
	SetCoordSys(5, m);

	// TODO: should be scaling by u, v
	m_ii.m_s = ic->m_u;
	m_ii.m_t = ic->m_v;

	// calculate du, dv
	double d0 = (m_ii.m_I%ic->m_dPdu)/(m_ii.m_I.length()*ic->m_dPdu.length());
	double d1 = (m_ii.m_I%ic->m_dPdv)/(m_ii.m_I.length()*ic->m_dPdv.length());
	double ku = 1/sqrt(1-d0*d0);
	double kv = 1/sqrt(1-d1*d1);
	double ds;
	if (m_options.cam_projection==0) {
		ds = (m_options.screen_top-m_options.screen_bot)/m_options.format_yres;
	}
	else {
		ds = 0.5*sin(m_options.cam_fov/2)/m_options.format_yres;
	}
	double dest = ds*m_ii.m_ic.m_intpt.z();

	m_ii.m_du = dest*ku/ic->m_dPdu.length();
	m_ii.m_dv = dest*kv/ic->m_dPdv.length();

}

void ShaderVm::SetEnvironment(ShaderInst *si)
{
	switch (si->file->GetType()) {
	case SHADER_DISPLACEMENT:
		SetMemp(REG_LE, DLE_P, m_ii.m_ic.m_intpt);
		SetMemv(REG_LE, DLE_dPdu, m_ii.m_ic.m_dPdu);
		SetMemv(REG_LE, DLE_dPdv, m_ii.m_ic.m_dPdv);
		SetMemv(REG_LE, DLE_N, m_ii.m_ic.m_normal);
		SetMemv(REG_LE, DLE_Ng, m_ii.m_ic.m_normal);
		SetMemv(REG_LE, DLE_I, m_ii.m_I);
		SetMemp(REG_LE, DLE_E, point3d(0, 0, 0));
		SetMemf(REG_LE, DLE_u,  m_ii.m_ic.m_u);
		SetMemf(REG_LE, DLE_v, m_ii.m_ic.m_v);
		SetMemf(REG_LE, DLE_du, m_ii.m_du);
		SetMemf(REG_LE, DLE_dv, m_ii.m_dv);
		SetMemf(REG_LE, DLE_s, m_ii.m_s);
		SetMemf(REG_LE, DLE_t, m_ii.m_t);
		break;
	case SHADER_SURFACE: 
		{
		IntersectCache *ic = &m_ii.m_ic;
		RAttribute *ra = m_ii.m_node->GetAttribute();
		ShaderInst *si = ra->surface;

		// load surface shader environment
		SetMemc(REG_LE, SLE_Cs, ra->color);
		SetMemc(REG_LE, SLE_Os, ra->opacity);
		SetMemp(REG_LE, SLE_P, ic->m_intpt);
		SetMemv(REG_LE, SLE_dPdu, ic->m_dPdu);
		SetMemv(REG_LE, SLE_dPdv, ic->m_dPdv);
		SetMemv(REG_LE, SLE_N, ic->m_normal);
		SetMemv(REG_LE, SLE_Ng, ic->m_normal);
		SetMemf(REG_LE, SLE_u, ic->m_u);
		SetMemf(REG_LE, SLE_v, ic->m_v);
		SetMemf(REG_LE, SLE_du, m_ii.m_du);
		SetMemf(REG_LE, SLE_dv, m_ii.m_dv);
		SetMemf(REG_LE, SLE_s, m_ii.m_s);
		SetMemf(REG_LE, SLE_t, m_ii.m_t);
		// L, Cl, Ol is available only inside illuminance statements
		SetMemp(REG_LE, SLE_E, point3d(0, 0, 0));
		SetMemv(REG_LE, SLE_I, m_ii.m_I);
		// TODO: ncomps, time, dtime, dPdtime
		SetMemc(REG_LE, SLE_Ci, color3d::white);
		SetMemc(REG_LE, SLE_Oi, color3d::white);
		}
		break;
	case SHADER_LIGHT:
		// TODO
		break;
	}
}
