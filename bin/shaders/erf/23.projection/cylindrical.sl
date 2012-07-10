surface cylindrical (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;

    point PP=transform("object",P);
    float repeatCount=10;
    float ss=atan(xcomp(PP),zcomp(PP))/(2*PI)*repeatCount;
    float tt=ycomp(PP)*repeatCount;
    Ct=color cellnoise(ss,tt);;
    

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

