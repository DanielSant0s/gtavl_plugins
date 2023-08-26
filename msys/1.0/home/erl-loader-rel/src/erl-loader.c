#include "erl.h"
#include "recycle.h"

/* And our global variables. */
struct erl_record_t * erl_record_root;

htab * global_symbols;
htab * loosy_relocs;

struct dependancy_t * dependancy_root;

reroot * symbol_recycle;


struct export_list_t {
    char * name;
    void * pointer;
} export_list[] = {
    { "sinf",    0x53B178 },
    { "cosf",    0x53AE48 },
    { "atan2f",  0x53B338 },
    { "floorf",  0x53AF48 },
    { "powf",    0x537468 },
    { "printf",  0x54F7D0 },
    { "sprintf", 0x550F10 },
    { "malloc",  0x1005E0 },
    { "free",    0x100780 },
    { "memset",  0x54E568 },
    { "memcpy",  0x54E3B0 },
    { "memcmp",  0x54E318 },
    { "strlen",  0x5517B8 },
    { "strcmp",  0x551558 },
    { "strcpy",  0x5516A0 },
    { "strcat",  0x551298 },
    { "strncpy", 0x551D80 },
    { "strncmp", 0x551BC8 },
    { 0, 0 }
};

static char * prohibit_list[] = {
    "_edata", "_end", "_end_bss", "_fbss", "_fdata", "_fini",
    "_ftext", "_gp", "_init", "main",
    0
};

static void export_symbols() {
    struct export_list_t * p;

    for (p = export_list; p->name; p++) {
    int i;
    int prohibit;
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

int _start() {
    erl_record_root = *(struct erl_record_t **)0x8924D8;
    global_symbols = *(htab **)0x8924DC;
    loosy_relocs = *(htab **)0x8924E0;
    dependancy_root = *(struct dependancy_t ** )0x8924E4;
    symbol_recycle = *(reroot **)0x8924E8;

    *(struct erl_record_t **)0x394148 = *(struct erl_record_t **)0x8924D8;
    *(htab **)0x39414C = *(htab **)0x8924DC;
    *(htab **)0x394150 = *(htab **)0x8924E0;
    *(struct dependancy_t ** )0x394154 = *(struct dependancy_t ** )0x8924E4;
    *(reroot **)0x394158 = *(reroot **)0x8924E8;

    *(u32*)0x39415C = load_plugin;
    *(u32*)0x394160 = unload_erl;
    *(u32*)0x394164 = find_erl;
    *(u32*)0x394168 = erl_find_symbol;
    *(u32*)0x39416C = erl_find_local_symbol;

    export_symbols();

    *(struct erl_record_t **)0x8924D8 = erl_record_root;
    *(htab **)0x8924DC = global_symbols;
    *(htab **)0x8924E0 = loosy_relocs;
    *(struct dependancy_t ** )0x8924E4 = dependancy_root;
    *(reroot **)0x8924E8 = symbol_recycle;

    return 0;
}
