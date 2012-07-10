/* point.c - create a simple Particle System */

#include <ri.h>
#include <math.h>

#define COUNT 1000

float jitter(float scale)
{
	float val=random()%1000;
	return (val/500-1)*scale;
}

int main(int argc, char *argv[])
{
RtPoint position[COUNT];
RtColor red={1,0,0};
float fov=30;
int i;

/*Generate Particle Postions*/
for(i=0;i<COUNT;i++)
	{
	position[i][0]=sin(i*0.5)*50+jitter(2);
	position[i][1]=cos(i*0.1)*50+jitter(2);
	position[i][2]=cos(i*0.5)*100+jitter(2);
	}

RiBegin(RI_NULL);
	RiDisplay ("point.tiff","file","rgb",RI_NULL);
	RiProjection ("perspective",
			"fov",&fov,
			RI_NULL);
	RiWorldBegin();
		RiTranslate(0,0,300);
		RiColor(red);
		RiPoints(COUNT,"P",position,
				RI_NULL);
	RiWorldEnd();
RiEnd();
return 0;
}
