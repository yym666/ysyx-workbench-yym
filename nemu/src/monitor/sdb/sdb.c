/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
//PA1.1
#include <memory/paddr.h>

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();
//PA1.3
void wp_display();
void set_watchpoint(char* args);
void del_watchpoint(char* s);
void watchpoint();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
//PA1.0
  set_nemu_state(4, 0, 0);
  return -1;
}

//PA1.1
static int chartonum(char *args){
  int num = 0, i = 0;
  while (args[i] != '\0') 
    num = num * 10 + (args[i]-'0'), i++;
  return num;
}

//PA1.1
static int my_expr(char *args){
  printf("%s\n\n", args);
  if (args[0] != '0' || args[1] != 'x'){
   printf("Number fault!\n");
   return -1;
  }
  int len = strlen(args) - 2;
  int ret = 0;
  for (int i = 2; i < len+2; ++i){
    ret = ret * 16 + (args[i] - '0');
  }
  return ret;
}

//PA1.1
static int cmd_si(char *args) {
  int step_num = (args == NULL) ? 1 : chartonum(args);
  cpu_exec(step_num);
  return 0;
}

//PA1.1
static int cmd_info(char *args){
  if (args == NULL){
    printf("Command info need args: \n    r (for register)\n    w (for watchpoint)\n");
    return 0;
  }
  if (args[0] == 'r'){
    isa_reg_display();
  }
  else if (args[0] == 'w'){
    wp_display();
  }
  return 0;
}

//PA1.1
static int cmd_x(char *addr_ch){
  char *args = strtok(addr_ch, " ");
  addr_ch = args + strlen(args) + 1;
  printf("%s\n\n", args);
  if (args == NULL || addr_ch == NULL){
    printf("Command x need two args!\n");
    return 0;
  }
  //jif (addr == NULL)
  int len = chartonum(args);
  int addr = my_expr(addr_ch);

  for (int i = 0; i < len; ++i){
    printf("0x%08x\n", paddr_read(addr, 4));
    addr += 4;
  }
  return 0;
}

//PA1.3
static int cmd_w(char *args){
  watchpoint();
  if (args == NULL){
    printf("Command w need one args!\n");
    return 0;
  }
  set_watchpoint(args);
  return 0;
}

//PA1.3
static int cmd_d(char *args){
  if (args == NULL){
    printf("Command d need one args!\n");
    return 0;
  }
  if (args[0] == '$'){
   // args = args + 1;
  }
  printf("%s\n", args);
  del_watchpoint(args);
  return 0;
}

static int cmd_help(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display information about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
//PA1.1
  { "si", "Single step execute", cmd_si},
  { "info", "Infomation print", cmd_info},
  { "x", "Scan the memory", cmd_x},
//PA1.3
  { "w", "Set watchpoint", cmd_w},
  { "d", "Delete watchpoint", cmd_d},
  /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
