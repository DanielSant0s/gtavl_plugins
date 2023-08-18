#include <tamtypes.h>
#include <sifcmd.h>
#include <kernel.h>
#include <sifrpc.h>

int main()
{   
   SifInitRpc(0); 

   init_scr();
   scr_printf("Hello, world!\n"); /* hello world in the screen */

   printf("Hello, world!\n");
   nprintf("Hello, again, from Naplink RPC!\n");

   /* Return to the bootloader or PS2 browser. */
   return 0;
}
