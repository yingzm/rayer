surface fclamp  ( float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;
    float filterWidth;
    float val;
    float scale=pow(4.5,floor(t*4)+1);
    float ss=s*scale;
    float fade;
    filterWidth=abs(Du(ss)*du)+abs(Dv(ss)*dv);


    if(s>0.5)
	{
		val=noise(ss);
	}
    else
	{
	fade=smoothstep(0.2,0.6,filterWidth);
	val=(1-fade)*(float noise(ss))+fade*0.5;
	}

    Ct=val;
    

    Oi = Os;
    Ci = Oi * ( Ct );
}

