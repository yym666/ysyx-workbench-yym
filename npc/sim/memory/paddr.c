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

// #if   defined(CONFIG_PMEM_MALLOC)
// static uint8_t *pmem = NULL;
// #else // CONFIG_PMEM_GARRAY
// // static uint8_t pmem[CONFIG_MSIZE] PG_ALIGN = {};
// //note: CONFIG_MSIZE = 0x8000000 = 128MB
// #endif

static uint8_t *pmem = NULL;
static uint8_t *flash = NULL;

// static inline bool in_pmem(paddr_t addr) { return addr - RESET_VECTOR < 0xfffffff; }
// static inline bool in_mrom(paddr_t addr) { return addr - MROM_BASE < MROM_SIZE; }
// static inline bool in_sram(paddr_t addr) { return addr - SRAM_BASE < SRAM_SIZE; }
static inline bool in_flash(paddr_t addr) { return addr - FLASH_BASE < FLASH_SIZE; }

uint8_t* guest_to_host(paddr_t paddr) {
	if(in_flash(paddr))
		return flash + (paddr - FLASH_BASE);
	else if(in_pmem(paddr))
		return pmem + (paddr - RESET_VECTOR);
	else{
    printf("paddr: %x\n", paddr);
		assert(0);
	}
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

// void init_mem() {
// #if   defined(CONFIG_PMEM_MALLOC)
//   pmem = malloc(CONFIG_MSIZE);
//   assert(pmem);
// #endif
//   IFDEF(CONFIG_MEM_RANDOM, memset(pmem, rand(), CONFIG_MSIZE));
//   Log("physical memory area [" FMT_PADDR ", " FMT_PADDR "]", PMEM_LEFT, PMEM_RIGHT);
// }
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

void init_mem(){ 
  int siz = 0x7fffffff;
	pmem = (uint8_t *)malloc(siz * sizeof(uint8_t));
	memcpy(pmem , img , sizeof(img));
	if(pmem == NULL){exit(0);}
	Log("npc physical mrom area [%#x, %#lx]", RESET_VECTOR, RESET_VECTOR + siz * sizeof(uint8_t));
}

void init_flash() {
	flash = (uint8_t *)malloc(0xfffffff * sizeof(uint8_t));
	memcpy(flash , flash_test , sizeof(flash_test));
	if(flash == NULL) assert(0);
	Log("flash area [%#x, %#x]",FLASH_BASE, FLASH_BASE + FLASH_SIZE);
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
