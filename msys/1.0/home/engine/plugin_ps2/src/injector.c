#include <injector.h>

#define jal(fun) 0x0C000000 + (fun/4)
#define j(fun) 0x08000000 + (fun/4)

void RedirectCall(unsigned int call_addr, void* addr) {
    *(unsigned int*)call_addr = jal((uint32_t)addr);
}

void RedirectJump(unsigned int jump_addr, void* addr) {
    *(unsigned int*)jump_addr = j((uint32_t)addr);
}

uint32_t ReadCall(uint32_t addr) {
    return ((uint32_t)((*(uint32_t*)addr)-0x0C000000)*4);
}

void RedirectFunction(void* old, void* new) {
    uint32_t* function = old;
    uint32_t patch = new;

    function[0] = 0x08000000+(patch/4);
    function[1] = 0;

}

int SearchRedirectCalls(uint32_t start, uint32_t end, void* old_addr, void* addr) {
    int found_calls = 0;

    uint32_t i;
    for(i = start; i <= end; i += 4) {
        if(jal((uint32_t)old_addr) == *(uint32_t*)i) {
            RedirectCall(i, addr);
            found_calls++;
        }
    }

    return found_calls;
}

void WriteByte(uint32_t address, uint8_t value) {
    *(uint8_t*)address = value;
}

void WriteWord(uint32_t address, uint16_t value) {
    *(uint16_t*)address = value;
}

void WriteDword(uint32_t address, uint32_t value) {
    *(uint32_t*)address = value;
}

void MakeNop(uint32_t address) {
    WriteDword(address, 0);
}

void MakeRetn(uint32_t address) {
    WriteDword(address, 0x03E00008);
    MakeNop(address+4);
}

void MakeBranch(uint32_t address, uint16_t offset) {
    WriteDword(address, 0x10000000 + offset);
    MakeNop(address+4);
}
