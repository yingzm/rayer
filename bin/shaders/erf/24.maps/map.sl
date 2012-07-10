surface map (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;


    Ct=color texture("pebbles.tiff");
	
	#The image in the book is actually:
    #Ct=color texture("pebbles.tiff",s*2,t);
    #I cheated it so it looked nice...

    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

