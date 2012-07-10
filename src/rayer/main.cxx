#include "common.hxx"
#include "RayEngine.hxx"

void PrintUsage()
{
	printf("Usage: rayer ribfile\n");
}

int main(int argc, char **argv)
{
	if (argc<2) {
		PrintUsage();
		return 2;
	}

	RayEngine re;	

	re.Create();

	if (re.ReadRib(argv[1])<0)
		return 1;

	re.Destroy();

	return 0;
}
