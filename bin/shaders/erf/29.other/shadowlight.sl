light shadowlight (
	float intensity = 1;
	color lightcolor = 1;
	point from = point "shader" (0,0,0);
	point to = point "shader" (0,0,1);
	string mapname="";
	)
{
    float atten=1;
    uniform vector A = normalize(to-from);

    illuminate (from, A, radians(30))
	{
	if(mapname != "")
	 	atten=1-shadow(mapname,Ps);
	Cl = atten*intensity * lightcolor/(L.L) ;
	}
}
