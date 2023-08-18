#ifndef __R5900DISASM_H__
#define __R5900DISASM_H__
 
#include <tamtypes.h>
#include "sio_shell.h"

void disasm(const u32* addr, int num);
void disasm_verbose(const struct ee_regs* regs, const u32* addr);

#endif
