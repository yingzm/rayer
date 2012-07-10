surface wood (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;
	float scale=0.06;
	)
{
    color lightWood=color "rgb" (1,0.6,0.5);
    color darkWood=color "rgb" (0.3,0.2,0.2);
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;
    point PP;
    float l;

    PP=transform("shader",P)*scale;
    PP=PP+point noise(PP*10)*0.1;
    l=sqrt(xcomp(PP)*xcomp(PP)+ycomp(PP)*ycomp(PP));
    Ct=mix(darkWood,lightWood,mod(l*8,1));

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
                specularcolor * Ks*specular(Nf,V,roughness));

}

