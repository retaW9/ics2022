#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>

typedef struct watchpoint {

  int NO; 
  /* 记录表达式 */
  char expr[32];
  /* 记录表达式的值 */
  word_t expr_val;

  struct watchpoint *next;
} WP;

word_t expr(char *e, bool *success);
WP* new_wp(char * exprs);
void free_wp(WP *wp);
int test_diff();
void print_wp_info();
void delete_wp(int no);

#endif
