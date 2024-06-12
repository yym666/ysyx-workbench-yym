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

#define CONFIG_DEVICE 1

#if   defined(CONFIG_PMEM_MALLOC)
static uint8_t *pmem = NULL;
#else // CONFIG_PMEM_GARRAY
static uint8_t pmem[CONFIG_MSIZE] PG_ALIGN = {};
//note: CONFIG_MSIZE = 0x8000000 = 128MB
#endif

static uint8_t *mrom = NULL;
static uint8_t *sram = NULL;

// static inline bool likely_in_pmem(paddr_t addr) { return addr - CONFIG_MBASE < CONFIG_MSIZE; }
static inline bool in_mrom(paddr_t addr) { return addr - MROM_BASE < MROM_SIZE; }
static inline bool in_sram(paddr_t addr) { return addr - SRAM_BASE < SRAM_SIZE; }

uint8_t* guest_to_host(paddr_t paddr) {
	// if(likely_in_mrom(paddr))
	// 	return mrom + (paddr - MROM_BASE);
	// // else if(likely_in_pmem(paddr))
	// // 	return pmem + (paddr - RESET_VECTOR);
	// else if(likely_in_sram(paddr))
	// 	return sram + (paddr - SRAM_BASE);
	// else{
	// 	panic("%#x is out of bound in npc", paddr);
	// }
    return pmem + paddr - CONFIG_MBASE; 
}
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }

static word_t pmem_read(paddr_t addr, int len) {
  word_t ret = host_read(guest_to_host(addr), len);
  return ret;
}

static void pmem_write(paddr_t addr, int len, word_t data) {
  host_write(guest_to_host(addr), len, data);
}

static void out_of_bound(paddr_t addr) {
  panic("address = " FMT_PADDR " is out of bound of pmem [" FMT_PADDR ", " FMT_PADDR "] at pc = " FMT_WORD,
      addr, PMEM_LEFT, PMEM_RIGHT, cpu.pc);
}

void init_mem() {
#if   defined(CONFIG_PMEM_MALLOC)
  pmem = malloc(CONFIG_MSIZE);
  assert(pmem);
#endif
  IFDEF(CONFIG_MEM_RANDOM, memset(pmem, rand(), CONFIG_MSIZE));
  Log("physical memory area [" FMT_PADDR ", " FMT_PADDR "]", PMEM_LEFT, PMEM_RIGHT);
}

word_t paddr_read(paddr_t addr, int len) {
  #ifdef CONFIG_MTRACE_COND
    void mtrace_read(paddr_t addr, int len);
    mtrace_read(addr, len);
  #endif
  if (likely(in_pmem(addr))) return pmem_read(addr, len);
  IFDEF(CONFIG_DEVICE, return mmio_read(addr, len));
  // return pmem_read(addr, len);
  // printf("read");
  out_of_bound(addr);
  return 0;
}

void paddr_write(paddr_t addr, int len, word_t data) {
  #ifdef CONFIG_MTRACE_COND
    void mtrace_write(paddr_t addr, int len, word_t data);
    mtrace_write(addr, len, data);
  #endif
  if (likely(in_pmem(addr))) { pmem_write(addr, len, data); return; }
  
  IFDEF(CONFIG_DEVICE, mmio_write(addr, len, data); return);
  
  // printf("write");
  // pmem_write(addr, len, data); return;
  out_of_bound(addr);
}
