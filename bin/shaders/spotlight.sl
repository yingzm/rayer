/* spotlight.sl - simple spotlight for BMRT
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */

light
spotlight ( float intensity = 1;
	    color lightcolor = 1;
	    point from = point "shader" (0,0,0);
	    point to = point "shader" (0,0,1);
	    float coneangle = radians(30);
	    float conedeltaangle = radians(5);
	    float beamdistribution = 2; )
{
    uniform vector axis = normalize(to-from);

    illuminate (from, axis, coneangle) {
	float cosangle = (L . axis) / length(L);
	float atten = pow (cosangle, beamdistribution) / (L . L);
	atten *= smoothstep (cos(coneangle), cos(coneangle-conedeltaangle),
			     cosangle);
	Cl = atten * intensity * lightcolor;
    }
}

