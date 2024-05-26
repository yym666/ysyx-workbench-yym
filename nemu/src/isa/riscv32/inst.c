/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include "local-include/reg.h"
#include <cpu/cpu.h>
#include <cpu/ifetch.h>
#include <cpu/decode.h>

#define R(i) gpr(i)
#define Mr vaddr_read
#define Mw vaddr_write

enum {
  TYPE_I, TYPE_U, TYPE_S,
  TYPE_J, TYPE_B, TYPE_R,
  TYPE_N, TYPE_C,  // none
};

#define MRET() { \
  s->dnpc = CSR(0x341); \
  cpu.csr.mstatus &= ~(1<<3); \
  cpu.csr.mstatus |= ((cpu.csr.mstatus&(1<<7))>>4); \
  cpu.csr.mstatus |= (1<<7); \
  cpu.csr.mstatus &= ~((1<<11)+(1<<12)); \
}

static vaddr_t *csr_register(word_t imm) {
  switch (imm)
  {
  case 0x341: return &(cpu.csr.mepc);
  case 0x342: return &(cpu.csr.mcause);
  case 0x300: return &(cpu.csr.mstatus);
  case 0x305: return &(cpu.csr.mtvec);
  default: panic("Unknown csr");
  }
}

#define ECALL(dnpc) {dnpc = (isa_raise_intr(11, s->pc)); }
#define CSR(i)  *csr_register(i)

#define src1R() do { *src1 = R(rs1); } while (0)
#define src2R() do { *src2 = R(rs2); } while (0)
#define immI() do { *imm = SEXT(BITS(i, 31, 20), 12); } while(0)
#define immC() do { *imm = BITS(i, 31, 20); } while(0)
#define immU() do { *imm = SEXT(BITS(i, 31, 12), 20) << 12; } while(0)
#define immS() do { *imm = (SEXT(BITS(i, 31, 25), 7) << 5) | BITS(i, 11, 7); } while(0)
#define immJ() do { *imm = (SEXT((BITS(i, 31, 31) << 19) | \
                                 (BITS(i, 19, 12) << 11) | \
                                 (BITS(i, 20, 20) << 10) | \
                                 (BITS(i, 30, 21)), 20) << 1); } while(0)
#define immB() do { *imm = (SEXT((BITS(i, 31, 31) << 11) | \
                                 (BITS(i,  7,  7) << 10) | \
                                 (BITS(i, 30, 25) <<  4) | \
                                 (BITS(i, 11,  8)), 12) << 1); } while(0)

static void decode_operand(Decode *s, int *rs1forcsr, int *rd, word_t *src1, word_t *src2, word_t *imm, int type) {
  uint32_t i = s->isa.inst.val;
  int rs1 = BITS(i, 19, 15);
  int rs2 = BITS(i, 24, 20);
  *rs1forcsr = rs1;
  *rd     = BITS(i, 11, 7);
  switch (type) {
    case TYPE_I: src1R();          immI(); break;
    case TYPE_C: src1R();          immC(); break;
    case TYPE_U:                   immU(); break;
    case TYPE_S: src1R(); src2R(); immS(); break;
    case TYPE_J:                   immJ(); break; 
    case TYPE_B: src1R(); src2R(); immB(); break;
    case TYPE_R: src1R(); src2R();         break;
  }
  //difftest
  // int d1, d2, d3;
  // switch (type) {
  //   case TYPE_I: src1R();          immI(); d1 = *src1, d2 = *imm; break;
  //   case TYPE_U:                   immU(); d1 = s->pc, d2 = *imm; break;
  //   case TYPE_S: src1R(); src2R(); immS(); d1 = *src1, d2 = *src2, d3 = *imm; break;
  //   case TYPE_J:                   immJ(); d1 = s->pc, d2 = *imm;  break; 
  //   case TYPE_B: src1R(); src2R(); immB(); d1 = *src1, d2 = *src2, d3 = *imm; break;
  //   case TYPE_R: src1R(); src2R();         d1 = *src1, d2 = *src2; break;
  // }
  // printf("rs1 = %d\nrs2 = %d\nrd = %d\n", rs1, rs2, *rd);
  // printf("d1 = %08x\nd2 = %08x\nd3 = %08x\n", d1, d2, d3);
}

void ftrace_jal(Decode *s, word_t rd){
  #ifdef CONFIG_FTRACE_COND
    void ftrace_call(vaddr_t pc, vaddr_t dnpc);
    if (rd == 1)
      ftrace_call(s->pc, s->dnpc);
  #endif
}

void ftrace_jalr(Decode *s, word_t rd, word_t imm){
  #ifdef CONFIG_FTRACE_COND
    void ftrace_call(vaddr_t pc, vaddr_t dnpc);
    void ftrace_ret(vaddr_t pc);
    if (s->isa.inst.val == 0x00008067)
      ftrace_ret(s->pc);
    else if (rd == 1)
      ftrace_call(s->pc, s->dnpc);
    else if (rd == 0 && imm == 0)
      ftrace_call(s->pc, s->dnpc);
  #endif
}

static int decode_exec(Decode *s) {
  int rd = 0, rs1forcsr = 0;
  word_t src1 = 0, src2 = 0, imm = 0, t = 0;
  s->dnpc = s->snpc;

#define INSTPAT_INST(s) ((s)->isa.inst.val)
#define INSTPAT_MATCH(s, name, type, ... /* execute body */ ) { \
  decode_operand(s, &rs1forcsr, &rd, &src1, &src2, &imm, concat(TYPE_, type)); \
  __VA_ARGS__ ; \
}

  INSTPAT_START();
  INSTPAT("??????? ????? ????? ??? ????? 01101 11", lui    , U, R(rd) = imm);
  INSTPAT("??????? ????? ????? ??? ????? 00101 11", auipc  , U, R(rd) = s->pc + imm);

  INSTPAT("??????? ????? ????? ??? ????? 1101111" , jal    , J, s->dnpc = s->pc + imm; ftrace_jal(s, rd); R(rd) = s->pc + 4);
  INSTPAT("??????? ????? ????? 000 ????? 1100111" , jalr   , I, t = s->pc + 4; s->dnpc = (src1 + imm)&(~(word_t)1); ftrace_jalr(s, rd, imm); R(rd) = t);

  INSTPAT("0000000 ????? ????? 000 ????? 01100 11", add    , R, R(rd) = src1 + src2);
  INSTPAT("0100000 ????? ????? 000 ????? 01100 11", sub    , R, R(rd) = src1 - src2);
  INSTPAT("0000000 ????? ????? 111 ????? 01100 11", and    , R, R(rd) = src1 & src2);
  INSTPAT("0000000 ????? ????? 110 ????? 01100 11", or     , R, R(rd) = src1 | src2);
  INSTPAT("0000001 ????? ????? 110 ????? 01100 11", rem    , R, R(rd) = (sword_t)src1 % (sword_t)src2);
  INSTPAT("0000001 ????? ????? 111 ????? 01100 11", remu   , R, R(rd) = src1 % src2);
  INSTPAT("0000000 ????? ????? 010 ????? 01100 11", slt    , R, R(rd) = (sword_t)src1 < (sword_t)src2);
  INSTPAT("0000000 ????? ????? 011 ????? 01100 11", sltu   , R, R(rd) = src1 < src2);
  INSTPAT("0000000 ????? ????? 100 ????? 01100 11", xor    , R, R(rd) = src1 ^ src2);
  INSTPAT("0000001 ????? ????? 100 ????? 01100 11", div    , R, R(rd) = (sword_t)src1 / (sword_t)src2);
  INSTPAT("0000001 ????? ????? 101 ????? 01100 11", divu   , R, R(rd) = src1 / src2);
  INSTPAT("0000001 ????? ????? 000 ????? 01100 11", mul    , R, R(rd) = src1 * src2);
  INSTPAT("0000001 ????? ????? 001 ????? 01100 11", mulh   , R, R(rd) = ((int64_t)(sword_t)src1 * (int64_t)(sword_t)src2) >> 32);
  INSTPAT("0000001 ????? ????? 011 ????? 01100 11", mulhu  , R, R(rd) = ((int64_t)src1 * (int64_t)src2) >> 32);
  INSTPAT("0100000 ????? ????? 101 ????? 01100 11", sra    , R, R(rd) = (sword_t)src1 >> BITS(src2, 4, 0));
  INSTPAT("0000000 ????? ????? 101 ????? 01100 11", srl    , R, R(rd) = src1 >> BITS(src2, 4, 0));
  INSTPAT("0000000 ????? ????? 001 ????? 01100 11", sll    , R, R(rd) = src1 << BITS(src2, 4, 0));

  INSTPAT("??????? ????? ????? 000 ????? 00100 11", addi   , I, R(rd) = src1 + imm);
  INSTPAT("??????? ????? ????? 111 ????? 00100 11", andi   , I, R(rd) = src1 & imm);
  INSTPAT("??????? ????? ????? 100 ????? 00100 11", xori   , I, R(rd) = src1 ^ imm);
  INSTPAT("??????? ????? ????? 110 ????? 00100 11", ori    , I, R(rd) = src1 | imm);
  INSTPAT("??????? ????? ????? 011 ????? 00100 11", sltui  , I, R(rd) = src1 < imm);
  INSTPAT("??????? ????? ????? 010 ????? 00100 11", slti   , I, R(rd) = (sword_t)src1 < (sword_t)imm);
  INSTPAT("000000 ?????? ????? 001 ????? 00100 11", slli   , I, R(rd) = src1 << BITS(imm, 5, 0));
  INSTPAT("000000 ?????? ????? 101 ????? 00100 11", srli   , I, R(rd) = src1 >> BITS(imm, 5, 0));
  INSTPAT("010000 ?????? ????? 101 ????? 00100 11", srai   , I, R(rd) = (sword_t)src1 >> BITS(imm, 5, 0));

  INSTPAT("??????? ????? ????? 000 ????? 00000 11", lb     , I, R(rd) = SEXT(Mr(src1 + imm, 1),  8));
  INSTPAT("??????? ????? ????? 001 ????? 00000 11", lh     , I, R(rd) = SEXT(Mr(src1 + imm, 2), 16));
  INSTPAT("??????? ????? ????? 010 ????? 00000 11", lw     , I, R(rd) = SEXT(Mr(src1 + imm, 4), 32));
  INSTPAT("??????? ????? ????? 100 ????? 00000 11", lbu    , I, R(rd) = Mr(src1 + imm, 1));
  INSTPAT("??????? ????? ????? 101 ????? 00000 11", lhu    , I, R(rd) = Mr(src1 + imm, 2));
  INSTPAT("??????? ????? ????? 010 ????? 00000 11", lwu    , I, R(rd) = Mr(src1 + imm, 4));

  INSTPAT("??????? ????? ????? 000 ????? 01000 11", sb     , S, Mw(src1 + imm, 1, src2));
  INSTPAT("??????? ????? ????? 001 ????? 01000 11", sh     , S, Mw(src1 + imm, 2, src2));
  INSTPAT("??????? ????? ????? 010 ????? 01000 11", sw     , S, Mw(src1 + imm, 4, src2));

  INSTPAT("??????? ????? ????? 000 ????? 11000 11", beq    , B, s->dnpc = (src1 == src2) ? (s->pc + imm) : s->snpc); //careful
  INSTPAT("??????? ????? ????? 001 ????? 11000 11", bne    , B, s->dnpc = (src1 != src2) ? (s->pc + imm) : s->snpc);
  INSTPAT("??????? ????? ????? 100 ????? 11000 11", blt    , B, s->dnpc = ((sword_t)src1 < (sword_t)src2) ? (s->pc + imm) : s->snpc);
  INSTPAT("??????? ????? ????? 110 ????? 11000 11", bltu   , B, s->dnpc = (src1 < src2) ? (s->pc + imm) : s->snpc);
  INSTPAT("??????? ????? ????? 101 ????? 11000 11", bge    , B, s->dnpc = ((sword_t)src1 >= (sword_t)src2) ? (s->pc + imm) : s->snpc);
  INSTPAT("??????? ????? ????? 111 ????? 11000 11", bgeu   , B, s->dnpc = (src1 >= src2) ? (s->pc + imm) : s->snpc);


  INSTPAT("??????? ????? ????? 001 ????? 11100 11", csrrw  , C, printf("src1=%x rd = %x\n", rs1forcsr, rd); if(rd != 0) {R(rd) = CSR(imm);} CSR(imm) = src1 );
  INSTPAT("??????? ????? ????? 010 ????? 11100 11", csrrs  , C, R(rd) = CSR(imm); if(rs1forcsr != 0) {CSR(imm) |= src1;} );
  INSTPAT("0000000 00000 00000 000 00000 11100 11", ecall  , N, ECALL(s->dnpc));
  INSTPAT("0011000 00010 00000 000 00000 11100 11", mret   , N, MRET());

  INSTPAT("0000000 00001 00000 000 00000 11100 11", ebreak , N, NEMUTRAP(s->pc, R(10))); // R(10) is $a0
  INSTPAT("??????? ????? ????? ??? ????? ????? ??", inv    , N, INV(s->pc));


  INSTPAT_END();

  R(0) = 0; // reset $zero to 0
  return 0;
}

int isa_exec_once(Decode *s) {
  s->isa.inst.val = inst_fetch(&s->snpc, 4);
  return decode_exec(s);
}
