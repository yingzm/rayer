#include "ExprNode.hxx"

#include "FileNode.hxx"
#include "Symbol.hxx"
#include "RelationNode.hxx"

#include "slc_tab.hxx"

#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926
#endif

void ExprNode::AllocSymbol()
{
	ASSERT(m_symbol==0);
	ASSERT(!m_type.IsInvalid());

	m_symbol = g_file.AllocTempVarSymbol(m_type);
}

void ExprNode::FreeTempSymbol()
{
	if (m_symbol && m_symbol->IsTemp())
		g_file.FreeTempSymbol(m_symbol);
}

NumberExprNode::NumberExprNode(Sfloat v)
{
	m_v = v;

	m_type = Type(VAR_FLOAT);
}

Node *NumberExprNode::Copy()
{
	NumberExprNode *node = new NumberExprNode(m_v);
	node->ExprNode::Copy(this);

	return node;
}

void NumberExprNode::Pass1()
{
	SetSymbol(g_file.AddConstantf(m_v));
}

TripleNode::TripleNode(Node *v0, Node *v1, Node *v2)
{
	m_v[0] = (ExprNode *)v0;
	m_v[1] = (ExprNode *)v1;
	m_v[2] = (ExprNode *)v2;

	m_type = Type(VAR_VECTOR);
}

Node *TripleNode::Copy()
{
	TripleNode *node = new TripleNode(m_v[0]->Copy(), 
		m_v[1]->Copy(), m_v[2]->Copy());

	node->ExprNode::Copy(this);

	return node;
}

int TripleNode::Eval(DataSymbol &ds)
{
	DataSymbol ds2;
	if (m_v[0]->Eval(ds2)<0 || ds2.size!=8) {
		g_file.ReportNonconstant(m_v[0]);
		return -1;
	}
	ds.flt[0] = ds2.flt[0];
	if (m_v[1]->Eval(ds2)<0 || ds2.size!=8) {
		g_file.ReportNonconstant(m_v[1]);
		return -1;
	}
	ds.flt[1] = ds2.flt[0];
	if (m_v[2]->Eval(ds2)<0 || ds2.size!=8) {
		g_file.ReportNonconstant(m_v[2]);
		return -1;
	}
	ds.flt[2]  = ds2.flt[0];
	ds.size = 24;
	ds.type = VAR_VECTOR;

	return 0;
}

void TripleNode::Pass1()
{
	DataSymbol ds;
	if (Eval(ds)>=0)
		SetSymbol(g_file.AddConstantv(ds.flt[0], ds.flt[1], ds.flt[2]));
}

SixteentupleNode::SixteentupleNode(Node * v0, Node * v1, Node * v2, Node * v3, 
	Node * v4, Node * v5, Node * v6, Node * v7, 
	Node * v8, Node * v9, Node * v10, Node * v11, 
	Node * v12, Node * v13, Node * v14, Node * v15)
{
	m_v[0] = (ExprNode *)v0; m_v[1] = (ExprNode *)v1; m_v[2] = (ExprNode *)v2; m_v[3] = (ExprNode *)v3;
	m_v[4] = (ExprNode *)v4; m_v[5] = (ExprNode *)v5; m_v[6] = (ExprNode *)v6; m_v[7] = (ExprNode *)v7;
	m_v[8] = (ExprNode *)v8; m_v[9] = (ExprNode *)v9; m_v[10] = (ExprNode *)v10; m_v[11] = (ExprNode *)v11;
	m_v[12] = (ExprNode *)v12; m_v[13] = (ExprNode *)v13; m_v[14] = (ExprNode *)v14; m_v[15] = (ExprNode *)v15;

	m_type = Type(VAR_MATRIX);
}

Node *SixteentupleNode::Copy()
{
	SixteentupleNode *node = new SixteentupleNode(m_v[0]->Copy(), m_v[1]->Copy(), m_v[2]->Copy(), m_v[3]->Copy(),
		m_v[4]->Copy(), m_v[5]->Copy(), m_v[6]->Copy(), m_v[7]->Copy(),
		m_v[8]->Copy(), m_v[9]->Copy(), m_v[10]->Copy(), m_v[11]->Copy(),
		m_v[12]->Copy(), m_v[13]->Copy(), m_v[14]->Copy(), m_v[15]->Copy());

	node->ExprNode::Copy(this);

	return node;
}

int SixteentupleNode::Eval(DataSymbol &ds)
{
	DataSymbol ds2;
	for (int i=0; i<16; ++i) {
		if (m_v[i]->Eval(ds2)<0 || ds2.size!=8) {
			g_file.ReportNonconstant(m_v[i]);
			return -1;
		}
		ds.flt[i] = ds2.flt[0];
	}
	ds.size = 16*8;
	ds.type = VAR_MATRIX;

	return 0;
}

void SixteentupleNode::Pass1()
{
	DataSymbol ds;
	if (Eval(ds)>=0)
		SetSymbol(g_file.AddConstantm(ds.flt));
}

StringConstantNode::StringConstantNode(const Schar *s)
{
	m_s = s;

	m_type = Type(VAR_STRING);
}

Node *StringConstantNode::Copy()
{
	StringConstantNode *node = new StringConstantNode(m_s.c_str());

	node->ExprNode::Copy(this);
	return node;
}

Sint StringConstantNode::Eval(DataSymbol &ds)
{
	const int REMAINING[] = {0, 3, 2, 1};
	ds.str = m_s;
	ds.type = VAR_STRING;
	int l = m_s.length();
	ds.size = l+REMAINING[l%4];

	return 0;
}

void StringConstantNode::Pass1()
{
	SetSymbol(g_file.AddConstants(m_s));
}

AssignExprNode::AssignExprNode(Node *lval, Node *expr)
{
	m_lval = (LValueNode *)lval;
	m_expr = (ExprNode *)expr;
}

Node *AssignExprNode::Copy()
{
	AssignExprNode *node = new AssignExprNode(m_lval->Copy(), m_expr->Copy());
	node->ExprNode::Copy(this);

	return node;
}

void AssignExprNode::Pass1()
{
	m_lval->AllocSymbol();
	m_symbol = m_lval->GetSymbol();
	m_type = m_symbol->m_type;

	if (m_expr==NULL)	// if it is DefVarNode, this may be NULL
		return;

	// if left value is triple, while expression is constant float
	if (m_expr->GetNodeType()==NUMBEREXPR_NODE && GetType().IsTriple()) {
		NumberExprNode *en = (NumberExprNode *)m_expr;
		NumberExprNode *en1 = (NumberExprNode *)en->Copy(),
			*en2 = (NumberExprNode *)en->Copy();
		TripleNode *tn = new TripleNode(en, en1, en2);
		m_expr = tn;
	}

/*
	if (m_expr->GetNodeType()==PROCCALL_NODE) {
		ProcCallNode *node = (ProcCallNode *)m_expr;
		node->SetType(m_lval->GetType());
	}
*/
	m_expr->SetSymbol(m_lval->GetSymbol());
	m_expr->Pass1();
}

void AssignExprNode::GenAssembly(aofstream &os)
{
	if (m_expr==NULL)
		return;

	if (!m_expr->IsSimple()) {
		m_expr->SetSymbol(m_lval->GetSymbol());
		m_expr->GenAssembly(os);

		return ;
	}
	else {
		Symbol *sym = m_lval->GetSymbol();
		os<<"mov"<<sym->m_type.Char()<<'\t'<<sym->m_name<<", "<< 
			m_expr->GetSymbol()->m_name<<endl;
	}

	return;
}

BinopExprNode::BinopExprNode(Node *expr1, Suint binop, Node *expr2)
{
	m_expr1 = (ExprNode *)expr1;
	m_binop = binop;
	m_expr2 = (ExprNode *)expr2;
}

Node *BinopExprNode::Copy()
{
	BinopExprNode *node = new BinopExprNode(m_expr1->Copy(), m_binop, m_expr2->Copy());
	node->ExprNode::Copy(this);

	return node;
}

void BinopExprNode::Pass1()
{
	m_expr1->Pass1();
	m_expr1->AllocSymbol();

	m_expr2->Pass1();
	m_expr2->AllocSymbol();

	m_type = GetDestType();

	m_expr1->FreeTempSymbol();
	m_expr2->FreeTempSymbol();
}


Type BinopExprNode::GetDestType()
{
	Type sb1 = m_expr1->GetType(), 
		sb2 = m_expr2->GetType();

	if (m_binop=='.' && (sb1.IsVector() || sb1.IsNormal()) &&
		(sb2.IsVector() || sb2.IsNormal()) )
		return Type(VAR_FLOAT);

	if ( (sb1.IsVector() && sb2.IsVector()) ||
		(sb1.IsVector() && sb2.IsFloat()) ||
		(sb1.IsFloat() && sb2.IsVector()) )
		return Type(VAR_VECTOR);
	else if ( (sb1.IsPoint() && sb2.IsVector()) ||
		(sb1.IsVector() && sb2.IsPoint()) )
		return Type(VAR_POINT);
	else if (sb1.IsFloat() && sb2.IsFloat())
		return Type(VAR_FLOAT);
	else if ( (sb1.IsColor() && sb2.IsColor()) ||
		(sb1.IsColor() && sb2.IsFloat()) ||
		(sb1.IsFloat() && sb2.IsColor()) )
		return Type(VAR_COLOR);
	else if ( (sb1.IsPoint() && sb2.IsPoint()) ||
		(sb1.IsPoint() && sb2.IsFloat()) ||
		(sb1.IsFloat() && sb2.IsPoint()) )
		return Type(VAR_VECTOR);
	else {
		g_file.ReportTypeError(this, sb1, sb2);
		return Type(VAR_ERROR);
	}
}

void BinopExprNode::GenAssembly(aofstream &os)
{
	m_expr1->GenAssembly(os);
	m_expr2->GenAssembly(os);

	Symbol *sb1 = m_expr1->GetSymbol(),
		*sb2 = m_expr2->GetSymbol();
	switch (m_binop) {
	case '.':
		os<<"dotv\t"<<m_symbol->m_name<<", "<<
				sb1->m_name<<", "<<sb2->m_name<<endl;
		break;
	case '^':
		os<<"crsv\t"<<m_symbol->m_name<<", "<<
			sb1->m_name<<", "<<sb2->m_name<<endl;
		break;
	case '*':
		if (sb1->m_type.IsSame(sb2->m_type) && sb1->m_type.IsTriple() && sb2->m_type.IsTriple()) {
			os<<"mulvv\t"<<m_symbol->m_name<<", "<<
				sb1->m_name<<", "<<sb2->m_name<<endl;
		}
		else if (sb1->m_type.IsSame(sb2->m_type) && sb1->m_type.IsFloat() && sb2->m_type.IsFloat()) {
			os<<"mulf\t"<<m_symbol->m_name<<", "<<
				sb1->m_name<<", "<<sb2->m_name<<endl;
		}
		else if (sb1->m_type.IsTriple() && sb2->m_type.IsFloat()) {
			os<<"mulv\t"<<m_symbol->m_name<<", "<<
				sb1->m_name<<",	"<<sb2->m_name<<endl;
		}
		else if (sb1->m_type.IsFloat() && sb2->m_type.IsTriple()) {
			os<<"mulv\t"<<m_symbol->m_name<<", "<<
				sb2->m_name<<", "<<sb1->m_name<<endl;
		}
		else {
			g_file.ReportTypeError(this, sb1->m_type, sb2->m_type);
		}
		break;
	case '+':
		os<<"add"<<GetType().Char()<<'\t'<<m_symbol->m_name<<", "<<
				sb1->m_name<<", "<<sb2->m_name<<endl;
		break;
	case '-':
		os<<"sub"<<GetType().Char()<<'\t'<<m_symbol->m_name<<", "<<
				sb1->m_name<<", "<<sb2->m_name<<endl;
		break;
	case '/': 
		os<<"div"<<GetType().Char()<<'\t'<<m_symbol->m_name<<", "<<
			sb1->m_name<<", "<<sb2->m_name<<endl;
		break;
	}
}

IdentNode::IdentNode(const Schar *name)
: LValueNode(name)
{
}

Node *IdentNode::Copy()
{
	IdentNode *node = new IdentNode(m_name.c_str());
	node->LValueNode::Copy(this);

	return node;
}

void IdentNode::AllocSymbol()
{
	if (m_symbol!=NULL)
		return;

	Symbol *sb = g_file.FindVarSymbol(m_name);
	if (sb==NULL)
		g_file.ReportUnresolvedSymbol(this, m_name);
	else {
		m_symbol = sb;
		m_type = m_symbol->m_type;
	}
}

void IdentNode::Pass1()
{
}

IdentArrayNode::IdentArrayNode(const Schar *name, Sfloat val)
: LValueNode(name)
{
	m_offset = (Sint)val;
}

Node *IdentArrayNode::Copy()
{
	IdentArrayNode *node = new IdentArrayNode(m_name.c_str(), m_offset);
	node->LValueNode::Copy(this);

	return node;
}

void IdentArrayNode::AllocSymbol()
{
	Symbol *sb = g_file.FindVarSymbol(m_name);
	if (sb==NULL) {
		g_file.ReportUnresolvedSymbol(this, m_name);
	}
	else {
		m_symbol=sb;
		m_type = m_symbol->m_type;
	}
}

void IdentArrayNode::Pass1()
{
}

ProcCallNode::ProcCallNode(const Schar *name, NodeList *procargs)
{
	m_name = name;
	m_procargs = procargs;
	m_funcsym = NULL;
	m_node=NULL;
}


Node *ProcCallNode::Copy()
{
	ProcCallNode *node = new ProcCallNode(m_name.c_str(), m_procargs->Copy());
	node->m_funcsym = m_funcsym;
	node->ExprNode::Copy(this);

	return node;
}

Sint ProcCallNode::Eval(DataSymbol &ds)
{
	GetFuncSymbol();
	if (m_funcsym->IsBuiltinFunc()) {
		DataSymbol *dsa=NULL;
		if (m_procargs!=NULL) {
			dsa = new DataSymbol[m_procargs->size()];
			NodeList::iterator it = m_procargs->begin();
			int i=0;
			for (; it<m_procargs->end(); ++it, ++i) {
				ExprNode *node = (ExprNode *)(*it);
				if (node->Eval(dsa[i])<0) {
					delete [] dsa;
					return -1;
				}
			}
		}

		int v = m_funcsym->u.addr;

		switch (v) {
		case 0:
			ds.flt[0] = (dsa[0].flt[0])*M_PI/180;
			ds.type = Type(VAR_FLOAT);
			ds.size = ds.type.Size();
			break;
		}

		delete [] dsa;
	}

	return 0;
}

void ProcCallNode::AllocSymbol()
{
	ExprNode::AllocSymbol();

	if (m_node!=NULL)
		m_node->SetSymbol(m_symbol);
}

void ProcCallNode::SetSymbol(Symbol *sym)
{
	m_symbol = sym; m_type = sym->m_type;

	if (m_node!=NULL)
		m_node->SetSymbol(m_symbol);
}

string ProcCallNode::GetMangledName(Type rt)
{
	string fullname = m_name;

	fullname += '_';
	fullname += rt.FuncChar();
	if (m_procargs==NULL) {
		fullname += '_';
	}
	else {
		Suint n = m_procargs->size();
		for (Suint i=0; i<n; ++i) {
			ExprNode *node = (ExprNode *)(*m_procargs)[i];
			fullname += node->GetType().FuncChar();
		}
	}

	return fullname;
}

void ProcCallNode::GetFuncSymbol()
{
	if (m_type.IsUnknown()) {
		Type rt;
		rt.m_type = VAR_FLOAT;
		while (rt.m_type<=VAR_VOID) {
			string fullname = GetMangledName(rt);
			m_funcsym = g_file.FindFuncSymbol(fullname);
			if (m_funcsym!=NULL) {
				m_type = rt;
				break;
			}
			rt.m_type++;
		}
	}
	else {
		string fullname = GetMangledName(m_type);
		m_funcsym = g_file.FindFuncSymbol(fullname);
	}
}

void ProcCallNode::Pass1()
{
	if (m_procargs!=NULL) {
		NodeList::iterator it = m_procargs->begin();
		for (; it<m_procargs->end(); ++it) {
			ExprNode *arg = (ExprNode *)(*it);
			arg->Pass1();
			arg->AllocSymbol();
		}
	}

	GetFuncSymbol();

	if (m_funcsym ==NULL) {
		g_file.ReportUnresolvedSymbol(this, m_name);
		return;
	}

	if (m_funcsym->IsFunc()) {
		m_node = (FuncDefNode *)m_funcsym->u.ptr->Copy();
		if (m_scope==NULL)
			m_scope = new Scope;
		if (m_scope->m_parent==NULL)
			m_scope->m_parent = g_file.GetCurrentScope();

		// get actual parameter
		Symbol **sb=NULL;
		if (m_procargs!=NULL) {
			NodeList::iterator it = m_procargs->begin();
			sb = new Symbol*[m_procargs->size()];
			for (int i=0; it<m_procargs->end(); ++it, ++i) {
				ExprNode *arg = (ExprNode *)(*it);
				sb[i] = arg->GetSymbol();
			}

		}
		g_file.BeginScope(m_scope, true);
		// create alias name for the formal parameter
		if (m_procargs!=NULL) {
			int n = m_procargs->size();
			for (int i=0; i<n; ++i) 
				m_node->SetArg(i, sb[i]);
		}
		m_node->Pass1();
		g_file.EndScope(true);

		// set return value for m_node; m_symbol can be NULL if the function is void
		if (m_symbol!=NULL)
			m_node->SetSymbol(m_symbol);
	}

	if (m_procargs!=NULL) {
		NodeList::iterator it;
		for (it=m_procargs->begin(); it!=m_procargs->end(); ++it) {
			ExprNode *arg = (ExprNode *)(*it);
			arg->FreeTempSymbol();
		}
	}
}

void ProcCallNode::GenAssembly(aofstream &os)
{
	int i, n;
	if (m_procargs!=NULL) {
		n = m_procargs->size();
		for (i=0; i<n; ++i) {
			ExprNode *node = (ExprNode *)(*m_procargs)[i];
			node->GenAssembly(os);
		}
	}

	if (m_funcsym->IsFunc()) {
		m_node->GenAssembly(os);
		return;
	}
	else {
		if (m_procargs!=NULL) {
			// gencode
			n = m_procargs->size();
			for (i=0; i<n; ++i) {
				ExprNode *node = (ExprNode *)(*m_procargs)[i];
				Symbol *sym = node->GetSymbol();

				os<<"push"<<sym->m_type.Char()<<'\t'<<sym->m_name<<endl;
			}
		}

		os<<"ccall\t"<<m_funcsym->m_name<<endl;

		if (m_symbol!=NULL) {
			os<<"pop"<<m_symbol->m_type.Char()<<'\t'<<m_symbol->m_name<<endl;
		}
	}
}

UnaryExprNode::UnaryExprNode(Suint unaryop, Node *expr)
{
	m_unaryop = unaryop;
	m_expr = (ExprNode *)expr;
}

Node *UnaryExprNode::Copy()
{
	UnaryExprNode *node = new UnaryExprNode(m_unaryop, m_expr->Copy());
	node->ExprNode::Copy(this);

	return node;
}

Sint UnaryExprNode::Eval(DataSymbol &ds)
{
	if (m_expr->Eval(ds)<0)
		return -1;

	if (ds.type.IsFloat())
		ds.flt[0] = -ds.flt[0];
	else if (ds.type.IsTriple()) {
		ds.flt[0] = -ds.flt[0];
		ds.flt[1] = -ds.flt[1];
		ds.flt[2] = -ds.flt[2];
	}
	else
		return -1;

	return 0;
}

void UnaryExprNode::Pass1()
{
	m_expr->Pass1();
	m_type = m_expr->GetType();
	m_expr->AllocSymbol();
	m_expr->FreeTempSymbol();
}

void UnaryExprNode::GenAssembly(aofstream &os)
{
	m_expr->GenAssembly(os);

	os<<"neg"<<GetType().Char()<<'\t'<<m_symbol->m_name<<endl;
}

QuestionExprNode::QuestionExprNode(Node *rel, Node *expr1, Node *expr2)
{
	m_rel = (RelationNode *)rel;
	m_expr1 = (ExprNode *)expr1;
	m_expr2 = (ExprNode *)expr2;
}

Node *QuestionExprNode::Copy()
{
	QuestionExprNode *node = new QuestionExprNode(m_rel->Copy(), m_expr1->Copy(), m_expr2->Copy());
	node->ExprNode::Copy(this);

	return node;
}

void QuestionExprNode::Pass1()
{
	m_rel->Pass1();
	m_expr1->Pass1();
	m_expr1->AllocSymbol();

	m_expr2->Pass1();
	m_expr2->AllocSymbol();

	m_label1 = g_file.AddLabel();
	m_label2 = g_file.AddLabel();

	m_expr1->FreeTempSymbol();
	m_expr2->FreeTempSymbol();
}

void QuestionExprNode::GenAssembly(aofstream &os)
{
	m_rel->GenAssembly(os);
	os<<"jf\t"<<m_label1<<endl;
	m_expr1->GenAssembly(os);
	os<<"mov"<<GetType().Char()<<'\t'<<m_symbol->m_name<<", "<<m_expr1->GetSymbol()->m_name<<endl;
	os<<"jmp\t"<<m_label2<<endl;
	os<<m_label1->m_name<<":"<<endl;
	m_expr2->GenAssembly(os);
	os<<"mov"<<GetType().Char()<<'\t'<<m_symbol->m_name<<", "<<m_expr2->GetSymbol()->m_name<<endl;
	os<<m_label2->m_name<<":"<<endl;
}


DefVarNode::DefVarNode(Node *lval, Node *din)
: AssignExprNode(lval, din)
{
	m_name = ((LValueNode *)lval)->GetName();
	m_type = ((LValueNode *)lval)->GetType();

	m_isparam = false;
	m_isfunc = false;
}

Node *DefVarNode::Copy()
{
	DefVarNode *node;
	if (m_expr)
		node = new DefVarNode(m_lval->Copy(), m_expr->Copy());
	else
		node = new DefVarNode(m_lval->Copy(), NULL);
	node->m_isparam = m_isparam;
	node->m_isfunc = m_isfunc;

	node->ExprNode::Copy(this);
	
	return node;
}

void DefVarNode::Pass1()
{
	bool isfunc = g_file.IsInFunc();
	if (isfunc)
		if (m_isparam)
			PassFuncParam();
		else
			PassFuncVar();
	else 
		if (m_isparam)
			PassShaderParam();
		else
			PassShaderVar();
}

void DefVarNode::PassFuncParam()
{
	// this is processed by another SetArg in ProcCallNode
}

void DefVarNode::PassFuncVar()
{
	if (m_lval && m_lval->GetNodeType()==IDENT_NODE) {
		LValueNode *d = (LValueNode *)m_lval;
		Symbol *sb = g_file.AllocTempVarSymbol(m_type);
		g_file.AddAliasSymbol(d->GetName(), sb);
		SetSymbol(sb);
	}
	else
		ASSERT(0);

	AssignExprNode::Pass1();
}

void DefVarNode::PassShaderParam()
{
	if (m_lval && m_lval->GetNodeType()==IDENT_NODE) {
		LValueNode *d = (LValueNode *)m_lval;
		m_symbol = g_file.AddVarSymbol(d->GetName(), true, m_type);
	}
	else 
		ASSERT(0);

	if (m_expr->GetNodeType()==NUMBEREXPR_NODE && GetType().IsTriple()) {
		NumberExprNode *en = (NumberExprNode *)m_expr;
		NumberExprNode *en1 = (NumberExprNode *)en->Copy(),
			*en2 = (NumberExprNode *)en->Copy();
		TripleNode *tn = new TripleNode(en, en1, en2);
		m_expr = tn;
	}

	if (m_expr->GetNodeType()==TYPECASTEXPR_NODE)
		m_expr->Pass1();
}

void DefVarNode::PassShaderVar()
{
	if (m_lval && m_lval->GetNodeType()==IDENT_NODE) {
		LValueNode *d = (LValueNode *)m_lval;
		g_file.AddVarSymbol(d->GetName(), m_isparam, m_type);
	}
	else
		ASSERT(0);

	AssignExprNode::Pass1();
}

void DefVarNode::GenAssembly(aofstream &os)
{
	bool isfunc = g_file.IsInFunc();
	if (isfunc)
		if (m_isparam)
			GenFuncParam(os);
		else
			GenFuncVar(os);
	else 
		if (m_isparam)
			GenShaderParam(os);
		else
			GenShaderVar(os);
}

void DefVarNode::GenFuncParam(aofstream &os)
{
}

void DefVarNode::GenFuncVar(aofstream &os)
{
	AssignExprNode::GenAssembly(os);
}

void DefVarNode::GenShaderParam(aofstream &os)
{
	if (m_expr->GetNodeType()==TYPECASTEXPR_NODE) {
		TypecastExprNode *tn = (TypecastExprNode *)m_expr;
		// the argument and return value are both the parameter
		tn->SetSymbol(m_symbol);
		tn->m_expr->SetSymbol(m_symbol);
		tn->GenAssembly(os);
	}
}

void DefVarNode::GenShaderVar(aofstream &os)
{
	AssignExprNode::GenAssembly(os);
}

Typecast::Typecast(Suint t)
{
	m_type = t;
	m_space = "";
}

Node *Typecast::Copy()
{
	Typecast *node = new Typecast(m_type.m_type);
	node->m_space = m_space;
	node->Node::Copy(this);

	return node;
}

Typecast::Typecast(Suint t, const Schar *space)
{
	m_type = t;
	m_space = space;
}

TypecastExprNode::TypecastExprNode(Node *tc, Node *expr)
{
	m_tc = (Typecast *)tc;
	m_expr = (ExprNode *)expr;
	m_type = m_tc->m_type;
}

Node *TypecastExprNode::Copy()
{
	TypecastExprNode *node = new TypecastExprNode(m_tc->Copy(), m_expr->Copy());
	node->ExprNode::Copy(this);

	return node;
}

Sint TypecastExprNode::Eval(DataSymbol &ds)
{
	return m_expr->Eval(ds);
}

void TypecastExprNode::Pass1()
{
	m_type = m_tc->m_type;
	if (m_tc->m_space!="") {
		m_sfrom = g_file.AddConstants(m_tc->m_space);
		if (m_type.IsPoint())
			m_sto = g_file.AddConstants("current");
		else if (m_type.IsColor())
			m_sto = g_file.AddConstants("rgb");
		else
			ASSERT(0);
	}

	if (m_expr->GetNodeType()==PROCCALL_NODE) {
		ProcCallNode *node = (ProcCallNode *)m_expr;
		node->SetType(m_tc->m_type);
	}
	m_expr->Pass1();
	m_expr->AllocSymbol();
}

void TypecastExprNode::GenAssembly(aofstream &os)
{
	if (m_tc->m_space=="") {
		m_expr->GenAssembly(os);
		if (m_expr->GetType().IsFloat() && m_type.IsTriple()) {
			string ftname = m_expr->GetSymbol()->m_name;
			os<<"pushv\t"<<m_symbol->m_name<<endl;
			os<<"pushf\t"<<ftname<<endl;
			os<<"ccall\tsetxcomp__vf"<<endl;
			os<<"pushv\t"<<m_symbol->m_name<<endl;
			os<<"pushf\t"<<ftname<<endl;
			os<<"ccall\tsetycomp__vf"<<endl;
			os<<"pushv\t"<<m_symbol->m_name<<endl;
			os<<"pushf\t"<<ftname<<endl;
			os<<"ccall\tsetzcomp__vf"<<endl;
		}
		else if (m_expr->GetType().IsTriple() && m_type.IsTriple()) {
			os<<"movv\t"<<m_symbol->m_name<<", "<<m_expr->GetSymbol()->m_name<<endl;
		}
		else if (m_expr->GetType().IsFloat() && m_type.IsFloat()) {
			os<<"movf\t"<<m_symbol->m_name<<", "<<m_expr->GetSymbol()->m_name<<endl;
		}
		else
			// TODO
			ASSERT(0);
	}
	else {
		m_expr->GenAssembly(os);

		if (m_expr->GetType().IsPoint()) {
			os<<"pushs\t"<<m_sfrom->m_name<<endl<<"pushs\t"<<m_sto->m_name<<endl<<
				"pushv\t"<<m_expr->GetSymbol()->m_name<<endl<<"ccall transform_pssp"<<endl<<
				"popv\t"<<m_symbol->m_name<<endl;
		}
		else if (m_expr->GetType().IsColor()) {
			if (m_sfrom->m_name.compare(m_sto->m_name)!=0) {
				os<<"pushs\t"<<m_sfrom->m_name<<endl<<"pushs\t"<<m_sto->m_name<<endl<<
					"pushv\t"<<m_expr->GetSymbol()->m_name<<endl<<"ccall transform_cssc"<<endl<<
					"popv\t"<<m_symbol->m_name<<endl;
			}
		}
		else {
			ASSERT(0);
			// TODO
		}
	}

}

