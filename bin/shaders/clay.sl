/************************************************************************
 * clay.sl -- simple clay surface using MaterialClay
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 ************************************************************************/


#include "material.h"


surface
clay ( float Ka = 1, Kd = 0.7, roughness = 0.1;)
{
    normal Nf = faceforward (normalize(N), I);
    Ci = MaterialClay (Nf, Cs, Ka, Kd, roughness);
    Oi = Os;  Ci *= Oi;
}
