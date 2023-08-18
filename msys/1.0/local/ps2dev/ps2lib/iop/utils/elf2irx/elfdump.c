/* 
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2001, Gustavo Scotti (gustavo@scotti.com)
  ------------------------------------------------------------------------
   ELFDUMP - Dump elf file into its sections

   June / 17th / 2001
   Jan  / 12th / 2002
       
   This utility objectives:
       This utility reconstructs a valid irx module from an elf file.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "tamtypes.h"
#include "loadelf.h"
#include "iop_lib.h"

main( int argc, char **argv) {

   struct elf_file		elf;
   int i;

   if (argc < 2) { exit(0); }
   elf_load( argv[1], &elf);
   for (i = 0; i < elf.hdr.e_shnum; i++)
      {
      struct elf_section_header		*section;
      
      section = elf.section[i];
      if (section->name && section->sh_size) {
         int fd;
         u8	buff[256];
         snprintf( buff, sizeof(buff), "%s%s", argv[1], section->name);
         
         fd = open( buff, O_CREAT | O_TRUNC | O_WRONLY);
         write(fd, section->data, section->sh_size);
         close(fd);
         }
      /* position it to its section position */
      }
}
