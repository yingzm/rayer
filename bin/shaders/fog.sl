/* fog.sl - simple exponential fog effect
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */


volume
fog (float distance = 1;
     color background = 0;)
{
    float d = 1 - exp (-length(I)/distance);
    Ci = mix (Ci, background, d);
    Oi = mix (Oi, color 1, d);
}
