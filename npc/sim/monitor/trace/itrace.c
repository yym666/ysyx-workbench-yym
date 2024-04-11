#include <memory/paddr.h>
#include <common.h>
#include <cpu/cpu.h>
#include <cpu/decode.h>
#include <locale.h>
#include <cpu/ifetch.h>
#include "disasm.h"

typedef struct {
    word_t pc;
    uint32_t inst;
}IRINGBUF_NODE;

#define MAX_IRINGBUF_NODE 10

void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);

IRINGBUF_NODE iringbuf[MAX_IRINGBUF_NODE];
int cur_node_pointer = 0;
bool full_flags = 0;

void iringbuf_update(word_t pc, uint32_t inst){
    iringbuf[cur_node_pointer].pc = pc;
    iringbuf[cur_node_pointer].inst = inst;
    cur_node_pointer++;
    if (cur_node_pointer == MAX_IRINGBUF_NODE)
        full_flags = true, cur_node_pointer = 0;
}

void iringbuf_display(){
    char logbuf[256];
    char *p;
    for (int i = 0; i < MAX_IRINGBUF_NODE; ++i){
        p = logbuf;
        p += snprintf(logbuf, sizeof(logbuf), "%s " FMT_WORD ": %08x ", 
            (i+1)%MAX_IRINGBUF_NODE == cur_node_pointer ? "--> " : "    ", iringbuf[i].pc, iringbuf[i].inst);
        disassemble(p, logbuf+sizeof(logbuf)-p, (uint64_t)iringbuf[i].pc, (uint8_t *)(&iringbuf[i].inst), 4);
        puts(logbuf);
    }
}