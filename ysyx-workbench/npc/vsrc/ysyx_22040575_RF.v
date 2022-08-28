`timescale 1ns / 1ps

// *********************************************************************************
// Project Name : ysyx_RV64M NPC
// Author       : ysyx_22040575 jinshuiw
// Website      : 
// Create Time  : 2022-06-09
// File Name    : ysyx_22040575_RF.v
// Module Name  : ysyx_22040575_RF
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


module ysyx_22040575_RF #(ADDR_WIDTH = 5,
                          DATA_WIDTH = 32)
                         (input wire clk,
                          input wire reset,

                          /*write destination regisiter*/
                          input wire rd_wen,                    //rd write enable 
                          input wire [ADDR_WIDTH-1:0] rd_addr,  //which reg to write 
                          input wire [DATA_WIDTH-1:0] rd_data,  //alu result

                          /*source regisiter 1*/
                          input wire rs1_ren,
                          input wire [ADDR_WIDTH-1:0] rs1_addr,
                          output reg [DATA_WIDTH-1:0] rs1_data,
                          
                          /*source regisiter 2*/
                          input wire rs2_ren,                       //rs2 read enable                            
                          input wire [ADDR_WIDTH-1:0] rs2_addr,     //which reg to read                     
                          output reg [DATA_WIDTH-1:0] rs2_data);    
    
    
    reg [DATA_WIDTH-1:0] regisiter_file [31:0];
    
    //assign  rs1_data = regisiter_file[rs1_addr];
    //assign  rs2_data = regisiter_file[rs2_addr];

    /*read socurce regisiter1 control*/  
    always @(*) begin
        if(rs1_addr == 5'b0)
            rs1_data = 32'b0;
        else if(rs1_ren == 1'b1)
            rs1_data =  regisiter_file[rs1_addr];
        else
            rs1_data = 32'b0;                  
    end

    /*read socurce regisiter2 control*/  
    always @(*) begin
        if(rs2_addr == 5'b0)
            rs2_data = 32'b0;
        else if(rs2_ren == 1'b1)
            rs2_data =  regisiter_file[rs2_addr];
        else
            rs2_data = 32'b0;                  
    end  

     /*write destination regisiter control*/  
    always @(posedge clk) begin
        if(reset == 1'b0) begin
            if (rd_wen&&(rd_addr != 5'b0))
                regisiter_file[rd_addr] <= rd_data;
        end
    end
               
endmodule
        
