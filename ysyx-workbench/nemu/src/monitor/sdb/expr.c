#include <isa.h>
#include <memory/vaddr.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, 
  TK_EQ,
  TK_UNEQ,
  TK_DEREF,          //259
  TK_NEG,            //260
  TK_DEC,
  TK_HEX,
  TK_REGNAME,
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},                // spaces
  {"\\+", '+'},                     // plus
  {"==", TK_EQ},                    // equal
  {"!=", TK_UNEQ},                  // unequal
  {"&&", '&'},                      // and
  {"\\|\\|", '|'},                  // or
  {"!", '!'},                       // not
  {"\\*", '*'},                     // multiple或者指针解引用
  {"\\-", '-'},                     // minus
  {"\\/", '/'},                     // devide
  {"\\)", ')'},                     // right parenthesis
  {"\\(", '('},                     // left parenthesis

  
  {"0[xX][0-9,a-z,A-Z]+",TK_HEX},    // hex number 先对十六进制数据进行匹配。 
  {"^[0-9]+", TK_DEC},               // dec number
  {"\\$[0-9,a-z]+",TK_REGNAME},      // regisiter name
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
  //char str[32];
  char * str;
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

        switch (rules[i].token_type) {
          case  TK_NOTYPE:  tokens[nr_token].type = TK_NOTYPE; //nr_token ++;丢弃空格？
                            break;
          case    '+'    :  tokens[nr_token].type = '+'; nr_token ++;
                            break;               
          case  TK_EQ    :  tokens[nr_token].type = TK_EQ; nr_token ++;
                            break;
          case    '-'    :  tokens[nr_token].type = '-'; nr_token ++;
                            break;
          case    '*'    :  tokens[nr_token].type = '*'; nr_token ++;
                            break;
          case    '/'    :  tokens[nr_token].type = '/'; nr_token ++;
                            break;
          case    '('    :  tokens[nr_token].type = '('; nr_token ++;
                            break;
          case    ')'    :  tokens[nr_token].type = ')'; nr_token ++;
                            break; 

          case TK_REGNAME : tokens[nr_token].type = TK_REGNAME; tokens[nr_token].str = substr_start;nr_token ++;
                            break;
          case  TK_DEC   :  tokens[nr_token].type = TK_DEC;     tokens[nr_token].str = substr_start; nr_token ++;
                            break;
          case  TK_HEX   :  tokens[nr_token].type = TK_HEX;     tokens[nr_token].str = substr_start; nr_token ++;
                            break;                                                                                                                 
          default: printf("There is no matched token!! Please try again!!");
                            break;
        }
       printf("inr_token = %d ,str = %s\n",nr_token,tokens[nr_token-1].str);
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

/*
 *   功能： 检测一个表达式expresion是否被一对括号包围
 */

bool check_parentheses(int p,int q) {
  uint8_t cnt = 0;
  uint8_t left_pos = 0;
  uint8_t right_pos = 0;
  for (int i = p+1; i < q; i++)
  {
    if(tokens[i].type =='(') {
      cnt ++;
      /* 记录最后一次左边括号的位置 */
      left_pos = i; 
      printf("left=%d\n",left_pos);                    
      }
    else if(tokens[i].type == ')') {
      cnt --;
      /* 记录最后一次右边边括号的位置 */
      right_pos = i;  
      printf("right=%d\n",right_pos);                   
    }   
  }
  //printf("tokens[p].type=%d\n",tokens[p].type);
  //printf("tokens[q-1].type=%d\n",tokens[q-1].type);
  if(cnt == 0&&tokens[p].type =='('&&tokens[q].type ==')'\
    &&(left_pos<=right_pos)) return true;
  else return false;
}

/*
*   功能： 寻找主运算符的位置
*/
typedef struct operate {
  int type;
  //char str[32];
  int pos;
} OP;

int find_dominate_op(int p,int q) {

 OP operate[32]  = {};
 int nr_op  = 0;

  /* 将表达式中的 括号()跟算术运算符‘+ - * /’ 单目运算符‘- *’ 提取出来 */
  for(int i = p; i <= q; i ++) {
    switch (tokens[i].type) {
      case    '('    :  operate[nr_op].type = '('; operate[nr_op].pos = i; nr_op ++; 
                        break;
      case    '+'    :  operate[nr_op].type = '+'; operate[nr_op].pos = i; nr_op ++;
                        break;               
      case    '*'    :  operate[nr_op].type = '*'; operate[nr_op].pos = i; nr_op ++;
                        break;
      case    '-'    :  operate[nr_op].type = '-'; operate[nr_op].pos = i; nr_op ++; 
                        break;
      case    '/'    :  operate[nr_op].type = '/'; operate[nr_op].pos = i; nr_op ++; 
                        break;   
      case    ')'    :  operate[nr_op].type = ')'; operate[nr_op].pos = i; nr_op ++; 
                        break;                                               
      case TK_DEREF  :  operate[nr_op].type = TK_DEREF; operate[nr_op].pos = i; nr_op ++; 
                        break;
      case TK_NEG    :  operate[nr_op].type = TK_NEG; operate[nr_op].pos = i; nr_op ++; 
                        break;                        

      default        :  break;
    }
    //printf("type0 = %d ,pos = %d,i = %d\n",operate[nr_op-1].type,operate[nr_op-1].pos,i);
  }
  for(int8_t i = nr_op -1 ;i >= 0 ; ){
    //printf("type1 = %d ,pos = %d,i = %d\n",operate[i].type,operate[i].pos,i);
    if(operate[i].type == ')'&&(operate[i-2].type == '('||operate[i-3].type == '(')){
      i = i - 3;
      continue;
    }
    else if(operate[i].type == '+'||operate[i].type == '-'){
      return operate[i].pos;
      i --;
    }
    else if((operate[i].type == '*'||operate[i].type == '/')&&((i == 0)||(operate[i-1].type != '+'&&operate[i-1].type != '-'\
          &&operate[i-1].type != '*'&&operate[i-1].type != '/'))){
      return operate[i].pos;
      i --;
    }
    else if(operate[i].type == TK_DEREF&&(operate[i-1].type != '+'&&operate[i-1].type != '-'\
          &&operate[i-1].type != '*'&&operate[i-1].type != '/') ){
      return operate[i].pos;
      i --;      
    }
    else if(operate[i].type == TK_NEG&&(operate[i-1].type != '+'&&operate[i-1].type != '-'\
          &&operate[i-1].type != '*'&&operate[i-1].type != '/') ){
      return operate[i].pos;
      i --;      
    }

    else i -- ;
  }
   return 0;
}

word_t eval(int p, int q) {

  word_t val1 = 0, val2 = 0;
  int op = 0;
  word_t single_token = 0;
  word_t reg_val = 0;

  if (p > q) {
    assert(0);
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    if(tokens[p].type == TK_DEC){
      sscanf(tokens[p].str, "%ld", &single_token);
      printf("single_token = %ld\n",single_token);
      return single_token ;
    }
    else if(tokens[p].type == TK_HEX){

      return 0 ;
    }
    else if(tokens[p].type == TK_REGNAME){ 

      reg_val = isa_reg_str2val(tokens[p].str, (_Bool *)true);
      printf("reg_val = %ld\n",reg_val);
      return reg_val;
    }
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1);
  }
  else {  
    op = find_dominate_op(p,q);
    printf("op_type = %d ,op = %d\n",tokens[op].type ,op);
    if(tokens[op].type == TK_DEREF||tokens[op].type == TK_NEG){
        val1 = 0;
        val2 = eval(op + 1, q);           
    }
    else{
        val1 = eval(p, op - 1);
        val2 = eval(op + 1, q);
    }

    printf("val1 = %ld,val2 = %ld\n",val1,val2);

    switch (tokens[op].type) {
      case '+':       return val1 + val2;         break;
      case '-':       return val1 - val2;         break;
      case '*':       return val1 * val2;         break;
      case '/':       return val1 / val2;         break;
      case TK_DEREF : return vaddr_read(val2, 4); break;
      case TK_NEG   : return val1 - val2;         break;
      default: assert(0);                         break;
    }
  }
  return 0;
}


word_t expr(char *e, bool *success) {
  word_t expr_val = 0;
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* 判断 '*' 是解指针运算还是乘法运算 */
  for (int8_t i = 0; i < nr_token; i ++) {
  if(tokens[i].type == '*' &&(i == 0||tokens[i-1].type == '-'||tokens[i-1].type == '/'\
   ||tokens[i-1].type == '+'||tokens[i-1].type == '*'||tokens[i-1].type == '(')) {
      tokens[i].type = TK_DEREF;
    }
  /* 判断 '-' 是负数运算还是减法运算 */
  if(tokens[i].type == '-' &&(i == 0||tokens[i-1].type == '-'||tokens[i-1].type == '/'\
   ||tokens[i-1].type == '+'||tokens[i-1].type == '*'||tokens[i-1].type == '(')) {
      tokens[i].type = TK_NEG;
    }
  }

  expr_val = eval(0,nr_token-1);
  printf("expr_val = %ld\n",expr_val);
  return expr_val;
}
