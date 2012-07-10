for i in 20 40 80 160 320 640
do
slander -g2.0  -r300 -wtile$i.tiff tile.slc:repeatCount=$i
done
