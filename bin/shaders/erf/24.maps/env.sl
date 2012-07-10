surface env ()
{
    color Ct;
    vector Iworld=normalize(vtransform("world",I));
    Ct=color environment("studio2.jpg",Iworld);

    Oi = Os;
    Ci = Ct*Oi;
}

