/*
 *           __        _
 *  ________/ /  ___ _(_)__  ___
 * / __/ __/ _ \/ _ `/ / _ \/ -_)
 * \__/\__/_//_/\_,_/_/_//_/\__/
 * 
 * Copyright (C) Clément Chaine
 * This file is part of rv32i <https://github.com/cchaine/rv32i>
 *
 * rv32i is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * rv32i is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rv32i.  If not, see <http://www.gnu.org/licenses/>.
 */

package riscv_pkg; 

parameter XLEN = 32;

// opcode field values
parameter  OP_LUI       =  5'b01101;
parameter  OP_AUIPC     =  5'b00101;
parameter  OP_JAL       =  5'b11011;
parameter  OP_JALR      =  5'b11001;
parameter  OP_BRANCH    =  5'b11000;
parameter  OP_LOAD      =  5'b00000;
parameter  OP_STORE     =  5'b01000;
parameter  OP_ALU_IMM   =  5'b00100;
parameter  OP_ALU       =  5'b01100;
parameter  OP_MISC_MEM  =  5'b00011;
parameter  OP_SYSTEM    =  5'b11100;

// branch funct3 field
parameter  F3_BRANCH_BEQ        =  3'b000;
parameter  F3_BRANCH_BNE        =  3'b001;
parameter  F3_BRANCH_BLT        =  3'b100;
parameter  F3_BRANCH_BGE        =  3'b101;
parameter  F3_BRANCH_BLTU       =  3'b110;
parameter  F3_BRANCH_BGEU       =  3'b111;
// load funct3 field
parameter  F3_LOAD_LB           =  3'b000;
parameter  F3_LOAD_LH           =  3'b001;
parameter  F3_LOAD_LW           =  3'b010;
parameter  F3_LOAD_LBU          =  3'b100;
parameter  F3_LOAD_LHU          =  3'b101;
// store funct3 field
parameter  F3_STORE_SB          =  3'b000;
parameter  F3_STORE_SH          =  3'b001;
parameter  F3_STORE_SW          =  3'b010;
// alu_imm funct3 field
parameter  F3_ALU_IMM_ADDI      =  3'b000;
parameter  F3_ALU_IMM_SLTI      =  3'b010;
parameter  F3_ALU_IMM_SLTIU     =  3'b011;
parameter  F3_ALU_IMM_XORI      =  3'b100;
parameter  F3_ALU_IMM_ORI       =  3'b110;
parameter  F3_ALU_IMM_ANDI      =  3'b111;
parameter  F3_ALU_IMM_SLLI      =  3'b001;
parameter  F3_ALU_IMM_SR        =  3'b101;
// alu funct3 field
parameter  F3_ALU_ADD           =  3'b000;
parameter  F3_ALU_SLL           =  3'b001;
parameter  F3_ALU_SLT           =  3'b010;
parameter  F3_ALU_SLTU          =  3'b011;
parameter  F3_ALU_XOR           =  3'b100;
parameter  F3_ALU_SR            =  3'b101;
parameter  F3_ALU_OR            =  3'b110;
parameter  F3_ALU_AND           =  3'b111;
// misc_mem funct3 field
parameter  F3_MISC_MEM_FENCE    =  3'b000;
parameter  F3_MISC_MEM_FENCE_I  =  3'b001;
// system funct3 field
parameter  F3_SYSTEM_CSRRW      =  3'b001;
parameter  F3_SYSTEM_CSRRS      =  3'b010;
parameter  F3_SYSTEM_CSRRC      =  3'b011;
parameter  F3_SYSTEM_CSRRWI     =  3'b101;
parameter  F3_SYSTEM_CSRRSI     =  3'b110;
parameter  F3_SYSTEM_CSRRCI     =  3'b111;

parameter  NOP_INSTRUCTION = 32'h13;

typedef struct packed {
  logic[4:0]   opcode;
  logic[2:0]   f3;
  logic[4:0]   rd;
  logic[4:0]   rs1;
  logic[4:0]   rs2;
  logic[6:0]   f7;
  logic[31:0]  imm;
  logic        is_imm;
} instruction_t;

endpackage // riscv_pkg
