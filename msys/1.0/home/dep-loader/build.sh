#!/bin/sh

ee-gcc -Wno-pointer-arith -fomit-frame-pointer -fno-exceptions \
-fno-asynchronous-unwind-tables -fno-unwind-tables -nostartfiles -nostdlib -mno-crt0 \
-fno-builtin-memcmp -fno-builtin-memcpy -fno-builtin-memset \
-fno-builtin-printf -fno-builtin-strcat -fno-builtin-strchr \
-fno-builtin-strcmp -fno-builtin-strcpy -fno-builtin-strlen \
-fno-builtin-strncpy -fno-builtin-strrchr -fno-builtin-strncmp \
-fno-builtin-sprintf -fno-builtin-floor -fno-builtin-pow \
-fno-builtin-malloc -fno-builtin-free \
-G0 -O2 \
-Wl,\
--section-start=.text=0x393E50 \
 -T format.ld elfparser.c -o loader.elf 