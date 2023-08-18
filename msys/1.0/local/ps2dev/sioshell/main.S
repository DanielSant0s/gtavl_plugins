#////////////////////////////////////////////////////////////////////////////////
#// main.S - Entry point for kernel mode sioshell
#// Written by TyRaNiD (c) 2005
#// Much props to Herben und Pixel
#////////////////////////////////////////////////////////////////////////////////

#include "r5900_regs.h"

	.text
	.p2align 3

	.set push
	.set noreorder
	.set noat

	.global siosh_init

	.global _start
	.ent _start

_start:
	addiu  sp, sp, -16 
	sd	ra, 0(sp)

	jal	siosh_init
	nop

	ld	ra, 0(sp) 
	addiu  sp, sp, 16
	jr	ra
	nop

	.end _start

	.global flush_cache
	.ent flush_cache
flush_cache:
	li	$8, 0
1:
	sync
	cache	0x14, 0($8)
	sync
	cache	0x14, 1($8)
	sync
	addiu	$8, 0x40
	slti	$9, $8, 0x1000
	bnez	$9, 1b
	nop
	li	$8, 0
2:
	sync
	cache	7, 0($8)
	cache	7, 1($8)
	sync
	addiu	$8, 0x40
	slti	$9, $8, 0x2000
	bnez	$9, 2b
	nop
	cache	0xc, 0($0)
	sync
	jr	$31
	nop
	.end flush_cache
