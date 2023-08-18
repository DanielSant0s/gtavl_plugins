/* 
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2001, Gustavo Scotti (gustavo@scotti.com)
  ------------------------------------------------------------------------
   ELF2IRX - Convert an object elf file to IOP IRX module

   June / 17th / 2001
   Jan  / 12th / 2002
       
   This utility objectives:
       This utility reconstructs a valid irx module from an elf file.
*/


#define IOP_AS 	"iop-as"
#define IOP_LD	"iop-ld"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "tamtypes.h"
#include "loadelf.h"
#include "iop_lib.h"

struct iop_module_stub *mod_stub = NULL;

void
remove_some_sections (struct elf_file *elf)
{
	static u8 *to_delete[] = { ".reginfo",
		NULL
	};
	struct elf_section_header *section;
	int i, n;

	for (i = 0; i < elf->hdr.e_shnum; i++)
	{
		section = elf->section[i];

		if (section)
			for (n = 0; to_delete[n]; n++)
				if (!strcmp (section->name, to_delete[n]))
				{
					/* delete this section */
/*               printf("           deleted %s\n", to_delete[n]); */
					elf_deletesection (section);
					elf->section[i] = NULL;
				}
	}
}

void
organize_sections (struct elf_file *elf)
{
	struct elf_section_header *section, **a_section;
	int s_free, s_total;
	int i, n;
	u32 addr, l_addr;

	s_total = elf->hdr.e_shnum;

	/* find out how many free sections we have */
	for (i = s_free = 0; i < s_total; i++)
	{
		if (!elf->section[i])
			s_free++;
	}
	if (s_free > 1)
		s_total -= (s_free - 1);
	else if (!s_free)
		s_total++;


	/* INSERT A NEW SECTION */
	a_section =
		(struct elf_section_header **) malloc (s_total *
						       sizeof (section));

	/* section 0 is reserved by elf
	 * section 1 is reserved by iopmod */
	a_section[0] = elf->section[0];
	a_section[1] = NULL;
	n = 2;

	for (i = 1; i < elf->hdr.e_shnum; i++)
	{
		section = elf->section[i];

		if (section)
			a_section[n++] = section;
	}

	/* RE-ARRANGE SECTION INFO AND LINK */
	for (i = 1; i < elf->hdr.e_shnum; i++)
	{
		section = elf->section[i];

		if (section)
		{
			if (section->sh_link)
			{
				for (n = 0; n < s_total; n++)
					if (a_section[n] ==
					    elf->section[section->sh_link])
						break;
				section->sh_link = n;
			}
			if (section->sh_info)
			{
				for (n = 0; n < s_total; n++)
					if (a_section[n] ==
					    elf->section[section->sh_info])
						break;
				section->sh_info = n;
			}
		}
	}

	/* RE-ARRANGE SYMBOL TABLE LINK */
	for (i = 1; i < elf->hdr.e_shnum; i++)
	{
		section = elf->section[i];

		if (section && section->sh_type == SHT_SYMTAB
		    && section->symbol_table)
		{
			int j;
			struct elf_symbol *symbol;

			for (j = 0; j < section->symbol_count; j++)
			{
				symbol = section->symbol_table[j];

				for (n = 0; n < s_total; n++)
					if (a_section[n]
					    && a_section[n] ==
					    elf->section[symbol->st_shndx])
						break;
				if (n == s_total)
				{
					n = 0;
					bzero (symbol, sizeof (*symbol));	/* Nulifies this symbol */
					symbol->st_type = 0;
					symbol->st_bind = 0;
				}
				symbol->st_shndx = n;
				symbol->st_addr -= a_section[n]->sh_addr;
			}
		}
	}
	for (n = 0; n < s_total; n++)
		if (a_section[n] == elf->strtable)
			break;
	elf->hdr.e_shstrndx = n;
	elf->hdr.e_shnum = s_total;
	free (elf->section);
	elf->section = a_section;

	addr = 0;		/* start from address 0 */
	/* RE-ARRANGE SECTION ADDRESSES */
	for (i = 1; i < elf->hdr.e_shnum; i++)
	{
		section = elf->section[i];
		if (!section)
			continue;
		switch (section->sh_type)
		{
		case SHT_PROGBITS:
			section->sh_addr = addr;
/*            			elf_setsymbol_addr( elf, section->string_name, addr); */
			l_addr = addr;
			addr += section->sh_size;
			break;
		case SHT_NOBITS:
			section->sh_addr = addr;
/*            			elf_setsymbol_addr( elf, section->string_name, l_addr); */
			break;
		default:
			section->sh_addr = 0;
		}
	}
	/* UPDATE SYMBOL ADDRESSES */
	for (i = 1; i < elf->hdr.e_shnum; i++)
	{
		section = elf->section[i];

		if (section && section->sh_type == SHT_SYMTAB
		    && section->symbol_table)
		{
			int j;
			struct elf_symbol *symbol;

			for (j = 0; j < section->symbol_count; j++)
			{
				symbol = section->symbol_table[j];
				if (!symbol)
					continue;
				if (symbol->st_shndx)
					symbol->st_addr +=
						elf->section[symbol->
							     st_shndx]->
						sh_addr;
			}
		}
	}
}

void
make_lib (struct elf_file *elf, u8 * elf_a, u8 * elf_b)
{
	struct elf_section_header *section;
	u8 buff[512];
	int fd, i, n, warn = 0;
        struct iop_module_stub *stub;

	for (i = 1; i < elf->hdr.e_shnum; i++)
	{
		section = elf->section[i];
		if (!section)
			continue;

		if (section->sh_type == SHT_REL)
		{
			struct elf_section_header *symtab;
			u8 *data;
			int d_size, r_count, n, t_count;
			struct elf_relocation **reloc;

			/* get relocations */
			reloc = section->relocation_table;
			r_count = section->relocation_count;

			/* get syms */
			symtab = elf->section[section->sh_link];

			/* get data to realocate */
			data = elf->section[section->sh_info]->data;
			d_size = elf->section[section->sh_info]->sh_size;

			for (n = 0; n < r_count; n++)
			{
				u8 *sym_name;
				struct elf_symbol *symbol;
				int ndx;

				if (!reloc[n])
					continue;

				ndx = reloc[n]->r_sym;
				sym_name =
					(u8 *) elf_getsymbol_name (elf,
								   symtab,
								   ndx);
				if (!*sym_name)

				{
					printf ("** UNDEFINED RELOCATION. quitting.\n");
					exit (0);
				}
				symbol = (struct elf_symbol *)
					elf_findsymbol (elf, sym_name);
				if (!symbol
				    || (symbol->st_type == STT_NOTYPE))
				{
				stub = mod_stub;
					while (stub)
					{
						struct iop_module_function *funct;
                                                funct = stub->mod_funct;
						while (funct)
						{
							if (!strcmp
							    (funct->name, sym_name))
							{
								funct->used = 1;
								break;
							}
							funct = funct->next;
						}
						if (funct)
                                                   break;
                                                stub = stub->next;
					}
					if (!stub)
					{
						warn++;
						printf ("** symbol '%s' undefined.\n", sym_name);
					}
					continue;
				}
			}
		}
	}
	if (warn)
	{
		printf ("** quitting\n");
		exit (0);
	}

	/* construct a .s file to handle iop module functions */
	snprintf (buff, sizeof (buff), "%s.s", elf_b);
	fd = open (buff, O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU);
	snprintf (buff, sizeof (buff), ".text\n.set noreorder\n\n");
	write (fd, buff, strlen (buff));

	stub = mod_stub;
	while (stub)
	{
		int f_st = 0;
                struct iop_module_function *funct;
                funct = stub->mod_funct;
		while (funct)
		{
			if (funct->used)
			{
				if (!f_st)
				{
					f_st = 1;
					snprintf (buff, sizeof (buff),
						  "\n.local %s_stub\n%s_stub:\n.word 0x41e00000\n.word 0\n.word 0x%08x\n.ascii \"%s",
						  stub->name,
						  stub->name,
						  stub->version,
						  stub->name);
					write (fd, buff, strlen (buff));
					for (i = 0;
					     i <
					     8 - strlen (stub->name);
					     i++)
						write (fd, "\\0", 2);
					snprintf (buff, sizeof (buff),
						  "\"\n.align 2\n\n");
					write (fd, buff, strlen (buff));
				}

				snprintf (buff, sizeof (buff),
					  ".globl %s\n%s:\nj $31\nli $0, %d\n",
					  funct->name,
					  funct->name,
					  funct->index);
				write (fd, buff, strlen (buff));
			}
                        funct = funct->next;
		}
		if (f_st)
		{
			snprintf (buff, sizeof (buff), ".word 0\n.word 0\n");
			write (fd, buff, strlen (buff));
		}
		stub = stub->next;
	}
	close (fd);

	snprintf (buff, sizeof (buff), "%s -o %s.o %s.s", IOP_AS, elf_b,
		  elf_b);
	system (buff);
	snprintf (buff, sizeof (buff), "%s -r %s %s.o -o %s", IOP_LD, elf_a,
		  elf_b, elf_b);
	system (buff);
	snprintf (buff, sizeof (buff), "%s.s", elf_b);
	unlink (buff);  
	snprintf (buff, sizeof (buff), "%s.o", elf_b);
	unlink (buff);
}

void
relocate_sections (struct elf_file *elf)
{
	struct elf_section_header *section;
	int i, n, warn = 0;
        u32	base_addr;

	for (i = 1; i < elf->hdr.e_shnum; i++)
	{
		section = elf->section[i];
		if (!section)
			continue;
		if (section->sh_type == SHT_REL)
		{
			struct elf_section_header *symtab;
			u8 *data;
			int d_size, r_count, n, t_count;
			struct elf_relocation **reloc;

			/* get relocations */
			reloc = section->relocation_table;
			r_count = section->relocation_count;

			/* get syms */
			symtab = elf->section[section->sh_link];

			/* get data to realocate */
			data = elf->section[section->sh_info]->data;
			d_size = elf->section[section->sh_info]->sh_size;
                        base_addr = elf->section[section->sh_info]->sh_addr;
			for (n = 0; n < r_count; n++)
			{
				u8 *sym_name;
				struct elf_symbol *symbol;
				int ndx;

				if (!reloc[n])
					continue;
				ndx = reloc[n]->r_sym;
				sym_name =
					(u8 *) elf_getsymbol_name (elf,
								   symtab,
								   ndx);
				if (!*sym_name)
				{
					printf ("** UNDEFINED RELOCATION. quitting.\n");
					exit (0);
				}
				symbol = (struct elf_symbol *)
					elf_findsymbol (elf, sym_name);
				if (!symbol
				    || (symbol->st_type == STT_NOTYPE))
				{
					warn++;
					printf ("** symbol '%s' undefined.\n",
						sym_name);
					continue;
				}
				switch (reloc[n]->r_type)
				{
				case R_MIPS_32:
				{
					u32 valuef;
					/* */
					valuef = reloc[n]->r_offset;
					valuef +=
						elf_getsymbol_addr (elf,
								    sym_name);
#ifdef DEBUG
					printf ("R_MIPS_32\t %08x [%08x] %s\n", reloc[n]->r_offset, valuef, sym_name);
#endif 
                                        reloc[n]->r_offset = valuef;
				}
					break;
				case R_MIPS_26:
				{
					u32 value1, value2, valuef;
					/* */
					value1 = *(u32 *) & data[reloc[n]->
								 r_offset];
					value2 = value1 & 0xfc000000;
					value1 &= 0x03ffffff;
					value1 +=
						elf_getsymbol_addr (elf,
								    sym_name)
						/ 4;
					valuef = value1 | value2;
#ifdef DEBUG
					printf ("R_MIPS_26\t %08x [%08x] %s\n", reloc[n]->r_offset, valuef, sym_name);
#endif
					*(u32 *) & data[reloc[n]->r_offset] =
						valuef;
				}
					break;
				case R_MIPS_HI16:
				{
					u32 value1, value2, valuef;
					/* */
					value1 = *(u32 *) & data[reloc[n]->
								 r_offset];
					value2 = value1 & 0xffff0000;
					value1 &= 0xffff;
					value1 +=
						(elf_getsymbol_addr
						 (elf, sym_name) >> 16);
					valuef = value1 | value2;
#ifdef DEBUG
					printf ("R_MIPS_HI16\t %08x [%08x] %s\n", reloc[n]->r_offset, valuef, sym_name);
#endif
					*(u32 *) & data[reloc[n]->r_offset] =
						valuef;
				}
					break;
				case R_MIPS_LO16:
				{
					u32 value1, value2, valuef;
					/* */
					value1 = *(u32 *) & data[reloc[n]->
								 r_offset];
					value2 = value1 & 0xffff0000;
					value1 &= 0xffff;
					value1 +=
						(elf_getsymbol_addr
						 (elf, sym_name) & 0xffff);
					valuef = value1 | value2;
#ifdef DEBUG
					printf ("R_MIPS_LO16\t %08x [%08x] %s\n", reloc[n]->r_offset, valuef, sym_name);
#endif
					*(u32 *) & data[reloc[n]->r_offset] =
						valuef;
				}
					break;
				default:
					reloc[n]->r_offset = 0;

				}
/*                        reloc[n]->r_offset += base_addr; */
			}
			if (warn)
			{
				printf ("quitting!\n");
				exit (0);
			}
			/* get relocations */
			reloc = section->relocation_table;
			r_count = section->relocation_count;
			/* re-count relocations */
			for (n = t_count = 0; n < r_count; n++)
				if (reloc[n]->r_offset)
				{
					reloc[n]->r_sym = 0;
					if (t_count != n)
						memcpy (reloc[t_count],
							reloc[n],
							sizeof (struct
								elf_relocation));
					t_count++;
				}
			section->relocation_count = t_count;
			section->sh_size = t_count * SIZE_REL;
		}
	}
}


struct iopmod_section
{
	u32 mod_st_addr,
		start_addr, heap_addr, text_size, data_size, bss_size;
	u16 version;
	u8 *name;
};

void
create_iopmod (struct elf_file *elf)
{
	struct elf_section_header *section;
	struct elf_prog_header *program;
	struct iopmod_section *iopmod;
	struct elf_symbol *symbol;
	int i, n;
	u32 s_text, s_data, s_bss, s_sdata;

	iopmod = (struct iopmod_section *) malloc (sizeof (*iopmod));
	bzero (iopmod, sizeof (*iopmod));
	iopmod->mod_st_addr = elf_getsymbol_addr (elf, "iop_module");
	iopmod->start_addr = elf_getsymbol_addr (elf, "start");
	symbol = (struct elf_symbol *) elf_findsymbol (elf, "start");
	if (!symbol || (symbol->st_type == STT_NOTYPE))
	{
		printf ("could not find symbol 'start'\n");
		exit (0);
	}

	s_text = s_data = s_bss = s_sdata = 0;

	for (i = 1; i < elf->hdr.e_shnum; i++)
	{
		section = elf->section[i];

		if (!section)
			continue;

		if (section->sh_type == SHT_PROGBITS &&
		    strstr (section->name, "text"))
			s_text += section->sh_size;

		if (section->sh_type == SHT_PROGBITS &&
		    strstr (section->name, "data"))
			s_data += section->sh_size;

		if (section->sh_type == SHT_PROGBITS &&
		    strstr (section->name, "sdata"))
			s_sdata += section->sh_size;

		if (section->sh_type == SHT_NOBITS &&
		    strstr (section->name, "bss"))
			s_bss += section->sh_size;
	}

	iopmod->heap_addr = 0x7ff0 + s_data + s_text - s_sdata;
	iopmod->text_size = s_text;
	iopmod->data_size = s_data;
	iopmod->bss_size = s_bss;
	if (iopmod->mod_st_addr)
	{
		u8 *addr;

		addr = (u8 *) iopmod->mod_st_addr;
		iopmod->version = *(u16 *) addr;
		addr += 2;
		iopmod->name = (u8 *) addr;
	}
	else
	{
		iopmod->mod_st_addr = 0xffffffff;
		iopmod->version = 0;
		iopmod->name = "";
	}

	/* create new section */
	section = (struct elf_section_header *) malloc (sizeof (*section));
	bzero (section, sizeof (*section));
	elf->section[1] = section;
	section->sh_type = 0x70000080;	/* iopmod type */
	section->sh_addralign = 4;
	section->sh_size = 27 + strlen (iopmod->name);
	section->name = strdup (".iopmod");
	section->data = malloc (section->sh_size);
	bzero (section->data, section->sh_size);
	memcpy (section->data, iopmod, 24);
	strcpy (&section->data[24], iopmod->name);
	elf->hdr.e_entry = iopmod->start_addr;
	elf->hdr.e_type = 0xff80;

	/* start program definition */
	elf->hdr.e_phentsize = PROGRAM_SIZE;
	elf->hdr.e_phnum = 2;
	elf->program = malloc (elf->hdr.e_phnum * sizeof (int *));

	/* FOR IOP HEADER */
	program = elf->program[0] = malloc (PROGRAM_SIZE);
	program->p_type = 0x70000080;
	program->p_offset = 0;
	program->p_vaddr = 0;
	program->p_paddr = 0;
	program->p_filesz = section->sh_size + 1;
	program->p_memsz = 0;
	program->p_flags = PF_R;
	program->p_align = 4;

	/* FOR IOP MODULE */
	program = elf->program[1] = malloc (PROGRAM_SIZE);
	program->p_type = PT_LOAD;
	program->p_offset = 0;
	program->p_vaddr = 0;
	program->p_paddr = 0;
	program->p_filesz = s_text + s_data;
	program->p_memsz = s_text + s_data + s_bss;
	program->p_flags = PF_R | PF_W | PF_X;
	program->p_align = 16;

	free (iopmod);
}

/* TODO:
   Inserir opcoes ---- */

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


int
main (int argc, char **argv)
{
	struct elf_file elf, elf_old;
	u8        old_name[128], *in_name, *out_name;
        int	  remove_symbol=0, i;

	printf ("ELF2IRX v1.2 - converts an elf file to a valid iop IRX image.\n");
	printf ("(C)2001,2002 Gustavo Scotti <gustavo@scotti.com>\n");
	if (argc < 3)
	{
		printf ("* usage: %s [-r] <elf input file> <irx output file>\n"
                        "             -r removes symbol table\n",
			argv[0]);
		exit (0);
	}

        in_name = out_name = NULL;
        for (i = 1; i < argc; i++)
           if (*argv[i]=='-') /* option */
              {
              if (tolower(argv[i][1])=='r')
                 remove_symbol = 1;
              }
           else
           if (!in_name)
              in_name = argv[i];
           else
           if (!out_name)
              out_name = argv[i];
           
              
	if (elf_load ( in_name, &elf_old) < 0)
	{
		printf ("* invalid elf file\n");
		exit (0);
	}
	printf ("\n");
        load_stub();
	snprintf (old_name, sizeof (old_name), "tmpe2i%d", getpid ());
	make_lib (&elf_old, in_name, old_name);
	elf_load (old_name, &elf);
	unlink (old_name);
/*
   printf(". stage 1: removing some elf sections\n");  */
	remove_some_sections (&elf);
/*   printf(". stage 2: organizing sections\n");  */
	organize_sections (&elf);
/*   printf(". stage 3: re-locating sections\n");  */
	relocate_sections (&elf);
/*   printf(". stage 4: creating iop section\n");  */
	create_iopmod (&elf);
/*   printf(". stage 5: pre-writing irx file\n"); */
	elf_save (&elf, out_name);
/*   printf(". stage 6: adjust program offset\n"); */
	elf.program[0]->p_offset = elf.section[1]->sh_offset;
	elf.program[1]->p_offset = elf.section[2]->sh_offset;
	elf_save (&elf, out_name);

	return 0;
}
