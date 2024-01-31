typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#define NULL (void*)0

typedef u32 uint32_t;
typedef u32 size_t;
typedef u32 FILE;


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

int (*sceDopen)(const char *dirname) = (int (*)(const char *dirname))0x542388;
int (*sceDclose)(int fd) = (int (*)(int fd))0x542450;
int (*sceDread)(int fd, struct sce_dirent *buf) = (int (*)(int fd, struct sce_dirent *buf))0x5425B8;
int (*sceRemove)(const char *filename) = (int (*)(const char *filename))0x542B18;
int (*sceMkdir)(const char *dirname, int flag) = (int (*)(const char *dirname, int flag))0x541DE0;
int (*sceRmdir)(const char *dirname) = (int (*)(const char *dirname))0x541F98;

void* (*memset)(void *, int, size_t) = (void* (*)(void *, int, size_t))0x54E568;
char* (*strncpy)(char *, const char *, size_t) = (char* (*)(char *, const char *, size_t))0x551D80;

size_t (*strlen)(const char *) = (size_t (*)(const char *))0x5517B8;
int (*strcmp)(const char *, const char *) = (int (*)(const char *, const char *))0x551558;

void (*load_code_overlay)(const char*, u32) = (void (*)(const char*, u32))0x3C6990;
void (*unload_code_overlay)(void) = (void (*)(void))0x3C6AA0;
void (*load_dynamic_plugin)(const char*, int) = (void (*)(const char*, int))0x890368;

int (*RwEngineInit)(uint32_t, char, uint32_t) = (int (*)(uint32_t, char, uint32_t))0x33E8A8;

uint32_t erl_record_root = 0;
uint32_t global_symbols = 0;
uint32_t loosy_relocs = 0;
uint32_t dependancy_root = 0;
uint32_t symbol_recycle = 0;

#define EndsWith(str, suffix) strcmp(str + strlen(str) - strlen(suffix), suffix) == 0

int main(unsigned int a1, char a2, unsigned int a3){
    struct sce_dirent dirbuf;
    char* section_names = NULL;
    const char* plugin_dir = "cdrom0:\\";
    char fname[32];

    RwEngineInit(a1, a2, a3);

    unload_code_overlay();

    load_code_overlay("relocator.nm", 0x88d880);

    int dirfd = sceDopen(plugin_dir);

    sceDread(dirfd, &dirbuf);
    sceDread(dirfd, &dirbuf);

    while (sceDread(dirfd, &dirbuf) > 0){
        if(EndsWith(dirbuf.d_name, ".ERL;1")) {
            memset(fname, 0, 32);
            strncpy(fname, dirbuf.d_name, strlen(dirbuf.d_name)-2);
            load_dynamic_plugin(fname, 0);
        }
    }

    sceDclose(dirfd);

    unload_code_overlay();

    load_code_overlay("init.nm", 0x88d880);

    return 0;

}
