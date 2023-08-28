#ifndef RWSE_CORE_H
#define RWSE_CORE_H

typedef struct {
    unsigned long long int ate    : 1;
    unsigned long long int atst   : 3;
    unsigned long long int aref   : 8;
    unsigned long long int afail  : 2;
    unsigned long long int date   : 1;
    unsigned long long int datm   : 1;
    unsigned long long int zte    : 1;
    unsigned long long int ztst   : 2;
} rwTest;

typedef union {
    unsigned long long int pack;
    rwTest data;
} pixeltests;

typedef struct { 
    int left;    // This is the upper-left corner x-coordinate.
    int top;     // The upper-left corner y-coordinate.
    int right;   // The lower-right corner x-coordinate.
    int bottom;  // The lower-right corner y-coordinate.
} Rect;

enum GsRenderState {
    GS_SCISSORTESTENABLE,
    GS_ALPHATESTMETHOD,
    GS_ALPHATESTFAIL
};

enum GsAlphaTestFail {
    GS_ALPHATESTFAILKEEP,
    GS_ALPHATESTFAILFBONLY,
    GS_ALPHATESTFAILZBONLY,
    GS_ALPHATESTFAILRGBONLY
};

enum GsAlphaTestMethod
{
    GS_ALPHATESTMETHODNEVER,         
        /**<Always fail the test */
    GS_ALPHATESTMETHODALWAYS,   
        /**<Always pass the test */
    GS_ALPHATESTMETHODLESS,          
        /**<Accept the new pixel if its alpha value is less than the value of
         *  the reference value */
    GS_ALPHATESTMETHODLESSEQUAL,     
        /**<Accept the new pixel if its alpha value is less than or equal to 
         *  the value of the reference value */
    GS_ALPHATESTMETHODEQUAL,         
        /**<Accept the new pixel if its alpha value equals the value of the
         *  reference value */
    GS_ALPHATESTMETHODGREATEREQUAL,  
        /**<Accept the new pixel if its alpha value is greater than or equal
         *  to the value of the reference value */
    GS_ALPHATESTMETHODGREATER,       
        /**<Accept the new pixel if its alpha value is greater than the value 
         *  of the reference value */
    GS_ALPHATESTMETHODNOTEQUAL      
        /**<Accept the new pixel if its alpha value does not equal the value of 
         *  the reference value */
};

void SetScissorRect(Rect* rect);

bool GsRenderStateSet(int state, void* value);

#endif
