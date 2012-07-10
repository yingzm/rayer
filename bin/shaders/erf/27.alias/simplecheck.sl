surface simplecheck  (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;


    float repeatCount=40;
    float ss=s*repeatCount+t;
    float tt=t*repeatCount-s;
    float filterWidthSS=abs(Du(ss)*du)+abs(Dv(ss)*dv);
    float filterWidthTT=abs(Du(tt)*du)+abs(Dv(tt)*dv);
    float smag,tmag;

    ss=mod(ss,2);
    tt=mod(tt,2);

    smag =smoothstep(0.5-filterWidthSS,0.5+filterWidthSS,ss);
    smag-=smoothstep(1.5-filterWidthSS,1.5+filterWidthSS,ss);
    smag=smag*2-1;
    
    tmag =smoothstep(0.5-filterWidthTT,0.5+filterWidthTT,tt);
    tmag-=smoothstep(1.5-filterWidthTT,1.5+filterWidthTT,tt);
    tmag=tmag*2-1;
    
    
    Ct=(smag*tmag)/2+0.5;

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

