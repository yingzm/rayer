RENDER=angel
FILE=red
#FILE=beginend
#FILE=opacity
#FILE=shading

VIEWER=viewer

all : ${FILE}.tiff
	${VIEWER} ${FILE}.tiff

${FILE}.tiff : ${FILE}.rib
	${RENDER} ${FILE}.rib

clean :
	rm -f *.tiff
