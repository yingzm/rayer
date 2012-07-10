/* light.c - create a light source */

#include <ri.h>

int main(int argc, char *argv[])
{
RtPoint square[4]={{0,0,0},{1,0,0},{0,1,0},{1,1,0}};
RtPoint lightPos[1]={{0,0,-0.2}};
RtLightHandle theLight;
RtColor red={1,0,0};
float fov=30;

RiBegin(RI_NULL);
	RiDisplay ("light.tiff","file","rgb",RI_NULL);
	RiProjection ("perspective",
					"fov",&fov,
					RI_NULL);
	RiTranslate(-0.5,-0.5,3);
	RiWorldBegin();
		theLight=RiLightSource("pointlight",
					"from",lightPos,
					RI_NULL);
		RiColor(red);
		RiPatch("bilinear",
					"P",square,
					RI_NULL);
	RiWorldEnd();
RiEnd();
return 0;
}
