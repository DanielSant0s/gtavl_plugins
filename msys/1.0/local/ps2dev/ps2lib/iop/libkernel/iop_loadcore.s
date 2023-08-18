/*
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2002, David Ryan (Oobles@hotmail.com)
  ------------------------------------------------------------------------
  iop_loadcore.s	   Core IOP Functions.
			   taken from .irx files with symbol table.
*/

	.text
	.set	noreorder



/* ############################### LOADCORE STUB ###### */
/* # Added by Oobles, 5th March 2002                  # */

	.local	loadcore_stub
loadcore_stub:
	.word	0x41e00000
	.word	0
	.word	0x00000101
	.ascii	"loadcore"
	.align	2

	.globl	GetExportTable			# 0x03
GetExportTable:
	j	$31
	li	$0, 0x03

	.globl	FlushDcache			# 0x05
FlushDcache:
	j	$31
	li	$0, 0x05

	.global RegisterLibraryEntries		# 0x06
RegisterLibraryEntries:
	j	$31
	li	$0, 0x06

	.globl	QueryBootMode			# 0x0c
QueryBootMode:
	j	$31
	li	$0, 0x0c

	.word	0
	.word	0



