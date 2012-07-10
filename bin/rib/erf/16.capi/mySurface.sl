surface mySurface (color customColor=color "rgb" (0,1,0);)
{
color Ct;
normal Nf=normalize(faceforward(N,I));
Ct=mix(customColor,Cs,s);
Oi=Os;
Ci=diffuse(Nf)*Ct*Oi;
}
