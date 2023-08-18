#ifndef _COMPAT_H
#define _COMPAT_H

#define t_sema t_ee_sema
#define thread_attr t_ee_thread

#define k_SleepThread SleepThread
#define k_iWakeupThread iWakeupThread
#define k_WakeupThread WakeupThread
#define k_EnableDmac EnableDmac
#define k_DisableIntc DisableIntc
#define k_DisableDmac DisableDmac
#define k_FlushCache FlushCache
#define k_iFlushCache iFlushCache
#define k_AddDmacHandler AddDmacHandler
#define k_RemoveDmacHandler RemoveDmacHandler
#define k_iSignalSema iSignalSema
#define k_SignalSema SignalSema
#define k_CreateSema CreateSema
#define k_WaitSema WaitSema
#define k_DeleteSema DeleteSema
#define k_EndOfHeap EndOfHeap
#define k_CreateThread CreateThread
#define k_StartThread StartThread
#define k_GetThreadID GetThreadID
#define k_ChangeThreadPriority ChangeThreadPriority
#define k_TerminateThread TerminateThread
#define k_DeleteThread DeleteThread
#define k_GsPutIMR GsPutIMR
#define k_SetGsCrt SetGsCrt
#define k_sceSifStopDma SifStopDma
#define k_LoadExecPS2 LoadExecPS2
#define k_ExecPS2 ExecPS2

#define sif_cmd_handler_t SifCmdHandler_t
#define t_sif_cmd_header t_SifCmdHeader
#define t_sif_dma_transfer t_SifDmaTransfer
#define t_sif_handler t_SifCmdHandlerData
#define k_SifWritebackDcache SifWritebackDcache
#define SifWritebackDcache SifWriteBackDCache
#define k_sceSifSetDma SifSetDma
#define k_isceSifSetDma iSifSetDma
#define k_sceSifDmaStat SifDmaStat
#define k_sceSifSetDChain SifSetDChain
#define k_isceSifSetDChain iSifSetDChain
#define k_sceSifGetReg SifGetReg
#define k_sceSifSetReg SifSetReg
#define sif_cmd_send SifSendCmd
#define SifCmdSend SifSendCmd
#define sif_cmd_isend iSifSendCmd
#define SifCmdISend iSifSendCmd
#define sif_cmd_addhandler SifAddCmdHandler
#define SifCmdAddhandler SifAddCmdHandler
#define sif_cmd_init SifInitCmd
#define SifCmdInit SifInitCmd
#define sif_cmd_exit SifExitCmd
#define SifCmdExit SifExitCmd
#define sif_sreg_get SifGetSreg
#define SifSregGet SifGetSreg

#define sif_rpc_func_t SifRpcFunc_t
#define sif_rpc_end_func_t SifRpcEndFunc_t
#define t_sif_rpc_pkt_header t_SifRpcPktHeader
#define t_sif_rpc_rend t_SifRpcRendPkt
#define t_sif_rpc_other_data t_SifRpcOtherDataPkt
#define t_sif_rpc_bind t_SifRpcBindPkt
#define t_sif_rpc_call t_SifRpcCallPkt
#define t_rpc_server_data t_SifRpcServerData
#define t_rpc_header t_SifRpcHeader
#define t_rpc_client_data t_SifRpcClientData
#define t_rpc_receive_data t_SifRpcReceiveData
#define t_rpc_data_queue t_SifRpcDataQueue
#define exec_data t_ExecData
#define sif_rpc_init SifInitRpc
#define sif_rpc_exit SifExitRpc
#define sif_set_rpc_queue SifSetRpcQueue
#define sif_register_rpc SifRegisterRpc
#define sif_get_next_request SifGetNextRequest
#define rpc_get_fpacket2 RpcGetFpacket2
#define sif_exec_request SifExecRequest
#define sif_rpc_loop SifRpcLoop
#define sif_bind_rpc SifBindRpc
#define sif_call_rpc SifCallRpc
#define sif_load_module SifLoadModule
#define sif_load_module_encrypted SifLoadModuleEncrypted
#define sif_load_elf_part SifLoadElfPart
#define sif_load_elf SifLoadElf
#define sif_reset_iop SifResetIop

#define fio_init fioInit
#define fio_exit fioExit
#define fio_open fioOpen
#define fio_close fioClose
#define fio_read fioRead
#define fio_write fioWrite
#define fio_lseek fioLseek
#define fio_mkdir fioMkdir
#define fio_putc fioPutc
#define fio_getc fioGetc

#define mcicon mcIcon
#define mctable mcTable
#define mclib_init mcInit
#define mc_getdir mcGetDir
#define mc_getinfo mcGetInfo
#define mc_delete mcDelete

#endif
