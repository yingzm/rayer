/*
 * roughmetal.sl
 *
 * Description:
 *   Rough metal without coherent reflections
 *
 * Parameters:
 *   Ka, Kd, Ks - ambient, diffuse, specular weights
 *   roughness - highlight width
 * 
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */


#include "material.h"


surface
roughmetal ( float Ka = 1, Kd = 0.1, Ks = .9, roughness = 0.4; )
{
    normal Nf = faceforward (normalize(N), I);
    Ci = MaterialRoughMetal (Nf, Cs, Ka, Kd, Ks, roughness);
    Oi = Os;  Ci *= Oi;
}
