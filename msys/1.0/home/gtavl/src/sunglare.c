
#include <injector.h>
#include <rwcore.h>
#include <secore.h>
#include <CVector.h>
#include <CVehicle.h>
#include <CCoronas.h>
#include <common.h>
#include <stdlib.h>
#include <stdio.h>
#include <CCam.h>
#include <math.h>

#include "modules.h"

extern CVector CTimeCycle_m_VectorToSun[16];

static void CVector2D_Normalise(CVector* a1)
{
    float v1; // st5
    float v2; // st7

    v1 = *(float *)a1 * *(float *)a1 + *(float *)(a1 + 4) * *(float *)(a1 + 4);
    if ( v1 > 0.0f )
    {
        v2 = 1.0f / sqrtf(v1);
        *(float *)a1 = v2 * *(float *)a1;
        *(float *)(a1 + 4) = v2 * *(float *)(a1 + 4);
    }
    else
    {
        *(float *)a1 = 1.0f;
    }
}

void CCollision_CalculateTrianglePlanes(CCollisionData* a1);

void CMatrix_mul(CVector *outPoint, CMatrix *m, CVector *point);

static void CVehicle_DoSunGlare(int this)
{
    bool v2;
    float *v3;
    int v4;
    float *v5;
    CSimpleTransform *v6;
    float v7;
    float v8;
    float v9;
    float v10;
    float v11;
    float v12;
    float v13;
    int v14;
    float v15;
    float fwdness;
    float strength;
    float intensity;
    int v19;
    int v20; 
    short *v21; 
    int v22; 
    int v23; 
    int v24; 
    int v25; 
    short *v26; 
    int v27; 
    int v28; 
    int v29; 
    short *v30; 
    int v31; 
    int v32; 
    float v33;
    int v34; 
    int v35; 
    short *v36; 
    int v37; 
    int v38; 
    float v39; 
    int v40; 
    short *v41; 
    int v42; 
    int v43;
    float v44; 
    int v45; 
    short *v46; 
    int v47; 
    int v48; 
    int v49; 
    float v50; 
    float v52; 
    uint8_t v53;
    uint8_t v54; 
    float v55;
    uint8_t v57; 
    uint8_t v58;
    bool v59; 
    uint8_t v60; 
    float v62;
    int v63;
    float radius; 
    CMatrix *v67;
    float dist_;
    int v69; 
    int v70; 
    int v71; 
    int v72; 
    int v73; 
    int v74; 
    float v75;
    int v76;
    int v77;
    CVector v79; 
    float v81;
    float v82;
    float v83;
    float v84;
    int v85; 
    int v86; 
    CVector v87;
    CVector a7; 
    int v90;
    int v91;
    int v92;
    int v93;
    int v94;
    int v95;
    CVector thisa;
    float v97;
    float v98;
    float v99;
    int v101; 
    float v102; 
    float v103; 
    float v104; 
    float v105; 
    float v106; 
    float v107; 
    float v108; 
    float v109; 
    int objects[9]; 
    float v111;
    float v112;
    float v113;
    float v114;
    float v115;
    CVector point;
    float v117;
    float v118;
    float v119;
    CVector outPoint;

	int r, g, b;

    float CWeather_SunGlare = *(float*)0x66BD74;
    int CTimeCycle_m_CurrentStoredValue = *(int*)0x66B9E4;
    CColourSet* CTimeCycle_m_CurrentColours = (CColourSet*)0x7067C0;

    v2 = (*(uint32_t *)(this + 64) & 0x20000000) == 0;
    v101 = this;
    if ( v2
      && *(float *)(*(uint32_t *)(this + 20) + 40) >= 0.0f
      && getVehicleAppearance(this) == 1
      && CWeather_SunGlare > 0.0f )
    {
        vector_constructor_403D60((char *)objects, 12, 4, (int)CVector_CVector_2);
        v3 = *(float **)(this + 20);
        v4 = 0;
        v5 = v3 + 12;
        if ( !v3 )
            v5 = (float *)(this + 4);
        if ( TheCamera.placeable.m_matrix )
            v6 = (CSimpleTransform *)&TheCamera.placeable.m_matrix->pos;
        else
            v6 = &TheCamera.placeable.placement;
        v79.x = v6->m_vPosn.x - *v5;
        v79.y = v6->m_vPosn.y - v5[1];
        v79.z = v6->m_vPosn.z - v5[2];
        dist_ = sqrtf(v79.x * v79.x + v79.z * v79.z + v79.y * v79.y);
        v7 = 2.0f / dist_;
        v81 = v79.x * v7;
        v82 = v79.y * v7;
        v83 = v79.z * v7;
        v8 = v81 + CTimeCycle_m_VectorToSun[CTimeCycle_m_CurrentStoredValue].x;
        v9 = v82 + CTimeCycle_m_VectorToSun[CTimeCycle_m_CurrentStoredValue].y;
        v10 = v83 + CTimeCycle_m_VectorToSun[CTimeCycle_m_CurrentStoredValue].z;
        thisa.x = v10 * v3[2] + v9 * v3[1] + v8 * *v3;
        v11 = v10 * v3[6];
        v12 = v9 * v3[5];
        v13 = v8 * v3[4];
        thisa.z = 0.0f;
        thisa.y = v11 + v12 + v13;
        CVector_Normalise((CVector *)&thisa);
        v14 = *(uint32_t *)(this + 20);
        v15 = *(float *)(v14 + 20);
        v87.x = *(float *)(v14 + 16);
        v87.y = v15;
        v79.y = v82;
        v79.x = v81;
        CVector2D_Normalise(&v87);
        CVector2D_Normalise(&v79);
        fwdness = v79.y * v87.y + v79.x * v87.x;
        if ( fwdness < 0.0f )
            fwdness = -fwdness;
        if ( fwdness <= 0.995f )
        {
            if ( fwdness <= 0.99000001f )
                return;
            strength = (fwdness - 0.99000001f) * 200.0f;
        }
        else
        {
            strength = 1.0f;
        }
        if ( dist_ <= 30.0f )
        {
            if ( dist_ <= 13.0f )
                return;
            strength = strength * (dist_ - 13.0f) * 0.05882353f;
        }
        intensity = strength * 0.80000001f * CWeather_SunGlare;
        r = ((float)(CTimeCycle_m_CurrentColours->m_nSunCoreRed + 510) * intensity * 0.33333334f);
        g = ((float)(CTimeCycle_m_CurrentColours->m_nSunCoreGreen + 510) * intensity * 0.33333334f);
        b = ((float)(CTimeCycle_m_CurrentColours->m_nSunCoreBlue + 510) * intensity * 0.33333334f);

        v19 = (int)CEntity_GetColModel(this)->colData;
        CCollision_CalculateTrianglePlanes((CCollisionData*)v19);
        if ( *(short *)(v19 + 4) - 2 >= 0 )
        {
            do
            {
                v20 = *(unsigned short *)(*(uint32_t *)(v19 + 24) + 8 * v4);
                v21 = (short *)(*(uint32_t *)(v19 + 20) + 6 * v20);
                v22 = *v21;
                v23 = v21[1];
                v24 = v21[2];
                v84 = (float)v22 * 0.0078125f;
                *(float *)objects = v84;
                *(float *)&v85 = (float)v23 * 0.0078125f;
                objects[1] = v85;
                *(float *)&v86 = (float)v24 * 0.0078125f;
                objects[2] = v86;
                if ( *(float *)&v86 > 0.0f )
                {
                    v25 = *(unsigned short *)(*(uint32_t *)(v19 + 24) + 8 * v4 + 2);
                    v26 = (short *)(*(uint32_t *)(v19 + 20) + 6 * v25);
                    v69 = *v26;
                    v27 = v26[1];
                    v28 = v26[2];
                    *(float *)&v90 = (float)v69 * 0.0078125f;
                    objects[3] = v90;
                    *(float *)&v91 = (float)v27 * 0.0078125f;
                    objects[4] = v91;
                    *(float *)&v92 = (float)v28 * 0.0078125f;
                    objects[5] = v92;
                    v29 = *(unsigned short *)(*(uint32_t *)(v19 + 24) + 8 * v4 + 4);
                    v30 = (short *)(*(uint32_t *)(v19 + 20) + 6 * v29);
                    v70 = *v30;
                    v31 = v30[1];
                    v32 = v30[2];
                    v33 = (float)v70 * 0.0078125f;
                    v71 = v31;
                    v34 = 0;
                    *(float *)&v93 = v33;
                    objects[6] = v93;
                    *(float *)&v94 = (float)v71 * 0.0078125f;
                    objects[7] = v94;
                    v72 = 0;
                    *(float *)&v95 = (float)v32 * 0.0078125f;
                    objects[8] = v95;
                    v35 = *(unsigned short *)(*(uint32_t *)(v19 + 24) + 8 * v4 + 8);
                    if ( v35 != v20 && v35 != v25 && v35 != v29 )
                    {
                        v36 = (short *)(*(uint32_t *)(v19 + 20) + 6 * v35);
                        v73 = *v36;
                        v37 = v36[1];
                        v38 = v36[2];
                        v39 = (float)v73 * 0.0078125f;
                        v74 = v37;
                        v34 = 1;
                        v105 = v39;
                        v111 = v105;
                        v106 = (float)v74 * 0.0078125f;
                        v112 = v106;
                        v72 = 1;
                        v107 = (float)v38 * 0.0078125f;
                        v113 = v107;
                    }
                    v40 = *(unsigned short *)(*(uint32_t *)(v19 + 24) + 8 * v4 + 10);
                    if ( v40 != v20 && v40 != v25 && v40 != v29 )
                    {
                        v41 = (short *)(*(uint32_t *)(v19 + 20) + 6 * v40);
                        v76 = *v41;
                        v42 = v41[1];
                        v43 = v41[2];
                        v44 = (float)v76 * 0.0078125f;
                        v77 = v42;
                        v34 = v72 + 1;
                        v97 = v44;
                        v111 = v97;
                        v98 = (float)v77 * 0.0078125f;
                        v112 = v98;
                        v99 = (float)v43 * 0.0078125f;
                        v113 = v99;
                    }
                    v45 = *(unsigned short *)(*(uint32_t *)(v19 + 24) + 8 * v4 + 12);
                    if ( v45 != v20 && v45 != v25 && v45 != v29 )
                    {
                        v46 = (short *)(*(uint32_t *)(v19 + 20) + 6 * v45);
                        v47 = *v46;
                        v48 = v46[1];
                        v49 = v46[2];
                        ++v34;
                        v102 = (float)v47 * 0.0078125f;
                        v111 = v102;
                        v103 = (float)v48 * 0.0078125f;
                        v112 = v103;
                        v104 = (float)v49 * 0.0078125f;
                        v113 = v104;
                    }
                    if ( v34 == 1 )
                    {
                        v119 = *(float *)&v92 + *(float *)&v86;
                        v118 = *(float *)&v90 + v84 + *(float *)&v93;
                        v114 = v111 + v118;
                        v115 = v112 + *(float *)&v91 + *(float *)&v85 + *(float *)&v94;
                        v108 = v114 * 0.25f;
                        v109 = v115 * 0.25f;
                        v50 = v108 - v84;
                        v52 = v50;
                        if ( v53 | v54 )
                            v52 = -v50;
                        v55 = v109 - *(float *)&v85;
                        v75 = v55;
                        if ( v57 | v58 )
                            v55 = -v55;
                        if ( v52 >= v55 )
                        {
                            v50 = v75;
                        }
                        else
                        {
                            v59 = v50 < 0.0f;
                            v60 = 0;
                        }
                        if ( v59 | v60 )
                            v50 = -v50;
                        v62 = v50 * 1.4f;
                        if ( v62 > 0.60000002f )
                        {
                            v63 = v101;
                            v67 = *(CMatrix **)(v101 + 20);
                            v117 = v62 * thisa.z;
                            point.x = thisa.x * v62 + v108;
                            point.y = v62 * thisa.y + v109;
                            point.z = v117 + (v113 + v119 + *(float *)&v95) * 0.25f;
							
                            CMatrix_mul((CVector *)&outPoint, v67, (CVector *)&point);

                            a7.x = outPoint.x + v81;
                            a7.y = outPoint.y + v82;
                            a7.z = outPoint.z + v83;
                            radius = CWeather_SunGlare * 0.89999998f;
                            CCoronas_RegisterCorona(v4 + v63 + 27, NULL, r, g, b, 255, &a7, radius, 90.0f, 0, 0, 0, 0, 0, 0, 0, 1.5f, 0, 15.0f, 0, 0);
                        }
                    }
                }
                v4 += 2;
            }
            while ( v4 <= *(short *)(v19 + 4) - 2 );
        }
    }
}

void CShadows_StoreShadowForVehicle(uint32_t, uint32_t);

void SunGlareInject(uint32_t vehicle, uint32_t a2) {
    CShadows_StoreShadowForVehicle(vehicle, a2);
    CVehicle_DoSunGlare(vehicle);
}
