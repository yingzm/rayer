surface random (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;

    float fuzz=0.025;
    float repeatCount=5;
    float ss=mod(repeatCount*s,1);
    float tt=mod(repeatCount*t,1);
    point centre=point (0.5,0.5,0);
    point here=point (ss,tt,0);
    float dist=distance(centre,here);
    float radius=float cellnoise(repeatCount*s,repeatCount*t)*0.4;
    color myColor=color cellnoise(repeatCount*s,repeatCount*t);
    float inDisk=1-smoothstep(radius-fuzz,radius+fuzz,dist);

    Ct=mix(Cs,myColor,inDisk);
    

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

