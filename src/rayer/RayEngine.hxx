#ifndef __RAYENGINE_HXX
#define __RAYENGINE_HXX

#include "ri.hxx"

class RayEngine
{
public:
	int Create();
	void Destroy();
	/*
	 * Read Rib and generate function call
	 */
	int ReadRib(const string &str);

private:
	Ri m_ri;

	char *m_filebuf;
	char *m_fileptr, *m_fileend;
	char m_buffer[1024];

	int m_curline;
	string m_curcmd;

	bool IsWhiteSpace(char c);
	bool IsValidInteger(const char *s) const;
	bool IsValidDouble(const char *s) const;

	void SkipWhiteSpace();
	bool SkipWhiteSpaceNoReturn();
	int ReadWord();

	int ReadToken();
	int ReadInt(int *i);
	int ReadDouble(double *i);
	int ReadString(char *s);
	int ReadStringOrValue(char *s);
	int ReadColor(RtColor c);
	int ReadMatrix(RtMatrix m);
	int ReadArrayi(int *&buf);
	int ReadArrayd(double *&buf);
	int ReadParameter(vector<ShaderParam> &p);

	void NextChar();

	void PrintSimpleError();
};

inline void RayEngine::NextChar()
{
	if (*m_fileptr=='\n')
		m_curline++;

	m_fileptr++;
}

#endif /* __RAYENGINE_HXX */
