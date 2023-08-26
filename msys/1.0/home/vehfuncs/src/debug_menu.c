#include <CPad.h>
#include <stdio.h>
#include <CFont.h>
#include <injector.h>
#include <CRGBA.h>
#include <CVector.h>
#include <common.h>
#include <CTexture.h>
#include <CTxdStore.h>
#include <math.h>
#include <string.h>
#include "hooks.h"
#include "colour_filter.h"

static CPad* pads;

static RwRGBA selected_color;
static RwRGBA unselected_color;

static RwRGBA font_color;
static RwRGBA font_shadow;

static int display_info = 0, display_menu = 0;
static CVector player_coords;
static char tmp_str[1024], tmp_fps[16];
static uint32_t start_time = 0, end_time = 0, frames = 0;
static float elapsed_time = 0.0f;
static int menu_ptr = 0;

#define drawhud_call 0x2471E4

#define debugstats_row(val) (val*20)+5
#define debugmenu_row(val) (val*20)+100

#define menu_size(menu) sizeof(menu) / sizeof(menu[0])

#define justPressed(pad, button) (pad->NewState.button && !pad->OldState.button)

static unsigned int* strmem_used = (unsigned int*)0x66BE0C;
static unsigned int* strmem_total = (unsigned int*)0x666E08;

static void (*drawhud)() = NULL;
static void (*currentMenu)();

static uint32_t current_peds = 0;
static uint32_t total_peds = 74;

void (*CPlantMgr_render)() = (void (*)())0x489380;
static int enable_grass = 1;

void (*CPed_ctor)(void*, int) = (void (*)(void*, int))0x18FD10;
void (*CPed_dtor)(void*) = (void (*)(void*))0x190D60;

void dummy_func() { 
    if((pads->NewState.ButtonCross && !pads->OldState.ButtonCross)) {
        printf("Hi! I'm a dummy function\n");
    }
}

typedef struct {
    const char* name;
    void (*func)();
    void* data;
} MenuItem;

void setupDebugInfoText() {
    CFont_SetOrientation(ALIGN_LEFT);
    CFont_SetColor(&font_color);
    CFont_SetBackground(0, 0);
    CFont_SetWrapx(640.0f);
    CFont_SetScale(0.42f, 1.0f);
    CFont_SetFontStyle(FONT_SUBTITLES);
    CFont_SetProportional(1);
    CFont_SetDropShadowPosition(0);
    CFont_SetDropColor(&font_shadow);
    CFont_SetEdge(1);
}

void setupDebugMenuText() {
    CFont_SetOrientation(ALIGN_RIGHT);
    CFont_SetColor(&selected_color);
    CFont_SetBackground(0, 0);
    CFont_SetWrapx(640.0f);
    CFont_SetScale(0.42f, 1.0f);
    CFont_SetFontStyle(FONT_SUBTITLES);
    CFont_SetProportional(1);
    CFont_SetDropShadowPosition(0);
    CFont_SetDropColor(&font_shadow);
    CFont_SetEdge(1);
}

void processMenu(MenuItem *menu, uint32_t size, int *ptr) {
    setupDebugMenuText();

    if((pads->NewState.DPadUp && !pads->OldState.DPadUp) && *ptr > 0) {
        *ptr = *ptr - 1;
    }

    if((pads->NewState.DPadDown && !pads->OldState.DPadDown) && *ptr < size-1) {
        *ptr = *ptr + 1;
    }

    int i;
    for(i = 0; i < size; i++) {
        if(*ptr == i) {
            CFont_SetColor(&selected_color);
            menu[i].func();
        } else {
            CFont_SetColor(&unselected_color);
        }
        CFont_PrintString(630, debugmenu_row(i), menu[i].name);
    }
}

void showDebugMenu();
void showScreenMenu();
void showGraphicsMenu();
void toggle_vsync();
void toggle_grass();
void vehDensity();
void pedDensity();

void waitDebugMenu() {
    if((pads->NewState.ButtonCross && !pads->OldState.ButtonCross) &&
       (pads->NewState.DPadRight && !pads->OldState.DPadRight)) {
        currentMenu = showDebugMenu;
    }
}

void open_screen_menu() { 
    if((pads->NewState.ButtonCross && !pads->OldState.ButtonCross)) {
        currentMenu = showScreenMenu;
    }
}

void open_graphics_menu() { 
    if((pads->NewState.ButtonCross && !pads->OldState.ButtonCross)) {
        currentMenu = showGraphicsMenu;
    }
}

int (*RsRwInitialize)(uint32_t) = (int (*)(uint32_t))0x39B510;
char* vsync_passes = (char*)0x39B53C;

MenuItem main_menu[] = {{ .name = "Screen", .func = open_screen_menu, .data = NULL }, 
                        { .name = "Graphics", .func = open_graphics_menu, .data = NULL },
                        { .name = "Memory", .func = dummy_func, .data = NULL },
                        { .name = "Limits", .func = dummy_func, .data = NULL },
                        { .name = "Misc", .func = dummy_func, .data = NULL }};

uint32_t main_menu_size = menu_size(main_menu);

void showDebugMenu() {
    processMenu(main_menu, main_menu_size, &menu_ptr);
}

MenuItem screen_menu[] = {{ .name = "Disable VSync", .func = toggle_vsync, .data = NULL }};

uint32_t screen_menu_size = menu_size(screen_menu);

void toggle_vsync() { 
    if((pads->NewState.ButtonCross && !pads->OldState.ButtonCross)) {
        if(*vsync_passes == 2) {
            screen_menu[0].name = "Enable VSync";
            *vsync_passes = 1;
        } else {
            screen_menu[0].name = "Disable VSync";
            *vsync_passes = 2;
        }
        
        RsRwInitialize(0);
    }
}

void showScreenMenu() {
    if((pads->NewState.ButtonTriangle && !pads->OldState.ButtonTriangle)) {
        currentMenu = showDebugMenu;
    }

    processMenu(screen_menu, screen_menu_size, &menu_ptr);
}

static char veh_density_str[32];
static char ped_density_str[32];

static char str_r_replace[48], str_g_replace[48], str_b_replace[48], str_a_replace[48];
static char str_r_add[48], str_g_add[48], str_b_add[48], str_a_add[48];

void r_add();
void g_add();
void b_add();
void a_add();
void r_replace();
void g_replace();
void b_replace();
void a_replace();
void toggle_colour_filter();

MenuItem graphics_menu[] = {{ .name = "Disable grass", .func = toggle_grass, .data = NULL },
                            { .name = veh_density_str, .func = vehDensity, .data = NULL },
                            { .name = ped_density_str, .func = pedDensity, .data = NULL },
                            { .name = "Disable colour filter", .func = toggle_colour_filter, .data = NULL },
                            { .name = str_r_add, .func = r_add, .data = NULL },
                            { .name = str_g_add, .func = g_add, .data = NULL },
                            { .name = str_b_add, .func = b_add, .data = NULL },
                            { .name = str_a_add, .func = a_add, .data = NULL },
                            { .name = str_r_replace, .func = r_replace, .data = NULL },
                            { .name = str_g_replace, .func = g_replace, .data = NULL },
                            { .name = str_b_replace, .func = b_replace, .data = NULL },
                            { .name = str_a_replace, .func = a_replace, .data = NULL },
};

uint32_t graphics_menu_size = menu_size(graphics_menu);

void toggle_colour_filter() { 
    if((pads->NewState.ButtonCross && !pads->OldState.ButtonCross)) {
        if(colour_filter_status) {
            graphics_menu[3].name = "Enable colour filter";
            colour_filter_status = 0;
        } else {
            graphics_menu[3].name = "Disable colour filter";
            colour_filter_status = 1;
        }
    }
}

// FILTER ADD

void r_add() { 
    if(pads->NewState.DPadLeft && add_weights.r > 0) {
        add_weights.r--;
        sprintf(str_r_add, "Colour filter red add: %d", add_weights.r);
    }

    if(pads->NewState.DPadRight && add_weights.r < 255) {
        add_weights.r++;
        sprintf(str_r_add, "Colour filter red add: %d", add_weights.r);
    }
}

void g_add() { 
    if(pads->NewState.DPadLeft && add_weights.g > 0) {
        add_weights.g--;
        sprintf(str_g_add, "Colour filter green add: %d", add_weights.g);
    }

    if(pads->NewState.DPadRight && add_weights.g < 255) {
        add_weights.g++;
        sprintf(str_g_add, "Colour filter green add: %d", add_weights.g);
    }
}

void b_add() { 
    if(pads->NewState.DPadLeft && add_weights.b > 0) {
        add_weights.b--;
        sprintf(str_b_add, "Colour filter blue add: %d", add_weights.b);
    }

    if(pads->NewState.DPadRight && add_weights.b < 255) {
        add_weights.b++;
        sprintf(str_b_add, "Colour filter blue add: %d", add_weights.b);
    }
}

void a_add() { 
    if(pads->NewState.DPadLeft && add_weights.a > 0) {
        add_weights.a--;
        sprintf(str_a_add, "Colour filter alpha add: %d", add_weights.a);
    }

    if(pads->NewState.DPadRight && add_weights.a < 255) {
        add_weights.a++;
        sprintf(str_a_add, "Colour filter alpha add: %d", add_weights.a);
    }
}

// FILTER REPLACE

void r_replace() { 
    if(pads->NewState.DPadLeft && rep_weights.r > 0) {
        rep_weights.r--;
        sprintf(str_r_replace, "Colour filter red replace: %d", rep_weights.r);
    }

    if(pads->NewState.DPadRight && rep_weights.r < 255) {
        rep_weights.r++;
        sprintf(str_r_replace, "Colour filter red replace: %d", rep_weights.r);
    }
}

void g_replace() { 
    if(pads->NewState.DPadLeft && rep_weights.g > 0) {
        rep_weights.g--;
        sprintf(str_g_replace, "Colour filter green replace: %d", rep_weights.g);
    }

    if(pads->NewState.DPadRight && rep_weights.g < 255) {
        rep_weights.g++;
        sprintf(str_g_replace, "Colour filter green replace: %d", rep_weights.g);
    }
}

void b_replace() { 
    if(pads->NewState.DPadLeft && rep_weights.b > 0) {
        rep_weights.b--;
        sprintf(str_b_replace, "Colour filter blue replace: %d", rep_weights.b);
    }

    if(pads->NewState.DPadRight && rep_weights.b < 255) {
        rep_weights.b++;
        sprintf(str_b_replace, "Colour filter blue replace: %d", rep_weights.b);
    }
}

void a_replace() { 
    if(pads->NewState.DPadLeft && rep_weights.a > 0) {
        rep_weights.a--;
        sprintf(str_a_replace, "Colour filter alpha replace: %d", rep_weights.a);
    }

    if(pads->NewState.DPadRight && rep_weights.a < 255) {
        rep_weights.a++;
        sprintf(str_a_replace, "Colour filter alpha replace: %d", rep_weights.a);
    }
}

void toggle_grass() { 
    if((pads->NewState.ButtonCross && !pads->OldState.ButtonCross)) {
        if(enable_grass) {
            RedirectCall(0x246DC8, dummy_func);
            graphics_menu[0].name = "Enable grass";
        } else {
            RedirectCall(0x246DC8, CPlantMgr_render);
            graphics_menu[0].name = "Disable grass";
        }
        enable_grass ^= 1;
    }
}

float* CCarCtrl_CarDensityMultiplier = (float*)0x665134;

void vehDensity() { 
    if((pads->NewState.DPadLeft && !pads->OldState.DPadLeft) && *CCarCtrl_CarDensityMultiplier > 0.0f) {
        *CCarCtrl_CarDensityMultiplier = *CCarCtrl_CarDensityMultiplier - 0.5f;
        sprintf(veh_density_str, "Vehicle density: %.1f", *CCarCtrl_CarDensityMultiplier);
    }

    if((pads->NewState.DPadRight && !pads->OldState.DPadRight) && *CCarCtrl_CarDensityMultiplier < 10.0f) {
        *CCarCtrl_CarDensityMultiplier = *CCarCtrl_CarDensityMultiplier + 0.5f;
        sprintf(veh_density_str, "Vehicle density: %.1f", *CCarCtrl_CarDensityMultiplier);
    }
}

float* CPopulation_PedDensityMultiplier = (float*)0x6650AC;

void pedDensity() { 
    if((pads->NewState.DPadLeft && !pads->OldState.DPadLeft) && *CPopulation_PedDensityMultiplier > 0.0f) {
        *CPopulation_PedDensityMultiplier = *CPopulation_PedDensityMultiplier - 0.5f;
        sprintf(ped_density_str, "Pedestrian density: %.1f", *CPopulation_PedDensityMultiplier);
    }

    if((pads->NewState.DPadRight && !pads->OldState.DPadRight) && *CPopulation_PedDensityMultiplier < 10.0f) {
        *CPopulation_PedDensityMultiplier = *CPopulation_PedDensityMultiplier + 0.5f;
        sprintf(ped_density_str, "Pedestrian density: %.1f", *CPopulation_PedDensityMultiplier);
    }
}

void showGraphicsMenu() {
    if((pads->NewState.ButtonTriangle && !pads->OldState.ButtonTriangle)) {
        currentMenu = showDebugMenu;
    }

    processMenu(graphics_menu, graphics_menu_size, &menu_ptr);
}

void hook_CPed_ctor(void* ped, int pedType) {
    CPed_ctor(ped, pedType);
    current_peds++;
}

void hook_CPed_dtor(void* ped) {
    CPed_dtor(ped);
    current_peds--;
}

static uint32_t current_vehs = 0;
static uint32_t total_vehs = 70;

void (*CVehicle_ctor)(void*, int) = (void (*)(void*, int))0x166F30;
void (*CVehicle_dtor)(void*) = (void (*)(void*))0x167B90;

void hook_CVehicle_ctor(void* veh, int vehType) {
    CVehicle_ctor(veh, vehType);
    current_vehs++;
}

void hook_CVehicle_dtor(void* veh) {
    CVehicle_dtor(veh);
    current_vehs--;
}

static uint32_t current_buildings = 0;
static uint32_t total_buildings = 12000;

void* (*CBuilding_ctor)(void*) = (void* (*)(void*))0x2016C0;
void (*CBuilding_dtor)(void*, int) = (void (*)(void*, int))0x2018C0;

void* hook_CBuilding_ctor(void* building) {
    current_buildings++;
    return CBuilding_ctor(building);
}

void hook_CBuilding_dtor(void* building, int qt) {
    CBuilding_dtor(building, qt);
    current_buildings--;
}

static uint32_t current_objs = 0;
static uint32_t total_objs = 250;

void (*CObject_ctor)(void*) = (void (*)(void*))0x1B10F0;
void (*CObject_dtor)(void*) = (void (*)(void*))0x1B0F60;

void hook_CObject_ctor(void* obj) {
    current_objs++;
    CObject_ctor(obj);
}

void hook_CObject_dtor(void* obj) {
    CObject_dtor(obj);
    current_objs--;
}

void showDebugInfo() {
    if((pads->NewState.ButtonCross && !pads->OldState.ButtonCross) &&
       (pads->NewState.DPadLeft && !pads->OldState.DPadLeft)) {
        display_info ^= 1;
    }

    if(display_info) {
        setupDebugInfoText();

        FindPlayerCoors(&player_coords, 0);
        sprintf(tmp_str, "Player coords: %f %f %f", player_coords.x, player_coords.y, player_coords.z);
        CFont_PrintString(15, debugstats_row(0), tmp_str);

        sprintf(tmp_str, "Stream memory: %d/%d", *strmem_used, *strmem_total);
        CFont_PrintString(15, debugstats_row(1), tmp_str);

        sprintf(tmp_str, "Peds: %d/%d", current_peds, total_peds);
        CFont_PrintString(15, debugstats_row(2), tmp_str);

        sprintf(tmp_str, "Vehicles: %d/%d", current_vehs, total_vehs);
        CFont_PrintString(15, debugstats_row(3), tmp_str);

        sprintf(tmp_str, "Buildings: %d/%d", current_buildings, total_buildings);
        CFont_PrintString(15, debugstats_row(4), tmp_str);

        sprintf(tmp_str, "Objects: %d/%d", current_objs, total_objs);
        CFont_PrintString(15, debugstats_row(5), tmp_str);

        frames++;

        end_time = *CTimer_m_snTimeInMilliseconds;
        elapsed_time = (float)((end_time - start_time) / 1000);

        if (elapsed_time >= 1.0f) {
            uint32_t fps = (uint32_t)(frames / elapsed_time);

            sprintf(tmp_fps, "%d FPS", fps);

            start_time = *CTimer_m_snTimeInMilliseconds;
            frames = 0;
        }

        CFont_PrintString(15, debugstats_row(6), tmp_fps);

    }
}

void debugMenuLoop() {
    pads = CPad_GetPad(0);

    showDebugInfo();
    currentMenu();

    drawhud();
}

void setupDebugStatsMenu() {
    int found = 0;

    currentMenu = waitDebugMenu;

    drawhud = (void (*)())ReadCall(0x2471E4); // Store whatever function that is hooking CHud_Draw into drawhud.

    CRGBA_CRGBA(&unselected_color, 128, 128, 128, 255);
    CRGBA_CRGBA(&selected_color, 255, 255, 255, 255);

    CRGBA_CRGBA(&font_color, 0, 255, 128, 255);
    CRGBA_CRGBA(&font_shadow, 0, 0, 0, 255);

    RedirectCall(drawhud_call, debugMenuLoop);

    strcpy(veh_density_str, "Vehicle density: 1.0");
    strcpy(ped_density_str, "Pedestrian density: 1.0");

    sprintf(str_r_add, "Colour filter red add: %d", add_weights.r);
    sprintf(str_g_add, "Colour filter green add: %d", add_weights.g);
    sprintf(str_b_add, "Colour filter blue add: %d", add_weights.b);
    sprintf(str_a_add, "Colour filter alpha add: %d", add_weights.a);

    sprintf(str_r_replace, "Colour filter red replace: %d", rep_weights.r);
    sprintf(str_g_replace, "Colour filter green replace: %d", rep_weights.g);
    sprintf(str_b_replace, "Colour filter blue replace: %d", rep_weights.b);
    sprintf(str_a_replace, "Colour filter alpha replace: %d", rep_weights.a);

    printf("******************************************************\n");
    printf("GTA V Legacy Debug hooks\n\n");

    found = SearchRedirectCalls(0x18F0E4, 0x1A03E8, CPed_ctor, hook_CPed_ctor);
    printf("Found %d CPed constructor calls, redirecting...\n", found);
    found = SearchRedirectCalls(0x18F170, 0x1A06B8, CPed_dtor, hook_CPed_dtor);
    printf("Found %d CPed destructor calls, redirecting...\n", found);

    found = SearchRedirectCalls(0x141418, 0x322EB4, CVehicle_ctor, hook_CVehicle_ctor);
    printf("Found %d CVehicle constructor calls, redirecting...\n", found);
    found = SearchRedirectCalls(0x14202C, 0x3DAF3C, CVehicle_dtor, hook_CVehicle_dtor);
    printf("Found %d CVehicle destructor calls, redirecting...\n", found);

    found = SearchRedirectCalls(0x3C86E8, 0x510654, CBuilding_ctor, hook_CBuilding_ctor);
    printf("Found %d CBuilding constructor calls, redirecting...\n", found);
    *((uint32_t*)0x65C028) = hook_CBuilding_dtor;
    //found = SearchRedirectCalls(0x2017B8, 0x2017B8, CBuilding_dtor, hook_CBuilding_dtor);
    //printf("Found %d CBuilding destructor calls, redirecting...\n", found);

    found = SearchRedirectCalls(0x1B0B14, 0x322EB4, CObject_ctor, hook_CObject_ctor);
    printf("Found %d CObject init calls, redirecting...\n", found);
    found = SearchRedirectCalls(0x1B0D20, 0x1B103C, CObject_dtor, hook_CObject_dtor);
    printf("Found %d CObject destructor calls, redirecting...\n", found);

    printf("\n\n******************************************************\n");
}
