/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# $Id: erl-loader.c 1150 2005-06-12 11:42:08Z pixel $
# Small erl loader to provide a startup core.
*/

#include <string.h>
#include <erl.h>

extern struct export_list_t {
    char * name;
    void * pointer;
} export_list[];

static char * prohibit_list[] = {
    "_edata", "_end", "_end_bss", "_fbss", "_fdata", "_fini",
    "_ftext", "_gp", "_init", "main", 
    0
};

static void export_symbols() {
    struct export_list_t * p;
    int i, prohibit;
    
    for (p = export_list; p->name; p++) {
	prohibit = 0;
	for (i = 0; prohibit_list[i]; i++) {
	    if (!(strcmp(prohibit_list[i], p->name))) {
		prohibit = 1;
		break;
	    }
	}
	if (!prohibit)
	    erl_add_global_symbol(p->name, (u32) p->pointer);
    }
}

typedef void (*func_t)(void);

int main(int argc, char ** argv) {
    struct symbol_t * s;

    strcpy(_init_erl_prefix, "host:");

    export_symbols();
    
	_init_load_erl_from_file("hello.erl", 0);
    
    if ((s = erl_find_symbol("main"))) {
	((func_t) s->address)();
    }
    
    return 0;
}
