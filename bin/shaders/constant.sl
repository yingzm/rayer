/* constant.sl - just slap a color on a surface
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */

surface
constant (/* no parameters*/)
{
    Oi = Os;
    Ci = Oi * Cs;
}

