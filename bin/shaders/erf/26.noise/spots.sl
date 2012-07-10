surface spots (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;
    float inSpot;
    float mag=noise(P*5);

    inSpot=smoothstep(0.55,0.6,mag);
    Ct=mix(Cs,color "rgb" (0,1,0),inSpot);

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
                specularcolor * Ks*specular(Nf,V,roughness));

}

