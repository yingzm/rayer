#ifndef __SLI_HXX__
#define __SLI_HXX__

typedef enum {
	SHADER_SURFACE,
	SHADER_LIGHT,
	SHADER_DISPLACEMENT,
	SHADER_VOLUME,
	SHADER_IMAGER
} shader_t;

inline const char *GetShaderTypeStr(shader_t st)
{
	extern const char *str_shadertype[];

	return str_shadertype[(int)st];
}


typedef enum {
	VAR_FLOAT,
	VAR_POINT,
	VAR_VECTOR,
	VAR_NORMAL,
	VAR_COLOR,
	VAR_MATRIX,
	VAR_STRING,
	
	VAR_ARRAY = 0x10,

	VAR_UNIFORM = 0x20,
	VAR_VARYING = 0x40,

	VAR_OUTPUT = 0x80,
	VAR_EXTERN = 0x100
} variable_t;

inline const char *GetVariableTypeStr(variable_t st)
{
	extern const char *str_variabletype[];

	return str_variabletype[((int)st&0xf)];
}


extern const unsigned int size_variable[];
inline unsigned int variable_elemdword(variable_t t)
{
	return size_variable[int(t&0xf)];
}

inline unsigned int variable_elemnum(variable_t t)
{
	return (int(t)>>16)&0xffff;
}

inline unsigned int variable_dword(variable_t t)
{
	if (t&VAR_ARRAY)
		return size_variable[int(t&0xf)]*variable_elemnum(t);
	else
		return size_variable[int(t&0xf)];
}

// shader vm instructions
enum {
	NA =	0,	// not-valid
	NOP,	// empty instruction
	END,	// end of shader

// data transfer
	MOVF,	// movf %dst, %src;
	MOVV,	// movv %dst, %src;
	MOVS,

// mathematical operations
	ADDR,
	ADDF,
	ADDV,

	SUBR,
	SUBF,
	SUBV,

	MULF,
	MULV,	// mulv %dst, %v3, %d0
	MULVV,	// mulv %dst, %v0, %v1

	DIVF,
	DIVV,

	DOTV,
	CRSV,

	NEGF,
	NEGV,

// branch
	JMP,
	JT,	// jump if true
	JF,	// jump if false

// compare
	GTF,
	GEF,
	LTF,
	LEF,
	EQF,
	EQV,
	EQS,
	NEF,
	NEV,
	NES,
	NEG,	// negate the compare flag

	PUSHF,
	PUSHV,
	PUSHS,
	POPF,
	POPV,
	POPS,
	CCALL
};

extern const char *str_registers[];

#define LE_ADDR(m)	((REG_LE<<24)+m)
#define LP_ADDR(m)	((REG_LP<<24)+m)
#define LS_ADDR(m)	((REG_LS<<24)+m)
#define CODE_ADDR(m)	((REG_CODES<<24)+m)
#define DATA_ADDR(m)	((REG_DATAS<<24)+m)

enum {
	REG_FLG,	// flags, 0=compare result
	REG_NP,		// next program
	REG_LE,		// local environment start
	REG_LP,		// local parameter start
	REG_LS,		// local stack start
	REG_CS,		// call stack start
	REG_LSTOP,	// local stack top
	REG_CODES,	// code start
	REG_CODEE,	// code end
	REG_DATAS,	// data start
	REG_DATAE,	// data end
	REG_LES,	// local environment stack
};

// flag bits
enum {
	FLAG_CMP=1
};

// surface shader local environment
enum {
	SLE_Cs = 0*4,
	SLE_Os = 6*4,
	SLE_P = 12*4,
	SLE_dPdu = 18*4,
	SLE_dPdv = 24*4,
	SLE_N = 30*4,
	SLE_Ng = 36*4,
	SLE_u = 42*4,
	SLE_v = 44*4,
	SLE_du = 46*4,
	SLE_dv = 48*4,
	SLE_s = 50*4,
	SLE_t = 52*4,
	SLE_L = 54*4,
	SLE_CI = 60*4,
	SLE_OI = 66*4,
	SLE_E = 72*4,
	SLE_I = 78*4,
	SLE_ncomps = 84*4,
	SLE_time = 86*4,
	SLE_dtime = 88*4,
	SLE_dPdtime = 90*4,
	SLE_Ci = 96*4,
	SLE_Oi = 102*4,
	SLE_SIZE = 108*4
};

// light shader environment
enum {
	LLE_P = 0*4,		// light eval
	LLE_dPdu = 6*4,
	LLE_dPdv = 12*4,
	LLE_N = 18*4,
	LLE_Ng = 24*4,
	LLE_u = 30*4,
	LLE_v = 32*4,
	LLE_du = 34*4,
	LLE_dv = 36*4,
	LLE_s = 38*4,
	LLE_t = 40*4,
	LLE_L = 42*4,
	LLE_Ps = 48*4,
	LLE_E = 54*4,
	LLE_ncomps = 60*4,
	LLE_time = 62*4,
	LLE_dtime = 64*4,
	LLE_Cl = 66*4,
	LLE_Ol = 72*4,
	LLE_SIZE = 78*4
};

enum {
	DLE_P = 0*4,
	DLE_dPdu = 6*4,
	DLE_dPdv  = 12*4,
	DLE_N = 18*4,
	DLE_Ng = 24*4,
	DLE_I = 30*4,
	DLE_E = 36*4,
	DLE_u = 42*4,
	DLE_v = 44*4,
	DLE_du = 46*4,
	DLE_dv = 48*4,
	DLE_s = 50*4,
	DLE_t = 52*4,
	DLE_ncomps = 54*4,
	DLE_time = 56*4,
	DLE_dtime = 58*4,
	DLE_dPdtime = 60*4,
	DLE_SIZE = 66*4
};

enum {
	VLE_P = 0*4,
	VLE_I = 6*4,
	VLE_E = 12*4,
	VLE_Ci = 18*4,
	VLE_Oi = 24*4,
	VLE_ncomps = 30*4,
	VLE_time = 32*4,
	VLE_dtime = 34*4,
	VLE_SIZE = 48*4 
};

enum {
	ILE_P = 0*4,
	ILE_Ci = 6*4,
	ILE_Oi = 12*4,
	ILE_alpha = 18*4,
	ILE_ncomps = 20*4,
	ILE_time = 22*4,
	ILE_dtime = 24*4,
	ILE_SIZE = 26*4
};

extern const char *str_cfuncs[];

class EnvDesc
{
public:
	const char *name;
	int val;
};

extern const EnvDesc desc_surfaceenv[24];
extern const EnvDesc desc_lightenv[19];
extern const EnvDesc desc_volumeenv[10];
extern const EnvDesc desc_displacementenv[17];
extern const EnvDesc desc_imagerenv[7];

#endif /* __SLI_HXX__ */
