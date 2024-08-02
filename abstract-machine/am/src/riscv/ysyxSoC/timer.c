#include <am.h>
#include <riscv/riscv.h>

void __am_timer_init() {
}

#define RTC_ADDR 0x02000000

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uint32_t l = inl(RTC_ADDR);
  uint32_t h = inl(RTC_ADDR + 4);
  uint64_t tmp = ((((uint64_t)h) << 32) + (uint64_t)l);
  uptime->us = (uint64_t)((tmp << 1) + (tmp >> 2));
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
