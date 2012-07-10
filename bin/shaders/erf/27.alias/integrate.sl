surface integrate  ( float Ka = 1;
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
    float scale=pow(10,floor(t*4)+1)*2;
    float ss=s*scale;
    filterWidth=abs(Du(ss)*du)+abs(Dv(ss)*dv);


    if(s>0.5)
	{
		val=cos(ss);
	}
    else
	{
		val=(sin(ss+filterWidth)-sin(ss))
			/filterWidth;
	}

    Ct=val*0.5+0.5;
    

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

