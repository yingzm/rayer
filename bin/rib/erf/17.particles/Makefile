LIBDIR=./PIXAR
CFLAGS=-I${LIBDIR}/include
LIBS=gzlib.o -L${LIBDIR}/lib -lrib -lm
RENDERER=angel

SCENE=curves

all :${SCENE}.tiff
	viewer ${SCENE}.tiff
${SCENE}.tiff : ${SCENE}.rib
	${RENDERER} ${SCENE}.rib

${SCENE}.rib : ${SCENE}.gen
	./${SCENE}.gen >${SCENE}.rib

${SCENE}.gen : ${SCENE}.c
	cc ${CFLAGS} -o ${SCENE}.gen  ${SCENE}.c ${LIBS}

clean :
	rm -f *.tiff *.rib *.gen
