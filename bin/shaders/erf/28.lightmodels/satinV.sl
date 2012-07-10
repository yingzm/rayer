surface satinV  (
        float roughness = 0.1;
	color specularcolor = 1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    color Caniso=0;
	vector dir=normalize(Dv(P));

    illuminance(P,Nf,PI/2)
		{
		vector H=normalize(normalize(V)+normalize(L));
		float scale=sqrt(1-(H.dir*H.dir));
		Caniso+=Cl*pow(scale,1/roughness);
		}

    Oi = Os;
    Ci = Os * (Caniso+Cs*(ambient()+0.2*diffuse(Nf)));
}

