#include "verilated.h"
#include "verilated_vcd_c.h"
#include "obj_dir/VTOP.h"

VerilatedContext* contextp = NULL;
VerilatedVcdC* tfp = NULL;

static VTOP* top;

void step_and_dump_wave(){
  top->eval();
  contextp->timeInc(1);
  tfp->dump(contextp->time());
}
void sim_init(){
  contextp = new VerilatedContext;
  tfp = new VerilatedVcdC;
  top = new VTOP;
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

  sim_exit();
}
