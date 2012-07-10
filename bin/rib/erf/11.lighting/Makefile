RENDER=angel
FILE=pointlight
FILE=distantlight
FILE=spotlight
FILE=ambientlight
FILE=spotambient

VIEWER=viewer

all : ${FILE}.tiff
	${VIEWER} ${FILE}.tiff

${FILE}.tiff : ${FILE}.rib
	${RENDER} ${FILE}.rib

clean :
	rm -f *.tiff
