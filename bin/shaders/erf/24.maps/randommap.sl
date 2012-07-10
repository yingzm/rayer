surface randommap (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;
    float repeatCount=5;
    float ss=mod(s*repeatCount,1);
    float tt=mod(t*repeatCount,1);



    if(float cellnoise(s*repeatCount,t*repeatCount)>0.5)
	    Ct=color texture("pebbles.tiff",ss,tt);
    else
	    Ct=color texture("rock.tiff",ss,tt);
    

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

