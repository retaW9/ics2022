`timescale 1ns / 1ps

// *********************************************************************************
// Project Name : ysyx_RV64M NPC
// Author       : ysyx_22040575 jinshuiw
// Website      : 
// Create Time  : 2022-06-09
// File Name    : ysyx_22040575_EXU.v
// Module Name  : ysyx_22040575_EXU
// Called By    :
// Abstract     :
//
// 
// *********************************************************************************
// Modification History:
// Date         By              Version             Change Description
// -----------------------------------------------------------------------
// 2022-06-09    jinshuiw        1.0              add auipc/lui/jal/jalr/ instruction
//  
// *********************************************************************************


module ysyx_22040575_EXU#(ADDR_WIDTH = 5,
                          DATA_WIDTH = 64)
                         (//input wire clk,
                          input wire reset,
                          /* instruction from Instruction Fetch Unit */
                          input wire [DATA_WIDTH-1:0] pc,
                          input wire [31:0] instruction_w,
                          output ebreak,

                          /* instruction from Instruction Decode Unit */
                          input wire [DATA_WIDTH-1:0] operator_rs1,
                          input wire [DATA_WIDTH-1:0] operator_rs2,

                          /* regisiter address to RF unit */
                          output wire [ADDR_WIDTH-1:0] rd_addr_w,
                          output reg  [ADDR_WIDTH-1:0] rd_data_w,
                          output wire [ADDR_WIDTH-1:0] rs1_addr_w,
                          output reg  [ADDR_WIDTH-1:0] rs1_data_w,
                          output wire [ADDR_WIDTH-1:0] rs2_addr_w,
                          output reg  [ADDR_WIDTH-1:0] rs2_data_w,
                          output wire rd_wen,
                          output wire rs1_ren,
                          output wire rs2_ren,
                          output reg [DATA_WIDTH-1:0] alu_result  //write back to rd 

                          /* regisiter address to data memory unit */
                          //output wire branchtaken,
                          //output wire mem_ren,
                          //output wire mem_wen,
                                                   
                          //output wire [DATA_WIDTH-1:0] imm_switch_w
                          //output reg [DATA_WIDTH-1:0] pc_jump_r
                          );




//=================================================================================
// Parameter declaration
//=================================================================================
localparam      ALU_ADD     =   4'd0,
                ALU_SUB     =   4'd1,
                ALU_AND     =   4'd2,
                ALU_OR      =   4'd3,
                ALU_XOR     =   4'd4,
                //ALU_SLT     =   4'd5,
                //ALU_SLTU    =   4'd6,
                ALU_SHL     =   4'd7,
                ALU_SHR     =   4'd8,
                //ALU_MULL    =   4'd9,
                //ALU_MULH    =   4'd10,
                //ALU_DIV     =   4'd11,
                //ALU_REM     =   4'd12,
                ALU_NPC     =   4'd13;
                //ALU_AUIPC   =   4'd14;




//=================================================================================
// Signal declaration
//=================================================================================                   
wire    [3:0]   alu_operation;
wire    [63:0]  imm_switch_w;

wire    [6:0]   op_w;        //opcode
wire    [4:0]   rd_w;        //rd
wire    [2:0]   f3_w;        //funct3
wire    [4:0]   rs1_w;       //rs1
wire    [4:0]   rs2_w;       //rs2
wire    [6:0]   f7_w;        //funct7

wire    [63:0]  imm_i_w;     //I-type immediate
wire    [63:0]  imm_s_w;     //S-type immediate
wire    [63:0]  imm_b_w;     //B-type immediate
wire    [63:0]  imm_u_w;     //U-type immediate
wire    [63:0]  imm_j_w;     //J-type immediate

/* Integer Regisiter-Immediate Instruaction */
wire lui_w,auipc_w; 

/* load & store Instruction */
wire lui_w,lw_w,sw_w;

/* Integer Regisiter-Regisiter Operation */


/* jump & branch Instruction */
wire jal_w,beq_w,jalr_w;//bgeu_w,//bltu;

/* add & sub Instruction */
wire add_w,sub_w,addi_w;

/* bit operate Instruction */
wire and_w,or_w,xor_w,srl_w,sll_w;
     //andi_w,
     //ori_w,
     //xori_w,

/* systemn instruction */         
wire ebreak_w;
    
//=================================================================================
// Logic Design
//=================================================================================    
/* RISC-V Instruction Format Decode from 6 types */
assign op_w[6:0]  = instruction_w[6:0];    //opcode
assign rd_w[4:0]  = instruction_w[11:7];   //rd
assign f3_w[2:0]  = instruction_w[14:12];  //funct3
assign rs1_w[4:0] = instruction_w[19:15];  //rs1
assign rs2_w[4:0] = instruction_w[24:20];  //rs2
assign f7_w[6:0]  = instruction_w[31:25];  //funct7

/* extend RISC-V immediate to 64bit */
assign imm_i_w[63:0] = { {52{instruction_w[31]}}, instruction_w[31:20] };
assign imm_s_w[63:0] = { {52{instruction_w[31]}}, instruction_w[31:25], instruction_w[11:7] };
assign imm_b_w[63:0] = { {51{instruction_w[31]}}, instruction_w[31], instruction_w[7], instruction_w[30:25], instruction_w[11:8], 1'b0 };
assign imm_u_w[63:0] = { {32{instruction_w[31]}}, {instruction_w[31:12], 12'h0} };
assign imm_j_w[63:0] = { {43{instruction_w[31]}}, instruction_w[31], instruction_w[19:12], instruction_w[20], instruction_w[30:21], 1'b0 };



/* Integer Regisiter-Immediate Instruaction */
assign auipc_w  =   (7'b0010111 == op_w);
assign lui_w    =   (7'b0110111 == op_w);  

/* load & store Instruction */
assign lw_w     = (7'b0000011 == op_w) && (3'b010 == f3_w); //load word
assign sw_w     = (7'b0100011 == op_w) && (3'b010 == f3_w); //store word



/* jump & branch Instruction */
assign jal_w    =   (7'b1101111 == op_w);                     //jump and link
assign jalr_w   =   (7'b1100111 == op_w) && (3'b010 == f3_w); //jump and link register    
assign beq_w    =   (7'b1100011 == op_w) && (3'b000 == f3_w); //branch if equal
//assign bgeu_w =   (7'b1100011 == op_w) && (3'b111 == f3_w); //branch if greater than or equal, unsigned
//assign bltu_w =   (7'b1100011 == op_w) && (3'b110 == f3_w); //branch if less than, unsigned



/* add & sub Instruction */
assign add_w  = (7'b0110011 == op_w) && (3'b000 == f3_w) && (7'b0000000 == f7_w); //add
assign sub_w  = (7'b0110011 == op_w) && (3'b000 == f3_w) && (7'b0100000 == f7_w); //sub
assign addi_w = (7'b0010011 == op_w) && (3'b000 == f3_w);                         //add immediate number



/* bit operate Instruction */    
assign and_w = (7'b0110011 == op_w) && (3'b111 == f3_w) && (7'b0000000 == f7_w);
assign or_w  = (7'b0110011 == op_w) && (3'b110 == f3_w) && (7'b0000000 == f7_w);
assign xor_w = (7'b0110011 == op_w) && (3'b100 == f3_w) && (7'b0000000 == f7_w);   
//assign andi_w = (7'b0010011 == op_w) && (3'b111 == f3_w);//and immediate
//assign ori_w  = (7'b0010011 == op_w) && (3'b110 == f3_w);//or  immediate
//assign xori_w = (7'b0010011 == op_w) && (3'b100 == f3_w);//xor immediate    
assign srl_w = (7'b0110011 == op_w) && (3'b101 == f3_w) && (7'b0000000 == f7_w);//shift right logical
assign sll_w = (7'b0110011 == op_w) && (3'b001 == f3_w) && (7'b0000000 == f7_w);//shift left  logical


/* system instruction */         
assign ebreak_w = (7'b1110011 == op_w) && (3'b000 == f3_w) && (7'b0000000 == f7_w)&&
                  (rd_w == 5'd0) && (rs1_w == 5'd0) && (rs2_w == 5'd1) ;


/* switch immediate in a instruction //U B I S J   */
assign imm_switch_w[63:0] = ({64{lui_w||auipc_w}}& imm_u_w) |
                            ({64{beq_w}}&imm_b_w) |
                            ({64{lw_w||addi_w||jalr_w}}& imm_i_w)|
                            ({64{sw_w}} & imm_s_w) |
                            ({64{jal_w}}& imm_j_w) ;

/* find rd/rs1/rs2 regisiter address in a instruction */
assign  rd_addr_w[4:0]  = (lui_w  || auipc_w || lw_w  || jal_w || jalr_w || add_w || 
                           addi_w || sub_w || and_w  || or_w   || xor_w ) ? rd_w : 5'd0;

assign  rs1_addr_w[4:0] = (jalr_w || lw_w   || sw_w  || beq_w ||  add_w ||
                           addi_w || sub_w || and_w  || or_w  || xor_w ) ? rs1_w : 5'd0;

assign  rs2_addr_w[4:0] = (sw_w  || beq_w ||add_w  || sub_w || 
                           and_w  || or_w ||xor_w ||srl_w || sll_w )     ? rs2_w : 5'd0;


assign alu_operation[3:0] = (add_w  || addi_w || lui_w || lw_w  || sw_w) ? ALU_ADD :                          
                            (sll_w)                                      ? ALU_SHL :
                            (srl_w)                                      ? ALU_SHR :
                            (jal_w||jalr_w)                              ? ALU_NPC : ALU_SUB;

always @(*) begin
    if (reset)
        alu_result = 64'h0;
    else
        case(alu_operation[3:0])
            ALU_ADD:
            if (add_w)       alu_result = operator_rs1 + operator_rs2;
            else if (addi_w) alu_result = operator_rs1 + imm_switch_w;
            else if (lui_w)  alu_result = imm_switch_w;
            else if (lw_w)   alu_result = operator_rs1 + imm_switch_w;
            else if (sw_w)   alu_result = operator_rs1 + imm_switch_w;
            else             alu_result = 64'h0;
            ALU_AND:
            if (and_w)       alu_result = operator_rs1 & operator_rs2;
            else             alu_result = 64'h0;
            ALU_OR :
            if (or_w)        alu_result = operator_rs1 | operator_rs2;
            else             alu_result = 64'h0;
            ALU_XOR:
            if (xor_w)       alu_result = operator_rs1 ^ operator_rs2;
            else             alu_result = 64'h0;
            ALU_SHR:         alu_result = operator_rs1 >> operator_rs2;
            ALU_NPC:         if(jal_w) begin 
                             alu_result = pc + imm_switch_w;
                               
                             end
            else if(jalr_w)  alu_result = pc         + imm_switch_w;
            ALU_SUB:
            if (sub_w)       alu_result = operator_rs1 - operator_rs2;
            else             alu_result = 64'h0;
            default:
            if (beq_w)       alu_result = pc + imm_switch_w;
            else             alu_result = 64'h0;
        endcase 
    end

//assign branchtaken = (jal_w|(beq_w& (operator_rs1 == operator_rs2))) ? 1 :0 ;
assign rd_wen   = (add_w|addi_w|lui_w|lw_w|and_w);
assign rs1_ren  =  addi_w;
assign rs2_ren  =  add_w;
assign ebreak   =  ebreak_w ;
//assign mem_wen   = sw_w;
//assign mem_ren   = lw_w;

endmodule
    
