#include "sdb.h"

#define NR_WP 32



static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }
  /* 头指针 用于组织使用中监视点结构 */
  head = NULL;  
  /* 头指针 用于组织空闲的监视点结构 */
  free_ = wp_pool;
}

/* 功能 ： free_申请一个空闲的监视点结构 记录某个表达式 */
WP* new_wp(char * exprs){
  /* 从空闲的监视点池中选取第一个节点使用 */
  if(free_ == NULL) {
    printf("The quantity of wp reached the upper limit .\n");
    assert(0);  
  }
  WP *temp = free_;
  free_ = free_->next;
  temp->next = NULL;

  strcpy(temp->expr,exprs);
  temp->expr_val = expr(exprs ,(_Bool *)true); 

  /* 将上述插入使用中的监视点 */
  if(head == NULL)
    head = temp;
  else{
    WP *p =  head;
    while(p -> next != NULL){
      p = p -> next;
    }
      p -> next = temp;
  } 
  return temp;
}

/*  功能 ： 释放一个监视点 将其归还到free连表中  */
void free_wp(WP *wp){
	if(wp == NULL) printf("There is no watchpoint. \n");
	if(wp == head) head = head -> next;
	else{
	   	WP *p = head;
	   	while(p->next != wp) p = p->next;
	   	//if(p == NULL) return 0;
	   	p -> next = wp -> next;
	}
  /* 将头指针指向 指向连表头部 wp首插*/
	wp -> next = free_;  
	free_ = wp;           
}

/*  功能 ： 测试监视点的值是否发生变化  */
int test_diff(){
  word_t val  = 0 ;
  WP *p = head ;
  if(p == NULL) printf("There is no watchpoint.\n");
  else{
    while(p->next != NULL){
      val = expr(p->expr,(_Bool *)true); 
      if(val == p->expr_val) return 0;
      else return 1;
      p = p->next;
    }
  }  
  return 0;
}

/*  功能 ： 打印所有监视点的信息  */
void print_wp_info(){
  WP *p = head ;
  while(p->next != NULL){
    printf("The no. of wacth point is %d.\n\
The expr of wacth point is %s.\nThe value of expr is %ld.\n",p->NO,p->expr,p->expr_val); 
    p = p->next;
  }  
}

/*  功能 ： 删除一个监视  */
void delete_wp(int no){
  WP *p = head ;
  if(no == 0) head = p->next;
  else{
  while(p->next != NULL){
    if(p->next->NO == no) {
        free_wp(p->next);
        p = p->next->next;
        printf("Delete no.%d watch point successfully.\n",no);
      }
    else  p = p->next;     
    }
  }   
}