#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
#define ZEROPAD 1  /* pad with zero */
#define SIGN 2     /* unsigned/signed long */
#define PLUS 4     /* show plus */
#define SPACE 8    /* space if plus */
#define LEFT 16    /* left justified */
#define SPECIAL 32 /* 0x */
#define SMALL 64   /* use 'abcdef' instead of 'ABCDEF' */

int printf(const char *fmt, ...)
{
  char buffer[1024] = {0};
  int i;
  va_list aptr;
  va_start(aptr, fmt);
  
  int ret = vsprintf(buffer, fmt, aptr);
  for(i = 0;i <= ret;i ++){
     putch(*(buffer+i));
  }
  va_end(aptr);
  return ret;
}
// we use this so that we can do without the ctype library
#define is_digit(c) ((c) >= '0' && (c) <= '9')

static int skip_atoi(const char **s) // 将连续的字符串转成整数
{
  int i = 0;

  while (is_digit(**s))           // 判断是否为数字
    i = i * 10 + *((*s)++) - '0'; // 转数字
  return i;
}


static char *number(char *str, unsigned long long num, int size, int precision, int type)
{
  const char *digits="0123456789abcdefghijklmnopqrstuvwxyz";
  char c, sign, tmp[66];
  //char temp;
  int i;
  const int base = 10;
  //check filedwidth  like %02d
  c = (type & ZEROPAD) ? '0' : ' ';    //如果标志符有0则补0，否则补空格；例如%02d
  sign = 0;                         
  if ((signed long long)num < 0) 
    {
			sign = '-';
			num = - (signed long long)num;        //取正值
			size--;                               //字段宽度减1
		}
  
  i = 0;
	if (num == 0)                                    //如果参数为0，则记录字符0
		tmp[i++]='0';                                   //tmp中的内容会放到缓冲区中
	else while(num != 0)                               //循环,num /= base
   {
      tmp[i++] = digits[num % base];               // 0~9ASCII 48~57；0+48=48，ASCII码对应字符'0'
      num /= base;
	 }
   
   //precision analysis like %2.4f                                       
  if (i > precision)
    precision = i;
  size -= precision;
  if (!(type & (ZEROPAD)))                //没有'-'和补0,直接补空格
    while (size-- > 0)
      *str++ = ' ';
  
  if (sign) {                             //如果有符号，输出符号，符号包括：'-','+','',0
    *str++ = sign;
    }

 
    while (size-- > 0)
      *str++ = c;                       // c为0或空格
    while (i < precision--)             // i为转换后存在tmp中字符的个数
      *str++ = '0';
    while (i-- > 0)
      *str++ = tmp[i];                 // tmp中存储着转换了的参数
  
  return str;


}

int vsprintf(char *out, const char *fmt, va_list ap)
{
  int temp = 0;

  // save format
  char *buf;
  char *s;
  int flags = 0;            // flags to number()

  int field_width = -1;     // width of output field
  int precision = -1 ;      // min. # of digits for integers; max
                            // number of chars for from string

  // traversing the string fmt
  for (buf = out; *(fmt) != '\0'; ++fmt)
  {
    if (*fmt != '%')
    {
      *buf++ = *fmt;  
      continue;
    }

    flags = 0;
    repeat:
    ++fmt; 
   //跳过第一个 '%'
    switch (*fmt)                               //判断%后面的字符，对格式运算符的标志的处理
    {
      case '-':  flags |= LEFT;    goto repeat; // flags=10000(二进制，下面一样)
      case '+':  flags |= PLUS;    goto repeat; // flags=100
      case ' ':  flags |= SPACE;   goto repeat; // flags=1000
      case '#':  flags |= SPECIAL; goto repeat; // flags=10 0000
      case '0':  flags |= ZEROPAD; goto repeat; // flags=1
    }

    // get field width
    field_width = -1;
    if (is_digit(*fmt)) // 判断数字字符
      {
       field_width = skip_atoi(&fmt);
      }
    else if (*fmt == '*')
    {
      // it's the next argument
      field_width = va_arg(ap, int); //获取参数值
      if (field_width < 0)
      {
        field_width = -field_width;
        flags |= LEFT; //左对齐
      }
    }

    // get the precision
    precision = -1;
    if (*fmt == '.')
    {
      ++fmt;
      if (is_digit(*fmt)) //判断数字字符
        precision = skip_atoi(&fmt);
      else if (*fmt == '*')
      {
        //it's the next argument 
        precision = va_arg(ap, int);
      }
      if (precision < 0)
        precision = 0;
    }
    //  decimal type
    switch (*fmt)
    {
      //  TODO():float type
      case 'f': temp = va_arg(ap, int); *(buf++) = temp;
      break;

    //  string type
      case 's': s = va_arg(ap, char *);
                //putch(*buf);
                while (*s != '\0')
                 {
                   *(buf++) = *(s++);
                 }
                continue;
      // integrate type
      case 'd': temp = va_arg(ap, unsigned int);
                buf = number(buf,temp, field_width, precision, flags);
                continue;
		}   
}
   *buf = '\0';
   return buf - out;       
}


int sprintf(char *out, const char *fmt, ...)
{

  va_list args;
  int ret;
  //  Initialize va_list get the first parameter's address(args).
  va_start(args, fmt);
  ret = vsprintf(out, fmt, args);
  // Set args to 0.
  va_end(args);
  return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...)
{
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap)
{
  panic("Not implemented");
}

#endif
