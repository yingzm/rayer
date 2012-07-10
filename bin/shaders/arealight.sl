/*
 * arealight.sl - light source shader appropriate for an area light source.
 *
 * DESCRIPTION:
 *   Makes light with cosine falloff from the normal of the light source
 *   surface.  This is perfect for using with area light sources.
 *
 * PARAMETERS
 *   intensity, lightcolor - same meanings as pointlight
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */

light
arealight (float intensity = 1;
	   color lightcolor = 1;)
{
#ifdef BMRT
    illuminate (P, N, 1.5707963 /* PI/2 */) {
	Cl = (intensity / (L.L)) * lightcolor;
    }
#else
    /* This is so if we use one of these lights in other renderers, it will
     * approximately work for a flat light source whose normal points
     * toward local +z.
     */
    vector Nl = normalize (vector "shader" (0,0,1));
    illuminate (point "shader" (0,0,0), Nl, PI/2) {
	Cl = (intensity  * (Nl.normalize(L) / (L.L)) * lightcolor);
    }
#endif
}
