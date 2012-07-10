/*
 * wood2.sl -- another surface shader for wood.
 *
 * DESCRIPTION:
 *   Makes wood solid texture.
 * 
 * PARAMETERS:
 *   Ka, Kd, Ks, specular, roughness - work just like the plastic shader
 *   txtscale - overall scaling factor for the texture
 *   ringscale - scaling for the ring spacing
 *   lightwood, darkwood - surface colors for the wood itself
 *   grainy - relative graininess (0 = no fine grain)
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 *
 * $Revision: 1.2 $    $Date: 2000/11/14 05:53:45 $
 */


#include "material.h"


surface
wood2 ( float Ka = 1, Kd = .75, Ks = .4, roughness = .1;
        float ringscale = 15;
        float txtscale = 1;
        color lightwood = color (0.69, 0.44, 0.25);
        color darkwood  = color (0.35, 0.22, 0.08);
        float grainy = 1; )
{
    /* Calculate in shader space */
    point PP = txtscale * transform ("shader", P);

    float my_t = zcomp(PP) / ringscale;
    point PQ = point (xcomp(PP)*8, ycomp(PP)*8, zcomp(PP));
    my_t += noise (PQ) / 16;
  
    PQ = point (xcomp(PP), my_t, ycomp(PP)+12.93);
    float r = ringscale * noise (PQ);
    r -= floor (r);
    r = 0.2 + 0.8 * smoothstep(0.2, 0.55, r) * (1 - smoothstep(0.75, 0.8, r));

    /* \/--  extra line added for fine grain */
    PQ = point (xcomp(PP)*128+5, zcomp(PP)*8-3, ycomp(PP)*128+1);
    float r2 = grainy * (1.3 - noise (PQ)) + (1-grainy);

    color Ct = mix (lightwood, darkwood, r*r2*r2);

    /* Use the plastic illumination model */
    normal Nf = faceforward (normalize(N),I);
    Ci = MaterialPlastic (Nf, Ct, Ka, Kd, Ks, roughness);
    Oi = Os;  Ci *= Oi;
}
