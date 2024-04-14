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

#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

int checkregs(CPU_state *ref_r, vaddr_t pc) {
  for (int i = 0; i < 32; ++i){
    if (ref_r->gpr[i] != gpr(i)){
      printf("i :: %d\n", i);
      return false;
    }
  }
  if (ref_r->pc != cpu.pc)
    return false;
  return true;
}

void diff_print_regs(CPU_state *ref_r, vaddr_t pc) {
  printf("reg_name  &  npc_reg  &  nemu_reg\n");
  for (int i = 0; i < 32; ++i){
    printf("%6s     0x%08x 0x%08x\n", regs[i], cpu.gpr[i], ref_r->gpr[i]);
  }
  printf("%6s     0x%08x 0x%08x\n", "pc", cpu.pc, pc);
}

void isa_reg_display() {
  for (int i = 0; i < 32; ++i){
    printf("%6s     0x%08x\n", regs[i], cpu.gpr[i]);
  }
  printf("%6s     0x%08x\n", "pc", cpu.pc);
}

word_t isa_reg_str2val(const char *s, bool *success) {
  s = s + 1;
  for (int i = 0; i < 32; ++i){
    if (strcmp(regs[i], s) == 0){
      return cpu.gpr[i];
    }
  }
  if (strcmp(s, "pc") == 0)
    return cpu.pc;
  assert(0);
  return 0;
}