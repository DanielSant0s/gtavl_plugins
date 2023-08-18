/*
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2001, Gustavo Scotti (gustavo@scotti.com)
  ------------------------------------------------------------------------
  iop_sifman.s		   Serial Interface Manager Functions.
			   taken from .irx files with symbol table.
*/

	.text
	.set	noreorder


/* ############################### SIFMAN STUB ######## */
/* # Added by Oobles, 7th March 2002                  # */

	.local	sifman_stub
sifman_stub:
	.word	0x41e00000
	.word	0
	.word	0x00000101
	.ascii	"sifman\0\0"
	.align	2

	.globl  SifInit				# 0x05
SifInit:
	j	$31
	li	$0, 0x05

	.globl	SifSetDChain			# 0x06
SifSetDChain:
	j	$31
	li	$0, 0x06

	.globl	SifSetDma			# 0x07
SifSetDma:
	j	$31
	li	$0, 0x07

	.globl	SifDmaStat			# 0x08
SifDmaStat:
	j	$31
	li	$0, 0x08
	

	.globl	SifCheckInit			# 0x1d
SifCheckInit:
	j	$31
	li	$0, 0x1d

	.globl	SifSetDmaIntr			# 0x20
SifSetDmaIntr:
	j	$31
	li	$0, 0x20

	.word	0
	.word	0


