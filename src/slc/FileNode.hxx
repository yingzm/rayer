#ifndef __FILENODE_HXX__
#define __FILENODE_HXX__

#include "Node.hxx"
#include "DefNode.hxx"

class Symbol;
class DataSymbol;
class Scope;


class FileNode : public Node
{
public:
	FileNode();
	Suint32 GetNodeType() const { return FILE_NODE; }

	Node *Copy() { return NULL; }
	
	void AddFuncDef(DefNode *node);
	void AddShaderDef(DefNode *node);
	
	void Pass1();
	void Pass2();
	
	Sint GenAssembly(const Schar *name);

	void GenDeclare(aofstream &os);
	
	/*
	 * allocate memory at data mem
	 */
	Sint AllocNumber(Sfloat v);
	Sint AllocString(const string &s);
	Sint AllocTriple(Sfloat v0, Sfloat v1, Sfloat v2);
	Sint AllocSixteen(Sfloat v0, Sfloat v1, Sfloat v2, Sfloat v3, 
		Sfloat v4, Sfloat v5, Sfloat v6, Sfloat v7, 
		Sfloat v8, Sfloat v9, Sfloat v10, Sfloat v11, 
		Sfloat v12, Sfloat v13, Sfloat v14, Sfloat v15);

	/*
	 * Symbol Management
	 */	
	void AddShaderEnv(shader_t st);
	Symbol *FindFuncSymbol(const string &s);
	Symbol *FindVarSymbol(const string &s, bool isextern=false);
	Symbol *AddVarSymbol(const string &s, bool isparam, Type type);
	Symbol *AddFuncSymbol(const string &s, FuncDefNode *fdn);
	Symbol *AllocTempVarSymbol(Type vt);
	void FreeTempSymbol(Symbol *sb);

	void AddSymbol(Symbol *sb) { m_curscope->AddSymbol(sb); }
	void AddAliasSymbol(const string &name, Symbol *sb) { m_curscope->AddAliasSymbol(name, sb); }

	Symbol *AddConstantf(Sfloat v);
	Symbol *AddConstantv(Sfloat v0, Sfloat v1, Sfloat v2);
	Symbol *AddConstantm(Sfloat *v);
	Symbol *AddConstants(const string &s);

	Symbol *AddLabel();

	Scope *GetCurrentScope() { return m_curscope; }
	void BeginScope(Scope *sc, bool addcnt);
	void EndScope(bool subcnt);
	bool IsInFunc() { return m_funccnt!=0; }

	Symbol *GetBreakLabel(Sint val);
	void PushBreakLabel(Symbol *s);
	void PopBreakLabel();

	Symbol *GetContinueLabel(Sint val);
	void PushContinueLabel(Symbol *s);
	void PopContinueLabel();

	void ReportUnresolvedSymbol(Node *nd, const string &name);
	void ReportTypeError(Node *nd, Type sb1, Type sb2);
	void ReportNonconstant(Node *nd);
	
private:
	NodeList m_funcdef;
	ShaderDefNode *m_shaderdef;
	DefNode *m_curdef;
	
	map<string, Symbol *> m_sysfuncsymbols;
	Scope m_globalscope;	// global function definition
	Scope *m_curscope;
	stack<Scope *> m_scopestack;
	int m_funccnt;

	vector<Symbol *> m_breakstack;
	vector<Symbol *> m_continuestack;

	Suint m_numerror, m_numwarning;
};

extern FileNode g_file;

#endif