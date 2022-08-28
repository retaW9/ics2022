`timescale 1ns / 1ps

// *********************************************************************************
// Project Name : ysyx_RV64M NPC
// Author       : ysyx_22040575 jinshuiw
// Website      : 
// Create Time  : 2022-06-09
// File Name    : ysyx_22040575_IFU.v
// Module Name  : ysyx_22040575_IFU
// Called By    :
// Abstract     : instruction fetch module
//
// 
// *********************************************************************************
// Modification History:
// Date         By              Version                 Change Description
// -----------------------------------------------------------------------
// 2022-06-25    Macro           1.0                     Original
//  
// *********************************************************************************

module ysyx_22040575_IFU#(DATA_WIDTH = 32)
                         (input clk,
                          input reset,
                          input [DATA_WIDTH-1:0] instruction_i,

                          /* signal from EXU to change pc */
                          //input branchtaken,                      //beq
                          //input [DATA_WIDTH-1:0] pc_jump_r,
                          input wire ebreak,
                          /* signals to isntruction decode unit */
                          output reg unsigned [63:0] pc,
                          output [DATA_WIDTH-1:0] instruction_o
                          );


                          
//=================================================================================
// Parameter declaration
//=================================================================================





//=================================================================================
// Signal declaration
//=================================================================================                          
wire    [63:0]   next_pc;



//=================================================================================
// DPI-C Design
//=================================================================================
import "DPI-C" function void npc_trap (input int ebreak);




//=================================================================================
// Logic Design
//=================================================================================
assign  instruction_o = instruction_i;
//assign  instruction_o = pmem_read(32'h8000_0000+pc);
//assign  next_pc = branchtaken ? pc_jump_r : (pc + 4);
assign next_pc = pc + 64'h1;



    
always @(posedge clk ) begin
    if (reset)
        pc <= 64'h0000_0000_8000_0000;        
    else
        pc <= next_pc;
end

always @(posedge clk ) begin
    npc_trap({31'b0,ebreak});
end
    
endmodule
    
