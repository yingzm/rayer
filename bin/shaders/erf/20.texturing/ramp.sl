surface ramp (
	color otherColor=color "rgb" (0,1,0);
	float orientation=0;
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;

    if(orientation==0)
	    Ct=mix(Cs,otherColor,s);
    else
	    Ct=mix(Cs,otherColor,t);

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

