typedef union {
	int i;
	double d;
	char str[256];
	Node *node;
	NodeList *nodelist;
} YYSTYPE;
#define	LIGHT	258
#define	SURFACE	259
#define	VOLUME	260
#define	DISPLACEMENT	261
#define	IMAGER	262
#define	CPP_LINE	263
#define	FLOAT	264
#define	STRING	265
#define	COLOR	266
#define	POINT	267
#define	VECTOR	268
#define	NORMAL	269
#define	MATRIX	270
#define	VOID	271
#define	VARYING	272
#define	UNIFORM	273
#define	OUTPUT	274
#define	EXTERN	275
#define	RETURN	276
#define	IF	277
#define	ELSE	278
#define	WHILE	279
#define	FOR	280
#define	SOLAR	281
#define	ILLUMINATE	282
#define	ILLUMINANCE	283
#define	BREAK	284
#define	CONTINUE	285
#define	NUMBER	286
#define	IDENTIFIER	287
#define	STRINGCONSTANT	288
#define	GT	289
#define	GE	290
#define	LT	291
#define	LE	292
#define	EQ	293
#define	NE	294
#define	AND	295
#define	OR	296
#define	ASSIGN	297
#define	ADDASSIGN	298
#define	SUBASSIGN	299
#define	MULASSIGN	300
#define	DIVASSIGN	301
#define	LOWER_THAN_ELSE	302
#define	CAST	303
#define	TYPECAST	304
#define	UMINUS	305
#define	HIGHEST	306


extern YYSTYPE yylval;
