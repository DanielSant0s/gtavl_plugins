#include <CRGBA.h>
#include <injector.h>
#include "colour_filter.h"

RwRGBA rep_weights = { .r=140, .g=170, .b=255, .a=255 };
RwRGBA add_weights = { .r=150, .g=180, .b=255, .a=255 };

int colour_filter_status = 1;

int enable_blur = 0;

void (*CPostEffects_ColourFilter)(RwRGBA, RwRGBA) = (void (*)(RwRGBA, RwRGBA))0x519F70;
void (*CPostEffects_InfraredVision)(RwRGBA, RwRGBA) = (void (*)(RwRGBA, RwRGBA))0x5162B0;

void ColourFilterHook(RwRGBA c1, RwRGBA c2) {
    if(colour_filter_status) {
        RwRGBA out1 = { .r = (c1.r * rep_weights.r) / 255, 
                        .g = (c1.g * rep_weights.g) / 255, 
                        .b = (c1.b * rep_weights.b) / 255, 
                        .a = (c1.a * rep_weights.a) / 255 };

        RwRGBA out2 = { .r = (c2.r * add_weights.r) / 255, 
                        .g = (c2.g * add_weights.g) / 255, 
                        .b = (c2.b * add_weights.b) / 255, 
                        .a = (c2.a * add_weights.a) / 255 };

        CPostEffects_ColourFilter(out1, out2);
        if(enable_blur) {
            CPostEffects_InfraredVision(out1, out2);
        }
    }
}

void toggleBlurFilter() {
    enable_blur ^= 1;
}


void setupColourFilterPatches() {
    RedirectCall(0x515568, ColourFilterHook);
    RedirectFunction(0x110498, toggleBlurFilter);
}
