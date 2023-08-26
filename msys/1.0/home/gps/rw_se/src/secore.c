
#include <rwcore.h>
#include <secore.h>

#   define MAKE128(RES, MSB, LSB) \
    __asm volatile ( "pcpyld %0, %1, %2" : "=r" (RES) : "r" (MSB), "r" (LSB))

#define _rwDMAPktPtr (*(__uint128_t**)0x66C170)

int (*_rwDMAOpenGIFPkt)(RwUInt32, RwUInt32) = (int (*)(RwUInt32, RwUInt32))0x34E378;
int (*_rwDMAOpenVIFPkt)(RwUInt32, RwUInt32) = (int (*)(RwUInt32, RwUInt32))0x34E878;

#define RWDMA_FIXUP     0x80000000

#define GIF_AD 0x0E
#define GS_TEST_1     0x47
#define GS_SCISSOR_1    0x40

#define GIF_TAG(NLOOP,EOP,PRE,PRIM,FLG,NREG) \
                ((unsigned long long int)(NLOOP)	<< 0)	| \
                ((unsigned long long int)(EOP)	<< 15)	| \
                ((unsigned long long int)(PRE)	<< 46)	| \
                ((unsigned long long int)(PRIM)	<< 47)	| \
                ((unsigned long long int)(FLG)	<< 58)	| \
                ((unsigned long long int)(NREG)	<< 60);


#define ADDTOPKT(_a, _b) { __uint128_t _c; MAKE128(_c, _a, _b); *_rwDMAPktPtr++ = _c; }

void SetScissorRect(Rect* rect){
    unsigned long long int tmp;

    _rwDMAOpenVIFPkt(RWDMA_FIXUP, 2);

    tmp = GIF_TAG(1,1,0,0,0,1);
    ADDTOPKT(GIF_AD, tmp);

    tmp = (rect->left & 0x7ffl) | ((rect->right-1) << 16) | ((rect->top & 0x7ffl) <<32) | (((long)(rect->bottom-1)) << 48);
    ADDTOPKT(GS_SCISSOR_1, tmp);
}

void SetTests(pixeltests test){
    unsigned long long int tmp;

    _rwDMAOpenVIFPkt(RWDMA_FIXUP, 2);

    tmp = GIF_TAG(1,1,0,0,0,1);
    ADDTOPKT(GIF_AD, tmp);

    tmp = test.pack;

    ADDTOPKT(GS_TEST_1, tmp);
}

bool GsRenderStateSet(int state, void* value) {
    pixeltests def_test;
    def_test.pack = *(unsigned long long int*)0x667930;

    switch (state)
    {
    case GS_SCISSORTESTENABLE:
        {
            Rect rect = { .left = 0, .right = 640, .top = 0, .bottom = 448 };
            if (!value) {
                SetScissorRect(&rect);
            } 

        }
        break;
    case GS_ALPHATESTMETHOD:
        {
            def_test.data.atst = (int)value;
            SetTests(def_test);
        }
        break;
    case GS_ALPHATESTFAIL:
        {
            def_test.data.afail = (int)value;
            SetTests(def_test);
        }
        break;
    
    default:
        break;
    }

}
