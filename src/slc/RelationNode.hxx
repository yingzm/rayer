#ifndef __RELATIONNODE_HXX__
#define __RELATIONNODE_HXX__

#include "Node.hxx"

class ExprNode;
class Symbol;

class RelationNode : public Node
{
};

class SimpleRelNode : public RelationNode
{
public:
	SimpleRelNode(Node *);
	Suint32 GetNodeType() const { return RELATION_NODE; }

	Node *Copy();

	void Pass1();
	void GenAssembly(aofstream &os);

private:
	RelationNode *m_rel;
};


class RelopNode: public RelationNode
{
public:
	RelopNode(Node *expr1, Suint relop, Node *expr2);
	Suint32 GetNodeType() const { return RELOP_NODE; }

	Node *Copy();

	void Pass1();
	void GenAssembly(aofstream &os);
	
private:
	ExprNode *m_expr1, *m_expr2;
	Suint m_relop;
};

class LogopNode : public RelationNode
{
public:
	LogopNode(Node *expr1, Suint logop, Node *expr2);
	Suint32 GetNodeType() const { return LOGOP_NODE; }

	Node *Copy();
	
	void Pass1();
	void GenAssembly(aofstream &os);
	
private:
	RelationNode *m_expr1, *m_expr2;
	Suint m_logop;
	Symbol *m_label1;
};

class NotRelNode : public RelationNode
{
public:
	NotRelNode(Node *expr);
	Suint32 GetNodeType() const { return NOTREL_NODE; }

	Node *Copy();
	
	void Pass1();
	void GenAssembly(aofstream &os);

private: 
	RelationNode *m_expr;
};

#endif 