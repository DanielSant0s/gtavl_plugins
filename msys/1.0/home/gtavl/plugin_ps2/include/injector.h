#ifndef INJECTOR_H_
#define INJECTOR_H_

#include <stdint.h>

void RedirectCall(unsigned int call_addr, void* addr);

void RedirectJump(unsigned int jump_addr, void* addr);

void RedirectFunction(void* old, void* new);

uint32_t ReadCall(uint32_t addr);

int SearchRedirectCalls(uint32_t start, uint32_t end, void* old_addr, void* addr);

void WriteByte(uint32_t address, uint8_t value);

void WriteWord(uint32_t address, uint16_t value);

void WriteDword(uint32_t address, uint32_t value);

void WriteFloat(uint32_t address, float value);

void MakeNop(uint32_t address);

void MakeRetn(uint32_t address);

void MakeBranch(uint32_t address, uint16_t offset);

void CallOnEvent(uint32_t address, uint16_t event, bool after);


#endif
