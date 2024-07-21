#include <injector.h>
#include <rwcore.h>
#include <string.h>
#include <common.h>
#include <stdio.h>

void (*CEntity_UpdateRwFrame)(CEntity *e) = (void (*)(CEntity *e))0x22F470;

void broken_entity_tracer(CEntity *e) {
    printf("Loading model... ID: %d\n", e->m_nModelIndex);
    CEntity_UpdateRwFrame(e);
}

int _start()
{
    RedirectCall(0x27DEE0, broken_entity_tracer);
    return 0;
}
