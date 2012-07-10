#include "FileNode.hxx"
#include "slc_tab.hxx"
#include <process.h>

extern FILE *yyin;
extern int yyparse();
extern int yydebug;

int main(int argc, char **argv)
{
	if (argc!=2) {
		printf("Usage: slc shaderfile\n");
		return 1;
	}

	char buf[256];
	strcpy(buf, "cpp ");
	strcat(buf, argv[1]);
	strcat(buf, " > __temp.sl");
	system(buf);
	
	FILE *fp = fopen("__temp.sl", "r");
	if (fp==NULL)
		return 2;
	
	yyin = fp;
	g_filename = argv[1];
	// yydebug=1;
	
	yyparse();
	
	fclose(fp);

	// system("del __temp.sl");

	g_file.Pass1();
	g_file.Pass2();
	g_file.GenAssembly(argv[1]);

	
	return 0;
}