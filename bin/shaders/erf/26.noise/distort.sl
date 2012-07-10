surface distort (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;
    float ss=s+float noise(P*5)*0.5;
    float tt=t+float noise(P*5+point(100,100,100))*0.5;

    Ct=color cellnoise(ss*10,tt*10);

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
                specularcolor * Ks*specular(Nf,V,roughness));
}

