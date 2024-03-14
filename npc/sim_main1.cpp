#include "verilated.h"
#include "verilated_vcd_c.h"
#include "obj_dir/VNJU1.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SIM_TIME 200

VerilatedContext* contextp = NULL;
VerilatedVcdC* tfp = NULL;

static VNJU1* top;

void step_and_dump_wave(){
  top->eval();
  contextp->timeInc(1);
  tfp->dump(contextp->time());
}
void sim_init(){
  contextp = new VerilatedContext;
  tfp = new VerilatedVcdC;
  top = new VNJU1;
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
        int a = rand() % 4;        
        int b = rand() % 4;        
        int c = rand() % 4;        
        int d = rand() % 4;        
        int s = rand() % 4;
        top->io_x0 = a; 
        top->io_x1 = b; 
        top->io_x2 = c; 
        top->io_x3 = d; 
        top->io_s = s; 
        step_and_dump_wave();       
        printf("%d\n", top->io_y);
    }
    sim_exit();
}
