surface standard (
		float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
		color specularcolor = 1;)
{
    /*Setup*/
	normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Ct;

	/*Texturing*/
    Ct=Cs;

	/*Shading*/
    Oi = Os;
    Ci = Oi * ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
		specularcolor * Ks*specular(Nf,V,roughness));
}

