
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t paddr_t;

#define PG_ALIGN __attribute((aligned(4096)))
#define INSTRUCTION_MSIZE 32
#define INSTRUCTION_MBASE 0X80000000

/* request dynamic instruction memroy */
void init_mem();

/* read instruction memroy */
uint32_t pmem_read(uint32_t addr);

/* write instruction memroy */
void pmem_write(uint32_t addr, uint32_t data);

void init_isa();

uint32_t *guest_to_host(uint32_t paddr);

uint8_t host_to_guest(uint8_t *haddr);