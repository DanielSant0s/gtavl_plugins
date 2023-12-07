#include <injector.h>
#include "hooks.h"


void injectMiscPatches() {
    MakeNop(0x267E24);

    MakeRetn(0x4FB520); // Fix heap corruption

    MakeNop(0x5157AC); // Disable motion blur
    MakeNop(0x5157C0); // Disable motion blur

    MakeNop(0x14EB50); // No Wheel Turn Back
    MakeNop(0x14ECF0); // No Wheel Turn Back
    MakeNop(0x14ED84); // No Wheel Turn Back

    MakeNop(0x262804); // No flip explosions - player
    MakeNop(0x146790); // No flip explosions - ped

    WriteByte(0x6FF98F, 1); // Widescreen by default
	WriteByte(0x6FF99A, 0); // Enable Radio EQ
	WriteByte(0x66B8B8, 0); // Enable Invert Look

    WriteByte(0x64B6A8, 255); // marker_r
    WriteByte(0x64B6A9, 255); //marker_g
    WriteByte(0x64B6AA, 0); //marker_b

    WriteWord(0x242DFC, 2000);  // Make a day pass in 48 minutes
    WriteDword(0x66B79C, 2000); // Make a day pass in 48 minutes

    MakeRetn(0x516D80); // Disable heat haze

    MakeBranch(0x4BB188, 0x15B); // Disable enex

    MakeBranch(0x4BC998, 0x3F); // Disable Door Camera

}
