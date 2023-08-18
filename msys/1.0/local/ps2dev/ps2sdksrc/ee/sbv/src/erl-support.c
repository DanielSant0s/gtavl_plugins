/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# $Id: erl-support.c 1604 2009-09-10 16:51:05Z jim $
# The erl-tags support
*/

#include <erl.h>

char * erl_id = "libsbv";

char * erl_dependancies[] = {
    "libkernel",
    0
};
