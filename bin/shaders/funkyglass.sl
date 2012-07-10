/* funkyglass.sl - randomly colored "glass" (transparent, but no refl/refr).
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */



surface
funkyglass (float Ka = .2;
	    float Kd = .2;
	    float Ks = 1;
	    float roughness = .08;
	    color specularcolor = 1;)
{
    vector V = normalize(I);
    normal Nf = faceforward (normalize(N),V);
    point PP = transform ("shader", P);
    color Ct = 2 * (color noise (PP) - .5) + .5;
    color Ot = (comp(Ct,0) + comp(Ct,1) + comp(Ct,2))/3 + (1-Ct);

    Oi = Ot * (0.75 - 0.5*abs(V.Nf));
    Ci = ( Ct * (Ka*ambient() + Kd*diffuse(Nf)) +
	   specularcolor * Ks*specular(Nf,-V,roughness));
}

