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

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,
//PA1.2
  TK_NUM,
  TK_HEX,
  TK_VAR,
  TK_REG,
  TK_NEQ,
  TK_POINT,
  TK_AND
  /* TODO: Add more token types */
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
//PA1.2
  {"-", '-'},
  {"\\*", '*'},
  {"/", '/'},
  {"!=", TK_NEQ},
  {"0x[a-fA-F0-9]{8}", TK_HEX},
  {"\\d+", TK_NUM},
  {"[a-zA-Z][\\w]*", TK_VAR},
  {"^\\$(0|[xast]\\d{1,2}|ra|sp|gp|tp)", TK_REG},
  {"\\(", '('},
  {"\\)", ')'},
  {"&&", TK_AND}
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
//PA1.2
        if (substr_len > 32) assert(0);
        
        switch (rules[i].token_type) {
//PA1.2
          case TK_NOTYPE: 
            tokens[nr_token++].type = rules[i].token_type;
            break;
          default: 
            strcpy(tokens[nr_token].str, substr_start);
            tokens[nr_token++].type = rules[i].token_type;
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

//PA1.2
bool check_parentheses(int p, int q){
  if (tokens[p].type != '(' || tokens[q].type != ')')
    return false;
  int count = 0;
  for (int i = p+1; i <= q-1; ++i){
    if (tokens[p].type == '('){
      count++;
    }
    else if (tokens[p].type == ')'){
      count--;
    }
    if (count < 0){
      return false;
    }
  }
  if (count != 0){
    return false;
  }
  return true;
}

//PA1.2
int get_main_opt(int p, int q){
  int qt_cnt = 0, ret = 0, level = 0;
  for (int i = p; i <= q; ++i){
    if (tokens[i].type == '('){
      qt_cnt++;
    }
    else if (tokens[i].type == ')'){
      qt_cnt--;
    }
    if (qt_cnt < 0) assert(0);
    if (qt_cnt > 0) continue;
    switch(tokens[i].type){
      case '+': case '-':
        if (ret == 0 || level > 1) 
          ret = i, level = 1; 
        break;
      case '*': case '/': 
        if (ret == 0 || level > 2) 
          ret = i, level = 2;
        break;
      case TK_EQ: case TK_NEQ:
        if (ret == 0)
          ret = i, level = 3;
        break;
      default: assert(0);
    }
  }
  if (qt_cnt != 0) assert(0);
  return ret;
}

//PA1.2
word_t eval(int p, int q){
  if (p > q){
    assert(0);
    return 0;
  }
  else if (p == q){
    if (tokens[p].type == TK_NUM){
      return (word_t)(strtol(tokens[p].str, NULL, 10));
    }
    else if (tokens[p].type == TK_HEX){
      return (word_t)(strtol(tokens[p].str, NULL, 16));
    }
    else if (tokens[p].type == TK_REG){
      return isa_reg_str2val(tokens[p].str, NULL);
    }
    assert(0);
  }
  else if (check_parentheses(p, q) == 1){
    return eval(p+1, q-1);
  }
  else{
    int opt = get_main_opt(p, q);
    word_t val1 = eval(p, opt-1);
    word_t val2 = eval(opt+1, q);
    switch (tokens[opt].type)
    {
      case '+': return val1 + val2; break;
      case '-': return val1 - val2; break;
      case '*': return val1 * val2; break;
      case '/': 
        if (val2 == 0) assert(0);
        return val1 / val2;
        break;
      case TK_EQ: return val1 == val2; break;
      case TK_NEQ: return val1 != val2; break;
      default: assert(0); break;
    }
  }
  assert(0);
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
//PA1.2
  return eval(0, nr_token-1);
//return 0;
}