#include <CTxdStore.h>

void (*CTxdStore_PushCurrentTxd)() = (void (*)())0x3014B0;
void (*CTxdStore_PopCurrentTxd)() = (void (*)())0x3014D0;
int (*CTxdStore_FindTxdSlot)(const char*) = (int (*)(const char*))0x300E60;
int (*CTxdStore_AddTxdSlot)(const char*) = (int (*)(const char*))0x300D50;
void (*CTxdStore_LoadTxd)(int, const char*) = (void (*)(int, const char*))0x300F90;
void (*CTxdStore_AddRef)(int) = (void (*)(int))0x3014F0;
void (*CTxdStore_SetCurrentTxd)(int) = (void (*)(int))0x301460;
void (*CTxdStore_RemoveTxd)(int) = (void (*)(int))0x3013C0;
void (*CTxdStore_RemoveTxdSlot)(int) = (void (*)(int))0x300DB0;