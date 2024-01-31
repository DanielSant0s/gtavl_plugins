#include <CVehicle.h>

void CVehicle__SetEngineState(uint32_t *this, bool state)
{
    m_nVehicleFlags* vehFlags = getVehicleFlags(this);

    if (!vehFlags->bEngineBroken) 
        vehFlags->bEngineOn = state;
}
