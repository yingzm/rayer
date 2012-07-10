surface perspective (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;

    point PP=transform("camera",P);
    float repeatCount=20;
    float ss=xcomp(PP)/zcomp(PP)*repeatCount;
    float tt=ycomp(PP)/zcomp(PP)*repeatCount;
    Ct=color cellnoise(ss,tt);;
    

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

