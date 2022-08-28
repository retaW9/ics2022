// *********************************************************************************
// Project Name : ysyx_RV64M NPC
// Author       : ysyx_22040575 jinshuiw
// Website      :
// Create Time  : 2022-06-09
// File Name    : sim_main.cpp
// Module Name  : 
// Called By    :
// Abstract     : Verilator simulation file
//
//
// *********************************************************************************
// Modification History:
// Date         By              Version                 Change Description
// -----------------------------------------------------------------------
// 2022-05-25    Macro           1.0                     Original
//
// *********************************************************************************

#include "verilated.h"
#include "verilated_vcd_c.h"
#include "instruction_men.h"
#include "Vysyx_22040575_RVNPC.h"
#include "svdpi.h"
#include "Vysyx_22040575_RVNPC__Dpi.h"


VerilatedContext *contextp = NULL;
VerilatedVcdC *tfp = NULL;

static Vysyx_22040575_RVNPC *top;
vluint64_t main_time = 0;
vluint64_t sim_time = 900;
int sim_stop = 0;

double sc_time_stamp()
{                     // Called by $time in Verilog
    return main_time; // converts to double, to match
}

void step_and_dump_wave()
{
    contextp->timeInc(1);
    top->eval();
    tfp->dump(contextp->time());
}
void sim_init()
{
    init_mem();
    init_isa();
    printf("load img to memory successful !\n");
    contextp = new VerilatedContext;
    /* create a VerilatedVcdC boject */
    tfp = new VerilatedVcdC;
    top = new Vysyx_22040575_RVNPC;
    contextp->traceEverOn(true);
    top->trace(tfp, 1);
    tfp->open("dump.vcd");
}

void sim_exit()
{
    top->final();
    tfp->close();
}

int main()
{
    sim_init();
    top->reset = 1;
    top->clk = 0;
    while (contextp->time() < sim_time && !contextp->gotFinish())
    {
        if (main_time > 100)
        {
            top->reset = 0;
            step_and_dump_wave();
        }
        if (main_time > 100 && ((main_time % 10) == 1))
        {
            top->clk = 1;
            // step_and_dump_wave();
            printf("read instruction  !\n");
            top->instruction_i = pmem_read(0x80000000 + top->pc);
            step_and_dump_wave();
        }
        if (main_time > 100 && ((main_time % 10) == 6))
        {
            top->clk = 0;
            step_and_dump_wave();
        }
        if(sim_stop == 1) break;
        // step_and_dump_wave();
        printf("time = %ldps\n", main_time);
        main_time++; // Time passes...
    }
    sim_exit();
}

void npc_trap(int ebreak) { 
	printf("Vertification has Finished !\n");
	printf("ebreak =%d\n",ebreak);
	printf("sim_stop =%d\n",sim_stop);
	if(ebreak == 1) {
		sim_stop = 1;	
	    printf("sim_stop =%d\n",sim_stop);
    }
};