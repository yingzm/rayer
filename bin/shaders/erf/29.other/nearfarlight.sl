light nearfarlight (
	float intensity = 1;
	color lightcolor = 1;
	float near = 2;
	float far = 3;
	point from = point "shader" (0,0,0);)
{
    float length;
    float brightness;
    
    illuminate (from)
    	{
	length=sqrt(L.L);
	if(length<near || length>far)
		brightness=0;
	else
		brightness=1/length;
	Cl = intensity * lightcolor*brightness;
	}
}
