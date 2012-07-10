surface filterWidth  ( float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;
    float filterWidth;
    float onRight;

    float ss=s+0.1*t;
    filterWidth=abs(Du(ss)*du)+abs(Dv(ss)*dv);


    if(t>0.5)
	{
		if(ss>0.5)
			Ct=color "rgb" (0,1,0);
		else
			Ct=Cs;
	}
    else
	{
		onRight=smoothstep(0.5-filterWidth,0.5+filterWidth,ss);
		Ct=mix(Cs,color "rgb" (0,1,0),onRight);
	}
    

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

