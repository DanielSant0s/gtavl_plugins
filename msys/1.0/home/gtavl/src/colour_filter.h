#include <CRGBA.h>

extern RwRGBA customFilterReplaceColor;
extern RwRGBA customFilterAddColor;

extern RwRGBA BlurFilterReplaceColor;
extern RwRGBA BlurFilterAddColor;

extern int colour_filter_status;

extern int enable_blur;
extern bool bCustomBlurColor;

void SetCustomRadiosity(int intensityLimit, int filterPasses, int renderPasses, int intensity);
void ClearCustomRadiosity();
