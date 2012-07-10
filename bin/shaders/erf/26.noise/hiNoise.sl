surface hiNoise (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	float repeatCount=10;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;
    Ct=float noise(s*repeatCount,t*repeatCount);

    Oi = Os;
    Ci = Ct;
}

