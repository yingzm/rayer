/*
 * clamptoalpha.sl -- imager shader that forces Ci to be non-negative
 *    and with each component to greater than the alpha value of the
 *    pixel.
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */


imager
clamptoalpha ()
{
    Ci = clamp (Ci, color 0, color alpha);
}
