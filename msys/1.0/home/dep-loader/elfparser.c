typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#define NULL (void*)0

typedef u32 uint32_t;
typedef u32 size_t;
typedef u32 FILE;

void (*load_code_overlay)(const char*, u32) = (void (*)(const char*, u32))0x3C6990;
void (*unload_code_overlay)(void) = (void (*)(void))0x3C6AA0;
void (*load_dynamic_plugin)(const char*, int) = (void (*)(const char*, int))0x88FB68;

int (*strncmp)(const char *str1, const char *str2, size_t n) = (int (*)(const char *str1, const char *str2, size_t n))0x551BC8;
uint32_t (*strlen)(const char *)  = (uint32_t (*)(const char *))0x005517B8;
void* (*memset)(void *, int, uint32_t)  = (void* (*)(void *, int, uint32_t))0x0054E568;
char* (*strncpy)(char *, const char *, uint32_t) = (char* (*)(char *, const char *, uint32_t))0x00551D80;

struct sce_stat {
        unsigned int    st_mode;       
                                    
        unsigned int    st_attr;       
        unsigned int    st_size;       
        unsigned char   st_ctime[8];   
        unsigned char   st_atime[8];   
        unsigned char   st_mtime[8];   
        unsigned int    st_hisize;     
        unsigned int    st_private[6]; 
};

struct sce_dirent {
        struct sce_stat d_stat; 
        char d_name[256];       
        void    *d_private;     
};

int (*sceDopen)(const char *dirname) = (int (*)(const char *))0x00542388;
int (*sceDclose)(int fd) = (int (*)(int))0x00542450;
int (*sceDread)(int fd, struct sce_dirent *buf) = (int (*)(int, struct sce_dirent *))0x005425B8;

uint32_t erl_record_root = 0;
uint32_t global_symbols = 0;
uint32_t loosy_relocs = 0;
uint32_t dependancy_root = 0;
uint32_t symbol_recycle = 0;

void (*load_plugin)(const char*, int) = (void (*)(const char*, int))0;
uint32_t unload_erl = 0;
uint32_t find_erl = 0;
uint32_t erl_find_symbol = 0;
uint32_t erl_find_local_symbol = 0;

int EndsWith(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

typedef struct
{
	u8 ident[16];  // struct definition for ELF object header
	u16 type;
	u16 machine;
	u32 version;
	u32 entry;
	u32 phoff;
	u32 shoff;
	u32 flags;
	u16 ehsize;
	u16 phentsize;
	u16 phnum;
	u16 shentsize;
	u16 shnum;
	u16 shstrndx;
} elf_header_t;

typedef struct
{
     uint32_t   sh_name;
     uint32_t   sh_type;
     uint32_t   sh_flags;
     uint32_t sh_addr;
     uint32_t   sh_offset;
     uint32_t   sh_size;
     uint32_t   sh_link;
     uint32_t   sh_info;
     uint32_t   sh_addralign;
     uint32_t   sh_entsize;
} section_header_t;

int load_default_plugins(){
    struct sce_dirent dirbuf;
    char* section_names = NULL;
    const char* plugin_dir = "cdrom0:\\";
    char fname[32];

    unload_code_overlay();

    load_code_overlay("relocator.nm", 0x88d880);

    int dirfd = sceDopen(plugin_dir);

    sceDread(dirfd, &dirbuf);
    sceDread(dirfd, &dirbuf);

    load_dynamic_plugin("BASE.ERL", 0);

    while (sceDread(dirfd, &dirbuf) > 0){
        if(EndsWith(dirbuf.d_name, ".ERL;1")) {
            memset(fname, 0, 32);
            strncpy(fname, dirbuf.d_name, strlen(dirbuf.d_name)-2);
            if (strncmp(fname, "BASE.ERL", strlen(fname)) != 0) {
                load_plugin(fname, 0);
            }
        }
    }

    sceDclose(dirfd);

    unload_code_overlay();

    load_code_overlay("init.nm", 0x88d880);

    return 0;

}
