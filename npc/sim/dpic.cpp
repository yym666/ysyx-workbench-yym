#include <memory.h>
#include <common.h>
#include <memory/vaddr.h>
extern bool is_skip_ref;

#define SERIAL_PORT 0xa00003f8
#define RTC_ADDR    0xa0000048

#define SEXT(x, len) ({ struct { int64_t n : len; } __x = { .n = x }; (uint64_t)__x.n; })

extern "C" uint32_t dpmem_read(uint32_t addr, char mask){
    // printf("addr = %x\n\n", addr);

    int rmask = (int)mask;
    // printf("addr = %08x\n mask = %08x\n", addr, rmask);
    if(!((addr >= 0x80000000 && addr <= 0x87ffffff) || 
         (addr == RTC_ADDR) || (addr == RTC_ADDR + 4))) 
		return 0;
	if(addr == RTC_ADDR || addr == RTC_ADDR + 4)
		is_skip_ref = true;
    if (addr == 0xa0000048) {return (word_t)get_time();}
    if (addr == 0xa000004c) {return get_time() << 32;}

    bool flg = false;
    if (rmask % 2 == 0) flg = true, rmask--;
    int len = ((rmask == 1) ? 1 : ((rmask == 3) ? 2 : 4));
    if (!flg){
        if (rmask == 1)
            return SEXT(vaddr_read(addr, len), 8);
        else if (rmask == 3)
            return SEXT(vaddr_read(addr, len), 16);
        else if (rmask == 15){
            return SEXT(vaddr_read(addr, len), 32);

        }
    }
    else
        return  vaddr_read(addr, len);
    assert(0);
}

extern "C" void dpmem_write(int waddr, int wdata, char mask){
    int wmask = (int)mask;
    if(!((waddr >= 0x80000000 && waddr <= 0x87ffffff) || (waddr == SERIAL_PORT))) 
		return;
	if(waddr == SERIAL_PORT){
		is_skip_ref = true;
	}
    if(waddr == SERIAL_PORT) {
        printf("%c", wdata);
        // putc((char)wdata, stderr);
        return;
    }

    int len = ((wmask == 1) ? 1 : ((wmask == 3) ? 2 : 4));
    vaddr_write(waddr, len, wdata);
}