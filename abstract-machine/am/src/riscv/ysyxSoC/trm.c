#include <am.h>
#include <klib-macros.h>
#include <riscv/riscv.h>
// #include <ysyx.h>

#define UART_BASE 0x10000000L
#define UART_TX   0x0

#define UART_REG_RB  0x10000000
#define UART_REG_LC  0x10000003
#define UART_REG_DL1 0x10000000
#define UART_REG_DL2 0x10000001
#define UART_REG_LS  0x10000005

#define UART_LS_DR	0	// Data ready
#define UART_LS_OE	1	// Overrun Error
#define UART_LS_PE	2	// Parity Error
#define UART_LS_FE	3	// Framing Error
#define UART_LS_BI	4	// Break interrupt
#define UART_LS_TFE	5	// Transmit FIFO is empty
#define UART_LS_TE	6	// Transmitter Empty indicator
#define UART_LS_EI	7	// Error indicator

extern char _heap_start;
int main(const char *args);

extern char _pmem_start;
#define PMEM_SIZE (128 * 1024 * 1024)
#define PMEM_END  ((uintptr_t)&_pmem_start + PMEM_SIZE)

# define npc_trap(code) asm volatile("mv a0, %0; ebreak" : :"r"(code))

Area heap = RANGE(&_heap_start, PMEM_END);
#ifndef MAINARGS
#define MAINARGS ""
#endif
static const char mainargs[] = MAINARGS;

void bootloader(){
    extern char _erodata, _data, _edata, _bss_start, _bss_end;//, _stack_top;
    char *src = &_erodata;
    char *dst = &_data;
    while (dst < &_edata) *dst++ = *src++;
    char *bst = &_bss_start;
    while (bst < &_bss_end) {
      *bst++ = 0;
    }
}

void uart_init(int16_t rate){
  outb(UART_REG_LC, 0b10000011);
  outb(UART_REG_DL2, (uint8_t)(rate >> 8));
  outb(UART_REG_DL1, (uint8_t)rate);
  outb(UART_REG_LC, 0b00000011);
}
void putch(char ch) {
  uint8_t get_LSR,  get_TFE;
  do {
    get_LSR = inb(UART_REG_LS);
    get_TFE = (get_LSR >> UART_LS_TFE) & 1;
  } while (!get_TFE);
  outb(UART_REG_RB, ch);
}

void halt(int code) {
  npc_trap(code);
  while (1);
}

void _trm_init() {
  bootloader();
  uart_init(120);
  int ret = main(mainargs);
  halt(ret);
}