#include <CVehicle.h>

void CVehicle__SetEngineState(uint32_t *this, char a2)
{
    if (*(uint32_t*)((uint32_t)this + 1077) & 0x200)
        *(uint8_t*)((uint32_t)this + 1072) &= 0xEFu;
    else
        *(uint8_t*)((uint32_t)this + 1072) ^= (*(uint8_t*)((uint32_t)this + 1072) ^ (16 * a2)) & 0x10;
}