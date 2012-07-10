/* screen.sl - shader for a metalic screen.
 *
 * DESCRIPTION:
 *   Makes a surface that looks like a metal screen.  Strips of metal run
 *   parallel to lines of s and t.  You can adjust the Ka, Kd, Ks, etc.
 *   to change the material appearance.
 * 
 * PARAMETERS:
 *   Ka, Kd, Ks, roughness, specularcolor - work just like the plastic shader
 *   frequency - how many cycles of screen in st space
 *   density - how much of each cycle is opaque?
 *
 * WARNINGS:
 *   No antialiasing is performed here.
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */



surface
screen (float Ka = 1, Kd = 0.75, Ks = 0.4, roughness = 0.1;
        color specularcolor = 1;
	float density = 0.25, frequency = 20; )
{
    if (mod (s*frequency, 1) < density  ||  mod (t*frequency, 1) < density) {
	Oi = 1;
	normal Nf = faceforward (normalize(N),I);
	Ci = Os * ( Cs * (Ka*ambient() + Kd*diffuse(Nf)) +
		    specularcolor * Ks*specular(Nf,-normalize(I),roughness));
    }
    else {
	Oi = 0;
	Ci = 0;
    }
}
