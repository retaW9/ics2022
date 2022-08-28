#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
/**内存控制块数据结构，用于管理所有的内存块
 * is_available: 标志着该块是否可用。1表示可用，0表示不可用
 * size: 该块的大小
 **/
typedef struct mem_control_block
{
  bool is_available;
  int size;
} MEM_BLOCK_CTRL;

static char *sbrk;
static unsigned long int next = 1;
int has_initialized = 0;    // 初始化标志
void *managed_memory_start; // 指向堆底（内存块起始位置）
void *last_valid_address;   // 指向堆顶

int rand(void)
{
  // RAND_MAX assumed to be 32767
  next = next * 1103515245 + 12345;
  return (unsigned int)(next / 65536) % 32768;
}

void srand(unsigned int seed)
{
  next = seed;
}

int abs(int x)
{
  return (x < 0 ? -x : x);
}

int atoi(const char *nptr)
{
  int x = 0;
  while (*nptr == ' ')
  {
    nptr++;
  }
  while (*nptr >= '0' && *nptr <= '9')
  {
    x = x * 10 + *nptr - '0';
    nptr++;
  }
  return x;
}
void malloc_init()
{
  // 这里不向操作系统申请堆空间，只是为了获取堆的起始地址
  last_valid_address = heap.end;
  sbrk = heap.start;
  has_initialized = 1;
}

void *malloc(size_t size)
{
  // On native, malloc() will be called during initializaion of C runtime.
  // Therefore do not call panic() here, else it will yield a dead recursion:
  //   panic() -> putchar() -> (glibc) -> malloc() -> panic()
  //#if !(defined(__ISA_NATIVE__) && defined(__NATIVE_USE_KLIB__))
  // panic("Not implemented");
  //#endif

  void *current_location;                         // 当前访问的内存位置
  struct mem_control_block *current_location_mcb; // 只是作了一个强制类型转换
  void *memory_location;                          // 这是要返回的内存位置。初始时设为0，表示没有找到合适的位置

  if (size == 0)
    return NULL;

  if (!has_initialized)
  {
    malloc_init();
  }

  // current location is heap.start   sbrk point to heap.start
  current_location = sbrk;
  // memory block = ctral block + memsize
  size = size + sizeof(struct mem_control_block);
  sbrk += size;

  current_location_mcb = (struct mem_control_block *)current_location;

  // 初始时设为 0，表示没有找到合适的位置
  memory_location = 0;
  // Begin searching at the start of managed memory
  //  managed_memory_start 是在 malloc_init 中通过 sbrk 函数设置的
  while (current_location != last_valid_address)
  {
    
    // current_location 是一个 void 指针，用来计算地址 ；
    // current_location_mcb 是一个具体的结构体类型
    if (current_location_mcb->is_available)
    {
    
      if (current_location_mcb->size >= size)
      {
        // 找到一个可用、大小适合的内存块
        current_location_mcb->is_available = 0; // 设为不可用
        memory_location = current_location;     // 设置内存地址
        break;
      }
      else
      {
        current_location += current_location_mcb->size; 
      }
    }
    //
    // current_location = current_location + current_location_mcb->size;
  }
  // TODO : 循环结束，没有找到合适的位置，需要向操作系统申请更多内存
  assert(memory_location);

  // memory_location 保存了大小为 numbyte的内存空间，并且在空间的开始处包含了一个内存控制块，记录了元信息
  // 内存控制块对于用户而言应该是透明的，因此返回指针前，跳过内存分配块
  memory_location = memory_location + sizeof(struct mem_control_block);
  return memory_location;
}

void free(void *ptr)
{
}

#endif
