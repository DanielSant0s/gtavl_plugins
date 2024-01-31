#include <injector.h>

void injectLimitHooks() {
    WriteWord(0x244634, 3620);
    WriteWord(0x244E10, 3620);
    WriteWord(0x300C80, 3620);
}
