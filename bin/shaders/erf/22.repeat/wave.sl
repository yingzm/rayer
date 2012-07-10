surface wave (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;

    color green=color "rgb" (0,1,0);
    float fuzz=0.025;

    float ss=s+sin(t*2*PI)*0.4;
    float dist=abs(ss-0.5);
    float inLine=1-smoothstep(0.1-fuzz,0.1+fuzz,dist);

    Ct=mix(Cs,green,inLine);
    

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

