#ifndef __SHADERFILE_HXX__
#define __SHADERFILE_HXX__

#include "sli.hxx"

class ShaderParamDesc
{
public:
	string name;
	variable_t type;
	unsigned int offset;
};

class ShaderFileSectionDesc
{
public:
	char name[4];
	unsigned int length;
	unsigned int offset;
};

typedef enum {
	EVAL_P = 1,
	EVAL_U = 2,
	EVAL_V = 4,
	EVAL_DPDU = 8,
	EVAL_DPDV = 0x10,
	EVAL_NORMAL = 0x20,
	EVAL_DU = 0x30,
	EVAL_DV = 0x40,
} evalmask_t;


class ShaderFile
{
public:

	ShaderFile();
	~ShaderFile();
	
	int Load(const string &name);

	shader_t GetType() const;
	const string &GetName() const;
	evalmask_t GetEvalMask() const;
	
	
	bool GetParamDesc(const string &name, int &offset, int &type) const;
	
	unsigned int GetParamDefaultSize() const;
	unsigned int GetDataSize() const;
	unsigned int GetCodeSize() const;
	
	const char *GetParamDefault() const;
	const char *GetData() const;
	const char *GetCode() const;
	
	static const char MAGIC[4];	// magic number
	static const char S_SHDR[4];	// header
	static const char S_PRDS[4];	// param description
	static const char S_PRDE[4];	// param default
	static const char S_DATA[4];	// data
	static const char S_CODE[4];	// code

private:
	shader_t m_shadertype;
	string m_name;
	
	evalmask_t m_evalmask;
	
	vector<ShaderParamDesc> m_paramdesc;

	int m_lenparamdefault;
	char *m_paramdefault;
	int m_lendata;
	char *m_data;
	int m_lencode;
	char *m_code;
};

inline shader_t ShaderFile::GetType() const
{
	return m_shadertype;
}

inline const string &ShaderFile::GetName() const
{
	return m_name;
}

inline evalmask_t ShaderFile::GetEvalMask() const
{
	return m_evalmask;
}
	
inline unsigned int ShaderFile::GetParamDefaultSize() const
{
	return m_lenparamdefault;
}

inline unsigned int ShaderFile::GetDataSize() const
{
	return m_lendata;
}

inline unsigned int ShaderFile::GetCodeSize() const
{
	return m_lencode;
}

inline const char *ShaderFile::GetParamDefault() const
{
	return m_paramdefault;
}

inline const char *ShaderFile::GetData() const
{
	return m_data;
}

inline const char *ShaderFile::GetCode() const
{
	return m_code;
}


#endif /* __SHADERFILE_HXX__ */