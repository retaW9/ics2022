#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc)
{
  bool no_diff = false;
  if (ref_r->pc == pc)
  {
    for (int i = 0; i < 32; i++)
    {
      // printf("R%d\t0x%016lx\t0x%016lx\t\n\r", i, ref_r->gpr[i], cpu.gpr[i]);
      if (ref_r->gpr[i] == cpu.gpr[i])
      {       
        no_diff = true;
      }
      else printf("R%d\t0x%016lx\t0x%016lx\t\n\r", i, ref_r->gpr[i], cpu.gpr[i]);
    }
  }
  else
    no_diff = false;

  if (no_diff == true)
  {
    return true;
  }
  else
    return false;
}

void isa_difftest_attach()
{
}
