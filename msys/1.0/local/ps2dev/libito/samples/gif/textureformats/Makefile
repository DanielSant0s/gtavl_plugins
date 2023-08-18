
EE_BIN = textureformats.elf
EE_OBJS = main.o

IMAGES  = rgba32.iif rgb24.iif rgba16.iif clut8rgba32.iif clut8rgba16.iif clut4rgba32.iif clut4rgba16.iif
IMAGES += s-rgba16.iif s-clut8rgba32.iif s-clut8rgba16.iif s-clut4rgba32.iif s-clut4rgba16.iif

CLEAN_FILES = $(IMAGES)

all : $(EE_BIN) $(IMAGES)

rgba32.iif : hedwig.bmp
	$(IIT) -p RGBA32 $< $@

rgb24.iif : hedwig.bmp
	$(IIT) -p RGB24 $< $@

rgba16.iif : hedwig.bmp
	$(IIT) -p RGBA16 $< $@

clut8rgba32.iif : hedwig.bmp
	$(IIT) -p CLUT8-RGBA32 $< $@

clut8rgba16.iif : hedwig.bmp
	$(IIT) -p CLUT8-RGBA16 $< $@

clut4rgba32.iif : hedwig.bmp
	$(IIT) -p CLUT4-RGBA32 $< $@

clut4rgba16.iif : hedwig.bmp
	$(IIT) -p CLUT4-RGBA16 $< $@

s-rgba16.iif : hedwig.bmp
	$(IIT) -s -p RGBA16 $< $@

s-clut8rgba32.iif : hedwig.bmp
	$(IIT) -s -p CLUT8-RGBA32 $< $@

s-clut8rgba16.iif : hedwig.bmp
	$(IIT) -s -p CLUT8-RGBA16 $< $@

s-clut4rgba32.iif : hedwig.bmp
	$(IIT) -s -p CLUT4-RGBA32 $< $@

s-clut4rgba16.iif : hedwig.bmp
	$(IIT) -s -p CLUT4-RGBA16 $< $@


include $(LIBITO)/Makefile.prefab
