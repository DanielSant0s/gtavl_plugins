#ifndef CTXDSTORE_H_
#define CTXDSTORE_H_

void CTxdStore_PushCurrentTxd();
void CTxdStore_PopCurrentTxd();
int CTxdStore_FindTxdSlot(const char*);
int CTxdStore_AddTxdSlot(const char*);
void CTxdStore_LoadTxd(int, const char*);
void CTxdStore_AddRef(int);
void CTxdStore_SetCurrentTxd(int);
void CTxdStore_RemoveTxd(int);
void CTxdStore_RemoveTxdSlot(int);

#endif
