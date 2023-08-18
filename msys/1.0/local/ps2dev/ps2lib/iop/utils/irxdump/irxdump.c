/* 
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2001, Gustavo Scotti (gustavo@scotti.com)
  ------------------------------------------------------------------------
   IRXDUMP - Dumps information about IRX files

   Apr / 2002
       
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "tamtypes.h"
#include "loadelf.h"
#include "iop_lib.h"

struct iop_module_stub *mod_stub = NULL;

struct iopmod_section
{
	u32 mod_st_addr,
		start_addr, heap_addr, text_size, data_size, bss_size;
	u16 version;
	u8 name[8];
};

/* inspired by php explode function :) */
int 	strexplode( u8 *toparse, u8 **argv, int max_argc, u8 *separators, u8 *quoters)
{
   int 				arg = 0;
   u8 				*tmp, *quote = NULL;
   
   tmp = toparse;
   while (arg < max_argc)
      {
      if (tmp != toparse)
         {
         *tmp = 0; 
         tmp++;
         }
      /* strip leading separators */
      while (*tmp && strchr( separators, *tmp)) 
         tmp++;
      if (!*tmp) break;
      
      argv[ arg++] = tmp;
      if (quoters)
         quote = strchr( quoters, *tmp);
      if (quote) /* supress everything that is inside quotes */
         {
         tmp++; 
         quote++;
         argv[ arg - 1] = tmp;
         while (*tmp && *tmp!=*quote)
            tmp++;
         if (!*tmp) break;
         *tmp = 0;
         tmp++;
         }
      while (*tmp && !strchr( separators, *tmp)) 
         tmp++;
      if (!*tmp) break; 
      }
   return arg;
}


int
load_stub()
{
   FILE		*fp;
   u8		buff[512], *param[16];
   int		n, i;

   struct	iop_module_stub 	*stub = NULL;
   
   fp = fopen( _STUB_FILE, "rt");
   if (!fp)
      {
      printf("error opening stub file '%s'\n", _STUB_FILE);
      exit(0);
      }
   while (!feof(fp))
      {
      buff[0] = 0;
      fgets( buff, sizeof(buff), fp);
      n = strlen(buff);
      if (n>0)
         buff[n-1] = 0;
      if (buff[0]=='#') continue; /* comment */
      bzero(param, sizeof(param));
      n = strexplode( buff, param, 16, " \t", NULL);
      if (!n)
         continue; /* no lines */
      if (n>2) /* new stub */
         {
         stub = (struct iop_module_stub *)malloc(sizeof(*stub));
         stub->name    = strdup( param[1]);
         stub->version = strtoul( param[2], NULL, 16);
         stub->mod_funct = NULL;
         stub->next = mod_stub;
         mod_stub = stub;
         }
      else  /* new function */
      if (n>1)
         {
         struct	iop_module_function *funct, *ll_funct;
         if (!stub)
            {
            printf("cannot assign function to inexistent stub!\n");
            continue;
            }
         funct = (struct iop_module_function *)malloc(sizeof(*funct));
         funct->used = 0;
         funct->index = atoi( param[0]);
         funct->name = strdup( param[1]);
         funct->next = NULL;
         ll_funct = stub->mod_funct;
         if (!ll_funct)
            {
            stub->mod_funct = funct;
            continue;
            }
         while (ll_funct->next)
            ll_funct = ll_funct->next;
         ll_funct->next = funct;
         }
            
      }
}

void *irx_var( struct elf_file *elf, u32 var_addr)
{
   int i;
   struct elf_section_header	*section = elf->section[0];

   for (i = 0; i < elf->hdr.e_shnum; section = elf->section[++i]) 
      {
      if (section->sh_type != SHT_PROGBITS)
         continue;
      if (section->sh_addr <= var_addr &&
         (section->sh_addr + section->sh_size) > var_addr) /* found you */
         {
         var_addr -= section->sh_addr;
         return &section->data[var_addr];
         }
      }
   return NULL;
}

struct irx_header {
   u32	irx_name;
   u16	irx_version;
};

void dump_header( struct elf_file *elf)
{
   struct iopmod_section	*iopmod;
   struct irx_header		*irx_header;
   struct elf_section_header	*section = elf->section[0];
   int i;
   
   if (elf->hdr.e_type != 0xff80)
      {
      printf("This is not a playstation2 irx file!\n");
      exit(0);
      }

   for (i = 0; i < elf->hdr.e_shnum; section = elf->section[++i]) 
      if (section->sh_type == 0x70000080)
         break;
   if (i == elf->hdr.e_shnum)
      return;
   
   iopmod = (void *)section->data;
   
   printf("Module info ............................ ");
   /* MODULE INFO */
   
   irx_header = NULL;
   if (iopmod->mod_st_addr)
      irx_header = irx_var( elf, iopmod->mod_st_addr);
   if (!irx_header)
      printf("** no info **\n");
   else
      printf("%s (%x.%02x)\n", irx_var( elf, irx_header->irx_name), 
         	(irx_header->irx_version >> 8),
         	(irx_header->irx_version & 0xff)  );

   printf("Start Address .......................... 0x%08x\n", iopmod->start_addr);
   printf("Heap Start Address ..................... 0x%08x\n", iopmod->heap_addr);
   printf("TEXT Segment Size ...................... %d\n", iopmod->text_size);
   printf("DATA Segment Size ...................... %d\n", iopmod->data_size);
   printf("BSS Segment Size ....................... %d\n", iopmod->bss_size);

   /* CHECK IF IT HAS SYMBOL TABLE */
/*    printf("Has Symbol Table ....................... "); */

}

u8	*get_stub_function_name( u8 *stub_name , int ref)
{
   struct iop_module_stub 	*stub;
   struct iop_module_function	*funct;
   
   for (stub = mod_stub; stub; stub = stub->next)
      if (!strcmp( stub->name, stub_name))
         break;
   if (!stub)
      return NULL;

   for (funct = stub->mod_funct; funct; funct = funct->next)
      if (funct->index == ref)
         return funct->name;
   return NULL;
}

void dump_imports( struct elf_file *elf)
{
   int i;
   struct elf_section_header	*section = elf->section[0];
   u32	*code;
   
   for (i = 0; i < elf->hdr.e_shnum; section = elf->section[++i]) 
      if (section->sh_type == SHT_PROGBITS && !strcmp( section->name, ".text"))
         break;
   if (i == elf->hdr.e_shnum)
      return;
       
   printf("\n ADDRESS      #    function name          IMPORT INFORMATION\n");
   code = (u32 *)section->data;
   for (i = 0; i < section->sh_size; i+=4, code++)
      {
      if (*code == 0x41e00000 && *(code + 1) == 0) 
         {
         u8	name[12];
         u16	version;
         
/*         printf("  0x%08x |     |", i); */
         printf("0x%08x |-----+---------------------- ", i);
         version = *(code + 2);
         strncpy( name, (char *)(code + 3), 8);
         name[8] = 0;
         printf(" %s (%x.%02x)\n", name, (version >> 8), (version & 0xff));
         code += 5; i += 20;
         while (i < section->sh_size)
            {	/* dumps functions */
               u32	f_ref;
               u8	*fname;
               
               if (!*code && !*(code + 1)) {
                  code +=1;
                  i+=4;
                  break;
                  }
               
               code++;
               f_ref = *code++; 
               f_ref &= 0xffffff;
               printf("0x%08x | %03d | ", i, f_ref);
               fname = elf_getsymbol_byaddr( elf, i);
               if (!fname) /* search in iop stub database */
                  {
                  printf("* ");
                  fname = get_stub_function_name( name, f_ref);
                  }
               if (fname)
                  printf("%s", fname);
               else
                  printf("** N/A **");
               printf("\n");
               i+=8;
               
            }
         }
      }
}  


void dump_exports( struct elf_file *elf)
{
   int i, n;
   struct elf_section_header	*section = elf->section[0];
   u32	*code;
   
   for (i = 0; i < elf->hdr.e_shnum; section = elf->section[++i]) 
      if (section->sh_type == SHT_PROGBITS && !strcmp( section->name, ".text"))
         break;
   if (i == elf->hdr.e_shnum)
      return;
       
   printf("\n ADDRESS      #    function name          EXPORT INFORMATION\n");
   code = (u32 *)section->data;
   for (i = 0; i < section->sh_size; i+=4, code++)
      {
      if (*code == 0x41c00000 && *(code + 1) == 0) 
         {
         u8	name[12];
         u16	version;
         
/*         printf("  0x%08x |     |", i); */
         printf("0x%08x |-----+---------------------- ", i);
         version = *(code + 2);
         strncpy( name, (char *)(code + 3), 8);
         name[8] = 0;
         printf(" %s (%x.%02x)\n", name, (version >> 8), (version & 0xff));
         code += 5; i += 20; n=0;
         while (i < section->sh_size)
            {	/* dumps functions */
               u32	f_ref;
               u8	*fname;
               
               if (!*code && n>3)
                  break;
               
               printf("0x%08x | %03d | ", *code, n++);
               fname = elf_getsymbol_byaddr( elf, *code);
               if (fname)
                  printf("%s", fname);
               else
                  printf("** N/A **");
               printf("\n");
               code++; i+=4;
            }
         }
      }
}  

int
main (int argc, char **argv)
{
	struct elf_file elf;

	printf ("IRXDUMP v1.0 - Dumps information about IRX files.\n");
	printf ("(C)2002 Gustavo Scotti <gustavo@scotti.com>\n");
	if (argc < 2)
	{
		printf ("* usage: %s <irx file>\n",
			argv[0]);
		exit (0);
	}

        load_stub();
        
	if (elf_load ( argv[1], &elf) < 0)
	{
		printf ("* invalid elf file\n");
		exit (0);
	}
	printf ("\n");
        dump_header( &elf);
        dump_imports( &elf);
        dump_exports( &elf);
	return 0;
}
