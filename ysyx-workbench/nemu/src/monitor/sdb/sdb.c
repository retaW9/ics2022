#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include <memory/vaddr.h>
#include <stdio.h> 
static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

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

/*  功能：单步骤执行  */

static int cmd_si(char *args) {
  int n = 0;

  char *arg = strtok(NULL, " ");
  if(arg == NULL) n = 1;
  else n = atoi(arg);
  cpu_exec(n);
  return 0;
}

static int cmd_q(char *args) {
  nemu_state.state = NEMU_QUIT;
  return -1;
}

/* 
 *  功能：1 打印寄存器内容  
 *       2 打印监视点信息
 */

static int cmd_info(char *args) {
  char *arg = strtok(NULL, " ");
  
  if(*arg == 'r') {
    isa_reg_display();  
  }
  else if(*arg == 'w') {
    print_wp_info();
  }
  else printf("Unknown command '%s' please input 'info w' or 'info r'\n", arg);

  return 0;
}

/* 
 *  功能：扫描内存  scan the memory
 */

static int cmd_x(char *args) {
  uint16_t step = 0;
  word_t v_saddr = 0;
  
  char *step_arg = strtok(NULL, " ");
  sscanf(step_arg, "%hd", &step);
  //printf("%hd\n", step);
  char *s_addr_arg = strtok(NULL, " ");
  sscanf(s_addr_arg, "%lx", &v_saddr);
   //v_saddr = expr(s_addr_arg,(_Bool *)true);
  //printf("%lx\n", v_saddr);
  printf("address\t\tvalues(hex)\n");
  for (uint8_t i = 0; i < step; i++)
  {
    word_t scan_mem = vaddr_read(v_saddr + i*4, 4);
    printf("0x%lx\t0x%08lx\n",v_saddr + i*4,scan_mem);
  }
  return 0;
}

/* 
 * 功能 ： 表达式求值
 */

static int cmd_p(char *args) {
  printf("%s\n", args); 
  expr(args,(_Bool *)true);
  return 0;
}

/* 
 *  功能 ： 设置一个监视点 
 *  作用 ： 监视表达式的数值的变化情况，并且要求程序暂停执行
 */

static int cmd_w(char *expr) {
  WP *wp = NULL; 
  printf("%s\n", expr); 
  
  wp = new_wp(expr);
  printf("Set new watch point successfully.\nThe no. of wacth point is %d.\n\
The expr of wacth point is %s.\nThe value of expr is %ld.\n",wp->NO,wp->expr,wp->expr_val); 
  return 0;
}

/* 
 * 功能 ： 删除监视点
 */

static int cmd_d(char *args) {
  int no = -1;
  printf("%s\n", args); 
  sscanf(args, "%d", &no);
  delete_wp(no);
  return 0;
}

/* 
 * 功能 ： 提示 nemu指令
 */

static int cmd_help(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si","Single Step",cmd_si},
  { "info","info r:Show the status of regisiter info w:Show the info of watchpoint",cmd_info},
  { "x","x N EXPR, eg :x 10 0x80000000 Scan the memory",cmd_x},
  { "p","Calcuate the value of expression",cmd_p},
  { "w","Set a Watch Point.",cmd_w},
  { "d","Delete a Watch Point",cmd_d}
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
     * which may need further parsingf
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
