#ifndef __DEFNODE_HXX__
#define __DEFNODE_HXX__

#include "Node.hxx"
#include "Symbol.hxx"

class StmtNode;
class Symbol;

/**
 * definition 
 */
 
class DefNode : public Node
{
};

class ShaderDefNode : public DefNode, public ScopeNode
{
public:
	ShaderDefNode(Suint32 type, const Schar *name, vector<Node *> *params,
		vector<Node *> *stmts);
	Suint32 GetNodeType() const { return SHADERDEF_NODE; }

	Node *Copy() { return NULL; }

	void Pass1();

	void GenAssembly(aofstream &os);

	Symbol *AddConstantf(Sfloat v);
	Symbol *AddConstantv(Sfloat v0, Sfloat v1, Sfloat v2);
	Symbol *AddConstantm(Sfloat *v);
	Symbol *AddConstants(const string &s);
	Symbol *FindConstantf(Sfloat v);
	Symbol *FindConstantv(Sfloat v0, Sfloat v1, Sfloat v2);
	Symbol *FindConstantm(Sfloat *v);
	Symbol *FindConstants(const string &s);

	Symbol *AddLabel();

	Symbol *AddParamSymbol(const string &s, Type type);
	Symbol *AddLocalSymbol(const string &s, Type type);

	Symbol *AddTempSymbol(Type tp);
	Symbol *FindFreeSymbol(Type tp);
	void FreeLocalSymbol(Symbol *sb);

	
public:
	shader_t m_type;
	string m_name;
	vector<Node *> *m_params;
	vector<Node *> *m_stmts;

	vector<DataSymbol *> m_datasymbols;
	Suint m_nextdatavar;
	vector<Symbol *> m_localvars;
	vector<Symbol *> m_freevars;
	Suint m_nextlocal;
	Suint m_nextparam;
	Suint m_label;
};

class FuncDefNode : public DefNode
{
public:
	FuncDefNode(Sint rttype, const Schar *name, NodeList *m_params, NodeList *stmts);
	FuncDefNode(Type rttype, const Schar *name, NodeList *m_params, NodeList *stmts);
	Suint32 GetNodeType() const { return FUNCDEF_NODE; }

	Node *Copy();

	void Pass1();
	void GenAssembly(aofstream &os);
	void SetSymbol(Symbol *sym);

	void SetArg(int i, Symbol *s);

public:
	Type m_rttype;
	string m_name;
	NodeList *m_params;
	NodeList *m_stmts;
	Symbol *m_symbol;

	string GetMangledName(const string &name);
};

#endif
