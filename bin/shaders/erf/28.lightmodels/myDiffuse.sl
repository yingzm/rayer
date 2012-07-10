surface myDiffuse  (
	float Ka = 1;
        float Kd = .5;
        float Ks = .5;
        float roughness = .1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Cdiff=0;

    illuminance(P,Nf,PI/2)
	{
		Cdiff+=Cl*normalize(L).Nf;
	}

    Oi = Os;
    Ci = Cs*Cdiff*Oi ;
}

