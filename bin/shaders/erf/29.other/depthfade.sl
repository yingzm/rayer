volume
depthfade (float mindistance = 1, maxdistance = 9;
	  color background = 0;)
{
    float d;
	vector Icam=vtransform("camera",I);
    d = smoothstep (mindistance, maxdistance, length(Icam));
    Ci = mix (Ci, background, d);
}
