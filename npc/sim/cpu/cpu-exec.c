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

#include <cpu/cpu.h>
#include <cpu/decode.h>
#include <locale.h>
#include <cpu/ifetch.h>
#include <verilated.h>  
#include <verilated_vcd_c.h> 
#include "VTOP.h"

VerilatedContext *contextp = NULL;
VerilatedVcdC *tfp = NULL;

VTOP top ;

/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INST_TO_PRINT 300
//#define MAX_INST_TO_PRINT 10

CPU_state cpu = {};
uint64_t g_nr_guest_inst = 0;
static uint64_t g_timer = 0; // unit: us
static bool g_print_step = false;
static bool halt = false;

void device_update();
void watchpoint();
void iringbuf_display();

static void single_cycle() {
    top.clock = 0;
    top.eval();
    top.clock = 1;
    top.eval();
}

static void trace_and_difftest(Decode *_this, vaddr_t dnpc) {
#ifdef CONFIG_ITRACE_COND
  if (ITRACE_COND) { log_write("%s\n", _this->logbuf); }
#endif
  if (g_print_step) { IFDEF(CONFIG_ITRACE, puts(_this->logbuf)); }
  IFDEF(CONFIG_DIFFTEST, difftest_step(_this->pc, dnpc));
  IFDEF(CONFIG_WATCHPOINT, watchpoint());
}

static void exec_once(Decode *s, vaddr_t pc) {
  s->pc = pc;
  s->snpc = pc;
  top.io_inst = vaddr_read(pc, 4);
  s->isa.inst.val = top.io_inst;
  top.eval();
  Log("%x %08x", s->pc, s->isa.inst.val);
  Log("rs1: %d", top.io_rs1);
  Log("rs2: %d", top.io_rs2);
  Log("rd : %d",  top.io_rd);
  Log("data1 : %08x",  top.io_data1);
  Log("data2 : %08x",  top.io_data2);
  Log("res: %08x", top.io_res);
  Log("isStore: %s", top.io_Store ? "true" : "false");
  if (top.io_Store){
    Log("wdata: %08x", top.io_data_sram_wdata);
    Log("waddr: %08x", top.io_data_sram_addr);
  }
  if (top.io_Store == true) {
    vaddr_write(top.io_data_sram_addr, top.io_SL_len, top.io_data_sram_wdata);
  }
  if (top.io_Load  == true) {
    top.io_data_sram_rdata = vaddr_read(top.io_data_sram_addr, top.io_SL_len);
  }

  single_cycle();
  // s->dnpc = top.io_pc;

  halt = top.io_halt;
  if (halt) NPCTRAP(s->pc, 0);

// #ifdef CONFIG_ITRACE
//   char *p = s->logbuf;
//   p += snprintf(p, sizeof(s->logbuf), FMT_WORD ":", s->pc);
//   int ilen = s->snpc - s->pc;
//   int i;
//   uint8_t *inst = (uint8_t *)&s->isa.inst.val;
//   for (i = ilen - 1; i >= 0; i --) {
//     p += snprintf(p, 4, " %02x", inst[i]);
//   }
//   int ilen_max = MUXDEF(CONFIG_ISA_x86, 8, 4);
//   int space_len = ilen_max - ilen;
//   if (space_len < 0) space_len = 0;
//   space_len = space_len * 3 + 1;
//   memset(p, ' ', space_len);
//   p += space_len;

// #ifndef CONFIG_ISA_loongarch32r
//   void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
//   disassemble(p, s->logbuf + sizeof(s->logbuf) - p,
//       MUXDEF(CONFIG_ISA_x86, s->snpc, s->pc), (uint8_t *)&s->isa.inst.val, ilen);
// #else
//   p[0] = '\0'; // the upstream llvm does not support loongarch32r
// #endif
// #endif

// #ifdef CONFIG_IRINGBUF
//   void iringbuf_update(word_t pc, uint32_t inst);
//   iringbuf_update(s->pc, s->isa.inst.val);
// #endif
}

static void execute(uint64_t n) {
  Decode s;
  for (;n > 0; n --) {
    exec_once(&s, top.io_pc);
    if (halt) return;
    g_nr_guest_inst ++;
    trace_and_difftest(&s, cpu.pc);
    if (npc_state.state != NPC_RUNNING) break;
    IFDEF(CONFIG_DEVICE, device_update());
  }
}

static void statistic() {
  IFNDEF(CONFIG_TARGET_AM, setlocale(LC_NUMERIC, ""));
#define NUMBERIC_FMT MUXDEF(CONFIG_TARGET_AM, "%", "%'") PRIu64
  Log("host time spent = " NUMBERIC_FMT " us", g_timer);
  Log("total guest instructions = " NUMBERIC_FMT, g_nr_guest_inst);
  if (g_timer > 0) Log("simulation frequency = " NUMBERIC_FMT " inst/s", g_nr_guest_inst * 1000000 / g_timer);
  else Log("Finish running in less than 1 us and can not calculate the simulation frequency");
}

void assert_fail_msg() {
  isa_reg_display();
  statistic();
}

/* Simulate how the CPU works. */
void cpu_exec(uint64_t n) {
  g_print_step = (n < MAX_INST_TO_PRINT);
  switch (npc_state.state) {
    case NPC_END: case NPC_ABORT:
      printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
      return;
    default: npc_state.state = NPC_RUNNING;
  }

  uint64_t timer_start = get_time();

  execute(n);

  uint64_t timer_end = get_time();
  g_timer += timer_end - timer_start;

  switch (npc_state.state) {
    case NPC_RUNNING: npc_state.state = NPC_STOP; break;

    case NPC_END: case NPC_ABORT:
      Log("npc: %s at pc = " FMT_WORD,
          (npc_state.state == NPC_ABORT ? ANSI_FMT("ABORT", ANSI_FG_RED) :
           (npc_state.halt_ret == 0 ? ANSI_FMT("HIT GOOD TRAP", ANSI_FG_GREEN) :
            ANSI_FMT("HIT BAD TRAP", ANSI_FG_RED))),
          npc_state.halt_pc);
      if (npc_state.state == NPC_ABORT || npc_state.halt_ret != 0){
        #ifdef CONFIG_IRINGBUF_COND
          if (IRINGBUF_COND) iringbuf_display();
        #endif
      }
      // fall through
    case NPC_QUIT: statistic();
  }
}
