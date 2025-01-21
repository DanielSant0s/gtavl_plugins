#include <CVehicle.h>
#include <math.h>

void CVehicle__SetEngineState(uint32_t *this, bool state)
{
    m_nVehicleFlags* vehFlags = getVehicleFlags(this);

    if (!vehFlags->bEngineBroken) 
        vehFlags->bEngineOn = state;
}

void getVehicleDummyPos(CVector* ret, uint16_t veh_id, uint16_t dummy_id) {
	CVector* base = &(*(CVector**)(CModelInfo_ms_modelInfoPtrs[veh_id] + 0x5C))[dummy_id];
	ret->x = base->x;
	ret->y = base->y;
	ret->z = base->z;
}

float getVehicleHeading(uint32_t vehicle) {
	uint32_t v70;
	float v71;

    v70 = *(uint32_t *)(vehicle + 20);
    if ( v70 )
        v71 = atan2f(-*(float *)(v70 + 16), *(float *)(v70 + 20));
    else
        v71 = *(float *)(vehicle + 16);
	return v71;
}