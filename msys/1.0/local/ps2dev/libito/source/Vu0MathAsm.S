.set noat
.set noreorder
.text


#ifdef F_VpuReset
	.globl	VpuReset
	.ent	VpuReset
VpuReset:
	cfc2		$3,$28
	li		$2, 0xFFFFFF00
	and		$2, $3
	ori		$2, 2
	ctc2		$3, $28
	sync.p
	jr		$31
	nop
	.end VpuReset
#endif

#ifdef F_vsinf
	.globl	vsinf
	.ent	vsinf
vsinf:
	mfc1		$14, $f12
	qmtc2		$14, $vf19
	daddu		$14, $31, $0
	jal		macroSinCos
	nop
	qmfc2		$13, $vf19
	daddu		$31, $14, $0
	jr		$31
	mtc1		$13, $f0
	.end vsinf
#endif


#ifdef F_vcosf
	.globl	vcosf
	.ent	vcosf
vcosf:
	mfc1		$14, $f12
	qmtc2		$14, $vf19
	daddu		$14, $31, $0
	jal		macroSinCos
	nop
	vaddy.x		$vf19x, $vf0x, $vf19y
	qmfc2		$13, $vf19
	daddu		$31, $14, $0
	jr		$31
	mtc1		$13, $f0
	.end vcosf
#endif


#//////////////////////////////////////////////////////////////////////////////////
# input vf19 , x , z
# output vf19 x=sinx,y=cosx z=sinz,w=cosz
#
# touches
# ACC,I
# vf14, vf15, vf16, vf17, vf18, vf19, vf20, vf21, $15
# --------------------------------------------------------------------
# doSinCos - Returns the sin and cos of up to 2 angles, which must
# be contained in the X and Z elements of "angle".  The sin/cos pair
# will be contained in the X/Y elements of "sincos" for the first
# angle, and Z/W for the second one.
# Thanks to Colin Hughes (SCEE) for that one.
# Thanks to the playstation2 linux forums and site for it.
# Thanks to adresd for vu1 version this is based on and blackdroid for help on sync.p
# Original name doSinCos


#ifdef F_MacroSinCos
	.globl	MacroSinCos
	.ent	MacroSinCos
MacroSinCos:
	vmulz.w		$vf19, $vf00, $vf19z	# copy angle from z to w
	vaddx.y		$vf19, $vf00, $vf19x	# copy angle from x to y
	li		$15, 0x3fc90fd8		# 1.570796
	ctc2		$15, $21
	sync.p
	vsubi.xz	$vf19, $vf19, I		# phase difference for sin as cos ( pi/2 )
	vabs		$vf19, $vf19		# mirror cos around zero
	vmaxw		vf20, vf00, vf00w	# initialise all 1s 	
	li		$15, 0xbe22f987		# -0.159155
	ctc2		$15, $21
	sync.p
	vmulai		ACC, $vf19, I		# scale so single cycle is range 0 to -1 ( *-1/2pi )
	li		$15, 0x4b400000		# 12582912.0
	ctc2		$15, $21
	sync.p
	vmsubai		ACC,  $vf20, I		# apply bias to remove fractional part
	vmaddai		ACC,  $vf20, I		# remove bias to leave original int part
	li		$15, 0xbe22f987		# -0.159155
	ctc2		$15, $21
	sync.p
	vmsubai		ACC,  $vf19, I		# apply original number to leave fraction range only
	li		$15, 0x3f000000		# 0.5
	ctc2		$15, $21
	sync.p
	vmsubi		$vf19, $vf20, I		# ajust range: -0.5 to +0.5
	vabs		$vf19, $vf19		# clamp: 0 to +0.5
	li		$15, 0x3e800000		# 0.25
	ctc2		$15, $21
	sync.p
	vsubi		$vf19, $vf19, I		# ajust range: -0.25 to +0.25
	vmul		$vf21, $vf19, $vf19	# a^2
	li		$15, 0xc2992661		# -76.574959
	ctc2		$15, $21
	sync.p
	vmuli		$vf14, $vf19, I		# k4 a 
	li		$15, 0xc2255de0		# -41.341675
	ctc2		$15, $21
	sync.p
	vmuli		$vf16, $vf19, I		# k2 a
	li		$15, 0x42a33457		# 81.602226
	ctc2		$15, $21
	sync.p
	vmuli		$vf15, $vf19, I		# k3 a
	vmul		$vf18, $vf21, $vf21	# a^4
	vmul		$vf14, $vf14, $vf21	# k4 a^3
	vmula		ACC,  $vf16, $vf21	# + k2 a^3
	li		$15, 0x421ed7b7		# 39.710659
	ctc2		$15, $21
	sync.p
	vmuli		$vf16, $vf19, I		# k5 a
	vmul		$vf17, $vf18, $vf18	# a^8
	vmadda		ACC, $vf14, $vf18	# + k4 a^7
	vmadda		ACC, $vf15, $vf18	# + k3 a^5
	li		$15, 0x40c90fda		# 6.283185
	ctc2		$15, $21
	sync.p
	vmaddai		ACC, $vf19, I		# + k1 a
	vmadd		$vf19, $vf16, $vf17	# + k5 a^9
	jr		$31
	nop
	.end MacroSinCos
#endif

#ifdef F_MacroMatrixMult
	.globl	MacroMatrixMult
	.ent	MacroMatrixMult
MacroMatrixMult:
	addiu		$2, $0, 4		# 4 colums
MacroMatrixMultLoop:
	lqc2		$vf4, 0($4)		# load colum 
	vmulax.xyzw	ACC, $vf6, $vf4x
	vmadday.xyzw	ACC, $vf7, $vf4y
	vmaddaz.xyzw	ACC, $vf8, $vf4z
	vmaddw.xyzw	$vf5, $vf9, $vf4w	# +ACC also included
	sqc2		$vf5,  0($4)		# save colum / vector
	addi		$2, -1
	bnel		$2, $0, MacroMatrixMultLoop
	addi		$4, 16			# next colum
	jr		$31
	nop
	.end	MacroMatrixMult
#endif
