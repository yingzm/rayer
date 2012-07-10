surface marble (
        float layers=4;
        float startingFreq=1;
        float gain=1;
        float lacunarity=1.9132;
		string noiseSpace="shader";
		float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
		color specularcolor = 1;
		float scale=0.04)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;
    float i;
    float mag=0;
    float freq=startingFreq;
    point PP=ptransform(noiseSpace,P);
    PP*=scale;

    for(i=0;i<layers;i+=1)
        {
		mag+=abs(float noise(PP*freq)-0.5)*2/freq;
        freq*=lacunarity;
	}

    mag=smoothstep(0,0.4,mag);

    Ct=spline(mag,
	color "rgb" (0.25,0.35,0.25),
	color "rgb" (0.25,0.35,0.25),
	color "rgb" (0.20,0.30,0.20),
	color "rgb" (0.20,0.30,0.20),
	color "rgb" (0.20,0.30,0.20),
	color "rgb" (0.25,0.35,0.35),
	color "rgb" (0.25,0.35,0.35),
	color "rgb" (0.15,0.25,0.10),
	color "rgb" (0.15,0.25,0.10),
	color "rgb" (0.10,0.20,0.10),
	color "rgb" (0.10,0.20,0.10),
	color "rgb" (0.25,0.35,0.25),
	color "rgb" (0.10,0.10,0.20)
	);


    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
                specularcolor * Ks*specular(Nf,V,roughness));
}

