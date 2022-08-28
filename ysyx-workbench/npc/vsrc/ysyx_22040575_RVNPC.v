`timescale 1ns / 1ps
// *********************************************************************************
// Project Name : ysyx_RV64M NPC
// Author       : ysyx_22040575 jinshuiw
// Website      :
// Create Time  : 2022-06-09
// File Name    : ysyx_22040575_RVNPC.v
// Module Name  : ysyx_22040575_RVNPC
// Called By    :
// Abstract     :
//
//
// *********************************************************************************
// Modification History:
// Date         By              Version                 Change Description
// -----------------------------------------------------------------------
// 2022-05-25    Macro           1.0                     Original
//
// *********************************************************************************

module ysyx_22040575_RVNPC(input wire clk,
                           input wire reset,
                           input wire [31:0] instruction_i,
                           output reg [31:0] pc,
                           output ebreak,
                           output wire [31:0] alu_result);
    
    //wire brachtaken;
    //wire [31:0]  alu_result;
    wire [31:0]  rs1_data;
    wire [31:0]  rs2_data;
    //wire [31:0]  imm;
    wire [31:0] instr_out;
    
    wire [4:0] rd_addr,rs1_addr,rs2_addr;
    wire rd_wen;
    //wire men_wen,mem_ren;
    wire rs1_ren,rs2_ren;
    
    //export "DPI-C" function ;
    
    
    
    ysyx_22040575_IFU#(
    .DATA_WIDTH(32)
    )
    ifu_inst
    (
    .clk(clk),
    .reset(reset),
    .ebreak(ebreak),
    .instruction_i(instruction_i),
    .pc(pc),
    .instruction_o(instr_out)
    );
    
    ysyx_22040575_RF#(
    .DATA_WIDTH(32),
    .ADDR_WIDTH(5)
    )
    rf_inst
    (
    .clk(clk),
    .reset(reset),
    /* write destination regisiter */
    .rd_wen(rd_wen),
    .rd_addr(rd_addr),
    .rd_data(alu_result),
    /* source regisiter 1 */
    .rs1_ren(rs1_ren),
    .rs1_addr(rs1_addr),
    .rs1_data(rs1_data),
    /* source regisiter 2 */
    .rs2_ren(rs2_ren),
    .rs2_addr(rs2_addr),
    .rs2_data(rs2_data)
    );
    
    ysyx_22040575_EXU#(
    .DATA_WIDTH(32),
    .ADDR_WIDTH(5)
    )
    exu_inst
    (
    //.clk(clk),
    .reset(reset),
    /* instruction from Instruction Fetch Unit */
    .pc(pc),
    .instruction_w(instr_out),
    .ebreak(ebreak),
    /* instruction from Instruction Decode Unit */
    .operator_rs1(rs1_data),
    .operator_rs2(rs2_data),
    /* regisiter address to RF unit */
    .rd_addr_w(rd_addr),
    .rs1_addr_w(rs1_addr),
    .rs2_addr_w(rs2_addr),
    .rd_wen(rd_wen),
    .rs1_ren(rs1_ren),
    .rs2_ren(rs2_ren),
    .alu_result(alu_result)
    //.imm_switch_w(imm)
    );
    
endmodule
