/* rayserver.h - Shading Language (PRMan side) include file for ray server.
 * 
 * These macros translate calls to trace(), visibility(), and
 * rayhittest() into calls to rayserver().  Each generates different
 * numbers of args, allowing rayserver() to be polymorphic.
 *
 * It is assumed that the rayserver() function itself is implemented
 * as a DSO shadeop.  (See docs for PRMan 3.8 or later.)
 *
 * Note that the ray server expects its data in world space.
 *
 * The Blue Moon Rendering Tools (BMRT) are:
 * (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.
 */

#ifndef RAYSERVER_H
#define RAYSERVER_H

#ifndef BMRT

/* PRMan side only -- BMRT already knows these functions */

#define worldp(p) transform("world",p)
#define worldv(v) vtransform("world",v)
#define worldn(n) ntransform("world",n)

#define trace(p,d) rayserver(worldp(p), worldv(d))

#define visibility(from,to) rayserver(worldp(from), worldp(to))

#define rayhittest(p,d,phit,nhit) \
        rayserver(worldp(p), worldv(d), phit,nhit)

#define fulltrace(p,d,chit,hitdist,phit,nhit,pmiss,rmiss) \
        rayserver(worldp(p),worldv(d), chit,hitdist,phit,nhit,pmiss,rmiss)

#define areashadow(Ps,nsamples) rayserver(worldp(Ps), nsamples, __lightname)

#define rs_indirect(Ps,Ns) \
        rayserver_indirect(worldp(Ps), normalize(worldn(Ns)))

#define rs_caustic(P,I,N,L) \
	rayserver_photon(worldp(P), worldv(I), normalize(worldn(N)), L)

float raylevel () { return 0; }
float isshadowray () { return 0; }

#endif

#endif
