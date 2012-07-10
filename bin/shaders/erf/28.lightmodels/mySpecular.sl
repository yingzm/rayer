surface mySpecular  (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Cspec=0;

    illuminance(P,Nf,PI/2)
	{
	Cspec+=Cl*pow(normalize(V+normalize(L)).Nf,1/roughness);
	}

    Oi = Os;
    Ci = Cs*Cspec*Oi ;
}

