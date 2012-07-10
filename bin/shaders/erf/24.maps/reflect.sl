surface reflect (
	float Ka = 1;
        float Kd = .5;
        float Ks = .4;
        float Kr = .3;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;
    vector Rcurrent=reflect(I,Nf);
    vector Rworld=vtransform("world",Rcurrent);
    color Cr=color environment("studio2.jpg",Rworld);

    Ct=Cs;

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * (Ks*specular(Nf,V,roughness)+Kr*Cr));
}

