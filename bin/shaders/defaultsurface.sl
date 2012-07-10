/* defaultsurface.sl - just color the surface, don't even look at lights
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */

surface
defaultsurface (float Kd = 0.8, Ka = 0.2)
{
    float d = normalize(I) . normalize(N);
    Ci = Cs * (Ka + Kd * d*d);
    Oi = Os;  Ci *= Oi;
}
