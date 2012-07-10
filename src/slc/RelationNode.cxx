#include "RelationNode.hxx"
#include "ExprNode.hxx"
#include "FileNode.hxx"
#include "slc_tab.hxx"

SimpleRelNode::SimpleRelNode(Node *n)
{
	m_rel = (RelationNode *)n;
}

Node *SimpleRelNode::Copy()
{
	SimpleRelNode *node = new SimpleRelNode(m_rel->Copy());
	node->Node::Copy(this);

	return node;
}

void SimpleRelNode::Pass1()
{
	m_rel->Pass1();
}

void SimpleRelNode::GenAssembly(aofstream &os)
{
	m_rel->GenAssembly(os);
}

LogopNode::LogopNode(Node *expr1, Suint logop, Node *expr2)
{
	m_expr1 = (RelationNode *)expr1;
	m_logop = logop;
	m_expr2 = (RelationNode *)expr2;
}

Node *LogopNode::Copy()
{
	LogopNode *node = new LogopNode(m_expr1->Copy(), m_logop, m_expr2->Copy());
	node->Node::Copy(this);

	return node;
}

void LogopNode::Pass1()
{
	m_expr1->Pass1();
	m_expr2->Pass2();
	m_label1 = g_file.AddLabel();
}

void LogopNode::GenAssembly(aofstream &os)
{
	if (m_logop==AND) {
		m_expr1->Pass1();
		os<<"jf\t"<<m_label1->m_name<<endl;
		m_expr2->Pass2();
		os<<m_label1->m_name<<':'<<endl;
	}
	else if (m_logop==OR) {
		m_expr1->Pass1();
		os<<"jt\t"<<m_label1->m_name<<endl;
		m_expr2->Pass2();
		os<<m_label1->m_name<<':'<<endl;
	}
}

NotRelNode::NotRelNode(Node *expr)
{
	m_expr = (RelationNode *)expr;
}

Node *NotRelNode::Copy()
{
	NotRelNode *node = new NotRelNode(m_expr->Copy());
	node->Node::Copy(this);

	return node;
}

void NotRelNode::Pass1()
{
	m_expr->Pass1();
}

void NotRelNode::GenAssembly(aofstream &os)
{
	m_expr->GenAssembly(os);
	os<<"neg"<<endl;
}

RelopNode::RelopNode(Node *expr1, Suint relop, Node *expr2)
{
	m_expr1 = (ExprNode *)expr1;
	m_relop = relop;
	m_expr2 = (ExprNode *)expr2;
}

Node *RelopNode::Copy()
{
	RelopNode *node = new RelopNode(m_expr1->Copy(), m_relop, m_expr2->Copy());
	node->Node::Copy(this);

	return node;
}

void RelopNode::Pass1()
{
	m_expr1->Pass1();
	m_expr2->Pass1();

	m_expr1->AllocSymbol();
	m_expr2->AllocSymbol();

	m_expr1->FreeTempSymbol();
	m_expr2->FreeTempSymbol();
}

void RelopNode::GenAssembly(aofstream &os)
{
	m_expr1->GenAssembly(os);
	m_expr2->GenAssembly(os);

	Schar c = m_expr1->GetSymbol()->m_type.Char();
	switch (m_relop) {
	case GT:
		os<<"gt"<<c<<'\t'<<m_expr1->GetSymbol()->m_name<<
			", "<<m_expr2->GetSymbol()->m_name<<endl;
		break;
	case GE:
		os<<"ge"<<c<<'\t'<<m_expr1->GetSymbol()->m_name<<
			", "<<m_expr2->GetSymbol()->m_name<<endl;
		break;
	case LT:
		os<<"lt"<<c<<'\t'<<m_expr1->GetSymbol()->m_name<<
			", "<<m_expr2->GetSymbol()->m_name<<endl;
		break;
	case LE:
		os<<"le"<<c<<'\t'<<m_expr1->GetSymbol()->m_name<<
			", "<<m_expr2->GetSymbol()->m_name<<endl;
		break;
	case EQ:
		os<<"eq"<<c<<'\t'<<m_expr1->GetSymbol()->m_name<<
			", "<<m_expr2->GetSymbol()->m_name<<endl;
		break;
	case NE:
		os<<"ne"<<c<<'\t'<<m_expr1->GetSymbol()->m_name<<
			", "<<m_expr2->GetSymbol()->m_name<<endl;
		break;
	}
}