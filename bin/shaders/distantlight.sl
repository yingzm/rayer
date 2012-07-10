/* distantlight.sl - simple unshadowed light with parallel rays
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */

light
distantlight ( float intensity = 1;
	       color lightcolor = 1;
	       point from = point "shader" (0,0,0);
	       point to = point "shader" (0,0,1); )
{
    solar (to-from, 0) {
	Cl = intensity * lightcolor;
    }
}

