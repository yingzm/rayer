giles hiNoise.sl

for i in 2 4 8 16 32 
do
echo $i
slander -r300 -g2.0 -wnoise${i}.tiff hiNoise.slc:Ks=0.1:repeatCount=$i
done
