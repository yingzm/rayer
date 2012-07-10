#ifndef __EXPRNODE_HXX__
#define __EXPRNODE_HXX__

#include "Node.hxx"
#include "Symbol.hxx"

class RelationNode;

class ExprNode : public Node
{
public:
	ExprNode() {m_symbol=NULL; m_type = Type(VAR_UNKNOWN); }
	/**
	 * if the father can't provide an symbol for it, it must
	 * call AllocSymbol of it
	 */
	virtual void AllocSymbol();
	/**
	 * Free the temp symbol 
	 */
	virtual void FreeTempSymbol();

	/**
	 * Evaluation a constant value for Expression
	 * @param DataSymbol &ds, the evaluation result container
	 * @return -1, if failed to evaluation a constant
	 *	   0, if success.
	 */
	virtual Sint Eval(DataSymbol &ds) {return -1; }

	virtual bool IsSimple() { return false; }

	virtual Node *Copy()=0;
	void Copy(const ExprNode *n) { Node::Copy(n); m_symbol = n->m_symbol; m_type = n->m_type; }

	virtual Sint GetAddr() { return m_symbol->u.addr; }
	Type GetType() { return m_type; }
	void SetType(Type type) { m_type = type; }

	virtual void SetSymbol(Symbol *sym) { ASSERT(sym!=NULL); 
		m_symbol = sym; m_type = m_symbol->m_type; }
	virtual Symbol *GetSymbol() { return m_symbol; }

protected:
	Symbol *m_symbol;
	// Normally this is the type of m_symbol
	// When m_symbol==NULL, this can be void, unknown or error
	Type m_type;
};

/**
 * PrimexprNode 
 */
class PrimExprNode : public ExprNode
{
};

class NumberExprNode : public PrimExprNode
{
public:
	NumberExprNode(Sfloat v);
	Suint32 GetNodeType() const { return NUMBEREXPR_NODE; }
	int Eval(DataSymbol &ds) { ds.flt[0] = m_v; ds.size = 8; ds.type = VAR_FLOAT; return 0; }
	bool IsSimple() { return true; }

	void AllocSymbol() {}
	void SetSymbol(Symbol *sym) {m_symbol = sym; m_type = sym->m_type;}

	Node *Copy();

	void Pass1();
	
public:
	Sfloat m_v;
};

class TripleNode : public PrimExprNode
{
public:
	TripleNode(Node *v0, Node *v1, Node *v2);
	Suint32 GetNodeType() const { return TRIPLE_NODE; }
	int Eval(DataSymbol &ds);
	bool IsSimple() { return true; }
	void AllocSymbol() {}
	void SetSymbol(Symbol *sym) {m_symbol = sym; m_type = sym->m_type;}
	Node *Copy();

	void Pass1();
	
private:
	ExprNode *m_v[3];
};

class SixteentupleNode : public PrimExprNode
{
public:
	SixteentupleNode(Node * v0, Node * v1, Node * v2, Node * v3, 
		Node * v4, Node * v5, Node * v6, Node * v7, 
		Node * v8, Node * v9, Node * v10, Node * v11, 
		Node * v12, Node * v13, Node * v14, Node * v15);
	Suint32 GetNodeType() const { return SIXTEENTUPLE_NODE; }
	bool IsSimple() { return true; }
	void AllocSymbol() {}
	void SetSymbol(Symbol *sym) {};
	Node *Copy();

	int Eval(DataSymbol &ds);

	void Pass1();

private:
	ExprNode *m_v[16];
};

class StringConstantNode : public PrimExprNode
{
public:
	StringConstantNode(const Schar *s);
	Suint32 GetNodeType() const { return STRINGCONSTANT_NODE; }
	bool IsSimple() { return true; }
	void AllocSymbol() {}
	void SetSymbol(Symbol *sym) {};
	Node *Copy();

	Sint Eval(DataSymbol &ds);

	void Pass1();
	
private:
	string m_s;
};

class LValueNode : public PrimExprNode
{
public:
	LValueNode(const Schar *name) { m_name = name; }
	const string &GetName() const { return m_name; }
	virtual Node *Copy()=0;
	void Copy(const LValueNode *n) { ExprNode::Copy(n); m_name = n->GetName(); }

protected:
	string m_name;
};

class IdentNode : public LValueNode
{
public:
	IdentNode(const Schar *name);
	Suint32 GetNodeType() const { return IDENT_NODE; }
	bool IsSimple() { return true; }
	void AllocSymbol();
	void SetSymbol(Symbol *sym) {AllocSymbol();};

	Node *Copy();

	void Pass1();
};

class IdentArrayNode : public LValueNode
{
public:
	IdentArrayNode(const Schar *name, Sfloat val);
	Suint32 GetNodeType() const { return IDENTARRAY_NODE; }
	bool IsSimple() { return true; }
	Node *Copy();
	void AllocSymbol();
	void SetSymbol(Symbol *sym) {AllocSymbol();};

	void Pass1();

	Sint GetAddr() { return m_symbol->u.addr+m_symbol->m_type.Size()*m_offset; }
	
public:
	Sint m_offset;
};

class ProcCallNode : public PrimExprNode, public ScopeNode
{
public:
	ProcCallNode(const Schar *name, NodeList *);
	Suint32 GetNodeType() const { return PROCCALL_NODE; }
	Node *Copy();

	void AllocSymbol();
	void SetSymbol(Symbol *sym);

	Sint Eval(DataSymbol &ds);

	void Pass1();
	void GenAssembly(aofstream &os);
	
public:
	Symbol *m_funcsym;
	string m_name;
	NodeList *m_procargs;
	FuncDefNode *m_node;

	string GetMangledName(Type t);
	void GetFuncSymbol();
};

class AssignExprNode : public PrimExprNode
{
public:
	AssignExprNode(Node *lval, Node *expr);
	Suint32 GetNodeType() const { return ASSIGNEXPR_NODE; }
	Sbool IsSimple() { return true; }
	Node *Copy();

	void AllocSymbol() {if (m_symbol!=NULL) return; m_lval->AllocSymbol(); m_symbol = m_lval->GetSymbol(); m_type = m_symbol->m_type; }
	void SetSymbol(Symbol *) {}

	void GenAssembly(aofstream &os);
	void Pass1();
	
public:
	LValueNode *m_lval;
	ExprNode *m_expr;
};


/**
 * BinopExprNode
 */
class BinopExprNode : public ExprNode
{
public:
	BinopExprNode(Node *expr1, Suint binop, Node *expr2);
	Suint32 GetNodeType() const { return BINOPEXPR_NODE; }
	Node *Copy();

	void GenAssembly(aofstream &os);
	void Pass1();

private:
	ExprNode *m_expr1, *m_expr2;
	Suint m_binop;

	Type GetDestType();
};

class UnaryExprNode : public ExprNode
{
public:
	UnaryExprNode(Suint unaryop, Node *expr);
	Suint32 GetNodeType() const { return UNARYEXPR_NODE; }

	Sint Eval(DataSymbol &ds);

	Node *Copy();

	void Pass1();

	void GenAssembly(aofstream &os);

private:
	ExprNode *m_expr;
	Suint m_unaryop;
};

class QuestionExprNode : public ExprNode
{
public:
	QuestionExprNode(Node *rel, Node *expr1, Node *expr2);
	Suint32 GetNodeType() const { return QUESTIONEXPR_NODE; }
	Node *Copy();

	void Pass1();
	void GenAssembly(aofstream &os);
	
private:
	RelationNode *m_rel;
	ExprNode *m_expr1, *m_expr2;
	Symbol *m_label1, *m_label2;
};

class Typecast : public Node
{
public:
	Typecast(Suint t);
	Typecast(Suint t, const Schar *space);
	Suint32 GetNodeType() const { return TYPECAST_NODE; }
	Node *Copy();

	Type m_type;
	string m_space;
};

class TypecastExprNode : public ExprNode
{
public:
	TypecastExprNode(Node *tc, Node *expr);
	Suint32 GetNodeType() const { return TYPECASTEXPR_NODE; }
	Node *Copy();

	Symbol *GetSymbol() { if (m_tc->m_space.compare("")==0) return m_expr->GetSymbol(); else return m_symbol; }

	void Pass1();
	void GenAssembly(aofstream &os);


	Sint Eval(DataSymbol &ds);
	
public:
	Typecast *m_tc;
	ExprNode *m_expr;
	Symbol *m_sfrom;
	Symbol *m_sto;
};

Type GetVariableType(Sint type);
class DefVarNode : public AssignExprNode
{
public:
	DefVarNode(Node *lval, Node *din);
	Suint32 GetNodeType() const { return DEFVAR_NODE; }
	Node *Copy();

	void AllocSymbol() {}
	void SetSymbol(Symbol *) {}

	void SetType(Sint type) { m_type.m_type = type; }
	void SetExtern() { m_type.m_type |= VAR_EXTERN; }
	void SetOutput() { m_type.m_type |= VAR_OUTPUT; }
	void SetParam() { m_isparam = true; }
	void SetFunc() { m_isfunc = true; }

	void Pass1();
	void GenAssembly(aofstream &os);

	
public:
	string m_name;
	bool m_isparam;
	bool m_isfunc;

	void PassShaderParam();
	void PassFuncParam();
	void PassShaderVar();
	void PassFuncVar();

	void GenShaderParam(aofstream &os);
	void GenFuncParam(aofstream &os);
	void GenShaderVar(aofstream &os);
	void GenFuncVar(aofstream &os);

};



#endif