surface smoothstep (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;

    float inTop;
    color red=color "rgb" (1,0,0);
    color green=color "rgb" (0,1,0);

    inTop=smoothstep(0.4,0.6,t);

    Ct=mix(green,red,inTop);
    

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

