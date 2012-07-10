RENDER=angel
#FILE=transform
#FILE=beginend
FILE=solar
VIEWER=viewer

all : ${FILE}.tiff
	${VIEWER} ${FILE}.tiff

${FILE}.tiff : ${FILE}.rib
	${RENDER} ${FILE}.rib

clean :
	rm -f *.tiff
