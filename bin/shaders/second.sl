surface second ()
{
    normal Nf = faceforward (normalize(N),I);
    Oi = Os;
    Ci = Cs * diffuse(Nf) * Oi;
}
