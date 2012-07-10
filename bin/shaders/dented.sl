/*
 * dented.sl -- displacement shader for dents
 *
 * DESCRIPTION:
 *   Makes a surface appear worn and dented.
 * 
 * Parameters:
 *   Km - the amplitude of the dents
 *   power - controls the fractal dimension of the dents (1 looks like
 *           previously crumpled paper or cloth, 3 looks like worn metal).
 *   shadingspace - the space that the dents (and their amplitudes) are
 *                  are defined in (default: "shader")
 *   shadingfreq - the frequency of the dents
 *   Pref - if supplied, a reference mesh
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */


#include "noises.h"
#include "displace.h"
#define USE_PREF 1
#include "pshad.h"



displacement
dented ( float Km = 1, power = 3, maxoctaves = 6;
	 float truedisp = 1;
         PSHAD_PARAMS("shader",1); )
{
    GET_PSHAD;
    float magnitude = turbulence (Pshad, dPshad, maxoctaves, 2, 0.5);
    magnitude = pow (magnitude/(2*shadingfreq), power);
    N = Displace (normalize(N), shadingspace, -Km*magnitude, truedisp);
}
