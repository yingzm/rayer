displacement fbmDisp (
	float Km = 0.1;
	)
{
    vector NN = normalize(N);
    float i;
    float mag=0;
    float freq=1;

    for(i=0;i<6;i+=1)
        {
		mag+=(float noise(P*freq)-0.5)*2/freq;
        freq*=2.1;
		}
    
	mag /= length(vtransform("object",NN));
	
	P=P+mag*NN*Km;
	
	N=calculatenormal(P);

}

