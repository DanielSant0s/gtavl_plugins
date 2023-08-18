#include <sio_shell.h>
#include "R5900Disasm.h"

#undef DI
#undef EI


static const char *GPR[32] = {
    "zr", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"
};


typedef void (*decode_op)(u32 opcode, u32 pc);

#define D_OP(op) static void op(u32 opcode, u32 pc)
#define RS ((opcode >> 21) & 0x1F)
#define RT ((opcode >> 16) & 0x1F)
#define RD ((opcode >> 11) & 0x1F)
#define SA ((opcode >> 6) & 0x1F)
#define FUNCTION (opcode & 0x3F)

#define Name(name) printf(#name "\t")

#define Sep() printf(", ")

#define Imm() { s16 v = (opcode & 0xFFFF); printf("%c0x%04x", v < 0 ? '-' : ' ', v < 0 ? -v : v); }
#define UImm() printf("0x%04x", ((u32)(opcode & 0xFFFF)))
#define Sa()  printf("%d", SA)

#define GPR(reg) printf("$%s", GPR[reg])

#define C0R(reg) printf("$%02x", reg);
#define FPR(reg) printf("$f%02x", reg);
#define C2R(reg) printf("$v%02x", reg);

#define OfS16() printf("%08x", (u32) (((s32) (((opcode & 0xFFFF) << 2)) + (s32) (pc + 4))))
#define OfS26() printf("%08x", (((opcode & 0x3FFFFFF) << 2) + (pc & 0xF0000000)))
#define OfB() { s16 v = (opcode & 0xFFFF); printf("%c0x%04x($%s)", v < 0 ? '-' : ' ', v < 0 ? -v : v, GPR[RS]); }

#define IsZero(field) if (field) printf("; field " #field " not empty - opcode invalid ?\n")
#define Invalid() printf("; Strange or invalid.");

#define Trap() printf("; code = %x.", (opcode >> 6) & 0x3ff)

#define UNDEF { printf("UNDEFINED %08X", opcode); }


extern decode_op special_table[64];
extern decode_op regimm_table[32];
extern decode_op cop0_table[32];
extern decode_op cop1_table[32];
extern decode_op cop2_table[32];
extern decode_op mmi_table[64];
extern decode_op bc0_table[32];
extern decode_op c0_table[64];


D_OP(UNK) 	{ Name(UNKNOWN); Invalid(); }

/* Normal Opcode Table */
// 000000
D_OP(SPECIAL)	{ special_table[FUNCTION](opcode, pc); }
D_OP(REGIMM)	{ regimm_table[RT](opcode, pc); }
D_OP(J) 	{ Name(J);      OfS26(); }
D_OP(JAL)	{ Name(JAL);    OfS26(); }
D_OP(BEQ)	{
    if ((!RS) && (!RT)) {
	Name(B);      OfS16();
    } else if (!RT) {
	Name(BEZ);    GPR(RS); Sep(); OfS16();
    } else {
	Name(BEQ);    GPR(RS); Sep(); GPR(RT); Sep(); OfS16();
    }
}
D_OP(BNE)	{
    if (!RT) {
	Name(BEQ);    GPR(RS); Sep(); OfS16();
    } else {
	Name(BNE);    GPR(RS); Sep(); GPR(RT); Sep(); OfS16();
    }
}
D_OP(BLEZ)	{ Name(BLEZ);   GPR(RS); Sep(); OfS16(); IsZero(RT); }
D_OP(BGTZ)	{ Name(BGTZ);   GPR(RS); Sep(); OfS16(); IsZero(RT); }

// 001000
D_OP(ADDI)	{ Name(ADDI);   GPR(RT); Sep(); GPR(RS); Sep(); Imm(); }
D_OP(ADDIU)	{
    if (!RS) {
	Name(LI);     GPR(RT); Sep(); Imm();
    } else {
	Name(ADDIU);  GPR(RT); Sep(); GPR(RS); Sep(); Imm();
    }
}
D_OP(SLTI)	{ Name(SLTI);   GPR(RT); Sep(); GPR(RS); Sep(); Imm(); }
D_OP(SLTIU)	{ Name(SLTIU);  GPR(RT); Sep(); GPR(RS); Sep(); UImm(); }
D_OP(ANDI)	{
    if (RS == RT) {
	Name(ANDI);   GPR(RT); Sep(); UImm();
    } else {
        Name(ANDI);   GPR(RT); Sep(); GPR(RS); Sep(); UImm();
    }
}
D_OP(ORI)	{
    if (RS == RT) {
	Name(ORI);    GPR(RT); Sep(); UImm();
    } else {
        Name(ORI);    GPR(RT); Sep(); GPR(RS); Sep(); UImm();
    }
}
D_OP(XORI)	{
    if (RS == RT) {
	Name(XORI);   GPR(RT); Sep(); UImm();
    } else {
        Name(XORI);   GPR(RT); Sep(); GPR(RS); Sep(); UImm();
    }
}
D_OP(LUI)	{
    if (RS == RT) {
	Name(LUI);    GPR(RT); Sep(); UImm();
    } else {
        Name(LUI);    GPR(RT); Sep(); GPR(RS); Sep(); UImm();
    }
}

// 010000
D_OP(COP0)	{ cop0_table[RS](opcode, pc); }
D_OP(COP1)	{ cop1_table[RS](opcode, pc); }
D_OP(COP2)	{ cop2_table[RS](opcode, pc); }
// hole at 010011
D_OP(BEQL)	{
    if ((!RS) && (!RT)) {
	Name(BL);     OfS16();
    } else if (!RT) {
	Name(BEQZL);  GPR(RS); Sep(); OfS16();
    } else {
	Name(BEQL);   GPR(RT); Sep(); GPR(RS); Sep(); OfS16();
    }
}
D_OP(BNEL)	{
    if (!RT) {
	Name(BNZL);   GPR(RS); Sep(); OfS16();
    } else {
	Name(BNEL);   GPR(RT); Sep(); GPR(RS); Sep(); OfS16();
    }
}
D_OP(BLEZL)	{ Name(BLEZL);  GPR(RS); Sep(); OfS16(); IsZero(RT); }
D_OP(BGTZL)	{ Name(BGTZL);  GPR(RS); Sep(); OfS16(); IsZero(RT); }

// 011000
D_OP(DADDI)	{ Name(DADDI);  GPR(RT); Sep(); GPR(RS); Sep(); Imm(); }
D_OP(DADDIU)	{
    if (!RS) {
	Name(DLI);    GPR(RT); Sep(); Imm();
    } else {
	Name(DADDIU); GPR(RT); Sep(); GPR(RS); Sep(); Imm();
    }
}
D_OP(LDL)	{ Name(LDL);    GPR(RT); Sep(); OfB(); }
D_OP(LDR)	{ Name(LDR);    GPR(RT); Sep(); OfB(); }
D_OP(MMI)	{ mmi_table[FUNCTION](opcode, pc); }
// hole at 011101
D_OP(LQ)	{ Name(LQ);     GPR(RT); Sep(); OfB(); }
D_OP(SQ)	{ Name(SQ);     GPR(RT); Sep(); OfB(); }

// 100000
D_OP(LB)	{ Name(LB);     GPR(RT); Sep(); OfB(); }
D_OP(LH)	{ Name(LH);     GPR(RT); Sep(); OfB(); }
D_OP(LWL)	{ Name(LWL);    GPR(RT); Sep(); OfB(); }
D_OP(LW)	{ Name(LW);     GPR(RT); Sep(); OfB(); }
D_OP(LBU)	{ Name(LBU);    GPR(RT); Sep(); OfB(); }
D_OP(LHU)	{ Name(LHU);    GPR(RT); Sep(); OfB(); }
D_OP(LWR)	{ Name(LRW);    GPR(RT); Sep(); OfB(); }
D_OP(LWU)	{ Name(LWU);    GPR(RT); Sep(); OfB(); }

// 101000
D_OP(SB)	{ Name(SB);     GPR(RT); Sep(); OfB(); }
D_OP(SH)	{ Name(SH);     GPR(RT); Sep(); OfB(); }
D_OP(SWL)	{ Name(SWL);    GPR(RT); Sep(); OfB(); }
D_OP(SW)	{ Name(SW);     GPR(RT); Sep(); OfB(); }
D_OP(SDL)	{ Name(SDL);    GPR(RT); Sep(); OfB(); }
D_OP(SDR)	{ Name(SDR);    GPR(RT); Sep(); OfB(); }
D_OP(SWR)	{ Name(SWR);    GPR(RT); Sep(); OfB(); }
D_OP(CACHE)	{ Name(CACHE);  printf("0x%02X", RT); Sep(); OfB(); } // **TODO**

// 110000
// hole at 110000
D_OP(LWC1)	{ Name(LWC1);   FPR(RT); Sep(); OfB(); }
// hole at 110010
D_OP(PREF)      { Name(PREF);   printf("0x%02X", RT); Sep(); OfB(); IsZero(RT); } // **TODO**
// hole at 110100
// hole at 110101
D_OP(LQC2)      { Name(LQC2);   C2R(RT); Sep(); OfB(); }
D_OP(LD)        { Name(LD);     GPR(RT); Sep(); OfB(); }

// 111000
// hole at 111000
D_OP(SWC1)	{ Name(SWC1);   FPR(RT); Sep(); OfB(); }
// hole at 111010
// hole at 111011
// hole at 111100
// hole at 111101
D_OP(SQC2)      { Name(SQC2);   C2R(RT); Sep(); OfB(); }
D_OP(SD)        { Name(SD);     GPR(RT); Sep(); OfB(); }


// Special Opcodes table

// 000000
D_OP(SLL)	{
    if (!opcode) {
	Name(NOP);
    } else if (RT == RD) {
	Name(SLL);    GPR(RD); Sep(); Sa(); IsZero(RS);
    } else {
	Name(SLL);    GPR(RD); Sep(); GPR(RT); Sep(); Sa(); IsZero(RS);
    }
}
// hole at 000001
D_OP(SRL)	{
    if (RT == RD) {
	Name(SRL);    GPR(RD); Sep(); Sa(); IsZero(RS);
    } else {
	Name(SRL);    GPR(RD); Sep(); GPR(RT); Sep(); Sa(); IsZero(RS);
    }
}
D_OP(SRA)	{
    if (RT == RD) {
	Name(SRA);    GPR(RD); Sep(); Sa(); IsZero(RS);
    } else {
	Name(SRA);    GPR(RD); Sep(); GPR(RT); Sa(); IsZero(RS);
    }
}
D_OP(SLLV)	{
    if (RT == RD) {
	Name(SLLV);   GPR(RT); Sep(); GPR(RS); IsZero(SA);
    } else {
	Name(SLLV);   GPR(RD); Sep(); GPR(RT); Sep(); GPR(RS); IsZero(SA);
    }
}
// hole at 000101
D_OP(SRLV)	{
    if (RT == RD) {
	Name(SRLV);   GPR(RT); Sep(); GPR(RS); IsZero(SA);
    } else {
	Name(SRLV);   GPR(RD); Sep(); GPR(RT); Sep(); GPR(RS); IsZero(SA);
    }
}
D_OP(SRAV)	{
    if (RT == RD) {
	Name(SRAV);   GPR(RT); Sep(); GPR(RS); IsZero(SA);
    } else {
	Name(SRAV);   GPR(RD); Sep(); GPR(RT); Sep(); GPR(RS); IsZero(SA);
    }
}

// 001000
D_OP(JR)	{ Name(JR);     GPR(RS); IsZero(RT | RD | SA); }
D_OP(JALR)	{
    if (RD == 31) {
	Name(JALR);   GPR(RS); IsZero(RT | SA);
    } else {
	Name(JALR);   GPR(RD); Sep(); GPR(RS); IsZero(RT | SA);
    }
}
D_OP(MOVZ)	{ Name(MOVZ);   GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA); }
D_OP(MOVN)	{ Name(MOVN);   GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA); }
D_OP(SYSCALL)	{ Name(SYSCALL); printf("%d", (opcode >> 6) & 0xFFFFF); } // **TODO**
D_OP(BREAK)	{ Name(BREAK);  printf("%d", (opcode >> 6) & 0xFFFFF); } // **TODO**
// hole at 001110
D_OP(SYNC)	{
    if (SA & 0x10) {
	Name(SYNC.P); if ((opcode >> 11) & 0x7fff) Invalid();
    } else {
	Name(SYNC.L); if ((opcode >> 11) & 0x7fff) Invalid();
    }
}

// 010000
D_OP(MFHI)	{ Name(MFHI);   GPR(RD); IsZero(RS | RT | SA); }
D_OP(MTHI)	{ Name(MTHI);   GPR(RS); IsZero(RD | RT | SA); }
D_OP(MFLO)	{ Name(MFLO);   GPR(RD); IsZero(RS | RT | SA); }
D_OP(MTLO)	{ Name(MTLO);   GPR(RS); IsZero(RD | RT | SA); }
D_OP(DSLLV)	{
    if (RD == RT) {
	Name(DSLLV);  GPR(RD); Sep(); GPR(RS); IsZero(SA);
    } else {
	Name(DSLLV);  GPR(RD); Sep(); GPR(RT); Sep(); GPR(RS); IsZero(SA);
    }
}
// hole at 010101
D_OP(DSRLV)	{
    if (RD == RT) {
	Name(DSRLV);  GPR(RD); Sep(); GPR(RS); IsZero(SA);
    } else {
	Name(DSRLV);  GPR(RD); Sep(); GPR(RT); Sep(); GPR(RS); IsZero(SA);
    }
}
D_OP(DSRAV)	{
    if (RD == RT) {
	Name(DSRAV);  GPR(RD); Sep(); GPR(RS); IsZero(SA);
    } else {
	Name(DSRAV);  GPR(RD); Sep(); GPR(RT); Sep(); GPR(RS); IsZero(SA);
    }
}

// 011000
D_OP(MULT)	{
    if (RD == RS) {
	Name(MULT);   GPR(RS); Sep(); GPR(RT); IsZero(SA);
    } else {
	Name(MULT);   GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA);
    }
}
D_OP(MULTU)	{
    if (RD == RS) {
	Name(MULTU);  GPR(RS); Sep(); GPR(RT); IsZero(SA);
    } else {
	Name(MULTU);  GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA);
    }
}
D_OP(DIV)	{ Name(DIV);    GPR(RS); Sep(); GPR(RT); IsZero(RD | SA); }
D_OP(DIVU)	{ Name(DIVU);   GPR(RS); Sep(); GPR(RT); IsZero(RD | SA); }
// hole at 011100
// hole at 011101
// hole at 011110
// hole at 011111

// 100000
D_OP(ADD)	{
    if (RD == RS) {
	Name(ADD);    GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else if (!RS) {
	Name(MOVE);   GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else {
	Name(ADD);    GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA);
    }
}
D_OP(ADDU)	{
    if (RD == RS) {
	Name(ADDU);   GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else if (!RS) {
	Name(MOVE);   GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else {
	Name(ADDU);   GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA);
    }
}
D_OP(SUB)	{
    if (RD == RS) {
	Name(SUB);    GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else if ((!RS) && (RD == RT)) {
	Name(NEG);    GPR(RD); IsZero(SA);
    } else if (!RS) {
	Name(NEG);    GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else {
	Name(SUB);    GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA);
    }
}
D_OP(SUBU)	{
    if (RD == RS) {
	Name(SUBU);   GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else if ((!RS) && (RD == RT)) {
	Name(NEG);    GPR(RD); IsZero(SA);
    } else if (!RS) {
	Name(NEG);    GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else {
	Name(SUBU);   GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA);
    }
}
D_OP(AND)	{
    if (RD == RS) {
	Name(AND);    GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else {
	Name(AND);    GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA);
    }
}
D_OP(OR)	{
    if (RD == RS) {
	Name(OR);     GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else {
	Name(OR);     GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA);
    }
}
D_OP(XOR)	{
    if (RD == RS) {
	Name(XOR);    GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else {
	Name(XOR);    GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA);
    }
}
D_OP(NOR)	{
    if (RD == RS) {
	Name(NOR);    GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else if ((!RS) && (RD == RT)) {
	Name(NOT);    GPR(RD); IsZero(SA);
    } else if (!RS) {
	Name(NOT);    GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else {
	Name(NOR);    GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA);
    }
}

// 101000
D_OP(MFSA)	{ Name(MFSA);   GPR(RD); IsZero(SA | RS | RT); }
D_OP(MTSA)	{ Name(MTSA);   GPR(RS); IsZero(SA | RD | RT); }
D_OP(SLT)	{
    if (RD == RS) {
	Name(SLT);    GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else {
	Name(SLT);    GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA);
    }
}
D_OP(SLTU)	{
    if (RD == RS) {
	Name(SLTU);   GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else {
	Name(SLTU);   GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA);
    }
}
D_OP(DADD)	{
    if (RD == RS) {
	Name(DADD);   GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else if (!RS) {
	Name(DMOVE);  GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else {
	Name(DADD);   GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA);
    }
}
D_OP(DADDU)	{
    if (RD == RS) {
	Name(DADDU);  GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else if (!RS) {
	Name(DMOVE);  GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else {
	Name(DADDU);  GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA);
    }
}
D_OP(DSUB)	{
    if (RD == RS) {
	Name(DSUB);   GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else if ((!RS) && (RD == RT)) {
	Name(DNEG);   GPR(RD); IsZero(SA);
    } else if (!RS) {
	Name(DNEG);   GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else {
	Name(DSUB);   GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA);
    }
}
D_OP(DSUBU)	{
    if (RD == RS) {
	Name(DSUBU);  GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else if ((!RS) && (RD == RT)) {
	Name(DNEG);   GPR(RD); IsZero(SA);
    } else if (!RS) {
	Name(DNEG);   GPR(RD); Sep(); GPR(RT); IsZero(SA);
    } else {
	Name(DSUBU);  GPR(RD); Sep(); GPR(RS); Sep(); GPR(RT); IsZero(SA);
    }
}

// 110000
D_OP(TGE)	{ Name(TGE);    GPR(RS); Sep(); GPR(RT); Trap(); }
D_OP(TGEU)	{ Name(TGEU);   GPR(RS); Sep(); GPR(RT); Trap(); }
D_OP(TLT)	{ Name(TLT);    GPR(RS); Sep(); GPR(RT); Trap(); }
D_OP(TLTU)	{ Name(TLTU);   GPR(RS); Sep(); GPR(RT); Trap(); }
D_OP(TEQ)	{ Name(TEQ);    GPR(RS); Sep(); GPR(RT); Trap(); }
// hole at 110101
D_OP(TNE)	{ Name(TNE);    GPR(RS); Sep(); GPR(RT); Trap(); }
// hole at 110111

// 111000
D_OP(DSLL)	{ Name(DSLL);   GPR(RD); Sep(); GPR(RT); Sep(); Sa(); IsZero(RS); }
// hole at 111001
D_OP(DSRL)	{ Name(DSRL);   GPR(RD); Sep(); GPR(RT); Sep(); Sa(); IsZero(RS); }
D_OP(DSRA)	{ Name(DSRA);   GPR(RD); Sep(); GPR(RT); Sep(); Sa(); IsZero(RS); }
D_OP(DSLL32)	{ Name(DSLL32); GPR(RD); Sep(); GPR(RT); Sep(); Sa(); IsZero(RS); }
// hole at 111101
D_OP(DSRL32)	{ Name(DSRL32); GPR(RD); Sep(); GPR(RT); Sep(); Sa(); IsZero(RS); }
D_OP(DSRA32)	{ Name(DSRA32); GPR(RD); Sep(); GPR(RT); Sep(); Sa(); IsZero(RS); }


/* RegIMM */

// 00000
D_OP(BLTZ)	{ Name(BLTZ);   GPR(RS); Sep(); OfS16(); }
D_OP(BGEZ)	{ Name(BGEZ);   GPR(RS); Sep(); OfS16(); }
D_OP(BLTZL)	{ Name(BLTZL);  GPR(RS); Sep(); OfS16(); }
D_OP(BGEZL)	{ Name(BGEZL);  GPR(RS); Sep(); OfS16(); }
// hole at 00100
// hole at 00101
// hole at 00110
// hole at 00111

// 01000
D_OP(TGEI)	{ Name(TGEI);   GPR(RS); Sep(); Imm(); }
D_OP(TGEIU)	{ Name(TGEIU);  GPR(RS); Sep(); Imm(); }
D_OP(TLTI)	{ Name(TLTI);   GPR(RS); Sep(); Imm(); }
D_OP(TLTIU)	{ Name(TLTIU);  GPR(RS); Sep(); Imm(); }
D_OP(TEQI)	{ Name(TEQI);   GPR(RS); Sep(); Imm(); }
// hole at 01101
D_OP(TNEI)	{ Name(TNEI);   GPR(RS); Sep(); Imm(); }
// hole at 01111

// 10000
D_OP(BLTZAL)	{ Name(BLTZAL); GPR(RS); Sep(); OfS16(); }
D_OP(BGEZAL)	{ Name(BGEZAL); GPR(RS); Sep(); OfS16(); }
D_OP(BLTZALL)	{ Name(BLTZALL); GPR(RS); Sep(); OfS16(); }
D_OP(BGEZALL)	{ Name(BGEZALL); GPR(RS); Sep(); OfS16(); }
// hole at 10100
// hole at 10101
// hole at 10110
// hole at 10111

// 11000
D_OP(MTSAB)	{ Name(MTSAB);  GPR(RS); Sep(); UImm(); }
D_OP(MTSAH)	{ Name(MTSAH);  GPR(RS); Sep(); UImm(); }
// hole at 11010
// hole at 11011
// hole at 11100
// hole at 11101
// hole at 11110
// hole at 11111


/* MMI */

D_OP(MADD)	UNDEF
D_OP(MADDU)	UNDEF
D_OP(PLZCW)	UNDEF
D_OP(MMI0)	UNDEF
D_OP(MMI2)	UNDEF
D_OP(MFHI1)	UNDEF
D_OP(MTHI1)	UNDEF
D_OP(MFLO1)	UNDEF
D_OP(MTLO1)	UNDEF
D_OP(MULT1)	UNDEF
D_OP(MULTU1)	UNDEF
D_OP(DIV1)	UNDEF
D_OP(DIVU1)	UNDEF
D_OP(MADD1)	UNDEF
D_OP(MADDU1)	UNDEF
D_OP(MMI1)	UNDEF
D_OP(MMI3)	UNDEF
D_OP(PMFHL)	UNDEF
D_OP(PMTHL)	UNDEF
D_OP(PSLLH)	UNDEF
D_OP(PSRLH)	UNDEF
D_OP(PSRAH)	UNDEF
D_OP(PSLLW)	UNDEF
D_OP(PSRLW)	UNDEF
D_OP(PSRAW)	UNDEF

/* MMI 0 */

D_OP(PADDW)	UNDEF
D_OP(PSUBW)	UNDEF
D_OP(PCGTW)	UNDEF
D_OP(PMAXW)	UNDEF
D_OP(PADDH)	UNDEF
D_OP(PSUBH)	UNDEF
D_OP(PCGTH)	UNDEF
D_OP(PMAXH)	UNDEF
D_OP(PADDB)	UNDEF
D_OP(PSUBB)	UNDEF
D_OP(PCGTB)	UNDEF
D_OP(PADDSW)	UNDEF
D_OP(PSUBSW)	UNDEF
D_OP(PEXTLW)	UNDEF
D_OP(PPACW)	UNDEF
D_OP(PADDSH)	UNDEF
D_OP(PSUBSH)	UNDEF
D_OP(PEXTLH)	UNDEF
D_OP(PPACH)	UNDEF
D_OP(PADDSB)	UNDEF
D_OP(PSUBSB)	UNDEF
D_OP(PEXTLB)	UNDEF
D_OP(PPACB)	UNDEF
D_OP(PEXT5)	UNDEF
D_OP(PPAC5)	UNDEF

/* MMI 1 */
D_OP(PABSW)	UNDEF
D_OP(PCEQW)	UNDEF
D_OP(PMINW)	UNDEF
D_OP(PADSBH)	UNDEF
D_OP(PABSH)	UNDEF
D_OP(PCEQH)	UNDEF
D_OP(PMINH)	UNDEF
D_OP(PCEQB)	UNDEF
D_OP(PADDUW)	UNDEF
D_OP(PSUBUW)	UNDEF
D_OP(PEXTUW)	UNDEF
D_OP(PADDUH)	UNDEF
D_OP(PSUBUH)	UNDEF
D_OP(PEXTUH)	UNDEF
D_OP(PADDUB)	UNDEF
D_OP(PSUBUB)	UNDEF
D_OP(PEXTUB)	UNDEF
D_OP(QFSRV)	UNDEF

/* MMI 2 */

D_OP(PMADDW)	UNDEF
D_OP(PSLLVW)	UNDEF
D_OP(PSRLVW)	UNDEF
D_OP(PMSUBW)	UNDEF
D_OP(PMFHI)	UNDEF
D_OP(PMFLO)	UNDEF
D_OP(PINTH)	UNDEF
D_OP(PMULTW)	UNDEF
D_OP(PDIVW)	UNDEF
D_OP(PCPYLD)	UNDEF
D_OP(PMADDH)	UNDEF
D_OP(PHMADH)	UNDEF
D_OP(PAND)	UNDEF
D_OP(PXOR)	UNDEF
D_OP(PMSUBH)	UNDEF
D_OP(PHMSBH)	UNDEF
D_OP(PEXEH)	UNDEF
D_OP(PREVH)	UNDEF
D_OP(PMULTH)	UNDEF
D_OP(PDIVBW)	UNDEF
D_OP(PEXEW)	UNDEF
D_OP(PROT3W)	UNDEF

/* MMI 3 */

D_OP(PMADDUW)	UNDEF
D_OP(PSRAVW)	UNDEF
D_OP(PMTHI)	UNDEF
D_OP(PMTLO)	UNDEF
D_OP(PINTEH)	UNDEF
D_OP(PMULTUW)	UNDEF
D_OP(PDIVUW)	UNDEF
D_OP(PCPYUD)	UNDEF
D_OP(POR)	UNDEF
D_OP(PNOR)	UNDEF
D_OP(PEXCH)	UNDEF
D_OP(PCPYH)	UNDEF
D_OP(PEXCW)	UNDEF

/* To complete with invalid fields */
/* COP0 */
D_OP(MFC0)	{ Name(MFC0);   GPR(RT); Sep(); C0R(RD); }
D_OP(MTC0)	{ Name(MTC0);   GPR(RT); Sep(); C0R(RD); }
D_OP(BC0)	{ bc0_table[RT](opcode, pc); }
D_OP(C0)	{ c0_table[FUNCTION](opcode, pc); }

/* BC0 */
D_OP(BC0F)	{ Name(BC0F);   OfS16(); }
D_OP(BC0T)	{ Name(BC0T);   OfS16(); }
D_OP(BC0FL)	{ Name(BC0FL);  OfS16(); }
D_OP(BC0TL)	{ Name(BC0TL);  OfS16(); }

/* C0 */
D_OP(TLBR)	{ Name(TLBR);  }
D_OP(TLBWI)	{ Name(TLBWI); }
D_OP(TLBWR)	{ Name(TLBWR); }
D_OP(TLBP)	{ Name(TLBP);  }
D_OP(ERET)	{ Name(ERET);  }
D_OP(EI)	{ Name(EI);    }
D_OP(DI)	{ Name(DI);    }

/* COP1 */
D_OP(MFC1)	UNDEF
D_OP(CFC1)	UNDEF
D_OP(MTC1)	UNDEF
D_OP(CTC1)	UNDEF
D_OP(BC1)	UNDEF
D_OP(S)	UNDEF
D_OP(W)	UNDEF

/* BC1 */
D_OP(BC1F)	UNDEF
D_OP(BC1T)	UNDEF
D_OP(BC1FL)	UNDEF
D_OP(BC1TL)	UNDEF

/* S */
D_OP(FADD)	UNDEF
D_OP(FSUB)	UNDEF
D_OP(FMUL)	UNDEF
D_OP(FDIV)	UNDEF
D_OP(FSQRT)	UNDEF
D_OP(FABS)	UNDEF
D_OP(FMOV)	UNDEF
D_OP(FNEG)	UNDEF
D_OP(FRSQRT)	UNDEF
D_OP(FADDA)	UNDEF
D_OP(FSUBA)	UNDEF
D_OP(FMULA)	UNDEF
D_OP(FMADD)	UNDEF
D_OP(FMSUB)	UNDEF
D_OP(FMADDA)	UNDEF
D_OP(FMSUBA)	UNDEF
D_OP(FMAX)	UNDEF
D_OP(FMIN)	UNDEF
D_OP(FCF)	UNDEF
D_OP(FCEQ)	UNDEF
D_OP(FCLT)	UNDEF
D_OP(FCLE)	UNDEF
D_OP(CVTW)	UNDEF

/* W */
D_OP(CVTS)	UNDEF


/* The various tables */

static decode_op opcode_table[64] = {
   SPECIAL, REGIMM, J,    JAL,   BEQ,  BNE,  BLEZ,  BGTZ,
   ADDI,    ADDIU,  SLTI, SLTIU, ANDI, ORI,  XORI,  LUI,
   COP0,    COP1,   COP2, UNK,   BEQL, BNEL, BLEZL, BGTZL,
   DADDI,   DADDIU, LDL,  LDR,   MMI,  UNK,  LQ,    SQ,
   LB,      LH,     LWL,  LW,    LBU,  LHU,  LWR,   LWU,
   SB,      SH,     SWL,  SW,    SDL,  SDR,  SWR,   CACHE,
   UNK,     LWC1,   UNK,  PREF,  UNK,  UNK,  LQC2,  LD,
   UNK,     SWC1,   UNK,  UNK,   UNK,  UNK,  SQC2,  SD
};

static decode_op special_table[64] = {
   SLL,  UNK,   SRL,  SRA,  SLLV,    UNK,   SRLV,   SRAV,
   JR,   JALR,  MOVZ, MOVN, SYSCALL, BREAK, UNK,    SYNC,
   MFHI, MTHI,  MFLO, MTLO, DSLLV,   UNK,   DSRLV,  DSRAV,
   MULT, MULTU, DIV,  DIVU, UNK,     UNK,   UNK,    UNK,
   ADD,  ADDU,  SUB,  SUBU, AND,     OR,    XOR,    NOR,
   MFSA, MTSA,  SLT,  SLTU, DADD,    DADDU, DSUB,   DSUBU,
   TGE,  TGEU,  TLT,  TLTU, TEQ,     UNK,   TNE,    UNK,
   DSLL, UNK,   DSRL, DSRA, DSLL32,  UNK,   DSRL32, DSRA32
};

static decode_op regimm_table[32] = {
   BLTZ,   BGEZ,   BLTZL,   BGEZL,   UNK,  UNK, UNK,  UNK,
   TGEI,   TGEIU,  TLTI,    TLTIU,   TEQI, UNK, TNEI, UNK,
   BLTZAL, BGEZAL, BLTZALL, BGEZALL, UNK,  UNK, UNK,  UNK,
   MTSAB,  MTSAH,  UNK,     UNK,     UNK,  UNK, UNK,  UNK
};

static decode_op mmi_table[64] = {
   MADD,  MADDU,  UNK,   UNK,   PLZCW, UNK, UNK,   UNK,
   MMI0,  MMI2,   UNK,   UNK,   UNK,   UNK, UNK,   UNK,
   MFHI1, MTHI1,  MFLO1, MTLO1, UNK,   UNK, UNK,   UNK,
   MULT1, MULTU1, DIV1,  DIVU1, UNK,   UNK, UNK,   UNK,
   MADD1, MADDU1, UNK,   UNK,   UNK,   UNK, UNK,   UNK,
   MMI1,  MMI3,   UNK,   UNK,   UNK,   UNK, UNK,   UNK,
   PMFHL, PMTHL,  UNK,   UNK,   PSLLH, UNK, PSRLH, PSRAH,
   UNK,   UNK,    UNK,   UNK,   PSLLW, UNK, PSRLW, PSRAW
};

static decode_op mmi0_table[32] = {
   PADDW,  PSUBW,  PCGTW,  PMAXW,
   PADDH,  PSUBH,  PCGTH,  PMAXH,
   PADDB,  PSUBB,  PCGTB,  UNK,
   UNK,    UNK,    UNK,    UNK,
   PADDSW, PSUBSW, PEXTLW, PPACW,
   PADDSH, PSUBSH, PEXTLH, PPACH,
   PADDSB, PSUBSB, PEXTLB, PPACB,
   UNK,    UNK,    PEXT5,  PPAC5
};

static decode_op mmi1_table[32] = {
   UNK,    PABSW,  PCEQW,  PMINW,
   PADSBH, PABSH,  PCEQH,  PMINH,
   UNK,    UNK,    PCEQB,  UNK,
   UNK,    UNK,    UNK,    UNK,
   PADDUW, PSUBUW, PEXTUW, UNK,
   PADDUH, PSUBUH, PEXTUH, UNK,
   PADDUB, PSUBUB, PEXTUB, QFSRV,
   UNK,    UNK,    UNK,    UNK
};

static decode_op mmi2_table[32] = {
   PMADDW, UNK,    PSLLVW, PSRLVW,
   PMSUBW, UNK,    UNK,    UNK,
   PMFHI,  PMFLO,  PINTH,  UNK,
   PMULTW, PDIVW,  PCPYLD, UNK,
   PMADDH, PHMADH, PAND,   PXOR, 
   PMSUBH, PHMSBH, UNK,    UNK,
   UNK,    UNK,    PEXEH,  PREVH,
   PMULTH, PDIVBW, PEXEW,  PROT3W
};

static decode_op mmi3_table[32] = {
   PMADDUW, UNK,    UNK,    PSRAVW,
   UNK,     UNK,    UNK,    UNK,
   PMTHI,   PMTLO,  PINTEH, UNK,
   PMULTUW, PDIVUW, PCPYUD, UNK,
   UNK,     UNK,    POR,    PNOR,
   UNK,     UNK,    UNK,    UNK,
   UNK,     UNK,    PEXCH,  PCPYH,
   UNK,     UNK,    PEXCW,  UNK
};

static decode_op cop0_table[32] = {
   MFC0, UNK, UNK, UNK, MTC0, UNK, UNK, UNK,
   BC0,  UNK, UNK, UNK, UNK,  UNK, UNK, UNK,
   C0,   UNK, UNK, UNK, UNK,  UNK, UNK, UNK,
   UNK,  UNK, UNK, UNK, UNK,  UNK, UNK, UNK
};

static decode_op bc0_table[32] = {
   BC0F, BC0T, BC0FL, BC0TL, UNK, UNK, UNK, UNK,
   UNK,  UNK,  UNK,   UNK,   UNK, UNK, UNK, UNK,
   UNK,  UNK,  UNK,   UNK,   UNK, UNK, UNK, UNK,
   UNK,  UNK,  UNK,   UNK,   UNK, UNK, UNK, UNK
};

static decode_op c0_table[64] = {
   UNK,  TLBR, TLBWI, UNK, UNK, UNK, TLBWR, UNK,
   TLBP, UNK,  UNK,   UNK, UNK, UNK, UNK,   UNK,
   UNK,  UNK,  UNK,   UNK, UNK, UNK, UNK,   UNK,
   ERET, UNK,  UNK,   UNK, UNK, UNK, UNK,   UNK,
   UNK,  UNK,  UNK,   UNK, UNK, UNK, UNK,   UNK,
   UNK,  UNK,  UNK,   UNK, UNK, UNK, UNK,   UNK,
   UNK,  UNK,  UNK,   UNK, UNK, UNK, UNK,   UNK,
   EI,   DI,   UNK,   UNK, UNK, UNK, UNK,   UNK
};

static decode_op cop1_table[32] = {
   MFC1, UNK, CFC1, UNK, MTC1, UNK, CTC1, UNK,
   BC1,  UNK, UNK,  UNK, UNK,  UNK, UNK,  UNK,
   S,    UNK, UNK,  UNK, W,    UNK, UNK,  UNK,
   UNK,  UNK, UNK,  UNK, UNK,  UNK, UNK,  UNK
};

static decode_op cop2_table[32] = {
   UNK, UNK, UNK, UNK, UNK, UNK, UNK, UNK,
   UNK, UNK, UNK, UNK, UNK, UNK, UNK, UNK,
   UNK, UNK, UNK, UNK, UNK, UNK, UNK, UNK,
   UNK, UNK, UNK, UNK, UNK, UNK, UNK, UNK,
};

static decode_op bc1_table[32] = {
   BC1F, BC1T, BC1FL, BC1TL, UNK, UNK, UNK, UNK,
   UNK,  UNK,  UNK,   UNK,   UNK, UNK, UNK, UNK,
   UNK,  UNK,  UNK,   UNK,   UNK, UNK, UNK, UNK,
   UNK,  UNK,  UNK,   UNK,   UNK, UNK, UNK, UNK
};

static decode_op s_table[64] = {
   FADD,  FSUB,  FMUL,  FDIV, FSQRT, FABS,  FMOV,   FNEG,
   UNK,   UNK,   UNK,   UNK,  UNK,   UNK,   UNK,    UNK,
   UNK,   UNK,   UNK,   UNK,  UNK,   UNK,   FRSQRT, UNK,
   FADDA, FSUBA, FMULA, UNK,  FMADD, FMSUB, FMADDA, FMSUBA,
   UNK,   UNK,   UNK,   UNK,  CVTW,  UNK,   UNK,    UNK,
   FMAX,  FMIN,  UNK,   UNK,  UNK,   UNK,   UNK,    UNK,
   FCF,   UNK,   FCEQ,  UNK,  FCLT,  UNK,   FCLE,   UNK,
   UNK,   UNK,   UNK,   UNK,  UNK,   UNK,   UNK,    UNK
};

static decode_op w_table[64] = {
   UNK,  UNK, UNK, UNK, UNK, UNK, UNK, UNK,
   UNK,  UNK, UNK, UNK, UNK, UNK, UNK, UNK,
   UNK,  UNK, UNK, UNK, UNK, UNK, UNK, UNK,
   UNK,  UNK, UNK, UNK, UNK, UNK, UNK, UNK,
   CVTS, UNK, UNK, UNK, UNK, UNK, UNK, UNK,
   UNK,  UNK, UNK, UNK, UNK, UNK, UNK, UNK,
   UNK,  UNK, UNK, UNK, UNK, UNK, UNK, UNK,
   UNK,  UNK, UNK, UNK, UNK, UNK, UNK, UNK
};

static void disasmR5900(u32 opcode, u32 pc)
{
	printf("%08x - %08x : ", pc, opcode);	
	opcode_table[opcode >> 26](opcode, pc);
	printf("\n");
}

void disasm(const u32 *addr, int num)
{
	int i;

	for(i = 0; i < num; i++)
	{
		disasmR5900(*addr, (u32) addr);
		addr += 1;
	}
}

void disasm_verbose(const struct ee_regs* regs, const u32* addr)
{
    disasmR5900(*addr, (u32) addr);
}
