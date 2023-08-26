#include <stdint.h>
#include <injector.h>
#include <CCam.h>
#include <common.h>
#include "hooks.h"

void VCamera(uint32_t this, CVector *a2, float a3, float a4, float a5, char a6) {
    CVector out, obj_space;
    uint32_t playa = FindPlayerPed(-1);

    out.x = 0.3f;
    out.y = 0.0f;
    out.z = 0.0f;

    obj_space = TransformFromObjectSpace(playa, out);
    CCam_Process_FollowPed_SA(this, &obj_space, a3, a4, a5, a6);
}

void setupCameraPatches()
{  
    RedirectCall(0x202A10, VCamera);
}
