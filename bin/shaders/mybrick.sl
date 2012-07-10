#define BRICKWIDTH	0.25
#define BRICKHEIGHT	0.08
#define MORTARTHICKNESS	0.01

#define BMWIDTH	(BRICKWIDTH+MORTARTHICKNESS)
#define BMHEIGHT (BRICKHEIGHT+MORTARTHICKNESS)

#define MWF	(MORTARTHICKNESS*0.5/BMWIDTH)
#define MHF	(MORTARTHICKNESS*0.5/BMHEIGHT)

surface mybrick	(
	uniform float Ka = 1;
	uniform float Kd = 1;
	uniform color Cbrick = (0.5, 0.15, 0.14);
	uniform color Cmortar = (0.5, 0.5, 0.5);
	uniform float Km = 0.1;
	)
{
	color Ct;
	normal Nf;
	float ss, tt, sbrick, tbrick, w, h;
	float scoord = s;
	float tcoord = t;
	float sbump, tbump, stbump;
	
	Nf = normalize(faceforward(N, I));
	
	ss = scoord/BMWIDTH;
	tt = tcoord/BMWIDTH;
	
	if (mod(tt*0.5, 1)>0.5)
		ss+=0.5;
		
	sbrick = floor(ss);
	tbrick = floor(tt);
	ss -= sbrick;
	tt -= tbrick;
	
	w = step(MWF, ss) - step(1-MWF, ss);
	h = step(MHF, tt) - step(1-MHF, tt);
	sbump = smoothstep(0, MWF, ss) - smoothstep(1-MWF, 1, ss);
	tbump = smoothstep(0, MHF, tt) - smoothstep(1-MHF, 1, tt);
	stbump = sbump+tbump;
	
	Nf = calculatenormal(P+normalize(N)*stbump*Km);
	Nf = normalize(faceforward(Nf, I));
	
	Ct = mix(Cmortar, Cbrick, w*h);
	
	Oi = Os;
	Ci = Os*Ct*(Ka*ambient()+Kd*diffuse(Nf));
}