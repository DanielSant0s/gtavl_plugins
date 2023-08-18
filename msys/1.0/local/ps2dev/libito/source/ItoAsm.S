
#ifdef F_ScratchpadStack
.sdata
	.align	4
stackpointer:
	.dword	0

.set noat
.set noreorder
.text

	.globl	EnableScratchpadStack
	.ent	EnableScratchpadStack
EnableScratchpadStack:
	sw $29, stackpointer
	li $29, 0x70000000
	jr $31
	nop
	.end EnableScratchpadStack

	.globl	DisableScratchpadStack
	.ent	DisableScratchpadStack
DisableScratchpadStack:
	lw $29, stackpointer
	jr $31
	nop
	.end DisableScratchpadStack
#endif
