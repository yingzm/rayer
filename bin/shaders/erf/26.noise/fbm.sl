surface fbm (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;
    float i;
    float mag=0;
    float freq=1;

    for(i=0;i<6;i+=1)
        {
	mag+=(float noise(P*freq)-0.5)*2/freq;
        freq*=2.1;
	}
    Ct=mag+0.5;


    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
                specularcolor * Ks*specular(Nf,V,roughness));
}

