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
#include "VysyxSoCFull___024root.h" 
#include "VysyxSoCFull.h"
#include "disasm.h"
#include <vcd.h>
#include "VysyxSoCFull__Dpi.h"
#include "svdpi.h"

#define SERIAL_PORT 0xa00003f8
#define RTC_ADDR    0xa0000048

#define TOPPC  top->rootp->ysyxSoCFull__DOT__asic__DOT__cpu__DOT__cpu__DOT__CORE__DOT__IFU__DOT__pc_reg
#define TOPREG top->rootp->ysyxSoCFull__DOT__asic__DOT__cpu__DOT__cpu__DOT__CORE__DOT__GPR__DOT__regs_ext__DOT__Memory

#define SEXT(x, len) ({ struct { int64_t n : len; } __x = { .n = x }; (uint64_t)__x.n; })


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

void check_halt(svBit flag){
  halt = flag;
}

static void single_cycle() {
    top->clock = 0;
    step_and_dump_wave();
    top->clock = 1;
    step_and_dump_wave();
}
void difftest_step(vaddr_t pc, vaddr_t npc);
void difftest_skip_ref();

static void trace_and_difftest(Decode *_this, vaddr_t dnpc) {
#ifdef CONFIG_ITRACE_COND
  if (ITRACE_COND) { log_write("%s\n", _this->logbuf); }
#endif
  if (g_print_step) { IFDEF(CONFIG_ITRACE, puts(_this->logbuf)); }
  IFDEF(CONFIG_DIFFTEST, difftest_step(_this->pc, dnpc));
  IFDEF(CONFIG_WATCHPOINT, watchpoint());
}

void RegUpdate(){
  for (int i = 0; i < 32; ++i){
    cpu.gpr[i] = TOPREG[i];
  }
  cpu.pc = TOPPC;
}

void ftrace_jal(Decode *s, word_t rd){
  #ifdef CONFIG_FTRACE_COND
    void ftrace_call(vaddr_t pc, vaddr_t dnpc);
    void ftrace_ret(vaddr_t pc);
    if (rd == 1)
      ftrace_call(s->pc, s->dnpc);
  #endif
}

void ftrace_jalr(Decode *s, word_t rd){
  #ifdef CONFIG_FTRACE_COND
    void ftrace_call(vaddr_t pc, vaddr_t dnpc);
    void ftrace_ret(vaddr_t pc);
    if (s->isa.inst.val == 0x00008067)
      ftrace_ret(s->pc);
    else if (rd == 1)
      ftrace_call(s->pc, s->dnpc);
  #endif
}

int last_pc = 0;

static void exec_once(Decode *s, vaddr_t pc) {
  s->pc = pc, s->snpc = pc;
  bool first_skip = false; 
  if (TOPPC == 0x30000000 || TOPPC == 0x1ffffffc) first_skip = true;
  else first_skip = false;

  //q s->isa.inst.val = top->io_inst;
#ifdef PRINT_LOG
  Log("%x %08x", s->pc, s->isa.inst.val);
#endif
#ifdef CONFIG_IRINGBUF
  void iringbuf_update(word_t pc, uint32_t inst);
  iringbuf_update(s->pc, s->isa.inst.val);
#endif
  // s->dnpc = top->io_br_target;
  // if (top->io_inst_code == isJAL) ftrace_jal(s, top->io_rd);
  // if (top->io_inst_code == isJALR) ftrace_jalr(s, top->io_rd);

  single_cycle();

#ifdef CONFIG_DIFFTEST
  RegUpdate();
  if (last_pc != TOPPC && !first_skip) {
    last_pc = TOPPC;
    s->pc   = TOPPC;
    trace_and_difftest(s, TOPPC);
  }
#endif
  if (halt) NPCTRAP(TOPPC, 0);

#ifdef CONFIG_ITRACE
  char *p = s->logbuf;
  p += snprintf(p, sizeof(s->logbuf), FMT_WORD ":", s->pc);
  int ilen = s->snpc - s->pc;
  int i;
  uint8_t *inst = (uint8_t *)&s->isa.inst.val;
  for (i = ilen - 1; i >= 0; i --) {
    p += snprintf(p, 4, " %02x", inst[i]);
  }
  int ilen_max = MUXDEF(CONFIG_ISA_x86, 8, 4);
  int space_len = ilen_max - ilen;
  if (space_len < 0) space_len = 0;
  space_len = space_len * 3 + 1;
  memset(p, ' ', space_len);
  p += space_len;

#ifndef CONFIG_ISA_loongarch32r
  extern void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
  disassemble(p, s->logbuf + sizeof(s->logbuf) - p,
      MUXDEF(CONFIG_ISA_x86, s->snpc, s->pc), (uint8_t *)&s->isa.inst.val, ilen);
#else
  p[0] = '\0'; // the upstream llvm does not support loongarch32r
#endif
#endif
}

static void execute(uint64_t n) {
  Decode s;
  for (;n > 0; n --) {
    exec_once(&s, TOPPC);
    
    if (halt) return;
    g_nr_guest_inst ++;
    
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

  //just for itrace test
  #ifdef CONFIG_IRINGBUF
   iringbuf_display(); 
  #endif

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

    tfp -> close();
}
