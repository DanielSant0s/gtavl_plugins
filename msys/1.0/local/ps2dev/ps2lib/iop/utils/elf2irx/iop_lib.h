/* 
  _____     ___ ____  
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2001, Gustavo Scotti (gustavo@scotti.com)
  ------------------------------------------------------------------------
   ELF2IRX - Convert an object elf file to IOP IRX module

   June / 17th / 2001
       
   This utility objectives:
       This utility reconstructs a valid irx module from an elf file.
*/
struct iop_module_function
{
   struct iop_module_function	*next;
   u8				*name;
   int				used;
   int				index;
};

struct iop_module_stub
{
   struct iop_module_stub	*next;
   u8				*name;
   u32				version;
   struct iop_module_function	*mod_funct;
};

#ifndef _STUB_FILE
#define _STUB_FILE "/cdrom/PSX2/ps2lib/iop/utils/elf2irx/stub.iop"
#endif

