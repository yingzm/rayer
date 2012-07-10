surface solidNoise (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;
	
	point PP=transform("shader",P);
    Ct=color noise(PP*10);

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
                specularcolor * Ks*specular(Nf,V,roughness));

}

