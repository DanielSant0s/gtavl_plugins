#ifndef CTXDSTORE_H_
#define CTXDSTORE_H_

extern void (*CTxdStore_PushCurrentTxd)();
extern void (*CTxdStore_PopCurrentTxd)();
extern int (*CTxdStore_FindTxdSlot)(const char*);
extern int (*CTxdStore_AddTxdSlot)(const char*);
extern void (*CTxdStore_LoadTxd)(int, const char*);
extern void (*CTxdStore_AddRef)(int);
extern void (*CTxdStore_SetCurrentTxd)(int);
extern void (*CTxdStore_RemoveTxd)(int);
extern void (*CTxdStore_RemoveTxdSlot)(int);

#endif
