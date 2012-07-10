displacement simpleBump (
	float Km = 0.1;)
{
    vector NN = normalize(N);
    float mag=0;
    point PP;

    /*Calculate mag*/
    mag=sin(s*10*2*PI)*sin(t*10*2*PI);
    mag /= length(vtransform("object",NN));


    /*Displace*/
    PP=P+mag*Km*NN;
    N=calculatenormal(PP);
}

