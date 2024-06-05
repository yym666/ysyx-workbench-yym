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
#include "VTOP___024root.h" 
#include "VTOP.h"
#include "disasm.h"
#include <vcd.h>
#include "VTOP__Dpi.h"
#include "svdpi.h"

#define SERIAL_PORT 0xa00003f8
#define RTC_ADDR    0xa0000048

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
// void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);

//DPI-C
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
    cpu.gpr[i] = top->rootp->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[i];
  }
  cpu.pc =  top->io_pc;
}

void PrintaLog(){
   if (top->io_inst_req) Log("");
  Log("mstat : %x",    top->io_mstatus);
  Log("data1 : %08x",  top->io_data1);
  Log("data2 : %08x",  top->io_data2);
  Log("id-ins: %08x",  top->io_id_inst);
  Log("ls-rdt: %08x",  top->io_ls_rdata);
  Log("wb-wdt: %08x",  top->io_wb_wdata);
  Log("rs1: %d", top->io_rs1);
  Log("rs2: %d", top->io_rs2);
  Log("rd : %d",  top->io_rd);
  // Log("data1 : %08x",  top->io_data1);
  // Log("data2 : %08x",  top->io_data2);
  // Log("res: %08x", top->io_res);
  Log("mem_valid: %d", top->io_mem_valid);
  Log("idu_mem_opt: %d", top->io_ldu_mem_opt);
  Log("br_taken : %d", top->io_br_taken);
  Log("br_target: %08x", top->io_br_target);
  // Log("inst_code: %d", top->io_inst_code);
  // Log("isStore: %s", top->io_Store ? "true" : "false");
  // Log("isLoad : %s", top->io_Load ? "true" : "false");
  // if (top->io_Store){
  //   Log("wdata: %08x", top->io_data_sram_wdata);
  //   Log("waddr: %08x", top->io_data_sram_addr);
  // }
  // Log("inst_req : %d", top->io_inst_req);
  printf("\n");
}

// void LoadStore(){
//    if (top->io_Store == true) {
//     vaddr_write(top->io_data_sram_addr, top->io_SL_len, top->io_data_sram_wdata);
//   }
//   if (top->io_Load  == true) {
//     assert(top->io_inst_code >= 24);
//     assert(top->io_inst_code <= 29);
//     if (top->io_inst_code <= 26){
//       if (top->io_inst_code == 24)
//         top->io_data_sram_rdata = SEXT(vaddr_read(top->io_data_sram_addr, top->io_SL_len), 8);
//       else if (top->io_inst_code == 25)
//         top->io_data_sram_rdata = SEXT(vaddr_read(top->io_data_sram_addr, top->io_SL_len), 16);
//       else if (top->io_inst_code == 26)
//         top->io_data_sram_rdata = SEXT(vaddr_read(top->io_data_sram_addr, top->io_SL_len), 32);
//     }
//     else if (top->io_inst_code >= 27)
//       top->io_data_sram_rdata = vaddr_read(top->io_data_sram_addr, top->io_SL_len);
//   }
// }

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

int getinst = 0;

static void exec_once(Decode *s, vaddr_t pc) {
  s->pc = pc, s->snpc = pc;
  bool first_skip = false;
  getinst *= 2;
  if (top->io_inst_req == 1) getinst = 1;

  // printf("get = %d\n", getinst);
  if (pc == 0x7ffffffc);//first_skip = true;
  else if (getinst == 2) {
    // getinst = 0, 
    // top->io_inst = vaddr_read(pc, 4);
  }
  if (pc == 0x80000000) first_skip = true;
  else first_skip = false;
  s->isa.inst.val = top->io_inst;
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
#ifdef PRINT_LOG
  PrintaLog();
#endif
  // LoadStore(); 

  single_cycle();
  if (halt) NPCTRAP(top->io_pc, 0);

// if (top->io_inst_req)
//   req_tag = 1;


#ifdef CONFIG_DIFFTEST
  RegUpdate();
  if (getinst == 2 && !first_skip) {
    getinst = 0, 
    trace_and_difftest(s, top->io_pc);
  }
#endif

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
    // exec_once(&s, top->io_pc);
    exec_once(&s, top->rootp->TOP__DOT__IFU__DOT__pc_reg);
    
    if (halt) return;
    g_nr_guest_inst ++;
    //cause strange output
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
}
