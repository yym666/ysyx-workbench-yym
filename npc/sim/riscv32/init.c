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
#include <memory/paddr.h>
#include "VysyxSoCFull___024root.h" 
#include "VysyxSoCFull.h"
#include <vcd.h>


// this is not consistent with uint8_t
// but it is ok since we do not access the array directly
static const uint32_t img [] = {
  0x00000297,  // auipc t0,0
  0x00028823,  // sb  zero,16(t0)
  0x0102c503,  // lbu a0,16(t0)
  0x00100073,  // ebreak (used as npc_trap)
  0xdeadbeef,  // some data
};

static const uint32_t flash_test [] = {
  0xffffffff,  // auipc t0,0
  0xffffffff,  // sb  zero,16(t0)
  0xffffffff,  // lbu a0,16(t0)
  0xffffffff,  // ebreak (used as npc_trap)
  0xffffffff,  // some data
};

static void restart() {
  /* Set the initial program counter. */
  cpu.pc = RESET_VECTOR;
  top->reset = 1;
  for (int i = 1; i <= 12; ++i){
  step_and_dump_wave();
  top->clock = 0;
  step_and_dump_wave();
  top->clock = 1;
  }
  // step_and_dump_wave();
  // top->clock = 0;
  // step_and_dump_wave();
  // top->clock = 1;
  // step_and_dump_wave();
  // top->clock = 0;
  // step_and_dump_wave();
  // top->clock = 1;
  // step_and_dump_wave();
  // top->clock = 0;
  // step_and_dump_wave();
  // top->clock = 1;
  step_and_dump_wave();
  top->reset = 0;
  // step_and_dump_wave();
  /* The zero register is always 0. */
  cpu.gpr[0] = 0;
  cpu.csr.mstatus = 0x00001800;
  //q top->rootp->TOP__DOT__CSR__DOT__csrs_ext__DOT__Memory[1] = 0x00001800;
//gpr = general purpose register
}

void init_isa() {
  /* Load built-in image. */
  memcpy(guest_to_host(RESET_VECTOR), img, sizeof(img));

  /* Initialize this virtual computer system. */
  restart();
}
