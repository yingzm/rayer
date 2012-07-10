#include "DefNode.hxx"
#include "slc_tab.hxx"
#include "StmtNode.hxx"

#include "ExprNode.hxx"
#include "FileNode.hxx"

shader_t GetShaderType(Suint32 type)
{
	switch (type) {
	case LIGHT:
		return SHADER_LIGHT;
	case SURFACE:
		return SHADER_SURFACE;
	case DISPLACEMENT:
		return SHADER_DISPLACEMENT;
	case VOLUME:
		return SHADER_VOLUME;
	case IMAGER:
		return SHADER_IMAGER;		
	}

	return SHADER_SURFACE;
}

Type GetVariableType(Sint type)
{
	switch (type) {
	case VAR_FLOAT:
		return Type(VAR_FLOAT);
	case VAR_POINT:
		return Type(VAR_POINT);
	case VAR_VECTOR:
		return Type(VAR_VECTOR);
	case VAR_NORMAL:
		return Type(VAR_NORMAL);
	case VAR_COLOR:
		return Type(VAR_COLOR);
	case VAR_MATRIX:
		return Type(VAR_MATRIX);
	case VAR_STRING:
		return Type(VAR_STRING);
	case -1:
		return Type(VAR_VOID);
	}

	return Type(VAR_ERROR);
}

ShaderDefNode::ShaderDefNode(Suint32 type, const Schar *name, vector<Node *> *params,
		vector<Node *> *stmts)
{
	m_type = (shader_t) type;
	m_name = name;
	m_params = params;
	if (params!=NULL) {
		// tag the DefVarNode as param DefVarNode
		int n=params->size();
		for (int i=0; i<n; ++i) {
			DefVarNode *node = (DefVarNode *)(*params)[i];
			node->SetParam();
		}
	}
	m_stmts = stmts;

	m_nextdatavar = 0;
	m_nextlocal = 0;
	m_label = 0;
	m_nextparam = 0;
}

void ShaderDefNode::Pass1()
{
	m_scope = new Scope;
	m_scope->m_parent = g_file.GetCurrentScope();
	m_scope->m_da2parent = true;
	g_file.BeginScope(m_scope, false);
	if (m_params!=NULL) {
		Suint n = (Suint) m_params->size();
		for (Suint i=0; i<n; ++i) {
			Node *node = (*m_params)[i];
			node->Pass1();
		}
	}

	if (m_stmts!=NULL) {
		Suint n = (Suint) m_stmts->size();
		for (Suint i=0; i<n; ++i) {
			Node *node = (*m_stmts)[i];
			node->Pass1();
		}
	}
	g_file.EndScope(false);
}

void ShaderDefNode::GenAssembly(aofstream &os)
{
	os<<".info"<<endl<<GetShaderTypeStr(m_type)<<"\t"<<m_name<<endl<<endl;

	if (m_params!=NULL) {
		os<<".params"<<endl;
		Suint n = (Suint)m_params->size();
		for (Suint i=0; i<n; ++i) {
			DefVarNode *node = (DefVarNode *)(*m_params)[i];
			DataSymbol ds;
			if (node->m_expr->Eval(ds)<0) {
				g_file.ReportNonconstant(this);
				return;
			}

			os<<node->GetType().GetStr()<<"\t"<<
				node->m_name<<"\t";
			os<<&ds<<endl;
		}
	}

	if (m_datasymbols.size()!=0) {
		os<<".data\n";
		int n = m_datasymbols.size();
		for (int i=0; i<n; ++i) {
			DataSymbol *ds = m_datasymbols[i];
			os<<ds->type.GetStr()<<'\t'<<ds->sym->m_name<<'\t';
			os<<ds<<endl;
		}
	}

	os<<".code"<<endl;

	Suint i, n;
	if (m_nextlocal!=0) {
		os<<"addr\tlstop, "<<m_nextlocal<<endl;
	}
	if (m_params) {
		n = (Suint)m_params->size();
		for (i=0; i<n; ++i) {
			DefVarNode *node = (DefVarNode *)(*m_params)[i];
			node->GenAssembly(os);
		}
	}

	n = (Suint)m_localvars.size();
	for (i=0; i<n; ++i) {
		Symbol *sb = m_localvars[i];
		os<<"declare\t"<<sb->m_type.GetStr()<<'\t'<<sb->m_name<<
			"\t["<<(sb->u.addr&0xffff)<<']'<<endl;
	}

	// some params need special code
	if (m_stmts!=NULL) {
		Suint n = (Suint) m_stmts->size();
		for (Suint i=0; i<n; ++i) {
			Node *node = (*m_stmts)[i];
			node->GenAssembly(os);
		}
	}
	if (m_nextlocal!=0) {
		os<<"subr\tlstop, "<<m_nextlocal<<endl;
	}

	os<<"end\n";
}

Symbol *ShaderDefNode::AddLabel()
{
	Schar buf[256];
	sprintf(buf, "_label%d", m_label);
	m_label++;
	Symbol *sb = new Symbol;
	sb->m_name = buf;
	sb->m_stype= Symbol::SYMBOL_LABEL;
	sb->u.addr = 0;

	return sb;
}

Symbol *ShaderDefNode::AddTempSymbol(Type tp)
{
	Symbol *sb = new Symbol;
	Schar buf[256];
	sprintf(buf, "_T%d", m_nextlocal);
	sb->m_name = buf;
	sb->m_stype = Symbol::SYMBOL_TEMPVAR;
	sb->m_type = tp;
	sb->u.addr = LS_ADDR(m_nextlocal);
	m_scope->AddSymbol(sb);
	printf("Add %s\n", buf);

	m_localvars.push_back(sb);

	m_nextlocal += tp.Size();
	
	return sb;
}

Symbol *ShaderDefNode::FindFreeSymbol(Type tp)
{
	printf("Find...%s...", tp.GetStr());
	vector<Symbol *>::iterator it = m_freevars.begin();
	for (; it<m_freevars.end(); ++it) {
		if ((*it)->m_type.IsSame(tp)) {
			printf("%s\n", (*it)->m_name.c_str());
			Symbol *sb = *it;
			m_freevars.erase(it);
			//
			printf("Symbols...\t");
			vector<Symbol *>::iterator it = m_freevars.begin();
			for (; it<m_freevars.end(); ++it) {
				printf("%s\t", (*it)->m_name.c_str());
			}
			printf("\n");
			return sb;
		}
	}

	return NULL;
}

void ShaderDefNode::FreeLocalSymbol(Symbol *sb)
{
	vector<Symbol *>::iterator it = m_localvars.begin();
	for (; it<m_localvars.end(); ++it) {
		if (*it==sb) {
			printf("Free %s\n", sb->m_name.c_str());
			it = m_freevars.begin();
			for (; it<m_freevars.end(); ++it) {
				Symbol *sb1 = *it;
				if (sb1->m_name.compare(sb->m_name)==0) {
					return;
				}
			}
			m_freevars.push_back(sb);
			return;
		}
	}
}

Symbol *ShaderDefNode::AddConstantf(Sfloat v)
{
	Symbol *sb = new Symbol;
	Schar buf[256];
	sprintf(buf, "_Df%d", m_nextdatavar);
	sb->m_name = string(buf);
	sb->m_stype = Symbol::SYMBOL_CONSTANT;
	sb->m_type = VAR_FLOAT;
	sb->u.addr = DATA_ADDR(m_nextdatavar);

	DataSymbol *ds = new DataSymbol;
	ds->sym = sb;
	ds->size = sb->m_type.Size();
	ds->flt[0] = v;
	ds->type = VAR_FLOAT;
	m_datasymbols.push_back(ds);
	m_nextdatavar += ds->size;

	return sb;
}

Symbol *ShaderDefNode::FindConstantf(Sfloat v)
{
	int n = m_datasymbols.size();
	for (int i=0; i<n; ++i) {
		DataSymbol *ds = m_datasymbols[i];
		if (ds->type.IsFloat() && ds->flt[0] == v) {
			return ds->sym;
		}
	}

	return NULL;
}

Symbol *ShaderDefNode::AddConstantv(Sfloat v0, Sfloat v1, Sfloat v2)
{
	Symbol *sb = new Symbol;
	Schar buf[256];
	sprintf(buf, "_Dv%d", m_nextdatavar);
	sb->m_name = string(buf);
	sb->m_stype = Symbol::SYMBOL_CONSTANT;
	sb->m_type = VAR_VECTOR;
	sb->u.addr = DATA_ADDR(m_nextdatavar);
	m_scope->AddSymbol(sb);

	DataSymbol *ds = new DataSymbol;
	ds->sym = sb;
	ds->size = sb->m_type.Size();
	ds->flt[0] = v0;
	ds->flt[1] = v1;
	ds->flt[2] = v2;
	ds->type = VAR_VECTOR;
	m_datasymbols.push_back(ds);
	m_nextdatavar += ds->size;

	return sb;
}

Symbol *ShaderDefNode::FindConstantv(Sfloat v0, Sfloat v1, Sfloat v2)
{
	int n = m_datasymbols.size();
	for (int i=0; i<n; ++i) {
		DataSymbol *ds = m_datasymbols[i];
		if (ds->type.IsTriple() && (ds->flt[0]==v0) &&
			(ds->flt[1]==v1) && (ds->flt[2]==v2)) {
			return ds->sym;
		}
	}

	return NULL;
}

Symbol *ShaderDefNode::AddConstants(const string &s)
{
	Symbol *sb = new Symbol;
	Schar buf[256];
	sprintf(buf, "_Dv%d", m_nextdatavar);
	sb->m_name = string(buf);
	sb->m_stype = Symbol::SYMBOL_CONSTANT;
	sb->m_type = VAR_STRING;
	sb->u.addr = DATA_ADDR(m_nextdatavar);
	m_scope->AddSymbol(sb);

	DataSymbol *ds = new DataSymbol;
	ds->sym = sb;
	const int REMAINING[] = {0, 3, 2, 1};
	int l = s.length();
	ds->size = l+REMAINING[l%4];
	ds->str = s;
	ds->type = VAR_STRING;
	m_datasymbols.push_back(ds);
	m_nextdatavar += ds->size;

	return sb;
}

Symbol *ShaderDefNode::FindConstants(const string &s)
{
	int n = m_datasymbols.size();
	for (int i=0; i<n; ++i) {
		DataSymbol *ds = m_datasymbols[i];
		if (ds->type.IsString() && ds->str.compare(s)==0) {
			return ds->sym;
		}
	}

	return NULL;
}

Symbol *ShaderDefNode::AddParamSymbol(const string &s, Type type)
{
	Symbol *sb = new Symbol;

	sb->m_name = s;
	sb->m_stype = Symbol::SYMBOL_PARAM;
	sb->m_type = type;
	sb->u.addr = LP_ADDR(m_nextparam);
	m_nextparam += type.Size();

	return sb;
}

Symbol *ShaderDefNode::AddLocalSymbol(const string &s, Type type)
{
	Symbol *sb = new Symbol;

	sb->m_name = s;
	sb->m_stype = Symbol::SYMBOL_LOCALVAR;
	sb->m_type = type;
	sb->u.addr = LS_ADDR(m_nextlocal);
	m_nextlocal += type.Size();
	m_localvars.push_back(sb);

	return sb;
}



string FuncDefNode::GetMangledName(const string &name)
{
	string fullname = name;
	fullname += '_';
	fullname += m_rttype.FuncChar();
	if (m_params) {
		NodeList::iterator it = m_params->begin();
		for (; it<m_params->end(); ++it) {
			DefVarNode *node = (DefVarNode *)(*it);
			fullname+=node->GetType().FuncChar();
		}
	}
	else
		fullname += '_';

	return fullname;
}

FuncDefNode::FuncDefNode(Sint rttype, const Schar *name, NodeList *params, NodeList *stmts)
{
	m_rttype = GetVariableType(rttype);
	m_params = params;
	m_stmts = stmts;
	m_name = GetMangledName(name);
}

FuncDefNode::FuncDefNode(Type rttype, const Schar *name, NodeList *params, NodeList *stmts)
{
	m_rttype = rttype;
	m_name = name;
	m_params = params;
	m_stmts = stmts;
}


Node *FuncDefNode::Copy()
{
	FuncDefNode *node = new FuncDefNode(m_rttype, m_name.c_str(), m_params->Copy(), m_stmts->Copy());
	node->DefNode::Copy(this);

	return node;
}

void FuncDefNode::Pass1()
{
	int n, i;
/*	n = m_params->size();
	for (i=0; i<n; ++i) {
		DefVarNode *node = (DefVarNode *)(*m_params)[i];
		node->Pass1();
	}
*/
	n = m_stmts->size();
	for (i=0; i<n; ++i) {
		StmtNode *node = (StmtNode *)(*m_stmts)[i];
		node->Pass1();
	}
}

void FuncDefNode::SetSymbol(Symbol *sym)
{
	m_symbol = sym;

	NodeList::iterator it;

	for (it=m_stmts->begin(); it!=m_stmts->end(); ++it) {
		Node *node = (*it);
		if (node->GetNodeType()==RETURNSTMT_NODE) {
			ReturnStmtNode *rsn = (ReturnStmtNode *)node;
			rsn->SetSymbol(m_symbol);
		}
	}
}


void FuncDefNode::GenAssembly(aofstream &os)
{
	int n, i;
	n = m_stmts->size();
	for (i=0; i<n; ++i) {
		StmtNode *node = (StmtNode *)(*m_stmts)[i];
		node->GenAssembly(os);
	}
}

void FuncDefNode::SetArg(int i, Symbol *s)
{
	DefVarNode *node = (DefVarNode *)(*m_params)[i];
	
	Symbol *sb = new Symbol;
	sb->m_name = s->m_name;
	sb->m_stype = Symbol::SYMBOL_PARAM;
	sb->m_type = s->m_type;
	sb->u.addr = s->u.addr;
	g_file.AddAliasSymbol(node->m_name, sb);
}