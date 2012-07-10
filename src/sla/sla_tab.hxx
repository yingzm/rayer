typedef union {
	int ival;
	double dval;
	char str[256];
	datav dat;
	vector<datav> *datarr;
} YYSTYPE;
#define	INFO	258
#define	PARAMS	259
#define	CODE	260
#define	DATA	261
#define	DECLARE	262
#define	IDENTIFIER	263
#define	VALUE	264
#define	C_STRING	265
#define	T_LIGHT	266
#define	T_SURFACE	267
#define	T_DISPLACEMENT	268
#define	T_VOLUME	269
#define	T_IMAGER	270
#define	T_FLOAT	271
#define	T_POINT	272
#define	T_NORMAL	273
#define	T_VECTOR	274
#define	T_COLOR	275
#define	T_MATRIX	276
#define	T_STRING	277
#define	OPCODE0	278
#define	OPCODE1	279
#define	OPCODE2	280
#define	OPCODE3	281


extern YYSTYPE yylval;
