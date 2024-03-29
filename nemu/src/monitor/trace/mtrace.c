#include <common.h>
#include <memory/paddr.h>

#define MAX_RW 10;

typedef struct{
    word_t pc;
    paddr_t addr;
}MTRACE_NODE;

void mtrace_read(paddr_t addr, int len, bool read_flag, word_t data){
    Log(" mtrace_read : " FMT_PADDR ", len=%d.", 
        addr, len);
}

void mtrace_write(paddr_t addr, int len, word_t data){
    Log("mtrace_write: " FMT_PADDR ", len=%d, data = %u.", 
        addr, len, data);
}