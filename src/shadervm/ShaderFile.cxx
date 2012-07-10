#include "common.hxx"
#include "ShaderFile.hxx"


const char ShaderFile::MAGIC[4] = { 'R', 'S', 'L', 'O' };
const char ShaderFile::S_SHDR[4] = { 'S', 'H', 'D', 'R' };
const char ShaderFile::S_PRDS[4] = { 'P', 'R', 'D', 'S' };
const char ShaderFile::S_PRDE[4] = { 'P', 'R', 'D', 'E' };
const char ShaderFile::S_DATA[4] = { 'D', 'A', 'T', 'A' };
const char ShaderFile::S_CODE[4] = { 'C', 'O', 'D', 'E' };

ShaderFile::ShaderFile()
{
	m_lenparamdefault = m_lendata = m_lencode = 0;

	m_paramdefault = NULL;
	m_data = NULL;
	m_code = NULL;
}

ShaderFile::~ShaderFile()
{
	delete [] m_paramdefault;
	delete [] m_data;
	delete [] m_code;
}

int ShaderFile::Load(const string &name)
{
	FILE *fp = fopen(name.c_str(), "rb");
	if (fp==NULL) {
		return -1;
	}

	char magic[4];
	fread(magic, 1, 4, fp);
	if (memcmp(magic, ShaderFile::MAGIC, 4)!=0) {
		return -1;
	}

	ShaderFileSectionDesc spd;

	size_t rt;
	int val;
	short len;
	char mem[256];
	while (1) {
		rt = fread(&spd.name, 4, 1, fp);
		if (rt==0)
			break;
		fread(&spd.length, 1, sizeof(int), fp);
		fread(&spd.offset, 1, sizeof(int), fp);

		if (memcmp(spd.name, ShaderFile::S_SHDR, 4)==0) {
			fread(&len, 1, 2, fp);
			fread(mem, len, 1, fp);
			m_name = string(mem);
			fread(&val, 1, 4, fp);
			m_shadertype = (shader_t)val;
			fread(&val, 1, 4, fp);
			m_evalmask = (evalmask_t)val;
		}
		else if (memcmp(spd.name, ShaderFile::S_PRDS, 4)==0) {
			fread(&val, 1, 4, fp);
			int sz = val;
			int offset = 0;
			ShaderParamDesc spd;
			for (int i=0; i<sz; ++i) {
				fread(&len, 1, 2, fp);
				fread(mem, len, 1, fp);
				fread(&val, 1, 4, fp);
				spd.name = string(mem);
				spd.type = (variable_t)val;
				spd.offset = offset;
				offset += variable_dword(spd.type)*4;
				m_paramdesc.push_back(spd);
			}
		}
		else if (memcmp(spd.name, ShaderFile::S_PRDE, 4)==0) {
			if (spd.length>0) {
				m_paramdefault = new char[spd.length];
				fread(m_paramdefault, spd.length, 1, fp);
			}
			else
				m_paramdefault = NULL;
			m_lenparamdefault = spd.length;
		}
		else if (memcmp(spd.name, ShaderFile::S_DATA, 4)==0) {
			if (spd.length>0) {
				m_data = new char[spd.length];
				fread(m_data, spd.length, 1, fp);
			}
			else 
				m_data = new char[spd.length];
			m_lendata = spd.length;
		}
		else if (memcmp(spd.name, ShaderFile::S_CODE, 4)==0) {
			if (spd.length>0) {
				m_code = new char[spd.length];
				fread(m_code, spd.length, 1, fp);
			}
			else {
				return -1;
			}

			m_lencode = spd.length;
		}
	}

	fclose(fp);

	return 0;
}

bool ShaderFile::GetParamDesc(const string &name, int &offset, int &type) const
{
	int n = m_paramdesc.size();
	for (int i=0; i<n; ++i) {
		const ShaderParamDesc &spd = m_paramdesc[i];
		if (spd.name.compare(name)==0) {
			offset = spd.offset;
			type = spd.type;
			return true;
		}
	}

	return false;
}


