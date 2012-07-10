/* depthcue.sl - fade out with distance
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */


volume
depthcue (float mindistance = 0, maxdistance = 1;
	  color background = 0;)
{
  Ci = mix (Ci, background,
	    clamp ((depth(P)-mindistance) / (maxdistance-mindistance), 0, 1));
}
