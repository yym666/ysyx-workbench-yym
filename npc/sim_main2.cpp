#include "verilated.h"
#include "verilated_vcd_c.h"
#include "obj_dir/VNJU2.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SIM_TIME 200

VerilatedContext* contextp = NULL;
VerilatedVcdC* tfp = NULL;

static VNJU2* top;

void step_and_dump_wave(){
  top->eval();
  contextp->timeInc(1);
  tfp->dump(contextp->time());
}
void sim_init(){
  contextp = new VerilatedContext;
  tfp = new VerilatedVcdC;
  top = new VNJU2;
  contextp->traceEverOn(true);
  top->trace(tfp, 0);
  tfp->open("dump.vcd");
}

void sim_exit(){
  step_and_dump_wave();
  tfp->close();
}

int main() {
    sim_init();

    for (int i = 1; i < SIM_TIME; ++i){
        int a = rand() % 8;        
        if (a == 0) top->io_in_0 = 1;
        else top->io_in_0 = 0;
        if (a == 1) top->io_in_1 = 1;
        else top->io_in_1 = 0;
        if (a == 2) top->io_in_2 = 1;
        else top->io_in_2 = 0;
        if (a == 3) top->io_in_3 = 1;
        else top->io_in_3 = 0;
        if (a == 4) top->io_in_4 = 1;
        else top->io_in_4 = 0;
        if (a == 5) top->io_in_5 = 1;
        else top->io_in_5 = 0;
        if (a == 6) top->io_in_6 = 1;
        else top->io_in_6 = 0;
        if (a == 7) top->io_in_7 = 1;
        else top->io_in_7 = 0;
        top->io_en = 1; 
        step_and_dump_wave();       
        printf("%d\n", top->io_out);
    }
    sim_exit();
}
