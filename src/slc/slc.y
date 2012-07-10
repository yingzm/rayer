%{

#include <malloc.h>
#include <stdio.h>
#include "Node.hxx"
#include "FileNode.hxx"
#include "DefNode.hxx"
#include "ExprNode.hxx"
#include "RelationNode.hxx"
#include "StmtNode.hxx"

#define YYDEBUG 1 
#pragma warning(disable:4129)

extern int g_numlines;
extern string g_filename;
extern FILE *yyin;
extern int yylex();
void yyerror(const char *s)
{
	printf("%s:%d> %s\n", g_filename.c_str(), g_numlines, s);
}

%}

%token LIGHT SURFACE VOLUME DISPLACEMENT IMAGER CPP_LINE
%token FLOAT STRING COLOR POINT VECTOR NORMAL MATRIX VOID
%token VARYING UNIFORM OUTPUT EXTERN 
%token RETURN IF ELSE WHILE FOR SOLAR ILLUMINATE ILLUMINANCE BREAK CONTINUE
%token NUMBER IDENTIFIER STRINGCONSTANT
%token GT GE LT LE EQ NE AND OR ASSIGN ADDASSIGN SUBASSIGN MULASSIGN DIVASSIGN

%union {
	int i;
	double d;
	char str[256];
	Node *node;
	NodeList *nodelist;
}

%type<i> shader_type type detail  typespec 
%type<d> NUMBER 
%type<str> IDENTIFIER STRINGCONSTANT
%type<node> shader_definition statement assignexpression expression primary def_expression procedurecall relation function_definition loop_control typecast
%type<nodelist> statements variable_definitions def_expressions formals expressionlist

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%nonassoc CAST
%nonassoc STRINGCONSTANT
%right ASSIGN ADDASSIGN SUBASSIGN MULASSIGN DIVASSIGN
%nonassoc TYPECAST
%right '?' ':'
%left OR
%left AND 
%left EQ NE
%left GT GE LT LE 
%left '+' '-'
%left '^'
%left '/' '*'
%left '.'
%right UMINUS '!'
%nonassoc HIGHEST

%%

file: definition
	| file definition
	;

definition: shader_definition	{ g_file.AddShaderDef((DefNode *)$1); }
	| function_definition { g_file.AddFuncDef((DefNode *)$1); }
	| cpp_line
	;
	
cpp_line: CPP_LINE NUMBER	{ g_numlines = (int)($2)-1; }
	| CPP_LINE NUMBER STRINGCONSTANT { g_numlines = (int)($2)-1; g_filename = $3; }
	| CPP_LINE NUMBER STRINGCONSTANT NUMBER { g_numlines = (int)($2)-1; g_filename = $3; }
	;
	
shader_definition: shader_type IDENTIFIER '(' ')' '{' statements '}'	{ $$ = new ShaderDefNode((Suint32)$1, $2, NULL, $6); } 
	| shader_type IDENTIFIER '(' formals ')' '{' statements '}'	{ $$ = new ShaderDefNode((Suint32)$1, $2, $4, $7); }
	| shader_type IDENTIFIER '(' formals ';' ')' '{' statements '}'	{ $$ = new ShaderDefNode((Suint32)$1, $2, $4, $8); }
	;

function_definition: IDENTIFIER '(' ')' '{' statements '}' { $$ = new FuncDefNode(-1, $1, NULL, $5); }
	| typespec IDENTIFIER '(' ')' '{' statements '}' { $$ = new FuncDefNode($1, $2, NULL, $6); }
	| IDENTIFIER '(' formals ')' '{' statements '}' { $$ = new FuncDefNode(-1, $1, $3, $6); }
	| IDENTIFIER '(' formals ';' ')' '{' statements '}' { $$ = new FuncDefNode(-1, $1, $3, $7); }
	| typespec IDENTIFIER '(' formals ')' '{' statements '}' { $$ = new FuncDefNode($1, $2, $4, $7); }
	| typespec IDENTIFIER '(' formals ';' ')' '{' statements '}' { $$ = new FuncDefNode($1, $2, $4, $8); }
	;

shader_type: LIGHT	{ $$ = SHADER_LIGHT; }
	| SURFACE	{ $$ = SHADER_SURFACE; }
	| VOLUME	{ $$ = SHADER_VOLUME; }
	| DISPLACEMENT	{ $$ = SHADER_DISPLACEMENT; }
	| IMAGER	{ $$ = SHADER_IMAGER; }
	;

formals: variable_definitions { $$ = $1; }
	| formals ';' variable_definitions { int n=$3->size(); for (int i=0; i<n; ++i) {Node *node = (*$3)[i]; $1->push_back(node); } delete $3;  $$=$1;} 
	;

variable_definitions: typespec def_expressions { $$=$2; int n = $2->size(); for (int i=0; i<n; ++i) { DefVarNode *node = (DefVarNode *)(*$2)[i]; node->SetType($1); } }
	| EXTERN typespec def_expressions { $$=$3; int n = $3->size(); for (int i=0; i<n; ++i) { DefVarNode *node = (DefVarNode *)(*$3)[i]; node->SetType($2); node->SetExtern(); } }
	| OUTPUT typespec def_expressions { $$=$3; int n = $3->size(); for (int i=0; i<n; ++i) { DefVarNode *node = (DefVarNode *)(*$3)[i]; node->SetType($2); node->SetExtern(); } }
	;
	
typespec: type	{ $$ = $1; }
	| detail type { $$ = $1 | $2; }
	;
	
def_expressions: def_expression	{ $$ = new NodeList; $$->push_back($1); }
	| def_expressions ',' def_expression { $$ = $1, $$->push_back($3); }
	;
	
def_expression: IDENTIFIER	{ $$ = new DefVarNode(new IdentNode($1), NULL); }
	| IDENTIFIER ASSIGN expression	{ $$ = new DefVarNode(new IdentNode($1), (ExprNode *)$3); }
	;
	
type: FLOAT	{ $$ = VAR_FLOAT; }
	| STRING { $$ = VAR_STRING; }
	| COLOR { $$ = VAR_COLOR; }
	| POINT { $$ = VAR_POINT; }
	| VECTOR { $$ = VAR_VECTOR; }
	| NORMAL { $$ = VAR_NORMAL; }
	| MATRIX { $$ = VAR_MATRIX; }
	| VOID { $$ = -1; }
	;
	
detail: VARYING	{ $$ = VAR_VARYING; }
	| UNIFORM { $$ = VAR_UNIFORM; }
	;

statements: statement		{ $$ = new NodeList; $$->push_back($1); }
	| statements statement { $1->push_back($2); }	
	;
	

statement: variable_definitions ';'	{ $$ = new VarDefStmtNode($1); }
	| assignexpression ';'		{ $$ = new AssignStmtNode((ExprNode *)$1); }
	| procedurecall ';'		{ $$ = new ProcCallStmtNode($1); }
	| RETURN expression ';'	{ $$ = new ReturnStmtNode($2); }
	| BREAK ';'	{ $$ = new BreakStmtNode(0); }
	| BREAK NUMBER ';' { $$ = new BreakStmtNode($2); }
	| CONTINUE ';'	{ $$ = new ContinueStmtNode(0); }
	| CONTINUE NUMBER ';' { $$ = new ContinueStmtNode($2); }
	| IF relation statement	%prec LOWER_THAN_ELSE { $$ = new IfStmtNode($2, $3, NULL); }
	| IF relation statement ELSE statement { $$ = new IfStmtNode($2, $3, $5); }
	| loop_control statement { LoopControlNode *node = (LoopControlNode *)$1; node->m_stmt = (StmtNode *)$2; $$=$1; }
	| '{' statements '}' { $$ = new StmtListNode($2); }
	;
	
loop_control: WHILE relation	{ $$ = new WhileNode($2); }
	| FOR '(' expression ';' relation ';' expression ')' { $$ = new ForNode($3, $5, $7); }
	| SOLAR '(' ')' { $$ = new ProcCallLoopNode(new ProcCallNode("solar", NULL)); }
	| SOLAR '(' expressionlist ')' { $$ = new ProcCallLoopNode(new ProcCallNode("solar", $3)); }
	| ILLUMINATE '(' ')' { $$ = new ProcCallLoopNode(new ProcCallNode("illuminate", NULL)); }
	| ILLUMINATE '(' expressionlist ')' { $$ = new ProcCallLoopNode(new ProcCallNode("illuminate", $3)); }
	| ILLUMINANCE '(' ')' { $$ = new ProcCallLoopNode(new ProcCallNode("illuminance", NULL)); }
	| ILLUMINANCE '(' expressionlist ')' { $$ = new ProcCallLoopNode(new ProcCallNode("illuminance", $3)); }
	;
	
expressionlist: expression { $$ = new NodeList; $$->push_back($1); }
	| expressionlist ',' expression { $$ = $1; $$->push_back($3); }
	;
	
expression: primary	{ $$ = $1; }
	| expression '.' expression	{ $$ = new BinopExprNode((ExprNode *)$1, '.', (ExprNode *)$3); }
	| expression '^' expression	{ $$ = new BinopExprNode((ExprNode *)$1, '^', (ExprNode *)$3); }	
	| expression '+' expression	{ $$ = new BinopExprNode((ExprNode *)$1, '+', (ExprNode *)$3); }	
	| expression '-' expression	{ $$ = new BinopExprNode((ExprNode *)$1, '-', (ExprNode *)$3); }	
	| expression '*' expression	{ $$ = new BinopExprNode((ExprNode *)$1, '*', (ExprNode *)$3); }	
	| expression '/' expression	{ $$ = new BinopExprNode((ExprNode *)$1, '/', (ExprNode *)$3); }	
	| '-' expression %prec UMINUS	{ $$ = new UnaryExprNode('-', $2); }
	| relation '?' expression ':' expression { $$ = new QuestionExprNode($1, $3, $5); }
	| typecast expression	%prec TYPECAST{ $$ = new TypecastExprNode($1, $2); }
	| '(' expression ')' %prec HIGHEST	{ $$ = $2; }
	;
	
typecast: typespec	{ $$ = new Typecast($1); }
	| typespec STRINGCONSTANT { $$ = new Typecast($1, $2); }
	;
	
primary: NUMBER	{ $$ = new NumberExprNode($1); }
	| STRINGCONSTANT	{ $$ = new StringConstantNode($1); }
	| IDENTIFIER	{ $$ = new IdentNode($1); }
	| IDENTIFIER '[' NUMBER ']' { $$ = new IdentArrayNode($1, $3); }
	| procedurecall { $$ = $1; }
	| assignexpression { $$ = $1; }
	| '(' expression ',' expression ',' expression ')' { $$ = new TripleNode($2, $4, $6); }
	| '(' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ')' { $$= new SixteentupleNode($2, $4, $6, $8, $10, $12, $14, $16, $18, $20, $22, $24, $26, $28, $30, $32); }
	;

relation: '(' relation ')'	{ $$ = $2; }
	| expression GT expression { $$ = new RelopNode($1, GT, $3); }
	| expression GE expression { $$ = new RelopNode($1, GE, $3); }
	| expression LT expression { $$ = new RelopNode($1, LT, $3); }
	| expression LE expression { $$ = new RelopNode($1, LE, $3); }
	| expression EQ expression { $$ = new RelopNode($1, EQ, $3); }
	| expression NE expression { $$ = new RelopNode($1, NE, $3); }
	| relation AND relation	{ $$ = new LogopNode($1, AND, $3); }
	| relation OR relation	{ $$ = new LogopNode($1, OR, $3); }
	| '!' relation		{ $$ = new NotRelNode($2); }
	;

assignexpression: IDENTIFIER ASSIGN expression		{ IdentNode *ien = new IdentNode($1); $$ = new AssignExprNode(ien, (ExprNode *)$3); }
	| IDENTIFIER '[' NUMBER ']' ASSIGN expression	{ $$ = new AssignExprNode(new IdentArrayNode($1, $3), (ExprNode *)$6); }
	| IDENTIFIER ADDASSIGN expression				{ IdentNode *ien = new IdentNode($1); BinopExprNode *ben = new BinopExprNode(ien, '+', (ExprNode *)$3); $$ = new AssignExprNode(ien, ben); }
	| IDENTIFIER '[' NUMBER ']' ADDASSIGN expression { $$ = new AssignExprNode(new IdentArrayNode($1, $3), (ExprNode *)$6); }
	| IDENTIFIER SUBASSIGN expression				{ IdentNode *ien = new IdentNode($1); BinopExprNode *ben = new BinopExprNode(ien, '-', (ExprNode *)$3); $$ = new AssignExprNode(ien, ben); }
	| IDENTIFIER '[' NUMBER ']' SUBASSIGN expression { $$ = new AssignExprNode(new IdentArrayNode($1, $3), (ExprNode *)$6); }
	| IDENTIFIER MULASSIGN expression				{ IdentNode *ien = new IdentNode($1); BinopExprNode *ben = new BinopExprNode(ien, '*', (ExprNode *)$3); $$ = new AssignExprNode(ien, ben); }
	| IDENTIFIER '[' NUMBER ']' MULASSIGN expression { $$ = new AssignExprNode(new IdentArrayNode($1, $3), (ExprNode *)$6); }
	| IDENTIFIER DIVASSIGN expression				{ IdentNode *ien = new IdentNode($1); BinopExprNode *ben = new BinopExprNode(ien, '/', (ExprNode *)$3); $$ = new AssignExprNode(ien, ben); }
	| IDENTIFIER '[' NUMBER ']' DIVASSIGN expression { $$ = new AssignExprNode(new IdentArrayNode($1, $3), (ExprNode *)$6); }
	;

procedurecall: IDENTIFIER '(' ')' { $$ = new ProcCallNode($1, NULL); }
	| IDENTIFIER '(' expressionlist ')' { $$ = new ProcCallNode($1, $3); }
	;
	
	
%%


