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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

//PA1.2
int pos = 0;

//PA1.2
static void gen_num(){
  int len = (rand() % 4) + 1;
  buf[pos++] = (char)('1' + rand() % 8);
  for (int i = 1; i < len; ++i){
    buf[pos++] = (char)('0' + rand() % 9);
  }
}

//PA1.2
static int gen_rand_op(){
  int op = rand() % 4;
  switch (op){
    case 0: buf[pos++] = '+'; break;
    case 1: buf[pos++] = '-'; break;
    case 2: buf[pos++] = '*'; break;
    case 3: buf[pos++] = '/'; return 1;
    //case 4: buf[pos++] = '%'; break;
    default: assert(0); break;
  }
  return 0;
}

static void gen_rand_expr() {
//PA1.2
//  buf[0] = '\0';
  int is_div;
  switch (rand() % 3) {
  case 0: gen_num(); break;
  case 1: buf[pos++] = '('; gen_rand_expr(); buf[pos++] = ')'; break;
  default:  gen_rand_expr(); 
            is_div = gen_rand_op(); 
            if (is_div) 
              buf[pos++] = '1', 
              buf[pos++] = '+';
            gen_rand_expr(); 
            break;
  }
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    pos = 0;
    gen_rand_expr();
    buf[pos] = '\0';
    if (pos > 60) continue;
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    ret = fscanf(fp, "%d", &result);
    pclose(fp);
//PA1.2
    if (result < 0 || pos > 30) continue;
    printf("%u %s\n", result, buf);
  }
  return 0;
}
