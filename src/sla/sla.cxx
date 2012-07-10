#include "sla.hxx"
#include "sli.hxx"
#include "sla_tab.hxx"
#include "ShaderFile.hxx"
#include <stdarg.h>

extern int num_lines;

const char ShaderFile::MAGIC[4] = { 'R', 'S', 'L', 'O' };
const char ShaderFile::S_SHDR[4] = { 'S', 'H', 'D', 'R' };
const char ShaderFile::S_PRDS[4] = { 'P', 'R', 'D', 'S' };
const char ShaderFile::S_PRDE[4] = { 'P', 'R', 'D', 'E' };
const char ShaderFile::S_DATA[4] = { 'D', 'A', 'T', 'A' };
const char ShaderFile::S_CODE[4] = { 'C', 'O', 'D', 'E' };

Sla::Sla()
{
	m_type = -1;
	m_name = "";

	m_parammem=NULL;
	m_parammemsize=0;
	m_paramsize=0;

	m_datamemsize = 0;
	m_datasize = 0;
	m_codememsize = 0;
	m_codesize = 0;

	Symbol sb;
	int i=0;
	while (1) {
		if (strcmp(str_cfuncs[i], "null")==0)
			break;
		sb.name = str_cfuncs[i];
		sb.space = Symbol::CFUNC;
		sb.value = i;
		m_symbols.push_back(sb);
		++i;
	}

	int n = 16;
	for (i=0; i<n; ++i) {
		sb.name = str_registers[i];
		sb.space = Symbol::REGISTER;
		sb.value = i;
		m_symbols.push_back(sb);
	}

	sb.name = "__null";
	m_unresolved.push_back(sb);

	m_error = m_warning = 0;
}

Sla::~Sla()
{
}

short Sla::StrToMem(char *mem, const char *str)
{
	const int REMAINING[4] = { 0, 3, 2, 1};
	int l = strlen(str)+1;
	strcpy(mem, str);
	int remaining = REMAINING[l%4];

	if (remaining) {
		int zero = 0;
		memcpy(mem+l, &zero, remaining);
	}

	return short(l+remaining);
}

void Sla::Save()
{

	Pass2();

	printf("%d warnings, %d errors\n", m_warning, m_error);
	if (m_error>0)
		return;

	string s(m_name);
	s.append(".slo");
	FILE *fp = fopen(s.c_str(), "wb");
	if (fp==NULL)
		return;

	fwrite(ShaderFile::MAGIC, 4, 1, fp);


	char strmem[256];

	// SHDR
	ShaderFileSectionDesc shdr;
	memcpy(shdr.name, ShaderFile::S_SHDR, 4);
	shdr.offset = 4;
	shdr.length = 0;
	fwrite(shdr.name, 4, 1, fp);
	fwrite(&shdr.length, 1, sizeof(int), fp);
	fwrite(&shdr.offset, 1, sizeof(int), fp);
	short lm = StrToMem(strmem, m_name.c_str());
	fwrite(&lm, 1, 2, fp);
	fwrite(strmem, lm, 1, fp);
	shdr.length += 2+lm;
	fwrite(&m_type, 1, sizeof(int), fp);
	shdr.length += 4;
	int evalmask = EVAL_U | EVAL_V;
	fwrite(&evalmask, 1, sizeof(int), fp);
	shdr.length += 4;

	// PRDS
	ShaderFileSectionDesc prds;
	memcpy(prds.name, ShaderFile::S_PRDS, 4);
	prds.offset = 0;
	prds.length = 0;
	fwrite(prds.name, 4, 1, fp);
	fwrite(&prds.length, 1, sizeof(int), fp);
	fwrite(&prds.offset, 1, sizeof(int), fp);
	int sz = m_paramdesc.size();
	fwrite(&sz, 1, sizeof(int), fp);
	prds.length += 4;
	for (int i=0; i<sz; ++i) {
		ShaderParamDesc &spd = m_paramdesc[i];
		short lm = StrToMem(strmem, spd.name.c_str());
		fwrite(&lm, 1, 2, fp);
		fwrite(strmem, lm, 1, fp);
		prds.length+=2+lm;
		fwrite(&spd.type, 1, sizeof(variable_t), fp);
		prds.length+=4;
	}

	// PRDE
	ShaderFileSectionDesc prde;
	memcpy(prde.name, ShaderFile::S_PRDE, 4);
	prde.offset = 0;
	prde.length = m_paramsize;
	fwrite(prde.name, 4, 1, fp);
	fwrite(&prde.length, 1, sizeof(int), fp);
	fwrite(&prde.offset, 1, sizeof(int), fp);
	fwrite(m_parammem, prde.length, 1, fp);
	
	// DATA
	ShaderFileSectionDesc data;
	memcpy(data.name, ShaderFile::S_DATA, 4);
	data.offset = 0;
	data.length = m_datasize;
	fwrite(data.name, 4, 1, fp);
	fwrite(&data.length, 1, sizeof(int), fp);
	fwrite(&data.offset, 1, sizeof(int), fp);
	fwrite(m_datamem, data.length, 1, fp);

	// CODE
	ShaderFileSectionDesc code;
	memcpy(code.name, ShaderFile::S_CODE, 4);
	code.offset = 0;
	code.length = m_codesize*4;
	fwrite(code.name, 4, 1, fp);
	fwrite(&code.length, 1, sizeof(int), fp);
	fwrite(&code.offset, 1, sizeof(int), fp);
	fwrite(m_codemem, code.length, 1, fp);

	fseek(fp, shdr.offset+4, SEEK_SET);
	fwrite(&shdr.length, 1, sizeof(int), fp);
	fwrite(&shdr.offset, 1, sizeof(int), fp);

	prds.offset = shdr.offset+shdr.length+12;
	fseek(fp, prds.offset+4, SEEK_SET);
	fwrite(&prds.length, 1, sizeof(int), fp);
	fwrite(&prds.offset, 1, sizeof(int), fp);

	prde.offset = prds.offset+prds.length+12;
	fseek(fp, prde.offset+4, SEEK_SET);
	fwrite(&prde.length, 1, sizeof(int), fp);
	fwrite(&prde.offset, 1, sizeof(int), fp);

	data.offset = prde.offset+prde.length+12;
	fseek(fp, data.offset+4, SEEK_SET);
	fwrite(&data.length, 1, sizeof(int), fp);
	fwrite(&data.offset, 1, sizeof(int), fp);

	code.offset = data.offset+data.length+12;
	fseek(fp, code.offset+4, SEEK_SET);
	fwrite(&code.length, 1, sizeof(int), fp);
	fwrite(&code.offset, 1, sizeof(int), fp);

	fclose(fp);

	printf("%s is successfully compiled.\n", m_name.c_str());
	printf("parameter size = %d\n", m_paramsize);
	printf("data size = %d\n", m_datasize);
	printf("code size = %d\n", m_codesize*4);
}

bool Sla::TypeCheckValid(int type, int datatype)
{
	if ( ((type&T_FLOAT) && datatype==datav::TP_DOUBLE) ||
		( ((type&T_POINT)||(type&T_VECTOR)||(type&T_NORMAL)||(type&T_COLOR))&&
		datatype==datav::TP_TRIPLE ) ||
		( (type&T_STRING) && datatype==datav::TP_STRING) )
		return true;
	else
		return false;
}

void Sla::WriteData(datav &dv, unsigned char *mem)
{
	if (dv.type==datav::TP_STRING) {
		int n = (int)dv.dval;
		memcpy(mem, &n, sizeof(int));
	}
	else if (dv.type==datav::TP_DOUBLE) {
		memcpy(mem, &dv.dval, sizeof(dv.dval)); 
	}	
	else if (dv.type==datav::TP_TRIPLE) {
		memcpy(mem, dv.tri, sizeof(dv.tri));
	}
}

int Sla::GetShaderFileType(int type)
{
	switch (type) {
	case T_LIGHT:
		return SHADER_LIGHT;
	case T_SURFACE:
		return SHADER_SURFACE;
	case T_DISPLACEMENT:
		return SHADER_DISPLACEMENT;
	case T_VOLUME:
		return SHADER_VOLUME;
	case T_IMAGER:
		return SHADER_IMAGER;		
	}
}

void Sla::AddSymbol(const char *name, int pt, int type, int val)
{
	Symbol sb;
	sb.space = pt;
	sb.type = GetVariableType(type);
	sb.name = name;
	sb.value = val;
	m_symbols.push_back(sb);
}

int Sla::AllocateString(const char *s)
{
	int l = strlen(s);
	l = (l+1+3)/4*4;
	unsigned char *mem = GetDataMem(l);

	int offset = m_datasize;
	m_datasize += l;
	strcpy((char *)(mem+offset), s);	
	
	return DATA_ADDR(offset);
}
const char *STR_TYPE [] = {
	"float", "point", "vector", "normal", "color", "matrix", "string"
};
const char *STR_DATAV_TYPE[] = {
	"float", "triple", "string"
};

int Sla::GetDataSize(datav &dv)
{
	switch (dv.type) {
	case datav::TP_DOUBLE:
		return 8;
	case datav::TP_TRIPLE:
		return 24;
	case datav::TP_STRING:
		return 4;
	}
}

variable_t Sla::GetVariableType(int type)
{
	switch(type) {
	case T_FLOAT:
		return VAR_FLOAT;
	case T_POINT:
		return VAR_POINT;
	case T_VECTOR:
		return VAR_VECTOR;
	case T_NORMAL:
		return VAR_NORMAL;
	case T_COLOR:
		return VAR_COLOR;
	case T_MATRIX:
		return VAR_MATRIX;
	case T_STRING:
		return VAR_STRING;
	}
}

void Sla::AddParam(int type, const char *name, datav &dat)
{
	if ( TypeCheckValid(type, dat.type) ) {
		AddSymbol(name, Symbol::PARAM, type, LP_ADDR(m_paramsize));	
		int size = GetDataSize(dat);
		unsigned char *mem = GetParamMem(size);
		if (dat.type==datav::TP_STRING) {
			dat.dval = AllocateString(dat.str);	
		}
		WriteData(dat, mem+m_paramsize);
		m_paramsize += size;

		ShaderParamDesc spd;
		spd.name = name;
		spd.type = GetVariableType(type);
		spd.offset = 0;
		m_paramdesc.push_back(spd);
	}
	else {
		printf("! %d: symbol type is %s, while data type is%s\n", 
			num_lines, STR_TYPE[type-T_FLOAT], STR_DATAV_TYPE[dat.type]);
	}

}

void Sla::AddParamArray(int type, int num, const char *name, 
	vector<datav> *datarr)
{
	int n = datarr->size();
	if (n==0) {
		printf("! %d: Empty array constant is not allowed\n", num_lines);
		return ;
	}

	if (!TypeCheckValid(type, (*datarr)[0].type)) {
		printf("! %d: symbol type is %s, while data type is %s\n",
			num_lines, STR_TYPE[type-T_FLOAT], 
			STR_DATAV_TYPE[(*datarr)[0].type]);	
	}

	AddSymbol(name, Symbol::PARAM, type, LP_ADDR(m_paramsize));
	int totalsize = 0;
	for (int i=0; i<n; ++i) {
		datav &dv = (*datarr)[i];
		int size = GetDataSize(dv);
		unsigned char *mem = GetParamMem(size);
		if (dv.type==datav::TP_STRING) {
			dv.dval = AllocateString(dv.str);	
		}
		WriteData(dv, mem+m_paramsize);	
		m_paramsize+=size;	
		totalsize+=size;
	}
	ShaderParamDesc spd;
	spd.name = name;
	spd.type = GetVariableType(type);
	spd.offset = 0;
	m_paramdesc.push_back(spd);
}
	
void Sla::AddData(int type, const char *name, datav &dat)
{
	if ( TypeCheckValid(type, dat.type) ) {
		int datasize = m_datasize;
		AddSymbol(name, Symbol::COMMON, type, DATA_ADDR(m_datasize));	
		int size = GetDataSize(dat);
		m_datasize += size;
		unsigned char *mem = GetDataMem(size);
		if (dat.type==datav::TP_STRING) {
			dat.dval = AllocateString(dat.str);	
		}
		WriteData(dat, mem+datasize);
		++m_datanum;
	}
	else {
		printf("! %d: symbol type is %s, while data type is%s\n", 
			num_lines, STR_TYPE[type-T_FLOAT], STR_DATAV_TYPE[dat.type]);
	}

}

void Sla::AddDataArray(int type, int num, const char *name, 
	vector<datav> *datarr)
{
	int n = datarr->size();
	if (n==0) {
		printf("! %d: Empty array constant is not allowed\n", num_lines);
		return ;
	}

	if (!TypeCheckValid(type, (*datarr)[0].type)) {
		printf("! %d: symbol type is %s, while data type is %s\n",
			num_lines, STR_TYPE[type-T_FLOAT], 
			STR_DATAV_TYPE[(*datarr)[0].type]);	
	}

	AddSymbol(name, Symbol::COMMON, type, m_datasize);
	int totalsize = 0;
	for (int i=0; i<n; ++i) {
		datav &dv = (*datarr)[i];
		int size = GetDataSize(dv);
		unsigned char *mem = GetDataMem(size);
		if (dv.type==datav::TP_STRING) {
			dv.dval = AllocateString(dv.str);	
		}
		WriteData(dv, mem+m_datasize);	
		m_datasize+=size;	
		totalsize+=size;
	}
}
	
void Sla::Op0(int op)
{
	int *mem = (int *)GetCodeMem(4);
	mem[m_codesize] = op;	
	m_codesize++;
}

void Sla::Op1(int op, int opr0)
{
	int *mem = (int *)GetCodeMem(8);
	mem[m_codesize++] = op;
	mem[m_codesize++] = opr0;
}

void Sla::Op2(int op, int opr0, int opr1)
{
	int *mem = (int *)GetCodeMem(12);
	mem[m_codesize++] = op;
	mem[m_codesize++] = opr0;
	mem[m_codesize++] = opr1;
}

void Sla::Op3(int op, int opr0, int opr1, int opr2)
{
	int *mem = (int *)GetCodeMem(16);
	mem[m_codesize++] = op;
	mem[m_codesize++] = opr0;
	mem[m_codesize++] = opr1;
	mem[m_codesize++] = opr2;
}

int Sla::ResolveSymbol(const char *name)
{
	int n = m_symbols.size();
	for (int i=0; i<n; ++i) {
		Symbol &s = m_symbols[i];

		if (s.name.compare(name)==0) {
			return s.value;
		}
	}

	return AddUnresolvedSymbol(name);
}

int Sla::AddUnresolvedSymbol(const char *name)
{
	int n = m_unresolved.size();
	for (int i=0; i<n; ++i) {
		Symbol &sb = m_unresolved[i];
		if (sb.name.compare(name)==0)
			return -i;
	}

	Symbol sb;
	sb.name = name;	
	sb.value = -m_unresolved.size();
	sb.reported = 0;
	m_unresolved.push_back(sb);	

	return sb.value;
}

void Sla::Resolve(int &s)
{
	if (IsUnresolved(s)) { 
		int reported; 
		const char *name = GetUnresolvedSymbolName(s, &reported); 
		(s) = ResolveSymbol(name); 
		if ( (s) < 0 && m_unresolved[-s].reported==0) { 
			g_a.Error("Unresolved symbol %s\n", name); 
			m_unresolved[-s].reported = 1; 
			return ; 
		}
	}
}

void Sla::Pass2()
{

	int *membase = (int *)GetCodeMem(0);
	int *mem = membase;

	bool end =false;
	while (!end) {
		switch (*mem) {
		case NOP:
		case END:
		case NEG:
			mem++;	
			end = true;
			break;
		case NEGF:
		case NEGV:
		case PUSHF:
		case PUSHV:
		case PUSHS:
		case POPF:
		case POPV:
		case POPS:
		case CCALL:
			Resolve(*(mem+1));
			mem += 2;
			break;
		case JMP:
		case JT:
		case JF:
			Resolve(*(mem+1));
			*(mem+1) -= mem-membase;
			*(mem+1) *= 4;

			mem+=2;
			break;
		
		case MOVF:
		case MOVV:
		case MOVS:
		case ADDR:
		case SUBR:
		case GTF:
		case GEF:
		case LTF:
		case LEF:
		case EQF:
		case NEF:
		case EQV:
		case NEV:
		case EQS:
		case NES:
			Resolve(*(mem+1));	
			Resolve(*(mem+2));

			mem+=3;
			break;
		case ADDF:
		case ADDV:
		case SUBF:
		case SUBV:
		case MULF:
		case MULV:
		case MULVV:
		case DIVF:
		case DIVV:
		case DOTV:
		case CRSV:
			Resolve(*(mem+1));
			Resolve(*(mem+2));
			Resolve(*(mem+3));

			mem+=4;
			break;
		}
	}
}

unsigned char *Sla::GetParamMem(unsigned int size)
{
	if (m_paramsize+size>=m_parammemsize) {
		if (m_parammemsize==0) {
			m_parammemsize = 16;
			m_parammem = (unsigned char *)malloc(m_parammemsize);
		}
		else {
			m_parammemsize *= 2;	
			m_parammem = (unsigned char *)realloc(m_parammem, m_parammemsize);
		}
	}

	return m_parammem;
}

unsigned char *Sla::GetDataMem(unsigned int size)
{
	if (m_datasize+size>=m_datamemsize) {
		if (m_datamemsize==0) {
			m_datamemsize = 16;
			m_datamem = (unsigned char *)malloc(m_datamemsize);
		}
		else {
			m_datamemsize *=2;
			m_datamem = (unsigned char *)realloc(m_datamem, m_datamemsize);
		}
	}

	return m_datamem;
}

int *Sla::GetCodeMem(unsigned int size)
{
	if (m_codesize+size>=m_codememsize) {
		if (m_codememsize==0) {
			m_codememsize = 16;
			m_codemem = (int *)malloc(sizeof(int)*m_codememsize);
		}
		else {
			m_codememsize *=2;
			m_codemem = (int *)realloc(m_codemem, sizeof(int)*m_codememsize);
		}
	}

	return m_codemem;
}


void Sla::Shader(int type, const char *name)
{
	m_type = GetShaderFileType(type);
	m_name = string(name);

	Symbol sb;
	int i;
	int n;
	const EnvDesc *ed;
	switch (m_type) {
	case SHADER_SURFACE:
		ed = desc_surfaceenv;
		n = sizeof(desc_surfaceenv)/sizeof(EnvDesc);
		break;
	case SHADER_LIGHT:
		ed = desc_lightenv;
		n = sizeof(desc_lightenv)/sizeof(EnvDesc);
		break;
	case SHADER_DISPLACEMENT:
		ed = desc_lightenv;
		n = sizeof(desc_displacementenv)/sizeof(EnvDesc);
		break;
	case SHADER_VOLUME:
		ed = desc_volumeenv;
		n = sizeof(desc_volumeenv)/sizeof(EnvDesc);
		break;
	case SHADER_IMAGER:
		ed = desc_volumeenv;
		n = sizeof(desc_imagerenv)/sizeof(EnvDesc);
		break;
	}

	int offset=0;
	for (i=0; i<n; ++i) {
		sb.name = ed[i].name;
		sb.space = Symbol::LOCALENV;
		sb.value = LE_ADDR(offset);
		offset += variable_dword(variable_t(ed[i].val))*4;

		m_symbols.push_back(sb);
	}

}

const char *Sla::GetUnresolvedSymbolName(int i, int *reported)
{
	Symbol &sb = m_unresolved[-i];
	*reported = sb.reported;
	return sb.name.c_str();
}

void Sla::Warning(const char *s, ...)
{
	printf("?>");
	va_list arg;
	va_start(arg, s);

	vprintf(s, arg);

	va_end(arg);

	m_warning++;
}

void Sla::Error(const char *s, ...)
{
	printf("!>");
	va_list arg;
	va_start(arg, s);

	vprintf(s, arg);

	va_end(arg);

	m_error++;
}

void Sla::AddLabel(const char *name)
{
	AddSymbol(name, Symbol::COMMON, 0, m_codesize);
}

Sla g_a;

