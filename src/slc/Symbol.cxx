#include "Symbol.hxx"

aofstream &operator<<(aofstream &os, DataSymbol *ds)
{
	Type tp = ds->type;
	if (tp.IsFloat()) {
		os<<ds->flt[0];
	}
	else if (tp.IsVector()) {
		os<<'['<<ds->flt[0]<<' '<<ds->flt[1]<<' '<<ds->flt[2]<<']';
	}
	else if (tp.IsMatrix()) {
		os<<'['<<ds->flt[0]<<' '<<ds->flt[1]<<' '<<ds->flt[2]<<' '<<ds->flt[3]<<' '<<
			ds->flt[4]<<' '<<ds->flt[5]<<' '<<ds->flt[6]<<' '<<ds->flt[7]<<' '<<
			ds->flt[8]<<' '<<ds->flt[9]<<' '<<ds->flt[10]<<' '<<ds->flt[11]<<' '<<
			ds->flt[12]<<' '<<ds->flt[13]<<' '<<ds->flt[14]<<' '<<ds->flt[15]<<']';
	}
	else if (tp.IsString()) {
		os<<'\"'<<ds->str<<'\"';
	}
	else
		ASSERT(0);

	return os;
}

const char *str_add_types[] = {
	"error",
	"unknown",
	"void"
};

const char *Type::GetStr() const
{
	if (IsError())
		return str_add_types[0];
	else if (IsUnknown())
		return str_add_types[1];
	else if (IsVoid())
		return str_add_types[2];
	else
		return GetVariableTypeStr((variable_t)m_type);
}

Symbol *Scope::FindSymbol(const string &name, bool isextern)
{
	Symbol *sb;
	map<string, Symbol *>::iterator it = m_symbols.find(name);
	if (it==m_symbols.end()) {
		if ((isextern || m_da2parent)&&m_parent!=NULL)
			sb = m_parent->FindSymbol(name, isextern);
		else
			sb = NULL;
	}
	else 
		sb = it->second;

	return sb;

}
	
bool Scope::AddSymbol(Symbol *sb)
{
	map<string, Symbol *>::iterator it = m_symbols.find(sb->m_name);
	if (it!=m_symbols.end())
		return false;

	m_symbols[sb->m_name] = sb;

	return true;
}
	
void Scope::AddUnresolvedSymbol(const string &name)
{
	vector<string>::iterator it = m_unresolvedsym.begin();
	for (; it<m_unresolvedsym.end(); ++it) {
		if (name.compare(*it)==0) {
			return;
		}
	}

	m_unresolvedsym.push_back(name);
}

void Scope::AddAliasSymbol(const string &name, Symbol *s)
{
	m_symbols[name] = s;
}