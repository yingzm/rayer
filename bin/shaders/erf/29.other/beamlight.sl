light
beamlight ( float intensity = 1;
            color lightcolor = 1;
            point from = point "shader" (0,0,0);
            point to = point "shader" (0,0,1);
            )
{
    uniform vector A = normalize(to-from);

    illuminate (from, A, radians(30))
	 {
        Cl = intensity * lightcolor/(L.L) ;
    }
}
