#include <stdint.h>
#include <injector.h>
#include <CVector.h>
#include <common.h>
#include <math.h>
#include "hooks.h"

static int sub_10001600(float *a1, float *a2, float *a3, float *a4)
{
    int result;
    float v5;
    float v6;
    float v7;
    float v8;
    float v9;
    float v10;
    float v11;
    float v12;
    float v13;
    float v14;
    int v15;
    float v16;
    float v17;
    float v18;
    float v19;
    float v20;
    float v21;
    float v22;
    float v23;
    float v24;
    float v25;
    float v26;
    float v27;
    float v28;
    float v29;
    float v30;
    float v31;
    float v32;
    float v33;
    float v34;
    float v35;
    float v36;
    float v37;
    float v38;
    float v39;
    float v40;
    float v41;
    float v42;
    float v43;
    float v44;
    float v45;
    float v46;

    if ( 0.0f == a2[1] )
    {
        a1[1] = 0.0f;
        if ( 0.0f == *a2 )
        {
            *a1 = 0.0f;
            result = 0;
            *a3 = 0.0f;
            *a4 = 0.0f;
            return result;
        }
        if ( *a2 <= 0.0f )
        {
            v7 = -1.0f;
            *a1 = -1.0f;
            v6 = v7 - *a2;
        }
        else
        {
            v5 = 1.0f;
            *a1 = 1.0f;
            v6 = *a2 - v5;
        }
        v27 = v6;
        v35 = fabsf(v27);
        *a3 = v35;
        v36 = fabsf(*a2);
        *a4 = v36;
        return v27 >= 0.0f;
    }
    if ( a2[1] <= 0.0f )
    {
        v26 = -1.0f;
        v9 = -1.0f;
        v8 = -1.0f - a2[1];
    }
    else
    {
        v26 = 1.0f;
        v8 = a2[1] - 1.0f;
        v9 = 1.0f;
    }
    v30 = v8;
    if ( 0.0f == *a2 )
    {
        *a1 = 0.0f;
        a1[1] = v9;
        v37 = fabsf(v30);
        *a3 = v37;
        v38 = fabsf(a2[1]);
        *a4 = v38;
        if ( v30 < 0.0f )
            return 0;
        result = 1;
    }
    else
    {
        v10 = v30;
        v43 = 1.0f * v30;
        v11 = v43;
        v44 = 1.0f + v43;
        v39 = fabsf(*a2);
        if ( v44 <= (float)v39 )
        {
            if ( *a2 <= 0.0f )
            {
                v25 = -1.0f;
                v18 = -1.0f;
                v17 = -1.0f - *a2;
            }
            else
            {
                v25 = 1.0f;
                v17 = *a2 - 1.0f;
                v18 = 1.0f;
            }
            v29 = v17;
            v45 = 1.0f * v29;
            v19 = v45;
            v46 = 1.0f + v45;
            v32 = fabsf(a2[1]);
            if ( v46 <= (float)v32 )
            {
                *a1 = v25;
                a1[1] = v26;
                v21 = v30;
                v20 = v29;
            }
            else
            {
                v20 = v29;
                v31 = a2[1] / v46 * v19;
                *a1 = v18;
                v21 = v31;
                a1[1] = a2[1] - v31;
            }
            v22 = v21;
            if ( v20 < 0.0f )
            {
                v23 = -1.0f;
                v15 = 0;
            }
            else
            {
                v23 = v39;
                v15 = 1;
            }
            v42 = v23;
            v41 = v20 / v42;
            v24 = v22;
            v13 = v20;
            v14 = v24;
        }
        else
        {
            v28 = *a2 / v44 * v11;
            *a1 = *a2 - v28;
            v12 = v9;
            v13 = v28;
            a1[1] = v12;
            v14 = v30;
            if ( v10 < 0.0f )
            {
                v15 = 0;
                v16 = v10 / -1.0f;
            }
            else
            {
                v15 = 1;
                v40 = fabsf(a2[1]);
                v16 = v10 / v40;
            }
            v41 = v16;
        }
        v33 = v14 * v14 + v13 * v13;
        v34 = sqrtf(v33);
        result = v15;
        *a3 = v34;
        *a4 = v34 * v41;
    }
    return result;
}

float rectLimitRadarPoint(float *a1)
{
    float result; // st7
    float v2; // st7
    int v3; // ecx
    float v4; // [esp+10h] [ebp-10h] BYREF
    float v5; // [esp+14h] [ebp-Ch] BYREF
    int v6[2]; // [esp+18h] [ebp-8h] BYREF

    if ( *(unsigned char*)(&FrontEndMenuManager + 101) )
    {
        v5 = a1[1] * a1[1] + *a1 * *a1;
        v5 = sqrtf(v5);
        result = v5;
    } else {
        if ( sub_10001600((float *)v6, a1, &v4, &v5) )
        {
            v2 = v4 + 1.0f;
            v3 = v6[1];
            *a1 = *(float *)v6;
            *((uint32_t *)a1 + 1) = v3;
            v4 = v2;
        }
        else
        {
            v4 = (v5 - v4) / v5;
        }
        result = v4;
    }
    return result;
}

int _start()
{
    RedirectCall(0x238974, rectLimitRadarPoint);
    RedirectCall(0x23BE14, rectLimitRadarPoint);
    RedirectCall(0x23BECC, rectLimitRadarPoint);
    RedirectCall(0x23BF18, rectLimitRadarPoint);
    RedirectCall(0x23BF80, rectLimitRadarPoint);
    RedirectCall(0x23BFE4, rectLimitRadarPoint);
    RedirectCall(0x23C048, rectLimitRadarPoint);
    RedirectCall(0x23C134, rectLimitRadarPoint);
    RedirectCall(0x23C400, rectLimitRadarPoint);
    RedirectCall(0x23C44C, rectLimitRadarPoint);
    RedirectCall(0x23C5B0, rectLimitRadarPoint);
    RedirectCall(0x23C5FC, rectLimitRadarPoint);
    RedirectCall(0x23C76C, rectLimitRadarPoint);
    RedirectCall(0x23C7B4, rectLimitRadarPoint);
    RedirectCall(0x23C914, rectLimitRadarPoint);
    RedirectCall(0x23C95C, rectLimitRadarPoint);
    RedirectCall(0x268164, rectLimitRadarPoint);
    RedirectCall(0x2681C4, rectLimitRadarPoint);
    RedirectCall(0x268488, rectLimitRadarPoint);
    RedirectCall(0x268534, rectLimitRadarPoint);
    RedirectCall(0x268818, rectLimitRadarPoint);
    RedirectCall(0x268FB8, rectLimitRadarPoint);
    RedirectCall(0x269118, rectLimitRadarPoint);
    RedirectCall(0x2696D0, rectLimitRadarPoint);
    RedirectCall(0x26A5D0, rectLimitRadarPoint);

    *(uint32_t*)(0x26F0AC) = 0; // Disable radar mask

    return 0;
}
