////////////////////////////////////////////////////////////////////////////////
// sio_shell.c - C side code for the SIO shell
// Written by TyRaNiD (c) 2005
// Much props to Herben und Pixel
////////////////////////////////////////////////////////////////////////////////
#include <tamtypes.h>
#include <stdio.h>
#include <string.h>
#include <kernel.h>
#include <sio.h>
#include <sio_shell.h>
#include <hwbp.h>
#include "R5900Disasm.h"
#include "ymodem.h"
#include "iopmon.h"

////////////////////////////////////////////////////////////////////////////////
// Function Prototypes
////////////////////////////////////////////////////////////////////////////////


/* Command prototypes */
static int exit_cmd(void);
static int md_cmd(void);
static int help_cmd(void);
static int pb_cmd(void);
static int ph_cmd(void);
static int pw_cmd(void);
static int pd_cmd(void);
static int dr_cmd(void);
static int er_cmd(void);
static int exec_cmd(void);
static int start_cmd(void);
static int dis_cmd(void);
static int bpi_cmd(void);
static int bpv_cmd(void);
static int bpd_cmd(void);
static int swbp_cmd(void);
static int shswbp_cmd(void);
static int swdel_cmd(void);
static int bpregs_cmd(void);
static int bpoff_cmd(void);
static int fill_cmd(void);
static int copy_cmd(void);
static int find_cmd(void);
static int step_cmd(void);
static int skip_cmd(void);
static int dt_cmd(void);
static int ch_cmd(void);
static int ih_cmd(void);
static int recv_cmd(void);
static int send_cmd(void);
static int ct_cmd(void);
static int sp_cmd(void);
static int hs_cmd(void);
static int us_cmd(void);
static int baud_cmd(void);

////////////////////////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////////////////////////

/* Maximum command line */
#define CLI_MAX			128
/* Maximum history */
#define CLI_HISTSIZE	8
/* Last command line (history) */
static char g_lastcli[CLI_HISTSIZE][CLI_MAX];
/* Current command line */
static char g_cli[CLI_MAX];
/* Current position in the command line buffer */
static int  g_cli_pos = 0;
/* Current size of the cli buffer */
static int  g_cli_size = 0;
/* Current insert mode */
static int  g_insert_mode = 0;
/* Last position in the history buffer */
static int  g_lastcli_pos = 0;
/* Current scrolling position in the history buffer */
static int  g_currcli_pos = 0;

/* Pointer to the kernel printf function */
printf_type g_sio_printf;

struct ee_regs* g_currSavedRegs;
/* Saved registers from level2 handler */
extern struct ee_regs _level2SavedRegs;
/* Saved registers from level1 handler */
extern struct ee_regs _level1SavedRegs;
/* Pointer to the syscall table */
struct syscall_table* _syscallTable;
/* Pointer to the stored syscall table (for hooking) */
struct syscall_table* _syscallTableOld;
/* Current exception level */
int	 g_currLevel;
/* Pointer to the kernel thread setup routine */
int (*g_thread_setup)(char *, u32, u32, u32);
/* Break on syscall */
int  g_breakOnSyscall;

////////////////////////////////////////////////////////////////////////////////
// Utility Functions
////////////////////////////////////////////////////////////////////////////////

/* Search pattern for start of printf function */
#define PRINTF_SEARCH_PATTERN "\x70\xFF\xBD\x27\x58\x00\xA5\xFF"


/* Find the kernel printf function */
static void find_sioprintf(void)
{
	u8 *ptr;
	u8 *search = PRINTF_SEARCH_PATTERN;

	ptr = (u8 *) 0x80004000;
	while(ptr < (u8 *) 0x80008000)
	{
		int i;
		int found = 1;

		for(i = 0; i < 8; i++)
		{
			if(ptr[i] != search[i])
			{
				found = 0;
				break;
			}
		}

		if(found)
		{
			printf = (printf_type) ptr;
			break;
		}

		ptr += 4;
	}
}

/* Make the character upper case */
static char upcase(char ch)
{
	if((ch >= 'a') && (ch <= 'z'))
	{
		ch ^= (1 << 5);
	}

	return ch;
}

/* Check if character is a hexadecimal character */
static int is_hex(char ch)
{
	ch = upcase(ch);

	if((ch >= '0') && (ch <= '9'))
		return 1;

	if((ch >= 'A') && (ch <= 'F'))
		return 1;

	return 0;
}

static int is_alnum(char ch)
{
	ch = upcase(ch);
	if((ch >= '0') && (ch <= '9'))
		return 1;

	if((ch >= 'A') && (ch <= 'Z'))
		return 1;

	return 0;
}

/* Check if character is a space/tab */
static int is_space(char ch)
{
	if((ch == ' ') || (ch == '\t'))
	{
		return 1;
	}

	return 0;
}

/* Convert a single hex digit to an int */
static int hex_to_int(char ch)
{
	if((ch >= '0') && (ch <= '9'))
	{
		return ch - '0';
	}

	ch = upcase(ch);
	if((ch >= 'A') && (ch <= 'F'))
	{
		return ch - 'A' + 10;
	}

	return 0;
}

// Convert a hex number to an integer of a set number of nibbles
// Also assumes there is no leading white space strtok should take care of this
static u64 hextoi_generic(const char *hex, int total_nibbles)
{
	int pos = 0;
	int nibbles = 0;
	u64 output = 0;

	// Optional 0x prefix
	if((hex[pos] == '0') && (toupper(hex[pos+1]) == 'X'))
	{
		pos += 2;
	}

	while(hex[pos] && (nibbles < total_nibbles))
	{
		if(is_hex(hex[pos]))
		{
			output = (output << 4) | hex_to_int(hex[pos]);
			nibbles++;
			pos++;
		}
		else
		{
			break;
		}
	}

	return output;
}

/* Convert a hex string to a double word */
static u64 hextod(const char *hex)
{
	return hextoi_generic(hex, 16);
}

/* Conver a hex string to a word */
static u32 hextoi(const char *hex)
{
	return (u32) hextoi_generic(hex, 8);
}

static u32 dectoi(const char *dec)
{
	u32 i = 0;

	while((*dec) && (*dec >= '0') && (*dec <= '9'))
	{
		i *= 10;
		i += *dec - '0';
		dec++;
	}

	return i;
}

/* List of EE register names */
static const unsigned char regName[32][5] =
{
    "zr", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", 
    "t8", "t9", "s0", "s1", "s2", "s3", "s4", "s5",
    "s6", "s7", "k0", "k1", "gp", "sp", "fp", "ra"
};

/* Find a register based on its name */
static u32 *find_reg32(const char *reg)
{
	if(strcasecmp(reg, "ErrorPC") == 0)
	{
		return &g_currSavedRegs->ErrorPC;
	}
	else if(strcasecmp(reg, "EPC") == 0)
	{
		return &g_currSavedRegs->EPC;
	}
	else // Do search of normal regs
	{
		int reg_loop;

		for(reg_loop = 0; reg_loop < 32; reg_loop++)
		{
			if(strcasecmp(regName[reg_loop], reg) == 0)
			{
				return g_currSavedRegs->r[reg_loop].r32;
			}
		}
	}

	return NULL;
}

#define MEM_ATTRIB_READ	 (1 << 0)
#define MEM_ATTRIB_WRITE (1 << 1)
#define MEM_ATTRIB_EXEC	 (1 << 2)
#define MEM_ATTRIB_BYTE  (1 << 3)
#define MEM_ATTRIB_HALF  (1 << 4)
#define MEM_ATTRIB_WORD  (1 << 5)
#define MEM_ATTRIB_DBL   (1 << 6)

#define MEM_ATTRIB_ALL	 0xFFFFFFFF

struct mem_entry
{
	u32 addr;
	s32 size;
	u32 attrib;
};

static struct mem_entry g_memareas[] = 
{
	{ 0, (32 * 1024 * 1024), MEM_ATTRIB_ALL },								/* Normal memory */
	{ 0x80000000, (32 * 1024 * 1024), MEM_ATTRIB_ALL },						/* Kernel memory */
	{ 0xBC000000, (2 * 1024 * 1024), MEM_ATTRIB_ALL & ~MEM_ATTRIB_EXEC },	/* IOP memory */
	{ 0xBFC00000, (32 * 1024 * 1024), MEM_ATTRIB_ALL & ~MEM_ATTRIB_WRITE },	/* Rom0 memory */
	{ 0xBE000000, (32 * 1024 * 1024), MEM_ATTRIB_ALL & ~MEM_ATTRIB_WRITE }, /* Rom1 memory */
	{ 0, 0, 0 }
};

// return the size available that can be read/written (checking)
// If any attrib flags are passed then they must be present in the memory bit field
static s32 validate_memaddr(u32 addr, u32 attrib)
{
	const struct mem_entry *entry;
	int size_left = 0;

	entry = g_memareas;

	while(entry->size != 0)
	{
		if((addr >= entry->addr) && (addr < (entry->addr + (u32) entry->size)))
		{
			// Only pass through areas with valid attributes (e.g. write or execute)
			if((entry->attrib & attrib) == attrib)
			{
				size_left = entry->size - (int) (addr - entry->addr);
			}
			break;
		}

		entry++;
	}

	return size_left;
}

// Return 1 if decode was successful
// Takes forms either $reg or addr or $reg+ofs/$ref-ofs with optional @'s for dereferencing
static int decode_memaddr(const char *straddr, u32 *memaddr)
{
	int ret = 0;
	int deref = 0;
	int i = 0;
	u32 addr = 0;

	/* We are dereferencing */
	while(*straddr == '@')
	{
		/* Incremement the deref count */
		deref++;
		straddr++;
	}

	// This is a register
	if(straddr[0] == '$')
	{
		const u32 *reg_data;
		char reg_name[8];
		int reg_pos;
		int ofs;
		int sign;
		
		straddr++;
		reg_pos = 0;
		while((is_alnum(*straddr)) && (reg_pos < 7))
		{
			reg_name[reg_pos++] = *straddr++;
		}
		reg_name[reg_pos] = 0;

		reg_data = find_reg32(reg_name);
		if(reg_data)
		{
			addr = reg_data[0];

			/* Let's get an offset if it exists */
			do
			{
				if(*straddr == '+')
				{
					sign = 1;
					straddr++;
				}
				else if(*straddr == '-')
				{
					sign = -1;
					straddr++;
				}
				else
				{
					break;
				}

				if(is_hex(*straddr))
				{
					ofs = hextoi(straddr);
					addr += sign * ofs;
				}
			}
			while(0);

			ret = 1;
		}
		else
		{
			printf("Could not find register %s\n", straddr);
			ret = 0;
		}
	}
	else
	{
		if(is_hex(straddr[0]))
		{
			addr = hextoi(straddr);
			ret = 1;
		}
		else
		{
			printf("Could not decode address %s\n", straddr);
			ret = 0;
		}
	}

	/* Only deref if it was valid */
	i = 0;
	while((i < deref) && (ret))
	{
		addr = addr & ~3;
		if(validate_memaddr(addr, MEM_ATTRIB_READ | MEM_ATTRIB_WORD))
		{
			addr = _lw(addr);
		}
		else
		{
			printf("Invalid de-reference at depth %d (%08x)\n", i, addr);
			ret = 0;
		}
		i++;
	}

	if(ret)
	{
		*memaddr = addr;
	}


	return ret;
}

#if 0
// Return 1 if decode was successful
// Takes forms either $reg or addr or $reg[ofs] with optional @'s for dereferencing
static int decode_memaddr(const char *straddr, u32 *memaddr)
{
	int ret = 0;
	int deref = 0;
	int i = 0;
	u32 addr = 0;

	/* We are dereferencing */
	while(*straddr == '@')
	{
		/* Incremement the deref count */
		deref++;
		straddr++;
	}

	// This is a register
	if(straddr[0] == '$')
	{
		const u32 *reg_data;
		
		reg_data = find_reg32(&straddr[1]);
		if(reg_data)
		{
			addr = reg_data[0];
			ret = 1;
		}
		else
		{
			printf("Could not find register %s\n", straddr);
			ret = 0;
		}
	}
	else
	{
		if(is_hex(straddr[0]))
		{
			addr = hextoi(straddr);
			ret = 1;
		}
		else
		{
			printf("Could not decode address %s\n", straddr);
			ret = 0;
		}
	}

	/* Only deref if it was valid */
	i = 0;
	while((i < deref) && (ret))
	{
		addr = addr & ~3;
		if(validate_memaddr(addr, MEM_ATTRIB_READ | MEM_ATTRIB_WORD))
		{
			addr = _lw(addr);
		}
		else
		{
			printf("Invalid de-reference at depth %d (%08x)\n", i, addr);
			ret = 0;
		}
		i++;
	}

	if(ret)
	{
		*memaddr = addr;
	}


	return ret;
}
#endif

////////////////////////////////////////////////////////////////////////////////
// Debugging functions
////////////////////////////////////////////////////////////////////////////////

#define MAX_SWBREAKS	16
#define SW_BREAK_INST	0x0000000d

/* Define a software breakpoint structure */
struct sw_breakpoint
{
	u32 addr;
	u32 oldinst;
	u32 active;
};

struct sw_breakpoint g_swbreaks[MAX_SWBREAKS];
// Software breakpoints used for stepping
struct sw_breakpoint g_stepbp[2];

/* Initialise the software breakpoint array */
static void init_swbps(void)
{
	int i;

	for(i = 0; i < MAX_SWBREAKS; i++)
	{
		g_swbreaks[i].active = 0;
	}

	g_stepbp[0].active = 0;
	g_stepbp[1].active = 0;
}

/* Find a software breakpoint based on its address */
static int find_swbp(u32 addr)
{
	int bp;

	for(bp = 0; bp < MAX_SWBREAKS; bp++)
	{
		if((g_swbreaks[bp].active) && (g_swbreaks[bp].addr == addr))
		{
			break;
		}
	}

	if(bp < MAX_SWBREAKS)
	{
		return bp;
	}
	
	/* let's check the step breakpoints */
	if((g_stepbp[0].active) && (g_stepbp[0].addr == addr))
	{
		return MAX_SWBREAKS;
	}
	else if((g_stepbp[1].active) && (g_stepbp[1].addr == addr))
	{
		return MAX_SWBREAKS + 1;
	}

	return -1;
}

/* Clear a software breakpoint */
static void clear_swbp(int bp)
{
	if(bp >= MAX_SWBREAKS)
	{
		if(g_stepbp[0].active)
		{
			_sw(g_stepbp[0].oldinst, g_stepbp[0].addr);
			g_stepbp[0].active = 0;
		}

		if(g_stepbp[1].active)
		{
			_sw(g_stepbp[1].oldinst, g_stepbp[1].addr);
			g_stepbp[1].active = 0;
		}
	}
	else
	{
		_sw(g_swbreaks[bp].oldinst, g_swbreaks[bp].addr);
		g_swbreaks[bp].active = 0;
	}

	flush_cache();
}

/* Find a free (non active) break point. Returns -1 if not found */
static int find_freeswbp(void)
{
	int bp;

	for(bp = 0; bp < MAX_SWBREAKS; bp++)
	{
		if(g_swbreaks[bp].active == 0)
		{
			break;
		}
	}

	if(bp < MAX_SWBREAKS)
	{
		return bp;
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////
// Exception Handlers
////////////////////////////////////////////////////////////////////////////////

extern void level1ExceptionHandler(void);
extern void level2ExceptionHandler(void);

/* Install a V_DEBUG handler in place of the kernel one */
static void install_debug_handler(void)
{
   u32 data;

   /* Build a 'j level2ExceptionHandler' instruction */
   data = (u32) level2ExceptionHandler & 0xFFFFFFF;
   data = 0x8000000 | (data >> 2);
   _sw(data, 0x80000100);
   _sw(0, 0x80000104);
   flush_cache();
}

/* Install a Level 1 BREAK and Syscall handler */
static void install_level1_handler(void)
{
	_syscallTable->SetVCommonHandler(9, level1ExceptionHandler);
}


////////////////////////////////////////////////////////////////////////////////
// Shell commands
////////////////////////////////////////////////////////////////////////////////

/* Return values for the commands */
#define CMD_EXITSHELL 	1
#define CMD_OK		  	0
#define CMD_ERROR		-1

/* Structure to hold a single command entry */
struct sh_command 
{
	const char *name;		/* Normal name of the command */
	const char *syn;		/* Synonym of the command */
	int (*func)(void);		/* Pointer to the command function */
	const char *desc;		/* Textual description */
	const char *help;		/* Command usage */
};

/* Define the list of commands */
struct sh_command commands[] = {
	{ "exit", "ex", exit_cmd, "Exit the shell", "exit [retaddr]" },
	{ "memdump", "md", md_cmd, "Dump memory to shell", "memdump addr" },
	{ "pokeb", "pb", pb_cmd, "Poke in bytes", "pokeb pokeaddr value [value2 ...]" },
	{ "pokeh", "ph", ph_cmd, "Poke in half words", "pokeh pokeaddr value [value2 ...]" },
	{ "pokew", "pw", pw_cmd, "Poke in words", "pokew pokeaddr value [value2 ...]" },
	{ "poked", "pd", pd_cmd, "Poke in doubles", "poked pokeaddr value [value2 ...]" },
	{ "fill",  "fi" , fill_cmd, "Fill memory up with a single byte value", "fill memaddr size val"},
	{ "copy",  "cp" , copy_cmd, "Copy memory from one place to another", "copy srcaddr destaddr size"},
	{ "find",  "fd" , find_cmd, "Find an ASCII or hex string in memory", "find startaddr size \"ASCII\"/Hex Values [mask M1...]" },
	{ "dumpreg", "dr", dr_cmd, "Dump EE regs", "dumpreg" },
	{ "editreg", "er", er_cmd, "Edit an EE reg (32bit only)", "editreg $reg value" },
	{ "exec", NULL, exec_cmd, "Exec a program (LoadExecPS2)", "exec elfname [arg1 arg2 ...]" },
	{ "start", NULL, start_cmd, "Start an already loaded program (ExecPS2)", "start addr [arg1 arg2 ...]" }, 
	{ "disasm", "dis", dis_cmd, "Disassemble instructions", "disasm disaddr [count]" },
	{ "bpinst", "bpi", bpi_cmd, "Set the hardware instruction bp", "bpinst address mask [uskx]" },
	{ "bpdata", "bpd", bpd_cmd, "Set the hardware data bp", "bpdata address mask [rwuskx]" },
	{ "bpval", "bpv", bpv_cmd, "Set the hardware data value bp", "bpval address amask value vmask [rwuskx]]" },
	{ "swbp", NULL, swbp_cmd, "Set a software breakpoint", "swbp address" },
	{ "shswbp", NULL, shswbp_cmd, "Show the current software breakpoints", "shswbp" },
	{ "swdel", NULL, swdel_cmd, "Delete a currently set software breakpoint", "swdel num" },
	{ "bpregs", "bpr", bpregs_cmd, "Dump the breakpoint registers", "bpregs"},
	{ "bpoff", "bpo", bpoff_cmd, "Disable the hardware breakpoint(s)", "dboff" },
	{ "step", "s", step_cmd, "Step to the next instruction", "step" },
	{ "skip", "k", skip_cmd, "Step the next command, skipping sub routine jump", "skip" },
	{ "dumpt", "dt", dt_cmd, "Dump the current thread list", "dumpt" },
	{ "recv", NULL, recv_cmd, "Receive a data block using ymodem", "recv startaddr [maxsize]" },
	{ "send", NULL, send_cmd, "Send a data block using ymodem", "send startaddr size filename" },
	{ "createt", "ct", ct_cmd, "Create a new thread on the EE", "createt startaddr stackaddr priority" },
	{ "sprint", "sp", sp_cmd, "Write out a print handler to EE memeory", "sprint startaddr" },
	{ "hooksys", "hs", hs_cmd, "Hook the syscall handler and optionally break", "hooksys [break]" },
	{ "unhksys", "us", us_cmd, "Unhook the syscall handler", "unhksys" },
	{ "baud", NULL, baud_cmd, "Set the SIO baud rate", "baud (9600|19200|38400|57600|115200)" },
	{ "clihist", "ch", ch_cmd, "Displays the CLI history", "clihist" },
	{ "insthdl", "ih", ih_cmd, "Install EE exception handlers for common errors", "insthld" },
	{ "help", "?", help_cmd, "Help (Obviously)", "help [command]" },
	{ NULL, NULL, NULL, NULL }
};

/* Find a command from the command list */
static struct sh_command* find_command(const char *cmd)
{
	struct sh_command* found_cmd = NULL;
	int cmd_loop;

	for(cmd_loop = 0; commands[cmd_loop].name != NULL; cmd_loop++)
	{
		if(strcasecmp(cmd, commands[cmd_loop].name) == 0)
		{
			found_cmd = &commands[cmd_loop];
			break;
		}

		if(commands[cmd_loop].syn)
		{
			if(strcasecmp(cmd, commands[cmd_loop].syn) == 0)
			{
				found_cmd = &commands[cmd_loop];
				break;
			}
		}
	}

	return found_cmd;
}

/* Exit command */
static int exit_cmd(void)
{
	char *retaddr;
	u32 ret = 0xffffffff;

	retaddr = strtok(NULL, " \t");
	if(retaddr)
	{
		ret = hextoi(retaddr);
		ret &= ~3;
		if(validate_memaddr(ret, MEM_ATTRIB_EXEC))
		{
			if(g_currLevel == 1)
			{
				g_currSavedRegs->EPC = ret;
				printf("Setting EPC to %08x\n", ret);
			}
			else
			{
				g_currSavedRegs->ErrorPC = ret;
				printf("Setting ErrorPC to %08x\n", ret);
			}
		}
	}

	printf("Exiting Shell\n");

	return CMD_EXITSHELL;
}

/* Maximum memory dump size (per screen) */
#define MAX_MEMDUMP_SIZE 256

/* Print a row of a memory dump, up to row_size */
static void print_row(const u32* row, s32 row_size, u32 addr)
{
	int i = 0;

	printf("%08x - ", addr);
	for(i = 0; i < 16; i++)
	{
		if(i < row_size)
		{
			printf("%02x ", row[i]);
		}
		else
		{
			printf("-- ", row[i]);
		}
	}

	printf("- ");
	for(i = 0; i < 16; i++)
	{
		if(i < row_size)
		{
			if((row[i] >= 32) && (row[i] < 127))
			{
				sio_putc(row[i]);
			}
			else
			{
				sio_putc('.');
			}
		}
		else
		{
			sio_putc('.');
		}
	}

	printf("\n");
}

/* Print a memory dump to SIO */
static void print_memdump(u32 addr, s32 size)
{
	int size_left;
	u32 row[16];
	int row_size;
	u8 *p_addr = (u8 *) addr;

	printf("         - 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f - 0123456789abcdef\n");
	printf("-----------------------------------------------------------------------------\n");

	size_left = size > MAX_MEMDUMP_SIZE ? MAX_MEMDUMP_SIZE : size;
	row_size = 0;

	while(size_left > 0)
	{
		row[row_size] = p_addr[row_size];
		row_size++;
		if(row_size == 16)
		{
			// draw row
			print_row(row, row_size, (u32) p_addr);
			p_addr += 16;
			row_size = 0;
		}

		size_left--;
	}
}

/* Memory dump command */
static int md_cmd(void)
{
	char *mem_addr;
	u32 addr;
	s32 size_left;

	// Get memory address
	mem_addr = strtok(NULL, " ");
	if((mem_addr != NULL) && (decode_memaddr(mem_addr, &addr)))
	{
		size_left = validate_memaddr(addr, MEM_ATTRIB_READ | MEM_ATTRIB_BYTE);

		if(size_left > 0)
		{
			while(size_left > 0)
			{
				char ch;

				print_memdump(addr, size_left);

				printf("Press b to go back, space to go forward, or q to quit.\n");
				while((ch = sio_getc()) == -1);

				ch = upcase(ch);
				if(ch == 'Q')
				{
					break;
				}

				if(ch == 'B')
				{
					addr -= MAX_MEMDUMP_SIZE;
				}
				else
				{
					addr += MAX_MEMDUMP_SIZE;
				}

				size_left = validate_memaddr(addr, MEM_ATTRIB_READ | MEM_ATTRIB_BYTE);
			}
		}
		else
		{
			printf("Invalid memory address %x\n", addr);
		}
	}
	else
	{
		return CMD_ERROR;
	}

	return CMD_OK;
}

/* generic poke command, size is the number of bytes to poke */
static int poke_generic(int size)
{
	if((size == 1) || (size == 2) || (size == 4) || (size == 8))
	{
		char *mem_addr;
		char *mem_data;
		u32  addr_mask;
		u32 addr;
		u64 data;
		u32 mem_attrib = 0;

		switch(size)
		{
			case 1: addr_mask = 0xFFFFFFFF;
					mem_attrib = MEM_ATTRIB_BYTE;
					break;
			case 2: addr_mask = 0xFFFFFFFE;
					mem_attrib = MEM_ATTRIB_HALF;
					break;
			case 4: addr_mask = 0xFFFFFFFC;
					mem_attrib = MEM_ATTRIB_WORD;
					break;
			case 8: addr_mask = 0xFFFFFFF8;
					mem_attrib = MEM_ATTRIB_DBL;
					break;
			default: // should not get here
					return 0;
		};

		mem_addr = strtok(NULL, " \t");
		mem_data = strtok(NULL, " \t");

		if((mem_addr) && (mem_data) && (decode_memaddr(mem_addr, &addr)))
		{
			addr &= addr_mask;
			while(mem_data != NULL)
			{
				data = hextod(mem_data);
				if(validate_memaddr(addr, mem_attrib | MEM_ATTRIB_READ) >= size)
				{
					switch(size)
					{
						case 1: _sb(data & 0xFF, addr);
								break;
						case 2: _sh(data & 0xFFFF, addr); 
								break;
						case 4: _sw(data & 0xFFFFFFFF, addr);
								break;
						case 8: _sd(data, addr);
								break;
						default: // should not get here
								break;
					};

				}
				else
				{
					printf("Invalid poke memory address %08x\n", addr);
					break;
				}
				addr += size;
				mem_data = strtok(NULL, " \t");
			}

			flush_cache();
		}
		else
		{
			return CMD_ERROR;
		}
	}

	return CMD_OK;
}

/* Poke byte command */
static int pb_cmd(void)
{
	return poke_generic(1);
}

/* Poke half word command */
static int ph_cmd(void)
{
	return poke_generic(2);
}

/* Poke word command */
static int pw_cmd(void)
{
	return poke_generic(4);
}

/* Poke double command */
static int pd_cmd(void)
{
	return poke_generic(8);
}

static u32 get_bpi_opts(const char *str_opts)
{
	u32 opts = BPC_IUE | BPC_ISE | BPC_IKE | BPC_IXE;

	if((str_opts) && (str_opts[0] != 0))
	{
		opts = 0;
		while(*str_opts)
		{
			switch(upcase(*str_opts))
			{
				case 'U' : opts |= BPC_IUE;
						   break;
				case 'S' : opts |= BPC_ISE;
						   break;
				case 'K' : opts |= BPC_IKE;
						   break;
				case 'X' : opts |= BPC_IXE;
						   break;
				default:
						   printf("Unknown option %c\n", *str_opts);
						   break;
			};
			str_opts++;
		}

		if((opts & (BPC_IUE | BPC_ISE | BPC_IKE | BPC_IXE)) == 0)
		{
			printf("Could not determine mode setting. Setting default\n");
			opts |= BPC_IUE | BPC_ISE | BPC_IKE | BPC_IXE;
		}
	}

	return opts;
}

/* Set instruction breakpoint (hw) command */
static int bpi_cmd(void)
{
	char *mem_addr;
	char *mem_mask;
	char *mem_opts;
	u32  addr;
	u32  mask;
	u32  opts;

	mem_addr = strtok(NULL, " \t");
	mem_mask = strtok(NULL, " \t");
	mem_opts = strtok(NULL, " \t");
	if((mem_addr) && (mem_mask) && (decode_memaddr(mem_addr, &addr)))
	{
		opts = get_bpi_opts(mem_opts);
		mask = hextoi(mem_mask);
		printf("Setting Instruction BP at %08x mask %08x\n", addr, mask);
		SetInstructionBP(addr, mask, opts);
		printf("BPC: %08x\n", GetBPC());
	}
	else
	{
		return CMD_ERROR;
	}

	return CMD_OK;
}

static u32 get_bpd_opts(const char *str_opts)
{
	u32 opts = BPC_DRE | BPC_DWE | BPC_DUE | BPC_DSE | BPC_DKE | BPC_DXE;

	if((str_opts) && (str_opts[0] != 0))
	{
		opts = 0;
		while(*str_opts)
		{
			switch(upcase(*str_opts))
			{
				case 'R' : opts |= BPC_DRE;
						   break;
				case 'W' : opts |= BPC_DWE;
						   break;
				case 'U' : opts |= BPC_DUE;
						   break;
				case 'S' : opts |= BPC_DSE;
						   break;
				case 'K' : opts |= BPC_DKE;
						   break;
				case 'X' : opts |= BPC_DXE;
						   break;
				default:
						   printf("Unknown option %c\n", *str_opts);
						   break;
			};
			str_opts++;
		}
		if((opts & (BPC_DRE | BPC_DWE)) == 0)
		{
			printf("Could not determine read/write. Setting default\n");
			opts |= BPC_DRE | BPC_DWE;
		}

		if((opts & (BPC_DUE | BPC_DSE | BPC_DKE | BPC_DXE)) == 0)
		{
			printf("Could not determine mode setting. Setting default\n");
			opts |= BPC_DUE | BPC_DSE | BPC_DKE | BPC_DXE;
		}
	}

	return opts;
}

/* Set a data breakpoint command */
static int bpd_cmd(void)
{
	char *mem_addr;
	char *mem_mask;
	char *mem_opts;
	u32  addr;
	u32  mask;
	u32  opts;

	mem_addr = strtok(NULL, " \t");
	mem_mask = strtok(NULL, " \t");
	mem_opts = strtok(NULL, " \t");
	if((mem_addr) && (mem_mask) && (decode_memaddr(mem_addr, &addr)))
	{
		opts = get_bpd_opts(mem_opts);
		mask = hextoi(mem_mask);
		printf("Data Address BP at %08x mask %08x opts %s\n", addr, mask,
				mem_opts == NULL ? "rwuskx" : mem_opts);

		SetDataAddrBP(addr, mask, opts);
		printf("BPC: %08x\n", GetBPC());
	}
	else
	{
		return CMD_ERROR;
	}

	return CMD_OK;
}

/* Set a data value breakpoint command */
static int bpv_cmd(void)
{
	char *mem_addr;
	char *mem_mask;
	char *mem_val;
	char *mem_valmask;
	char *mem_opts;
	u32  addr;
	u32  mask;
	u32  val;
	u32  valmask;
	u32  opts;

	mem_addr = strtok(NULL, " \t");
	mem_mask = strtok(NULL, " \t");
	mem_val  = strtok(NULL, " \t");
	mem_valmask = strtok(NULL, " \r");
	mem_opts = strtok(NULL, " \t");
	if((mem_addr) && (mem_mask) && (mem_val) && (mem_valmask) && (decode_memaddr(mem_addr, &addr)))
	{
		opts = get_bpd_opts(mem_opts);
		mask = hextoi(mem_mask);
		val  = hextoi(mem_val);
		valmask = hextoi(mem_valmask);

		printf("Data Value BP %08x msk %08x val %08x vmsk %08x %s\n", addr, mask,
				val, valmask, mem_opts == NULL ? "rwuskx" : mem_opts);

		SetDataValueBP(addr, mask, val, valmask, opts);
		printf("BPC: %08x\n", GetBPC());
	}
	else
	{
		return CMD_ERROR;
	}

	return CMD_OK;
}

/* Dump breakpoint registers command */
static int bpregs_cmd(void)
{
	u32 bpc;

	bpc = GetBPC();
	if((bpc & BPC_BED) != 0)
	{
		printf("Breakpoint exceptions disabled\n");
	}

	if(bpc & BPC_IAE)
	{
		char opts[5];
		char *pOpts = opts;

		if(bpc & BPC_IUE)
		{
			*pOpts++ = 'u';
		}

		if(bpc & BPC_ISE)
		{
			*pOpts++ = 's';
		}

		if(bpc & BPC_IKE)
		{
			*pOpts++ = 'k';
		}

		if(bpc & BPC_IXE)
		{
			*pOpts++ = 'x';
		}

		*pOpts = 0;

		printf("Instruction BP addr %08x mask %08x opts '%s'\n", GetIAB(), GetIABM(), opts);
	}

	if(bpc & (BPC_DRE | BPC_DWE | BPC_DVE))
	{
		char opts[7];
		char *pOpts = opts;

		if(bpc & BPC_DRE)
		{
			*pOpts++ = 'r';
		}

		if(bpc & BPC_DWE)
		{
			*pOpts++ = 'w';
		}

		if(bpc & BPC_DUE)
		{
			*pOpts++ = 'u';
		}

		if(bpc & BPC_DSE)
		{
			*pOpts++ = 's';
		}

		if(bpc & BPC_DKE)
		{
			*pOpts++ = 'k';
		}

		if(bpc & BPC_DXE)
		{
			*pOpts++ = 'x';
		}

		*pOpts = 0;

		if(bpc & BPC_DVE)
		{
			printf("Data Val BP addr %08x msk %08x val %08x vmsk %08x opts '%s'\n", 
					GetDAB(), GetDABM(), GetDVB(), GetDVBM(), opts);
		}
		else
		{
			printf("Data Addr BP addr %08x mask %08x opts '%s'\n", GetDAB(), GetDABM(), opts);
		}
	}

	return CMD_OK;
}

/* Disable breakpoints command */
static int bpoff_cmd(void)
{
	// Just call initBPC again
	InitBPC();

	return CMD_OK;
}

/* Help command */
static int help_cmd(void)
{
	int cmd_loop;
	char *cmd;

	cmd = strtok(NULL, " \t");

	if(cmd == NULL)
	{
		printf("Command Help\n\n");
		for(cmd_loop = 0; commands[cmd_loop].name; cmd_loop++)
		{
			printf("%s\t- %s\n", commands[cmd_loop].name, commands[cmd_loop].desc);
			if((cmd_loop % 24) == 20)
			{
				char ch;
				printf("Press any key to continue, or q to quit\n");

				while((ch = sio_getc()) == -1);
				ch = upcase(ch);
				if(ch == 'Q')
				{
					break;
				}
			}
		}
	}
	else
	{
		struct sh_command* found_cmd;

		found_cmd = find_command(cmd);
		if(found_cmd != NULL)
		{
			printf("%s\t - %s\n", found_cmd->name, found_cmd->desc);
			if(found_cmd->syn)
			{
				printf("Synonym: %s\n", found_cmd->syn);
			}
			printf("Usage: %s\n", found_cmd->help);
		}
	}

	return CMD_OK;
}

static void print_regs(void)
{
	int i;

	printf("EPC      %08x\n", g_currSavedRegs->EPC);
	printf("ErrorPC  %08x\n", g_currSavedRegs->ErrorPC);
	printf("Cause    %08x\n", g_currSavedRegs->Cause);
	printf("Status   %08x\n", g_currSavedRegs->Status);
	for(i = 0; i < 32/2; i++)
	{
		printf("%s: ", regName[i]);
		printf("%08x %08x ", g_currSavedRegs->r[i].r32[3], g_currSavedRegs->r[i].r32[2]);
		printf("%08x %08x ", g_currSavedRegs->r[i].r32[1], g_currSavedRegs->r[i].r32[0]);

		printf("%s: ", regName[i+16]);
		printf("%08x %08x ", g_currSavedRegs->r[i+16].r32[3], g_currSavedRegs->r[i+16].r32[2]);
		printf("%08x %08x ", g_currSavedRegs->r[i+16].r32[1], g_currSavedRegs->r[i+16].r32[0]);
		printf("\n");
	}
}

/* Dump registers command */
static int dr_cmd(void)
{
	print_regs();

	return CMD_OK;
}

/* Edit register command (32bit only) */
static int er_cmd(void)
{
	char *reg;
	char *value;
	int ret = CMD_OK;

	reg   = strtok(NULL, " \t");
	value = strtok(NULL, " \t");
	if(reg && value)
	{
		u32 *pReg;

		if(reg[0] == '$')
		{
			pReg = find_reg32(&reg[1]);
		}
		else
		{
			pReg = find_reg32(reg);
		}

		if(pReg)
		{
			u32 val;

			val = hextoi(value);
			pReg[0] = val;
		}
		else
		{
			printf("Cannot find register %s\n", reg);
		}
	}
	else
	{
		ret = CMD_ERROR;
	}

	return ret;
}

#define MAX_ELF_PATH	260
#define MAX_ARG_LENGTH	260
#define MAX_ARG_PTRS	16
static char g_elf_name[MAX_ELF_PATH];
static char g_elf_args[MAX_ARG_LENGTH];
static char *g_elf_args_ptr[MAX_ARG_PTRS];

/* Simple arg builder. Doesn't account for spaces in args */
static int exec_build_args(void)
{
	char *arg;
	int arg_pos;
	int arg_count;

	arg_pos = 0;
	arg_count = 0;
	arg = strtok(NULL, " \t");
	while((arg) && (arg_pos < (MAX_ARG_LENGTH-1)) && (arg_count < MAX_ARG_PTRS))
	{
		/* Ok dirty I know, so sue me :P */
		g_elf_args_ptr[arg_count++] = &g_elf_args[arg_pos];
		while((*arg) && (arg_pos < (MAX_ARG_LENGTH-1)))
		{
			g_elf_args[arg_pos++] = *arg++;
		}
		g_elf_args[arg_pos++] = 0;

		arg = strtok(NULL, " \t");
	}

	return arg_count;
}

/* Execute EE program command */
int exec_cmd(void)
{
	char *elf_name;
	//u32 inst;
	u32 exec_addr;
	int ret;
	ee_thread_t thread;
	int arg_count;

	elf_name = strtok(NULL, " \t");
	if(elf_name != NULL)
	{
		int pos = 0;

		while(elf_name[pos])
		{
			g_elf_name[pos] = elf_name[pos];
			pos++;
		}
		g_elf_name[pos] = 0;

		arg_count = exec_build_args();

		printf("Executing %s\n", g_elf_name);

		/* Get current thread */
		ret = _syscallTable->ReferThreadStatus(0, &thread);
		/* If it is EENULL then create a new thread at 0x100000 */
		if(thread.func < (void *) 0x90000)
		{
			exec_addr = 0x100000;
			/* Create a thread for our boot stub, use ROMVER cause we can :) */
			g_thread_setup("ROMVER", 0x100000, 32 * 1024 * 1024 - 0x10000, 0);
		}
		else /* Already a thread (user app) running */
		{
			if(g_currLevel == 1)
			{
				exec_addr = g_currSavedRegs->EPC;
			}
			else
			{
				exec_addr = g_currSavedRegs->ErrorPC;
			}
		}

		/* Write in our code stub */
		/* load address of the exec path, zero a1 and a2 then do LoadExecPS2 */
		_sw(0x3c040000 | (((u32) g_elf_name) >> 16), exec_addr);
		_sw(0x34840000 | (((u32) g_elf_name) & 0xFFFF), exec_addr + 4);

		if(arg_count > 0)
		{
			_sw(0x20050000 | arg_count, exec_addr + 8);
			_sw(0x3c060000 | (((u32) g_elf_args_ptr) >> 16), exec_addr + 12);
			_sw(0x34c60000 | (((u32) g_elf_args_ptr) & 0xFFFF), exec_addr + 16);
		}
		else
		{
			_sw(0x3c050000, exec_addr + 8);
			_sw(0x3c060000, exec_addr + 12);
			_sw(0x00000000, exec_addr + 16);
		}

		_sw(0x24030006, exec_addr + 20);
		_sw(0x0000000c, exec_addr + 24);
		_sw(0x03e00008, exec_addr + 28);
		_sw(0x00000000, exec_addr + 32);

		/* Set return PC to the start of our stub */
		if(g_currLevel == 1)
		{
			if(g_currSavedRegs->EPC != exec_addr)
			{
				g_currSavedRegs->EPC = exec_addr;
			}
		}
		else
		{
			if(g_currSavedRegs->ErrorPC != exec_addr)
			{
				g_currSavedRegs->ErrorPC = exec_addr;
			}
		}
		
		flush_cache();

		return CMD_EXITSHELL;
	}
	else
	{
		return CMD_ERROR;
	}

	return CMD_OK;
}

static int start_cmd(void)
{
	char *mem_addr;
	u32 exec_addr;
	int ret;
	ee_thread_t thread;
	int arg_count;

	mem_addr = strtok(NULL, " \t");
	if(mem_addr)
	{
		u32 addr;

		addr = hextoi(mem_addr);
		if(validate_memaddr(addr, MEM_ATTRIB_EXEC) == 0)
		{
			printf("Invalid execute address\n");
			return CMD_ERROR;
		}

		arg_count = exec_build_args();

		printf("Executing from %08x\n", addr);

		/* Get current thread */
		ret = _syscallTable->ReferThreadStatus(0, &thread);
		/* If it is EENULL then create a new thread at 0x100000 */
		if(thread.func < (void *) 0x90000)
		{
			exec_addr = 0xfff00;
			/* Create a thread for our boot stub, use ROMVER cause we can :) */
			g_thread_setup("ROMVER", exec_addr, 32 * 1024 * 1024 - 0x10000, 0);
		}
		else /* Already a thread (user app) running */
		{
			if(g_currLevel == 1)
			{
				exec_addr = g_currSavedRegs->EPC;
			}
			else
			{
				exec_addr = g_currSavedRegs->ErrorPC;
			}
		}

		/* Write in our code stub */
		/* Set a0 to be the start address */
		_sw(0x3c040000 | (addr >> 16), exec_addr);
		_sw(0x34840000 | (addr & 0xFFFF), exec_addr + 4);

		if(arg_count > 0)
		{
			/* Set gp to 0 */
			_sw(0x3c050000, exec_addr + 8);
			/* Set arg count in a2 */
			_sw(0x20060000 | arg_count, exec_addr + 12);
			/* Set arg pointer in a3 */	
			_sw(0x3c070000 | (((u32) g_elf_args_ptr) >> 16), exec_addr + 16);
			_sw(0x34e70000 | (((u32) g_elf_args_ptr) & 0xFFFF), exec_addr + 20);
		}
		else
		{
			_sw(0x3c050000, exec_addr + 8);
			_sw(0x3c060000, exec_addr + 12);
			_sw(0x3c070000, exec_addr + 16);
			_sw(0x00000000, exec_addr + 20);
		}

		_sw(0x24030007, exec_addr + 24);
		_sw(0x0000000c, exec_addr + 28);
		_sw(0x03e00008, exec_addr + 32);
		_sw(0x00000000, exec_addr + 36);

		/* Set return PC to the start of our stub */
		if(g_currLevel == 1)
		{
			if(g_currSavedRegs->EPC != exec_addr)
			{
				g_currSavedRegs->EPC = exec_addr;
			}
		}
		else
		{
			if(g_currSavedRegs->ErrorPC != exec_addr)
			{
				g_currSavedRegs->ErrorPC = exec_addr;
			}
		}
		
		flush_cache();

		return CMD_EXITSHELL;
	}
	else
	{
		return CMD_ERROR;
	}

	return CMD_OK;

}

/* Disassemble command */
static int dis_cmd(void)
{
	char *addr;
	char *count;
	u32 disaddr;
	/* Kinda useless otherwise */
	u32 discount = 8;

	addr = strtok(NULL, " \t");
	count = strtok(NULL, "  \t");

	if(addr == NULL)
	{
		if(g_currLevel == 1)
		{
			addr = "$epc";
		}
		else
		{
			addr = "$errorpc";
		}
	}

	if(decode_memaddr(addr, &disaddr))
	{
		int bytes_left;

		disaddr &= ~3;

		if(count != NULL)
		{
			discount = hextoi(count);
		}

		bytes_left = validate_memaddr(disaddr, MEM_ATTRIB_READ | MEM_ATTRIB_WORD);
		if(bytes_left > 4)
		{
			discount = discount > (bytes_left / 4) ? bytes_left / 4 : discount;
		}

		if(discount > 0)
		{
			disasm((u32 *) disaddr, discount);
		}
	}
	else
	{
		return CMD_ERROR;
	}

	return CMD_OK;
}

/* Set a software breakpoint command */
static int swbp_cmd(void)
{
	char *mem_addr;
	u32  addr;

	mem_addr = strtok(NULL, " \t");
	if((mem_addr) && (decode_memaddr(mem_addr, &addr)))
	{
		int bp;

		bp = find_freeswbp();
		if(bp >= 0)
		{
			addr &= ~3;
			if(validate_memaddr(addr, MEM_ATTRIB_WRITE | MEM_ATTRIB_EXEC | MEM_ATTRIB_WORD) >= 4)
			{
				g_swbreaks[bp].oldinst = _lw(addr);
				_sw(SW_BREAK_INST, addr);
				g_swbreaks[bp].active = 1;
				g_swbreaks[bp].addr = addr;
				flush_cache();
				printf("Set software breakpoint at %08x\n", addr);
			}
			else
			{
				printf("Invalid software breakpoint address\n");
			}
		}
		else
		{
			printf("No available software breakpoints\n");
		}
	}

	return CMD_OK;
}

/* Show software breakpoints command */
static int shswbp_cmd(void)
{
	int bp;

	printf("Software Breakpoints\n");
	for(bp = 0; bp < MAX_SWBREAKS; bp++)
	{
		if(g_swbreaks[bp].active)
		{
			printf("%x : Address %08x\n", bp, g_swbreaks[bp].addr);
		}
	}

	return CMD_OK;
}

/* Delete a software breakpoint command */
static int swdel_cmd(void)
{
	char *bp_num;
	int bp;
	int ret = CMD_OK;

	bp_num = strtok(NULL, " \t");
	if((bp_num) && (is_hex(bp_num[0])))
	{
		bp = hextoi(bp_num);
		if((bp >= 0) && (bp < MAX_SWBREAKS))
		{
			if(g_swbreaks[bp].active)
			{
				// Assume it is a valid address ?
				u32 inst;

				inst = _lw(g_swbreaks[bp].addr);
				// Check we have not got stale data, only delete if actually a break
				if(inst == SW_BREAK_INST)
				{
					_sw(g_swbreaks[bp].oldinst, g_swbreaks[bp].addr);
					flush_cache();
				}
				g_swbreaks[bp].active = 0;
			}
		}
	}
	else
	{
		ret = CMD_ERROR;
	}

	return ret;
}

/* Scan through memory and find strings */
static void find_string(const u8 *saddr, u32 size, const u8 *search, u32 search_size)
{
	int size_left;

	size_left = validate_memaddr((u32) saddr, MEM_ATTRIB_BYTE | MEM_ATTRIB_READ);
	if(size_left > size)
	{
		size_left = size;
	}
			
	while(size_left >= search_size)
	{
		int pos;

		pos = 0;
		while(pos < search_size)
		{
			if(saddr[pos] != search[pos])
			{
				break;
			}
			pos++;
		}

		if(pos == search_size)
		{
			char ch;
			printf("Match found at %08x\n", (u32) saddr);
			printf("Press Q to quit, D to dump memory or any other key to continue\n");
			while((ch = sio_getc()) == -1);

			ch = upcase(ch);
			if(ch == 'Q')
			{
				break;
			}
			else if(ch == 'D')
			{
				print_memdump((u32) saddr, size_left);
				printf("Press any key to continue\n");
				while((ch = sio_getc()) == -1);
			}
		}

		saddr++;
		size_left--;
	}
}

/* Find memory string command */
static int find_cmd(void)
{
	char *mem_saddr;
	char *mem_size;
	char *mem_search;
	u32 saddr;
	int ret = CMD_OK;
	u8 search_str[CLI_MAX];
	int search_size = 0;

	mem_saddr = strtok(NULL, " \t");
	mem_size = strtok(NULL, " \t");
	mem_search = strtok(NULL, "");

	if((mem_saddr) && (mem_size) && (mem_search) 
			&& decode_memaddr(mem_saddr, &saddr))
	{
		u32 size;

		size = hextoi(mem_size);

		while((*mem_search) && (is_space(*mem_search)))
		{
			mem_search++;
		}

		if(*mem_search == 0)
		{
			printf("No search string specified\n");
		}
		else
		{
			// ASCII String
			if(*mem_search == '"')
			{
				mem_search++;
				while((*mem_search) && (*mem_search != '"'))
				{
					search_str[search_size++] = *mem_search;
					mem_search++;
				}
			}
			else
			{
				char *byte;
				
				byte = strtok(mem_search, " \t");
				while((byte) && (is_hex(*byte)))
				{
					u32 val;

					val = hextoi(byte);
					search_str[search_size++] = (u8) (val & 0xff);
					byte = strtok(NULL, " \t");
				}
			}

			if(search_size > 0)
			{
				printf("Searching from %08x size %x search size %x\n", saddr, size, search_size);
				find_string((u8 *) saddr, size, search_str, search_size);
			}
			else
			{
				printf("No search string specified\n");
			}
		}
	}
	else
	{
		ret = CMD_ERROR;
	}

	return ret;
}

/* Copy memory command */
static int copy_cmd(void)
{
	char *mem_saddr;
	char *mem_daddr;
	char *mem_size;
	int ret = CMD_OK;

	mem_saddr = strtok(NULL, " \t");
	mem_daddr = strtok(NULL, " \t");
	mem_size = strtok(NULL, " \t");

	if((mem_saddr) && (mem_daddr) && (mem_size))
	{
		u32 saddr;
		u32 daddr;
		u32 size;
		int sbytes_left;
		int dbytes_left;
		int bytes_left;

		saddr = hextoi(mem_saddr);
		daddr = hextoi(mem_daddr);
		size = hextoi(mem_size);

		sbytes_left = validate_memaddr(saddr, MEM_ATTRIB_READ  | MEM_ATTRIB_BYTE);
		dbytes_left = validate_memaddr(daddr, MEM_ATTRIB_WRITE | MEM_ATTRIB_BYTE);

		// Get smaller of the two amounts
		bytes_left = dbytes_left < sbytes_left ? dbytes_left : sbytes_left;
		size = bytes_left < size ? bytes_left : size;

		if(size > 0)
		{
			printf("Copying %x bytes from %08x to %08x\n", size, saddr, daddr);
			memmove((void *) daddr, (void *) saddr, size);
			flush_cache();
		}
		else
		{
			printf("Invalid memory region(s) specified\n");
		}
	}
	else
	{
		ret = CMD_ERROR;
	}

	return ret;
}

/* Fill memory command */
static int fill_cmd(void)
{
	char *mem_addr;
	char *mem_size;
	char *mem_val;
	int ret = CMD_OK;

	mem_addr = strtok(NULL, " \t");
	mem_size = strtok(NULL, " \t");
	mem_val = strtok(NULL, " \t");

	if((mem_addr) && (mem_size) && (mem_val))
	{
		u32 addr;
		u32 size;
		u32 val;
		u32 bytes_left;

		addr = hextoi(mem_addr);
		size = hextoi(mem_size);
		val = hextoi(mem_val);

		bytes_left = validate_memaddr(addr, MEM_ATTRIB_WRITE | MEM_ATTRIB_BYTE);
		if(bytes_left < size)
		{
			size = bytes_left;
		}

		if(size > 0)
		{
			printf("Filling memory from %08x size %x with value %x\n", addr, size, val);
			memset((void *) addr, val, size);
			flush_cache();
		}
		else
		{
			printf("Invalid memory address %08x\n", addr);
		}
	}
	else
	{
		ret = CMD_ERROR;
	}

	return ret;
}

/* Define some opcode stuff for the stepping function */
#define BEQ_OPCODE		0x4
#define BEQL_OPCODE		0x14
#define BGTZ_OPCODE 	0x7
#define BGTZL_OPCODE	0x17
#define BLEZ_OPCODE		0x6
#define BLEZL_OPCODE	0x16
#define BNE_OPCODE		0x5
#define BNEL_OPCODE		0x15

/* Reg Imm */
#define REGIMM_OPCODE 	0x1
#define BGEZ_OPCODE		0x1
#define BGEZAL_OPCODE	0x11
#define BGEZALL_OPCODE	0x13
#define BGEZL_OPCODE	0x3
#define BLTZ_OPCODE		0
#define BLTZAL_OPCODE	0x10
#define BLTZALL_OPCODE	0x12
#define BLTZL_OPCODE	0x2

#define J_OPCODE		0x2
#define JAL_OPCODE		0x3

/* Special opcode */
#define SPECIAL_OPCODE	0
#define JALR_OPCODE		0x9
#define JR_OPCODE		0x8

/* Cop Branches (all the same) */
#define COP0_OPCODE		0x10
#define COP1_OPCODE		0x11
#define COP2_OPCODE		0x12
#define BCXF_OPCODE		0x100
#define BCXFL_OPCODE	0x102
#define BCXT_OPCODE		0x101
#define BCXTL_OPCODE	0x103

/* Generic step command , if skip then will try to skip over jals */
static void step_generic(int skip)
{
	u32 opcode;
	u32 epc;
	u32 targetpc;
	int branch = 0;
	int cond   = 0;
	int link   = 0;
	struct ee_regs *regs;

	/* Disable the hw breakpoints first */
	InitBPC();

	if(g_currLevel == 1)
	{
		epc = _level1SavedRegs.EPC;
		regs = &_level1SavedRegs;
	}
	else
	{
		epc = _level2SavedRegs.ErrorPC;
		regs = &_level2SavedRegs;
	}

	targetpc = epc + 4;

	opcode = _lw(epc);

	switch(opcode >> 26)
	{
		case BEQ_OPCODE:
		case BEQL_OPCODE:
		case BGTZ_OPCODE:
		case BGTZL_OPCODE:
		case BLEZ_OPCODE:
		case BLEZL_OPCODE: 
		case BNE_OPCODE:
		case BNEL_OPCODE:
						    {
							   short ofs;

							   ofs = (short) (opcode & 0xffff);
							   cond = 1;
							   branch = 1;
							   targetpc += ofs * 4;
						   }
						   break;
		case REGIMM_OPCODE: {
								switch((opcode >> 16) & 0x1f)
								{
									case BGEZ_OPCODE:
									case BGEZAL_OPCODE:
									case BGEZALL_OPCODE:	
									case BGEZL_OPCODE:
									case BLTZ_OPCODE:
									case BLTZAL_OPCODE:
									case BLTZALL_OPCODE:
									case BLTZL_OPCODE: {
														   short ofs;

														   ofs = (short) (opcode & 0xffff);
														   cond = 1;
														   branch = 1;
														   targetpc += ofs * 4;
													   }
													   break;
								}
						    }
							break;
		case JAL_OPCODE:	link = 1;
		case J_OPCODE: {
							 u32 ofs;
							 
							 ofs = opcode & 0x3ffffff;
							 targetpc = (ofs << 2) | (targetpc & 0xf0000000);
							 branch = 1;
							 cond = 0;
						 }
						 break;
		case SPECIAL_OPCODE:
						 {
							 switch(opcode & 0x3f)
							 {
								 case JALR_OPCODE: link = 1;
								 case JR_OPCODE:
												 {
													 u32 rs;

													 rs = (opcode >> 21) & 0x1f;
													 targetpc = regs->r[rs].r32[0];
													 branch = 1;
													 cond = 0;
												 }
												 break;
							 };
						 }
						 break;
		case COP0_OPCODE:
		case COP1_OPCODE:
		case COP2_OPCODE:
						 {
							 switch((opcode >> 16) & 0x3ff)
							 {
								 case BCXF_OPCODE:
								 case BCXFL_OPCODE:
								 case BCXT_OPCODE:
								 case BCXTL_OPCODE:
									 				{
														short ofs;

														ofs = (short) (opcode & 0xffff);
														cond = 1;
														branch = 1;
														targetpc += ofs * 4;
													}
													break;
							 };
						 }
						 break;
	};

	if(link && skip)
	{
		printf("STEP skip link branch\n");
		g_stepbp[1].addr = epc + 8;
		g_stepbp[1].oldinst = _lw(epc + 8);
		g_stepbp[1].active = 1;
		_sw(SW_BREAK_INST, epc + 8);
	}
	else if(branch)
	{
		printf("STEP branch targetpc %08x\n", targetpc);

		g_stepbp[0].addr = targetpc;
		g_stepbp[0].oldinst = _lw(targetpc);
		g_stepbp[0].active = 1;
		_sw(SW_BREAK_INST, targetpc);
			
		if((cond) && (targetpc != (epc + 8)))
		{
			printf("STEP conditional\n");
			g_stepbp[1].addr = epc + 8;
			g_stepbp[1].oldinst = _lw(epc + 8);
			g_stepbp[1].active = 1;
			_sw(SW_BREAK_INST, epc + 8);
		}

	}
	else
	{
		printf("STEP to %08x\n", targetpc);
		g_stepbp[0].addr = targetpc;
		g_stepbp[0].active = 1;
		g_stepbp[0].oldinst = _lw(targetpc);
		_sw(SW_BREAK_INST, targetpc);
	}

	flush_cache();
}

/* Step command */
static int step_cmd(void)
{
	step_generic(0);

	return CMD_EXITSHELL;
}

/* Skip command */
static int skip_cmd(void)
{
	step_generic(1);

	return CMD_EXITSHELL;
}

/* Dump the current thread list */
static int dt_cmd(void)
{
	ee_thread_t thread;
	s32 ret;
	int i;

	for(i = 0; i < 256; i++)
	{
		//ret = ReferThread(i, &thread);
		ret = _syscallTable->ReferThreadStatus(i, &thread);
		if(ret)
		{
			printf("Thread %d func %08x stack %08x gp %08x status %08x pri %d\n", 
					i, (u32) thread.func, (u32) thread.stack, (u32) thread.initial_priority, 
					thread.status, thread.current_priority);
				
		}
	}

	return CMD_OK;
}

static int ct_cmd(void)
{
	char *mem_addr;
	char *mem_stack;
	char *mem_priority;

	mem_addr = strtok(NULL, " \t");
	mem_stack = strtok(NULL, " \t");
	mem_priority = strtok(NULL, " \t");
	if((mem_addr) && (mem_stack) && (mem_priority))
	{
		u32 addr;
		u32 stack;
		u32 priority;

		addr = hextoi(mem_addr) & ~3;
		stack = (hextoi(mem_stack) & ~16) - 0x10000;
		priority = hextoi(mem_priority) & 127;
		if(validate_memaddr(addr, MEM_ATTRIB_EXEC) > 0)
		{
			if(validate_memaddr(stack, MEM_ATTRIB_READ | MEM_ATTRIB_WRITE) > 0x10000)
			{
				int thid;
				ee_thread_t thread;

				g_thread_setup("ROMVER", addr, stack, 0);
				/* Let's try and find the thread we just created */
				/* We assume there will only be one :P */
				for(thid = 1; thid < 128; thid++)
				{
					_syscallTable->ReferThreadStatus(thid, &thread);
					if(thread.func == (void*) addr)
					{
						break;
					}
				}

				if(thid < 128)
				{
					_syscallTable->iChangeThreadPriority(thid, priority);
				}
				else
				{
					printf("Create thread failed\n");
				}
			}
			else
			{
				printf("Invalid stack address %08x\n", stack);
			}
		}
		else
		{
			printf("Invalid start address %08x\n", addr);
		}
	}
	else
	{
		return CMD_ERROR;
	}

	return CMD_OK;
}

static int send_cmd(void)
{
	char *mem_addr;
	char *mem_size;
	char *mem_filename;

	mem_addr = strtok(NULL, " \t");
	mem_size = strtok(NULL, " \t");
	mem_filename = strtok(NULL, " \t");

	if((mem_addr) && (mem_size) && (mem_filename))
	{
		u32 addr;
		u32 size;
		u32 bytes_left;

		addr = hextoi(mem_addr);
		size = hextoi(mem_size);
		bytes_left = validate_memaddr(addr, MEM_ATTRIB_READ | MEM_ATTRIB_BYTE);
		if(bytes_left < size)
		{
			size = bytes_left;
		}

		printf("Sending %08x size %d\n", addr, size);
		ymodem_send_data((u8 *)addr, size, mem_filename);
	}
	else
	{
		return CMD_ERROR;
	}

	return CMD_OK;
}

static int recv_cmd(void)
{
	char *mem_addr;
	char *mem_size;

	mem_addr = strtok(NULL, " \t");
	mem_size = strtok(NULL, " \t");

	if(mem_addr)
	{
		u32 addr;
		int size;
		int valid_size;

		addr = hextoi(mem_addr);
		valid_size = validate_memaddr(addr, MEM_ATTRIB_WRITE | MEM_ATTRIB_BYTE);

		if(mem_size)
		{
			size = hextoi(mem_size);
			/* Adjust to be at maximum the valid size */
			size = size > valid_size ? valid_size : size;
		}
		else
		{
			size = valid_size;
		}

		if(size > 0)
		{
			printf("Receving data to address %08x, max size %d\n", addr, size);
			ymodem_recv_data((u8*) addr, size);
			flush_cache();
		}
		else
		{
			printf("Invalid memory region\n");
		}
	}
	else
	{
		return CMD_ERROR;
	}

	return CMD_OK;
}

/* Display the command line history */
static int ch_cmd(void)
{
	int i;

	for(i = 0; i < CLI_HISTSIZE; i++)
	{
		printf("%d '%s'\n", i, g_lastcli[i]);
	}

	return CMD_OK;
}

/* Install our own interrupt exception handlers */
static int ih_cmd(void)
{
	int i;

	for(i = 1; i < 4; i++)
	{
		_syscallTable->SetVTLBRefillHandler(i, level1ExceptionHandler);
	}

	for(i = 4; i < 14; i++)
	{
		/* Skip syscall */
		if(i != 8)
		{
			_syscallTable->SetVCommonHandler(i, level1ExceptionHandler);
		}
	}

	return CMD_OK;
}

static int sp_cmd(void)
{
	char *mem_addr;

	mem_addr = strtok(NULL, " \t");
	if(mem_addr)
	{
		u32 addr;

		addr = hextoi(mem_addr) & ~3;
		if(validate_memaddr(addr, MEM_ATTRIB_EXEC | MEM_ATTRIB_WRITE) >= 16)
		{
			_sw(0x24030075, addr);
			_sw(0xc, addr+4);
			_sw(0x3e00008, addr+8);
			_sw(0, addr+12);
		}
	}
	else
	{
		return CMD_ERROR;
	}

	return CMD_OK;
}

/* Hook system call command */
static int hs_cmd(void)
{
	char *mem_break;

	mem_break = strtok(NULL, " \t");
	if((mem_break) && (strcasecmp(mem_break, "break") == 0))
	{
		printf("Hooking syscall with break\n");
		g_breakOnSyscall = 1;
	}
	else
	{
		printf("Hooking syscall without break\n");
		g_breakOnSyscall = 0;
	}

	_syscallTable->SetVCommonHandler(8, level1ExceptionHandler);

	return CMD_OK;
}

/* Unhook system call handler */
static int us_cmd(void)
{
	g_breakOnSyscall = 0;
	_syscallTable->SetVCommonHandler(8, (void*) 0x80000280);
	printf("Unhooked syscall\n");

	return CMD_OK;
}

static int baud_cmd(void)
{
	char *mem_baud;

	mem_baud = strtok(NULL, " \t");
	if(mem_baud)
	{
		u32 baud;

		baud = dectoi(mem_baud);
		switch(baud)
		{
			case 9600:
			case 19200:
			case 38400:
			case 57600:
			case 115200: break;
			default: printf("Invalid baud rate %d\n", baud);
					 return CMD_ERROR;
		};

		printf("Setting baud rate %d\n", baud);
		sio_init(baud, 0, 0, 0, 0);
		/* Exit immediately */
		return CMD_EXITSHELL;
	}
	else
	{
		return CMD_ERROR;
	}

	return CMD_OK;
}

////////////////////////////////////////////////////////////////////////////////
// Shell Main functions
////////////////////////////////////////////////////////////////////////////////

#define SHELL_PROMPT	"ps2sh> "

/* Initialise the SIO shell, must be called in kernel mode */
void siosh_init(void)
{
    u32 *pAddr;
	int i;
	u32 inst;

	sio_init(115200, 0, 0, 0, 0);

	// Get the syscall table pointer
    pAddr = (u32 *) 0x800002f0;
    _syscallTable = (struct syscall_table *) ((pAddr[0] << 16) | (pAddr[2] & 0xFFFF));
	_syscallTableOld = (struct syscall_table *) (0x80070000 - sizeof(struct syscall_table));

	inst = _lw(0x800010b0);
	g_thread_setup = (void *) (0x80000000 | ((inst & 0x3FFFFFF) << 2));

	install_debug_handler();
	install_level1_handler();
	init_swbps();
	find_sioprintf();
	_syscallTable->print = (void *) g_sio_printf;
	install_sifhook();

	g_cli_pos = 0;
	g_cli_size = 0;
	g_lastcli_pos = 0;
	g_currcli_pos = 0;
	g_insert_mode = 0;
	g_breakOnSyscall = 0;
	memset(g_cli, 0, CLI_MAX);

	for(i = 0; i < CLI_HISTSIZE; i++)
	{
		g_lastcli[i][0] = 0;
	}

	_sw(SIO_IER_ERDAI | SIO_IER_ELSI, SIO_IER);
	InitBPC();
	printf("SIOSHELL Installed\n");
}

/* Clear the SIO RX fifo */
static void clear_RX_FIFO(void)
{
   while(sio_getc() != -1);
}

/* Read a character with a timeout */
static char read_char(void)
{
	int i;
	char ch = -1;

	for(i = 0; i < 100000; i++)
	{
		ch = sio_getc();
		if(ch != -1)
		{
			break;
		}
	}

	return ch;
}

/* Process command line */
static int process_cli()
{
	int ret = CMD_OK;
	char *cmd;

    sio_putc(13);
    sio_putc(10);
	g_cli[g_cli_pos] = 0;
	memcpy(&g_lastcli[g_lastcli_pos][0], g_cli, CLI_MAX);
	g_lastcli_pos = (g_lastcli_pos + 1) % CLI_HISTSIZE;
	g_currcli_pos = g_lastcli_pos;

	cmd = strtok(g_cli, " ");

	if(cmd != NULL)
	{
		struct sh_command *found_cmd;

		found_cmd = find_command(cmd);
		if(found_cmd)
		{
			ret = found_cmd->func();
			if(ret == CMD_ERROR)
			{
				printf("Usage: %s\n", found_cmd->help);
			}
		}
		else
		{
			printf("Unknown command %s\n", cmd);
		}
	}

	g_cli_pos = 0;
	if(ret != CMD_EXITSHELL)
	{
		printf(SHELL_PROMPT);
	}

	return ret;
}

/* Handle an escape sequence */
static void cli_handle_escape(void)
{
	char ch;

	ch = read_char();

	if(ch != -1)
	{
		/* Arrow keys UDRL/ABCD */
		if(ch == '[')
		{
			ch = read_char();
			switch(ch)
			{
				case 'A' : {
							   int pos;

							   pos = g_currcli_pos - 1;
							   if(pos < 0)
							   {
								   pos += CLI_HISTSIZE;
							   }

							   if(g_lastcli[pos][0] != 0)
							   {
								   char *src, *dst;

								   src = g_lastcli[pos];
								   dst = g_cli;
								   g_currcli_pos = pos;
								   g_cli_pos = 0;
								   g_cli_size = 0;
								   while(*src)
								   {
									   *dst++ = *src++;
									   g_cli_pos++;
									   g_cli_size++;
								   }
								   *dst = 0;

								   printf("\n%s%s", SHELL_PROMPT, g_cli);
							   } 
						   } 
						   break;

				case 'B' : {
							   int pos;

							   pos = g_currcli_pos + 1;
							   pos %= CLI_HISTSIZE;

							   if(g_lastcli[pos][0] != 0)
							   {
								   char *src, *dst;

								   src = g_lastcli[pos];
								   dst = g_cli;
								   g_currcli_pos = pos;
								   g_cli_pos = 0;
								   g_cli_size = 0;
								   while(*src)
								   {
									   *dst++ = *src++;
									   g_cli_pos++;
									   g_cli_size++;
								   }
								   *dst = 0;

								   printf("\n%s%s", SHELL_PROMPT, g_cli);
							   } 
						   } 
						   break;



				default: 
							printf("Unknown character %d\n", ch);
						   break;
			};
		}
		else
		{
			printf("Unknown character %d\n", ch);
		}
	}
}

/* Main shell function */
static void shell()
{		
	int exit_shell = 0;

	printf(SHELL_PROMPT);
	g_cli_pos = 0;
	g_cli_size = 0;
	memset(g_cli, 0, CLI_MAX);

	while(!exit_shell) {
		char ch;

		ch = sio_getc();
		switch(ch)
		{
			case -1 : break; // No char
					  /* ^D */
			case 4  : printf("\nExiting Shell\n");
					  exit_shell = 1;
					  break;
			case 0xB : /* ^K */
					  printf("\n");
					  exit_shell = skip_cmd();
					  break;
			case 0x13 : /* ^S */
					  printf("\n");
					  exit_shell = step_cmd();
					  break;
			case 8  : if(g_cli_pos > 0)
					  {
						  g_cli_pos--;
						  sio_putc(8);
						  sio_putc(' ');
						  sio_putc(8);
					  }
					  break;
			case 9  : break; // Ignore tab
			case 13 :		 // Enter key 
			case 10 : if(process_cli() == CMD_EXITSHELL) 
					  {
						  exit_shell = 1;
					  }
					  break;
			case 27 : /* Escape character */
					  cli_handle_escape();
					  break;
			default : if((g_cli_pos < (CLI_MAX - 1)) && (ch >= 32))
					  {
						  g_cli[g_cli_pos++] = ch;
						  sio_putc(ch);
					  }
					  break;
		}
	}
}

static char codeTxt[13][24] = 
{
    "Interrupt", "TLB modification", "TLB load/inst fetch", "TLB store",
    "Address load/inst fetch", "Address store", "Bus error (instr)", 
    "Bus error (data)", "Syscall", "Breakpoint", "Reserved instruction", 
    "Coprocessor unusable", "Arithmetic overflow"
};

static void print_exception(void)
{
	int cause = (_level1SavedRegs.Cause >> 2) & 0xf;

	printf("Exception %s\n", codeTxt[cause]);
}

#define REG_A0 (g_currSavedRegs->r[4].r32[0])
#define REG_A1 (g_currSavedRegs->r[5].r32[0])
#define REG_A2 (g_currSavedRegs->r[6].r32[0])
#define REG_A3 (g_currSavedRegs->r[7].r32[0])

/* If break on syscall set and we hit a syscall we want then return */
int handle_syscall(int syscall)
{
	int break_syscall = 1;
	/* Only handle ones we are interested in */
	switch(syscall)
	{
		case 4:		printf("Exit\n");
					break_syscall = 0;
					break;
		case 6: 	printf("LoadExecPS2 elf %s, argcount %d, argp %08x\n", REG_A0, REG_A1, REG_A2);
					break;
		case 7:		printf("ExecPS2 entry %08x gp %08x argcount %d argp %08x\n", REG_A0, REG_A1, REG_A2, REG_A3);
					break;
		case 0xA :  printf("AddSbusIntcHandler cause %d, handler %08x\n", REG_A0, REG_A1);
					break;
		case 0xc :  printf("Interrupt2Iop cause %d\n", REG_A0);
					break;
		case 0x20 : {
						ee_thread_t *thread;
						thread = (ee_thread_t *) REG_A0;
						printf("CreateThread func %08x, sp %08x, gp %08x, pri %d\n", 
								thread->func, thread->stack, thread->gp_reg, thread->initial_priority);
						break;
					}
		case 0x22 : {
						printf("StartThread thid %d, args %08x\n", REG_A0, REG_A1);
					}
					break;
		default:	break_syscall = 0;
					break;
	};

	return break_syscall;
}

/* Main function in a level1 exception */
void sio_shell_level1()
{
	u32 ier = _lw(SIO_IER);
	_sw(ier & ~(SIO_IER_ERDAI | SIO_IER_ELSI), SIO_IER);

	g_currLevel = 1;
	g_currSavedRegs = &_level1SavedRegs;

	switch((_level1SavedRegs.Cause >> 2) & 0xf)
	{
		// Handle syscall
		case 8 : {
					 int ret;
					 ret = handle_syscall(_level1SavedRegs.r[3].r32[0]);
					 /* If we chose to break, then err break :P */
					 if(ret && g_breakOnSyscall)
					 {
						 shell();
					 }
					clear_RX_FIFO();
					/* Clear interrupt status regs */
				    _sw(SIO_IER_ERDAI | SIO_IER_ELSI, SIO_ISR);
				    ier = _lw(SIO_IER);
				    _sw(SIO_IER_ERDAI | SIO_IER_ELSI | ier, SIO_IER);
				    restartSyscallException();
				 }
				 break;
		// At break if it one of ours reset the original instruction, flush and jump back
		case 9 : {
					 int bp;
					 printf("Software Breakpoint at %08x\n", _level1SavedRegs.EPC);
					 bp = find_swbp(_level1SavedRegs.EPC);
					 if(bp >= 0)
					 {
						 clear_swbp(bp);
						 disasm_verbose(g_currSavedRegs, (u32 *) g_currSavedRegs->EPC);
					 }
					 else
					 {
						 printf("Software breakpoint, not ours\n");
						 // Not our breakpoint, just skip it
						 _level1SavedRegs.EPC += 4;
					 }
				 }
				 break;
		default: print_exception();
				 break;
	}

	shell();
	clear_RX_FIFO();
   /* Clear interrupt status regs */
   _sw(SIO_IER_ERDAI | SIO_IER_ELSI, SIO_ISR);
   ier = _lw(SIO_IER);
   _sw(SIO_IER_ERDAI | SIO_IER_ELSI | ier, SIO_IER);
}

/* Main function in level2 exception */
void sio_shell_level2()
{
	u32 ier = _lw(SIO_IER);
	_sw(ier & ~(SIO_IER_ERDAI | SIO_IER_ELSI), SIO_IER);

	g_currLevel = 2;
	g_currSavedRegs = &_level2SavedRegs;

	// Let's check which event occurred
	if(_level2SavedRegs.Cause & SIO_CAUSE_BIT)
	{
		u32 stat = _lw(SIO_ISR);

		/* Clear all line status bits */
		if(stat & SIO_IER_ELSI)
		{
			int line_stat = _lw(SIO_LSR);

			if(line_stat & SIO_LSR_FE)
			{
				printf("SIO: Framing error\n");
			}

			if(line_stat & SIO_LSR_PE)
			{
				printf("SIO: Parity error\n");
			}

			if(line_stat & SIO_LSR_OE)
			{
				printf("SIO: Overun error\n");
			}

			_sw(SIO_LSR_FE | SIO_LSR_PE | SIO_LSR_OE, SIO_LSR);
		}

		/* Do something if we have data waiting */
		if(stat & SIO_IER_ERDAI)
		{
			sio_getc();
			shell();
			clear_RX_FIFO();
		}

	}
	else
	{
		u32 bpcmask = 0xFFFFFFFF;
		u32 bpc;

		printf("HW Breakpoint at 0x%08x\n", _level2SavedRegs.ErrorPC);
		printf("BP Cause: ");
		bpc = GetBPC();

		if(bpc & BPC_IAB)
		{
			bpcmask ^= BPC_IAB;
			printf("Instruction ");
		}

		if(bpc & BPC_DRB)
		{
			bpcmask ^= BPC_DRB;
			printf("Data Read ");
		}

		if(bpc & BPC_DWB)
		{
			bpcmask ^= BPC_DWB;
			printf("Data Write ");
		}
		printf("\n");
		SetBPC(bpc & bpcmask);
		disasm_verbose(g_currSavedRegs, (u32 *) g_currSavedRegs->ErrorPC);

		sio_getc();
		shell();
		clear_RX_FIFO();
	}

   /* Clear interrupt status regs */
   _sw(SIO_IER_ERDAI | SIO_IER_ELSI, SIO_ISR);
   ier = _lw(SIO_IER);
   _sw(SIO_IER_ERDAI | SIO_IER_ELSI | ier, SIO_IER);
}
