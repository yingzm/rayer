#include "FileNode.hxx"

#include "Symbol.hxx"
#include "DefNode.hxx"

FileNode g_file;

FileNode::FileNode()
{	
	for (int i=0; ; ++i) {
		const char *name = str_cfuncs[i];
		if (strcmp(name, "null")==0)
			break;
		Symbol *sb = new Symbol;
		sb->m_name = name;
		sb->m_stype = Symbol::SYMBOL_BUILTINFUNC;
		sb->u.addr  = i;
		this->m_sysfuncsymbols[sb->m_name] = sb;
	}

	m_numerror = m_numwarning = 0;
	m_funccnt=0;
}

void FileNode::AddFuncDef(DefNode *node)
{
	FuncDefNode *fn = (FuncDefNode *)(node);
	Symbol *sb = new Symbol;
	sb->m_name = fn->m_name;
	sb->m_type = fn->m_rttype;
	sb->m_stype = Symbol::SYMBOL_FUNC;
	sb->u.ptr = fn;
	m_globalscope.AddSymbol(sb);
}

void FileNode::AddShaderDef(DefNode *node)
{
	m_shaderdef = (ShaderDefNode *)node;
}
	
void FileNode::Pass1()
{
	m_curscope = new Scope;
	m_curscope->m_parent = NULL;
	
	AddShaderEnv(m_shaderdef->m_type);

	m_shaderdef->Pass1();

}

void FileNode::Pass2()
{
}

void FileNode::BeginScope(Scope *sc, bool addcnt)
{
	m_scopestack.push(m_curscope);
	m_curscope = sc;
	if (addcnt)
		m_funccnt++;
}

void FileNode::EndScope(bool subcnt)
{
	map<string, Symbol *>::iterator it = m_curscope->m_symbols.begin();
	for (; it!=m_curscope->m_symbols.end(); ++it) {
		Symbol *sb = it->second;
		if (sb->IsTemp() || sb->IsLocal())
			m_shaderdef->FreeLocalSymbol(sb);
	}
	m_curscope = m_scopestack.top();
	m_scopestack.pop();
	if (subcnt)
		m_funccnt--;
}

Sint FileNode::GenAssembly(const Schar *assem)
{
	if (m_numerror>0)
		return -1;

	Schar name[256];
	strcpy(name, assem);
	strcat(name, "a");

	aofstream os;
	os.open(name ,ios_base::out);

	m_shaderdef->GenAssembly(os);

	os.close();
	return 0;
}
	
	
Sint FileNode::AllocNumber(Sfloat v)
{
	return 0;
}

Sint FileNode::AllocString(const string &s)
{
	return 0;
}

Sint FileNode::AllocTriple(Sfloat v0, Sfloat v1, Sfloat v2)
{
	return 0;
}

Sint FileNode::AllocSixteen(Sfloat v0, Sfloat v1, Sfloat v2, Sfloat v3, 
		Sfloat v4, Sfloat v5, Sfloat v6, Sfloat v7, 
		Sfloat v8, Sfloat v9, Sfloat v10, Sfloat v11, 
		Sfloat v12, Sfloat v13, Sfloat v14, Sfloat v15)
{
	return 0;
}

	/*
	 * Symbol Management
	 */		
Symbol *FileNode::FindFuncSymbol(const string &s)
{
	map<string, Symbol *>::iterator it = m_sysfuncsymbols.find(s);
	if (it==m_sysfuncsymbols.end())  {
		Symbol *sb = m_curscope->FindSymbol(s, false);
		if (sb==NULL)
			sb = m_globalscope.FindSymbol(s, false);
		return sb;
	}
	else {
		return it->second;
	}
}

Symbol *FileNode::FindVarSymbol(const string &s, bool isextern)
{
	Symbol *sym = m_curscope->FindSymbol(s, isextern);
	return sym;
}

Symbol *FileNode::AddVarSymbol(const string &s, bool isparam, Type type)
{
	if (type.IsExtern()) {
		Symbol *sb = m_curscope->FindSymbol(s, true);
		m_curscope->AddSymbol(sb);
		return sb;
	}
	Symbol *sb;
	if (isparam) {
		sb = m_shaderdef->AddParamSymbol(s, type);
	}
	else {
		sb = m_shaderdef->AddLocalSymbol(s, type);
	}

	m_curscope->AddSymbol(sb);
	return sb;
}

Symbol *FileNode::AddFuncSymbol(const string &s, FuncDefNode *fdn)
{
	return NULL;
}

Symbol *FileNode::AddLabel()
{
	Symbol *sb = m_shaderdef->AddLabel();

	return sb;
}

Symbol *FileNode::AllocTempVarSymbol(Type vt)
{
	// first find some vacant in freesymbols
	Symbol *sb = m_shaderdef->FindFreeSymbol(vt);
	if (sb!=NULL)
		return sb;

	return m_shaderdef->AddTempSymbol(vt);
}

void FileNode::FreeTempSymbol(Symbol *sb)
{
	m_shaderdef->FreeLocalSymbol(sb);
}

void FileNode::ReportUnresolvedSymbol(Node *nd, const string &name)
{
	printf("%s:%d> Unresolved symbol %s\n", nd->m_file.c_str(), nd->m_linenum,
		name.c_str());
	++m_numerror;
}

void FileNode::ReportTypeError(Node *nd, Type sb1, Type sb2)
{
	printf("%s:%d> Incompatible type of %s and %s\n", nd->m_file.c_str(),
		nd->m_linenum, sb1.GetStr(), sb2.GetStr());

	++m_numerror;
}

void FileNode::ReportNonconstant(Node *nd)
{
	printf("%s:%d> Nonconstant value\n", nd->m_file.c_str(),
		nd->m_linenum);

	++m_numerror;
}

void FileNode::AddShaderEnv(shader_t st)
{
	int i, n;
	Symbol *sb;
	int off = 0;
	const EnvDesc *desc;
	switch (st) {
	case SHADER_SURFACE:
		n = sizeof(desc_surfaceenv)/sizeof(EnvDesc);
		desc = desc_surfaceenv;
		break;
	case SHADER_LIGHT:
		n = sizeof(desc_lightenv)/sizeof(EnvDesc);
		desc = desc_lightenv;
		break;
	case SHADER_DISPLACEMENT:
		n = sizeof(desc_displacementenv)/sizeof(EnvDesc);
		desc = desc_displacementenv;
		break;
	case SHADER_VOLUME:
		n = sizeof(desc_volumeenv)/sizeof(EnvDesc);
		desc = desc_volumeenv;
		break;
	case SHADER_IMAGER:
		n = sizeof(desc_imagerenv)/sizeof(EnvDesc);
		desc = desc_imagerenv;
		break;
	}

	for (i=0; i<n; ++i) {
		sb = new Symbol;
		const EnvDesc &ed = desc[i];

		sb->m_name = ed.name;
		sb->m_stype = Symbol::SYMBOL_ENV;
		sb->m_type = ed.val;
		sb->u.addr = LE_ADDR(off);
		off += sb->m_type.Size();

		GetCurrentScope()->AddSymbol(sb);
	}
}

Symbol *FileNode::AddConstantf(Sfloat v)
{
	Symbol *sb = m_shaderdef->FindConstantf(v);
	if (sb!=NULL)
		return sb;

	sb = m_shaderdef->AddConstantf(v);

	return sb;
}


Symbol *FileNode::AddConstantv(Sfloat v0, Sfloat v1, Sfloat v2)
{
	Symbol *sb = m_shaderdef->FindConstantv(v0, v1, v2);
	if (sb!=NULL)
		return sb;
	
	sb = m_shaderdef->AddConstantv(v0, v1, v2);

	return sb;
}

Symbol *FileNode::AddConstantm(Sfloat *v)
{
	return NULL;
}

Symbol *FileNode::AddConstants(const string &s)
{
	Symbol *sb = m_shaderdef->FindConstants(s);
	if (sb!=NULL)
		return sb;
	
	sb = m_shaderdef->AddConstants(s);

	return sb;
}

void FileNode::PushBreakLabel(Symbol *s)
{
	m_breakstack.push_back(s);
}

void FileNode::PopBreakLabel()
{
	m_breakstack.pop_back();
}

Symbol *FileNode::GetBreakLabel(int i)
{
	if (i>=m_breakstack.size() || i<1)
		return NULL;

	return m_breakstack[m_breakstack.size()-i];
}

void FileNode::PushContinueLabel(Symbol *s)
{
	m_continuestack.push_back(s);
}

void FileNode::PopContinueLabel()
{
	m_continuestack.pop_back();
}

Symbol *FileNode::GetContinueLabel(int i)
{
	if (i>=m_continuestack.size() || i<1)
		return NULL;

	return m_continuestack[m_continuestack.size()-i];
}