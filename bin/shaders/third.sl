surface third ()
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
	
    Oi = Os;
    Ci = Oi * Cs * specular(Nf,V,0.1);
}

