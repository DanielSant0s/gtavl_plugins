# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# $Id: Makefile.sample 1150 2005-06-12 11:42:08Z pixel $

EE_ERL = gtavl.erl

#EE_CFLAGS += -std=c99

EE_INCS += -Iplugin_sa/include/ -Iplugin_ps2/include/ -Irw_se/include/ 

GTAVL_OBJS += 	src/main.o \
				src/alloc.o \
		   	  	src/bios_alloc.o \
				src/virtual_mem.o \
				src/renderbuffer.o \
				\
				src/colour_filter.o \
				src/menu_ui.o \
				src/limits.o \
			  	src/oil_system.o \
		   	  	src/pickups.o \
           	  	src/wasted_busted.o \
		   	  	src/draw_rotate.o \
		   	  	src/v_aim.o \
		   	  	src/fov.o \
		   	  	src/radio_wheel.o \
		   	  	src/pause_menu.o \
		   	  	\
		   	  	src/veh_funcs.o \
		   	  	src/render_ped.o \
		   	  	src/weapon.o \
		   	  	src/sunglare.o \
				\
				src/hud.o \
		   	  	src/loading.o \
				src/clouds.o \
		   	  	src/gps.o \
				src/radar.o \
				src/skyedge.o \
				src/rain_fx.o \
				src/postfx.o \
				src/rubbish.o \
		   	  	src/misc.o 

EE_STDLIB = plugin_ps2/src/math.o \
		    plugin_ps2/src/string.o \
		    plugin_ps2/src/injector.o 

EE_SALIB = plugin_sa/src/common.o \
		   plugin_sa/src/CVector.o \
		   plugin_sa/src/CVehicle.o 

EE_RWSE  = rw_se/src/secore.o 

EE_OBJS = $(EE_STDLIB) $(EE_SALIB) $(EE_RWSE) $(GTAVL_OBJS)

all: $(EE_ERL)

clean:
	rm -rf *.erl src/*.o
	rm -rf plugin_sa/src/*.o
	rm -rf plugin_ps2/src/*.o

include Makefile.pref
include Makefile.eeglobal

