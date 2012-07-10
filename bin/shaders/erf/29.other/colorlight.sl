light colorlight (
	float intensity = 1;
	point from = point "shader" (0,0,0);
	point to = point "shader" (0,0,1);
	)
{
    float cosangle;
    color lightcolor = 1;
    uniform vector A = normalize(to-from);

    illuminate (from, A, radians(30))
	{
	cosangle=(L.A)/length(L);
	lightcolor=color "hsv" (cosangle*10,1,1);
	Cl = intensity * lightcolor/(L.L) ;
	}
}
