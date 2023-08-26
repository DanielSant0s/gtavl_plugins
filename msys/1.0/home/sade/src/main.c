#include <injector.h>

int _start()
{
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

    WriteWord(0x242DFC, 2000);  // Make a day pass in 48 minutes
    WriteDword(0x66B79C, 2000); // Make a day pass in 48 minutes

    WriteDword(0x666d50, 0x00642524); // No zeroes on money psoitive
    WriteDword(0x666d54, 0x00000000); // No zeroes on money psoitive
    WriteDword(0x666d58, 0x6425242d); // No zeroes on money negative
    WriteDword(0x666d5c, 0x00000000); // No zeroes on money negative

    return 0;
}
