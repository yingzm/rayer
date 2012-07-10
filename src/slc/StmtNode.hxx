#ifndef __STMTNODE_HXX__
#define __STMTNODE_HXX__

#include "Node.hxx"
#include "Symbol.hxx"

class ExprNode;
class RelationNode;
class ProcCallNode;
class Symbol;

class StmtNode : public Node
{
public:	
};

class StmtListNode : public StmtNode, public ScopeNode
{
public:
	StmtListNode(NodeList *);
	Suint32 GetNodeType() const { return STMTLIST_NODE; }

	Node *Copy();

	void Pass1();
	void GenAssembly(aofstream &os);

public:
	NodeList *m_stmts;
};

class EndStmtNode : public Node
{
public:
	Suint32 GetNodeType() const { return ENDSTMT_NODE; }
	void GenAssembly(aofstream &os);

	Node *Copy();
};

class VarDefStmtNode : public StmtNode
{
public:
	VarDefStmtNode(NodeList *node);
	Suint32 GetNodeType() const { return VARDEFSTMT_NODE; }

	Node *Copy();
	void Pass1();
	void GenAssembly(aofstream &os);

public:
	NodeList *m_vardef;
};

class AssignStmtNode : public StmtNode
{
public:
	AssignStmtNode(Node *expr);
	Suint32 GetNodeType() const { return ASSIGNSTMT_NODE; }

	Node *Copy();

	void Pass1();

	void GenAssembly(aofstream &os);
	
private:
	ExprNode *m_expr;
};

class ProcCallStmtNode : public StmtNode
{
public:
	ProcCallStmtNode(Node *expr);
	Suint32 GetNodeType() const { return PROCCALLSTMT_NODE; }

	Node *Copy();

	void Pass1();

	void GenAssembly(aofstream &os);
	
public:
	ProcCallNode *m_expr;
};

class ReturnStmtNode : public StmtNode
{
public:
	ReturnStmtNode(Node *expr);
	Suint32 GetNodeType() const { return RETURNSTMT_NODE; }

	void SetSymbol(Symbol *sym);

	Node *Copy();
	void Pass1();
	void GenAssembly(aofstream &os);
	
private:
	ExprNode *m_expr;
	Symbol *m_symbol;
};

class BreakStmtNode : public StmtNode
{
public:
	BreakStmtNode(Sint val);
	Suint32 GetNodeType() const { return BREAKSTMT_NODE; }

	Node *Copy();
	void Pass1();
	void GenAssembly(aofstream &os);
	
private:
	Sint m_val;
	Symbol *m_label;
};

class ContinueStmtNode : public StmtNode
{
public:
	ContinueStmtNode(Sint val);
	Suint32 GetNodeType() const { return CONTINUESTMT_NODE; }
	Node *Copy();
	void Pass1();
	void GenAssembly(aofstream &os);
	
private:
	Sint m_val;
	Symbol *m_label;
};


class IfStmtNode : public StmtNode
{
public:
	IfStmtNode(Node *rel, Node *stmt, Node *elsestmt);
	Suint32 GetNodeType() const { return IFSTMT_NODE; }
	Node *Copy();
	void Pass1();
	void GenAssembly(aofstream &os);

private:
	RelationNode *m_rel;
	StmtNode *m_stmt, *m_elsestmt;
	Symbol *m_label1, *m_label2;
};

class LoopControlNode : public Node
{
public:
	void Copy(const Node *node);
	StmtNode *m_stmt;
};

class ProcCallLoopNode : public LoopControlNode
{
public:
	ProcCallLoopNode(Node *node);
	Suint32 GetNodeType() const { return PROCCALLLOOP_NODE; }

	Node *Copy();
	void Pass1();
	void GenAssembly(aofstream &os);

public:
	ProcCallNode *m_proc;
	Symbol *m_label;
};

class WhileNode : public LoopControlNode
{
public:
	WhileNode(Node *rel);
	Suint32 GetNodeType() const { return WHILE_NODE; }

	Node *Copy();
	void Pass1();
	void GenAssembly(aofstream &os);
	
private:
	RelationNode *m_rel;
	Symbol *m_label1, *m_label2;
};

class ForNode : public LoopControlNode
{
public:
	ForNode(Node *expr1, Node *rel, Node *expr2);
	Suint32 GetNodeType() const { return FOR_NODE; }

	Node *Copy();
	void Pass1();
	void GenAssembly(aofstream &os);
	
private:
	ExprNode *m_expr1, *m_expr2;
	RelationNode *m_rel;
	Symbol *m_label1, *m_label2;
};



#endif /* __STMTNODE_HXX__ */