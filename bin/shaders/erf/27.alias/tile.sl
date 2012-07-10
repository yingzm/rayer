surface tile  (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;
	float repeatCount=100;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;

    float ss=s*repeatCount+t;
    float tt=t*repeatCount-s;

    float sTile=floor(ss);
    float tTile=floor(tt);
    Ct=mod(sTile+tTile,2);;
    

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

