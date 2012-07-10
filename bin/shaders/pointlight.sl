/* pointlight.sl - simple unshadowed point light source
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */

light
pointlight (float intensity = 1;
	    color lightcolor = 1;
	    point from = point "shader" (0,0,0);)
{
    illuminate (from) {
	Cl = intensity * lightcolor / (L . L);
    }
}
