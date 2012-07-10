#ifndef __NODE_HXX__
#define __NODE_HXX__

#include "sli.hxx"
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <stack>
using namespace std;

typedef double Sfloat;
typedef int Sint;
typedef unsigned int Suint;
typedef int Sint32;
typedef unsigned int Suint32;
typedef char Schar;
typedef unsigned char Suchar;
typedef bool Sbool;

#define ASSERT(a) { if (!(a)) { int b, c=0; b=1/c; } }

enum {
	FILE_NODE,

	SHADERDEF_NODE,
	FUNCDEF_NODE,

	NUMBEREXPR_NODE,
	TRIPLE_NODE,
	SIXTEENTUPLE_NODE,
	STRINGCONSTANT_NODE,
	IDENT_NODE,
	IDENTARRAY_NODE,
	PROCCALL_NODE,
	ASSIGNEXPR_NODE,
	BINOPEXPR_NODE,
	UNARYEXPR_NODE,
	QUESTIONEXPR_NODE,
	TYPECASTEXPR_NODE,

	RELATION_NODE,
	RELOP_NODE,
	LOGOP_NODE,
	NOTREL_NODE,

	TYPECAST_NODE,

	DEFVAR_NODE,
	STMTLIST_NODE,
	VARDEFSTMT_NODE,
	PROCCALLLOOP_NODE,
	ENDSTMT_NODE,
	PROCCALLSTMT_NODE,
	ASSIGNSTMT_NODE,
	RETURNSTMT_NODE,
	BREAKSTMT_NODE,
	CONTINUESTMT_NODE,
	IFSTMT_NODE,
	WHILE_NODE,
	FOR_NODE,
	SOLAR_NODE,
	ILLUMINATE_NODE,
	ILLUMINANCE_NODE,
};

extern int g_numlines;
extern string g_filename;


class aofstream : public ofstream
{
};

class bofstream : public ofstream
{
};

class Node
{
public:
	Node() { m_linenum = g_numlines; m_file = g_filename;}
	virtual Suint32 GetNodeType() const = 0;

	virtual void Pass1() {}
	virtual void Pass2() {}

	virtual void GenAssembly(aofstream &os) {}
	virtual void GenCode(bofstream &os) {}

	virtual Node *Copy()=0;
	void Copy(const Node *n) { m_linenum = n->m_linenum; m_file = n->m_file; }

	static shader_t ConvertToShaderType(Suint32 type);
	static variable_t ConvertToVariableType(Suint32 type);

public:
	Suint32 m_linenum;
	string m_file;
	
	Suint32 m_codeline;
};

class NodeList : public vector<Node *>
{
public:
	NodeList *Copy() { NodeList *ni = new NodeList; NodeList::iterator it = begin(); for (; it<end(); ++it) { Node *node = (*it)->Copy(); ni->push_back(node); }  return ni;}
};


#endif