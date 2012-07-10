#ifndef __SLA_HXX__
#define __SLA_HXX__

#include <vector>
#include <string>
using namespace std;

#include "sli.hxx"

#include "ShaderFile.hxx"

class datav
{
public:
	enum {
		TP_DOUBLE,
		TP_TRIPLE,
		TP_STRING
	};
	int type;
	double dval;
	double tri[3];
	char str[256];
};

class Symbol
{
public:
	enum {
		REGISTER,
		CFUNC,
		LOCALENV,
		PARAM,
		COMMON
	};
	int space;
	variable_t type;
	string name;
	int value;
	int reported;	// one unresolved symbol will report once
};

class Sla
{
public:
	Sla();
	~Sla();
	void Save();
	void Shader(int type, const char *name);

	void AddParam(int type, const char *name, datav &dat);
	void AddParamArray(int type, int num, const char *name, vector<datav> *datarr);
	
	void AddData(int type, const char *name, datav &dat);
	void AddDataArray(int type, int num, const char *name, vector<datav> *datarr);
	
	void Op0(int op);
	void Op1(int op, int opr0);
	void Op2(int op, int opr0, int opr1);
	void Op3(int op, int opr0, int opr1, int opr2);
	void AddLabel(const char *name);

	int ResolveSymbol(const char *name);

	void Pass2();

	void Warning(const char *s, ...);
	void Error(const char *s, ...);

private:
	int m_type;
	string m_name;

	unsigned char *m_parammem;
	int m_parammemsize;
	int m_paramsize;

	// used to write param
	vector<ShaderParamDesc> m_paramdesc;

	unsigned char *m_datamem;
	int m_datamemsize;
	int m_datasize;
	int m_datanum;

	int *m_codemem;
	int m_codememsize;
	int m_codesize;

	vector<Symbol> m_symbols;
	vector<Symbol> m_unresolved;

	int m_error, m_warning;

	bool TypeCheckValid(int type, int datatype);
public:
	void AddSymbol(const char *name, int space, int type, int value);
private:
	unsigned char *GetParamMem(unsigned int size);
	unsigned char *GetDataMem(unsigned int size);
	int *GetCodeMem(unsigned int size);
	void WriteData(datav &dv, unsigned char *mem);
	int GetDataSize(datav &dv);

	int GetShaderFileType(int type);
	variable_t GetVariableType(int type);

	int AllocateString(const char *s);

	int AddUnresolvedSymbol(const char *s);
	bool IsUnresolved(int i) { if (i<0) return true; else return false; }
	const char *GetUnresolvedSymbolName(int i, int *reported);
	short StrToMem(char *mem, const char *str);

	void Resolve(int &s);
};

extern Sla g_a;

#endif 
