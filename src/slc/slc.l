%{

#include <string.h>
#include <string>
#include <vector>
using namespace std;
class Node;
class NodeList;
#include "slc_tab.hxx"

int g_numlines = 1;
string g_filename;
#define isatty(a) 0
#define MAX_STRLEN 255

%}

%option noyywrap

%s COMMENT


DVALUE	((([0-9]+)|([0-9]*\.[0-9]+))([eE][+-]?[0-9]+)?)
CHAR	[a-zA-Z_0-9]

%%
[ \t]*"/*"	{ BEGIN COMMENT; }
<COMMENT>"*/"[ \t]+		{ BEGIN 0; }
<COMMENT>"*/"\n	{ ++g_numlines; BEGIN 0; }
<COMMENT>[^\n\*]*	{ }
<COMMENT>\*		{ }
<COMMENT>\n		{ ++g_numlines; }
#	{ return CPP_LINE; }
#pragma[^\n]*\n { }
\n		{ ++g_numlines; }
light	{ return LIGHT; }
surface	{ return SURFACE; }
volume	{ return VOLUME; }
displacement	{ return DISPLACEMENT; }
imager	{ return IMAGER; }
float	{ return FLOAT; }
string	{ return STRING; }
color	{ return COLOR; }
point	{ return POINT; }
vector	{ return VECTOR; }
normal	{ return NORMAL; }
matrix	{ return MATRIX; }
void	{ return VOID; }
varying	{ return VARYING; }
uniform	{ return UNIFORM; }
output	{ return OUTPUT; }
extern	{ return EXTERN; }

return	{ return RETURN; }
if		{ return IF; }
else	{ return ELSE; }
while	{ return WHILE; }
for		{ return FOR; }
solar	{ return SOLAR; }
illuminate	{ return ILLUMINATE; }
illuminance	{ return ILLUMINANCE; }
break	{ return BREAK; }
continue	{ return CONTINUE; }

>			{ return GT; }
>=			{ return GE; }
\<			{ return LT; }
\<=			{ return LE; }
==			{ return EQ; }
!=			{ return NE; }
&&			{ return AND; }
\|\|			{ return OR; }
=			{ return ASSIGN; }
\+=			{ return ADDASSIGN; }
-=			{ return SUBASSIGN; }
\*=			{ return MULASSIGN; }
\/=			{ return DIVASSIGN; }

[0-9]+		{ yylval.d = atof(yytext); return NUMBER; }
{DVALUE}	{ yylval.d = atof(yytext);  return NUMBER; }
[a-zA-Z_]+{CHAR}*		{ if (strlen(yytext)>MAX_STRLEN) { printf("%s exceed maximum string length, truncated.\n", yytext); strncpy(yylval.str, yytext, 255); } else strcpy(yylval.str, yytext); return IDENTIFIER; }
\"[^\n\"]*\"	{ if (strlen(yytext)>MAX_STRLEN) { printf("%s exceed maximum string length, truncated.\n", yytext); strncpy(yylval.str, yytext+1, 255); } else { strcpy(yylval.str, yytext+1); yylval.str[strlen(yylval.str)-1]=0; } return STRINGCONSTANT; }
[^ \n\t]	{ return yytext[0]; }
[ \t]		{}

%%

