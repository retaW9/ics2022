#include <cpu/cpu.h>
#include <trace.h>
iRingBuff inst_ringbuf;

void sdb_mainloop();

void engine_start() {
#ifdef CONFIG_ITRACE
initRingbuffer(&inst_ringbuf,INST_RING_BUF_SIZE);
#endif  
#ifdef CONFIG_TARGET_AM
  cpu_exec(-1);
#else
  /* Receive commands from user. */
  sdb_mainloop();
#endif
}
