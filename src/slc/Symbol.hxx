#ifndef __SYMBOL_HXX__
#define __SYMBOL_HXX__

class FuncDefNode;

class Symbol;
#include "Node.hxx"


#define VAR_VOID 7
#define VAR_UNKNOWN 8
#define VAR_ERROR 9

class Type
{
public:
	Type() { m_type = VAR_UNKNOWN; }
	Type(Sint i) { m_type = i; }

	/**
	 * Judge if two types are equal, notice that normal and vector are
	 * equal here.
	 * TODO: overload operator+
	 */
	Sbool IsSame(const Type &) const;
	/** 
	 * If type is any of vector, point, normal or color, it is a triple.
	 * A constant (a, b, c) can be interpreted to any of the types.
	 * Further more, The shading virtual machine does not make any
	 * difference between them.
	 */
	Sbool IsTriple() const { return (m_type&0xf)==VAR_NORMAL||(m_type&0xf)==VAR_VECTOR||
		(m_type&0xf)==VAR_POINT||(m_type&0xf)==VAR_COLOR; }

	/**
	 * A type is invalid if it is any of the error, unknown or void.
	 * Such a type can't be the type of a variable
	 */
	Sbool IsInvalid() const { return IsError() || IsUnknown() || IsVoid(); }
	/**
	 * Some error occurs during the type deduction of the variable 
	 */
	Sbool IsError() const { return ((m_type&0x0f)==VAR_ERROR); }
	Sbool IsUnknown() const { return ((m_type&0x0f)==VAR_UNKNOWN); }
	Sbool IsVoid() const { return (m_type&0x0f)==VAR_VOID; }

	Sbool IsFloat() const { return (m_type&0xf)==VAR_FLOAT; }
	Sbool IsPoint() const { return (m_type&0xf)==VAR_POINT; }
	Sbool IsNormal() const { return (m_type&0xf)==VAR_NORMAL||(m_type&0xf)==VAR_VECTOR; }
	Sbool IsVector() const { return (m_type&0xf)==VAR_NORMAL||(m_type&0xf)==VAR_VECTOR; }
	Sbool IsColor() const { return (m_type&0xf)==VAR_COLOR; }
	Sbool IsString() const { return (m_type&0xf)==VAR_STRING; }
	Sbool IsMatrix() const { return (m_type&0xf)==VAR_MATRIX; }
	Sbool IsArray() const { return (m_type&VAR_ARRAY) ? true : false; }
	Sbool IsExtern() const { return (m_type&VAR_EXTERN) ? true :false; }

	/**
	 * get the character of the type
	 * float-->'f', triple-->'v', string-->'s', matrix-->'m', void-->'_'
	 */
	Schar Char() const;
	Schar FuncChar() const;
	/**
	 * get the string of name of the type
	 */
	const char *GetStr() const;
	/**
	 * size of the type
	 */
	Sint Size() const;

public:
	Sint m_type;
};

/**
 * used to store the value of a constant(in data segment).
 * Also used as a value container for Eval function or ExprNode
 */
class DataSymbol
{
public:
	friend aofstream &operator<<(aofstream &os, DataSymbol *);

	Symbol *sym;
	Type type;	// type coincide with sym->m_type, since we also use this class
			// as a value container for Eval function. In that case, sym==NULL
	Suint size;	// size of the data, this may be different from type.Size()
			// when type.IsString().
	Sfloat flt[16];
	string str;
};

/**
 *
 */
class Symbol
{
public:
	Sbool IsTemp() const { return m_stype==SYMBOL_TEMPVAR; }
	Sbool IsConstant() const { return m_stype==SYMBOL_CONSTANT; }
	Sbool IsBuiltinFunc() const { return m_stype==SYMBOL_BUILTINFUNC; }
	Sbool IsFunc() const { return m_stype==SYMBOL_FUNC; }
	Sbool IsParam() const { return m_stype==SYMBOL_PARAM; }
	Sbool IsLabel() const { return m_stype==SYMBOL_LABEL; }
	Sbool IsLocal() const { return m_stype==SYMBOL_LOCALVAR; }


	string m_name;
	enum {
		SYMBOL_FUNC,
		SYMBOL_BUILTINFUNC,
		SYMBOL_ENV,
		SYMBOL_PARAM,
		SYMBOL_LOCALVAR,
		SYMBOL_CONSTANT,
		SYMBOL_TEMPVAR,
		SYMBOL_LABEL
	};

public:
	Suint m_stype;
	Type m_type;
	union {
		Sint addr;
		FuncDefNode *ptr;
	} u;
};

class Scope
{
public:
	Scope() { m_parent = NULL; m_da2parent=false; }
	Symbol *FindSymbol(const string &name, bool isextern);
	bool AddSymbol(Symbol *);
	void AddAliasSymbol(const string &name, Symbol *s);
	void AddUnresolvedSymbol(const string &name);

public:
	map<string, Symbol *> m_symbols;
	vector<string> m_unresolvedsym;

	Scope *m_parent;
	bool m_da2parent; // direct access to parent
};

class ScopeNode
{
public:
	ScopeNode() { m_scope = NULL; }
	
	ScopeNode *CopyScope(const ScopeNode *node) { delete m_scope; m_scope = node->m_scope; }

	Scope *m_scope;
};


inline Sbool Type::IsSame(const Type &sb) const
{
	if ( ((m_type&0x1f) == (sb.m_type&0x1f)) || 
		( (IsVector() || IsNormal()) && (sb.IsNormal() || sb.IsVector()) ) )
		return true;
	else
		return false;

}

inline Schar Type::Char() const
{
	switch (m_type&0xf) {
	case VAR_FLOAT:
		return 'f';
	case VAR_POINT:
	case VAR_VECTOR:
	case VAR_NORMAL:
	case VAR_COLOR:
		return 'v';
	case VAR_MATRIX:
		return 'm';
	case VAR_STRING:
		return 's';
	}

	ASSERT(0);
	return '_';
}

inline Schar Type::FuncChar() const
{
	switch (m_type&0xf) {
	case VAR_FLOAT:
		return 'f';
	case VAR_POINT:
		return 'p';
	case VAR_VECTOR:
	case VAR_NORMAL:
		return 'v';
	case VAR_COLOR:
		return 'c';
	case VAR_MATRIX:
		return 'm';
	case VAR_STRING:
		return 's';
	case VAR_VOID:
		return '_';
	}

	ASSERT(0);
	return '_';
}


inline Sint Type::Size() const
{
	if (IsInvalid())
		return 0;
	else {
		return variable_dword((variable_t)m_type)*4;
	}
}





#endif