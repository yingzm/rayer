/* color.c - Create Particles of different colors*/

#include <ri.h>
#include <math.h>

#define COUNT 2000
#ifdef WIN32
#define random rand
#endif

float jitter(float scale)
{
	float val=random()%1000;
	return (val/500-1)*scale;
}

int main(int argc, char *argv[])
{
RtPoint position[COUNT];
RtColor color[COUNT];
float constantwidth=0.5;
float fov=30;
int i;

/*Generate Particle Postions*/
for(i=0;i<COUNT;i++)
	{
	position[i][0]=sin(i*0.5)*50+jitter(2);
	position[i][1]=cos(i*0.1)*50+jitter(2);
	position[i][2]=cos(i*0.5)*100+jitter(2);
	color[i][0]=jitter(0.5)+0.5;
	color[i][1]=jitter(0.5)+0.5;
	color[i][2]=jitter(0.5)+0.5;
	}

RiBegin(RI_NULL);
	RiDisplay ("color.tiff","file","rgb",RI_NULL);
	RiProjection ("perspective",
			"fov",&fov,
			RI_NULL);
	RiWorldBegin();
		RiTranslate(0,0,300);
		RiPoints(COUNT,"P",position,
				"constantwidth", &constantwidth,
				"Cs",color,
				RI_NULL);
	RiWorldEnd();
RiEnd();
return 0;
}
