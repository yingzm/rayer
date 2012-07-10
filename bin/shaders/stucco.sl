/*
 * stucco.sl -- displacement shader for stucco
 *
 * DESCRIPTION:
 *   Displacees a surface to make it look like stucco.
 * 
 * PARAMETERS:
 *   Km 	   	the amplitude of the stucco pimples
 *   power	   	controls the shape of the pimples
 *   frequency  	the frequency of the pimples
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */



displacement
stucco ( float Km = 0.05, power = 5, frequency = 10; )
{
    point PP = transform ("shader", P);
    float magnitude = Km * pow (noise (PP*frequency), power);
    P += magnitude * normalize (N);
    N = calculatenormal (P);
}
