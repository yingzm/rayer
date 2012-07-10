surface fbm  ( float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;
    float val;
    float scale=pow(4.5,floor(t*4)+1);
    float ss=s*scale;
    float tt=t*scale;
    float fade;
    float i;
    float freq=1;
    float mag=0;
    float filterWidthSS=abs(Du(ss)*du)+abs(Dv(ss)*dv);
    float filterWidthTT=abs(Du(tt)*du)+abs(Dv(tt)*dv);
    float filterWidth=filterWidthSS>filterWidthTT?filterWidthSS:filterWidthTT;


    for(i=0;i<6;i+=1)
	{
	if(s>0.5)
		{
			val=noise(ss*freq,tt*freq);
		}
	    else
		{
		fade=smoothstep(0.2,0.6,filterWidth*freq);
		val=(1-fade)*(float noise(ss*freq,tt*freq))+fade*0.5;
		}
	mag+=(val-0.5)/freq;
	freq *=2;
	}

    Ct=mag+0.5;
    

    Oi = Os;
    Ci = Oi * ( Ct );
}

