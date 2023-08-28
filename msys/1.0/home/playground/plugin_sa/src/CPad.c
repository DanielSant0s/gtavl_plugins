#include <CPad.h>

CPad* (*CPad_GetPad)(int) = ( CPad* (*)(int) )0x248E70;
void (*CPad_UpdatePads)(void) = (void (*)(void))0x247940;

