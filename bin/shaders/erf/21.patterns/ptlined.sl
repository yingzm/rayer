surface ptlined (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;

    color red=color "rgb" (1,0,0);
    color green=color "rgb" (0,1,0);
    color blue=color "rgb" (0,0,1);
    float fuzz=0.025;
    float dist;
    float inLine;
    point start=point (0.1,0.3,0);
    point end=point (0.7,0.7,0);
    point here=point(s,t,0);

    Ct=Cs;

    dist=ptlined(start,end,here);
    inLine=1-smoothstep(0.1-fuzz,0.1+fuzz,dist);
    Ct=mix(Ct,blue,inLine);
    

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

