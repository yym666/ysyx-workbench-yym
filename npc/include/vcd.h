
#ifndef __VCD_H__
#define __VCD_H__

#include <generated/autoconf.h>
#include <verilated.h>   //访问验证程序例程的库
#include <verilated_vcd_c.h>  //向VCD文件中写入波形
#include "VysyxSoCFull.h"

// #ifdef WAVE_ON
extern VerilatedContext *contextp;
extern VerilatedVcdC *tfp;
// #endif
extern VysyxSoCFull* top;

extern void step_and_dump_wave();

#endif