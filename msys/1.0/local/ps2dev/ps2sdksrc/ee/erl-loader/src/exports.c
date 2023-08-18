void AddDmacHandler();
void AddIntcHandler();
void CreateSema();
void DIntr();
void DeleteSema();
void DisableDmac();
void DisableIntc();
void EIntr();
void EnableDmac();
void EnableIntc();
void EndOfHeap();
void FlushCache();
void RemoveDmacHandler();
void RemoveIntcHandler();
void SifAddCmdHandler();
void SifBindRpc();
void SifCallRpc();
void SifExitCmd();
void SifExitRpc();
void SifGetReg();
void SifGetSreg();
void SifInitCmd();
void SifInitRpc();
void SifSendCmd();
void SifSetDChain();
void SifSetDma();
void SifSetReg();
void SifWriteBackDCache();
void SignalSema();
void WaitSema();
void _DisableDmac();
void _DisableIntc();
void _EnableDmac();
void _EnableIntc();
void _GLOBAL__F___udivdi3();
void _GLOBAL__F___umoddi3();
void _SifCmdIntHandler();
void __alloc_heap_base();
void __alloc_heap_head();
void __alloc_heap_tail();
void __clz_tab();
void __direct_pwd();
void __fpcmp_parts_d();
void __iob();
void __pack_d();
void __ps2_normalize_path();
void __sout();
void __stdio_get_fd_type();
void __stdio_initialised();
void __stdio_tmpnam();
void __stdlib_env();
void __stdlib_exit_func();
void __stdlib_exit_index();
void __stdlib_mb_shift();
void __stdlib_rand_seed();
void __thenan_df();
void __udivdi3();
void __umoddi3();
void __unpack_d();
void _edata();
void _end();
void _end_bss();
void _exit();
void _fbss();
void _fcloseall();
void _fdata();
void _fini();
void _fio_block_mode();
void _fio_cd();
void _fio_completion_sema();
void _fio_init();
void _fio_intr();
void _fio_intr_data();
void _fio_read_intr();
void _fio_recv_data();
void _ftext();
void _gp();
void _heap_mem_fit();
void _heap_size();
void _init();
void _init_erl_prefix();
void _init_load_erl();
void _init_load_erl_from_file();
void _init_load_erl_from_file_to_addr();
void _iop_reboot_count();
void _ps2sdk_alloc_deinit();
void _ps2sdk_alloc_init();
void _ps2sdk_alloc_lock();
void _ps2sdk_alloc_unlock();
void _ps2sdk_args_parse();
void _ps2sdk_close();
void _ps2sdk_libc_deinit();
void _ps2sdk_libc_init();
void _ps2sdk_lseek();
void _ps2sdk_open();
void _ps2sdk_read();
void _ps2sdk_remove();
void _ps2sdk_rename();
void _ps2sdk_stdio_deinit();
void _ps2sdk_stdio_init();
void _ps2sdk_stdlib_deinit();
void _ps2sdk_stdlib_init();
void _ps2sdk_time_deinit();
void _ps2sdk_time_init();
void _ps2sdk_write();
void _rpc_get_fpacket();
void _rpc_get_packet();
void _sif_cmd_data();
void _sif_rpc_data();
void _stack();
void _stack_size();
void _start();
void chdir();
void checksum();
void clock();
void dpadd();
void dpcmp();
void dpmul();
void dpsub();
void dptoli();
void erl_add_global_symbol();
void erl_find_local_symbol();
void erl_find_symbol();
void erl_flush_symbols();
void erl_resolve();
void errno();
void fclose();
void find_erl();
void fioClose();
void fioInit();
void fioLseek();
void fioOpen();
void fioRead();
void fioRemove();
void fioRemove_Helper();
void fioRename();
void fioRmdir();
void fioWrite();
void fopen();
void fread();
void free();
void fseek();
void hadd();
void hcreate();
void hdel();
void hdestroy();
void hfind();
void hfirst();
void hnbucket();
void iSifSendCmd();
void iSifSetDChain();
void iSifSetDma();
void iSignalSema();
void iWakeupThread();
void isdigit();
void litodp();
void load_erl_from_file();
void load_erl_from_file_to_addr();
void load_erl_from_mem();
void load_erl_from_mem_to_addr();
void lookup();
void main();
void malloc();
void memcmp();
void memcpy();
void memset();
void printf();
void ps2_sbrk();
void r_unload_dependancies();
void refree();
void remalloc();
void remkroot();
void renewx();
void strcat();
void strchr();
void strcmp();
void strcpy();
void strdup();
void strlen();
void strncmp();
void strncpy();
void time();
void unload_erl();
void vprintf();
void vsnprintf();
struct export_list_t {
    char * name;
    void * pointer;
} export_list[] = {
    { "AddDmacHandler", AddDmacHandler }, 
    { "AddIntcHandler", AddIntcHandler }, 
    { "CreateSema", CreateSema }, 
    { "DIntr", DIntr }, 
    { "DeleteSema", DeleteSema }, 
    { "DisableDmac", DisableDmac }, 
    { "DisableIntc", DisableIntc }, 
    { "EIntr", EIntr }, 
    { "EnableDmac", EnableDmac }, 
    { "EnableIntc", EnableIntc }, 
    { "EndOfHeap", EndOfHeap }, 
    { "FlushCache", FlushCache }, 
    { "RemoveDmacHandler", RemoveDmacHandler }, 
    { "RemoveIntcHandler", RemoveIntcHandler }, 
    { "SifAddCmdHandler", SifAddCmdHandler }, 
    { "SifBindRpc", SifBindRpc }, 
    { "SifCallRpc", SifCallRpc }, 
    { "SifExitCmd", SifExitCmd }, 
    { "SifExitRpc", SifExitRpc }, 
    { "SifGetReg", SifGetReg }, 
    { "SifGetSreg", SifGetSreg }, 
    { "SifInitCmd", SifInitCmd }, 
    { "SifInitRpc", SifInitRpc }, 
    { "SifSendCmd", SifSendCmd }, 
    { "SifSetDChain", SifSetDChain }, 
    { "SifSetDma", SifSetDma }, 
    { "SifSetReg", SifSetReg }, 
    { "SifWriteBackDCache", SifWriteBackDCache }, 
    { "SignalSema", SignalSema }, 
    { "WaitSema", WaitSema }, 
    { "_DisableDmac", _DisableDmac }, 
    { "_DisableIntc", _DisableIntc }, 
    { "_EnableDmac", _EnableDmac }, 
    { "_EnableIntc", _EnableIntc }, 
    { "_GLOBAL__F___udivdi3", _GLOBAL__F___udivdi3 }, 
    { "_GLOBAL__F___umoddi3", _GLOBAL__F___umoddi3 }, 
    { "_SifCmdIntHandler", _SifCmdIntHandler }, 
    { "__alloc_heap_base", __alloc_heap_base }, 
    { "__alloc_heap_head", __alloc_heap_head }, 
    { "__alloc_heap_tail", __alloc_heap_tail }, 
    { "__clz_tab", __clz_tab }, 
    { "__direct_pwd", __direct_pwd }, 
    { "__fpcmp_parts_d", __fpcmp_parts_d }, 
    { "__iob", __iob }, 
    { "__pack_d", __pack_d }, 
    { "__ps2_normalize_path", __ps2_normalize_path }, 
    { "__sout", __sout }, 
    { "__stdio_get_fd_type", __stdio_get_fd_type }, 
    { "__stdio_initialised", __stdio_initialised }, 
    { "__stdio_tmpnam", __stdio_tmpnam }, 
    { "__stdlib_env", __stdlib_env }, 
    { "__stdlib_exit_func", __stdlib_exit_func }, 
    { "__stdlib_exit_index", __stdlib_exit_index }, 
    { "__stdlib_mb_shift", __stdlib_mb_shift }, 
    { "__stdlib_rand_seed", __stdlib_rand_seed }, 
    { "__thenan_df", __thenan_df }, 
    { "__udivdi3", __udivdi3 }, 
    { "__umoddi3", __umoddi3 }, 
    { "__unpack_d", __unpack_d }, 
    { "_edata", _edata }, 
    { "_end", _end }, 
    { "_end_bss", _end_bss }, 
    { "_exit", _exit }, 
    { "_fbss", _fbss }, 
    { "_fcloseall", _fcloseall }, 
    { "_fdata", _fdata }, 
    { "_fini", _fini }, 
    { "_fio_block_mode", _fio_block_mode }, 
    { "_fio_cd", _fio_cd }, 
    { "_fio_completion_sema", _fio_completion_sema }, 
    { "_fio_init", _fio_init }, 
    { "_fio_intr", _fio_intr }, 
    { "_fio_intr_data", _fio_intr_data }, 
    { "_fio_read_intr", _fio_read_intr }, 
    { "_fio_recv_data", _fio_recv_data }, 
    { "_ftext", _ftext }, 
    { "_gp", _gp }, 
    { "_heap_mem_fit", _heap_mem_fit }, 
    { "_heap_size", _heap_size }, 
    { "_init", _init }, 
    { "_init_erl_prefix", _init_erl_prefix }, 
    { "_init_load_erl", _init_load_erl }, 
    { "_init_load_erl_from_file", _init_load_erl_from_file }, 
    { "_init_load_erl_from_file_to_addr", _init_load_erl_from_file_to_addr }, 
    { "_iop_reboot_count", _iop_reboot_count }, 
    { "_ps2sdk_alloc_deinit", _ps2sdk_alloc_deinit }, 
    { "_ps2sdk_alloc_init", _ps2sdk_alloc_init }, 
    { "_ps2sdk_alloc_lock", _ps2sdk_alloc_lock }, 
    { "_ps2sdk_alloc_unlock", _ps2sdk_alloc_unlock }, 
    { "_ps2sdk_args_parse", _ps2sdk_args_parse }, 
    { "_ps2sdk_close", _ps2sdk_close }, 
    { "_ps2sdk_libc_deinit", _ps2sdk_libc_deinit }, 
    { "_ps2sdk_libc_init", _ps2sdk_libc_init }, 
    { "_ps2sdk_lseek", _ps2sdk_lseek }, 
    { "_ps2sdk_open", _ps2sdk_open }, 
    { "_ps2sdk_read", _ps2sdk_read }, 
    { "_ps2sdk_remove", _ps2sdk_remove }, 
    { "_ps2sdk_rename", _ps2sdk_rename }, 
    { "_ps2sdk_stdio_deinit", _ps2sdk_stdio_deinit }, 
    { "_ps2sdk_stdio_init", _ps2sdk_stdio_init }, 
    { "_ps2sdk_stdlib_deinit", _ps2sdk_stdlib_deinit }, 
    { "_ps2sdk_stdlib_init", _ps2sdk_stdlib_init }, 
    { "_ps2sdk_time_deinit", _ps2sdk_time_deinit }, 
    { "_ps2sdk_time_init", _ps2sdk_time_init }, 
    { "_ps2sdk_write", _ps2sdk_write }, 
    { "_rpc_get_fpacket", _rpc_get_fpacket }, 
    { "_rpc_get_packet", _rpc_get_packet }, 
    { "_sif_cmd_data", _sif_cmd_data }, 
    { "_sif_rpc_data", _sif_rpc_data }, 
    { "_stack", _stack }, 
    { "_stack_size", _stack_size }, 
    { "_start", _start }, 
    { "chdir", chdir }, 
    { "checksum", checksum }, 
    { "clock", clock }, 
    { "dpadd", dpadd }, 
    { "dpcmp", dpcmp }, 
    { "dpmul", dpmul }, 
    { "dpsub", dpsub }, 
    { "dptoli", dptoli }, 
    { "erl_add_global_symbol", erl_add_global_symbol }, 
    { "erl_find_local_symbol", erl_find_local_symbol }, 
    { "erl_find_symbol", erl_find_symbol }, 
    { "erl_flush_symbols", erl_flush_symbols }, 
    { "erl_resolve", erl_resolve }, 
    { "errno", errno }, 
    { "fclose", fclose }, 
    { "find_erl", find_erl }, 
    { "fioClose", fioClose }, 
    { "fioInit", fioInit }, 
    { "fioLseek", fioLseek }, 
    { "fioOpen", fioOpen }, 
    { "fioRead", fioRead }, 
    { "fioRemove", fioRemove }, 
    { "fioRemove_Helper", fioRemove_Helper }, 
    { "fioRename", fioRename }, 
    { "fioRmdir", fioRmdir }, 
    { "fioWrite", fioWrite }, 
    { "fopen", fopen }, 
    { "fread", fread }, 
    { "free", free }, 
    { "fseek", fseek }, 
    { "hadd", hadd }, 
    { "hcreate", hcreate }, 
    { "hdel", hdel }, 
    { "hdestroy", hdestroy }, 
    { "hfind", hfind }, 
    { "hfirst", hfirst }, 
    { "hnbucket", hnbucket }, 
    { "iSifSendCmd", iSifSendCmd }, 
    { "iSifSetDChain", iSifSetDChain }, 
    { "iSifSetDma", iSifSetDma }, 
    { "iSignalSema", iSignalSema }, 
    { "iWakeupThread", iWakeupThread }, 
    { "isdigit", isdigit }, 
    { "litodp", litodp }, 
    { "load_erl_from_file", load_erl_from_file }, 
    { "load_erl_from_file_to_addr", load_erl_from_file_to_addr }, 
    { "load_erl_from_mem", load_erl_from_mem }, 
    { "load_erl_from_mem_to_addr", load_erl_from_mem_to_addr }, 
    { "lookup", lookup }, 
    { "main", main }, 
    { "malloc", malloc }, 
    { "memcmp", memcmp }, 
    { "memcpy", memcpy }, 
    { "memset", memset }, 
    { "printf", printf }, 
    { "ps2_sbrk", ps2_sbrk }, 
    { "r_unload_dependancies", r_unload_dependancies }, 
    { "refree", refree }, 
    { "remalloc", remalloc }, 
    { "remkroot", remkroot }, 
    { "renewx", renewx }, 
    { "strcat", strcat }, 
    { "strchr", strchr }, 
    { "strcmp", strcmp }, 
    { "strcpy", strcpy }, 
    { "strdup", strdup }, 
    { "strlen", strlen }, 
    { "strncmp", strncmp }, 
    { "strncpy", strncpy }, 
    { "time", time }, 
    { "unload_erl", unload_erl }, 
    { "vprintf", vprintf }, 
    { "vsnprintf", vsnprintf }, 
    { 0, 0},
};
