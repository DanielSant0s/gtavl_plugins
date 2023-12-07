# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# $Id: Makefile.sample 1150 2005-06-12 11:42:08Z pixel $

EE_ERL = skyedge.erl

EE_OBJS = src/main.o src/sunglare.o src/renderbuffer.o src/rubbish.o

all: $(EE_ERL)

clean:
	rm -rf *.erl src/*.o
	rm -rf src/*.o

include $(HOME)/engine/Makefile.pref
include $(HOME)/engine/Makefile.eeglobal

