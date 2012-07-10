surface stripes  (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;


    float repeatCount=40;
    float ss=s*repeatCount+t;
    float filterWidthSS=abs(Du(ss)*du)+abs(Dv(ss)*dv);
    float smag;

    ss=mod(ss,2);
    smag =smoothstep(0.5-filterWidthSS,0.5+filterWidthSS,ss);
    smag-=smoothstep(1.5-filterWidthSS,1.5+filterWidthSS,ss);
    
    Ct=smag;

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

