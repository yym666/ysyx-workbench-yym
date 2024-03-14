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

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
//PA1.3
  char expr[32];
  word_t las_val;
} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

//PA1.3
WP* new_WP();
void free_wp(WP *wp);

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

//PA1.3
WP* new_WP(){
  if (free_ == NULL){
    assert(0);
    return NULL;
  }
  WP* cur = free_;
  free_ = free_->next;
  cur->next = head;
  head = cur;
  return cur;
}

//PA1.3
void del_watchpoint(char* s){
  WP* cur = head;
  WP* las = NULL;
  while (cur != NULL){
    if (strcmp(cur->expr, s) == 0){
      if (las != NULL){
        las->next = cur->next;
      }
      head = cur->next;
      cur->next = free_;
      free_ = cur;
      printf("Watchpoint %d deleted\n", cur->NO);
      return;
    }
    las = cur;
    cur = cur->next;
  }
}

//PA1.3
void free_wp(WP *wp){
  WP* cur = head;
  
  if (cur == wp){
    head = cur->next;
    cur->next = free_;
    free_ = cur;
    return;
  }
  while (cur->next != NULL){
    if (cur->next == wp){
      cur->next = wp->next;
      wp->next = free_;
      free_ = wp;
      printf("Watchpoint %d deleted\n", cur->NO);
      return;
    }
    cur = cur->next;
  }
  assert(0);
}

//PA1.3
void watchpoint(){
  WP *cur = head;
  while (cur != NULL){
    bool success = true;
    word_t res = expr(cur->expr, &success);
    if (res != cur->las_val){
      printf("Watchpoint %d: %s\n",cur->NO, cur->expr);
      printf("Old_value = %x\nNew_value = %x\n", cur->las_val, res);
      cur->las_val = res;
    }
    cur = cur->next;
  }
}

//PA1.3
void wp_display(){
  WP* cur = head;
  while (cur != NULL){
    printf("Watchpoint %d: %s = %x\n", cur->NO, cur->expr, cur->las_val);
    cur = cur->next;
  }
}

//PA1.3
void set_watchpoint(char* args){
  WP* cur = new_WP();
  strcpy(cur->expr, args);
  cur->las_val = expr(args, NULL);
  printf("Watchpoint %d: %s\n", cur->NO, args);
}