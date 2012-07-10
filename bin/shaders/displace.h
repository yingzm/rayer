/************************************************************************
 * displace.h - Routines for displacing
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 *
 * $Revision: 1.4 $    $Date: 2000/11/14 05:53:41 $
 *
 ************************************************************************/



#ifndef DISPLACE_H
#define DISPLACE_H 1




/* Combine displacement and bump mapping, with units relative to 
 * a particular space.  When truedisp != 0, this function modifies
 * P as a side effect.
 *
 * Inputs:
 *    dir       direction in which to push the surface, assumed to
 *                 already be in "current" space and normalized.
 *    amp       amplitude of the actual displacement or bumping.
 *    space     the name of the coordinate system against which the
 *                 amplitude is measured.
 *    truedisp  when 1, truly displace; when 0, just bump.
 * Return value: the normal of the displaced and bumped surface,
 *                in "current" space, normalized.
 */
normal Displace (vector dir;  string space;
                 float amp; float truedisp;)
{
    extern point P;
    float spacescale = length(vtransform(space, dir));
    vector Ndisp = dir * (amp / max(spacescale,1e-6));
    P += truedisp * Ndisp;
    return normalize (calculatenormal (P + (1-truedisp)*Ndisp));
}


#endif /* DISPLACE_H */
