#include <common.h>
#include <isa.h>
#include <vcd.h>
#include <nvboard.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
void sdb_mainloop();
int is_exit_status_bad();

VerilatedContext *contextp = NULL;
VerilatedVcdC *tfp = NULL;
VysyxSoCFull* top = new VysyxSoCFull;

void nvboard_bind_all_pins(VysyxSoCFull* top);

void step_and_dump_wave(){
  top->eval();
#ifdef NVBOARD
  nvboard_update();
#endif
  
  // contextp->timeInc(1);
  // tfp->dump(contextp->time());
}

static void sim_init(){
    // contextp = new VerilatedContext;
    // tfp = new VerilatedVcdC;
    top = new VysyxSoCFull;
    // contextp -> traceEverOn(true);
	  // top -> trace(tfp, 0);
    // tfp -> open("dump.vcd");
}

int main(int argc, char *argv[]) {
	Verilated::commandArgs(argc, argv);
  sim_init();
#ifdef NVBOARD
  nvboard_bind_all_pins(top);
  nvboard_init();
#endif
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
//   printf("argc: %d\n", argc);
// printf("argv: %s\n", argv[0]);
  init_monitor(argc, argv);
#endif


  /* Start engine. */
  engine_start();

  return is_exit_status_bad();
}

void engine_start() {
#ifdef CONFIG_TARGET_AM
  cpu_exec(-1);
#else
  /* Receive commands from user. */
  sdb_mainloop();
#endif
}