/* min.c - a minimal C program to use RenderMan */

#include <ri.h>

int main(int argc, char *argv[])
{
RiBegin(RI_NULL);
	RiDisplay ("min.tiff","file","rgb",RI_NULL);
	RiProjection ("perspective",RI_NULL);
	RiWorldBegin();
		RiTranslate(0,0,2);
		RiSphere(1,-1,1,360,RI_NULL);
	RiWorldEnd();
RiEnd();
return 0;
}
