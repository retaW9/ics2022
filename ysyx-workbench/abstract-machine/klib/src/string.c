#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

/*
 *  The strlen() function calculates the length of the string
 *  pointed to by s, excluding the terminating null byte ('\0').
 */
size_t strlen(const char *s)
{
  int count = 0;
  assert(s != NULL);
  while (*s != '\0') // while (*str)
  {
    count++;
    s++;
  }
  return count;
}

/*
 *   The  strcpy() function copies the string pointed to by src,
 *   including the terminating null byte ('\0'), to the buffer pointed
 *   to  by  dest.
 */
char *strcpy(char *dst, const char *src)
{
  assert(dst != NULL && src != NULL);
  char *ret = dst;
  while ((*dst++ = *src++))
  {
    ;
  }
  return ret;
}

/*
 *  copying takes place as though the bytes in src are first copied
 *  into a tempo‐rary array that does not overlap src or dest, and
 *  the bytes are then copied from the temporary array to dest.
 */
char *strncpy(char *dst, const char *src, size_t n)
{
  assert(dst != NULL);
  assert(src != NULL);
  char *ret = dst;
  while (n && (*dst++ = *src++))
  {
    n--;
  }
  if (n)
  {
    while (--n)
    {
      *dst = '\0';
    }
  }
  return ret;
}

/*
 *  copying takes place as though the bytes in src are first copied
 *  into a tempo‐rary array that does not overlap src or dest, and
 *  the bytes are then copied from the temporary array to dest.
 */
char *strcat(char *dst, const char *src)
{
  assert(dst != NULL && src != NULL);
  char *ret = dst;
  while (*dst)
  {
    dst++;
  }
  while ((*dst++ = *src++))
  {
    ;
  }
  return ret;
}

/*
 *  copying takes place as though the bytes in src are first copied
 *  into a tempo‐rary array that does not overlap src or dest, and
 *  the bytes are then copied from the temporary array to dest.
 */
int strcmp(const char *s1, const char *s2)
{
  assert(s1 && s2);
  while (*s1 == *s2)
  {
    if (*s1 == '\0')
    {
      return 0;
    }
    s1++;
    s2++;
  }
  return *s1 - *s2;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
  assert(s1 != NULL && s2 != NULL && n > 0);

  while ((--n != 0) && *s2 != '\0' && *s1 != '\0' && *s1 == *s2)
  {
    s1++;
    s2++;
  }
  return *s1 - *s2;
}

/*
 *  The  memset()  function  fills  the first n bytes of the memory
 *  area pointed to by s with the constant byte c.
 */
void *memset(void *s, int c, size_t n)
{
  assert(s != NULL);
  char *temp = (char *)s;
  while (n--)
  {
    *temp++ = c;
  }
  return s;
}

/*
 *  copying takes place as though the bytes in src are first copied
 *  into a tempo‐rary array that does not overlap src or dest, and
 *  the bytes are then copied from the temporary array to dest.
 */
void *memmove(void *dst, const void *src, size_t n)
{
  assert(dst != NULL);
  assert(src != NULL);
  void *rest = dst;
  if (dst < src)
  {
    //
    while (n--)
    {
      *(char *)dst = *(char *)src;
      dst = (char *)dst + 1;
      src = (char *)src + 1;
    }
  }
  else
  {
    while (n--)
    {
      *((char *)dst + n) = *((char *)src + n);
    }
  }
  return rest;
}

/*
 *  The memcpy() function copies n bytes from memory area src to
 *  memory area dest. The memory areas must not overlap.
 */
void *memcpy(void *dst, const void *src, size_t n)
{
  void *ret = dst;
  assert(dst != NULL && src != NULL);
  while (n--)
  {
    *(char *)dst = *(char *)src;
    dst = (char *)dst + 1;
    src = (char *)src + 1;
  }
  return ret;
}

/*
 *  The memcmp() function compares the first n bytes
 *  (each interpreted as unsigned char) of the memory areas s1 and s2.
 */
int memcmp(const void *s1, const void *s2, size_t n)
{

  if (!n)
  {
    return 0;
  }
  assert(s1 != NULL && s2 != NULL);
  while (--n && *(unsigned char *)s1 == *(unsigned char *)s2)
  {
    s1 = (unsigned char *)s1 + 1;
    s2 = (unsigned char *)s2 + 1;
  }
  return (*((unsigned char *)s1) - *((unsigned char *)s2));
}

#endif
