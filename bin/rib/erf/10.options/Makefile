RENDER=angel
FILE=fovnear
#FILE=fovfar
#FILE=samples1
#FILE=exposure

VIEWER=viewer

all : ${FILE}.tiff
	${VIEWER} ${FILE}.tiff

${FILE}.tiff : ${FILE}.rib
	${RENDER} ${FILE}.rib

clean :
	rm -f *.tiff
