/*
 * background.sl -- imager shader to put a solid background color behind
 *                  an image.
 *
 * DESCRIPTION:
 *   Puts a solid background color in all the pixels which do not contain
 *   foreground objects.
 * 
 * PARAMETERS:
 *   bgcolor - the color of the background
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */



imager background (color bgcolor = 1, background = 1)
{
    Ci += (1-alpha) * (bgcolor * background);
    Oi = 1;
    alpha = 1;
}
