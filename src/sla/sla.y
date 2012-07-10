%{
#include <malloc.h>
#include <stdio.h>
#include "sla.hxx"
extern int num_lines;
extern int yylex();
void yyerror(const char *s)
{
	printf("%d: %s\n", num_lines, s);
}
%}

%union {
	int ival;
	double dval;
	char str[256];
	datav dat;
	vector<datav> *datarr;
};

%token INFO PARAMS CODE DATA 
%token DECLARE IDENTIFIER VALUE C_STRING
%token T_LIGHT T_SURFACE T_DISPLACEMENT T_VOLUME T_IMAGER
%token T_FLOAT T_POINT T_NORMAL T_VECTOR T_COLOR T_MATRIX T_STRING
%token OPCODE0 OPCODE1 OPCODE2 OPCODE3

%type<ival> OPCODE0 OPCODE1 OPCODE2 OPCODE3 operand shadertype variabletype
%type<ival> T_LIGHT T_SURFACE T_VOLUME T_DISPLACEMENT T_IMAGER
%type<dval> VALUE
%type<str> IDENTIFIER C_STRING
%type<dat> datavalue
%type<datarr> datavaluelist

%%
file: sectionlist { g_a.Save(); }
	;

sectionlist: section
	| sectionlist section
	;

section : INFO infobody 
	| PARAMS paramsbody 
	| CODE codebody 
	| DATA databody 
	;
	
infobody: shadertype IDENTIFIER { g_a.Shader($1, $2); }
	;

shadertype: T_LIGHT	{ $$=T_LIGHT; }
	| T_SURFACE	{ $$=T_SURFACE; }
	| T_DISPLACEMENT	{ $$=T_DISPLACEMENT; }
	| T_VOLUME	{ $$=T_VOLUME; }
	| T_IMAGER	{ $$=T_IMAGER; }
	;
	
paramsbody: paramlist
	;

paramlist: param
	| paramlist param
	;

param: variabletype IDENTIFIER datavalue { g_a.AddParam($1, $2, $3); }
	| variabletype '[' VALUE ']' IDENTIFIER '[' datavaluelist ']' { g_a.AddParamArray($1, (int)$3, $5, $7); delete $7; }
	;

variabletype: T_FLOAT { $$=T_FLOAT; }
	| T_POINT { $$=T_POINT; }
	| T_NORMAL { $$=T_NORMAL; }
	| T_VECTOR { $$=T_VECTOR; }
	| T_COLOR { $$=T_COLOR; }
	| T_MATRIX { $$=T_MATRIX; }
	| T_STRING { $$=T_STRING; }
	;

codebody: cmds { }
	;
	
cmds: cmd
	| cmds cmd  
	;
	
cmd: OPCODE0 { g_a.Op0($1);  }
	| OPCODE1 operand { g_a.Op1($1, $2); }
	| OPCODE2 operand ',' operand { g_a.Op2($1, $2, $4); }
	| OPCODE3 operand ',' operand ',' operand { g_a.Op3($1, $2, $4, $6); }
	| DECLARE variabletype IDENTIFIER '[' VALUE ']' { g_a.AddSymbol($3, Symbol::COMMON, $2, LS_ADDR((int)$5) ); }
	| IDENTIFIER operand { g_a.Error("%d: Unknown command %s\n", num_lines, $1); }
	| IDENTIFIER operand ',' operand { g_a.Error("%d: Unknown command %s\n", num_lines, $1); }
	| IDENTIFIER operand ',' operand ',' operand { g_a.Error("%d: Unknown command %s\n", num_lines, $1); }
	| IDENTIFIER ':' { g_a.AddLabel($1); }
	;

operand: IDENTIFIER  { $$=g_a.ResolveSymbol($1); }
	| VALUE { $$=$1; }
	;
	
datavalue: VALUE { $$.type = datav::TP_DOUBLE; $$.dval = $1; }
	| '[' VALUE VALUE VALUE ']' { $$.type=datav::TP_TRIPLE; $$.tri[0]=$2; $$.tri[1]=$3; $$.tri[2]=$4; }
	| C_STRING { $$.type=datav::TP_STRING; strcpy($$.str, $1); }
	;

datavaluelist: datavalue { $$ = new vector<datav>; $$->push_back($1); }
	| datavaluelist datavalue { $1->push_back($2); }
	;
	
databody: datalist
	;

datalist: dataitem
	| datalist dataitem
	;

dataitem: variabletype IDENTIFIER datavalue { g_a.AddData($1, $2, $3); }
	| variabletype '[' VALUE ']' IDENTIFIER '[' datavaluelist ']' { g_a.AddDataArray($1, (int)$3, $5, $7); delete $7; }
	; 
	
%%


extern FILE *yyin;
int main(int argc, char **argv)
{
	if (argc!=2) {
		printf("Usage: sla slafile\n");
		return -1;
	}
	yyin = fopen(argv[1], "r");
	if (yyin==NULL) {
		printf("Failed to open %s\n", argv[1]);
		return 0;
	}
	
	printf("Compiling...\n");

	return yyparse();
}
