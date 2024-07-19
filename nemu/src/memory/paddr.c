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

#include <memory/host.h>
#include <memory/paddr.h>
#include <device/mmio.h>
#include <isa.h>

#if   defined(CONFIG_PMEM_MALLOC)
static uint8_t *pmem = NULL;
#else // CONFIG_PMEM_GARRAY
// static uint8_t pmem[CONFIG_MSIZE] PG_ALIGN = {};
//note: CONFIG_MSIZE = 0x8000000 = 128MB
#endif

// static uint8_t *mrom = NULL;
static uint8_t sram[SRAM_SIZE];
static uint8_t uart[UART_SIZE];
static uint8_t flash[FLASH_SIZE];
static uint8_t sdram[SDRAM_SIZE];

static inline bool in_mrom(paddr_t addr) { return addr - MROM_BASE < MROM_SIZE; }
static inline bool in_sram(paddr_t addr) { return addr - SRAM_BASE < SRAM_SIZE; }
static inline bool in_uart(paddr_t addr) { return addr - UART_BASE < UART_SIZE; }
static inline bool in_flash(paddr_t addr) { return (addr - FLASH_BASE < FLASH_SIZE); }
static inline bool in_sdram(paddr_t addr) { return addr - SDRAM_BASE < SDRAM_SIZE; }

static void out_of_bound(paddr_t addr) {
  panic("address = " FMT_PADDR " is out of bound of pmem [" FMT_PADDR ", " FMT_PADDR "] at pc = " FMT_WORD,
      addr, PMEM_LEFT, PMEM_RIGHT, cpu.pc);
}

uint8_t* guest_to_host(paddr_t paddr) { 
    if(in_flash(paddr)){
      return flash + paddr - FLASH_BASE;
      }
    else if(in_sram(paddr)){
      // printf("addr = 0x%08x\n", paddr);
      return sram + paddr - SRAM_BASE;
    }
    else if(in_uart(paddr)){
      return uart + paddr - UART_BASE;
    }
    else if(in_sdram(paddr)){
      return sdram + paddr - SDRAM_BASE;
    }
    else {
      printf("gth");
      out_of_bound(paddr);
    }
    return 0;//pmem + paddr - CONFIG_MBASE; 
}
// paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }

static word_t pmem_read(paddr_t addr, int len) {
  word_t ret = host_read(guest_to_host(addr), len);
  return ret;
}

static void pmem_write(paddr_t addr, int len, word_t data) {
  if (in_flash(addr)) assert(0);
  host_write(guest_to_host(addr), len, data);
}

// void init_mem() {
// #if   defined(CONFIG_PMEM_MALLOC)
//   pmem = malloc(CONFIG_MSIZE);
//   assert(pmem);
// #endif
//   IFDEF(CONFIG_MEM_RANDOM, memset(pmem, rand(), CONFIG_MSIZE));
//   Log("physical memory area [" FMT_PADDR ", " FMT_PADDR "]", PMEM_LEFT, PMEM_RIGHT);
// }

// void init_mrom() {
//   mrom = malloc(0xfff);
//   assert(mrom);
//   memset(mrom, 0, MROM_SIZE);
//   Log("mrom area [" FMT_PADDR ", " FMT_PADDR "]", MROM_BASE, MROM_BASE + MROM_SIZE);
// }


void init_flash() {
  memset(flash, 0, sizeof(flash));
  Log("flash area [" FMT_PADDR ", " FMT_PADDR "]", FLASH_BASE, MROM_BASE + FLASH_SIZE);
}

void display_flash(){
  for (int i = 0; i < 900; i+=4){
    printf("%08x : %02x %02x %02x %02x\n", 0x30000000 + i, flash[i+3], flash[i+2], flash[i+1], flash[i]);
  }
}

void init_sdram() {
  memset(sdram, 0, sizeof(sdram));
  Log("sdram area [" FMT_PADDR ", " FMT_PADDR "]", SDRAM_BASE, SDRAM_BASE + SDRAM_SIZE);
}

void init_sram() {
  memset(sram, 0, sizeof(sram));
  Log("sram area [" FMT_PADDR ", " FMT_PADDR "]", SRAM_BASE, SRAM_BASE + SRAM_SIZE);
}

void init_uart() {
  memset(uart, 0, sizeof(uart));
  Log("uart area [" FMT_PADDR ", " FMT_PADDR "]", UART_BASE, UART_BASE + UART_SIZE);
}

bool nemu_skip = false;

word_t paddr_read(paddr_t addr, int len) {
  #ifdef CONFIG_MTRACE_COND
    if (MTRACE_COND){
      void mtrace_read(paddr_t addr, int len);
      mtrace_read(addr, len);
    }
  #endif
  if (likely(in_sram(addr) || in_sdram(addr) || in_flash(addr))) return pmem_read(addr, len);
  if (in_uart(addr)) { nemu_skip = true; return 0;}
  IFDEF(CONFIG_DEVICE, return mmio_read(addr, len));
      printf("pr");
  out_of_bound(addr);
  return 0;
}

void paddr_write(paddr_t addr, int len, word_t data) {
  #ifdef CONFIG_MTRACE_COND
    if (MTRACE_COND){
      void mtrace_write(paddr_t addr, int len, word_t data);
      mtrace_write(addr, len, data);
    }
  #endif
  if (likely(in_sram(addr) || in_sdram(addr) || in_flash(addr))) { pmem_write(addr, len, data); return; }
  if (in_uart(addr)) return;
  IFDEF(CONFIG_DEVICE, mmio_write(addr, len, data); return);
      printf("pw");
  out_of_bound(addr);
}
