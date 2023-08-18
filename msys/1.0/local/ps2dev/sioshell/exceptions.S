#////////////////////////////////////////////////////////////////////////////////
#// exceptions.S - Exception handlers for sio shell
#// Written by TyRaNiD (c) 2005
#// Much props to Herben und Pixel
#////////////////////////////////////////////////////////////////////////////////

#include "r5900_regs.h"

	.text
	.p2align 3

	.set push
	.set noreorder
	.set noat

    .global _level2SavedRegs
	.global _level2ExceptionStack
        
    .global level2ExceptionHandler
    .ent	level2ExceptionHandler

level2ExceptionHandler:

	sq	k0, -0x20($0)
	la	k0, _level2SavedRegs
#lui	k1, 0x8000
#	or	k0, k0, k1

    sq      $0,  0x00(k0)
    sq      $1,  0x10(k0)
    sq      $2,  0x20(k0)
    sq      $3,  0x30(k0)
    sq      $4,  0x40(k0)
    sq      $5,  0x50(k0)
    sq      $6,  0x60(k0)
    sq      $7,  0x70(k0)
    sq      $8,  0x80(k0)
    sq      $9,  0x90(k0)
    sq      $10,  0xa0(k0)
    sq      $11,  0xb0(k0)
    sq      $12,  0xc0(k0)
    sq      $13,  0xd0(k0)
    sq      $14,  0xe0(k0)
    sq      $15,  0xf0(k0)
    sq      $16,  0x100(k0)
    sq      $17,  0x110(k0)
    sq      $18,  0x120(k0)
    sq      $19,  0x130(k0)
    sq      $20,  0x140(k0)
    sq      $21,  0x150(k0)
    sq      $22,  0x160(k0)
    sq      $23,  0x170(k0)
    sq      $24,  0x180(k0)
    sq      $25,  0x190(k0)
# Restore k0 and save
	lq		$1, -0x20($0)
    sq      $1, 0x1a0(k0)         # zero instead of k0
    sq      $27,  0x1b0(k0)          # $k1
    sq      $28,  0x1c0(k0)
    sq      $29,  0x1d0(k0)          # sp
    sq      $30,  0x1e0(k0)
    sq      $31,  0x1f0(k0)          # $ra

    pmfhi   t0                      # HI
    pmflo   t1                      # LO
    sq      t0, 0x200(k0)
    sq      t1, 0x210(k0)
       
    mfc0    t0, BadVAddr            # Cop0 state regs
    mfc0    t1, Status
    sw      t0, 0x220(k0)
    sw      t1, 0x224(k0)

    mfc0    t0, Cause
    mfc0    t1, EPC
    sw      t0, 0x228(k0)
    sw      t1, 0x22c(k0)

    # Kernel saves these two also..
	mfc0	t0, ErrorPC
    mfc0    t1, PerfCnt
    sw      t0, 0x230(k0)
    sw      t1, 0x234(k0)

    mfsa    t0
    sw      t0, 0x238(k0)

# Use our own stack..
    la      sp, _level2ExceptionStack+0x2800-16
    la      gp, _gp                 # Use exception handlers _gp

    # Return from exception and start 'debugger'
    jal      sio_shell_level2 
	nop

    la      k0, _level2SavedRegs
              
    lq      t0, 0x200(k0)
    lq      t1, 0x210(k0)
    pmthi   t0                      # HI
    pmtlo   t1                      # LO
      
    lw      t0, 0x220(k0)
    lw      t1, 0x224(k0)
    mtc0    t0, BadVAddr
    mtc0    t1, Status

    lw      t0, 0x228(k0)
    lw      t1, 0x22c(k0)
    mtc0    t0, Cause
    mtc0    t1, EPC

    # Kernel saves these two also..
    lw      t0, 0x230(k0)
    lw      t1, 0x234(k0)
	mtc0	t0, ErrorPC
    mtc0    t1, PerfCnt

    # Shift Amount reg
    lw      t0, 0x238(k0)
    mtsa    t0

    lq      $0,  0x00(k0)
    lq      $1,  0x10(k0)
    lq      $2,  0x20(k0)
    lq      $3,  0x30(k0)
    lq      $4,  0x40(k0)
    lq      $5,  0x50(k0)
    lq      $6,  0x60(k0)
    lq      $7,  0x70(k0)
    lq      $8,  0x80(k0)
    lq      $9,  0x90(k0)
    lq      $10,  0xa0(k0)
    lq      $11,  0xb0(k0)
    lq      $12,  0xc0(k0)
    lq      $13,  0xd0(k0)
    lq      $14,  0xe0(k0)
    lq      $15,  0xf0(k0)
    lq      $16,  0x100(k0)
    lq      $17,  0x110(k0)
    lq      $18,  0x120(k0)
    lq      $19,  0x130(k0)
    lq      $20,  0x140(k0)
    lq      $21,  0x150(k0)
    lq      $22,  0x160(k0)
    lq      $23,  0x170(k0)
    lq      $24,  0x180(k0)
    lq      $25,  0x190(k0)
# no k0
    lq      $27,  0x1b0(k0)          # $k1
    lq      $28,  0x1c0(k0)
    lq      $29,  0x1d0(k0)          # sp
    lq      $30,  0x1e0(k0)
    lq      $31,  0x1f0(k0)          # $ra

	lq		k0, -0x20($0)

	sync
	eret	
	nop

    .end	level2ExceptionHandler

    .global _level1SavedRegs
	.global _level1ExceptionStack
        
    .global level1ExceptionHandler
    .ent	level1ExceptionHandler

level1ExceptionHandler:


# Kernel doesn't save k0 anyway so why should we bother (it is already trashed)
	la	k0, _level1SavedRegs


    sq      $0,  0x00(k0)


    sq      $1,  0x10(k0)
    sq      $2,  0x20(k0)
    sq      $3,  0x30(k0)
    sq      $4,  0x40(k0)
    sq      $5,  0x50(k0)
    sq      $6,  0x60(k0)
    sq      $7,  0x70(k0)
    sq      $8,  0x80(k0)
    sq      $9,  0x90(k0)
    sq      $10,  0xa0(k0)
    sq      $11,  0xb0(k0)
    sq      $12,  0xc0(k0)
    sq      $13,  0xd0(k0)
    sq      $14,  0xe0(k0)
    sq      $15,  0xf0(k0)
    sq      $16,  0x100(k0)
    sq      $17,  0x110(k0)
    sq      $18,  0x120(k0)
    sq      $19,  0x130(k0)
    sq      $20,  0x140(k0)
    sq      $21,  0x150(k0)
    sq      $22,  0x160(k0)
    sq      $23,  0x170(k0)
    sq      $24,  0x180(k0)
    sq      $25,  0x190(k0)
# Restore k0 and save
    sq      $0, 0x1a0(k0)         # zero instead of k0
    sq      $27,  0x1b0(k0)          # $k1
    sq      $28,  0x1c0(k0)
    sq      $29,  0x1d0(k0)          # sp
    sq      $30,  0x1e0(k0)
    sq      $31,  0x1f0(k0)          # $ra

    pmfhi   t0                      # HI
    pmflo   t1                      # LO
    sq      t0, 0x200(k0)
    sq      t1, 0x210(k0)
       
    mfc0    t0, BadVAddr            # Cop0 state regs
    mfc0    t1, Status
    sw      t0, 0x220(k0)
    sw      t1, 0x224(k0)

    mfc0    t0, Cause
    mfc0    t1, EPC
    sw      t0, 0x228(k0)
    sw      t1, 0x22c(k0)

    # Kernel saves these two also..
	mfc0	t0, ErrorPC
    mfc0    t1, PerfCnt
    sw      t0, 0x230(k0)
    sw      t1, 0x234(k0)

    mfsa    t0
    sw      t0, 0x238(k0)

# Use our own stack..
    la      sp, _level1ExceptionStack+0x2800-16
    la      gp, _gp                 # Use exception handlers _gp

    # Return from exception and start 'debugger'
	jal      sio_shell_level1
	nop

    la      k0, _level1SavedRegs
              
    lq      t0, 0x200(k0)
    lq      t1, 0x210(k0)
    pmthi   t0                      # HI
    pmtlo   t1                      # LO
      
    lw      t0, 0x220(k0)
    lw      t1, 0x224(k0)
    mtc0    t0, BadVAddr
    mtc0    t1, Status

    lw      t0, 0x228(k0)
    lw      t1, 0x22c(k0)
    mtc0    t0, Cause
    mtc0    t1, EPC

    # Kernel saves these two also..
    lw      t0, 0x230(k0)
    lw      t1, 0x234(k0)
	mtc0	t0, ErrorPC
    mtc0    t1, PerfCnt

    # Shift Amount reg
    lw      t0, 0x238(k0)
    mtsa    t0

    lq      $0,  0x00(k0)
    lq      $1,  0x10(k0)
    lq      $2,  0x20(k0)
    lq      $3,  0x30(k0)
    lq      $4,  0x40(k0)
    lq      $5,  0x50(k0)
    lq      $6,  0x60(k0)
    lq      $7,  0x70(k0)
    lq      $8,  0x80(k0)
    lq      $9,  0x90(k0)
    lq      $10,  0xa0(k0)
    lq      $11,  0xb0(k0)
    lq      $12,  0xc0(k0)
    lq      $13,  0xd0(k0)
    lq      $14,  0xe0(k0)
    lq      $15,  0xf0(k0)
    lq      $16,  0x100(k0)
    lq      $17,  0x110(k0)
    lq      $18,  0x120(k0)
    lq      $19,  0x130(k0)
    lq      $20,  0x140(k0)
    lq      $21,  0x150(k0)
    lq      $22,  0x160(k0)
    lq      $23,  0x170(k0)
    lq      $24,  0x180(k0)
    lq      $25,  0x190(k0)
# no k0
    lq      $27,  0x1b0(k0)          # $k1
    lq      $28,  0x1c0(k0)
    lq      $29,  0x1d0(k0)          # sp
    lq      $30,  0x1e0(k0)
    lq      $31,  0x1f0(k0)          # $ra

	sync
	eret	
	nop

    .end	level1ExceptionHandler

# Restore the context and jump to Syscall handler
	.global restartSyscallException
	.ent 	restartSyscallException
restartSyscallException:
	la      k0, _level1SavedRegs
              
    lq      t0, 0x200(k0)
    lq      t1, 0x210(k0)
    pmthi   t0                      # HI
    pmtlo   t1                      # LO
      
    lw      t0, 0x220(k0)
    lw      t1, 0x224(k0)
    mtc0    t0, BadVAddr
    mtc0    t1, Status

    lw      t0, 0x228(k0)
    lw      t1, 0x22c(k0)
    mtc0    t0, Cause
    mtc0    t1, EPC

    # Kernel saves these two also..
    lw      t0, 0x230(k0)
    lw      t1, 0x234(k0)
	mtc0	t0, ErrorPC
    mtc0    t1, PerfCnt

    # Shift Amount reg
    lw      t0, 0x238(k0)
    mtsa    t0

    lq      $0,  0x00(k0)
    lq      $1,  0x10(k0)
    lq      $2,  0x20(k0)
    lq      $3,  0x30(k0)
    lq      $4,  0x40(k0)
    lq      $5,  0x50(k0)
    lq      $6,  0x60(k0)
    lq      $7,  0x70(k0)
    lq      $8,  0x80(k0)
    lq      $9,  0x90(k0)
    lq      $10,  0xa0(k0)
    lq      $11,  0xb0(k0)
    lq      $12,  0xc0(k0)
    lq      $13,  0xd0(k0)
    lq      $14,  0xe0(k0)
    lq      $15,  0xf0(k0)
    lq      $16,  0x100(k0)
    lq      $17,  0x110(k0)
    lq      $18,  0x120(k0)
    lq      $19,  0x130(k0)
    lq      $20,  0x140(k0)
    lq      $21,  0x150(k0)
    lq      $22,  0x160(k0)
    lq      $23,  0x170(k0)
    lq      $24,  0x180(k0)
    lq      $25,  0x190(k0)
# no k0
    lq      $27,  0x1b0(k0)          # $k1
    lq      $28,  0x1c0(k0)
    lq      $29,  0x1d0(k0)          # sp
    lq      $30,  0x1e0(k0)
    lq      $31,  0x1f0(k0)          # $ra

	j		0x280
	nop

	.end	restartSyscallException

	.bss
	.align  4
	.ent	_level2SavedRegs
_level2SavedRegs:
	.space  0x240, 0
	.end	_level2SavedRegs

	.align  4
	.ent	_level2ExceptionStack
_level2ExceptionStack:
	.space	0x2800, 0
	.end	_level2ExceptionStack

	.align  4
	.ent	_level1SavedRegs
	_level1SavedRegs:
.space	0x240, 0
	.end	_level1SavedRegs

	.align  4
	.ent	_level1ExceptionStack
_level1ExceptionStack:
	.space	0x2800, 0
	.end	_level1ExceptionStack
