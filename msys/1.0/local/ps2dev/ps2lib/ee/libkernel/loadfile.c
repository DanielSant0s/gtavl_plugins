/*
  _____     ___ ____
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2001, Gustavo Scotti (gustavo@scotti.com)
                           (c) 2003 Marcus R. Brown (mrbrown@0xd6.org)
  ------------------------------------------------------------------------
  loadfile.c
			   IOP executable file loader API.
*/

#include <tamtypes.h>
#include <ps2lib_err.h>
#include <kernel.h>
#include <sifrpc.h>

#include <loadfile.h>

/* loadfile capabilities */
#define LF_C_BOUND	0x0001	/* bound and initialized */
#define LF_C_EXTRA	0x0002  /* extra calls past the BIOS version */

enum _lf_functions {
	LF_F_MOD_LOAD = 0,
	LF_F_ELF_LOAD,

	LF_F_SET_ADDR,
	LF_F_GET_ADDR,

	LF_F_MG_MOD_LOAD,
	LF_F_MG_ELF_LOAD,

	LF_F_MOD_BUF_LOAD,

	LF_F_VERSION = 255,
};

struct _lf_iop_val_arg {
	union {
		u32	iop_addr;
		int	result;
	} p;
	int	type;
	union {
		u8	b;
		u16	s;
		u32	l;
	} val;
} ALIGNED(16);

extern SifRpcClientData_t _lf_cd;
extern int _lf_caps;	/* Capabilities - do we support newer calls? */

int _SifLoadModule(const char *path, int arg_len, const char *args,
		int *modres, int fno);
int _SifLoadElfPart(const char *path, const char *secname, t_ExecData *data, int fno);
int _SifLoadModuleBuffer(void *ptr, int arg_len, const char *args, int *modres);

#ifdef F_SifLoadFileInit
SifRpcClientData_t _lf_cd;
int _lf_caps = 0;

int SifLoadFileInit()
{
	int arg; /* Implicitly aligned. */
	int res;

	if (_lf_caps)
		return 0;

	SifInitRpc(0);

	while ((res = SifBindRpc(&_lf_cd, 0x80000006, 0)) >= 0 && !_lf_cd.server)
		nopdelay();

	if (res < 0)
		return -E_SIF_RPC_BIND;

	_lf_caps |= LF_C_BOUND;

	/* Retrive the loadfile module version, if supported.  */
	if (SifCallRpc(&_lf_cd, LF_F_VERSION, 0, NULL, 0, &arg, 4, NULL, NULL) < 0)
		return -E_SIF_RPC_CALL;

	/* If the version isn't 0 then we at least can do some of the newer
	   calls.
	   TODO: We need to check the actual version.  */
	if (arg)
		_lf_caps |= LF_C_EXTRA;

	return 0;
}
#endif

#ifdef F_SifLoadFileExit
void SifLoadFileExit()
{
	_lf_caps = 0;
	memset(&_lf_cd, 0, sizeof _lf_cd);
}
#endif

#ifdef F__SifLoadModule
struct _lf_module_load_arg {
	union {
		int	arg_len;
		int	result;
	} p;
	int	modres;
	char	path[LF_PATH_MAX];
	char	args[LF_ARG_MAX];
} ALIGNED(16);

int _SifLoadModule(const char *path, int arg_len, const char *args, int *modres,
		int fno)
{
	struct _lf_module_load_arg arg;

	if (!_lf_caps && SifLoadFileInit() < 0)
		return -E_LIB_API_INIT;

	memset(&arg, 0, sizeof arg);

	strncpy(arg.path, path, LF_PATH_MAX);

	if (args && arg_len) {
		arg.p.arg_len = arg_len > LF_ARG_MAX ? LF_ARG_MAX : arg_len;
		memcpy(arg.args, args, arg.p.arg_len);
	}

	if (SifCallRpc(&_lf_cd, fno, 0, &arg, sizeof arg, &arg, 8, NULL, NULL) < 0)
		return -E_SIF_RPC_CALL;

	if (modres)
		*modres = arg.modres;

	return arg.p.result;
}
#endif

#ifdef F_SifLoadModule
int SifLoadModule(const char *path, int arg_len, const char *args)
{
	return _SifLoadModule(path, arg_len, args, NULL, LF_F_MOD_LOAD);
}
#endif

#ifdef F_SifLoadModuleEncrypted
int SifLoadModuleEncrypted(const char *path, int arg_len, const char *args)
{
	return _SifLoadModule(path, arg_len, args, NULL, LF_F_MG_MOD_LOAD);
}
#endif

#ifdef F__SifLoadElfPart
struct _lf_elf_load_arg {
	union {
		u32	epc;
		int	result;
	} p;
	u32	gp;
	char	path[LF_PATH_MAX];
	char	secname[LF_ARG_MAX];
} ALIGNED(16);

int _SifLoadElfPart(const char *path, const char *secname, t_ExecData *data, int fno)
{
	struct _lf_elf_load_arg arg;

	if (!_lf_caps && SifLoadFileInit() < 0)
		return -E_LIB_API_INIT;

	strncpy(arg.path, path, LF_PATH_MAX);
	strncpy(arg.secname, secname, LF_ARG_MAX);

	if (SifCallRpc(&_lf_cd, fno, 0, &arg, sizeof arg, &arg,
				sizeof(t_ExecData), NULL, NULL) < 0)
		return -E_SIF_RPC_CALL;

	if (arg.p.result < 0)
		return arg.p.result;

	if (data) {
		data->epc = arg.p.epc;
		data->gp  = arg.gp;
	}

	return 0;
}
#endif

#ifdef F_SifLoadElfPart
int SifLoadElfPart(const char *path, const char *secname, t_ExecData *data)
{
	return _SifLoadElfPart(path, secname, data, LF_F_ELF_LOAD);
}
#endif

#ifdef F_SifLoadElf
int SifLoadElf(const char *path, t_ExecData *data)
{
	u32 secname = 0x6c6c61;  /* "all" */
	return _SifLoadElfPart(path, (char *)&secname, data, LF_F_ELF_LOAD);
}
#endif

#ifdef F_SifLoadElfEncrypted
int SifLoadElfEncrypted(const char *path, t_ExecData *data)
{
	u32 secname = 0x6c6c61;  /* "all" */
	return _SifLoadElfPart(path, (char *)&secname, data, LF_F_MG_ELF_LOAD);
}
#endif

#ifdef F_SifIopSetVal
int SifIopSetVal(u32 iop_addr, int val, int type)
{
	struct _lf_iop_val_arg arg;

	if (!_lf_caps && SifLoadFileInit() < 0)
		return -E_LIB_API_INIT;

	switch (type) {
		case LF_VAL_BYTE:
			arg.val.b = (u8)(val & 0xff);
			break;
		case LF_VAL_SHORT:
			arg.val.s = (u16)(val & 0xffff);
			break;
		case LF_VAL_LONG:
			arg.val.l = val;
			break;
		default:
			return -E_LIB_INVALID_ARG;
	}

	arg.p.iop_addr = iop_addr;
	arg.type = type;

	if (SifCallRpc(&_lf_cd, LF_F_SET_ADDR, 0, &arg, sizeof arg, &arg, 4,
				NULL, NULL) < 0)
		return -E_SIF_RPC_CALL;

	return arg.p.result;
}
#endif

#ifdef F_SifIopGetVal
int SifIopGetVal(u32 iop_addr, void *val, int type)
{
	struct _lf_iop_val_arg arg;

	if (!_lf_caps && SifLoadFileInit() < 0)
		return -E_LIB_API_INIT;

	arg.p.iop_addr = iop_addr;
	arg.type = type;

	if (SifCallRpc(&_lf_cd, LF_F_GET_ADDR, 0, &arg, sizeof arg, &arg, 4,
				NULL, NULL) < 0)
		return -E_SIF_RPC_CALL;

	if (val) {
		switch (type) {
			case LF_VAL_BYTE:
				*(u8 *)val  = (u8)arg.p.result & 0xff;
				break;
			case LF_VAL_SHORT:
				*(u16 *)val = (u16)arg.p.result & 0xffff;
				break;
			case LF_VAL_LONG:
				*(u32 *)val = arg.p.result;
				break;
		}
	}

	return 0;
}
#endif

#ifdef F__SifLoadModuleBuffer
struct _lf_module_buffer_load_arg {
	union {
		void	*ptr;
		int	result;
	} p;
	union {
		int	arg_len;
		int	modres;
	} q;
	char	unused[LF_PATH_MAX];
	char	args[LF_ARG_MAX];
} ALIGNED(16);

/* TODO: This needs some sort of capability check.  */
int _SifLoadModuleBuffer(void *ptr, int arg_len, const char *args, int *modres)
{
	struct _lf_module_buffer_load_arg arg;

	if (!_lf_caps && SifLoadFileInit() < 0)
		return -E_LIB_API_INIT;

	memset(&arg, 0, sizeof arg);

	arg.p.ptr = ptr;
	if (args && arg_len) {
		arg.q.arg_len = arg_len > LF_ARG_MAX ? LF_ARG_MAX : arg_len;
		memcpy(arg.args, args, arg.q.arg_len);
	}

	if (SifCallRpc(&_lf_cd, LF_F_MOD_BUF_LOAD, 0, &arg, sizeof arg, &arg, 8,
				NULL, NULL) < 0)
		return -E_SIF_RPC_CALL;

	if (modres)
		*modres = arg.q.modres;

	return arg.p.result;
}
#endif

#ifdef F_SifLoadModuleBuffer
int SifLoadModuleBuffer(void *ptr, int arg_len, const char *args)
{
	return _SifLoadModuleBuffer(ptr, arg_len, args, NULL);
}
#endif
