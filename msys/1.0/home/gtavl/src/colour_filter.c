#include <CRGBA.h>
#include <injector.h>
#include "colour_filter.h"
#include <CTexture.h>
#include <secore.h>

static RwRGBA rep_weights = { .r=140, .g=170, .b=255, .a=255 };
static RwRGBA add_weights = { .r=150, .g=180, .b=255, .a=255 };

RwRGBA customFilterReplaceColor = { .r=255, .g=255, .b=255, .a=255 };
RwRGBA customFilterAddColor =     { .r=255, .g=255, .b=255, .a=255 };

RwRGBA BlurFilterReplaceColor = { .r=255, .g=255, .b=255, .a=255 };
RwRGBA BlurFilterAddColor =     { .r=255, .g=255, .b=255, .a=255 };

int colour_filter_status = 1;

int enable_blur = 0;
bool bCustomBlurColor = false;

void CPostEffects_ColourFilter(RwRGBA, RwRGBA);
void CPostEffects_InfraredVision(RwRGBA, RwRGBA);

static bool bEnableRageFilter = false;

static bool bEnableCustomRadiosity = false;
static int CustomRadiosity_intensityLimit;
static int CustomRadiosity_filterPasses;
static int CustomRadiosity_renderPasses;
static int CustomRadiosity_intensity;

void SetCustomRadiosity(int intensityLimit, int filterPasses, int renderPasses, int intensity) {
    CustomRadiosity_intensityLimit = intensityLimit;
    CustomRadiosity_filterPasses = filterPasses;
    CustomRadiosity_renderPasses = renderPasses;
    CustomRadiosity_intensity = intensity;

    bEnableCustomRadiosity = true;
}

void ClearCustomRadiosity() {
    CustomRadiosity_intensityLimit = 0;
    CustomRadiosity_filterPasses = 0;
    CustomRadiosity_renderPasses = 0;
    CustomRadiosity_intensity = 0;

    bEnableCustomRadiosity = false;
}


void ToggleRageFilter() {
    bEnableRageFilter ^= 1;
    if (bEnableRageFilter) {
        //enable_blur = true;
    } else {
        //enable_blur = false;
    }
}

void CPostEffects_Grain(int, bool);
void CPostEffects_Radiosity(int intensityLimit, int filterPasses, int renderPasses, int intensity);

static void RenderCustomFiltering() {
    if (bEnableRageFilter) {
        CPostEffects_Grain(10, true);
        CPostEffects_Radiosity(220, 4, 4, 120);
    }

    if (bEnableCustomRadiosity) {
        CPostEffects_Radiosity(CustomRadiosity_intensityLimit, 
                               CustomRadiosity_filterPasses, 
                               CustomRadiosity_renderPasses, 
                               CustomRadiosity_intensity);
    }
    
}

static void ColourFilterHook(RwRGBA c1, RwRGBA c2) {
    if(colour_filter_status) {
        RwRGBA out1 = { .r = (float)((c1.r * rep_weights.r) / 255.0f) * (customFilterReplaceColor.r / 255.0f), 
                        .g = (float)((c1.g * rep_weights.g) / 255.0f) * (customFilterReplaceColor.g / 255.0f), 
                        .b = (float)((c1.b * rep_weights.b) / 255.0f) * (customFilterReplaceColor.b / 255.0f), 
                        .a = (float)((c1.a * rep_weights.a) / 255.0f) * (customFilterReplaceColor.a / 255.0f) };

        RwRGBA out2 = { .r = (float)((c2.r * add_weights.r) / 255.0f) * (customFilterAddColor.r / 255.0f), 
                        .g = (float)((c2.g * add_weights.g) / 255.0f) * (customFilterAddColor.g / 255.0f), 
                        .b = (float)((c2.b * add_weights.b) / 255.0f) * (customFilterAddColor.b / 255.0f), 
                        .a = (float)((c2.a * add_weights.a) / 255.0f) * (customFilterAddColor.a / 255.0f) };

        //CPostEffects_ColourFilter(out1, out2);
        if(enable_blur) {
            if (bCustomBlurColor) {
                out1.r = BlurFilterReplaceColor.r;
                out1.g = BlurFilterReplaceColor.g;
                out1.b = BlurFilterReplaceColor.b;
                out1.a = BlurFilterReplaceColor.a;

                out2.r = BlurFilterAddColor.r;
                out2.g = BlurFilterAddColor.g;
                out2.b = BlurFilterAddColor.b;
                out2.a = BlurFilterAddColor.a;
            }
            CPostEffects_InfraredVision(out1, out2);
        }
    }

    RenderCustomFiltering();
}

static void toggleBlurFilter() {
    enable_blur ^= 1;
}


void setupColourFilterPatches() {
    RedirectCall(0x515568, &ColourFilterHook);
    RedirectFunction(0x110498, &toggleBlurFilter);
}
