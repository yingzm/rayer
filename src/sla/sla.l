%{
#include <string.h>
#include "sla.hxx"
#include "sla_tab.hxx"
int num_lines=1;
#define isatty(a) 0
// #define PRINTF(a, b) printf(a, b)
#define PRINTF(a, b)
%}
%option noyywrap

CHAR	[a-zA-Z_]
%%
\n			{ num_lines++; }
;[^\n]*			{}
[+-]?[0-9]*\.[0-9]+ { yylval.dval = atof(yytext); PRINTF("value=%s\n", yytext); return VALUE; }
[+-]?[0-9]+		{ yylval.dval = atof(yytext); PRINTF("valuE=%s\n", yytext); return VALUE; }
[+-]?(([0-9]+)|([0-9]*\.[0-9]+))([eE][+-]?[0-9]+)? { yylval.dval = atof(yytext); PRINTF("Value=%s\n", yytext); return VALUE; }
light			{ return T_LIGHT; }
surface			{ return T_SURFACE; }
displacement		{ return T_DISPLACEMENT; }
volume			{ return T_VOLUME; }
imager			{ return T_IMAGER; }
float			{ return T_FLOAT; }
vector			{ return T_VECTOR; }
normal			{ return T_NORMAL; }
point			{ return T_POINT; }
color			{ return T_COLOR; }
matrix			{ return T_MATRIX; }
string			{ return T_STRING; }
declare			{ return DECLARE; }

nop|NOP			{ yylval.ival = NOP; return OPCODE0; }
end|END			{ yylval.ival = END; return OPCODE0; }
neg|NEG			{ yylval.ival = NEG; return OPCODE0; }

negf|NEGF		{ yylval.ival = NEGF; return OPCODE1; }
negv|NEGV		{ yylval.ival = NEGV; return OPCODE1; }
jmp|JMP			{ yylval.ival = JMP; return OPCODE1; }
jt|JT			{ yylval.ival = JT; return OPCODE1; }
jf|JF			{ yylval.ival = JF; return OPCODE1; }
pushf|PUSHF		{ yylval.ival = PUSHF; return OPCODE1; }
pushv|PUSHV		{ yylval.ival = PUSHV; return OPCODE1; }
pushs|PUSHS		{ yylval.ival = PUSHS; return OPCODE1; }
popf|POPF		{ yylval.ival = POPF; return OPCODE1; }
popv|POPV		{ yylval.ival = POPV; return OPCODE1; }
ccall|CCALL		{ yylval.ival = CCALL; return OPCODE1; }

movf|MOVF		{ yylval.ival = MOVF; return OPCODE2; }
movv|MOVV		{ yylval.ival = MOVV; return OPCODE2; }
movs|MOVS		{ yylval.ival = MOVS; return OPCODE2; }
addr|ADDR		{ yylval.ival = ADDR; return OPCODE2; }
subr|SUBR		{ yylval.ival = SUBR; return OPCODE2; }
gtf|GTF			{ yylval.ival = GTF; return OPCODE2; }
gef|GEF			{ yylval.ival = GEF; return OPCODE2; }
ltf|LTF			{ yylval.ival = LTF; return OPCODE2; }
lef|LEF			{ yylval.ival = LEF; return OPCODE2; }
eqf|EQF			{ yylval.ival = EQF; return OPCODE2; }
nef|NEF			{ yylval.ival = NEF; return OPCODE2; }
eqv|EQV			{ yylval.ival = EQV; return OPCODE2; }
nev|NEV			{ yylval.ival = NEV; return OPCODE2; }
eqs|EQS			{ yylval.ival = EQS; return OPCODE2; }
nes|NES			{ yylval.ival = NES; return OPCODE2; }

addf|ADDF		{ yylval.ival = ADDF; return OPCODE3; }
addv|ADDV		{ yylval.ival = ADDV; return OPCODE3; }
subf|SUBF		{ yylval.ival = SUBF; return OPCODE3; }
subv|SUBV		{ yylval.ival = SUBV; return OPCODE3; }
mulf|MULF		{ yylval.ival = MULF; return OPCODE3; }
mulv|MULV		{ yylval.ival = MULV; return OPCODE3; }
mulvv|MULVV		{ yylval.ival = MULVV; return OPCODE3; }
divf|DIVF		{ yylval.ival = DIVF; return OPCODE3; }
divv|DIVV		{ yylval.ival = DIVV; return OPCODE3; }
dotv|DOTV		{ yylval.ival = DOTV; return OPCODE3; }
crsv|CRSV		{ yylval.ival = CRSV; return OPCODE3; }

\.info	{ return INFO; }
\.params { return PARAMS; }
\.code	{ return CODE; }
\.data	{ return DATA; }
[a-zA-Z_0-9]+ { PRINTF("ident=%s\n", yytext); strcpy(yylval.str, yytext);  return IDENTIFIER; }
\"[^\"\n]*\" { PRINTF("string=%s\n", yytext); strcpy(yylval.str, yytext+1); yylval.str[strlen(yytext)-2]=0; return C_STRING; }
[^ \n\t]	{ PRINTF("c=%c\n", yytext[0]); return yytext[0]; }
[ \t]		{}

%%
