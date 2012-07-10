# The Blue Moon Rendering Tools (BMRT) are:
# (c) Copyright 1990-2000 Exluna, Inc. and Larry Gritz. All rights reserved.


SLC= slc

ARCHCODE=$(shell $(SLC) -arch)

.SUFFIXES: .slc .sl .$(ARCHCODE).slc

# The rule for compiling shading language programs
.sl.slc:
	$(SLC) $*.sl

ifeq ($(findstring intelnt, $(ARCH)), intelnt)
IMPORTLIBOPTS = -l ../lib/$(ARCH)/rendrib.lib
endif

SLCDSOOPTS = -dso $(IMPORTLIBOPTS)

.sl.$(ARCHCODE).slc:
	$(SLC) $(SLCDSOOPTS) $*.sl

SHADERS = ambientlight.sl arealight.sl \
	background.sl brick.sl brickbump.sl brushedmetal.sl \
	castucco.sl ceramic.sl ceramictiles.sl \
	clamptoalpha.sl clay.sl constant.sl \
	defaultsurface.sl dented.sl depthcue.sl distantlight.sl \
	fog.sl funkyglass.sl \
	glass.sl gmarbtile_polish.sl greenmarble.sl \
	lensflare.sl \
	matte.sl metal.sl \
	null.sl fakesky.sl \
	oak.sl oakplank.sl \
	paintedplastic.sl parquet_plank.sl plank.sl \
	plastic.sl pointlight.sl \
	roughmetal.sl \
	screen.sl screen_aa.sl shiny.sl shinymetal.sl shinyplastic.sl \
	smoke.sl spotlight.sl stucco.sl superplank.sl supertexmap.sl \
	uberlight.sl \
	veinedmarble.sl \
	wallpaper_2stripe.sl windowlight.sl wood2.sl


COMPILED_SHADERS = $(SHADERS:.sl=.slc)

COMPILED_SHADERS += $(SHADERS:.sl=.$(ARCHCODE).slc)


all: $(COMPILED_SHADERS) $(SLC)
	@- rm -f shaders/so_locations


clean::
	@- rm -f *~ *.slo *.slc

