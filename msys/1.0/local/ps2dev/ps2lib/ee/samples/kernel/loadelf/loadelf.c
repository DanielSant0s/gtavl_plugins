/*      
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (c) 2003 Marcus R. Brown (mrbrown@0xd6.org)
  ------------------------------------------------------------------------
  loadelf.c
                           loadfile API sample.
*/

/* This example loads the TESTMODE ELF file from ROM, then executes it.  */

#include <tamtypes.h>
#include <ps2lib_err.h>
#include <kernel.h>
#include <loadfile.h>

const char *testmode = "rom0:TESTMODE";

int main(int argc, char **argv)
{
	t_ExecData exec_data;

	if (SifLoadElf(testmode, &exec_data) < 0)
		SleepThread();

	/* Even if SifLoadElf returned successfully, we still need to be sure
	   we have a valid start address.  */
	if (!exec_data.epc)
		SleepThread();

	/* Everything's loaded, let's executed it.  */
	ExecPS2((void *)exec_data.epc, (void *)exec_data.gp, 0, NULL);

	return 0;
}
