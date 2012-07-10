/*declare.c - declare the type of a parameter
			to a custom shader */

#include <ri.h>

int main(int argc, char *argv[])
{
RtPoint square[4]={{0,0,0},{1,0,0},{0,1,0},{1,1,0}};
RtColor red={1,0,0};
float fov=30;

RiBegin(RI_NULL);
	RiDisplay ("declare.tiff","file","rgb",RI_NULL);
	RiProjection ("perspective",
						"fov",&fov,
						RI_NULL);
	RiTranslate(-0.5,-0.5,3);
	RiWorldBegin();
		RiDeclare("customColor","uniform color");
		RiSurface("myConstantSurface",
						"customColor",red,
						RI_NULL);
		RiPatch("bilinear",
						"P",square,
						RI_NULL);
	RiWorldEnd();
RiEnd();
return 0;
}
