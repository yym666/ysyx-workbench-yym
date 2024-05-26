#include <common.h>
#include <memory/paddr.h>

#define MAX_RW 10;

typedef struct{
    word_t pc;
    paddr_t addr;
}MTRACE_NODE;

void mtrace_read(paddr_t addr, int len){
//    Log(" mtrace_read : " FMT_PADDR ", len=%d.", addr, len);
}

void mtrace_write(paddr_t addr, int len, word_t data){
    if (addr == 0x800dc1b0)
        Log("mtrace_write: " FMT_PADDR ", len=%d, data = %x.", addr, len, data);
}