typedef union {
	double d;
	char str[256];
	Node *node;
	vector<Node *> *nodelist;
} YYSTYPE;
#define	LIGHT	258
#define	SURFACE	259
#define	VOLUME	260
#define	DISPLACEMENT	261
#define	IMAGER	262
#define	FLOAT	263
#define	STRING	264
#define	COLOR	265
#define	POINT	266
#define	VECTOR	267
#define	NORMAL	268
#define	MATRIX	269
#define	VOID	270
#define	VARYING	271
#define	UNIFORM	272
#define	OUTPUT	273
#define	EXTERN	274
#define	RETURN	275
#define	IF	276
#define	ELSE	277
#define	WHILE	278
#define	FOR	279
#define	SOLAR	280
#define	ILLUMINATE	281
#define	ILLUMINANCE	282
#define	BREAK	283
#define	CONTINUE	284
#define	NUMBER	285
#define	IDENTIFIER	286
#define	STRINGCONSTANT	287
#define	GT	288
#define	GE	289
#define	LT	290
#define	LE	291
#define	EQ	292
#define	NE	293
#define	AND	294
#define	OR	295
#define	ASSIGN	296
#define	ADDASSIGN	297
#define	SUBASSIGN	298
#define	MULASSIGN	299
#define	DIVASSIGN	300


extern YYSTYPE yylval;
