/* loop.c - Create a line of Spheres */

#include <ri.h>

int main(int argc, char *argv[])
{
int i;
RiBegin(RI_NULL);
	RiDisplay ("loop.tiff","file","rgb",RI_NULL);
	RiProjection ("perspective",RI_NULL);
	RiDepthOfField(2.8,0.100,2);
	RiTranslate(0,0,1);
	RiWorldBegin();
		RiTranslate(0.3,0,0);
		for(i=0;i<12;i++)
			{
			RiSphere(0.25,-0.25,0.25,360,RI_NULL);
			RiTranslate(-0.2,0,0.5);
			}
	RiWorldEnd();
RiEnd();
return 0;
}
