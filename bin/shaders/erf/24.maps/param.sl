surface param (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
	string mapname="";
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;


    if(mapname != "")
	Ct=color texture(mapname);
    else
	Ct=Cs;
    

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

