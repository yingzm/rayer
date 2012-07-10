/* matte.sl - diffuse-only surface
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */

surface matte (float Ka = 1, Kd = 1)
{
    normal Nf = faceforward (normalize(N),I);
    Ci = Cs * (Ka * ambient() + Kd * diffuse(Nf));
    Oi = Os;  Ci *= Oi;
}
