#include <CVehicle.h>

void CVehicle__SetEngineState(uint32_t *this, bool state)
{
    m_nVehicleFlags* vehFlags = getVehicleFlags(this);

    if (!vehFlags->bEngineBroken) 
        vehFlags->bEngineOn = state;
}

void (*CVehicle_Render)(uint32_t vehicle) = (void (*)(uint32_t))0x167F50;
int (*getVehicleAppearance)(uint32_t vehicle) = (int (*)(uint32_t))0x168BA0;
