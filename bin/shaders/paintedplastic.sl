/* paintedplastic.sl - simple st texture mapped plastic
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */


surface
paintedplastic ( float Ka = 1, Kd = .5, Ks = .5, roughness = .1;
		 color specularcolor = 1;
		 string texturename = ""; )
{
    Ci = Cs;
    if (texturename != "")
	Ci *= color texture (texturename);

    normal Nf = faceforward (normalize(N),I);
    Ci = Ci * (Ka*ambient() + Kd*diffuse(Nf)) +
	 specularcolor * Ks*specular(Nf,-normalize(I),roughness);
    Oi = Os;  Ci *= Oi;
}
