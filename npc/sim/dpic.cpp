#include <memory.h>
#include <common.h>
#include <memory/vaddr.h>
#include <memory/paddr.h>
#include <isa.h>
extern bool is_skip_ref;

#define SERIAL_PORT 0xa00003f8
#define RTC_ADDR    0xa0000048

#define SEXT(x, len) ({ struct { int64_t n : len; } __x = { .n = x }; (uint64_t)__x.n; })

extern "C" void flash_read(int addr, int *data) { 
	int align_addr = addr + FLASH_BASE;
  // printf("Addr=%x\n", addr);
	*data = *(int *)guest_to_host(align_addr);
  // printf("Data=%x\n", *data);
  assert(*data != 0);
  return;
}

extern "C" void mrom_read(int addr, int *data) {
	int align_addr = addr & (~3);
	*data = *(int *)guest_to_host(align_addr);
	return;
}

extern "C" void diff_skip(){
  is_skip_ref = true;
}

extern "C" uint32_t dpmem_read(uint32_t addr){
    if(!((addr >= 0x80000000 && addr <= 0x87ffffff) || 
         (addr == RTC_ADDR) || (addr == RTC_ADDR + 4))) 
		return 0;
	if(addr == RTC_ADDR || addr == RTC_ADDR + 4)
		is_skip_ref = true;
    if (addr == 0xa0000048) {return (word_t)get_time();}
    if (addr == 0xa000004c) {return get_time() << 32;}
    uint32_t *paddr = (uint32_t *)guest_to_host(addr);
    return *paddr;

    assert(0);
}

extern "C" void dpmem_write(int waddr, int wdata, char mask){
  if(!((waddr >= 0x80000000 && waddr <= 0x87ffffff) || (waddr == SERIAL_PORT))) 
		return;
	if(waddr == SERIAL_PORT){
		is_skip_ref = true;
	}
  if(waddr == SERIAL_PORT) {
      printf("%c", wdata);
      return;
  }

  uint8_t *vaddr = guest_to_host(waddr);
  uint8_t *iaddr;
  int i;
  int j;
  for(i = 0,j = 0;i < 4;i++){
    if(mask & (1 << i)){
      iaddr = vaddr + i;
      *iaddr = (wdata >> (j * 8)) & 0xFF;
      j++;
    }
  }
}