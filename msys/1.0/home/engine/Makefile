# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# $Id: Makefile.sample 1150 2005-06-12 11:42:08Z pixel $

EE_LIB = engine.a

EE_STDLIB = plugin_ps2/src/math.o \
		    plugin_ps2/src/stdio.o \
		    plugin_ps2/src/sifdev.o \
		    plugin_ps2/src/stdlib.o \
		    plugin_ps2/src/string.o \
		    plugin_ps2/src/injector.o \
			plugin_ps2/src/main.o

EE_SALIB = plugin_sa/src/common.o \
		   plugin_sa/src/CVector.o \
		   plugin_sa/src/CVehicle.o \
		   plugin_sa/src/CTexture.o \
		   plugin_sa/src/CPad.o \
		   plugin_sa/src/CTxdStore.o \
		   plugin_sa/src/CRadar.o \
		   plugin_sa/src/CRGBA.o \
		   plugin_sa/src/CFont.o \
		   plugin_sa/src/CCam.o \
		   plugin_sa/src/rwcore.o

#EE_READINI = readini/src/readini.o

EE_RWSE  = rw_se/src/secore.o 

EE_OBJS = $(EE_STDLIB) $(EE_SALIB) $(EE_RWSE) 

all: $(EE_LIB)

clean:
	rm -rf *.erl src/*.o
	rm -rf rw_se/src/*.o
	rm -rf plugin_sa/src/*.o
	rm -rf plugin_ps2/src/*.o

include Makefile.pref
include Makefile.eeglobal

