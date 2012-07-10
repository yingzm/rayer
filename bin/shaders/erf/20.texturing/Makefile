RENDER=prman
SLCOMPILER=shader
VIEWER=viewer
SLEXT=slc

SHADERNAME=oldRain

all : ${SHADERNAME}.tiff
	${VIEWER} ${SHADERNAME}.tiff

${SHADERNAME}.tiff : ${SHADERNAME}.${SLEXT} test.rib
	cat test.rib | sed s/XXXX/${SHADERNAME}/g | ${RENDER}

${SHADERNAME}.${SLEXT} : ${SHADERNAME}.sl
	${SLCOMPILER} ${SHADERNAME}.sl

clean :
	rm -f *.tiff *.slc
