surface turbulance (
        float layers=4;
        float startingFreq=4;
        float gain=1;
        float lacunarity=1.9132;
		string noiseSpace="shader";
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
    point PP=ptransform(noiseSpace,P);
    PP*=startingFreq;

    for(i=0;i<layers;i+=1)
        {
	mag+=abs(float noise(PP*freq)-0.5)*2/pow(freq,gain);
        freq*=lacunarity;
	}
    Ct=mag;


    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
                specularcolor * Ks*specular(Nf,V,roughness));
}

