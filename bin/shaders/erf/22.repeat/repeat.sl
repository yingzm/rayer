surface repeat (
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
    float inDisk;
    float ss=mod(s*5,1);
    float tt=mod(t*5,1);
    point centre=point (0.5,0.5,0);
    point here=point (ss,tt,0);

    Ct=Cs;

    dist=distance(centre,here);
    inDisk=1-smoothstep(0.3-fuzz,0.3+fuzz,dist);
    Ct=mix(Ct,blue,inDisk);
    

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

