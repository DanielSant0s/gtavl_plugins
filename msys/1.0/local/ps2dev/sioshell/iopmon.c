#include <kernel.h>
#include <sifcmd.h>
#include <sifrpc.h>
#include <ps2lib_err.h>
#include "sio_shell.h"
#include "iopmon.h"

/*static SifRpcClientData_t client __attribute__((aligned(64)));*/

#define CMD_PACKET_MAX		128
#define CMD_PACKET_DATA_MAX 	112
/* Set if the packet has been allocated */
#define PACKET_F_ALLOC	0x01
#define RPC_PACKET_SIZE	64

static int SifSetRegHook(u32 reg, u32 val);
static void OnRpcInit(void);

static struct cmd_data* g_cmd;
static struct rpc_data* g_rpc;

struct cmd_data {
	void	*pktbuf;	/* Command packet received from the IOP */
	void	*unused;	/* Called "cmdbuf", but unused. */
	void	*iopbuf;	/* Address of IOP SIF DMA receive address */
	SifCmdHandlerData_t *sys_cmd_handlers;
	u32	nr_sys_handlers;
	SifCmdHandlerData_t *usr_cmd_handlers;
	u32	nr_usr_handlers;
	int	*sregs;
};

/* RPC Data structure for current RPC implementation */
struct rpc_data {
	int	pid;
	void	*pkt_table;
	int	pkt_table_len;
	int	unused1;
	int	unused2;
	u8	*rdata_table;
	int	rdata_table_len;
	u8	*client_table;
	int	client_table_len;
	int	rdata_table_idx;
	void	*active_queue;
};

extern void *_rpc_get_packet(struct rpc_data *rpc_data);

void install_sifhook(void)
{
	/* We have not already installed a hook */
	if(0x80050000 > (u32) _syscallTable->SifSetReg)
	{
		_syscallTableOld->SifSetReg = _syscallTable->SifSetReg;
	}

	_syscallTable->SifSetReg = (void *) SifSetRegHook;
	flush_cache();
}

static int SifSetRegHook(u32 reg, u32 val)
{
	int ret;
	static int reboot = 0;
	static int rpcinit = 0;
	ret = _syscallTableOld->SifSetReg(reg, val);
	if((reg == 0x80000000) && (val == 0))
	{
		rpcinit = 0;
		reboot = 1;
		printf("IOP Reboot\n");
	}
	else if((reg == 0x80000002) && (val == 1))
	{
		if(reboot && !rpcinit)
		{
			reboot = 0;
			rpcinit = 1;
			printf("RPC Init\n");
			OnRpcInit();
		}
	}

	return ret;
}

static int _SifBindRpc(SifRpcClientData_t *cd, int sid, int mode);
static int _SifCheckStatRpc(SifRpcClientData_t *cd);

static void OnRpcInit(void)
{
	//int ret;

	g_cmd = (struct cmd_data*) _syscallTable->SifGetReg(0x80000001);
	printf("SifReg 80000001 = %08x\n", (u32) g_cmd);
	printf("Pktbuf %08x, iopbuf %08x\n", (u32) g_cmd->pktbuf, (u32) g_cmd->iopbuf);
	g_rpc = (struct rpc_data *) g_cmd->sys_cmd_handlers[8].harg;
	printf("RPC Pid %08x pkt_tbl %08x\n", g_rpc->pid, g_rpc->pkt_table);

	//ret = _SifBindRpc(&client, 0x80000003, 0);
	//printf("ret %d\n", ret);
	//while(_SifCheckStatRpc(&client));
}

/*static u32 _SifSendCmd(int cid, void *pkt, u32 pktsize, void *src, void *dest, int size)*/
/*{*/
/*    SifDmaTransfer_t dmat[2];*/
/*    SifCmdHeader_t *header;*/
/*    int count = 0;*/

/*    pktsize &= 0xff;*/

/*    if (pktsize > CMD_PACKET_DATA_MAX)*/
/*        return 0;*/

/*    header = (SifCmdHeader_t *)pkt;*/
/*    header->cid  = cid;*/
/*    header->size = pktsize;*/
/*    header->dest = NULL;*/

/*    if (size > 0) {*/
/*        header->size = pktsize | (size << 8);*/
/*        header->dest = dest;*/

/*        dmat[count].src  = src;*/
/*        dmat[count].dest = dest;*/
/*        dmat[count].size = size;*/
/*        dmat[count].attr = 0;*/
/*        count++;*/
/*    }*/

/*    dmat[count].src  = pkt;*/
/*    dmat[count].dest = g_cmd->iopbuf;*/
/*    dmat[count].size = pktsize;*/
/*    dmat[count].attr = 0x40 | SIF_DMA_INT_O;*/
/*    count++;*/

/*    SifWriteBackDCache(pkt, pktsize);*/

/*    return _syscallTable->SifSetDma(dmat, count);*/
/*}*/

/*static int _SifCheckStatRpc(SifRpcClientData_t *cd)*/
/*{*/
/*    SifRpcPktHeader_t *packet = (SifRpcPktHeader_t *)cd->hdr.pkt_addr;*/

/*    if (!packet)*/
/*        return 0;*/

/*    if (cd->hdr.rpc_id != packet->rpc_id)*/
/*        return 0;*/

/*    if (!(packet->rec_id & PACKET_F_ALLOC))*/
/*        return 0;*/

/*    return 1;*/
/*}*/

/*static int _SifBindRpc(SifRpcClientData_t *cd, int sid, int mode)*/
/*{*/
/*    SifRpcBindPkt_t *bind;*/

/*    bind = (SifRpcBindPkt_t *)_rpc_get_packet(g_rpc);*/
/*    if (!bind)*/
/*        return -E_SIF_PKT_ALLOC;*/

/*    cd->command      = 0;*/
/*    cd->server       = NULL;*/
/*    cd->hdr.pkt_addr = bind;*/
/*    cd->hdr.rpc_id   = bind->rpc_id;*/
/*    cd->hdr.sema_id  = -1;*/

/*    bind->sid        = sid;*/
/*    bind->pkt_addr   = bind;*/
/*    bind->client     = cd;*/

/*    if (!_SifSendCmd(0x80000009, bind, RPC_PACKET_SIZE, NULL, NULL, 0))*/
/*            return -E_SIF_PKT_SEND;*/

/*    return 0;*/
/*}*/

