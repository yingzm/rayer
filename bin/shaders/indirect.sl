/* indirect.sl - retrieve radiosity data from the "ray server"
 * 
 * This shader is never called from BMRT -- the "indirect" light source
 * is built into the renderer.
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */


#include "rayserver.h"


light
indirect (output float __nonspecular = 1;)
{
#ifdef BMRT
    /* Just in case this is ever compiled for BMRT, make it do nothing. */
    Cl = 0;

#else  /* for other renderers, looks up from the ray server */

    /* Exploit the fact that PRMan (incorrectly) leaves the surface normal
     * in N.
     */
    vector axis = normalize(N);
    solar (-axis, 0) {
	Cl = rs_indirect (Ps, axis);
    }
#endif
}
