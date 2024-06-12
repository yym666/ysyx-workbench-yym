
#ifndef __VCD_H__
#define __VCD_H__

#include <verilated.h>   //访问验证程序例程的库
#include <verilated_vcd_c.h>  //向VCD文件中写入波形
#include "VysyxSoCFull.h"

extern VerilatedContext *contextp;
extern VerilatedVcdC *tfp;
extern VysyxSoCFull* top;

extern void step_and_dump_wave();

#endif