#include <am.h>
#include <klib-macros.h>
#include <riscv/riscv.h>
#define CONFIG_RTT 1

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
extern char _sdram_start;
int main(const char *args);

// extern char _pmem_start;
#define SDRAM_SIZEH 0x8000000
#define SDRAM_END  ((uintptr_t)&_sdram_start + SDRAM_SIZEH)

# define npc_trap(code) asm volatile("mv a0, %0; ebreak" : :"r"(code))

Area heap = RANGE(&_heap_start, SDRAM_END);
#ifndef MAINARGS
#define MAINARGS ""
#endif
static const char mainargs[] = MAINARGS;


// void bootloader() __attribute__((section(".loader")));
// void bootloader(){
//     extern char _data_cpy, _data, _edata, _bss_start, _bss_end;
//     // extern char _text_cpy, _mytext, _emytext;
//     char *src; char *dst;
// #ifdef CONFIG_RTT
//     extern char _cpy_data_extra, __fsymtab_start, __am_apps_data_end, _cpy_bss_extra, __am_apps_bss_start, __am_apps_bss_end;
//     src = &_cpy_data_extra;
//     dst = &__fsymtab_start;
//     while (dst < &__am_apps_data_end) *dst++ = *src++;
//     src = &_cpy_bss_extra;
//     dst = &__am_apps_bss_start;
//     while (dst < &__am_apps_bss_end) *dst++ = 0;
// #endif
//     src = &_data_cpy;
//     dst = &_data;
//     while (dst < &_edata) *dst++ = *src++;
//     // src = &_text_cpy;
//     // dst = &_mytext;
//     // while (dst < &_emytext) *dst++ = *src++;
//     dst = &_bss_start;
//     while (dst < &_bss_end) *dst++ = 0;
// }

void _bootloader1() __attribute__((section(".loader")));
void _bootloader1(){
    extern uint8_t _loader2_cpy, _ld2_start, _ld2_end;
    uint64_t *src = (uint64_t*)&_loader2_cpy;
    uint64_t *dst = (uint64_t*)&_ld2_start;
    while ((uint8_t*)dst < &_ld2_end) *dst++ = *src++;
}

void _bootloader2() __attribute__((section(".loader2")));
void _bootloader2(){
    extern uint8_t _data_cpy, _data, _edata; 
    extern uint8_t _text_cpy, _mytext, _emytext;
    extern uint8_t _bss_start, _bss_end;
    uint8_t *dta_src; uint8_t *dta_dst; 
    uint64_t *txt_src; uint64_t *txt_dst; 
    uint8_t *bss_dst;

#ifdef CONFIG_RTT
    extern uint8_t _cpy_data_extra, __fsymtab_start, __am_apps_data_end;
    extern uint8_t __am_apps_bss_start, __am_apps_bss_end;

    dta_src = (uint8_t*)&_cpy_data_extra;
    dta_dst = (uint8_t*)&__fsymtab_start;
    while ((uint8_t*)dta_dst < &__am_apps_data_end) *dta_dst++ = *dta_src++;

    bss_dst = &__am_apps_bss_start;
    while (bss_dst < &__am_apps_bss_end) *bss_dst++ = 0;
#endif
    txt_src = (uint64_t*)&_text_cpy;
    txt_dst = (uint64_t*)&_mytext;
    while ((uint8_t*)txt_dst < &_emytext) *txt_dst++ = *txt_src++;

    dta_src = (uint8_t*)&_data_cpy;
    dta_dst = (uint8_t*)&_data;
    while ((uint8_t*)dta_dst < &_edata) *dta_dst++ = *dta_src++;

    bss_dst = &_bss_start;
    while (bss_dst < &_bss_end) *bss_dst++ = 0;
}

void uart_init(uint16_t rate){
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

void _trm_init() __attribute__((section(".loader")));
void _trm_init() {
  // bootloader();
  _bootloader1();
  _bootloader2();
  uart_init(60);
  int ret = main(mainargs);
  halt(ret);
}