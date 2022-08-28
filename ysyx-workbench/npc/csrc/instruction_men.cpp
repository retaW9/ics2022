// *********************************************************************************
// Project Name : ysyx_RV64M NPC
// Author       : ysyx_22040575 jinshuiw
// Website      :
// Create Time  : 2022-06-09
// File Name    : instruction_men.cpp instruction_men.h
// Module Name  :
// Called By    :
// Abstract     : C++ simulated isntruction menory
//
//
// *********************************************************************************
// Modification History:
// Date         By              Version                 Change Description
// -----------------------------------------------------------------------
// 2022-05-25    Macro           1.0                     Original
//
// *********************************************************************************

#include "instruction_men.h"
//#include "svdpi.h"
//#include "Vysyx_22040575_RVNPC__Dpi.h"

static const uint32_t img[] = {
    0x00000297, // auipc t0,0
    0x0002b823, // sd  zero,16(t0)
    0x0102b503, // ld  a0,16(t0)
    0x00100073, // ebreak (used as nemu_trap)
    0xdeadbeef, // some data
};

static uint32_t *pmem = NULL;




/* request dynamic instruction memroy */
void init_mem()
{
    printf("request memory !\n");
    // pmem = new unsigned int;
    pmem = (uint32_t *)malloc(INSTRUCTION_MSIZE);
    if (pmem == NULL)
    {
        printf("request failed ! \n");
        assert(0);
    }
}

/* read instruction memroy */
uint32_t pmem_read(uint32_t addr)
{
    // printf("read instruction 22!\n");
    return *guest_to_host(addr);
}

/* write instruction memroy */
/*void pmem_write(uint32_t addr, uint32_t data)
{
    *(uint32_t *)addr = data;
}*/

void init_isa()
{
    /* Load built-in image. */
    memcpy(guest_to_host(INSTRUCTION_MBASE), img, sizeof(img));
    /* Initialize this virtual computer system. */
    // restart();
}

uint32_t *guest_to_host(uint32_t paddr)
{
    printf("paddr = %d\n", paddr);
    return pmem + paddr - INSTRUCTION_MBASE;
}

/*uint32_t host_to_guest(uint8_t *haddr) {
    return haddr - pmem + INSTRUCTION_MBASE;
}*/