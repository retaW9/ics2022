#include <isa.h>
#include <cpu/cpu.h>
#include <difftest-def.h>
#include <memory/paddr.h>

#define NXPR (32)
typedef struct diff_context_t
{
  uint64_t pc;
  uint64_t gpr[32];
} REF_state;

void diff_get_regs(void *diff_context)
{
  REF_state *ctx = (REF_state *)diff_context;
  for (int i = 0; i < NXPR; i++)
  {
    ctx->gpr[i] = cpu.gpr[i];
    // printf("ctx->gpr[%d]=%lx\n", i, ctx->gpr[i]);
  }

  ctx->pc = cpu.pc;
}

void diff_set_regs(void *diff_context)
{
  REF_state *ctx = (REF_state *)diff_context;
  /*for (int i = 0; i < NXPR; i++)
  {
    //printf("nemu.gpr[%d]=%lx\n", i, cpu.gpr[i]);

    //printf("ctx->gpr[%d]=%lx\n", i, *(ctx->gpr + i));
    cpu.gpr[i] = ctx->gpr[i];
  }*/
  cpu.pc = ctx->pc;
   printf("ref.pc = %lx !!!!\n", cpu.pc);
}

void difftest_memcpy(paddr_t addr, void *buf, size_t n, bool direction)
{
  // printf("memncpy !!!!\n");
  if (direction == DIFFTEST_TO_REF)
  {
    memcpy(guest_to_host(addr), (uint8_t *)buf, n);
    // printf("%s!!%ld\n", (char *)buf, n);
  }
  else
  {
    assert(0);
  }
}
// `direction`为`DIFFTEST_TO_DUT`时, 获取REF的寄存器状态到`dut`;
// `direction`为`DIFFTEST_TO_REF`时, 设置REF的寄存器状态为`dut`;
void difftest_regcpy(void *dut, bool direction)
{

  if (direction == DIFFTEST_TO_REF)
  {
    // printf("DIFFTEST_TO_REF\n");
    diff_set_regs(dut);
  }
  else
  {
    // printf("DIFFTEST_TO_DUT\n");
    diff_get_regs(dut);
  }
}

void difftest_exec(uint64_t n)
{
   printf("exec %ld ,pc = %lx!!!!\n", n, cpu.pc);
  cpu_exec(n);
}

void difftest_raise_intr(word_t NO)
{
  assert(0);
}

void difftest_init(int port)
{
  // printf("%d, diff_init!!\n", port);
  /* Perform ISA dependent initialization. */
  init_mem();
}
