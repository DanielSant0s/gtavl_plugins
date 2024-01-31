#include <injector.h>
#include <rwcore.h>
#include <string.h>
#include <stdio.h>

void FlushCache(int);

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

int sceDopen(const char *dirname);
int sceDclose(int fd);
int sceDread(int fd, struct sce_dirent *buf);

int first_digit(uint32_t val) {
    return (val >> ((sizeof(int) * 8) - 4)) & 0xF;
}

int load_patch(const char* fname) {
    char line[256];
    RwFileFunction* fs = RwOsGetFileInterface();

    int should_patch;
    uint32_t address, value;
    char type[16];

    uint8_t* byte_addr;
    uint16_t* short_addr;
    uint32_t* word_addr;

    void *f = fs->rwfopen(fname, "r");

    while (fs->rwfgets(line, sizeof(line), f) != NULL) {
        if ((line[0] == '/' && line[1] == '/') || line[0] == '\n' || line[0] == '\r') {
            continue;
        }

        if (sscanf(line, "patch=%d,EE,%08x,%[^,],%08x", &should_patch, &address, type, &value) == 4) {
            if (!strcmp(type, "extended")) {
                switch (first_digit(address)) {
                    case 0:
                        {
                            byte_addr = (uint8_t*)address;
                            *byte_addr = (uint8_t)value;
                        }
                        break;
                    case 1:
                        {
                            short_addr = (uint16_t*)(address-0x10000000);
                            *short_addr = (uint16_t)value;
                        }
                        break;
                    case 2:
                        {
                            word_addr = (uint32_t*)(address-0x20000000);
                            *word_addr = (uint32_t)value;
                        }
                        break;
                }
            } else if (!strcmp(type, "byte")) {
                byte_addr = (uint8_t*)address;
                *byte_addr = (uint8_t)value;
            } else if (!strcmp(type, "short")) {
                short_addr = (uint16_t*)address;
                *short_addr = (uint16_t)value;
            } else if (!strcmp(type, "word")) {
                word_addr = (uint32_t*)address;
                *word_addr = (uint32_t)value;
            }
        }
    }

    fs->rwfclose(f);

    return 0;
}

int _start()
{
    struct sce_dirent dirbuf;
    char fname[32];

    int dirfd = sceDopen("cdrom0:\\");

    sceDread(dirfd, &dirbuf);
    sceDread(dirfd, &dirbuf);

    while (sceDread(dirfd, &dirbuf) > 0){
        if(EndsWith(dirbuf.d_name, ".PNACH;1")) {
            memset(fname, 0, 32);
            strncpy(fname, dirbuf.d_name, strlen(dirbuf.d_name)-2);
            load_patch(fname);
        }
    }

    sceDclose(dirfd);
    
    
    FlushCache(0);
    FlushCache(2);

    return 0;
}
