#include "StmtNode.hxx"

#include "ExprNode.hxx"
#include "RelationNode.hxx"
#include "FileNode.hxx"
#include "Symbol.hxx"

void EndStmtNode::GenAssembly(aofstream &os)
{
	os<<"end"<<endl;
}

Node *EndStmtNode::Copy()
{
	EndStmtNode *node = new EndStmtNode();
	node->Node::Copy(this);

	return node;
}

VarDefStmtNode::VarDefStmtNode(NodeList *node)
{
	m_vardef = node;
};

Node *VarDefStmtNode::Copy()
{
	VarDefStmtNode *node = new VarDefStmtNode(m_vardef->Copy());
	node->Node::Copy(this);

	return node;
}

void VarDefStmtNode::Pass1()
{
	int n= m_vardef->size();
	for (int i=0; i<n; ++i) {
		DefVarNode *node = (DefVarNode *)(*m_vardef)[i];

		node->Pass1();
	}
}
	
void VarDefStmtNode::GenAssembly(aofstream &os)
{
	int n = m_vardef->size();
	for (int i=0; i<n; ++i) {
		DefVarNode *node = (DefVarNode *)(*m_vardef)[i];
		node->GenAssembly(os);
	}
}


AssignStmtNode::AssignStmtNode(Node *expr)
{
	m_expr = (ExprNode *)expr;
}

Node *AssignStmtNode::Copy()
{
	AssignStmtNode *node = new AssignStmtNode(m_expr->Copy());

	return node;
}

void AssignStmtNode::GenAssembly(aofstream &os)
{
	m_expr->GenAssembly(os);
}

void AssignStmtNode::Pass1()
{
	m_expr->Pass1();
}

BreakStmtNode::BreakStmtNode(Sint val)
{
	m_val = val;
}

Node *BreakStmtNode::Copy()
{
	BreakStmtNode *node = new BreakStmtNode(m_val);

	return node;
}

void BreakStmtNode::Pass1()
{
	m_label = g_file.GetBreakLabel(m_val);
}

void BreakStmtNode::GenAssembly(aofstream &os)
{
	os<<"jmp\t"<<m_label->m_name<<endl;
}

ContinueStmtNode::ContinueStmtNode(Sint val)
{
	m_val = val; 
}

Node *ContinueStmtNode::Copy()
{
	ContinueStmtNode *node = new ContinueStmtNode(m_val);

	return node;
}

void ContinueStmtNode::Pass1()
{
	m_label = g_file.GetContinueLabel(m_val);
}

void ContinueStmtNode::GenAssembly(aofstream &os)
{
	os<<"jmp\t"<<m_label->m_name<<endl;
}



IfStmtNode::IfStmtNode(Node *rel, Node *stmt, Node *elsestmt)
{
	m_rel = (RelationNode *)rel;
	m_stmt = (StmtNode *)stmt;
	m_elsestmt = (StmtNode *)elsestmt;
}

Node *IfStmtNode::Copy()
{
	IfStmtNode *node;
	if (m_elsestmt)
		node = new IfStmtNode(m_rel->Copy(), m_stmt->Copy(), m_elsestmt->Copy());
	else
		node = new IfStmtNode(m_rel->Copy(), m_stmt->Copy(), NULL);

	return node;
}

void IfStmtNode::Pass1()
{
	m_rel->Pass1();

	m_stmt->Pass1();

	if (m_elsestmt) {
		m_elsestmt->Pass1();
	}

	m_label1 = g_file.AddLabel();
	if (m_elsestmt)
		m_label2= g_file.AddLabel();
	else
		m_label2 = NULL;
}
	
void IfStmtNode::GenAssembly(aofstream &os)
{
	m_rel->GenAssembly(os);

	if (m_elsestmt)
		os<<"jf\t"<<m_label2->m_name<<endl;
	else
		os<<"jf\t"<<m_label1->m_name<<endl;
	
	m_stmt->GenAssembly(os);

	if (m_elsestmt)
		os<<"jmp\t"<<m_label1->m_name<<endl;

	if (m_elsestmt) {
		os<<m_label2->m_name<<':'<<endl;
		m_elsestmt->GenAssembly(os);
	}

	os<<m_label1->m_name<<':'<<endl;
}


ProcCallStmtNode::ProcCallStmtNode(Node *expr)
{
	m_expr = (ProcCallNode *)expr;
}

Node *ProcCallStmtNode::Copy()
{
	ProcCallStmtNode *node = new ProcCallStmtNode(m_expr->Copy());

	return node;
}

void ProcCallStmtNode::Pass1()
{
	m_expr->Pass1();
}

void ProcCallStmtNode::GenAssembly(aofstream &os)
{
	m_expr->GenAssembly(os);
}

ReturnStmtNode::ReturnStmtNode(Node *expr)
{
	m_expr = (ExprNode *)expr;
	m_symbol = NULL;
}

Node *ReturnStmtNode::Copy()
{
	ReturnStmtNode *node = new ReturnStmtNode(m_expr->Copy());
	node->m_symbol = m_symbol;
	node->Node::Copy(this);

	return node;
}

void ReturnStmtNode::SetSymbol(Symbol *sym)
{
	m_symbol = sym;
}

void ReturnStmtNode::Pass1()
{
	m_expr->Pass1();

	m_expr->AllocSymbol();
}

void ReturnStmtNode::GenAssembly(aofstream &os)
{
	m_expr->GenAssembly(os);
	os<<"mov"<<m_symbol->m_type.Char()<<'\t'<<m_symbol->m_name<<", "<<m_expr->GetSymbol()->m_name<<endl;
}

ProcCallLoopNode::ProcCallLoopNode(Node *node)
{
	m_proc = (ProcCallNode *)node;
}

Node *ProcCallLoopNode::Copy()
{
	ProcCallLoopNode *node = new ProcCallLoopNode(m_proc->Copy());
	node->LoopControlNode::Copy(this);

	return node;
}

void ProcCallLoopNode::Pass1()
{
	m_proc->Pass1();
	m_stmt->Pass1();
	m_label = g_file.AddLabel();
}

void ProcCallLoopNode::GenAssembly(aofstream &os)
{
	m_proc->GenAssembly(os);
	os<<"jf\t"<<m_label->m_name<<endl;
	m_stmt->GenAssembly(os);

	os<<m_label->m_name<<':'<<endl;
}

StmtListNode::StmtListNode(NodeList *stmts)
{
	m_stmts = stmts;
}

Node *StmtListNode::Copy()
{
	StmtListNode *node = new StmtListNode(m_stmts->Copy());
	return node;
}

void StmtListNode::Pass1()
{
	int n = m_stmts->size();
	m_scope = new Scope;
	m_scope->m_parent = g_file.GetCurrentScope();
	m_scope->m_da2parent = true;
	g_file.BeginScope(m_scope, false);
	for (int i=0; i<n; ++i) {
		StmtNode *node = (StmtNode *)(*m_stmts)[i];
		node->Pass1();
	}
	g_file.EndScope(false);
}

void StmtListNode::GenAssembly(aofstream &os)
{
	int n = m_stmts->size();
	for (int i=0; i<n; ++i) {
		StmtNode *node = (StmtNode *)(*m_stmts)[i];
		node->GenAssembly(os);
	}
}

void LoopControlNode::Copy(const Node *node)
{
	LoopControlNode *nd = (LoopControlNode *)node;
	m_stmt = (StmtNode *)(nd->m_stmt->Copy());
	Node::Copy(this);
}

WhileNode::WhileNode(Node *rel)
{
	m_rel = (RelationNode *)rel;
}

Node *WhileNode::Copy()
{
	WhileNode *node = new WhileNode(m_rel->Copy());
	node->LoopControlNode::Copy(this);

	return node;
}

void WhileNode::Pass1()
{
	m_rel->Pass1();
	m_label1 = g_file.AddLabel();
	m_label2 = g_file.AddLabel();
	g_file.PushContinueLabel(m_label1);
	g_file.PushBreakLabel(m_label2);
	m_stmt->Pass1();
	g_file.PopContinueLabel();
	g_file.PopBreakLabel();
}

void WhileNode::GenAssembly(aofstream &os)
{
	os<<m_label1->m_name<<':'<<endl;
	m_rel->GenAssembly(os);
	os<<"jf\t"<<m_label2->m_name<<':'<<endl;
	m_stmt->GenAssembly(os);
	os<<m_label2->m_name<<':'<<endl;
}


ForNode::ForNode(Node *expr1, Node *rel, Node *expr2)
{
	m_expr1 = (ExprNode *)expr1;
	m_rel = (RelationNode *)rel;
	m_expr2 = (ExprNode *)expr2;
}

Node *ForNode::Copy()
{
	ForNode *node = new ForNode(m_expr1->Copy(), m_rel->Copy(), m_expr2->Copy());
	node->LoopControlNode::Copy(this);

	return node;
}

void ForNode::Pass1()
{
	m_expr1->Pass1();
	m_label1 = g_file.AddLabel();
	m_label2 = g_file.AddLabel();
	g_file.PushContinueLabel(m_label1);
	g_file.PushBreakLabel(m_label2);
	m_rel->Pass1();
	m_stmt->Pass1();
	m_expr2->Pass1();
	g_file.PopContinueLabel();
	g_file.PopBreakLabel();
}

void ForNode::GenAssembly(aofstream &os)
{
	m_expr1->GenAssembly(os);
	os<<m_label1->m_name<<':'<<endl;
	m_rel->GenAssembly(os);
	os<<"jf\t"<<m_label2->m_name<<endl;
	m_stmt->GenAssembly(os);
	m_expr2->GenAssembly(os);
	os<<"jmp\t"<<m_label1->m_name<<endl;
	os<<m_label2->m_name<<':'<<endl;
}


