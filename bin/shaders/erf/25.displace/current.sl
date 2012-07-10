displacement current (
	float Km = 0.1;)
{
    normal NN = normalize(N);
    float mag=0;

    /*Calculate mag*/
    mag=sin(s*10*2*PI)*sin(t*10*2*PI);


    /*Displace*/
    P=P+mag*Km*NN;
    N=calculatenormal(P);
}

