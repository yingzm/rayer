/* ambientlight.sl - simple ambient lighth source
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */


light
ambientlight (float intensity = 1;
	      color lightcolor = 1;)
{
    Cl = intensity * lightcolor;
    L = 0;
}
