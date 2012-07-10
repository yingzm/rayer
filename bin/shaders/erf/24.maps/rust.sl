surface map (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;
    float freq=20;
    color rust=float noise(s*freq,t*freq)*color "rgb" (1,0.2,0.2);


    float surfaceType=float texture("rustControl.tiff");
    Ct=mix(Cs,rust,surfaceType);
    

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + surfaceType*diffuse(Nf) +
		(1-surfaceType)*specular(Nf,V,roughness)));
}

