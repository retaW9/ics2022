#include <isa.h>
#include <memory/paddr.h>
// * len 1,2,4 rv64 8 分别返回uint8 uint16 uint32 rv64 返回uint64 *//
word_t vaddr_ifetch(vaddr_t addr, int len) {
  return paddr_read(addr, len);
}

word_t vaddr_read(vaddr_t addr, int len) {
  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
  paddr_write(addr, len, data);
}
