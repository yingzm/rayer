displacement disk (
	float Km = 0.1;)
{
    normal NN = normalize(N);
    float mag=0;
    float fuzz=0.05;
	
    /*Calculate mag*/
    float dist=sqrt((s-0.5)*(s-0.5)+(t-0.5)*(t-0.5));
    mag=smoothstep(0.3-fuzz,0.3+fuzz,dist);


    /*Displace*/
    P=P+mag*Km*NN;
    N=calculatenormal(P);
}

