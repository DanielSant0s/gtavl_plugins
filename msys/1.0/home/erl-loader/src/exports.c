void FlushCache();
void __muldi3();
void _edata();
void _fini();
void _gp();
void _init();
void _init_load_erl();
void chdir();
void checksum();
void close();
void erl_add_global_symbol();
void erl_find_local_symbol();
void erl_find_symbol();
void erl_flush_symbols();
void erl_resolve();
void find_erl();
void free();
void hadd();
void hcreate();
void hdel();
void hdestroy();
void hfind();
void hfirst();
void hnbucket();
void loadLine();
void load_plugin();
void lookup();
void lseek();
void malloc();
void memcmp();
void memcpy();
void memset();
void open();
void printf();
void r_unload_dependancies();
void read();
void refree();
void remalloc();
void remkroot();
void renewx();
void rw_close();
void rw_open();
void rw_read();
void rw_seek();
void sceDclose();
void sceDopen();
void sceDread();
void sceMkdir();
void sceRemove();
void sceRmdir();
void setDirectory();
void strcat();
void strcmp();
void strcpy();
void strdup();
void strlen();
void unload_erl();
struct export_list_t {
    char * name;
    void * pointer;
} export_list[] = {
    { "FlushCache", FlushCache }, 
    { "__muldi3", __muldi3 }, 
    { "_edata", _edata }, 
    { "_fini", _fini }, 
    { "_gp", _gp }, 
    { "_init", _init }, 
    { "_init_load_erl", _init_load_erl }, 
    { "chdir", chdir }, 
    { "checksum", checksum }, 
    { "close", close }, 
    { "erl_add_global_symbol", erl_add_global_symbol }, 
    { "erl_find_local_symbol", erl_find_local_symbol }, 
    { "erl_find_symbol", erl_find_symbol }, 
    { "erl_flush_symbols", erl_flush_symbols }, 
    { "erl_resolve", erl_resolve }, 
    { "find_erl", find_erl }, 
    { "free", free }, 
    { "hadd", hadd }, 
    { "hcreate", hcreate }, 
    { "hdel", hdel }, 
    { "hdestroy", hdestroy }, 
    { "hfind", hfind }, 
    { "hfirst", hfirst }, 
    { "hnbucket", hnbucket }, 
    { "loadLine", loadLine }, 
    { "load_plugin", load_plugin }, 
    { "lookup", lookup }, 
    { "lseek", lseek }, 
    { "malloc", malloc }, 
    { "memcmp", memcmp }, 
    { "memcpy", memcpy }, 
    { "memset", memset }, 
    { "open", open }, 
    { "printf", printf }, 
    { "r_unload_dependancies", r_unload_dependancies }, 
    { "read", read }, 
    { "refree", refree }, 
    { "remalloc", remalloc }, 
    { "remkroot", remkroot }, 
    { "renewx", renewx }, 
    { "rw_close", rw_close }, 
    { "rw_open", rw_open }, 
    { "rw_read", rw_read }, 
    { "rw_seek", rw_seek }, 
    { "sceDclose", sceDclose }, 
    { "sceDopen", sceDopen }, 
    { "sceDread", sceDread }, 
    { "sceMkdir", sceMkdir }, 
    { "sceRemove", sceRemove }, 
    { "sceRmdir", sceRmdir }, 
    { "setDirectory", setDirectory }, 
    { "strcat", strcat }, 
    { "strcmp", strcmp }, 
    { "strcpy", strcpy }, 
    { "strdup", strdup }, 
    { "strlen", strlen }, 
    { "unload_erl", unload_erl }, 
    { 0, 0},
};
