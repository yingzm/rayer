/* metal.sl - simple metal surface
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */


surface
metal (float Ka = 1, Ks = 1;
       float roughness = .1;)
{
    normal Nf = faceforward (normalize(N),I);
    vector V = -normalize(I);
    Ci = Cs * (Ka*ambient() + Ks*specular(Nf,V,roughness));
    Oi = Os;  Ci *= Oi;
}

